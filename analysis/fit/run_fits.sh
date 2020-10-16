#! /bin/bash

for plotdir in 201015_2016_tight 201015_2016_v4_tight
do

    ./resolution.py $plotdir > outputs/$plotdir.txt

done
