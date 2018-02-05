#! /bin/bash

nevents=250000

crombie tmva \
    --config      classify_branches.txt \
    --signal      /data/t3home000/dabercro/training/Signal.root \
    --background  /data/t3home000/dabercro/training/MC.root \
    --weight      'abs(scale_factors * XSecWeight)' \
    --cut         'event_num % 2 == 1' \
    --method      BDT \
    --prepare     "nTrain_Signal=${nevents}:nTrain_Background=${nevents}:SplitMode=Random:NormMode=NumEvents:!V" \
    --methodopt   'BoostType=Bagging:nCuts=20:PruneMethod=CostComplexity:PruneStrength=20' \
    --output      classify.root \
    --traineropt  '!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification' \
    --trainername 'Classifier' \

# http://tmva.sourceforge.net/optionRef.html
