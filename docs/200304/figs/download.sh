#!/bin/bash

rm -r 2*

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200303_alpha_lines_v2%2Fsmearplot_alpha"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200303_nbjets_noenv%2Fsmearplot_1_jet1_response%2C200303_nbjets_noenv%2Fsmearplot_2_jet1_response%2C200303_nbjets_noenv%2Fsmearplot_3_jet1_response%2C200303_nbjets_noenv%2Fsmearplot_4_jet1_response"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200303_nbjets%2Fsmearplot_2_jet1_response"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200303_smear_200303_nbjets%2Fresolution_jet1_response_smear_0"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200303_rho_200303_cb%2Fresolution_jet1_response_rho_0%2C200303_rho_200303_cb%2Fresolution_jet1_response_rho_1"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200303_smear_200303_cb%2Fresolution_jet1_response_unbinned_scale_nominal_smear_0%2C200303_smear_200303_cb%2Fresolution_jet1_response_single_scale_nominal_smear_0%2C200303_smear_200303_cb%2Fresolution_jet1_response_scale_nominal_smear_0%2C200303_smear_200303_cb%2Fresolution_jet1_response_unbinned_scale_up_smear_0%2C200303_smear_200303_cb%2Fresolution_jet1_response_single_scale_up_smear_0%2C200303_smear_200303_cb%2Fresolution_jet1_response_scale_up_smear_0"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200224_rho_200224_3rho_nb%2Fscale_fit%2C200303_rho_200303_cb%2Fscale_fit"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200303_cb_roofit%2Fsum_xsec_weight_alpha_data_slice0%2C200303_cb_roofit%2Fsum_xsec_weight_jet1_response_data_slice0%2C200303_cb_roofit%2Fsum_xsec_weight_alpha_mc_slice0%2C200303_cb_roofit%2Fsum_xsec_weight_jet1_response_mc_slice0%2C200303_cb_roofit%2Fsum_xsec_weight_alpha_gen_slice0%2C200303_cb_roofit%2Fsum_xsec_weight_jet1_response_intrinsic_gen_slice0"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200303_roofit%2Fsum_xsec_weight_rhoAll_data%2C200303_roofit%2Fsum_xsec_weight_rhoAll_mc"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200304_smear_200304_3d%2Fresolution_jet1_response_unbinned_3d_nominal_smear_0%2C200304_smear_200304_3d%2Fresolution_jet1_response_unbinned_3d_up_smear_0"

for f in */*.pdf
do
    pdfcrop $f $f
done
