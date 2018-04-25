#! /usr/bin/env python

import os

##

files_per_job = 40
version='v0'

##

def makeconfig(resub=False):

    files = []
    config = []

    with open('files.txt', 'r') as inp:
        files.extend([f.strip() for f in inp])

    with open('condor/tmpl.cfg', 'r') as tmpl:
        config.extend([l.strip().replace('dabercro', os.environ['USER']) for l in tmpl])

    this_dir = os.path.abspath(os.path.dirname(__file__))

    config.extend(['Executable = %s' % os.path.join(this_dir, 'run.sh'),
                   'transfer_input_files = %s' % os.path.join(this_dir, 'src.tar.gz')])

    log_dir = '%s/public_html/logs/skimold/%s' % (os.environ['HOME'], version)
    for d in ['err', 'out']:
        if not os.path.exists(os.path.join(log_dir, d)):
            os.makedirs(os.path.join(log_dir, d))

    out_dir = '/data/t3home000/%s/skimold/%s' % (os.environ['USER'], version)
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)

    n_job = 0
    low = 0
    high = files_per_job
    while low < len(files):
        # Files for this job
        job = files[low:min(high, len(files))]
    
        low = high
        high += files_per_job

        output_file = '%s/%i.root' % (out_dir, n_job)

        if not resub or not os.path.exists(output_file):
            config.extend(['Output = %s/out/%i.out' % (log_dir, n_job),
                           'Error = %s/err/%i.err' %  (log_dir, n_job),
                           'transfer_output_files = output.root',
                           'transfer_output_remaps = "output.root = %s"' % output_file,
                           'Arguments = %s' % ' '.join(job),
                           'Queue'])

        n_job += 1

    name = 'resub.cfg' if resub else 'condor.cfg'
    with open(name, 'w') as condor:
        for l in config:
            condor.write(l + '\n')

if __name__ == '__main__':
    makeconfig()
