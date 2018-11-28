#! /usr/bin/env python

import os

from CrombieTools.AnalysisTools.HistAnalysis import histAnalysis
from CrombieTools import Nminus1Cut

from ROOT import TFile

directory = '/data/t3home000/dabercro/hbb/compare'
files = ['2016.root', '2018.root']

thecut = '(met_trigger && pfmet > 170 && met_filter == 1 && n_lep_presel == 0 && n_centerjet < 4 && min_dphi_metj_hard > 0.5 && dphi_met_dztrkmet < 0.5 && cmva_hbb_m_reg_old < 500 && cmva_daughter_max_pt > 60 && cmva_daughter_min_pt > 35 && cmva_jet2_cmva > -0.5884 && cmva_dphi_uh > 2.0 && cmva_hbb_pt_reg_old > 120)'

for f, t in zip(files, [histAnalysis.kData, histAnalysis.kBackground]):
    histAnalysis.AddFile('events', os.path.join(directory, f), -1, '', 1, t)

for expr in [
    ('npv', 25, 0, 50),
    ]:
    histAnalysis.SetBaseCut('1')

    histAnalysis.SetDataWeight(Nminus1Cut(thecut, expr[0]))
    histAnalysis.SetMCWeight(Nminus1Cut(thecut.replace('cmva_', ''), expr[0]))

    histAnalysis.SetDefaultExpr(expr[0])

    histAnalysis.MakeReweightHist('npv2018to2016.root', 'puweight', expr[0], expr[1], expr[2], expr[3])
