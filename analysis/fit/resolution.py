#! /usr/bin/env python

import os
import sys
import datetime
import math
import numpy
import random


bintype = 'rho'

end = '_tosmear'

newdir = os.path.join(
    os.environ['HOME'],
    'public_html/plots',
    '%s_resolution%s%s' % (
        datetime.date.strftime(
            datetime.datetime.now(), '%y%m%d'
            ),
        end,
        sys.argv[1] if len(sys.argv) > 1 else ''
        )
    )

if not os.path.exists(newdir):
    os.mkdir(newdir)

alphadir = '/home/dabercro/public_html/plots/191121_alpha'
ratiodir = '/home/dabercro/public_html/plots/191213%s' % end

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


# Name of region and max alpha value
ranges = [
    ('%splot_1' % bintype, 0.155, MeanCalc(), MeanCalc()),
    ('%splot_2' % bintype, 0.185, MeanCalc(), MeanCalc()),
    ('%splot_3' % bintype, 0.23, MeanCalc(), MeanCalc()),
    ('%splot_4' % bintype, 0.3, MeanCalc(), MeanCalc()),
]

rhos = [
    ('_0', 16.5, MeanCalc(), MeanCalc()),
    ('_1', 22, MeanCalc(), MeanCalc()),
    ('_2', 65, MeanCalc(), MeanCalc())
]

def rhotitle (bin):
    if bin == 0:
        return '#rho < 16.5'
    if bin == 1:
        return '16.5 < #rho < 22'
    return '22 < #rho'


sys.argv.append('-b')
import ROOT

for calcs, filename in [(ranges, 'smearplot_alpha.root'),
                        (rhos, 'smearplot_rhoAll.root')]:

    index = 0

    average_file = ROOT.TFile(os.path.join(alphadir, filename))
    data_hist = average_file.Get("Data")
    mc_hist = average_file.Get("DY")
    bin = 1

    while index != len(calcs):
        calcs[index][2].add(data_hist.GetBinCenter(bin), data_hist.GetBinContent(bin))
        calcs[index][3].add(mc_hist.GetBinCenter(bin), mc_hist.GetBinContent(bin))

        bin += 1
        if data_hist.GetBinCenter(bin) > calcs[index][1]:
            index += 1


trainings = [
    ('jet1_response', '')
    ]

smear_fit = ROOT.TGraphErrors(len(rhos))

