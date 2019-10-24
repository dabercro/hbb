#! /bin/bash

indir=$1
outdir=/data/t3home000/dabercro/training/classify/$(date '+%y%m%d')

for sample in `cat samples.txt`
do

    crombie convert \
        --input $indir/$sample \
        --output  $outdir/$sample \
        --cut '((((hbb_m < 500 && (daughter_max_pt > 60 && daughter_min_pt > 35) && jet2_btagloose && dphi_uh > 2. && hbb_pt_190904_0 > 120 && (recoil > 170 && met_filter == 1)) && n_lep_preselected == 0 && min_dphi_metj_hard > 0.5) && jet1_btagtight == 1) && (60 < hbb_m_190904_0 && 160 > hbb_m_190904_0))' \
        --nprocs 16 \
        --branchfiles class_targets.txt classify3.txt class_decorrelate.txt

done
