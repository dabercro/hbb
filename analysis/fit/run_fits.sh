#! /bin/bash

for plotdir in 201002_2018 201002_2018_v6 201002_2018_v6_norejec 201003_2016 201003_2016_v4 201003_2017 201003_2017_v4
do

    ./resolution.py $plotdir > outputs/$plotdir.txt

done
