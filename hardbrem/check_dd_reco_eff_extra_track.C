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

//void check_data_driven_reco_eff( std::string infile_Forest = "filelist_19.txt",
void check_dd_reco_eff_extra_track( std::string infile_Forest = "data2.txt",
      std::string out = "test.root") 
{
   bool ismc = 1; 
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
   treeHLT->SetBranchStatus("NL1IsolEm",1);     // enable photon branches
   treeHLT->SetBranchStatus("L1IsolEm*",1);     // enable photon branches

   treeHLT->SetBranchStatus("NL1NIsolEm",1);     // enable photon branches
   treeHLT->SetBranchStatus("L1NIsolEm*",1);     // enable photon branches

   Int_t           NL1IsolEm;
   Float_t         L1IsolEmEt[4];   //[NL1IsolEm]
   Float_t         L1IsolEmE[4];   //[NL1IsolEm]
   Float_t         L1IsolEmEta[4];   //[NL1IsolEm]
   Float_t         L1IsolEmPhi[4];   //[NL1IsolEm]
   Int_t           NL1NIsolEm;
   Float_t         L1NIsolEmEt[4];   //[NL1NIsolEm]
   Float_t         L1NIsolEmE[4];   //[NL1NIsolEm]
   Float_t         L1NIsolEmEta[4];   //[NL1NIsolEm]
   Float_t         L1NIsolEmPhi[4]; //[NL1NIsolEm]

   treeHLT->SetBranchAddress("NL1IsolEm",&NL1IsolEm);     // enable trigger branches
   treeHLT->SetBranchAddress("L1IsolEmEt",L1IsolEmEt);     // enable trigger branches
   treeHLT->SetBranchAddress("L1IsolEmE",L1IsolEmE);     // enable trigger branches
   treeHLT->SetBranchAddress("L1IsolEmEta",L1IsolEmEta);     // enable trigger branches
   treeHLT->SetBranchAddress("L1IsolEmPhi",L1IsolEmPhi);     // enable trigger branches

   treeHLT->SetBranchAddress("NL1NIsolEm",&NL1NIsolEm);     // enable trigger branches
   treeHLT->SetBranchAddress("L1NIsolEmEt",L1NIsolEmEt);     // enable trigger branches
   treeHLT->SetBranchAddress("L1NIsolEmE",L1NIsolEmE);     // enable trigger branches
   treeHLT->SetBranchAddress("L1NIsolEmEta",L1NIsolEmEta);     // enable trigger branches
   treeHLT->SetBranchAddress("L1NIsolEmPhi",L1NIsolEmPhi);     // enable trigger branches



   treeHLT->SetBranchStatus("HLT_HIUPCL1SingleEG5*",1);     // enable trigger branches
   treeHLT->SetBranchStatus("HLT_HIUPCL1DoubleEG2*",1);     // enable trigger branches

   Int_t HLT_HIUPCL1DoubleEG2NotHF2_v1 ;   Int_t HLT_HIUPCL1SingleEG5NotHF2_v1;
   TBranch *b_HLT_HIUPCL1DoubleEG2NotHF2_v1 ;   TBranch *b_HLT_HIUPCL1SingleEG5NotHF2_v1;

   treeHLT->SetBranchAddress("HLT_HIUPCL1SingleEG5NotHF2_v1",&HLT_HIUPCL1SingleEG5NotHF2_v1, &b_HLT_HIUPCL1DoubleEG2NotHF2_v1);     // enable trigger branches
   treeHLT->SetBranchAddress("HLT_HIUPCL1DoubleEG2NotHF2_v1",&HLT_HIUPCL1DoubleEG2NotHF2_v1, &b_HLT_HIUPCL1SingleEG5NotHF2_v1);     // enable trigger branches

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


   Int_t nMC;
   std::vector<float>* mcE=0;
   std::vector<float>* mcEt=0;
   std::vector<float>* mcPt=0;
   std::vector<float>* mcP=0;
   std::vector<float>* mcEta=0;
   std::vector<float>* mcPhi=0;

   tree->SetBranchStatus("nMC",1);     // enable photon branches
   tree->SetBranchStatus("mc*",1);     // enable photon branches
   tree->SetBranchAddress("nMC",&nMC);
   tree->SetBranchAddress("mcE",&mcE);
   tree->SetBranchAddress("mcEt",&mcEt);
   tree->SetBranchAddress("mcPt",&mcPt);
   tree->SetBranchAddress("mcP",&mcP);
   tree->SetBranchAddress("mcEta",&mcEta);
   tree->SetBranchAddress("mcPhi",&mcPhi);

   // GSF electron tracks
   // GED RECO electrons
   Int_t nEle;
   TBranch *b_nEle;
   std::vector<int>*   eleCharge=0;
   std::vector<float>* eleP=0;
   std::vector<float>* elePt=0;
   std::vector<float>* eleEta=0;
   std::vector<float>* elePhi=0;
   std::vector<float>* eleEn=0;


   tree->SetBranchStatus("nEle",1);     // enable electron branches
   tree->SetBranchStatus("ele*",1);     // enable electron branches
   tree->SetBranchAddress("nEle",&nEle, &b_nEle);
   tree->SetBranchAddress("eleCharge",&eleCharge);
   tree->SetBranchAddress("eleP",&eleP);
   tree->SetBranchAddress("elePt",&elePt);
   tree->SetBranchAddress("eleEta",&eleEta);
   tree->SetBranchAddress("elePhi",&elePhi);
   tree->SetBranchAddress("eleEn",&eleEn);

   // RECO photons
   Int_t nPho;
   TBranch *b_nPho;
   std::vector<float>* phoE=0;
   std::vector<float>* phoEt=0;
   std::vector<float>* phoEta=0;
   std::vector<float>* phoPhi=0;
   std::vector<float>* phoSCEt=0;
   std::vector<float>* phoSCEta=0;
   std::vector<float>* phoSCPhi=0;
   std::vector<float>* pho_ecalClusterIsoR4=0;
   std::vector<float>* pho_hcalRechitIsoR4=0;
   std::vector<float>* pho_trackIsoR4PtCut20=0;
   std::vector<float>* phoR9=0;
   std::vector<float>* phoHoverE=0;
   std::vector<float>* phoSigmaIEtaIEta=0;
   std::vector<float>* phoSigmaIEtaIEta_2012=0;
   std::vector<float>* phoSCRawE=0;
   std::vector<float>* phoE5x5=0;
   std::vector<float>* phoSCEtaWidth=0;
   std::vector<float>* phoSCPhiWidth=0;
   std::vector<float>* pho_swissCrx=0;
   std::vector<float>* pho_seedTime=0;
   std::vector<int>* phohasPixelSeed=0;
   std::vector<int>* phopassConversionVeto=0;


   tree->SetBranchStatus("nPho",1);     // enable photon branches
   tree->SetBranchAddress("nPho",&nPho, &b_nPho);
   tree->SetBranchStatus("pho*",1);     // enable photon branches
   tree->SetBranchAddress("phoE",&phoE);
   tree->SetBranchAddress("phoEt",&phoEt); 
   tree->SetBranchAddress("phoEta",&phoEta);
   tree->SetBranchAddress("phoPhi",&phoPhi);
   tree->SetBranchAddress("phoSCEt",&phoSCEt); 
   tree->SetBranchAddress("phoSCEta",&phoSCEta); 
   tree->SetBranchAddress("phoSCPhi",&phoSCPhi); 
   tree->SetBranchAddress("pho_ecalClusterIsoR4",&pho_ecalClusterIsoR4);
   tree->SetBranchAddress("pho_hcalRechitIsoR4",&pho_hcalRechitIsoR4);
   tree->SetBranchAddress("pho_trackIsoR4PtCut20",&pho_trackIsoR4PtCut20);
   tree->SetBranchAddress("phoR9",&phoR9);
   tree->SetBranchAddress("phoHoverE",&phoHoverE);
   tree->SetBranchAddress("phoSigmaIEtaIEta",&phoSigmaIEtaIEta);
   tree->SetBranchAddress("phoSigmaIEtaIEta_2012",&phoSigmaIEtaIEta_2012);
   tree->SetBranchAddress("phoSCRawE",&phoSCRawE);
   tree->SetBranchAddress("phoE5x5",&phoE5x5);
   tree->SetBranchAddress("phoSCEtaWidth",&phoSCEtaWidth);
   tree->SetBranchAddress("phoSCPhiWidth",&phoSCPhiWidth);
   tree->SetBranchAddress("pho_swissCrx",&pho_swissCrx);
   tree->SetBranchAddress("pho_seedTime",&pho_seedTime);
   tree->SetBranchAddress("phohasPixelSeed",&phohasPixelSeed);
   tree->SetBranchAddress("phopassConversionVeto",&phopassConversionVeto);



   // RECO muons
   Int_t nMu;

   tree->SetBranchStatus("nMu",1);     // enable muon branches
   tree->SetBranchAddress("nMu",&nMu);


   /// General track collection/////////
   Int_t ngenTrk;
   TBranch *b_ngenTrk;
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
   tree->SetBranchAddress("ngenTrk",&ngenTrk,&b_ngenTrk);
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



   TTree *outputTreediPho  = new TTree("diphoton","");


   outputTreeHLT->SetMaxTreeSize(MAXTREESIZE);
   outputTreediPho->SetMaxTreeSize(MAXTREESIZE);


   ////**************************** write output branches      *************************/

   outputTreediPho->Branch("run",&run);
   outputTreediPho->Branch("event",&event);
   outputTreediPho->Branch("lumis",&lumis);

   outputTreediPho->Branch("nVtx",&nVtx);
   outputTreediPho->Branch("xVtx",&xVtx);
   outputTreediPho->Branch("yVtx",&yVtx);
   outputTreediPho->Branch("zVtx",&zVtx);
   outputTreediPho->Branch("isFakeVtx",&isFakeVtx);

   std::vector<float> ele_matchedPt;
   std::vector<float> ele_matchedP;
   std::vector<float> ele_matchedEta;
   std::vector<float> ele_matchedPhi;
   std::vector<int>   ele_matchedCharge;
   std::vector<float> ele_matchedEn;
   std::vector<float> ele_matchedEt;


   outputTreediPho->Branch("nEle",&nEle);
   outputTreediPho->Branch("ele_matchedPt",&ele_matchedPt);
   outputTreediPho->Branch("ele_matchedP",&ele_matchedP);
   outputTreediPho->Branch("ele_matchedEta",&ele_matchedEta);
   outputTreediPho->Branch("ele_matchedPhi",&ele_matchedPhi);
   outputTreediPho->Branch("ele_matchedCharge",&ele_matchedCharge);
   outputTreediPho->Branch("ele_matchedEn",&ele_matchedEn);
   outputTreediPho->Branch("ele_matchedEt",&ele_matchedEt);

   std::vector<float> trk_matchedPt;
   std::vector<float> trk_matchedP;
   std::vector<float> trk_matchedEta;
   std::vector<float> trk_matchedPhi;
   std::vector<int>   trk_matchedCharge;

   outputTreediPho->Branch("trk_matchedPt",&trk_matchedPt);
   outputTreediPho->Branch("trk_matchedP",&trk_matchedP);
   outputTreediPho->Branch("trk_matchedEta",&trk_matchedEta);
   outputTreediPho->Branch("trk_matchedPhi",&trk_matchedPhi);
   outputTreediPho->Branch("trk_matchedCharge",&trk_matchedCharge);


   std::vector<float> trk_unmatchedPt;
   std::vector<float> trk_unmatchedP;
   std::vector<float> trk_unmatchedEta;
   std::vector<float> trk_unmatchedPhi;
   std::vector<int>   trk_unmatchedCharge;

   outputTreediPho->Branch("trk_unmatchedPt",&trk_unmatchedPt);
   outputTreediPho->Branch("trk_unmatchedP",&trk_unmatchedP);
   outputTreediPho->Branch("trk_unmatchedEta",&trk_unmatchedEta);
   outputTreediPho->Branch("trk_unmatchedPhi",&trk_unmatchedPhi);
   outputTreediPho->Branch("trk_unmatchedCharge",&trk_unmatchedCharge);




   // write tower info ////


   outputTreediPho->Branch("ngenTrk",&ngenTrk);


   std::vector<float> phoEt_1;
   std::vector<float> phoEta_1;
   std::vector<float> phoPhi_1;
   std::vector<float> phoSCEt_1;
   std::vector<float> phoSCEta_1;
   std::vector<float> phoSCPhi_1;
   std::vector<float> pho_ecalClusterIsoR4_1;
   std::vector<float> pho_hcalRechitIsoR4_1;
   std::vector<float> pho_trackIsoR4PtCut20_1;
   std::vector<float> phoR9_1;
   std::vector<float> phoHoverE_1;
   std::vector<float> phoSigmaIEtaIEta_1;
   std::vector<float> phoSigmaIEtaIEta_2012_1;
   std::vector<float> phoE5x5_1;
   std::vector<float> phoSCEtaWidth_1;
   std::vector<float> phoSCPhiWidth_1;
   std::vector<float> pho_swissCrx_1;
   std::vector<float> pho_seedTime_1;
   std::vector<int>   phohasPixelSeed_1;
   std::vector<int>   phopassConversionVeto_1;

   outputTreediPho->Branch("phoEt_1",&phoEt_1);
   outputTreediPho->Branch("phoEta_1",&phoEta_1);
   outputTreediPho->Branch("phoPhi_1",&phoPhi_1);
   outputTreediPho->Branch("phoSCEt_1",&phoSCEt_1);
   outputTreediPho->Branch("phoSCEta_1",&phoSCEta_1);
   outputTreediPho->Branch("phoSCPhi_1",&phoSCPhi_1);
   outputTreediPho->Branch("pho_ecalClusterIsoR4_1",&pho_ecalClusterIsoR4_1);
   outputTreediPho->Branch("pho_hcalRechitIsoR4_1",&pho_hcalRechitIsoR4_1);
   outputTreediPho->Branch("pho_trackIsoR4PtCut20_1",&pho_trackIsoR4PtCut20_1);
   outputTreediPho->Branch("phoR9_1",&phoR9_1);
   outputTreediPho->Branch("phoHoverE_1",&phoHoverE_1);
   outputTreediPho->Branch("phoSigmaIEtaIEta_1",&phoSigmaIEtaIEta_1);
   outputTreediPho->Branch("phoSigmaIEtaIEta_2012_1",&phoSigmaIEtaIEta_2012_1);
   outputTreediPho->Branch("phoE5x5_1",&phoE5x5_1);
   outputTreediPho->Branch("phoSCEtaWidth_1",&phoSCEtaWidth_1);
   outputTreediPho->Branch("phoSCPhiWidth_1",&phoSCPhiWidth_1);
   outputTreediPho->Branch("pho_swissCrx_1",&pho_swissCrx_1);
   outputTreediPho->Branch("pho_seedTime_1",&pho_seedTime_1);
   outputTreediPho->Branch("phohasPixelSeed_1",&phohasPixelSeed_1);
   outputTreediPho->Branch("phopassConversionVeto_1",&phopassConversionVeto_1);


   std::vector<float> phoEt_2;
   std::vector<float> phoEta_2;
   std::vector<float> phoPhi_2;
   std::vector<float> phoSCEt_2;
   std::vector<float> phoSCEta_2;
   std::vector<float> phoSCPhi_2;
   std::vector<float> phoR9_2;
   std::vector<float> phoHoverE_2;
   std::vector<float> phoSigmaIEtaIEta_2;
   std::vector<float> phoSigmaIEtaIEta_2012_2;
   std::vector<float> phoE5x5_2;
   std::vector<float> pho_swissCrx_2;
   std::vector<float> pho_seedTime_2;
   std::vector<int>   phohasPixelSeed_2;
   std::vector<int>   phopassConversionVeto_2;

   outputTreediPho->Branch("phoEt_2",&phoEt_2);
   outputTreediPho->Branch("phoEta_2",&phoEta_2);
   outputTreediPho->Branch("phoPhi_2",&phoPhi_2);
   outputTreediPho->Branch("phoSCEt_2",&phoSCEt_2);
   outputTreediPho->Branch("phoSCEta_2",&phoSCEta_2);
   outputTreediPho->Branch("phoSCPhi_2",&phoSCPhi_2);
   outputTreediPho->Branch("phoR9_2",&phoR9_2);
   outputTreediPho->Branch("phoHoverE_2",&phoHoverE_2);
   outputTreediPho->Branch("phoSigmaIEtaIEta_2",&phoSigmaIEtaIEta_2);
   outputTreediPho->Branch("phoSigmaIEtaIEta_2012_2",&phoSigmaIEtaIEta_2012_2);
   outputTreediPho->Branch("phoE5x5_2",&phoE5x5_2);
   outputTreediPho->Branch("pho_swissCrx_2",&pho_swissCrx_2);
   outputTreediPho->Branch("pho_seedTime_2",&pho_seedTime_2);
   outputTreediPho->Branch("phohasPixelSeed_2",&phohasPixelSeed_2);
   outputTreediPho->Branch("phopassConversionVeto_2",&phopassConversionVeto_2);


   std::vector<float> dr_1;
   std::vector<float> dr_2;

   outputTreediPho->Branch("dr_1",&dr_1);
   outputTreediPho->Branch("dr_2",&dr_2);

   std::vector<float> photon_unmatched_track_deta_1;
   std::vector<float> photon_unmatched_track_dphi_1;
   outputTreediPho->Branch("photon_unmatched_track_deta_1",&photon_unmatched_track_deta_1);
   outputTreediPho->Branch("photon_unmatched_track_dphi_1",&photon_unmatched_track_dphi_1);
   std::vector<float> photon_matched_track_deta_1;
   std::vector<float> photon_matched_track_dphi_1;
   outputTreediPho->Branch("photon_matched_track_deta_1",&photon_matched_track_deta_1);
   outputTreediPho->Branch("photon_matched_track_dphi_1",&photon_matched_track_dphi_1);



   std::vector<float> photon_unmatched_track_deta_2;
   std::vector<float> photon_unmatched_track_dphi_2;
   outputTreediPho->Branch("photon_unmatched_track_deta_2",&photon_unmatched_track_deta_2);
   outputTreediPho->Branch("photon_unmatched_track_dphi_2",&photon_unmatched_track_dphi_2);
   std::vector<float> photon_matched_track_deta_2;
   std::vector<float> photon_matched_track_dphi_2;
   outputTreediPho->Branch("photon_matched_track_deta_2",&photon_matched_track_deta_2);
   outputTreediPho->Branch("photon_matched_track_dphi_2",&photon_matched_track_dphi_2);


   std::vector<float> gen_trk_aco;
   outputTreediPho->Branch("gen_trk_aco",&gen_trk_aco);

   std::vector<float> gen_trk_dphi;
   outputTreediPho->Branch("gen_trk_dphi",&gen_trk_dphi);

   std::vector<int>   npho_inside_cone;
   outputTreediPho->Branch("npho_inside_cone",&npho_inside_cone);
   std::vector<int>   nextra_pho;
   outputTreediPho->Branch("nextra_pho",&nextra_pho);


   Long64_t entries = tree->GetEntries();
   Long64_t entriesAnalyzed = 0;   Long64_t id = 0;   Long64_t excl = 0; Long64_t ptcut = 0; Long64_t myloop = 0;
   std::cout << "entries         = " << entries << std::endl;
   std::cout<< "Loop : ggHiNtuplizer/EventTree" <<std::endl;

   for (Long64_t j_entry=0; j_entry<entries; ++j_entry){
      //for (Long64_t j_entry=5019; j_entry< 5020; ++j_entry){
      //for (Long64_t j_entry=12893; j_entry< 12894; ++j_entry){
      // for (Long64_t j_entry= 43923; j_entry< 43924; ++j_entry){
      //for (Long64_t j_entry= 0; j_entry< 200; ++j_entry){

      if (j_entry % 10000 == 0)  {
         std::cout << "current entry = " <<j_entry<<" out of "<<entries<<" : "<<std::setprecision(2)<<(double)j_entry/entries*100<<" %"<<std::endl;
      }

      b_HLT_HIUPCL1SingleEG5NotHF2_v1->GetEntry(j_entry);
      if(HLT_HIUPCL1SingleEG5NotHF2_v1!=1) continue;

      b_ngenTrk->GetEntry(j_entry);
      b_nEle->GetEntry(j_entry);
      b_nPho->GetEntry(j_entry);
      if(!(ngenTrk >=2 && ngenTrk <=4 && nEle >=1 && nEle <=3 && nPho>=0 && nPho <= 2) ) continue;

      entriesAnalyzed++; 

      treeHLT->GetEntry(j_entry);
      tree->GetEntry(j_entry);
      treePixel->GetEntry(j_entry);

      outputTreeHLT->Fill(); 

      // dielectron block

      ele_matchedPt.clear();
      ele_matchedP.clear();
      ele_matchedEta.clear();
      ele_matchedPhi.clear();
      ele_matchedCharge.clear();
      ele_matchedEn.clear();
      ele_matchedEt.clear();

      trk_matchedPt.clear();
      trk_matchedP.clear();
      trk_matchedEta.clear();
      trk_matchedPhi.clear();
      trk_matchedCharge.clear();


      trk_unmatchedPt.clear();
      trk_unmatchedP.clear();
      trk_unmatchedEta.clear();
      trk_unmatchedPhi.clear();
      trk_unmatchedCharge.clear();

      phoEt_1.clear();
      phoEta_1.clear();
      phoPhi_1.clear();
      phoSCEt_1.clear();     
      phoSCEta_1.clear();     
      phoSCPhi_1.clear();     
      phoR9_1.clear();
      phoHoverE_1.clear();
      phoSigmaIEtaIEta_1.clear();
      phoSigmaIEtaIEta_2012_1.clear();
      phoE5x5_1.clear();
      pho_swissCrx_1.clear();
      pho_seedTime_1.clear();
      phohasPixelSeed_1.clear();
      phopassConversionVeto_1.clear();

      phoEt_2.clear();
      phoEta_2.clear();
      phoPhi_2.clear();
      phoSCEt_2.clear();     
      phoSCEta_2.clear();     
      phoSCPhi_2.clear();     
      phoR9_2.clear();
      phoHoverE_2.clear();
      phoSigmaIEtaIEta_2.clear();
      phoSigmaIEtaIEta_2012_2.clear();
      phoE5x5_2.clear();
      pho_swissCrx_2.clear();
      pho_seedTime_2.clear();
      phohasPixelSeed_2.clear();
      phopassConversionVeto_2.clear();


      dr_1.clear();       
      dr_2.clear();       

      photon_unmatched_track_deta_1.clear();     
      photon_unmatched_track_dphi_1.clear();   
      photon_matched_track_deta_1.clear();     
      photon_matched_track_dphi_1.clear();  

      photon_unmatched_track_deta_2.clear();     
      photon_unmatched_track_dphi_2.clear();   
      photon_matched_track_deta_2.clear();     
      photon_matched_track_dphi_2.clear();  

      gen_trk_aco.clear();
      gen_trk_dphi.clear();

      npho_inside_cone.clear();
      nextra_pho.clear();

      //if(ngenTrk >=2 && ngenTrk <=4 && nEle ==2 && nPho>=0 && nPho <= 2 ){

      //if(nEle ==1 || nEle ==2)continue;

      // cout << ngenTrk << " " << nEle << " " << nPho << endl;

      double minDR[nEle], EtMinDR[nEle];

      for(int i=0; i< nEle; ++i){

         minDR[i] = {999.};
         EtMinDR[i]={0};

         for (int ii=0; ii<NL1IsolEm; ii++) {
            if (L1IsolEmEt[ii]<=4) continue;

            double dr0 =  getDR(L1IsolEmEta[ii],L1IsolEmPhi[ii],eleEta->at(i),elePhi->at(i)); 

            // cout << "Ele:" << i << "  DR0 1st loop: " << dr0 << "  elePt:" << elePt->at(i) << "   genTrkpT:" << gentrkPt->at(i); 
            // cout << "  PhoEt:" << phoSCEt->at(i) << "  isoEmet: " << L1IsolEmEt[ii] << endl;     
            if (dr0<minDR[i]) {
               minDR[i] = dr0;
               EtMinDR[i] = L1IsolEmEt[ii];
            }
         }


         for (int ii=0; ii<NL1NIsolEm; ii++) {
            if (L1NIsolEmEt[ii]<=4) continue;

            double dr0 =  getDR(L1NIsolEmEta[ii],L1NIsolEmPhi[ii],eleEta->at(i),elePhi->at(i)); 
            // cout << "Ele:" << i << "  DR0 2nd loop: " << dr0 << "  elePt:" << elePt->at(i) << "   genTrkpT:" << gentrkPt->at(i);
            // cout << "  PhoEt:" << phoSCEt->at(i) << "  isoEmEt: " << L1NIsolEmEt[ii] << endl;     
            if (dr0<minDR[i]) {
               minDR[i] = dr0;
               EtMinDR[i] = L1NIsolEmEt[ii];
            }
         }

      }// nelectron

      int nL1matched=0;
      int imatched=-1;
      for (int i=0; i<nEle; i++) 
         if(minDR[i]<DRmax) {
            imatched=i;
            nL1matched++;
         }

      // require a single electron matched to l1EG5 per event
      if (nL1matched!=1) continue;

      // the matched electron has to be pt>5
      if (elePt->at(imatched)<5) continue;

      if(nEle==2){
         cout << "entry:" << j_entry << "  coming in loop"<< "  gentrk:" << ngenTrk << "  electron: " << nEle;
         cout << "   photon:" << nPho << endl;

         cout << " 1st Min dr " << minDR[0] << " 2nd min Dr: " << minDR[1] <<endl;	
      }

      int ele_photon_umatched = 0;
      int ele_photon_matched = 0;

      for(int j=0; j< nPho; ++j){
         double photon_ele_match_deta = getDETA(eleEta->at(imatched),phoSCEta->at(j));
         double photon_ele_match_dphi = getDPHI(elePhi->at(imatched),phoSCPhi->at(j));


         if(photon_ele_match_deta < 0.15 && photon_ele_match_dphi < 0.7){
            ele_photon_matched ++ ;
            // cout << "Entry: "<< j_entry << " photon associated to ele" << endl;
         } else {
            ele_photon_umatched++;
            // cout << "Entry: "<< j_entry << " photon not associated to ele" << endl;
         }

      }//nPho

      npho_inside_cone.push_back(ele_photon_matched);
      nextra_pho.push_back(ele_photon_umatched);



      cout << "ok event" << endl;
      outputTreediPho->Fill();

   } //event loop
   std::cout<<  "Loop ENDED : ggHiNtuplizer/EventTree" <<std::endl;
   std::cout << "entries            = " << entries << std::endl;
   std::cout << "In my loop         = " << myloop << std::endl;
   std::cout << "In three body loop = " << id << std::endl;

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
      check_dd_reco_eff_extra_track(argv[1]);
      //mycheck_ele_tree(argv[3],argv[4]);
      return 0;
   }

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

