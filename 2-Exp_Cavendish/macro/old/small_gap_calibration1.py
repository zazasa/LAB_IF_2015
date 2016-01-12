#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: Salvatore Zaza
# @Date:   2015-11-24 11:39:28
# @Last Modified by:   Salvatore Zaza
# @Last Modified time: 2015-12-03 18:24:18




import sys
import ROOT as r

r.gROOT.ProcessLine(".x mystyle.C")

import numpy as np
from array import array

data = np.loadtxt('../data/151203_calibration_smallgap.txt')
time = array("f",data[4400:,0])
voltages = array("f",data[4400:,1])

theta = .0124

max_intervals = [
    (0,time.index(460.)),
    (time.index(480.),time.index(500.)),
    (time.index(520.),time.index(560.)),
]


min_intervals = [
    (time.index(460.),time.index(480.)),
    (time.index(500.),time.index(520.)),
    (time.index(560.),time.index(580.)),
]

maxs =[]
for i in max_intervals:
    imax = voltages.index(max(voltages[i[0]:i[1]]))
    maxs.append((time[imax],voltages[imax]))

mmax = np.mean(maxs,axis=0)[1]

mins =[]
for i in min_intervals:
    imin = voltages.index(min(voltages[i[0]:i[1]]))
    mins.append((time[imin],voltages[imin]))

mmin = np.mean(mins,axis=0)[1]

C = (2*theta)/(mmax-mmin)
print C

# sys.exit(0)
gr1 = r.TGraphErrors(len(time), time, voltages)

# hv = array("f",data[:,0])
# err_x = array("f",0*np.ones(len(hv)))
# pmt4 = array("f",data[:,1])
# pmt5 = array("f",data[:,2])
# pmt6 = array("f",data[:,3])

# err4_y = array("f",np.sqrt(data[:,1]))
# err5_y = array("f",np.sqrt(data[:,2]))
# err6_y = array("f",np.sqrt(data[:,3]))

# gr4 = r.TGraphErrors(len(hv), hv, pmt4, err_x, err4_y)
# gr5 = r.TGraphErrors(len(hv), hv, pmt5, err_x, err6_y)
# gr6 = r.TGraphErrors(len(hv), hv, pmt6, err_x, err6_y)

# gr4.SetLineColor(r.kBlue)
# gr4.SetMarkerColor(r.kBlue)

# gr5.SetLineColor(r.kRed)
# gr5.SetMarkerColor(r.kRed)

# gr6.SetLineColor(r.kGreen)
# gr6.SetMarkerColor(r.kGreen)

mgr = r.TMultiGraph()
mgr.Add(gr1)
# mgr.Add(gr5)
# mgr.Add(gr6)

c = r.TCanvas()
c.SetCanvasSize(1000,500)

# c.SetGrid()

mgr.Draw("al")
# mgr.GetXaxis().SetTitle('Tensione di alimentazione (V)')
# mgr.GetYaxis().SetTitle('Conteggi per 100 s')
# mgr.GetYaxis().SetTitleOffset(1.2)

mgr.GetXaxis().SetNdivisions(512)

# leg = r.TLegend(0.3,0.6,0.7,0.9,"Rate vs HV")
# leg.AddEntry(gr4, "PMT4", "l")
# leg.AddEntry(gr5, "PMT5", "l")
# leg.AddEntry(gr6, "PMT6", "l")
# # leg.SetFillStyle(1001)
# # leg.SetFillColor(r.kWhite)
# leg.SetBorderSize(0)
# leg.Draw("same l")

c.Modified()
c.Update()

# c.SaveAs("../relazione/fig/rate-hv.pdf")