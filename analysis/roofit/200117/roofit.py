#! /usr/bin/env python

import os
import re
import sys
import pprint
import math

import ROOT

from collections import defaultdict

from CrombieTools import LoadConfig


indir = '%s/single' % os.environ['CrombieInFilesDir']
mc = 'mc.root'
data = 'data.root'
cut = 'jet1_btagDeepB > 0.9'
cutvars = ['jet1_btagDeepB']

alpha_shape = sys.argv[1]
xsec_var = sys.argv[2]

toprint = defaultdict(dict)
results = defaultdict(lambda: defaultdict(dict))


for filename, kind in [(mc, 'mc'), (data, 'data')]:
    w = ROOT.RooWorkspace('w')

    fullname = os.path.join(indir, filename)

    print 'Opening', fullname

    infile = ROOT.TFile(fullname)

    # Fit

    # Alpha distribution
    w.factory('%s::alphafit(alpha[0, 0.3], alpha_mean[0.15, 0, 0.25], alpha_width[0.1, 0, 0.3])' % alpha_shape)

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
        roocutvars.append(ROOT.RooRealVar(xsec_var, xsec_var, -1, 1))
    else:
        cut += ' && trigger == 1'
        roocutvars.append(ROOT.RooRealVar('trigger', 'trigger', -1, 2))

    args = ['data', 'data', ROOT.RooArgSet(alpha, jet1_response, *roocutvars), ROOT.RooFit.Import(infile.events), ROOT.RooFit.Cut(cut)]


    if kind == 'mc':
        args.append(ROOT.RooFit.WeightVar(xsec_var))

    points = ROOT.RooDataSet(*args)

    model = w.pdf('model')

    if kind == 'mc':
        model.fitTo(points, ROOT.RooFit.SumW2Error(True))
    else:
        model.fitTo(points)

    model.Print('')
    
    for var in ['alpha_width', 'alpha_mean', 'mean_0', 'mean_1', 'width_intercept', 'width_slope']:
        handle = w.var(var)
        toprint[var][kind] = '%s +- %s' % (handle.getVal(), handle.getError())
        results[var][kind]['val'] = handle.getVal()
        results[var][kind]['err'] = handle.getError()

# Print results at the bottom

print 'Alpha shape:', alpha_shape
print 'Cross Section Branch:', xsec_var
pprint.pprint(dict(toprint))

data_int = results['width_intercept']['data']['val']
data_int_err = results['width_intercept']['data']['err']

mc_int = results['width_intercept']['mc']['val']
mc_int_err = results['width_intercept']['mc']['err']

smear = math.sqrt(pow(data_int, 2) - pow(mc_int, 2))
smear_err = math.sqrt(abs(math.pow(data_int * data_int_err, 2) +
                          math.pow(mc_int * mc_int_err, 2))) / smear

print ''
print 'Smear factor: %f +- %f' % (smear, smear_err)
print 'Scale factor: %f +- %f' % (data_int/mc_int - 1.0,
                                  math.sqrt(pow(data_int_err/mc_int, 2) +
                                            pow(data_int * mc_int_err/pow(mc_int, 2), 2)
                                            ))
