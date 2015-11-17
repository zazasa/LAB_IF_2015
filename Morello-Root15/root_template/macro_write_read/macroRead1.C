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




void macroRead1() {

  //Setting style
   labstyle();
   

   // ###################################################################
   // ##################### Create histrogram ###########################
   // ###################################################################

   // Create the canvas
   TCanvas *c_histo_gauss = new TCanvas("c_histo_gauss","canvas histo",600,600);
   c_histo_gauss->cd();
   
   //Create the histogram
   TH1F *histo_gauss = new TH1F("histo_gauss","",100,-4.,4.);
   
   std::ifstream myfile;
   myfile.open("_txt/gaussian.txt");
   
   Double_t x[MAXEVENTS] = {0.};
   Int_t j;
   j=0;
   while ( true ) {
     myfile >> x[j];
     if( myfile.eof() ) break;
     //std::cout << "event " << j << " :  " << x[j] << std::endl;
     ++j;
   };
   
   myfile.close();
   
   Int_t max_histo_gauss;
   max_histo_gauss = j;
   
   //Fill the histrogram 
   for(Int_t i=0;i<max_histo_gauss;++i){
     histo_gauss->Fill(x[i]);
   };
   
   histo_gauss->Draw();
   
   //Fit the histogram
   // Create Function
   TF1 *f_fit_gauss = new TF1("f_fit_gauss",pdf_gauss,-4.,4.,3);
   // Set function parameter
   f_fit_gauss->SetParameter(0, 30.);//N
   f_fit_gauss->SetParameter(1, 0.);//mean
   f_fit_gauss->SetParameter(2, 1.);//sigma
   
   histo_gauss-> Fit(f_fit_gauss,"","",-4.,4.);
   histo_gauss->Draw();
   
   c_histo_gauss->Print("./_fig/histo_gauss.pdf");
   c_histo_gauss->Print("./_fig/histo_gauss.eps");

   //################################################################################

   //Read histogram from .root file
   TFile *file = new TFile("_root/histo.root"); 
   TH1F *histo = (TH1F*)file->Get("histo"); 
   
   // Create the canvas
   TCanvas *c_histo_fromfile = new TCanvas("c_histo_fromfile","canvas histo",600,600);
   c_histo_fromfile->cd();

   histo->SetMarkerStyle(20);
   histo->SetMarkerSize(0.8);
   histo->SetMarkerColor(kRed);
   histo->SetMinimum(0);
   //histo->SetFillColor(4);
   histo->Draw("e");
 
}


