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

# Bkgd only
for process in MakeDatacards.alltrees['background']:
    applicator = Corrector.MakeApplicator('postfit%s' % version, True, 'events', 'events', 10000)
    applicator.SetInDirectory(os.environ['CrombieInFilesDir'])

    def add_corr(name, expr, cut, fileName, histName, matchName='', merge=True):
        corr = Corrector.MakeCorrector(name, expr, cut, fileName, histName)
        if expr == 'cmva_jet2_cmva':
            corr.SetBinning(40, -1.0, 1.0)
        elif expr == 'fatjet1_mSD_corr':
            corr.SetBinning(40, 0, 200)
        else:
            corr.SetBinning(40, -0.5, 1.5)

        if matchName:
            corr.SetMatchFileName(matchName)
        corr.Merge = merge
        applicator.AddCorrector(corr)


    for cat in ['boosted', 'inclusive']:
        for region in ['tt', 'heavyz', 'lightz', 'signal']:
            _region = '%s__%s' % (cat, region)
            add_corr('%s_%s%s' % (_region, process, version),
                     MakeDatacards.expr[cat].get(region, MakeDatacards.expr[cat]['default']),
                     cuts.cut(cat, region), rootfile,
                     ['shapes_fit_b/%s/%s' % (_region, process),
                      'shapes_prefit/%s/%s' % (_region, process)])

    if __name__ == '__main__':
        RunParallel(applicator, 'RunOnFile', [[f] for f in FileLists[process]])

# Put placeholder into Signal samples
for process in MakeDatacards.alltrees['signal']:
    applicator = Corrector.MakeApplicator('post_fit%s' % version, True, 'events', 'events', 10000)
    applicator.SetInDirectory(os.environ['CrombieInFilesDir'])
    if __name__ == '__main__':
        RunParallel(applicator, 'RunOnFile', [[f] for f in FileLists[process]])
