#! /usr/bin/env python

import sys

from CrombieTools.AnalysisTools.XSecAdder import RunXSecAdder, xSecAdder
from CrombieTools.CommonTools.FileConfigReader import SetupConfigFromEnv, SetFunctionFromEnv

if __name__ == '__main__':

    xSecAdder.AddMergeBranch('genWeight')
    xSecAdder.AddMergeBranch('bsf_DeepCSVB')
    xSecAdder.AddMergeBranch('scale_weight_6')

    SetupConfigFromEnv(xSecAdder)
    SetFunctionFromEnv([(xSecAdder.SetNumThreads, 'CrombieNLocalProcs')])

    RunXSecAdder('xsec_weight_low')
