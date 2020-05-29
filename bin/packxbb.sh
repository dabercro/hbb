#! /bin/bash

cd $CMSSW_BASE/..

tar --exclude '.git' --exclude 'samples' --directory $CMSSW_BASE -czf xbb_condor.tgz python src

ls -lh xbb_condor.tgz
