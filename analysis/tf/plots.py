#! /usr/bin/env python

import pandas as pd
import matplotlib.pyplot as plt

df1 = pd.read_csv('190814/origin_loss.csv')
df2 = pd.read_csv('190814/puppi_loss.csv')
df3 = pd.read_csv('190814/lstm_loss.csv')


fig, ax = plt.subplots()

plt.plot(df1['Step'], df1['Value'], color='blue', label='Original')
plt.plot(df2['Step'], df2['Value'], color='red', label='PUPPI')
plt.plot(df3['Step'], df3['Value'], color='green', label='LSTM')

plt.legend()
plt.xlabel('Steps')

plt.savefig('plot_loss.pdf')
plt.savefig('plot_loss.png')
