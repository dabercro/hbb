inp_dir=/mnt/hadoop/scratch/dabercro/hbb
#inp_dir=/data/t3home000/dabercro/hbb
version='009'

# Condor
export CrombieExe='eval input_dir=$INPUT_DIR mkhbb'
export CrombieInSample=pandaf/$version
export CrombieOutDir=$inp_dir/$(date +%y%m%d)_$version
export CrombieFilesPerJob=10
