#! /usr/bin/env python

from array import array

from CrombieTools.PlotTools.PlotHists import plotter
from CrombieTools.PlotTools import AddOutDir

from ROOT import TFile

directory = '/data/t3home000/dabercro/hbb/compare/wjets/'

nlo_file = TFile(directory + 'WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8.root')
lo_file = TFile(directory + 'WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root')
nlobinned_file = TFile(directory + 'WJetsToLNu_Pt.root')
lobinned_file = TFile(directory + 'WJetsToLNu_HT.root')

plotter.SetDefaultExpr('genboson_pt')
plotter.SetDefaultWeight('genboson * mc_weight * XSecWeight')

plotter.AddTree(nlo_file.Get('events'))
plotter.AddTree(nlobinned_file.Get('events'))
plotter.AddTree(lo_file.Get('events'))
plotter.AddTree(lobinned_file.Get('events'))

plotter.AddLegendEntry('NLO', 1, 2, 1)
plotter.AddLegendEntry('NLO binned', 3, 2, 1)
plotter.AddLegendEntry('LO', 2, 2, 1)
plotter.AddLegendEntry('LO binned', 4, 2, 1)

plotter.SetMakeRatio(True)
plotter.SetRatioIndex(3)

plotter.SetEventsPer(1)

plotter.SetRatioMinMax(0.5, 2.0)

binning = [40, 50, 75, 100, 125, 150, 200, 250, 300, 400, 500, 700, 1000]

hists = plotter.MakeHists(len(binning) - 1, array('d', binning))

out = TFile('wjets_kfactors.root', 'RECREATE')

for name, hist in zip(['NLO', 'NLO_binned', 'LO', 'LO_binned'], hists):
    out.WriteTObject(hist, name)

plotter.MakeCanvas(AddOutDir('wjets_1'), hists, 'Boson p_{T}', 'Events/GeV/fb^{-1}', True)

out.Close()
