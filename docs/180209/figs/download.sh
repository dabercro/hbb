#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180209_v1%2Fregression_withgen%2C180209_v1%2Fregression"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180209_v1%2Fquick_signal_cmva_hbb_m_reg%2C180209_v1%2Fquick_tt_cmva_hbb_m_reg%2C180209_v1%2Fquick_lightz_cmva_hbb_m_reg%2C180209_v1%2Fquick_heavyz_cmva_hbb_m_reg%2C180209_v1%2Fquick_signal_event_class%2C180209_v1%2Fquick_tt_event_class%2C180209_v1%2Fquick_lightz_event_class%2C180209_v1%2Fquick_heavyz_event_class"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180209_v1%2FZvvHbb_signal_cmva_hbb_m_reg%2C180209_v1%2FZvvHbb_signal_event_class"


for f in */*.pdf
do
    pdfcrop $f $f
done
