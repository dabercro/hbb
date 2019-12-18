#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191128_2016%2Fsmearplot_jet1_response%2C191128_2017%2Fsmearplot_jet1_response%2C191128_2018%2Fsmearplot_jet1_response%2C191128_2016%2Fsmearplot_jet1_response_nominal%2C191128_2017%2Fsmearplot_jet1_response_nominal%2C191128_2018%2Fsmearplot_jet1_response_nominal%2C191128_2016%2Fsmearplot_jet1_response_down%2C191128_2017%2Fsmearplot_jet1_response_down%2C191128_2018%2Fsmearplot_jet1_response_down%2C191128_2016%2Fsmearplot_jet1_response_up%2C191128_2017%2Fsmearplot_jet1_response_up%2C191128_2018%2Fsmearplot_jet1_response_up"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191128_resolution_response%2Fresolution_jet1_response_rho_0%2C191128_resolution_response%2Fresolution_jet1_response_rho_1%2C191128_resolution_response%2Fresolution_jet1_response_rho_2%2C191128_resolution_response%2Fsmear_fit"

# ((((lep1_pt > 20 && lep2_pt > 20 && (lep1_q + lep2_q) == 0 && dilep_pt > 100) && (lep1_miniPFRelIso_all < 0.25 && lep2_miniPFRelIso_all < 0.25) && (fabs(jet1_eta) < 2.4 && alpha < 0.3 && jet1_dphi_ll > 2.8 && jet1_btagDeepB > 0.9) && jet2) && (alpha < 0.185 && !(alpha < 0.155))) && (!(rhoAll < 16.5) && rhoAll < 22))
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191128_tosmear%2Frhoplot_2_1_jet1_response"

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191129_resolution_close%2Fresolution_jet1_response_smear_0%2C191129_resolution_close%2Fresolution_jet1_response_nominal_smear_0%2C191129_resolution_close%2Fresolution_jet1_response_down_smear_0%2C191129_resolution_close%2Fresolution_jet1_response_up_smear_0"

for f in */*.pdf
do
    pdfcrop $f $f
done
