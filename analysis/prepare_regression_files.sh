#! /bin/bash

source CrombiePlotterConfig.sh

export CrombieInFilesDir=$CrombieRegFilesDir

echo "Output directory: $CrombieInFilesDir"

mkdir -p $CrombieInFilesDir

cut="$(crombie2cut /home/dabercro/.crombie2/cuts/b3/b435037da855cb regression)"

echo "Cut: $cut"

directory="TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"

mkdir $CrombieInFilesDir/$directory

crombie skim \
    -n "$CrombieNLocalProcs" \
    -i "$CrombieBigFilesDir/$directory" \
    -o "$CrombieInFilesDir/$directory" \
    -c "$cut" \
    --keep regression_branches.txt \
    --copy htotal

hadd $CrombieInFilesDir/$directory.root $CrombieInFilesDir/$directory/*.root

crombie addxs skip

hadd $CrombieInFilesDir/training_$regvers.root $CrombieInFilesDir/*.root
