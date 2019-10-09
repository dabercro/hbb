#! /usr/bin/env python

import os
import json
import sys

import datetime

outdir = 'csvs_%s' % datetime.date.strftime(datetime.datetime.now(), '%y%m%d')

if not os.path.exists(outdir):
    os.makedirs(outdir)

with open(sys.argv[1], 'r') as rawfile:
    outfile = ''
    outhandle = None
    for line in rawfile:

        if 'validate_regression' in line:
            outfile = line.strip().split(',')[1].replace('\'', '').strip(']').lstrip()

        elif line.strip() == 'pt' or line.strip() == 'eta':
            if outhandle:
                outhandle.close()
            outhandle = open(os.path.join(outdir, outfile + '_' + line.strip() + '.csv'), 'w')

        elif ',' in line and outhandle:
            outhandle.write(line)
