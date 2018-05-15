#!/bin/bash

fresh=$1

source CrombieSlimmingConfig.sh

if [ ! -d $CrombieSkimDir ]
then
    mkdir -p $CrombieSkimDir
fi

SCRATCH=/data/t3home000/dabercro/scratch

if [ "$fresh" = "fresh" ]
then
    rm $SCRATCH/*.root 2> /dev/null
    rm $CrombieSkimDir/*.root 2> /dev/null
    rm $CrombieSkimDir/*/*.root 2> /dev/null
fi

# Make softlinks if we need to:
if [ ! -d $CrombieFullDir/links ]
then
    mkdir -p $CrombieFullDir/links
    pushd $CrombieFullDir
    for f in */*.root
    do 
        link=$(printf $f | perl -ne '$_ =~ s/\//_/g; print $_')
        ln -s ../$f links/$link
    done
    popd
fi

NUMDIRS=$(ls $CrombieFullDir | wc -l)  # This will include the "links" directory too, so the number of samples is one less than this
SAMPLES=$(ls $CrombieFullDir/links | perl -ne '/(.*)_\d+.root/ && print "$1\n"' | sort | uniq)

if [ $NUMDIRS -ne 2 ]
then

    crombie skim --cut '(recoil > 150 || pfmet > 150) && met_filter == 1 && (ak8fatjet1 || ca15fatjet1 || (cmva_daughter_min_pt > 25 && cmva_daughter_max_pt > 50 && cmva_hbb_pt > 70)) && (n_lep_presel == 0 || n_jet >= 4)' --tree 'events' --copy 'hSumW' --run 'runNumber' --lumi 'lumiNumber' --event 'eventNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $CrombieFullDir/links --outdir $SCRATCH

elif [ -d $CrombieFullDir/MET ]
then

    crombie skim --cut '(recoil > 150 || pfmet > 150) && met_filter == 1 && (ak8fatjet1 || ca15fatjet1 || (cmva_daughter_min_pt > 25 && cmva_daughter_max_pt > 50 && cmva_hbb_pt > 70)) && (n_lep_presel == 0 || n_jet >= 4)' --tree 'events' --copy 'hSumW' --run 'runNumber' --lumi 'lumiNumber' --event 'eventNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $CrombieFullDir/links --outdir $SCRATCH

else

#    crombie skim --cut 'n_lep_tight > 0 && n_lep_loose == 2 && cmva_jet1_pt > 25 && cmva_jet1_gen && (abs(cmva_jet1_gen_pdgid) == 5)' --tree 'events' --copy 'hSumW' --run 'runNumber' --lumi 'lumiNumber' --event 'eventNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $CrombieFullDir/links --outdir $SCRATCH
    crombie skim --cut 'n_lep_loose == 0 && cmva_jet1_pt > 25 && cmva_jet1_gen && (abs(cmva_jet1_gen_pdgid) == 5)' --tree 'events' --copy 'hSumW' --run 'runNumber' --lumi 'lumiNumber' --event 'eventNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $CrombieFullDir/links --outdir $SCRATCH

fi

for SAMPLE in $SAMPLES
do
    if [ ! -f $CrombieSkimDir/${SAMPLE}.root ]
    then
        hadd $CrombieSkimDir/${SAMPLE}.root $SCRATCH/${SAMPLE}_[0-9]*.root
    fi
done

./applycorrections.py $CrombieSkimDir

if [ $NUMDIRS -ne 2 ]
then

    VJDir=$CrombieSkimDir/VJets

    test -d $VJDir || mkdir $VJDir
    cp $CrombieSkimDir/{W,Z}Jets* $VJDir

    test -d $CrombieSkimDir/bDir || mkdir $CrombieSkimDir/bDir
    test -d $CrombieSkimDir/bbDir || mkdir $CrombieSkimDir/bbDir
    test -d $CrombieSkimDir/norm || mkdir $CrombieSkimDir/norm

    crombie skim --cut 'n_genB == 1' --tree 'events' --copy 'hSumW' --run 'runNumber' --lumi 'lumiNumber' --event 'eventNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $VJDir --outdir $CrombieSkimDir/bDir

    crombie skim --cut 'n_genB > 1' --tree 'events' --copy 'hSumW' --run 'runNumber' --lumi 'lumiNumber' --event 'eventNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $VJDir --outdir $CrombieSkimDir/bbDir

    crombie skim --cut 'n_genB == 0' --tree 'events' --copy 'hSumW' --run 'runNumber' --lumi 'lumiNumber' --event 'eventNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $VJDir --outdir $CrombieSkimDir/norm

    rm $CrombieSkimDir/?J*.root
    rm $VJDir/*.root
    rmdir $VJDir

fi
