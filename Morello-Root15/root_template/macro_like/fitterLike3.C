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
#define PARAMETERS 9 //<-- Numero massimo di parametri
Double_t xvar[MAX]; 
Double_t yvar[MAX];

#define MAX_PROJ 10000000


TRandom3 randGen(0);


Double_t fA  =  0.5;
Double_t mAx = -0.5;//-1. ;
Double_t mAy = -0.5;//-1. ;
Double_t mBx =  0.5;//1. ;
Double_t mBy =  0.5;//1. ;
Double_t sAx =  0.5;//0.5
Double_t sAy =  0.5;
Double_t sBx =  0.5;
Double_t sBy =  0.5;


// Double_t fA  =  0.5;
// Double_t mAx =  0.0;//-1. ;
// Double_t mAy =  0.0;//-1. ;
// Double_t mBx =  1.0;//1. ;
// Double_t mBy =  1.0;//1. ;
// Double_t sAx =  1.0;//0.5
// Double_t sAy =  1.0;
// Double_t sBx =  1.0;
// Double_t sBy =  1.0;

// histograms 
Double_t x_min  =  -4.;
Double_t x_max  =   4.;
Double_t y_min  =  -4.;
Double_t y_max  =   4.;
Int_t bin_number_x = 100;
Int_t bin_number_y = 100;
Int_t s_proj       = 1; //fattore di scale per la proiezione



Double_t pdf1D(Double_t *x, Double_t *par)
{

 Double_t _fA ;
 Double_t _mAx;
 Double_t _mBx;
 Double_t _sAx;
 Double_t _sBx;

 _fA  = par[0];
 _mAx = par[1];
 _mBx = par[2];
 _sAx = par[3];
 _sBx = par[4];

 Double_t value =      _fA*(TMath::Gaus(x[0],_mAx,_sAx,1)) 
                 + (1-_fA)*(TMath::Gaus(x[0],_mBx,_sBx,1));
 return value;
}

Double_t pdf1D_like(Double_t x, Double_t *par)
{
  Double_t data[1];
  data[0] = x;
  return   pdf1D(data, par);
}

Double_t pdf2D(Double_t *x, Double_t *par)
{

 Double_t _fA ;
 Double_t _mAx;
 Double_t _mAy;
 Double_t _mBx;
 Double_t _mBy;
 Double_t _sAx;
 Double_t _sAy;
 Double_t _sBx;
 Double_t _sBy;

 _fA  = par[0];
 _mAx = par[1];
 _mAy = par[2];
 _mBx = par[3];
 _mBy = par[4];
 _sAx = par[5];
 _sAy = par[6];
 _sBx = par[7];
 _sBy = par[8];


 Double_t value =      _fA*(TMath::Gaus(x[0],_mAx,_sAx,1) * TMath::Gaus(x[1],_mAy,_sAy,1)) 
                 + (1-_fA)*(TMath::Gaus(x[0],_mBx,_sBx,1)  *TMath::Gaus(x[1],_mBy,_sBy,1));
 return value;
}


Double_t pdf2D_like(Double_t x, Double_t y, Double_t *par)
{
  Double_t data[2];
  data[0] = x;
  data[1] = y;
  return   pdf2D(data, par);
}







//#####################################

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
    Like = Like + TMath::Log(pdf2D_like(xvar[i],yvar[i],par));
  };
   f= - 2. * Like;
}


void fcn1D(Int_t &, Double_t *, Double_t &f, Double_t *par, Int_t )
{

  Double_t Like=0;
  Int_t i=0;
  for(i=0;i<MAX;++i){
    Like = Like + TMath::Log(pdf1D_like(xvar[i],par));
  };
   f= - 2. * Like;
}





