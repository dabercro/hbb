#! /usr/bin/env python

import os
import glob
import sys
import datetime
import math
import numpy
import random


plotdir = '200619_2018'

divbymean = True

def report(title, value, error=None):

    numsig_figs = 2

    if not error:
        print '%s: %f' % (title, value)
        return
    
    order = int(math.floor(math.log10(error))) - (numsig_figs - 1)
    format = '%{0}f'.format('.%i' % (-1 * order) if order < 1 else '')

    print '%s: {f} +- {f}'.format(f=format) % (title, value, error)


class MeanCalc(object):

    def __init__(self):
        self.values = []

        self._mean = None
        self._std = None


    def add(self, value, weight):
        self.values.append((value, weight))

    def mean(self):
        if self._mean is None:
            tot = 0
            totw = 0
            for val, weight in self.values:
                tot += val * weight
                totw += weight
            self._mean = max(float(tot)/totw, 0)

        return self._mean

    def std(self):
        if self._std is None:
            # https://www.itl.nist.gov/div898/software/dataplot/refman2/ch2/weightsd.pdf
            mean = self.mean()
            numerator = 0
            totw = 0
            for val, weight in self.values:
                numerator += weight * pow(val - mean, 2)
                totw += weight
            self._std = math.sqrt((len(self.values) * numerator)/((len(self.values) - 1) * totw))

        return self._std

# Sample for the uncertainty
def toy_unc(data_val, data_err, mc_val, mc_err):
    num_toys = 100000
    toys = numpy.zeros(num_toys)
    for index in range(num_toys):
        data = random.gauss(data_val, data_err)
        mc = random.gauss(mc_val, mc_err)
        toys[index] = math.sqrt(abs(math.pow(data, 2) - math.pow(mc, 2)))

    return numpy.mean(toys), numpy.std(toys)

def stretch_unc(data_val, data_err, mc_val, mc_err):
    output = data_val/mc_val
    return output, math.sqrt(math.pow(data_err/mc_val, 2) + math.pow(output * mc_err/mc_val, 2))


sys.argv.append('-b')
import ROOT

trainings = [
    ('jet1_response', 'No Smearing'),
    ('jet1_adjusted_response', 'Adjust For Alpha'),
    ('jet1_adjusted_response_smeared_nominal', 'Adjust For Alpha'),
    ('jet1_adjusted_response_smeared_up', 'Adjust For Alpha'),
    ('jet1_adjusted_response_smeared_down', 'Adjust For Alpha'),
    ('jet1_half_adjusted_response_smeared_nominal', 'Adjust For Alpha'),
    ('jet1_half_adjusted_response_smeared_up', 'Adjust For Alpha'),
    ('jet1_half_adjusted_response_smeared_down', 'Adjust For Alpha'),
    ('jet1_adjusted_response_smeared_scaled_nominal', 'Adjust For Alpha'),
    ('jet1_adjusted_response_smeared_scaled_up', 'Adjust For Alpha'),
    ('jet1_adjusted_response_smeared_scaled_down', 'Adjust For Alpha'),
    ('jet1_half_adjusted_response_smeared_scaled_nominal', 'Adjust For Alpha'),
    ('jet1_half_adjusted_response_smeared_scaled_up', 'Adjust For Alpha'),
    ('jet1_half_adjusted_response_smeared_scaled_down', 'Adjust For Alpha'),
    ]


