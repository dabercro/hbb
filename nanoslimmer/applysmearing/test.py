#! /usr/bin/env python

import os
import unittest

import applysmearing

import ROOT


class TestSmearing(unittest.TestCase):

    epsilon = 0.01

    def run_test(self, year, isdata, infile):
        applicator = applysmearing.SmearApplicator(year=year, isdata=isdata)

        inputfile = ROOT.TFile(infile)
        inputtree = inputfile.events

        for ievent in xrange(inputtree.GetEntries()):
            inputtree.GetEntry(ievent)

            if not (ievent % 250):
                print '%i/%i' % (ievent, inputtree.GetEntries())

            # jet1
            jet1_tree_down = inputtree.jet1_pt_smeared_down
            jet1_tree_nominal = inputtree.jet1_pt_smeared_nominal
            jet1_tree_up = inputtree.jet1_pt_smeared_up

            jet1_applicator = applicator.get_smear(inputtree.jet1_pt, inputtree.jet1_bRegCorr, inputtree.jet1_gen_pt)

            self.assertTrue((jet1_tree_down - jet1_applicator.down)/jet1_tree_down < self.epsilon)
            self.assertTrue((jet1_tree_nominal - jet1_applicator.nominal)/jet1_tree_nominal < self.epsilon)
            self.assertTrue((jet1_tree_up - jet1_applicator.up)/jet1_tree_up < self.epsilon)

            # jet2
            jet2_tree_down = inputtree.jet2_pt_smeared_down
            jet2_tree_nominal = inputtree.jet2_pt_smeared_nominal
            jet2_tree_up = inputtree.jet2_pt_smeared_up

            if (jet2_tree_nominal):

                jet2_applicator = applicator.get_smear(inputtree.jet2_pt, inputtree.jet2_bRegCorr, inputtree.jet2_gen_pt)

                self.assertTrue((jet2_tree_down - jet2_applicator.down)/jet2_tree_down < self.epsilon)
                self.assertTrue((jet2_tree_nominal - jet2_applicator.nominal)/jet2_tree_nominal < self.epsilon)
                self.assertTrue((jet2_tree_up - jet2_applicator.up)/jet2_tree_up < self.epsilon)


    def test_2018(self):

        year = '2018'
        indir = '/data/t3home000/dabercro/nano/smearnano/200401_smearnano_2018_custom'

        mcfile = os.path.join(indir, 'DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8/3.root')
        datafile = os.path.join(indir, 'DoubleMuon/100.root')

        for isdata, infile in [(False, mcfile), (True, datafile)]:
            self.run_test(year=year, isdata=isdata, infile=infile)


    def test_2017(self):

        year = '2017'
        indir = '/data/t3home000/dabercro/nano/smearnano/200401_smearnano_2017'

        mcfile = os.path.join(indir, 'DYJetsToLL_M-50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8/414.root')
        datafile = os.path.join(indir, 'DoubleMuon/100.root')

        for isdata, infile in [(False, mcfile), (True, datafile)]:
            self.run_test(year=year, isdata=isdata, infile=infile)


if __name__ == '__main__':
    unittest.main()
