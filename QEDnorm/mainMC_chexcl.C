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

int ireg(double et, double eta);
double SF(double et, double eta);
double SF_uncert(double et, double eta);
double SF_uncert_diEG(double et1, double eta1, double et2, double eta2);

class clHist {
   public:
      TH1F *hpt;
      TH1F *hmass;
      TH1F *hrap;
      TH1F *haco;
      TH1F *hint;

      clHist(const char* name) {
         hpt = new TH1F(Form("hpt_%s",name),"",20,0,2);
         hmass = new TH1F(Form("hmass_%s",name),"",200,0,200);
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

void mainMC_chexcl(int idir=0) {
   TChain *tchHLT = new TChain("hltanalysis/HltTree");
   TChain *tchEvt = new TChain("ggHiNtuplizer/EventTree");
   TChain *tchPix = new TChain("pixel/PixelTree");

   tchHLT->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/0%d/hiForest_qedee_*.root",idir));
   tchEvt->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/0%d/hiForest_qedee_*.root",idir));
   tchPix->Add(Form("/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/0%d/hiForest_qedee_*.root",idir));
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
   evtR.fChain->SetBranchStatus("nVtx",1);
   evtR.fChain->SetBranchStatus("zVtx",1);
   evtR.fChain->SetBranchStatus("ngenTrk",1);
   evtR.fChain->SetBranchStatus("gentrk*",1);
   evtR.fChain->SetBranchStatus("nTower",1);
   evtR.fChain->SetBranchStatus("CaloTower*",1);

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
   // pixR.fChain->SetBranchStatus("r*",1); 
   pixR.fChain->SetBranchStatus("phi*",1); 

   if (evtR.fChain == 0) return;
   if (pixR.fChain == 0) return;
   if (hltR.fChain == 0) return;

   Long64_t nentries = evtR.fChain->GetEntriesFast();
   Long64_t nentries2 = hltR.fChain->GetEntriesFast();
   Long64_t nentries3 = pixR.fChain->GetEntriesFast();

   if (nentries != nentries2 || nentries != nentries3) return;

   TFile *fout = new TFile(Form("output_%d.root",idir),"RECREATE");
   clHist hGenAll("genall");
   clHist hGenPass("genpass");

   // plots for charged exclusivity
   TH1F *hextratracksGED = new TH1F("hextratracksGED","hextratracksGED",10,0,10);
   TH1F *hextratracksHM = new TH1F("hextratracksHM","hextratracksHM",10,0,10);
   TH1F *hextratracksGED_lacop = new TH1F("hextratracksGED_lacop","hextratracksGED_lacop",10,0,10);
   TH1F *hextratracksHM_lacop = new TH1F("hextratracksHM_lacop","hextratracksHM_lacop",10,0,10);
   TH1F *hextratracksGED_hacop = new TH1F("hextratracksGED_hacop","hextratracksGED_hacop",10,0,10);
   TH1F *hextratracksHM_hacop = new TH1F("hextratracksHM_hacop","hextratracksHM_hacop",10,0,10);
   TH1F *hnhits1 = new TH1F("hnhits1","hnhits1",10,0,10);
   TH1F *hnhits1_lacop = new TH1F("hnhits1_lacop","hnhits1_lacop",10,0,10);
   TH1F *hnhits1_hacop = new TH1F("hnhits1_hacop","hnhits1_hacop",10,0,10);
   TH1F *hnhits2 = new TH1F("hnhits2","hnhits2",10,0,10);
   TH1F *hnhits2_lacop = new TH1F("hnhits2_lacop","hnhits2_lacop",10,0,10);
   TH1F *hnhits2_hacop = new TH1F("hnhits2_hacop","hnhits2_hacop",10,0,10);
   TH1F *hnhits3 = new TH1F("hnhits3","hnhits3",10,0,10);
   TH1F *hnhits3_lacop = new TH1F("hnhits3_lacop","hnhits3_lacop",10,0,10);
   TH1F *hnhits3_hacop = new TH1F("hnhits3_hacop","hnhits3_hacop",10,0,10);
   TH1F *hnhits4 = new TH1F("hnhits4","hnhits4",10,0,10);
   TH1F *hnhits4_lacop = new TH1F("hnhits4_lacop","hnhits4_lacop",10,0,10);
   TH1F *hnhits4_hacop = new TH1F("hnhits4_hacop","hnhits4_hacop",10,0,10);
   TH1F *hnhits5 = new TH1F("hnhits5","hnhits5",10,0,10);
   TH1F *hnhits5_lacop = new TH1F("hnhits5_lacop","hnhits5_lacop",10,0,10);
   TH1F *hnhits5_hacop = new TH1F("hnhits5_hacop","hnhits5_hacop",10,0,10);
   
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

   // output TTree's
   float acopGED, acopHM;
   float massGED, massHM;
   float rapGED, rapHM;
   float ptGED, ptHM;
   float deltaptGED, deltaptHM;
   float eleGEDpt1, eleGEDeta1, eleGEDphi1, eleGEDpt2, eleGEDeta2, eleGEDphi2;
   float eleGEDSCEt1, eleGEDSCEt2, eleGEDSCEta1, eleGEDSCEta2, eleGEDSCPhi1, eleGEDSCPhi2;
   float eleHMpt1, eleHMeta1, eleHMphi1, eleHMpt2, eleHMeta2, eleHMphi2;
   int nextra_track_GED, nextra_track_HM;
   int nhits1, nhits2, nhits3, nhits4, nhits5;
   int doubleEG2okI, exclOKI;
   float SFweight[14];
   TTree *trGED = new TTree("trGED","tree for GED");
   TTree *trHM = new TTree("trHM","tree for HM");
   trGED->Branch("acop",&acopGED,"acop/F");
   trHM->Branch("acop",&acopHM,"acop/F");
   trGED->Branch("mass",&massGED,"mass/F");
   trHM->Branch("mass",&massHM,"mass/F");
   trGED->Branch("rap",&rapGED,"rap/F");
   trHM->Branch("rap",&rapHM,"rap/F");
   trGED->Branch("pt",&ptGED,"pt/F");
   trHM->Branch("pt",&ptHM,"pt/F");
   trGED->Branch("deltapt",&deltaptGED,"deltapt/F");
   trHM->Branch("deltapt",&deltaptHM,"deltapt/F");
   trGED->Branch("nextra_track",&nextra_track_GED,"nextra_track/I");
   trHM->Branch("nextra_track",&nextra_track_HM,"nextra_track/I");
   trGED->Branch("nhits1",&nhits1,"nhits1/I");
   trGED->Branch("nhits2",&nhits2,"nhits2/I");
   trGED->Branch("nhits3",&nhits3,"nhits3/I");
   trGED->Branch("nhits4",&nhits4,"nhits4/I");
   trGED->Branch("nhits5",&nhits5,"nhits5/I");
   trHM->Branch("nhits1",&nhits1,"nhits1/I");
   trHM->Branch("nhits2",&nhits2,"nhits2/I");
   trHM->Branch("nhits3",&nhits3,"nhits3/I");
   trHM->Branch("nhits4",&nhits4,"nhits4/I");
   trHM->Branch("nhits5",&nhits5,"nhits5/I");
   trGED->Branch("doubleEG2",&doubleEG2okI,"doubleEG2/I");
   trHM->Branch("doubleEG2",&doubleEG2okI,"doubleEG2/I");
   trGED->Branch("exclOK",&exclOKI,"exclOK/I");
   trHM->Branch("exclOK",&exclOKI,"exclOK/I");
   trGED->Branch("elept1",&eleGEDpt1,"elept1/F");
   trGED->Branch("eleeta1",&eleGEDeta1,"eleeta1/F");
   trGED->Branch("elephi1",&eleGEDphi1,"elephi1/F");
   trGED->Branch("elept2",&eleGEDpt2,"elept2/F");
   trGED->Branch("eleeta2",&eleGEDeta2,"eleeta2/F");
   trGED->Branch("elephi2",&eleGEDphi2,"elephi2/F");
   trGED->Branch("eleSCEt1",&eleGEDSCEt1,"eleSCEt1/F");
   trGED->Branch("eleSCEta1",&eleGEDSCEta1,"eleSCEta1/F");
   trGED->Branch("eleSCPhi1",&eleGEDSCPhi1,"eleSCPhi1/F");
   trGED->Branch("eleSCEt2",&eleGEDSCEt2,"eleSCEt2/F");
   trGED->Branch("eleSCEta2",&eleGEDSCEta2,"eleSCEta2/F");
   trGED->Branch("eleSCPhi2",&eleGEDSCPhi2,"eleSCPhi2/F");
   trHM->Branch("elept1",&eleHMpt1,"elept1/F");
   trHM->Branch("eleeta1",&eleHMeta1,"eleeta1/F");
   trHM->Branch("elephi1",&eleHMphi1,"elephi1/F");
   trHM->Branch("elept2",&eleHMpt2,"elept2/F");
   trHM->Branch("eleeta2",&eleHMeta2,"eleeta2/F");
   trHM->Branch("elephi2",&eleHMphi2,"elephi2/F");
   trGED->Branch("SFweight",SFweight,"SFweight[14]/F");

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      // if (jentry>500000) break;
      if (jentry%100000==0) cout << "--> " << jentry << "/" << nentries << endl;
      Long64_t ientry_evt = evtR.LoadTree(jentry);
      if (ientry_evt < 0) break;
      Long64_t ientry_pix = pixR.LoadTree(jentry);
      if (ientry_pix < 0) break;
      Long64_t ientry_hlt = hltR.LoadTree(jentry);
      if (ientry_hlt < 0) break;

      nb = evtR.fChain->GetEntry(jentry);   nbytes += nb;
      nb = hltR.fChain->GetEntry(jentry);   nbytes += nb;
      nb = pixR.fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      bool genok;
      bool recoGEDok;
      bool recoHMok;
      double genpt, genrap, genmass, gendphi;
      double recoGEDpt, recoGEDrap, recoGEDmass, recoGEDdphi;
      double recoHMpt, recoHMrap, recoHMmass, recoHMdphi;
      bool exclOK   =  false;
      bool SingleEG5ok = hltR.HLT_HIUPCL1SingleEG5NotHF2_v1;
      bool DoubleEG2ok = hltR.HLT_HIUPCL1DoubleEG2NotHF2_v1;
      doubleEG2okI = DoubleEG2ok;

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
            &&fabs(evtR.mcEta->at(0))<2.4&&fabs(evtR.mcEta->at(1))<2.4
            &&fabs(genrap)<2.4&&genmass>4&&genmass<53
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
      bool ele_eta   =  ele_two ? (fabs(evtR.eleEta->at(0))<2.4 && fabs(evtR.eleEta->at(1))<2.4 ) : false;
      bool ele_SCeta   =  ele_two ? (fabs(evtR.eleSCEta->at(0))<2.4 && fabs(evtR.eleSCEta->at(1))<2.4
            && (fabs(evtR.eleSCEta->at(0)) < 1.4442 || fabs(evtR.eleSCEta->at(0)) > 1.566)
            && (fabs(evtR.eleSCEta->at(1)) < 1.4442 || fabs(evtR.eleSCEta->at(1)) > 1.566)) : false;
      bool iso       =  (hovere && track_iso && ecal_iso && hcal_iso);
      nextra_track_GED=0;
      TLorentzVector ele0, ele1, diele;
      if (ele_two) {
         ele0.SetPtEtaPhiM(evtR.elePt->at(0),evtR.eleEta->at(0),evtR.elePhi->at(0),eleMass);
         ele1.SetPtEtaPhiM(evtR.elePt->at(1),evtR.eleEta->at(1),evtR.elePhi->at(1),eleMass);
         diele = ele0+ele1;
         recoGEDpt = diele.Pt();
         recoGEDrap = diele.Rapidity();
         recoGEDmass = diele.M();
         recoGEDdphi = ele0.DeltaPhi(ele1);

         eleGEDpt1 = ele0.Pt();
         eleGEDeta1 = ele0.Eta();
         eleGEDphi1 = ele0.Phi();
         eleGEDpt2 = ele1.Pt();
         eleGEDeta2 = ele1.Eta();
         eleGEDphi2 = ele1.Phi();

         eleGEDSCEt1 = evtR.eleSCEn->at(0)/cosh(evtR.eleSCEta->at(0));
         eleGEDSCEta1 = evtR.eleSCEta->at(0);
         eleGEDSCPhi1 = evtR.eleSCPhi->at(0);
         eleGEDSCEt2 = evtR.eleSCEn->at(1)/cosh(evtR.eleSCEta->at(1));
         eleGEDSCEta2 = evtR.eleSCEta->at(1);
         eleGEDSCPhi2 = evtR.eleSCPhi->at(1);

         double sf1 = SF(eleGEDSCEt1,eleGEDSCEta1);
         double sf2 = SF(eleGEDSCEt2,eleGEDSCEta2);
         SFweight[0] = sf1*sf2;

         for (int ivar=1; ivar<14; ivar++) {
            double sf1p=sf1, sf2p=sf2;
            if (ireg(eleGEDSCEt1,eleGEDSCEta1)==ivar) sf1p += SF_uncert(eleGEDSCEt1,eleGEDSCEta1);
            if (ireg(eleGEDSCEt2,eleGEDSCEta2)==ivar) sf2p += SF_uncert(eleGEDSCEt2,eleGEDSCEta2);
            SFweight[ivar] = sf1p*sf2p;
         }

         double EmEnergy_EB = 0;
         double EmEnergy_EE = 0;
         double HadEnergy_HB = 0;
         double HadEnergy_HE = 0;
         double HadEnergy_HF_Plus = 0;
         double HadEnergy_HF_Minus = 0;
      //    evtR.b_nTower->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_eta->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_phi->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_emE->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_hadE->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_e->GetEntry(ientry_evt);
         fillExclVars(evtR, 
               evtR.eleEta->at(0), evtR.elePhi->at(0), evtR.eleEta->at(1), evtR.elePhi->at(1), 
               EmEnergy_EB, EmEnergy_EE, HadEnergy_HB, HadEnergy_HE, HadEnergy_HF_Plus, HadEnergy_HF_Minus);
         exclOK = (EmEnergy_EB< 0.55 && EmEnergy_EE < 3.16 && HadEnergy_HB < 2.0 && HadEnergy_HE < 3.0 && HadEnergy_HF_Plus < 4.85 && HadEnergy_HF_Minus < 4.12);

      //    evtR.b_ngenTrk->GetEntry(ientry_evt);
      //    evtR.b_gentrkPt->GetEntry(ientry_evt);
      //    evtR.b_gentrkEta->GetEntry(ientry_evt);
      //    evtR.b_gentrkPhi->GetEntry(ientry_evt);
         fillNextraTracks(evtR, 
               evtR.eleEta->at(0), evtR.elePhi->at(0), evtR.eleEta->at(1), evtR.elePhi->at(1), 
               nextra_track_GED);

         exclOK = exclOK && (nextra_track_GED==0);
         exclOKI = exclOK;
      }
      recoGEDok    =  ele_pt && ele_eta && ele_SCeta && opp_chrg && (recoGEDmass>4) && miss_hit && iso 
         && (recoGEDpt < 2.0) && (fabs(recoGEDrap)<2.4);// && (acop(ele0.DeltaPhi(ele1)) < 0.01);
      // recoGEDok = recoGEDok && exclOK;
      acopGED = acop(ele0.DeltaPhi(ele1));
      deltaptGED = fabs(ele0.Pt()-ele1.Pt());
      ptGED = diele.Pt();
      rapGED = diele.Rapidity();
      massGED = diele.M();

      // --- HM cuts ---
      bool eleHM_two   =  (evtR.ngsfEle==2 );
      bool no_HMpho    =  (evtR.nHyPho==0);
      bool ele_gsf_chg =  eleHM_two ? ( evtR.elegsfTrkCharge->at(0) != evtR.elegsfTrkCharge->at(1)) : false;
      bool ele_gsf_pt  =  eleHM_two ? ( evtR.elegsfTrkPt->at(0) > 2 && evtR.elegsfTrkPt->at(1) > 2 ) : false;
      bool ele_gsf_eta  =  eleHM_two ? ( fabs(evtR.elegsfTrkEta->at(0)) < 2.4 && fabs(evtR.elegsfTrkEta->at(1)) < 2.4 ) : false;
      bool gsf_miss_hit=  eleHM_two ? ( evtR.elegsfTrkMissHits->at(0) <= 1 && evtR.elegsfTrkMissHits->at(1) <= 1) : false;
      nextra_track_HM=0;

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

         eleHMpt1 = ele0.Pt();
         eleHMeta1 = ele0.Eta();
         eleHMphi1 = ele0.Phi();
         eleHMpt2 = ele1.Pt();
         eleHMeta2 = ele1.Eta();
         eleHMphi2 = ele1.Phi();

         double EmEnergy_EB = 0;
         double EmEnergy_EE = 0;
         double HadEnergy_HB = 0;
         double HadEnergy_HE = 0;
         double HadEnergy_HF_Plus = 0;
         double HadEnergy_HF_Minus = 0;
      //    evtR.b_nTower->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_eta->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_phi->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_emE->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_hadE->GetEntry(ientry_evt);
      //    evtR.b_CaloTower_e->GetEntry(ientry_evt);
         fillExclVars(evtR, 
               ele0.Eta(), ele0.Phi(), ele1.Eta(), ele1.Phi(), 
               EmEnergy_EB, EmEnergy_EE, HadEnergy_HB, HadEnergy_HE, HadEnergy_HF_Plus, HadEnergy_HF_Minus);
         exclOK = (EmEnergy_EB< 0.55 && EmEnergy_EE < 3.16 && HadEnergy_HB < 2.0 && HadEnergy_HE < 3.0 && HadEnergy_HF_Plus < 4.85 && HadEnergy_HF_Minus < 4.12);

      //    evtR.b_ngenTrk->GetEntry(ientry_evt);
      //    evtR.b_gentrkPt->GetEntry(ientry_evt);
      //    evtR.b_gentrkEta->GetEntry(ientry_evt);
      //    evtR.b_gentrkPhi->GetEntry(ientry_evt);
         fillNextraTracks(evtR, 
               ele0.Eta(), ele0.Phi(), ele1.Eta(), ele1.Phi(), 
               nextra_track_HM);

         exclOK = exclOK && (nextra_track_HM==0);
         // exclOKI = exclOK;
      }

      recoHMok   = ele_gsf_pt && ele_gsf_eta && ele_gsf_chg && diele.M()>4 && gsf_miss_hit && diele.Pt() < 2.0;// && (acop(ele0.DeltaPhi(ele1)) < 0.01);  
      // recoHMok   = recoHMok && exclOK;
      acopHM = acop(ele0.DeltaPhi(ele1));
      deltaptHM = fabs(ele0.Pt()-ele1.Pt());
      ptHM = diele.Pt();
      rapHM = diele.Rapidity();
      massHM = diele.M();

      // remove pixel duplicates
      float dphimin=999;
      nhits1=0;
      nhits2=0;
      nhits3=0;
      nhits4=0;
      nhits5=0;
      for (int i=0; i<pixR.nhits1; i++) {
         dphimin=999.;
         for (int j=0; j<i; j++) {
            dphimin = min(dphimin,fabs(pixR.phi1[i]-pixR.phi1[j]));
         }
         if (dphimin>0.05) nhits1++;
      }
      for (int i=0; i<pixR.nhits2; i++) {
         dphimin=999.;
         for (int j=0; j<i; j++) {
            dphimin = min(dphimin,fabs(pixR.phi2[i]-pixR.phi2[j]));
         }
         if (dphimin>0.05) nhits2++;
      }
      for (int i=0; i<pixR.nhits3; i++) {
         dphimin=999.;
         for (int j=0; j<i; j++) {
            dphimin = min(dphimin,fabs(pixR.phi3[i]-pixR.phi3[j]));
         }
         if (dphimin>0.05) nhits3++;
      }
      for (int i=0; i<pixR.nhits4; i++) {
         dphimin=999.;
         for (int j=0; j<i; j++) {
            dphimin = min(dphimin,fabs(pixR.phi4[i]-pixR.phi4[j]));
         }
         if (dphimin>0.05) nhits4++;
      }
      for (int i=0; i<pixR.nhits5; i++) {
         dphimin=999.;
         for (int j=0; j<i; j++) {
            dphimin = min(dphimin,fabs(pixR.phi5[i]-pixR.phi5[j]));
         }
         if (dphimin>0.05) nhits5++;
      }

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

         // fill the extra tracks histos
         bool isLowAcop = (acop(recoGEDdphi)<0.005);
         hextratracksGED->Fill(nextra_track_GED);
         if (isLowAcop) hextratracksGED_lacop->Fill(nextra_track_GED);
         else hextratracksGED_hacop->Fill(nextra_track_GED);

         // fill pixel histos
         hnhits1->Fill(nhits1);
         hnhits2->Fill(nhits2);
         hnhits3->Fill(nhits3);
         hnhits4->Fill(nhits4);
         hnhits5->Fill(nhits5);
         if (isLowAcop) {
            hnhits1_lacop->Fill(nhits1);
            hnhits2_lacop->Fill(nhits2);
            hnhits3_lacop->Fill(nhits3);
            hnhits4_lacop->Fill(nhits4);
            hnhits5_lacop->Fill(nhits5);
         } else {
            hnhits1_hacop->Fill(nhits1);
            hnhits2_hacop->Fill(nhits2);
            hnhits3_hacop->Fill(nhits3);
            hnhits4_hacop->Fill(nhits4);
            hnhits5_hacop->Fill(nhits5);
         }
         trGED->Fill();
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

         // fill the extra tracks histos
         bool isLowAcop = (acop(recoHMdphi)<0.05);
         hextratracksHM->Fill(nextra_track_HM);
         if (isLowAcop) hextratracksHM_lacop->Fill(nextra_track_HM);
         else hextratracksHM_hacop->Fill(nextra_track_HM);

         trHM->Fill();
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
