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

def add_corr(name, expr, cut, fileName, histName, matchName='', merge=True):
    corr = Corrector.MakeCorrector(name, expr, cut, fileName, histName)
    if matchName:
        corr.SetMatchFileName(matchName)
    corr.Merge = merge
    applicator.AddCorrector(corr)

add_corr('sf_pu', 'npv_true', '1', 'data/puWeights_80x_37ifb.root', 'puWeights')
add_corr('sf_met_trigger','met','1','data/metTriggerEfficiency_recoil_monojet_TH1F.root','hden_monojet_recoil_clone_passed')

for pdgid, bos in [(23, 'z'), (24, 'w')]:
    add_corr('ewk_%s' % bos, 'genboson_pt', 'abs(genboson_pdgid) == %i' % pdgid, 'data/kfactors.root',
             ['EWKcorr/%s' % bos.upper(), '%sJets_012j_NLO/nominal' % bos.upper()], '.*_HT-.*')
    add_corr('%skfactor' % bos, 'genboson_pt', 'abs(genboson_pdgid) == %i' % pdgid, 'data/kfactors.root',
             ['%sJets_012j_NLO/nominal' % bos.upper(), '%sJets_LO/inv_pt' % bos.upper()], '.*_HT-.*')

    for pref in ['fact', 'ren']:
        for direction in ['up', 'down']:
            add_corr('%skfactor_%s%s%s' % (bos, bos, pref, direction.title()), 'genboson_pt', 'abs(genboson_pdgid) == %i' % pdgid, 'data/kfactors.root',
                     ['%sJets_012j_NLO/%s_%s' % (bos.upper(), pref, direction), '%sJets_LO/inv_pt' % bos.upper()], '.*_HT-.*', False)

for begin, pdgid in [('Wm', 24), ('Wp', 24), ('Zll', 23), ('Znn', 23)]:
    match = '%s.*_HToBB_W.*' % begin if begin[0] == 'W' else '.*ZH_HToBB_ZTo%s.*' % begin[1].upper()
    for direction in ['', '_up', '_down']:
        dname = '_ewk' + direction[1:].title() if direction else direction
        add_corr('vh_ewk%s' % dname, 'genboson_pt', 'abs(genboson_pdgid) == %i' % pdgid, 'data/%s_nloEWK_weight_unnormalized.root' % begin,
                 'SignalWeight_nloEWK%s_rebin' % direction, match, not bool(direction))

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
