import sys

jetgood    = 'jet1_chf > 0.15 && jet1_emfrac < 0.8'
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
jetpt      = 'jet1_pt > 60 && jet2_pt > 35'
mjjveto    = '(60 > cmva_hbb_m || 160 < cmva_hbb_m)'
antiQCD    = 'min_dphi_metj_hard > 0.5'
antierQCD  = 'min_dphi_metj_hard > 1.5'
deltaVH    = 'cmva_dphi_uh > 2.0'
undeltaVH  = 'cmva_dphi_uh < 2.0'
trkmetphi  = 'dphi_met_trkmet < 0.5'

common = ' && '.join([
        # jetgood,
        jetpt,
        hbbpt,
        metcut,
        lbtag
        ])

categoryCuts = {
    'ZvvHbb' : '1'
    }


regionCuts = {
    'tt' : ' && '.join([
            common,
            deltaVH,
            'n_lep_loose < 3',
            'n_lep_tight == 1',
            'n_jet >= 4',
            btag,
            'min_dphi_metj_hard < 1.57'
            ]),
    'multijet' : ' && '.join([
            common,
            undeltaVH,
            lepveto,
            'min_dphi_metj_hard < 0.4'
            ]),
    'lightz' : ' && '.join([
            common,
            lepveto,
            unbtag,
            antiQCD,
            deltaVH,
            trkmetphi,
            'n_jet < 4'
            ]),
    'heavyz' : ' && '.join([
            common,
            lepveto,
            tbtag,
            mjjveto,
            deltaVH,
            antiQCD,
            trkmetphi,
            'n_jet < 3'
            ]),
    'classify' : ' && '.join([
            'met_filter == 1',
            'met > 150',
            'jet1_chf > 0.15',
            'jet1_emfrac < 0.8',
            'jet1_pt > 40',
            'jet2_pt > 30',
            'cmva_hbb_pt > 100',
            'cmva_jet2_cmva > -0.6',
            'n_jet < 5',
            'min_dphi_metj_hard > 0.5',
            ])
    }

regionCuts['common'] = common
regionCuts['scaledtt'] = regionCuts['tt'] 
regionCuts['signal'] = regionCuts['heavyz'].replace(mjjveto, '60 < cmva_hbb_m && 160 > cmva_hbb_m')

# Making selection of multiple entries

def joinCuts(toJoin=regionCuts.keys(), cuts=regionCuts):
    return ' && '.join([cuts[cut] for cut in toJoin])

# A weight applied to all MC

defaultMCWeight = 'scale_factors * cmva_jet2_loose_sf_central * (event_num % 2 == 0) * 2'

# Additional weights applied to certain control regions

mettrigger = 'met_trigger'

region_weights = { # key : [Data,MC]
    'signal'   : ['0', '*'.join([defaultMCWeight, 'cmva_jet1_tight_sf_central'])],
    'heavyz'   : [mettrigger, '*'.join([defaultMCWeight, 'cmva_jet1_tight_sf_central'])],
    'lightz'   : [mettrigger, '*'.join([defaultMCWeight, 'cmva_jet1_loose_sf_central'])],
    'multijet' : [mettrigger, '*'.join([defaultMCWeight, 'cmva_jet1_loose_sf_central'])],
    'tt' : [mettrigger, '*'.join([defaultMCWeight, 'cmva_jet1_medium_sf_central'])],
    'scaledtt' : [mettrigger, '*'.join([defaultMCWeight, 'cmva_jet1_medium_sf_central', '((sf_tt == 1.0) + (sf_tt != 1.0) * 0.78)'])],
    'default'  : [mettrigger, defaultMCWeight],
    }

# Up and down

keys = list(regionCuts.keys())
for key in keys:
    pass

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
        print cut('ZvvHbb', sys.argv[1])
