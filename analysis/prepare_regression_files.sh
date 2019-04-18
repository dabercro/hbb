#! /bin/bash

export vers=v5
export type=regression

source CrombiePlotterConfig.sh

echo "Output directory: $CrombieInFilesDir"

mkdir -p $CrombieInFilesDir

cut="$(crombie2cut /home/dabercro/.crombie2/cuts/db/70ce239bddc877 regression) && jet1_gen_numnu == jet1_gen_numnu"

echo "Cut: $cut"

directory="TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"

mkdir $CrombieInFilesDir/$directory

crombie skim \
    -n "$CrombieNLocalProcs" \
    -i "$CrombieBigFilesDir/$directory" \
    -o "$CrombieInFilesDir/$directory" \
    -c "$cut" \
    --keep regression2.txt \
    --copy htotal

hadd $CrombieInFilesDir/$directory.root $CrombieInFilesDir/$directory/*.root
