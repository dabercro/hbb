#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190927_violin_190723_origin%2Fbjetreg_2018_violin_eta_Hbb_dreg_2018_wide%2C190927_violin_190904_0_2%2Fbjetreg_2018_violin_eta_Hbb_dreg_2018_wide%2C190927_violin_190723_origin%2Fbjetreg_2018_violin_Hbb_dreg_2018_wide%2C190927_violin_190904_0_2%2Fbjetreg_2018_violin_Hbb_dreg_2018_wide"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191003_compareres%2Fplot_compare_eta%2C191003_compareres%2Fplot_compare_pt"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191002_tosmear%2Fsmearplot_0_jet1_190904_0_2_pt_dilep_corr_pt%2C191002_tosmear%2Fsmearplot_1_jet1_190904_0_2_pt_dilep_corr_pt"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191003_resolution_tosmear%2Fresolution_190904_0_2"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191003_resolution_smeared%2Fresolution_190904_0_2"

for f in */*.pdf
do
    pdfcrop $f $f
done
