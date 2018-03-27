#! /usr/bin/env python

from CrombieTools.AnalysisTools import XSecAdder
from CrombieTools.CommonTools.FileConfigReader import SetupConfigFromEnv, SetFunctionFromEnv

XSecAdder.xSecAdder.SetKeepAllFiles(False)
XSecAdder.xSecAdder.SetAllHistName('hSumW')

SetupConfigFromEnv(XSecAdder.xSecAdder)
SetFunctionFromEnv([(XSecAdder.xSecAdder.SetNumThreads, 'CrombieNLocalProcs')])

XSecAdder.RunXSecAdder('xsec1')
