#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: Salvatore Zaza
# @Date:   2015-11-24 11:39:28
# @Last Modified by:   Salvatore Zaza
# @Last Modified time: 2015-11-26 15:44:03

import ROOT as r

r.gROOT.ProcessLine(".x mystyle.C")

import numpy as np
from array import array

data = np.loadtxt('delay_curve.dat')

hv = array("f",data[:,0])
err_x = array("f",1*np.ones(len(hv)))
pmt4 = array("f",data[:,1])


err4_y = array("f",np.sqrt(data[:,1]))


gr4 = r.TGraphErrors(len(hv), hv, pmt4, err_x, err4_y)

gr4.SetLineColor(r.kBlue)
gr4.SetMarkerColor(r.kBlue)



mgr = r.TMultiGraph()
mgr.Add(gr4)


c = r.TCanvas()
c.SetCanvasSize(1000,500)

c.SetGrid()

mgr.Draw("a")
mgr.GetXaxis().SetTitle('Ritardo tra i PMT 4 e 6 (ns)')
mgr.GetXaxis().SetNdivisions(511)
mgr.GetYaxis().SetTitle('Coincidenze misurate per 100 s')
mgr.GetYaxis().SetTitleOffset(1.2)


l = r.TLine(1860,0,1860,c.GetUymax())
l.SetLineColor(r.kRed)
l.Draw()

# leg = r.TLegend(0.2,0.7,0.6,1,"")
# leg.AddEntry(None, "", "")
# leg.AddEntry(gr4, "PMT4 & PMT6", "alp")

# leg.SetFillStyle(1001)
# leg.SetFillColor(r.kWhite)
# leg.SetBorderSize(0)
# leg.Draw("same l")

c.Modified()
c.Update()

c.SaveAs("../relazione/fig/delay_curve.pdf")