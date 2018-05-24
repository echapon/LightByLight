//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu May  3 19:12:53 2018 by ROOT version 6.06/08
// from TTree pbpb_photonuclear_tree/Single & double photodissoc. Starlight-DPMJET, PbPb-2.76 TeV
// found on file: starlight_90GeV.root
//////////////////////////////////////////////////////////

#ifndef acceptance_h
#define acceptance_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class acceptance {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           evtNum;
   Int_t           partNum;
   Float_t         mass;
   Float_t         charge;
   Float_t         pt;
   Float_t         eta;
   Float_t         phi;

   // List of branches
   TBranch        *b_evtNum;   //!
   TBranch        *b_partNum;   //!
   TBranch        *b_mass;   //!
   TBranch        *b_charge;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_phi;   //!

   acceptance(TTree *tree=0);
   virtual ~acceptance();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef acceptance_cxx
acceptance::acceptance(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("starlight_90GeV.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("starlight_90GeV.root");
      }
      f->GetObject("pbpb_photonuclear_tree",tree);

   }
   Init(tree);
}

acceptance::~acceptance()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t acceptance::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t acceptance::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void acceptance::Init(TTree *tree)
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

   fChain->SetBranchAddress("evtNum", &evtNum, &b_evtNum);
   fChain->SetBranchAddress("partNum", &partNum, &b_partNum);
   fChain->SetBranchAddress("mass", &mass, &b_mass);
   fChain->SetBranchAddress("charge", &charge, &b_charge);
   fChain->SetBranchAddress("pt", &pt, &b_pt);
   fChain->SetBranchAddress("eta", &eta, &b_eta);
   fChain->SetBranchAddress("phi", &phi, &b_phi);
   Notify();
}

Bool_t acceptance::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void acceptance::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t acceptance::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef acceptance_cxx
