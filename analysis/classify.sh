#! /bin/bash

crombie tmva \
    --config      classify_branches.txt \
    --signal      /data/t3home000/dabercro/training/Signal.root \
    --background  /data/t3home000/dabercro/training/MC.root \
    --weight      'scale_factors * XSecWeight' \
    --cut         "event_num % 2 == 1 && $(python ../plotter/cuts.py classify)" \
    --method      BDT \
    --prepare     "nTrain_Signal=150000:nTrain_Background=500000:SplitMode=Random:NormMode=NumEvents:!V" \
    --methodopt   'BoostType=AdaBoost:nCuts=50:PruneMethod=NoPruning:NTrees=800:NegWeightTreatment=InverseBoostNegWeights:CreateMVAPdfs=True' \
    --output      classify.root \
    --traineropt  '!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification' \
    --trainername 'Classifier' \

# http://tmva.sourceforge.net/optionRef.html
