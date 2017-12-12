#!/bin/bash

fresh=$1

source CrombieSlimmingConfig.sh

if [ "$fresh" = "fresh" ]
then
    rm $CrombieSkimDir/*.root 2> /dev/null
    rm $CrombieSkimDir/*/*.root 2> /dev/null
fi

crombie skim --cut 'pfmet > 150 && metFilter == 1 && hbbpt > 0 && jetCSV[hbbjtidx[1]] > 0.3' --tree 'events' --copy 'hDTotalMCWeight' --run 'runNumber' --lumi 'lumiNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $CrombieFullDir --outdir $CrombieSkimDir -x $CrombieDropBranches --map MapFiles.txt

hadd $CrombieSkimDir/MET.root $CrombieSkimDir/MET_*.root
rm $CrombieSkimDir/MET_*.root

VJDir=$CrombieSkimDir/VJets

test -d $VJDir || mkdir $VJDir
mv $CrombieSkimDir/{W,Z}Jets* $VJDir

test -d $CrombieSkimDir/bDir || mkdir $CrombieSkimDir/bDir
test -d $CrombieSkimDir/bbDir || mkdir $CrombieSkimDir/bbDir

crombie skim --cut 'abs(jetGenFlavor[hbbjtidx[0]]) == 5 && abs(jetGenFlavor[hbbjtidx[1]]) == 5' --tree 'events' --copy 'hDTotalMCWeight' --run 'runNumber' --lumi 'lumiNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $VJDir --outdir $CrombieSkimDir/bDir

crombie skim --cut '(abs(jetGenFlavor[hbbjtidx[0]]) == 5) != (abs(jetGenFlavor[hbbjtidx[1]]) == 5)' --tree 'events' --copy 'hDTotalMCWeight' --run 'runNumber' --lumi 'lumiNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $VJDir --outdir $CrombieSkimDir/bbDir

crombie skim --cut '(abs(jetGenFlavor[hbbjtidx[0]]) != 5) && (abs(jetGenFlavor[hbbjtidx[1]]) != 5)' --tree 'events' --copy 'hDTotalMCWeight' --run 'runNumber' --lumi 'lumiNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $VJDir --outdir $CrombieSkimDir