void fitterLike3() {

  // Setting per lo stile del canvas e del plot
  //gROOT->SetStyle("Plain");
  //gStyle->SetOptStat(0000000);
  //gStyle->SetOptFit(1111); //opt stat
  
  //Setting style
   labstyle();

   TCanvas *c_fun_tot = new TCanvas("c_fun_tot","canvas function",600,600);
   c_fun_tot->cd();
   //Create function
   TF2 *f_fun_tot = new TF2("f_fun_tot",pdf2D,x_min,x_max,y_min,y_max,9);
   f_fun_tot->SetParameter(0, fA) ;
   f_fun_tot->SetParameter(1,mAx) ;
   f_fun_tot->SetParameter(2,mAy) ;
   f_fun_tot->SetParameter(3,mBx) ;
   f_fun_tot->SetParameter(4,mBy) ;
   f_fun_tot->SetParameter(5,sAx) ;
   f_fun_tot->SetParameter(6,sAy) ;
   f_fun_tot->SetParameter(7,sBx) ;
   f_fun_tot->SetParameter(8,sBy) ;
   f_fun_tot->Draw("surf");
   std::cout << "Integral(fun_tot) = " << f_fun_tot->Integral(x_min,x_max,y_min,y_max) << std::endl; 

   TCanvas *c_fun_A = new TCanvas("c_fun_A","canvas function",600,600);
   c_fun_A->cd();
   TF2 *f_fun_A = new TF2("f_fun_A",pdf2D,x_min,x_max,y_min,y_max,9);
   f_fun_A->SetParameter(0, 1.) ;
   f_fun_A->SetParameter(1,mAx) ;
   f_fun_A->SetParameter(2,mAy) ;
   f_fun_A->SetParameter(3,mBx) ;
   f_fun_A->SetParameter(4,mBy) ;
   f_fun_A->SetParameter(5,sAx) ;
   f_fun_A->SetParameter(6,sAy) ;
   f_fun_A->SetParameter(7,sBx) ;
   f_fun_A->SetParameter(8,sBy) ;
   f_fun_A->Draw("surf");
   std::cout << "Integral(fun_A) = " << f_fun_A->Integral(x_min,x_max,y_min,y_max) << std::endl;

   TCanvas *c_fun_B = new TCanvas("c_fun_B","canvas function",600,600);
   c_fun_B->cd();
   TF2 *f_fun_B = new TF2("f_fun_B",pdf2D,x_min,x_max,y_min,y_max,9);
   f_fun_B->SetParameter(0, 0.) ;
   f_fun_B->SetParameter(1,mAx) ;
   f_fun_B->SetParameter(2,mAy) ;
   f_fun_B->SetParameter(3,mBx) ;
   f_fun_B->SetParameter(4,mBy) ;
   f_fun_B->SetParameter(5,sAx) ;
   f_fun_B->SetParameter(6,sAy) ;
   f_fun_B->SetParameter(7,sBx) ;
   f_fun_B->SetParameter(8,sBy) ;
   f_fun_B->Draw("surf");
   std::cout << "Integral(fun_B) = " << f_fun_B->Integral(x_min,x_max,y_min,y_max) << std::endl;
   
   //Double_t bin_width_x = (x_max-x_min)/bin_number_x;
   //Double_t bin_width_y = (y_max-y_min)/bin_number_y;
   
   //Create the histogramz
   TH2F *histo_tot = new TH2F("histo_tot","",bin_number_x,x_min,x_max,bin_number_y,y_min,y_max);
   TH2F *histo_A   = new TH2F("histo_A"  ,"",bin_number_x,x_min,x_max,bin_number_y,y_min,y_max);
   TH2F *histo_B   = new TH2F("histo_B"  ,"",bin_number_x,x_min,x_max,bin_number_y,y_min,y_max);
 
   TH1F *histo_tot_x = new TH1F("histo_tot_x","",bin_number_x,x_min,x_max);
   TH1F *histo_tot_y = new TH1F("histo_tot_y","",bin_number_y,y_min,y_max);
   TH1F *histo_A_x = new TH1F("histo_A_x"    ,"",bin_number_x,x_min,x_max);
   TH1F *histo_A_y = new TH1F("histo_A_y"    ,"",bin_number_y,y_min,y_max);
   TH1F *histo_B_x = new TH1F("histo_B_x"    ,"",bin_number_x,x_min,x_max);
   TH1F *histo_B_y = new TH1F("histo_B_y"    ,"",bin_number_y,y_min,y_max);

   //proj
   TH2F *histo_proj_tot = new TH2F("histo_proj_tot","",bin_number_x,x_min,x_max,bin_number_y,y_min,y_max);
   TH2F *histo_proj_A   = new TH2F("histo_proj_A"  ,"",bin_number_x,x_min,x_max,bin_number_y,y_min,y_max);
   TH2F *histo_proj_B   = new TH2F("histo_proj_B"  ,"",bin_number_x,x_min,x_max,bin_number_y,y_min,y_max);

   TH1F *histo_proj_tot_x = new TH1F("histo_proj_tot_x","",s_proj*bin_number_x,x_min,x_max);
   TH1F *histo_proj_tot_y = new TH1F("histo_proj_tot_y","",s_proj*bin_number_y,y_min,y_max);
   TH1F *histo_proj_A_x = new TH1F("histo_proj_A_x"    ,"",s_proj*bin_number_x,x_min,x_max);
   TH1F *histo_proj_A_y = new TH1F("histo_proj_A_y"    ,"",s_proj*bin_number_y,y_min,y_max);
   TH1F *histo_proj_B_x = new TH1F("histo_proj_B_x"    ,"",s_proj*bin_number_x,x_min,x_max);
   TH1F *histo_proj_B_y = new TH1F("histo_proj_B_y"    ,"",s_proj*bin_number_y,y_min,y_max);
   

   // ################################################
   // ###### GENERATION ##############################
   // ################################################

   // Event generation (Double Gaussian)
   Int_t i=0;
   for(i=0;i<(Int_t) (MAX*fA);i++){
     xvar[i] = randGen.Gaus(mAx,sAx);
     yvar[i] = randGen.Gaus(mAy,sAy);
     histo_tot->Fill(xvar[i],yvar[i]);
     histo_tot_x->Fill(xvar[i]);
     histo_tot_y->Fill(yvar[i]);
     histo_A->Fill(xvar[i],yvar[i]);
     histo_A_x->Fill(xvar[i]);
     histo_A_y->Fill(yvar[i]);
     //std::cout << "i = " << i << " "<< xvar[i] << " "<< yvar[i] << std::endl; 
  };
   
   for(i=(Int_t) (MAX*fA);i<MAX;i++){
     xvar[i]= randGen.Gaus(mBx,sBx);
     yvar[i]= randGen.Gaus(mBy,sBy);
     histo_tot->Fill(xvar[i],yvar[i]);
     histo_tot_x->Fill(xvar[i]);
     histo_tot_y->Fill(yvar[i]);
     histo_B->Fill(xvar[i],yvar[i]);
     histo_B_x->Fill(xvar[i]);
     histo_B_y->Fill(yvar[i]);
     //std::cout << "i = " << i << " "<< xvar[i] << " "<< yvar[i] << std::endl; 
   };
   
   // Draw the histogram
   //histo->GetXaxis()->SetTitle("R [cm] "); // Let us assume a lenght on x axis
   //histo->GetYaxis()->SetTitle("Events per 0.08 cm "); 
   histo_tot->Draw("contZ");
   std::cout<< "Entries histo_tot " <<histo_tot->GetEntries() << std::endl;
   std::cout<< "Entries histo_A " <<histo_A->GetEntries() << std::endl;
   std::cout<< "Entries histo_B " <<histo_B->GetEntries() << std::endl;
   
   // ################################################
   // ###### FIT 2D ###################################
   // ################################################
   
   TFitter *fitter = new TFitter(PARAMETERS); //initialize TFitter with a maximum of 5 params
   fitter->SetDefaultFitter("Minuit"); // set minimizer. Default is Minuit.
   fitter->SetFCN(fcn); // set FCN

   // Set starting values and step sizes for parameters  
   //Double_t vstart[PARAMETERS] = {0.3,  -1.  , 1.  , 0.5 , 0.5 };
   //Double_t step[PARAMETERS]   = {0.0001, 0.0001, 0.0001, 0.0001, 0.0001};   //step 0 li rende costanti

   //Int_t SetParameter(Int_t ipar, const char* parname, Double_t value, Double_t verr, Double_t vlow, Double_t vhigh)
   fitter->SetParameter(0, "fA ", fA  , 0.0001, 0.,1.); // Bounded
   fitter->SetParameter(1, "mAx", mAx , 0.0001, 0.,0.);
   fitter->SetParameter(2, "mAy", mAy , 0.0001, 0.,0.);
   fitter->SetParameter(3, "mBx", mBx , 0.0001, 0.,0.);
   fitter->SetParameter(4, "mBy", mBy , 0.0001, 0.,0.);
   fitter->SetParameter(5, "sAx", sAx , 0.0001, 0.,0.);
   fitter->SetParameter(6, "sAy", sAy , 0.0001, 0.,0.);
   fitter->SetParameter(7, "sBx", sBx , 0.0001, 0.,0.);
   fitter->SetParameter(8, "sBy", sBy , 0.0001, 0.,0.);
   
   // Now ready for minimization step
   // Start minimization with arglist[0] max iterations and arglist[1] tolerance:
   // the default tolerance is 0.1, and the minimization will stop when
   // the estimated vertical distance to the minimum (EDM) is less than 
   // 0.001*[tolerance]*ERRDEF
   //Int_t ExecuteCommand(const char* command, Double_t* args, Int_t nargs)
   Double_t arglist[2] = {0.,0.1}; //
   fitter->ExecuteCommand("MIGRAD", arglist, 2);

   Double_t fcnmin, edm, errdef;
   Int_t nvpar, nparx;
   fitter->GetStats(fcnmin, edm, errdef, nvpar, nparx);
   
   Double_t vpar[PARAMETERS];
   for(Int_t k=0; k<PARAMETERS; ++k){
     vpar[k] = fitter->GetParameter(k);
     //std::cout << vpar[k] << std::endl;
   };


   // ################################################
   // ###### PROJECTIONS #############################
   // ################################################

 
   //TH2F *histo_proj_tot = new TH2F("histo_proj_tot","",bin_number_x,x_min,x_max,bin_number_y,y_min,y_max);
   //TH2F *histo_proj_A   = new TH2F("histo_proj_A"  ,"",bin_number_x,x_min,x_max,bin_number_y,y_min,y_max);
   //TH2F *histo_proj_B   = new TH2F("histo_proj_B"  ,"",bin_number_x,x_min,x_max,bin_number_y,y_min,y_max);

   //TH1F *histo_proj_tot_x = new TH1F("histo_proj_tot_x","",s_proj*bin_number_x,x_min,x_max);
   //TH1F *histo_proj_tot_y = new TH1F("histo_proj_tot_y","",s_proj*bin_number_y,y_min,y_max);
   //TH1F *histo_proj_A_x = new TH1F("histo_proj_A_x"    ,"",s_proj*bin_number_x,x_min,x_max);
   //TH1F *histo_proj_A_y = new TH1F("histo_proj_A_y"    ,"",s_proj*bin_number_y,y_min,y_max);
   //TH1F *histo_proj_B_x = new TH1F("histo_proj_B_x"    ,"",s_proj*bin_number_x,x_min,x_max);
   //TH1F *histo_proj_B_y = new TH1F("histo_proj_B_y"    ,"",s_proj*bin_number_y,y_min,y_max);


   Double_t x,y,weight,weight_A,weight_B;
    for(Int_t j=0;j<MAX_PROJ;j++){
      x = randGen.Uniform(x_max-x_min) + x_min;
      y = randGen.Uniform(y_max-y_min) + y_min;
      weight = f_fun_tot->Eval(x,y);
      weight_A = f_fun_A->Eval(x,y);
      weight_B = f_fun_B->Eval(x,y);

      histo_proj_tot   ->Fill(x,y,weight);
      histo_proj_A     ->Fill(x,y,weight_A);
      histo_proj_B     ->Fill(x,y,weight_B);
      histo_proj_tot_x ->Fill(x,weight);
      histo_proj_tot_y ->Fill(y,weight);
      histo_proj_A_x   ->Fill(x,weight_A);
      histo_proj_A_y   ->Fill(y,weight_A);
      histo_proj_B_x   ->Fill(x,weight_B);
      histo_proj_B_y   ->Fill(y,weight_B);
      
      //histo_A->Fill(xvar[i],yvar[i]);
      //std::cout << "i = " << i << " "<< xvar[i] << " "<< yvar[i] << std::endl; 
    };
    


    TCanvas *c_proj_tot_x = new TCanvas("c_proj_tot_x","Projection x",600,600);
    c_proj_tot_x->cd();
    histo_tot_x->GetXaxis()->SetTitle("x [cm] "); // Let us assume a lenght on x axis
    histo_tot_x->GetYaxis()->SetTitle("Events per 0.08 cm "); 
    histo_tot_x->Draw("e"); //data x
    histo_proj_tot_x->SetLineColor(2);
    histo_proj_tot_x->Scale(MAX/histo_proj_tot_x->Integral(1,bin_number_x));
    histo_proj_A_x->Scale((MAX*fitter->GetParameter(0))/histo_proj_A_x->Integral(1,bin_number_x));
    histo_proj_B_x->Scale((MAX*(1.-fitter->GetParameter(0)))/histo_proj_B_x->Integral(1,bin_number_x));
    //std::cout << "Integrale proj = " << histo_proj_tot_x->Integral(1,bin_number_x) << std::endl;
    //std::cout << "Integrale data = " << histo_tot_x->Integral(1,bin_number_x) << std::endl;
    histo_proj_A_x->SetLineColor(4);
    histo_proj_B_x->SetLineColor(4);
    histo_proj_A_x->Draw("C same");//projection A x
    histo_proj_B_x->Draw("C same");//projection B x
    histo_proj_tot_x->Draw("C same");//projection x

    TCanvas *c_proj_tot_y = new TCanvas("c_proj_tot_y","Projection y",600,600);
    c_proj_tot_y->cd();
    histo_tot_y->GetXaxis()->SetTitle("y [cm] "); // Let us assume a lenght on y axis
    histo_tot_y->GetYaxis()->SetTitle("Events per 0.08 cm "); 
    histo_tot_y->Draw("e"); //data y
    histo_proj_tot_y->SetLineColor(2);
    histo_proj_tot_y->Scale(MAX/histo_proj_tot_y->Integral(1,bin_number_y));
    histo_proj_A_y->Scale((MAX*fitter->GetParameter(0))/histo_proj_A_y->Integral(1,bin_number_y));
    histo_proj_B_y->Scale((MAX*(1.-fitter->GetParameter(0)))/histo_proj_B_y->Integral(1,bin_number_y));
    //std::cout << "Integrale proj = " << histo_proj_tot_y->Integral(1,bin_number_y) << std::endl;
    //std::cout << "Integrale data = " << histo_tot_y->Integral(1,bin_number_y) << std::endl;
    histo_proj_A_y->SetLineColor(4);
    histo_proj_B_y->SetLineColor(4);
    histo_proj_A_y->Draw("same");//projection A y
    histo_proj_B_y->Draw("same");//projection B y
    histo_proj_tot_y->Draw("same");//projection y


    // ################################################
    // ###### FIT 1D ###################################
    // ################################################

   TFitter *fitter1D = new TFitter(PARAMETERS); //initialize TFitter with a maximum of 5 params
   fitter1D->SetDefaultFitter("Minuit"); // set minimizer. Default is Minuit.
   fitter1D->SetFCN(fcn1D); // set FCN

   fitter1D->SetParameter(0, "fA ", fA  , 0.0001, 0.,1.); // Bounded
   fitter1D->SetParameter(1, "mAx", mAx , 0.0001, 0.,0.);
   fitter1D->SetParameter(2, "mBx", mBx , 0.0001, 0.,0.);
   fitter1D->SetParameter(3, "sAx", sAx , 0.0001, 0.,0.);
   fitter1D->SetParameter(4, "sBx", sBx , 0.0001, 0.,0.);
   
   //Int_t ExecuteCommand(const char* command, Double_t* args, Int_t nargs)
   //Double_t arglist[2] = {0.,0.1}; //
   fitter1D->ExecuteCommand("MIGRAD", arglist, 2);

   //Double_t fcnmin, edm, errdef;
   //Int_t nvpar, nparx;
   fitter1D->GetStats(fcnmin, edm, errdef, nvpar, nparx);
   
   
   
   // ################################################
   // ###### SEPARATION POWER ########################
   // ################################################
   
   // best separation power
   Double_t sigma_best;
   sigma_best = TMath::Sqrt(fA*(1-fA)/MAX);
   cout << " ####### SEPARATION POWER ########" << std::endl;
   cout << "sigma_best = " << sigma_best << std::endl;
   
   // calcolo 2D dal fit
   Double_t sigma_fit2D;
   sigma_fit2D = fitter->GetParError(0);
   cout << "sigma_fit2D = " << sigma_fit2D << std::endl;
   cout << "sigma_best/sigma_fit2D = " << sigma_best/sigma_fit2D << std::endl;
   
   // calcolo 1D dal fit
   Double_t sigma_fit1D;
   sigma_fit1D = fitter1D->GetParError(0);
   cout << "sigma_fit1D = " << sigma_fit1D << std::endl;
   cout << "sigma_best/sigma_fit1D = " << sigma_best/sigma_fit1D << std::endl;
   
    



#if 0
    //calcolo dal Minumu Variance Bound 2D
    Double_t sigma_mvb2D;
    histo_proj_A -> Scale(1./histo_proj_A->Integral(1,bin_number_x,1,bin_number_y));
    histo_proj_B -> Scale(1./histo_proj_B->Integral(1,bin_number_x,1,bin_number_y));
  
    TH2F *histo_num   = new TH2F("histo_num"  ,"",bin_number_x,x_min,x_max,bin_number_y,y_min,y_max);
    TH2F *histo_den   = new TH2F("histo_den"  ,"",bin_number_x,x_min,x_max,bin_number_y,y_min,y_max);
    Double_t binX,binY;
    
    sigma_mvb2D = (1./MAX);
#endif    




}


