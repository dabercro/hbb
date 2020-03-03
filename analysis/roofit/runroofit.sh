#! /bin/bash

export SCRAM_ARCH=slc6_amd64_gcc700
export CMSSW_VER=CMSSW_10_2_5_patch1

source /cvmfs/cms.cern.ch/cmsset_default.sh

scram project CMSSW $CMSSW_VER

cd $CMSSW_VER/src
eval `scram runtime -sh`
cd -

outdir=$(date '+%y%m%d')

test -d plots || mkdir plots

tar -xf roofit.tgz

./roofit.py sum $1

test -d $HOME/public_html/plots/$outdir || mkdir $HOME/public_html/plots/$outdir
mv plots/* $HOME/public_html/plots/$outdir
