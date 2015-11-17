#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "TH1.h"
#include "TF1.h"
#include <TCanvas.h>
#include <TRandom3.h>  
#include <TMinuit.h>
#include "TFile.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TMath.h"
#include "TMatrixDSym.h"
#include "TFitResult.h"

#include "labstyle.C"

using namespace std;

TRandom3 randGen(0);

Double_t a_min = 0.; 
Double_t a_max = 10.;

// Generation
Double_t N     = 100000; // Total normalization
Double_t fs    = 0.3;    // Signal fraction; 
Double_t fA    = 0.7;    // Relative fraction of Gaussian A wrt Gaussian B
Double_t mA    = 4.;     // Mean Gaussian A
Double_t sA    = 0.3;    // Sigma Gaussian A
Double_t mB    = 5.;     // Mean Gaussian A
Double_t sB    = 0.3;    // Sigma Gaussian B
Double_t slope = -0.5;   // Exponential slope



// Probability Density Function (Normalization = N)
Double_t pdf(Double_t x, Double_t *par)
{
  Double_t _N ; // Total normalization
  Double_t _fs; // Signal fraction; 
  Double_t _fA; // Relative fraction of Gaussian A wrt Gaussian B
  Double_t _mA; // Mean Gaussian A
  Double_t _sA; // Sigma Gaussian A
  Double_t _mB; // Mean Gaussian A
  Double_t _sB; // Sigma Gaussian B
  Double_t _slope; // Exponential slope
 
  _N  = par[0];
  _fs = par[1];
  _fA = par[2];
  _mA = par[3];
  _sA = par[4];
  _mB = par[5];
  _sB = par[6];
  _slope = par[7];

  Double_t _fb;
  Double_t _fB;
  
  _fb = 1. - _fs;
  _fB = 1. - _fA;

  Double_t value = _N*( _fs * (_fA*(TMath::Gaus(x,_mA,_sA,1)) + _fB*(TMath::Gaus(x,_mB,_sB,1))) + 
			_fb * (_slope/(TMath::Exp(_slope * a_max)-TMath::Exp(_slope * a_min))) * TMath::Exp(_slope * x)  );
 return value;
}


Double_t pdf_func(Double_t *x, Double_t *par)
 {
     return pdf(x[0],par);

}

Double_t pdf_fit(Double_t *x, Double_t *par)
 {
   Double_t bin_width;
   bin_width =0.1;
   return bin_width*pdf(x[0],par);
}




