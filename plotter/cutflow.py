#! /usr/bin/env python

import os
import sys

from CrombieTools.LoadConfig import cuts
from CrombieTools.AnalysisTools.CutflowMaker import cutflowMaker

from ROOT import TFile


testFile = TFile(os.path.join(os.environ['CrombieInFilesDir'], 'MET.root'))

cutflowMaker.AddTree(testFile.Get('events'))

loosecuts = [
    'met_filter == 1',
    'met > 150',
    'jet1_pt > 50',
    'jet2_pt > 25',
    'cmva_hbb_pt > 50',
    'cmva_jet2_cmva > -0.7',
    'min_dphi_metj_hard > 0.5'
    ]

if len(sys.argv) == 1:
    for cut in loosecuts:
        cutflowMaker.AddCut(cut, cut)

    cutflowMaker.PrintCutflow()

else:
    region = sys.argv[1]
    regions = ['signal', 'heavyz', 'lightz', 'tt']

    if region in regions:
        cutflowMaker.Reset()
        for cut in loosecuts[:5] + ['met_trigger == 1'] + cuts.cut('ZvvHbb', region).split('&&'):
            cutflowMaker.AddCut(cut.strip(), cut)

        print '-' * 10
        print region.upper()
        print '-' * 10
        cutflowMaker.PrintCutflow()
