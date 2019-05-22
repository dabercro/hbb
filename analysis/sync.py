#! /usr/bin/env python

import sys

import pandas
import ROOT


TOLERANCE = 0.1


if __name__ == '__main__':

    event = int(sys.argv[2])

    themFile = pandas.read_hdf('/data/t3home000/bmaier/TTbar_nanoAOD_RegressionPerJet_nanoAOD_2018_JECv8_con5659_testing.hd5')
    meFile = ROOT.TFile('/home/dabercro/hbb/slimmer/reg.root')

    meFile.events.GetEntry(event)

    with open(sys.argv[1], 'r') as branches:
        with open('output.txt', 'w') as output:
            for line in branches:
                names = line.strip().split()
                if names:
                    themVal = themFile.iloc[event][names[0]]
                    mid = '!='
                    try:
                        meVal = getattr(meFile.events, names[0])

                        if meVal/themVal < (1 + TOLERANCE) and meVal/themVal < (1 + TOLERANCE):
                            mid = '==' if meVal == themVal else '~~'

                        meStr = '%f' % meVal
                    except:
                        meStr = 'Not Implemented'

                    output.write('%40s  %12s  %s  %s\n' % (
                            names[0],
                            '%f' % themVal,
                            mid,
                            meStr)
                    )