void fitter() {

  // Style
  labstyle();
  
  //Create Canvas
  TCanvas *c_pdf_gen = new TCanvas("c_pdf_gen","canvas histo",500,500);
  c_pdf_gen->cd();
 
  // Create Function
  TF1 *f_pdf = new TF1("f_pdf",pdf_func,a_min,a_max,8);
  // Set function parameter
  f_pdf->SetParameter(0, 1.    );//N
  f_pdf->SetParameter(1, fs    );//fs
  f_pdf->SetParameter(2, fA    );//fA
  f_pdf->SetParameter(3, mA    );//mA
  f_pdf->SetParameter(4, sA    );//sA
  f_pdf->SetParameter(5, mB    );//mB
  f_pdf->SetParameter(6, sB    );//sB
  f_pdf->SetParameter(7, slope );//slope
  
  // Set histogram axis labels
  f_pdf->GetXaxis()->SetTitle("Invariant mass [GeV/#font[10]{c^{2}}] ");
  f_pdf->GetYaxis()->SetTitle("a.u.");

  // Draw histogram
  f_pdf ->Draw();
      
  // Save canvas in .pdf and .epr format 
  c_pdf_gen->Print("./_fig/pdf_gen.pdf");
  c_pdf_gen->Print("./_fig/pdf_gen.eps");

  // ######################################################################
  // ##################### Data sample generation #########################
  // ######################################################################

  //Create Canvas
  TCanvas *c_histo_gen = new TCanvas("c_histo_gen","canvas histo",500,500);
  c_histo_gen->cd();
 
  //Create Histrogram
  TH1F *histo_gen = new TH1F("histo","",100,a_min,a_max);
  
  //Fill the histogram
  Double_t x_var;
  Double_t y_var;
  
  Int_t i;
  i=0;
  while(i<N){
    x_var= a_min + (a_max - a_min)*randGen.Uniform();
    y_var= 0.4 * randGen.Uniform();  
    if(y_var < f_pdf->Eval(x_var)){
    histo_gen->Fill(x_var);
    i++;
    };
  };

  // Set histogram Axis Labels
  histo_gen->GetXaxis()->SetTitle("Invariant mass [GeV/#font[10]{c^{2}}] ");
  histo_gen->GetYaxis()->SetTitle("Events per 0.1 GeV/#font[10]{c^{2}} "); 
  
  // Plot the histogram
  histo_gen -> Draw();

  // Save canvas in .pdf and .epr format
  c_histo_gen->Print("./_fig/histo_gen.pdf");
  c_histo_gen->Print("./_fig/histo_gen.eps");


  // ######################################################################
  // ############################# Fitter #################################
  // ######################################################################

  // Create Function
  TF1 *f_pdf_fit = new TF1("f_pdf_fit",pdf_fit,a_min,a_max,8);
  // Set Initial Parameters  (usually you don't know precisely them!!!)
  f_pdf_fit->SetParameter(0, 100000 );//N
  f_pdf_fit->SetParameter(1,  0.3   );//fs
  f_pdf_fit->SetParameter(2,  0.7   );//fA
  f_pdf_fit->SetParameter(3,  4.    );//mA
  f_pdf_fit->SetParameter(4,  0.3   );//sA
  f_pdf_fit->SetParameter(5,  5.    );//mB
  f_pdf_fit->SetParameter(6,  0.3   );//sB
  f_pdf_fit->SetParameter(7, -0.5   );//slope

  f_pdf_fit->SetParNames("N","fs","fA","mA","sA","mB","sB","slope");

  cout << "#####################  Minimal Fit ##################" << endl;

  // For more details please take a look at the ROOT Referecence Guide 
  // In particular for TH1 --> http://root.cern.ch/root/html/TH1.html#TH1:Fit 
  // In particular for TGraph and TGraphErrors --> https://root.cern.ch/root/html/TGraph.html
  histo_gen -> Fit(f_pdf_fit,"","",a_min,a_max);
 
  Double_t fit_chi2 = f_pdf_fit->GetChisquare();
  Int_t    fit_ndof = f_pdf_fit->GetNDF();
  Double_t fit_prob = f_pdf_fit->GetProb();

  cout << "chi2 = " << fit_chi2 << endl;
  cout << "ndof = " << fit_ndof << endl;
  cout << "prob = " << fit_prob << endl;

  cout << "#####################  Fit and Covariance Matrix ##################" << endl;

  // To access coveriance matrix and other fit information

  TFitResultPtr r = histo_gen->Fit(f_pdf_fit,"S","",a_min,a_max);  //Fit(myFunc,"S");
  TMatrixDSym cov = r->GetCovarianceMatrix();  //  to access the covariance matrix
  Double_t chi2   = r->Chi2(); // to retrieve the fit chi2
  Double_t par0   = r->Parameter(0); // retrieve the value for the parameter 0
  Double_t err0   = r->ParError(0); // retrieve the error for the parameter 0
  r->Print("V");     // print full information of fit including covariance matrix
  
  cout << endl;
  cout << "chi2 = " << chi2 << endl;
  cout << "par0 = " << par0 << endl;
  cout << "err0 = " << err0 << endl;
  cout << "cov(2,3)=" << cov(2,3) << endl;
  
  // Save canvas in .pdf and .epr format
  c_histo_gen->Print("./_fig/histo_fit.pdf");
  c_histo_gen->Print("./_fig/histo_fit.eps");


}


