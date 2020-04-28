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

while [ $# -ne 0 ]
do

    infile=$1
    shift

    ln -s $infile inputs/$count.root
    count=$((count + 1))

done

mkdir outputs

crombie skim -c "V_pt > 200 && H_mass < 500 && H_pt > 50" -i inputs -o outputs -t Events -r run -l luminosityBlock -e event --keep keep.txt --copy Runs

hadd -n 2 output.root outputs/*.root
