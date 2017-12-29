#! /usr/bin/env python

import os
from CrombieTools.SkimmingTools.HistWriter import MakeHist

here = os.path.dirname(__file__)

for hist in ['beff', 'ceff', 'lfeff']:
    MakeHist('%s/%s.txt' % (here, hist), '%s/btag_effs.root' % here, hist)
