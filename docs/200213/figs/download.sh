#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200208_zvv_2017%2Flightz_FatJet_eta%5BFatJet_HiggsProducts%5D%2C200208_zvv_2017%2Fheavyz_FatJet_eta%5BFatJet_HiggsProducts%5D%2C200210_test%2Flightz_FatJet_eta%2C200210_test%2Fheavyz_FatJet_eta"

for f in */*.pdf
do
    pdfcrop $f $f
done
