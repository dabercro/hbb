export CrombieNLocalProcs=7

export CrombieMCConfig=MCConfig.txt
export CrombieMCConfig=MCConfig010.txt
export CrombieInFilesDir=/data/t3home000/dabercro/hbb/180531_v1
export themfile=/data/t3home000/dabercro/skimold/in/v2.root
if [ -z $SUFF ]
then
    SUFF=v1
fi

export CrombieOutPlotDir=$HOME/public_html/plots/$(date +%y%m%d)_$SUFF

export CrombieSignalConfig=SignalPlot.txt
export CrombieSignalConfig=SignalPlot010.txt
#export CrombieSignalConfig=SignalConfig.txt
#export CrombieLuminosity=35900.0
export CrombieLuminosity=41300.0

export CrombieOutLimitTreeDir=limits/$(date +%y%m%d)

export CrombieCutsFile=cuts.py
