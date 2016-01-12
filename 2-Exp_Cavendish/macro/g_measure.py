#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: Salvatore Zaza
# @Date:   2015-12-09 10:45:54
# @Last Modified by:   Salvatore Zaza
# @Last Modified time: 2015-12-15 18:43:25

from utils import *
import numpy as np
from uncertainties import ufloat


a1 = show_plot('../data/151209_forzata_1.txt',False,False,0.02)
a2 = show_plot('../data/151209_forzata_2.txt',False,False,0.02)
# b1 = show_plot('../data/151204_static_relax_bassa.txt',False,False,0.02)


# rifare l'unione dei due file considerando la sovrapposizione
filename = '../data/151209_forzata_1_2_v2.txt'
cutx = [(118,None)]
f = show_plot(filename,cutx,True,0.1)

time,voltages = get_data(filename,cutx)

mins,maxs = get_locals_min_max(time,voltages,0.1)
# print mins
# print maxs
c_mins = []
c_maxs = []
c_mins.extend(mins)
c_maxs.extend(maxs)

c_mins.reverse()
c_maxs.reverse()

C = 0.034

b = 0.00136596

T = (2*np.pi)/(0.0337553)

x = np.exp(-b*T/2)

K = 10**-7

R = 0.049

M = 1.49

m = 0.015

d = 0.067



theta_disp = []
theta_pari = []
for i in range(len(mins+maxs)):
    if i%2 == 0:
        V = c_maxs.pop()[2]
        theta = C * V 
        theta_disp.append(theta)
    else:
        V = c_mins.pop()[2]
        theta = C * V 
        theta_pari.append(theta)

N = len(mins+maxs)

# sopprimere ultimo pari
theta_pari = theta_pari[:-1]

# coverto in numpy
theta_disp = np.array(theta_disp)
theta_pari = -1*np.array(theta_pari)

thetas = np.concatenate((theta_disp,theta_pari))

theta_d =  ( (1-x)*np.sum(thetas) - theta_disp[0] + x*theta_disp[-1]  ) / ( N*(1+x) )

G = K * float ( theta_d ) * R**2 / ( 2 * M * m * d)

print G

raw_input("Press Enter to terminate.")