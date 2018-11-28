#! /usr/bin/env python

import os

from CrombieTools.PlotTools.PlotHists import plotter
from CrombieTools.PlotTools import AddOutDir
from CrombieTools.LoadConfig import cuts
from CrombieTools import Nminus1Cut

from ROOT import TFile

#plotter.SetDebugLevel(3)

directory = '/data/t3home000/dabercro/hbb/compare'
files = [TFile(os.path.join(directory, f)) for f in ['2016.root', '2017.root', '2018.root']]

thecut = '(met_trigger && pfmet > 170 && met_filter == 1 && n_lep_presel == 0 && n_centerjet < 4 && min_dphi_metj_hard > 0.5 && dphi_met_dztrkmet < 0.5 && cmva_hbb_m_reg_old < 500 && cmva_daughter_max_pt > 60 && cmva_daughter_min_pt > 35 && cmva_jet2_cmva > -0.5884 && cmva_dphi_uh > 2.0 && cmva_hbb_pt_reg_old > 120)'
#thecut = '(met_trigger && pfmet > 170 && met_filter == 1 && n_lep_presel == 0 && n_centerjet >= 4 && min_dphi_metj_hard > 0.5 && dphi_met_dztrkmet < 0.5 && cmva_hbb_m_reg_old < 500 && cmva_daughter_max_pt > 60 && cmva_daughter_min_pt > 35 && cmva_jet2_cmva > -0.5884 && cmva_dphi_uh > 2.0 && cmva_hbb_pt_reg_old > 120)'

for f in files:
    plotter.AddTree(f.Get('events'))

plotter.AddLegendEntry('2016 Data', 1, 2, 1)
plotter.AddLegendEntry('2017 Data', 2, 2, 1)
plotter.AddLegendEntry('2018 Data', 3, 2, 1)

plotter.SetNormalizedHists(True)

for expr in [
    ('pfmet', 7, 150, 290),
    ('n_centerjet', 10, 0, 10),
    ('npv', 50, 0, 50),
    ('cmva_jet1_cmva', 42, -1.05, 1.05),
    ('cmva_jet2_cmva', 42, -1.05, 1.05),
    ('cmva_jet1_pt', 25, 0, 300),
    ('cmva_jet2_pt', 25, 0, 300),
    ('cmva_jet1_eta', 25, -2.5, 2.5),
    ('cmva_jet2_eta', 25, -2.5, 2.5),
    ('cmva_jet1_nef', 25, 0, 1.0),
    ('cmva_jet1_cef', 25, 0, 1.0),
    ('cmva_jet1_nhf', 25, 0, 1.0),
    ('cmva_jet1_chf', 25, 0, 1.0),
    ('cmva_jet1_efrac', 25, 0, 1.0),
    ('cmva_jet1_hfrac', 25, 0, 1.0),
    ('cmva_jet2_nef', 25, 0, 1.0),
    ('cmva_jet2_cef', 25, 0, 1.0),
    ('cmva_jet2_nhf', 25, 0, 1.0),
    ('cmva_jet2_chf', 25, 0, 1.0),
    ('cmva_jet2_efrac', 25, 0, 1.0),
    ('cmva_jet2_hfrac', 25, 0, 1.0),
    ('jet_pt', 50, 0, 300),
    ('jet_eta', 25, -5.0, 5.0),
    ('n_bcmva_loose', 10, 0, 10),
    ]:
    plotter.ResetWeight()
    plotter.ResetExpr()

#    for weight in ['/ 35.9', '* pu_2017to2016_v2 / 41.3']:
#        plotter.AddWeight(Nminus1Cut(thecut, expr[0]) + weight)
#    plotter.AddWeight(Nminus1Cut(thecut + '* pu_2018to2016_v2 / 12.3', expr[0]).replace('cmva_', ''))
    plotter.AddWeight(Nminus1Cut(thecut + '/ 35.9', expr[0]))
    plotter.AddWeight(Nminus1Cut(thecut + '/ 41.3', expr[0]))
    plotter.AddWeight(Nminus1Cut(thecut + '/ 12.3', expr[0]).replace('cmva_', ''))

    plotter.AddExpr(expr[0])
    plotter.AddExpr(expr[0])
    plotter.AddExpr(expr[0].replace('cmva_', '').replace('bloose', 'bcmva_loose'))

    plotter.MakeCanvas(AddOutDir(expr[0]), expr[1], expr[2], expr[3], expr[0], 'Events/fb')
    plotter.MakeCanvas(AddOutDir('log_' + expr[0]), expr[1], expr[2], expr[3], expr[0], 'Events/fb', True)
