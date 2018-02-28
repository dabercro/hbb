#! /bin/bash

nevents=1000000

for branches in "training_branches.txt" "old_branches.txt"
do

    if grep "old" <(echo $branches) >& /dev/null
    then
        suff='_old'
    else
        suff=''
    fi

    # For some reasons, BoostType=Grad gives shitty response?

#    for training in '!H:!V:NTrees=400:MinNodeSize=0.2:MaxDepth=40:BoostType=Bagging:SeparationType=RegressionVariance:nCuts=20:PruneMethod=CostComplexity:PruneStrength=20' \
#        '!H:!V:BoostType=Bagging:SeparationType=RegressionVariance:nCuts=10000:PruneMethod=CostComplexity:PruneStrength=20:MaxDepth=15:NTrees=100' \
#        '!H:!V:BoostType=Bagging:SeparationType=RegressionVariance:nCuts=20:PruneMethod=CostComplexity:PruneStrength=20'
    for training in '!H:!V:BoostType=Bagging:SeparationType=RegressionVariance:nCuts=10000:PruneMethod=CostComplexity:PruneStrength=20:MaxDepth=15:NTrees=100'
    do

        depth=$(echo "$training" | perl -ne 'if (/MaxDepth=(\d*)/) { print "$1\n"; } else { print "3\n"; }')

        # Benedikt's depth=40

        crombie tmva \
            --config      $branches \
            --regression  /data/t3home000/dabercro/hbb/breg/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8.root \
            --weight      'abs(scale_factors)' \
            --cut         'cmva_jet1_gen && n_lep_loose == 2 && n_lep_tight > 0 && cmva_jet1_pt > 30 && abs(cmva_jet1_gen_pdgid) == 5' \
            --target      cmva_jet1_gen_withnu_pt/cmva_jet1_pt \
            --method      BDT \
            --prepare     "nTrain_Regression=${nevents}:nTest_Regression=${nevents}:SplitMode=Random:NormMode=NumEvents:!V" \
            --methodopt   $training \
            --output      regression${suff}_${depth}.root \
            --trainername Regression${suff}_${depth} \
            --traineropt '!V:Silent:!DrawProgressBar:!Color' \
            &

    done

done

wait

echo "All done!"

# http://tmva.sourceforge.net/optionRef.html
