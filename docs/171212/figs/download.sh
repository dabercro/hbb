#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=171201%2FZvvHbb_common_deltaPhi__pfmetphi__hbbphi__%2C171201%2FZvvHbb_common_hbbm%2C171201%2FZvvHbb_common_nLooseLep%2C171201%2FZvvHbb_common_nTightLep%2C171201%2FZvvHbb_common_nJet%2C171201%2FZvvHbb_common_dphipfmet%2C171201%2FZvvHbb_common_hbbpt"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=171201%2FZvvHbb_tt_hbbm%2C171201%2FZvvHbb_scaledtt_hbbm"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=171201%2FZvvHbb_multijet_jetCSV%5Bhbbjtidx%5B0%5D%5D%2C171201%2FZvvHbb_scaledtt_jetCSV%5Bhbbjtidx%5B0%5D%5D%2C171201%2FZvvHbb_lightz_jetCSV%5Bhbbjtidx%5B0%5D%5D%2C171201%2FZvvHbb_heavyz_jetCSV%5Bhbbjtidx%5B0%5D%5D%2C171201%2FZvvHbb_multijet_jetCSV%5Bhbbjtidx%5B1%5D%5D%2C171201%2FZvvHbb_scaledtt_jetCSV%5Bhbbjtidx%5B1%5D%5D%2C171201%2FZvvHbb_lightz_jetCSV%5Bhbbjtidx%5B1%5D%5D%2C171201%2FZvvHbb_heavyz_jetCSV%5Bhbbjtidx%5B1%5D%5D%2C171201%2FZvvHbb_multijet_deltaPhi__pfmetphi__hbbphi__%2C171201%2FZvvHbb_scaledtt_deltaPhi__pfmetphi__hbbphi__%2C171201%2FZvvHbb_lightz_deltaPhi__pfmetphi__hbbphi__%2C171201%2FZvvHbb_heavyz_deltaPhi__pfmetphi__hbbphi__%2C171201%2FZvvHbb_multijet_hbbm%2C171201%2FZvvHbb_scaledtt_hbbm%2C171201%2FZvvHbb_lightz_hbbm%2C171201%2FZvvHbb_heavyz_hbbm%2C171201%2FZvvHbb_multijet_jet1Pt%2C171201%2FZvvHbb_scaledtt_jet1Pt%2C171201%2FZvvHbb_lightz_jet1Pt%2C171201%2FZvvHbb_heavyz_jet1Pt%2C171201%2FZvvHbb_multijet_jet2Pt%2C171201%2FZvvHbb_scaledtt_jet2Pt%2C171201%2FZvvHbb_lightz_jet2Pt%2C171201%2FZvvHbb_heavyz_jet2Pt%2C171201%2FZvvHbb_multijet_nJet%2C171201%2FZvvHbb_scaledtt_nJet%2C171201%2FZvvHbb_lightz_nJet%2C171201%2FZvvHbb_heavyz_nJet%2C171201%2FZvvHbb_multijet_pfmet%2C171201%2FZvvHbb_scaledtt_pfmet%2C171201%2FZvvHbb_lightz_pfmet%2C171201%2FZvvHbb_heavyz_pfmet%2C171201%2FZvvHbb_multijet_dphipfmet%2C171201%2FZvvHbb_scaledtt_dphipfmet%2C171201%2FZvvHbb_lightz_dphipfmet%2C171201%2FZvvHbb_heavyz_dphipfmet%2C171201%2FZvvHbb_multijet_hbbpt%2C171201%2FZvvHbb_scaledtt_hbbpt%2C171201%2FZvvHbb_lightz_hbbpt%2C171201%2FZvvHbb_heavyz_hbbpt"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=171212_old%2Ftmva"

wget http://t3serv001.mit.edu/~dabercro/misc/171201/table.png

for f in */*.pdf
do
    pdfcrop $f $f
done
