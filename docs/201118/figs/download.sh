#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=201114_STXS_VZ_unblinded_XbbVZ_nobtag_d308a317_0d357869%2Fsummary_stxs%2C201115_STXSfine_400split_Xbb_nobtag_f8140c84_0d357869%2Fsummary_stxs%2C201115_VZ_unblinded_XbbVZ_nobtag_d308a317_0d357869%2Fsummary_stxs%2C201115_inclVZ_unblinded_XbbVZ_d308a317_0d357869%2Fsummary_stxs"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=201115_inclVZ_unblinded_XbbVZ_d308a317_0d357869%2Fscan_nominal_r"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=201115_STXSfine_400split_Xbb_nobtag_f8140c84_0d357869%2Flimit"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=201116_VZyear_unblinded_XbbVZ_nobtag_d308a317_0d357869%2Fsummary_stxs"

for f in */*.pdf
do
    pdfcrop $f $f
done
