#! /usr/bin/env python

import os
import re
import pprint

import ROOT

from collections import defaultdict


indir = '/data/t3home000/dabercro/nano/smearnano/191218_cut/root'
mc = 'mc.root'
data = 'DoubleMuon.root'
cut = 'lep1_mass > 0.1 && lep2_mass > 0.1'
cutvars = ['lep1_mass', 'lep2_mass']


# scale_weight_6 = 'xsec_weight_low'
# scale_weight_2 = 'xsec_weight_high'

toprint = defaultdict(dict)

for filename, kind in [(mc, 'mc'), (data, 'data')]:
    w = ROOT.RooWorkspace('w')

    fullname = os.path.join(indir, filename)

    print 'Opening', fullname

    infile = ROOT.TFile(fullname)

    # Fit

    # Alpha distribution
    w.factory('Landau::alphafit(alpha[0, 0.3], alpha_mean[0.15, 0, 0.25], alpha_width[0.1, 0, 0.3])')

    alpha = w.var('alpha')

    # Mean is linear as a function of alpha
    w.factory('PolyVar::mean(alpha, {mean_0[1, 0, 2], mean_1[0, -5, 5]})')

    # Width is this weird things as a function of alpha
    width_intercept = ROOT.RooRealVar('width_intercept', 'width_intercept', 0.2, 0.0, 0.7)
    width_slope = ROOT.RooRealVar('width_slope', 'width_slope', 1, 0.0, 5)
    width = ROOT.RooGenericPdf('width', 'TMath::Sqrt(pow(width_intercept, 2) + pow(width_slope * alpha, 2))',
                               ROOT.RooArgList(alpha, width_intercept, width_slope))

    getattr(w, 'import')(width)

    w.factory('Gaussian::gauss(jet1_response[0, 2], mean, width)')

    w.factory('PROD::model(gauss|alpha, alphafit)')

    jet1_response = w.var('jet1_response')

    roocutvars = [ROOT.RooRealVar(var, var, 0, 1) for var in cutvars]

    if kind == 'mc':
        xsec_weight = ROOT.RooRealVar('xsec_weight', 'xsec_weight', -1, 1)
        roocutvars.append(xsec_weight)

    args = ['data', 'data', ROOT.RooArgSet(alpha, jet1_response, *roocutvars), ROOT.RooFit.Import(infile.events), ROOT.RooFit.Cut(cut)]
    if kind == 'mc':
        args.append(ROOT.RooFit.WeightVar('xsec_weight'))

    points = ROOT.RooDataSet(*args)

    if kind == 'mc':
        model.fitTo(points, ROOT.RooFit.SumW2Error(True))
    else:
        model.fitTo(points)

    model.Print('')

    
    for var in ['alpha_width', 'alpha_mean', 'mean_0', 'mean_1', 'width_intercept', 'width_slope']:
        handle = w.var(var)
        toprint[var][kind] = '%s +- %s' % (handle.getVal(), handle.getError())


pprint.pprint(dict(toprint))
