#! /usr/bin/env python

import pandas as pd
import matplotlib.pyplot as plt

df1 = pd.read_csv('190607_nopf_narrowloss.csv')
df2 = pd.read_csv('190607_pf_narrowloss.csv')
df3 = pd.read_csv('190603_v6.csv')


fig, ax = plt.subplots()

plt.plot(df1['Step'], df1['Value'], color='blue')
plt.plot(df2['Step'], df2['Value'], color='red')
plt.plot(df3['Step'], df3['Value'], color='green')

plt.savefig('plot_narrow.pdf')
plt.savefig('plot_narrow.png')
