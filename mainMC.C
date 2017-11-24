#include "eventTreeReader.C"
#include "hltTreeReader.C"
#include "pixelTreeReader.C"
#include "TH1F.h"
#include "TLorentzVector.h"
#include <iostream>

using namespace std;

const double eleMass = 0.5109989461e-3;

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
         hmass = new TH1F(Form("hmass_%s",name),"",20,0,20);
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

void fillExclVars(const eventTreeReader &evtR, 
      double eleEta0, double elePhi0, double eleEta1, double elePhi1, 
      double &EmEnergy_EB, double &EmEnergy_EE, double &HadEnergy_HB, double &HadEnergy_HE, double &HadEnergy_HF_Plus, double &HadEnergy_HF_Minus);
void fillNextraTracks(const eventTreeReader &evtR, 
      double eleEta0, double elePhi0, double eleEta1, double elePhi1, 
      int &nextratracks);

void mainMC() {
   TChain *tchHLT = new TChain("hltanalysis/HltTree");
   TChain *tchEvt = new TChain("ggHiNtuplizer/EventTree");
   TChain *tchPix = new TChain("pixel/PixelTree");

   for (int i=1; i<=8; i++) {
      tchHLT->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/hiforest_noptcut_3-53_invmass_more_var3/000%d/hiForest_qedee_*.root",i));
      tchEvt->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/hiforest_noptcut_3-53_invmass_more_var3/000%d/hiForest_qedee_*.root",i));
      tchPix->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/hiforest_noptcut_3-53_invmass_more_var3/000%d/hiForest_qedee_*.root",i));
   }
   cout << tchHLT->GetEntries() << " " << tchEvt->GetEntries() << " " << tchPix->GetEntries() << endl;
   // return;

   hltTreeReader hltR(tchHLT);
   eventTreeReader evtR(tchEvt);
   pixelTreeReader pixR(tchPix);

   // set branch status
   hltR.fChain->SetBranchStatus("*",0);
   evtR.fChain->SetBranchStatus("*",0);
   pixR.fChain->SetBranchStatus("*",0);

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

   evtR.fChain->SetBranchStatus("nMC",1);
   evtR.fChain->SetBranchStatus("mcPID",1);
   evtR.fChain->SetBranchStatus("mcStatus",1);
   evtR.fChain->SetBranchStatus("mcPt",1);
   evtR.fChain->SetBranchStatus("mcEta",1);
   evtR.fChain->SetBranchStatus("mcPhi",1);
   evtR.fChain->SetBranchStatus("mcMass",1);
   hltR.fChain->SetBranchStatus("HLT_HIUPCL1SingleEG5NotHF2_v1",1);
   hltR.fChain->SetBranchStatus("HLT_HIUPCL1SingleEG5NotHF2_v1_Prescl",1);
   hltR.fChain->SetBranchStatus("HLT_HIUPCL1DoubleEG2NotHF2_v1",1);
   hltR.fChain->SetBranchStatus("HLT_HIUPCL1DoubleEG2NotHF2_v1_Prescl",1);
   pixR.fChain->SetBranchStatus("nEv",1); 
   pixR.fChain->SetBranchStatus("nhits*",1); 

   if (evtR.fChain == 0) return;
   if (pixR.fChain == 0) return;
   if (hltR.fChain == 0) return;

   Long64_t nentries = evtR.fChain->GetEntriesFast();
   Long64_t nentries2 = hltR.fChain->GetEntriesFast();
   Long64_t nentries3 = pixR.fChain->GetEntriesFast();

   if (nentries != nentries2 || nentries != nentries3) return;

   TFile *fout = new TFile("output.root","RECREATE");
   clHist hGenAll("genall");
   clHist hGenPass("genpass");

   
   clHist hRecoGEDPass_gen("recoGEDpass_gen");
   clHist hRecoGEDPass_recoGED("recoGEDpass_recoGED");
   clHist hRecoGEDPassGenPass_gen("recoGEDpass_genpass_gen");
   clHist hRecoGEDPassGenPass_recoGED("recoGEDpass_genpass_recoGED");
   
   clHist hRecoGEDPassTrigSingle_gen("recoGEDpassTrigSingle_gen");
   clHist hRecoGEDPassTrigSingle_recoGED("recoGEDpassTrigSingle_recoGED");
   clHist hRecoGEDPassTrigSingleGenPassTrigSingle_gen("recoGEDpassTrigSingle_genpassTrigSingle_gen");
   clHist hRecoGEDPassTrigSingleGenPassTrigSingle_recoGED("recoGEDpassTrigSingle_genpassTrigSingle_recoGED");
   
   clHist hRecoGEDPassTrigDouble_gen("recoGEDpassTrigDouble_gen");
   clHist hRecoGEDPassTrigDouble_recoGED("recoGEDpassTrigDouble_recoGED");
   clHist hRecoGEDPassTrigDoubleGenPassTrigDouble_gen("recoGEDpassTrigDouble_genpassTrigDouble_gen");
   clHist hRecoGEDPassTrigDoubleGenPassTrigDouble_recoGED("recoGEDpassTrigDouble_genpassTrigDouble_recoGED");

   
   clHist hRecoHMPass_gen("recoHMpass_gen");
   clHist hRecoHMPass_recoHM("recoHMpass_recoHM");
   clHist hRecoHMPassGenPass_gen("recoHMpass_genpass_gen");
   clHist hRecoHMPassGenPass_recoHM("recoHMpass_genpass_recoHM");
   
   clHist hRecoHMPassTrigSingle_gen("recoHMpassTrigSingle_gen");
   clHist hRecoHMPassTrigSingle_recoHM("recoHMpassTrigSingle_recoHM");
   clHist hRecoHMPassTrigSingleGenPassTrigSingle_gen("recoHMpassTrigSingle_genpassTrigSingle_gen");
   clHist hRecoHMPassTrigSingleGenPassTrigSingle_recoHM("recoHMpassTrigSingle_genpassTrigSingle_recoHM");
   
   clHist hRecoHMPassTrigDouble_gen("recoHMpassTrigDouble_gen");
   clHist hRecoHMPassTrigDouble_recoHM("recoHMpassTrigDouble_recoHM");
   clHist hRecoHMPassTrigDoubleGenPassTrigDouble_gen("recoHMpassTrigDouble_genpassTrigDouble_gen");
   clHist hRecoHMPassTrigDoubleGenPassTrigDouble_recoHM("recoHMpassTrigDouble_genpassTrigDouble_recoHM");

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      // if (jentry>10000) break;
      if (jentry%100000==0) cout << "--> " << jentry << "/" << nentries << endl;
      Long64_t ientry_evt = evtR.LoadTree(jentry);
      if (ientry_evt < 0) break;
      // Long64_t ientry_pix = pixR.LoadTree(jentry);
      // if (ientry_pix < 0) break;
      Long64_t ientry_hlt = hltR.LoadTree(jentry);
      if (ientry_hlt < 0) break;

      nb = evtR.fChain->GetEntry(jentry);   nbytes += nb;
      nb = hltR.fChain->GetEntry(jentry);   nbytes += nb;
      // nb = pixR.fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      bool genok;
      bool recoGEDok;
      bool recoHMok;
      double genpt, genrap, genmass, gendphi;
      double recoGEDpt, recoGEDrap, recoGEDmass, recoGEDdphi;
      double recoHMpt, recoHMrap, recoHMmass, recoHMdphi;
      // bool exclOK   =  false;
      bool SingleEG5ok = hltR.HLT_HIUPCL1SingleEG5NotHF2_v1;
      bool DoubleEG2ok = hltR.HLT_HIUPCL1DoubleEG2NotHF2_v1;

      // --- GEN CUTS ---
      // usually we only have 2 MC particles and they are electrons
      if (evtR.nMC!=2 || abs(evtR.mcPID->at(0))!=11 || abs(evtR.mcPID->at(1)) != 11) {
         cout << "MC info error " << evtR.nMC << endl;
      }

      TLorentzVector genele0, genele1, gendiele;
      genele0.SetPtEtaPhiM(evtR.mcPt->at(0),evtR.mcEta->at(0),evtR.mcPhi->at(0),evtR.mcMass->at(0));
      genele1.SetPtEtaPhiM(evtR.mcPt->at(1),evtR.mcEta->at(1),evtR.mcPhi->at(1),evtR.mcMass->at(1));
      gendiele = genele0+genele1;
      genpt = gendiele.Pt();
      genrap = gendiele.Rapidity();
      genmass = gendiele.M();
      gendphi = genele0.DeltaPhi(genele1);
      genok = (evtR.mcPt->at(0)>2&&evtR.mcPt->at(1)>2
            &&fabs(evtR.mcEta->at(0))<2.5&&fabs(evtR.mcEta->at(1))<2.5
            &&fabs(genrap)<2.5&&genmass>4&&genmass<53
            &&genpt<2
            &&(evtR.mcPID->at(0)==-evtR.mcPID->at(1)));

      // --- GED cuts ---
      bool ele_two   =  (evtR.nEle==2 );
      bool no_pho    =  (evtR.nPho==0);
      bool hovere    =  ele_two ? (evtR.eleHoverE->at(0) < 0.02 && evtR.eleHoverE->at(1) < 0.02) : false;
      bool track_iso =  ele_two ? ((evtR.eleTrackIso->at(0)/evtR.elePt->at(0)) < 0.05 && (evtR.eleTrackIso->at(1)/evtR.elePt->at(1)) < 0.05) : false;
      bool ecal_iso  =  ele_two ? ((evtR.eleECalIso->at(0)/evtR.elePt->at(0))  < 0.3 && (evtR.eleECalIso->at(1)/evtR.elePt->at(1))  < 0.3) : false;
      bool hcal_iso  =  ele_two ? ((evtR.eleHCalIso->at(0)/evtR.elePt->at(0))  < 0.2 && (evtR.eleHCalIso->at(1)/evtR.elePt->at(1))  < 0.2) : false;
      bool miss_hit  =  ele_two ? (evtR.eleMissHits->at(0) <= 1 && evtR.eleMissHits->at(0) <= 1) : false;
      bool opp_chrg  =  ele_two ? (evtR.eleCharge->at(0) != evtR.eleCharge->at(1) ) : false;
      bool ele_pt    =  ele_two ? (evtR.elePt->at(0) > 2 && evtR.elePt->at(1) > 2 ) : false;
      bool ele_eta   =  ele_two ? (fabs(evtR.eleEta->at(0))<2.5 && fabs(evtR.eleEta->at(1))<2.5 ) : false;
      bool ele_SCeta   =  ele_two ? (fabs(evtR.eleSCEta->at(0))<2.5 && fabs(evtR.eleSCEta->at(1))<2.5
            && (fabs(evtR.eleSCEta->at(0)) < 1.4442 || fabs(evtR.eleSCEta->at(0)) > 1.566)
            && (fabs(evtR.eleSCEta->at(1)) < 1.4442 || fabs(evtR.eleSCEta->at(1)) > 1.566)) : false;
      bool iso       =  (hovere && track_iso && ecal_iso && hcal_iso);
      TLorentzVector ele0, ele1, diele;
      if (ele_two) {
         ele0.SetPtEtaPhiM(evtR.elePt->at(0),evtR.eleEta->at(0),evtR.elePhi->at(0),eleMass);
         ele1.SetPtEtaPhiM(evtR.elePt->at(1),evtR.eleEta->at(1),evtR.elePhi->at(1),eleMass);
         diele = ele0+ele1;
         recoGEDpt = diele.Pt();
         recoGEDrap = diele.Rapidity();
         recoGEDmass = diele.M();
         recoGEDdphi = ele0.DeltaPhi(ele1);

      //    double EmEnergy_EB = 0;
      //    double EmEnergy_EE = 0;
      //    double HadEnergy_HB = 0;
      //    double HadEnergy_HE = 0;
      //    double HadEnergy_HF_Plus = 0;
      //    double HadEnergy_HF_Minus = 0;
      //    evtR.b_nTower->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_eta->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_phi->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_emE->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_hadE->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_e->GetEntry(ientry_evt);
      //    fillExclVars(evtR, 
      //          evtR.eleEta->at(0), evtR.elePhi->at(0), evtR.eleEta->at(1), evtR.elePhi->at(1), 
      //          EmEnergy_EB, EmEnergy_EE, HadEnergy_HB, HadEnergy_HE, HadEnergy_HF_Plus, HadEnergy_HF_Minus);
      //    exclOK = (EmEnergy_EB< 0.55 && EmEnergy_EE < 3.16 && HadEnergy_HB < 2.0 && HadEnergy_HE < 3.0 && HadEnergy_HF_Plus < 4.85 && HadEnergy_HF_Minus < 4.12);

      //    evtR.b_ngenTrk->GetEntry(ientry_evt);
      //    evtR.b_gentrkPt->GetEntry(ientry_evt);
      //    evtR.b_gentrkEta->GetEntry(ientry_evt);
      //    evtR.b_gentrkPhi->GetEntry(ientry_evt);
      //    int nextra_track=0;
      //    fillNextraTracks(evtR, 
      //          evtR.eleEta->at(0), evtR.elePhi->at(0), evtR.eleEta->at(1), evtR.elePhi->at(1), 
      //          nextra_track);

      //    exclOK = exclOK && (nextra_track==0);
      }
      recoGEDok    =  ele_pt && ele_eta && ele_SCeta && (recoGEDmass>4) && miss_hit && iso 
         && (recoGEDpt < 2.0) && (fabs(recoGEDrap)<2.5) && (acop(ele0.DeltaPhi(ele1)) < 0.01);
      // recoGEDok = recoGEDok && exclOK;

      // --- HM cuts ---
      bool eleHM_two   =  (evtR.ngsfEle==2 );
      bool no_HMpho    =  (evtR.nHyPho==0);
      bool ele_gsf_pt  =  eleHM_two ? ( evtR.elegsfTrkPt->at(0) > 2 && evtR.elegsfTrkPt->at(1) > 2 ) : false;
      bool ele_gsf_eta  =  eleHM_two ? ( fabs(evtR.elegsfTrkEta->at(0)) < 2.5 && fabs(evtR.elegsfTrkEta->at(1)) < 2.5 ) : false;
      bool gsf_miss_hit=  eleHM_two ? ( evtR.elegsfTrkMissHits->at(0) <= 1 && evtR.elegsfTrkMissHits->at(1) <= 1) : false;

      // exclOK=false;
      ele0 = TLorentzVector();
      ele1 = TLorentzVector();
      diele = TLorentzVector();
      if (eleHM_two) {
         ele0.SetPtEtaPhiM(evtR.elegsfTrkPt->at(0),evtR.elegsfTrkEta->at(0),evtR.elegsfTrkPhi->at(0),eleMass);
         ele1.SetPtEtaPhiM(evtR.elegsfTrkPt->at(1),evtR.elegsfTrkEta->at(1),evtR.elegsfTrkPhi->at(1),eleMass);
         diele = ele0+ele1;
         recoHMpt = diele.Pt();
         recoHMrap = diele.Rapidity();
         recoHMmass = diele.M();
         recoHMdphi = ele0.DeltaPhi(ele1);

      //    double EmEnergy_EB = 0;
      //    double EmEnergy_EE = 0;
      //    double HadEnergy_HB = 0;
      //    double HadEnergy_HE = 0;
      //    double HadEnergy_HF_Plus = 0;
      //    double HadEnergy_HF_Minus = 0;
      //    evtR.b_nTower->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_eta->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_phi->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_emE->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_hadE->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_e->GetEntry(ientry_evt);
      //    fillExclVars(evtR, 
      //          ele0.Eta(), ele0.Phi(), ele1.Eta(), ele1.Phi(), 
      //          EmEnergy_EB, EmEnergy_EE, HadEnergy_HB, HadEnergy_HE, HadEnergy_HF_Plus, HadEnergy_HF_Minus);
      //    exclOK = (EmEnergy_EB< 0.55 && EmEnergy_EE < 3.16 && HadEnergy_HB < 2.0 && HadEnergy_HE < 3.0 && HadEnergy_HF_Plus < 4.85 && HadEnergy_HF_Minus < 4.12);

      //    evtR.b_ngenTrk->GetEntry(ientry_evt);
      //    evtR.b_gentrkPt->GetEntry(ientry_evt);
      //    evtR.b_gentrkEta->GetEntry(ientry_evt);
      //    evtR.b_gentrkPhi->GetEntry(ientry_evt);
      //    int nextra_track=0;
      //    fillNextraTracks(evtR, 
      //          ele0.Eta(), ele0.Phi(), ele1.Eta(), ele1.Phi(), 
      //          nextra_track);

      //    exclOK = exclOK && (nextra_track==0);
      }

      recoHMok   = ele_gsf_pt && ele_gsf_eta && diele.M()>4 && gsf_miss_hit && diele.Pt() < 2.0 && (acop(ele0.DeltaPhi(ele1)) < 0.01);  
      // recoHMok   = recoHMok && exclOK;

      // --- FILL HISTOS ---
      hGenAll.fill(genpt,genrap,genmass,gendphi);
      if (genok) hGenPass.fill(genpt,genrap,genmass,gendphi);


      if (recoGEDok) {
         hRecoGEDPass_gen.fill(genpt,genrap,genmass,gendphi);
         hRecoGEDPass_recoGED.fill(recoGEDpt,recoGEDrap,recoGEDmass,recoGEDdphi);
         if (genok) {
            hRecoGEDPassGenPass_gen.fill(genpt,genrap,genmass,gendphi);
            hRecoGEDPassGenPass_recoGED.fill(recoGEDpt,recoGEDrap,recoGEDmass,recoGEDdphi);
         }

         if (SingleEG5ok) {
            hRecoGEDPassTrigSingle_gen.fill(genpt,genrap,genmass,gendphi);
            hRecoGEDPassTrigSingle_recoGED.fill(recoGEDpt,recoGEDrap,recoGEDmass,recoGEDdphi);
            if (genok) {
               hRecoGEDPassTrigSingleGenPassTrigSingle_gen.fill(genpt,genrap,genmass,gendphi);
               hRecoGEDPassTrigSingleGenPassTrigSingle_recoGED.fill(recoGEDpt,recoGEDrap,recoGEDmass,recoGEDdphi);
            }
         }

         if (DoubleEG2ok) {
            hRecoGEDPassTrigDouble_gen.fill(genpt,genrap,genmass,gendphi);
            hRecoGEDPassTrigDouble_recoGED.fill(recoGEDpt,recoGEDrap,recoGEDmass,recoGEDdphi);
            if (genok) {
               hRecoGEDPassTrigDoubleGenPassTrigDouble_gen.fill(genpt,genrap,genmass,gendphi);
               hRecoGEDPassTrigDoubleGenPassTrigDouble_recoGED.fill(recoGEDpt,recoGEDrap,recoGEDmass,recoGEDdphi);
            }
         }
      } // if recoGEDok


      if (recoHMok) {
         hRecoHMPass_gen.fill(genpt,genrap,genmass,gendphi);
         hRecoHMPass_recoHM.fill(recoHMpt,recoHMrap,recoHMmass,recoHMdphi);
         if (genok) {
            hRecoHMPassGenPass_gen.fill(genpt,genrap,genmass,gendphi);
            hRecoHMPassGenPass_recoHM.fill(recoHMpt,recoHMrap,recoHMmass,recoHMdphi);
         }

         if (SingleEG5ok) {
            hRecoHMPassTrigSingle_gen.fill(genpt,genrap,genmass,gendphi);
            hRecoHMPassTrigSingle_recoHM.fill(recoHMpt,recoHMrap,recoHMmass,recoHMdphi);
            if (genok) {
               hRecoHMPassTrigSingleGenPassTrigSingle_gen.fill(genpt,genrap,genmass,gendphi);
               hRecoHMPassTrigSingleGenPassTrigSingle_recoHM.fill(recoHMpt,recoHMrap,recoHMmass,recoHMdphi);
            }
         }

         if (DoubleEG2ok) {
            hRecoHMPassTrigDouble_gen.fill(genpt,genrap,genmass,gendphi);
            hRecoHMPassTrigDouble_recoHM.fill(recoHMpt,recoHMrap,recoHMmass,recoHMdphi);
            if (genok) {
               hRecoHMPassTrigDoubleGenPassTrigDouble_gen.fill(genpt,genrap,genmass,gendphi);
               hRecoHMPassTrigDoubleGenPassTrigDouble_recoHM.fill(recoHMpt,recoHMrap,recoHMmass,recoHMdphi);
            }
         }
      } // if recoHMok
   } // event loop

   fout->Write();
   fout->Close();
}

