#define MC // comment this line to run on data

#ifdef MC
#include "eventTreeReader.C"
#include "hltTreeReader.C"
#include "pixelTreeReader.C"
#define eventTreeReader eventTreeReader
#define hltTreeReader   hltTreeReader
#define pixelTreeReader pixelTreeReader
#else
#include "eventTreeReaderData.C"
#include "hltTreeReaderData.C"
#include "pixelTreeReaderData.C"
#define eventTreeReader eventTreeReaderData
#define hltTreeReader   hltTreeReaderData
#define pixelTreeReader pixelTreeReaderData
#endif
#include "TH1F.h"
#include "TLorentzVector.h"
#include <iostream>

using namespace std;

const double eleMass = 0.5109989461e-3;
const double DRmax   = 1.;

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

void fillExclVars(const eventTreeReader &evtR, 
      double eleEta0, double elePhi0, double eleEta1, double elePhi1, 
      double &EmEnergy_EB, double &EmEnergy_EE, double &HadEnergy_HB, double &HadEnergy_HE, double &HadEnergy_HF_Plus, double &HadEnergy_HF_Minus);
void fillNextraTracks(const eventTreeReader &evtR, 
      double eleEta0, double elePhi0, double eleEta1, double elePhi1, 
      int &nextratracks);

void fillHistosTrD(int is=0) {
   TChain *tchHLT = new TChain("hltanalysis/HltTree");
   TChain *tchEvt = new TChain("ggHiNtuplizer/EventTree");
   TChain *tchPix = new TChain("pixel/PixelTree");

#ifdef MC
   // tchHLT->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/hiforest_noptcut_3-53_invmass_more_var3/000%d/hiForest_qedee_*.root",i));
   // tchEvt->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/hiforest_noptcut_3-53_invmass_more_var3/000%d/hiForest_qedee_*.root",i));
   // tchPix->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/hiforest_noptcut_3-53_invmass_more_var3/000%d/hiForest_qedee_*.root",i));
   tchHLT->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/0%d/hiForest_qedee_*.root",is));
   tchEvt->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/0%d/hiForest_qedee_*.root",is));
   tchPix->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/0%d/hiForest_qedee_*.root",is));
#else
   tchHLT->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/%02d/*root",is));
   tchEvt->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/%02d/*root",is));
   tchPix->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/%02d/*root",is));
#endif
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
   evtR.fChain->SetBranchStatus("eleSCEn",1);
   evtR.fChain->SetBranchStatus("eleSCEta",1);
   evtR.fChain->SetBranchStatus("eleSCPhi",1);

   // HM
   evtR.fChain->SetBranchStatus("ngsfEle",1);
   evtR.fChain->SetBranchStatus("elegsf*",1);
   evtR.fChain->SetBranchStatus("nsc_*",1);
   evtR.fChain->SetBranchStatus("sc_*",1);

   hltR.fChain->SetBranchStatus("HLT_HIUPCL1SingleEG5NotHF2_v1",1);
   hltR.fChain->SetBranchStatus("HLT_HIUPCL1SingleEG5NotHF2_v1_Prescl",1);
   hltR.fChain->SetBranchStatus("HLT_HIUPCL1DoubleEG2NotHF2_v1",1);
   hltR.fChain->SetBranchStatus("HLT_HIUPCL1DoubleEG2NotHF2_v1_Prescl",1);
   hltR.fChain->SetBranchStatus("NL1IsolEm",1);
   hltR.fChain->SetBranchStatus("L1IsolEm*",1);
   hltR.fChain->SetBranchStatus("NL1NIsolEm",1);
   hltR.fChain->SetBranchStatus("L1NIsolEm*",1);
   pixR.fChain->SetBranchStatus("nEv",1); 
   pixR.fChain->SetBranchStatus("nhits*",1); 

   if (evtR.fChain == 0) return;
   if (pixR.fChain == 0) return;
   if (hltR.fChain == 0) return;

   Long64_t nentries = evtR.fChain->GetEntriesFast();
   Long64_t nentries2 = hltR.fChain->GetEntriesFast();
   Long64_t nentries3 = pixR.fChain->GetEntriesFast();

   if (nentries != nentries2 || nentries != nentries3) return;

#ifdef MC
   TFile *fout = new TFile(Form("outputMC_%d.root",is),"RECREATE");
#else
   TFile *fout = new TFile(Form("outputData_%d.root",is),"RECREATE");
#endif
   
   clHist hRecoGEDPassTrigSingle_recoGED("recoGEDpassTrigSingle_recoGED");
   clHist hRecoHMPassTrigSingle_recoHM("recoHMpassTrigSingle_recoHM");

   // min DR
   TH1F *hMinDR_GED = new TH1F("hMinDR_GED","minDR_GEd",100,0,2.5);
   TH1F *hMinDR_HM = new TH1F("hMinDR_HM","minDR_GEd",100,0,2.5);

   // acop histos
   TH2F *hacop_all_EB_GED = new TH2F("hacop_all_EB_GED","hacop_all_EB_GED",40,0,20,30,0,0.06);
   TH2F *hacop_all_EE_GED = new TH2F("hacop_all_EE_GED","hacop_all_EE_GED",40,0,20,30,0,0.06);
   TH2F *hacop_pass_EB_GED = new TH2F("hacop_pass_EB_GED","hacop_pass_EB_GED",40,0,20,30,0,0.06);
   TH2F *hacop_pass_EE_GED = new TH2F("hacop_pass_EE_GED","hacop_pass_EE_GED",40,0,20,30,0,0.06);

   TH2F *hacop_all_EB_HM = new TH2F("hacop_all_EB_HM","hacop_all_EB_HM",40,0,20,30,0,0.06);
   TH2F *hacop_all_EE_HM = new TH2F("hacop_all_EE_HM","hacop_all_EE_HM",40,0,20,30,0,0.06);
   TH2F *hacop_pass_EB_HM = new TH2F("hacop_pass_EB_HM","hacop_pass_EB_HM",40,0,20,30,0,0.06);
   TH2F *hacop_pass_EE_HM = new TH2F("hacop_pass_EE_HM","hacop_pass_EE_HM",40,0,20,30,0,0.06);

   // tree for tnp
   TDirectory *tdirGED = fout->mkdir("trigTree_GED");
   tdirGED->cd();
   TTree *trGED = new TTree("fitter_tree","");
   fout->cd();
   TDirectory *tdirHM = fout->mkdir("trigTree_HM");
   tdirHM->cd();
   TTree *trHM = new TTree("fitter_tree","");
   // tag variables
   float tag_pt, tag_eta, tag_abseta, tag_phi, tag_mindr, tag_EtMindr;
   // probe varialbles
   float pt, eta, abseta, phi, mindr, EtMindr;
   float SCE, SCEt, SCEta, SCPhi;
   // diele variables
   float diPt, diRap, diPhi, diMass, diAcop;
   // flags
   int doubleEG2, matchedOK;
   // book variables
   trGED->Branch("tag_pt",&tag_pt,"tag_pt/F");
   trGED->Branch("tag_eta",&tag_eta,"tag_eta/F");
   trGED->Branch("tag_abseta",&tag_abseta,"tag_abseta/F");
   trGED->Branch("tag_phi",&tag_phi,"tag_phi/F");
   trGED->Branch("tag_mindr",&tag_mindr,"tag_mindr/F");
   trGED->Branch("tag_EtMindr",&tag_EtMindr,"tag_EtMindr/F");
   trGED->Branch("pt",&pt,"pt/F");
   trGED->Branch("eta",&eta,"eta/F");
   trGED->Branch("abseta",&abseta,"abseta/F");
   trGED->Branch("phi",&phi,"phi/F");
   trGED->Branch("mindr",&mindr,"mindr/F");
   trGED->Branch("EtMindr",&EtMindr,"EtMindr/F");
   trGED->Branch("SCE",&SCE,"SCE/F");
   trGED->Branch("SCEt",&SCEt,"SCEt/F");
   trGED->Branch("SCEta",&SCEta,"SCEta/F");
   trGED->Branch("SCPhi",&SCPhi,"SCPhi/F");
   trGED->Branch("diPt",&diPt,"diPt/F");
   trGED->Branch("diRap",&diRap,"diRap/F");
   trGED->Branch("diPhi",&diPhi,"diPhi/F");
   trGED->Branch("diMass",&diMass,"diMass/F");
   trGED->Branch("diAcop",&diAcop,"diAcop/F");
   trGED->Branch("doubleEG2",&doubleEG2,"doubleEG2/I");
   trGED->Branch("matchedOK",&matchedOK,"matchedOK/I");
   trHM->Branch("tag_pt",&tag_pt,"tag_pt/F");
   trHM->Branch("tag_eta",&tag_eta,"tag_eta/F");
   trHM->Branch("tag_abseta",&tag_abseta,"tag_abseta/F");
   trHM->Branch("tag_phi",&tag_phi,"tag_phi/F");
   trHM->Branch("tag_mindr",&tag_mindr,"tag_mindr/F");
   trHM->Branch("tag_EtMindr",&tag_EtMindr,"tag_EtMindr/F");
   trHM->Branch("pt",&pt,"pt/F");
   trHM->Branch("eta",&eta,"eta/F");
   trHM->Branch("abseta",&abseta,"abseta/F");
   trHM->Branch("phi",&phi,"phi/F");
   trHM->Branch("mindr",&mindr,"mindr/F");
   trHM->Branch("EtMindr",&EtMindr,"EtMindr/F");
   // trHM->Branch("SCE",&SCE,"SCE/F");
   // trHM->Branch("SCEt",&SCEt,"SCEt/F");
   // trHM->Branch("SCEta",&SCEta,"SCEta/F");
   // trHM->Branch("SCPhi",&SCPhi,"SCPhi/F");
   trHM->Branch("diPt",&diPt,"diPt/F");
   trHM->Branch("diRap",&diRap,"diRap/F");
   trHM->Branch("diPhi",&diPhi,"diPhi/F");
   trHM->Branch("diMass",&diMass,"diMass/F");
   trHM->Branch("diAcop",&diAcop,"diAcop/F");
   trHM->Branch("doubleEG2",&doubleEG2,"doubleEG2/I");
   trHM->Branch("matchedOK",&matchedOK,"matchedOK/I");

   int nnum=0, nden=0;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      // if (jentry>100000) break;
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

      bool recoGEDok;
      bool recoHMok;
      double recoGEDpt, recoGEDrap, recoGEDmass, recoGEDdphi;
      double recoHMpt, recoHMrap, recoHMmass, recoHMdphi;
      // bool exclOK   =  false;
      bool SingleEG5ok = true; //hltR.HLT_HIUPCL1SingleEG5NotHF2_v1;
      bool DoubleEG2ok = hltR.HLT_HIUPCL1DoubleEG2NotHF2_v1;


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
      TLorentzVector ele0GED, ele1GED, dieleGED;
      if (ele_two) {
         ele0GED.SetPtEtaPhiM(evtR.elePt->at(0),evtR.eleEta->at(0),evtR.elePhi->at(0),eleMass);
         ele1GED.SetPtEtaPhiM(evtR.elePt->at(1),evtR.eleEta->at(1),evtR.elePhi->at(1),eleMass);
         dieleGED = ele0GED+ele1GED;
         recoGEDpt = dieleGED.Pt();
         recoGEDrap = dieleGED.Rapidity();
         recoGEDmass = dieleGED.M();
         recoGEDdphi = ele0GED.DeltaPhi(ele1GED);

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
      recoGEDok    =  ele_pt && ele_eta && ele_SCeta && opp_chrg  && miss_hit && iso
         && (recoGEDpt < 2.0) && (fabs(recoGEDrap)<2.5);// && (acop(ele0GED.DeltaPhi(ele1GED)) < 0.01);
      // recoGEDok = recoGEDok && exclOK;

      // --- HM cuts ---
      bool eleHM_two   =  (evtR.ngsfEle==2 );
      bool no_HMpho    =  (evtR.nHyPho==0);
      bool ele_gsf_chg =  eleHM_two ? ( evtR.elegsfTrkCharge->at(0) != evtR.elegsfTrkCharge->at(1)) : false;
      bool ele_gsf_pt  =  eleHM_two ? ( evtR.elegsfTrkPt->at(0) > 2 && evtR.elegsfTrkPt->at(1) > 2 ) : false;
      bool ele_gsf_eta  =  eleHM_two ? ( fabs(evtR.elegsfTrkEta->at(0)) < 2.5 && fabs(evtR.elegsfTrkEta->at(1)) < 2.5 ) : false;
      bool gsf_miss_hit=  eleHM_two ? ( evtR.elegsfTrkMissHits->at(0) <= 1 && evtR.elegsfTrkMissHits->at(1) <= 1) : false;

      // exclOK=false;
      TLorentzVector ele0HM, ele1HM, dieleHM;
      if (eleHM_two) {
         ele0HM.SetPtEtaPhiM(evtR.elegsfTrkPt->at(0),evtR.elegsfTrkEta->at(0),evtR.elegsfTrkPhi->at(0),eleMass);
         ele1HM.SetPtEtaPhiM(evtR.elegsfTrkPt->at(1),evtR.elegsfTrkEta->at(1),evtR.elegsfTrkPhi->at(1),eleMass);
         dieleHM = ele0HM+ele1HM;
         recoHMpt = dieleHM.Pt();
         recoHMrap = dieleHM.Rapidity();
         recoHMmass = dieleHM.M();
         recoHMdphi = ele0HM.DeltaPhi(ele1HM);

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

      recoHMok   = ele_gsf_pt && ele_gsf_eta && ele_gsf_chg && dieleHM.M()>4 && gsf_miss_hit && dieleHM.Pt() < 2.0;// && (acop(ele0HM.DeltaPhi(ele1HM)) < 0.01);  
      // recoHMok   = recoHMok && exclOK;

      // --- FILL HISTOS ---

      if (recoGEDok) nden++;
      if (recoGEDok && DoubleEG2ok) nnum++;

      if (recoGEDok) {
         if (SingleEG5ok) {
            hRecoGEDPassTrigSingle_recoGED.fill(recoGEDpt,recoGEDrap,recoGEDmass,recoGEDdphi);

            double minDR0 = 999., minDR1 = 999.;
            double EtMinDR0=0, EtMinDR1=0;

            // L1-offline matching
            for (int i=0; i<hltR.NL1IsolEm; i++) {
               if (hltR.L1IsolEmEt[i]<=0) continue;
               TLorentzVector tlv; tlv.SetPtEtaPhiM(hltR.L1IsolEmEt[i],hltR.L1IsolEmEta[i],hltR.L1IsolEmPhi[i],0);
               double dr0 = tlv.DeltaR(ele0GED);
               double dr1 = tlv.DeltaR(ele1GED);
               if (dr0<minDR0) {
                  minDR0 = dr0;
                  EtMinDR0 = hltR.L1IsolEmEt[i];
               }
               if (dr1<minDR1) {
                  minDR1 = dr1;
                  EtMinDR1 = hltR.L1IsolEmEt[i];
               }
            }
            for (int i=0; i<hltR.NL1NIsolEm; i++) {
               if (hltR.L1NIsolEmEt[i]<=0) continue;
               TLorentzVector tlv; tlv.SetPtEtaPhiM(hltR.L1NIsolEmEt[i],hltR.L1NIsolEmEta[i],hltR.L1NIsolEmPhi[i],0);
               double dr0 = tlv.DeltaR(ele0GED);
               double dr1 = tlv.DeltaR(ele1GED);
               if (dr0<minDR0) {
                  minDR0 = dr0;
                  EtMinDR0 = hltR.L1NIsolEmEt[i];
               }
               if (dr1<minDR1) {
                  minDR1 = dr1;
                  EtMinDR1 = hltR.L1NIsolEmEt[i];
               }
            }

            hMinDR_GED->Fill(min(minDR0,minDR1));

            // fill tnp histos as well as variables for the tree
            diPt = dieleGED.Pt();
            diRap = dieleGED.Rapidity();
            diPhi = dieleGED.Phi();
            diMass = dieleGED.M();
            diAcop = acop(ele0GED.DeltaPhi(ele1GED));
            doubleEG2 = DoubleEG2ok;

            // is ele0 a tag?
            // if (minDR0<DRmax && EtMinDR0>=5) {
            if (true) {
               if (fabs(ele1GED.Eta())<1.5) hacop_all_EB_GED->Fill(ele1GED.Pt(),acop(ele0GED.DeltaPhi(ele1GED)));
               else hacop_all_EE_GED->Fill(ele1GED.Pt(),acop(ele0GED.DeltaPhi(ele1GED)));

               if (minDR1<DRmax && EtMinDR1>=2 && DoubleEG2ok) {
                  if (fabs(ele1GED.Eta())<1.5) hacop_pass_EB_GED->Fill(ele1GED.Pt(),acop(ele0GED.DeltaPhi(ele1GED)));
                  else hacop_pass_EE_GED->Fill(ele1GED.Pt(),acop(ele0GED.DeltaPhi(ele1GED)));
               }

               tag_pt = ele0GED.Pt();
               tag_eta = ele0GED.Eta();
               tag_abseta = ele0GED.Eta();
               tag_phi = ele0GED.Phi();
               tag_mindr = minDR0;
               tag_EtMindr = EtMinDR0;
               pt = ele1GED.Pt();
               eta = ele1GED.Eta();
               abseta = ele1GED.Eta();
               phi = ele1GED.Phi();
               mindr = minDR1;
               EtMindr = EtMinDR1;
               SCE = evtR.eleSCEn->at(1);
               SCEt = evtR.eleSCEn->at(1)*sin(2.*atan(exp(-evtR.eleSCEta->at(1))));
               SCEta = evtR.eleSCEta->at(1);
               SCPhi = evtR.eleSCPhi->at(1);
               matchedOK = minDR1<DRmax && EtMinDR1>=2;
               trGED->Fill();
            }

            // is ele1 a tag?
            // if (minDR1<DRmax && EtMinDR1>=5) {
            if (true) {
               if (fabs(ele0GED.Eta())<1.5) hacop_all_EB_GED->Fill(ele0GED.Pt(),acop(ele0GED.DeltaPhi(ele1GED)));
               else hacop_all_EE_GED->Fill(ele0GED.Pt(),acop(ele0GED.DeltaPhi(ele1GED)));

               if (minDR0<DRmax && EtMinDR0>=2 && DoubleEG2ok) {
                  if (fabs(ele0GED.Eta())<1.5) hacop_pass_EB_GED->Fill(ele0GED.Pt(),acop(ele0GED.DeltaPhi(ele1GED)));
                  else hacop_pass_EE_GED->Fill(ele0GED.Pt(),acop(ele0GED.DeltaPhi(ele1GED)));
               }

               tag_pt = ele1GED.Pt();
               tag_eta = ele1GED.Eta();
               tag_abseta = ele1GED.Eta();
               tag_phi = ele1GED.Phi();
               tag_mindr = minDR1;
               tag_EtMindr = EtMinDR1;
               pt = ele0GED.Pt();
               eta = ele0GED.Eta();
               abseta = ele0GED.Eta();
               phi = ele0GED.Phi();
               mindr = minDR0;
               EtMindr = EtMinDR0;
               SCE = evtR.eleSCEn->at(0);
               SCEt = evtR.eleSCEn->at(0)*sin(2.*atan(exp(-evtR.eleSCEta->at(0))));
               SCEta = evtR.eleSCEta->at(0);
               SCPhi = evtR.eleSCPhi->at(0);
               matchedOK = minDR0<DRmax && EtMinDR0>=2;
               trGED->Fill();
            }
         } // if SingleEG5
      } // if recoGEDok


      if (recoHMok) {
         if (SingleEG5ok) {
            hRecoHMPassTrigSingle_recoHM.fill(recoHMpt,recoHMrap,recoHMmass,recoHMdphi);

            double minDR0 = 999., minDR1 = 999.;
            double EtMinDR0=0, EtMinDR1=0;

            // L1-offline matching
            for (int i=0; i<hltR.NL1IsolEm; i++) {
               if (hltR.L1IsolEmEt[i]<=0) continue;
               TLorentzVector tlv; tlv.SetPtEtaPhiM(hltR.L1IsolEmEt[i],hltR.L1IsolEmEta[i],hltR.L1IsolEmPhi[i],0);
               double dr0 = tlv.DeltaR(ele0HM);
               double dr1 = tlv.DeltaR(ele1HM);
               if (dr0<minDR0) {
                  minDR0 = dr0;
                  EtMinDR0 = hltR.L1IsolEmEt[i];
               }
               if (dr1<minDR1) {
                  minDR1 = dr1;
                  EtMinDR1 = hltR.L1IsolEmEt[i];
               }
            }
            for (int i=0; i<hltR.NL1NIsolEm; i++) {
               if (hltR.L1NIsolEmEt[i]<=0) continue;
               TLorentzVector tlv; tlv.SetPtEtaPhiM(hltR.L1NIsolEmEt[i],hltR.L1NIsolEmEta[i],hltR.L1NIsolEmPhi[i],0);
               double dr0 = tlv.DeltaR(ele0HM);
               double dr1 = tlv.DeltaR(ele1HM);
               if (dr0<minDR0) {
                  minDR0 = dr0;
                  EtMinDR0 = hltR.L1NIsolEmEt[i];
               }
               if (dr1<minDR1) {
                  minDR1 = dr1;
                  EtMinDR1 = hltR.L1NIsolEmEt[i];
               }
            }

            hMinDR_HM->Fill(min(minDR0,minDR1));

            // fill tnp histos as well as variables for the tree
            diPt = dieleHM.Pt();
            diRap = dieleHM.Rapidity();
            diPhi = dieleHM.Phi();
            diMass = dieleHM.M();
            diAcop = acop(ele0HM.DeltaPhi(ele1HM));
            doubleEG2 = DoubleEG2ok;

            // is ele0 a tag?
            if (minDR0<DRmax && EtMinDR0>=5) {
               if (fabs(ele1HM.Eta())<1.5) hacop_all_EB_HM->Fill(ele1HM.Pt(),acop(ele0HM.DeltaPhi(ele1HM)));
               else hacop_all_EE_HM->Fill(ele1HM.Pt(),acop(ele0HM.DeltaPhi(ele1HM)));

               if (minDR1<DRmax && EtMinDR1>=2 && DoubleEG2ok) {
                  if (fabs(ele1HM.Eta())<1.5) hacop_pass_EB_HM->Fill(ele1HM.Pt(),acop(ele0HM.DeltaPhi(ele1HM)));
                  else hacop_pass_EE_HM->Fill(ele1HM.Pt(),acop(ele0HM.DeltaPhi(ele1HM)));
               }

               tag_pt = ele0HM.Pt();
               tag_eta = ele0HM.Eta();
               tag_abseta = ele0HM.Eta();
               tag_phi = ele0HM.Phi();
               tag_mindr = minDR0;
               tag_EtMindr = EtMinDR0;
               pt = ele1HM.Pt();
               eta = ele1HM.Eta();
               abseta = ele1HM.Eta();
               phi = ele1HM.Phi();
               mindr = minDR1;
               EtMindr = EtMinDR1;
               matchedOK = minDR1<DRmax && EtMinDR1>=2;
               trHM->Fill();
            }

            // is ele1 a tag?
            if (minDR1<DRmax && EtMinDR1>=5) {
               if (fabs(ele0HM.Eta())<1.5) hacop_all_EB_HM->Fill(ele0HM.Pt(),acop(ele0HM.DeltaPhi(ele1HM)));
               else hacop_all_EE_HM->Fill(ele0HM.Pt(),acop(ele0HM.DeltaPhi(ele1HM)));

               if (minDR0<DRmax && EtMinDR0>=2 && DoubleEG2ok) {
                  if (fabs(ele0HM.Eta())<1.5) hacop_all_EB_HM->Fill(ele0HM.Pt(),acop(ele0HM.DeltaPhi(ele1HM)));
                  else hacop_all_EE_HM->Fill(ele0HM.Pt(),acop(ele0HM.DeltaPhi(ele1HM)));
               }

               tag_pt = ele1HM.Pt();
               tag_eta = ele1HM.Eta();
               tag_abseta = ele1HM.Eta();
               tag_phi = ele1HM.Phi();
               tag_mindr = minDR1;
               tag_EtMindr = EtMinDR1;
               pt = ele0HM.Pt();
               eta = ele0HM.Eta();
               abseta = ele0HM.Eta();
               phi = ele0HM.Phi();
               mindr = minDR0;
               EtMindr = EtMinDR0;
               matchedOK = minDR0<DRmax && EtMinDR0>=2;
               trHM->Fill();
            }
         } // if SingleEG5
      } // if recoHMok
   } // event loop

   fout->Write();
   fout->Close();

   cout << nnum << "/" << nden << " = " << (double) nnum / (double) nden << endl;
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
