#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=201023_STXSfine_Xbb_nobtag_f8c96d5f_0d357869%2Fsummary_stxs%2C201026_vh_AT_nobtag_70c6808f_0d357869%2Fsummary_stxs"


for f in */*.pdf
do
    pdfcrop $f $f
done
