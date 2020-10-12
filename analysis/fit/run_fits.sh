#! /bin/bash

for plotdir in 201009_2018ABC_v6 201009_2018D_v6
do

    ./resolution.py $plotdir > outputs/$plotdir.txt

done
