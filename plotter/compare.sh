#! /bin/bash

. CrombiePlotterConfig.sh

me_cut="$(python cuts.py inclusive signal)"
them_cut='metFilter==1 && pfmet>170 && Sum$(jotPt>30 && abs(jotEta)<2.5)<4 && hbbm_reg<500 && max(jotPt[hbbjtidx[0]],jotPt[hbbjtidx[1]])>60 && min(jotPt[hbbjtidx[0]],jotPt[hbbjtidx[1]])>35 && (hbbm_reg<160 && hbbm_reg>60) && hbbpt_reg>120 && (nLooseMuon+nLooseElectron)==0 && Sum$(jotCSV>0.8001)>0  &&  Sum$(jotCSV>0.1522)>1 && jotCSV[hbbjtidx[0]]>0.8001 && jotCSV[hbbjtidx[1]]>0.1522 && deltaPhi(trkmetphi,pfmetphi)<.5 && deltaPhi(hbbphi,pfmetphi)>2. && Sum$(jotPt>30 && abs(jotEta)<2.5 && deltaPhi(jotPhi,pfmetphi)<0.5)==0 && (trigger&1)!=0'


#them_cut='(Vtype==4 && (HCMVAV2_reg_mass>60&&HCMVAV2_reg_mass<160) && Sum$(abs(TVector2::Phi_mpi_pi(Jet_phi-V_phi))<0.5 && Jet_pt>30 && Jet_puId>=4)==0 && jetCSV[hJCMVAV2idx[0]]>0.9432 && nselLeptons==0 && Jet_pt[hJCMVAV2idx[0]]>60 && abs(TVector2::Phi_mpi_pi(V_phi-tkMet_phi))<0.5 && Sum$(Jet_pt>30 && abs(Jet_eta)<2.4 && Jet_puId>=4)<4 && abs(TVector2::Phi_mpi_pi(HCMVAV2_reg_phi-V_phi))>2) && met_pt > 170 && (HLT_BIT_HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v + HLT_BIT_HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v + HLT_BIT_HLT_PFMET170_NoiseCleaned_v + HLT_BIT_HLT_PFMET170_HBHECleaned_v + HLT_BIT_HLT_PFMET170_HBHE_BeamHaloCleaned_v) > 0 && Jet_pt[hJCMVAV2idx[1]]>35 && HCMVAV2_reg_mass < 500 && jetCSV[hJCMVAV2idx[1]] > -0.5884 && HCMVAV2_reg_pt > 120'

#me_cut="$(python cuts.py inclusive heavyz)"
#them_cut='(metFilter==1) && (pfmet>170) && (Sum$(jetPt>30 && abs(jetEta)<2.4)<3) && (hbbm_reg>160 || hbbm_reg<60) && (hbbm_reg<500) && (hbbpt_reg>120) && ((nLooseMuon+nLooseElectron)==0) && (jetCMVA[hbbjtidx[0]]>0.9432) && (jetCMVA[hbbjtidx[1]]>-0.5884) && (max(jetPt[hbbjtidx[0]],jetPt[hbbjtidx[1]])>60) && (min(jetPt[hbbjtidx[0]],jetPt[hbbjtidx[1]])>35) && (jetPt[hbbjtidx[0]]>60) && (jetPt[hbbjtidx[1]]>35) && (deltaPhi(hbbphi,pfmetphi)>2.) && (deltaPhi(trkmetphi,pfmetphi)<.5) && (Sum$(deltaPhi(jetPhi,pfmetphi)<0.5 && jetPt>30 && abs(jetEta)<2.5)==0) && (trigger&1)!=0'
#them_cut='(Jet_pt_reg[hJCMVAV2idx[0]] > 60 & Jet_pt_reg[hJCMVAV2idx[1]] > 35 & HCMVAV2_reg_pt > 120 & met_pt > 170) && (Vtype==4 && (HCMVAV2_reg_mass<60||HCMVAV2_reg_mass>160) && Sum$(abs(TVector2::Phi_mpi_pi(Jet_phi-V_phi))<0.5 && Jet_pt>30 && Jet_puId>=4)==0 && Jet_btagCMVAV2[hJCMVAV2idx[0]]>0.9432 && nselLeptons==0 && Jet_pt[hJCMVAV2idx[0]]>60 && abs(TVector2::Phi_mpi_pi(V_phi-tkMet_phi))<0.5 && Sum$(Jet_pt>30 && abs(Jet_eta)<2.4 && Jet_puId>=4)<3 && abs(TVector2::Phi_mpi_pi(HCMVAV2_reg_phi-V_phi))>2) && met_pt > 170 && (HLT_BIT_HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v + HLT_BIT_HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v + HLT_BIT_HLT_PFMET170_NoiseCleaned_v + HLT_BIT_HLT_PFMET170_HBHECleaned_v + HLT_BIT_HLT_PFMET170_HBHE_BeamHaloCleaned_v) > 0 && Jet_pt[hJCMVAV2idx[1]]>35 && HCMVAV2_reg_mass < 500 && Jet_btagCMVAV2[hJCMVAV2idx[1]] > -0.5884 && HCMVAV2_reg_pt > 120'