for training, trainname in trainings:

    for bin, rho in enumerate(rhos):

        index = 0

        data_graph_res = ROOT.TGraphErrors(len(ranges))
        data_graph_res.SetMarkerStyle(8)
        data_graph_res.SetMarkerColor(1)

        mc_graph_res = ROOT.TGraphErrors(len(ranges))
        mc_graph_res.SetMarkerStyle(8)
        mc_graph_res.SetMarkerColor(2)

        data_graph_mean = ROOT.TGraph(len(ranges))
        data_graph_mean.SetMarkerStyle(8)
        data_graph_mean.SetMarkerColor(1)

        mc_graph_mean = ROOT.TGraph(len(ranges))
        mc_graph_mean.SetMarkerStyle(8)
        mc_graph_mean.SetMarkerColor(2)

        for mean in ranges:

            smearfile = ROOT.TFile(
                os.path.join(
                    ratiodir,
                    '%s%s_%s.root' % (
                        mean[0], rho[0], training)
                    )
                )

            data_hist = smearfile.Get("Data")
            data_mean = mean[2].mean()
            data_graph_res.SetPoint(index, data_mean, data_hist.GetStdDev())
            data_graph_res.SetPointError(index, 0, #mean[2].std(),
                                         data_hist.GetStdDevError())
            data_graph_mean.SetPoint(index, data_mean, data_hist.GetMean())

            mc_hist = smearfile.Get("MC")
            mc_mean = mean[3].mean()
            mc_graph_res.SetPoint(index, mc_mean, mc_hist.GetStdDev())
            mc_graph_res.SetPointError(index, 0, #mean[3].std(),
                                       mc_hist.GetStdDevError())
            mc_graph_mean.SetPoint(index, mc_mean, mc_hist.GetMean())

            index += 1

        for data, mc, name, mini, maxi, fit, makesub in [
            (data_graph_res, mc_graph_res, '#sigma', 0.0, 0.4,
             lambda: ROOT.TF1('resolution', 'TMath::Sqrt(pow([0], 2) + pow([1] * x, 2))', 0, 0.3), True),
            (data_graph_mean, mc_graph_mean, '#mu', 0.7, 1.0,
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
            leg.Draw()

            print '-'*30
            print trainname
            print training
            print '-'*30

            if makesub:
                data_sub1 = ROOT.TF1('lin', '[0] * x + [1]', 0, 0.3)
                data_sub2 = ROOT.TF1('lin', '[0] * x', 0, 0.3)
                data_sub1.SetParameter(0, 0)
#                data_sub1.SetParameter(0, abs(data_func.GetParameter(2)))
                data_sub1.SetParameter(1, abs(data_func.GetParameter(0)))
                data_sub2.SetParameter(0, abs(data_func.GetParameter(1)))

                print 'Data at y-axis: %s +- %s' % (abs(data_func.GetParameter(0)), datares.Error(0))

                for data_sub in [data_sub1, data_sub2]:
                    data_sub.SetLineWidth(1)
                    data_sub.SetLineColor(data_func.GetLineColor())
                    data_sub.Draw('same')

                mc_sub1 = ROOT.TF1('lin', '[0] * x + [1]', 0, 0.3)
                mc_sub2 = ROOT.TF1('lin', '[0] * x', 0, 0.3)
                mc_sub1.SetParameter(0, 0)
#                mc_sub1.SetParameter(0, abs(mc_func.GetParameter(2)))
                mc_sub1.SetParameter(1, abs(mc_func.GetParameter(0)))
                mc_sub2.SetParameter(0, abs(mc_func.GetParameter(1)))

                print 'MC at y-axis: %s +- %s' % (abs(mc_func.GetParameter(0)), mcres.Error(0))

                smear = math.sqrt(abs(math.pow(data_func.GetParameter(0), 2) - 
                                      math.pow(mc_func.GetParameter(0), 2)))

                smear_err = math.sqrt(abs(math.pow(data_func.GetParameter(0) * datares.Error(0), 2) +
                                          math.pow(mc_func.GetParameter(0) * mcres.Error(0), 2))) / smear

                smear_unc, unc_unc = stretch_unc(data_func.GetParameter(0), datares.Error(0),
                                                 mc_func.GetParameter(0), mcres.Error(0))

                print 'Smear factor: %f +- %f (%f +- %f)' % (smear, smear_err, smear_unc, unc_unc)

#                smear_fit.SetPoint(bin, (rho[2].mean() + rho[3].mean())/2.0, smear)
#                smear_fit.SetPointError(bin, 0, #(rho[2].std() + rho[3].std())/2,
#                                        smear_err)

                smear_fit.SetPoint(bin, (rho[2].mean() + rho[3].mean())/2.0, smear_unc - 1)
                smear_fit.SetPointError(bin, 0, #(rho[2].std() + rho[3].std())/2,
                                        unc_unc)

                for mc_sub in [mc_sub1, mc_sub2]:
                    mc_sub.SetLineWidth(1)
                    mc_sub.SetLineColor(mc_func.GetLineColor())
                    mc_sub.Draw('same')

            else:
                print 'Data at y-axis:', data_func.GetParameter(1)
                print 'MC at y-axis:', mc_func.GetParameter(1)
                print 'Scale factor:', data_func.GetParameter(1)/mc_func.GetParameter(1)

            for ext in ['pdf', 'png', 'C']:
                c1.SaveAs(
                    os.path.join(newdir,
                                 os.path.basename('%s_%s_%s_%i.%s' % (data_func.GetName(), training, bintype, bin, ext))
                                 )
                    )

smear_func = ROOT.TF1('lin', '[0] * x + [1]', 0, 40)

derivative_expr = 'TMath::Sqrt(TMath::Power((x + [2] * [1]) * [3], 2) + TMath::Power(([2] * [0] * x + 1) * [4], 2))'

smear_func_down = ROOT.TF1('down', '[0] * x + [1] - ' + derivative_expr, 0, 40)
smear_func_up = ROOT.TF1('down', '[0] * x + [1] + ' + derivative_expr, 0, 40)

smear_fit_res = smear_fit.Fit(smear_func, 'S')
matrix = smear_fit_res.GetCovarianceMatrix()

big_smear_up = ROOT.TF1('lin', '([0] + [3]) * x + [1] + [4]', 0, 40)
big_smear_down = ROOT.TF1('lin', '([0] - [3]) * x + [1] - [4]', 0, 40)

print matrix(0, 0), matrix(0, 1)
print matrix(1, 0), matrix(1, 1)

for func in [smear_func_down, smear_func_up, big_smear_up, big_smear_down]:
    func.SetParameter(0, smear_func.GetParameter(0))
    func.SetParameter(1, smear_func.GetParameter(1))
    func.SetParameter(2, matrix(0, 1))
    func.SetParameter(3, smear_fit_res.Error(0))
    func.SetParameter(4, smear_fit_res.Error(1))

    func.SetLineColor(2)
    func.SetLineWidth(1)

print smear_func_down.Eval(0)
print smear_func_up.Eval(0)

print smear_func_down.Eval(16)
print smear_func_up.Eval(16)

print smear_func_down.Eval(24)
print smear_func_up.Eval(24)

hide2 = ROOT.TGraph(2)

hide2.SetTitle('Smearing;#rho;#sigma_{smear}')
hide2.SetPoint(0, 0, -0.5)
hide2.SetPoint(1, 35, 0.5)

smear_fit.SetMarkerStyle(8)

c2 = ROOT.TCanvas()
hide2.Draw('ap')
smear_fit.Draw('p,same')
smear_func.Draw('same')
smear_func_down.Draw('same')
smear_func_up.Draw('same')
#big_smear_down.Draw('same')
#big_smear_up.Draw('same')

for ext in ['pdf', 'png', 'C']:
    c2.SaveAs(
        os.path.join(newdir,
                     os.path.basename('smear_fit.%s' % ext)
                     )
        )

os.system('cp %s %s/models.cnf' % (__file__, newdir))
