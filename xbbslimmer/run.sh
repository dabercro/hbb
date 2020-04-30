#! /bin/bash

hostname
ls

export USER=dabercro

export SCRAM_ARCH=slc6_amd64_gcc700
BASE=/home/$USER/cms/cmssw/CMSSW_10_2_5_patch1

source /cvmfs/cms.cern.ch/cmsset_default.sh

cd $BASE/src
eval `scram runtime -sh`
cd -

export CROMBIEPATH=/home/$USER/CrombieTools
export PYTHONPATH=$CROMBIEPATH/python:$PYTHONPATH
export PATH=$CROMBIEPATH/bin:$PATH

mkdir inputs

ls

count=0

sample=$(perl -ne '/(Zvv|Zll|Wlv)/ && print $1' <(echo $1))

while [ $# -ne 0 ]
do

    infile=$1
    shift

    ln -s $infile inputs/$count.root
    count=$((count + 1))

done

mkdir outputs

cut="1"

if [ $sample = "Zvv" ]
then

    cut="((V_pt > 250 && Hbb_fjidx > -1) || (hJidx[1] > -1 && H_pt > 80 && V_pt > 120 && Jet_pt[hJidx[0]] * Jet_bRegCorr[hJidx[0]] > 25 && Jet_pt[hJidx[1]] * Jet_bRegCorr[hJidx[1]] > 25))"

fi

if [ $sample = "Wlv" ]
then

    cut="lepMetDPhi < 2.0 && nAddLep15_2p5 == 0 && ((V_pt > 250 && Hbb_fjidx > -1) || (hJidx[1] > -1 && H_pt > 80 && V_pt > 120 && (Jet_pt[hJidx[0]] * Jet_bRegCorr[hJidx[0]]) > 25 && (Jet_pt[hJidx[1]] * Jet_bRegCorr[hJidx[1]]) > 25))"

fi

crombie skim -c "$cut" -i inputs -o outputs -t Events -r run -l luminosityBlock -e event --copy Runs --disable drop.txt

hadd -n 2 output.root outputs/*.root
