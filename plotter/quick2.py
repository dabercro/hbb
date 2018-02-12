#! /usr/bin/env python

from CrombieTools.PlotTools.PlotHists import plotter
from CrombieTools.PlotTools import AddOutDir
from CrombieTools.LoadConfig import cuts

from ROOT import TFile

infile = TFile('/data/t3home000/dabercro/hbb/180208/ZH_HToBB_ZToNuNu_M125_13TeV_powheg_pythia8.root')

plotter.SetDefaultCut('%s * (%s)' % (cuts.cut('ZvvHbb', 'signal'), cuts.dataMCCuts('signal', False)))
plotter.SetDefaultTree(infile.Get('events'))

plotter.AddExpr('cmva_hbb_m')
plotter.AddExpr('cmva_hbb_m_reg_old')
plotter.AddExpr('cmva_hbb_m_reg')

plotter.SetNormalizedHists(True)

plotter.AddLegendEntry('No Regression', 1, 2, 1)
plotter.AddLegendEntry('Old Regression', 2, 2, 1)
plotter.AddLegendEntry('New Regression', 3, 2, 1)

plotter.SetDebugLevel(2)

plotter.MakeCanvas(AddOutDir('regression'), 50, 50, 200, 'Dijet Mass', 'A.U.')

plotter.SetDebugLevel(1)

plotter.AddExpr('cmva_gen_withnu_dijet_m')
plotter.AddLegendEntry('Dijet Gen Mass', 1, 1, 2)

plotter.MakeCanvas(AddOutDir('regression_withgen'), 50, 50, 200, 'Dijet Mass', 'A.U.')

plotter.ResetExpr()
plotter.ResetLegend()

plotter.AddExpr('1.0 - cmva_jet1_gen_withnu_pt/cmva_jet1_pt')
plotter.AddExpr('cmva_jet1_pt_ratio - cmva_jet1_gen_withnu_pt/cmva_jet1_pt')
plotter.AddExpr('cmva_jet1_pt_ratio_old - cmva_jet1_gen_withnu_pt/cmva_jet1_pt')

plotter.AddLegendEntry('No Regression', 1, 2, 1)
plotter.AddLegendEntry('Old Regression', 2, 2, 1)
plotter.AddLegendEntry('New Regression', 3, 2, 1)

plotter.MakeCanvas(AddOutDir('regression_perform'), 20, -1, 1, 'Regression - Truth', 'Normalized')