for bintype in ['smear', 'rho']:

    newdir = os.path.join(
        os.environ['HOME'],
        'public_html/plots',
        '%s_%s%s_%s%s' % (
            datetime.date.strftime(
                datetime.datetime.now(), '%y%m%d'
                ),
            bintype,
            sys.argv[1] if len(sys.argv) > 2 else '',
            plotdir,
            '_divmean' if divbymean else ''
            )
        )

    if not os.path.exists(newdir):
        os.mkdir(newdir)

    ratiodir = '/home/dabercro/public_html/plots/%s' % (plotdir)
    alphadir = ratiodir
    #alphadir = '/home/dabercro/public_html/plots/200116_singlebin_alpha'


    # Name of region and max alpha value
    ranges = [
        ('%splot_1' % bintype, 0.155, MeanCalc(), MeanCalc()),
        ('%splot_2' % bintype, 0.185, MeanCalc(), MeanCalc()),
        ('%splot_3' % bintype, 0.23, MeanCalc(), MeanCalc()),
        ('%splot_4' % bintype, 0.3, MeanCalc(), MeanCalc()),
    ]

    rhos = ([
        ('_1', 16.5, MeanCalc(), MeanCalc()),
        ('_2', 22, MeanCalc(), MeanCalc()),
        ('_3', 65, MeanCalc(), MeanCalc())
        ] if glob.glob(os.path.join(ratiodir, 'rhoplot_?_3*')) else [
            ('_1', 19, MeanCalc(), MeanCalc()),
            ('_2', 65, MeanCalc(), MeanCalc())
            ]) if bintype == 'rho' else [
        ('', 65, MeanCalc(), MeanCalc())
        ]

    jets = [
        ('_1', 100, MeanCalc(), MeanCalc()),
        ('_2', 130, MeanCalc(), MeanCalc()),
        ('_3', 500, MeanCalc(), MeanCalc())
        ]

    def rhotitle (bin):
        if len(rhos) == 1:
            if bintype != 'jet':
                return ''
            if bin == 0:
                return 'p_{T} < 100'
            if bin == 1:
                return '100 #leq p_{T} < 130'
            if bin == 2:
                return '130 #leq p_{T} < 500'
        if bin == 0:
            return '#rho < 16.5' if len(rhos) == 3 else '#rho < 19'
        if bin == 1:
            return '16.5 #leq #rho < 22' if len(rhos) == 3 else '19 #leq #rho'
        return '22 #leq #rho'


    other = (rhos, 'smearplot_rhoAll.root') if bintype != 'jet' else (jets, 'smearplot_jet1_pt_regressed.root')

    for calcs, filename in [(ranges, 'smearplot_alpha.root'),
                            other]:

        index = 0

        average_file = ROOT.TFile(os.path.join(alphadir, filename))
        data_hist = average_file.Get('Data')
        mc_hist = average_file.Get('DY') + average_file.Get('TT')
        bin = 1

        while index != len(calcs):
            calcs[index][2].add(data_hist.GetBinCenter(bin), data_hist.GetBinContent(bin))
            calcs[index][3].add(mc_hist.GetBinCenter(bin), mc_hist.GetBinContent(bin))

            bin += 1
            if data_hist.GetBinCenter(bin) > calcs[index][1]:
                index += 1


    smear_fit = ROOT.TGraphErrors(len(rhos))
    scale_fit = ROOT.TGraphErrors(len(rhos))

    for training, trainname in trainings:

        if not glob.glob(os.path.join(ratiodir,
                                      '*%s*.root' % training)):
            continue

        for bin, rho in enumerate(other[0]):

            index = 0

            data_graph_res = ROOT.TGraphErrors(len(ranges))
            data_graph_res.SetMarkerStyle(8)
            data_graph_res.SetMarkerColor(1)

            mc_graph_res = ROOT.TGraphErrors(len(ranges))
            mc_graph_res.SetMarkerStyle(8)
            mc_graph_res.SetMarkerColor(2)

            data_graph_mean = ROOT.TGraphErrors(len(ranges))
            data_graph_mean.SetMarkerStyle(8)
            data_graph_mean.SetMarkerColor(1)

            mc_graph_mean = ROOT.TGraphErrors(len(ranges))
            mc_graph_mean.SetMarkerStyle(8)
            mc_graph_mean.SetMarkerColor(2)

            gen_graph_res = ROOT.TGraphErrors(len(ranges))
            gen_graph_res.SetMarkerStyle(8)
            gen_graph_res.SetMarkerColor(3)

            for mean in ranges:

                smearfile = ROOT.TFile(
                    os.path.join(
                        ratiodir,
                        '%s%s_%s.root' % (
                            mean[0], rho[0], training)
                        )
                    )

                data_hist = smearfile.Get('Data')
                data_mean = mean[2].mean()
                data_graph_res.SetPoint(index, data_mean, data_hist.GetStdDev()/(data_hist.GetMean() if divbymean else 1.0))
                data_graph_res.SetPointError(index, 0, #mean[2].std(),
                                             math.sqrt(pow(data_hist.GetStdDevError()/data_hist.GetMean(), 2) +
                                                       pow(data_hist.GetStdDev() * data_hist.GetMeanError()/pow(data_hist.GetMean(), 2), 2)))
                data_graph_mean.SetPoint(index, data_mean, data_hist.GetMean())
                data_graph_mean.SetPointError(index, 0, data_hist.GetMeanError())

                mc_hist = smearfile.Get('DY') + smearfile.Get('TT')
                mc_mean = mean[3].mean()
                mc_graph_res.SetPoint(index, mc_mean, mc_hist.GetStdDev()/(mc_hist.GetMean() if divbymean else 1.0))
                mc_graph_res.SetPointError(index, 0, #mean[3].std(),
                                           math.sqrt(pow(mc_hist.GetStdDevError()/mc_hist.GetMean(), 2) +
                                                     pow(mc_hist.GetStdDev() * mc_hist.GetMeanError()/pow(mc_hist.GetMean(), 2), 2)))

                mc_graph_mean.SetPoint(index, mc_mean, mc_hist.GetMean())
                mc_graph_mean.SetPointError(index, 0, mc_hist.GetMeanError())

                genfile = ROOT.TFile(
                    os.path.join(
                        ratiodir,
                        '%s%s_jet1_intrinsic.root' % (mean[0], rho[0])
                        )
                    )

                gen_hist = genfile.Get('DY') + genfile.Get('TT')
                gen_mean = mean[3].mean()
                gen_graph_res.SetPoint(index, gen_mean, gen_hist.GetStdDev()/(gen_hist.GetMean() if divbymean else 1.0))
                gen_graph_res.SetPointError(index, 0,
                                            math.sqrt(pow(gen_hist.GetStdDevError()/gen_hist.GetMean(), 2) +
                                                      pow(gen_hist.GetStdDev() * gen_hist.GetMeanError()/pow(gen_hist.GetMean(), 2), 2)))

                index += 1

            for data, mc, gen, name, mini, maxi, fit, makesub in [
                (data_graph_res, mc_graph_res, gen_graph_res, '#sigma/#mu', 0.0, 0.4,
                 lambda: ROOT.TF1('resolution', 'TMath::Sqrt(pow([0] * x, 2) + pow([1] * (1 + [2] * x), 2))', 0, 0.3), True),
                (data_graph_mean, mc_graph_mean, None, '#mu', 0.7, 1.0,
                 lambda: ROOT.TF1('mean', '[0] * x + [1]', 0, 0.3), False)
                ]:

                hide = ROOT.TGraph(2)
                hide.SetTitle('%s;#alpha;%s' % (rhotitle(bin), name))
                hide.SetPoint(0, 0, mini)
                hide.SetPoint(1, 0.3, maxi)

                for hist in [data, mc]:
                    hist.SetMinimum(mini)
                    hist.SetMaximum(maxi)

                data_func = fit()
                data_func.SetLineColor(1)
                mc_func = fit()
                mc_func.SetLineColor(2)

                c1 = ROOT.TCanvas()

                if gen:
                    gen_func = ROOT.TF1('Intrinsic', '[0] + [1] * x', 0, 0.3)
                    gen_func.SetLineColor(3)
                    genres = gen.Fit(gen_func, 'S')

                    for func in [data_func, mc_func]:
                        func.FixParameter(2, gen_func.GetParameter(1)/gen_func.GetParameter(0))

                datares = data.Fit(data_func, 'S')
                mcres = mc.Fit(mc_func, 'S')

                hide.Draw('ap')
                data.Draw('p,same')
                mc.Draw('p,same')
                data_func.Draw('same')
                mc_func.Draw('same')

                leg = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
                leg.AddEntry(data, 'Data', 'p')
                leg.AddEntry(mc, 'MC', 'p')

                if gen:
                    gen.Draw('p,same')
                    gen_func.Draw('same')
                    leg.AddEntry(gen, 'Intrinsic', 'p')

                leg.Draw()

                print '-'*30
                print trainname
                print training
                print rhotitle(bin)
                print '-'*30

                data_int, data_int_err = (abs(data_func.GetParameter(1)), datares.Error(1))
                mc_int, mc_int_err = (abs(mc_func.GetParameter(1)), mcres.Error(1))

                if makesub:

                    report('Data at y-axis', data_int, data_int_err)
                    report('MC at y-axis', mc_int, mc_int_err)

                    smear = math.sqrt(abs(pow(data_int, 2) - pow(mc_int, 2))) * (data_int - mc_int) / abs(data_int - mc_int)
                    smear_err = math.sqrt(abs(math.pow(data_int * data_int_err, 2) +
                                              math.pow(mc_int * mc_int_err, 2))) / abs(smear)


                    scale = data_int/mc_int - 1.0
                    scale_err = math.sqrt(pow(data_int_err/mc_int, 2) +
                                          pow(data_int * mc_int_err/pow(mc_int, 2), 2))

                    report('Smear factor', smear, smear_err)
                    report('Scale factor', scale, scale_err)

                    gen_int, gen_int_err = (gen_func.GetParameter(0), genres.Error(0))

                    report('Scale factor (from gen)',
                           (data_int - gen_int)/(mc_int - gen_int) - 1.0,
                           math.sqrt(
                            pow(data_int_err/(mc_int - gen_int), 2) +
                            pow((data_int - gen_int) * mc_int_err/pow(mc_int - gen_int, 2), 2) +
                            pow(gen_int_err * (2 * gen_int - mc_int - data_int)/pow(mc_int - gen_int, 2), 2)
                            )
                           )

                    smear_fit.SetPoint(bin, rho[3].mean(), smear)
                    smear_fit.SetPointError(bin, 0, smear_err)
                    scale_fit.SetPoint(bin, rho[3].mean(), scale)
                    scale_fit.SetPointError(bin, 0, scale_err)

                else:
                    scale_err = math.sqrt(pow(mc_int_err/data_int, 2) +
                                          pow(mc_int * data_int_err/pow(data_int, 2), 2))

                    report('Data at y-axis', data_func.GetParameter(1))
                    report('MC at y-axis', mc_func.GetParameter(1))
                    report('Scale factor', data_func.GetParameter(1)/mc_func.GetParameter(1))
                    report('Scale (data) factor', mc_func.GetParameter(1)/data_func.GetParameter(1),  scale_err)

                    scale_err = math.sqrt(pow(data_int_err/mc_int, 2) +
                                          pow(data_int * mc_int_err/pow(mc_int, 2), 2))

                    report('Scale (mc) factor', data_func.GetParameter(1)/mc_func.GetParameter(1), scale_err)

                for ext in ['pdf', 'png', 'C']:
                    c1.SaveAs(
                        os.path.join(newdir,
                                     os.path.basename('%s_%s_%s_%i.%s' % (data_func.GetName(), training, bintype, bin, ext))
                                     )
                        )

    if len(rhos) > 1 or bintype == 'jet':

        for name, tofit in [('smear_fit', smear_fit), ('scale_fit', scale_fit)]:

            print '-' * 20
            print name
            print '-' * 20

            smear_func = ROOT.TF1('lin', '[0] * x + [1]', 0, 40 if bintype == 'rho' else 400)

            derivative_expr = 'TMath::Sqrt(TMath::Power((x + [2] * [1]) * [3], 2) + TMath::Power(([2] * [0] * x + 1) * [4], 2))'

            smear_func_down = ROOT.TF1('down', '[0] * x + [1] - ' + derivative_expr, 0, 40 if bintype == 'rho' else 400)
            smear_func_up = ROOT.TF1('down', '[0] * x + [1] + ' + derivative_expr, 0, 40 if bintype == 'rho' else 400)

            smear_fit_res = tofit.Fit(smear_func, 'S')
            matrix = smear_fit_res.GetCovarianceMatrix()

            print matrix(0, 0), matrix(0, 1)
            print matrix(1, 0), matrix(1, 1)

            for func in [smear_func_down, smear_func_up]:
                func.SetParameter(0, smear_func.GetParameter(0))
                func.SetParameter(1, smear_func.GetParameter(1))
                func.SetParameter(2, matrix(0, 1))
                func.SetParameter(3, smear_fit_res.Error(0))
                func.SetParameter(4, smear_fit_res.Error(1))

                func.SetLineColor(2)
                func.SetLineWidth(1)

            hide2 = ROOT.TGraph(2)

            hide2.SetTitle('%s;#rho;#sigma_{smear}' % ('Smearing' if name == 'smear_fit' else 'Scaling',))
            hide2.SetPoint(0, 0, -0.5)
            hide2.SetPoint(1, 35 if bintype == 'rho' else 250, 0.5)

            tofit.SetMarkerStyle(8)

            c2 = ROOT.TCanvas()
            hide2.Draw('ap')
            tofit.Draw('p,same')
            smear_func.Draw('same')
            smear_func_down.Draw('same')
            smear_func_up.Draw('same')

            for ext in ['pdf', 'png', 'C']:
                c2.SaveAs(
                    os.path.join(newdir,
                                 os.path.basename('%s.%s' % (name, ext))
                                 )
                    )

    os.system('cp %s %s/models.cnf' % (__file__, newdir))
