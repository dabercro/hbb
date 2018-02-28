#! /bin/bash

for which_mass in "m" "m_reg_3" "m_reg_40"
do
    crombie tmva \
        --config      <(sed "s/hbb_m/hbb_${which_mass}/" classify_branches.txt) \
        --signal      /data/t3home000/dabercro/training/Signal.root \
        --background  /data/t3home000/dabercro/training/MC.root \
        --weight      'scale_factors * XSecWeight' \
        --cut         "eventNumber % 2 == 1" \
        --method      BDT \
        --prepare     "nTrain_Signal=77500:nTrain_Background=48500:SplitMode=Random:NormMode=NumEvents:!V" \
        --methodopt   'BoostType=AdaBoost:nCuts=50:PruneMethod=NoPruning:NTrees=800:NegWeightTreatment=InverseBoostNegWeights:CreateMVAPdfs=True' \
        --output      classify_${which_mass}.root \
        --traineropt  '!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification' \
        --trainername "Classifier_${which_mass}" \

done

# http://tmva.sourceforge.net/optionRef.html
