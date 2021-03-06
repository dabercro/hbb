#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191117_resolution_envps%2Fresolution_jet1_response_rho_0%2C191117_resolution_onebin%2Fresolution_jet1_response_smear_0%2C191117_resolution_onebin_down%2Fresolution_jet1_response_smear_0%2C191117_resolution_onebin_nominal%2Fresolution_jet1_response_smear_0%2C191117_resolution_onebin_up%2Fresolution_jet1_response_smear_0%2C191117_resolution_envps%2Fresolution_jet1_response_rho_1%2C191117_resolution_envps%2Fresolution_jet1_response_rho_2"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191029_resolution_res%2Fresolution_190723_origin_2_rho_0%2C191029_resolution_res%2Fresolution_190723_origin_2_rho_1%2C191029_resolution_res%2Fresolution_190723_origin_2_rho_2"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191117_envps%2Frhoplot_4_0_jet1_response%2C191117_envps%2Frhoplot_4_1_jet1_response%2C191117_envps%2Frhoplot_4_2_jet1_response"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191118_really_onebin%2Fsmearplot_jet1_response%2C191118_really_onebin_down%2Fsmearplot_jet1_response%2C191118_really_onebin_nominal%2Fsmearplot_jet1_response%2C191118_really_onebin_up%2Fsmearplot_jet1_response"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191117_resolution_onebin_nominal%2Fmean_jet1_response_smear_0"

for f in */*.pdf
do
    pdfcrop $f $f
done
