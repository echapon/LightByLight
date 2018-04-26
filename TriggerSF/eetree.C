#define eetree_cxx
#include "eetree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

int ireg(double et, double eta);
double SF(double et, double eta);
double SF_uncert(double et, double eta);
double SF_uncert_diEG(double et1, double eta1, double et2, double eta2);

void eetree::Loop()
{
   //   In a ROOT session, you can do:
   //      root> .L eetree.C
   //      root> eetree t
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
   TFile *output;
   output = new TFile("histos_ee.root","recreate");

   const int nptbins=7;
   double ptbin[nptbins]={0,2,4,6,8,10,160};
   const int nptbin= sizeof(ptbin)/sizeof(double) - 1;
   TH1D* hnum_wt       = new TH1D("hnum_wt" ,"",nptbin,ptbin);
   TH1D* hnum          = new TH1D("hnum" ,"",nptbin,ptbin);

   TH1D** hnum_uncert          = new TH1D*[14];
   for (int i=0; i<14; i++) hnum_uncert[i] = new TH1D(Form("hnum_uncert%d",i) ,"",nptbin,ptbin);


   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if (doubleEG2!=1) continue;
      if (mass<5) continue;
      if (acop>0.01) continue;
      if (pt>1) continue;
      hnum->Fill(mass);

      double sf1 = SF(eleSCEt1,eleSCEta1);
      double sf2 = SF(eleSCEt2,eleSCEta2);

      hnum_wt->Fill(mass, sf1*sf2);


      hnum_uncert[0]->Fill(mass, SF_uncert_diEG(eleSCEt1,eleSCEta1,eleSCEt2,eleSCEta2));
      for (int ivar=1; ivar<14; ivar++) {
         double sf1p=sf1, sf2p=sf2;
         if (ireg(eleSCEt1,eleSCEta1)==ivar) sf1p += SF_uncert(eleSCEt1,eleSCEta1);
         if (ireg(eleSCEt2,eleSCEta2)==ivar) sf2p += SF_uncert(eleSCEt2,eleSCEta2);
         hnum_uncert[ivar]->Fill(mass,sf1p*sf2p);
      };
   }
   hnum->Write();
   hnum_wt->Write();
   for (int i=0; i<14; i++) hnum_uncert[i]->Write();
   output->Close();
}


int ireg(double et, double eta) {
   if (fabs(eta)<1.5) {
      if (et<3) return 1;
      else if (et<4) return 2;
      else if (et<5) return 3;
      else if (et<6) return 4;
      else if (et<7) return 5;
      else if (et<10) return 6;
      else if (et<14) return 7;
      else return 8;
   } else {
      if (et<3.5) return 9;
      else if (et<5) return 10;
      else if (et<7) return 11;
      else if (et<10) return 12;
      else return 13;
   }
}

double SF(double et, double eta) {
   if (fabs(eta)<1.5) {
      if (et<3) return 1.04953;
      else if (et<4) return 1.14323;
      else if (et<5) return 1.02158;
      else if (et<6) return 0.999508;
      else if (et<7) return 1.01261;
      else if (et<10) return 0.99313;
      else if (et<14) return 0.9943;
      else return 0.978535;
   } else {
      if (et<3.5) return 0.969575;
      else if (et<5) return 0.912982;
      else if (et<7) return 0.963249;
      else if (et<10) return 0.935725;
      else return 1;
   }
}

double SF_uncert(double et, double eta) {
   if (fabs(eta)<1.5) {
      if (et<3) return 0.404692;
      else if (et<4) return 0.0488214;
      else if (et<5) return 0.0193057;
      else if (et<6) return 0.0102954;
      else if (et<7) return 0.00513479;
      else if (et<10) return 0.00466996;
      else if (et<14) return 0.00807531;
      else return 0.0217015;
   } else {
      if (et<3.5) return 0.207799;
      else if (et<5) return 0.0621188;
      else if (et<7) return 0.0275508;
      else if (et<10) return 0.0373266;
      else return 0.0211426;
   }
}

double SF_uncert_diEG(double et1, double eta1, double et2, double eta2) {
   double SF1 = SF(et1,eta1);
   double SF2 = SF(et2,eta2);
   int i1 = ireg(et1,eta1);
   int i2 = ireg(et2,eta2);
   double uncert1 = SF_uncert(et1,eta1);
   double uncert2 = SF_uncert(et2,eta2);
   if (uncert1==uncert2 && SF1==SF2) return 2.*SF1*SF1*(uncert1/SF1);
   else return SF1*SF2*sqrt(pow(uncert1/SF1,2)+pow(uncert2/SF2,2));
}


