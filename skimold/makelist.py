#! /usr/bin/env python

import subprocess

files = []

def get_subs(paths):
    output = []
    for path in paths:
        if path.startswith('/store/user/lpchbb/HeppyNtuples/V25/MET'):
            if path.endswith('.root'):
                files.append(path)
            else:
                output.extend(subprocess.check_output('xrdfs cmsxrootd-site1.fnal.gov:1092 ls %s' % path, shell=True).split('\n')[:-1])
    if output:
        get_subs(output)

get_subs(['/store/user/lpchbb/HeppyNtuples/V25/MET'])

with open('files.txt', 'w') as output:
    for f in files:
        output.write(f + '\n')
