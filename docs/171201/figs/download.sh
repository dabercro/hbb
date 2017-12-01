#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=171130_v2%2FZvvHbb_multijet_hbbm%2C171130_v2%2FZvvHbb_tt_hbbm%2C171130_v2%2FZvvHbb_scaledtt_hbbm%2C171130_v2%2FZvvHbb_lightz_hbbm%2C171130_v2%2FZvvHbb_heavyz_hbbm"

for f in */*.pdf
do
    pdfcrop $f $f
done
