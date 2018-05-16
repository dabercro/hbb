#!/bin/bash

rm */*.pdf
rmdir 1*

# Jet pt plots with old cut line

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180514_old%2Finclusive_signal_cmva_jet1_pt%2C180514_old%2Finclusive_tt_cmva_jet1_pt%2C180514_old%2Finclusive_lightz_cmva_jet1_pt%2C180514_old%2Finclusive_heavyz_cmva_jet1_pt%2C180514_old%2Finclusive_signal_cmva_jet2_pt%2C180514_old%2Finclusive_tt_cmva_jet2_pt%2C180514_old%2Finclusive_lightz_cmva_jet2_pt%2C180514_old%2Finclusive_heavyz_cmva_jet2_pt"

# CR plots

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180514__pre%2Finclusive_tt_cmva_jet2_cmva%2C180514_v1%2Finclusive_tt_cmva_jet2_cmva%2C180514__pre%2Finclusive_lightz_cmva_jet2_cmva%2C180514_v1%2Finclusive_lightz_cmva_jet2_cmva%2C180514__pre%2Finclusive_heavyz_cmva_jet2_cmva%2C180514_v1%2Finclusive_heavyz_cmva_jet2_cmva"

# Signal plots

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180515_class%2Finclusive_signal_maier_event_class%2C180515_class_post%2Finclusive_signal_maier_event_class"

# Data comparisons

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180515_lumi%2Fnpv%2C180515_npv%2Fnpv"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180515_fin%2Fn_centerjet%2C180515_fin%2Fpfmet"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180515_lumi%2Flog_cmva_jet1_cmva%2C180515_lumi%2Flog_cmva_jet2_cmva%2C180515_lumi%2Fdphi_met_dztrkmet%2C180515_lumi%2Fcmva_jet1_pt%2C180515_lumi%2Fcmva_jet2_pt"

for f in */*.pdf
do
    pdfcrop $f $f
done

test -f 2dpt.png || wget http://t3serv001.mit.edu/~dabercro/misc/2dpt.png
