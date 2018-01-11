#! /bin/bash

nevents=500000

crombie tmva \
    --config     training_branches.txt \
    --input      /data/t3home000/dabercro/hbb/180109/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8.root \
    --weight     'abs(scale_factors)' \
    --cut        'cmva_jet1_gen && n_lep_loose == 2 && n_lep_tight > 0' \
    --target     cmva_jet1_gen_pt/cmva_jet1_pt \
    --method     BDT \
    --methodname BDTG \
    --prepare    "nTrain_Regression=${nevents}:nTest_Regression=${nevents}:SplitMode=Random:NormMode=NumEvents:!V" \
    --methodopt  '!H:!V:NTrees=2000::BoostType=Grad:Shrinkage=0.1:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=4' \


# !H:!V:NTrees=100:MinNodeSize=1.0%:BoostType=AdaBoostR2:SeparationType=RegressionVariance:nCuts=20:PruneMethod=CostComplexity:PruneStrength=30
