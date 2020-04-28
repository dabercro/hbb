#! /bin/bash

year=$1
sample=$2

perl -nE 's|/||, /^(\S+)\s\&\s([\d\.]+)/ && say "$1 $2"' /home/dabercro/AN-19-229/Tables/samples_$year.tex | xargs -n2 echo | grep $sample | cut -d ' ' -f 2
