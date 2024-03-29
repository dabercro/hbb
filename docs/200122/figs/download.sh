#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200115_1D_bins%2Fsmearplot_1_jet1_response%2C200115_1D_bins%2Fsmearplot_2_jet1_response%2C200115_1D_bins%2Fsmearplot_3_jet1_response%2C200115_1D_bins%2Fsmearplot_4_jet1_response"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200116_1D_bins_allenv%2Fsmearplot_2_jet1_response%2C200116_1D_bins_psenv%2Fsmearplot_2_jet1_response%2C200116_1D_bins_scaleenv%2Fsmearplot_2_jet1_response"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200116_singlebin_alpha%2Fsmearplot_alpha"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200116_1D_bins_allenv_resolution%2Fresolution_jet1_response_smear_0"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200121_roofit%2FGaussian_xsec_weight_mc%2C200121_roofit%2FLandau_xsec_weight_mc"
 crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200122_comparison_resolution%2Fresolution_jet1_response_single_scale_down_smear_0%2C200122_comparison_resolution%2Fresolution_jet1_response_single_scale_nominal_smear_0%2C200122_comparison_resolution%2Fresolution_jet1_response_single_scale_up_smear_0%2C200122_comparison_resolution%2Fresolution_jet1_response_single_smear_down_smear_0%2C200122_comparison_resolution%2Fresolution_jet1_response_single_smear_nominal_smear_0%2C200122_comparison_resolution%2Fresolution_jet1_response_single_smear_up_smear_0%2C200122_comparison_resolution%2Fresolution_jet1_response_unbinned_scale_down_smear_0%2C200122_comparison_resolution%2Fresolution_jet1_response_unbinned_scale_nominal_smear_0%2C200122_comparison_resolution%2Fresolution_jet1_response_unbinned_scale_up_smear_0%2C200122_comparison_resolution%2Fresolution_jet1_response_unbinned_smear_down_smear_0%2C200122_comparison_resolution%2Fresolution_jet1_response_unbinned_smear_nominal_smear_0%2C200122_comparison_resolution%2Fresolution_jet1_response_unbinned_smear_up_smear_0"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200122_peak_lep%2Fcomparison%2C200122_peak_met%2Fcomparison"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191216_resolution_tosmear%2Fsmear_fit"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200122_comparison%2Fsmearplot_jet1_response_unbinned_scale_nominal%2C200122_comparison%2Fsmearplot_jet1_response_single_scale_nominal%2C200122_comparison%2Fsmearplot_jet1_response_unbinned_smear_nominal%2C200122_comparison%2Fsmearplot_jet1_response_single_smear_nominal%2C200122_comparison%2Fsmearplot_jet1_response_unbinned_scale_up%2C200122_comparison%2Fsmearplot_jet1_response_single_scale_up%2C200122_comparison%2Fsmearplot_jet1_response_unbinned_smear_up%2C200122_comparison%2Fsmearplot_jet1_response_single_smear_up"

for f in */*.pdf
do
    pdfcrop $f $f
done
