#define MC // comment this line to run on data

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
#include "TDirectory.h"
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
double acop(double dphi) {
   return 1.-fabs(TVector2::Phi_mpi_pi(dphi))/TMath::Pi(); 
};

void hardbrem(int idir=0) {
   TChain *tchHLT = new TChain("hltanalysis/HltTree");
   TChain *tchEvt = new TChain("ggHiNtuplizer/EventTree");

   TString filename;
#ifdef MC
  if (idir==0) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/01/hiForest_qedee_0.root";
  else if (idir==1) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/01/hiForest_qedee_10.root";
  else if (idir==2) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/01/hiForest_qedee_1.root";
  else if (idir==3) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/01/hiForest_qedee_2.root";
  else if (idir==4) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/01/hiForest_qedee_3.root";
  else if (idir==5) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/01/hiForest_qedee_5.root";
  else if (idir==6) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/01/hiForest_qedee_6.root";
  else if (idir==7) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/01/hiForest_qedee_7.root";
  else if (idir==8) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/01/hiForest_qedee_8.root";
  else if (idir==9) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/01/hiForest_qedee_9.root";
  else if (idir==10) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/02/hiForest_qedee_0.root";
  else if (idir==11) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/02/hiForest_qedee_10.root";
  else if (idir==12) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/02/hiForest_qedee_1.root";
  else if (idir==13) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/02/hiForest_qedee_2.root";
  else if (idir==14) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/02/hiForest_qedee_4.root";
  else if (idir==15) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/02/hiForest_qedee_5.root";
  else if (idir==16) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/02/hiForest_qedee_6.root";
  else if (idir==17) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/02/hiForest_qedee_7.root";
  else if (idir==18) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/02/hiForest_qedee_8.root";
  else if (idir==19) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/02/hiForest_qedee_9.root";
  else if (idir==20) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/03/hiForest_qedee_0.root";
  else if (idir==21) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/03/hiForest_qedee_1.root";
  else if (idir==22) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/03/hiForest_qedee_2.root";
  else if (idir==23) filename = "/eos/cms/store/group/phys_diffraction/diphoton/qed_ee/qed_ee_approval/hiforest/03/hiForest_qedee_3.root";
#else
  if (idir==0) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_0.root";
  else if (idir==1   ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_10.root";
  else if (idir==2   ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_11.root";
  else if (idir==3   ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_12.root";
  else if (idir==4   ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_13.root";
  else if (idir==5   ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_14.root";
  else if (idir==6   ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_15.root";
  else if (idir==7   ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_16.root";
  else if (idir==8   ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_17.root";
  else if (idir==9   ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_18.root";
  else if (idir==10  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_19.root";
  else if (idir==11  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_1.root";
  else if (idir==12  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_2.root";
  else if (idir==13  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_3.root";
  else if (idir==14  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_4.root";
  else if (idir==15  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_5.root";
  else if (idir==16  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_6.root";
  else if (idir==17  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_7.root";
  else if (idir==18  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_8.root";
  else if (idir==19  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/00/hiforest_9.root";
  else if (idir==20  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_0.root";
  else if (idir==21  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_10.root";
  else if (idir==22  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_11.root";
  else if (idir==23  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_12.root";
  else if (idir==24  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_13.root";
  else if (idir==25  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_14.root";
  else if (idir==26  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_15.root";
  else if (idir==27  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_16.root";
  else if (idir==28  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_17.root";
  else if (idir==29  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_18.root";
  else if (idir==30  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_19.root";
  else if (idir==31  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_1.root";
  else if (idir==32  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_2.root";
  else if (idir==33  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_3.root";
  else if (idir==34  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_4.root";
  else if (idir==35  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_5.root";
  else if (idir==36  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_6.root";
  else if (idir==37  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_7.root";
  else if (idir==38  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_8.root";
  else if (idir==39  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/01/hiforest_9.root";
  else if (idir==40  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_0.root";
  else if (idir==41  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_10.root";
  else if (idir==42  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_11.root";
  else if (idir==43  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_12.root";
  else if (idir==44  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_13.root";
  else if (idir==45  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_14.root";
  else if (idir==46  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_15.root";
  else if (idir==47  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_16.root";
  else if (idir==48  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_17.root";
  else if (idir==49  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_18.root";
  else if (idir==50  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_19.root";
  else if (idir==51  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_1.root";
  else if (idir==52  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_2.root";
  else if (idir==53  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_3.root";
  else if (idir==54  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_4.root";
  else if (idir==55  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_5.root";
  else if (idir==56  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_6.root";
  else if (idir==57  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_7.root";
  else if (idir==58  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_8.root";
  else if (idir==59  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/02/hiforest_9.root";
  else if (idir==60  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_0.root";
  else if (idir==61  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_10.root";
  else if (idir==62  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_11.root";
  else if (idir==63  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_12.root";
  else if (idir==64  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_13.root";
  else if (idir==65  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_14.root";
  else if (idir==66  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_15.root";
  else if (idir==67  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_16.root";
  else if (idir==68  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_17.root";
  else if (idir==69  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_18.root";
  else if (idir==70  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_19.root";
  else if (idir==71  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_1.root";
  else if (idir==72  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_2.root";
  else if (idir==73  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_3.root";
  else if (idir==74  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_4.root";
  else if (idir==75  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_5.root";
  else if (idir==76  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_6.root";
  else if (idir==77  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_7.root";
  else if (idir==78  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_8.root";
  else if (idir==79  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/03/hiforest_9.root";
  else if (idir==80  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_0.root";
  else if (idir==81  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_10.root";
  else if (idir==82  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_11.root";
  else if (idir==83  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_12.root";
  else if (idir==84  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_13.root";
  else if (idir==85  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_14.root";
  else if (idir==86  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_15.root";
  else if (idir==87  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_16.root";
  else if (idir==88  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_17.root";
  else if (idir==89  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_18.root";
  else if (idir==90  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_19.root";
  else if (idir==91  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_1.root";
  else if (idir==92  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_2.root";
  else if (idir==93  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_3.root";
  else if (idir==94  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_4.root";
  else if (idir==95  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_5.root";
  else if (idir==96  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_6.root";
  else if (idir==97  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_7.root";
  else if (idir==98  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_8.root";
  else if (idir==99  ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/04/hiforest_9.root";
  else if (idir==100 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_0.root";
  else if (idir==101 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_10.root";
  else if (idir==102 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_11.root";
  else if (idir==103 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_12.root";
  else if (idir==104 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_13.root";
  else if (idir==105 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_14.root";
  else if (idir==106 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_15.root";
  else if (idir==107 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_16.root";
  else if (idir==108 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_17.root";
  else if (idir==109 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_18.root";
  else if (idir==110 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_19.root";
  else if (idir==111 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_1.root";
  else if (idir==112 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_2.root";
  else if (idir==113 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_3.root";
  else if (idir==114 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_4.root";
  else if (idir==115 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_5.root";
  else if (idir==116 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_6.root";
  else if (idir==117 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_7.root";
  else if (idir==118 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_8.root";
  else if (idir==119 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/05/hiforest_9.root";
  else if (idir==120 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_0.root";
  else if (idir==121 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_10.root";
  else if (idir==122 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_11.root";
  else if (idir==123 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_12.root";
  else if (idir==124 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_13.root";
  else if (idir==125 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_14.root";
  else if (idir==126 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_15.root";
  else if (idir==127 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_16.root";
  else if (idir==128 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_17.root";
  else if (idir==129 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_18.root";
  else if (idir==130 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_19.root";
  else if (idir==131 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_1.root";
  else if (idir==132 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_2.root";
  else if (idir==133 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_3.root";
  else if (idir==134 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_4.root";
  else if (idir==135 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_5.root";
  else if (idir==136 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_6.root";
  else if (idir==137 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_7.root";
  else if (idir==138 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_8.root";
  else if (idir==139 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/06/hiforest_9.root";
  else if (idir==140 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_0.root";
  else if (idir==141 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_100.root";
  else if (idir==142 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_101.root";
  else if (idir==143 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_10.root";
  else if (idir==144 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_11.root";
  else if (idir==145 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_12.root";
  else if (idir==146 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_13.root";
  else if (idir==147 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_14.root";
  else if (idir==148 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_15.root";
  else if (idir==149 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_16.root";
  else if (idir==150 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_19.root";
  else if (idir==151 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_1.root";
  else if (idir==152 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_2.root";
  else if (idir==153 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_3.root";
  else if (idir==154 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_4.root";
  else if (idir==155 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_5.root";
  else if (idir==156 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_6.root";
  else if (idir==157 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_7.root";
  else if (idir==158 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_8.root";
  else if (idir==159 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/07/hiforest_9.root";
  else if (idir==160 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_0.root";
  else if (idir==161 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_10.root";
  else if (idir==162 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_11.root";
  else if (idir==163 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_12.root";
  else if (idir==164 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_13.root";
  else if (idir==165 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_14.root";
  else if (idir==166 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_15.root";
  else if (idir==167 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_16.root";
  else if (idir==168 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_17.root";
  else if (idir==169 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_18.root";
  else if (idir==170 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_19.root";
  else if (idir==171 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_1.root";
  else if (idir==172 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_2.root";
  else if (idir==173 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_3.root";
  else if (idir==174 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_4.root";
  else if (idir==175 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_5.root";
  else if (idir==176 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_6.root";
  else if (idir==177 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_7.root";
  else if (idir==178 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_8.root";
  else if (idir==179 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/08/hiforest_9.root";
  else if (idir==180 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/09/hiforest_0.root";
  else if (idir==181 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/09/hiforest_10.root";
  else if (idir==182 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/09/hiforest_11.root";
  else if (idir==183 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/09/hiforest_1.root";
  else if (idir==184 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/09/hiforest_2.root";
  else if (idir==185 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/09/hiforest_3.root";
  else if (idir==186 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/09/hiforest_4.root";
  else if (idir==187 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/09/hiforest_5.root";
  else if (idir==188 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/09/hiforest_6.root";
  else if (idir==189 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/09/hiforest_7.root";
  else if (idir==190 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/09/hiforest_8.root";
  else if (idir==191 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/09/hiforest_9.root";
  else if (idir==192 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/10/hiforest_0.root";
  else if (idir==193 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/10/hiforest_1.root";
  else if (idir==194 ) filename = "/eos/cms/store/group/phys_diffraction/diphoton/aug_reco_data_check_for_lumi/hiforest2/10/hiforest_2.root";
#endif

   tchHLT->Add(filename);
   tchEvt->Add(filename);

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
   evtR.fChain->SetBranchStatus("eleSCEn",1);
   evtR.fChain->SetBranchStatus("eleSCEta",1);
   evtR.fChain->SetBranchStatus("eleEn",1);
   evtR.fChain->SetBranchStatus("eleSCPhi",1);

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
   TDirectory *tdir = fout->mkdir("tpTree_conv");
   tdir->cd();
   
   TTree *tr = new TTree("fitter_tree","");
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
   tr->Branch("gentrkcharge",&(evtR.gentrkcharge));
   tr->Branch("gentrkPt",&(evtR.gentrkPt));
   tr->Branch("gentrkEta",&(evtR.gentrkEta));
   tr->Branch("gentrkPhi",&(evtR.gentrkPhi));
   // derived quantities
   int nPho_notag; float phoSCEt_notag, phoSCEta_notag, phoSCPhi_notag, phoAco;
   tr->Branch("nPho_notag",&nPho_notag,"nPho_notag/I");
   tr->Branch("phoSCEt_notag",&phoSCEt_notag,"phoSCEt_notag/F");
   tr->Branch("phoSCEta_notag",&phoSCEta_notag,"phoSCEta_notag/F");
   tr->Branch("phoSCPhi_notag",&phoSCPhi_notag,"phoSCPhi_notag/F");
   tr->Branch("phoAco",&phoAco,"phoAco/F");
   float tagPt, tagEta, tagPhi, tagEt, tagSCEt; int tagCharge;
   tr->Branch("tagCharge",&tagCharge,"tagCharge/I");
   tr->Branch("tagEt",&tagEt,"tagEt/F");
   tr->Branch("tagSCEt",&tagSCEt,"tagSCEt/F");
   tr->Branch("tagPt",&tagPt,"tagPt/F");
   tr->Branch("tagEta",&tagEta,"tagEta/F");
   tr->Branch("tagPhi",&tagPhi,"tagPhi/F");
   float probetkPt, probetkEta, probetkPhi, probeTkMinDphi, probetkAco; int probetkCharge; float probetkMinDpt, probetkMinDet;
   tr->Branch("probetkCharge",&probetkCharge,"probetkCharge/I");
   tr->Branch("probetkPt",&probetkPt,"probetkPt/F");
   tr->Branch("probetkEta",&probetkEta,"probetkEta/F");
   tr->Branch("probetkPhi",&probetkPhi,"probetkPhi/F");
   tr->Branch("probetkMinDpt",&probetkMinDpt,"probetkMinDpt/F");
   tr->Branch("probetkAco",&probetkAco,"probetkAco/F");
   int nmatchele; float matchelePt, matcheleEta, matchelePhi;
   int nmatchtrk; float matchtrkPt, matchtrkEta, matchtrkPhi;
   tr->Branch("nmatchele",&nmatchele,"nmatchele/I");
   tr->Branch("matchelePt",&matchelePt,"matchelePt/F");
   tr->Branch("matcheleEta",&matcheleEta,"matcheleEta/F");
   tr->Branch("matchelePhi",&matchelePhi,"matchelePhi/F");
   tr->Branch("nmatchtrk",&nmatchtrk,"nmatchtrk/I");
   tr->Branch("matchtrkPt",&matchtrkPt,"matchtrkPt/F");
   tr->Branch("matchtrkEta",&matchtrkEta,"matchtrkEta/F");
   tr->Branch("matchtrkPhi",&matchtrkPhi,"matchtrkPhi/F");

   int cnt[10] = {0};

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      cnt[0]++;
      // if (jentry>100000) break;
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
      if (evtR.nPho==0 || evtR.nPho>2) continue;
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
               // evtR.elePt->at(i)>5 &&
               (evtR.eleEn->at(i)/cosh(evtR.eleSCEta->at(i)))>5 &&
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

      tagCharge = evtR.eleCharge->at(itag);
      tagEt = evtR.eleEn->at(itag)/cosh(evtR.eleSCEta->at(itag));
      tagSCEt = evtR.eleSCEn->at(itag)/cosh(evtR.eleSCEta->at(itag));
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

      // ignore photons close to this tag electron, or that fail ID cuts
      for (int i=0; i<evtR.nPho; i++) {
         double deta = getDETA(evtR.eleEta->at(itag), evtR.phoSCEta->at(i));
         double dphi = getDPHI(evtR.elePhi->at(itag), evtR.phoSCPhi->at(i));

         bool idok = true;
         idok = idok && evtR.phoSCEt->at(i)>2;
         idok = idok && fabs(evtR.phoSCEta->at(i))<2.4;
         idok = idok && ((fabs(evtR.phoSCEta->at(i)) < 1.4791 && evtR.phoSigmaIEtaIEta->at(i) < 0.02) ||
               (fabs(evtR.phoSCEta->at(i)) > 1.4791 && evtR.phoSigmaIEtaIEta->at(i) < 0.06));
         idok = idok && (fabs(evtR.phoSCEta->at(i)) < 1.4442 || fabs(evtR.phoSCEta->at(i)) > 1.566);
         idok = idok && evtR.pho_swissCrx->at(i) < 0.95;
         idok = idok && fabs(evtR.pho_seedTime->at(i)) < 3.6;

         if ((deta<0.15 && dphi<0.7) || !idok) continue;
         else {
            nPho_notag++;
            phoSCEt_notag = evtR.phoSCEt->at(i);
            phoSCEta_notag = evtR.phoSCEta->at(i);
            phoSCPhi_notag = evtR.phoSCPhi->at(i);
            phoAco = acop(tagPhi-phoSCPhi_notag);
         }
      }

      // there should be max 1 photon unmatched to the tag
      if (nPho_notag!=1) continue;
      cnt[7]++;

      // the tag and photon should be +- back to back
      if (getDPHI(tagPhi,phoSCPhi_notag)<2) continue;
      cnt[8]++;

      // find the track not matched to the electron tag or the photon. there should be only 1
      probetkMinDpt=999.;
      probetkMinDet=999.;
      for (int i=0; i<evtR.ngenTrk; i++) {
         if (evtR.gentrkPt->at(i)>3) continue;
         double deta = getDETA(evtR.gentrkEta->at(i), tagEta);
         double dphi = getDPHI(evtR.gentrkPhi->at(i), tagPhi);
         if (deta<0.15 && dphi<0.7) continue;

         // deta = getDETA(evtR.gentrkEta->at(i), phoSCEta_notag);
         // dphi = getDPHI(evtR.gentrkPhi->at(i), phoSCPhi_notag);
         // removing the matching with the photon
         // if (deta<0.15 && dphi<0.7) continue;

         double dpt = fabs((tagPt-evtR.gentrkPt->at(i))-phoSCEt_notag);
         double det = fabs((tagEt-evtR.gentrkPt->at(i))-phoSCEt_notag);

         if (det<probetkMinDet) {
            probetkMinDet = det;
            probetkMinDpt = dpt;
            probetkCharge = evtR.gentrkcharge->at(i);   
            probetkPt = evtR.gentrkPt->at(i);   
            probetkEta = evtR.gentrkEta->at(i);   
            probetkPhi = evtR.gentrkPhi->at(i);   
            probetkAco = acop(tagPhi-probetkPhi);
         }
      }
      if (probetkMinDpt>990) continue;
      cnt[9]++;

      // check event kinematics: is it a hard brem event?
      // if (fabs((tagPt-probetkPt)-phoSCEt_notag) > 1) continue;
      // cnt[9]++;

      // at this point we have a tag electron, an unmatched track, and a hard brem photon. Check if there is sthg suspicious around the photon.
      int ieleminDphi=-1, itrkminDphi=-1;
      float eleminDphi=999., trkminDphi=999;
      nmatchele=0; matchelePt=-999.; matcheleEta=-999; matchelePhi=-999.;
      nmatchtrk=0; matchtrkPt=-999.; matchtrkEta=-999; matchtrkPhi=-999.;

      for (int i=0; i<evtR.nEle; i++) {
         // ignore the tag
         if (getDPHI(evtR.elePhi->at(i),tagPhi)<1e-6) continue;

         double deta = getDETA(evtR.eleEta->at(i),phoSCEta_notag);
         double dphi = getDPHI(evtR.elePhi->at(i),phoSCPhi_notag);
         if (deta<0.15 && dphi<0.7) {
            nmatchele++;
            if (dphi<eleminDphi) {
               matchelePt=evtR.elePt->at(i);
               matcheleEta=evtR.eleEta->at(i);
               matchelePhi=evtR.elePhi->at(i);
            }
         }
      }

      for (int i=0; i<evtR.ngenTrk; i++) {
         // ignore the tag
         if (getDPHI(evtR.gentrkPhi->at(i),tagPhi)<1e-6) continue;
         if (getDPHI(evtR.gentrkPhi->at(i),probetkPhi)<1e-6) continue;

         double deta = getDETA(evtR.gentrkEta->at(i),phoSCEta_notag);
         double dphi = getDPHI(evtR.gentrkPhi->at(i),phoSCPhi_notag);
         if (deta<0.15 && dphi<0.7) {
            nmatchtrk++;
            if (dphi<trkminDphi) {
               matchtrkPt=evtR.gentrkPt->at(i);
               matchtrkEta=evtR.gentrkEta->at(i);
               matchtrkPhi=evtR.gentrkPhi->at(i);
            }
         }
      }

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
