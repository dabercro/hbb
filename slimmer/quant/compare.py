#! /usr/bin/env python

import pandas as pd

import matplotlib.pyplot as plt

for num in ['4', '6', '8', '12', '16']:
    fig, ax = plt.subplots()

    df4 = pd.read_csv('csvs/190607_pf_wideloss_%s0000_pt.csv' % num)
    dfno = pd.read_csv('csvs/190607_pf_narrowloss_%s0000_pt.csv' % num)

    plt.plot(df4['x'], df4['jecs'], label='JECs')
    plt.plot(df4['x'], df4['dnn'], label='Wide')
    plt.plot(dfno['x'], dfno['dnn'], label='Narrow')

    plt.legend()

    plt.savefig('/home/dabercro/public_html/plots/190611/plot_time_%s0000_compare.pdf' % num)
    plt.savefig('/home/dabercro/public_html/plots/190611/plot_time_%s0000_compare.png' % num)
