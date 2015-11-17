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

#include "labstyle.C"

#define MAXEVENTS   2000


using namespace std;

TRandom3 randGen(0);

Double_t a_min = 0.; 
Double_t a_max = 10.;


// Probability Density Function (Normalization = N)
Double_t pdf_gauss(Double_t *x, Double_t *par)
{
  Double_t Norm ; // Total normalization
  Double_t mean; // Mean Gaussian A
  Double_t sigma; // Sigma Gaussian A

  Norm  = par[0];
  mean  = par[1];
  sigma = par[2];
 
  Double_t value;
  value = Norm * TMath::Gaus(x[0],mean,sigma,1);
  return value;
}


Double_t straight_line(Double_t *x, Double_t *par)
{
  Double_t m,q; // Exponential slope
 
  q = par[0];
  m = par[1];

  return q + m*x[0];

}


void macroRead2() {

  //Setting style
   labstyle();
     
   // Read the file
   std::ifstream myfile;
   myfile.open("_txt/graph.txt");
   
   Double_t x[MAXEVENTS] = {0.};
   Double_t y[MAXEVENTS] = {0.};
   Double_t x_err[MAXEVENTS] = {0.};
   Double_t y_err[MAXEVENTS] = {0.};

   Int_t j;
   j=0;
   while ( true ) {
     myfile >> x[j] >> y[j] >> x_err[j] >> y_err[j];
     if( myfile.eof() ) break;
     std::cout << "event " << j << " :  " << x[j] << " "<< y[j] << " "
	       << x_err[j] << " " << y_err[j]<<  std::endl;
     ++j;
   };
   
   myfile.close();
   
   Int_t max_events;
   max_events = j;
   

   // Create the canvas
   TCanvas *c_line_smeared = new TCanvas("c_line_smeared","canvas",600,600);
   c_line_smeared->cd();

   //Create a TGraphErrors
   TGraphErrors *g_line = new TGraphErrors(max_events, x,y, x_err, y_err);
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

   Double_t fit_chi2 = f_fit->GetChisquare();
   Int_t    fit_ndof = f_fit->GetNDF();
   Double_t fit_prob = f_fit->GetProb();
   
   cout << "chi2 = " << fit_chi2 << endl;
   cout << "ndof = " << fit_ndof << endl;
   cout << "prob = " << fit_prob << endl;
  

   c_line_smeared->Print("./_fig/graph.pdf");
   c_line_smeared->Print("./_fig/graph.eps");

}


