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

# Don't need to skim these anymore since we do the filtering in the slimmer
#    crombie skim --cut '(recoil > 150 || met > 150) && met_filter == 1' --tree 'events' --copy 'htotal' --run 'runNumber' --lumi 'lumiNumber' --event 'eventNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $CrombieFullDir/links --outdir $SCRATCH

    for f in $CrombieFullDir/links/*
    do
        ln -s $f $SCRATCH/$(basename $f)
    done

else

    crombie skim --cut 'met_filter == 1 && cmva_hbb && abs(cmva_jet1_gen_pdgid) == 5' --tree 'events' --copy 'htotal' --run 'runNumber' --lumi 'lumiNumber' --event 'eventNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $CrombieFullDir/links --outdir $SCRATCH

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

    crombie skim --cut 'n_genB == 1' --tree 'events' --copy 'htotal' --run 'runNumber' --lumi 'lumiNumber' --event 'eventNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $VJDir --outdir $CrombieSkimDir/bDir

    crombie skim --cut 'n_genB > 1' --tree 'events' --copy 'htotal' --run 'runNumber' --lumi 'lumiNumber' --event 'eventNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $VJDir --outdir $CrombieSkimDir/bbDir

    crombie skim --cut 'n_genB == 0' --tree 'events' --copy 'htotal' --run 'runNumber' --lumi 'lumiNumber' --event 'eventNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $VJDir --outdir $CrombieSkimDir/norm

fi