void fillExclVars(const eventTreeReader &evtR, 
      double eleEta0, double elePhi0, double eleEta1, double elePhi1, 
      double &EmEnergy_EB, double &EmEnergy_EE, double &HadEnergy_HB, double &HadEnergy_HE, double &HadEnergy_HF_Plus, double &HadEnergy_HF_Minus) {
   EmEnergy_EB = 0;
   EmEnergy_EE = 0;
   HadEnergy_HB = 0;
   HadEnergy_HE = 0;
   HadEnergy_HF_Plus = 0;
   HadEnergy_HF_Minus = 0;

   for(int k = 0; k< evtR.nTower; ++k){

      if (fabs(evtR.CaloTower_eta->at(k)) < 1.479){ //start filling EM tower in barel region  
         if (fabs(TVector2::Phi_mpi_pi(evtR.CaloTower_phi->at(k)-elePhi0))<0.7 && fabs(evtR.CaloTower_eta->at(k)-eleEta0)<0.15) continue;
         if (fabs(TVector2::Phi_mpi_pi(evtR.CaloTower_phi->at(k)-elePhi1))<0.7 && fabs(evtR.CaloTower_eta->at(k)-eleEta1)<0.15) continue;
         if (evtR.CaloTower_emE->at(k) > EmEnergy_EB){                         
            EmEnergy_EB = evtR.CaloTower_emE->at(k) ;
         }
      }

      if (fabs(evtR.CaloTower_eta->at(k)) > 1.479 && fabs(evtR.CaloTower_eta->at(k)) < 3 ){           
         if (fabs(TVector2::Phi_mpi_pi(evtR.CaloTower_phi->at(k)-elePhi0))<0.7 && fabs(evtR.CaloTower_eta->at(k)-eleEta0)<0.15) continue;
         if (fabs(TVector2::Phi_mpi_pi(evtR.CaloTower_phi->at(k)-elePhi1))<0.7 && fabs(evtR.CaloTower_eta->at(k)-eleEta1)<0.15) continue;
         if (evtR.CaloTower_emE->at(k) > EmEnergy_EE){                         
            EmEnergy_EE = evtR.CaloTower_emE->at(k) ;
         }

      }

      if (fabs(evtR.CaloTower_eta->at(k)) < 1.305){           
         if (evtR.CaloTower_hadE->at(k) > HadEnergy_HB){                         
            HadEnergy_HB = evtR.CaloTower_hadE->at(k) ;
         }
      }

      if (fabs(evtR.CaloTower_eta->at(k)) > 1.305 && fabs(evtR.CaloTower_eta->at(k)) < 3 ){           
         if (evtR.CaloTower_hadE->at(k) > HadEnergy_HE){                         
            HadEnergy_HE = evtR.CaloTower_hadE->at(k) ;
         }

      }

      if (evtR.CaloTower_eta->at(k) > 2.9 && evtR.CaloTower_eta->at(k) < 5.2 ){           
         if (evtR.CaloTower_e->at(k) > HadEnergy_HF_Plus){                         
            HadEnergy_HF_Plus = evtR.CaloTower_e->at(k) ;
         }
      }


      if (evtR.CaloTower_eta->at(k) < -2.9 && evtR.CaloTower_eta->at(k) > -5.2 ){           
         if (evtR.CaloTower_e->at(k) > HadEnergy_HF_Minus){                         
            HadEnergy_HF_Minus = evtR.CaloTower_e->at(k) ;
         }
      }

   } //ntower
}

void fillNextraTracks(const eventTreeReader &evtR, 
      double eleEta0, double elePhi0, double eleEta1, double elePhi1, 
      int &nextratracks) {
   nextratracks=0;
   for(int kk = 0; kk < evtR.ngenTrk; ++kk){


      if (fabs(TVector2::Phi_mpi_pi(evtR.gentrkPhi->at(kk)-elePhi0))<0.7 && fabs(evtR.gentrkEta->at(kk)-eleEta0)<0.15) continue;
      if (fabs(TVector2::Phi_mpi_pi(evtR.gentrkPhi->at(kk)-elePhi1))<0.7 && fabs(evtR.gentrkEta->at(kk)-eleEta1)<0.15) continue;

      //allEmEnergy_EB.push_back(CaloTower_emE->at(k));

      /*if (CaloTower_emE->at(k) > EmEnergy_EB){                         
        EmEnergy_EB = CaloTower_emE->at(k) ;
        }*/
      nextratracks++;

   }
}
