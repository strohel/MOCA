#!/usr/bin/env python

from glob import glob
from math import sqrt

import matplotlib.pyplot as plt


files = glob("*-*-*.txt")
for file in files:
    lines = open(file, "r")
    samples = []
    for line in lines:
        samples.append(float(line))
    plt.clf()  # clear figure
    hist_params = {}
    hist_params["bins"] = sqrt(len(samples))  # pocet prihradek
    plt.hist(samples, **hist_params)
    plt.savefig(file[0:-4] + ".png")
