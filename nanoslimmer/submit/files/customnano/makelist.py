#! /usr/bin/env python

import subprocess

##

directory = '/store/user/paus/nanoao/503'
door = 'xrootd.cmsaf.mit.edu'

##

files = []

def get_subs(paths):
    output = []
    for path in paths:
        if path.startswith(directory):
            if path.endswith('.root'):
                files.append(path)
            elif '/tmp_' not in path:
                print 'Checking', path
                output.extend(subprocess.check_output('xrdfs %s ls %s' % (door, path), shell=True).split('\n')[:-1])
    if output:
        get_subs(output)

get_subs([directory])

with open('files.txt', 'w') as output:
    for f in files:
        output.write(f + '\n')
