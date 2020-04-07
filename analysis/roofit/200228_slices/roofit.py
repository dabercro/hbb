#! /usr/bin/env python

import os
import re
import sys
import pprint
import math

import ROOT

from collections import defaultdict

from CrombieTools import LoadConfig


indir = '%s/skimmed' % os.environ['CrombieInFilesDir']
mc = 'mc.root'
data = 'data.root'
cut = '1'
cutvars = []

alpha_shape = sys.argv[1] if len(sys.argv) > 1 else 'sum'
xsec_var = sys.argv[2] if len(sys.argv) > 2 else 'xsec_weight'

toprint = defaultdict(dict)
results = defaultdict(lambda: defaultdict(dict))


def do_plots(workspace, points, xvar, yvar, kind, xslices=[()], yslices=[()]):
    model = workspace.pdf('model')

    hist_2d = model.createHistogram('%shist' % kind, workspace.var(xvar), ROOT.RooFit.Binning(20),
                                    ROOT.RooFit.YVar(workspace.var(yvar), ROOT.RooFit.Binning(20)))
    data_2d = points.createHistogram(workspace.var(xvar), workspace.var(yvar), 20, 20, '', '%sdata' % kind)
    data_2d.Scale((1000 if kind == 'data' and alpha_shape == 'sum' else 100)/data_2d.Integral()/(1 if alpha_shape == 'sum' else 100))
    data_2d.SetLineColor(ROOT.kRed)

    for style in ['box', 'surf']:

        c_2d = ROOT.TCanvas()
        hist_2d.Draw(style)
        data_2d.Draw('%s,same' % style)

        for end in ['pdf', 'png']:
            c_2d.SaveAs('plots/%s_%s_2d_%s_%s.%s' % (alpha_shape, xsec_var, kind, style, end))

    for var, slices, other in [(xvar, yslices, yvar), (yvar, xslices, xvar)]:
        for i_slice, slic in enumerate(slices):
            if slic:
                workspace.var(other).setRange('%s_%s_slice_%i' % (kind, other, i_slice), slic[0], slic[1])
                frame = workspace.var(var).frame(ROOT.RooFit.Title('%f < %s < %f' % (slic[0], other, slic[1])))
            else:
                frame = workspace.var(var).frame()

            if slic:
                points.plotOn(frame, ROOT.RooFit.CutRange('%s_%s_slice_%i' % (kind, other, i_slice)))
                model.plotOn(frame, ROOT.RooFit.ProjectionRange('%s_%s_slice_%i' % (kind, other, i_slice)))

            else:
                points.plotOn(frame)
                model.plotOn(frame)

            c = ROOT.TCanvas()
            frame.Draw()
            for end in ['pdf', 'png']:
                c.SaveAs('plots/%s_%s_%s_%s_slice%i.%s' % (alpha_shape, xsec_var, var, kind, i_slice, end))



g = ROOT.RooWorkspace('g')

genfile = ROOT.TFile(os.path.join(indir, mc))

# Linear intrinsic fit

# Alpha distribution
if alpha_shape == 'sum':
    g.factory('Landau::alpha_landau(alpha[0, 0.3], alpha_mean_landau[0.15, 0, 0.25], alpha_width_landau[0.1, 0, 0.3])')
    g.factory('Gaussian::alpha_gaussian(alpha, alpha_mean_gaussian[0.15, 0, 0.25], alpha_width_gaussian[0.05, 0, 0.3])')
    g.factory('SUM::alphafit(landau_norm[0.5, 0, 1.0] * alpha_landau, gaussian_norm[0.5, 0, 1.0] * alpha_gaussian)')
else:
    g.factory('%s::alphafit(alpha[0, 0.3], alpha_mean[0.15, 0, 0.25], alpha_width[0.1, 0, 0.3])' % alpha_shape)


# Mean is linear as a function of alpha
g.factory('PolyVar::mean(alpha, {mean_0[1, 0, 2], mean_1[0, -5, 5]})')

# Width is linear as a function of alpha
g.factory('PolyVar::width(alpha, {width_0[0.2, 0, 0.7], width_slope[0.1, -1, 5]})')

g.factory('Gaussian::gauss(jet1_response_intrinsic[0, 2], mean, width)')

g.factory('PROD::model(gauss|alpha, alphafit)')


gencut = 'jet1_response_intrinsic > 0'
gencutvars = [ROOT.RooRealVar(var, var, 0, 10000) for var in cutvars]
gencutvars.append(ROOT.RooRealVar(xsec_var, xsec_var, -1, 1))

