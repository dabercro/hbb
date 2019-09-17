#! /usr/bin/env python

import os
import datetime

import ROOT


newdir = os.path.join(
    os.environ['HOME'],
    'public_html/plots',
    '%s_resolution' % datetime.date.strftime(
        datetime.datetime.now(), '%y%m%d'
        )
    )

if not os.path.exists(newdir):
    os.mkdir(newdir)

alphadir = '/home/dabercro/public_html/plots/190917_alpha'
ratiodir = '/home/dabercro/public_html/plots/190916_alphacuts2'

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
        return tot/totw


# Name of region and max alpha value
ranges = [
    ('smearplot_0_to_p1', 0.1, MeanCalc(), MeanCalc()),
    ('smearplot_p1_to_p15', 0.15, MeanCalc(), MeanCalc()),
    ('smearplot_p15_to_p2', 0.2, MeanCalc(), MeanCalc()),
    ('smearplot_p2_to_p25', 0.25, MeanCalc(), MeanCalc()),
    ('smearplot_p25', 0.3, MeanCalc(), MeanCalc())
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
    'jet1_tf_190904_0_2_ptratio*',
    'jet1_tf_190723_origin_ptratio*',
    'jet1_tf_190723_puppi_ptratio*',
    ]

for training in trainings:

    index = 0

    data_graph = ROOT.TGraph(len(ranges))
    data_graph.SetMarkerStyle(8)
    data_graph.SetMarkerColor(2)

    mc_graph = ROOT.TGraph(len(ranges))
    mc_graph.SetMarkerStyle(8)
    mc_graph.SetMarkerColor(1)

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

        data_graph.SetPoint(index, mean[2].mean(), smearfile.Get("Data").GetStdDev())
        mc_graph.SetPoint(index, mean[3].mean(), smearfile.Get("DY").GetStdDev())

        index += 1

    c1 = ROOT.TCanvas()
    data_graph.Draw('')
    mc_graph.Draw('lp,same')

    for ext in ['pdf', 'png', 'C']:
        c1.SaveAs(
            os.path.join(newdir,
                         os.path.basename('resolution_%s.%s' % (training.rstrip('*'), ext))
                         )
            )
