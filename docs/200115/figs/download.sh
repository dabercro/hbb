#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200115_1D_bins%2Fsmearplot_1_jet1_response%2C200115_1D_bins%2Fsmearplot_2_jet1_response%2C200115_1D_bins%2Fsmearplot_3_jet1_response%2C200115_1D_bins%2Fsmearplot_4_jet1_response"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200116_1D_bins_allenv%2Fsmearplot_2_jet1_response%2C200116_1D_bins_psenv%2Fsmearplot_2_jet1_response%2C200116_1D_bins_scaleenv%2Fsmearplot_2_jet1_response"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200116_singlebin_alpha%2Fsmearplot_alpha"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200116_1D_bins_allenv_resolution%2Fresolution_jet1_response_smear_0"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200121_roofit%2FGaussian_xsec_weight_mc%2C200121_roofit%2FLandau_xsec_weight_mc"

for f in */*.pdf
do
    pdfcrop $f $f
done
