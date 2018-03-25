#! /bin/bash

plotsdir=datacards/plots
plotsfile=plots_dat.root

if [ ! -f $plotsdir/$plotsfile -o "x$(find $plotsdir -newer $plotsdir/$plotsfile -type f -name '*.root')" != "x" ]
then
    test -f $plotsdir/$plotsfile && rm $plotsdir/$plotsfile
    hadd $plotsdir/$plotsfile $plotsdir/*.root
fi
