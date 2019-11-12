#! /bin/bash

hostname
ls

export USER=dabercro

export SCRAM_ARCH=slc6_amd64_gcc700
export CMSSW_VER=CMSSW_10_2_5_patch1

source /cvmfs/cms.cern.ch/cmsset_default.sh

scram project CMSSW $CMSSW_VER

cd $CMSSW_VER

tar -xvf ../nano.tgz
ls

cd src
eval `scram runtime -sh`
cd ../..

ls

$@ output.root
