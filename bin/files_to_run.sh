#! /bin/bash

directory=$1
shift

if [ "x$directory" = "x" ]
then

    echo "Directory argument empty"
    exit 1

fi

server=xrootd.cmsaf.mit.edu


recursive_list () {

    outtxt=$1
    shift
    dirtolist=$1
    shift

    echo $dirtolist

    contents=$(xrdfs $server ls $dirtolist | sort -u)

    for rootfile in $(echo $contents | grep '.root')
    do

        if [ "$(basename $rootfile)" != "log" ]
        then

            echo $rootfile >> $outtxt

        fi

    done

    for subdir in $(echo $contents | grep -v '.root')
    do

        recursive_list $outtxt $subdir

    done

}


for sampledir in $(xrdfs $server ls $directory | grep -v '.root')
do

    sample=$(echo $sampledir | perl -ne 'm|/V13/([^/]+)| && print $1')
    recursive_list $sample.txt $sampledir

done
