#! /usr/bin/env python

import os
import json
import sys

with open(sys.argv[1], 'r') as rawfile:
    outfile = ''
    outhandle = None
    for line in rawfile:

        if 'validate_regression' in line:
            outfile = line.strip().split(',')[1].replace('\'', '').strip(']').lstrip()

        elif line.strip() == 'pt' or line.strip() == 'eta':
            if outhandle:
                outhandle.close()
            outhandle = open(os.path.join('csvs', outfile + '_' + line.strip() + '.csv'), 'w')

        elif ',' in line and outhandle:
            outhandle.write(line)
