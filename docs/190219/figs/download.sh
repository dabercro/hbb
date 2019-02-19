#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190219_nopu%2Flightz_npv%2C190219_v1%2Flightz_npv"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190131_2018mc%2Ftt_jet2_deepCSVb%2C190219_newdeep%2Ftt_jet2_deepCSVb%2C190131_2018mc%2Fheavyz_jet2_deepCSVb%2C190219_newdeep%2Fheavyz_jet2_deepCSVb"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190214_tnp_bjets%2Flight_pt_0_150_eta_0_1p4_probe_deepCSVb%2C190214_tnp_bjets%2Fheavy_pt_0_150_eta_0_1p4_probe_deepCSVb%2C190214_tnp_bjets%2Flight_pt_150_250_eta_0_1p4_probe_deepCSVb%2C190214_tnp_bjets%2Fheavy_pt_150_250_eta_0_1p4_probe_deepCSVb%2C190214_tnp_bjets%2Flight_pt_0_150_eta_1p4_2p5_probe_deepCSVb%2C190214_tnp_bjets%2Fheavy_pt_0_150_eta_1p4_2p5_probe_deepCSVb%2C190214_tnp_bjets%2Flight_pt_150_250_eta_1p4_2p5_probe_deepCSVb%2C190214_tnp_bjets%2Fheavy_pt_150_250_eta_1p4_2p5_probe_deepCSVb"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190215_closure%2Ftt_jet2_deepCSVb%2C190215_closure%2Fz_jet2_deepCSVb"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190219_newdeep%2Ftt_jet2_deepCSVb%2C190219_newdeep%2Fheavyz_jet2_deepCSVb"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190219_olddeep%2Ftt_jet2_deepCSVb%2C190219_olddeep%2Fheavyz_jet2_deepCSVb"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190219_newtrig%2Ftt_recoil%2C190219_oldtrig%2Ftt_recoil%2C190219_newtrig%2Flightz_recoil%2C190219_oldtrig%2Flightz_recoil%2C190219_newtrig%2Fheavyz_recoil%2C190219_oldtrig%2Fheavyz_recoil"

for f in */*.pdf
do
    pdfcrop $f $f
done
