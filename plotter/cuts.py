import os
import sys
import re
import subprocess

from CrombieTools import Nminus1Cut

metcut     = ' && '.join([
        'pfmet > 170',
        'met_filter == 1',
        ])
lepveto    = 'n_lep_presel == 0'
#lepveto    = '(Sum$(muon_reliso < 0.4) + Sum$(ele_reliso < 0.4)) == 0'

btag_csv   = 'csv_jet1_csv > 0.8484'
unbtag_csv = 'csv_jet1_csv < 0.8484'
lbtag_csv  = 'csv_jet2_csv > 0.5426'
tbtag_csv  = 'csv_jet1_csv > 0.9535'

btag       = 'cmva_jet1_cmva > 0.4432'
unbtag     = 'cmva_jet1_cmva < 0.4432'
lbtag      = 'cmva_jet2_cmva > -0.5884'
tbtag      = 'cmva_jet1_cmva > 0.9432'

fat_btag   = 'ak8fatjet1_double_sub > 0.8'
fat_unbtag = 'ak8fatjet1_double_sub < 0.8'
fat_tbtag  = 'ak8fatjet1_double_sub > 0.8'

hbbpt      = 'cmva_hbb_pt_reg_old > 120'
jetpt      = ' && '.join(['cmva_daughter_max_pt > 60',
                          'cmva_daughter_min_pt > 35',
                          'cmva_jet1_pt > 60'
                          ])
mjjveto    = '(60 > cmva_hbb_m_reg_old || 160 < cmva_hbb_m_reg_old)'
antiQCD    = 'min_dphi_metj_hard > 0.5'
#antiQCD    = 'Sum$(jet_pt > 30.0&&deltaPhi(jet_phi, pfmetphi) < 0.5) == 0'
antierQCD  = 'min_dphi_metj_hard > 1.5'
deltaVH    = 'cmva_dphi_uh > 2.0'
undeltaVH  = 'cmva_dphi_uh < 2.0'
trkmetphi  = 'dphi_met_dztrkmet < 0.5'
#trkmetphi  = 'dphi_met_trkmet < 0.5'

common = ' && '.join([
        metcut,
        ])

categoryCuts = {
    'inclusive': ' && '.join([
            'cmva_hbb_m_reg_old < 500',
            jetpt,
            lbtag,
            deltaVH,
            hbbpt,
            ]),
    'boosted': ' && '.join([
            'ak8fatjet1_pt > 250',
            'ak8fatjet1_mSD_corr > 40',
            '(!cmva_jet2 || cmva_jet2_cmva < -0.5884)',
            ])
    }
#categoryCuts['resolved'] = ' && '.join([
#        categoryCuts['inclusive'],
#        '(%s)' % ' || '.join(['!(%s)' % cut for cut in categoryCuts['boosted'].split(' && ')])
#        ])


regionCuts = {
    'nocut' : common,
    'tt' : ' && '.join([
            common,
            'n_lep_tight == 1',
            'n_centerjet >= 4',
            btag,
            'min_dphi_recoilb < 1.57',
            ]),
    'lightz' : ' && '.join([
            common,
            lepveto,
            'n_centerjet < 4',
            unbtag,
            antiQCD,
            trkmetphi,
            ]),
    'heavyz' : ' && '.join([
            common,
            lepveto,
            'n_centerjet < 3',
            tbtag,
            antiQCD,
            trkmetphi,
            mjjveto
            ]),
    }

regionCuts['signal'] = ' && '.join([
        regionCuts['heavyz'].replace(mjjveto, '60 < cmva_hbb_m_reg_old && 160 > cmva_hbb_m_reg_old').replace('jet < 3', 'jet < 4'),
#        'maier_event_class > -0.8'
        ])

# Making selection of multiple entries

def joinCuts(toJoin=regionCuts.keys(), cuts=regionCuts):
    return ' && '.join([cuts[cut] for cut in toJoin])

# A weight applied to all MC

defaultMCWeight = ' * '.join(
    ['sf_pu', 'sf_met_trigger',
     'ewk_z', 'ewk_w',
     'wkfactor', 'zkfactor',
     'vh_ewk', 'sf_tt',
     'mc_weight',
     'pdf',
     'btag_sf',
#     'beff_sf',
#     'cmva_jet2_sf_loose',
     os.environ.get('post', '1'),           # Postfit expression
     ])

# Additional weights applied to certain control regions

#mettrigger = 'hbb_2016_trigger'
mettrigger = 'met_trigger'

#signal = os.environ.get('signal', '0.5 > maier_event_class')      # Signal cut
signal = os.environ.get('signal', '0')      # Signal cut

region_weights = { # key : [Data,MC]
    'signal'   : [signal, ' * '.join([defaultMCWeight,
#                                      'cmva_jet1_sf_tight',
                                      '(eventNumber % 2 == 0) * 2'
                                      ])],
#    'heavyz'   : [mettrigger, ' * '.join([defaultMCWeight, 'cmva_jet1_sf_tight'])],
#    'lightz'   : [mettrigger, ' * '.join([defaultMCWeight, 'cmva_jet1_sf_loose'])],
#    'multijet' : [mettrigger, ' * '.join([defaultMCWeight, 'cmva_jet1_sf_loose'])],
#    'tt' : [mettrigger, ' * '.join([defaultMCWeight, 'cmva_jet1_sf_medium'])],
    'default'  : [mettrigger, defaultMCWeight],
    }

# Up and down

