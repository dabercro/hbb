#! /usr/bin/env python

import os
import glob
import sys

##

indir = '/mnt/hadoop/scratch/dabercro/Xbb'

##

def makeconfig():

    files = []
    config = []

    with open('tmpl.cfg', 'r') as tmpl:
        config.extend([l.strip().replace('dabercro', os.environ['USER']) for l in tmpl])

    this_dir = os.path.abspath(os.path.dirname(__file__))

    config.extend(['Executable = %s' % os.path.join(this_dir, 'run.sh'),
                   'transfer_input_files = %s' % os.path.join(this_dir, 'keep.txt')])

    log_dir = '%s/public_html/logs/xbbslimmer' % os.environ['HOME']
    if not os.path.exists(log_dir):
        os.makedirs(log_dir)

    out_dir = '/data/t3home000/%s/Xbb/' % os.environ['USER']

    for infile in glob.iglob('%s/2*/*/*/*.root' % indir \
                                 if len(sys.argv) == 1 else \
                                 '%s/%s/*/*.root' % (indir, sys.argv[1])):

        output_file = '%s/%s' % (out_dir, '/'.join(infile.split('/')[-4:]))
        output_dir = os.path.dirname(output_file)
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)

        config.extend(['Output = %s/%s.out' % (log_dir, os.path.basename(output_file)),
                       'Error = %s/%s.err' %  (log_dir, os.path.basename(output_file)),
                       'transfer_output_files = output.root',
                       'transfer_output_remaps = "output.root = %s"' % output_file,
                       'Arguments = %s' % infile,
                       'Queue'])

    with open('condor.cfg', 'w') as condor:
        for l in config:
            condor.write(l + '\n')

if __name__ == '__main__':
    makeconfig()
