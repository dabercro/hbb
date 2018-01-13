#! /bin/bash

nevents=50000

crombie tmva \
    --config     training_branches.txt \
    --input      /data/t3home000/dabercro/hbb/180109/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8.root \
    --weight     'abs(scale_factors)' \
    --cut        'cmva_jet1_gen && n_lep_loose == 2 && n_lep_tight > 0 && cmva_jet1_pt > 30 && abs(cmva_jet1_gen_pdgid) == 5' \
    --target     cmva_jet1_gen_pt/cmva_jet1_pt \
    --method     BDT \
    --prepare    "nTrain_Regression=${nevents}:nTest_Regression=${nevents}:SplitMode=Random:NormMode=NumEvents:!V" \
    --methodopt  'BoostType=Bagging:SeparationType=RegressionVariance:nCuts=20:PruneMethod=CostComplexity:PruneStrength=20' \

# http://tmva.sourceforge.net/optionRef.html
