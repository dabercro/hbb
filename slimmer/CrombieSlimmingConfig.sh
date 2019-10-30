day=191030
exe=mkhbb
#exe=regresstree
#exe=smeartree
#puppi='ispuppi=1 '
#puppimet='ispuppimet=1 '

#inp_dir=/mnt/hadoop/scratch/dabercro/hbb
inp_dir=/data/t3home000/dabercro/hbb

if [ -z $version ]
then
    version=013
fi

if [ -z $puppi ]
then
    jet=chs
else
    jet=puppi
fi

if [ -z $puppimet ]
then
    met=''
else
    met='_puppimet'
fi

fin_dir=${day}_${exe}_${jet}${met}

# Condor
export CrombieExe='eval '$puppi$puppimet'input_dir=$INPUT_DIR '$exe
export CrombieInSample=pandaf/$version
export CrombieOutDir=$inp_dir/$fin_dir
export CrombieFilesPerJob=3

export CrombieBakDir=/data/t3home000/dabercro/tars/hbb/$fin_dir
