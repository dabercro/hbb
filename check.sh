#! /bin/bash

for f in /mnt/hadoop/cms/store/user/paus/pandaf/013/TTToHadronic_TuneCP5_13TeV-powheg-pythia8+RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1+MINIAODSIM/*.root
do

    echo $f
    root -l -n -b -q -e '((TTree*)TFile::Open("'$f'")->Get("events"))->GetEntries("eventNumber == 114960006")'

done
