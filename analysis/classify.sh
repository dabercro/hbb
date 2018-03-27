#! /bin/bash

#for which_mass in "m" "m_reg_3" "m_reg_40"
#do

#        --config      <(sed "s/hbb_m/hbb_${which_mass}/" classify_branches.txt) \
#        --weight      'scale_factors * XSecWeight' \
#        --output      classify_${which_mass}.root \
#        --trainername "Classifier_${which_mass}" \

    crombie tmva \
        --config      boosted_class.txt \
        --signal      /data/t3home000/dabercro/training/Signal.root \
        --background  /data/t3home000/dabercro/training/MC.root \
        --weight      'scale_factors * xsec1' \
        --cut         "eventNumber % 2 == 1" \
        --method      BDT \
        --prepare     "nTrain_Signal=77500:nTrain_Background=48500:SplitMode=Random:NormMode=NumEvents:!V" \
        --methodopt   'BoostType=AdaBoost:nCuts=50:PruneMethod=NoPruning:NTrees=800:NegWeightTreatment=InverseBoostNegWeights:CreateMVAPdfs=True' \
        --output      classify_wboost.root \
        --traineropt  '!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification' \
        --trainername "Classifier_wboost" \

#done

# http://tmva.sourceforge.net/optionRef.html
