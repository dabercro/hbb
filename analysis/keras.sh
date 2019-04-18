#! /bin/bash

input=/local/dabercro/files/regression_v2.root

test -f $input || input=/data/t3home000/dabercro/training/regression/v2/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8.root

num=5000

./regression.py \
    --config    regression2.txt \
    --input     $input \
    --target    'jet1_gen_withnu_pt/jet1_pt|jet1_gen_withnu_eta-jet1_eta|TVector2::Phi_mpi_pi(jet1_gen_withnu_phi-jet1_phi)|jet1_gen_withnu_m/jet1_m' \
    --weights   '2|5|5|1' \
    --output    regression_v2_0.pb \
    --epochs    10 \
    --batch     $(echo "$num/4" | bc) \
    --layers    5 \
    --numevents $num \
    --cut       "((n_lep_medium > 1 && n_lep_tight > 0) && (jet1_gen_numB) && (fabs(jet1_gen_withnu_eta-jet1_eta) < 0.05 && fabs(TVector2::Phi_mpi_pi(jet1_gen_withnu_phi-jet1_phi)) < 0.05 && jet1_gen_withnu_m/jet1_m < 2 && jet1_gen_withnu_m/jet1_m > 0.4 && jet1_gen_withnu_pt/jet1_pt < 1.8 && jet1_gen_withnu_pt/jet1_pt > 0.6))" \
    --seed      12348765

# Cut from `crombie2cut /home/dabercro/.crombie2/cuts/db/70ce239bddc877 regression`
