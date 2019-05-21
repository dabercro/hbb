#inp_dir=/mnt/hadoop/scratch/dabercro/hbb
inp_dir=/data/t3home000/dabercro/hbb
version='013'
#fin_dir=$(date +%y%m%d)_$version
fin_dir=190512_$version

# Condor
export CrombieExe='eval input_dir=$INPUT_DIR mkhbb'
export CrombieInSample=pandaf/$version
export CrombieOutDir=$inp_dir/$fin_dir
export CrombieFilesPerJob=10

export CrombieBakDir=/data/t3home000/dabercro/tars/hbb/$fin_dir
