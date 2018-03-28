#!/bin/bash

rm */*

test -d dylan || mkdir dylan
cd dylan

wget http://t3serv001.mit.edu/~dhsu/MitVHBBAnalysis/plots/all_orthodox11vars_singleClass/WHSR_WToENu_MVAVar.pdf
wget http://t3serv001.mit.edu/~dhsu/MitVHBBAnalysis/plots/all_orthodox11vars_singleClass/WH2TopCR_WToENu_MVAVar.pdf
wget http://t3serv001.mit.edu/~dhsu/MitVHBBAnalysis/plots/all_orthodox11vars_singleClass/WHLightFlavorCR_WToMuNu_MVAVar.pdf
wget http://t3serv001.mit.edu/~dhsu/MitVHBBAnalysis/plots/all_orthodox11vars_singleClass/WHHeavyFlavorLowMassCR_WToMuNu_MVAVar.pdf
wget http://t3serv001.mit.edu/~dhsu/MitVHBBAnalysis/plots/all_orthodox11vars_singleClass/WHHeavyFlavorHighMassCR_WToENu_MVAVar.pdf
wget http://t3serv001.mit.edu/~dhsu/MitVHBBAnalysis/plots/boosted_13vars_noECFs_singleClass/WHHeavyFlavorFJCR_WToMuNu_MVAVar.pdf
wget http://t3serv001.mit.edu/~dhsu/MitVHBBAnalysis/plots/boosted_13vars_noECFs_singleClass/WHTT1bFJCR_WToMuNu_MVAVar.pdf
wget http://t3serv001.mit.edu/~dhsu/MitVHBBAnalysis/plots/boosted_13vars_noECFs_singleClass/WHLightFlavorFJCR_WToENu_MVAVar.pdf
wget http://t3serv001.mit.edu/~dhsu/MitVHBBAnalysis/plots/boosted_13vars_noECFs_singleClass/WHTT2bFJCR_WToMuNu_MVAVar.pdf
wget http://t3serv001.mit.edu/~dhsu/MitVHBBAnalysis/plots/boosted_13vars_noECFs_singleClass/WHTT2bFJCR_WToMuNu_MVAVar.pdf
wget http://t3serv001.mit.edu/~dhsu/mva/whbb/plots/overtrain_BDT_optimizeBoostedAda_NTrees200_MaxDepth3_AdaBoostBeta0.12_nCuts100.png
wget http://t3serv001.mit.edu/~dhsu/MitVHBBAnalysis/plots/boosted_13vars_noECFs_singleClass/WHLightFlavorFJCR_WToMuNu_fj1Tau21.pdf
wget http://t3serv001.mit.edu/~dhsu/MitVHBBAnalysis/plots/boosted_13vars_noECFs_singleClass/WHFJSR_WToMuNu_MVAVar.pdf
wget http://t3serv001.mit.edu/~dhsu/mva/whbb/plots/CorrelationMatrixS_optimizeBoostedAda.png
wget http://t3serv001.mit.edu/~dhsu/MitVHBBAnalysis/plots/boosted_13vars_noECFs_singleClass/WHHeavyFlavorFJCR_WToMuNu_minSubjetCSV.pdf
wget http://t3serv001.mit.edu/~dhsu/MitVHBBAnalysis/whBoostedStrategyDiagram.pdf

mv overtrain*.png over.png

cd -

test -d bm || mkdir bm
cd bm

wget http://t3serv001.mit.edu/~bmaier/figs/hbb/zhnn/v7/resolved/cr_ttbar/cr_ttbar_jetCMVA.pdf
wget http://t3serv001.mit.edu/~bmaier/figs/hbb/zhnn/v7/resolved/cr_ttbar/cr_ttbar_CMVA2_postfit.pdf
wget http://t3serv001.mit.edu/~bmaier/figs/hbb/zhnn/v7/resolved/sr/sr_BDT.pdf

cd -

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180209_v1%2Fregression_withgen%2C180209_v1%2Fregression"

crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180327_v1%2Finclusive_signal_event_class_reg_3%2C180327_yesterday_post%2Finclusive_signal_event_class_reg_3"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180327_inc%2Finclusive_tt_cmva_jet2_cmva%2C180327_post%2Finclusive_tt_cmva_jet2_cmva%2C180327_inc%2Finclusive_lightz_cmva_jet2_cmva%2C180327_post%2Finclusive_lightz_cmva_jet2_cmva%2C180327_inc%2Finclusive_heavyz_cmva_jet2_cmva%2C180327_post%2Finclusive_heavyz_cmva_jet2_cmva"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180326_v3%2Fboosted_signal_fatjet1_mSD_corr%2C180327_yesterday_post%2Fboosted_signal_fatjet1_mSD_corr%2C180326_v3%2Fboosted_tt_fatjet1_mSD_corr%2C180327_yesterday_post%2Fboosted_tt_fatjet1_mSD_corr%2C180326_v3%2Fboosted_lightz_fatjet1_mSD_corr%2C180327_yesterday_post%2Fboosted_lightz_fatjet1_mSD_corr%2C180326_v3%2Fboosted_heavyz_fatjet1_mSD_corr%2C180327_yesterday_post%2Fboosted_heavyz_fatjet1_mSD_corr"
crombie downloadtar "http://t3serv001.mit.edu/~dabercro/plotviewer/returntar.php?only=pdf&files=180327_v4%2Fboosted_signal_fatjet1_mSD_corr%2C180327_v4%2Fboosted_tt_fatjet1_mSD_corr%2C180327_v4%2Fboosted_lightz_fatjet1_mSD_corr%2C180327_v4%2Fboosted_heavyz_fatjet1_mSD_corr"

for f in 1*/*.pdf
do
    pdfcrop $f $f
done
