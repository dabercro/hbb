#! /bin/bash

IFS=$'\n'

to_download=$1
shift

if [ -f $to_download ]
then

    echo $to_download exists already
    exit 1

fi

server=root://t3se01.psi.ch:1095

Zvv_2017=/store/user/krgedia/VHbb/VHbbPostNano2018/V5/Zvv/eval/17jan20v3
Zvv_2018=/store/user/krgedia/VHbb/VHbbPostNano2018/V5/Zvv/eval/3mar20v5
Wlv_2018=/store/user/krgedia/VHbb/VHbbPostNano2018/V12/Wlv/mva/3mar20v2
Zll_2018=/store/user/krgedia/VHbb/Zll/VHbbPostNano2018/mva/2may20v1

store_dir=/mnt/hadoop/scratch/dabercro/Xbb

store () {
    year=$1
    shift

    while (($#))
    do

        out_dir=$year/$1
        shift
        in_dir=$1
        shift

        sample_dirs=$(xrdfs $server ls $in_dir | grep -v '.root')

        for sample_dir in $sample_dirs
        do

            echo Reading $sample_dir

            test -d $store_dir/$out_dir/$(basename $sample_dir) || mkdir -p -v $store_dir/$out_dir/$(basename $sample_dir)

            files=$(xrdfs $server ls -l $sample_dir | grep '.root' | sed 's/ \+/ /g')

            for f in $files
            do

                name=$(basename $(echo $f | cut -d ' ' -f 5))
                size=$(echo $f | cut -d ' ' -f 4)

                out_file=$store_dir/$out_dir/$(basename $sample_dir)/$name

                if [ -f $out_file ]
                then

                    if [ $(ls -l $out_file | sed 's/ \+/ /g' | cut -d ' ' -f 5) -eq $size ]
                    then

                        continue

                    fi

                fi

                test ! -f $out_file || rm $out_file

                echo $server/$(echo $f | cut -d ' ' -f 5) $out_file >> $to_download

            done

        done

    done
    
}

#store 2017 Zvv $Zvv_2017
store 2018 Zvv $Zvv_2018 Wlv $Wlv_2018 Zll $Zll_2018
