// #define MC // comment this line to run on data

#ifdef MC
#include "eventTreeReader.C"
#include "hltTreeReader.C"
#define eventTreeReader eventTreeReader
#define hltTreeReader   hltTreeReader
#else
#include "eventTreeReaderData.C"
#include "hltTreeReaderData.C"
#define eventTreeReader eventTreeReaderData
#define hltTreeReader   hltTreeReaderData
#endif
#include "TH1F.h"
#include "TLorentzVector.h"
#include <iostream>

using namespace std;

const int MAXJETS = 500;
const float cutPt = 10;
const float cutEta = 1.4791;
const float cutDeltaR = 0.15;
const long MAXTREESIZE = 200000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB7

const double eleMass = 0.000511;
const double muMass  = 0.105658;
const double DRmax = 1.;
//float wt  = 0.0083;
float wt  = 1;

const bool looseIsolation  = true;
#define PI 3.141592653589
const float awayRange = PI * 7./8.;

//void mycheck( );
Double_t getDR  ( Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2);
Double_t getDPHI( Double_t phi1, Double_t phi2);
Double_t getDETA(Double_t eta1, Double_t eta2);
Double_t getX(Double_t Pt, Double_t Phi);
Double_t getY(Double_t Pt, Double_t Phi);
Double_t getZ(Double_t Pt, Double_t Eta);
Double_t cosTheta12(Double_t Eta1, Double_t Phi1, Double_t Eta2, Double_t Phi2);
Double_t getInvMass(Double_t Energy1, Double_t Eta1, Double_t Phi1, Double_t Energy2, Double_t Eta2, Double_t Phi2);

