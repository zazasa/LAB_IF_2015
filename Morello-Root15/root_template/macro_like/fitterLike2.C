#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "TFile.h"
#include <TCanvas.h>
#include <TRandom.h>
#include <TRandom3.h>
#include <TMinuit.h>
#include "TH1.h"
#include "TF1.h"
#include "Riostream.h"
#include "TGraphErrors.h"
#include "TROOT.h"
#include "TStyle.h"
#include <TH2.h>
#include <TH2F.h>
#include <TH3.h>
#include <TH3F.h>
#include "TF2.h"
#include "TGraph2D.h"
#include "TH1F.h"
#include "TMath.h"
#include "TFitter.h"
#include <string>
#include <iomanip>

#include "labstyle.C"

using namespace std;

#define MAX 10000 //<---Numero di eventi
#define PARAMETERS 5 //<-- Numero massimo di parametri
Double_t xvar[MAX]; 

TRandom3 randGen(0);
Double_t mA= -1.;
Double_t mB= 1.;
Double_t sA= 0.5;
Double_t sB= 0.5;

Double_t pdf(Double_t x, Double_t *par)
{

 Double_t _fA;
 Double_t _mA;
 Double_t _mB;
 Double_t _sA;
 Double_t _sB;
 
 _fA  = par[0];				
 _mA  = par[1];
 _mB  = par[2];
 _sA  = par[3];
 _sB  = par[4];

 Double_t value = _fA*(TMath::Gaus(x,_mA,_sA,1)) + (1-_fA)*(TMath::Gaus(x,_mB,_sB,1));
 return value;
}

Double_t pdf_proj(Double_t *x, Double_t *par)
{
  Double_t bin_width;
  bin_width =0.08;
  return (MAX*bin_width)*pdf(x[0],par);

}


void fcn(Int_t &, Double_t *, Double_t &f, Double_t *par, Int_t )
{

  Double_t Like=0;
  Int_t i=0;
  for(i=0;i<MAX;++i){
   Like = Like + TMath::Log(pdf(xvar[i],par));
  };
   f= - 2. * Like;
}


