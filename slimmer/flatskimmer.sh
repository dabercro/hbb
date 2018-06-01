#!/bin/bash

fresh=$1

source CrombieSlimmingConfig.sh

if [ ! -d $CrombieSkimDir ]
then
    mkdir -p $CrombieSkimDir
fi

if [ "$fresh" = "fresh" ]
then
    rm $CrombieSkimDir/*.root 2> /dev/null
    rm $CrombieSkimDir/*/*.root 2> /dev/null
fi

NUMDIRS=$(ls $CrombieFullDir | wc -l)

for SAMPLE in `ls $CrombieFullDir`
do
    if [ ! -f $CrombieSkimDir/${SAMPLE}.root ]
    then
        hadd $CrombieSkimDir/${SAMPLE}.root $CrombieFullDir/${SAMPLE}/*.root
    fi
done

if [ $NUMDIRS -ne 1 ]
then

    VJDir=$CrombieSkimDir/VJets

    test -d $VJDir || mkdir $VJDir
    mv $CrombieSkimDir/Z{1,2}Jets* $VJDir

    test -d $CrombieSkimDir/bDir || mkdir $CrombieSkimDir/bDir
    test -d $CrombieSkimDir/bbDir || mkdir $CrombieSkimDir/bbDir
    test -d $CrombieSkimDir/norm || mkdir $CrombieSkimDir/norm

    crombie skim --cut 'n_genB == 1' --tree 'events' --copy 'hSumW' --run 'runNumber' --lumi 'lumiNumber' --event 'eventNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $VJDir --outdir $CrombieSkimDir/bDir

    crombie skim --cut 'n_genB > 1' --tree 'events' --copy 'hSumW' --run 'runNumber' --lumi 'lumiNumber' --event 'eventNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $VJDir --outdir $CrombieSkimDir/bbDir

    crombie skim --cut 'n_genB == 0' --tree 'events' --copy 'hSumW' --run 'runNumber' --lumi 'lumiNumber' --event 'eventNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $VJDir --outdir $CrombieSkimDir/norm

    rm $VJDir/*.root
    rmdir $VJDir

fi
