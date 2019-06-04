#! /usr/bin/env python

import sys

import pandas
import ROOT


TOLERANCE = 0.02

themFile = pandas.read_hdf('/data/t3home000/bmaier/TTbar_nanoAOD_RegressionPerJet_nanoAOD_2018_JECv8_con5659_testing.hd5')


def dump(syncfile, themEvent, meEvent=None, index=None):

#    themFile = ROOT.TFile('/home/dabercro/hbb/slimmer/reg.root')
    meFile = ROOT.TFile('/home/dabercro/hbb/slimmer/reg.root')
#    meFile = ROOT.TFile('/data/t3home000/bmaier/panda/v_013_breg_synch/batch/TTToHadronic_CP5_1302_999.root')

    if meEvent is None:
        meEvent = themEvent 

#    themFile.events.GetEntry(themEvent)
    meFile.events.GetEntry(meEvent)

    with open(syncfile, 'r') as branches:
        with open('output.txt', 'w') as output:
            for line in branches:
                names = line.strip().split()
                if names:
                    themVal = themFile.iloc[themEvent][names[0]]
#                    themVal = getattr(themFile.events, names[0])
                    mid = '!='
                    try:
                        meVal = getattr(meFile.events, names[1])[index]
                    except:
                        meVal = getattr(meFile.events, names[1])

                    if meVal and themVal:
                        if (meVal/themVal) > 0 and \
                                (meVal/themVal) < (1 + TOLERANCE) and \
                                (themVal/meVal) < (1 + TOLERANCE):
                            mid = '==' if meVal == themVal else '~~'
                    else:
                        mid = '==' if meVal == themVal else '!='

                    meStr = '%f' % meVal
                    ratStr = '%f' % (themVal/meVal) if mid != '==' and meVal else '1.000'

                    output.write('%40s  %12s  %s  %s  (ratio %s)\n' % (
                            names[0],
                            '%f' % themVal,
                            mid,
                            meStr,
                            ratStr)
                    )

#    themFile.Close()
    meFile.Close()


if __name__ == '__main__':

    themEvent = int(sys.argv[2])
    meEvent = int(sys.argv[3])

    dump(sys.argv[1], themEvent, meEvent)
