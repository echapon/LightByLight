//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Apr 23 18:36:48 2018 by ROOT version 6.04/02
// from TTree fitter_tree/
// found on file: outputMCAllNew.root
//////////////////////////////////////////////////////////

#ifndef tnpClass_h
#define tnpClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class tnpClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         tag_pt;
   Float_t         tag_eta;
   Float_t         tag_abseta;
   Float_t         tag_phi;
   Float_t         tag_mindr;
   Float_t         tag_EtMindr;
   Float_t         pt;
   Float_t         eta;
   Float_t         abseta;
   Float_t         phi;
   Float_t         mindr;
   Float_t         EtMindr;
   Float_t         SCE;
   Float_t         SCEt;
   Float_t         SCEta;
   Float_t         SCPhi;
   Float_t         diPt;
   Float_t         diRap;
   Float_t         diPhi;
   Float_t         diMass;
   Float_t         diAcop;
   Int_t           doubleEG2;
   Int_t           matchedOK;

   // List of branches
   TBranch        *b_tag_pt;   //!
   TBranch        *b_tag_eta;   //!
   TBranch        *b_tag_abseta;   //!
   TBranch        *b_tag_phi;   //!
   TBranch        *b_tag_mindr;   //!
   TBranch        *b_tag_EtMindr;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_abseta;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_mindr;   //!
   TBranch        *b_EtMindr;   //!
   TBranch        *b_SCE;   //!
   TBranch        *b_SCEt;   //!
   TBranch        *b_SCEta;   //!
   TBranch        *b_SCPhi;   //!
   TBranch        *b_diPt;   //!
   TBranch        *b_diRap;   //!
   TBranch        *b_diPhi;   //!
   TBranch        *b_diMass;   //!
   TBranch        *b_diAcop;   //!
   TBranch        *b_doubleEG2;   //!
   TBranch        *b_matchedOK;   //!

   tnpClass(TTree *tree=0);
   virtual ~tnpClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef tnpClass_cxx
tnpClass::tnpClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("outputMCAllNew.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("outputMCAllNew.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("outputMCAllNew.root:/trigTree_GED");
      dir->GetObject("fitter_tree",tree);

   }
   Init(tree);
}

tnpClass::~tnpClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t tnpClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t tnpClass::LoadTree(Long64_t entry)
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

void tnpClass::Init(TTree *tree)
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

   fChain->SetBranchAddress("tag_pt", &tag_pt, &b_tag_pt);
   fChain->SetBranchAddress("tag_eta", &tag_eta, &b_tag_eta);
   fChain->SetBranchAddress("tag_abseta", &tag_abseta, &b_tag_abseta);
   fChain->SetBranchAddress("tag_phi", &tag_phi, &b_tag_phi);
   fChain->SetBranchAddress("tag_mindr", &tag_mindr, &b_tag_mindr);
   fChain->SetBranchAddress("tag_EtMindr", &tag_EtMindr, &b_tag_EtMindr);
   fChain->SetBranchAddress("pt", &pt, &b_pt);
   fChain->SetBranchAddress("eta", &eta, &b_eta);
   fChain->SetBranchAddress("abseta", &abseta, &b_abseta);
   fChain->SetBranchAddress("phi", &phi, &b_phi);
   fChain->SetBranchAddress("mindr", &mindr, &b_mindr);
   fChain->SetBranchAddress("EtMindr", &EtMindr, &b_EtMindr);
   fChain->SetBranchAddress("SCE", &SCE, &b_SCE);
   fChain->SetBranchAddress("SCEt", &SCEt, &b_SCEt);
   fChain->SetBranchAddress("SCEta", &SCEta, &b_SCEta);
   fChain->SetBranchAddress("SCPhi", &SCPhi, &b_SCPhi);
   fChain->SetBranchAddress("diPt", &diPt, &b_diPt);
   fChain->SetBranchAddress("diRap", &diRap, &b_diRap);
   fChain->SetBranchAddress("diPhi", &diPhi, &b_diPhi);
   fChain->SetBranchAddress("diMass", &diMass, &b_diMass);
   fChain->SetBranchAddress("diAcop", &diAcop, &b_diAcop);
   fChain->SetBranchAddress("doubleEG2", &doubleEG2, &b_doubleEG2);
   fChain->SetBranchAddress("matchedOK", &matchedOK, &b_matchedOK);
   Notify();
}

Bool_t tnpClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void tnpClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t tnpClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef tnpClass_cxx
