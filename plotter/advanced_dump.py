#! /usr/bin/env python

import os
import sys

mode = sys.argv[1]   # '<' or '>'

sys.path.append(os.path.join(os.environ['HOME'], 'CrombieTools/scripts'))
import eventdump

limit = 15
events = []

with open('diff.txt', 'r') as diff_file:
    for line in diff_file:
        if line.startswith(mode):
            events.append(line.split()[1:4])
            limit -= 1
            if not limit:
                break

with open('translating_the_name.txt', 'r') as translation:
    branches = [line.split() for line in translation]

more_files = ['more_them.txt', 'more_us.txt']

for index, run_lumi_evt in enumerate([['run', 'lumi', 'evt'],
                                      ['runNumber', 'lumiNumber', 'eventNumber']]):
    eventdump.main(
        ['/data/t3home000/dabercro/skimold/out/v1.root',
         '/data/t3home000/dabercro/hbb/180426_v0/MET.root'
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
        output.write('*'.join([fmt % e for e in 'run lumi event'.split()] + [fmt % b[1] for b in branches]) + '\n')
        output.write('%s\n%s\n' % (t, u))
