#! /usr/bin/env python

import sys

from CrombieTools.AnalysisTools.XSecAdder import RunXSecAdder, xSecAdder
from CrombieTools.CommonTools.FileConfigReader import SetupConfigFromEnv, SetFunctionFromEnv

from CrombieTools.SkimmingTools.FormulaCorrector import MakeFormulaCorrector
from CrombieTools.SkimmingTools.Corrector import MakeApplicator
from CrombieTools.Parallelization import RunOnDirectory

#    <weight id="1" MUR="1" MUF="1" PDF="306000"> mur=1 muf=1 </weight>
#    <weight id="2" MUR="1" MUF="2" PDF="306000"> mur=1 muf=2 </weight>
#    <weight id="3" MUR="1" MUF="0.5" PDF="306000"> mur=1 muf=0.5 </weight>
#    <weight id="4" MUR="2" MUF="1" PDF="306000"> mur=2 muf=1 </weight>
#    <weight id="5" MUR="2" MUF="2" PDF="306000"> mur=2 muf=2 </weight>
#    <weight id="6" MUR="2" MUF="0.5" PDF="306000"> mur=2 muf=0.5 </weight>    !!!!
#    <weight id="7" MUR="0.5" MUF="1" PDF="306000"> mur=0.5 muf=1 </weight>
#    <weight id="8" MUR="0.5" MUF="2" PDF="306000"> mur=0.5 muf=2 </weight>    !!!!
#    <weight id="9" MUR="0.5" MUF="0.5" PDF="306000"> mur=0.5 muf=0.5 </weight>

if __name__ == '__main__':

    xSecAdder.AddMergeBranch('genWeight')
    xSecAdder.AddMergeBranch('bsf_DeepCSVB')

    for branch in sys.argv[1:]:
        if 'ps_weight' in branch:
            corrector = MakeApplicator('%s_corrected' % branch, False)
            corrector.AddCorrector(
                MakeFormulaCorrector(
                    '%s_formula' % branch,
                    '%s * (1 + (LHE_weight_orig - 1) * (genWeight<=1 && genWeight > 0.98))' % branch
                    ))

            SetFunctionFromEnv([(corrector.SetInDirectory, 'CrombieInFilesDir')])
            RunOnDirectory(corrector)

            branch += '_corrected'

        xSecAdder.AddMergeBranch(branch)

    SetupConfigFromEnv(xSecAdder)
    SetFunctionFromEnv([(xSecAdder.SetNumThreads, 'CrombieNLocalProcs')])

    RunXSecAdder('_'.join(['xsec_weight'] + sys.argv[1:]))
