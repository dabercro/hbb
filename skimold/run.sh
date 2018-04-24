#! /bin/bash

ls

export SCRAM_ARCH=slc6_amd64_gcc630
BASE=/home/$USER/cms/cmssw/CMSSW_8_0_29

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

while [ $# -ne 0 ]
do

    infile=$1
    shift

    xrdcp root://cmsxrootd-site1.fnal.gov:1092/$infile inputs

done

mkdir outputs

crombie skim -c "$(./cuts.py)" -i inputs -o outputs

hadd output.root outputs/*.root
