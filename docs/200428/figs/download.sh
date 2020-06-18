#!/bin/bash

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200428_Wlv%2Fsignal_FatJet_Msoftdrop%2C200428_Wlv%2Ftt_FatJet_Msoftdrop%2C200428_Wlv%2Flightz_FatJet_Msoftdrop%2C200428_Wlv%2Fheavyz_FatJet_Msoftdrop%2C200428_Wlv%2Fsignal_V_pt%2C200428_Wlv%2Ftt_V_pt%2C200428_Wlv%2Flightz_V_pt%2C200428_Wlv%2Fheavyz_V_pt%2C200428_Wlv%2Fsignal_FatJet_pt%2C200428_Wlv%2Ftt_FatJet_pt%2C200428_Wlv%2Flightz_FatJet_pt%2C200428_Wlv%2Fheavyz_FatJet_pt"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=200428_Wlv%2Fsignal_FatJet_deepTagMD_bbvsLight%2C200428_Wlv%2Ftt_FatJet_deepTagMD_bbvsLight%2C200428_Wlv%2Flightz_FatJet_deepTagMD_bbvsLight%2C200428_Wlv%2Fheavyz_FatJet_deepTagMD_bbvsLight"

for f in */*.pdf
do
    pdfcrop $f $f
done
