#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include "TFile.h"
#include <TCanvas.h>
#include <iostream>
#include <fstream>  // necessario per scrivere su file
#include <string>
#include <TRandom3.h>
#include <TMinuit.h>
#include "TH1.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TMath.h"
#include "TMatrixDSym.h"
#include "TFitResult.h"

#include "labstyle.C" // stile per i grafici


#define MAXEVENTS   1000
#define MAXEVENTS_2 20

using namespace std;

TRandom3 randGen(0);

Double_t a_min = 0.; 
Double_t a_max = 10.;



Double_t straight_line(Double_t *x, Double_t *par)
{
  Double_t m,q; // Exponential slope
 
  q = par[0];
  m = par[1];

  return q + m*x[0];

}



void macroWrite() {

  //Setting style
  labstyle();
   
  

   // ###################################################################
   // ##################### Create histrogram ###########################
   // ###################################################################

   // Create the canvas
   TCanvas *c_histo = new TCanvas("c_histo","canvas histo",400,400);
   c_histo->cd();
   
   //Create the histogram
   TH1F *histo = new TH1F("histo","",100,-4.,4.);
   
   //Event generation (Single Gaussia mean=0, sigma=1)
   Double_t xvar[MAXEVENTS];
   for(Int_t i=0;i<MAXEVENTS;++i){
     xvar[i]= randGen.Gaus(0,1);
     histo->Fill(xvar[i]);
   };

   histo->Draw();

   // Write on .txt file
   std::ofstream myfile;
   myfile.open("_txt/gaussian.txt");
   
   for(Int_t i=0; i<MAXEVENTS; ++i){
     myfile << xvar[i] << std::endl;
   };
   myfile.close();

   // Write on .root file the histogram
   TFile *hfile = new TFile("./_root/histo.root","RECREATE","Demo ROOT file with histograms");
   hfile->cd();
   histo->Write();
   hfile->Close();

   // ###################################################################
   // ################# Create a TGraphErrors ###########################
   // ###################################################################


   // Create the canvas
   TCanvas *c_line = new TCanvas("c_line","canvas histo",400,400);
   c_line->cd();

   //Double_t a_min, a_max;
   a_min = 0.;
   a_max =10.;
   

   // Create Function
   TF1 *f_line = new TF1("f_line",straight_line,a_min,a_max,2);
   // Set function parameter
   f_line->SetParameter(0, 1.);//q
   f_line->SetParameter(1, 0.3);//m
   f_line->SetMinimum(0.);//m
   f_line->Draw();
   

   Double_t x_val[MAXEVENTS_2];
   Double_t y_val[MAXEVENTS_2];
   Double_t x_err[MAXEVENTS_2];
   Double_t y_err[MAXEVENTS_2];

   Double_t error_x = 0.1;
   Double_t error_y = 0.1;


   for(Int_t i=0; i<MAXEVENTS_2; ++i){
     x_val[i] = a_min + i*(a_max-a_min)/(MAXEVENTS_2);
     x_val[i] = x_val[i] + randGen.Gaus(0.,error_x);
     x_err[i] = error_x;
     y_val[i] = f_line->Eval(x_val[i]);
     y_val[i] = y_val[i] + randGen.Gaus(0.,error_y);
     y_err[i] = error_y;
     //cout << i <<" ---- "<<x_val[i] << " --- " << y_val[i] << endl;
   };


   // Write on .txt file
   std::ofstream myfile2;
   myfile2.open("_txt/graph.txt");
   
   for(Int_t i=0; i<MAXEVENTS_2; ++i){
     myfile2 << x_val[i] << " " << y_val[i] << " " << x_err[i]<< " " << y_err[i] << std::endl;
   };

   myfile2.close();


   // Create the canvas
   TCanvas *c_line_smeared = new TCanvas("c_line_smeared","canvas",400,400);
   c_line_smeared->cd();

   //Create a TGraphErrors
   TGraphErrors *g_line = new TGraphErrors(MAXEVENTS_2, x_val,y_val, x_err, y_err);
   g_line->SetMarkerStyle(20);
   g_line->SetMinimum(0);
   // Drawing options in http://root.cern.ch/root/html/THistPainter.html
   g_line->Draw("AP");

   // Create Function
   TF1 *f_fit = new TF1("f_fit",straight_line,a_min,a_max,2);
   // Set function parameter
   f_fit->SetParameter(0, 1.);//q
   f_fit->SetParameter(1, 0.3);//m
   g_line-> Fit(f_fit,"","",a_min,a_max);

   c_line_smeared->Print("./_fig/graph.pdf");
   c_line_smeared->Print("./_fig/graph.eps");
  


}


