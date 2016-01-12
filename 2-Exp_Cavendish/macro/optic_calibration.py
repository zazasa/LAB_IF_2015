#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: Salvatore Zaza
# @Date:   2015-12-04 12:49:01
# @Last Modified by:   Salvatore Zaza
# @Last Modified time: 2015-12-15 10:54:58

from utils import *

dataparams = [
    # ('../data/151203_calibration_smallgap.txt',.0214,(440,600),0.001),
    # ('../data/151203_calibration_smallgap2.txt',.0214,(680,None),0.001),
    # ('../data/151203_calibration_smallgap3.txt',.0214,(10,120),0.001),
    # ('../data/151204_calibration_largegap2.txt',.032,(6,46),0.05),
    # ('../data/151204_calibration_largegap3.txt',.032,(35,200),0.02),
    # ('../data/151204_calibration_smallgap.txt',.024,(162,440),0.02),
]

filename = '../data/151204_calibration_free1.txt'
c = show_plot(filename,False,True,0.1)
time,voltages = get_data(filename, False)
mins,maxs = get_locals_min_max(time,voltages,0.1)

for i in range(len(mins)):
    min = mins[i][2]
    max = maxs[i][2]
    print max-min



for item in dataparams:
    print c_calculation(*item)

raw_input("Press Enter to terminate.")