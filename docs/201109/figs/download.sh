#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=201105_STXSfine_400split_Xbb_nobtag_ddac2796_0d357869%2Fsummary_stxs%2C201107_inclVZ_XbbVZ_nobtag_dc5fd809_0d357869%2Fsummary_stxs%2C201108_STXS_VZ_XbbVZ_nobtag_b82db76a_0d357869%2Fsummary_stxs%2C201108_leptons_Xbb_nobtag_a321b761_0d357869%2Fsummary_stxs"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=201105_STXSfine_400split_Xbb_nobtag_ddac2796_0d357869%2Fprefit_summary_stxs"

for f in */*.pdf
do
    pdfcrop $f $f
done