void fitterLike2() {
  
  //Setting style
   labstyle();
   
   // Create the canvas
   TCanvas *c_histo = new TCanvas("c_histo","canvas histo",600,600);
   c_histo->cd();
   
   //Create the histogram
   TH1F *histo = new TH1F("histo","",100,-4.,4.);
   
   // Event generation (Double Gaussian)
   Int_t i=0;
   for(i=0;i<7000;++i){
     xvar[i]= randGen.Gaus(mA,sA);
     histo->Fill(xvar[i]);
  };
   
   for(i=7000;i<10000;++i){
     xvar[i]= randGen.Gaus(mB,sB);
     histo->Fill(xvar[i]);
   };
   
   // Draw the histogram
   histo->GetXaxis()->SetTitle("R [cm] "); // Let us assume a lenght on x axis
   histo->GetYaxis()->SetTitle("Events per 0.08 cm "); 
   histo->Draw();

   TFitter *fitter = new TFitter(PARAMETERS); //initialize TFitter with a maximum of 5 params
   fitter->SetDefaultFitter("Minuit"); // set minimizer. Default is Minuit.
   fitter->SetFCN(fcn); // set FCN

   // Set starting values and step sizes for parameters  
   Double_t vstart[PARAMETERS] = {0.3,  -1.  , 1.  , 0.5 , 0.5 };
   Double_t step[PARAMETERS]   = {0.0001, 0.0001, 0.0001, 0.0001, 0.0001};   //step 0 li rende costanti

   //Int_t SetParameter(Int_t ipar, const char* parname, Double_t value, Double_t verr, Double_t vlow, Double_t vhigh)
   fitter->SetParameter(0, "fA", vstart[0] , step[0], 0.,1.); // Bounded
   fitter->SetParameter(1, "mA", vstart[1] , step[1], 0.,0.);
   fitter->SetParameter(2, "mB", vstart[2] , step[2], 0.,0.);
   fitter->SetParameter(3, "sA", vstart[3] , step[3], 0.,0.);
   fitter->SetParameter(4, "sB", vstart[4] , step[4], 0.,0.);
   
   // Now ready for minimization step
   // Start minimization with arglist[0] max iterations and arglist[1] tolerance:
   // the default tolerance is 0.1, and the minimization will stop when
   // the estimated vertical distance to the minimum (EDM) is less than 
   // 0.001*[tolerance]*ERRDEF
   //Int_t ExecuteCommand(const char* command, Double_t* args, Int_t nargs)
   Double_t arglist[2] = {0.,0.1}; //
   fitter->ExecuteCommand("MIGRAD", arglist, 2);

   //arglist[0] = 0.;  
   //fitter->ExecuteCommand("HESSE", arglist, 1);
   //fitter->ExecuteCommand("MINOS", arglist, 1);

   //qui il metodo: https://root.cern.ch/root/html/TFitter.html#TFitter:GetStats
   Double_t fcnmin, edm, errdef;
   Int_t nvpar, nparx;
   fitter->GetStats(fcnmin, edm, errdef, nvpar, nparx);
   std::cout << "######### FIT INFO ################ " << std::endl;
   std::cout << "FCN(min) = " << fcnmin << endl; 
   std::cout << "EDM      = " << edm << endl; 
   std::cout << "ERRDEF   = " << errdef << endl;
   std::cout << "nvpar    = " << nvpar << endl;
   std::cout << "nparx    = " << nparx << endl;
   std::cout << "######### PARAMETERS ################ " << std::endl;
   std::cout << "fA = " << fitter->GetParameter(0) << "+/-" << fitter->GetParError(0) << std::endl;
   std::cout << "mA = " << fitter->GetParameter(1) << "+/-" << fitter->GetParError(1) << std::endl;
   std::cout << "sA = " << fitter->GetParameter(2) << "+/-" << fitter->GetParError(2) << std::endl;
   std::cout << "mB = " << fitter->GetParameter(3) << "+/-" << fitter->GetParError(3) << std::endl;
   std::cout << "sB = " << fitter->GetParameter(4) << "+/-" << fitter->GetParError(4) << std::endl;
   

   //Double_t GetCovarianceMatrixElement(Int_t i, Int_t j) const
   //std::cout << " prova del primo elemento della matrice = "<<fitter->GetCovarianceMatrixElement(0,0) << std::endl;

   std::cout << "######### COVARIANCE MATRIX ################ " << std::endl;
   // Write on .txt file covariance Matrix 
   FILE *file_cov = fopen ("_txt/covariance_matrix2.txt", "w");
   Int_t icol=0;
   Int_t irow=0;
   if (nvpar==1)
     {
       fprintf (file_cov, "%e \n", fitter->GetCovarianceMatrixElement(0,0)); 
       printf ("%e \n", fitter->GetCovarianceMatrixElement(0,0)); 
     }
   else
     {
       for (irow=0; irow<nvpar; irow++)
	 {
	   for (icol=0; icol<nvpar; icol++)
	     { 
               fprintf (file_cov, "%e ", fitter->GetCovarianceMatrixElement(irow,icol));
	       printf ("%e ", fitter->GetCovarianceMatrixElement(irow,icol));
	     };
	   fprintf (file_cov, "\n");
	   printf ("\n");
	 };
       //fprintf (file_cov, "\n");  
       //printf ("\n");  
     };
   fclose (file_cov);
   std::cout << "############################################ " << std::endl;
   
 //Projection
   TF1 *f_proj = new TF1("f_proj",pdf_proj,-4.,4.,5);
   
   // Sets values 
   for(Int_t k=0; k<PARAMETERS; k++){
     f_proj->SetParameter(k,fitter->GetParameter(k));
   };
   
   f_proj->Draw("SAME");

   c_histo->Print("./_fig/histo2.pdf");
   c_histo->Print("./_fig/histo2.eps");

}


