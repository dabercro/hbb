#! /bin/bash

export vers=v3
export type=classification

source CrombiePlotterConfig.sh

echo "Output directory: $CrombieInFilesDir"

mkdir -p $CrombieInFilesDir

cut="$(crombie2cut /home/dabercro/.crombie2/cuts/44/ce8b76e38c1ae9 light_training)"

echo "Cut: $cut"

while read line
do

    if echo $line | grep .root > /dev/null
    then
        directory=`echo $line | cut -d ' ' -f 2  | cut -d '.' -f 1`

        mkdir $CrombieInFilesDir/$directory

        crombie skim \
            -n "$CrombieNLocalProcs" \
            -i "$CrombieBigFilesDir/$directory" \
            -o "$CrombieInFilesDir/$directory" \
            -c "$cut" \
            --keep classify2.txt \
            --copy htotal

        hadd $CrombieInFilesDir/$directory.root $CrombieInFilesDir/$directory/*.root

    fi

done < "$CrombieMCConfig"

crombie addxs

hadd $CrombieInFilesDir/training_$vers.root $CrombieInFilesDir/*.root
