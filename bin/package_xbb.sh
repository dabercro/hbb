#! /bin/bash

cd $CMSSW_BASE/..

tar --exclude '.git' -czvf xbb_condor.tgz CMSSW_10_1_0
