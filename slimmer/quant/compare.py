#! /usr/bin/env python

import os
import pandas as pd

import matplotlib.pyplot as plt

for var in ['pt', 'eta']:

    fig, ax = plt.subplots()

    df4 = pd.read_csv('csvs/pf_%s.csv' % var)
    dfno = pd.read_csv('csvs/nopf_%s.csv' % var)

    plt.plot(df4['x'], df4['jecs'], label='JECs')
    plt.plot(df4['x'], df4['dnn'], label='With PF')
    plt.plot(dfno['x'], dfno['dnn'], label='Without PF')

    plt.legend()

    outputdir = '/home/dabercro/public_html/plots/190612'

    if not os.path.exists(outputdir):
        os.makedirs(outputdir)

    plt.savefig(os.path.join(outputdir, 'plot_compare_%s.pdf' % var))
    plt.savefig(os.path.join(outputdir, 'plot_compare_%s.png' % var))