genpoints = ROOT.RooDataSet('gen', 'gen',
                            ROOT.RooArgSet(g.var('alpha'), g.var('jet1_response_intrinsic'), *gencutvars),
                            ROOT.RooFit.Import(genfile.events), ROOT.RooFit.Cut(gencut),
                            ROOT.RooFit.WeightVar(xsec_var))

genmodel = g.pdf('model')
genmodel.fitTo(genpoints, ROOT.RooFit.SumW2Error(True))


genmodel.Print('')

alpha_slices = [
    (),
    (0.0, 0.1),
    (0.1, 0.15),
    (0.15, 0.2),
    (0.2, 0.3)
    ]

jet_intrinsic_slices = [
    (),
    (0.0, 0.6),
    (0.6, 0.8),
    (0.8, 0.9),
    (0.9, 1.0),
    (1.0, 1.1),
    (1.1, 1.2),
    (1.2, 1.4),
    (1.4, 2.0)
    ]

do_plots(g, genpoints, 'alpha', 'jet1_response_intrinsic', 'gen', alpha_slices, jet_intrinsic_slices)


print '=' * 20
print 'End of GEN'
print '=' * 20


# Width slope from intrinsic is fixed

width_slope = g.var('width_slope')
width_slope.setConstant(True)

for filename, kind in [(mc, 'mc'), (data, 'data')]:
    w = ROOT.RooWorkspace('w')

    fullname = os.path.join(indir, filename)

    print 'Opening', fullname

    infile = ROOT.TFile(fullname)

    # Fit

    # Alpha distribution
    if alpha_shape == 'sum':
        w.factory('Landau::alpha_landau(alpha[0, 0.3], alpha_mean_landau[0.15, 0, 0.25], alpha_width_landau[0.1, 0, 0.3])')
        w.factory('Gaussian::alpha_gaussian(alpha, alpha_mean_gaussian[0.15, 0, 0.25], alpha_width_gaussian[0.05, 0, 0.3])')
        w.factory('SUM::alphafit(landau_norm[0.5, 0, 1.0] * alpha_landau, gaussian_norm[0.5, 0, 1.0] * alpha_gaussian)')
    else:
        w.factory('%s::alphafit(alpha[0, 0.3], alpha_mean[0.15, 0, 0.25], alpha_width[0.1, 0, 0.3])' % alpha_shape)


    alpha = w.var('alpha')

    # Mean is linear as a function of alpha
    w.factory('PolyVar::mean(alpha, {mean_0[1, 0, 2], mean_1[0, -5, 5]})')

    # Width is this weird things as a function of alpha
    width_intercept = ROOT.RooRealVar('width_intercept', 'width_intercept', 0.2, 0.0, 0.7)
    width_slope_2 = ROOT.RooRealVar('width_slope_2', 'width_slope_2', 1, 0.0, 5)
    width = ROOT.RooGenericPdf('width', 'TMath::Sqrt(pow(width_intercept + width_slope * alpha, 2) + pow(width_slope_2 * alpha, 2))',
                               ROOT.RooArgList(alpha, width_intercept, width_slope, width_slope_2))

    getattr(w, 'import')(width)

    w.factory('Gaussian::gauss(jet1_response[0, 2], mean, width)')

    w.factory('PROD::model(gauss|alpha, alphafit)')

    jet1_response = w.var('jet1_response')

    roocutvars = [ROOT.RooRealVar(var, var, 0, 10000) for var in cutvars]

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

    jet_slices = jet_intrinsic_slices

    do_plots(w, points, 'alpha', 'jet1_response', kind, alpha_slices, jet_slices)


    for var in ['mean_0', 'mean_1', 'width_intercept', 'width_slope', 'width_slope_2'] + (
        ['alpha_width', 'alpha_mean'] if alpha_shape != 'sum' else
        ['alpha_width_landau', 'alpha_mean_landau', 'alpha_width_gaussian', 'alpha_mean_gaussian']
        ):
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

smear = math.sqrt(abs(pow(data_int, 2) - pow(mc_int, 2))) * (data_int - mc_int)/(abs(data_int - mc_int) if data_int != mc_int else 1)
smear_err = math.sqrt(abs(math.pow(data_int * data_int_err, 2) +
                          math.pow(mc_int * mc_int_err, 2))) / smear

print ''
print 'Smear factor: %f +- %f' % (smear, smear_err)
print 'Scale factor: %f +- %f' % (data_int/mc_int - 1.0,
                                  math.sqrt(pow(data_int_err/mc_int, 2) +
                                            pow(data_int * mc_int_err/pow(mc_int, 2), 2)
                                            ))
