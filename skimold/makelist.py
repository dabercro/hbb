#! /usr/bin/env python

import subprocess

##

directory = '/store/user/lpchbb/HeppyNtuples/V25/MET'
door = 'cmsxrootd-site1.fnal.gov:1092'

##

files = []

def get_subs(paths):
    output = []
    for path in paths:
        if path.startswith(directory):
            if path.endswith('.root'):
                files.append(path)
            else:
                output.extend(subprocess.check_output('xrdfs %s ls %s' % (door, path), shell=True).split('\n')[:-1])
    if output:
        get_subs(output)

get_subs([directory])

with open('files.txt', 'w') as output:
    for f in files:
        output.write(f + '\n')
