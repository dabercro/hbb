#inp_dir=/mnt/hadoop/scratch/dabercro/hbb
inp_dir=/data/t3home000/dabercro/hbb
version='013'
fin_dir=190610_$version

# Condor
export CrombieExe='eval input_dir=$INPUT_DIR regresstree'
export CrombieInSample=pandaf/$version
export CrombieOutDir=$inp_dir/$fin_dir
export CrombieFilesPerJob=1

export CrombieBakDir=/data/t3home000/dabercro/tars/hbb/$fin_dir
