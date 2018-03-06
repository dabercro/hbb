#! /bin/bash

. CrombiePlotterConfig.sh

#me_cut="$(python cuts.py heavyz)"
#them_cut='metFilter==1 && pfmet>170 && Sum$(jetPt>30 && abs(jetEta)<2.4)<3 && (hbbm>160 || hbbm<60) && hbbpt>120 && (nLooseMuon+nLooseElectron)==0 && jetCMVA[hbbjtidx[0]]>0.9432 && jetCMVA[hbbjtidx[1]]>-0.5884 && max(jetPt[hbbjtidx[0]],jetPt[hbbjtidx[1]])>60 && min(jetPt[hbbjtidx[0]],jetPt[hbbjtidx[1]])>35 && jetPt[hbbjtidx[0]]>60 && jetPt[hbbjtidx[1]]>35 && Sum$(deltaPhi(jetPhi,pfmetphi)<0.5 && jetPt>30 && abs(jetEta)<2.5)==0 && deltaPhi(hbbphi,pfmetphi)>2. && deltaPhi(trkmetphi,pfmetphi)<.5'

#me_cut="$(python cuts.py lightz)"
#them_cut='metFilter==1 && pfmet>170 && Sum$(jetPt>30 && abs(jetEta)<2.4)<4 && max(jetPt[hbbjtidx[0]],jetPt[hbbjtidx[1]])>60 && min(jetPt[hbbjtidx[0]],jetPt[hbbjtidx[1]])>35 && jetPt[hbbjtidx[0]]>60 && jetPt[hbbjtidx[1]]>35 && hbbpt>120 && (nLooseMuon+nLooseElectron)==0 && Sum$(jetCMVA>0.4432)==0 && Sum$(jetCMVA>-0.5884)>1 && jetCMVA[hbbjtidx[0]]<0.4432 && jetCMVA[hbbjtidx[1]]>-0.5884 && deltaPhi(trkmetphi,pfmetphi)<.5 && deltaPhi(hbbphi,pfmetphi)>2. && Sum$(deltaPhi(jetPhi,pfmetphi)<0.5 && jetPt>30 && abs(jetEta)<2.5)==0'

me_cut="$(python cuts.py tt)"
them_cut='metFilter==1 && pfmet>170 && nJet>=4 && Sum$(jetPt>30 && abs(jetEta)<2.4)>=4 && max(jetPt[hbbjtidx[0]],jetPt[hbbjtidx[1]])>60 && min(jetPt[hbbjtidx[0]],jetPt[hbbjtidx[1]])>35 && jetPt[hbbjtidx[0]]>60 && jetPt[hbbjtidx[1]]>35 && deltaPhi(hbbphi,vectorSumPhi(pfmet,pfmetphi,Alt$((nTightMuon==1)*muonPt[0],0)+Alt$((nTightElectron==1)*electronPt[0],0),Alt$((nTightMuon==1)*muonPhi[0],0)+Alt$((nTightElectron==1)*electronPhi[0],0)))>2 && ((Alt$(muonSelBit[0],0)&8) || (Alt$(electronSelBit[0],0)&8)) && hbbpt>120 && Sum$(jetCMVA>0.4432)>0 && Sum$(jetCMVA>-0.5884)>1 && jetCMVA[hbbjtidx[0]]>0.4432 && jetCMVA[hbbjtidx[1]]>-0.5884 && (nTightMuon+nTightElectron)==1 && (nLooseMuon+nLooseElectron)>=1 && Sum$(deltaPhi(jetPhi,pfmetphi)<1.57 && jetPt>30 && abs(jetEta)<2.5)>0 && min(deltaPhi(jetPhi[hbbjtidx[0]],pfmetphi),deltaPhi(jetPhi[hbbjtidx[1]],pfmetphi))<1.57'

crombie eventdump /data/t3home000/dabercro/hbb/180304/MET.root me.txt "$me_cut"
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
    perl -nae 'print "runNumber==$F[1] && lumiNumber==$F[2] && eventNumber==$F[3]\n"' <(echo $have)
    if [ "x$have" != "x" ]
    then
        for sample in $(grep '+Run201' ../slimmer/samples.txt)
        do
            file=`dasgoclient -query "file dataset=/$(perl -nae '/([^\+]+)\+([^\+]+)\+([^\+\s]+)/ && print "$1/$2/$3 run=$F[2] lumi=$F[3]\n"' <(echo $sample $have))"`
            if [ "$file" != "" ]
            then
                echo $file
                localfile=$(perl -ne 'my @parts = (split /\//, $_) ; print "/mnt/hadoop/cms/store/user/paus/pandaf/009/$parts[4]+$parts[3]-$parts[6]+$parts[5]/$parts[-1]"' <(echo $file))
                if [ -f $localfile ]
                then
                    echo "rm hmm.root;" debugevent=\"$(perl -nae 'print "$F[1] $F[2] $F[3]\n"' <(echo $have))\" testhbbtree $localfile hmm.root
                else
                    echo "$localfile does not exist"
                fi
                break
            fi
        done
    fi
done
