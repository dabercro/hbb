#! /usr/bin/env python

import glob
import os
import datetime
import sys

sys.argv.append('-b')
import ROOT

from HelperPython import Bukin


newdir = os.path.join(
    os.environ['HOME'],
    'public_html/plots',
    '%s_fit' % datetime.date.strftime(datetime.datetime.now(), '%y%m%d')
)

if not os.path.exists(newdir):
    os.mkdir(newdir)

indir = '/home/dabercro/public_html/plots/200818_Zll2018_runplot-v1'
title = ';m_{jj} [GeV];Events/10 GeV'

for jets in ['__', '_0j__', '_1j__', '_ge1j__', '_ge2j__']:
    for pt in ['low', 'medhigh', 'med', 'high']:

        functions = []

        plots = [('Kinematic Fit', 'Hmass_kinfit_'), ('With Regression', 'Hmass_'), ('Without Regression', 'Hmass_noReg_')]
        if pt == 'high':
            plots.append(('Fat Jet', 'FatJet_mSD_'))

        for label, plot in plots:

            filename = '{indir}/SR_baser_{pt}_Zll{jets}{plot}.shapes.root'.format(indir=indir, jets=jets, pt=pt, plot=plot)

            infile = ROOT.TFile(filename)

            c1 = ROOT.TCanvas()

            histogram = infile.summedSignalHistograms

            plotMean   = histogram.GetMean()
            plotRMS    = histogram.GetRMS()
            plotRes    = float(plotRMS)/float(plotMean)
            plotHeight = histogram.GetMaximum()

            function = ROOT.TF1(title, Bukin(), 50, 200, 6)
            function.SetParName(0, "height")
            function.SetParName(1, "mean")
            function.SetParName(2, "width")
            function.SetParName(3, "asymmetry")
            function.SetParName(4, "size of lower tail")
            function.SetParName(5, "size of higher tail")
            function.SetParameter(0, float(plotHeight))
            function.SetParameter(1, float(plotMean))
            function.SetParameter(2, float(plotRMS))
            function.SetParameter(3, -0.2 )
            function.SetParameter(4, 0.2)
            function.SetParameter(5, 0.001)

            functions.append((label, function))

            result = histogram.Fit(title, "SML", "histsame")

            fittedHist = histogram.GetFunction(title)
            fittedHist.Draw("histsame")

            for ext in ['.pdf', '.png', '.C']:
                c1.SaveAs(
                    os.path.join(newdir,
                                 os.path.basename(filename).replace('.root', ext)
                                 )
                    )

            print filename, '#sigma = %f +- %f' % (function.GetParameter(2), result.Error(2)), 'mean = %f +- %f' % (function.GetParameter(1), result.Error(1))
            print filename, '#sigma/#mu = %f' % (function.GetParameter(2)/function.GetParameter(1))

        test = ROOT.TCanvas()
        legend = ROOT.TLegend(0.15, 0.875, 0.4, 0.675) if pt != 'low' else \
            ROOT.TLegend(0.9, 0.875, 0.65, 0.675)

        # If more than two jets, draw without regression first
        if pt == 'high':
            functions[3][1].Draw()

        elif jets == '_ge2j__':
            functions[2][1].Draw()

        elif pt == 'low':
            functions[1][1].Draw()

        for idx, stuff in enumerate(functions):

            label, function = stuff

            function.SetLineColor(idx + 1)
            if idx or jets == '_ge2j__' or pt == 'low':
                function.Draw('same')
            else:
                function.Draw()

            legend.AddEntry(function, label, 'L')

        legend.Draw()
        outbase = 'fits_SR_{pt}_Hmass{jets}'.format(pt=pt, jets=jets)

        for ext in ['.pdf', '.png', '.C']:
            test.SaveAs(
                os.path.join(newdir,
                             outbase + ext)
                )


os.system('cp %s %s/models.cnf' % (__file__, newdir))
