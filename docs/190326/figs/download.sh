#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190304_newb_norm%2Ftt_n_jet%2C190311_jecV8_norm%2Fnewb_tt_n_jet%2C190304_newb_norm%2Ftt_jet1_pt%2C190311_jecV8_norm%2Fnewb_tt_jet1_pt%2C190304_newb_norm%2Flightz_jet1_pt%2C190311_jecV8_norm%2Fnewb_lightz_jet1_pt%2C190304_newb_norm%2Fheavyz_jet1_pt%2C190311_jecV8_norm%2Fnewb_heavyz_jet1_pt%2C190304_newb_norm%2Ftt_jet2_pt%2C190311_jecV8_norm%2Fnewb_tt_jet2_pt%2C190304_newb_norm%2Flightz_jet2_pt%2C190311_jecV8_norm%2Fnewb_lightz_jet2_pt%2C190304_newb_norm%2Fheavyz_jet2_pt%2C190311_jecV8_norm%2Fnewb_heavyz_jet2_pt"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190326_v2%2Fsignal_tf_v5_yes%2C190326_v2%2Fheavyz_tf_v5_yes"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=testhbb%2Ftf_v3_yes%2Ctesthbb%2Ftf_v5_yes"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190326_v2%2Ftt_recoil%2C190326_v2%2Flightz_recoil%2C190326_v2%2Fheavyz_recoil"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190326_v2%2Fsignal_tf_v3_yes%2C190326_v2%2Ftt_tf_v3_yes%2C190326_v2%2Flightz_tf_v3_yes%2C190326_v2%2Fheavyz_tf_v3_yes%2C190326_v2%2Fsignal_tf_v5_yes%2C190326_v2%2Ftt_tf_v5_yes%2C190326_v2%2Flightz_tf_v5_yes%2C190326_v2%2Fheavyz_tf_v5_yes"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=190326_v2%2Fsignal_n_centerjet%2C190326_v2%2Ftt_n_centerjet%2C190326_v2%2Flightz_n_centerjet%2C190326_v2%2Fheavyz_n_centerjet"

for f in */*.pdf
do
    pdfcrop $f $f
done
