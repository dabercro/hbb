#!/bin/bash

rm */*.pdf
rmdir 1*

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180514_old%2Finclusive_signal_cmva_jet1_pt%2C180514_old%2Finclusive_tt_cmva_jet1_pt%2C180514_old%2Finclusive_lightz_cmva_jet1_pt%2C180514_old%2Finclusive_heavyz_cmva_jet1_pt%2C180514_old%2Finclusive_signal_cmva_jet2_pt%2C180514_old%2Finclusive_tt_cmva_jet2_pt%2C180514_old%2Finclusive_lightz_cmva_jet2_pt%2C180514_old%2Finclusive_heavyz_cmva_jet2_pt"

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180514__pre%2Finclusive_tt_cmva_jet2_cmva%2C180514_v1%2Finclusive_tt_cmva_jet2_cmva%2C180514__pre%2Finclusive_lightz_cmva_jet2_cmva%2C180514_v1%2Finclusive_lightz_cmva_jet2_cmva%2C180514__pre%2Finclusive_heavyz_cmva_jet2_cmva%2C180514_v1%2Finclusive_heavyz_cmva_jet2_cmva%2C180514__pre%2Finclusive_signal_maier_event_class%2C180514_v1%2Finclusive_signal_maier_event_class"

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180514_quick%2Fnpv"

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180509_v1%2Fcmva_jet1_cmva%2C180509_v1%2Fcmva_jet2_cmva%2C180509_v1%2Fmin_dphi_metj_hard%2C180509_v1%2Fn_centerjet%2C180509_v1%2Fpfmet%2C180509_v1%2Fcmva_jet1_pt%2C180509_v1%2Fcmva_jet2_pt%2C180509_v1%2Fnpv"

for f in */*.pdf
do
    pdfcrop $f $f
done
