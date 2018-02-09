#! /usr/bin/env python

from CrombieTools.PlotTools.QuickPlot import plotter, SetupFromEnv, SetCuts
from CrombieTools.PlotTools import AddOutDir

cat = 'ZvvHbb'

SetupFromEnv()

if __name__ == '__main__':
    plotter.ReadMCConfig('QuickConfig.txt', plotter.kBackground)
    plotter.SetCompareShapes(True)
    plotter.AddBackground('Backgrounds', 1)
    plotter.AddBackground('H #rightarrow bb', 2, plotter.kSignal)
    for reg in ['signal', 'heavyz', 'lightz', 'tt']:
        SetCuts(cat, reg)

        if reg == 'signal':
            plotter.SetLegendLocation(plotter.kUpper, plotter.kLeft)
        else:
            plotter.SetLegendLocation(plotter.kUpper, plotter.kRight)
        plotter.SetDefaultExpr('event_class')
        plotter.MakeCanvas(AddOutDir('quick_%s_event_class' % reg), 20, -0.5, 0.5, 'Event Classifier')

        plotter.SetLegendLocation(plotter.kUpper, plotter.kRight)
        for mass in ['cmva_hbb_m', 'cmva_hbb_m_reg', 'cmva_hbb_m_reg_old']:
            plotter.SetDefaultExpr(mass)
            plotter.MakeCanvas(AddOutDir('quick_%s_%s' % (reg, mass)), 50, 50, 200, 'Dijet Mass')

        plotter.SetDefaultExpr('cmva_jet1_pt_ratio')
        plotter.MakeCanvas(AddOutDir('quick_%s_ratio' % reg), 40, 0.8, 1.6, 'Regression Ratio')
