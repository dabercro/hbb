#! /bin/bash

for plotdir in 201007_2016 201007_2016_v4
do

    ./resolution.py $plotdir > outputs/$plotdir.txt

done
