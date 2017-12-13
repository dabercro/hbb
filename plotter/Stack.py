#! /usr/bin/env python

import sys

from CrombieTools.PlotTools.PlotStack import plotter, SetupFromEnv, MakePlots
from array import array
import cuts

plotter.SetAllHistName('')

SetupFromEnv()

plotter.SetAddSignal(True)
plotter.SetCMSLabel('Preliminary')
plotter.SetTreeName('events')
plotter.SetMinLegendFrac(0.03)
plotter.SetIgnoreInLinear(0.005)
plotter.SetRatioMinMax(0.5, 1.5)
plotter.SetOthersColor(922)
plotter.SetRatioTitle('Data/Pred.')
plotter.SetRatioDivisions(504, False)
plotter.SetRatioGrid(1)
plotter.SetCanvasSize(600, 700)
plotter.SetFontSize(0.03)
plotter.SetAxisTitleOffset(1.55)

plotter.AddDataFile('MET.root')


def parse_regions(check=None):
    regions = cuts.regionCuts.keys()

    if True in [arg in regions for arg in sys.argv]:
        regions = [r for r in regions if r in sys.argv]
    return [r for r in regions if check is None or r in check]


def parse_plots(check=None):
    plots = [
            ['hbbm', 24, 0, 600, 'm_{bb} [GeV]'],
            ['hbbpt', 24, 0, 600, 'p_{T,bb} [GeV]'],
            ['pfmet', 40, 100, 500, 'E_{T}^{miss} [GeV]'],
            ['npv', 40, 0, 80, 'NPV'],
            ['dphipfmet', 40, 0, 4, '#Delta#phi(E_{T}^{miss}, j)'],
            ['nJet', 10, 0, 10, 'Num Jets'],
            ['jet1Pt', 50, 0, 500, 'Jet 1 p_{T} [GeV]'],
            ['jet2Pt', 50, 0, 500, 'Jet 2 p_{T} [GeV]'],
            ['jetPt[2]', 40, 0, 400, 'Jet 3 p_{T} [GeV]'],
            ['jetCSV[hbbjtidx[0]]', 40, 0, 1.0, 'CSV jet 1'],
            ['jetCSV[hbbjtidx[1]]', 40, 0, 1.0, 'CSV jet 2'],
            ['nLooseLep', 5, -1, 4, 'Num Loose Leptons'],
            ['nTightLep', 5, -1, 4, 'Num Tight Leptons'],
            ['deltaPhi(pfmetphi,hbbphi)', 40, 0, 4, '#Delta#phi(E_{T}^{miss}, jj)'],
            ['jet1IsTight', 5, -1, 4, 'Some Jet ID'],
            ['jetEMFrac[hbbjtidx[0]]', 40, 0, 1.0, 'EM Frac jet 1'],
            ['jetEMFrac[hbbjtidx[1]]', 40, 0, 1.0, 'EM Frac jet 1'],
            ['jetHadFrac[hbbjtidx[0]]', 40, 0, 1.0, 'Hadron Frac jet 1'],
            ['jetHadFrac[hbbjtidx[1]]', 40, 0, 1.0, 'Hadron Frac jet 1'],
            ]

    if True in [arg in [p[0] for p in plots] for arg in sys.argv]:
        plots = [p for p in plots if p[0] in sys.argv]

    return [plot for plot in plots if check is None or plot[0] in check]


def submit_plots(regions, plots):
    cats = ['ZvvHbb']
    # Parse everything one last time so that left plots don't slip through
    MakePlots(cats, parse_regions(regions), parse_plots(plots))


def RunPlots(all_left, some_left):
    regions = parse_regions()
    all_right = [plot[0] for plot in parse_plots() if plot[0] not in sum([all_left] + some_left.values(), [])]

    plotter.SetLegendLocation(plotter.kUpper, plotter.kRight, 0.25, 0.5)
    submit_plots(regions, all_right)
    for key in some_left:
        submit_plots([reg for reg in regions if key not in reg], some_left[key])

    plotter.SetLegendLocation(plotter.kUpper, plotter.kLeft, 0.25, 0.5)
    submit_plots(regions, all_left)
    for key in some_left:
        submit_plots([reg for reg in regions if key in reg], some_left[key])


if __name__ == '__main__':
    RunPlots(all_left=['jetCSV[hbbjtidx[0]]',
                       'deltaPhi(pfmetphi,hbbphi)'
                       ],
             some_left={'tt': ['jetCSV[hbbjtidx[1]]',
                               ],
                        'multijet': ['jet1Pt',
                                     ],
                        })
