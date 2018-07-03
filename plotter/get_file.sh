#! /bin/bash

dassearch=$(perl -ne '/(\d+)[^\d]+(\d+)[^\d]+(\d+)/; print "run=$1 lumi=$2"' <(echo "$1"))
debugsearch=$(perl -ne '/(\d+)[^\d]+(\d+)[^\d]+(\d+)/; print "$1 $2 $3"' <(echo "$1"))

echo $dassearch
echo $debugsearch

#for sample in $(grep '+Run201' ../slimmer/samples.txt)
for sample in $(grep '+Run201' ../slimmer/data2017.txt)
do
    echo dasgoclient -query "file dataset=/$(perl -ne '/([^\+]+)\+([^\+]+)\+([^\+\s]+)/ && print "$1/$2/$3"' <(echo $sample)) $dassearch"
    file=`dasgoclient -query "file dataset=/$(perl -ne '/([^\+]+)\+([^\+]+)\+([^\+\s]+)/ && print "$1/$2/$3"' <(echo $sample)) $dassearch"`
    if [ "$file" != "" ]
    then
        echo $file
        localfile=$(perl -ne 'my @parts = (split /\//, $_) ; print "/mnt/hadoop/cms/store/user/paus/pandaf/009/$parts[4]+$parts[3]-$parts[6]+$parts[5]/$parts[-1]"' <(echo $file))
        if [ -f $localfile ]
        then
            echo "rm hmm.root;" input_dir=$localfile debugevent=\"$debugsearch\" testhbbtree $localfile hmm.root
        else
            echo "$localfile does not exist"
        fi
        break
    fi
done
