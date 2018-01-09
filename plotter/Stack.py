#! /usr/bin/env python

import sys

from CrombieTools.PlotTools.PlotStack import plotter, SetupFromEnv, MakePlots, PreparePlots
from CrombieTools.PlotTools import AddOutDir

from array import array
from CrombieTools.LoadConfig import cuts

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

cats = ['ZvvHbb']

def parse_regions(check=None):
    regions = [reg for orig in cuts.regionCuts.keys() for reg in [orig, orig + '+csv']]

    if True in [arg in regions for arg in sys.argv]:
        regions = [r for r in regions if r in sys.argv]
    return [r for r in regions if check is None or r in check]


def parse_plots(check=None):
    plots = [
            ['cmva_hbb_m', 24, 0, 600, 'm_{bb} [GeV]'],
            ['cmva_hbb_pt', 24, 0, 600, 'p_{T,bb} [GeV]'],
            ['cmva_jet1_cmva', 40, -1.0, 1.0, 'CMVA jet 1'],
            ['cmva_jet2_cmva', 40, -1.0, 1.0, 'CMVA jet 2'],
            ['cmva_jet1_qgl', 40, 0, 1.0, 'QGL jet 1'],
            ['cmva_jet2_qgl', 40, 0, 1.0, 'QGL jet 2'],
            ['dphi_uh_cmva', 40, 0, 4, '#Delta#phi(E_{T}^{miss}, jj)'],
            ['csv_hbb_m', 24, 0, 600, 'm_{bb} [GeV]'],
            ['csv_hbb_pt', 24, 0, 600, 'p_{T,bb} [GeV]'],
            ['csv_jet1_csv', 40, 0, 1.0, 'CSV jet 1'],
            ['csv_jet2_csv', 40, 0, 1.0, 'CSV jet 2'],
            ['csv_jet1_qgl', 40, 0, 1.0, 'QGL jet 1'],
            ['csv_jet2_qgl', 40, 0, 1.0, 'QGL jet 2'],
            ['dphi_uh_csv', 40, 0, 4, '#Delta#phi(E_{T}^{miss}, jj)'],
            ['met', 40, 100, 500, 'E_{T}^{miss} [GeV]'],
            ['npv', 40, 0, 80, 'NPV'],
            ['min_dphi_metj_hard', 40, 0, 4, '#Delta#phi(E_{T}^{miss}, j)'],
            ['n_jet', 10, 0, 10, 'Num Jets'],
            ['jet1_pt', 50, 0, 500, 'Jet 1 p_{T} [GeV]'],
            ['jet2_pt', 50, 0, 500, 'Jet 2 p_{T} [GeV]'],
            ['jet3_pt', 40, 0, 400, 'Jet 3 p_{T} [GeV]'],
            ['n_lep_loose', 5, -1, 4, 'Num Loose Leptons'],
            ['n_lep_tight', 5, -1, 4, 'Num Tight Leptons'],
            ['deltaPhi(metphi,trkmetphi)', 40, 0, 4, '#Delta#phi(PFMET, TrkMET)'],
            ['jet1_emfrac', 40, 0, 1.0, 'EM Frac jet 1'],
            ['jet2_emfrac', 40, 0, 1.0, 'EM Frac jet 2'],
            ['jet1_chf', 40, 0, 1.0, 'CHF Frac jet 1'],
            ['jet2_chf', 40, 0, 1.0, 'CHF Frac jet 2'],
            ['jet1_nhf', 40, 0, 1.0, 'NHF Frac jet 1'],
            ['jet2_nhf', 40, 0, 1.0, 'NHF Frac jet 2'],
            ]

    if True in [arg in [p[0] for p in plots] for arg in sys.argv]:
        plots = [p for p in plots if p[0] in sys.argv]

    return [plot for plot in plots if check is None or plot[0] in check]

def submit_plots(regions, plots):
    limithistsdir = 'datacards' if sys.argv[1:] == ['hbbm'] else ''

    # Parse everything one last time so that left plots don't slip through
    MakePlots(cats, parse_regions(regions), [[plot[0], plot[-1]] for plot in parse_plots(plots)], parallel=False)


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
    PreparePlots(cats, parse_regions(), [plot[:4] for plot in parse_plots()])

    if 'debug' in sys.argv:
        exit(0)

    RunPlots(all_left=['jet1_chf',
                       'jet2_chf',
                       'jet1_cmva',
                       'jet2_cmva',
                       'dphi_uh_cmva',
                       'cmva_jet1_cmva'
                       ],
             some_left={'signal': ['jet1_chf',
                                   ],
                        'tt': ['n_jet',
                               ]
                        })
