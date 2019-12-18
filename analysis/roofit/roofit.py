#! /usr/bin/env python

import os
import re

import ROOT

indir = '/data/t3home000/dabercro/nano/smearnano/191215_cut'
mc = 'mc.root'
data = 'DoubleMuon.root'
cut = 'lep1_mass > 0.1 && lep2_mass > 0.1'
cutvars = ['lep1_mass', 'lep2_mass']


w = ROOT.RooWorkspace('w')

for filename, kind in [(mc, 'mc'), (data, 'data')]:
    fullname = os.path.join(indir, filename)

    print 'Opening', fullname

    infile = ROOT.TFile(fullname)

    # Linear fit
    w.factory('Gaussian::rhofit(rhoAll[0, 60], rho_mean[40, 0, 80], rho_width[20, 0, 50])')
    w.factory('PolyVar::mean(rhoAll, {m0[1, 0, 2], m1[0, -0.03, 0.03], m2[0, -0.01, 0.01]})')
    w.factory('PolyVar::width(rhoAll, {w0[0.25, 0, 1], w1[0, -0.03, 0.03]})')
    w.factory('Gaussian::gauss(jet1_response[0, 2], mean, width)')

    w.factory('PROD::model(gauss|rhoAll, rhofit)')

    alpha = ROOT.RooRealVar('alpha', 'alpha', 0, 0.4)
    jet1_response = w.var('jet1_response')
    rho = w.var('rhoAll')

    roocutvars = [ROOT.RooRealVar(var, var, 0, 1) for var in cutvars]

    if kind == 'mc':
        xsec_weight = ROOT.RooRealVar('xsec_weight', 'xsec_weight', -1, 1)
        roocutvars.append(xsec_weight)

    print ' Importing ...'

    args = ['data', 'data', ROOT.RooArgSet(alpha, jet1_response, rho, *roocutvars), ROOT.RooFit.Import(infile.events), ROOT.RooFit.Cut(cut)]
    if kind == 'mc':
        args.append(ROOT.RooFit.WeightVar('xsec_weight'))

    points = ROOT.RooDataSet(*args)

    print ' Done !'

    model = w.pdf('model')

    if kind == 'mc':
        model.fitTo(points, ROOT.RooFit.SumW2Error(True))
    else:
        model.fitTo(points)

    model.Print('')


    frame = jet1_response.frame()
    points.plotOn(frame)
    model.plotOn(frame)
    
    frame.Draw()

    raw_input('...')

    break
