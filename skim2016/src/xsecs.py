#! /usr/bin/env python

import sys
import re
from BetterConfigParser import BetterConfigParser

config = BetterConfigParser()
config.read('samples_nosplit.ini')

for arg in sys.argv[1:]:
    expr = config.get(arg, 'xSec').replace('[', '(').replace(']', ')')
    # Make sure input is sanitary
    if re.match(r'[\d\(\)\=\-\*\/\.]*', expr):
        print eval(expr)
