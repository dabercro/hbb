inp_dir=/mnt/hadoop/scratch/dabercro/hbb
#inp_dir=/data/t3home000/dabercro/hbb
suff='_011'

# Condor
export CrombieExe='eval input_dir=$INPUT_DIR mkhbb'
export CrombieInSample=pandaf/011
export CrombieOutDir=$inp_dir/$(date +%y%m%d)$suff
export CrombieFilesPerJob=18
