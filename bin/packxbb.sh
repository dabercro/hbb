#! /bin/bash

cd $CMSSW_BASE/..

tar --exclude '.git' --exclude 'samples' --exclude 'logs_*' --exclude '*.tar' --exclude 'tree_*.root' --exclude 'condor_*' --directory $CMSSW_BASE -czf xbb_condor.tgz python src

ls -lh xbb_condor.tgz
