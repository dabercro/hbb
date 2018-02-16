#! /usr/bin/env python

import os
import sys

from CrombieTools.LoadConfig import cuts
from CrombieTools.AnalysisTools.CutflowMaker import cutflowMaker

from ROOT import TFile


testFile = TFile('/data/t3home000/dhsu/dylansVHSkims/MET.root')

cutflowMaker.AddTree(testFile.Get('events'))

loosecuts = [
    'trigger&1',
    'metFilter==1',
    'jet1Pt > 55',
    'jet2Pt > 30',
    'pfmet > 160',
    'hbbpt > 50',
    'pfmet>170',
    'Sum$(jetPt>30 && abs(jetEta)<2.4)<4',
    'max(jetPt[hbbjtidx[0]],jetPt[hbbjtidx[1]])>60',
    'min(jetPt[hbbjtidx[0]],jetPt[hbbjtidx[1]])>35',
    'jetPt[hbbjtidx[0]]>60',
    'jetPt[hbbjtidx[1]]>35',
    '(nLooseMuon+nLooseElectron)==0',
    'jetCMVA[hbbjtidx[0]]>0.9432',
    'jetCMVA[hbbjtidx[1]]>-0.5884',
    '(hbbm_reg<160 && hbbm_reg>60)',
    'hbbpt_reg>120',
    'deltaPhi(trkmetphi,pfmetphi)<.5',
    'deltaPhi(hbbphi,pfmetphi)>2.'
    ]

for cut in loosecuts:
    cutflowMaker.AddCut(cut, cut)

cutflowMaker.PrintCutflow()
