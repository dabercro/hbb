import ROOT
from ROOT import gROOT, TFile, TTree, TChain, gPad, gDirectory, AddressOf, TH1F
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
import seaborn as sns
from pprint import pprint
from scipy.stats import norm
from scipy.optimize import curve_fit
from scipy import asarray as ar,exp
from scipy.special import erf

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

def gaus(x,a,x0,sigma, b, m):
   return a*exp(-(x-x0)**2/(2*sigma**2)) + b*x + m

def gausserfexp(x,a,x0,sigma,b,c):
   gausspart = a*exp(-(x-x0)**2/(2*sigma**2))
   erfexppart = 0.5*c*(1+(erf(x-x0)/b))
   return erfexppart + gausspart

def plot_binned_data(axes, binedges, data,
              *args, **kwargs):
   #The dataset values are the bin centres
   x = (binedges[1:] + binedges[:-1]) / 2.0
   #The weights are the y-values of the input binned data
   weights = data
   return axes.hist(x, bins=binedges, weights=weights,
              *args, **kwargs)

def load_data(jsonfile):
	with open(jsonfile) as config_file:
		data = json.load(config_file)

	return data

def main(options,args):
	data = load_data(options.config)

        fig = plt.figure()
        ax = plt.subplot()
        ax.set_xlim(data['xmin'],data['xmax'])
        ax.set_xlabel(data["xtitle"],fontsize=20,labelpad=22)
        ax.set_ylabel(data["ytitle"],fontsize=20,labelpad=22)
        ax.get_yaxis().set_tick_params(which='both', direction='in',zorder=99)
        ax.get_xaxis().set_tick_params(which='both', direction='in',zorder=99)

        maxy = 0

	for key in data['mc']['samples']:
           #print data['mc']['samples'][key]['file']

	   #Open file
           ifile = data['mc']['path'] + data['mc']['samples'][key]['file']
           print "Opening ..." + ifile
           tf = ROOT.TFile(ifile);
           tt = tf.Get("events");

	   #Get histogram
           hist = TH1F("tmp","tmp",data['nbinsx'],data['xmin'],data['xmax'])
           weightstring = "(" + data['mc']['weights']+ ")*" + "(" + data['cuts'] + ")"


           tt.Project(hist.GetName(),data['mc']['samples'][key]['variable'],weightstring)

           if data['normalize']:
              hist.Scale(1./hist.Integral())

           #Get histogram contents
           value = []
           xmin = []
           xcen = []
           xmax = []
           error = []

           for i in range(1,hist.GetNbinsX()+1):
              if hist.GetBinContent(i) > maxy:
                 maxy = hist.GetBinContent(i)
              value.append(hist.GetBinContent(i))
              xmin.append(hist.GetBinLowEdge(i))
              xcen.append(hist.GetBinLowEdge(i)+hist.GetBinWidth(i)/2)
              xmax.append(hist.GetBinLowEdge(i)+hist.GetBinWidth(i))
              error.append(hist.GetBinError(i)*10)

           bins = np.linspace(xmin[0],xmax[len(value)-1],len(value)+1)
        
           plot_binned_data(ax,bins,value,histtype='step',color=data['mc']['samples'][key]['color'],normed=0,linewidth=2,rwidth=2,label=data['mc']['samples'][key]['label'])
           #plt.errorbar(xcen, value, error,fmt="o",color=data['mc']['samples'][key]['color'],linewidth=1,markersize=0)

           if data['fit']:
              x = xcen
              y = value

              n = len(x)                          #the number of data
              mean = np.sum(np.array(x)*np.array(y))/n                   #note this correction
              sigma = np.sum(y*(x-mean)**2)/n        #note this correction
              
              
              # Gauss fit
              #popt,pcov = curve_fit(gaus,x,y,p0=[1,mean,sigma, -0.00002, 0.005])
              #plt.plot(x,gaus(x,*popt),'-',color=data['mc']['samples'][key]['color'],linewidth=0.7,label="$\\overline{m_\\mathrm{H}}=%.1f\,\\mathrm{GeV}$, $\\sigma=%.1f\,\\mathrm{GeV}$" % (popt[1],abs(popt[2])))
              #print popt[2]

              # Errexpgauss fit
              popt,pcov = curve_fit(gausserfexp,x,y,p0=[1,mean,sigma,10,12])
              plt.plot(x,gausserfexp(x,*popt),'-',color=data['mc']['samples'][key]['color'],linewidth=0.7,label="$\\overline{m_\\mathrm{H}}=%.1f\,\\mathrm{GeV}$, $\\sigma=%.1f\,\\mathrm{GeV}$" % (popt[1],abs(popt[2])))


        for label in data['labels']:
           x = label.split(":")[0]
           y = label.split(":")[1]
           size = label.split(":")[2]
           text = label.split(":")[3]
           plt.text(x,y,text,fontsize=size,transform=ax.transAxes)
           

        ax.set_ylim(0,1.3*maxy)
        ax.legend(loc=1,ncol=1,fontsize=9)
	plt.savefig("%s/regression_new.png" % data['opath'],dpi=300,bbox_inches='tight')
	plt.savefig("%s/regression_new.pdf" % data['opath'],bbox_inches='tight')



##----##----##----##----##----##----##
if __name__ == '__main__':
	parser = OptionParser()
	parser.add_option('-c','--config', dest='config', default = 'config.json',help='JSON with information', metavar='config')
	#parser.add_option('-ddt','--ddtfile', dest='ddtfile', default = '/home/bmaier/cms/MonoHiggs/higgstagging/N2DDT/h3_n2ddt.root',help='n2ddr.root file', metavar='ddtfile')

	(options, args) = parser.parse_args()
	 
	main(options,args)

