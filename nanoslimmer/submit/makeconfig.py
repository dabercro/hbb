#! /usr/bin/env python

import os
import glob

##

files_per_job = 3
version='v0'
door='root://cms-xrd-global.cern.ch/'

##

def makeconfig(resub=False):

    config = []

    this_dir = os.path.abspath(os.path.dirname(__file__))

    with open(os.path.join(this_dir, 'tmpl.cfg'), 'r') as tmpl:
        config.extend([l.strip().replace('dabercro', os.environ['USER']) for l in tmpl])

    config.extend(['Executable = %s' % os.path.join(this_dir, 'run.sh'),
                   'transfer_input_files = %s' % os.path.join(os.environ['CMSSW_BASE'], 'nano.tgz')])

    log_dir = '%s/public_html/logs/nano/%s' % (os.environ['HOME'], version)
    for d in ['err', 'out']:
        if not os.path.exists(os.path.join(log_dir, d)):
            os.makedirs(os.path.join(log_dir, d))

    out_dir = '/data/t3home000/%s/nano/%s' % (os.environ['USER'], version)

    for file_list in glob.glob(os.path.join(this_dir, 'files', '*.txt')):

        n_job = 0

        this_out = os.path.join(out_dir, os.path.basename(file_list).split('.')[0])

        if not os.path.exists(this_out):
            os.makedirs(this_out)

        with open(file_list, 'r') as files_file:
            files = [line.strip() for line in files_file if line.strip()]

        low = 0
        high = files_per_job
        while low < len(files):
            # Files for this job
            job = ['%s%s' % (door, f) for f in files[low:min(high, len(files))]]
    
            low = high
            high += files_per_job

            output_file = '%s/%i.root' % (this_out, n_job)

            if not resub or not os.path.exists(output_file):
                config.extend(['Output = %s/out/%s_%i.out' % (log_dir, os.path.basename(this_out), n_job),
                               'Error = %s/err/%s_%i.err' %  (log_dir, os.path.basename(this_out), n_job),
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
