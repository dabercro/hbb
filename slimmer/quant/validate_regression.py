#! /usr/bin/env python

import ROOT
from ROOT import gROOT, TFile, TTree, TChain, gPad, gDirectory, AddressOf, TH1F, TF1
from optparse import OptionParser
from operator import add
import math
import sys
import array
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import scipy
import os
import csv
import json
import subprocess
import time
import datetime

sys.path.append('.')

import seaborn_local as sns
import pandas as pd
from pprint import pprint
from scipy.stats import norm
from scipy.optimize import curve_fit
from scipy import asarray as ar,exp
from scipy.special import erf

from matplotlib.ticker import MultipleLocator, FormatStrFormatter, AutoMinorLocator

sns.set(style="ticks")
sns.set_context("poster")

mpl.rcParams['text.latex.preamble'] = [
    r'\usepackage{siunitx}',   # i need upright \micro symbols, but you need...
    r'\sisetup{detect-all}',   # ...this to force siunitx to actually use your fonts
    r'\usepackage{helvet}',    # set the normal font here
    r'\usepackage{sansmath}',  # load up the sansmath so that math -> helvet
    r'\usepackage{upgreek}',
    r'\sansmath'               # <- tricky! -- gotta actually tell tex to use!
]


mpl.rcParams['axes.linewidth'] = 1.25
mpl.rcParams['legend.numpoints'] = 1

fig_size = mpl.rcParams["figure.figsize"]
fig_size[0] = 10
fig_size[1] = 9
mpl.rcParams["figure.figsize"] = fig_size

version = sys.argv[1]

def load_data(jsonfile):
    with open(jsonfile) as config_file:
        data = json.load(config_file)

    data['opath'] = '/home/dabercro/public_html/plots/%s_violin_%s/' % \
        (datetime.date.fromtimestamp(time.time()).strftime('%y%m%d'),
         version)


    if not os.path.exists(data['opath']):
        os.makedirs(data['opath'])

    return data


def violin(option,args):
   data = load_data(options.config)

   for key in data['mc']['samples']:
      if "plotviolin" not in data['mc']['samples'][key]:
         continue

      dataframe_data = {"pt":[],"category":[],"ptcateg":[],"etacateg":[]}
      ptcateg = [0,60,120,180,240,300,360,420,480]
      etacateg = [0,0.4,0.8,1.2,1.6,2.0]

      #Open file
      indir = '/data/t3home000/dabercro/hbb/190610_013/TTToHadronic_TuneCP5_13TeV-powheg-pythia8'
      infiles = sorted(os.listdir(indir))

      for index in xrange(int(os.environ.get('num', len(infiles)))):

          ifile = os.path.join(indir, infiles[index])

