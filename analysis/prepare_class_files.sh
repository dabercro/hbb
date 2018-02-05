#! /bin/bash

source ../plotter/CrombiePlotterConfig.sh

cut=$(python ../plotter/cuts.py common)

for conf in "MC" "Signal"
do
    test -d links && rm -rf links
    mkdir links

    trainingdir=/data/t3home000/$USER/training
    if [ -d $trainingdir ]
    then
        rm $trainingdir/[0-9]*.root
    else
        mkdir $trainingdir
    fi

    count=0

    for f in $(perl -ne '/^[^#].*\s([^\s]*\.root)/ && print "$1\n"' ../plotter/${conf}Config.txt)
    do
        ln -s $CrombieInFilesDir/$f links/$count.root
        count=$((count + 1))
    done
    crombie skim  --cut "$cut" --tree 'events' --run 'run_num' --lumi 'lumi_num' --event 'event_num' --freq 100000 --numproc $CrombieNLocalProcs --indir links --outdir $trainingdir

    hadd -f $trainingdir/$conf.root $trainingdir/[0-9]*.root
done