#me_cut="$(python cuts.py inclusive lightz)"
#them_cut='(Vtype==4 && Sum$(abs(TVector2::Phi_mpi_pi(Jet_phi-V_phi))<0.5 && Jet_pt>30 && Jet_puId>=4)==0 && Jet_btagCMVAV2[hJCMVAV2idx[0]]<0.4432 && abs(TVector2::Phi_mpi_pi(V_phi-tkMet_phi))<0.5 && nselLeptons==0 && Jet_pt[hJCMVAV2idx[0]]>60 && Sum$(Jet_pt>30 && abs(Jet_eta)<2.4 && Jet_puId>=4)<4 && abs(TVector2::Phi_mpi_pi(HCMVAV2_reg_phi-V_phi))>2) && met_pt > 170 && (HLT_BIT_HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v + HLT_BIT_HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v + HLT_BIT_HLT_PFMET170_NoiseCleaned_v + HLT_BIT_HLT_PFMET170_HBHECleaned_v + HLT_BIT_HLT_PFMET170_HBHE_BeamHaloCleaned_v) > 0 && Jet_pt[hJCMVAV2idx[1]]>35 && HCMVAV2_reg_mass < 500 && Jet_btagCMVAV2[hJCMVAV2idx[1]] > -0.5884 && HCMVAV2_reg_pt > 120'

#me_cut="$(python cuts.py inclusive tt)"
#them_cut='((Vtype==2||Vtype==3) && Min$(abs(TVector2::Phi_mpi_pi(Jet_phi[hJCMVAV2idx]-V_phi)))<1.57 && nselLeptons>=1 && Jet_btagCMVAV2[hJCMVAV2idx[0]]>0.4432 && Jet_pt[hJCMVAV2idx[0]]>60 && Sum$(Jet_pt>30 && abs(Jet_eta)<2.4 && Jet_puId>=4)>=4 && abs(TVector2::Phi_mpi_pi(HCMVAV2_reg_phi-V_phi))>2) && met_pt > 170 && (HLT_BIT_HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v + HLT_BIT_HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v + HLT_BIT_HLT_PFMET170_NoiseCleaned_v + HLT_BIT_HLT_PFMET170_HBHECleaned_v + HLT_BIT_HLT_PFMET170_HBHE_BeamHaloCleaned_v) > 0 && Jet_pt[hJCMVAV2idx[1]]>35 && HCMVAV2_reg_mass < 500 && Jet_btagCMVAV2[hJCMVAV2idx[1]] > -0.5884 && HCMVAV2_reg_pt > 120'


crombie eventdump /data/t3home000/dabercro/hbb/compare/deep/2017.root me.txt "met_trigger == 1 && $me_cut"
#crombie eventdump $CrombieInFilesDir/../180520_v2/MET.root them.txt "met_trigger == 1 && $me_cut"
#tree=tree crombie eventdump $themfile them.txt "$them_cut && json != 0" run lumi evt
crombie eventdump /data/t3home000/hbb/zhnn/v9/sr/MET.root them.txt "$them_cut"

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
        for sample in $(grep '+Run201' ../slimmer/data2017.txt)
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
