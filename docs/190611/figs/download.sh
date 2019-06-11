#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190611%2Fplot_time_120000_compare%2C190611%2Fplot_time_40000_compare%2C190611%2Fplot_time_160000_compare%2C190611%2Fplot_time_60000_compare%2C190611%2Fplot_time_80000_compare"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190611_violin_190607_pf_narrowloss_60000%2Fbjetreg_2018_violin_eta_Hbb_dreg_2018_wide%2C190611_violin_190607_pf_narrowloss_60000%2Fbjetreg_2018_violin_Hbb_dreg_2018_wide"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190611%2Fplot_time_120000_wide%2C190611%2Fplot_time_40000_wide%2C190611%2Fplot_time_160000_wide%2C190611%2Fplot_time_60000_wide%2C190611%2Fplot_time_80000_wide%2C190611%2Fplot_time_wide%2C190611%2Fplot_time_pf_wide%2C190611%2Fplot_wide%2C190611%2Fplot_time_120000_narrow%2C190611%2Fplot_time_40000_narrow%2C190611%2Fplot_time_160000_narrow%2C190611%2Fplot_time_60000_narrow%2C190611%2Fplot_time_80000_narrow%2C190611%2Fplot_time_narrow%2C190611%2Fplot_time_pf_narrow%2C190611%2Fplot_narrow"

for f in */*.pdf
do
    pdfcrop $f $f
done
