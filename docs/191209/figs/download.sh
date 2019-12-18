#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191209_peak_lep%2Fcomparison%2C191209_peak_lep_up%2Fcomparison%2C191209_peak_met%2Fcomparison%2C191209_peak_met_up%2Fcomparison"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191209_smearplots%2Fsmearplot_jet1_response_old_nominal%2C191209_smearplots%2Fsmearplot_jet1_response_nominal%2C191209_smearplots%2Fsmearplot_jet1_response_old_up%2C191209_smearplots%2Fsmearplot_jet1_pt_dilep_pt"

for f in */*.pdf
do
    pdfcrop $f $f
done
