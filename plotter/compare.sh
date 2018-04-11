#! /bin/bash

. CrombiePlotterConfig.sh

me_cut='cmva_hbb_m < 500 && met > 170 && met_filter == 1 && cmva_daughter_max_pt > 60 && cmva_daughter_min_pt > 35 && cmva_jet1_pt > 60 && cmva_jet2_cmva > -0.5884 && cmva_hbb_pt_reg_old > 120 && cmva_dphi_uh > 2.0 && n_lep_tight == 1 && n_jet >= 4 && cmva_jet1_cmva > 0.4432 && min(deltaPhi(cmva_jet1_phi, recoilphi), deltaPhi(cmva_jet2_phi, recoilphi)) < 1.57'
them_cut="$(python cuts.py inclusive tt)"

#me_cut='cmva_hbb_m < 500 && met > 170 && met_filter == 1 && cmva_daughter_max_pt > 60 && cmva_daughter_min_pt > 35 && cmva_jet1_pt > 60 && cmva_jet2_cmva > -0.5884 && cmva_hbb_pt_reg_old > 120 && cmva_dphi_uh > 2.0 && n_lep_presel == 0 && n_jet < 3 && cmva_jet1_cmva > 0.9432 && min_dphi_metj_hard > 0.5 && dphi_met_trkmet < 0.5 && (60 > cmva_hbb_m_reg_old || 160 < cmva_hbb_m_reg_old)'
#them_cut="$(python cuts.py inclusive heavyz)"

#me_cut='cmva_hbb_m < 500 && met > 170 && met_filter == 1 && cmva_daughter_max_pt > 60 && cmva_daughter_min_pt > 35 && cmva_jet1_pt > 60 && cmva_jet2_cmva > -0.5884 && cmva_hbb_pt_reg_old > 120 && cmva_dphi_uh > 2.0 && n_lep_presel == 0 && n_jet < 4 && cmva_jet1_cmva < 0.4432 && min_dphi_metj_hard > 0.5 && dphi_met_trkmet < 0.5'
#them_cut="$(python cuts.py inclusive lightz)"

crombie eventdump /data/t3home000/dabercro/hbb/180308/MET.root me.txt "met_trigger && $me_cut"
crombie eventdump /data/t3home000/dabercro/hbb/180406/MET.root them.txt "met_trigger && $them_cut"

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
