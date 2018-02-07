# Condor
export CrombieExe=makehbbtree
export CrombieInSample=pandaf/008
export CrombieOutDir=/data/t3home000/dabercro/fullhbb/$(date +%y%m%d)
export CrombieFilesPerJob=15

# Skimming
export CrombieNLocalProcs=12
inp_dir=/data/t3home000/dabercro/fullhbb
#export CrombieFullDir=$inp_dir/reg
export CrombieFullDir=$inp_dir/$(ls -1 -t $inp_dir | head -n1)
export CrombieSkimDir=/data/t3home000/dabercro/hbb/$(date +%y%m%d)
