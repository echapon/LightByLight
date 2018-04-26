//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Apr 25 22:56:52 2018 by ROOT version 6.04/02
// from TTree trGED/tree for GED
// found on file: outputMCee.root
//////////////////////////////////////////////////////////

#ifndef eetree_h
#define eetree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class eetree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         acop;
   Float_t         mass;
   Float_t         rap;
   Float_t         pt;
   Float_t         deltapt;
   Int_t           nextra_track;
   Int_t           nhits1;
   Int_t           nhits2;
   Int_t           nhits3;
   Int_t           nhits4;
   Int_t           nhits5;
   Int_t           doubleEG2;
   Float_t         elept1;
   Float_t         eleeta1;
   Float_t         elephi1;
   Float_t         elept2;
   Float_t         eleeta2;
   Float_t         elephi2;
   Float_t         eleSCEt1;
   Float_t         eleSCEta1;
   Float_t         eleSCPhi1;
   Float_t         eleSCEt2;
   Float_t         eleSCEta2;
   Float_t         eleSCPhi2;

   // List of branches
   TBranch        *b_acop;   //!
   TBranch        *b_mass;   //!
   TBranch        *b_rap;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_deltapt;   //!
   TBranch        *b_nextra_track;   //!
   TBranch        *b_nhits1;   //!
   TBranch        *b_nhits2;   //!
   TBranch        *b_nhits3;   //!
   TBranch        *b_nhits4;   //!
   TBranch        *b_nhits5;   //!
   TBranch        *b_doubleEG2;   //!
   TBranch        *b_elept1;   //!
   TBranch        *b_eleeta1;   //!
   TBranch        *b_elephi1;   //!
   TBranch        *b_elept2;   //!
   TBranch        *b_eleeta2;   //!
   TBranch        *b_elephi2;   //!
   TBranch        *b_eleSCEt1;   //!
   TBranch        *b_eleSCEta1;   //!
   TBranch        *b_eleSCPhi1;   //!
   TBranch        *b_eleSCEt2;   //!
   TBranch        *b_eleSCEta2;   //!
   TBranch        *b_eleSCPhi2;   //!

   eetree(TTree *tree=0);
   virtual ~eetree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef eetree_cxx
eetree::eetree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("outputMCee.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("outputMCee.root");
      }
      f->GetObject("trGED",tree);

   }
   Init(tree);
}

eetree::~eetree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t eetree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t eetree::LoadTree(Long64_t entry)
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

void eetree::Init(TTree *tree)
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

   fChain->SetBranchAddress("acop", &acop, &b_acop);
   fChain->SetBranchAddress("mass", &mass, &b_mass);
   fChain->SetBranchAddress("rap", &rap, &b_rap);
   fChain->SetBranchAddress("pt", &pt, &b_pt);
   fChain->SetBranchAddress("deltapt", &deltapt, &b_deltapt);
   fChain->SetBranchAddress("nextra_track", &nextra_track, &b_nextra_track);
   fChain->SetBranchAddress("nhits1", &nhits1, &b_nhits1);
   fChain->SetBranchAddress("nhits2", &nhits2, &b_nhits2);
   fChain->SetBranchAddress("nhits3", &nhits3, &b_nhits3);
   fChain->SetBranchAddress("nhits4", &nhits4, &b_nhits4);
   fChain->SetBranchAddress("nhits5", &nhits5, &b_nhits5);
   fChain->SetBranchAddress("doubleEG2", &doubleEG2, &b_doubleEG2);
   fChain->SetBranchAddress("elept1", &elept1, &b_elept1);
   fChain->SetBranchAddress("eleeta1", &eleeta1, &b_eleeta1);
   fChain->SetBranchAddress("elephi1", &elephi1, &b_elephi1);
   fChain->SetBranchAddress("elept2", &elept2, &b_elept2);
   fChain->SetBranchAddress("eleeta2", &eleeta2, &b_eleeta2);
   fChain->SetBranchAddress("elephi2", &elephi2, &b_elephi2);
   fChain->SetBranchAddress("eleSCEt1", &eleSCEt1, &b_eleSCEt1);
   fChain->SetBranchAddress("eleSCEta1", &eleSCEta1, &b_eleSCEta1);
   fChain->SetBranchAddress("eleSCPhi1", &eleSCPhi1, &b_eleSCPhi1);
   fChain->SetBranchAddress("eleSCEt2", &eleSCEt2, &b_eleSCEt2);
   fChain->SetBranchAddress("eleSCEta2", &eleSCEta2, &b_eleSCEta2);
   fChain->SetBranchAddress("eleSCPhi2", &eleSCPhi2, &b_eleSCPhi2);
   Notify();
}

Bool_t eetree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void eetree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t eetree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef eetree_cxx
