import os
import sys
import re
import subprocess

metcut     = 'pfmet > 170 && met_filter == 1'
lepveto    = 'n_lep_presel == 0'

btag_csv   = 'csv_jet1_csv > 0.8484'
unbtag_csv = 'csv_jet1_csv < 0.8484'
lbtag_csv  = 'csv_jet2_csv > 0.5426'
tbtag_csv  = 'csv_jet1_csv > 0.9535'

btag       = 'cmva_jet1_cmva > 0.4432'
unbtag     = 'cmva_jet1_cmva < 0.4432'
lbtag      = 'cmva_jet2_cmva > -0.5884'
tbtag      = 'cmva_jet1_cmva > 0.9432'

hbbpt      = 'cmva_hbb_pt_reg_old > 120'
jetpt      = ' && '.join(['cmva_daughter_max_pt > 60',
                          'cmva_daughter_min_pt > 35',
                          'cmva_jet1_pt > 60'
                          ])
mjjveto    = '(60 > cmva_hbb_m_reg_old || 160 < cmva_hbb_m_reg_old)'
antiQCD    = 'min_dphi_metj_hard > 0.5'
antierQCD  = 'min_dphi_metj_hard > 1.5'
deltaVH    = 'cmva_dphi_uh > 2.0'
undeltaVH  = 'cmva_dphi_uh < 2.0'
trkmetphi  = 'dphi_met_trkmet < 0.5'

common = ' && '.join([
        'cmva_hbb_m < 500',
        metcut,
        jetpt,
        lbtag,
        hbbpt
        ])

categoryCuts = {
    'ZvvHbb' : '1'
    }

regionCuts = {
    'nocut' : ' && '.join([metcut, jetpt]),
    'tt' : ' && '.join([
            common,
            deltaVH,
            'n_lep_tight == 1',
            'n_jet >= 4',
#            'n_centerjet >= 4',
            btag,
            'min_dphi_recoilb < 1.57',
#            'min(deltaPhi(cmva_jet1_phi, recoilphi), deltaPhi(cmva_jet2_phi, recoilphi)) < 1.57',
            ]),
    'lightz' : ' && '.join([
            common,
            deltaVH,
            lepveto,
            'n_jet < 4',
            unbtag,
            antiQCD,
            trkmetphi,
            ]),
    'heavyz' : ' && '.join([
            common,
            deltaVH,
            lepveto,
            'n_jet < 3',
            tbtag,
            antiQCD,
            trkmetphi,
            mjjveto
            ]),
    'multijet' : ' && '.join([
            common,
            undeltaVH,
            lepveto,
            'min_dphi_metj_hard < 0.4'
            ]),
    'classify' : ' && '.join([
            'met_filter == 1',
            'met > 150',
            'jet1_chf > 0.15',
            'jet1_efrac < 0.8',
            'jet1_pt > 40',
            'jet2_pt > 30',
            'cmva_hbb_pt > 100',
            'cmva_jet2_cmva > -0.6',
            'n_jet < 5',
            'min_dphi_metj_hard > 0.5'
            ])
    }

regionCuts['common'] = common
regionCuts['signal'] = ' && '.join([
        regionCuts['heavyz'].replace(mjjveto, '60 < cmva_hbb_m_reg_old && 160 > cmva_hbb_m_reg_old').replace('jet < 3', 'jet < 4'),
        'event_class > -0.3'
        ])
regionCuts['classifyHveto'] = '%s && !(%s)' % (regionCuts['classify'], regionCuts['signal'])

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
     'post_fit_mix3',
     'cmva_jet2_sf_loose',
     '(eventNumber % 2 == 0) * 2',
     ])

# Additional weights applied to certain control regions

mettrigger = 'met_trigger'

signal = os.environ.get('signal', '0')

region_weights = { # key : [Data,MC]
    'signal'   : [signal, ' * '.join([defaultMCWeight, 'cmva_jet1_sf_tight'])],
    'heavyz'   : [mettrigger, ' * '.join([defaultMCWeight, 'cmva_jet1_sf_tight'])],
    'lightz'   : [mettrigger, ' * '.join([defaultMCWeight, 'cmva_jet1_sf_loose'])],
    'multijet' : [mettrigger, ' * '.join([defaultMCWeight, 'cmva_jet1_sf_loose'])],
    'tt' : [mettrigger, ' * '.join([defaultMCWeight, 'cmva_jet1_sf_medium'])],
    'classify'  : [signal, defaultMCWeight],
    'default'  : [mettrigger, defaultMCWeight],
    }

# Up and down

check_header = lambda systematic: subprocess.check_output(
    'perl -ne \'/^\s*(\w*)_' + systematic + 'Up\s\=/ && print"$1\n"\' ../slimmer/include/hbbfile.h | sort | uniq', shell=True
    ).split('\n')[:-1]

syst = {
    'wfact': ['wkfactor'],
    'wren': ['wkfactor'],
    'zfact': ['zkfactor'],
    'zren': ['zkfactor'],
    'ewk': ['vh_ewk'],
    'btagsf': check_header('btagsf'),
    'jetpt': check_header('jetpt'),
    'pdf': check_header('pdf'),
    }

env = {
    'renorm': ['1 + %s' % b for b in ['r1f2DW', 'r1f5DW', 'r2f1DW', 'r2f2DW', 'r5f1DW', 'r5f5DW']]
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

    return cut

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
            print cut('ZvvHbb', sys.argv[1])
