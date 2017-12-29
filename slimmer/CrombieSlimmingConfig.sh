export CrombieNLocalProcs=8

# Condor
export CrombieExe=makehbbtree
export CrombieInSample=pandaf/008
export CrombieOutDir=/data/t3home000/dabercro/fullhbb/$(date +%y%m%d)
export CrombieFilesPerJob=15

#export CrombieFullDir=/local/dabercro/pandaexpr/v_008_v2
export CrombieFullDir=/data/t3home000/dabercro/fullhbb/171228
export CrombieSkimDir=/data/t3home000/dabercro/hbb/$(date +%y%m%d)

export CrombieCheckerScript="$CROMBIEPATH/scripts/findtree.py"

export CrombieDropBranches=DropBranches.txt