check_header = lambda systematic: subprocess.check_output(
    'perl -ne \'/^\s*(\w*)_' + systematic + 'Up\s\=/ && print"$1\n"\' ../slimmer/include/hbbfile.h | sort | uniq', shell=True
    ).split('\n')[:-1]

#btagsf_branches = check_header('btagsf')

syst = {
    'wfact': ['wkfactor'],
    'wren': ['wkfactor'],
    'zfact': ['zkfactor'],
    'zren': ['zkfactor'],
    'ewk': ['vh_ewk'],
#    'btagsf': btagsf_branches,
    }
syst.update(
    {key: check_header(key) for key in
     ['jetpt', 'pdf',
#      'JES', 'LF', 'HF',
#      'cErr1',  'cErr2', 'Stats1', 'Stats2'
      ]
     })

env = {
    'fact': [['1 + %s' % b, i] for i, b in [(2, 'r1f2DW'), (3, 'r1f5DW')]],
    'renorm': [['1 + %s' % b, i] for i, b in [(4, 'r2f1DW'), (6, 'r5f1DW')]],
#    'renorm': [['1 + %s' % b, i + 2] for i, b in enumerate(['r1f2DW', 'r1f5DW', 'r2f1DW', 'r2f2DW', 'r5f1DW', 'r5f5DW'])]
    }


keys = list(regionCuts.keys())
for key in keys:
    for envelope in env:
        for direction in ['Up', 'Down']:
            new_key = '%s__%s%s' % (key, envelope, direction)
            region_weights[new_key] = region_weights.get(key, region_weights['default'])
            regionCuts[new_key] = regionCuts[key]

    for systematic, affects in syst.iteritems():
        for direction in ['Up', 'Down']:
            new_key = '%s__%s%s' % (key, systematic, direction)
            newweight = list(region_weights.get(key, region_weights['default']))
            newcut = regionCuts[key]
            for branch in affects:
                sys_expr = '%s_%s%s' % (branch, systematic, direction)
                sub = lambda s: re.sub(r'\b' + re.escape(branch) + r'\b', sys_expr, s) 
                newweight[1] = sub(newweight[1])
                newcut = sub(newcut)

            region_weights[new_key] = newweight
            regionCuts[new_key] = newcut

#for key in keys:
#    origweight = list(region_weights.get(key, region_weights['default']))
#    pt_low = 20
#    for pt_up in [50, 80, 120, 200, 300, 400, 500, 700, 1000]:
#        eta_low = 0.0
#        for eta_up in [0.5, 1.5, 2.5]:
#            for direction in ['Up', 'Down']:
#                newsyst = 'btagsfp%se%s' % (pt_up, str(eta_up).replace('.', 'p'))
#                syst[newsyst] = []
#                new_key = '%s__%s%s' % (key, newsyst, direction)
#
#                newweight = re.sub(r'((cmva_jet\d)_sf_[a-z]+)',
#                                   r'(\1 + ((\1_btagsf{direction} - \1) * (\2_pt > {pt_low} && \2_pt < {pt_up} && abs(\2_eta) > {eta_low} && abs(\2_eta) < {eta_up})))',
#                                   origweight[1])
#
#                region_weights[new_key] = [origweight[0],
#                                           newweight.format(direction=direction,
#                                                            pt_low=pt_low, pt_up=pt_up,
#                                                            eta_low=eta_low, eta_up=eta_up)]
#                regionCuts[new_key] = regionCuts[key]
#
#            eta_low = eta_up
#        pt_low = pt_up


# Do not change the names of these functions or required parameters
# Otherwise you cannot use some convenience functions
# Multiple regions are concatenated with '+'
# Generally you can probably leave these alone

def cut(category='', region=''):
    regions = region.split('+')
    cut = regionCuts[regions[0]]
    if 'csv' in regions:
        for orig, new in [(btag, btag_csv), (unbtag, unbtag_csv),
                          (lbtag, lbtag_csv), (tbtag, tbtag_csv)]:
            cut = cut.replace(orig, new)
        cut = cut.replace('cmva', 'csv')
    if 'raw' in regions:
        cut = cut.replace('_reg_old', '')

    if category == 'boosted':
        cut = Nminus1Cut(cut, 'min_dphi_recoilb').\
            replace(btag, fat_btag).replace(tbtag, fat_tbtag).replace(unbtag, fat_unbtag).\
            replace('cmva_dphi_uh', 'deltaPhi(ak8fatjet1_phi, recoilphi)').\
            replace('cmva_hbb_m_reg_old', 'ak8fatjet1_mSD_corr').\
            replace('cmva_hbb', 'ak8fatjet1')
        cut = re.sub(r'\b60\b', '80', cut)  # Didn't make plots yet, but maybe try this

    return ('%s && %s' % (cut, categoryCuts[category])).replace('ak8', 'ca15')

def dataMCCuts(region, isData):
    key = 'default'
    info = region.split('+')
    region = info[0]
    if region in region_weights.keys():
        key = region

    index = 0 if isData else 1

    output = region_weights[key][index]
    if info[-1] == 'csv':
        output = output.replace('cmva', 'csv')

    return '(' + output + ')'

if __name__ == '__main__':
    if len(sys.argv) == 1:
        print regionCuts
        print region_weights
    else:
        if 'weight' in sys.argv:
            print dataMCCuts(sys.argv[1], False)
        else:
            print '(%s)' % ') || ('.join([cut(sys.argv[1], reg) for reg in sys.argv[2:]])
