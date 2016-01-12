#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: Salvatore Zaza
# @Date:   2015-11-24 11:39:28
# @Last Modified by:   Salvatore Zaza
# @Last Modified time: 2015-12-03 15:30:23

import ROOT as r

r.gROOT.ProcessLine(".x mystyle.C")

import numpy as np
from array import array

data = np.loadtxt('../data/151203_calibration_smallgap2.txt')
print data


# hv = array("f",data[:,0])
# err_x = array("f",0*np.ones(len(hv)))
# pmt4 = array("f",data[:,5])

# r4 = data[:,1]
# r6 = data[:,3]
# err_t = (2/100.)**2
# rc = data[:,5]
# err_rc = rc*np.sqrt(1/r4+1/r6+err_t)

# err4_y = array("f",err_rc)

# gr4 = r.TGraphErrors(len(hv), hv, pmt4, err_x, err4_y)

# gr4.SetLineColor(r.kBlue)
# gr4.SetMarkerColor(r.kBlue)

# mgr = r.TMultiGraph()
# mgr.Add(gr4)

# c = r.TCanvas()
# c.SetCanvasSize(1000,500)

# c.SetGrid()

# mgr.Draw("al")
# mgr.GetXaxis().SetTitle('Tensione di alimentazione (V)')
# mgr.GetYaxis().SetTitle('Coincidenze casuali attese (Hz)')
# mgr.GetYaxis().SetTitleOffset(1.2)



# # leg = r.TLegend(0.3,0.6,0.7,0.9)
# #leg.SetTextAlign(13)
# # leg.AddEntry(None, "", "")
# # leg.AddEntry(gr4, "PMT4 & PMT6", "l")
# # leg.SetFillStyle(1001)
# # leg.SetFillColor(r.kWhite)
# # leg.SetBorderSize(0)
# # leg.Draw("same l")

# c.Modified()
# c.Update()

# c.SaveAs("../relazione/fig/random_coincidence.pdf")