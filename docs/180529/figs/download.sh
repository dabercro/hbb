#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180529_v1%2Finclusive_signal_pfmet"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180515_fin%2Fpfmet%2C180516_trig%2Fpfmet%2C180515_npv%2Fnpv"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180515_lumi%2Fnpv"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180529_v1%2Finclusive_signal_event_class"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180514__pre%2Finclusive_tt_cmva_jet2_cmva%2C180514_v1%2Finclusive_tt_cmva_jet2_cmva%2C180514__pre%2Finclusive_lightz_cmva_jet2_cmva%2C180514_v1%2Finclusive_lightz_cmva_jet2_cmva%2C180514__pre%2Finclusive_heavyz_cmva_jet2_cmva%2C180514_v1%2Finclusive_heavyz_cmva_jet2_cmva"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180515_class%2Finclusive_signal_maier_event_class%2C180515_class_post%2Finclusive_signal_maier_event_class"

for f in */*.pdf
do
    pdfcrop $f $f
done

wget -O tf.pdf 'http://t3serv001.mit.edu/~dabercro/plots/180521_test/cmva_hbb_m_reg_old__cmva_hbb_m_tf__cmva_hbb_m_reg_old___250_____pfmet_%3e_170____met_filter_==_1____n_.pdf'
