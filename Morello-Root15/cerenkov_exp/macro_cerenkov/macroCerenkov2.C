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
#include "TTree.h"

#include "labstyle.C"

#define MAX 49 // 47+2
#define MAX_nPIXELS 47 // 47+2
 
using namespace std;

// Tree variables
const Int_t MAXEVENTS = 50000;
typedef struct {
  Int_t    n_track;
  Int_t    n_pixel;
  Int_t    x_pixel[MAX_nPIXELS];
  Int_t    y_pixel[MAX_nPIXELS];
  Double_t R;
  Double_t x0;
  Double_t y0;
  Double_t FCN;
} Circle_t;

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




void macroCerenkov2() {

  //Setting style
   labstyle();
    
   Int_t vec_data[MAX]; 
   Int_t n_track    = {0.};
   Int_t n_pixel    = {0.};
   Int_t x[MAX_nPIXELS]     = {0.};
   Int_t y[MAX_nPIXELS]     = {0.};

   
   // //Create the histogram
   TH1F *histo_R         = new TH1F("histo_R","",100,25,26.);
   TH1F *histo_x0        = new TH1F("histo_x0","",100,24,25.);
   TH1F *histo_y0        = new TH1F("histo_y0","",100,24.,25.);
   TH1F *histo_FCN       = new TH1F("histo_FCN","",100,0.,10.);
   TH1F *histo_n_pixel   = new TH1F("histo_n_pixel","",100,0,50.);
   
   
   //create the file, the Tree and a few branches with 
   //a subset of Circle
   TFile tree_file("./_root/tree_circle.root","recreate");
   TTree t2("t2","a Tree with data");
   Circle_t ring;
   t2.Branch("n_track",&ring.n_track,"n_track/I");
   t2.Branch("n_pixel",&ring.n_pixel,"n_pixel/I");
   t2.Branch("x_pixel",ring.x_pixel,"x_pixel[n_pixel]/I");
   t2.Branch("y_pixel",ring.y_pixel,"y_pixel[n_pixel]/I");
   t2.Branch("R",&ring.R,"R/D");
   t2.Branch("x0",&ring.x0,"x0/D");
   t2.Branch("y0",&ring.y0,"y0/D");
   t2.Branch("FCN",&ring.FCN,"FCN/D");


   // Read the file
   std::ifstream myfile;
   myfile.open("_txt/mydata.txt");
   // myfile.open("_txt/mydata_short.txt");
   
   
   gr_total = new TGraph(); 
   Int_t cont; // per il graph integrale
   cont=0;
   
   // Loop sugli eventi
   Int_t j;
   j=0;
   while ( true ) {
     for(Int_t k=0 ; k<MAX; k++){myfile >> vec_data[k];};
     
     //####### my code #############
     n_track        = vec_data[0];
     n_pixel        = vec_data[1];

     // per il tree
     ring.n_track = n_track;
     ring.n_pixel = n_pixel;
     
     histo_n_pixel  ->Fill(n_pixel);     

     for(Int_t k=2 ; k<MAX; k++)
       { 
	 x[k-2] = vec_data[k]/1000;
	 y[k-2] = vec_data[k] % 1000;
	 ring.x_pixel[k-2] = vec_data[k]/1000;
	 ring.y_pixel[k-2] = vec_data[k] % 1000;
       };
         
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
     
     fitter->SetParameter(0, "x0",   25., 0.1, 0,0);
     fitter->SetParameter(1, "y0",   25., 0.1, 0,0);
     fitter->SetParameter(2, "R",    25., 0.1, 0,0);
     
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
     
     ring.x0 = fitter->GetParameter(0);
     ring.y0 = fitter->GetParameter(1);
     ring.R = fitter->GetParameter(2);
     ring.FCN = fcnmin;
    
     // fill the Tree with current step parameters
     t2.Fill();

     gr->Delete();
    

     //####### end my code ########


     if( myfile.eof() ) break;
     ++j;
   }; // end Loop sugli eventi
   

   t2.Write();
   tree_file.Close();
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


