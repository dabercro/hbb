#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200115_1D_bins%2Fsmearplot_1_jet1_response%2C200115_1D_bins%2Fsmearplot_2_jet1_response%2C200115_1D_bins%2Fsmearplot_3_jet1_response%2C200115_1D_bins%2Fsmearplot_4_jet1_response"

for f in */*.pdf
do
    pdfcrop $f $f
done
