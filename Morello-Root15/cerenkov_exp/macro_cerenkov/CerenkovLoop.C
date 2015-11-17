#define CerenkovLoop_cxx
#include "CerenkovLoop.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "TString.h"

#include "labstyle.C"

#define MAX_nPIXELS 47

// A function to convert an integer to a TString
TString ToString(int num){
  ostringstream start;
  start << num;
  TString start1 = start.str();
  return start1;
}

void CerenkovLoop::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L CerenkovLoop.C
//      Root > CerenkovLoop t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   
  //Setting style
   labstyle();

   // //Create the histogram
   TH1F *histo_R         = new TH1F("histo_R","",100,25,26.);
   TH1F *histo_x0        = new TH1F("histo_x0","",100,24,25.);
   TH1F *histo_y0        = new TH1F("histo_y0","",100,24.,25.);
   TH1F *histo_FCN       = new TH1F("histo_FCN","",100,0.,10.);
   TH1F *histo_n_pixel   = new TH1F("histo_n_pixel","",50,0,50.);
  
   TH1F *histo_R_noTrack            = new TH1F("histo_R_noTrack","",100,25,26.);
   TH1F *histo_R_withTrack          = new TH1F("histo_R_withTrack","",100,25,26.);
   TH2F *histo_R_n_pixel_noTrack    = new TH2F("histo_R_n_pixel_noTrack","",100,25,26.,50,0.,50.);
   TH2F *histo_R_n_pixel_withTrack  = new TH2F("histo_R_n_pixel_withTrack","",100,25,26.,50,0.,50.);
   TH2F *histo_R_n_pixel_both       = new TH2F("histo_R_n_pixel_both","",100,25,26.,50,0.,50.);


   std::vector<TH1F*> histo_R_vs_n_pixel; 
   for(Int_t i=0; i< MAX_nPIXELS; i++){
     //Convert the integer 'i' to a TString
     TString numstr = ToString(i);
     //Create the histogram's name
     TString histoname="histo"+numstr;
     //Declare the histogram
     TH1F* hist = new TH1F(histoname,histoname, 100,25.,26.);
     //Put the histogram pointer into a vector
     histo_R_vs_n_pixel.push_back(hist);
   };



  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    
    // My code ####################################################
    
    //Create the histogram
    histo_R        -> Fill(R);
    histo_x0       -> Fill(x0);
    histo_y0       -> Fill(y0);
    histo_FCN      -> Fill(FCN);
    histo_n_pixel  -> Fill(n_pixel);
          
    histo_R_n_pixel_both  ->Fill(R,n_pixel);      
    if(n_track==0) 
      {
	histo_R_noTrack->Fill(R);
	histo_R_n_pixel_noTrack->Fill(R,n_pixel);
      }; // controllare se il pione e' 0 !!!
    if(n_track==1) 
      {
	histo_R_withTrack->Fill(R);
	histo_R_n_pixel_withTrack->Fill(R,n_pixel);
      };

    // histo_R_vs_n_pixel[0]->Fill(R);
    for(Int_t i=0; i<  histo_R_vs_n_pixel.size(); i++)
      {
    	if (n_pixel == i){ histo_R_vs_n_pixel[i]->Fill(R);};
      };

    // End of my code #############################################
    

  }; // End for on  jentry  (sugli eventi del Tree)



  // Write on .root file the histogram
   TFile *hfile = new TFile("./_root/histo_cerenkovLoop.root","RECREATE","Demo ROOT file with histograms");
   hfile->cd();
   histo_R->Write();
   histo_x0->Write();
   histo_y0->Write();
   histo_FCN->Write();
   histo_n_pixel->Write();
   
   histo_R_noTrack->Write();
   histo_R_withTrack->Write();
   histo_R_n_pixel_noTrack->Write();
   histo_R_n_pixel_withTrack->Write();
   histo_R_n_pixel_both->Write();

   //histo_R_vs_n_pixel[0]->Write();   
   for(Int_t i=0; i<  histo_R_vs_n_pixel.size(); i++)
      {
       histo_R_vs_n_pixel[i]->Write();;
      };

   hfile->Close();

}
