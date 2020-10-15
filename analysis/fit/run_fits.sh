#! /bin/bash

for plotdir in 201013_2016_nobsf_tight_loose 201013_2016_medium_loose 201013_2016_tight_loose 201013_2016_tight_medium 201013_2016_tight 201013_2016_v4_medium_loose 201013_2016_v4_tight_loose 201013_2016_v4_tight_medium 201013_2016_v4_tight
do

    ./resolution.py $plotdir > outputs/$plotdir.txt

done
