#! /bin/bash

epochs=20
batch=170000
layers=5
output=test.pb
seed=12348765

while getopts ":he:b:l:o:" o
do
    case "${o}" in
        h)
            echo "$0 [-h] [-e EPOCHS (${epochs})] [-b BATCHES (${batch})] [-l LAYERS (${layers})] [-o OUTPUT (${output})] [-s SEED ${seed}]"
            exit 1
            ;;
        e)
            epochs=$OPTARG        
            ;;
        b)
            batch=$OPTARG
            ;;
        l)
            layers=$OPTARG
            ;;
        o)
            output=$OPTARG
            ;;
    esac
done

input=/local/dabercro/files/training_v3.root

source CrombiePlotterConfig.sh

test -f $input || input=$CrombieInFilesDir/$(basename $input)

test -d history || mkdir history

./classifier.py \
    -c classify2.txt \
    -i $input\
    -t hbb_gen \
    -o $output \
    -e $epochs \
    -b $batch \
    -l $layers \
    -a 'hbb_m,hbb_pt' \
    -w XSecWeight \
    -s $seed \
    -r
