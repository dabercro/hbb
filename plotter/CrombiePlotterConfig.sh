export CrombieNLocalProcs=8

export CrombieMCConfig=MCConfig.txt
export CrombieInFilesDir=/data/t3home000/dabercro/hbb/180326
if [ -z $SUFF ]
then
    SUFF=v1
fi

export CrombieOutPlotDir=$HOME/public_html/plots/$(date +%y%m%d)_$SUFF

export CrombieSignalConfig=SignalConfig.txt
export CrombieLuminosity=35900.0

export CrombieOutLimitTreeDir=limits/$(date +%y%m%d)

export CrombieCutsFile=cuts.py
