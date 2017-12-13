#! /usr/bin/env python

from CrombieTools.PlotTools.PlotHists import plotter
from CrombieTools.PlotTools import AddOutDir
from CrombieTools.LoadConfig import cuts

from ROOT import TFile

infile = TFile('TMVAReg_0_new.root')

plotter.SetDefaultCut('genPt > 5')

plotter.AddTreeExpr(infile.Get('TestTree'), '1.0 - target')
plotter.AddTreeExpr(infile.Get('TrainTree'), 'oldReg - target')
plotter.AddTreeExpr(infile.Get('TrainTree'), 'BDTG - target')
plotter.AddTreeExpr(infile.Get('TestTree'), 'BDTG - target')

plotter.SetNormalizedHists(True)

plotter.AddLegendEntry('No Regression', 1, 2, 2)
plotter.AddLegendEntry('Old Regression', 1, 2, 1)
plotter.AddLegendEntry('New (Training)', 2, 2, 2)
plotter.AddLegendEntry('New (Test)', 4, 2, 3)

plotter.MakeCanvas(AddOutDir('tmva'), 20, -1, 1, "Regression - Truth", "Normalized")
