#! /usr/bin/env python

import os
import sys

from CrombieTools.LoadConfig import cuts
from CrombieTools.AnalysisTools.CutflowMaker import cutflowMaker

from ROOT import TFile


testFile = TFile(sys.argv.pop()) if os.path.exists(sys.argv[-1]) else TFile(os.path.join(os.environ['CrombieInFilesDir'], 'MET.root'))

cutflowMaker.AddTree(testFile.Get('events'))

loosecuts = [
    'met_trigger == 1',
    'met_filter == 1',
    'central_jet1_pt > 55',
    'central_jet2_pt > 30',
    'met > 160',
    'cmva_hbb_pt > 50',
    'met > 170',
    'n_centerjet < 4',
    'max(cmva_jet1_pt, cmva_jet2_pt) > 60',
    'min(cmva_jet1_pt, cmva_jet2_pt) > 35',
    'cmva_jet1_pt > 60',
    'cmva_jet2_pt > 35',
    'n_lep_loose == 0',
    'cmva_jet1_cmva > 0.9432',
    'cmva_jet2_cmva > -0.5884',
    'cmva_hbb_m_reg_old < 160 && cmva_hbb_m_reg_old > 60',
    'cmva_hbb_pt_reg_old > 120',
    'dphi_met_trkmet < 0.5',
    'deltaPhi(metphi, cmva_hbb_phi) > 2.0',
    'cmva_dphi_uh > 2.0',
    'min_dphi_metj_hard > 0.5'
    ]

if len(sys.argv) == 1:
    for cut in loosecuts:
        cutflowMaker.AddCut(cut, cut)

    cutflowMaker.PrintCutflow()

else:
    for region in sys.argv[1:]:
        cutflowMaker.Reset()
        for cut in ['met_trigger == 1'] + cuts.cut('ZvvHbb', region).split(' && '):
#        for cut in cuts.cut('ZvvHbb', region).split(' && '):
            cutflowMaker.AddCut(cut.strip(), cut)

        print '-' * 10
        print region.upper()
        print '-' * 10
        cutflowMaker.PrintCutflow()
