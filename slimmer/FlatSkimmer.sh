#!/bin/bash

fresh=$1

source CrombieSlimmingConfig.sh

if [ "$fresh" = "fresh" ]
then
    rm $CrombieSkimDir/*.root 2> /dev/null
fi

crombie skim --cut 'pfmet > 150 && metFilter == 1 && hbbpt > 0 && jetCSV[hbbjtidx[1]] > 0.3' --tree 'events' --copy 'hDTotalMCWeight' --run 'runNumber' --lumi 'lumiNumber' --freq 100000 --numproc $CrombieNLocalProcs --indir $CrombieFullDir --outdir $CrombieSkimDir -x $CrombieDropBranches --map MapFiles.txt

hadd $CrombieSkimDir/MET.root $CrombieSkimDir/MET_*.root
