inp_dir=/data/t3home000/dabercro/fullhbb
suff='_v1'

# Condor
export CrombieExe=makehbbtree
export CrombieInSample=pandaf/009
export CrombieOutDir=$inp_dir/$(date +%y%m%d)$suff
export CrombieFilesPerJob=3

# Skimming
export CrombieNLocalProcs=12
export CrombieFullDir=$(ls -1 -t -d $inp_dir/*$suff | head -n1)
#export CrombieSkimDir=/data/t3home000/dabercro/hbb/$(date +%y%m%d)$suff
export CrombieSkimDir=/data/t3home000/dabercro/hbb/breg$suff
