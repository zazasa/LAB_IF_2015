#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: Salvatore Zaza
# @Date:   2015-12-04 12:49:01
# @Last Modified by:   Salvatore Zaza
# @Last Modified time: 2015-12-15 11:40:10

from utils import *

dataparams = [
    ('../data/151203_calibration_smallgap.txt',.0214,[(440,600)],0.001),
    ('../data/151203_calibration_smallgap2.txt',.0214,[(680,None)],0.001),
    ('../data/151203_calibration_smallgap3.txt',.0214,[(10,120)],0.001),
    ('../data/151204_calibration_smallgap.txt',.0214,[(200,440)],0.02),
    ('../data/151204_calibration_largegap2.txt',.032,[(6,46)],0.05),
    ('../data/151204_calibration_largegap3.txt',.032,[(35,200)],0.02),
    ('../data/151211_small_gap.txt',.0214,[(None,100),(167,392)],0.02),
    ('../data/151214_small_gap.txt',.0214,[(30,100)],0.02),
    ('../data/151214_small_gap_2.txt',.0214,[(5,70),(106,200),(260,326)],0.02),

]

c = show_plot('../data/151203_calibration_smallgap.txt',[(440,600)],True,0.001)
c2 = show_plot('../data/151203_calibration_smallgap2.txt',[(680,None)],True,0.001)
c3 = show_plot('../data/151204_calibration_smallgap.txt',[(200,440)],True,0.02)
c4 = show_plot('../data/151204_calibration_largegap2.txt',[(6,46)],True,0.05)
c5 = show_plot('../data/151204_calibration_largegap3.txt',[(35,200)],True,0.02)
c6 = show_plot('../data/151211_small_gap.txt',[(None,100),(167,392)],True,0.02)

# c6 = show_plot('../data/151211_small_gap.txt',[],False,0.02)
# c7 = show_plot('../data/151211_small_gap_2.txt',[],False,0.02)
c8 = show_plot('../data/151214_small_gap.txt',[(30,100)],True,0.02)
c9 = show_plot('../data/151214_small_gap_2.txt',[(5,70),(106,200),(260,326)],True,0.02)


res = []
for item in dataparams:
    res.append(c_calculation(*item))
    print res[-1]

print "mean: %s" %np.mean(res)
raw_input("Press Enter to terminate.")