#!/bin/bash

rm -rf 19*

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191004_compareres%2Fplot_compare_eta%2C191004_compareres%2Fplot_compare_pt"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191002_tosmear%2Fsmearplot_0_jet1_190904_0_2_pt_dilep_corr_pt%2C191002_tosmear%2Fsmearplot_1_jet1_190904_0_2_pt_dilep_corr_pt"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191003_resolution_smeared%2Fresolution_190723_origin_2%2C191003_resolution_tosmear%2Fresolution_190723_origin_2%2C191003_resolution_smeared%2Fresolution_190725_lstm_pf%2C191003_resolution_tosmear%2Fresolution_190725_lstm_pf%2C191003_resolution_smeared%2Fresolution_190904_0_2%2C191003_resolution_tosmear%2Fresolution_190904_0_2"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190925_compareres%2Fplot_compare_eta%2C190925_compareres%2Fplot_compare_pt"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191004_violin_190723_origin_2%2Fbjetreg_2018_violin_eta_Hbb_dreg_2018_wide%2C191004_violin_190725_lstm_pf%2Fbjetreg_2018_violin_eta_Hbb_dreg_2018_wide%2C191004_violin_190904_0_3%2Fbjetreg_2018_violin_eta_Hbb_dreg_2018_wide%2C191004_violin_190723_origin_2%2Fbjetreg_2018_violin_Hbb_dreg_2018_wide%2C191004_violin_190725_lstm_pf%2Fbjetreg_2018_violin_Hbb_dreg_2018_wide%2C191004_violin_190904_0_3%2Fbjetreg_2018_violin_Hbb_dreg_2018_wide"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190830_puppi_raw%2Ftt_jet1_puppi_charged_pt_jet1_rawPt%2C190830_puppi_raw%2Ftt_jet1_puppi_neutral_pt_jet1_rawPt"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190910_valid%2Ftt_jet1_tf_190904_0_2_ptratio%2C190910_valid%2Flightz_jet1_tf_190904_0_2_ptratio%2C190910_valid%2Fheavyz_jet1_tf_190904_0_2_ptratio%2C190910_valid%2Ftt_jet2_tf_190904_0_2_ptratio%2C190910_valid%2Flightz_jet2_tf_190904_0_2_ptratio%2C190910_valid%2Fheavyz_jet2_tf_190904_0_2_ptratio"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191008_resolution_ptbinned%2Fresolution_190723_origin_2_pt_0%2C191008_resolution_ptbinned%2Fresolution_190904_0_3_pt_0%2C191008_resolution_ptbinned%2Fresolution_190725_lstm_pf_pt_0%2C191008_resolution_ptbinned%2Fresolution_190723_origin_2_pt_1%2C191008_resolution_ptbinned%2Fresolution_190904_0_3_pt_1%2C191008_resolution_ptbinned%2Fresolution_190725_lstm_pf_pt_1"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191008_resolution_etabinned%2Fresolution_190723_origin_2_eta_0%2C191008_resolution_etabinned%2Fresolution_190904_0_3_eta_0%2C191008_resolution_etabinned%2Fresolution_190725_lstm_pf_eta_0%2C191008_resolution_etabinned%2Fresolution_190723_origin_2_eta_1%2C191008_resolution_etabinned%2Fresolution_190904_0_3_eta_1%2C191008_resolution_etabinned%2Fresolution_190725_lstm_pf_eta_1"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=191009_compareres%2Fplot_compare_eta%2C191009_compareres%2Fplot_compare_pt"

for f in */*.pdf
do
    pdfcrop $f $f
done

cd ..

crombie backupslides

cd -
