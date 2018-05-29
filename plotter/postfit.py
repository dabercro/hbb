#! /usr/bin/env python

import os
import sys

import MakeDatacards

from CrombieTools.Parallelization import RunParallel
from CrombieTools.SkimmingTools import Corrector

from CrombieTools.LoadConfig import cuts
from CrombieTools.ConfigTools import FileLists

rootfile = 'fitDiagnostics.root'
version = sys.argv[1] if len(sys.argv) > 1 else ''
out_branch = 'postfit%s' % version

# Bkgd only
for process in MakeDatacards.alltrees['background']:
    applicator = Corrector.MakeApplicator(out_branch, True, 'events', 'events', 10000)
    applicator.SetInDirectory(os.environ['CrombieInFilesDir'])

    def add_corr(name, expr, cut, fileName, histName, matchName='', merge=True):
        corr = Corrector.MakeCorrector(name, expr, cut, fileName, histName)
        if expr == 'cmva_jet2_cmva':
            corr.SetBinning(37, -0.6, 1.05)
        elif expr == 'fatjet1_mSD_corr':
            corr.SetBinning(24, 80, 160)
        else:
            corr.SetBinning(36, -0.8, 1.0)

        if matchName:
            corr.SetMatchFileName(matchName)
        corr.Merge = merge
        applicator.AddCorrector(corr)


    for cat in [
#        'boosted',
        'inclusive',
        ]:
        for region in ['tt', 'heavyz', 'lightz', 'signal']:
            _region = '%s__%s' % (cat, region)
            try:
                add_corr('%s_%s%s' % (_region, process, version),
                         MakeDatacards.expr[cat].get(region, MakeDatacards.expr[cat]['default']),
                         cuts.cut(cat, region), rootfile,
                         ['shapes_fit_b/%s/%s' % (_region, process),
                          'shapes_prefit/%s/%s' % (_region, process)])
            except:
                pass

    if __name__ == '__main__':
        RunParallel(applicator, 'RunOnFile', [[f] for f in FileLists[process]])

# Put placeholder into Signal samples
for process in MakeDatacards.alltrees['signal']:
    applicator = Corrector.MakeApplicator(out_branch, True, 'events', 'events', 10000)
    applicator.SetInDirectory(os.environ['CrombieInFilesDir'])
    if __name__ == '__main__':
        RunParallel(applicator, 'RunOnFile', [[f] for f in FileLists[process]])
