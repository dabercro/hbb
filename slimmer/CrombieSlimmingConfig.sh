inp_dir=/data/t3home000/dabercro/fullhbb
suff='_v1'

# Condor
export CrombieExe='eval input_dir=$INPUT_DIR makehbbtree'
export CrombieInSample=pandaf/009
export CrombieOutDir=$inp_dir/$(date +%y%m%d)$suff
export CrombieFilesPerJob=15

# Skimming
export CrombieNLocalProcs=16
export CrombieFullDir=$(ls -1 -t -d $inp_dir/*$suff | head -n1)
export CrombieSkimDir=/data/t3home000/dabercro/hbb/$(date +%y%m%d)$suff
#export CrombieSkimDir=/data/t3home000/dabercro/hbb/breg$suff
