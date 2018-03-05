import os
import sys

jetgood    = 'jet1_chf > 0.15 && jet1_efrac < 0.8'
metcut     = 'met > 170 && met_filter == 1'
lepveto    = 'n_lep_loose == 0'

btag_csv   = 'csv_jet1_csv > 0.8484'
unbtag_csv = 'csv_jet1_csv < 0.8484'
lbtag_csv  = 'csv_jet2_csv > 0.5426'
tbtag_csv  = 'csv_jet1_csv > 0.9535'

btag       = 'cmva_jet1_cmva > 0.4432'
unbtag     = 'cmva_jet1_cmva < 0.4432'
lbtag      = 'cmva_jet2_cmva > -0.5884'
tbtag      = 'cmva_jet1_cmva > 0.9432'

hbbpt      = 'cmva_hbb_pt > 120'
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
        # jetgood,
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
#            '(muon1_pt > 25&&muon1_tight) || (ele1_pt > 30&&ele1_tight)',
            'n_centerjet >= 4',
            btag,
            'min_dphi_metj_hard < 1.57',
            ]),
    'lightz' : ' && '.join([
            common,
            deltaVH,
            lepveto,
            'n_centerjet < 4',
            unbtag,
            antiQCD,
            trkmetphi,
            ]),
    'heavyz' : ' && '.join([
            common,
            deltaVH,
            lepveto,
            'n_centerjet < 3',
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
            'n_centerjet < 5',
            'min_dphi_metj_hard > 0.5'
            ])
    }

regionCuts['common'] = common
regionCuts['signal'] = ' && '.join([
        regionCuts['heavyz'].replace(mjjveto, '60 < cmva_hbb_m_reg_old && 160 > cmva_hbb_m_reg_old'),
        ])
regionCuts['classifyHveto'] = '%s && !(%s)' % (regionCuts['classify'], regionCuts['signal'])

# Making selection of multiple entries

def joinCuts(toJoin=regionCuts.keys(), cuts=regionCuts):
    return ' && '.join([cuts[cut] for cut in toJoin])

# A weight applied to all MC

defaultMCWeight = 'scale_factors * cmva_jet2_loose_sf_central * (eventNumber % 2 == 0) * 2'

# Additional weights applied to certain control regions

mettrigger = 'met_trigger'

signal = os.environ.get('signal', '0')

region_weights = { # key : [Data,MC]
    'signal'   : [signal, '*'.join([defaultMCWeight, 'cmva_jet1_tight_sf_central'])],
    'heavyz'   : [mettrigger, '*'.join([defaultMCWeight, 'cmva_jet1_tight_sf_central'])],
    'lightz'   : [mettrigger, '*'.join([defaultMCWeight, 'cmva_jet1_loose_sf_central'])],
    'multijet' : [mettrigger, '*'.join([defaultMCWeight, 'cmva_jet1_loose_sf_central'])],
    'tt' : [mettrigger, '*'.join([defaultMCWeight, 'cmva_jet1_medium_sf_central'])],
    'classify'  : [signal, defaultMCWeight],
    'default'  : [mettrigger, defaultMCWeight],
    }

# Up and down

syst = [('wkfactor', ['_fact', '_ren']),
        ('zkfactor', ['_fact', '_ren']),
        ('vh_ewk', ['']),
        ('bjetcalib', [''])]

keys = list(regionCuts.keys())
for key in keys:
    for old, new_list in syst:
        for new_stuff in new_list:
            for direction in ['up', 'down']:
                new = '%s%s_%s' % (old, new_stuff, direction)
                new_key = '%s__%s%s' % (key, old + new_stuff, direction.title())
                regionCuts[new_key] = regionCuts[key]
                if old == 'bjetcalib':
                    region_weights[new_key] = [mettrigger, (region_weights.get(key, [None, None])[1] or defaultMCWeight).replace('_central', '_%s' % direction)]
                else:
                    region_weights[new_key] = [mettrigger, '%s / %s * %s' % (region_weights.get(key, [None, None])[1] or defaultMCWeight, old, new)]

# Do not change the names of these functions or required parameters
# Otherwise you cannot use some convenience functions
# Multiple regions are concatenated with '+'
# Generally you can probably leave these alone

def cut(category, region):
    regions = region.split('+')
    cut = regionCuts[regions[0]]
    if regions[-1] == 'csv':
        for orig, new in [(btag, btag_csv), (unbtag, unbtag_csv),
                          (lbtag, lbtag_csv), (tbtag, tbtag_csv)]:
            cut = cut.replace(orig, new)
        cut = cut.replace('cmva', 'csv')

    return cut.replace('_reg_old', '')

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
        print cut('ZvvHbb', sys.argv[1])
