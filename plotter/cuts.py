from CrombieTools import Nminus1Cut

jetgood    = 'jet1_chf > 0.15 && jet1_emfrac < 0.8'
metcut     = 'met > 170 && met_filter == 1'
lepveto    = 'n_looselep == 0'

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
deltaVH    = 'dphi_uh_cmva > 2.0'
undeltaVH  = 'dphi_uh_cmva < 2.0'
trkmetphi  = 'deltaPhi(metphi,trkmetphi) < 0.5'

common = ' && '.join([
        jetgood,
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
            'n_looselep < 3',
            'n_tightlep == 1',
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
    }

regionCuts['common'] = common
regionCuts['scaledtt'] = regionCuts['tt'] 
regionCuts['signal'] = regionCuts['heavyz'].replace(mjjveto, '60 < cmva_hbb_m && 160 > cmva_hbb_m')

# Making selection of multiple entries

def joinCuts(toJoin=regionCuts.keys(), cuts=regionCuts):
    return ' && '.join([cuts[cut] for cut in toJoin])

# A weight applied to all MC

defaultMCWeight = 'scale_factors/abs(mc_weight)'

# Additional weights applied to certain control regions

mettrigger = 'met_trigger'

region_weights = { # key : [Data,MC]
    'signal'   : ['0', defaultMCWeight],
    'scaledtt' : [mettrigger, '*'.join([defaultMCWeight, '((sf_tt == 1.0) + (sf_tt != 1.0) * 0.78)'])],
    'default'  : [mettrigger, defaultMCWeight],
    }

# Do not change the names of these functions or required parameters
# Otherwise you cannot use some convenience functions
# Multiple regions are concatenated with '+'
# Generally you can probably leave these alone

def cut(category, region):
    return '(' + joinCuts(toJoin=region.split('+')) + ')'

def dataMCCuts(region, isData):
    key = 'default'
    if region in region_weights.keys():
        key = region

    index = 0 if isData else 1

    return '(' + region_weights[key][index] + ')'

if __name__ == '__main__':
    print regionCuts
    print region_weights
