#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=201009_smear_201009_2018ABC_divmean%2Fresolution_jet1_adjusted_response_smear_0%2C201009_smear_201009_2018D_divmean%2Fresolution_jet1_adjusted_response_smear_0"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=201008_2017_split%2Fsmearplot_jet1_adjusted_response_smeared_scaled_nominal%2C201008_2017_split_oldsel%2Fsmearplot_jet1_adjusted_response_smeared_scaled_nominal%2C201008_2018_split%2Fsmearplot_jet1_adjusted_response_smeared_scaled_nominal%2C201008_2018_split_oldsel%2Fsmearplot_jet1_adjusted_response_smeared_scaled_nominal%2C201012_2018_split_tight_loose%2Fsmearplot_jet1_adjusted_response_smeared_scaled_nominal%2C201012_2018_split_tight_medium%2Fsmearplot_jet1_adjusted_response_smeared_scaled_nominal"

for f in */*.pdf
do
    pdfcrop $f $f
done