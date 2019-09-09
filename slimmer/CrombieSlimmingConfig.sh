day=190906
version='014'
#exe=mkhbb
#exe=regresstree
exe=smeartree
#puppi='ispuppi=1 '
#inp_dir=/mnt/hadoop/scratch/dabercro/hbb
inp_dir=/data/t3home000/dabercro/hbb

if [ -z $puppi ]
then
    jet=puppi
else
    jet=chs
fi

fin_dir=${day}_${exe}_${jet}_${version}

# Condor
export CrombieExe='eval '$puppi'input_dir=$INPUT_DIR '$exe
export CrombieInSample=pandaf/$version
export CrombieOutDir=$inp_dir/$fin_dir
export CrombieFilesPerJob=3

export CrombieBakDir=/data/t3home000/dabercro/tars/hbb/$fin_dir
