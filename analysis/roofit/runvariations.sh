#! /bin/bash

outdir=${1:?}

test -d $outdir || mkdir $outdir
test -d plots || mkdir plots

for shape in sum #Landau Gaussian
do

    ./roofit.py $shape xsec_weight > $outdir/${shape}_xsec_weight.txt

    for weight in {0..8}
    do

        if [ $weight -eq 5 ] || [ $weight -eq 7 ]
        then
            continue
        fi

        ./roofit.py $shape xsec_weight_scale_weight_$weight > $outdir/${shape}_xsec_weight_scale_weight_$weight.txt

    done


    for weight in {0..3}
    do

        ./roofit.py $shape xsec_weight_ps_weight_$weight > $outdir/${shape}_xsec_weight_ps_weight_$weight.txt

    done

done

cp CrombieAnalysisConfig.sh $outdir
cp roofit.py $outdir
mv plots/* $outdir
