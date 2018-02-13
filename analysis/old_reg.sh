#! /bin/bash

nevents=2000000

crombie tmva \
    --config      old_branches.txt \
    --regression  /data/t3home000/dabercro/hbb/old_reg/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8.root \
    --weight      'abs(scale_factors)' \
    --cut         'cmva_hbb && abs(cmva_jet1_gen_pdgid) == 5' \
    --target      cmva_jet1_gen_withnu_pt/cmva_jet1_pt \
    --method      BDT \
    --prepare     "nTrain_Regression=${nevents}:nTest_Regression=${nevents}:SplitMode=Random:NormMode=NumEvents:!V" \
    --methodopt   '!H:V:NTrees=400:MinNodeSize=0.2:MaxDepth=40:BoostType=Bagging:SeparationType=RegressionVariance:nCuts=20:PruneMethod=CostComplexity:PruneStrength=20' \
    --output      regression_old_method.root \
    --trainername Regression_old_method \

# http://tmva.sourceforge.net/optionRef.html
