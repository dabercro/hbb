#! /usr/bin/env python

import os
import re
import subprocess

from makeconfig import makeconfig

heldargs = subprocess.check_output(
    'condor_q {0} --constraint \'JobStatus == 5\' -format \'%s\n\' Args'.format(os.environ['USER']),
    shell=True).split('\n')[:-1]

for args in heldargs:
    output_file = re.match(
        r'.*?\s([^\s]*)\"$',
        subprocess.check_output(
            'grep -B1 "%s" condor.cfg' % args, shell=True
            ).split('\n')[0]
        ).group(1)

    print output_file
    if os.path.exists(output_file) and output_file.endswith('.root'):
        os.remove(output_file)

subprocess.check_output('condor_rm {0} --constraint \'JobStatus == 5\''.format(os.environ['USER']),
                        shell=True)
makeconfig(True)
