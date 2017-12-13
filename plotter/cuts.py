from CrombieTools import Nminus1Cut

metcut     = 'pfmet > 170 && metFilter == 1'
lepveto    = 'nLooseLep == 0'
btag       = 'jetCMVA[hbbjtidx[0]] > 0.4432'
unbtag     = 'jetCMVA[hbbjtidx[0]] < 0.4432'
lbtag      = 'jetCMVA[hbbjtidx[1]] > -0.5884'
tbtag      = 'jetCMVA[hbbjtidx[0]] > 0.9432'
hbbpt      = 'hbbpt_reg > 120'
jetpt      = 'jet1Pt > 60 && jet2Pt > 35'
mjjveto    = '(60 > hbbm || 160 < hbbm)'
antiQCD    = 'dphipfmet > 0.5'
antierQCD  = 'dphipfmet > 1.5'
deltaVH    = 'deltaPhi(pfmetphi,hbbphi) > 2.0'
undeltaVH  = 'deltaPhi(pfmetphi,hbbphi) < 2.0'
deltaVHlep = 'deltaPhi(hbbphi,vectorSumPhi(pfmet,pfmetphi,Alt$((nTightMuon==1)*muonPt[0],0)+Alt$((nTightElectron==1)*electronPt[0],0),Alt$((nTightMuon==1)*muonPhi[0],0)+Alt$((nTightElectron==1)*electronPhi[0],0))) > 2' 

common = ' && '.join([
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
            deltaVHlep,
            'nTightLep == 1',
            'nJet >= 4',
            btag,
            'dphipfmet < 1.57'
            ]),
    'multijet' : ' && '.join([
            common,
            undeltaVH,
            lepveto,
            'dphipfmet < 0.4'
            ]),
    'lightz' : ' && '.join([
            common,
            lepveto,
            unbtag,
            antiQCD,
            deltaVH,
            'nJet < 4'
            ]),
    'heavyz' : ' && '.join([
            common,
            lepveto,
            tbtag,
            mjjveto,
            deltaVH,
            antiQCD,
            'nJet < 3'
            ]),
    }

regionCuts['common'] = common
regionCuts['scaledtt'] = regionCuts['tt'] 

regionCuts['signal'] = regionCuts['heavyz'].replace(mjjveto, '60 < hbbm && 160 > hbbm')

# Making selection of multiple entries

def joinCuts(toJoin=regionCuts.keys(), cuts=regionCuts):
    return ' && '.join([cuts[cut] for cut in toJoin])

# A weight applied to all MC

defaultMCWeight = '*'.join([
        'normalizedWeight',
        'sf_pu',
        'sf_ewkV',
        'sf_qcdV',
        'sf_metTrig',
#        'sf_lepID',
#        'sf_lepIso',
#        'sf_lepTrack',
#        'sf_tt',
        'sf_cmvaWeight_Cent'
        ])

# Additional weights applied to certain control regions

mettrigger = '(trigger & 1) != 0'

region_weights = { # key : [Data,MC]
    'signal'   : ['0', defaultMCWeight],
    'scaledtt' : [mettrigger, '*'.join([defaultMCWeight, '((sf_tt == 1.0) + (sf_tt != 1.0) * 0.78)'])],
    'scaledcommon' : [mettrigger, '*'.join([defaultMCWeight, '((sf_tt == 1.0) + (sf_tt != 1.0) * 0.78)'])],
    'default'  : [mettrigger, defaultMCWeight],
    }

# Do not change the names of these functions or required parameters
# Otherwise you cannot use some convenience functions
# Multiple regions are concatenated with '+'
# Generally you can probably leave these alone

def cut(category, region):
    return ('(' + joinCuts(toJoin=region.split('+')) + ')')

def dataMCCuts(region, isData):
    key = 'default'
    if region in region_weights.keys():
        key = region

    index = 0 if isData else 1

    return '(' + region_weights[key][index] + ')'
