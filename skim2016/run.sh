#! /bin/bash

hostname
ls

export USER=dabercro

export SCRAM_ARCH=slc6_amd64_gcc630
BASE=/home/$USER/cms/cmssw/CMSSW_9_4_5

source /cvmfs/cms.cern.ch/cmsset_default.sh

cd $BASE/src
eval `scram runtime -sh`
cd -

export CROMBIEPATH=/home/$USER/CrombieTools
export PYTHONPATH=$CROMBIEPATH/python:$PYTHONPATH
export PATH=$CROMBIEPATH/bin:$PATH

tar -xvf src.tar.gz
cp src/* .

mkdir inputs

ls

count=0

while [ $# -ne 0 ]
do

    infile=$1
    shift

    xrdcp -s root://cmsxrootd-site1.fnal.gov:1092/$infile inputs/$count.root
    count=$((count + 1))

done

mkdir outputs

crombie skim -c "$(./cuts.py)" -i inputs -o outputs -t tree -r run -l lumi -e evt

hadd -n 2 output.root outputs/*.root
