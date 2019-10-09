#! /usr/bin/env python

import os
import pandas as pd

import datetime

import matplotlib.pyplot as plt

outputdir = '/home/dabercro/public_html/plots/%s_compareres' % datetime.date.strftime(datetime.datetime.now(), '%y%m%d')

for var in ['pt', 'eta']:

    fig, ax = plt.subplots()

#    df0 = pd.read_csv('csvs_191009/190723_origin_%s.csv' % var)
    df0_2 = pd.read_csv('csvs_191009/190723_origin_2_%s.csv' % var)
#    df0_3 = pd.read_csv('csvs_191009/190723_origin_3_%s.csv' % var)
#    df1 = pd.read_csv('csvs_191009/190723_puppi_%s.csv' % var)
#    df1_2 = pd.read_csv('csvs_191009/190723_puppi_2_%s.csv' % var)
    df2 = pd.read_csv('csvs_191009/190725_lstm_pf_%s.csv' % var)
#    df3 = pd.read_csv('csvs_191009/190904_0_%s.csv' % var)
#    df3_2 = pd.read_csv('csvs_191009/190904_0_2_%s.csv' % var)
    df3_3 = pd.read_csv('csvs_191009/190904_0_3_%s.csv' % var)
#    df4 = pd.read_csv('csvs_191009/190924_0_%s.csv' % var)

    plt.plot(df0_2['x'], df0_2['jecs'], label='JECs')
#    plt.plot(df0['x'], df0['dnn'], label='Original')
    plt.plot(df0_2['x'], df0_2['dnn'], label='Original')
#    plt.plot(df0_3['x'], df0_3['dnn'], label='Original 3')
#    plt.plot(df1['x'], df1['dnn'], label='PUPPI')
#    plt.plot(df1_2['x'], df1_2['dnn'], label='PUPPI 2')
    plt.plot(df2['x'], df2['dnn'], label='LSTM')
#    plt.plot(df3['x'], df3['dnn'], label='PUPPI R')
#    plt.plot(df3_2['x'], df3_2['dnn'], label='PUPPI R 2')
    plt.plot(df3_3['x'], df3_3['dnn'], label='PUPPI')
#    plt.plot(df4['x'], df4['dnn'], label='Big Batch')

    plt.xlabel({'pt': '$p_T$ [GeV]', 'eta': '$\eta$'}[var])
    plt.ylabel('Relative Resolution ($\sigma/\mu$)')

    plt.legend(loc=('upper left' if var == 'eta' else 'upper right'))

    if not os.path.exists(outputdir):
        os.makedirs(outputdir)

    plt.savefig(os.path.join(outputdir, 'plot_compare_%s.pdf' % var))
    plt.savefig(os.path.join(outputdir, 'plot_compare_%s.png' % var))

os.system('cp %s %s/models.cnf' % (__file__, outputdir))
