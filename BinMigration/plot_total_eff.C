#include <iostream>
#include <fstream>
#include <sstream>
#include <TFile.h>
#include <TDirectory.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TROOT.h>
// #include "../../../CMS_lumi.C"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TText.h"
#include "TCut.h"
#include "TChain.h"
#include "THStack.h"
#include "TString.h"
#include "TGraphAsymmErrors.h"


int xlo = 1;
int xhi = 2;
double norm_exgg = 0.0064; //older value norm_exgg = norm_exgg
double norm_qed  = 1.01; //older value norm_qed = 1.03

//double norm_cep  = 0.117; // without scaling cep
double norm_cep  = 0.117*0.346; // with proper_hf_calo_cuts
double getError(double A, double eA, double B, double eB);
const char *dir = "fig_eff";

const int nptbins=7;
double ptbin[nptbins]={0,2,4,6,8,10,16};
const int nptbin= sizeof(ptbin)/sizeof(double) - 1;


void make_hist(TH1D *&, Color_t , int );

void plot_total_eff(){
 
  //gROOT->LoadMacro("CMS_lumi.C");
  bool outOfFrame    = false;
  TH1::SetDefaultSumw2();
   gStyle->SetOptStat(1);

   TFile *outf= new TFile("all_plot.root","recreate");  

  TChain *hm = new TChain("diphoton");
  hm->Add("hm_photon_efficiency_gen_reco_match_v2.root");


   const TCut crack = "fabs(phoSCEta_gen_reco_1)<2.4 && fabs(phoSCEta_gen_reco_2)<2.4 && (fabs(phoSCEta_gen_reco_1) < 1.4442 || fabs(phoSCEta_gen_reco_1) > 1.566) && (fabs(phoSCEta_gen_reco_2) < 1.4442 || fabs(phoSCEta_gen_reco_2) > 1.566) ";


  const TCut purity = "pho_swissCrx_gen_reco_1 < 0.95 && fabs(pho_seedTime_gen_reco_1) < 3.6 && pho_swissCrx_gen_reco_2 < 0.95 && fabs(pho_seedTime_gen_reco_2) < 3.6 && vSum_M > 5" ;
 
  const TCut excl = "ngenTrk ==0 && nEle ==0 && ngsfEle==0";
  const TCut eta_barrel = " fabs(phoEta_gen_reco) < 1.4791 && phoSigmaIEtaIEta_gen_reco < 0.02";
  const TCut eta_endcap = " fabs(phoEta_gen_reco) > 1.4791 && phoSigmaIEtaIEta_gen_reco < 0.06";


  const TCut calocut = "leadingEmEnergy_EB< 0.55 && leadingEmEnergy_EE < 3.16 && leadingHadEnergy_HB < 2.0 && leadingHadEnergy_HE < 3.0 && leadingHadEnergy_HF_Plus < 4.85 && leadingHadEnergy_HF_Minus < 4.12 ";  



  const TCut apply_cut     =  purity && excl && calocut && "vSum_Pt_doubleEG2 < 1 && pho_aco_doubleEG2 < 0.01" && crack ;


  //// hm histogram

  TH1D* hnum_hm          = new TH1D("hnum_hm" ,"",nptbin,ptbin);
  TH1D* hnum_hm2         = new TH1D("hnum_hm2","",nptbin,ptbin);
  TH1D* hden_hm2         = new TH1D("hden_hm2","",nptbin,ptbin);
  TH1D* hden_hm          = new TH1D("hden_hm" ,"",nptbin,ptbin);
 
  hm->Project(hnum_hm->GetName(),        "recoEt_doubleEG2_1"  ,apply_cut);
  hm->Project(hnum_hm2->GetName(),       "recoEt_doubleEG2_2"  ,apply_cut);


  hm->Project(hden_hm->GetName(),        "genphoEt1_accp"); 
  hm->Project(hden_hm2->GetName(),       "genphoEt2_accp");


  hnum_hm->Add(hnum_hm2);
  hden_hm->Add(hden_hm2);

  TH1D* hinvmass_lbyl    = new TH1D("hinvmass_lbyl","",10,0,30);
  hm->Project(hinvmass_lbyl->GetName(), "vSum_M_doubleEG2"  ,apply_cut );
  
  double genError;
  double int_num = hnum_hm->IntegralAndError(1,6,genError);
  double int_num_err = genError;

  double denError;
  double int_den = hden_hm->IntegralAndError(1,6,denError);
  double int_den_err = denError;


  cout << "Integral:" << int_num << "  error num:" << int_num_err << "  den int:" << int_den << "  error" << int_den_err; 
  cout << "  efficiency:" << int_num/int_den << "  error: " << getError(int_num,int_num_err,int_den,int_den_err) <<  endl;  


  TGraphAsymmErrors* gr = new TGraphAsymmErrors();
  gr->Divide( hnum_hm, hden_hm );


 //// ged histogram

  TChain *ged = new TChain("diphoton");
  ged->Add("scged_photon_efficiency_gen_reco_match_v2.root");

   const TCut crack1 = "fabs(phoEta_gen_reco_1)<2.4 && fabs(phoEta_gen_reco_2)<2.4 && (fabs(phoEta_gen_reco_1) < 1.4442 || fabs(phoEta_gen_reco_1) > 1.566) && (fabs(phoEta_gen_reco_2) < 1.4442 || fabs(phoEta_gen_reco_2) > 1.566) ";

  const TCut ged_cut     =  purity && excl && calocut && "vSum_Pt_doubleEG2 < 1 && pho_aco_doubleEG2 < 0.01 && vSum_M_doubleEG2 > 5" && crack1 ;
  const TCut gencut = "vSum_gg_M_gen > 5 && vSum_gg_Pt_gen < 1 && gen_aco < 0.01";
  const TCut gencutfid = "genphoEt1_all>2&&genphoEt2_all>2&&abs(genphoEta1_all)<2.4&&abs(genphoEta2_all)<2.4";

  //// ged histogram

  TH1D* hnum_ged          = new TH1D("hnum_ged" ,"",nptbin,ptbin);
  TH1D* hnum_ged2         = new TH1D("hnum_ged2","",nptbin,ptbin);
  TH1D* hden_ged2         = new TH1D("hden_ged2","",nptbin,ptbin);
  TH1D* hden_ged          = new TH1D("hden_ged" ,"",nptbin,ptbin);
 
  ged->Project(hnum_ged->GetName(),        "3"  ,ged_cut);
  ged->Project(hnum_ged2->GetName(),       "3"  ,ged_cut && gencutfid);
  ged->Project(hden_ged->GetName(),        "3", gencut && gencutfid);
  ged->Project(hden_ged2->GetName(),       "3", gencut && gencutfid);


  // hnum_ged->Add(hnum_ged2);
  // hden_ged->Add(hden_ged2);
  
  double genError2;
  double int_num2 = hnum_ged->IntegralAndError(1,6,genError2);
  double int_num_err2 = genError2;

  double denError2;
  double int_den2 = hden_ged->IntegralAndError(1,6,denError2);
  double int_den_err2 = denError2;


  cout << "Integral:" << int_num2 << "  error num:" << int_num_err2 << "  den int:" << int_den2 << "  error" << int_den_err2; 
  cout << "  efficiency:" << int_num2/int_den2 << "  error: " << getError(int_num2,int_num_err2,int_den2,int_den_err2) <<  endl;  
  
  genError2;
  int_num2 = hnum_ged2->IntegralAndError(1,6,genError2);
  int_num_err2 = genError2;

  denError2;
  int_den2 = hden_ged2->IntegralAndError(1,6,denError2);
  int_den_err2 = denError2;


  cout << "without bin migration:" << endl;
  cout << "Integral:" << int_num2 << "  error num:" << int_num_err2 << "  den int:" << int_den2 << "  error" << int_den_err2; 
  cout << "  efficiency:" << int_num2/int_den2 << "  error: " << getError(int_num2,int_num_err2,int_den2,int_den_err2) <<  endl;  


  TGraphAsymmErrors* gr2 = new TGraphAsymmErrors();
  gr2->Divide( hnum_ged, hden_ged );
  
 
  TLegend *leg2=new TLegend(0.69,0.27,0.96,0.39,NULL,"brNDC");
  leg2->SetFillColor(0);
  leg2->SetBorderSize(0);
  leg2->SetFillStyle(0);
  leg2->SetTextFont(43);
  leg2->SetTextSize(24);
  leg2->AddEntry(gr,"HM","pl");
  leg2->AddEntry(gr2,"GED ","pl");
  
  
  
    
  int W = 700;
  int H = 600;
  
  float T = 0.08;
  float B = 0.14; 
  float L = 0.14;
  float R = 0.04;


  //// Diphoton invmass......................
  TCanvas* c1 = new TCanvas("c1","Reco efficiency",50,50,W,H);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetFrameFillStyle(0);
  c1->SetFrameBorderMode(0);
  c1->SetLeftMargin( L );
  c1->SetRightMargin( R );
  c1->SetTopMargin( T );
  c1->SetBottomMargin( B );
  c1->SetTickx(0);
  c1->SetTicky(0);
  
  c1->cd();
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gr->SetMaximum(1.2);
  gr->SetMinimum(0.0);
  //gr->SetMarkerColor(kRed);
  //gr->SetLineColor(kRed);
  gr->GetXaxis()->SetTitle("Photon E_{T} (GeV)");
  gr->GetYaxis()->SetTitle("Eff ^{#gamma#gamma}");
  gr->Draw("APE");
  gr2->SetMarkerColor(kRed);
  gr2->SetLineColor(kRed);
  gr2->SetMarkerStyle(22);
  gr2->Draw("PSAME");

 /* hpho_pt_eff->SetMaximum(1.2);
  hpho_pt_eff->SetMinimum(0.0);

  make_hist(hpho_pt_eff,kRed,20);
  hpho_pt_eff->GetXaxis()->SetTitle("Photon E_{T} (GeV)");
  hpho_pt_eff->GetYaxis()->SetTitle("Efficiency");
  hpho_pt_eff->Draw("p");*/

  
  leg2->Draw();
  //CMS_lumi( c1, 104, 10,lumi_PbPb2015 );
  c1->Print("MC_driven_total_eff_wo_pixel_cut.gif");
  c1->Print("MC_driven_total_eff_wo_pixel_cut.pdf");

  cout << "Light by light Integral:   "  << hinvmass_lbyl->Integral() << endl;

  new TCanvas(); 
  hden_hm->Draw("p");
  hnum_hm->SetMarkerColor(kRed);
  hnum_hm->Draw("psames");
  
  }

void make_hist(TH1D *& hist, Color_t kcolor, int kstyle){
  // For the axis labels:
  hist->SetLabelColor(1, "XYZ");
  hist->SetLabelFont(42, "XYZ");
  hist->SetLabelOffset(0.007, "XYZ");
  hist->SetLabelSize(0.05, "XYZ");
 
  hist->SetMarkerColor(kcolor);
  hist->SetFillColor(kcolor);
  hist->SetMarkerStyle(kstyle);
  
 
}

//Ratio Error
double getError(double A, double eA, double B, double eB){
  double f=A/B;
  double fA=eA/A;
  double fB=eB/B;
  double eR=  f*sqrt( (fA*fA + fB*fB )) ;
  return eR;
}
