# Condor
export CrombieExe=makehbbtree
export CrombieInSample=pandaf/008
export CrombieOutDir=/data/t3home000/dabercro/fullhbb/$(date +%y%m%d)
export CrombieFilesPerJob=20

# Skimming
export CrombieNLocalProcs=12
export CrombieFullDir=/data/t3home000/dabercro/fullhbb/$(ls -1 -t /data/t3home000/dabercro/fullhbb | head -n1)
export CrombieSkimDir=/data/t3home000/dabercro/hbb/$(date +%y%m%d)
