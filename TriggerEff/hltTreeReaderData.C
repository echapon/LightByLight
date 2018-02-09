#define hltTreeReaderData_cxx
#include "hltTreeReaderData.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void hltTreeReaderData::Loop()
{
//   In a ROOT session, you can do:
//      root> .L hltTreeReaderData.C
//      root> hltTreeReaderData t
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
   fChain->SetBranchStatus("*",0);  // disable all branches
   fChain->SetBranchStatus("HLT_*",1);  // activate branchname
   fChain->SetBranchStatus("*_Prescl",0);  
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      if (!HLT_HIUPCL1SingleEG5NotHF2_v1) continue;

      cout << jentry;
      if (           HLT_HIPuAK4CaloJet40_Eta5p1_v1) cout << " " << "HLT_HIPuAK4CaloJet40_Eta5p1_v1";
      if (           HLT_HIPuAK4CaloJet60_Eta5p1_v1) cout << " " << "HLT_HIPuAK4CaloJet60_Eta5p1_v1";
      if (           HLT_HIPuAK4CaloJet80_Eta5p1_v1) cout << " " << "HLT_HIPuAK4CaloJet80_Eta5p1_v1";
      if (           HLT_HIPuAK4CaloJet80_Eta5p1ForZS_v1) cout << " " << "HLT_HIPuAK4CaloJet80_Eta5p1ForZS_v1";
      if (           HLT_HIPuAK4CaloJet100_Eta5p1_v1) cout << " " << "HLT_HIPuAK4CaloJet100_Eta5p1_v1";
      if (           HLT_HIPuAK4CaloJet110_Eta5p1_v1) cout << " " << "HLT_HIPuAK4CaloJet110_Eta5p1_v1";
      if (           HLT_HIPuAK4CaloJet120_Eta5p1_v1) cout << " " << "HLT_HIPuAK4CaloJet120_Eta5p1_v1";
      if (           HLT_HIPuAK4CaloJet150_Eta5p1_v1) cout << " " << "HLT_HIPuAK4CaloJet150_Eta5p1_v1";
      if (           HLT_HIPuAK4CaloJet40_Eta5p1_Cent30_100_v1) cout << " " << "HLT_HIPuAK4CaloJet40_Eta5p1_Cent30_100_v1";
      if (           HLT_HIPuAK4CaloJet60_Eta5p1_Cent30_100_v1) cout << " " << "HLT_HIPuAK4CaloJet60_Eta5p1_Cent30_100_v1";
      if (           HLT_HIPuAK4CaloJet80_Eta5p1_Cent30_100_v1) cout << " " << "HLT_HIPuAK4CaloJet80_Eta5p1_Cent30_100_v1";
      if (           HLT_HIPuAK4CaloJet100_Eta5p1_Cent30_100_v1) cout << " " << "HLT_HIPuAK4CaloJet100_Eta5p1_Cent30_100_v1";
      if (           HLT_HIPuAK4CaloJet40_Eta5p1_Cent50_100_v1) cout << " " << "HLT_HIPuAK4CaloJet40_Eta5p1_Cent50_100_v1";
      if (           HLT_HIPuAK4CaloJet60_Eta5p1_Cent50_100_v1) cout << " " << "HLT_HIPuAK4CaloJet60_Eta5p1_Cent50_100_v1";
      if (           HLT_HIPuAK4CaloJet80_Eta5p1_Cent50_100_v1) cout << " " << "HLT_HIPuAK4CaloJet80_Eta5p1_Cent50_100_v1";
      if (           HLT_HIPuAK4CaloJet100_Eta5p1_Cent50_100_v1) cout << " " << "HLT_HIPuAK4CaloJet100_Eta5p1_Cent50_100_v1";
      if (           HLT_HIPuAK4CaloJet80_Jet35_Eta1p1_v1) cout << " " << "HLT_HIPuAK4CaloJet80_Jet35_Eta1p1_v1";
      if (           HLT_HIPuAK4CaloJet80_Jet35_Eta0p7_v1) cout << " " << "HLT_HIPuAK4CaloJet80_Jet35_Eta0p7_v1";
      if (           HLT_HIPuAK4CaloJet100_Jet35_Eta1p1_v1) cout << " " << "HLT_HIPuAK4CaloJet100_Jet35_Eta1p1_v1";
      if (           HLT_HIPuAK4CaloJet100_Jet35_Eta0p7_v1) cout << " " << "HLT_HIPuAK4CaloJet100_Jet35_Eta0p7_v1";
      if (           HLT_HIPuAK4CaloJet80_45_45_Eta2p1_v1) cout << " " << "HLT_HIPuAK4CaloJet80_45_45_Eta2p1_v1";
      if (           HLT_HIPuAK4CaloDJet60_Eta2p1_v1) cout << " " << "HLT_HIPuAK4CaloDJet60_Eta2p1_v1";
      if (           HLT_HIPuAK4CaloDJet80_Eta2p1_v1) cout << " " << "HLT_HIPuAK4CaloDJet80_Eta2p1_v1";
      if (           HLT_HIPuAK4CaloBJetCSV60_Eta2p1_v1) cout << " " << "HLT_HIPuAK4CaloBJetCSV60_Eta2p1_v1";
      if (           HLT_HIPuAK4CaloBJetCSV80_Eta2p1_v1) cout << " " << "HLT_HIPuAK4CaloBJetCSV80_Eta2p1_v1";
      if (           HLT_HIPuAK4CaloBJetSSV60_Eta2p1_v1) cout << " " << "HLT_HIPuAK4CaloBJetSSV60_Eta2p1_v1";
      if (           HLT_HIPuAK4CaloBJetSSV80_Eta2p1_v1) cout << " " << "HLT_HIPuAK4CaloBJetSSV80_Eta2p1_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt20_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt20_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt20_Cent0_10_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt20_Cent0_10_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt20_Cent30_100_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt20_Cent30_100_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt20_Cent50_100_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt20_Cent50_100_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt30_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt30_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt30_Cent0_10_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt30_Cent0_10_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt30_Cent30_100_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt30_Cent30_100_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt30_Cent50_100_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt30_Cent50_100_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt40_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt40_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt40_Cent0_10_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt40_Cent0_10_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt40_Cent30_100_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt40_Cent30_100_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt40_Cent50_100_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt40_Cent50_100_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt50_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt50_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt60_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt60_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt70_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt70_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt60_Cent30_100_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt60_Cent30_100_v1";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt60_Cent50_100_v1) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt60_Cent50_100_v1";
      if (           HLT_HISinglePhoton10_Eta1p5_v1) cout << " " << "HLT_HISinglePhoton10_Eta1p5_v1";
      if (           HLT_HISinglePhoton15_Eta1p5_v1) cout << " " << "HLT_HISinglePhoton15_Eta1p5_v1";
      if (           HLT_HISinglePhoton20_Eta1p5_v1) cout << " " << "HLT_HISinglePhoton20_Eta1p5_v1";
      if (           HLT_HISinglePhoton30_Eta1p5_v1) cout << " " << "HLT_HISinglePhoton30_Eta1p5_v1";
      if (           HLT_HISinglePhoton40_Eta1p5_v1) cout << " " << "HLT_HISinglePhoton40_Eta1p5_v1";
      if (           HLT_HISinglePhoton50_Eta1p5_v1) cout << " " << "HLT_HISinglePhoton50_Eta1p5_v1";
      if (           HLT_HISinglePhoton60_Eta1p5_v1) cout << " " << "HLT_HISinglePhoton60_Eta1p5_v1";
      if (           HLT_HISinglePhoton10_Eta1p5_Cent50_100_v1) cout << " " << "HLT_HISinglePhoton10_Eta1p5_Cent50_100_v1";
      if (           HLT_HISinglePhoton15_Eta1p5_Cent50_100_v1) cout << " " << "HLT_HISinglePhoton15_Eta1p5_Cent50_100_v1";
      if (           HLT_HISinglePhoton20_Eta1p5_Cent50_100_v1) cout << " " << "HLT_HISinglePhoton20_Eta1p5_Cent50_100_v1";
      if (           HLT_HISinglePhoton30_Eta1p5_Cent50_100_v1) cout << " " << "HLT_HISinglePhoton30_Eta1p5_Cent50_100_v1";
      if (           HLT_HISinglePhoton40_Eta1p5_Cent50_100_v1) cout << " " << "HLT_HISinglePhoton40_Eta1p5_Cent50_100_v1";
      if (           HLT_HISinglePhoton10_Eta1p5_Cent30_100_v1) cout << " " << "HLT_HISinglePhoton10_Eta1p5_Cent30_100_v1";
      if (           HLT_HISinglePhoton15_Eta1p5_Cent30_100_v1) cout << " " << "HLT_HISinglePhoton15_Eta1p5_Cent30_100_v1";
      if (           HLT_HISinglePhoton20_Eta1p5_Cent30_100_v1) cout << " " << "HLT_HISinglePhoton20_Eta1p5_Cent30_100_v1";
      if (           HLT_HISinglePhoton30_Eta1p5_Cent30_100_v1) cout << " " << "HLT_HISinglePhoton30_Eta1p5_Cent30_100_v1";
      if (           HLT_HISinglePhoton40_Eta1p5_Cent30_100_v1) cout << " " << "HLT_HISinglePhoton40_Eta1p5_Cent30_100_v1";
      if (           HLT_HISinglePhoton40_Eta2p1_v1) cout << " " << "HLT_HISinglePhoton40_Eta2p1_v1";
      if (           HLT_HISinglePhoton10_Eta3p1_v1) cout << " " << "HLT_HISinglePhoton10_Eta3p1_v1";
      if (           HLT_HISinglePhoton15_Eta3p1_v1) cout << " " << "HLT_HISinglePhoton15_Eta3p1_v1";
      if (           HLT_HISinglePhoton20_Eta3p1_v1) cout << " " << "HLT_HISinglePhoton20_Eta3p1_v1";
      if (           HLT_HISinglePhoton30_Eta3p1_v1) cout << " " << "HLT_HISinglePhoton30_Eta3p1_v1";
      if (           HLT_HISinglePhoton40_Eta3p1_v1) cout << " " << "HLT_HISinglePhoton40_Eta3p1_v1";
      if (           HLT_HISinglePhoton50_Eta3p1_v1) cout << " " << "HLT_HISinglePhoton50_Eta3p1_v1";
      if (           HLT_HISinglePhoton60_Eta3p1_v1) cout << " " << "HLT_HISinglePhoton60_Eta3p1_v1";
      if (           HLT_HISinglePhoton10_Eta3p1_Cent50_100_v1) cout << " " << "HLT_HISinglePhoton10_Eta3p1_Cent50_100_v1";
      if (           HLT_HISinglePhoton15_Eta3p1_Cent50_100_v1) cout << " " << "HLT_HISinglePhoton15_Eta3p1_Cent50_100_v1";
      if (           HLT_HISinglePhoton20_Eta3p1_Cent50_100_v1) cout << " " << "HLT_HISinglePhoton20_Eta3p1_Cent50_100_v1";
      if (           HLT_HISinglePhoton30_Eta3p1_Cent50_100_v1) cout << " " << "HLT_HISinglePhoton30_Eta3p1_Cent50_100_v1";
      if (           HLT_HISinglePhoton40_Eta3p1_Cent50_100_v1) cout << " " << "HLT_HISinglePhoton40_Eta3p1_Cent50_100_v1";
      if (           HLT_HISinglePhoton10_Eta3p1_Cent30_100_v1) cout << " " << "HLT_HISinglePhoton10_Eta3p1_Cent30_100_v1";
      if (           HLT_HISinglePhoton15_Eta3p1_Cent30_100_v1) cout << " " << "HLT_HISinglePhoton15_Eta3p1_Cent30_100_v1";
      if (           HLT_HISinglePhoton20_Eta3p1_Cent30_100_v1) cout << " " << "HLT_HISinglePhoton20_Eta3p1_Cent30_100_v1";
      if (           HLT_HISinglePhoton30_Eta3p1_Cent30_100_v1) cout << " " << "HLT_HISinglePhoton30_Eta3p1_Cent30_100_v1";
      if (           HLT_HISinglePhoton40_Eta3p1_Cent30_100_v1) cout << " " << "HLT_HISinglePhoton40_Eta3p1_Cent30_100_v1";
      if (           HLT_HIDoublePhoton15_Eta1p5_Mass50_1000_v1) cout << " " << "HLT_HIDoublePhoton15_Eta1p5_Mass50_1000_v1";
      if (           HLT_HIDoublePhoton15_Eta1p5_Mass50_1000_R9HECut_v1) cout << " " << "HLT_HIDoublePhoton15_Eta1p5_Mass50_1000_R9HECut_v1";
      if (           HLT_HIDoublePhoton15_Eta2p1_Mass50_1000_R9Cut_v1) cout << " " << "HLT_HIDoublePhoton15_Eta2p1_Mass50_1000_R9Cut_v1";
      if (           HLT_HIDoublePhoton15_Eta2p5_Mass50_1000_R9SigmaHECut_v1) cout << " " << "HLT_HIDoublePhoton15_Eta2p5_Mass50_1000_R9SigmaHECut_v1";
      if (           HLT_HIL2Mu3Eta2p5_PuAK4CaloJet40Eta2p1_v1) cout << " " << "HLT_HIL2Mu3Eta2p5_PuAK4CaloJet40Eta2p1_v1";
      if (           HLT_HIL2Mu3Eta2p5_PuAK4CaloJet60Eta2p1_v1) cout << " " << "HLT_HIL2Mu3Eta2p5_PuAK4CaloJet60Eta2p1_v1";
      if (           HLT_HIL2Mu3Eta2p5_PuAK4CaloJet80Eta2p1_v1) cout << " " << "HLT_HIL2Mu3Eta2p5_PuAK4CaloJet80Eta2p1_v1";
      if (           HLT_HIL2Mu3Eta2p5_PuAK4CaloJet100Eta2p1_v1) cout << " " << "HLT_HIL2Mu3Eta2p5_PuAK4CaloJet100Eta2p1_v1";
      if (           HLT_HIL2Mu3Eta2p5_HIPhoton10Eta1p5_v1) cout << " " << "HLT_HIL2Mu3Eta2p5_HIPhoton10Eta1p5_v1";
      if (           HLT_HIL2Mu3Eta2p5_HIPhoton15Eta1p5_v1) cout << " " << "HLT_HIL2Mu3Eta2p5_HIPhoton15Eta1p5_v1";
      if (           HLT_HIL2Mu3Eta2p5_HIPhoton20Eta1p5_v1) cout << " " << "HLT_HIL2Mu3Eta2p5_HIPhoton20Eta1p5_v1";
      if (           HLT_HIL2Mu3Eta2p5_HIPhoton30Eta1p5_v1) cout << " " << "HLT_HIL2Mu3Eta2p5_HIPhoton30Eta1p5_v1";
      if (           HLT_HIL2Mu3Eta2p5_HIPhoton40Eta1p5_v1) cout << " " << "HLT_HIL2Mu3Eta2p5_HIPhoton40Eta1p5_v1";
      if (           HLT_HIUCC100_v1) cout << " " << "HLT_HIUCC100_v1";
      if (           HLT_HIUCC020_v1) cout << " " << "HLT_HIUCC020_v1";
      if (           HLT_HIL1DoubleMu0_v1) cout << " " << "HLT_HIL1DoubleMu0_v1";
      if (           HLT_HIL1DoubleMu0_2HF_v1) cout << " " << "HLT_HIL1DoubleMu0_2HF_v1";
      if (           HLT_HIL1DoubleMu0_2HF0_v1) cout << " " << "HLT_HIL1DoubleMu0_2HF0_v1";
      if (           HLT_HIL1DoubleMu10_v1) cout << " " << "HLT_HIL1DoubleMu10_v1";
      if (           HLT_HIL2DoubleMu0_NHitQ_v2) cout << " " << "HLT_HIL2DoubleMu0_NHitQ_v2";
      if (           HLT_HIL2DoubleMu0_NHitQ_2HF_v1) cout << " " << "HLT_HIL2DoubleMu0_NHitQ_2HF_v1";
      if (           HLT_HIL2DoubleMu0_NHitQ_2HF0_v1) cout << " " << "HLT_HIL2DoubleMu0_NHitQ_2HF0_v1";
      if (           HLT_HIL2Mu3_NHitQ10_2HF_v1) cout << " " << "HLT_HIL2Mu3_NHitQ10_2HF_v1";
      if (           HLT_HIL2Mu3_NHitQ10_2HF0_v1) cout << " " << "HLT_HIL2Mu3_NHitQ10_2HF0_v1";
      if (           HLT_HIL3Mu3_NHitQ15_2HF_v1) cout << " " << "HLT_HIL3Mu3_NHitQ15_2HF_v1";
      if (           HLT_HIL3Mu3_NHitQ15_2HF0_v1) cout << " " << "HLT_HIL3Mu3_NHitQ15_2HF0_v1";
      if (           HLT_HIL2Mu5_NHitQ10_2HF_v1) cout << " " << "HLT_HIL2Mu5_NHitQ10_2HF_v1";
      if (           HLT_HIL2Mu5_NHitQ10_2HF0_v1) cout << " " << "HLT_HIL2Mu5_NHitQ10_2HF0_v1";
      if (           HLT_HIL3Mu5_NHitQ15_2HF_v1) cout << " " << "HLT_HIL3Mu5_NHitQ15_2HF_v1";
      if (           HLT_HIL3Mu5_NHitQ15_2HF0_v1) cout << " " << "HLT_HIL3Mu5_NHitQ15_2HF0_v1";
      if (           HLT_HIL2Mu7_NHitQ10_2HF_v1) cout << " " << "HLT_HIL2Mu7_NHitQ10_2HF_v1";
      if (           HLT_HIL2Mu7_NHitQ10_2HF0_v1) cout << " " << "HLT_HIL2Mu7_NHitQ10_2HF0_v1";
      if (           HLT_HIL3Mu7_NHitQ15_2HF_v1) cout << " " << "HLT_HIL3Mu7_NHitQ15_2HF_v1";
      if (           HLT_HIL3Mu7_NHitQ15_2HF0_v1) cout << " " << "HLT_HIL3Mu7_NHitQ15_2HF0_v1";
      if (           HLT_HIL2Mu15_v2) cout << " " << "HLT_HIL2Mu15_v2";
      if (           HLT_HIL2Mu15_2HF_v1) cout << " " << "HLT_HIL2Mu15_2HF_v1";
      if (           HLT_HIL2Mu15_2HF0_v1) cout << " " << "HLT_HIL2Mu15_2HF0_v1";
      if (           HLT_HIL3Mu15_v1) cout << " " << "HLT_HIL3Mu15_v1";
      if (           HLT_HIL3Mu15_2HF_v1) cout << " " << "HLT_HIL3Mu15_2HF_v1";
      if (           HLT_HIL3Mu15_2HF0_v1) cout << " " << "HLT_HIL3Mu15_2HF0_v1";
      if (           HLT_HIL2Mu20_v1) cout << " " << "HLT_HIL2Mu20_v1";
      if (           HLT_HIL2Mu20_2HF_v1) cout << " " << "HLT_HIL2Mu20_2HF_v1";
      if (           HLT_HIL2Mu20_2HF0_v1) cout << " " << "HLT_HIL2Mu20_2HF0_v1";
      if (           HLT_HIL3Mu20_v1) cout << " " << "HLT_HIL3Mu20_v1";
      if (           HLT_HIL3Mu20_2HF_v1) cout << " " << "HLT_HIL3Mu20_2HF_v1";
      if (           HLT_HIL3Mu20_2HF0_v1) cout << " " << "HLT_HIL3Mu20_2HF0_v1";
      if (           HLT_HIL1DoubleMu0_2HF_Cent30100_v1) cout << " " << "HLT_HIL1DoubleMu0_2HF_Cent30100_v1";
      if (           HLT_HIL1DoubleMu0_2HF0_Cent30100_v1) cout << " " << "HLT_HIL1DoubleMu0_2HF0_Cent30100_v1";
      if (           HLT_HIL2DoubleMu0_2HF_Cent30100_NHitQ_v1) cout << " " << "HLT_HIL2DoubleMu0_2HF_Cent30100_NHitQ_v1";
      if (           HLT_HIL1DoubleMu0_Cent30_v1) cout << " " << "HLT_HIL1DoubleMu0_Cent30_v1";
      if (           HLT_HIL2DoubleMu0_2HF0_Cent30100_NHitQ_v1) cout << " " << "HLT_HIL2DoubleMu0_2HF0_Cent30100_NHitQ_v1";
      if (           HLT_HIL2DoubleMu0_Cent30_OS_NHitQ_v1) cout << " " << "HLT_HIL2DoubleMu0_Cent30_OS_NHitQ_v1";
      if (           HLT_HIL2DoubleMu0_Cent30_NHitQ_v1) cout << " " << "HLT_HIL2DoubleMu0_Cent30_NHitQ_v1";
      if (           HLT_HIL3DoubleMu0_Cent30_v1) cout << " " << "HLT_HIL3DoubleMu0_Cent30_v1";
      if (           HLT_HIL3DoubleMu0_Cent30_OS_m2p5to4p5_v1) cout << " " << "HLT_HIL3DoubleMu0_Cent30_OS_m2p5to4p5_v1";
      if (           HLT_HIL3DoubleMu0_Cent30_OS_m7to14_v1) cout << " " << "HLT_HIL3DoubleMu0_Cent30_OS_m7to14_v1";
      if (           HLT_HIL3DoubleMu0_OS_m2p5to4p5_v1) cout << " " << "HLT_HIL3DoubleMu0_OS_m2p5to4p5_v1";
      if (           HLT_HIL3DoubleMu0_OS_m7to14_v1) cout << " " << "HLT_HIL3DoubleMu0_OS_m7to14_v1";
      if (           HLT_HIUPCL1SingleMuOpenNotHF2_v1) cout << " " << "HLT_HIUPCL1SingleMuOpenNotHF2_v1";
      if (           HLT_HIUPCSingleMuNotHF2Pixel_SingleTrack_v1) cout << " " << "HLT_HIUPCSingleMuNotHF2Pixel_SingleTrack_v1";
      if (           HLT_HIUPCL1DoubleMuOpenNotHF2_v1) cout << " " << "HLT_HIUPCL1DoubleMuOpenNotHF2_v1";
      if (           HLT_HIUPCDoubleMuNotHF2Pixel_SingleTrack_v1) cout << " " << "HLT_HIUPCDoubleMuNotHF2Pixel_SingleTrack_v1";
      if (           HLT_HIUPCL1SingleEG2NotHF2_v1) cout << " " << "HLT_HIUPCL1SingleEG2NotHF2_v1";
      if (           HLT_HIUPCSingleEG2NotHF2Pixel_SingleTrack_v1) cout << " " << "HLT_HIUPCSingleEG2NotHF2Pixel_SingleTrack_v1";
      if (           HLT_HIUPCL1DoubleEG2NotHF2_v1) cout << " " << "HLT_HIUPCL1DoubleEG2NotHF2_v1";
      if (           HLT_HIUPCDoubleEG2NotHF2Pixel_SingleTrack_v1) cout << " " << "HLT_HIUPCDoubleEG2NotHF2Pixel_SingleTrack_v1";
      if (           HLT_HIUPCL1SingleEG5NotHF2_v1) cout << " " << "HLT_HIUPCL1SingleEG5NotHF2_v1";
      if (           HLT_HIUPCSingleEG5NotHF2Pixel_SingleTrack_v1) cout << " " << "HLT_HIUPCSingleEG5NotHF2Pixel_SingleTrack_v1";
      if (           HLT_HIUPCL1DoubleMuOpenNotHF1_v1) cout << " " << "HLT_HIUPCL1DoubleMuOpenNotHF1_v1";
      if (           HLT_HIUPCDoubleMuNotHF1Pixel_SingleTrack_v1) cout << " " << "HLT_HIUPCDoubleMuNotHF1Pixel_SingleTrack_v1";
      if (           HLT_HIUPCL1DoubleEG2NotZDCAND_v1) cout << " " << "HLT_HIUPCL1DoubleEG2NotZDCAND_v1";
      if (           HLT_HIUPCL1DoubleEG2NotZDCANDPixel_SingleTrack_v1) cout << " " << "HLT_HIUPCL1DoubleEG2NotZDCANDPixel_SingleTrack_v1";
      if (           HLT_HIUPCL1DoubleMuOpenNotZDCAND_v1) cout << " " << "HLT_HIUPCL1DoubleMuOpenNotZDCAND_v1";
      if (           HLT_HIUPCL1DoubleMuOpenNotZDCANDPixel_SingleTrack_v1) cout << " " << "HLT_HIUPCL1DoubleMuOpenNotZDCANDPixel_SingleTrack_v1";
      if (           HLT_HIUPCL1EG2NotZDCAND_v1) cout << " " << "HLT_HIUPCL1EG2NotZDCAND_v1";
      if (           HLT_HIUPCEG2NotZDCANDPixel_SingleTrack_v1) cout << " " << "HLT_HIUPCEG2NotZDCANDPixel_SingleTrack_v1";
      if (           HLT_HIUPCL1MuOpenNotZDCAND_v1) cout << " " << "HLT_HIUPCL1MuOpenNotZDCAND_v1";
      if (           HLT_HIUPCL1MuOpenNotZDCANDPixel_SingleTrack_v1) cout << " " << "HLT_HIUPCL1MuOpenNotZDCANDPixel_SingleTrack_v1";
      if (           HLT_HIUPCL1NotHFplusANDminusTH0BptxAND_v1) cout << " " << "HLT_HIUPCL1NotHFplusANDminusTH0BptxAND_v1";
      if (           HLT_HIUPCL1NotHFplusANDminusTH0BptxANDPixel_SingleTrack_v1) cout << " " << "HLT_HIUPCL1NotHFplusANDminusTH0BptxANDPixel_SingleTrack_v1";
      if (           HLT_HIUPCL1NotHFMinimumbiasHFplusANDminustTH0_v1) cout << " " << "HLT_HIUPCL1NotHFMinimumbiasHFplusANDminustTH0_v1";
      if (           HLT_HIUPCL1NotHFMinimumbiasHFplusANDminustTH0Pixel_SingleTrack_v1) cout << " " << "HLT_HIUPCL1NotHFMinimumbiasHFplusANDminustTH0Pixel_SingleTrack_v1";
      if (           HLT_HIUPCL1DoubleMuOpenNotHFMinimumbiasHFplusANDminustTH0_v1) cout << " " << "HLT_HIUPCL1DoubleMuOpenNotHFMinimumbiasHFplusANDminustTH0_v1";
      if (           HLT_HIUPCL1DoubleMuOpenNotHFMinimumbiasHFplusANDminustTH0Pixel_SingleTrack_v1) cout << " " << "HLT_HIUPCL1DoubleMuOpenNotHFMinimumbiasHFplusANDminustTH0Pixel_SingleTrack_v1";
      if (           HLT_HIL1CastorMediumJet_v1) cout << " " << "HLT_HIL1CastorMediumJet_v1";
      if (           HLT_HIL1CastorMediumJetAK4CaloJet20_v1) cout << " " << "HLT_HIL1CastorMediumJetAK4CaloJet20_v1";
      if (           HLT_HICastorMediumJetPixel_SingleTrack_v1) cout << " " << "HLT_HICastorMediumJetPixel_SingleTrack_v1";
      if (           HLT_HIUPCL1NotMinimumBiasHF2_AND_v1) cout << " " << "HLT_HIUPCL1NotMinimumBiasHF2_AND_v1";
      if (           HLT_HIUPCL1NotMinimumBiasHF2_ANDPixel_SingleTrack_v1) cout << " " << "HLT_HIUPCL1NotMinimumBiasHF2_ANDPixel_SingleTrack_v1";
      if (           HLT_HIUPCL1ZdcOR_BptxAND_v1) cout << " " << "HLT_HIUPCL1ZdcOR_BptxAND_v1";
      if (           HLT_HIUPCL1ZdcOR_BptxANDPixel_SingleTrack_v1) cout << " " << "HLT_HIUPCL1ZdcOR_BptxANDPixel_SingleTrack_v1";
      if (           HLT_HIUPCL1ZdcXOR_BptxAND_v1) cout << " " << "HLT_HIUPCL1ZdcXOR_BptxAND_v1";
      if (           HLT_HIUPCL1ZdcXOR_BptxANDPixel_SingleTrack_v1) cout << " " << "HLT_HIUPCL1ZdcXOR_BptxANDPixel_SingleTrack_v1";
      if (           HLT_HIUPCL1NotZdcOR_BptxAND_v1) cout << " " << "HLT_HIUPCL1NotZdcOR_BptxAND_v1";
      if (           HLT_HIUPCL1NotZdcOR_BptxANDPixel_SingleTrack_v1) cout << " " << "HLT_HIUPCL1NotZdcOR_BptxANDPixel_SingleTrack_v1";
      if (           HLT_HIZeroBias_v1) cout << " " << "HLT_HIZeroBias_v1";
      if (           HLT_HIL1Tech5_BPTX_PlusOnly_v1) cout << " " << "HLT_HIL1Tech5_BPTX_PlusOnly_v1";
      if (           HLT_HIL1Tech6_BPTX_MinusOnly_v1) cout << " " << "HLT_HIL1Tech6_BPTX_MinusOnly_v1";
      if (           HLT_HIL1Tech7_NoBPTX_v1) cout << " " << "HLT_HIL1Tech7_NoBPTX_v1";
      if (           HLT_HIL1MinimumBiasHF1OR_v1) cout << " " << "HLT_HIL1MinimumBiasHF1OR_v1";
      if (           HLT_HIL1MinimumBiasHF2OR_v1) cout << " " << "HLT_HIL1MinimumBiasHF2OR_v1";
      if (           HLT_HIL1MinimumBiasHF1AND_v1) cout << " " << "HLT_HIL1MinimumBiasHF1AND_v1";
      if (           HLT_HIL1MinimumBiasHF1ANDExpress_v1) cout << " " << "HLT_HIL1MinimumBiasHF1ANDExpress_v1";
      if (           HLT_HIL1MinimumBiasHF2AND_v1) cout << " " << "HLT_HIL1MinimumBiasHF2AND_v1";
      if (           HLT_HIL1MinimumBiasHF1ANDPixel_SingleTrack_v1) cout << " " << "HLT_HIL1MinimumBiasHF1ANDPixel_SingleTrack_v1";
      if (           HLT_HIZeroBiasPixel_SingleTrack_v1) cout << " " << "HLT_HIZeroBiasPixel_SingleTrack_v1";
      if (           HLT_HIPhysics_v1) cout << " " << "HLT_HIPhysics_v1";
      if (           HLT_HIPhysicsNoZS_v1) cout << " " << "HLT_HIPhysicsNoZS_v1";
      if (           HLT_HIRandom_v1) cout << " " << "HLT_HIRandom_v1";
      if (           HLT_EcalCalibration_v1) cout << " " << "HLT_EcalCalibration_v1";
      if (           HLT_HcalCalibration_v1) cout << " " << "HLT_HcalCalibration_v1";
      if (           HLT_HIPuAK4CaloJet40_Eta5p1_v2) cout << " " << "HLT_HIPuAK4CaloJet40_Eta5p1_v2";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt20_v2) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt20_v2";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt20_Cent0_10_v2) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt20_Cent0_10_v2";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt20_Cent30_100_v2) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt20_Cent30_100_v2";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt20_Cent30_100_v3) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt20_Cent30_100_v3";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt20_Cent50_100_v2) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt20_Cent50_100_v2";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt20_Cent50_100_v3) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt20_Cent50_100_v3";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt30_Cent0_10_v2) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt30_Cent0_10_v2";
      if (           HLT_HIDmesonHITrackingGlobal_Dpt40_Cent0_10_v2) cout << " " << "HLT_HIDmesonHITrackingGlobal_Dpt40_Cent0_10_v2";
      if (           HLT_HISinglePhoton10_Eta1p5_v2) cout << " " << "HLT_HISinglePhoton10_Eta1p5_v2";
      if (           HLT_HISinglePhoton15_Eta1p5_v2) cout << " " << "HLT_HISinglePhoton15_Eta1p5_v2";
      if (           HLT_HISinglePhoton20_Eta1p5_v2) cout << " " << "HLT_HISinglePhoton20_Eta1p5_v2";
      if (           HLT_HISinglePhoton10_Eta3p1_v2) cout << " " << "HLT_HISinglePhoton10_Eta3p1_v2";
      if (           HLT_HISinglePhoton15_Eta3p1_v2) cout << " " << "HLT_HISinglePhoton15_Eta3p1_v2";
      if (           HLT_HISinglePhoton20_Eta3p1_v2) cout << " " << "HLT_HISinglePhoton20_Eta3p1_v2";
      if (           HLT_HIL2Mu3Eta2p5_PuAK4CaloJet60Eta2p1_v2) cout << " " << "HLT_HIL2Mu3Eta2p5_PuAK4CaloJet60Eta2p1_v2";
      if (           HLT_HIUCC100_v2) cout << " " << "HLT_HIUCC100_v2";
      if (           HLT_HIUCC100_v3) cout << " " << "HLT_HIUCC100_v3";
      if (           HLT_HIUCC020_v2) cout << " " << "HLT_HIUCC020_v2";
      if (           HLT_HIUCC020_v3) cout << " " << "HLT_HIUCC020_v3";
      if (           HLT_HIL1DoubleMu0_part1_v1) cout << " " << "HLT_HIL1DoubleMu0_part1_v1";
      if (           HLT_HIL1DoubleMu0_part2_v1) cout << " " << "HLT_HIL1DoubleMu0_part2_v1";
      if (           HLT_HIL1DoubleMu0_part3_v1) cout << " " << "HLT_HIL1DoubleMu0_part3_v1";
      if (           HLT_HIL2DoubleMu0_NHitQ_v1) cout << " " << "HLT_HIL2DoubleMu0_NHitQ_v1";
      if (           HLT_HIL2Mu15_v1) cout << " " << "HLT_HIL2Mu15_v1";
      if (           HLT_HICastorMediumJetPixel_SingleTrack_v2) cout << " " << "HLT_HICastorMediumJetPixel_SingleTrack_v2";
      if (           HLT_HIL1MinimumBiasHF2AND_part1_v1) cout << " " << "HLT_HIL1MinimumBiasHF2AND_part1_v1";
      if (           HLT_HIL1MinimumBiasHF2AND_part2_v1) cout << " " << "HLT_HIL1MinimumBiasHF2AND_part2_v1";
      if (           HLT_HIL1MinimumBiasHF2AND_part3_v1) cout << " " << "HLT_HIL1MinimumBiasHF2AND_part3_v1";
      if (           HLT_HIL1MinimumBiasHF2AND_part4_v1) cout << " " << "HLT_HIL1MinimumBiasHF2AND_part4_v1";
      if (           HLT_HIL1MinimumBiasHF2AND_part5_v1) cout << " " << "HLT_HIL1MinimumBiasHF2AND_part5_v1";
      if (           HLT_HIL1MinimumBiasHF2AND_part6_v1) cout << " " << "HLT_HIL1MinimumBiasHF2AND_part6_v1";
      if (           HLT_HIL1MinimumBiasHF2AND_part7_v1) cout << " " << "HLT_HIL1MinimumBiasHF2AND_part7_v1";
      if (           HLT_HIL1MinimumBiasHF2AND_part8_v1) cout << " " << "HLT_HIL1MinimumBiasHF2AND_part8_v1";
      if (           HLT_HIL1MinimumBiasHF2AND_part9_v1) cout << " " << "HLT_HIL1MinimumBiasHF2AND_part9_v1";
      if (           HLT_HIL1MinimumBiasHF2AND_part10_v1) cout << " " << "HLT_HIL1MinimumBiasHF2AND_part10_v1";
      if (           HLT_HIL1MinimumBiasHF2AND_part11_v1) cout << " " << "HLT_HIL1MinimumBiasHF2AND_part11_v1";
      if (           HLT_HIL1MinimumBiasHF2ANDExpress_v1) cout << " " << "HLT_HIL1MinimumBiasHF2ANDExpress_v1";
      if (           HLT_HIL1HFplusANDminusTH0Express_v1) cout << " " << "HLT_HIL1HFplusANDminusTH0Express_v1";
      if (           HLT_HIL1MinimumBiasHF2ANDPixel_SingleTrack_v1) cout << " " << "HLT_HIL1MinimumBiasHF2ANDPixel_SingleTrack_v1";
      if (           HLT_HIPhysicsForZS_v1) cout << " " << "HLT_HIPhysicsForZS_v1";
      cout << " " << endl;
   }
}
