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

const double eleMass = 0.5109989461e-3;
const double DRmax   = 0.3;

double acop(double dphi) {
   return 1.-fabs(TVector2::Phi_mpi_pi(dphi))/TMath::Pi(); 
};

class clHist {
   public:
      TH1F *hpt;
      TH1F *hmass;
      TH1F *hrap;
      TH1F *haco;
      TH1F *hint;

      clHist(const char* name) {
         hpt = new TH1F(Form("hpt_%s",name),"",20,0,2);
         hmass = new TH1F(Form("hmass_%s",name),"",80,0,20);
         hrap = new TH1F(Form("hrap_%s",name),"",30,-3,3);
         haco = new TH1F(Form("haco_%s",name),"",30,0,0.06);
         hint = new TH1F(Form("hint_%s",name),"",1,0,1);
      };

      void fill(double pt, double rap, double mass, double dphi, double weight=1.) {
         hpt->Fill(pt,weight);
         hrap->Fill(rap,weight);
         hmass->Fill(mass,weight);
         haco->Fill(acop(dphi),weight);
         hint->Fill(0.5,weight);
      };
};


void fillHistosTnP(int idir=0) {
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

   // HM
   evtR.fChain->SetBranchStatus("ngsfEle",1);
   evtR.fChain->SetBranchStatus("elegsf*",1);
   evtR.fChain->SetBranchStatus("nsc_*",1);
   evtR.fChain->SetBranchStatus("sc_*",1);

   hltR.fChain->SetBranchStatus("HLT_HIUPCL1SingleEG5NotHF2_v1",1);
   // hltR.fChain->SetBranchStatus("HLT_HIUPCL1SingleEG5NotHF2_v1_Prescl",1);
   hltR.fChain->SetBranchStatus("NL1IsolEm",1);
   hltR.fChain->SetBranchStatus("L1IsolEm*",1);
   hltR.fChain->SetBranchStatus("NL1NIsolEm",1);
   hltR.fChain->SetBranchStatus("L1NIsolEm*",1);
   evtR.fChain->SetBranchStatus("ngenTrk",1);
   evtR.fChain->SetBranchStatus("gentrk*",1);

   if (evtR.fChain == 0) return;
   if (hltR.fChain == 0) return;

   Long64_t nentries = evtR.fChain->GetEntriesFast();
   Long64_t nentries2 = hltR.fChain->GetEntriesFast();

   if (nentries != nentries2) return;

#ifdef MC
   TFile *fout = new TFile(Form("outputMC%d.root",idir),"RECREATE");
#else
   TFile *fout = new TFile(Form("outputData%d.root",idir),"RECREATE");
#endif

   // tree for tnp
   TDirectory *tdir = fout->mkdir("eleTree");
   tdir->cd();
   TTree *tr = new TTree("fitter_tree","");
   // probe varialbles
   float pt, eta, abseta, phi, mindr, EtMindr;
   // diele variables
   float diPt, diRap, diPhi, diMass, diAcop;
   // flags
   int passGED, passGSF;
   // book variables
   tr->Branch("pt",&pt,"pt/F");
   tr->Branch("eta",&eta,"eta/F");
   tr->Branch("abseta",&abseta,"abseta/F");
   tr->Branch("phi",&phi,"phi/F");
   tr->Branch("diPt",&diPt,"diPt/F");
   tr->Branch("diRap",&diRap,"diRap/F");
   tr->Branch("diPhi",&diPhi,"diPhi/F");
   tr->Branch("diMass",&diMass,"diMass/F");
   tr->Branch("diAcop",&diAcop,"diAcop/F");
   tr->Branch("passGED",&passGED,"passGED/I");
   tr->Branch("passGSF",&passGSF,"passGSF/I");

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      if (jentry>100000) break;
      if (jentry%100000==0) cout << "--> " << jentry << "/" << nentries << endl;
      Long64_t ientry_evt = evtR.LoadTree(jentry);
      if (ientry_evt < 0) break;
      Long64_t ientry_hlt = hltR.LoadTree(jentry);
      if (ientry_hlt < 0) break;

      // basic cuts
      evtR.b_nEle->GetEntry(ientry_evt);
      evtR.b_ngsfEle->GetEntry(ientry_evt);
      evtR.b_nPho->GetEntry(ientry_evt);
      bool ele_twomax   =  ( (evtR.nEle==1 || evtR.nEle==2) && evtR.ngsfEle<=2);
      bool no_pho    =  (evtR.nPho==0);
      // if (!ele_twomax || !no_pho) continue;
      if (!ele_twomax) continue;
      evtR.b_ngenTrk->GetEntry(ientry_evt);
      if (evtR.ngenTrk>10) continue;

      nb = evtR.fChain->GetEntry(jentry);   nbytes += nb;
      nb = hltR.fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      // bool exclOK   =  false;
      bool SingleEG5ok = hltR.HLT_HIUPCL1SingleEG5NotHF2_v1;
      if (!SingleEG5ok) continue;


      // look for a tag
      bool ele0ok = (
            evtR.eleHoverE->at(0) < 0.02 &&
            (evtR.eleTrackIso->at(0)/evtR.elePt->at(0)) < 0.05 &&
            (evtR.eleECalIso->at(0)/evtR.elePt->at(0))  < 0.3 && 
            (evtR.eleHCalIso->at(0)/evtR.elePt->at(0))  < 0.2 && 
            evtR.eleMissHits->at(0) <= 1 && 
            evtR.elePt->at(0) > 2 && 
            fabs(evtR.eleEta->at(0))<2.4 && 
            fabs(evtR.eleSCEta->at(0))<2.4 && 
            (fabs(evtR.eleSCEta->at(0)) < 1.4442 || fabs(evtR.eleSCEta->at(0)) > 1.566)
            );
      bool ele1ok = (evtR.nEle==2 &&
            evtR.eleHoverE->at(1) < 0.02 &&
            (evtR.eleTrackIso->at(1)/evtR.elePt->at(1)) < 0.05 &&
            (evtR.eleECalIso->at(1)/evtR.elePt->at(1))  < 0.3 && 
            (evtR.eleHCalIso->at(1)/evtR.elePt->at(1))  < 0.2 && 
            evtR.eleMissHits->at(1) <= 1 && 
            evtR.elePt->at(1) > 2 && 
            fabs(evtR.eleEta->at(1))<2.4 && 
            fabs(evtR.eleSCEta->at(1))<2.4 && 
            (fabs(evtR.eleSCEta->at(1)) < 1.4442 || fabs(evtR.eleSCEta->at(1)) > 1.566)
            );

      TLorentzVector ele0GED,ele1GED;
      ele0GED.SetPtEtaPhiM(evtR.elePt->at(0),evtR.eleEta->at(0),evtR.elePhi->at(0),eleMass);
      if (evtR.nEle==2) ele1GED.SetPtEtaPhiM(evtR.elePt->at(1),evtR.eleEta->at(1),evtR.elePhi->at(1),eleMass);

      // trigger matching on the tag
      double minDR0 = 999., minDR1 = 999.;
      for (int i=0; i<hltR.NL1IsolEm; i++) {
         if (hltR.L1IsolEmEt[i]<5) continue;
         TLorentzVector tlv; tlv.SetPtEtaPhiM(hltR.L1IsolEmEt[i],hltR.L1IsolEmEta[i],hltR.L1IsolEmPhi[i],0);
         double dr0 = tlv.DeltaR(ele0GED);
         double dr1 = tlv.DeltaR(ele1GED);
         if (dr0<minDR0) {
            minDR0 = dr0;
         }
         if (dr1<minDR1) {
            minDR1 = dr1;
         }
      }
      for (int i=0; i<hltR.NL1NIsolEm; i++) {
         if (hltR.L1NIsolEmEt[i]<5) continue;
         TLorentzVector tlv; tlv.SetPtEtaPhiM(hltR.L1NIsolEmEt[i],hltR.L1NIsolEmEta[i],hltR.L1NIsolEmPhi[i],0);
         double dr0 = tlv.DeltaR(ele0GED);
         double dr1 = tlv.DeltaR(ele1GED);
         if (dr0<minDR0) {
            minDR0 = dr0;
         }
         if (dr1<minDR1) {
            minDR1 = dr1;
         }
      }

      if (!ele0ok && !ele1ok) continue;

      ele0ok = ele0ok && minDR0<DRmax;
      ele1ok = ele1ok && minDR1<DRmax;

      if (!ele0ok && !ele1ok) continue;

      // now look at the probe
      TLorentzVector trk0, trk1;
      TLorentzVector tagtrk0, tagtrk1;
      double diAcop0, diAcop1;
      bool probe0=false, probe1=false;
      for (int i=0; i<evtR.ngenTrk; i++) {
         if (evtR.gentrkPt->at(i)<2 || fabs(evtR.gentrkEta->at(i)>2.4)) continue;
         TLorentzVector tmptrk;
         tmptrk.SetPtEtaPhiM(evtR.gentrkPt->at(i),evtR.gentrkEta->at(i),evtR.gentrkPhi->at(i),eleMass);

         TLorentzVector tmp;
         if (ele0ok && evtR.eleCharge->at(0) != evtR.gentrkcharge->at(i)) {
            tmp = tmptrk+ele0GED;
            if (tmp.M()>4 && tmp.Pt()<2) {
               trk0 = tmptrk;
               tagtrk0 = tmp;
               diAcop0 = acop(tmptrk.DeltaPhi(ele0GED));
               probe0=true;
            }
         }
         if (ele1ok && evtR.eleCharge->at(1) != evtR.gentrkcharge->at(i)) {
            tmp = tmptrk+ele1GED;
            if (tmp.M()>4 && tmp.Pt()<2) {
               trk1 = tmptrk;
               tagtrk1 = tmp;
               diAcop1 = acop(tmptrk.DeltaPhi(ele1GED));
               probe1=true;
            }
         }
      }

      if (!probe0 && !probe1) continue;

      // we have the probes. are they passing probes (ie matched to passing electrons)?
      bool pass0GED=false, pass1GED=false;
      bool pass0GSF=false, pass1GSF=false;

      // first look at GSF electrons
      bool eleGSF0ok = (evtR.ngsfEle>0 &&
            evtR.elegsfTrkPt->at(0) > 2 && 
            fabs(evtR.elegsfTrkEta->at(0)) < 2.4 && 
            evtR.elegsfTrkMissHits->at(0) <= 1
            );
      bool eleGSF1ok = (evtR.ngsfEle==2 &&
            evtR.elegsfTrkPt->at(1) > 2 && 
            fabs(evtR.elegsfTrkEta->at(1)) < 2.4 && 
            evtR.elegsfTrkMissHits->at(1) <= 1
            );
      TLorentzVector ele0GSF,ele1GSF;
      ele0GSF.SetPtEtaPhiM(evtR.elegsfTrkPt->at(0),evtR.elegsfTrkEta->at(0),evtR.elegsfTrkPhi->at(0),eleMass);
      if (evtR.nEle==2) ele1GSF.SetPtEtaPhiM(evtR.elegsfTrkPt->at(1),evtR.elegsfTrkEta->at(1),evtR.elegsfTrkPhi->at(1),eleMass);

      // matching phase with the GED and GSF electrons we found (matching: see criteria for fillNExtratracks)
      if (probe0) {
         // if (trk0.DeltaPhi(ele0GED)<0.7 && fabs(trk0.Eta()-ele0GED.Eta())<0.15) pass0GED=true;
         if (trk0.DeltaPhi(ele1GED)<0.7 && fabs(trk0.Eta()-ele1GED.Eta())<0.15) pass0GED=true;
         if (trk0.DeltaPhi(ele0GSF)<0.7 && fabs(trk0.Eta()-ele0GSF.Eta())<0.15) pass0GSF=true;
         if (trk0.DeltaPhi(ele1GSF)<0.7 && fabs(trk0.Eta()-ele1GSF.Eta())<0.15) pass0GSF=true;
      }
      if (probe1) {
         if (trk1.DeltaPhi(ele0GED)<0.7 && fabs(trk1.Eta()-ele0GED.Eta())<0.15) pass1GED=true;
         // if (trk1.DeltaPhi(ele1GED)<0.7 && fabs(trk1.Eta()-ele1GED.Eta())<0.15) pass1GED=true;
         if (trk1.DeltaPhi(ele0GSF)<0.7 && fabs(trk1.Eta()-ele0GSF.Eta())<0.15) pass1GSF=true;
         if (trk1.DeltaPhi(ele1GSF)<0.7 && fabs(trk1.Eta()-ele1GSF.Eta())<0.15) pass1GSF=true;
      }

      // fill the tree
      if (probe0) {
         pt = trk0.Pt();
         eta = trk0.Eta();
         abseta = fabs(trk0.Eta());
         phi = trk0.Phi();
         diPt = tagtrk0.Pt();
         diRap = tagtrk0.Rapidity();
         diPhi = tagtrk0.Phi();
         diMass = tagtrk0.M();
         diAcop = diAcop0;
         passGED = pass0GED;
         passGSF = pass0GSF;
         tr->Fill();
      }
      if (probe1) {
         pt = trk1.Pt();
         eta = trk1.Eta();
         abseta = fabs(trk1.Eta());
         phi = trk1.Phi();
         diPt = tagtrk1.Pt();
         diRap = tagtrk1.Rapidity();
         diPhi = tagtrk1.Phi();
         diMass = tagtrk1.M();
         diAcop = diAcop1;
         passGED = pass1GED;
         passGSF = pass1GSF;
         tr->Fill();
      }
   } // event loop

   fout->Write();
   fout->Close();
}