void hardbrem(int idir=0) {
   TChain *tchHLT = new TChain("hltanalysis/HltTree");
   TChain *tchEvt = new TChain("ggHiNtuplizer/EventTree");

#ifdef MC
   tchHLT->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/hiforest_noptcut_3-53_invmass_more_var3/000%d/hiForest_qedee_*.root",idir));
   tchEvt->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/hiforest_noptcut_3-53_invmass_more_var3/000%d/hiForest_qedee_*.root",idir));
#else
   tchHLT->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/%02d/*root",idir));
   tchEvt->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/%02d/*root",idir));
#endif
   cout << tchHLT->GetEntries() << " " << tchEvt->GetEntries() << endl;
   // return;

   hltTreeReader hltR(tchHLT);
   eventTreeReader evtR(tchEvt);

   // set branch status
   hltR.fChain->SetBranchStatus("*",0);
   evtR.fChain->SetBranchStatus("*",0);

   // GED
   evtR.fChain->SetBranchStatus("nEle",1);
   evtR.fChain->SetBranchStatus("nPho",1);
   evtR.fChain->SetBranchStatus("eleHoverE",1);
   evtR.fChain->SetBranchStatus("eleTrackIso",1);
   evtR.fChain->SetBranchStatus("eleECalIso",1);
   evtR.fChain->SetBranchStatus("eleHCalIso",1);
   evtR.fChain->SetBranchStatus("eleMissHits",1);
   evtR.fChain->SetBranchStatus("eleCharge",1);
   evtR.fChain->SetBranchStatus("elePt",1);
   evtR.fChain->SetBranchStatus("eleEta",1);
   evtR.fChain->SetBranchStatus("elePhi",1);
   // evtR.fChain->SetBranchStatus("eleSCEn",1);
   evtR.fChain->SetBranchStatus("eleSCEta",1);
   // evtR.fChain->SetBranchStatus("eleSCPhi",1);

   evtR.fChain->SetBranchStatus("pho*",1);
   evtR.fChain->SetBranchStatus("ngenTrk",1);
   evtR.fChain->SetBranchStatus("gentrk*",1);

   hltR.fChain->SetBranchStatus("HLT_HIUPCL1SingleEG5NotHF2_v1",1);
   hltR.fChain->SetBranchStatus("HLT_HIUPCL1SingleEG5NotHF2_v1_Prescl",1);
   hltR.fChain->SetBranchStatus("NL1IsolEm",1);
   hltR.fChain->SetBranchStatus("L1IsolEm*",1);
   hltR.fChain->SetBranchStatus("NL1NIsolEm",1);
   hltR.fChain->SetBranchStatus("L1NIsolEm*",1);
   // hltR.fChain->SetBranchStatus("L1Hf*",1);

   if (evtR.fChain == 0) return;
   if (hltR.fChain == 0) return;

   Long64_t nentries = evtR.fChain->GetEntriesFast();
   Long64_t nentries2 = hltR.fChain->GetEntriesFast();

   if (nentries != nentries2) return;

#ifdef MC
   TFile *fout = new TFile(Form("outputMC_%d.root",idir),"RECREATE");
#else
   TFile *fout = new TFile(Form("outputData_%d.root",idir),"RECREATE");
#endif
   
   TTree *tr = new TTree("tr","");
   // original quantities
   tr->Branch("nPho",&(evtR.nPho),"nPho/I");
   tr->Branch("phoSCEt",&(evtR.phoSCEt));
   tr->Branch("phoSCEta",&(evtR.phoSCEta));
   tr->Branch("phoSCPhi",&(evtR.phoSCPhi));
   tr->Branch("nEle",&(evtR.nEle),"nEle/I");
   tr->Branch("eleCharge",&(evtR.eleCharge));
   tr->Branch("elePt",&(evtR.elePt));
   tr->Branch("eleEta",&(evtR.eleEta));
   tr->Branch("elePhi",&(evtR.elePhi));
   tr->Branch("ngenTrk",&(evtR.ngenTrk),"ngenTrk/I");
   tr->Branch("gentrkPt",&(evtR.gentrkPt));
   tr->Branch("gentrkEta",&(evtR.gentrkEta));
   tr->Branch("gentrkPhi",&(evtR.gentrkPhi));
   // derived quantities
   int nPho_notag; float phoSCEt_notag, phoSCEta_notag, phoSCPhi_notag;
   tr->Branch("nPho_notag",&nPho_notag,"nPho_notag/I");
   tr->Branch("phoSCEt_notag",&phoSCEt_notag,"phoSCEt_notag/F");
   tr->Branch("phoSCEta_notag",&phoSCEta_notag,"phoSCEta_notag/F");
   tr->Branch("phoSCPhi_notag",&phoSCPhi_notag,"phoSCPhi_notag/F");
   float tagPt, tagEta, tagPhi;
   tr->Branch("tagPt",&tagPt,"tagPt/F");
   tr->Branch("tagEta",&tagEta,"tagEta/F");
   tr->Branch("tagPhi",&tagPhi,"tagPhi/F");

   int cnt[10] = {0};

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      cnt[0]++;
      if (jentry>100000) break;
      if (jentry%100000==0) cout << "--> " << jentry << "/" << nentries << endl;

      Long64_t ientry_evt = evtR.LoadTree(jentry);
      if (ientry_evt < 0) break;
      Long64_t ientry_hlt = hltR.LoadTree(jentry);
      if (ientry_hlt < 0) break;

      // do a first check
      evtR.b_ngenTrk->GetEntry(ientry_evt);
      if (evtR.ngenTrk<2 || evtR.ngenTrk>4) continue;
      cnt[1]++;
      evtR.b_nEle->GetEntry(ientry_evt);
      if (evtR.nEle==0 || evtR.nEle>3) continue;
      cnt[2]++;
      evtR.b_nPho->GetEntry(ientry_evt);
      if (evtR.nPho>2) continue;
      cnt[3]++;

      hltR.b_HLT_HIUPCL1SingleEG5NotHF2_v1->GetEntry(ientry_hlt);
      if (hltR.HLT_HIUPCL1SingleEG5NotHF2_v1!=1) continue;
      cnt[4]++;

      // init variables
      nPho_notag=0;
      phoSCEt_notag=-999;
      phoSCEta_notag=-999;
      phoSCPhi_notag=-999;

      // read the full tree
      nb = evtR.fChain->GetEntry(jentry);   nbytes += nb;
      nb = hltR.fChain->GetEntry(jentry);   nbytes += nb;

      // find the tag electron
      int itag=-1; int ntag=0;
      for (int i=0; i<evtR.nEle; i++) {
         bool istag = (
               evtR.elePt->at(i)>5 &&
               fabs(evtR.eleEta->at(i)<2.4) &&
               evtR.eleHoverE->at(i) < 0.02 &&
               (evtR.eleTrackIso->at(i)/evtR.elePt->at(i)) < 0.05 &&
               (evtR.eleECalIso->at(i)/evtR.elePt->at(i))  < 0.3 &&
               (evtR.eleHCalIso->at(i)/evtR.elePt->at(i))  < 0.2 &&
               evtR.eleMissHits->at(i) <= 1 &&
               fabs(evtR.eleSCEta->at(i))<2.4 &&
               (fabs(evtR.eleSCEta->at(i)) < 1.4442 || fabs(evtR.eleSCEta->at(i)) > 1.566)
               );
         if (istag) {
            itag = i;
            ntag++;
         }
      }
      if (ntag!=1) continue;
      cnt[5]++;

      tagPt = evtR.elePt->at(itag);
      tagEta = evtR.eleEta->at(itag);
      tagPhi = evtR.elePhi->at(itag);

      TLorentzVector ele0; 
      ele0.SetPtEtaPhiM(evtR.elePt->at(itag), evtR.eleEta->at(itag), evtR.elePhi->at(itag), eleMass);

      // match the electron to the trigger
      double minDR0=999;
      for (int i=0; i<hltR.NL1IsolEm; i++) {
         if (hltR.L1IsolEmEt[i]<=5) continue;
         TLorentzVector tlv; tlv.SetPtEtaPhiM(hltR.L1IsolEmEt[i],hltR.L1IsolEmEta[i],hltR.L1IsolEmPhi[i],0);
         double dr0 = tlv.DeltaR(ele0);
         if (dr0<minDR0) {
            minDR0 = dr0;
         }
      }
      for (int i=0; i<hltR.NL1NIsolEm; i++) {
         if (hltR.L1NIsolEmEt[i]<=5) continue;
         TLorentzVector tlv; tlv.SetPtEtaPhiM(hltR.L1NIsolEmEt[i],hltR.L1NIsolEmEta[i],hltR.L1NIsolEmPhi[i],0);
         double dr0 = tlv.DeltaR(ele0);
         if (dr0<minDR0) {
            minDR0 = dr0;
         }
      }
      if (minDR0>DRmax) continue;
      cnt[6]++;

      // ignore photons close to this tag electron
      vector<int> phoig;
      for (int i=0; i<evtR.nPho; i++) {
         double deta = getDETA(evtR.eleEta->at(itag), evtR.phoSCEta->at(i));
         double dphi = getDPHI(evtR.elePhi->at(itag), evtR.phoSCPhi->at(i));
         if (deta<0.15 && dphi<0.7) phoig.push_back(i);
         else {
            nPho_notag++;
            phoSCEt_notag = evtR.phoSCEt->at(i);
            phoSCEta_notag = evtR.phoSCEta->at(i);
            phoSCPhi_notag = evtR.phoSCPhi->at(i);
         }
      }

      // there should be max 1 photon unmatched to the tag
      if (nPho_notag>1) continue;
      cnt[7]++;

      // ignore the track matched to the tag electron
      int itrktag=-1;
      for (int i=0; i<evtR.ngenTrk; i++) {
         double deta = getDETA(evtR.eleEta->at(itag), evtR.gentrkEta->at(i));
         double dphi = getDPHI(evtR.elePhi->at(itag), evtR.gentrkPhi->at(i));
         if (deta<0.15 && dphi<0.7) {
            if (itrktag>=0) {
               // cout << "Error... we had already found a tag track..." << endl;
               // itrktag=-1;
               // break;
            }
            itrktag = i;
         };
      }
      if (itrktag<0) continue;
      cnt[8]++;

      // fill the tree
      tr->Fill();
   }

   fout->Write();
   fout->Close();

   for (int i=0; i<10; i++) cout << i << ": " << cnt[i] << "; ";
   cout << endl;
}

Double_t getDR( Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2){
   Double_t theDphi = getDPHI( phi1, phi2);
   Double_t theDeta = eta1 - eta2;
   return TMath::Sqrt ( theDphi*theDphi + theDeta*theDeta);
}

Double_t getDPHI( Double_t phi1, Double_t phi2) {
   return fabs(TVector2::Phi_mpi_pi(phi1-phi2));
}

Double_t getDETA(Double_t eta1, Double_t eta2){
   return TMath::Abs(eta1 - eta2);
}
Double_t getX(Double_t Pt, Double_t Phi){
   return Pt*TMath::Cos(Phi);
}
Double_t getY(Double_t Pt, Double_t Phi){
   return Pt*TMath::Sin(Phi);
}
Double_t getZ(Double_t Pt, Double_t Eta){
   return Pt*sinh(Eta);
}
Double_t cosTheta12(Double_t Eta1, Double_t Phi1, Double_t Eta2, Double_t Phi2) {
   return ((cos(Phi1 - Phi2) + sinh(Eta1) * sinh(Eta2)) / (cosh(Eta1) * cosh(Eta2)));
}
Double_t getInvMass(Double_t Energy1, Double_t Eta1, Double_t Phi1, Double_t Energy2, Double_t Eta2, Double_t Phi2) {
   return (sqrt(2 * Energy1 * Energy2 * (1 - cosTheta12(Eta1, Phi1, Eta2, Phi2))));
}
