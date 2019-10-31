#! /usr/bin/env python

import os
import re

import ROOT

indir = '/data/t3home000/dabercro/hbb/191007_smeartree_chs'
mc = 'DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.root'
data = 'DoubleMuon.root'

#cut = '(jet2 && ((lep1_corrpt > 20 && lep2_corrpt > 20 && (lep1_q + lep2_q) == 0 && dilep_corr_pt > 100 && lep1_ismuon && lep2_ismuon) && (lep1_minireliso < 0.25 && lep2_minireliso < 0.25) && (fabs(jet1_eta) < 2.0 && jet2_pt/dilep_corr_pt < 0.3 && jet1_dphi_ll > 2.8 && jet1_btagDeepB > 0.9)))'
cut = 'jet1_btagDeepB > 0.9 && alpha < 0.3'
cutvars = ['jet1_btagDeepB']


w = ROOT.RooWorkspace('w')

for filename, kind in [(mc, 'mc'), (data, 'data')]:
    fullname = os.path.join(indir, filename)

    print 'Opening', fullname

    infile = ROOT.TFile(fullname)

    jet2_pt = ROOT.RooRealVar("jet2_pt", "jet2_pt", 0, 200)
    jet1_pt = ROOT.RooRealVar("jet1_pt", "jet1_pt", 0, 200)
    dilep_corr_pt = ROOT.RooRealVar('dilep_corr_pt', 'dilep_corr_pt', 50, 200)

    response = ROOT.RooRealVar("", "jet1_pt/dilep_corr_pt", 0.0, 2.0)
    roocutvars = [ROOT.RooRealVar(var, var, -1000, 1000) for var in cutvars]

    print ' Importing ...'

    points = ROOT.RooDataSet("data", "data", ROOT.RooArgSet(jet2_pt, jet1_pt, *roocutvars), ROOT.RooFit.Import(infile.events), ROOT.RooFit.Cut(cut))

    print ' Done !'

    frame = jet2_pt.frame()
    points.plotOn(frame)
    frame.Draw()

    raw_input('...')

    break
