source ../slimmer/CrombieSlimmingConfig.sh

if [ -z $FIRST ]
then
    export CrombieMCConfig=MCConfig.txt
    export CrombieInFilesDir=/data/t3home000/dabercro/hbb/171129
    export CrombieOutPlotDir=$HOME/public_html/plots/$(date +%y%m%d)
else
    export CrombieMCConfig=FirstConfig.txt
    export CrombieInFilesDir=/data/t3home000/dabercro/hbb/171127
    export CrombieOutPlotDir=$HOME/public_html/plots/$(date +%y%m%d)_first
fi

#export CrombieSignalConfig=SignalConfig.txt
export CrombieLuminosity=35900.0

export CrombieOutLimitTreeDir=limits/$(date +%y%m%d)

export CrombieCutsFile=cuts.py
