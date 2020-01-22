#! /usr/bin/env python

import os
import glob
import shutil

##

year='2018'

exe='smearnano'
#exe='hbbnano'
version='200122_%s_%s' % (exe, year)

use_custom = False
files_per_job = 1

##

if use_custom:
    version += '_custom'

door='root://xrootd.cmsaf.mit.edu/' if use_custom else 'root://cms-xrd-global.cern.ch/'

def makeconfig():

    config = []

    this_dir = os.path.abspath(os.path.dirname(__file__))

    with open(os.path.join(this_dir, 'tmpl.cfg'), 'r') as tmpl:
        config.extend([l.strip().replace('dabercro', os.environ['USER']) for l in tmpl])

    log_dir = '%s/public_html/logs/nano/%s/%s' % (os.environ['HOME'], exe, version)
    if not os.path.exists(log_dir):
        os.makedirs(log_dir)

    out_dir = '/data/t3home000/%s/nano/%s/%s' % (os.environ['USER'], exe, version)
    tar_dir = '/data/t3home000/%s/nano/tars/%s/%s' % (os.environ['USER'], exe, version)

    tarfile = 'nano.tgz'
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)

    if not os.path.exists(tar_dir):
        os.makedirs(tar_dir)
        shutil.copy(os.path.join(os.environ['CMSSW_BASE'], tarfile), tar_dir)
        shutil.copy('run.sh', tar_dir)

    config.extend(['Executable = %s' % os.path.join(tar_dir, 'run.sh'),
                   'transfer_input_files = %s' % os.path.join(tar_dir, tarfile)])

    n_job = 0

    for file_list in glob.glob(os.path.join(this_dir, 'files/customnano' if use_custom else 'files',
                                            exe, year, '*.txt')):

        this_out = os.path.join(out_dir, os.path.basename(file_list).split('.')[0])

        if not os.path.exists(this_out):
            os.makedirs(this_out)

        with open(file_list, 'r') as files_file:
            files = [line.strip() for line in files_file if line.strip()]

        low = 0
        high = files_per_job

        name = 'condor.cfg'

        while low < len(files):
            # Files for this job
            job = ['%s%s' % (door, f) for f in files[low:min(high, len(files))]]
    
            low = high
            high += files_per_job

            output_file = '%s/%i.root' % (this_out, n_job)

            if not os.path.exists(output_file) or not os.stat(output_file).st_size:
                config.extend(['Output = %s/%s_%i.out' % (log_dir, os.path.basename(this_out), n_job),
                               'Error = %s/%s_%i.err' %  (log_dir, os.path.basename(this_out), n_job),
                               'transfer_output_files = output.root',
                               'transfer_output_remaps = "output.root = %s"' % output_file,
                               'Arguments = %s %s %s' % (exe, year, ' '.join(job)),
                               'Queue'])

            else:
                name = 'resub.cfg'

            n_job += 1

    with open(name, 'w') as condor:
        for l in config:
            condor.write(l + '\n')

    shutil.copy(name, tar_dir)

if __name__ == '__main__':
    makeconfig()
