inp_dir=/data/t3home000/dabercro/fullhbb
suff='_v1'

# Condor
export CrombieExe='eval input_dir=$INPUT_DIR mkhbb'
export CrombieInSample=pandaf/010
export CrombieOutDir=$inp_dir/$(date +%y%m%d)$suff
export CrombieFilesPerJob=5

# Skimming
export CrombieNLocalProcs=8
export CrombieFullDir=$(ls -1 -t -d $inp_dir/* | head -n1)
export CrombieSkimDir=/data/t3home000/dabercro/hbb/$(date +%y%m%d)$suff
#export CrombieSkimDir=/data/t3home000/dabercro/hbb/breg$suff
