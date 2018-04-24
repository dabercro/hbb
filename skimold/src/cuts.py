#! /usr/bin/env python

from BetterConfigParser import BetterConfigParser

config = BetterConfigParser()
config.read('cuts.ini')

print ' || '.join(
    ['(%s)' % config.get('Cuts', region) for region in ['tt', 'Zlf', 'Zhf', 'Sig']]
    )
