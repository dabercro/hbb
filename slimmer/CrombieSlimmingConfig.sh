#inp_dir=/mnt/hadoop/scratch/dabercro/hbb
inp_dir=/data/t3home000/dabercro/hbb
version='013'
fin_dir=190830_puppijets_breg2018_$version

# Condor
export CrombieExe='eval skiptf=1 ispuppi=1 input_dir=$INPUT_DIR regresstree'
#export CrombieExe='eval ispuppi=1 input_dir=$INPUT_DIR mkhbb'
export CrombieInSample=pandaf/$version
export CrombieOutDir=$inp_dir/$fin_dir
export CrombieFilesPerJob=3

export CrombieBakDir=/data/t3home000/dabercro/tars/hbb/$fin_dir
