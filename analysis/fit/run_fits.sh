#! /bin/bash

for plotdir in 201004_2017_v4
do

    ./resolution.py $plotdir > outputs/$plotdir.txt

done
