#! /usr/bin/env python

import os

##

files_per_job = 20

##

files = []
config = []

with open('files.txt', 'r') as inp:
    files.extend([f.strip() for f in inp])

with open('condor/tmpl.cfg', 'r') as tmpl:
    config.extend([l.strip() for l in tmpl])

this_dir = os.path.abspath(os.path.dirname(__file__))

config.extend(['Executable = %s' % os.path.join(this_dir, 'run.sh'),
               'transfer_input_files = %s' % os.path.join(this_dir, 'src.tar.gz')])

n_job = 0
low = 0
high = files_per_job
while low < len(files):
    # Files for this job
    job = files[low:min(high, len(files))]
    
    low = high
    high += files_per_job

    config.extend(['Output = %s/public_html/logs/skimold/out/%i.out' % (os.environ['HOME'], n_job),
                   'Error = %s/public_html/logs/skimold/err/%i.err' %  (os.environ['HOME'], n_job),
                   'transfer_output_files = output.root',
                   'transfer_output_remaps = "output.root = /data/t3home000/%s/skimold/%i.root"' % (os.environ['USER'], n_job),
                   'Arguments = %s' % ' '.join(job),
                   'Queue'])

    n_job += 1

with open('condor.cfg', 'w') as condor:
    for l in config:
        condor.write(l + '\n')
