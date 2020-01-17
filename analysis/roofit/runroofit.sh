#! /bin/bash

for shape in Landau Gaussian
do

    ./roofit.py $shape xsec_weight

    for weight in {0..8}
    do

        if [ weight -eq 5 ] || [ weight -eq 7 ]
        then
            continue
        fi

        ./roofit.py $shape xsec_weight_scale_weight_$weight

    done


    for weight in {0..3}
    do

        ./roofit.py $shape xsec_weight_ps_weight_$weight

    done

done
