#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=201217_STXS_VZ_unblinded_XbbVZ_bba1b36f_4ef7b680%2Fsummary_stxs%2C201217_VZ_channel_year_unblinded_XbbVZ_bba1b36f_4ef7b680%2Fsummary_stxs%2C201217_VZ_unblinded_XbbVZ_bba1b36f_4ef7b680%2Fsummary_stxs%2C201217_inclVZ_unblinded_XbbVZ_bba1b36f_4ef7b680%2Fsummary_stxs"

for f in */*.pdf
do
    pdfcrop $f $f
done
