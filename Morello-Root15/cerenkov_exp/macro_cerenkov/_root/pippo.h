//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jun 25 16:15:11 2015 by ROOT version 5.28/00
// from TTree t2/a Tree with data
// found on file: tree_circle.root
//////////////////////////////////////////////////////////

#ifndef pippo_h
#define pippo_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class pippo {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           n_track;
   Int_t           n_pixel;
   Int_t           x_pixel[47];   //[n_pixel]
   Int_t           y_pixel[47];   //[n_pixel]
   Double_t        R;
   Double_t        x0;
   Double_t        y0;
   Double_t        FCN;

   // List of branches
   TBranch        *b_n_track;   //!
   TBranch        *b_n_pixel;   //!
   TBranch        *b_x_pixel;   //!
   TBranch        *b_y_pixel;   //!
   TBranch        *b_R;   //!
   TBranch        *b_x0;   //!
   TBranch        *b_y0;   //!
   TBranch        *b_FCN;   //!

   pippo(TTree *tree=0);
   virtual ~pippo();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef pippo_cxx
pippo::pippo(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("_root/tree_circle.root");
      if (!f) {
         f = new TFile("tree_circle.root");
      }
      tree = (TTree*)gDirectory->Get("t2");

   }
   Init(tree);
}

pippo::~pippo()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t pippo::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t pippo::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void pippo::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("n_track", &n_track, &b_n_track);
   fChain->SetBranchAddress("n_pixel", &n_pixel, &b_n_pixel);
   fChain->SetBranchAddress("x_pixel", x_pixel, &b_x_pixel);
   fChain->SetBranchAddress("y_pixel", y_pixel, &b_y_pixel);
   fChain->SetBranchAddress("R", &R, &b_R);
   fChain->SetBranchAddress("x0", &x0, &b_x0);
   fChain->SetBranchAddress("y0", &y0, &b_y0);
   fChain->SetBranchAddress("FCN", &FCN, &b_FCN);
   Notify();
}

Bool_t pippo::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void pippo::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t pippo::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef pippo_cxx
