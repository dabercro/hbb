#! /bin/bash

nevents=1000000

crombie tmva \
    --config      <(sed 's/1_pt/1_ptSmear/' training_branches.txt) \
    --regression  /data/t3home000/dabercro/hbb/180328/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8.root \
    --weight      'abs(scale_factors)' \
    --cut         'cmva_jet1_gen && n_lep_loose == 2 && n_lep_tight > 0 && cmva_jet1_ptSmear > 30 && abs(cmva_jet1_gen_pdgid) == 5' \
    --target      cmva_jet1_gen_withnu_pt/cmva_jet1_ptSmear \
    --method      BDT \
    --prepare     "nTrain_Regression=${nevents}:nTest_Regression=${nevents}:SplitMode=Random:NormMode=NumEvents:!V" \
    --methodopt   '!H:!V:BoostType=Bagging:SeparationType=RegressionVariance:nCuts=10000:PruneMethod=CostComplexity:PruneStrength=20:MaxDepth=15:NTrees=100' \
    --output      regression_smeared.root \
    --trainername Regression_smeared \
    --traineropt '!V:Silent:!DrawProgressBar:!Color' \

# http://tmva.sourceforge.net/optionRef.html
