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
// Style 
#include "labstyle.C"

//spider-2:macro_cerenkov morello$ wc -l _txt/cerenkov.dat 
// 1437818 _txt/cerenkov.dat

#define MAXEVENTS 1500000
//#define MAX_nPIXELS 49 // 47+2
 
using namespace std;

void macroCerenkov() {

  //Setting style
   labstyle();
     
   // Read the file
   std::ifstream myfile;
   myfile.open("_txt/cerenkov.dat");
   //myfile.open("_txt/cerenkov_short.dat");  // to make fast checks

   Int_t temp[MAXEVENTS];
   Int_t j;
   j=0;
   while ( true ) {
     myfile >> temp[j] ;
     if( myfile.eof() ) break;
     ++j;
   };
   
   myfile.close();
   
   Int_t max_events;
   max_events = j;
   std::cout << "Max number of lines  = " << j << std::endl;
   
   //Find the maximum numbero of pixels fired in the event
   Int_t max_pixels;
   max_pixels=0;
   for(Int_t k=0; k<max_events; k++)
     {
       if (temp[k]<999) { 
	 if (max_pixels<temp[k]) max_pixels = temp[k];
       }; //end if(k>999)
     };//end for su k

   std::cout << "Max number of pixels = " << max_pixels <<   std::endl;

   Int_t MAX_nPIXELS;
   MAX_nPIXELS = max_pixels + 2;

   // Write on .txt file
   std::ofstream myfile2;
   myfile2.open("_txt/mydata.txt");

   Int_t kk;
   kk = 0;
   while ( kk< max_events) {

     if (temp[kk]<999) { 
       Int_t vec[MAX_nPIXELS];
       for(Int_t i =0 ; i<MAX_nPIXELS;i++){ vec[i]=0;}; 
       vec[0]=temp[kk];
       vec[1]=temp[kk+1];
       //std::cout << temp[kk+1] << std::endl;
       for(Int_t i =0 ; i<temp[kk+1];i++){vec[i+2] = temp[kk+2+i]; };
       for(Int_t i =0 ; i<MAX_nPIXELS;i++){ myfile2 << vec[i] << " ";}; 
       myfile2 << std::endl;
       kk = kk+2;
       }; //end if(k>999)
     ++kk;
   };

   myfile2.close();

}


