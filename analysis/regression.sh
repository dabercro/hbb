#! /bin/bash

nevents=20000

for branches in "regression_branches.txt"
do

    suff='_test'

    # For some reasons, BoostType=Grad gives shitty response?

    for training in '!H:!V:BoostType=Bagging:SeparationType=RegressionVariance:nCuts=10000:PruneMethod=CostComplexity:PruneStrength=20:NTrees=200'
#        '!H:V:NCycles=10'
#        '!H:!V:NTrees=400:MinNodeSize=0.2:MaxDepth=40:BoostType=Bagging:SeparationType=RegressionVariance:nCuts=10000:PruneMethod=CostComplexity:PruneStrength=20'
#        '!H:!V:BoostType=Bagging:SeparationType=RegressionVariance:nCuts=10000:PruneMethod=CostComplexity:PruneStrength=20:MaxDepth=15:NTrees=100'
#        '!H:!V:BoostType=Bagging:SeparationType=RegressionVariance:nCuts=10000:PruneMethod=CostComplexity:PruneStrength=20'
    do

#            --target      'cmva_jet1_gen_withnu_pt/cmva_jet1_pt|cmva_jet1_gen_withnu_eta-cmva_jet1_eta|TVector2::Phi_mpi_pi(cmva_jet1_gen_withnu_phi-cmva_jet1_phi)|cmva_jet1_gen_withnu_m/cmva_jet1_m' \
#            --targetname  'ptratio|deta|dphi|mratio' \
#            --weight      'abs(scale_factors)' \
#            --cut         'cmva_jet1_gen && n_lep_loose == 2 && n_lep_tight > 0 && cmva_jet1_pt > 30 && abs(cmva_jet1_gen_pdgid) == 5' \

        crombie tmva \
            --config      $branches \
            --regression  /data/t3home000/dabercro/hbb/breg2/TTTo2L2Nu*.root \
            --weight      '1' \
            --cut         '1' \
            --method      BDT \
            --target      'cmva_jet1_gen_withnu_pt/cmva_jet1_pt' \
            --targetname  'ptratio' \
            --prepare     "nTrain_Regression=${nevents}:nTest_Regression=${nevents}:SplitMode=Random:!V" \
            --methodopt   $training \
            --output      regression${suff}.root \
            --trainername Regression${suff} \
            --traineropt 'V:!Silent:AnalysisType=Regression' \
#            --traineropt '!V:Silent:!DrawProgressBar:!Color:AnalysisType=Regression' \
#            &

    done

done

wait

echo "All done!"

# http://tmva.sourceforge.net/optionRef.html
