#! /usr/bin/env python

import sys

if len(sys.argv) == 1:
    print ' && '.join([
            'met_pt > 170',
            'json != 0',
#            'jet_pt0 > 50',
#            'jet_pt1 > 30',
#            'HCMVAV2_reg_pt > 80',
            ])
    exit(0)

from BetterConfigParser import BetterConfigParser

config = BetterConfigParser()
config.read('cuts.ini')

print ' && '.join(['(%s)' % config.get('Cuts', reg) for reg in sys.argv[1:]])
