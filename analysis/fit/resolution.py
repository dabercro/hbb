#! /usr/bin/env python

import os
import sys
import datetime

import ROOT


newdir = os.path.join(
    os.environ['HOME'],
    'public_html/plots',
    '%s_resolution%s' % (
        datetime.date.strftime(
            datetime.datetime.now(), '%y%m%d'
            ),
        sys.argv[1] if len(sys.argv) > 1 else ''
        )
    )

if not os.path.exists(newdir):
    os.mkdir(newdir)
elif len(sys.argv) < 2:
    print newdir, 'exists. Please delete instead of overwriting.'
    exit(1)

alphadir = '/home/dabercro/public_html/plots/190919_alpha'
ratiodir = '/home/dabercro/public_html/plots/190920_smear_weighted'

class MeanCalc(object):

    def __init__(self):
        self.values = []

    def add(self, value, weight):
        self.values.append((value, weight))

    def mean(self):
        tot = 0
        totw = 0
        for val in self.values:
            tot += val[0] * val[1]
            totw += val[1]
        return max(tot/totw, 0)


# Name of region and max alpha value
ranges = [
    ('smearplot_1', 0.185, MeanCalc(), MeanCalc()),
    ('smearplot_2', 0.245, MeanCalc(), MeanCalc()),
    ('smearplot_3', 0.3, MeanCalc(), MeanCalc()),
]
index = 0

average_file = ROOT.TFile(os.path.join(alphadir, 'smearplot_jet2_pt_dilep_corr_pt.root'))
data_hist = average_file.Get("Data")
mc_hist = average_file.Get("DY")
bin = 1

while index != len(ranges):
    ranges[index][2].add(data_hist.GetBinCenter(bin), data_hist.GetBinContent(bin))
    ranges[index][3].add(mc_hist.GetBinCenter(bin), mc_hist.GetBinContent(bin))

    bin += 1
    if data_hist.GetBinCenter(bin) > ranges[index][1]:
        index += 1

trainings = [
    '',
    'jet1_tf_190904_0_2_ptratio_',
    'jet1_tf_190723_origin_ptratio_',
    'jet1_tf_190723_puppi_ptratio_',
    'jet1_tf_190725_lstm_pf_ptratio_',
    ]

for training in trainings:

    index = 0

    data_graph_res = ROOT.TGraph(len(ranges))
    data_graph_res.SetMarkerStyle(8)
    data_graph_res.SetMarkerColor(1)

    mc_graph_res = ROOT.TGraph(len(ranges))
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
                '%s_%s%s.root' % (
                    mean[0], training, 'jet1_pt_dilep_corr_pt'
                    )
                )
            )

        print '-' * 20
        print mean[0]
        print training
        print mean[2].mean(), smearfile.Get("Data").GetStdDev()
        print mean[3].mean(), smearfile.Get("DY").GetStdDev()

        data_hist = smearfile.Get("Data")
        data_mean = mean[2].mean()
        data_graph_res.SetPoint(index, data_mean, data_hist.GetStdDev())
        data_graph_mean.SetPoint(index, data_mean, data_hist.GetMean())

        mc_hist = smearfile.Get("DY")
        mc_mean = mean[3].mean()
        mc_graph_res.SetPoint(index, mc_mean, mc_hist.GetStdDev())
        mc_graph_mean.SetPoint(index, mc_mean, mc_hist.GetMean())

        index += 1

    for data, mc, name, mini, maxi, fit, makesub in [
        (data_graph_res, mc_graph_res, 'resolution', 0.0, 0.4,
         lambda: ROOT.TF1('res', 'TMath::Sqrt(pow([0] * x + [1], 2) + pow([2] * x, 2))', 0, 0.3), True),
        (data_graph_mean, mc_graph_mean, 'mean', 0.7, 1.0,
         lambda: ROOT.TF1('lin', '[0] * x + [1]', 0, 0.3), False)
        ]:

        hide = ROOT.TGraph(2)
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

        datares = data.Fit(data_func)
        mcres = mc.Fit(mc_func)

        hide.Draw('ap')
        data.Draw('p,same')
        mc.Draw('p,same')
        data_func.Draw('same')
        mc_func.Draw('same')

        if makesub:
            data_sub1 = ROOT.TF1('lin', '[0] * x + [1]', 0, 0.3)
            data_sub2 = ROOT.TF1('lin', '[0] * x', 0, 0.3)
            data_sub1.SetParameter(0, data_func.GetParameter(0))
            data_sub1.SetParameter(1, data_func.GetParameter(1))
            data_sub2.SetParameter(0, data_func.GetParameter(2))

            for data_sub in [data_sub1, data_sub2]:
                data_sub.SetLineWidth(1)
                data_sub.SetLineColor(data_func.GetLineColor())
                data_sub.Draw('same')

            mc_sub1 = ROOT.TF1('lin', '[0] * x + [1]', 0, 0.3)
            mc_sub2 = ROOT.TF1('lin', '[0] * x', 0, 0.3)
            mc_sub1.SetParameter(0, mc_func.GetParameter(0))
            mc_sub1.SetParameter(1, mc_func.GetParameter(1))
            mc_sub2.SetParameter(0, mc_func.GetParameter(2))

            for mc_sub in [mc_sub1, mc_sub2]:
                mc_sub.SetLineWidth(1)
                mc_sub.SetLineColor(mc_func.GetLineColor())
                mc_sub.Draw('same')

        for ext in ['pdf', 'png', 'C']:
            c1.SaveAs(
                os.path.join(newdir,
                             os.path.basename('%s_%s.%s' % (name, training.rstrip('*'), ext))
                             )
                )

os.system('cp %s %s/models.cnf' % (__file__, newdir))
