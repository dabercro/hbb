#! /usr/bin/env python

import pandas as pd

df1 = pd.read_csv('old.csv')
df2 = pd.read_csv('pf.csv')

import matplotlib.pyplot as plt

fig, ax = plt.subplots()

plt.plot(df1['jecs'], color='blue')
plt.plot(df1['dnn'], color='indianred')
plt.plot(df2['dnn'], color='gold')

plt.savefig('plot.pdf')
