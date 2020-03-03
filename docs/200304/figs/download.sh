#!/bin/bash

rm -r 2*

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200303_alpha_lines_v2%2Fsmearplot_alpha"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200303_nbjets_noenv%2Fsmearplot_1_jet1_response%2C200303_nbjets_noenv%2Fsmearplot_2_jet1_response%2C200303_nbjets_noenv%2Fsmearplot_3_jet1_response%2C200303_nbjets_noenv%2Fsmearplot_4_jet1_response"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200303_nbjets%2Fsmearplot_2_jet1_response"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200303_smear_200303_nbjets%2Fresolution_jet1_response_smear_0"

for f in */*.pdf
do
    pdfcrop $f $f
done
