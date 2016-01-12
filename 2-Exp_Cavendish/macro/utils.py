#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: Salvatore Zaza
# @Date:   2015-12-03 22:27:50
# @Last Modified by:   Salvatore Zaza
# @Last Modified time: 2015-12-15 12:40:40
import numpy as np 
import StringIO
import ROOT as r
from array import array
from uncertainties import ufloat

sign = lambda x: (1, -1)[x<0]

def compare(a,b,toll):
    d = a-b;
    # print b,a,d,abs(d)>toll
    if abs(d) < toll:
        return 0
    else:
        return sign(d)

def cut_gap_check(x,i,toll):
    # print len(x), i
    # print x[i],x[i-1],x[i+1]
    if i > (len(x)-1) or i == 0 : return False

    if round(x[i]-x[i-1],2) != round(x[i+1]-x[i],2) : return False


    return True

def get_locals_min_max(x,y, toll):
    trend = False
    mins = []
    maxs = []
    last = 0
    # print y
    for i,value in enumerate(y):
        if i == 0: continue

        c = compare(value,y[last],toll)

        # print trend,value,y[last]
        
        if trend > 0 and c < 0:
            trend = c

            maxV = max(y[last-1:i+1])
            
            indexMax = last -1 + y[last-1:i+1].index(maxV)
            
            # print cut_gap_check(x,indexMax)
            if not cut_gap_check(x,indexMax,toll): continue

            maxs.append((indexMax,x[indexMax],y[indexMax]))
            # print "max %s" %y[indexMax]

        elif trend < 0 and c > 0:
            trend = c

            minV = min(y[last-1:i+1])

            indexMin = last -1 + y[last-1:i+1].index(minV)
            
            if not cut_gap_check(x,indexMin,toll): continue

            mins.append((indexMin,x[indexMin],y[indexMin]))

            # print "min %s" %y[indexMin]


        if c != 0 : 
            last = i
            if not trend: trend = c

    return mins,maxs


def get_data(filename, cutx):
    s = open(filename).read().replace(',','.')
    data = np.loadtxt(StringIO.StringIO(s))
    time = array("f",data[:,0])
    voltages = array("f",data[:,1])

    out_time = array("f")
    out_voltages = array("f")

    if not cutx:
        out_time.extend(time)
        out_voltages.extend(voltages)        

    else:
        for cut in cutx:
    # if cutx:
            a = time.index(cut[0]) if cut[0] in time else None
            b = time.index(cut[1]) if cut[1] in time else None

            if a or b:
                out_time.extend(time[a:b])
                out_voltages.extend(voltages[a:b])


    return out_time,out_voltages


def c_calculation(filename,theta,cutx,toll):
    time,voltages = get_data(filename,cutx)
    
    mins,maxs = get_locals_min_max(time,voltages,toll)
    # print mins,maxs
    mmax = ufloat(np.mean(maxs,axis=0)[2],np.std(maxs,axis=0)[2])
    mmin = ufloat(np.mean(mins,axis=0)[2],np.std(mins,axis=0)[2])
    # print mmax,np.std(maxs,axis=0)[2], np.std(maxs,axis=0)[2]/mmax
    # print mmin,np.std(mins,axis=0)[2]

    # print mmax-mmin

    C = (2*ufloat(theta,0.0003))/(mmax-mmin)
    return C


def show_plot(filename,cutx, picks, toll):
    # r.gROOT.ProcessLine(".x mystyle.C")
    time,voltages = get_data(filename,cutx)

    gr1 = r.TGraphErrors(len(time), time, voltages)
    gr1.SetMarkerSize(2)
    gr1.SetMarkerStyle(7)

    mgr = r.TMultiGraph()
    mgr.Add(gr1)

    c = r.TCanvas()
    # c.SetCanvasSize(1000,500)
    c.SetGrid()

    mgr.Draw("ap")

    mgr.GetXaxis().SetNdivisions(514)

    lines=[]
    if picks:
        mins,maxs = get_locals_min_max(time,voltages,toll)
        # print mins,maxs
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

    return [c,gr1,mgr,lines]


def data_plot(x,y):
    time = array("f",x)
    voltages = array("f",y)

    gr1 = r.TGraphErrors(len(time), time, voltages)
    gr1.SetMarkerSize(2)
    gr1.SetMarkerStyle(7)

    mgr = r.TMultiGraph()
    mgr.Add(gr1)

    c = r.TCanvas()
    # c.SetCanvasSize(1000,500)
    c.SetGrid()

    mgr.Draw("ap")

    mgr.GetXaxis().SetNdivisions(514)

    # lines=[]
    # if picks:
    #     mins,maxs = get_locals_min_max(time,voltages,toll)
    #     # print mins,maxs
    #     for item in maxs:
    #         l = r.TLine(item[1],c.GetUymin(),item[1],c.GetUymax())
    #         l.SetLineColor(r.kRed)
    #         l.Draw()
    #         lines.append(l)


    #     for item in mins:
    #         l = r.TLine(item[1],c.GetUymin(),item[1],c.GetUymax())
    #         l.SetLineColor(r.kBlue)
    #         l.Draw()
    #         lines.append(l)

    c.Modified()
    c.Update()

    return [c,gr1,mgr]


# sample usage 
if __name__ == '__main__':
    filename = '../data/151204_calibration_largegap2.txt'
   
    time,voltages = get_data(filename,(13,17))
    mins,maxs = get_locals_min_max(time,voltages,0.05)

    C = c_calculation(filename,.032,(6,46), 0.05)

    print "C = %s" %C


    c = show_plot(filename,False,False, 0.001)

    c2 = show_plot(filename,(6,46),True, 0.05)

    raw_input("Press Enter to terminate.")