#      for ifile in ['../reg_big.root']:

          print "Opening ..." + ifile
          tf = ROOT.TFile(ifile);
          tt = tf.Get("events");

          for i in range(int(tt.GetEntries())):
         
              tt.GetEntry(i)

              if tt.Jet_pt > 5:
                  dataframe_data["pt"].append(tt.Jet_mcPt/tt.Jet_pt)
                  dataframe_data["category"].append("JECs only")
                  dataframe_data["pt"].append(tt.Jet_mcPt/
                                              (tt.Jet_pt*getattr(tt, 'Jet_ptfactor_%s' % version)))
                  dataframe_data["category"].append("DNN regression")


              for k in range(2):
                  for j in range(len(ptcateg)):
                      if ptcateg[-1] < tt.Jet_mcPt:
                          dataframe_data["ptcateg"].append(ptcateg[-1])
                          break
                      if ptcateg[j] > tt.Jet_mcPt:
                          dataframe_data["ptcateg"].append(ptcateg[j-1])
                          break

              for k in range(2):
                  for j in range(len(etacateg)):
                      if etacateg[-1] < abs(tt.Jet_eta):
                          dataframe_data["etacateg"].append(etacateg[-1])
                          break
                      if etacateg[j] > abs(tt.Jet_eta):
                          dataframe_data["etacateg"].append(etacateg[j-1])
                          break

      # Create dataframe
      df = pd.DataFrame(data=dataframe_data)         
      df = df[df.pt>0]

      #
      # PT PLOT
      #

      fig = plt.figure()

      ax1 = fig.add_subplot(2,1,1)
      ax1.set_ylim(0.3,1.8)

      sns.violinplot(x="ptcateg", y="pt", hue="category",
                     split=True, inner="quart",
                     palette={"JECs only": "royalblue", "DNN regression": "crimson", "No lepton": "royalblue", "With lepton": "crimson"},
                     data=df)

      ax1.set_xlabel("gen jet $p_\\mathrm{T}$ (GeV)",fontsize=20,labelpad=22)
      ax1.set_ylabel("jet gen/reco $p_\\mathrm{T}$",fontsize=20,labelpad=22)
      ax1.get_yaxis().set_tick_params(which='both', direction='in',zorder=99)
      ax1.get_xaxis().set_tick_params(which='both', direction='in',zorder=99)

      low,high = ax1.get_xlim()

      ax2 = fig.add_subplot(2,1,2)

      df2 = pd.read_csv("tmp.csv")

      df2 = df2.iloc[-2*len(ptcateg):]

      jecs25 = df2["q25"].values.tolist()[::2]
      dnn25 = df2["q25"].values.tolist()[1::2]
      jecs50 = df2["q50"].values.tolist()[::2]
      dnn50 = df2["q50"].values.tolist()[1::2]
      jecs75 = df2["q75"].values.tolist()[::2]
      dnn75 = df2["q75"].values.tolist()[1::2]

      jecs = []
      dnn = []

      for r in range(len(jecs25)):
         jecs.append(0.5*(jecs75[r]-jecs25[r])/jecs50[r])
         dnn.append(0.5*(dnn75[r]-dnn25[r])/dnn50[r])

      ax2.set_xlim(ptcateg[0]-(ptcateg[1]-ptcateg[0])/2,ptcateg[-1]+(ptcateg[1]-ptcateg[0])/2)
      ax2.set_xticks(ptcateg)
      ax2.set_xticklabels(["%i" % i for i in ptcateg])
      
      ax2.plot(ptcateg,jecs,label="JECs",color="royalblue",marker=".",linewidth=0,markersize=20)
      ax2.plot(ptcateg,dnn,label="DNN regression",color="crimson",marker="*",linewidth=0,markersize=20)

      ax2.get_yaxis().set_tick_params(which='both', direction='in',zorder=99)
      ax2.get_xaxis().set_tick_params(which='both', direction='in',zorder=99)

      ax2.set_xlabel("gen jet $p_\\mathrm{T}$ (GeV)",fontsize=20,labelpad=22)
      ax2.set_ylabel("$(\\sigma - \\sigma_{\\mathrm{base}}) / \\sigma_{\\mathrm{base}}$",fontsize=20,labelpad=22)

      low,high = ax2.get_ylim()
      ax2.set_ylim(low/1.1,high*1.1)

      jecs_over_dnn = []
      
      print 'pt'
      print 'x,dnn,jecs'
      for i_pt in range(len(jecs)):
          x, dn, je = ptcateg[i_pt], dnn[i_pt], jecs[i_pt]
          jecs_over_dnn.append((1-(dn/je))*100)
          print '%f,%f,%f' % (x, dn, je)

      ax3 = ax2.twinx()
      ax3.get_yaxis().set_tick_params(which='both', direction='in',zorder=99)
      ax3.get_xaxis().set_tick_params(which='both', direction='in',zorder=99)
      ax3.set_ylabel("Improvement (%)",color='goldenrod')
      ax3.plot(ptcateg,jecs_over_dnn,color='goldenrod')
      ax3.set_ylim(ax3.get_ylim()[0]/1.3,ax3.get_ylim()[1]*1.3)
      ax3.tick_params(axis='y',labelcolor='goldenrod')
      ax2.set_xlim(ptcateg[0]-(ptcateg[1]-ptcateg[0])/2,ptcateg[-1]+(ptcateg[1]-ptcateg[0])/2)

      plt.subplots_adjust(wspace=0, hspace=0)

      plt.savefig("%s/bjetreg_%s_violin_%s.png" % (data['opath'],data['year'],key),dpi=300,bbox_inches='tight')
      plt.savefig("%s/bjetreg_%s_violin_%s.pdf" % (data['opath'],data['year'],key),bbox_inches='tight')


      #
      # ETA PLOT
      #

      fig = plt.figure()

      ax1 = fig.add_subplot(2,1,1)
      ax1.set_ylim(0.3,1.8)

      sns.violinplot(x="etacateg", y="pt", hue="category",
                     split=True, inner="quart",
                     palette={"JECs only": "royalblue", "DNN regression": "crimson", "No lepton": "royalblue", "With lepton": "crimson"},
                     data=df)

      ax1.set_xlabel("jet $|\eta|$",fontsize=20,labelpad=22)
      ax1.set_ylabel("jet gen/reco $p_\\mathrm{T}$",fontsize=20,labelpad=22)
      ax1.get_yaxis().set_tick_params(which='both', direction='in',zorder=99)
      ax1.get_xaxis().set_tick_params(which='both', direction='in',zorder=99)

      ax2 = fig.add_subplot(2,1,2)

      df2 = pd.read_csv("tmp.csv")
      df2 = df2.iloc[-2*len(etacateg):]

      jecs25 = df2["q25"].values.tolist()[::2]
      dnn25 = df2["q25"].values.tolist()[1::2]
      jecs50 = df2["q50"].values.tolist()[::2]
      dnn50 = df2["q50"].values.tolist()[1::2]
      jecs75 = df2["q75"].values.tolist()[::2]
      dnn75 = df2["q75"].values.tolist()[1::2]

      jecs = []
      dnn = []

      for r in range(len(jecs25)):
         jecs.append(0.5*(jecs75[r]-jecs25[r])/jecs50[r])
         dnn.append(0.5*(dnn75[r]-dnn25[r])/dnn50[r])

      ax2.set_xlim(etacateg[0]-(etacateg[1]-etacateg[0])/2,etacateg[-1]+(etacateg[1]-etacateg[0])/2)
      ax2.set_xticks(etacateg)
      ax2.set_xticklabels(["%.1f" % float(i) for i in etacateg])

      ax2.plot(etacateg,jecs,label="JECs",color="royalblue",marker=".",linewidth=0,markersize=20)
      ax2.plot(etacateg,dnn,label="DNN regression",color="crimson",marker="*",linewidth=0,markersize=20)

      ax2.get_yaxis().set_tick_params(which='both', direction='in',zorder=99)
      ax2.get_xaxis().set_tick_params(which='both', direction='in',zorder=99)

      ax2.set_xlabel("jet $|\eta|$",fontsize=20,labelpad=22)
      ax2.set_ylabel("$(\\sigma - \\sigma_{\\mathrm{base}}) / \\sigma_{\\mathrm{base}}$",fontsize=20,labelpad=22)

      low,high = ax2.get_ylim()
      ax2.set_ylim(low/1.1,high*1.1)

      jecs_over_dnn = []

      print 'eta'
      print 'x,dnn,jecs'
      for i_pt in range(len(jecs)):
          x, dn, je = etacateg[i_pt], dnn[i_pt], jecs[i_pt]
          jecs_over_dnn.append((1-(dn/je))*100)
          print '%f,%f,%f' % (x, dn, je)

      ax3 = ax2.twinx()
      ax3.get_yaxis().set_tick_params(which='both', direction='in',zorder=99)
      ax3.get_xaxis().set_tick_params(which='both', direction='in',zorder=99)
      ax3.set_ylabel("Improvement (%)",color='goldenrod')
      ax3.plot(etacateg,jecs_over_dnn,color='goldenrod')
      ax3.set_ylim(ax3.get_ylim()[0]/1.3,ax3.get_ylim()[1]*1.3)
      ax3.tick_params(axis='y',labelcolor='goldenrod')
      ax3.set_xlim(etacateg[0]-(etacateg[1]-etacateg[0])/2,etacateg[-1]+(etacateg[1]-etacateg[0])/2)


      plt.subplots_adjust(wspace=0, hspace=0)

      plt.savefig("%s/bjetreg_%s_violin_eta_%s.png" % (data['opath'],data['year'],key),dpi=300,bbox_inches='tight')
      plt.savefig("%s/bjetreg_%s_violin_eta_%s.pdf" % (data['opath'],data['year'],key),bbox_inches='tight')



##----##----##----##----##----##----##
if __name__ == '__main__':
    print sys.argv
    parser = OptionParser()
    parser.add_option('-c','--config', dest='config', default = 'config.json',help='JSON with information', metavar='config')

    (options, args) = parser.parse_args()
	 
    violin(options,args)
