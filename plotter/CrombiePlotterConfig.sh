export CrombieNLocalProcs=1

#export CrombieMCConfig=MCConfig.txt
export CrombieMCConfig=wjets.txt
#export CrombieMCConfig=MCConfig010.txt
#export CrombieMCConfig=MCConfigTestExpanded.txt
#export CrombieInFilesDir=/data/t3home000/dabercro/hbb/180523_v2
#export CrombieInFilesDir=/data/t3home000/dabercro/hbb/180531_v1
export CrombieInFilesDir=/data/t3home000/dabercro/hbb/compare/wjets
export themfile=/data/t3home000/hbb/zhnn/v9/sr/MET.root
if [ -z $SUFF ]
then
    SUFF=v0
fi

export CrombieOutPlotDir=$HOME/public_html/plots/$(date +%y%m%d)_$SUFF

#export CrombieSignalConfig=SignalPlot.txt
#export CrombieSignalConfig=SignalPlot010.txt
#export CrombieSignalConfig=SignalConfig.txt
#export CrombieSignalConfig=SignalConfig010.txt
export CrombieLuminosity=35900.0
#export CrombieLuminosity=41300.0

export CrombieOutLimitTreeDir=limits/$(date +%y%m%d)

export CrombieCutsFile=cuts.py
#export CrombieCutsFile=cuts2017.py
