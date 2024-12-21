#!/usr/bin/env python
# coding: utf-8


import numpy as np
import pandas as pd
from matplotlib import pyplot as plt

import matplotlib
COLORS = matplotlib.rcParams['axes.prop_cycle'].by_key()['color']


def plot_true_pred(x, y, y_pred, ylabel, legend, figname):
    plt.figure()
    plt.plot([], [], c='k', label='True')
    plt.plot([], [], '^--', c='k', label='Pred')
    for i in range(2):
        plt.plot(t, y[:,i], c=COLORS[i], lw=1.)
        plt.plot(t, y_pred[:,i], c=COLORS[i], ls='--', marker='^', markevery=200, lw=1.)
        plt.plot([], [], c=COLORS[i], label=legend[i])
    plt.xscale('log')
    plt.xlabel('t [s]')
    plt.ylabel(ylabel)
    plt.legend()
    plt.savefig(figname)
    plt.close()


data = pd.read_csv('true.csv').to_numpy()
t = np.take(data, [4], axis=1)
y = np.take(data, [5,6,7,8], axis=1)
y_pred = np.loadtxt('./../output/pred.csv')

for k in ('mass','temp'):
    idx = [0,1] if k == 'mass' else [2,3]
    if k == 'mass':
        idx = [0,1]
        legend = ['Y$_O$', 'Y$_{O_2}$']
        ylabel = 'Y'
    else:
        idx = [2,3]
        legend = ['$T_h$', '$T_v$']
        ylabel = 'T [K]'
    plot_true_pred(
        t,
        np.take(y, idx, 1),
        np.take(y_pred, idx, 1),
        ylabel,
        legend,
        f'./../output/{k}.png'
    )
