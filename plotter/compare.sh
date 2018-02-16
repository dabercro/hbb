#! /bin/bash

. CrombiePlotterConfig.sh

me_cut='met_trigger == 1 && met_filter == 1 && central_jet1_pt > 55 && central_jet2_pt > 30 && cmva_hbb_pt > 50 && met > 170'
them_cut='trigger&1 && metFilter==1 && jet1Pt > 55 && jet2Pt > 30 && pfmet > 160 && hbbpt > 50 && pfmet>170'

crombie eventdump $CrombieInFilesDir/MET.root me.txt "$me_cut" run_num lumi_num event_num
crombie eventdump /data/t3home000/dhsu/dylansVHSkims/old/MET.root them.txt "$them_cut"

# Get the event numbers and sort them
for b in "me" "them"
do
    perl -ne '/.*\s(\d+)\s\*\s*(\d+)\s\*\s*(\d+)\s\*$/ && print "$1 $2 $3\n"' $b.txt | sort > ${b}_sort.txt
done

diff me_sort.txt them_sort.txt > diff.txt

i_have=$(grep '<' diff.txt | head -n1)
they_have=$(grep '>' diff.txt | head -n1)

for have in "$i_have" "$they_have"
do
    echo $have
    for sample in $(grep '+Run201' ../slimmer/samples.txt)
    do
        file=`dasgoclient -query "file dataset=/$(perl -nae '/([^\+]+)\+([^\+]+)\+([^\+\s]+)/ && print "$1/$2/$3 run=$F[2] lumi=$F[3]\n"' <(echo $sample $have))"`
        if [ "$file" != "" ]
        then
            echo $file
            break
        fi
    done
done
