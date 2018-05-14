#! /usr/bin/env python

import os
import sys

from CrombieTools.PlotTools.PlotStack import plotter, SetupFromEnv, MakePlots, PreparePlots
from CrombieTools.PlotTools import AddOutDir

from array import array
from CrombieTools.LoadConfig import cuts

plotter.SetAllHistName('hSumW')

SetupFromEnv()

plotter.SetAddSignal(True)
plotter.SetCMSLabel('Preliminary')
plotter.SetTreeName('events')
plotter.SetMinLegendFrac(0.03)
plotter.SetIgnoreInLinear(0.005)
plotter.SetRatioMinMax(0.5, 2.0)
plotter.SetOthersColor(922)
plotter.SetRatioTitle('Data/Pred.')
plotter.SetRatioDivisions(504, False)
plotter.SetRatioGrid(1)
plotter.SetCanvasSize(600, 700)
plotter.SetFontSize(0.03)
plotter.SetAxisTitleOffset(1.55)

plotter.AddDataFile('MET.root')

do_limit_dump = os.environ.get('syst')
cats = cuts.categoryCuts.keys()

system = ''

region = ''

for args in sys.argv:
    if '=' in args:
        com, val = args.split('=')
        if com == 'sys':
            system = val
        if com == 'region':
            region = val


def parse_regions(check=None):
    if region:
        return [region]

    regions = ['signal', 'heavyz', 'lightz', 'tt']

    if do_limit_dump:
        new_regions = [key for key in cuts.regionCuts.keys() if True in [key.startswith(reg) for reg in regions]]
        return new_regions

    if True in [arg in regions for arg in sys.argv]:
        regions = [r for r in regions if r in sys.argv]

    if system:
        regions = ['%s_%s' % (r, system) for r in regions]

    return [r for r in regions if check is None or r in check]


def parse_plots(check=None):
    plots = [
            ['cmva_hbb_m', 24, 0, 600, 'm_{bb} [GeV]'],
            ['cmva_hbb_pt', 24, 0, 600, 'p_{T,bb} [GeV]'],
            ['cmva_jet1_cmva', 40, -1.0, 1.0, 'CMVA jet 1'],
            ['cmva_jet2_cmva', 37, -0.6, 1.05, 'CMVA jet 2'],
#            ['cmva_jet2_cmva', 36, -0.6, 1.0, 'CMVA jet 2'],
            ['cmva_dphi_uh', 40, 0, 4, '#Delta#phi(E_{T}^{miss}, jj)'],
            ['pfmet', 40, 100, 500, 'E_{T}^{miss} [GeV]'],
            ['npv', 40, 0, 80, 'NPV'],
            ['min_dphi_metj_hard', 40, 0, 4, '#Delta#phi(E_{T}^{miss}, j)'],
            ['n_jet', 10, 0, 10, 'Num Jets'],
            ['n_soft_2', 20, 0, 20, 'Num Soft'],
            ['n_soft_5', 20, 0, 20, 'Num Soft'],
            ['n_soft_10', 20, 0, 20, 'Num Soft'],
            ['ak8fatjet1_n_iso', 10, 0, 10, 'Num Jets'],
            ['ak8fatjet1_n_iso_b', 10, 0, 10, 'Num Jets'],
            ['cmva_jet1_pt', 50, 0, 500, 'Jet 1 p_{T} [GeV]'],
            ['cmva_jet2_pt', 50, 0, 500, 'Jet 2 p_{T} [GeV]'],
            ['cmva_jet1_eta', 30, -2.5, 5, 'Jet 1 #eta [GeV]'],
            ['cmva_jet2_eta', 30, -2.5, 5, 'Jet 2 #eta [GeV]'],
            ['cmva_daughter_dR', 40, 0, 6.0, '#Delta R_{jj}'],
            ['maier_event_class', 42, -1.05, 1.05, 'Event Classifier'],
            ['ak8fatjet1_mSD_corr', 24, 80, 160, 'fat jet m_{SD}'],
            ['ak8fatjet1_pt', 30, 0, 600, 'fat jet p_{T}'],
            ['ak8fatjet1_double_sub', 42, -1.05, 1.05, 'fat jet double sub'],
            ]

    if True in [arg in [p[0] for p in plots] for arg in sys.argv]:
        plots = [p for p in plots if p[0] in sys.argv]

    for plot in plots:
        plot[0] += system

    return [plot for plot in plots if check is None or plot[0] in check]

def submit_plots(regions, plots):
    limithistsdir = 'datacards/plots' if do_limit_dump else ''

    # Parse everything one last time so that left plots don't slip through
    MakePlots(cats, parse_regions(regions), [[plot[0], plot[-1]] for plot in parse_plots(plots)], limitHistsDir=limithistsdir,
              parallel=False, systematics=cuts.syst)


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
    PreparePlots(cats, parse_regions(), [plot[:4] for plot in parse_plots()],
                 systematics=cuts.syst, envelope=cuts.env)

    if 'debug' in sys.argv:
        exit(0)

    RunPlots(all_left=['jet1_chf',
                       'jet2_chf',
                       'jet1_cmva',
                       'jet2_cmva',
                       'cmva_dphi_uh',
                       'cmva_jet1_cmva'
                       ],
             some_left={'signal': ['jet1_chf',
                                   'min_dphi_metj_hard',
                                   'event_class'
                                   ],
                        'tt': ['n_jet',
                               ]
                        })

    print 'Finished'
