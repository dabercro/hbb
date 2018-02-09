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
    'met > 170',
    'n_lep_loose == 0',
    'cmva_hbb_m_reg_old > 160 || cmva_hbb_m_reg_old < 60',
    'cmva_hbb_pt_reg_old > 120',
    'dphi_met_trkmet < 0.5',
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
