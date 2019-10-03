#! /usr/bin/env python

import os
import pandas as pd

import matplotlib.pyplot as plt

for var in ['pt', 'eta']:

    fig, ax = plt.subplots()

    df0 = pd.read_csv('csvs/190723_origin_%s.csv' % var)
#    df1 = pd.read_csv('csvs/190723_puppi_%s.csv' % var)
#    df2 = pd.read_csv('csvs/190725_lstm_pf_%s.csv' % var)
    df3 = pd.read_csv('csvs/190904_0_2_%s.csv' % var)

    plt.plot(df0['x'], df0['jecs'], label='JECs')
    plt.plot(df0['x'], df0['dnn'], label='Original')
#    plt.plot(df1['x'], df1['dnn'], label='PUPPI')
#    plt.plot(df2['x'], df2['dnn'], label='LSTM')
    plt.plot(df3['x'], df3['dnn'], label='PUPPI')

    plt.xlabel({'pt': '$p_T$ [GeV]', 'eta': '$\eta$'}[var])
    plt.ylabel('Relative Resolution ($\sigma/\mu$)')

    plt.legend(loc=('upper left' if var == 'eta' else 'upper right'))

    outputdir = '/home/dabercro/public_html/plots/191003_compareres'

    if not os.path.exists(outputdir):
        os.makedirs(outputdir)

    plt.savefig(os.path.join(outputdir, 'plot_compare_%s.pdf' % var))
    plt.savefig(os.path.join(outputdir, 'plot_compare_%s.png' % var))
