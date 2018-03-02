#! /bin/bash

. CrombiePlotterConfig.sh

me_cut=$(python cuts.py heavyz)
them_cut='metFilter==1 && pfmet>170 && hbbm<500 && Sum$(jetPt>30 && abs(jetEta)<2.4)<3 && (hbbm>160 || hbbm<60) && hbbpt>120 && (nLooseMuon+nLooseElectron)==0 && jetCMVA[hbbjtidx[0]]>0.9432 && jetCMVA[hbbjtidx[1]]>-0.5884 && max(jetPt[hbbjtidx[0]],jetPt[hbbjtidx[1]])>60 && min(jetPt[hbbjtidx[0]],jetPt[hbbjtidx[1]])>35 && jetPt[hbbjtidx[0]]>60 && jetPt[hbbjtidx[1]]>35 && Sum$(deltaPhi(jetPhi,pfmetphi)<0.5 && jetPt>30 && abs(jetEta)<2.5)==0 && deltaPhi(hbbphi,pfmetphi)>2. && deltaPhi(trkmetphi,pfmetphi)<.5'

crombie eventdump $CrombieInFilesDir/MET.root me.txt "$me_cut"
crombie eventdump /mnt/hadoop/scratch/dhsu/dylansVHSkims/MET.root them.txt "$them_cut"

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
