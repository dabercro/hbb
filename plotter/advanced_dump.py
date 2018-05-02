#! /usr/bin/env python

import os
import sys

mode = sys.argv[1]   # '<' or '>'
limit = int(sys.argv[2])

sys.path.append(os.path.join(os.environ['HOME'], 'CrombieTools/scripts'))
import eventdump

from CrombieTools import LoadConfig

events = []

with open('diff.txt', 'r') as diff_file:
    for line in diff_file:
        if line.startswith(mode):
            events.append(line.split()[1:4])
            limit -= 1
            if not limit:
                break

with open('translating_the_name.txt', 'r') as translation:
    branches = [line.split() for line in translation if not line.startswith('#')]

more_files = ['more_them.txt', 'more_us.txt']

for index, run_lumi_evt in enumerate([['run', 'lumi', 'evt'],
                                      ['runNumber', 'lumiNumber', 'eventNumber']]):
    eventdump.main(
        [os.environ['themfile'],
         os.path.join(os.environ['CrombieInFilesDir'], 'MET.root')
         ][index],
        more_files[index],
        ' || '.join(['(%s)' % ' && '.join(['{branch} == {value}'.format(branch=b, value=v)
                                           for b, v in zip(run_lumi_evt, vals)])
                     for vals in events]),
        *(run_lumi_evt + [b[index] for b in branches]),
        tree=['tree', 'events'][index]
        )

# Now splice files
them_branches = []
us_branches = []

for index, stuff in enumerate(zip(more_files, [them_branches, us_branches])):
    in_file, b_list = stuff
    with open(in_file, 'r') as in_handle:
        b_list.extend(['*'.join(line.split('*')[3-index:-1]) for line in list(in_handle)[3:]])

    b_list.sort()

fmt = ' %20s '
with open('advanced_output.txt', 'w') as output:
    for t, u in list(zip(them_branches, us_branches))[:-1]:
        output.write('*'.join([fmt % e for e in 'run lumi event'.split()] + [fmt % b[1][-20:] for b in branches]) + '\n')
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
