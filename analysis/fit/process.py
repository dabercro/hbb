#! /usr/bin/env python

import re

# Dictionary of 'file': (sigma, mu)
vals = {}

for line in open('191008.txt', 'r'):
    match = re.match('.*/([^/\.]+)\.root.*sigma\s=\s([\d\.]+).*mean\s=\s([\d\.]+)\s.*', line)
    
    if match:
        vals[match.group(1)] = (float(match.group(2)), float(match.group(3)))

trainings = [
    '',
    '190723_origin_2',
    '190904_0_3',
    '190725_lstm_pf'
    ]

for training in trainings:

    column = []

    for recoilbin in range(3):

        base = 'signal_%i_hbb_m' % recoilbin
        trained = ('%s_%s' % (base, training)).rstrip('_')

        ratio = lambda x: vals[x][0]/vals[x][1]
        line = lambda x: '%.3f (%.3f)' % (ratio(x), 1 - ratio(x)/ratio(base))

        column.extend(['%.3f' % vals[trained][0], '%.3f' % vals[trained][1], line(trained)])

    print ' & '.join(column) + ' \\\\'
