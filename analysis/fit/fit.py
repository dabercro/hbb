#! /usr/bin/env python

import glob
import os
import datetime

import ROOT

from HelperPython import Bukin


newdir = os.path.join(
    os.environ['HOME'],
    'public_html/plots',
    '%s_fit' % datetime.date.strftime(datetime.datetime.now(), '%y%m%d')
)

if not os.path.exists(newdir):
    os.mkdir(newdir)


for filename in sorted(glob.glob('/home/dabercro/public_html/plots/191008_dijet_recoilbin2/*.root')):

    infile = ROOT.TFile(filename)

    histogram = infile.ZH

    test = ROOT.TCanvas()

    plotMean   = histogram.GetMean()
    plotRMS    = histogram.GetRMS()
    plotRes    = float(plotRMS)/float(plotMean)
    plotHeight = histogram.GetMaximum()
    function = ROOT.TF1("bukin", Bukin(), 80, 160, 6)
    function.SetParName(0, "height")
    function.SetParName(1, "mean")
    function.SetParName(2,"width")
    function.SetParName(3,"asymmetry")
    function.SetParName(4,"size of lower tail")
    function.SetParName(5,"size of higher tail")
    function.SetParameter(0, float(plotHeight))
    function.SetParameter(1, float(plotMean))
    function.SetParameter(2, float(plotRMS))
    function.SetParameter(3, -0.2 )
    function.SetParameter(4, 0.2)
    function.SetParameter(5, 0.001)
    result = histogram.Fit("bukin", "S", "histsame")
    fittedHist = histogram.GetFunction("bukin")
    fittedHist.Draw("histsame")

    for ext in ['.pdf', '.png', '.C']:
        test.SaveAs(
            os.path.join(newdir,
                         os.path.basename(filename).replace('.root', ext)
                         )
            )

    print filename, '#sigma = %f +- %f' % (function.GetParameter(2), result.Error(2)), 'mean = %f +- %f' % (function.GetParameter(1), result.Error(1))
