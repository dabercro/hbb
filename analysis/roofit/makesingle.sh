#! /bin/bash

. CrombieAnalysisConfig.sh

filesdir=${CrombieInFilesDir:?} || exit 1

cd $filesdir || exit 2

rm *.root

for d in single skimmed
do
    test ! -d $d || rm -r $d
done

ls --color=none | xargs -n1 -P${CrombieNLocalProcs:-1} -I {} bash -c 'hadd -f {}.root {}/*.root'

cd -

./addxs.py

for weight in {0..8}
do

    ./addxs.py scale_weight_$weight

done


for weight in {0..3}
do

    ./addxs.py ps_weight_$weight

done

cd -

mkdir single || exit 3

hadd -f mc.root DYJets*.root TT*.root
hadd -f data.root DoubleMuon.root *EG*.root

mv -v *.root single

mv -v single/data.root .

crombie skim -n 1 -i . -o single --copy htotal -r run -l luminosityBlock -e event \
    -d --disable /home/dabercro/hbb/analysis/roofit/ps_corrected.txt

mv -v single/data.root single/mc.root .

crombie skim -n ${CrombieNLocalProcs:-1} -i . -o skimmed --copy htotal -r run -l luminosityBlock -e event \
    -c 'dilep_pt > 50 && jet2 && num_bjet == 2'

mv -v *.root single

cd -
