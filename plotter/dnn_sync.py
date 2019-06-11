#! /usr/bin/env python

import os
import sys

limit = int(sys.argv[1])

sys.path.append(os.path.join(os.environ['HOME'], 'CrombieTools/scripts'))
import eventdump

from CrombieTools import LoadConfig

indir = '/data/t3home000/bmaier/CMSSW_9_4_6/src/PandaAnalysis/Flat/test'
input_name = os.path.join(indir, 'common.csv')

events = []

with open(input_name, 'r') as diff_file:
    for line in diff_file:
        if line[0].isdigit():
            events.append(line.split('.'))
            limit -= 1
            if not limit:
                break

with open(os.path.join(indir, 'translation.txt'), 'r') as translation:
    branches = [line.split()[0:2] for line in translation if not line.startswith('#')]

root_files = ['testskim.root', 'ttbar.root']
out_files = ['pexpr.txt', 'nanoaod.txt']

for index, run_lumi_evt in enumerate([['eventNumber'],
                                      ['event']]):
    eventdump.main(os.path.join(indir, root_files[index]),
                   out_files[index],
                   ['(%s) && (jotFlav==5&&fabs(jotEta)<2.5)', '(%s)'][index] %
                   ' || '.join(['(%s)' % ' && '.join(['{branch} == {value}'.format(branch=b, value=v)
                                                      for b, v in zip(run_lumi_evt, vals)])
                                for vals in events]),
                   *(run_lumi_evt + [b[1 - index] for b in branches]),
                   tree=['events', 'jets'][index]
        )

# Now splice files
us_branches = []
them_branches = []

start = 3

for in_file, b_list in zip(out_files, [us_branches, them_branches]):
    with open(in_file, 'r') as in_handle:
        b_list.extend(['*'.join(line.split('*')[start:-1]) for line in list(in_handle)[3:]])

    start -= 1
    b_list.sort()

fmt = ' %20s '
with open('dnn_output_v2.txt', 'w') as output:
    for t, u in list(zip(them_branches, us_branches))[:-1]:
        output.write('*'.join([fmt % 'event'] + [fmt % b[1][-20:] for b in branches]) + '\n')
        for t_val, u_val in zip(t.split('*'), u.split('*')):
            try:
                t_float = float(t_val.strip())
                u_float = float(u_val.strip())
                if abs(t_float - u_float)/(abs(t_float) + abs(u_float) + 0.2) > 0.1 and t_float:
                    t = t.replace(t_val, '%s#' % t_val[:-1])
                    u = u.replace(u_val, '%s#' % u_val[:-1])
            except:
                pass

        output.write('%s\n%s\n' % (t, u))
