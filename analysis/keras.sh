#! /bin/bash

crombie kerasreg \
    --config    regression_branches.txt \
    --input     /data/t3home000/dabercro/hbb/breg_v3/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8.root \
    --target    'cmva_jet1_gen_withnu_pt/cmva_jet1_pt|cmva_jet1_gen_withnu_eta-cmva_jet1_eta|TVector2::Phi_mpi_pi(cmva_jet1_gen_withnu_phi-cmva_jet1_phi)|cmva_jet1_gen_withnu_m/cmva_jet1_m' \
    --cut       'fabs(cmva_jet1_gen_withnu_eta-cmva_jet1_eta) < 0.05 && fabs(TVector2::Phi_mpi_pi(cmva_jet1_gen_withnu_phi-cmva_jet1_phi)) < 0.05' \
    --output    output_quick.pb \
    --epochs    300 \
    --batch     10000 \
    --layers    10 \
    --weights   '2|5|5|1' \
    --numevents 25000
