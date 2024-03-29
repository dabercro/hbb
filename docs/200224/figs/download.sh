#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200223_roofit_custom%2Fsum_xsec_weight_data%2C200224_roofit_custom%2Fsum_xsec_weight_data%2C200223_roofit_custom%2Fsum_xsec_weight_gen%2C200224_roofit_custom%2Fsum_xsec_weight_gen"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200224_rho_200224_3rho_nb%2Fscale_fit%2C200224_rho_200224_fin_nbjet%2Fscale_fit"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200223_smear_200223_2rho_custom_2018%2Fresolution_jet1_response_scale_nominal_smear_0%2C200223_smear_200223_3rho_applied_custom_2018%2Fresolution_jet1_response_scale_nominal_smear_0"
#crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200223_smear_200223_2rho_custom_2018%2Fresolution_jet1_response_scale_nominal_smear_0%2C200223_smear_200223_3rho_applied_custom_2018%2Fresolution_jet1_response_scale_nominal_smear_0%2C200223_smear_200223_2rho_custom_2018%2Fresolution_jet1_response_scale_up_smear_0%2C200223_smear_200223_3rho_applied_custom_2018%2Fresolution_jet1_response_scale_up_smear_0"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200223_2rho_custom_2018%2Fsmearplot_jet1_response_scale_nominal%2C200223_3rho_applied_custom_2018%2Fsmearplot_jet1_response_scale_nominal%2C200223_2rho_custom_2018%2Fsmearplot_jet1_response_scale_up%2C200223_3rho_applied_custom_2018%2Fsmearplot_jet1_response_scale_up"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200224_jet_200224_custom_pt%2Fresolution_jet1_response_jet_0%2C200224_jet_200224_custom_pt%2Fresolution_jet1_response_jet_1%2C200224_jet_200224_custom_pt%2Fresolution_jet1_response_jet_2"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200224_fin%2Fsmearplot_jet1_response_unbinned_scale_nominal%2C200224_fin%2Fsmearplot_jet1_response_unbinned_scale_up"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200224_jet_200224_custom_pt%2Fresolution_jet1_response_unbinned_scale_nominal_jet_0%2C200224_jet_200224_custom_pt%2Fresolution_jet1_response_scale_nominal_jet_0%2C200224_jet_200224_custom_pt%2Fresolution_jet1_response_unbinned_scale_nominal_jet_1%2C200224_jet_200224_custom_pt%2Fresolution_jet1_response_scale_nominal_jet_1%2C200224_jet_200224_custom_pt%2Fresolution_jet1_response_unbinned_scale_nominal_jet_2%2C200224_jet_200224_custom_pt%2Fresolution_jet1_response_scale_nominal_jet_2"

for f in */*.pdf
do
    pdfcrop $f $f
done
