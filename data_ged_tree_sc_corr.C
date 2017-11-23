/*
 * macro to analyze and save di-Photon, di-Electron and di-Muon spectrum
 */
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TCut.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TList.h>
#include <TLorentzVector.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>

using namespace std;

const int MAXJETS = 500;
const int MAXMC = 10;
const float cutPt = 10;
const float cutEta = 1.4791;
const float cutDeltaR = 0.15;
const long MAXTREESIZE = 200000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB7

const double eleMass = 0.000511;
const double muMass  = 0.105658;
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

void data_ged_tree_sc_corr( std::string infile_Forest = "mc.txt",
			 std::string out = "test.root") 
{

  TH1::SetDefaultSumw2();

  TChain *treeHLT   = new TChain("hltanalysis/HltTree");
  TChain *tree      = new TChain("ggHiNtuplizer/EventTree");
  TChain *treePixel = new TChain("pixel/PixelTree");
  
  std::ifstream instr_Forest(infile_Forest.c_str(),std::ifstream::in);
  std::string filename_Forest;
  while (instr_Forest.good()) {
     instr_Forest>>filename_Forest;
     if (!instr_Forest.good()) break;
     cout << "opening file " << filename_Forest << endl;
     treeHLT->AddFile(filename_Forest.c_str());
     tree->AddFile(filename_Forest.c_str());
     treePixel->AddFile(filename_Forest.c_str());
  }

  TH1D* htrigger_Ereco_before_trigg = new TH1D("htrigger_Ereco_before_trigg","",20,0,10);
  TH1D* htrigger_Ereco_after_trigg  = new TH1D("htrigger_Ereco_after_trigg","",20,0,10);
  TH1D* htrigger_eff_vs_Ereco   = new TH1D("htrigger_eff_vs_Ereco","",20,0,10);
  
  TH1D* hsingle_pt   = new TH1D("hsingle_pt","",30,0,15);
  TH1D* hsingle_eta  = new TH1D("hsingle_eta","",6,-2.4,2.4);
  TH1D* hsingle_phi  = new TH1D("hsingle_phi","",8,-4,4);
  
  TH1D* hdpt   = new TH1D("hdpt","",10,0,5);
  TH1D* hdeta  = new TH1D("hdeta","",6,0,2.4);
  TH1D* hdphi  = new TH1D("hdphi","",20,2.65,3.15);
  TH1D* haco   = new TH1D("haco","",12,0,0.06);
 
  TH1D* hdouble_pt    = new TH1D("hdouble_pt","",12,0,2);
  TH1D* hdouble_rap   = new TH1D("hdouble_rap","",6,-2.4,2.4);
  TH1D* hdouble_mass  = new TH1D("hdouble_mass","",10,0,30);
 
  /// HLT tree ////////////////////////////////////////////////////////  
  treeHLT->SetBranchStatus("*",0);     // disable all branches
  treeHLT->SetBranchStatus("HLT_HIUPCL1SingleEG5*",1);     // enable photon branches
  treeHLT->SetBranchStatus("HLT_HIUPCL1DoubleEG2*",1);     // enable photon branches
  
  Int_t HLT_HIUPCL1DoubleEG2NotHF2_v1 ;   Int_t HLT_HIUPCL1SingleEG5NotHF2_v1;
  treeHLT->SetBranchAddress("HLT_HIUPCL1SingleEG5NotHF2_v1",&HLT_HIUPCL1SingleEG5NotHF2_v1);     // enable photon branches
  treeHLT->SetBranchAddress("HLT_HIUPCL1DoubleEG2NotHF2_v1",&HLT_HIUPCL1DoubleEG2NotHF2_v1);     // enable photon branches
  
  // event information
  UInt_t run, lumis;
  ULong64_t event;
  tree->SetBranchStatus("*",0);     // disable all branches
  tree->SetBranchStatus("run",1);
  tree->SetBranchStatus("event",1);
  tree->SetBranchStatus("lumis",1);
  tree->SetBranchAddress("run", &run);
  tree->SetBranchAddress("event", &event);
  tree->SetBranchAddress("lumis", &lumis);

  // MC
  int nMC;
  vector<int> *mcPID;
  vector<float> *mcPt;
  vector<float> *mcEta;
  vector<float> *mcPhi;
  vector<float> *mcMass;
  tree->SetBranchStatus("nMC",1); tree->SetBranchAddress("nMC",&nMC);
  tree->SetBranchStatus("mcPID",1); tree->SetBranchAddress("mcPID",&mcPID);
  tree->SetBranchStatus("mcPt",1); tree->SetBranchAddress("mcPt",&mcPt);
  tree->SetBranchStatus("mcEta",1); tree->SetBranchAddress("mcEta",&mcEta);
  tree->SetBranchStatus("mcPhi",1); tree->SetBranchAddress("mcPhi",&mcPhi);
  tree->SetBranchStatus("mcMass",1); tree->SetBranchAddress("mcMass",&mcMass);
 
  // vertex info
  Int_t nVtx;
  std::vector<float>* xVtx=0;
  std::vector<float>* yVtx=0;
  std::vector<float>* zVtx=0;
  std::vector<int>*   isFakeVtx=0;

  tree->SetBranchStatus("nVtx",1);     
  tree->SetBranchAddress("nVtx",&nVtx);
  tree->SetBranchStatus("xVtx",1);     
  tree->SetBranchStatus("yVtx",1);     
  tree->SetBranchStatus("zVtx",1);  
  tree->SetBranchStatus("isFakeVtx",1);  
   
  tree->SetBranchAddress("xVtx",&xVtx);
  tree->SetBranchAddress("yVtx",&yVtx);
  tree->SetBranchAddress("zVtx",&zVtx);
  tree->SetBranchAddress("isFakeVtx",&isFakeVtx);
 
  // GED RECO electrons
  Int_t nEle;
  std::vector<int>*   eleCharge=0;
  std::vector<float>* eleEn=0;
  std::vector<float>* eleP=0;
  std::vector<float>* elePt=0;
  std::vector<float>* eleEta=0;
  std::vector<float>* elePhi=0;
  std::vector<float>* eleHoverE=0;
  std::vector<float>* eleSigmaIEtaIEta=0;
  std::vector<float>* eleSigmaIEtaIEta_2012=0;
  std::vector<float>* eleSigmaIPhiIPhi=0;
  std::vector<float>* eleEoverP=0;
  std::vector<float>* eledEtaAtVtx=0;
  std::vector<float>* eledPhiAtVtx=0;
  std::vector<float>* eledEtaSeedAtVtx=0;
  std::vector<float>* eleD0=0;
  std::vector<float>* eleDz=0;
  std::vector<float>* eleTrkPt=0;
  std::vector<int>*   eleMissHits=0;
  std::vector<float>* eleTrackIso=0;
  std::vector<float>* eleHCalIso=0;
  std::vector<float>* eleECalIso=0;
  std::vector<float>* eleECalDriven=0;
  std::vector<float>* eleSCEn=0;
  std::vector<float>* eleSCRawEn=0;
  std::vector<float>* eleSCEta=0;
  std::vector<float>* eleSCPhi=0;
  std::vector<float>* eleBrem=0;
  std::vector<int>*   NClusters=0;

   
  tree->SetBranchStatus("nEle",1);     // enable electron branches
  tree->SetBranchStatus("ele*",1);     // enable electron branches
  tree->SetBranchAddress("nEle",&nEle);
  tree->SetBranchAddress("eleEn",&eleEn);
  tree->SetBranchAddress("eleCharge",&eleCharge);
  tree->SetBranchAddress("eleP",&eleP);
  tree->SetBranchAddress("elePt",&elePt);
  tree->SetBranchAddress("eleEta",&eleEta);
  tree->SetBranchAddress("elePhi",&elePhi);
  tree->SetBranchAddress("eleHoverE",&eleHoverE);
  tree->SetBranchAddress("eleSigmaIEtaIEta",&eleSigmaIEtaIEta);
  tree->SetBranchAddress("eleSigmaIEtaIEta_2012",&eleSigmaIEtaIEta_2012);
  tree->SetBranchAddress("eleSigmaIPhiIPhi",&eleSigmaIPhiIPhi);
  tree->SetBranchAddress("eleEoverP",&eleEoverP);
  tree->SetBranchAddress("eledEtaAtVtx",&eledEtaAtVtx);
  tree->SetBranchAddress("eledPhiAtVtx",&eledPhiAtVtx);
  tree->SetBranchAddress("eledEtaSeedAtVtx",&eledEtaSeedAtVtx);
  tree->SetBranchAddress("eleD0",&eleD0);
  tree->SetBranchAddress("eleDz",&eleDz);
  tree->SetBranchAddress("eleTrkPt",&eleTrkPt);
  tree->SetBranchAddress("eleMissHits",&eleMissHits);
  tree->SetBranchAddress("eleTrackIso",&eleTrackIso);
  tree->SetBranchAddress("eleHCalIso",&eleHCalIso);
  tree->SetBranchAddress("eleECalIso",&eleECalIso);
  tree->SetBranchAddress("eleECalDriven",&eleECalDriven);
  tree->SetBranchAddress("eleSCEn",&eleSCEn);
  tree->SetBranchAddress("eleSCRawEn",&eleSCRawEn);
  tree->SetBranchAddress("eleSCEta",&eleSCEta);
  tree->SetBranchAddress("eleSCPhi",&eleSCPhi);
  tree->SetBranchAddress("eleBrem",&eleBrem);
  tree->SetBranchAddress("NClusters",&NClusters);

  // RECO muons
  Int_t nMu;  
  tree->SetBranchStatus("nMu",1);     // enable muon branches
  tree->SetBranchAddress("nMu",&nMu);

  // calo tower
  Int_t nTower;
  std::vector<float>* CaloTower_hadE=0;
  std::vector<float>* CaloTower_emE=0;
  std::vector<float>* CaloTower_e=0;
  std::vector<float>* CaloTower_et=0;
  std::vector<float>* CaloTower_eta=0;
  std::vector<float>* CaloTower_phi=0;

  tree->SetBranchStatus("nTower",1);     // enable photon branches
  tree->SetBranchAddress("nTower",&nTower);
  tree->SetBranchStatus("Calo*",1);
  tree->SetBranchAddress("CaloTower_hadE",&CaloTower_hadE);
  tree->SetBranchAddress("CaloTower_emE",&CaloTower_emE);
  tree->SetBranchAddress("CaloTower_e",&CaloTower_e);
  tree->SetBranchAddress("CaloTower_et",&CaloTower_et);
  tree->SetBranchAddress("CaloTower_eta",&CaloTower_eta);
  tree->SetBranchAddress("CaloTower_phi",&CaloTower_phi);


  // pixel info  
  Int_t nhits1, nhits2, nhits3, nhits4, nhits5, nEv;
  treePixel->SetBranchStatus("nEv",1);     // enable photon branches
  treePixel->SetBranchAddress("nEv",&nEv);     // enable photon branches
  treePixel->SetBranchStatus("nhits*",1);     // enable pixel branches
  treePixel->SetBranchAddress("nhits1",&nhits1);     
  treePixel->SetBranchAddress("nhits2",&nhits2);     
  treePixel->SetBranchAddress("nhits3",&nhits3);   
  treePixel->SetBranchAddress("nhits4",&nhits4);     
  treePixel->SetBranchAddress("nhits5",&nhits5);  
  
  /// General track collection/////////
  Int_t ngenTrk;
  std::vector<float>* gentrkPt=0;
  std::vector<float>* gentrkP=0;
  std::vector<float>* gentrkEta=0;
  std::vector<float>* gentrkPhi=0;
  std::vector<int>*   gentrkcharge=0;
  std::vector<float>* gentrkchi2=0;
  std::vector<float>* gentrknormchi2=0;
  std::vector<int>*   gentrkValidHits=0;
  std::vector<int>*   gentrkMissHits=0;


  tree->SetBranchStatus("ngenTrk",1);     // enable electron branches
  tree->SetBranchAddress("ngenTrk",&ngenTrk);
  tree->SetBranchStatus("gen*",1);     // enable electron branches
  tree->SetBranchAddress("gentrkPt",&gentrkPt);
  tree->SetBranchAddress("gentrkP",&gentrkP);
  tree->SetBranchAddress("gentrkEta",&gentrkEta);
  tree->SetBranchAddress("gentrkPhi",&gentrkPhi);
  tree->SetBranchAddress("gentrkcharge",&gentrkcharge);
  tree->SetBranchAddress("gentrkchi2",&gentrkchi2);
  tree->SetBranchAddress("gentrknormchi2",&gentrknormchi2);
  tree->SetBranchAddress("gentrkValidHits",&gentrkValidHits);
  tree->SetBranchAddress("gentrkMissHits",&gentrkMissHits);   
  
  int pix_hit1;
  int pix_hit2;
  int pix_hit3;
  int pix_hit4;
  int pix_hit5;


  float phodpt;
  float phodeta;
  float phodphi;
  float aco;
  
   TFile *output;
   output = new TFile(Form("%s",out.c_str()),"recreate");

  // output tree variables

  TTree *outputTreeHLT    = treeHLT->CloneTree(0);
  outputTreeHLT->SetName("hltTree");

  TTree *outputTreePho    = tree->CloneTree(0);
  outputTreePho->SetName("photons");
  outputTreePho->SetTitle("Event data + photons");
       
 
  TTree *outputTreediPho  = new TTree("diphoton","");
  TTree *outputTreediEle  = new TTree("dielectron","");
  TTree *outputTreeMC  = new TTree("mcTree","");
 
  outputTreeHLT->SetMaxTreeSize(MAXTREESIZE);
  outputTreediPho->SetMaxTreeSize(MAXTREESIZE);
  outputTreediEle->SetMaxTreeSize(MAXTREESIZE);

  ////**************************** write output branches      *************************/
  
  outputTreeMC->Branch("nMC",&nMC,"nMC/I");
  outputTreeMC->Branch("mcPID",&mcPID);
  outputTreeMC->Branch("mcPt",&mcPt);
  outputTreeMC->Branch("mcEta",&mcEta);
  outputTreeMC->Branch("mcPhi",&mcPhi);
  outputTreeMC->Branch("mcMass",&mcMass);

  outputTreediPho->Branch("run",&run);
  outputTreediPho->Branch("event",&event);
  outputTreediPho->Branch("lumis",&lumis);

  outputTreediPho->Branch("nVtx",&nVtx);
  outputTreediPho->Branch("xVtx",&xVtx);
  outputTreediPho->Branch("yVtx",&yVtx);
  outputTreediPho->Branch("zVtx",&zVtx);
  outputTreediPho->Branch("isFakeVtx",&isFakeVtx);
  
  std::vector<int>   eleCharge_1;
  std::vector<float> eleP_1;
  std::vector<float> elePt_1;
  std::vector<float> eleEta_1;
  std::vector<float> elePhi_1;
  std::vector<float> eleHoverE_1;
  std::vector<float> eleSigmaIEtaIEta_1;
  std::vector<float> eleSigmaIEtaIEta_2012_1;
  std::vector<float> eleSigmaIPhiIPhi_1;
  std::vector<float> eleEoverP_1;
  std::vector<float> eledEtaAtVtx_1;
  std::vector<float> eledPhiAtVtx_1;
  std::vector<float> eledEtaSeedAtVtx_1;
  std::vector<float> eleD0_1;
  std::vector<float> eleDz_1;
  std::vector<int>   eleMissHits_1;
  std::vector<float> eleTrackIso_1;
  std::vector<float> eleHCalIso_1;
  std::vector<float> eleECalIso_1;
  std::vector<float> eleECalDriven_1;
  std::vector<float> eleBrem_1;
  
   
  std::vector<int>   eleCharge_2;
  std::vector<float> eleP_2;
  std::vector<float> elePt_2;
  std::vector<float> eleEta_2;
  std::vector<float> elePhi_2;
  std::vector<float> eleHoverE_2;
  std::vector<float> eleSigmaIEtaIEta_2;
  std::vector<float> eleSigmaIEtaIEta_2012_2;
  std::vector<float> eleSigmaIPhiIPhi_2;
  std::vector<float> eleEoverP_2;
  std::vector<float> eledEtaAtVtx_2;
  std::vector<float> eledPhiAtVtx_2;
  std::vector<float> eledEtaSeedAtVtx_2;
  std::vector<float> eleD0_2;
  std::vector<float> eleDz_2;
  std::vector<int>   eleMissHits_2;
  std::vector<float> eleTrackIso_2;
  std::vector<float> eleHCalIso_2;
  std::vector<float> eleECalIso_2;
  std::vector<float> eleECalDriven_2;
  std::vector<float> eleBrem_2;
 
  
  std::vector<float> vSum_ee_M;
  std::vector<float> vSum_ee_Energy;
  std::vector<float> vSum_ee_Pt;
  std::vector<float> vSum_ee_Eta;
  std::vector<float> vSum_ee_Phi;
  std::vector<float> vSum_ee_Rapidity;
  
  outputTreediPho->Branch("nEle",&nEle);
  outputTreediPho->Branch("eleCharge_1",&eleCharge_1);
  outputTreediPho->Branch("eleP_1",&eleP_1);
  outputTreediPho->Branch("elePt_1",&elePt_1);
  outputTreediPho->Branch("eleEta_1",&eleEta_1);
  outputTreediPho->Branch("elePhi_1",&elePhi_1);
  outputTreediPho->Branch("eleHoverE_1",&eleHoverE_1);
  outputTreediPho->Branch("eleSigmaIEtaIEta_1",&eleSigmaIEtaIEta_1);
  outputTreediPho->Branch("eleSigmaIEtaIEta_2012_1",&eleSigmaIEtaIEta_2012_1);
  outputTreediPho->Branch("eleSigmaIPhiIPhi_1",&eleSigmaIPhiIPhi_1);
  outputTreediPho->Branch("eleEoverP_1",&eleEoverP_1);
  outputTreediPho->Branch("eledEtaAtVtx_1",&eledEtaAtVtx_1);
  outputTreediPho->Branch("eledPhiAtVtx_1",&eledPhiAtVtx_1);
  outputTreediPho->Branch("eledEtaSeedAtVtx_1",&eledEtaSeedAtVtx_1);
  outputTreediPho->Branch("eleD0_1",&eleD0_1);
  outputTreediPho->Branch("eleDz_1",&eleDz_1);
  outputTreediPho->Branch("eleMissHits_1",&eleMissHits_1);
  outputTreediPho->Branch("eleTrackIso_1",&eleTrackIso_1);
  outputTreediPho->Branch("eleHCalIso_1",&eleHCalIso_1);
  outputTreediPho->Branch("eleECalIso_1",&eleECalIso_1);
  outputTreediPho->Branch("eleECalDriven_1",&eleECalDriven_1);
  outputTreediPho->Branch("eleBrem_1",&eleBrem_1);
  


  outputTreediPho->Branch("eleCharge_2",&eleCharge_2);
  outputTreediPho->Branch("eleP_2",&eleP_2);
  outputTreediPho->Branch("elePt_2",&elePt_2);
  outputTreediPho->Branch("eleEta_2",&eleEta_2);
  outputTreediPho->Branch("elePhi_2",&elePhi_2);
  outputTreediPho->Branch("eleHoverE_2",&eleHoverE_2);
  outputTreediPho->Branch("eleSigmaIEtaIEta_2",&eleSigmaIEtaIEta_2);
  outputTreediPho->Branch("eleSigmaIEtaIEta_2012_2",&eleSigmaIEtaIEta_2012_2);
  outputTreediPho->Branch("eleSigmaIPhiIPhi_2",&eleSigmaIPhiIPhi_2);
  outputTreediPho->Branch("eleEoverP_2",&eleEoverP_2);
  outputTreediPho->Branch("eledEtaAtVtx_2",&eledEtaAtVtx_2);
  outputTreediPho->Branch("eledPhiAtVtx_2",&eledPhiAtVtx_2);
  outputTreediPho->Branch("eledEtaSeedAtVtx_2",&eledEtaSeedAtVtx_2);
  outputTreediPho->Branch("eleD0_2",&eleD0_2);
  outputTreediPho->Branch("eleDz_2",&eleDz_2);
  outputTreediPho->Branch("eleMissHits_2",&eleMissHits_2);
  outputTreediPho->Branch("eleTrackIso_2",&eleTrackIso_2);
  outputTreediPho->Branch("eleHCalIso_2",&eleHCalIso_2);
  outputTreediPho->Branch("eleECalIso_2",&eleECalIso_2);
  outputTreediPho->Branch("eleECalDriven_2",&eleECalDriven_2);
  outputTreediPho->Branch("eleBrem_2",&eleBrem_2);
 
  
  outputTreediPho->Branch("vSum_M",&vSum_ee_M);
  outputTreediPho->Branch("vSum_Energy",&vSum_ee_Energy);
  outputTreediPho->Branch("vSum_Pt",&vSum_ee_Pt);
  outputTreediPho->Branch("vSum_Eta",&vSum_ee_Eta);
  outputTreediPho->Branch("vSum_Phi",&vSum_ee_Phi);
  outputTreediPho->Branch("vSum_Rapidity",&vSum_ee_Rapidity);

  std::vector<float> ele_dpt;
  std::vector<float> ele_deta;
  std::vector<float> ele_dphi;
  std::vector<float> ele_aco;

  outputTreediPho->Branch("ele_dpt", &ele_dpt);
  outputTreediPho->Branch("ele_deta",&ele_deta);
  outputTreediPho->Branch("ele_dphi",&ele_dphi);
  outputTreediPho->Branch("ele_aco", &ele_aco);

  // write ged barel supercluster info

  std::vector<int>    nsc_ged_brl_1;
  std::vector<float>  sc_ged_brl_E_1;
  std::vector<float>  sc_ged_brl_Et_1;
  std::vector<float>  sc_ged_brl_Eta_1;
  std::vector<float>  sc_ged_brl_Phi_1;
  std::vector<float>  sc_ged_brl_RawE_1;
  std::vector<float>  sc_ged_brl_RawEt_1;  
  std::vector<float>  sc_ged_brl_raw_eoverp_1;
  std::vector<float>  sc_ged_brl_eoverp_1;
  
  outputTreediPho->Branch("nsc_ged_brl_1",&nsc_ged_brl_1);
  outputTreediPho->Branch("sc_ged_brl_E_1",&sc_ged_brl_E_1);
  outputTreediPho->Branch("sc_ged_brl_Et_1",&sc_ged_brl_Et_1);
  outputTreediPho->Branch("sc_ged_brl_Eta_1",&sc_ged_brl_Eta_1);
  outputTreediPho->Branch("sc_ged_brl_Phi_1",&sc_ged_brl_Phi_1);  
  outputTreediPho->Branch("sc_ged_brl_RawE_1",&sc_ged_brl_RawE_1);
  outputTreediPho->Branch("sc_ged_brl_RawEt_1",&sc_ged_brl_RawEt_1);  
  outputTreediPho->Branch("sc_ged_brl_raw_eoverp_1",&sc_ged_brl_raw_eoverp_1);
  outputTreediPho->Branch("sc_ged_brl_eoverp_1",&sc_ged_brl_eoverp_1);


  // write ged barel supercluster info
  std::vector<int>    nsc_ged_brl_2;
  std::vector<float>  sc_ged_brl_E_2;
  std::vector<float>  sc_ged_brl_Et_2;
  std::vector<float>  sc_ged_brl_Eta_2;
  std::vector<float>  sc_ged_brl_Phi_2;
  std::vector<float>  sc_ged_brl_RawE_2;
  std::vector<float>  sc_ged_brl_RawEt_2;
  std::vector<float>  sc_ged_brl_raw_eoverp_2;
  std::vector<float>  sc_ged_brl_eoverp_2;
  
  
  outputTreediPho->Branch("nsc_ged_brl_2",&nsc_ged_brl_2);
  outputTreediPho->Branch("sc_ged_brl_E_2",&sc_ged_brl_E_2);
  outputTreediPho->Branch("sc_ged_brl_Et_2",&sc_ged_brl_Et_2);
  outputTreediPho->Branch("sc_ged_brl_Eta_2",&sc_ged_brl_Eta_2);
  outputTreediPho->Branch("sc_ged_brl_Phi_2",&sc_ged_brl_Phi_2);  
  outputTreediPho->Branch("sc_ged_brl_RawE_2",&sc_ged_brl_RawE_2);
  outputTreediPho->Branch("sc_ged_brl_RawEt_2",&sc_ged_brl_RawEt_2); 
  outputTreediPho->Branch("sc_ged_brl_raw_eoverp_2",&sc_ged_brl_raw_eoverp_2);
  outputTreediPho->Branch("sc_ged_brl_eoverp_2",&sc_ged_brl_eoverp_2); 


  // write di-cluster info with hybrid
  std::vector<float> vSum_ged_brl_reco_M;
  std::vector<float> vSum_ged_brl_reco_Energy;
  std::vector<float> vSum_ged_brl_reco_Et;
  std::vector<float> vSum_ged_brl_reco_Pt;
  std::vector<float> vSum_ged_brl_reco_Eta;
  std::vector<float> vSum_ged_brl_reco_Phi;
  std::vector<float> vSum_ged_brl_reco_Rapidity;
  std::vector<float> vSum_ged_brl_reco_Theta;
  
  outputTreediPho->Branch("vSum_ged_brl_reco_M",&vSum_ged_brl_reco_M);
  outputTreediPho->Branch("vSum_ged_brl_reco_Energy",&vSum_ged_brl_reco_Energy);
  outputTreediPho->Branch("vSum_ged_brl_reco_Et",&vSum_ged_brl_reco_Et);
  outputTreediPho->Branch("vSum_ged_brl_reco_Pt",&vSum_ged_brl_reco_Pt);
  outputTreediPho->Branch("vSum_ged_brl_reco_Eta",&vSum_ged_brl_reco_Eta);
  outputTreediPho->Branch("vSum_ged_brl_reco_Phi",&vSum_ged_brl_reco_Phi);
  outputTreediPho->Branch("vSum_ged_brl_reco_Rapidity",&vSum_ged_brl_reco_Rapidity);
  outputTreediPho->Branch("vSum_ged_brl_reco_Theta",&vSum_ged_brl_reco_Theta);


  std::vector<float> vSum_ged_brl_raw_M;
  std::vector<float> vSum_ged_brl_raw_Energy;
  std::vector<float> vSum_ged_brl_raw_Et;
  std::vector<float> vSum_ged_brl_raw_Pt;
  std::vector<float> vSum_ged_brl_raw_Eta;
  std::vector<float> vSum_ged_brl_raw_Phi;
  std::vector<float> vSum_ged_brl_raw_Rapidity;
  std::vector<float> vSum_ged_brl_raw_Theta;
  
  outputTreediPho->Branch("vSum_ged_brl_raw_M",&vSum_ged_brl_raw_M);
  outputTreediPho->Branch("vSum_ged_brl_raw_Energy",&vSum_ged_brl_raw_Energy);
  outputTreediPho->Branch("vSum_ged_brl_raw_Et",&vSum_ged_brl_raw_Et);
  outputTreediPho->Branch("vSum_ged_brl_raw_Pt",&vSum_ged_brl_raw_Pt);
  outputTreediPho->Branch("vSum_ged_brl_raw_Eta",&vSum_ged_brl_raw_Eta);
  outputTreediPho->Branch("vSum_ged_brl_raw_Phi",&vSum_ged_brl_raw_Phi);
  outputTreediPho->Branch("vSum_ged_brl_raw_Rapidity",&vSum_ged_brl_raw_Rapidity);
  outputTreediPho->Branch("vSum_ged_brl_raw_Theta",&vSum_ged_brl_raw_Theta);
  

  // write ged barel ele info
  std::vector<float> ele_ged_brl_Pt_1;
  std::vector<float> ele_ged_brl_P_1;
  std::vector<float> ele_ged_brl_Eta_1;
  std::vector<float> ele_ged_brl_Phi_1;
  std::vector<int>   ele_ged_brl_Charge_1;
  
  outputTreediPho->Branch("nEle",&nEle);
  outputTreediPho->Branch("ele_ged_brl_Pt_1", &ele_ged_brl_Pt_1);
  outputTreediPho->Branch("ele_ged_brl_P_1",  &ele_ged_brl_P_1);
  outputTreediPho->Branch("ele_ged_brl_Eta_1",&ele_ged_brl_Eta_1);
  outputTreediPho->Branch("ele_ged_brl_Phi_1",&ele_ged_brl_Phi_1);
  outputTreediPho->Branch("ele_ged_brl_Charge_1",&ele_ged_brl_Charge_1);

  std::vector<float> ele_ged_brl_Pt_2;
  std::vector<float> ele_ged_brl_P_2;
  std::vector<float> ele_ged_brl_Eta_2;
  std::vector<float> ele_ged_brl_Phi_2;
  std::vector<int>   ele_ged_brl_Charge_2;

  outputTreediPho->Branch("ele_ged_brl_Pt_2", &ele_ged_brl_Pt_2);
  outputTreediPho->Branch("ele_ged_brl_P_2",  &ele_ged_brl_P_2);
  outputTreediPho->Branch("ele_ged_brl_Eta_2",&ele_ged_brl_Eta_2);
  outputTreediPho->Branch("ele_ged_brl_Phi_2",&ele_ged_brl_Phi_2);
  outputTreediPho->Branch("ele_ged_brl_Charge_2",&ele_ged_brl_Charge_2);
  
  // write dielectron info for electron-cluster matched 
  std::vector<float> vSum_ee_ged_brl_M;
  std::vector<float> vSum_ee_ged_brl_Energy;
  std::vector<float> vSum_ee_ged_brl_Et;
  std::vector<float> vSum_ee_ged_brl_Pt;
  std::vector<float> vSum_ee_ged_brl_Eta;
  std::vector<float> vSum_ee_ged_brl_Phi;
  std::vector<float> vSum_ee_ged_brl_Rapidity;
  std::vector<float> vSum_ee_ged_brl_Theta;
  
  outputTreediPho->Branch("vSum_ee_ged_brl_M",&vSum_ee_ged_brl_M);
  outputTreediPho->Branch("vSum_ee_ged_brl_Energy",&vSum_ee_ged_brl_Energy);
  outputTreediPho->Branch("vSum_ee_ged_brl_Et",&vSum_ee_ged_brl_Et);
  outputTreediPho->Branch("vSum_ee_ged_brl_Pt",&vSum_ee_ged_brl_Pt);
  outputTreediPho->Branch("vSum_ee_ged_brl_Eta",&vSum_ee_ged_brl_Eta);
  outputTreediPho->Branch("vSum_ee_ged_brl_Phi",&vSum_ee_ged_brl_Phi);
  outputTreediPho->Branch("vSum_ee_ged_brl_Rapidity",&vSum_ee_ged_brl_Rapidity);
  outputTreediPho->Branch("vSum_ee_ged_brl_Theta",&vSum_ee_ged_brl_Theta);
  

  // write ged endcap supercluster info -------------------------------------------------------------------------------

  std::vector<int>    nsc_ged_end_1;
  std::vector<float>  sc_ged_end_E_1;
  std::vector<float>  sc_ged_end_Et_1;
  std::vector<float>  sc_ged_end_Eta_1;
  std::vector<float>  sc_ged_end_Phi_1;
  std::vector<float>  sc_ged_end_RawE_1;
  std::vector<float>  sc_ged_end_RawEt_1;  
  std::vector<float>  sc_ged_end_raw_eoverp_1;
  std::vector<float>  sc_ged_end_eoverp_1;
  
  outputTreediPho->Branch("nsc_ged_end_1",&nsc_ged_end_1);
  outputTreediPho->Branch("sc_ged_end_E_1",&sc_ged_end_E_1);
  outputTreediPho->Branch("sc_ged_end_Et_1",&sc_ged_end_Et_1);
  outputTreediPho->Branch("sc_ged_end_Eta_1",&sc_ged_end_Eta_1);
  outputTreediPho->Branch("sc_ged_end_Phi_1",&sc_ged_end_Phi_1);  
  outputTreediPho->Branch("sc_ged_end_RawE_1",&sc_ged_end_RawE_1);
  outputTreediPho->Branch("sc_ged_end_RawEt_1",&sc_ged_end_RawEt_1);  
  outputTreediPho->Branch("sc_ged_end_raw_eoverp_1",&sc_ged_end_raw_eoverp_1);
  outputTreediPho->Branch("sc_ged_end_eoverp_1",&sc_ged_end_eoverp_1);



  // write ged endcap supercluster info
  std::vector<int>    nsc_ged_end_2;
  std::vector<float>  sc_ged_end_E_2;
  std::vector<float>  sc_ged_end_Et_2;
  std::vector<float>  sc_ged_end_Eta_2;
  std::vector<float>  sc_ged_end_Phi_2;
  std::vector<float>  sc_ged_end_RawE_2;
  std::vector<float>  sc_ged_end_RawEt_2;
  std::vector<float>  sc_ged_end_raw_eoverp_2;
  std::vector<float>  sc_ged_end_eoverp_2;
  
  outputTreediPho->Branch("nsc_ged_end_2",&nsc_ged_end_2);
  outputTreediPho->Branch("sc_ged_end_E_2",&sc_ged_end_E_2);
  outputTreediPho->Branch("sc_ged_end_Et_2",&sc_ged_end_Et_2);
  outputTreediPho->Branch("sc_ged_end_Eta_2",&sc_ged_end_Eta_2);
  outputTreediPho->Branch("sc_ged_end_Phi_2",&sc_ged_end_Phi_2);  
  outputTreediPho->Branch("sc_ged_end_RawE_2",&sc_ged_end_RawE_2);
  outputTreediPho->Branch("sc_ged_end_RawEt_2",&sc_ged_end_RawEt_2);  
  outputTreediPho->Branch("sc_ged_end_raw_eoverp_2",&sc_ged_end_raw_eoverp_2);
  outputTreediPho->Branch("sc_ged_end_eoverp_2",&sc_ged_end_eoverp_2);



  // write di-cluster info in GED endcap
  std::vector<float> vSum_ged_end_reco_M;
  std::vector<float> vSum_ged_end_reco_Energy;
  std::vector<float> vSum_ged_end_reco_Et;
  std::vector<float> vSum_ged_end_reco_Pt;
  std::vector<float> vSum_ged_end_reco_Eta;
  std::vector<float> vSum_ged_end_reco_Phi;
  std::vector<float> vSum_ged_end_reco_Rapidity;
  std::vector<float> vSum_ged_end_reco_Theta;
  
  outputTreediPho->Branch("vSum_ged_end_reco_M",&vSum_ged_end_reco_M);
  outputTreediPho->Branch("vSum_ged_end_reco_Energy",&vSum_ged_end_reco_Energy);
  outputTreediPho->Branch("vSum_ged_end_reco_Et",&vSum_ged_end_reco_Et);
  outputTreediPho->Branch("vSum_ged_end_reco_Pt",&vSum_ged_end_reco_Pt);
  outputTreediPho->Branch("vSum_ged_end_reco_Eta",&vSum_ged_end_reco_Eta);
  outputTreediPho->Branch("vSum_ged_end_reco_Phi",&vSum_ged_end_reco_Phi);
  outputTreediPho->Branch("vSum_ged_end_reco_Rapidity",&vSum_ged_end_reco_Rapidity);
  outputTreediPho->Branch("vSum_ged_end_reco_Theta",&vSum_ged_end_reco_Theta);


  std::vector<float> vSum_ged_end_raw_M;
  std::vector<float> vSum_ged_end_raw_Energy;
  std::vector<float> vSum_ged_end_raw_Et;
  std::vector<float> vSum_ged_end_raw_Pt;
  std::vector<float> vSum_ged_end_raw_Eta;
  std::vector<float> vSum_ged_end_raw_Phi;
  std::vector<float> vSum_ged_end_raw_Rapidity;
  std::vector<float> vSum_ged_end_raw_Theta;
  
  outputTreediPho->Branch("vSum_ged_end_raw_M",&vSum_ged_end_raw_M);
  outputTreediPho->Branch("vSum_ged_end_raw_Energy",&vSum_ged_end_raw_Energy);
  outputTreediPho->Branch("vSum_ged_end_raw_Et",&vSum_ged_end_raw_Et);
  outputTreediPho->Branch("vSum_ged_end_raw_Pt",&vSum_ged_end_raw_Pt);
  outputTreediPho->Branch("vSum_ged_end_raw_Eta",&vSum_ged_end_raw_Eta);
  outputTreediPho->Branch("vSum_ged_end_raw_Phi",&vSum_ged_end_raw_Phi);
  outputTreediPho->Branch("vSum_ged_end_raw_Rapidity",&vSum_ged_end_raw_Rapidity);
  outputTreediPho->Branch("vSum_ged_end_raw_Theta",&vSum_ged_end_raw_Theta);


  // write ged barel ele info
  std::vector<float> ele_ged_end_Pt_1;
  std::vector<float> ele_ged_end_P_1;
  std::vector<float> ele_ged_end_Eta_1;
  std::vector<float> ele_ged_end_Phi_1;
  std::vector<int>   ele_ged_end_Charge_1;
  
  outputTreediPho->Branch("ele_ged_end_Pt_1", &ele_ged_end_Pt_1);
  outputTreediPho->Branch("ele_ged_end_P_1",  &ele_ged_end_P_1);
  outputTreediPho->Branch("ele_ged_end_Eta_1",&ele_ged_end_Eta_1);
  outputTreediPho->Branch("ele_ged_end_Phi_1",&ele_ged_end_Phi_1);
  outputTreediPho->Branch("ele_ged_end_Charge_1",&ele_ged_end_Charge_1);

  std::vector<float> ele_ged_end_Pt_2;
  std::vector<float> ele_ged_end_P_2;
  std::vector<float> ele_ged_end_Eta_2;
  std::vector<float> ele_ged_end_Phi_2;
  std::vector<int>   ele_ged_end_Charge_2;

  outputTreediPho->Branch("ele_ged_end_Pt_2", &ele_ged_end_Pt_2);
  outputTreediPho->Branch("ele_ged_end_P_2",  &ele_ged_end_P_2);
  outputTreediPho->Branch("ele_ged_end_Eta_2",&ele_ged_end_Eta_2);
  outputTreediPho->Branch("ele_ged_end_Phi_2",&ele_ged_end_Phi_2);
  outputTreediPho->Branch("ele_ged_end_Charge_2",&ele_ged_end_Charge_2);

  // write dielectron info for electron-cluster matched 
  std::vector<float> vSum_ee_ged_end_M;
  std::vector<float> vSum_ee_ged_end_Energy;
  std::vector<float> vSum_ee_ged_end_Et;
  std::vector<float> vSum_ee_ged_end_Pt;
  std::vector<float> vSum_ee_ged_end_Eta;
  std::vector<float> vSum_ee_ged_end_Phi;
  std::vector<float> vSum_ee_ged_end_Rapidity;
  std::vector<float> vSum_ee_ged_end_Theta;
  
  outputTreediPho->Branch("vSum_ee_ged_end_M",&vSum_ee_ged_end_M);
  outputTreediPho->Branch("vSum_ee_ged_end_Energy",&vSum_ee_ged_end_Energy);
  outputTreediPho->Branch("vSum_ee_ged_end_Et",&vSum_ee_ged_end_Et);
  outputTreediPho->Branch("vSum_ee_ged_end_Pt",&vSum_ee_ged_end_Pt);
  outputTreediPho->Branch("vSum_ee_ged_end_Eta",&vSum_ee_ged_end_Eta);
  outputTreediPho->Branch("vSum_ee_ged_end_Phi",&vSum_ee_ged_end_Phi);
  outputTreediPho->Branch("vSum_ee_ged_end_Rapidity",&vSum_ee_ged_end_Rapidity);
  outputTreediPho->Branch("vSum_ee_ged_end_Theta",&vSum_ee_ged_end_Theta);
  

  std::vector<float> invmass_ged_brl_cluster_reco_formula;
  std::vector<float> invmass_ged_brl_cluster_raw_formula;
  std::vector<float> invmass_ged_end_cluster_reco_formula;
  std::vector<float> invmass_ged_end_cluster_raw_formula;
  
  outputTreediPho->Branch("invmass_ged_brl_cluster_reco_formula",&invmass_ged_brl_cluster_reco_formula);
  outputTreediPho->Branch("invmass_ged_brl_cluster_raw_formula",&invmass_ged_brl_cluster_raw_formula);
  outputTreediPho->Branch("invmass_ged_end_cluster_reco_formula",&invmass_ged_end_cluster_reco_formula);
  outputTreediPho->Branch("invmass_ged_end_cluster_raw_formula",&invmass_ged_end_cluster_raw_formula);
  
  // write tower info //// --------------------------------------------------------
  std::vector<float>  allEmEnergy_EB;
  std::vector<float>  allEmEnergy_EE;
  std::vector<float>  allHadEnergy_HB; 
  std::vector<float>  allHadEnergy_HE;
  std::vector<float>  allHadEnergy_HF_Plus;
  std::vector<float>  allHadEnergy_HF_Minus;
  
  std::vector<float>  leadingEmEnergy_EB;
  std::vector<float>  leadingEmEnergy_EE;
  std::vector<float>  leadingHadEnergy_HB; 
  std::vector<float>  leadingHadEnergy_HE;
  std::vector<float>  leadingHadEnergy_HF_Plus;
  std::vector<float>  leadingHadEnergy_HF_Minus;

  outputTreediPho->Branch("nTower",&nTower);
  outputTreediPho->Branch("allEmEnergy_EB",&allEmEnergy_EB);
  outputTreediPho->Branch("allEmEnergy_EE",&allEmEnergy_EE);
  outputTreediPho->Branch("allHadEnergy_HB",&allHadEnergy_HB);
  outputTreediPho->Branch("allHadEnergy_HE",&allHadEnergy_HE);
  outputTreediPho->Branch("allHadEnergy_HF_Plus",&allHadEnergy_HF_Plus);
  outputTreediPho->Branch("allHadEnergy_HF_Minus",&allHadEnergy_HF_Minus);

  outputTreediPho->Branch("leadingEmEnergy_EB",&leadingEmEnergy_EB);
  outputTreediPho->Branch("leadingEmEnergy_EE",&leadingEmEnergy_EE);
  outputTreediPho->Branch("leadingHadEnergy_HB",&leadingHadEnergy_HB);
  outputTreediPho->Branch("leadingHadEnergy_HE",&leadingHadEnergy_HE);
  outputTreediPho->Branch("leadingHadEnergy_HF_Plus",&leadingHadEnergy_HF_Plus);
  outputTreediPho->Branch("leadingHadEnergy_HF_Minus",&leadingHadEnergy_HF_Minus);


 /// write pixel info///////////////////

  outputTreediPho->Branch("pix_hit1",&pix_hit1);
  outputTreediPho->Branch("pix_hit2",&pix_hit2);
  outputTreediPho->Branch("pix_hit3",&pix_hit3);
  outputTreediPho->Branch("pix_hit4",&pix_hit4);
  outputTreediPho->Branch("pix_hit5",&pix_hit5);
  
  std::vector<float> trkpt;
  std::vector<float> trketa;
  std::vector<float> trkphi;

  outputTreediPho->Branch("ngenTrk",&ngenTrk);
  outputTreediPho->Branch("trkpt",&trkpt);
  outputTreediPho->Branch("trketa",&trketa);
  outputTreediPho->Branch("trkphi",&trkphi);
  
  //int   nextra_track;
  std::vector<int>   nextra_track;
  std::vector<int>   nextra_track_pt0p06;
  std::vector<int>   nextra_track_pt0p14;
  std::vector<int>   nextra_track_pt0p1;
  std::vector<float> extrkpt;
  std::vector<float> extrketa;
  std::vector<float> extrkphi;

  outputTreediPho->Branch("nextra_track",&nextra_track);
  outputTreediPho->Branch("nextra_track_pt0p06",&nextra_track_pt0p06);
  outputTreediPho->Branch("nextra_track_pt0p14",&nextra_track_pt0p14);
  outputTreediPho->Branch("nextra_track_pt0p1",&nextra_track_pt0p1);

  outputTreediPho->Branch("extrkpt",&extrkpt);
  outputTreediPho->Branch("extrketa",&extrketa);
  outputTreediPho->Branch("extrkphi",&extrkphi);
  

  float eledpt;
  float eledeta;
  float eledphi;
  float eleaco;
  

  //EventMatcher* em = new EventMatcher();
  //Long64_t duplicateEntries = 0;
  
  Long64_t entries = tree->GetEntries();
  Long64_t entriesAnalyzed = 0;   Long64_t id = 0;   Long64_t excl = 0; Long64_t ptcut = 0; Long64_t myloop = 0;
  std::cout << "entries         = " << entries << std::endl;
  std::cout<< "Loop : ggHiNtuplizer/EventTree" <<std::endl;
  
  //for (Long64_t j_entry= 0; j_entry< 2000; ++j_entry){
  for (Long64_t j_entry=0; j_entry < entries; ++j_entry){
    
    if (j_entry % 10000 == 0)  {
      std::cout << "current entry = " <<j_entry<<" out of "<<entries<<" : "<<std::setprecision(2)<<(double)j_entry/entries*100<<" %"<<std::endl;
    }
    if (j_entry>10000) break;
    
    treeHLT->GetEntry(j_entry);
    tree->GetEntry(j_entry);
    treePixel->GetEntry(j_entry);

    outputTreeMC->Fill();
    
    if(HLT_HIUPCL1DoubleEG2NotHF2_v1==1 || HLT_HIUPCL1SingleEG5NotHF2_v1==1){   entriesAnalyzed++; 
      
      outputTreeHLT->Fill(); 
      
      // dielectron block
      eleCharge_1.clear();
      elePt_1.clear();
      eleEta_1.clear();
      elePhi_1.clear();
      eleHoverE_1.clear();
      eleSigmaIEtaIEta_1.clear();
      eleSigmaIEtaIEta_2012_1.clear();
      eleSigmaIPhiIPhi_1.clear();
      eleEoverP_1.clear();
      eledEtaAtVtx_1.clear();
      eledPhiAtVtx_1.clear();
      eledEtaSeedAtVtx_1.clear();
      eleD0_1.clear();
      eleDz_1.clear();
      eleMissHits_1.clear();
      eleTrackIso_1.clear();
      eleHCalIso_1.clear();
      eleECalIso_1.clear();
      eleECalDriven_1.clear();
      eleBrem_1.clear();

      eleCharge_2.clear();
      elePt_2.clear();
      eleEta_2.clear();
      elePhi_2.clear();
      eleHoverE_2.clear();
      eleSigmaIEtaIEta_2.clear();
      eleSigmaIEtaIEta_2012_2.clear();
      eleSigmaIPhiIPhi_2.clear();
      eleEoverP_2.clear();
      eledEtaAtVtx_2.clear();
      eledPhiAtVtx_2.clear();
      eledEtaSeedAtVtx_2.clear();
      eleD0_2.clear();
      eleDz_2.clear();
      eleMissHits_2.clear();
      eleTrackIso_2.clear();
      eleHCalIso_2.clear();
      eleECalIso_2.clear();
      eleECalDriven_2.clear();
      eleBrem_2.clear();
      
      vSum_ee_M.clear();
      vSum_ee_Energy.clear();
      vSum_ee_Pt.clear();
      vSum_ee_Eta.clear();
      vSum_ee_Phi.clear();
      vSum_ee_Rapidity.clear();

      ele_dpt.clear();
      ele_deta.clear();
      ele_dphi.clear();
      ele_aco.clear();	
      
      
      // ged barrel branches
      sc_ged_brl_E_1.clear();
      sc_ged_brl_Et_1.clear();
      sc_ged_brl_Eta_1.clear();
      sc_ged_brl_Phi_1.clear();
      sc_ged_brl_RawE_1.clear();
      sc_ged_brl_RawEt_1.clear();
      sc_ged_brl_raw_eoverp_1.clear();
      sc_ged_brl_eoverp_1.clear();

     
      sc_ged_brl_E_2.clear();
      sc_ged_brl_Et_2.clear();
      sc_ged_brl_Eta_2.clear();
      sc_ged_brl_Phi_2.clear();
      sc_ged_brl_RawE_2.clear();
      sc_ged_brl_RawEt_2.clear();
      sc_ged_brl_raw_eoverp_2.clear();
      sc_ged_brl_eoverp_2.clear();
   
      vSum_ged_brl_reco_M.clear();
      vSum_ged_brl_reco_Energy.clear();
      vSum_ged_brl_reco_Pt.clear();
      vSum_ged_brl_reco_Et.clear();
      vSum_ged_brl_reco_Eta.clear();
      vSum_ged_brl_reco_Phi.clear();
      vSum_ged_brl_reco_Rapidity.clear();
      vSum_ged_brl_reco_Theta.clear();

      vSum_ged_brl_raw_M.clear();
      vSum_ged_brl_raw_Energy.clear();
      vSum_ged_brl_raw_Pt.clear();
      vSum_ged_brl_raw_Et.clear();
      vSum_ged_brl_raw_Eta.clear();
      vSum_ged_brl_raw_Phi.clear();
      vSum_ged_brl_raw_Rapidity.clear();
      vSum_ged_brl_raw_Theta.clear();
      
      ele_ged_brl_Pt_1.clear();
      ele_ged_brl_P_1.clear();
      ele_ged_brl_Eta_1.clear();
      ele_ged_brl_Phi_1.clear();
      ele_ged_brl_Charge_1.clear();

      ele_ged_brl_Pt_2.clear();
      ele_ged_brl_P_2.clear();
      ele_ged_brl_Eta_2.clear();
      ele_ged_brl_Phi_2.clear();
      ele_ged_brl_Charge_2.clear();
      
      vSum_ee_ged_brl_M.clear();
      vSum_ee_ged_brl_Energy.clear();
      vSum_ee_ged_brl_Et.clear();
      vSum_ee_ged_brl_Pt.clear();
      vSum_ee_ged_brl_Eta.clear();
      vSum_ee_ged_brl_Phi.clear();
      vSum_ee_ged_brl_Rapidity.clear();
      vSum_ee_ged_brl_Theta.clear();
 
      
      // ged endcap branches

      sc_ged_end_E_1.clear();
      sc_ged_end_Et_1.clear();
      sc_ged_end_Eta_1.clear();
      sc_ged_end_Phi_1.clear();
      sc_ged_end_RawE_1.clear();
      sc_ged_end_RawEt_1.clear();
      sc_ged_end_raw_eoverp_1.clear();
      sc_ged_end_eoverp_1.clear();

      sc_ged_end_E_2.clear();
      sc_ged_end_Et_2.clear();
      sc_ged_end_Eta_2.clear();
      sc_ged_end_Phi_2.clear();
      sc_ged_end_RawE_2.clear();
      sc_ged_end_RawEt_2.clear();
      sc_ged_end_raw_eoverp_2.clear();
      sc_ged_end_eoverp_2.clear();

   

      vSum_ged_end_reco_M.clear();
      vSum_ged_end_reco_Energy.clear();
      vSum_ged_end_reco_Et.clear();
      vSum_ged_end_reco_Pt.clear();
      vSum_ged_end_reco_Eta.clear();
      vSum_ged_end_reco_Phi.clear();
      vSum_ged_end_reco_Rapidity.clear();
      vSum_ged_end_reco_Theta.clear();

      vSum_ged_end_raw_M.clear();
      vSum_ged_end_raw_Energy.clear();
      vSum_ged_end_raw_Et.clear();
      vSum_ged_end_raw_Pt.clear();
      vSum_ged_end_raw_Eta.clear();
      vSum_ged_end_raw_Phi.clear();
      vSum_ged_end_raw_Rapidity.clear();
      vSum_ged_end_raw_Theta.clear();

      
      ele_ged_end_Pt_1.clear();
      ele_ged_end_P_1.clear();
      ele_ged_end_Eta_1.clear();
      ele_ged_end_Phi_1.clear();
      ele_ged_end_Charge_1.clear();

      ele_ged_end_Pt_2.clear();
      ele_ged_end_P_2.clear();
      ele_ged_end_Eta_2.clear();
      ele_ged_end_Phi_2.clear();
      ele_ged_end_Charge_2.clear();

      vSum_ee_ged_end_M.clear();
      vSum_ee_ged_end_Energy.clear();
      vSum_ee_ged_end_Pt.clear();
      vSum_ee_ged_end_Et.clear();
      vSum_ee_ged_end_Eta.clear();
      vSum_ee_ged_end_Phi.clear();
      vSum_ee_ged_end_Rapidity.clear();
      vSum_ee_ged_end_Theta.clear();
      
      invmass_ged_brl_cluster_reco_formula.clear();
      invmass_ged_brl_cluster_raw_formula.clear();
      invmass_ged_end_cluster_reco_formula.clear();
      invmass_ged_end_cluster_raw_formula.clear();

      allEmEnergy_EB.clear();
      allEmEnergy_EE.clear();
      allHadEnergy_HB.clear();
      allHadEnergy_HE.clear();
      allHadEnergy_HF_Plus.clear();
      allHadEnergy_HF_Minus.clear();
      
      
      leadingEmEnergy_EB.clear();
      leadingEmEnergy_EE.clear();
      leadingHadEnergy_HB.clear();
      leadingHadEnergy_HE.clear();
      leadingHadEnergy_HF_Plus.clear();
      leadingHadEnergy_HF_Minus.clear();
      
      trkpt.clear();
      trketa.clear();
      trkphi.clear();

      nextra_track.clear();
      nextra_track_pt0p06.clear();
      nextra_track_pt0p14.clear();
      nextra_track_pt0p1.clear();

      extrkpt.clear();
      extrketa.clear();
      extrkphi.clear();
      
      
      if(nEle != 2) continue;
      for(int i=0; i<nEle; ++i){
	for (int j=i+1; j<nEle; ++j){
	  
	  if (eleCharge->at(i) == eleCharge->at(j)) continue;
	  
	  eleCharge_1.push_back(eleCharge->at(i));
	  elePt_1.push_back(elePt->at(i));
	  eleEta_1.push_back(eleEta->at(i));
	  elePhi_1.push_back(elePhi->at(i));
	  eleHoverE_1.push_back(eleHoverE->at(i));
	  eleSigmaIEtaIEta_1.push_back(eleSigmaIEtaIEta->at(i));
	  eleSigmaIEtaIEta_2012_1.push_back(eleSigmaIEtaIEta_2012->at(j));
	  eleSigmaIPhiIPhi_1.push_back(eleSigmaIPhiIPhi->at(i));
	  eleEoverP_1.push_back(eleEoverP->at(i));
	  eledEtaAtVtx_1.push_back(eledEtaAtVtx->at(i));
	  eledPhiAtVtx_1.push_back(eledPhiAtVtx->at(i));
	  eledEtaSeedAtVtx_1.push_back(eledEtaSeedAtVtx->at(i));
	  eleD0_1.push_back(eleD0->at(i));
	  eleDz_1.push_back(eleDz->at(i));
	  eleMissHits_1.push_back(eleMissHits->at(i));
	  eleTrackIso_1.push_back(eleTrackIso->at(i));
	  eleHCalIso_1.push_back(eleHCalIso->at(i));
	  eleECalIso_1.push_back(eleECalIso->at(i));
	  eleECalDriven_1.push_back(eleECalDriven->at(i));
          eleBrem_1.push_back(eleBrem->at(i));
	  
	  eleCharge_2.push_back(eleCharge->at(j));
	  elePt_2.push_back(elePt->at(j));
	  eleEta_2.push_back(eleEta->at(j));
	  elePhi_2.push_back(elePhi->at(j));
	  eleHoverE_2.push_back(eleHoverE->at(j));
	  eleSigmaIEtaIEta_2.push_back(eleSigmaIEtaIEta->at(j));
	  eleSigmaIEtaIEta_2012_2.push_back(eleSigmaIEtaIEta_2012->at(j));
	  eleSigmaIPhiIPhi_2.push_back(eleSigmaIPhiIPhi->at(j));
	  eleEoverP_2.push_back(eleEoverP->at(j));
	  eledEtaAtVtx_2.push_back(eledEtaAtVtx->at(j));
	  eledPhiAtVtx_2.push_back(eledPhiAtVtx->at(j));
	  eledEtaSeedAtVtx_2.push_back(eledEtaSeedAtVtx->at(j));
	  eleD0_2.push_back(eleD0->at(j));
	  eleDz_2.push_back(eleDz->at(j));
	  eleMissHits_2.push_back(eleMissHits->at(j));
	  eleTrackIso_2.push_back(eleTrackIso->at(j));
	  eleHCalIso_2.push_back(eleHCalIso->at(j));
	  eleECalIso_2.push_back(eleECalIso->at(j));
	  eleECalDriven_2.push_back(eleECalDriven->at(j));
          eleBrem_2.push_back(eleBrem->at(j));
	  
	  
	  TLorentzVector v1, v2, vSum;
	  v1.SetPtEtaPhiM( elePt->at(i), eleEta->at(i),
			   elePhi->at(i), eleMass);
	  v2.SetPtEtaPhiM( elePt->at(j), eleEta->at(j),
			   elePhi->at(j), eleMass);
	  vSum = v1+v2;
	  
	  vSum_ee_M.push_back(vSum.M());
	  vSum_ee_Energy.push_back(vSum.Energy());
	  vSum_ee_Pt.push_back(vSum.Pt());
	  vSum_ee_Eta.push_back(vSum.Eta());
	  vSum_ee_Phi.push_back(vSum.Phi());
	  vSum_ee_Rapidity.push_back(vSum.Rapidity());
	  
	  eledpt  = fabs(elePt->at(i) - elePt->at(j));
	  eledeta = fabs(eleEta->at(i) - eleEta->at(j));
	  eledphi = getDPHI(elePhi->at(i) ,elePhi->at(j));
	  eleaco = 1- (eledphi/3.141592653589); 
	  
	  ele_dpt.push_back(eledpt);
	  ele_deta.push_back(eledeta);
	  ele_dphi.push_back(eledphi);
	  ele_aco.push_back(eleaco);
	  
          int nextratracks =0; int nextratracks_pt0p06 =0;    int nextratracks_pt0p14 =0;  int nextratracks_pt0p1 =0;
	  cout<<"Entry:" << j_entry<< "  No of tracks:" << ngenTrk ;
	  for(int kk = 0; kk < ngenTrk; ++kk){
	  
	    trkpt.push_back(gentrkPt->at(kk));
	    trketa.push_back(gentrkEta->at(kk));
	    trkphi.push_back(gentrkPhi->at(kk));  

	    
	    if (getDPHI(gentrkPhi->at(kk), elePhi->at(i))<0.7 && getDETA(gentrkEta->at(kk), eleEta->at(i))<0.15) continue;
	    if (getDPHI(gentrkPhi->at(kk), elePhi->at(j))<0.7 && getDETA(gentrkEta->at(kk), eleEta->at(j))<0.15) continue;
	    
	    //allEmEnergy_EB.push_back(CaloTower_emE->at(k));
	    
	    /*if (CaloTower_emE->at(k) > EmEnergy_EB){                         
	      EmEnergy_EB = CaloTower_emE->at(k) ;
	      }*/
	    nextratracks++;
	    if(gentrkPt->at(kk) > 0.06)nextratracks_pt0p06++;
	    if(gentrkPt->at(kk) > 0.14)nextratracks_pt0p14++;
	    if(gentrkPt->at(kk) > 0.1)nextratracks_pt0p1++;


	    extrkpt.push_back(gentrkPt->at(kk));
	    extrketa.push_back(gentrkEta->at(kk));
	    extrkphi.push_back(gentrkPhi->at(kk)); 
	    
	  }
	  cout<<"  Entry:" << j_entry<< "  No of extra tracks:" << nextratracks << endl;
 	  
          //cout << "Event no. " << j_entry << " No. of extra track " <<  nextratracks << endl; 
	  nextra_track.push_back( nextratracks);
	  nextra_track_pt0p06.push_back( nextratracks_pt0p06);
	  nextra_track_pt0p14.push_back( nextratracks_pt0p14);
	  nextra_track_pt0p1.push_back( nextratracks_pt0p1);
	  
	  float  EmEnergy_EB = 0;
	  float  EmEnergy_EE = 0;
	  float  HadEnergy_HB = 0;
	  float  HadEnergy_HE = 0;
	  float  HadEnergy_HF_Plus = 0;
	  float  HadEnergy_HF_Minus = 0;
	  
	  if(j_entry==69)cout<< "photon stuff finished and ntower is: " <<nTower<< endl;   
	  for(int k = 0; k< nTower; ++k){
	    
	    if (fabs(CaloTower_eta->at(k)) < 1.479){ //start filling EM tower in barel region  
	      if (getDPHI(CaloTower_phi->at(k), elePhi->at(i))<0.7 && getDETA(CaloTower_eta->at(k), eleEta->at(i))<0.15) continue;
	      if (getDPHI(CaloTower_phi->at(k), elePhi->at(j))<0.7 && getDETA(CaloTower_eta->at(k), eleEta->at(j))<0.15) continue;
	      
	      allEmEnergy_EB.push_back(CaloTower_emE->at(k));
	      
	      if (CaloTower_emE->at(k) > EmEnergy_EB){                         
		EmEnergy_EB = CaloTower_emE->at(k) ;
	      }
	    }
	    
	    if (fabs(CaloTower_eta->at(k)) > 1.479 && fabs(CaloTower_eta->at(k)) < 3 ){           
	      if (getDPHI(CaloTower_phi->at(k), elePhi->at(i))<0.4 && getDETA(CaloTower_eta->at(k), eleEta->at(i))<0.15) continue;
	      if (getDPHI(CaloTower_phi->at(k), elePhi->at(j))<0.4 && getDETA(CaloTower_eta->at(k), eleEta->at(j))<0.15) continue;
	      allEmEnergy_EE.push_back(CaloTower_emE->at(k));
	      if (CaloTower_emE->at(k) > EmEnergy_EE){                         
		EmEnergy_EE = CaloTower_emE->at(k) ;
	      }
	      
	    }
	    
	    if (fabs(CaloTower_eta->at(k)) < 1.305){           
	      allHadEnergy_HB.push_back(CaloTower_hadE->at(k));
	      if (CaloTower_hadE->at(k) > HadEnergy_HB){                         
		HadEnergy_HB = CaloTower_hadE->at(k) ;
	      }
	    }
	    
	    if (fabs(CaloTower_eta->at(k)) > 1.305 && fabs(CaloTower_eta->at(k)) < 3 ){           
	      allHadEnergy_HE.push_back(CaloTower_hadE->at(k));
	      if (CaloTower_hadE->at(k) > HadEnergy_HE){                         
		HadEnergy_HE = CaloTower_hadE->at(k) ;
	      }
	      
	    }
	    
	    if (CaloTower_eta->at(k) > 2.9 && CaloTower_eta->at(k) < 5.2 ){           
	      allHadEnergy_HF_Plus.push_back(CaloTower_e->at(k));
	      if (CaloTower_e->at(k) > HadEnergy_HF_Plus){                         
		HadEnergy_HF_Plus = CaloTower_e->at(k) ;
	      }
	    }
	    
	    
	    if (CaloTower_eta->at(k) < -2.9 && CaloTower_eta->at(k) > -5.2 ){           
	      allHadEnergy_HF_Minus.push_back(CaloTower_e->at(k));
	      if (CaloTower_e->at(k) > HadEnergy_HF_Minus){                         
		HadEnergy_HF_Minus = CaloTower_e->at(k) ;
	      }
	    }
	    
	  } //ntower
	  
	  leadingEmEnergy_EB.push_back(EmEnergy_EB);
	  leadingEmEnergy_EE.push_back(EmEnergy_EE);
	  leadingHadEnergy_HB.push_back(HadEnergy_HB);
	  leadingHadEnergy_HE.push_back(HadEnergy_HE);
	  leadingHadEnergy_HF_Plus.push_back(HadEnergy_HF_Plus);
	  leadingHadEnergy_HF_Minus.push_back(HadEnergy_HF_Minus);
	  
	  pix_hit1 = nhits1;
	  pix_hit2 = nhits2;
	  pix_hit3 = nhits3;
	  pix_hit4 = nhits4;
	  pix_hit5 = nhits5;
	  
	  
	  double cluster_raw_energy; 
    	  double cluster_eta;
    	  double cluster_phi;
	  
          double raw_eoverp, eoverp;
	  
 	  double deta_1=0, dphi_1 = 0 ,deta_2 = 0 , dphi_2 = 0;
	  
          //if(eleaco < 0.05){
	    
	    if (TMath::Abs(eleEta->at(i)) < cutEta && TMath::Abs(eleEta->at(j)) < cutEta) {  // if ele is in barel
	      
	      deta_1 = fabs(eledEtaSeedAtVtx->at(i)); 
	      dphi_1 = fabs(eledPhiAtVtx->at(i)); 
	      
	      deta_2 = fabs(eledEtaSeedAtVtx->at(j)); 
	      dphi_2 = fabs(eledPhiAtVtx->at(j)); 
	      
	      double sc_ged_brl_et_1 = 0, sc_ged_brl_et_2 = 0,    sc_ged_brl_eta_1 = 0, sc_ged_brl_eta_2 = 0;
	      double sc_ged_brl_phi_1 = 0, sc_ged_brl_phi_2 = 0, sc_ged_brl_energy_1= 0,  sc_ged_brl_energy_2 = 0;
	      double sc_ged_brl_raw_et_1 = 0, sc_ged_brl_raw_et_2 = 0, sc_ged_brl_raw_energy_1= 0,  sc_ged_brl_raw_energy_2 = 0;
	      
	      double ele_pt_1 = 0, ele_pt_2 = 0,    ele_eta_1 = 0, ele_eta_2 = 0;
	      double ele_phi_1 = 0, ele_phi_2 = 0, ele_energy_1= 0,  ele_energy_2 = 0;
	      
              bool ele1_cluster_match = 0, ele2_cluster_match = 0;  
	      
	      if(deta_1 < 0.05 && dphi_1 < 0.5 ){ // supercluster-track matching
		//nsc_ged_brl_1.push_back(NClusters->at(i));
		sc_ged_brl_E_1.push_back(eleSCEn->at(i));
		sc_ged_brl_Et_1.push_back(eleSCEn->at(i)/cosh(eleSCEta->at(i)));
		sc_ged_brl_Eta_1.push_back(eleSCEta->at(i));
		sc_ged_brl_Phi_1.push_back(eleSCPhi->at(i));
		sc_ged_brl_RawE_1.push_back(eleSCRawEn->at(i));
		sc_ged_brl_RawEt_1.push_back(eleSCRawEn->at(i)/cosh(eleSCEta->at(i)));
		
		ele_ged_brl_Pt_1.push_back(elePt->at(i));
		ele_ged_brl_Eta_1.push_back(eleEta->at(i));
		ele_ged_brl_Phi_1.push_back(elePhi->at(i));
		ele_ged_brl_Charge_1.push_back(eleCharge->at(i));
		
	        sc_ged_brl_raw_eoverp_1.push_back(eleSCRawEn->at(i)/eleP->at(i));
		sc_ged_brl_eoverp_1.push_back(eleEoverP->at(i));
		
		sc_ged_brl_et_1  = eleSCEn->at(i)/cosh(eleSCEta->at(i));
		sc_ged_brl_eta_1 = eleSCEta->at(i);
		sc_ged_brl_phi_1 = eleSCPhi->at(i);
		sc_ged_brl_energy_1 = eleSCRawEn->at(i);
		sc_ged_brl_raw_et_1  = eleSCRawEn->at(i)/cosh(eleSCEta->at(i));
		sc_ged_brl_raw_energy_1 = eleSCRawEn->at(i);
		
		ele_pt_1  = elePt->at(i);
		ele_eta_1  = eleEta->at(i);
		ele_phi_1  = elePhi->at(i);
		
		ele1_cluster_match = true;
      
		//cout << "hybrid eta:" << sc_ged_brl_eta_1 << "  track eta:" << ele_eta_1 << " deta_1: " << deta_1 << endl;
		//cout << "hybrid phi:" << sc_ged_brl_phi_1 << "  track phi:" << ele_phi_1 << " dphi_1: " << dphi_1 << endl;
		
	      }//if deta and dphi in cone then fill Eoverp
	      
	      if(deta_2 < 0.05 && dphi_2 < 0.5 ){
		//nsc_ged_brl_2.push_back(NClusters->at(i));
		sc_ged_brl_E_2.push_back(eleSCEn->at(j));
		sc_ged_brl_Et_2.push_back(eleSCEn->at(j)/cosh(eleSCEta->at(j)));
		sc_ged_brl_Eta_2.push_back(eleSCEta->at(j));
		sc_ged_brl_Phi_2.push_back(eleSCPhi->at(j));
		sc_ged_brl_RawE_2.push_back(eleSCRawEn->at(j));
		sc_ged_brl_RawEt_2.push_back(eleSCRawEn->at(j)/cosh(eleSCEta->at(j)));
		
		ele_ged_brl_Pt_2.push_back(elePt->at(j));
		ele_ged_brl_Eta_2.push_back(eleEta->at(j));
		ele_ged_brl_Phi_2.push_back(elePhi->at(j));
		ele_ged_brl_Charge_2.push_back(eleCharge->at(j));
		
	        sc_ged_brl_raw_eoverp_2.push_back(eleSCRawEn->at(j)/eleP->at(j));
		sc_ged_brl_eoverp_2.push_back(eleEoverP->at(j));
		
		sc_ged_brl_et_2  = eleSCEn->at(j)/cosh(eleSCEta->at(j));
		sc_ged_brl_eta_2 = eleSCEta->at(j);
		sc_ged_brl_phi_2 = eleSCPhi->at(j);
		sc_ged_brl_energy_2 = eleSCRawEn->at(j);
		sc_ged_brl_raw_et_1  = eleSCRawEn->at(j)/cosh(eleSCEta->at(j));
		sc_ged_brl_raw_energy_1 = eleSCRawEn->at(j);
		
		ele_pt_2  = elePt->at(j);
		ele_eta_2  = eleEta->at(j);
		ele_phi_2  = elePhi->at(j);
		
		ele2_cluster_match = true;
                //cout << "coming in eta2 loop" << endl;
		//cout << "hybrid eta:" << sc_ged_brl_eta_2 << "  track eta:" << ele_eta_2 << " deta_2: " << deta_2 << endl;
		//cout << "hybrid phi:" << sc_ged_brl_phi_2 << "  track phi:" << ele_phi_2 << " dphi_2: " << dphi_2 << endl;
		
	      }//if deta and dphi in cone then fill Eoverp


	      if(ele1_cluster_match && ele2_cluster_match){
	       
               // fill di-cluster info with reco energy inputs	
		TLorentzVector v3, v4, vSum2;
		v3.SetPtEtaPhiE( sc_ged_brl_et_1 ,  sc_ged_brl_eta_1,
				 sc_ged_brl_phi_1,  sc_ged_brl_energy_1);
		v4.SetPtEtaPhiE( sc_ged_brl_et_2 ,  sc_ged_brl_eta_2,
				 sc_ged_brl_phi_2,  sc_ged_brl_energy_2);
		vSum2 = v3+v4;
		
		vSum_ged_brl_reco_M.push_back(vSum2.M());
		vSum_ged_brl_reco_Energy.push_back(vSum2.Energy());
		vSum_ged_brl_reco_Et.push_back(vSum2.Et());
		vSum_ged_brl_reco_Pt.push_back(vSum2.Pt());
		vSum_ged_brl_reco_Eta.push_back(vSum2.Eta());
		vSum_ged_brl_reco_Phi.push_back(vSum2.Phi());
		vSum_ged_brl_reco_Rapidity.push_back(vSum2.Rapidity());
		
		double ged_brl_theta = v3.Angle(v4.Vect()); 
		vSum_ged_brl_reco_Theta.push_back(ged_brl_theta);

		double dicluster_invmass = getInvMass(sc_ged_brl_energy_1, sc_ged_brl_eta_1, sc_ged_brl_phi_1,  
						      sc_ged_brl_energy_2, sc_ged_brl_eta_2, sc_ged_brl_phi_2);  

		invmass_ged_brl_cluster_reco_formula.push_back(dicluster_invmass);

                // fill di-cluster info with raw energy inputs
		TLorentzVector v3_raw, v4_raw, vSum2_raw;
		v3_raw.SetPtEtaPhiE( sc_ged_brl_raw_et_1 ,  sc_ged_brl_eta_1, 
				     sc_ged_brl_phi_1    ,  sc_ged_brl_raw_energy_1);

		v4_raw.SetPtEtaPhiE( sc_ged_brl_raw_et_2 ,  sc_ged_brl_eta_2,
				     sc_ged_brl_phi_2    ,   sc_ged_brl_raw_energy_2);

		vSum2_raw = v3_raw + v4_raw;
		
		vSum_ged_brl_raw_M.push_back(vSum2_raw.M());
		vSum_ged_brl_raw_Energy.push_back(vSum2_raw.Energy());
		vSum_ged_brl_raw_Et.push_back(vSum2_raw.Et());
		vSum_ged_brl_raw_Pt.push_back(vSum2_raw.Pt());
		vSum_ged_brl_raw_Eta.push_back(vSum2_raw.Eta());
		vSum_ged_brl_raw_Phi.push_back(vSum2_raw.Phi());
		vSum_ged_brl_raw_Rapidity.push_back(vSum2_raw.Rapidity());
		
		double ged_brl_theta_raw = v3_raw.Angle(v4_raw.Vect()); 
		vSum_ged_brl_raw_Theta.push_back(ged_brl_theta_raw);
		
 		double dicluster_raw_invmass = getInvMass(sc_ged_brl_raw_energy_1, sc_ged_brl_eta_1, sc_ged_brl_phi_1,  
						          sc_ged_brl_raw_energy_2, sc_ged_brl_eta_2, sc_ged_brl_phi_2);  

		invmass_ged_brl_cluster_raw_formula.push_back(dicluster_raw_invmass);
		
                // fill dielectron info for electrons matched to clusters
		TLorentzVector v5, v6, vSum3;
		v5.SetPtEtaPhiM( ele_pt_1  ,  ele_eta_1,
				 ele_phi_1 ,  eleMass);
		v6.SetPtEtaPhiM( ele_pt_2  ,  ele_eta_2,
			       ele_phi_2 ,  eleMass);
		vSum3 = v5+v6;
		
		vSum_ee_ged_brl_M.push_back(vSum3.M());
		vSum_ee_ged_brl_Energy.push_back(vSum3.Energy());
		vSum_ee_ged_brl_Pt.push_back(vSum3.Pt());
		vSum_ee_ged_brl_Et.push_back(vSum3.Et());
		vSum_ee_ged_brl_Eta.push_back(vSum3.Eta());
		vSum_ee_ged_brl_Phi.push_back(vSum3.Phi());
		vSum_ee_ged_brl_Rapidity.push_back(vSum3.Rapidity());
		
		double ee_ged_brl_theta = v5.Angle(v6.Vect()); 
		vSum_ee_ged_brl_Theta.push_back(ee_ged_brl_theta);



		//cout << "Entry:" << j_entry << "  no. of clusters:" << NClusters->at(i) << endl;
	      } // fill di-cluster info if both electron matches to cluster

	    } // barrel region
	    
	    
	    
	    /// start endcap region here /////////////////////////////////
	    
	    if (TMath::Abs(eleEta->at(i)) > cutEta && TMath::Abs(eleEta->at(j)) > cutEta) {  // if ele is in barel
	      	      
	      double sc_ged_end_et_1 = 0, sc_ged_end_et_2 = 0,    sc_ged_end_eta_1 = 0, sc_ged_end_eta_2 = 0;
	      double sc_ged_end_phi_1 = 0, sc_ged_end_phi_2 = 0, sc_ged_end_energy_1= 0,  sc_ged_end_energy_2 = 0;
	      double sc_ged_end_raw_et_1 = 0, sc_ged_end_raw_et_2 = 0, sc_ged_end_raw_energy_1= 0,  sc_ged_end_raw_energy_2 = 0;
	      
	      double ele_end_pt_1 = 0, ele_end_pt_2 = 0,    ele_end_eta_1 = 0, ele_end_eta_2 = 0;
	      double ele_end_phi_1 = 0, ele_end_phi_2 = 0, ele_end_energy_1= 0,  ele_end_energy_2 = 0;
	      
	      bool ele1_ged_end_match = 0, ele2_ged_end_match = 0;  
	      
	      deta_1 = fabs(eledEtaSeedAtVtx->at(i)); 
	      dphi_1 = fabs(eledPhiAtVtx->at(i)); 
	      
	      deta_2 = fabs(eledEtaSeedAtVtx->at(j)); 
	      dphi_2 = fabs(eledPhiAtVtx->at(j)); 
	      
	      if(deta_1 < 0.05 && dphi_1 < 0.5 ){

		sc_ged_end_E_1.push_back(eleSCEn->at(i));
		sc_ged_end_Et_1.push_back(eleSCEn->at(i)/cosh(eleSCEta->at(i)));
		sc_ged_end_Eta_1.push_back(eleSCEta->at(i));
		sc_ged_end_Phi_1.push_back(eleSCPhi->at(i));
		sc_ged_end_RawE_1.push_back(eleSCRawEn->at(i));
		sc_ged_end_RawEt_1.push_back(eleSCRawEn->at(i)/cosh(eleSCEta->at(i)));
		
		
		ele_ged_end_Pt_1.push_back(elePt->at(i));
		ele_ged_end_Eta_1.push_back(eleEta->at(i));
		ele_ged_end_Phi_1.push_back(elePhi->at(i));
		ele_ged_end_Charge_1.push_back(eleCharge->at(i));

	        sc_ged_end_raw_eoverp_1.push_back(eleSCRawEn->at(i)/eleP->at(i));
		sc_ged_end_eoverp_1.push_back(eleEoverP->at(i));

		sc_ged_end_et_1  = eleSCEn->at(i)/cosh(eleSCEta->at(i));
		sc_ged_end_eta_1 = eleSCEta->at(i);
		sc_ged_end_phi_1 = eleSCPhi->at(i);
		sc_ged_end_energy_1 = eleSCRawEn->at(i);
		sc_ged_end_raw_et_1  = eleSCRawEn->at(i)/cosh(eleSCEta->at(i));
		sc_ged_end_raw_energy_1 = eleSCRawEn->at(i);
		
		ele_end_pt_1   = elePt->at(i);
		ele_end_eta_1  = eleEta->at(i);
		ele_end_phi_1  = elePhi->at(i);
		
		ele1_ged_end_match = true;
	      }//if deta and dphi in cone then fill Eoverp
	      
	      if(deta_2 < 0.05 && dphi_2 < 0.5 ){
				

                sc_ged_end_E_2.push_back(eleSCEn->at(j));
		sc_ged_end_Et_2.push_back(eleSCEn->at(j)/cosh(eleSCEta->at(j)));
		sc_ged_end_Eta_2.push_back(eleSCEta->at(j));
		sc_ged_end_Phi_2.push_back(eleSCPhi->at(j));
		sc_ged_end_RawE_2.push_back(eleSCRawEn->at(j));
		sc_ged_end_RawEt_2.push_back(eleSCRawEn->at(j)/cosh(eleSCEta->at(j)));

		ele_ged_end_Pt_2.push_back(elePt->at(j));
		ele_ged_end_Eta_2.push_back(eleEta->at(j));
		ele_ged_end_Phi_2.push_back(elePhi->at(j));
		ele_ged_end_Charge_2.push_back(eleCharge->at(j));
		
	        sc_ged_end_raw_eoverp_2.push_back(eleSCRawEn->at(j)/eleP->at(j));
		sc_ged_end_eoverp_2.push_back(eleEoverP->at(j));

		sc_ged_end_et_2  = eleSCEn->at(j)/cosh(eleSCEta->at(j));
		sc_ged_end_eta_2 = eleSCEta->at(j);
		sc_ged_end_phi_2 = eleSCPhi->at(j);
		sc_ged_end_energy_2 = eleSCRawEn->at(j);
		sc_ged_end_raw_et_2  = eleSCRawEn->at(j)/cosh(eleSCEta->at(j));
		sc_ged_end_raw_energy_2 = eleSCRawEn->at(j);
		
		ele_end_pt_2   = elePt->at(j);
		ele_end_eta_2  = eleEta->at(j);
		ele_end_phi_2  = elePhi->at(j);
		
		ele2_ged_end_match = true;
	      }//if deta and dphi in cone then fill Eoverp

	      if(ele1_ged_end_match && ele2_ged_end_match){
	       
               // fill di-cluster info with reco energy inputs	
		TLorentzVector v7, v8, vSum4;
		v7.SetPtEtaPhiE( sc_ged_end_et_1 ,  sc_ged_end_eta_1,
				 sc_ged_end_phi_1,  sc_ged_end_energy_1);
		v8.SetPtEtaPhiE( sc_ged_end_et_2 ,  sc_ged_end_eta_2,
				 sc_ged_end_phi_2,  sc_ged_end_energy_2);
		vSum4 = v7+v8;
		
		vSum_ged_end_reco_M.push_back(vSum4.M());
		vSum_ged_end_reco_Energy.push_back(vSum4.Energy());
		vSum_ged_end_reco_Et.push_back(vSum4.Et());
		vSum_ged_end_reco_Pt.push_back(vSum4.Pt());
		vSum_ged_end_reco_Eta.push_back(vSum4.Eta());
		vSum_ged_end_reco_Phi.push_back(vSum4.Phi());
		vSum_ged_end_reco_Rapidity.push_back(vSum4.Rapidity());
		
		double ged_end_theta = v7.Angle(v8.Vect()); 
		vSum_ged_end_reco_Theta.push_back(ged_end_theta);

		double dicluster_ged_end_invmass = getInvMass(sc_ged_end_energy_1, sc_ged_end_eta_1, sc_ged_end_phi_1,  
					            sc_ged_end_energy_2, sc_ged_end_eta_2, sc_ged_end_phi_2);  

		invmass_ged_end_cluster_reco_formula.push_back(dicluster_ged_end_invmass);


                // fill di-cluster info with raw energy inputs
		TLorentzVector v7_raw, v8_raw, vSum4_raw;
		v7_raw.SetPtEtaPhiE( sc_ged_end_raw_et_1 ,  sc_ged_end_eta_1, 
				     sc_ged_end_phi_1    ,  sc_ged_end_raw_energy_1);

		v8_raw.SetPtEtaPhiE( sc_ged_end_raw_et_2 ,  sc_ged_end_eta_2,
				     sc_ged_end_phi_2    ,  sc_ged_end_raw_energy_2);

		vSum4_raw = v7_raw + v8_raw;
		
		vSum_ged_end_raw_M.push_back(vSum4_raw.M());
		vSum_ged_end_raw_Energy.push_back(vSum4_raw.Energy());
		vSum_ged_end_raw_Et.push_back(vSum4_raw.Et());
		vSum_ged_end_raw_Pt.push_back(vSum4_raw.Pt());
		vSum_ged_end_raw_Eta.push_back(vSum4_raw.Eta());
		vSum_ged_end_raw_Phi.push_back(vSum4_raw.Phi());
		vSum_ged_end_raw_Rapidity.push_back(vSum4_raw.Rapidity());
		
		double ged_end_theta_raw = v7_raw.Angle(v8_raw.Vect()); 
		vSum_ged_end_raw_Theta.push_back(ged_end_theta_raw);

		double dicluster_ged_end_raw_invmass = getInvMass(sc_ged_end_energy_1, sc_ged_end_eta_1, sc_ged_end_phi_1,  
					            sc_ged_end_energy_2, sc_ged_end_eta_2, sc_ged_end_phi_2);  

		invmass_ged_end_cluster_raw_formula.push_back(dicluster_ged_end_raw_invmass);
		
		
                // fill dielectron info for electrons matched to clusters
		TLorentzVector v9, v10, vSum5;
		v9.SetPtEtaPhiM( ele_end_pt_1  ,  ele_end_eta_1,
				 ele_end_phi_1 ,  eleMass);
		v10.SetPtEtaPhiM( ele_end_pt_2 ,  ele_end_eta_2,
			       ele_end_phi_2 ,  eleMass);
		vSum5 = v9+v10;
		
		vSum_ee_ged_end_M.push_back(vSum5.M());
		vSum_ee_ged_end_Energy.push_back(vSum5.Energy());
		vSum_ee_ged_end_Et.push_back(vSum5.Et());
		vSum_ee_ged_end_Pt.push_back(vSum5.Pt());
		vSum_ee_ged_end_Eta.push_back(vSum5.Eta());
		vSum_ee_ged_end_Phi.push_back(vSum5.Phi());
		vSum_ee_ged_end_Rapidity.push_back(vSum5.Rapidity());
		
		double ee_ged_end_theta = v9.Angle(v10.Vect()); 
		vSum_ee_ged_end_Theta.push_back(ee_ged_end_theta);
	      } // fill di-cluster info if both electron matches to cluster


	    } // endcap region
	 // } //electron acoplanarity	    
	}// ele j
      }//ele i
      
      outputTreePho->Fill();
      outputTreediPho->Fill();
        
    }
  }
  std::cout<<  "Loop ENDED : ggHiNtuplizer/EventTree" <<std::endl;
  std::cout << "entries            = " << entries << std::endl;
  
  std::cout << "entriesAnalyzed  after trigger  = " << entriesAnalyzed << std::endl;
  std::cout << "after 2 photon exclusive cut    = " << excl << std::endl;
  std::cout << "for reconstruction number       = " << myloop << std::endl;
  std::cout << "photons after applying ieta id  = " << id << std::endl;
  std::cout << "outputTreeMC->GetEntries()   = " << outputTreeMC->GetEntries() << std::endl;
  std::cout << "outputTreeHLT->GetEntries()   = " << outputTreeHLT->GetEntries() << std::endl;
  std::cout << "outputTreediPho->GetEntries()    = " << outputTreediPho->GetEntries() << std::endl;
  std::cout << "outputTreediEle->GetEntries()    = " << outputTreediEle->GetEntries() << std::endl;
  
  output->cd();
  hsingle_pt->Write();
  hsingle_eta->Write();
  hsingle_phi->Write();
  hdouble_pt->Write();
  hdouble_rap->Write();
  hdouble_mass->Write();
  hdpt->Write();
  hdeta->Write();
  hdphi->Write();
  haco->Write();
  output->Write();
  output->Close();
}

int main(int argc, char *argv[])
{ 
  data_ged_tree_sc_corr(argv[1]);
  //mycheck_ele_tree(argv[3],argv[4]);
  return 0;
}

Double_t getDR( Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2){
  Double_t theDphi = getDPHI( phi1, phi2);
  Double_t theDeta = eta1 - eta2;
  return TMath::Sqrt ( theDphi*theDphi + theDeta*theDeta);
}

Double_t getDPHI( Double_t phi1, Double_t phi2) {
  return TVector2::Phi_mpi_pi(phi1-phi2);
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


