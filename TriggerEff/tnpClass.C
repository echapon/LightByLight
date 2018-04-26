#define tnpClass_cxx
#include "tnpClass.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void tnpClass::Loop()
{
//   In a ROOT session, you can do:
//      root> .L tnpClass.C
//      root> tnpClass t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
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
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   TFile *f = TFile::Open("histos.root","RECREATE");
   const int nbins=11;
   double bins[nbins+1] = {0, 2, 2.5, 3, 3.5, 4, 5, 6, 8, 10, 14, 20};
   TH1F *hnum = new TH1F ("hnum",";E_{T} [GeV];Efficiency",nbins,bins);
   TH1F *hden = new TH1F ("hden",";E_{T} [GeV];Efficiency",nbins,bins);
   TH1F *hnum_EB = new TH1F ("hnum_EB",";E_{T} [GeV];Efficiency",nbins,bins);
   TH1F *hden_EB = new TH1F ("hden_EB",";E_{T} [GeV];Efficiency",nbins,bins);
   TH1F *hnum_EE = new TH1F ("hnum_EE",";E_{T} [GeV];Efficiency",nbins,bins);
   TH1F *hden_EE = new TH1F ("hden_EE",";E_{T} [GeV];Efficiency",nbins,bins);

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      
      double var = SCEt; // pt;
      hden->Fill(var);
      if (fabs(eta)<1.5) hden_EB->Fill(var);
      else hden_EE->Fill(var);
      if (matchedOK==1) {
         hnum->Fill(var);
         if (fabs(eta)<1.5) hnum_EB->Fill(var);
         else hnum_EE->Fill(var);
      }
   }

   f->Write();
   f->Close();
}
