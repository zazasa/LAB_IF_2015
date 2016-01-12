#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: Salvatore Zaza
# @Date:   2015-12-03 22:27:50
# @Last Modified by:   Salvatore Zaza
# @Last Modified time: 2015-12-03 23:13:45
import numpy as np 
from array import array
sign = lambda x: (1, -1)[x<0]

def compare(a,b,toll):
    d = a-b;
    if abs(d) < toll:
        return 0
    else:
        return sign(d)


def get_locals_min_max(x,y, toll):
    trend = False
    mins = []
    maxs = []
    last = y[0]
    for i,value in enumerate(y):
        if i == 0: continue
        c = compare(value,last,toll)
        if c != 0 : last = value

        if not trend: 
            trend = c
        elif trend > 0 and c < 0:
            trend = c
            maxs.append((i,x[i],value))
        elif trend < 0 and c > 0:
            trend = c
            mins.append((i,x[i],value))

    return mins,maxs




# sample usage 
if __name__ == '__main__':
    data = np.loadtxt('../data/151203_calibration_smallgap.txt')
    time = array("f",data[4400:,0])
    voltages = array("f",data[4400:,1])
    mins,maxs = get_locals_min_max(time,voltages,0.001)


