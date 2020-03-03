#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200226_woprefire_v1%2Flightz_FatJet_eta%2C200226_wprefire_v1%2Flightz_FatJet_eta%2C200226_woprefire_v1%2Fheavyz_FatJet_eta%2C200226_wprefire_v1%2Fheavyz_FatJet_eta"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200226_wprefire_v1%2Flightz_V_pt%2C200226_wprefire_v2%2Flightz_V_pt%2C200226_wprefire_v1%2Fheavyz_V_pt%2C200226_wprefire_v2%2Fheavyz_V_pt"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200227_v1_norm%2Flightz_V_pt%2C200227_v2_norm%2Flightz_V_pt%2C200227_v1_norm%2Fheavyz_V_pt%2C200227_v2_norm%2Fheavyz_V_pt"

for f in */*.pdf
do
    pdfcrop $f $f
done
