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

void data_hm_tree_sc_corr( std::string infile_Forest = "filelist_28.txt",
			 std::string out = "test.root") 
{

  TH1::SetDefaultSumw2();
  
  std::ifstream instr_Forest(infile_Forest.c_str(),std::ifstream::in);
  std::string filename_Forest;

  instr_Forest>>filename_Forest;
  cout << "opening file " << filename_Forest << endl;
  TFile *fin = TFile::Open(filename_Forest.c_str());

  TTree *treeHLT   = (TTree*)fin->Get("hltanalysis/HltTree");
  TTree *tree      = (TTree*)fin->Get("ggHiNtuplizer/EventTree");
  TTree *treePixel = (TTree*)fin->Get("pixel/PixelTree");

  TH1D* htrigger_Ereco_before_trigg = new TH1D("htrigger_Ereco_before_trigg","",20,0,10);
  TH1D* htrigger_Ereco_after_trigg  = new TH1D("htrigger_Ereco_after_trigg","",20,0,10);
  TH1D* htrigger_eff_vs_Ereco   = new TH1D("htrigger_eff_vs_Ereco","",20,0,10);
  
 
  TH1D* hsingle_pt   = new TH1D("hsingle_pt","",20,0,10);
  TH1D* hsingle_eta  = new TH1D("hsingle_eta","",6,-2.4,2.4);
  TH1D* hsingle_phi  = new TH1D("hsingle_phi","",8,-4,4);
  
  TH1D* hdpt   = new TH1D("hdpt","",10,0,5);
  TH1D* hdeta  = new TH1D("hdeta","",100,0,0.1);
  TH1D* hdphi  = new TH1D("hdphi","",100,0,0.5);
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
  int mcPID[MAXMC];
  float mcPt[MAXMC];
  float mcEta[MAXMC];
  float mcPhi[MAXMC];
  float mcMass[MAXMC];
  tree->SetBranchStatus("nMC",1); tree->SetBranchAddress("nMC",&nMC);
  tree->SetBranchStatus("mcPID",1); tree->SetBranchAddress("mcPID",mcPID);
  tree->SetBranchStatus("mcPt",1); tree->SetBranchAddress("mcPt",mcPt);
  tree->SetBranchStatus("mcEta",1); tree->SetBranchAddress("mcEta",mcEta);
  tree->SetBranchStatus("mcPhi",1); tree->SetBranchAddress("mcPhi",mcPhi);
  tree->SetBranchStatus("mcMass",1); tree->SetBranchAddress("mcMass",mcMass);
 
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
  

  // GSF electron tracks
  Int_t ngsfEle;
  std::vector<float>* elegsfTrkPt=0;
  std::vector<float>* elegsfTrkP=0;
  std::vector<float>* elegsfTrkEta=0;
  std::vector<float>* elegsfTrkPhi=0;
  std::vector<int>*   elegsfTrkCharge=0;
  std::vector<float>* elegsfTrkChi2=0;
  std::vector<float>* elegsfTrkNdof=0;
  std::vector<float>* elegsfTrkNormalizedChi2=0;
  std::vector<int>*   elegsfTrkValidHits=0;
  std::vector<int>*   elegsfTrkMissHits=0;
  std::vector<int>*   elegsfTrkLayers=0;
  std::vector<float>* elegsfD0=0;  
  std::vector<float>* elegsfDz=0;  
  std::vector<float>* elegsfD0Err=0;  
  std::vector<float>* elegsfDzErr=0;  

  tree->SetBranchStatus("ngsfEle",1);     // enable electron branches
  tree->SetBranchAddress("ngsfEle",&ngsfEle);
  tree->SetBranchStatus("elegsf*",1);     // enable electron branches
  tree->SetBranchAddress("elegsfTrkPt",&elegsfTrkPt);
  tree->SetBranchAddress("elegsfTrkP",&elegsfTrkP);
  tree->SetBranchAddress("elegsfTrkEta",&elegsfTrkEta);
  tree->SetBranchAddress("elegsfTrkPhi",&elegsfTrkPhi);
  tree->SetBranchAddress("elegsfTrkCharge",&elegsfTrkCharge);
  tree->SetBranchAddress("elegsfTrkChi2",&elegsfTrkChi2);
  tree->SetBranchAddress("elegsfTrkNdof",&elegsfTrkNdof);
  tree->SetBranchAddress("elegsfTrkNormalizedChi2",&elegsfTrkNormalizedChi2);
  tree->SetBranchAddress("elegsfTrkValidHits",&elegsfTrkValidHits);
  tree->SetBranchAddress("elegsfTrkMissHits",&elegsfTrkMissHits);
  tree->SetBranchAddress("elegsfTrkLayers",&elegsfTrkLayers);
  tree->SetBranchAddress("elegsfD0",&elegsfD0);
  tree->SetBranchAddress("elegsfDz",&elegsfDz);
  tree->SetBranchAddress("elegsfD0Err",&elegsfD0Err);
  tree->SetBranchAddress("elegsfDzErr",&elegsfDzErr);

  // hybrid super cluster collection ///
  Int_t          nsc_hybrid;
  vector<float>*  sc_hybrid_E=0;
  vector<float>*  sc_hybrid_Et=0;
  vector<float>*  sc_hybrid_Eta=0;
  vector<float>*  sc_hybrid_Phi=0;
  vector<float>*  sc_hybrid_x=0;
  vector<float>*  sc_hybrid_y=0;
  vector<float>*  sc_hybrid_z=0;
  vector<float>*  sc_hybrid_EtaWidth=0;
  vector<float>*  sc_hybrid_PhiWidth=0;
  vector<float>*  sc_hybrid_RawE=0;
  vector<float>*  sc_hybrid_RawEt=0;
  
  tree->SetBranchStatus("nsc_*",1);     // enable photon branches
  tree->SetBranchAddress("nsc_hybrid",&nsc_hybrid);
  tree->SetBranchStatus("sc*",1);
  tree->SetBranchAddress("sc_hybrid_E",&sc_hybrid_E);
  tree->SetBranchAddress("sc_hybrid_Et",&sc_hybrid_Et);
  tree->SetBranchAddress("sc_hybrid_Eta",&sc_hybrid_Eta);
  tree->SetBranchAddress("sc_hybrid_Phi",&sc_hybrid_Phi);
  tree->SetBranchAddress("sc_hybrid_x",&sc_hybrid_x);
  tree->SetBranchAddress("sc_hybrid_y",&sc_hybrid_y);
  tree->SetBranchAddress("sc_hybrid_z",&sc_hybrid_z);
  tree->SetBranchAddress("sc_hybrid_EtaWidth",&sc_hybrid_EtaWidth);
  tree->SetBranchAddress("sc_hybrid_PhiWidth",&sc_hybrid_PhiWidth);
  tree->SetBranchAddress("sc_hybrid_RawE",&sc_hybrid_RawE);
  tree->SetBranchAddress("sc_hybrid_RawEt",&sc_hybrid_RawEt);
  
  //reco::mult55 supercluster
  Int_t          nsc_mult55;
  vector<float>*  sc_mult55_E=0;
  vector<float>*  sc_mult55_Et=0;
  vector<float>*  sc_mult55_Eta=0;
  vector<float>*  sc_mult55_Phi=0;
  vector<float>*  sc_mult55_x=0;
  vector<float>*  sc_mult55_y=0;
  vector<float>*  sc_mult55_z=0;
  vector<float>*  sc_mult55_EtaWidth=0;
  vector<float>*  sc_mult55_PhiWidth=0;
  vector<float>*  sc_mult55_RawE=0;
  vector<float>*  sc_mult55_RawEt=0;

  tree->SetBranchAddress("nsc_mult55",&nsc_mult55);
  tree->SetBranchAddress("sc_mult55_E",&sc_mult55_E);
  tree->SetBranchAddress("sc_mult55_Et",&sc_mult55_Et);
  tree->SetBranchAddress("sc_mult55_Eta",&sc_mult55_Eta);
  tree->SetBranchAddress("sc_mult55_Phi",&sc_mult55_Phi);
  tree->SetBranchAddress("sc_mult55_x",&sc_mult55_x);
  tree->SetBranchAddress("sc_mult55_y",&sc_mult55_y);
  tree->SetBranchAddress("sc_mult55_z",&sc_mult55_z);
  tree->SetBranchAddress("sc_mult55_EtaWidth",&sc_mult55_EtaWidth);
  tree->SetBranchAddress("sc_mult55_PhiWidth",&sc_mult55_PhiWidth);
  tree->SetBranchAddress("sc_mult55_RawE",&sc_mult55_RawE);
  tree->SetBranchAddress("sc_mult55_RawEt",&sc_mult55_RawEt);


  // RECO muons
  Int_t nMu;
  
  tree->SetBranchStatus("nMu",1);     // enable muon branches
  tree->SetBranchAddress("nMu",&nMu);
  
 // Calo tower tree ////////////////////////////////////////////

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

   /// Pixel tree ///////////////////////////////////////////  
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
  outputTreeMC->Branch("mcPID",mcPID,"mcPID[nMC]/I");
  outputTreeMC->Branch("mcPt",mcPt,"mcPt[nMC]/F");
  outputTreeMC->Branch("mcEta",mcEta,"mcEta[nMC]/F");
  outputTreeMC->Branch("mcPhi",mcPhi,"mcPhi[nMC]/F");
  outputTreeMC->Branch("mcMass",mcMass,"mcMass[nMC]/F");

  outputTreediPho->Branch("run",&run);
  outputTreediPho->Branch("event",&event);
  outputTreediPho->Branch("lumis",&lumis);
  
  outputTreediPho->Branch("nVtx",&nVtx);
  outputTreediPho->Branch("xVtx",&xVtx);
  outputTreediPho->Branch("yVtx",&yVtx);
  outputTreediPho->Branch("zVtx",&zVtx);
  outputTreediPho->Branch("isFakeVtx",&isFakeVtx);
  
  std::vector<float> elegsfTrkPt_1;
  std::vector<float> elegsfTrkP_1;
  std::vector<float> elegsfTrkEta_1;
  std::vector<float> elegsfTrkPhi_1;
  std::vector<int>   elegsfTrkCharge_1;
  std::vector<float> elegsfTrkChi2_1;
  std::vector<float> elegsfTrkNdof_1;
  std::vector<float> elegsfTrkNormalizedChi2_1;
  std::vector<int>   elegsfTrkValidHits_1;
  std::vector<int>   elegsfTrkMissHits_1;
  std::vector<int>   elegsfTrkLayers_1;
  std::vector<float> elegsfD0_1;  
  std::vector<float> elegsfDz_1;  
  std::vector<float> elegsfD0Err_1;  
  std::vector<float> elegsfDzErr_1;  

  std::vector<float> elegsfTrkPt_2;
  std::vector<float> elegsfTrkP_2;
  std::vector<float> elegsfTrkEta_2;
  std::vector<float> elegsfTrkPhi_2;
  std::vector<int>   elegsfTrkCharge_2;
  std::vector<float> elegsfTrkChi2_2;
  std::vector<float> elegsfTrkNdof_2;
  std::vector<float> elegsfTrkNormalizedChi2_2;
  std::vector<int>   elegsfTrkValidHits_2;
  std::vector<int>   elegsfTrkMissHits_2;
  std::vector<int>   elegsfTrkLayers_2;
  std::vector<float> elegsfD0_2;  
  std::vector<float> elegsfDz_2;  
  std::vector<float> elegsfD0Err_2;  
  std::vector<float> elegsfDzErr_2;  

  outputTreediPho->Branch("ngsfEle",&ngsfEle);
  outputTreediPho->Branch("elegsfTrkPt_1",&elegsfTrkPt_1);
  outputTreediPho->Branch("elegsfTrkP_1",&elegsfTrkP_1);
  outputTreediPho->Branch("elegsfTrkEta_1",&elegsfTrkEta_1);
  outputTreediPho->Branch("elegsfTrkPhi_1",&elegsfTrkPhi_1);
  outputTreediPho->Branch("elegsfTrkCharge_1",&elegsfTrkCharge_1);
  outputTreediPho->Branch("elegsfTrkChi2_1",&elegsfTrkChi2_1);
  outputTreediPho->Branch("elegsfTrkNdof_1",&elegsfTrkNdof_1);
  outputTreediPho->Branch("elegsfTrkNormalizedChi2_1",&elegsfTrkNormalizedChi2_1);
  outputTreediPho->Branch("elegsfTrkValidHits_1",&elegsfTrkValidHits_1);
  outputTreediPho->Branch("elegsfTrkMissHits_1",&elegsfTrkMissHits_1);
  outputTreediPho->Branch("elegsfTrkLayers_1",&elegsfTrkLayers_1);
  outputTreediPho->Branch("elegsfD0_1",&elegsfD0_1);
  outputTreediPho->Branch("elegsfDz_1",&elegsfDz_1);
  outputTreediPho->Branch("elegsfD0Err_1",&elegsfD0Err_1);
  outputTreediPho->Branch("elegsfDzErr_1",&elegsfDzErr_1);
  
  outputTreediPho->Branch("elegsfTrkPt_2",&elegsfTrkPt_2);
  outputTreediPho->Branch("elegsfTrkP_2",&elegsfTrkP_2);
  outputTreediPho->Branch("elegsfTrkEta_2",&elegsfTrkEta_2);
  outputTreediPho->Branch("elegsfTrkPhi_2",&elegsfTrkPhi_2);
  outputTreediPho->Branch("elegsfTrkCharge_2",&elegsfTrkCharge_2);
  outputTreediPho->Branch("elegsfTrkChi2_2",&elegsfTrkChi2_2);
  outputTreediPho->Branch("elegsfTrkNdof_2",&elegsfTrkNdof_2);
  outputTreediPho->Branch("elegsfTrkNormalizedChi2_2",&elegsfTrkNormalizedChi2_2);
  outputTreediPho->Branch("elegsfTrkValidHits_2",&elegsfTrkValidHits_2);
  outputTreediPho->Branch("elegsfTrkMissHits_2",&elegsfTrkMissHits_2);
  outputTreediPho->Branch("elegsfTrkLayers_2",&elegsfTrkLayers_2);
  outputTreediPho->Branch("elegsfD0_2",&elegsfD0_2);
  outputTreediPho->Branch("elegsfDz_2",&elegsfDz_2);
  outputTreediPho->Branch("elegsfD0Err_2",&elegsfD0Err_2);
  outputTreediPho->Branch("elegsfDzErr_2",&elegsfDzErr_2);
  
  std::vector<float> vSum_ee_M;
  std::vector<float> vSum_ee_Energy;
  std::vector<float> vSum_ee_Pt;
  std::vector<float> vSum_ee_Eta;
  std::vector<float> vSum_ee_Phi;
  std::vector<float> vSum_ee_Rapidity;
  std::vector<float> vSum_ee_Theta;
  
  outputTreediPho->Branch("vSum_M",&vSum_ee_M);
  outputTreediPho->Branch("vSum_Energy",&vSum_ee_Energy);
  outputTreediPho->Branch("vSum_Pt",&vSum_ee_Pt);
  outputTreediPho->Branch("vSum_Eta",&vSum_ee_Eta);
  outputTreediPho->Branch("vSum_Phi",&vSum_ee_Phi);
  outputTreediPho->Branch("vSum_Rapidity",&vSum_ee_Rapidity);
  outputTreediPho->Branch("vSum_Theta",&vSum_ee_Theta);

  std::vector<float> ele_dpt;
  std::vector<float> ele_deta;
  std::vector<float> ele_dphi;
  std::vector<float> ele_aco;

  outputTreediPho->Branch("ele_dpt", &ele_dpt);
  outputTreediPho->Branch("ele_deta",&ele_deta);
  outputTreediPho->Branch("ele_dphi",&ele_dphi);
  outputTreediPho->Branch("ele_aco", &ele_aco);


  // write hybrid supercluster info 
 
  std::vector<float>  sc_hy_E_1;
  std::vector<float>  sc_hy_Et_1;
  std::vector<float>  sc_hy_Eta_1;
  std::vector<float>  sc_hy_Phi_1;
  std::vector<float>  sc_hy_Eta_corr_1;
  std::vector<float>  sc_hy_Phi_corr_1;
  std::vector<float>  sc_hy_RawE_1;
  std::vector<float>  sc_hy_RawEt_1;
  std::vector<float>  sc_hy_raw_eoverp_1;
  std::vector<float>  sc_hy_eoverp_1; 
  
  outputTreediPho->Branch("nsc_hybrid",&nsc_hybrid);
  outputTreediPho->Branch("sc_hy_E_1",&sc_hy_E_1);
  outputTreediPho->Branch("sc_hy_Et_1",&sc_hy_Et_1);
  outputTreediPho->Branch("sc_hy_Eta_1",&sc_hy_Eta_1);
  outputTreediPho->Branch("sc_hy_Phi_1",&sc_hy_Phi_1);
  outputTreediPho->Branch("sc_hy_Eta_corr_1",&sc_hy_Eta_corr_1);
  outputTreediPho->Branch("sc_hy_Phi_corr_1",&sc_hy_Phi_corr_1);
  outputTreediPho->Branch("sc_hy_RawE_1",&sc_hy_RawE_1);
  outputTreediPho->Branch("sc_hy_RawEt_1",&sc_hy_RawEt_1);
  outputTreediPho->Branch("sc_hy_raw_eoverp_1",&sc_hy_raw_eoverp_1);
  outputTreediPho->Branch("sc_hy_eoverp_1",&sc_hy_eoverp_1);
 

  std::vector<float>  sc_hy_E_2;
  std::vector<float>  sc_hy_Et_2;
  std::vector<float>  sc_hy_Eta_2;
  std::vector<float>  sc_hy_Phi_2;
  std::vector<float>  sc_hy_Eta_corr_2;
  std::vector<float>  sc_hy_Phi_corr_2;
  std::vector<float>  sc_hy_RawE_2;
  std::vector<float>  sc_hy_RawEt_2;
  std::vector<float>  sc_hy_raw_eoverp_2;
  std::vector<float>  sc_hy_eoverp_2; 
  
  outputTreediPho->Branch("nsc_hybrid",&nsc_hybrid);
  outputTreediPho->Branch("sc_hy_E_2",&sc_hy_E_2);
  outputTreediPho->Branch("sc_hy_Et_2",&sc_hy_Et_2);
  outputTreediPho->Branch("sc_hy_Eta_2",&sc_hy_Eta_2);
  outputTreediPho->Branch("sc_hy_Phi_2",&sc_hy_Phi_2);
  outputTreediPho->Branch("sc_hy_Eta_corr_2",&sc_hy_Eta_corr_2);
  outputTreediPho->Branch("sc_hy_Phi_corr_2",&sc_hy_Phi_corr_2);
  outputTreediPho->Branch("sc_hy_RawE_2",&sc_hy_RawE_2);
  outputTreediPho->Branch("sc_hy_RawEt_2",&sc_hy_RawEt_2);
  outputTreediPho->Branch("sc_hy_raw_eoverp_2",&sc_hy_raw_eoverp_2);
  outputTreediPho->Branch("sc_hy_eoverp_2",&sc_hy_eoverp_2);
 
  
  // write di-cluster info with hybrid
  std::vector<float> vSum_hybrid_reco_M;
  std::vector<float> vSum_hybrid_reco_Energy;
  std::vector<float> vSum_hybrid_reco_Et;
  std::vector<float> vSum_hybrid_reco_Pt;
  std::vector<float> vSum_hybrid_reco_Eta;
  std::vector<float> vSum_hybrid_reco_Phi;
  std::vector<float> vSum_hybrid_reco_Rapidity;
  std::vector<float> vSum_hybrid_reco_Theta;
  
  outputTreediPho->Branch("vSum_hybrid_reco_M",&vSum_hybrid_reco_M);
  outputTreediPho->Branch("vSum_hybrid_reco_Energy",&vSum_hybrid_reco_Energy);
  outputTreediPho->Branch("vSum_hybrid_reco_Et",&vSum_hybrid_reco_Et);
  outputTreediPho->Branch("vSum_hybrid_reco_Pt",&vSum_hybrid_reco_Pt);
  outputTreediPho->Branch("vSum_hybrid_reco_Eta",&vSum_hybrid_reco_Eta);
  outputTreediPho->Branch("vSum_hybrid_reco_Phi",&vSum_hybrid_reco_Phi);
  outputTreediPho->Branch("vSum_hybrid_reco_Rapidity",&vSum_hybrid_reco_Rapidity);
  outputTreediPho->Branch("vSum_hybrid_reco_Theta",&vSum_hybrid_reco_Theta);
  
  std::vector<float> vSum_hybrid_raw_M;
  std::vector<float> vSum_hybrid_raw_Energy;
  std::vector<float> vSum_hybrid_raw_Et;
  std::vector<float> vSum_hybrid_raw_Pt;
  std::vector<float> vSum_hybrid_raw_Eta;
  std::vector<float> vSum_hybrid_raw_Phi;
  std::vector<float> vSum_hybrid_raw_Rapidity;
  std::vector<float> vSum_hybrid_raw_Theta;
  
  outputTreediPho->Branch("vSum_hybrid_raw_M",&vSum_hybrid_raw_M);
  outputTreediPho->Branch("vSum_hybrid_raw_Energy",&vSum_hybrid_raw_Energy);
  outputTreediPho->Branch("vSum_hybrid_raw_Et",&vSum_hybrid_raw_Et);
  outputTreediPho->Branch("vSum_hybrid_raw_Pt",&vSum_hybrid_raw_Pt);
  outputTreediPho->Branch("vSum_hybrid_raw_Eta",&vSum_hybrid_raw_Eta);
  outputTreediPho->Branch("vSum_hybrid_raw_Phi",&vSum_hybrid_raw_Phi);
  outputTreediPho->Branch("vSum_hybrid_raw_Rapidity",&vSum_hybrid_raw_Rapidity);
  outputTreediPho->Branch("vSum_hybrid_raw_Theta",&vSum_hybrid_raw_Theta);
  
  // write barrel gsf ele track info 
  std::vector<float> ele_brl_Pt_1;
  std::vector<float> ele_brl_P_1;
  std::vector<float> ele_brl_Eta_1;
  std::vector<float> ele_brl_Phi_1;
  std::vector<int>   ele_brl_Charge_1;
  
  outputTreediPho->Branch("ele_brl_Pt_1", &ele_brl_Pt_1);
  outputTreediPho->Branch("ele_brl_P_1",  &ele_brl_P_1);
  outputTreediPho->Branch("ele_brl_Eta_1",&ele_brl_Eta_1);
  outputTreediPho->Branch("ele_brl_Phi_1",&ele_brl_Phi_1);
  outputTreediPho->Branch("ele_brl_Charge_1",&ele_brl_Charge_1);

  std::vector<float> ele_brl_Pt_2;
  std::vector<float> ele_brl_P_2;
  std::vector<float> ele_brl_Eta_2;
  std::vector<float> ele_brl_Phi_2;
  std::vector<int>   ele_brl_Charge_2;
  
  outputTreediPho->Branch("ele_brl_Pt_2", &ele_brl_Pt_2);
  outputTreediPho->Branch("ele_brl_P_2",  &ele_brl_P_2);
  outputTreediPho->Branch("ele_brl_Eta_2",&ele_brl_Eta_2);
  outputTreediPho->Branch("ele_brl_Phi_2",&ele_brl_Phi_2);
  outputTreediPho->Branch("ele_brl_Charge_2",&ele_brl_Charge_2);
  
  // write dielectron info for electron-cluster matched 
  std::vector<float> vSum_ee_hybrid_M;
  std::vector<float> vSum_ee_hybrid_Energy;
  std::vector<float> vSum_ee_hybrid_Pt;
  std::vector<float> vSum_ee_hybrid_Et;
  std::vector<float> vSum_ee_hybrid_Eta;
  std::vector<float> vSum_ee_hybrid_Phi;
  std::vector<float> vSum_ee_hybrid_Rapidity;
  std::vector<float> vSum_ee_hybrid_Theta;
  
  outputTreediPho->Branch("vSum_ee_hybrid_M",&vSum_ee_hybrid_M);
  outputTreediPho->Branch("vSum_ee_hybrid_Energy",&vSum_ee_hybrid_Energy);
  outputTreediPho->Branch("vSum_ee_hybrid_Pt",&vSum_ee_hybrid_Pt);
  outputTreediPho->Branch("vSum_ee_hybrid_Et",&vSum_ee_hybrid_Et);
  outputTreediPho->Branch("vSum_ee_hybrid_Eta",&vSum_ee_hybrid_Eta);
  outputTreediPho->Branch("vSum_ee_hybrid_Phi",&vSum_ee_hybrid_Phi);
  outputTreediPho->Branch("vSum_ee_hybrid_Rapidity",&vSum_ee_hybrid_Rapidity);
  outputTreediPho->Branch("vSum_ee_hybrid_Theta",&vSum_ee_hybrid_Theta);
  
  
  // write m55 supercluster info  ............................................................................
  std::vector<float>  sc_m55_E_1;
  std::vector<float>  sc_m55_Et_1;
  std::vector<float>  sc_m55_Eta_1;
  std::vector<float>  sc_m55_Phi_1;
  std::vector<float>  sc_m55_Eta_corr_1;
  std::vector<float>  sc_m55_Phi_corr_1;
  std::vector<float>  sc_m55_RawE_1;
  std::vector<float>  sc_m55_RawEt_1;
  std::vector<float>  sc_m55_raw_eoverp_1;
  std::vector<float>  sc_m55_eoverp_1;

  outputTreediPho->Branch("sc_m55_E_1",&sc_m55_E_1);
  outputTreediPho->Branch("sc_m55_Et_1",&sc_m55_Et_1);
  outputTreediPho->Branch("sc_m55_Eta_1",&sc_m55_Eta_1);
  outputTreediPho->Branch("sc_m55_Phi_1",&sc_m55_Phi_1);
  outputTreediPho->Branch("sc_m55_Eta_corr_1",&sc_m55_Eta_corr_1);
  outputTreediPho->Branch("sc_m55_Phi_corr_1",&sc_m55_Phi_corr_1);
  outputTreediPho->Branch("sc_m55_RawE_1",&sc_m55_RawE_1);
  outputTreediPho->Branch("sc_m55_RawEt_1",&sc_m55_RawEt_1);
  outputTreediPho->Branch("sc_m55_raw_eoverp_1",&sc_m55_raw_eoverp_1);
  outputTreediPho->Branch("sc_m55_eoverp_1",&sc_m55_eoverp_1);


  std::vector<float>  sc_m55_E_2;
  std::vector<float>  sc_m55_Et_2;
  std::vector<float>  sc_m55_Eta_2;
  std::vector<float>  sc_m55_Phi_2;
  std::vector<float>  sc_m55_Eta_corr_2;
  std::vector<float>  sc_m55_Phi_corr_2;
  std::vector<float>  sc_m55_RawE_2;
  std::vector<float>  sc_m55_RawEt_2;
  std::vector<float>  sc_m55_raw_eoverp_2;
  std::vector<float>  sc_m55_eoverp_2;

  outputTreediPho->Branch("sc_m55_E_2",&sc_m55_E_2);
  outputTreediPho->Branch("sc_m55_Et_2",&sc_m55_Et_2);
  outputTreediPho->Branch("sc_m55_Eta_2",&sc_m55_Eta_2);
  outputTreediPho->Branch("sc_m55_Phi_2",&sc_m55_Phi_2);
  outputTreediPho->Branch("sc_m55_Eta_corr_2",&sc_m55_Eta_corr_2);
  outputTreediPho->Branch("sc_m55_Phi_corr_2",&sc_m55_Phi_corr_2);
  outputTreediPho->Branch("sc_m55_RawE_2",&sc_m55_RawE_2);
  outputTreediPho->Branch("sc_m55_RawEt_2",&sc_m55_RawEt_2);
  outputTreediPho->Branch("sc_m55_raw_eoverp_2",&sc_m55_raw_eoverp_2);
  outputTreediPho->Branch("sc_m55_eoverp_2",&sc_m55_eoverp_2);


  
  // write di-cluster info with m55
  std::vector<float> vSum_m55_reco_M;
  std::vector<float> vSum_m55_reco_Energy;
  std::vector<float> vSum_m55_reco_Et;
  std::vector<float> vSum_m55_reco_Pt;
  std::vector<float> vSum_m55_reco_Eta;
  std::vector<float> vSum_m55_reco_Phi;
  std::vector<float> vSum_m55_reco_Rapidity;
  std::vector<float> vSum_m55_reco_Theta;
  
  outputTreediPho->Branch("vSum_m55_reco_M",&vSum_m55_reco_M);
  outputTreediPho->Branch("vSum_m55_reco_Energy",&vSum_m55_reco_Energy);
  outputTreediPho->Branch("vSum_m55_reco_Et",&vSum_m55_reco_Et);
  outputTreediPho->Branch("vSum_m55_reco_Pt",&vSum_m55_reco_Pt);
  outputTreediPho->Branch("vSum_m55_reco_Eta",&vSum_m55_reco_Eta);
  outputTreediPho->Branch("vSum_m55_reco_Phi",&vSum_m55_reco_Phi);
  outputTreediPho->Branch("vSum_m55_reco_Rapidity",&vSum_m55_reco_Rapidity);
  outputTreediPho->Branch("vSum_m55_reco_Theta",&vSum_m55_reco_Theta);
  
  std::vector<float> vSum_m55_raw_M;
  std::vector<float> vSum_m55_raw_Energy;
  std::vector<float> vSum_m55_raw_Et;
  std::vector<float> vSum_m55_raw_Pt;
  std::vector<float> vSum_m55_raw_Eta;
  std::vector<float> vSum_m55_raw_Phi;
  std::vector<float> vSum_m55_raw_Rapidity;
  std::vector<float> vSum_m55_raw_Theta;
  
  outputTreediPho->Branch("vSum_m55_raw_M",&vSum_m55_raw_M);
  outputTreediPho->Branch("vSum_m55_raw_Energy",&vSum_m55_raw_Energy);
  outputTreediPho->Branch("vSum_m55_raw_Et",&vSum_m55_raw_Et);
  outputTreediPho->Branch("vSum_m55_raw_Pt",&vSum_m55_raw_Pt);
  outputTreediPho->Branch("vSum_m55_raw_Eta",&vSum_m55_raw_Eta);
  outputTreediPho->Branch("vSum_m55_raw_Phi",&vSum_m55_raw_Phi);
  outputTreediPho->Branch("vSum_m55_raw_Rapidity",&vSum_m55_raw_Rapidity);
  outputTreediPho->Branch("vSum_m55_raw_Theta",&vSum_m55_raw_Theta);
  
  
  // write endcap gsf ele track info 
  std::vector<float> ele_end_Pt_1;
  std::vector<float> ele_end_P_1;
  std::vector<float> ele_end_Eta_1;
  std::vector<float> ele_end_Phi_1;
  std::vector<int>   ele_end_Charge_1;
  
  outputTreediPho->Branch("ele_end_Pt_1", &ele_end_Pt_1);
  outputTreediPho->Branch("ele_end_P_1",  &ele_end_P_1);
  outputTreediPho->Branch("ele_end_Eta_1",&ele_end_Eta_1);
  outputTreediPho->Branch("ele_end_Phi_1",&ele_end_Phi_1);
  outputTreediPho->Branch("ele_end_Charge_1",&ele_end_Charge_1);


  std::vector<float> ele_end_Pt_2;
  std::vector<float> ele_end_P_2;
  std::vector<float> ele_end_Eta_2;
  std::vector<float> ele_end_Phi_2;
  std::vector<int>   ele_end_Charge_2;
  
  outputTreediPho->Branch("ele_end_Pt_2", &ele_end_Pt_2);
  outputTreediPho->Branch("ele_end_P_2",  &ele_end_P_2);
  outputTreediPho->Branch("ele_end_Eta_2",&ele_end_Eta_2);
  outputTreediPho->Branch("ele_end_Phi_2",&ele_end_Phi_2);
  outputTreediPho->Branch("ele_end_Charge_2",&ele_end_Charge_2);
  
  // write dielectron info for electron-cluster matched 
  std::vector<float> vSum_ee_m55_M;
  std::vector<float> vSum_ee_m55_Energy;
  std::vector<float> vSum_ee_m55_Pt;
  std::vector<float> vSum_ee_m55_Et;
  std::vector<float> vSum_ee_m55_Eta;
  std::vector<float> vSum_ee_m55_Phi;
  std::vector<float> vSum_ee_m55_Rapidity;
  std::vector<float> vSum_ee_m55_Theta;
  
  outputTreediPho->Branch("vSum_ee_m55_M",&vSum_ee_m55_M);
  outputTreediPho->Branch("vSum_ee_m55_Energy",&vSum_ee_m55_Energy);
  outputTreediPho->Branch("vSum_ee_m55_Pt",&vSum_ee_m55_Pt);
  outputTreediPho->Branch("vSum_ee_m55_Et",&vSum_ee_m55_Et);
  outputTreediPho->Branch("vSum_ee_m55_Eta",&vSum_ee_m55_Eta);
  outputTreediPho->Branch("vSum_ee_m55_Phi",&vSum_ee_m55_Phi);
  outputTreediPho->Branch("vSum_ee_m55_Rapidity",&vSum_ee_m55_Rapidity);
  outputTreediPho->Branch("vSum_ee_m55_Theta",&vSum_ee_m55_Theta);
  
  std::vector<float> invmass_hybrid_cluster_reco_formula;
  std::vector<float> invmass_hybrid_cluster_raw_formula;
  std::vector<float> invmass_m55_cluster_reco_formula;
  std::vector<float> invmass_m55_cluster_raw_formula;
  
  outputTreediPho->Branch("invmass_hybrid_cluster_reco_formula",&invmass_hybrid_cluster_reco_formula);
  outputTreediPho->Branch("invmass_hybrid_cluster_raw_formula",&invmass_hybrid_cluster_raw_formula);
  outputTreediPho->Branch("invmass_m55_cluster_reco_formula",&invmass_m55_cluster_reco_formula);
  outputTreediPho->Branch("invmass_m55_cluster_raw_formula",&invmass_m55_cluster_raw_formula);

  // write tower info ////
  
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

  outputTreediPho->Branch("ngenTrk",&ngenTrk);

  std::vector<int>   nextra_track;
  outputTreediPho->Branch("nextra_track",&nextra_track);

  float eledpt;
  float eledeta;
  float eledphi;
  float eleaco;


  Long64_t entries = tree->GetEntries();
  Long64_t entriesAnalyzed = 0;   Long64_t id = 0;   Long64_t excl = 0; Long64_t ptcut = 0; Long64_t myloop = 0;
  std::cout << "entries         = " << entries << std::endl;
  std::cout<< "Loop : ggHiNtuplizer/EventTree" <<std::endl;
  
  for (Long64_t j_entry=0; j_entry<entries; ++j_entry){
    //for (Long64_t j_entry= 0; j_entry< 50000; ++j_entry){
    
    if (j_entry % 1000 == 0)  {
      std::cout << "current entry = " <<j_entry<<" out of "<<entries<<" : "<<std::setprecision(2)<<(double)j_entry/entries*100<<" %"<<std::endl;
    }
    
    treeHLT->GetEntry(j_entry);
    tree->GetEntry(j_entry);
    treePixel->GetEntry(j_entry);
    
    if(HLT_HIUPCL1DoubleEG2NotHF2_v1==1 || HLT_HIUPCL1SingleEG5NotHF2_v1==1){   entriesAnalyzed++; 
      
      outputTreeHLT->Fill(); 
      
      // dielectron block
      
      elegsfTrkPt_1.clear();
      elegsfTrkP_1.clear();
      elegsfTrkEta_1.clear();
      elegsfTrkPhi_1.clear();
      elegsfTrkCharge_1.clear();
      elegsfTrkChi2_1.clear();
      elegsfTrkNdof_1.clear();
      elegsfTrkNormalizedChi2_1.clear();
      elegsfTrkValidHits_1.clear();
      elegsfTrkMissHits_1.clear();
      elegsfTrkLayers_1.clear();
      elegsfD0_1.clear();  
      elegsfDz_1.clear();  
      elegsfD0Err_1.clear();  
      elegsfDzErr_1.clear();  
      
      elegsfTrkPt_2.clear();
      elegsfTrkP_2.clear();
      elegsfTrkEta_2.clear();
      elegsfTrkPhi_2.clear();
      elegsfTrkCharge_2.clear();
      elegsfTrkChi2_2.clear();
      elegsfTrkNdof_2.clear();
      elegsfTrkNormalizedChi2_2.clear();
      elegsfTrkValidHits_2.clear();
      elegsfTrkMissHits_2.clear();
      elegsfTrkLayers_2.clear();
      elegsfD0_2.clear();  
      elegsfDz_2.clear();  
      elegsfD0Err_2.clear();  
      elegsfDzErr_2.clear();  
            
      vSum_ee_M.clear();
      vSum_ee_Energy.clear();
      vSum_ee_Pt.clear();
      vSum_ee_Eta.clear();
      vSum_ee_Phi.clear();
      vSum_ee_Rapidity.clear();
      vSum_ee_Theta.clear();
      
      ele_dpt.clear();
      ele_deta.clear();
      ele_dphi.clear();
      ele_aco.clear();	

       
      sc_hy_E_1.clear();
      sc_hy_Et_1.clear();
      sc_hy_Eta_1.clear();
      sc_hy_Phi_1.clear();
      sc_hy_Eta_corr_1.clear();
      sc_hy_Phi_corr_1.clear();
      sc_hy_RawE_1.clear();
      sc_hy_RawEt_1.clear();
      sc_hy_raw_eoverp_1.clear();
      sc_hy_eoverp_1.clear();

      sc_hy_E_2.clear();
      sc_hy_Et_2.clear();
      sc_hy_Eta_2.clear();
      sc_hy_Phi_2.clear();
      sc_hy_Eta_corr_2.clear();
      sc_hy_Phi_corr_2.clear();
      sc_hy_RawE_2.clear();
      sc_hy_RawEt_2.clear();
      sc_hy_raw_eoverp_2.clear();
      sc_hy_eoverp_2.clear();


      vSum_hybrid_reco_M.clear();
      vSum_hybrid_reco_Energy.clear();
      vSum_hybrid_reco_Et.clear();
      vSum_hybrid_reco_Pt.clear();
      vSum_hybrid_reco_Eta.clear();
      vSum_hybrid_reco_Phi.clear();
      vSum_hybrid_reco_Rapidity.clear();
      vSum_hybrid_reco_Theta.clear();


      vSum_hybrid_raw_M.clear();
      vSum_hybrid_raw_Energy.clear();
      vSum_hybrid_raw_Et.clear();
      vSum_hybrid_raw_Pt.clear();
      vSum_hybrid_raw_Eta.clear();
      vSum_hybrid_raw_Phi.clear();
      vSum_hybrid_raw_Rapidity.clear();
      vSum_hybrid_raw_Theta.clear();

      
      ele_brl_Pt_1.clear();
      ele_brl_P_1.clear();
      ele_brl_Eta_1.clear();
      ele_brl_Phi_1.clear();
      ele_brl_Charge_1.clear();

      ele_brl_Pt_2.clear();
      ele_brl_P_2.clear();
      ele_brl_Eta_2.clear();
      ele_brl_Phi_2.clear();
      ele_brl_Charge_2.clear();

      vSum_ee_hybrid_M.clear();
      vSum_ee_hybrid_Energy.clear();
      vSum_ee_hybrid_Pt.clear();
      vSum_ee_hybrid_Et.clear();
      vSum_ee_hybrid_Eta.clear();
      vSum_ee_hybrid_Phi.clear();
      vSum_ee_hybrid_Rapidity.clear();
      vSum_ee_hybrid_Theta.clear();

      
      sc_m55_E_1.clear();
      sc_m55_Et_1.clear();
      sc_m55_Eta_1.clear();
      sc_m55_Phi_1.clear();
      sc_m55_Eta_corr_1.clear();
      sc_m55_Phi_corr_1.clear();
      sc_m55_RawE_1.clear();
      sc_m55_RawEt_1.clear();
      sc_m55_raw_eoverp_1.clear();
      sc_m55_eoverp_1.clear();


      sc_m55_E_2.clear();
      sc_m55_Et_2.clear();
      sc_m55_Eta_2.clear();
      sc_m55_Phi_2.clear();
      sc_m55_Eta_corr_2.clear();
      sc_m55_Phi_corr_2.clear();
      sc_m55_RawE_2.clear();
      sc_m55_RawEt_2.clear();
      sc_m55_raw_eoverp_2.clear();
      sc_m55_eoverp_2.clear();


      vSum_m55_reco_M.clear();
      vSum_m55_reco_Energy.clear();
      vSum_m55_reco_Et.clear();
      vSum_m55_reco_Pt.clear();
      vSum_m55_reco_Eta.clear();
      vSum_m55_reco_Phi.clear();
      vSum_m55_reco_Rapidity.clear();
      vSum_m55_reco_Theta.clear();

      vSum_m55_raw_M.clear();
      vSum_m55_raw_Energy.clear();
      vSum_m55_raw_Et.clear();
      vSum_m55_raw_Pt.clear();
      vSum_m55_raw_Eta.clear();
      vSum_m55_raw_Phi.clear();
      vSum_m55_raw_Rapidity.clear();
      vSum_m55_raw_Theta.clear();
      
      ele_end_Pt_1.clear();
      ele_end_P_1.clear();
      ele_end_Eta_1.clear();
      ele_end_Phi_1.clear();
      ele_end_Charge_1.clear();

      ele_end_Pt_2.clear();
      ele_end_P_2.clear();
      ele_end_Eta_2.clear();
      ele_end_Phi_2.clear();
      ele_end_Charge_2.clear();

      vSum_ee_m55_M.clear();
      vSum_ee_m55_Energy.clear();
      vSum_ee_m55_Pt.clear();
      vSum_ee_m55_Et.clear();
      vSum_ee_m55_Eta.clear();
      vSum_ee_m55_Phi.clear();
      vSum_ee_m55_Rapidity.clear();
      vSum_ee_m55_Theta.clear();


      invmass_hybrid_cluster_reco_formula.clear();
      invmass_hybrid_cluster_raw_formula.clear();
      invmass_m55_cluster_reco_formula.clear();
      invmass_m55_cluster_raw_formula.clear();

  
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
 
      nextra_track.clear();
      
      if(ngsfEle != 2 ) continue; //cout << " coming in electron loop " << endl;
      for(int i=0; i<ngsfEle; ++i){
	
	for (int j=i+1; j<ngsfEle; ++j){
	  
	  if (elegsfTrkCharge->at(i) == elegsfTrkCharge->at(j)) continue;
	  
	  elegsfTrkPt_1.push_back(elegsfTrkPt->at(i));
	  elegsfTrkP_1.push_back(elegsfTrkP->at(i));
	  elegsfTrkEta_1.push_back(elegsfTrkEta->at(i));
	  elegsfTrkPhi_1.push_back(elegsfTrkPhi->at(i));
	  elegsfTrkCharge_1.push_back(elegsfTrkCharge->at(i));
	  elegsfTrkChi2_1.push_back(elegsfTrkChi2->at(i));
	  elegsfTrkNdof_1.push_back(elegsfTrkNdof->at(i));
	  elegsfTrkNormalizedChi2_1.push_back(elegsfTrkNormalizedChi2->at(i));
	  elegsfTrkValidHits_1.push_back(elegsfTrkValidHits->at(i));
	  elegsfTrkMissHits_1.push_back(elegsfTrkMissHits->at(i));
	  elegsfTrkLayers_1.push_back(elegsfTrkLayers->at(i));
	  elegsfD0_1.push_back(elegsfD0->at(i));  
	  elegsfDz_1.push_back(elegsfDz->at(i));  
	  elegsfD0Err_1.push_back(elegsfD0Err->at(i));  
	  elegsfDzErr_1.push_back(elegsfDzErr->at(i));  

	  elegsfTrkPt_2.push_back(elegsfTrkPt->at(j));
	  elegsfTrkP_2.push_back(elegsfTrkP->at(j));
	  elegsfTrkEta_2.push_back(elegsfTrkEta->at(j));
	  elegsfTrkPhi_2.push_back(elegsfTrkPhi->at(j));
	  elegsfTrkCharge_2.push_back(elegsfTrkCharge->at(j));
	  elegsfTrkChi2_2.push_back(elegsfTrkChi2->at(i));
	  elegsfTrkNdof_2.push_back(elegsfTrkNdof->at(j));
	  elegsfTrkNormalizedChi2_2.push_back(elegsfTrkNormalizedChi2->at(j));
	  elegsfTrkValidHits_2.push_back(elegsfTrkValidHits->at(j));
	  elegsfTrkMissHits_2.push_back(elegsfTrkMissHits->at(j));
	  elegsfTrkLayers_2.push_back(elegsfTrkLayers->at(j));
	  elegsfD0_2.push_back(elegsfD0->at(j));  
	  elegsfDz_2.push_back(elegsfDz->at(j));  
	  elegsfD0Err_2.push_back(elegsfD0Err->at(j));  
	  elegsfDzErr_2.push_back(elegsfDzErr->at(j));  
	  
	  TLorentzVector v1, v2, vSum;
	  v1.SetPtEtaPhiM( elegsfTrkPt->at(i), elegsfTrkEta->at(i),
			   elegsfTrkPhi->at(i), eleMass);
	  v2.SetPtEtaPhiM( elegsfTrkPt->at(j), elegsfTrkEta->at(j),
			   elegsfTrkPhi->at(j), eleMass);
	  vSum = v1+v2;
	  
	  vSum_ee_M.push_back(vSum.M());
	  vSum_ee_Energy.push_back(vSum.Energy());
	  vSum_ee_Pt.push_back(vSum.Pt());
	  vSum_ee_Eta.push_back(vSum.Eta());
	  vSum_ee_Phi.push_back(vSum.Phi());
	  vSum_ee_Rapidity.push_back(vSum.Rapidity());

          double ee_theta = v1.Angle(v2.Vect()); 
	  vSum_ee_Theta.push_back(ee_theta);
	  
	  eledpt  = fabs(elegsfTrkPt->at(i) - elegsfTrkPt->at(j));
	  eledeta = fabs(elegsfTrkEta->at(i) - elegsfTrkEta->at(j));
	  eledphi = getDPHI(elegsfTrkPhi->at(i) ,elegsfTrkPhi->at(j));
	  eleaco = 1- (eledphi/3.141592653589); 
	  
	  ele_dpt.push_back(eledpt);
	  ele_deta.push_back(eledeta);
	  ele_dphi.push_back(eledphi);
	  ele_aco.push_back(eleaco);
	  
          
          int nextratracks =0;
	  for(int kk = 0; kk < ngenTrk; ++kk){
	    
	    if (getDPHI(gentrkPhi->at(kk), elegsfTrkPhi->at(i))<0.7 && getDETA(gentrkEta->at(kk), elegsfTrkEta->at(i))<0.15) continue;
	    if (getDPHI(gentrkPhi->at(kk), elegsfTrkPhi->at(j))<0.7 && getDETA(gentrkEta->at(kk), elegsfTrkEta->at(j))<0.15) continue;
	    
	    //allEmEnergy_EB.push_back(CaloTower_emE->at(k));
	    
	    /*if (CaloTower_emE->at(k) > EmEnergy_EB){                         
	      EmEnergy_EB = CaloTower_emE->at(k) ;
	      }*/
	    nextratracks++;
	    
	  }
 	  
          //cout << "Event no. " << j_entry << " No. of extra track " <<  nextratracks << endl; 
	  nextra_track.push_back( nextratracks);
	  
	  
	  // general track
	  float  EmEnergy_EB = 0;
	  float  EmEnergy_EE = 0;
	  float  HadEnergy_HB = 0;
	  float  HadEnergy_HE = 0;
	  float  HadEnergy_HF_Plus = 0;
	  float  HadEnergy_HF_Minus = 0;
	  
	  
	  //cout <<"event no. " << j_entry <<  " No. of tower " << nTower << endl;
	  for(int k = 0; k< nTower; ++k){
	    
	    if (fabs(CaloTower_eta->at(k)) < 1.479){ //start filling EM tower in barel region  
	      if (getDPHI(CaloTower_phi->at(k), elegsfTrkPhi->at(i))<0.7 && getDETA(CaloTower_eta->at(k), elegsfTrkEta->at(i))<0.15) continue;
	      if (getDPHI(CaloTower_phi->at(k), elegsfTrkPhi->at(j))<0.7 && getDETA(CaloTower_eta->at(k), elegsfTrkEta->at(j))<0.15) continue;
	      
	      allEmEnergy_EB.push_back(CaloTower_emE->at(k));
	      
	      if (CaloTower_emE->at(k) > EmEnergy_EB){                         
		EmEnergy_EB = CaloTower_emE->at(k) ;
	      }
	    }
	    
	    if (fabs(CaloTower_eta->at(k)) > 1.479 && fabs(CaloTower_eta->at(k)) < 3 ){           
	      if (getDPHI(CaloTower_phi->at(k), elegsfTrkPhi->at(i))<0.4 && getDETA(CaloTower_eta->at(k), elegsfTrkEta->at(i))<0.15) continue;
	      if (getDPHI(CaloTower_phi->at(k), elegsfTrkPhi->at(j))<0.4 && getDETA(CaloTower_eta->at(k), elegsfTrkEta->at(j))<0.15) continue;
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
	  
          //if(eleaco < 0.05 ){
	  //cout << "Event No:"<< j_entry << " no. of hybrid cluster:" << nsc_hybrid << " Invariant mass:" <<vSum.M()<< endl;
	  
	  if(TMath::Abs(elegsfTrkEta->at(i)) < cutEta && TMath::Abs(elegsfTrkEta->at(j)) < cutEta && nsc_hybrid ==2){
	    
	    
	    double sc_hybrid_et_1 = 0,  sc_hybrid_eta_1 = 0, sc_hybrid_phi_1 = 0,  sc_hybrid_energy_1 = 0;  
	    double sc_hybrid_et_2 = 0,  sc_hybrid_eta_2 = 0, sc_hybrid_phi_2 = 0,  sc_hybrid_energy_2 = 0;  

	    double sc_hybrid_raw_et_1 = 0,  sc_hybrid_raw_energy_1 = 0; 
	    double sc_hybrid_raw_et_2 = 0,  sc_hybrid_raw_energy_2 = 0;

	    double sc_hybrid_eta_corr_1 = 0, sc_hybrid_phi_corr_1 = 0;
	    double sc_hybrid_eta_corr_2 = 0, sc_hybrid_phi_corr_2 = 0;
	    
	    double ele_pt_1 = 0, ele_eta_1 = 0, ele_phi_1 = 0, ele_energy_1 = 0;
	    double ele_pt_2 = 0, ele_eta_2 = 0, ele_phi_2 = 0, ele_energy_2 = 0;

	    bool ele1_cluster_match = 0, ele2_cluster_match = 0;  
	    
	    double sc_hybrid_et = 0, sc_hybrid_eta = 0, sc_hybrid_phi = 0,  sc_hybrid_energy = 0, sc_hybrid_eta_corr = 0, sc_hybrid_phi_corr = 0;  
	    double sc_hy_x  = 0, sc_hy_y  = 0,    sc_hy_z  = 0;

	      
	    for(int ii = 0;  ii < nsc_hybrid; ++ ii ){   //start cluster loop
	      
              /// correct sc position//////////////////////////
	      
	      sc_hybrid_et          = sc_hybrid_Et->at(ii);
	      sc_hybrid_eta         = sc_hybrid_Eta->at(ii);
	      sc_hybrid_phi         = sc_hybrid_Phi->at(ii);
	      sc_hy_x               = sc_hybrid_x->at(ii);
	      sc_hy_y               = sc_hybrid_y->at(ii);
	      sc_hy_z               = sc_hybrid_z->at(ii);
	      sc_hybrid_energy      = sc_hybrid_E->at(ii);
	      
	      
              double pvx =0 , pvy=0, pvz=0;
	      for(int vv = 0;  vv < nVtx; ++vv){
		pvx = xVtx->at(vv);
		pvy = yVtx->at(vv);
		pvz = zVtx->at(vv);
		
	      }
	      
	      TLorentzVector sc_corr;
	      TVector3 sc1dir(sc_hy_x-pvx, sc_hy_y -pvy,sc_hy_z-pvz);
	      sc1dir = sc1dir.Unit();
	      sc_corr.SetPxPyPzE(sc1dir.x()*sc_hybrid_energy, sc1dir.y()*sc_hybrid_energy, 
		                 sc1dir.z()*sc_hybrid_energy, sc_hybrid_energy);
	      
	      //cout << "Entry:" << j_entry << endl;
	      //cout<< "SC z:"<< eleSCz->at(i) << "  vertex z:" << pvz << "  Corr SC Z:"<< sc1dir.z() << endl;
	      //cout<< "Ele eta:"<< elegsfTrkEta->at(i) << "  SC Eta:" << sc_hybrid_eta << "  Corr SC Eta:"<< sc_corr.Eta() << endl;
	      //cout<< "Ele phi:"<< elegsfTrkPhi->at(i) << "  SC phi:" << sc_hybrid_phi << "  Corr SC phi:"<< sc_corr.Phi() << endl;
	      
	      sc_hybrid_eta_corr = sc_corr.Eta();
	      sc_hybrid_phi_corr = sc_corr.Phi();
              
	      	         
	      deta_1 = getDETA(sc_hybrid_eta_corr , elegsfTrkEta->at(i) );
	      dphi_1 = getDPHI(sc_hybrid_phi_corr , elegsfTrkPhi->at(i) );
	      
	      deta_2 = getDETA(sc_hybrid_eta_corr , elegsfTrkEta->at(j) );
	      dphi_2 = getDPHI(sc_hybrid_phi_corr , elegsfTrkPhi->at(j) );
	      
	   
	      
	      if(deta_1 < 0.05 && dphi_1 < 0.5 ){
		
		sc_hy_E_1.push_back(sc_hybrid_E->at(ii));
		sc_hy_Et_1.push_back(sc_hybrid_Et->at(ii));
		sc_hy_Eta_1.push_back(sc_hybrid_Eta->at(ii));
		sc_hy_Phi_1.push_back(sc_hybrid_Phi->at(ii));
		sc_hy_Eta_corr_1.push_back(sc_hybrid_eta_corr);
		sc_hy_Phi_corr_1.push_back(sc_hybrid_phi_corr);
		sc_hy_RawE_1.push_back(sc_hybrid_RawE->at(ii));
		sc_hy_RawEt_1.push_back(sc_hybrid_RawEt->at(ii));

		
		raw_eoverp = sc_hybrid_RawE->at(ii)/elegsfTrkP->at(i);
		eoverp = sc_hybrid_E->at(ii)/elegsfTrkP->at(i);
		
		sc_hy_raw_eoverp_1.push_back(raw_eoverp);
		sc_hy_eoverp_1.push_back(eoverp);
		
		ele_brl_P_1.push_back(elegsfTrkP->at(i));
		ele_brl_Pt_1.push_back(elegsfTrkPt->at(i));
		ele_brl_Eta_1.push_back(elegsfTrkEta->at(i));
		ele_brl_Phi_1.push_back(elegsfTrkPhi->at(i));
		ele_brl_Charge_1.push_back(elegsfTrkCharge->at(i));
		
		sc_hybrid_et_1          = sc_hybrid_Et->at(ii);
		sc_hybrid_eta_1         = sc_hybrid_Eta->at(ii);
		sc_hybrid_phi_1         = sc_hybrid_Phi->at(ii);
		sc_hybrid_energy_1      = sc_hybrid_E->at(ii);
		sc_hybrid_raw_et_1      = sc_hybrid_RawEt->at(ii);
		sc_hybrid_raw_energy_1  = sc_hybrid_RawE->at(ii);
		sc_hybrid_eta_corr_1    = sc_hybrid_eta_corr;
		sc_hybrid_phi_corr_1    = sc_hybrid_phi_corr;
		
		ele_pt_1  = elegsfTrkPt->at(i);
		ele_eta_1  = elegsfTrkEta->at(i);
		ele_phi_1  = elegsfTrkPhi->at(i);
		
		ele1_cluster_match = true;
	      }
	      
	      if(deta_2 < 0.05 && dphi_2 < 0.5 ){
		//cout << "after deta 2 cut " << endl;
		//cout << "hybrid eta:" << sc_hybrid_Eta->at(ii) << " track eta: " << elegsfTrkEta->at(j) << " deta_2: " << deta_2 << endl;
		
		sc_hy_E_2.push_back(sc_hybrid_E->at(ii));
		sc_hy_Et_2.push_back(sc_hybrid_Et->at(ii));
		sc_hy_Eta_2.push_back(sc_hybrid_Eta->at(ii));
		sc_hy_Phi_2.push_back(sc_hybrid_Phi->at(ii));
		sc_hy_Eta_corr_2.push_back(sc_hybrid_eta_corr);
		sc_hy_Phi_corr_2.push_back(sc_hybrid_phi_corr);
		sc_hy_RawE_2.push_back(sc_hybrid_RawE->at(ii));
		sc_hy_RawEt_2.push_back(sc_hybrid_RawEt->at(ii));

		
		raw_eoverp = sc_hybrid_RawE->at(ii)/elegsfTrkP->at(j);
		eoverp = sc_hybrid_E->at(ii)/elegsfTrkP->at(j);
		
		sc_hy_raw_eoverp_2.push_back(raw_eoverp);
		sc_hy_eoverp_2.push_back(eoverp);
		
		ele_brl_P_2.push_back(elegsfTrkP->at(j));
		ele_brl_Pt_2.push_back(elegsfTrkPt->at(j));
		ele_brl_Eta_2.push_back(elegsfTrkEta->at(j));
		ele_brl_Phi_2.push_back(elegsfTrkPhi->at(j));
		ele_brl_Charge_2.push_back(elegsfTrkCharge->at(j));
		
		sc_hybrid_et_2          = sc_hybrid_Et->at(ii);
		sc_hybrid_eta_2         = sc_hybrid_Eta->at(ii);
		sc_hybrid_phi_2         = sc_hybrid_Phi->at(ii);
		sc_hybrid_energy_2      = sc_hybrid_E->at(ii);
		sc_hybrid_raw_et_2      = sc_hybrid_RawEt->at(ii);
		sc_hybrid_raw_energy_2  = sc_hybrid_RawE->at(ii);
		sc_hybrid_eta_corr_2    = sc_hybrid_eta_corr;
		sc_hybrid_phi_corr_2    = sc_hybrid_phi_corr;
		
		ele_pt_2   = elegsfTrkPt->at(j);
		ele_eta_2  = elegsfTrkEta->at(j);
		ele_phi_2  = elegsfTrkPhi->at(j);
		
		  ele2_cluster_match = true;
	      } // track ckuster matching
	    } //hybrid cluster 
	    
	    
	    if(ele1_cluster_match && ele2_cluster_match){
	      
	      // fill di-cluster info with reco energy inputs	
	      TLorentzVector v3, v4, vSum2;
	      v3.SetPtEtaPhiE( sc_hybrid_et_1 ,  sc_hybrid_eta_corr_1,
			       sc_hybrid_phi_corr_1,  sc_hybrid_energy_1);
	      v4.SetPtEtaPhiE( sc_hybrid_et_2 ,  sc_hybrid_eta_corr_2,
			       sc_hybrid_phi_corr_2,  sc_hybrid_energy_2);
	      vSum2 = v3+v4;
	      
	      vSum_hybrid_reco_M.push_back(vSum2.M());
	      vSum_hybrid_reco_Energy.push_back(vSum2.Energy());
	      vSum_hybrid_reco_Et.push_back(vSum2.Et());
	      vSum_hybrid_reco_Pt.push_back(vSum2.Pt());
	      vSum_hybrid_reco_Eta.push_back(vSum2.Eta());
	      vSum_hybrid_reco_Phi.push_back(vSum2.Phi());
	      vSum_hybrid_reco_Rapidity.push_back(vSum2.Rapidity());
	      
	      double hybrid_theta = v3.Angle(v4.Vect()); 
	      vSum_hybrid_reco_Theta.push_back(hybrid_theta);
	      
	      double dicluster_invmass = getInvMass(sc_hybrid_energy_1, sc_hybrid_eta_corr_1, sc_hybrid_phi_corr_1,  
						    sc_hybrid_energy_2, sc_hybrid_eta_corr_2, sc_hybrid_phi_corr_2);  
	      
	      invmass_hybrid_cluster_reco_formula.push_back(dicluster_invmass);
	      
	      
	      // fill di-cluster info with raw energy inputs
	      TLorentzVector v3_raw, v4_raw, vSum2_raw;
	      v3_raw.SetPtEtaPhiE( sc_hybrid_raw_et_1 ,  sc_hybrid_eta_corr_1, 
				   sc_hybrid_phi_corr_1 , sc_hybrid_raw_energy_1);
	      
	      v4_raw.SetPtEtaPhiE( sc_hybrid_raw_et_2 ,  sc_hybrid_eta_corr_2,
				   sc_hybrid_phi_corr_2    ,  sc_hybrid_raw_energy_2);
	      
	      vSum2_raw = v3_raw + v4_raw;
	      
	      vSum_hybrid_raw_M.push_back(vSum2_raw.M());
	      vSum_hybrid_raw_Energy.push_back(vSum2_raw.Energy());
	      vSum_hybrid_raw_Et.push_back(vSum2_raw.Et());
	      vSum_hybrid_raw_Pt.push_back(vSum2_raw.Pt());
	      vSum_hybrid_raw_Eta.push_back(vSum2_raw.Eta());
	      vSum_hybrid_raw_Phi.push_back(vSum2_raw.Phi());
	      vSum_hybrid_raw_Rapidity.push_back(vSum2_raw.Rapidity());
	      
	      double hybrid_theta_raw = v3_raw.Angle(v4_raw.Vect()); 
	      vSum_hybrid_raw_Theta.push_back(hybrid_theta_raw);
	      
	      double dicluster_raw_invmass = getInvMass(sc_hybrid_raw_energy_1, sc_hybrid_eta_corr_1, sc_hybrid_phi_corr_1,  
						          sc_hybrid_raw_energy_2, sc_hybrid_eta_corr_2, sc_hybrid_phi_corr_2);  
	      
	      invmass_hybrid_cluster_raw_formula.push_back(dicluster_raw_invmass);
	      
	      
	      // fill dielectron info for electrons matched to clusters
	      TLorentzVector v5, v6, vSum3;
	      v5.SetPtEtaPhiM( ele_pt_1  ,  ele_eta_1,
			       ele_phi_1 ,  eleMass);
	      v6.SetPtEtaPhiM( ele_pt_2  ,  ele_eta_2,
			       ele_phi_2 ,  eleMass);
	      vSum3 = v5+v6;
	      
	      vSum_ee_hybrid_M.push_back(vSum3.M());
	      vSum_ee_hybrid_Energy.push_back(vSum3.Energy());
	      vSum_ee_hybrid_Et.push_back(vSum3.Et());
	      vSum_ee_hybrid_Pt.push_back(vSum3.Pt());
	      vSum_ee_hybrid_Eta.push_back(vSum3.Eta());
	      vSum_ee_hybrid_Phi.push_back(vSum3.Phi());
	      vSum_ee_hybrid_Rapidity.push_back(vSum3.Rapidity());
	      
	      double ee_hybrid_theta = v5.Angle(v6.Vect()); 
	      vSum_ee_hybrid_Theta.push_back(ee_hybrid_theta);
	      
	    } // fill di-cluster info if both electron matches to cluster   
	  } //eta cut
	  
	  
	  
	  
	    //******************************* start endcap electron cluster matching ********************************************/
	  
	  
	  if(TMath::Abs(elegsfTrkEta->at(i)) > cutEta && TMath::Abs(elegsfTrkEta->at(j)) > cutEta && nsc_mult55 == 2 ){
	    
	    double sc_m55_et_1 = 0,  sc_m55_eta_1 = 0, sc_m55_phi_1 = 0,  sc_m55_energy_1 = 0;  
	    double sc_m55_et_2 = 0,  sc_m55_eta_2 = 0, sc_m55_phi_2 = 0,  sc_m55_energy_2 = 0;  
	    
	    double sc_m55_raw_et_1 = 0,  sc_m55_raw_energy_1 = 0; 
	    double sc_m55_raw_et_2 = 0,  sc_m55_raw_energy_2 = 0;
	    
	    double sc_m55_eta_corr_1 = 0, sc_m55_phi_corr_1 = 0;
	    double sc_m55_eta_corr_2 = 0, sc_m55_phi_corr_2 = 0;
	    
	    double ele_end_pt_1 = 0, ele_end_eta_1 = 0, ele_end_phi_1 = 0, ele_end_energy_1 = 0;
	    double ele_end_pt_2 = 0, ele_end_eta_2 = 0, ele_end_phi_2 = 0, ele_end_energy_2 = 0;
	    
	    bool ele1_m55_match = 0, ele2_m55_match = 0;  
	    
	    double sc_m55_et = 0, sc_m55_eta = 0, sc_m55_phi = 0,  sc_m55_energy = 0, sc_m55_eta_corr = 0, sc_m55_phi_corr = 0;  
	    double sc_m55_x  = 0, sc_m55_y  = 0,    sc_m55_z  = 0;
	    
	    
	    
	    for(int ii = 0;  ii < nsc_mult55; ++ ii ){   //start cluster loop
	      
	      /// correct sc position//////////////////////////
	      
	      sc_m55_et          = sc_mult55_Et->at(ii);
	      sc_m55_eta         = sc_mult55_Eta->at(ii);
	      sc_m55_phi         = sc_mult55_Phi->at(ii);
	      sc_m55_x           = sc_mult55_x->at(ii);
	      sc_m55_y           = sc_mult55_y->at(ii);
	      sc_m55_z           = sc_mult55_z->at(ii);
	      sc_m55_energy      = sc_mult55_E->at(ii);
	      
	      
              double pvx =0 , pvy=0, pvz=0;
	      for(int vv = 0;  vv < nVtx; ++vv){
		pvx = xVtx->at(vv);
		pvy = yVtx->at(vv);
		pvz = zVtx->at(vv);
		
	      }
	      
	      TLorentzVector sc_corr;
	      TVector3 sc1dir(sc_m55_x-pvx, sc_m55_y -pvy,sc_m55_z-pvz);
	      sc1dir = sc1dir.Unit();
	      sc_corr.SetPxPyPzE(sc1dir.x()*sc_m55_energy, sc1dir.y()*sc_m55_energy, 
		                 sc1dir.z()*sc_m55_energy, sc_m55_energy);
	      
	      //cout << "Entry:" << j_entry << endl;
	      //cout<< "SC z:"<< eleSCz->at(i) << "  vertex z:" << pvz << "  Corr SC Z:"<< sc1dir.z() << endl;
	      //cout<< "Ele eta:"<< elegsfTrkEta->at(i) << "  SC Eta:" << sc_m55_eta << "  Corr SC Eta:"<< sc_corr.Eta() << endl;
	      //cout<< "Ele phi:"<< elegsfTrkPhi->at(i) << "  SC phi:" << sc_m55_phi << "  Corr SC phi:"<< sc_corr.Phi() << endl;
	      
	      sc_m55_eta_corr = sc_corr.Eta();
	      sc_m55_phi_corr = sc_corr.Phi();
              
	      
	      deta_1 = getDETA(sc_m55_eta_corr , elegsfTrkEta->at(i) );
	      dphi_1 = getDPHI(sc_m55_phi_corr , elegsfTrkPhi->at(i) );
	      
	      deta_2 = getDETA(sc_m55_eta_corr , elegsfTrkEta->at(j) );
	      dphi_2 = getDPHI(sc_m55_phi_corr , elegsfTrkPhi->at(j) );
	      

	      if(deta_1 < 0.05 && dphi_1 < 0.5 ){
		//cout << "after deta 1 cut " << endl;
		//cout << "mult55 eta:" << sc_mult55_Eta->at(ii) << " track eta: " << elegsfTrkEta->at(i) << " deta_1: " << deta_1 << endl;
		
		sc_m55_E_1.push_back(sc_mult55_E->at(ii));
		sc_m55_Et_1.push_back(sc_mult55_Et->at(ii));
		sc_m55_Eta_1.push_back(sc_mult55_Eta->at(ii));
		sc_m55_Phi_1.push_back(sc_mult55_Phi->at(ii));
		sc_m55_Eta_corr_1.push_back(sc_m55_eta_corr);
		sc_m55_Phi_corr_1.push_back(sc_m55_phi_corr);
		sc_m55_RawE_1.push_back(sc_mult55_RawE->at(ii));
		sc_m55_RawEt_1.push_back(sc_mult55_RawEt->at(ii));

		
		raw_eoverp = sc_mult55_RawE->at(ii)/elegsfTrkP->at(i);
		eoverp = sc_mult55_E->at(ii)/elegsfTrkP->at(i);
		
		sc_m55_raw_eoverp_1.push_back(raw_eoverp);
		sc_m55_eoverp_1.push_back(eoverp);
		
		ele_end_P_1.push_back(elegsfTrkP->at(i));
		ele_end_Pt_1.push_back(elegsfTrkPt->at(i));
		ele_end_Eta_1.push_back(elegsfTrkEta->at(i));
		ele_end_Phi_1.push_back(elegsfTrkPhi->at(i));
		ele_end_Charge_1.push_back(elegsfTrkCharge->at(i));
		
		sc_m55_et_1      = sc_mult55_Et->at(ii);
		sc_m55_eta_1     = sc_mult55_Eta->at(ii);
		sc_m55_phi_1     = sc_mult55_Phi->at(ii);
		sc_m55_energy_1  = sc_mult55_E->at(ii);
		sc_m55_raw_et_1  = sc_mult55_RawEt->at(ii);
		sc_m55_raw_energy_1 = sc_mult55_RawE->at(ii);
		sc_m55_eta_corr_1    = sc_m55_eta_corr;
		sc_m55_phi_corr_1    = sc_m55_phi_corr;
		
		ele_end_pt_1   = elegsfTrkPt->at(i);
		ele_end_eta_1  = elegsfTrkEta->at(i);
		ele_end_phi_1  = elegsfTrkPhi->at(i);
		
		ele1_m55_match = true;
		
	      }
	      //cout << "****************deta_2 " << deta_2 << endl;
	      
	      
	      if(deta_2 < 0.05 && dphi_2 < 0.5 ){
		//cout << "after deta 2 cut " << endl;
		//cout << "mult55 eta:" << sc_mult55_Eta->at(ii) << " track eta: " << elegsfTrkEta->at(j) << " deta_2: " << deta_2 << endl;
		
		sc_m55_E_2.push_back(sc_mult55_E->at(ii));
		sc_m55_Et_2.push_back(sc_mult55_Et->at(ii));
		sc_m55_Eta_2.push_back(sc_mult55_Eta->at(ii));
		sc_m55_Phi_2.push_back(sc_mult55_Phi->at(ii));
		sc_m55_Eta_corr_2.push_back(sc_m55_eta_corr);
		sc_m55_Phi_corr_2.push_back(sc_m55_phi_corr);
		sc_m55_RawE_2.push_back(sc_mult55_RawE->at(ii));
		sc_m55_RawEt_2.push_back(sc_mult55_RawEt->at(ii));

		raw_eoverp = sc_mult55_RawE->at(ii)/elegsfTrkP->at(j);
		eoverp = sc_mult55_E->at(ii)/elegsfTrkP->at(j);
		
		sc_m55_raw_eoverp_2.push_back(raw_eoverp);
		sc_m55_eoverp_2.push_back(eoverp);
		
		ele_end_P_2.push_back(elegsfTrkP->at(j));
		ele_end_Pt_2.push_back(elegsfTrkPt->at(j));
		ele_end_Eta_2.push_back(elegsfTrkEta->at(j));
		ele_end_Phi_2.push_back(elegsfTrkPhi->at(j));
		ele_end_Charge_2.push_back(elegsfTrkCharge->at(j));
		
		sc_m55_et_2  = sc_mult55_Et->at(ii);
		sc_m55_eta_2 = sc_mult55_Eta->at(ii);
		sc_m55_phi_2 = sc_mult55_Phi->at(ii);
		sc_m55_energy_2 = sc_mult55_E->at(ii);
		sc_m55_raw_et_2  = sc_mult55_RawEt->at(ii);
		sc_m55_raw_energy_2 = sc_mult55_RawE->at(ii);
		sc_m55_eta_corr_2    = sc_m55_eta_corr;
		sc_m55_phi_corr_2    = sc_m55_phi_corr;
		
		ele_end_pt_2   = elegsfTrkPt->at(j);
		ele_end_eta_2  = elegsfTrkEta->at(j);
		ele_end_phi_2  = elegsfTrkPhi->at(j);
		
		ele2_m55_match = true;
	      }
	      //mult55 cluster 
	      
	      if(ele1_m55_match && ele2_m55_match){
		
		TLorentzVector v7, v8, vSum4;
		v7.SetPtEtaPhiE( sc_m55_et_1 ,  sc_m55_eta_corr_1,
				 sc_m55_phi_corr_1,  sc_m55_energy_1);
		v8.SetPtEtaPhiE( sc_m55_et_2 ,  sc_m55_eta_corr_2,
				 sc_m55_phi_corr_2,  sc_m55_energy_2);
		vSum4 = v7+v8;
		
		vSum_m55_reco_M.push_back(vSum4.M());
		vSum_m55_reco_Energy.push_back(vSum4.Energy());
		vSum_m55_reco_Et.push_back(vSum4.Et());
		vSum_m55_reco_Pt.push_back(vSum4.Pt());
		vSum_m55_reco_Eta.push_back(vSum4.Eta());
		vSum_m55_reco_Phi.push_back(vSum4.Phi());
		vSum_m55_reco_Rapidity.push_back(vSum4.Rapidity());
		
		double m55_theta = v7.Angle(v8.Vect()); 
		vSum_m55_reco_Theta.push_back(m55_theta);
		
		double dicluster_m55_invmass = getInvMass(sc_m55_energy_1, sc_m55_eta_corr_1, sc_m55_phi_corr_1,  
							  sc_m55_energy_2, sc_m55_eta_corr_2, sc_m55_phi_corr_2);  
		
		invmass_m55_cluster_reco_formula.push_back(dicluster_m55_invmass);
		
		
		  // fill dicluster info with raw energy
		TLorentzVector v7_raw, v8_raw, vSum4_raw;
		v7_raw.SetPtEtaPhiE( sc_m55_raw_et_1 ,  sc_m55_eta_corr_1,
				     sc_m55_phi_corr_1    ,  sc_m55_raw_energy_1);
		v8_raw.SetPtEtaPhiE( sc_m55_raw_et_2 ,  sc_m55_eta_corr_2,
				     sc_m55_phi_corr_2    ,  sc_m55_raw_energy_2);
		vSum4_raw = v7_raw + v8_raw;
		  
		vSum_m55_raw_M.push_back(vSum4_raw.M());
		vSum_m55_raw_Energy.push_back(vSum4_raw.Energy());
		vSum_m55_raw_Et.push_back(vSum4_raw.Et());
		vSum_m55_raw_Pt.push_back(vSum4_raw.Pt());
		vSum_m55_raw_Eta.push_back(vSum4_raw.Eta());
		vSum_m55_raw_Phi.push_back(vSum4_raw.Phi());
		vSum_m55_raw_Rapidity.push_back(vSum4_raw.Rapidity());
		  
		double m55_theta_raw = v7_raw.Angle(v8_raw.Vect()); 
		vSum_m55_raw_Theta.push_back(m55_theta_raw);
		
		double dicluster_m55_raw_invmass = getInvMass(sc_m55_energy_1, sc_m55_eta_corr_1, sc_m55_phi_corr_1,  
							      sc_m55_energy_2, sc_m55_eta_corr_2, sc_m55_phi_corr_2);  

		invmass_m55_cluster_raw_formula.push_back(dicluster_m55_raw_invmass);
		
		
		// fill dielectron info 
		TLorentzVector v9, v10, vSum5;
		v9.SetPtEtaPhiM( ele_end_pt_1  ,  ele_end_eta_1,
				 ele_end_phi_1 ,  eleMass);
		v10.SetPtEtaPhiM( ele_end_pt_2  ,  ele_end_eta_2,
				    ele_end_phi_2 ,  eleMass);
		vSum5 = v9 + v10;
		
		vSum_ee_m55_M.push_back(vSum5.M());
		vSum_ee_m55_Energy.push_back(vSum5.Energy());
		vSum_ee_m55_Et.push_back(vSum5.Et());
		vSum_ee_m55_Pt.push_back(vSum5.Pt());
		vSum_ee_m55_Eta.push_back(vSum5.Eta());
		vSum_ee_m55_Phi.push_back(vSum5.Phi());
		vSum_ee_m55_Rapidity.push_back(vSum5.Rapidity());
		
		double ee_m55_theta = v9.Angle(v10.Vect()); 
		vSum_ee_m55_Theta.push_back(ee_m55_theta);
		
		//cout << "cluster invariant mass:" << vSum4.M() << " track inv mass:" << vSum.M() << " track-ele matched:" << vSum5.M() << endl;
	      } // fill di-cluster info if both electrons match
	    } // multi 55
	    
	  } //eta cut
	  // }// electron acoplanarity
	}// elej
      } // ele i
      
      outputTreediEle->Fill();
      outputTreePho->Fill();
      outputTreediPho->Fill();
      
    } //trigger
  } //event loop
  std::cout<<  "Loop ENDED : ggHiNtuplizer/EventTree" <<std::endl;
  std::cout << "entries            = " << entries << std::endl;
  
  std::cout << "entriesAnalyzed  after trigger  = " << entriesAnalyzed << std::endl;
  std::cout << "after 2 photon exclusive cut    = " << excl << std::endl;
  std::cout << "for reconstruction number       = " << myloop << std::endl;
  std::cout << "photons after applying ieta id  = " << id << std::endl;
  std::cout << "outputTreeMC->GetEntries()   = " << outputTreeMC->GetEntries() << std::endl;
  std::cout << "outputTreeHLT->GetEntries()   = " << outputTreeHLT->GetEntries() << std::endl;
  std::cout << "outputTreediEle->GetEntries()      = " << outputTreediEle->GetEntries() << std::endl;
  std::cout << "outputTreediPho->GetEntries()      = " << outputTreediPho->GetEntries() << std::endl;
  
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
  data_hm_tree_sc_corr(argv[1]);
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

