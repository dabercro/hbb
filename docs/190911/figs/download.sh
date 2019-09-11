#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190830_puppi_raw%2Ftt_jet1_puppi_charged_pt_jet1_rawPt%2C190830_puppi_raw%2Ftt_jet1_puppi_neutral_pt_jet1_rawPt%2C190830_puppi_raw%2Ftt_jet2_puppi_charged_pt_jet2_rawPt%2C190830_puppi_raw%2Ftt_jet2_puppi_neutral_pt_jet2_rawPt%2C190815_validation%2Ftt_jet1_puppi_charged_pt_jet1_pt%2C190815_validation%2Ftt_jet1_puppi_neutral_pt_jet1_pt%2C190815_validation%2Ftt_jet2_puppi_charged_pt_jet2_pt%2C190815_validation%2Ftt_jet2_puppi_neutral_pt_jet2_pt"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190910_valid%2Ftt_jet1_tf_190904_0_2_ptratio%2C190910_valid%2Ftt_jet2_tf_190904_0_2_ptratio"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190828_puppi%2Ftt_jet1_deepCSV%2C190910_not_puppi%2Ftt_jet1_deepCSV"

for f in */*.pdf
do
    pdfcrop $f $f
done
