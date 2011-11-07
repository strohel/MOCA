#!/usr/bin/env python

from glob import glob

import matplotlib.pyplot as plt


files = glob("*-*-*.txt")
for file in files:
    lines = open(file, "r")
    samples = []
    for line in lines:
        samples.append(float(line))
    plt.clf()  # clear figure
    hist_params = {}
    if "binom" in file:
        hist_params["bins"] = range(1, 21)  # aby to vypadalo hezky, musime rucne specifikovat kose
    else:
        hist_params["bins"] = 30  # dame jen pocet, matplotlib to uz nejak prechrousta
    plt.hist(samples, **hist_params)
    plt.savefig(file[0:-4] + ".png")
