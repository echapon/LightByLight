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
const float cutPt = 10;
const float cutEta = 1.4791;
const float cutDeltaR = 0.15;
const long MAXTREESIZE = 200000000000; // set maximum tree size from 10 GB to 100 GB, so that the code does not switch to a new file after 10 GB7

const double eleMass = 0.000511;
const double muMass  = 0.105658;
float Nevt = 4600000;
//float Nevt = 10000;
float Xsection = 9.00; //mb 
float luminosity = 0.4;   //0.4 nb

float wt=(Xsection*0.4*1000000)/Nevt; 


const bool looseIsolation  = true;
#define PI 3.141592653589
const float awayRange = PI * 7./8.;

//void mycheck( );
Double_t getDR  ( Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2);
Double_t getDPHI( Double_t phi1, Double_t phi2);
Double_t getDETA(Double_t eta1, Double_t eta2);

void efficiency_diphoton( std::string infile_Forest = "exclgg_mc_forests.txt",
			 std::string out = "hm_photon_efficiency_gen_reco_match_v2.root") 
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
  
 
 
  treeHLT->SetBranchStatus("*",0);     // disable all branches
  treeHLT->SetBranchStatus("HLT_HIUPCL1SingleEG5NotHF2_v1",1);     // enable photon branches
  treeHLT->SetBranchStatus("HLT_HIUPCL1DoubleEG2*",1);     // enable photon branches
  
  Int_t HLT_HIUPCL1DoubleEG2NotHF2_v1 ;   Int_t HLT_HIUPCL1SingleEG5NotHF2_v1;
  
  treeHLT->SetBranchAddress("HLT_HIUPCL1SingleEG5NotHF2_v1",&HLT_HIUPCL1SingleEG5NotHF2_v1);     // enable photon branches
  treeHLT->SetBranchAddress("HLT_HIUPCL1DoubleEG2NotHF2_v1",&HLT_HIUPCL1DoubleEG2NotHF2_v1);     // enable photon branches
  
 // event information
  Int_t run, lumis;
  Long64_t event;
  tree->SetBranchStatus("*",0);     // disable all branches
  
  tree->SetBranchStatus("run",1);
  tree->SetBranchStatus("event",1);
  tree->SetBranchStatus("lumis",1);
  tree->SetBranchAddress("run", &run);
  tree->SetBranchAddress("event", &event);
  tree->SetBranchAddress("lumis", &lumis);
   

  // RECO photons
  Int_t nMC;
  std::vector<float>* mcE=0;
  std::vector<float>* mcEt=0;
  std::vector<float>* mcEta=0;
  std::vector<float>* mcPhi=0;

  Int_t nHyPho;
  std::vector<float>* hyphoE=0;
  std::vector<float>* hyphoEt=0;
  std::vector<float>* hyphoEta=0;
  std::vector<float>* hyphoSCEta=0;
  std::vector<float>* hyphoPhi=0;
  std::vector<float>* hypho_ecalClusterIsoR4=0;
  std::vector<float>* hypho_hcalRechitIsoR4=0;
  std::vector<float>* hypho_trackIsoR4PtCut20=0;
  std::vector<float>* hyphoHoverE=0;
  std::vector<float>* hyphoSigmaIEtaIEta=0;
  std::vector<float>* hypho_swissCrx=0;
  std::vector<float>* hypho_seedTime=0;
  std::vector<int>*   hyphohasPixelSeed=0;
  std::vector<int>*   hyphopassConversionVeto=0;


  
  // RECO electrons
  Int_t nEle;

  
  // RECO muons
  Int_t nMu;
  
  
  tree->SetBranchStatus("nMC",1);     // enable photon branches
  tree->SetBranchStatus("mc*",1);     // enable photon branches
  tree->SetBranchAddress("nMC",&nMC);
  tree->SetBranchAddress("mcE",&mcE);
  tree->SetBranchAddress("mcEt",&mcEt);
  tree->SetBranchAddress("mcEta",&mcEta);
  tree->SetBranchAddress("mcPhi",&mcPhi);


  tree->SetBranchStatus("nHyPho",1);     // enable photon branches
  tree->SetBranchStatus("hypho*",1);     // enable photon branches
  tree->SetBranchAddress("nHyPho",&nHyPho);
  tree->SetBranchAddress("hyphoE",&hyphoE);
  tree->SetBranchAddress("hyphoEt",&hyphoEt);
  tree->SetBranchAddress("hyphoEta",&hyphoEta);
  tree->SetBranchAddress("hyphoSCEta",&hyphoSCEta);
  tree->SetBranchAddress("hyphoPhi",&hyphoPhi);
  tree->SetBranchAddress("hypho_ecalClusterIsoR4",&hypho_ecalClusterIsoR4);
  tree->SetBranchAddress("hypho_hcalRechitIsoR4",&hypho_hcalRechitIsoR4);
  tree->SetBranchAddress("hypho_trackIsoR4PtCut20",&hypho_trackIsoR4PtCut20);
  tree->SetBranchAddress("hyphoHoverE",&hyphoHoverE);
  tree->SetBranchAddress("hyphoSigmaIEtaIEta",&hyphoSigmaIEtaIEta);
  tree->SetBranchAddress("hypho_swissCrx",&hypho_swissCrx);
  tree->SetBranchAddress("hypho_seedTime",&hypho_seedTime);
  tree->SetBranchAddress("hyphohasPixelSeed",&hyphohasPixelSeed);
  tree->SetBranchAddress("hyphopassConversionVeto",&hyphopassConversionVeto);

  tree->SetBranchStatus("nEle",1);     // enable electron branches
  tree->SetBranchStatus("ele*",1);     // enable electron branches
  tree->SetBranchAddress("nEle",&nEle);
  
  tree->SetBranchStatus("nMu",1);     // enable muon branches
  tree->SetBranchStatus("mu*",1);     // enable muon branches
  tree->SetBranchAddress("nMu",&nMu);

  // RECO electrons
  Int_t ngsfEle;
  tree->SetBranchStatus("ngsfEle",1);     // enable electron branches
  tree->SetBranchAddress("ngsfEle",&ngsfEle);

  Int_t ngenTrk;
  tree->SetBranchStatus("ngenTrk",1);     // enable electron branches
  tree->SetBranchAddress("ngenTrk",&ngenTrk);

  Int_t nhits1, nhits2, nhits3, nhits4, nhits5, nEv;
  treePixel->SetBranchStatus("nEv",1);     // enable photon branches
  treePixel->SetBranchAddress("nEv",&nEv);     // enable photon branches
  treePixel->SetBranchStatus("nhits*",1);     // enable pixel branches
  treePixel->SetBranchAddress("nhits1",&nhits1);     
  treePixel->SetBranchAddress("nhits2",&nhits2);     
  treePixel->SetBranchAddress("nhits3",&nhits3);   
  treePixel->SetBranchAddress("nhits4",&nhits4);     
  treePixel->SetBranchAddress("nhits5",&nhits5);   

  // calotower 
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
 
  TFile *output;
   output = new TFile(Form("%s",out.c_str()),"recreate");

  // output tree variables

  TTree *outputTreeHLT    = treeHLT->CloneTree(0);
  outputTreeHLT->SetName("hltTree");
         
 
  TTree *outputTreediPho  = new TTree("diphoton","");
  
  outputTreeHLT->SetMaxTreeSize(MAXTREESIZE);
  outputTreediPho->SetMaxTreeSize(MAXTREESIZE);
 

  Int_t phoIdx;

  //outputTreeGammaJet->Branch("phoIdx",&phoIdx);   
  // output tree variables
  std::vector<float> genphoEt1_all;
  std::vector<float> genphoEta1_all;
  std::vector<float> genphoPhi1_all;
  std::vector<float> genphoEt2_all;
  std::vector<float> genphoEta2_all;
  std::vector<float> genphoPhi2_all;


  std::vector<float> genphoEt1_accp;
  std::vector<float> genphoEta1_accp;
  std::vector<float> genphoPhi1_accp;
  std::vector<float> genphoEt2_accp;
  std::vector<float> genphoEta2_accp;
  std::vector<float> genphoPhi2_accp;
  
  
  std::vector<float> phoEt_reco;
  std::vector<float> phoEta_reco;
  std::vector<float> phoPhi_reco;
  std::vector<float> pho_ecalClusterIsoR4_reco;
  std::vector<float> pho_hcalRechitIsoR4_reco;
  std::vector<float> pho_trackIsoR4PtCut20_reco;
  std::vector<float> phoHoverE_reco;
  std::vector<float> phoSigmaIEtaIEta_reco;
  std::vector<float> pho_swissCrx_reco;
  std::vector<float> pho_seedTime_reco;
  std::vector<int>   phohasPixelSeed_reco;
  std::vector<int>   phopassConversionVeto_reco;
   

  std::vector<float> phoEt_gen_reco_1;
  std::vector<float> phoEta_gen_reco_1;
  std::vector<float> phoSCEta_gen_reco_1;
  std::vector<float> phoPhi_gen_reco_1;
  std::vector<float> pho_ecalClusterIsoR4_gen_reco_1;
  std::vector<float> pho_hcalRechitIsoR4_gen_reco_1;
  std::vector<float> pho_trackIsoR4PtCut20_gen_reco_1;
  std::vector<float> phoHoverE_gen_reco_1;
  std::vector<float> phoSigmaIEtaIEta_gen_reco_1;
  std::vector<float> pho_swissCrx_gen_reco_1;
  std::vector<float> pho_seedTime_gen_reco_1;
  std::vector<int>   phohasPixelSeed_gen_reco_1;
  std::vector<int>   phopassConversionVeto_gen_reco_1;

  std::vector<float> phoEt_gen_reco_2;
  std::vector<float> phoEta_gen_reco_2;
  std::vector<float> phoSCEta_gen_reco_2;
  std::vector<float> phoPhi_gen_reco_2;
  std::vector<float> pho_ecalClusterIsoR4_gen_reco_2;
  std::vector<float> pho_hcalRechitIsoR4_gen_reco_2;
  std::vector<float> pho_trackIsoR4PtCut20_gen_reco_2;
  std::vector<float> phoHoverE_gen_reco_2;
  std::vector<float> phoSigmaIEtaIEta_gen_reco_2;
  std::vector<float> pho_swissCrx_gen_reco_2;
  std::vector<float> pho_seedTime_gen_reco_2;
  std::vector<int>   phohasPixelSeed_gen_reco_2;
  std::vector<int>   phopassConversionVeto_gen_reco_2;
  
  std::vector<float> vSum_gg_M;
  std::vector<float> vSum_gg_Pt;
  std::vector<float> vSum_gg_Rapidity;
  std::vector<float>  pho_aco;
  
  
  std::vector<float> recoEt_doubleEG2_1;
  std::vector<float> recoEta_doubleEG2_1;
  std::vector<float> recoPhi_doubleEG2_1;
  std::vector<float> reco_ecalClusterIsoR4_doubleEG2_1;
  std::vector<float> reco_hcalRechitIsoR4_doubleEG2_1;
  std::vector<float> reco_trackIsoR4PtCut20_doubleEG2_1;
  std::vector<float> recoHoverE_doubleEG2_1;
  std::vector<float> recoSigmaIEtaIEta_doubleEG2_1;
  std::vector<float> reco_swissCrx_doubleEG2_1;
  std::vector<float> reco_seedTime_doubleEG2_1;
  std::vector<int>   recohasPixelSeed_doubleEG2_1;
  std::vector<int>   recopassConversionVeto_doubleEG2_1;


  std::vector<float> recoEt_doubleEG2_2;
  std::vector<float> recoEta_doubleEG2_2;
  std::vector<float> recoPhi_doubleEG2_2;
  std::vector<float> reco_ecalClusterIsoR4_doubleEG2_2;
  std::vector<float> reco_hcalRechitIsoR4_doubleEG2_2;
  std::vector<float> reco_trackIsoR4PtCut20_doubleEG2_2;
  std::vector<float> recoHoverE_doubleEG2_2;
  std::vector<float> recoSigmaIEtaIEta_doubleEG2_2;
  std::vector<float> reco_swissCrx_doubleEG2_2;
  std::vector<float> reco_seedTime_doubleEG2_2;
  std::vector<int>   recohasPixelSeed_doubleEG2_2;
  std::vector<int>   recopassConversionVeto_doubleEG2_2;

  std::vector<float> vSum_gg_M_doubleEG2;
  std::vector<float> vSum_gg_Pt_doubleEG2;
  std::vector<float> vSum_gg_Rapidity_doubleEG2;
  std::vector<float>  pho_aco_doubleEG2;
  
  std::vector<float>  leadingEmEnergy_EB;
  std::vector<float>  leadingEmEnergy_EE;
  std::vector<float>  leadingHadEnergy_HB; 
  std::vector<float>  leadingHadEnergy_HE;
  std::vector<float>  leadingHadEnergy_HF_Plus;
  std::vector<float>  leadingHadEnergy_HF_Minus;
  
  int pix_hit1;
  int pix_hit2;
  int pix_hit3;
  int pix_hit4;
  int pix_hit5;
  
  
  
  outputTreediPho->Branch("nMC",&nMC);
  outputTreediPho->Branch("genphoEt1_all", &genphoEt1_all);
  outputTreediPho->Branch("genphoEta1_all",&genphoEta1_all);
  outputTreediPho->Branch("genphoPhi1_all",&genphoPhi1_all);
  outputTreediPho->Branch("genphoEt2_all", &genphoEt2_all);
  outputTreediPho->Branch("genphoEta2_all",&genphoEta2_all);
  outputTreediPho->Branch("genphoPhi2_all",&genphoPhi2_all);
  
  outputTreediPho->Branch("genphoEt1_accp", &genphoEt1_accp);
  outputTreediPho->Branch("genphoEta1_accp",&genphoEta1_accp);
  outputTreediPho->Branch("genphoPhi1_accp",&genphoPhi1_accp);
  outputTreediPho->Branch("genphoEt2_accp", &genphoEt2_accp);
  outputTreediPho->Branch("genphoEta2_accp",&genphoEta2_accp);
  outputTreediPho->Branch("genphoPhi2_accp",&genphoPhi2_accp);

  outputTreediPho->Branch("nMu",&nMu);
  outputTreediPho->Branch("ngenTrk",&ngenTrk);
  outputTreediPho->Branch("ngsfEle",&ngsfEle);
  outputTreediPho->Branch("nEle",&nEle);
  outputTreediPho->Branch("nHyPho",&nHyPho);



  outputTreediPho->Branch("phoEt_gen_reco_1",&phoEt_gen_reco_1);
  outputTreediPho->Branch("phoEta_gen_reco_1",&phoEta_gen_reco_1);
  outputTreediPho->Branch("phoSCEta_gen_reco_1",&phoSCEta_gen_reco_1);
  outputTreediPho->Branch("phoPhi_gen_reco_1",&phoPhi_gen_reco_1);
  outputTreediPho->Branch("pho_ecalClusterIsoR4_gen_reco_1",&pho_ecalClusterIsoR4_gen_reco_1);
  outputTreediPho->Branch("pho_hcalRechitIsoR4_gen_reco_1",&pho_hcalRechitIsoR4_gen_reco_1);
  outputTreediPho->Branch("pho_trackIsoR4PtCut20_gen_reco_1",&pho_trackIsoR4PtCut20_gen_reco_1);
  outputTreediPho->Branch("phoHoverE_gen_reco_1",&phoHoverE_gen_reco_1);
  outputTreediPho->Branch("phoSigmaIEtaIEta_gen_reco_1",&phoSigmaIEtaIEta_gen_reco_1);
  outputTreediPho->Branch("pho_swissCrx_gen_reco_1",&pho_swissCrx_gen_reco_1);
  outputTreediPho->Branch("pho_seedTime_gen_reco_1",&pho_seedTime_gen_reco_1);
  outputTreediPho->Branch("phohasPixelSeed_gen_reco_1",&phohasPixelSeed_gen_reco_1);
  outputTreediPho->Branch("phopassConversionVeto_gen_reco_1",&phopassConversionVeto_gen_reco_1);

  outputTreediPho->Branch("phoEt_gen_reco_2",&phoEt_gen_reco_2);
  outputTreediPho->Branch("phoEta_gen_reco_2",&phoEta_gen_reco_2);
  outputTreediPho->Branch("phoSCEta_gen_reco_2",&phoSCEta_gen_reco_2);
  outputTreediPho->Branch("phoPhi_gen_reco_2",&phoPhi_gen_reco_2);
  outputTreediPho->Branch("pho_ecalClusterIsoR4_gen_reco_2",&pho_ecalClusterIsoR4_gen_reco_2);
  outputTreediPho->Branch("pho_hcalRechitIsoR4_gen_reco_2",&pho_hcalRechitIsoR4_gen_reco_2);
  outputTreediPho->Branch("pho_trackIsoR4PtCut20_gen_reco_2",&pho_trackIsoR4PtCut20_gen_reco_2);
  outputTreediPho->Branch("phoHoverE_gen_reco_2",&phoHoverE_gen_reco_2);
  outputTreediPho->Branch("phoSigmaIEtaIEta_gen_reco_2",&phoSigmaIEtaIEta_gen_reco_2);
  outputTreediPho->Branch("pho_swissCrx_gen_reco_2",&pho_swissCrx_gen_reco_2);
  outputTreediPho->Branch("pho_seedTime_gen_reco_2",&pho_seedTime_gen_reco_2);
  outputTreediPho->Branch("phohasPixelSeed_gen_reco_2",&phohasPixelSeed_gen_reco_2);
  outputTreediPho->Branch("phopassConversionVeto_gen_reco_2",&phopassConversionVeto_gen_reco_2);

  outputTreediPho->Branch("vSum_M",&vSum_gg_M);
  outputTreediPho->Branch("vSum_Pt",&vSum_gg_Pt);
  outputTreediPho->Branch("vSum_Rapidity",&vSum_gg_Rapidity);
  outputTreediPho->Branch("pho_aco",&pho_aco);
 

  outputTreediPho->Branch("recoEt_doubleEG2_1",  &recoEt_doubleEG2_1);
  outputTreediPho->Branch("recoEta_doubleEG2_1", &recoEta_doubleEG2_1);
  outputTreediPho->Branch("recoPhi_doubleEG2_1",&recoPhi_doubleEG2_1);
  outputTreediPho->Branch("reco_ecalClusterIsoR4_doubleEG2_1",&reco_ecalClusterIsoR4_doubleEG2_1);
  outputTreediPho->Branch("reco_hcalRechitIsoR4_doubleEG2_1",&reco_hcalRechitIsoR4_doubleEG2_1);
  outputTreediPho->Branch("reco_trackIsoR4PtCut20_doubleEG2_1",&reco_trackIsoR4PtCut20_doubleEG2_1);
  outputTreediPho->Branch("recoHoverE_doubleEG2_1",&recoHoverE_doubleEG2_1);
  outputTreediPho->Branch("recoSigmaIEtaIEta_doubleEG2_1",&recoSigmaIEtaIEta_doubleEG2_1);
  outputTreediPho->Branch("reco_swissCrx_doubleEG2_1",&reco_swissCrx_doubleEG2_1);
  outputTreediPho->Branch("reco_seedTime_doubleEG2_1",&reco_seedTime_doubleEG2_1);
  outputTreediPho->Branch("recohasPixelSeed_doubleEG2_1",&recohasPixelSeed_doubleEG2_1);
  outputTreediPho->Branch("recopassConversionVeto_doubleEG2_1",&recopassConversionVeto_doubleEG2_1);



  outputTreediPho->Branch("recoEt_doubleEG2_2",  &recoEt_doubleEG2_2);
  outputTreediPho->Branch("recoEta_doubleEG2_2", &recoEta_doubleEG2_2);
  outputTreediPho->Branch("recoPhi_doubleEG2_2",&recoPhi_doubleEG2_2);
  outputTreediPho->Branch("reco_ecalClusterIsoR4_doubleEG2_2",&reco_ecalClusterIsoR4_doubleEG2_2);
  outputTreediPho->Branch("reco_hcalRechitIsoR4_doubleEG2_2",&reco_hcalRechitIsoR4_doubleEG2_2);
  outputTreediPho->Branch("reco_trackIsoR4PtCut20_doubleEG2_2",&reco_trackIsoR4PtCut20_doubleEG2_2);
  outputTreediPho->Branch("recoHoverE_doubleEG2_2",&recoHoverE_doubleEG2_2);
  outputTreediPho->Branch("recoSigmaIEtaIEta_doubleEG2_2",&recoSigmaIEtaIEta_doubleEG2_2);
  outputTreediPho->Branch("reco_swissCrx_doubleEG2_2",&reco_swissCrx_doubleEG2_2);
  outputTreediPho->Branch("reco_seedTime_doubleEG2_2",&reco_seedTime_doubleEG2_2);
  outputTreediPho->Branch("recohasPixelSeed_doubleEG2_2",&recohasPixelSeed_doubleEG2_2);
  outputTreediPho->Branch("recopassConversionVeto_doubleEG2_2",&recopassConversionVeto_doubleEG2_2);
  
  outputTreediPho->Branch("vSum_M_doubleEG2",&vSum_gg_M_doubleEG2);
  outputTreediPho->Branch("vSum_Pt_doubleEG2",&vSum_gg_Pt_doubleEG2);
  outputTreediPho->Branch("vSum_Rapidity_doubleEG2",&vSum_gg_Rapidity_doubleEG2);
  outputTreediPho->Branch("pho_aco_doubleEG2",&pho_aco_doubleEG2);
  
  outputTreediPho->Branch("nTower",&nTower);
  outputTreediPho->Branch("leadingEmEnergy_EB",&leadingEmEnergy_EB);
  outputTreediPho->Branch("leadingEmEnergy_EE",&leadingEmEnergy_EE);
  outputTreediPho->Branch("leadingHadEnergy_HB",&leadingHadEnergy_HB);
  outputTreediPho->Branch("leadingHadEnergy_HE",&leadingHadEnergy_HE);
  outputTreediPho->Branch("leadingHadEnergy_HF_Plus",&leadingHadEnergy_HF_Plus);
  outputTreediPho->Branch("leadingHadEnergy_HF_Minus",&leadingHadEnergy_HF_Minus);
  
  outputTreediPho->Branch("pix_hit1",&pix_hit1);
  outputTreediPho->Branch("pix_hit2",&pix_hit2);
  outputTreediPho->Branch("pix_hit3",&pix_hit3);
  outputTreediPho->Branch("pix_hit4",&pix_hit4);
  outputTreediPho->Branch("pix_hit5",&pix_hit5);
  
  
  //EventMatcher* em = new EventMatcher();
  //Long64_t duplicateEntries = 0;

  double phodphi;
  double aco;

  double phodphi2;
  double aco2;
  
  Long64_t entries = tree->GetEntries();
  Long64_t entriesAnalyzed = 0;   Long64_t id = 0;   Long64_t excl = 0; Long64_t ptcut = 0;
  std::cout << "entries         = " << entries << std::endl;
  std::cout<< "Loop : ggHiNtuplizer/EventTree" <<std::endl;
  
  //for (Long64_t j_entry=0; j_entry<100; ++j_entry){
  for (Long64_t j_entry=0; j_entry<entries; ++j_entry){
    
    if (j_entry % 10000 == 0)  {
      std::cout << "current entry = " <<j_entry<<" out of "<<entries<<" : "<<std::setprecision(2)<<(double)j_entry/entries*100<<" %"<<std::endl;
    }
    
    treeHLT->GetEntry(j_entry);
    tree->GetEntry(j_entry);
    treePixel->GetEntry(j_entry);
    
    genphoEt1_all.clear();
    genphoEta1_all.clear();
    genphoPhi1_all.clear();
    genphoEt2_all.clear();
    genphoEta2_all.clear();
    genphoPhi2_all.clear();
    
    genphoEt1_accp.clear();
    genphoEta1_accp.clear();
    genphoPhi1_accp.clear();
    genphoEt2_accp.clear();
    genphoEta2_accp.clear();
    genphoPhi2_accp.clear();
    
    
    // diphoton block
     
    phoEt_gen_reco_1.clear();
    phoEta_gen_reco_1.clear();
    phoSCEta_gen_reco_1.clear();
    phoPhi_gen_reco_1.clear();
    pho_ecalClusterIsoR4_gen_reco_1.clear();
    pho_hcalRechitIsoR4_gen_reco_1.clear();
    pho_trackIsoR4PtCut20_gen_reco_1.clear();
    phoHoverE_gen_reco_1.clear();
    phoSigmaIEtaIEta_gen_reco_1.clear();
    pho_swissCrx_gen_reco_1.clear();
    pho_seedTime_gen_reco_1.clear();
    phohasPixelSeed_gen_reco_1.clear();
    phopassConversionVeto_gen_reco_1.clear();


    phoEt_gen_reco_2.clear();
    phoEta_gen_reco_2.clear();
    phoSCEta_gen_reco_2.clear();
    phoPhi_gen_reco_2.clear();
    pho_ecalClusterIsoR4_gen_reco_2.clear();
    pho_hcalRechitIsoR4_gen_reco_2.clear();
    pho_trackIsoR4PtCut20_gen_reco_2.clear();
    phoHoverE_gen_reco_2.clear();
    phoSigmaIEtaIEta_gen_reco_2.clear();
    pho_swissCrx_gen_reco_2.clear();
    pho_seedTime_gen_reco_2.clear();
    phohasPixelSeed_gen_reco_2.clear();
    phopassConversionVeto_gen_reco_2.clear();
    
    vSum_gg_M.clear();
    vSum_gg_Pt.clear();
    vSum_gg_Rapidity.clear();
    pho_aco.clear();
    
    recoEt_doubleEG2_1.clear();
    recoEta_doubleEG2_1.clear();
    recoPhi_doubleEG2_1.clear();
    reco_ecalClusterIsoR4_doubleEG2_1.clear();
    reco_hcalRechitIsoR4_doubleEG2_1.clear();
    reco_trackIsoR4PtCut20_doubleEG2_1.clear();
    recoHoverE_doubleEG2_1.clear();
    recoSigmaIEtaIEta_doubleEG2_1.clear();
    reco_swissCrx_doubleEG2_1.clear();
    reco_seedTime_doubleEG2_1.clear();
    recohasPixelSeed_doubleEG2_1.clear();
    recopassConversionVeto_doubleEG2_1.clear();

    recoEt_doubleEG2_2.clear();
    recoEta_doubleEG2_2.clear();
    recoPhi_doubleEG2_2.clear();
    reco_ecalClusterIsoR4_doubleEG2_2.clear();
    reco_hcalRechitIsoR4_doubleEG2_2.clear();
    reco_trackIsoR4PtCut20_doubleEG2_2.clear();
    recoHoverE_doubleEG2_2.clear();
    recoSigmaIEtaIEta_doubleEG2_2.clear();
    reco_swissCrx_doubleEG2_2.clear();
    reco_seedTime_doubleEG2_2.clear();
    recohasPixelSeed_doubleEG2_2.clear();
    recopassConversionVeto_doubleEG2_2.clear();
    pho_aco_doubleEG2.clear();

    vSum_gg_M_doubleEG2.clear();
    vSum_gg_Pt_doubleEG2.clear();
    vSum_gg_Rapidity_doubleEG2.clear();
    
    
    leadingEmEnergy_EB.clear();
    leadingEmEnergy_EE.clear();
    leadingHadEnergy_HB.clear();
    leadingHadEnergy_HE.clear();
    leadingHadEnergy_HF_Plus.clear();
    leadingHadEnergy_HF_Minus.clear();
    
    for(int ii=0; ii<nMC; ++ii){
      for(int jj=ii+1; jj<nMC; ++jj){
	
	genphoEt1_all.push_back(mcEt->at(ii));
	genphoEta1_all.push_back(mcEta->at(ii));
	genphoPhi1_all.push_back(mcPhi->at(ii));
	
	genphoEt2_all.push_back(mcEt->at(jj));
	genphoEta2_all.push_back(mcEta->at(jj));
	genphoPhi2_all.push_back(mcPhi->at(jj));
	
	if(fabs(mcEta->at(ii)) < 2.4 && mcEt->at(ii) > 2.0 && fabs(mcEta->at(jj)) < 2.4 && mcEt->at(jj) > 2.0 ){ 
	  genphoEt1_accp.push_back(mcEt->at(ii));
	  genphoEta1_accp.push_back(mcEta->at(ii));
	  genphoPhi1_accp.push_back(mcPhi->at(ii));
	  
	  genphoEt2_accp.push_back(mcEt->at(jj));
	  genphoEta2_accp.push_back(mcEta->at(jj));
	  genphoPhi2_accp.push_back(mcPhi->at(jj));
	  
	} // if in acceptance of CMS
      } // jj gen MC
    }// ii gen MC
    
    
    if(nHyPho==2 ){
      
      for(int i=0; i<nHyPho; ++i){
	for(int j=i+1; j<nHyPho; ++j){
	  
	  //cout << " coming in reco loop" << endl;
	  
	  if(hyphoEt->at(i) > 2.0 && fabs(hyphoEta->at(i)) < 2.4 && hyphoEt->at(j) > 2.0 && fabs(hyphoEta->at(j)) < 2.4){ 
	    
	    for(int aa = 0 ; aa <nMC; ++aa ){    
	      for(int bb = aa+1 ; bb <nMC; ++bb ){    
		
		if(fabs(mcEta->at(aa)) < 2.4 && mcEt->at(aa) > 2.0 && fabs(mcEta->at(bb)) < 2.4 && mcEt->at(bb) > 2.0 ){ 
		  
		  if (getDR(hyphoEta->at(i), hyphoPhi->at(i), mcEta->at(aa), mcPhi->at(aa)) < 0.3 || 
		      getDR(hyphoEta->at(j), hyphoPhi->at(j), mcEta->at(bb), mcPhi->at(bb)) < 0.3 ||	
		      getDR(hyphoEta->at(i), hyphoPhi->at(i), mcEta->at(bb), mcPhi->at(bb)) < 0.3 || 
		      getDR(hyphoEta->at(j), hyphoPhi->at(j), mcEta->at(aa), mcPhi->at(aa)) < 0.3) {
		    
		    
		    
		    excl++;
		    //cout <<"Entry:" << j_entry << "  Gen photon1 Eta:" << mcEta->at(aa) << "  Gen photon2 Eta:" << mcEta->at(bb); 
		    //cout <<"  Reco photon1 Eta:" << hyphoEta->at(i) << "  Reco photon2 Eta:" << hyphoEta->at(j) << endl;
		    
		    
		    // branches for reco+ID.. 
		    
		    
		    if(fabs(hyphoEta->at(i)) < 1.4791 && hyphoSigmaIEtaIEta->at(i) > 0.02) continue;
		    if(fabs(hyphoEta->at(j)) < 1.4791 && hyphoSigmaIEtaIEta->at(j) > 0.02) continue;
		    if(fabs(hyphoEta->at(i)) > 1.4791 && hyphoSigmaIEtaIEta->at(i) > 0.06) continue;
		    if(fabs(hyphoEta->at(j)) > 1.4791 && hyphoSigmaIEtaIEta->at(j) > 0.06) continue;
		    
		    phoEt_gen_reco_1.push_back(hyphoEt->at(i));
		    phoEta_gen_reco_1.push_back(hyphoEta->at(i));
		    phoSCEta_gen_reco_1.push_back(hyphoSCEta->at(i));
		    phoPhi_gen_reco_1.push_back(hyphoPhi->at(i));
		    pho_ecalClusterIsoR4_gen_reco_1.push_back(hypho_ecalClusterIsoR4->at(i));
		    pho_hcalRechitIsoR4_gen_reco_1.push_back(hypho_hcalRechitIsoR4->at(i));
		    pho_trackIsoR4PtCut20_gen_reco_1.push_back(hypho_trackIsoR4PtCut20->at(i));
		    phoHoverE_gen_reco_1.push_back(hyphoHoverE->at(i));
		    phoSigmaIEtaIEta_gen_reco_1.push_back(hyphoSigmaIEtaIEta->at(i));
		    pho_swissCrx_gen_reco_1.push_back(hypho_swissCrx->at(i));
		    pho_seedTime_gen_reco_1.push_back(hypho_seedTime->at(i));
		    phohasPixelSeed_gen_reco_1.push_back(hyphohasPixelSeed->at(i));  	
		    
		    
		    phoEt_gen_reco_2.push_back(hyphoEt->at(j));
		    phoEta_gen_reco_2.push_back(hyphoEta->at(j));
		    phoSCEta_gen_reco_2.push_back(hyphoSCEta->at(j));
		    phoPhi_gen_reco_2.push_back(hyphoPhi->at(j));
		    pho_ecalClusterIsoR4_gen_reco_2.push_back(hypho_ecalClusterIsoR4->at(j));
		    pho_hcalRechitIsoR4_gen_reco_2.push_back(hypho_hcalRechitIsoR4->at(j));
		    pho_trackIsoR4PtCut20_gen_reco_2.push_back(hypho_trackIsoR4PtCut20->at(j));
		    phoHoverE_gen_reco_2.push_back(hyphoHoverE->at(j));
		    phoSigmaIEtaIEta_gen_reco_2.push_back(hyphoSigmaIEtaIEta->at(j));
		    pho_swissCrx_gen_reco_2.push_back(hypho_swissCrx->at(j));
		    pho_seedTime_gen_reco_2.push_back(hypho_seedTime->at(j));
		    phohasPixelSeed_gen_reco_2.push_back(hyphohasPixelSeed->at(j));  
		    
		    
		    
		    // diphoton inv mass
		    TLorentzVector v1, v2, vSum;
		    v1.SetPtEtaPhiE( hyphoEt->at(i), hyphoEta->at(i),
				     hyphoPhi->at(i), hyphoE->at(i));
		    v2.SetPtEtaPhiE( hyphoEt->at(j), hyphoEta->at(j),
				     hyphoPhi->at(j), hyphoE->at(j));
		    vSum = v1+v2;
		    
		    vSum_gg_M.push_back(vSum.M());
		    vSum_gg_Pt.push_back(vSum.Pt());
		    vSum_gg_Rapidity.push_back(vSum.Rapidity());

	  	      phodphi = getDPHI(hyphoPhi->at(i) ,hyphoPhi->at(j));
	              aco = 1- (phodphi/3.141592653589); 
	  	      pho_aco.push_back(aco);
		    
		    
		    //if(HLT_HIUPCL1DoubleEG2NotHF2_v1==1 || HLT_HIUPCL1SingleEG5NotHF2_v1==1){  // for trigger efficiency
		    if(HLT_HIUPCL1DoubleEG2NotHF2_v1==1){  // for trigger efficiency
		      
		      id ++ ;
		      
		      recoEt_doubleEG2_1.push_back(hyphoEt->at(i));
		      recoEta_doubleEG2_1.push_back(hyphoEta->at(i));
		      recoPhi_doubleEG2_1.push_back(hyphoPhi->at(i));
		      reco_ecalClusterIsoR4_doubleEG2_1.push_back(hypho_ecalClusterIsoR4->at(i));
		      reco_hcalRechitIsoR4_doubleEG2_1.push_back(hypho_hcalRechitIsoR4->at(i));
		      reco_trackIsoR4PtCut20_doubleEG2_1.push_back(hypho_trackIsoR4PtCut20->at(i));
		      recoHoverE_doubleEG2_1.push_back(hyphoHoverE->at(i));
		      recoSigmaIEtaIEta_doubleEG2_1.push_back(hyphoSigmaIEtaIEta->at(i));
		      reco_swissCrx_doubleEG2_1.push_back(hypho_swissCrx->at(i));
		      reco_seedTime_doubleEG2_1.push_back(hypho_seedTime->at(i));
		      recohasPixelSeed_doubleEG2_1.push_back(hyphohasPixelSeed->at(i));  	
		      
		      recoEt_doubleEG2_2.push_back(hyphoEt->at(j));
		      recoEta_doubleEG2_2.push_back(hyphoEta->at(j));
		      recoPhi_doubleEG2_2.push_back(hyphoPhi->at(j));
		      reco_ecalClusterIsoR4_doubleEG2_2.push_back(hypho_ecalClusterIsoR4->at(j));
		      reco_hcalRechitIsoR4_doubleEG2_2.push_back(hypho_hcalRechitIsoR4->at(j));
		      reco_trackIsoR4PtCut20_doubleEG2_2.push_back(hypho_trackIsoR4PtCut20->at(j));
		      recoHoverE_doubleEG2_2.push_back(hyphoHoverE->at(j));
		      recoSigmaIEtaIEta_doubleEG2_2.push_back(hyphoSigmaIEtaIEta->at(j));
		      reco_swissCrx_doubleEG2_2.push_back(hypho_swissCrx->at(j));
		      reco_seedTime_doubleEG2_2.push_back(hypho_seedTime->at(j));
		      recohasPixelSeed_doubleEG2_2.push_back(hyphohasPixelSeed->at(j));  	
		      
		      
		      // diphoton inv mass
		      TLorentzVector v3, v4, vSum2;
		      v3.SetPtEtaPhiE( hyphoEt->at(i), hyphoEta->at(i),
				       hyphoPhi->at(i), hyphoE->at(i));
		      v4.SetPtEtaPhiE( hyphoEt->at(j), hyphoEta->at(j),
				       hyphoPhi->at(j), hyphoE->at(j));
		      vSum2 = v3+v4;
		      
		      vSum_gg_M_doubleEG2.push_back(vSum2.M());
		      vSum_gg_Pt_doubleEG2.push_back(vSum2.Pt());
		      vSum_gg_Rapidity_doubleEG2.push_back(vSum2.Rapidity());

	  	      phodphi2 = getDPHI(hyphoPhi->at(i) ,hyphoPhi->at(j));
	              aco2 = 1- (phodphi2/3.141592653589); 
	  	      pho_aco_doubleEG2.push_back(aco2);
		      
		      
		      pix_hit1 = nhits1;
		      pix_hit2 = nhits2;
		      pix_hit3 = nhits3;
		      pix_hit4 = nhits4;
		      pix_hit5 = nhits5;
		      
		      
		      float  EmEnergy_EB = 0;
		      float  EmEnergy_EE = 0;
		      float  HadEnergy_HB = 0;
		      float  HadEnergy_HE = 0;
		      float  HadEnergy_HF_Plus = 0;
		      float  HadEnergy_HF_Minus = 0;
		      
		      for(int k = 0; k< nTower; ++k){
			
			if (fabs(CaloTower_eta->at(k)) < 1.479){ //start filling EM tower in barel region  
			  if (getDPHI(CaloTower_phi->at(k), hyphoPhi->at(i))<0.7 && getDETA(CaloTower_eta->at(k), hyphoEta->at(i))<0.15) continue;
			  if (getDPHI(CaloTower_phi->at(k), hyphoPhi->at(j))<0.7 && getDETA(CaloTower_eta->at(k), hyphoEta->at(j))<0.15) continue;
			  if (CaloTower_emE->at(k) > EmEnergy_EB){                         
			    EmEnergy_EB = CaloTower_emE->at(k) ;
			  }
			}
			
			if (fabs(CaloTower_eta->at(k)) > 1.479 && fabs(CaloTower_eta->at(k)) < 3 ){           
			  if (getDPHI(CaloTower_phi->at(k), hyphoPhi->at(i))<0.4 && getDETA(CaloTower_eta->at(k), hyphoEta->at(i))<0.15) continue;
			  if (getDPHI(CaloTower_phi->at(k), hyphoPhi->at(j))<0.4 && getDETA(CaloTower_eta->at(k), hyphoEta->at(j))<0.15) continue;
			  if (CaloTower_emE->at(k) > EmEnergy_EE){                         
			    EmEnergy_EE = CaloTower_emE->at(k) ;
			  }
			}
			
			if (fabs(CaloTower_eta->at(k)) < 1.305){           
			  if (CaloTower_hadE->at(k) > HadEnergy_HB){                         
			    HadEnergy_HB = CaloTower_hadE->at(k) ;
			  }
			}
			
			if (fabs(CaloTower_eta->at(k)) > 1.305 && fabs(CaloTower_eta->at(k)) < 3 ){           
			  if (CaloTower_hadE->at(k) > HadEnergy_HE){                         
			    HadEnergy_HE = CaloTower_hadE->at(k) ;
			  }
			}
			
			if (CaloTower_eta->at(k) > 2.9 && CaloTower_eta->at(k) < 5.2 ){           
			  if (CaloTower_e->at(k) > HadEnergy_HF_Plus){                         
			    HadEnergy_HF_Plus = CaloTower_e->at(k) ;
			  }
			}
			
			if (CaloTower_eta->at(k) < -2.9 && CaloTower_eta->at(k) > -5.2 ){           
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
		    } // trigger
		  } // gen-reco matching
		}//gen cuts
	      } // bb gen loopo
	    }// aa gen loop	  
	    
	    
	  } //reco cuts
	} // j reco loop
      } // i reco loop
    } // exclusivity cuts
    outputTreediPho->Fill();
  }//nentry    
  
  
  std::cout<<  "Loop ENDED : ggHiNtuplizer/EventTree" <<std::endl;
  std::cout << "entries            = " << entries << std::endl;
  
  std::cout << "entriesAnalyzed    = " << entriesAnalyzed << std::endl;
  std::cout << "id    = " << id << std::endl;
  std::cout << "excl    = " << excl << std::endl;
  std::cout << "outputTreeHLT->GetEntries()   = " << outputTreeHLT->GetEntries() << std::endl;
  std::cout << "outputTreediPho->GetEntries()    = " << outputTreediPho->GetEntries() << std::endl;
  
  output->Write();
  output->Close();
}

/*int main(int argc, char** argv)
  {
  if (argc == 3)    mycheck(argv[1], argv[2]);
  if (argc == 2)    mycheck(argv[1]);
  if (argc == 1)    return 1;
  return 0;
  }*/


Double_t getDR( Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2){
  Double_t theDphi = getDPHI( phi1, phi2);
  Double_t theDeta = eta1 - eta2;
  return TMath::Sqrt ( theDphi*theDphi + theDeta*theDeta);
}

Double_t getDPHI( Double_t phi1, Double_t phi2) {
  Double_t dphi = phi1 - phi2;
  
  if ( dphi > 3.141592653589 )
    dphi = dphi - 2. * 3.141592653589;
  if ( dphi <= -3.141592653589 )
    dphi = dphi + 2. * 3.141592653589;
  
  if ( TMath::Abs(dphi) > 3.141592653589 ) {
    cout << " commonUtility::getDPHI error!!! dphi is bigger than 3.141592653589 " << endl;
  }
  
  return TMath::Abs(dphi);
  //return dphi;
}

Double_t getDETA(Double_t eta1, Double_t eta2){
  return TMath::Abs(eta1 - eta2);
}
