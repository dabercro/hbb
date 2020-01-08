#! /usr/bin/env python

import os
import re

import ROOT

indir = '/data/t3home000/dabercro/nano/smearnano/191218_cut/root'
mc = 'mc.root'
data = 'DoubleMuon.root'
cut = 'lep1_mass > 0.1 && lep2_mass > 0.1'
cutvars = ['lep1_mass', 'lep2_mass']


# scale_weight_6 = 'xsec_weight_low'
# scale_weight_2 = 'xsec_weight_high'


for filename, kind in [(mc, 'mc'), (data, 'data')]:
    w = ROOT.RooWorkspace('w')

    fullname = os.path.join(indir, filename)

    print 'Opening', fullname

    infile = ROOT.TFile(fullname)

    # Linear fit

    w.factory('Landau::rhofit(rhoAll[0, 60], r_mean[20, 0, 40], r_width[40, 0, 70])')
    w.factory('Landau::alphafit(alpha[0, 0.3], a_mean[0.15, 0, 0.25], a_width[0.1, 0, 0.3])')

    w.factory('PolyVar::alpha_mean(alpha, {am0[1, 0, 2], am1[0, -0.03, 0.03], am2[0, -0.01, 0.01]})')
    w.factory('PolyVar::alpha_width(alpha, {aw0[0.25, 0, 1], aw1[0, -0.03, 0.03], aw2[0, -0.03, 0.03]})')

    w.factory('PolyVar::rho_mean(rhoAll, {rm0[1, 0, 2], rm1[0, -0.03, 0.03], rm2[0, -0.01, 0.01]})')
    w.factory('PolyVar::rho_width(rhoAll, {rw0[0.25, 0, 1], rw1[0, -0.03, 0.03], rw2[0, -0.03, 0.03]})')

    w.factory('Gaussian::gauss(jet1_response[0, 2], alpha_mean, alpha_width)')

    w.factory('PROD::model(gauss|alpha, alphafit)')


    alpha = ROOT.RooRealVar('alpha', 'alpha', 0, 0.3)
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
