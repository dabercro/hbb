#! /usr/bin/env python

import os
import sys
import re

from CrombieTools.Parallelization import RunOnDirectory
from CrombieTools.SkimmingTools import Corrector
from CrombieTools.SkimmingTools.FormulaCorrector import MakeFormulaCorrector
from CrombieTools.SkimmingTools.TMVACorrector import MakeTMVACorrector

import cuts

applicator = Corrector.MakeApplicator('scale_factors', True, 'events', 'events', 10000)

applicator.AddFactorToMerge('mc_weight')

def add_corr(name, expr, cut, fileName, histName, matchName=''):
    corr = Corrector.MakeCorrector(name, expr, cut, fileName, histName)
    if matchName:
        corr.SetMatchFileName(matchName)
    applicator.AddCorrector(corr)

add_corr('sf_pu', 'npv_true', '1', 'data/puWeights_80x_37ifb.root', 'puWeights')
add_corr('sf_met_trigger','met','1','data/metTriggerEfficiency_recoil_monojet_TH1F.root','hden_monojet_recoil_clone_passed')

add_corr('ewk_z', 'genboson_pt', 'abs(genboson_pdgid) == 23', 'data/kfactors.root', ['EWKcorr/Z', 'ZJets_012j_NLO/nominal'], '.*_HT-.*')
add_corr('ewk_w', 'genboson_pt', 'abs(genboson_pdgid) == 24', 'data/kfactors.root', ['EWKcorr/W', 'WJets_012j_NLO/nominal'], '.*_HT-.*')
add_corr('zkfactor', 'genboson_pt', 'abs(genboson_pdgid) == 23', 'data/kfactors.root', ['ZJets_012j_NLO/nominal', 'ZJets_LO/inv_pt'], '.*_HT-.*')
add_corr('wkfactor', 'genboson_pt', 'abs(genboson_pdgid) == 24', 'data/kfactors.root', ['WJets_012j_NLO/nominal', 'WJets_LO/inv_pt'], '.*_HT-.*')

tt_corr = MakeFormulaCorrector('sf_tt', 'sqrt(exp(0.0615-0.0005*min(400.0, gen_t_pt)) * exp(0.0615-0.0005 * min(400.00, gen_tbar_pt)))', 'gen_t && gen_tbar', 'TT.*')
applicator.AddCorrector(tt_corr)

unc_applicator = Corrector.MakeApplicator('', True, 'events', 'events', 100000, True)

def add_unc(name, expr, cut, fileName, histName):
    corr = Corrector.MakeCorrector(name, expr, cut, fileName, histName)
    corr.SetHistReader(corr.eUnityCenteredUnc)
    unc_applicator.AddCorrector(corr)

if __name__ == '__main__':
    directory = sys.argv[1]
    applicator.SetInDirectory(directory)
    RunOnDirectory(applicator)
#    unc_applicator.SetInDirectory(directory)
#    RunOnDirectory(unc_applicator)
