
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TF1.h"
#include "TH1.h"
#include "THStack.h"
#include "TPaveText.h"
#include "TFile.h"
#include "TRandom.h"
#include "TMath.h"
#include <iostream>
#include "labstyle.C"

void mvb(Double_t sep){

  
  //gStyle->SetOptStat(0000000);
  //gStyle->SetOptFit(1111); //opt stat


  //Setting style
   labstyle();

  Double_t xmin,xmax;
  Int_t bin;

  xmin = -6.;
  xmax =  10.;
  bin  =  300;

  TH1F  *h1     = new TH1F("h1","",bin,xmin,xmax);
  TH1F  *h2     = new TH1F("h2","",bin,xmin,xmax);
  TH1F  *h_diff = new TH1F("h_diff","",bin,xmin,xmax);
  TH1F  *h_diff2= new TH1F("h_diff2","",bin,xmin,xmax);
  
  TH1F  *h_sum    = new TH1F("h_sum","",bin,xmin,xmax);
  TH1F  *h_ratio  = new TH1F("h_ratio","",bin,xmin,xmax);


  h1     ->Sumw2();
  h2     ->Sumw2();
  h_diff ->Sumw2();
  h_diff2->Sumw2();
  h_sum  ->Sumw2();

  

  Double_t sigma;
  //Double_t sep;
  Double_t f;
  sigma = 1.;
  
  //s_pos = 0.598719
  //s_neg = 0.590821

  //sep = 0.2   // s = 0.099927
  //sep = 0.3   // s = 0.148308
  //sep = 0.5   // s = 0.242812
  //sep = 1     // s = 0.452087
  //sep = 1.1   // s = 0.488145
  //sep = 1.2   // s = 0.522638
  //sep = 1.3   // s = 0.556074
  //sep = 1.4   // s = 0.587137
  //sep = 1.5;  // s = 0.616995
  //sep = 1.6;  // s = 0.644731
  //sep = 1.7   // s = 0.671525
  //sep = 1.8   // s = 0.696229
  //sep = 1.9   // s = 0.719831
  //sep = 2.;   // s = 0.741836
  //sep = 3.;   // s = 0.895955
  //sep = 4. ;  // s = 0.965077
  //sep = 5. ;  // s = 0.990464
  //sep = 6. ;  // s = 0.99792
  


  //sep = 1.44;  // s = 0.599341
  //sep = 1.43;  // s = 0.596353
  //sep = 1.42;  // s = 0.593343
  //sep = 1.41;  // s = 0.590331


  f=0.5;

  double n1;
  double n2;
  double n3;
  for (Int_t j=0; j<100000000;j++) {
  
  n1=gRandom->Gaus(0,sigma);
  n2=gRandom->Gaus(sep*sigma,sigma);
  h1->Fill(n1);
  h2->Fill(n2);
  };

  
  Double_t Norm;
  Norm = h1->Integral(1,bin);
  std::cout<< "Norm = " << Norm << std::endl;
  h1->Scale(1./Norm);
  Norm = h2->Integral(1,bin);
  h2->Scale(1./Norm);

  
  h_diff  ->Add(h1,h2,1.,-1.);
  h_diff2 ->Multiply(h_diff,h_diff,1.,1.);
  h_sum   ->Add(h1,h2,f,1-f);
  h_ratio  ->Divide(h_diff2,h_sum,1.,1.);

 #if 1 
  TCanvas *c_gaussiane = new TCanvas("c_gaussiane","plot gaussiane",600,600);
  c_gaussiane->cd();
 
  h1->Draw("HIST");
  h2->SetLineColor(2);
  h2->Draw("SAME HIST");
  //h_diff2->SetLineColor(4);
  //h_ratio->Draw("HIST");
#endif

  Double_t s;
  s=h_ratio->Integral(1,bin);
  s= sqrt(f*(1-f)*s);
  std::cout << "s = " << s << std::endl;

  //c_gaussiane->Update();     
  //c_gaussiane->Print("./eps/gaussiane.gif");
  //c_gaussiane->Print("./eps/gaussiane.eps");



}


