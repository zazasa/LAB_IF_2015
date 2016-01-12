#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: Salvatore Zaza
# @Date:   2015-11-24 11:39:28
# @Last Modified by:   Salvatore Zaza
# @Last Modified time: 2015-12-04 12:34:09


import sys
import StringIO
import ROOT as r

# r.gROOT.ProcessLine(".x mystyle.C")

import numpy as np
from array import array
from utils import *





# sample usage
if __name__ == '__main__':
    
    theta = .0124
    # theta = .032

    calc_c = True
    filename = '../data/151203_calibration_smallgap.txt'

    s = open(filename).read().replace(',','.')
    data = np.loadtxt(StringIO.StringIO(s))


    time = array("f",data[:,0])
    voltages = array("f",data[:,1])

    cuts = []
    cuts.append((time.index(440.),time.index(600.)))
    for cutx in cuts:
        time = time[cutx[0]:cutx[1]]
        voltages = voltages[cutx[0]:cutx[1]]

    gr1 = r.TGraphErrors(len(time), time, voltages)

    mgr = r.TMultiGraph()
    mgr.Add(gr1)

    c = r.TCanvas()
    # c.SetCanvasSize(1000,500)
    c.SetGrid()

    mgr.Draw("al")


    mgr.GetXaxis().SetNdivisions(512)

    c.Modified()
    c.Update()

    if calc_c:
        mins,maxs = get_locals_min_max(time,voltages,0.01)

        mmax = np.mean(maxs,axis=0)[2]
        mmin = np.mean(mins,axis=0)[2]

        C = (2*theta)/(mmax-mmin)
        print "theta/dV = %s" %str(C)

        lines = []
        for item in maxs:
            l = r.TLine(item[1],c.GetUymin(),item[1],c.GetUymax())
            l.SetLineColor(r.kRed)
            l.Draw()
            lines.append(l)


        for item in mins:
            l = r.TLine(item[1],c.GetUymin(),item[1],c.GetUymax())
            l.SetLineColor(r.kBlue)
            l.Draw()
            lines.append(l)


        c.Modified()
        c.Update()

    raw_input("Press Enter to terminate.")