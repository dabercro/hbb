#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180531_v2%2Finclusive_tt_jet2_deepCSVb%2C180531_v2%2Finclusive_lightz_jet2_deepCSVb%2C180531_v2%2Finclusive_heavyz_jet2_deepCSVb%2C180531_v2%2Finclusive_signal_event_class%2C180531_v2%2Finclusive_tt_pfmet%2C180531_v2%2Finclusive_lightz_pfmet%2C180531_v2%2Finclusive_heavyz_pfmet"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180531_class_pre%2Finclusive_signal_event_class"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180531_post_v5%2Finclusive_tt_jet2_deepCSVb%2C180531_post_v5%2Finclusive_lightz_jet2_deepCSVb%2C180531_post_v5%2Finclusive_heavyz_jet2_deepCSVb%2C180531_post_v5%2Fboosted_tt_ak8fatjet1_mSD_corr%2C180531_post_v5%2Fboosted_lightz_ak8fatjet1_mSD_corr%2C180531_post_v5%2Finclusive_signal_event_class"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180531_v2%2Fboosted_tt_ak8fatjet1_mSD_corr%2C180531_v2%2Fboosted_lightz_ak8fatjet1_mSD_corr"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180531_post_v5%2Finclusive_signal_pfmet%2C180531_post_v5%2Finclusive_tt_pfmet%2C180531_post_v5%2Finclusive_lightz_pfmet%2C180531_post_v5%2Finclusive_heavyz_pfmet"

for f in */*.pdf
do
    pdfcrop $f $f
done
