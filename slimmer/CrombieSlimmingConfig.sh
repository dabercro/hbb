inp_dir=/data/t3home000/dabercro/fullhbb
#suff='_data'
suff=''

# Condor
export CrombieExe=makehbbtree
export CrombieInSample=pandaf/009
export CrombieOutDir=$inp_dir/$(date +%y%m%d)$suff
export CrombieFilesPerJob=15

# Skimming
export CrombieNLocalProcs=12
export CrombieFullDir=$inp_dir/$(ls -1 -t $inp_dir | head -n1)
export CrombieSkimDir=/data/t3home000/dabercro/hbb/$(date +%y%m%d)$suff
