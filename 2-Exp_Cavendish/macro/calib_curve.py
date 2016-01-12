#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: Salvatore Zaza
# @Date:   2015-12-15 12:27:36
# @Last Modified by:   Salvatore Zaza
# @Last Modified time: 2015-12-15 17:52:51
import sys
from utils import *
# from uncertainties import ufloat

filename = '../data/151204_calibration_free1.txt'

c = show_plot(filename,False,True,0.1)


time,voltages = get_data(filename,[])
mins,maxs = get_locals_min_max(time,voltages,0.1)


# V = range(7)
# V[0] = maxs[0][2]-mins[0][2]
# V[1] = maxs[1][2]-mins[0][2]
# V[2] = maxs[1][2]-mins[1][2]
# V[3] = maxs[2][2]-mins[1][2]
# V[4] = maxs[2][2]-mins[2][2]
# V[5] = maxs[3][2]-mins[2][2]
# V[6] = maxs[3][2]-mins[3][2]

S = [13.3,12.2,10.6,9.3,8.2,6.9,5.9]

L = 160

# theta = [s/L for s in S]























# sys.exit(0)


thetas = [ s/(L) for s in S ]

# thetas_p = []
total = thetas[0]
# thetas_p.append(thetas[0])
# for theta in thetas[1:]:
    # total += theta
    # thetas_p.append(total)

# thetas_p
# sys.exit(0)


peaks = mins+maxs

peaks.sort(key = lambda x:x[1]) # sort by time
# total = 0
# V_p = []
V = []
for i in range(1,len(peaks)):
    value = abs(peaks[i-1][2]-peaks[i][2])
    # total += value
    # V_p.append(total)
    V.append(value)

# d = data_plot(V_p,thetas_p)
d1 = data_plot(V,thetas)
g = d1[1]
c = d1[0]

f = g.Fit("pol1")
f.Draw("same")

c.Modified()
c.Update()



raw_input("Press Enter to terminate.")