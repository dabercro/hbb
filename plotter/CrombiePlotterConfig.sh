source ../slimmer/CrombieSlimmingConfig.sh

export CrombieMCConfig=MCConfig.txt
export CrombieInFilesDir=/data/t3home000/dabercro/hbb/171210
export CrombieOutPlotDir=$HOME/public_html/plots/$(date +%y%m%d)_oldcuts

#export CrombieSignalConfig=SignalConfig.txt
export CrombieLuminosity=35900.0

export CrombieOutLimitTreeDir=limits/$(date +%y%m%d)

export CrombieCutsFile=cuts.py
