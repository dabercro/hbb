#! /bin/sh

if [ -z $V ]
then
    V=0
fi

lumi=56100 nthreads=6 crombieplotter \
    /mnt/hadoop/scratch/dabercro/hbb/181211_013 \
    $HOME/public_html/plots/$(date +%y%m%d)_$V \
    cnfs/files2018.cnf \
    cnfs/plots.cnf \
    cnfs/cuts.cnf
