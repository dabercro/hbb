#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=201004_smear_201002_2018_divmean%2Fmean_jet1_adjusted_response_smear_0%2C201004_smear_201002_2018_v6_divmean%2Fmean_jet1_adjusted_response_smear_0%2C201004_smear_201002_2018_v6_norejec_divmean%2Fmean_jet1_adjusted_response_smear_0%2C201004_smear_201003_2017_divmean%2Fmean_jet1_adjusted_response_smear_0%2C201004_smear_201004_2017_v4_divmean%2Fmean_jet1_adjusted_response_smear_0%2C201004_smear_201002_2018_divmean%2Fresolution_jet1_adjusted_response_smear_0%2C201004_smear_201002_2018_v6_divmean%2Fresolution_jet1_adjusted_response_smear_0%2C201004_smear_201002_2018_v6_norejec_divmean%2Fresolution_jet1_adjusted_response_smear_0%2C201004_smear_201003_2017_divmean%2Fresolution_jet1_adjusted_response_smear_0%2C201004_smear_201004_2017_v4_divmean%2Fresolution_jet1_adjusted_response_smear_0"

for f in */*.pdf
do
    pdfcrop $f $f
done
