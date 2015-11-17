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
#include "TMath.h"
#include "labstyle.C"
#define MAXEVENTS   2000
using namespace std;

TRandom3 randGen(0);


Double_t bin_width = 0.01; //[cm]

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
  value = (Norm * bin_width) * TMath::Gaus(x[0],mean,sigma,1);
  return value;
}


// Probability Density Function (Normalization = N)
Double_t pdf_2gauss(Double_t *x, Double_t *par)
{
  Double_t Norm ; // Total normalization
  Double_t f1;    // Realtive fraction of Gaussian A
  Double_t mean1; // Mean Gaussian A
  Double_t sigma1; // Sigma Gaussian A
  Double_t mean2; // Mean Gaussian B
  Double_t sigma2; // Sigma Gaussian B


  Norm   = par[0];
  f1     = par[1];
  mean1  = par[2];
  sigma1 = par[3];
  mean2  = par[4];
  sigma2 = par[5];
  
 
  Double_t value;
  value = (Norm * bin_width) * ( f1*TMath::Gaus(x[0],mean1,sigma1,1) + (1-f1)*TMath::Gaus(x[0],mean2,sigma2,1) );
  return value;
}

Double_t straight_line(Double_t *x, Double_t *par)
{
  Double_t m,q; // Exponential slope
 
  q = par[0];
  m = par[1];

  return q + m*x[0];

}


void macroHisto() {
  
  //Setting style
  labstyle();
  
  //Read from file 
  TFile *hfile = new TFile("_root/histo_cerenkovLoop.root");

  TH1F *histo_R         = (TH1F*)hfile->Get("histo_R");
  TH1F *histo_x0        = (TH1F*)hfile->Get("histo_x0");
  TH1F *histo_y0        = (TH1F*)hfile->Get("histo_y0");
  TH1F *histo_FCN       = (TH1F*)hfile->Get("histo_FCN");
  TH1F *histo_n_pixel   = (TH1F*)hfile->Get("histo_n_pixel");
  
  TH1F *histo_R_noTrack            = (TH1F*)hfile->Get("histo_R_noTrack");
  TH1F *histo_R_withTrack          = (TH1F*)hfile->Get("histo_R_withTrack");
  TH2F *histo_R_n_pixel_noTrack    = (TH2F*)hfile->Get("histo_R_n_pixel_noTrack");
  TH2F *histo_R_n_pixel_withTrack  = (TH2F*)hfile->Get("histo_R_n_pixel_withTrack");
  TH2F *histo_R_n_pixel_both       = (TH2F*)hfile->Get("histo_R_n_pixel_both");
  
  histo_R        ->Sumw2();
  histo_x0       ->Sumw2();
  histo_y0       ->Sumw2();
  histo_FCN      ->Sumw2();
  histo_n_pixel  ->Sumw2();
  
  histo_R_n_pixel_noTrack    ->Sumw2();
  histo_R_n_pixel_withTrack  ->Sumw2();
  histo_R_n_pixel_both	     ->Sumw2();
  
  // Create the canvas
  TCanvas *c_R = new TCanvas("c_R","canvas",600,600);
  c_R->cd();
  
  // Create Function
  TF1 *f_R = new TF1("f_R",pdf_2gauss,25.,26.,6);

  f_R->SetParNames("N","f1","m1","s1","m2","s2");
  // Set function parameter  
  f_R->SetParameter(0, 4.98689e+04 );  //Norm Total normalization
  f_R->SetParameter(1, 6.93151e-01 ); //f1 Realtive fraction of Gaussian A
  f_R->SetParameter(2, 2.54111e+01 ); //mean1 Mean Gaussian A
  f_R->SetParameter(3, 6.86677e-02 ); //sigma1 Gaussian A
  f_R->SetParameter(4, 2.56904e+01 ); //mean2 Gaussian B
  f_R->SetParameter(5, 6.84141e-02 ); //sigma2 Gaussian B

  histo_R->Fit(f_R,"","",25.,26.);

  Double_t fit_chi2 = f_R->GetChisquare();
  Int_t    fit_ndof = f_R->GetNDF();
  Double_t fit_prob = f_R->GetProb();

  cout << "chi2 = " << fit_chi2 << endl;
  cout << "ndof = " << fit_ndof << endl;
  cout << "prob = " << fit_prob << endl;
  
  histo_R->GetXaxis()->SetTitle("R [cm] ");
  histo_R->GetYaxis()->SetTitle("Events per 0.01 cm "); 

  TCanvas *c_R_sep = new TCanvas("c_R_sep","canvas",600,600);
  c_R_sep->cd();
  histo_R_withTrack ->SetLineColor(2);
  histo_R_withTrack->Draw();
  histo_R_noTrack->Draw("same");
  histo_R_withTrack->GetXaxis()->SetTitle("R [cm] ");
  histo_R_withTrack->GetYaxis()->SetTitle("Events per 0.01 cm "); 

  std::cout << " ################################################# "<< std::endl; 
  std::cout << " ############## Separation Power ################# "<< std::endl;
  std::cout << " ################################################# "<< std::endl; 

  // Separation power in 1D in R
  Double_t sep_1D;
  sep_1D = (TMath::Abs(f_R->GetParameter(2) - f_R->GetParameter(4))) / f_R->GetParameter(3);
  std::cout << "Separation power 1D = " << sep_1D << std::endl;

  // Separation power in 2D in (R,n_pixel)
  // uno potrebbe calcolae il potere si separazione nello spazio 2D (R,n_pixel)
  // ma quanto possiamo guadagnare?

   Double_t sigma_best;
   sigma_best = TMath::Sqrt( f_R->GetParameter(1)* (1.- f_R->GetParameter(1))/ f_R->GetParameter(0));
   std::cout << "sigma_f(best) = " << sigma_best << std::endl;
   std::cout << "sigma_f = " << f_R->GetParError(1) << std::endl;
   std::cout << "sigma_f(best)/sigma_f = " << sigma_best/f_R->GetParError(1)  << std::endl;


}


