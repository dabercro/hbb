#! /bin/bash

#for which_mass in "m" "m_reg_3" "m_reg_40"
#do

#        --config      boosted_class.txt \
#        --weight      'scale_factors * XSecWeight' \
#        --output      classify_${which_mass}.root \
#        --trainername "Classifier_${which_mass}" \

    crombie tmva \
        --config      <(sed "s/hbb_m/hbb_m_reg_old/" classify_branches.txt) \
        --signal      /data/t3home000/dabercro/training/Signal.root \
        --background  /data/t3home000/dabercro/training/MC.root \
        --weight      'scale_factors * xsec1' \
        --cut         "eventNumber % 2 == 1" \
        --method      BDT \
        --prepare     "nTrain_Signal=70000:nTrain_Background=40000:SplitMode=Random:NormMode=NumEvents:!V" \
        --methodopt   'BoostType=Grad:nCuts=50:PruneMethod=NoPruning:NTrees=400:MaxDepth=5:MinNodeSize=0.4164:NegWeightTreatment=InverseBoostNegWeights:CreateMVAPdfs=True:UseBaggedBoost:Shrinkage=0.1:NodePurityLimit=0.1:SeparationType=crossentropy' \
        --output      classify_resolved.root \
        --traineropt  '!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification' \
        --trainername "Classifier_resolved" \

#done

# http://tmva.sourceforge.net/optionRef.html
