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
//#####
#include "TCanvas.h"
#include "TGraph.h"
#include "TArc.h"
#include "TVirtualFitter.h"
#include "TFitter.h"
// Style 
#include "labstyle.C"

#define MAX 49         // 47+2
#define MAX_nPIXELS 47 // 47+2

 
using namespace std;

TGraph *gr;
TGraph *gr_total;

//____________________________________________________________________
void myfcn(Int_t &, Double_t *, Double_t &f, Double_t *par, Int_t) {
   //minimisation function computing the sum of squares of residuals
   Int_t np = gr->GetN();
   f = 0;
   Double_t *x = gr->GetX();
   Double_t *y = gr->GetY();
   for (Int_t i=0;i<np;i++) {
      Double_t u = x[i] - par[0];
      Double_t v = y[i] - par[1];
      Double_t dr = par[2] - TMath::Sqrt(u*u+v*v);
      f += dr*dr;
   }
}

//____________________________________________________________________



void macroCerenkov1() {
  
  //Setting style
  labstyle();
  
  Int_t vec_data[MAX]; 
  Int_t n_track            = {0.};
  Int_t n_pixel            = {0.};
  Int_t x[MAX_nPIXELS]     = {0.};
  Int_t y[MAX_nPIXELS]     = {0.};
  
  // //Create the histogram
  TH1F *histo_R       = new TH1F("histo_R","",100,25,26.);
  TH1F *histo_x0      = new TH1F("histo_x0","",100,24,25.);
  TH1F *histo_y0      = new TH1F("histo_y0","",100,24.,25.);
  TH1F *histo_FCN     = new TH1F("histo_FCN","",100,0.,10.);
  TH1F *histo_n_pixel = new TH1F("histo_n_pixel","",100,0,50.);
  
  // Read the file
  std::ifstream myfile;
  myfile.open("_txt/mydata.txt");
  // myfile.open("_txt/mydata_short.txt");
  
  gr_total = new TGraph(); 
  Int_t cont; // contatore per per il TGraph totale con tutti i cerchi
  cont=0;
  
  // Loop sugli eventi
  Int_t j;
  j=0;
  while ( true ) {
    for(Int_t k=0 ; k<MAX; k++){myfile >> vec_data[k];};
    //for(Int_t k=0 ; k<MAX; k++){std::cout << "event_k " << k << " :   " << vec_data[k] <<  std::endl;  };    
    
    //####### my code #############
    n_track        = vec_data[0];
    n_pixel        = vec_data[1];
    
    if(1){
      //if (n_pixel>0) { // if sul numero di pixel
      //if (n_track==1) { // if sul numero di tracce nello scintillatore
      //if (n_track==1 && n_pixel==26) { // if sul numero di tracce nello scintillatore
      
      histo_n_pixel  ->Fill(n_pixel);     
      
      for(Int_t k=2 ; k<MAX; k++)
	{ 
	  x[k-2] = vec_data[k]/1000;
	  y[k-2] = vec_data[k] % 1000;
	};
      
      //for(Int_t k=0 ; k<MAX_nPIXELS; k++){std::cout << "event_j " << k << " :   " << x[k] << " " << y[k] <<std::endl;  };
      //std::cout<< n_track << std::endl;
      
      //plot totale (ma non e' che abbia molto senso)
      for(Int_t k=0 ; k<n_pixel; k++)
	{
	  //std::cout << k << " - " << n_pixel << " "<<x[k] << std::endl;  
	  gr_total->SetPoint(cont,x[k],y[k]);
	  cont=cont+1;
	};
      
      //Fit del Raggio ###################
      gr = new TGraph(n_pixel);
      for(Int_t k=0 ; k<n_pixel; k++)
	{	 
	  gr->SetPoint(k,x[k],y[k]);
	};
      
      TFitter *fitter = new TFitter(3);
      fitter->SetDefaultFitter("Minuit");
      fitter->SetFCN(myfcn);

      //Int_t SetParameter(Int_t ipar, const char* parname, Double_t value, Double_t verr, Double_t vlow, Double_t vhigh)
      fitter->SetParameter(0, "x0",   25., 0.1, 0,0);
      fitter->SetParameter(1, "y0",   25., 0.1, 0,0);
      fitter->SetParameter(2, "R",    25., 0.1, 0,0);
      
      //Int_t ExecuteCommand(const char* command, Double_t* args, Int_t nargs)
      Double_t arglist[1] = {0};
      fitter->ExecuteCommand("MIGRAD", arglist, 0);
      
      histo_x0->Fill(fitter->GetParameter(0));
      histo_y0->Fill(fitter->GetParameter(1));
      histo_R->Fill(fitter->GetParameter(2));
      
      Double_t fcnmin, edm, errdef;
      Int_t nvpar, nparx;
      fitter->GetStats(fcnmin, edm, errdef, nvpar, nparx);
      histo_FCN->Fill(fcnmin);
      //qui il metodo: https://root.cern.ch/root/html/TFitter.html#TFitter:GetStats
      
      
      gr->Delete();
      
    }; //end sul numero di pixels
    
    //####### end my code ########
    
    
    if( myfile.eof() ) break;
    ++j;
  }; // end Loop sugli eventi
  
  myfile.close();
  
  // Print on the screen the maximum number of events
  Int_t max_events;
  max_events = j;
  std::cout << "Max Number of Event = " <<j << std::endl;
  
  // All circles
  TCanvas *c_total = new TCanvas("c_total","All events",600,600);
  c_total->cd();
  gr_total->GetXaxis()->SetTitle("pixel x ");
  gr_total->GetYaxis()->SetTitle("pixel y"); 
  gr_total->Draw("AP");
  //gr->Draw("AP");
  
  // Radius
  TCanvas *c_histo_R = new TCanvas("c_histo_R","canvas Raggio",600,600);
  c_histo_R->cd();
  histo_R->GetXaxis()->SetTitle("Radius [cm] ");
  histo_R->GetYaxis()->SetTitle("Events per 0.01 cm "); 
  histo_R->Draw();
  
  // x0
  TCanvas *c_histo_x0 = new TCanvas("c_histo_x0","canvas x0",600,600);
  c_histo_x0->cd();
  histo_x0->GetXaxis()->SetTitle("x0 [cm] ");
  histo_x0->GetYaxis()->SetTitle("Events per 0.01 cm "); 
  histo_x0->Draw();
  
  // y0 
  TCanvas *c_histo_y0 = new TCanvas("c_histo_y0","canvas y0",600,600);
  c_histo_y0->cd();
  histo_y0->GetXaxis()->SetTitle("y0 [cm] ");
  histo_y0->GetYaxis()->SetTitle("Events per 0.01 cm "); 
  histo_y0->Draw();
  
  // FCN
  TCanvas *c_histo_FCN = new TCanvas("c_histo_FCN","canvas FCN",600,600);
  c_histo_FCN->cd();
  histo_FCN->GetXaxis()->SetTitle("FCN [cm^{2}] ");
  histo_FCN->GetYaxis()->SetTitle("Events per 0.1 cm^{2} ");
  histo_FCN->Draw();
  
  // n_pixels
  TCanvas *c_histo_n_pixel = new TCanvas("c_histo_n_pixel","canvas n_pixel",600,600);
  c_histo_n_pixel->cd();
  histo_n_pixel->GetXaxis()->SetTitle("n pixel");
  histo_n_pixel->GetYaxis()->SetTitle("Events per 0.5"); 
  histo_n_pixel->Draw();
  
  // Write on .root file the histogram
  TFile *hfile = new TFile("./_root/histo_cerenkov.root","RECREATE","Demo ROOT file with histograms");
  hfile->cd();
  histo_R->Write();
  histo_x0->Write();
  histo_y0->Write();
  histo_FCN->Write();
  histo_n_pixel->Write();
  hfile->Close();
  
}


