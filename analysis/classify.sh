#! /bin/bash

for which_mass in "m" "m_reg_15"
do
    crombie tmva \
        --config      <(sed "s/hbb_m/hbb_${which_mass}/" classify_branches.txt) \
        --signal      /data/t3home000/dabercro/training/Signal.root \
        --background  /data/t3home000/dabercro/training/MC.root \
        --weight      'scale_factors * XSecWeight' \
        --cut         "event_num % 2 == 1 && $(python ../plotter/cuts.py signal)" \
        --method      BDT \
        --prepare     "nTrain_Signal=50000:nTrain_Background=20000:SplitMode=Random:NormMode=NumEvents:!V" \
        --methodopt   'BoostType=AdaBoost:nCuts=50:PruneMethod=NoPruning:NTrees=800:NegWeightTreatment=InverseBoostNegWeights:CreateMVAPdfs=True' \
        --output      classify_${which_mass}.root \
        --traineropt  '!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification' \
        --trainername "Classifier_${which_mass}" \

done

# http://tmva.sourceforge.net/optionRef.html
