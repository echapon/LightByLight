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
#include "../../../CMS_lumi.C"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TText.h"
#include "THStack.h"
#include "TCut.h"
#include "TAxis.h"
#include "TChain.h"

void make_canvas(TCanvas *&);
void make_canvas_ratio(TCanvas *&);
void make_hist(TH1D *&, Color_t , int );
void make_hist_ratio(TH1D *&,Color_t, int) ;

void drawText(const char *text, float xp, float yp, int size){
  TLatex *tex = new TLatex(xp,yp,text);
  tex->SetTextFont(43);
  tex->SetTextSize(size);
  tex->SetTextColor(kMagenta);
  tex->SetLineWidth(1);
  tex->SetNDC();
  tex->Draw();
}

TCanvas* PlotHistsAndRatio(TCanvas* , TH1D* , TH1D*, double , double , int , int , double , double , const char *); 
void plot_hist_and_ratio_hm(){
 
  //gROOT->LoadMacro("CMS_lumi.C");
  bool outOfFrame    = false;
  TH1::SetDefaultSumw2();
   gStyle->SetOptStat(0);

   TFile *outf= new TFile("mycheck.root","recreate");  

  TChain *data = new TChain("diphoton");
  data->Add("~/diphoton/rereco/mc_pprereco/qed_noptcut_3-53_invmass/sc_position_mod/hf_boundries_corr/data_hybrid_dielectron_sc_hf_corr.root");
 
 
  TChain *qed = new TChain("diphoton");
  qed->Add("~/diphoton/rereco/mc_pprereco/qed_noptcut_3-53_invmass/sc_position_mod/hf_boundries_corr/mc_hybrid_dielectron_sc_hf_corr.root");
 
  const TCut hovere    = " eleHoverE_1 < 0.02 && eleHoverE_2 < 0.02";
  const TCut track_iso = " (eleTrackIso_1/elePt_1) < 0.05 && (eleTrackIso_2/elePt_2) < 0.05";
  const TCut ecal_iso  = " (eleECalIso_1/elePt_1)  < 0.3 && (eleECalIso_2/elePt_2)  < 0.3";
  const TCut hcal_iso  = " (eleHCalIso_1/elePt_1)  < 0.3 && (eleHCalIso_2/elePt_2)  < 0.3";

  const TCut opp_chrg  = " eleCharge_1 != eleCharge_2 ";
  const TCut ele_pt    = " elePt_1 > 2 && elePt_2 > 2  ";
  const TCut ele_two   = " nEle==2 ";
  const TCut no_pho    = " nPho==0";
  const TCut no_extrk  = " nextra_track==0";

  const TCut calocut = "leadingEmEnergy_EB< 0.55 && leadingEmEnergy_EE < 3.16 && leadingHadEnergy_HB < 2.0 && leadingHadEnergy_HE < 3.0 && leadingHadEnergy_HF_Plus < 4.85 && leadingHadEnergy_HF_Minus < 4.12 ";

  //const TCut calocut = "leadingEmEnergy_EB< 0.52 && leadingEmEnergy_EE < 2.18 && leadingHadEnergy_HB < 1.18 && leadingHadEnergy_HE < 1.95 && leadingHadEnergy_HF_Plus < 5 && leadingHadEnergy_HF_Minus < 5 ";

  const TCut ele_gsf_pt    = " elegsfTrkPt_1 > 2 && elegsfTrkPt_2 > 2  ";
  const TCut gsf_miss_hit  = " elegsfTrkMissHits_1 <= 1 && elegsfTrkMissHits_2 <= 1";

 
  const TCut ele_id     = ele_gsf_pt && "vSum_M > 4" && gsf_miss_hit && no_extrk && calocut && "vSum_Pt < 2.0 && ele_aco < 0.01";  
  const TCut ele_id_qed = ele_gsf_pt && "vSum_M > 4" && gsf_miss_hit && no_extrk && calocut && "vSum_Pt < 2.0 && ele_aco < 0.01";

  //const TCut ele_id     = "vSum_M > 4" && gsf_miss_hit && no_extrk && calocut && "vSum_Pt < 2.0 && ele_aco < 0.01";  
  //const TCut ele_id_qed = "vSum_M > 4" && gsf_miss_hit && no_extrk && calocut && "vSum_Pt < 2.0 && ele_aco < 0.01";

 
  //// data histogram

  TH1D* hele_pt_data   = new TH1D("hele_pt_data","",30,0,15);
  TH1D* hele_eta_data  = new TH1D("hele_eta_data","",25,-2.4,2.4);
  TH1D* hele_phi_data  = new TH1D("hele_phi_data","",16,-4,4);

  TH1D* hele_pt2_data   = new TH1D("hele_pt2_data","",30,0,15);
  TH1D* hele_eta2_data  = new TH1D("hele_eta2_data","",25,-2.4,2.4);
  TH1D* hele_phi2_data  = new TH1D("hele_phi2_data","",16,-4,4);
  
  TH1D* hdpt_data   = new TH1D("hdpt_data","",10,0,5);
  TH1D* hdeta_data  = new TH1D("hdeta_data","",6,0,2.4);

  
  TH1D* hdiele_pt_data         = new TH1D("hdiele_pt_data","",30,0,5);
  TH1D* hdiele_rapidity_data   = new TH1D("hdiele_rapidity_data","",25,-2.4,2.4);
  TH1D* hinvmass_data          = new TH1D("hinvmass_data","",25,0,50);
  TH1D* hinvmass_data_fine     = new TH1D("hinvmass_data_fine","",1000,0,100);


  cout << " data project ongoing" << endl;
  data->Project(hele_pt_data->GetName(), "elegsfTrkPt_1",ele_id);
  data->Project(hele_eta_data->GetName(), "elegsfTrkEta_1",ele_id);
  data->Project(hele_phi_data->GetName(), "elegsfTrkPhi_1",ele_id);
  data->Project(hele_pt2_data->GetName(), "elegsfTrkPt_2",ele_id);
  data->Project(hele_eta2_data->GetName(), "elegsfTrkEta_2",ele_id);
  data->Project(hele_phi2_data->GetName(), "elegsfTrkPhi_2",ele_id);

  hele_pt_data->Add(hele_pt2_data);
  hele_eta_data->Add(hele_eta2_data);
  hele_phi_data->Add(hele_phi2_data);
  
  data->Project(hdiele_pt_data->GetName(), "vSum_Pt",ele_id);
  data->Project(hdiele_rapidity_data->GetName(), "vSum_Rapidity",ele_id);
  data->Project(hinvmass_data->GetName(), "vSum_M",ele_id);
  data->Project(hinvmass_data_fine->GetName(), "vSum_M",ele_id);

 
  // get qed file

  TH1D* hele_pt_qed   = new TH1D("hele_pt_qed","",30,0,15);
  TH1D* hele_eta_qed  = new TH1D("hele_eta_qed","",25,-2.4,2.4);
  TH1D* hele_phi_qed  = new TH1D("hele_phi_qed","",16,-4,4);
  
  TH1D* hele_pt2_qed   = new TH1D("hele_pt2_qed","",30,0,15);
  TH1D* hele_eta2_qed  = new TH1D("hele_eta2_qed","",25,-2.4,2.4);
  TH1D* hele_phi2_qed  = new TH1D("hele_phi2_qed","",16,-4,4);
  
  TH1D* hdpt_qed   = new TH1D("hdpt_qed","",10,0,5);
  TH1D* hdeta_qed  = new TH1D("hdeta_qed","",6,0,2.4);
  TH1D* hdphi_qed  = new TH1D("hdphi_qed","",20,2.65,3.15);

  TH1D* hdiele_pt_qed         = new TH1D("hdiele_pt_qed","",30,0,5);
  TH1D* hdiele_rapidity_qed   = new TH1D("hdiele_rapidity_qed","",25,-2.4,2.4);
  TH1D* hinvmass_qed          = new TH1D("hinvmass_qed","",25,0,50);
  TH1D* hinvmass_qed_fine     = new TH1D("hinvmass_qed_fine","",1000,0,100);
  
  cout << "qed project ongoing" << endl;
  
  qed->Project(hele_pt_qed->GetName(), "elegsfTrkPt_1",ele_id_qed);
  qed->Project(hele_eta_qed->GetName(), "elegsfTrkEta_1",ele_id_qed);
  qed->Project(hele_phi_qed->GetName(), "elegsfTrkPhi_1",ele_id_qed);
  qed->Project(hele_pt2_qed->GetName(), "elegsfTrkPt_2",ele_id_qed);
  qed->Project(hele_eta2_qed->GetName(), "elegsfTrkEta_2",ele_id_qed);
  qed->Project(hele_phi2_qed->GetName(), "elegsfTrkPhi_2",ele_id_qed);

  hele_pt_qed->Add(hele_pt2_qed);
  hele_eta_qed->Add(hele_eta2_qed);
  hele_phi_qed->Add(hele_phi2_qed);
  
  qed->Project(hdiele_pt_qed->GetName(), "vSum_Pt",ele_id_qed);
  qed->Project(hdiele_rapidity_qed->GetName(), "vSum_Rapidity",ele_id_qed);
  qed->Project(hinvmass_qed->GetName(), "vSum_M",ele_id_qed);
  qed->Project(hinvmass_qed_fine->GetName(), "vSum_M",ele_id_qed);
  
  double lumiNorm = 1.01*0.8775;
  //double lumiNorm = 1.01;
  
  hele_pt_qed->Scale(lumiNorm);   hele_eta_qed->Scale(lumiNorm);   hele_phi_qed->Scale(lumiNorm);     
  hdiele_pt_qed->Scale(lumiNorm);   hdiele_rapidity_qed->Scale(lumiNorm);     hinvmass_qed->Scale(lumiNorm);   
  hinvmass_qed_fine->Scale(lumiNorm); 
  
  
  /*hele_pt_qed->Scale(0.14);   hele_eta_qed->Scale(0.14);   hele_phi_qed->Scale(0.14);     
    hdiele_pt_qed->Scale(0.14);   hdiele_rapidity_qed->Scale(0.14);     hinvmass_qed->Scale(0.14);*/    
  cout << "qed project finished" << endl;
  
  
  // ratio qed by data
  TH1D* hele_pt_ratio   = new TH1D("hele_pt_ratio","",30,0,15);
  TH1D* hele_eta_ratio  = new TH1D("hele_eta_ratio","",25,-2.4,2.4);
  TH1D* hele_phi_ratio  = new TH1D("hele_phi_ratio","",16,-4,4);
  
  TH1D* hdiele_pt_ratio         = new TH1D("hdiele_pt_ratio","",30,0,5);
  TH1D* hdiele_rapidity_ratio   = new TH1D("hdiele_rapidity_ratio","",25,-2.4,2.4);
  TH1D* hinvmass_ratio          = new TH1D("hinvmass_ratio","",25,0,50);
  TH1D* hinvmass_ratio_fine     = new TH1D("hinvmass_ratio_fine","",1000,0,100);
  
  hele_pt_ratio->Divide(hele_pt_qed,hele_pt_data);
  hele_eta_ratio->Divide(hele_eta_qed,hele_eta_data);
  hele_phi_ratio->Divide(hele_phi_qed,hele_phi_data);
  
  hdiele_pt_ratio->Divide(hdiele_pt_qed,hdiele_pt_data);
  hdiele_rapidity_ratio->Divide(hdiele_rapidity_qed,hdiele_rapidity_data);
  hinvmass_ratio->Divide(hinvmass_qed,hinvmass_data);
  hinvmass_ratio_fine->Divide(hinvmass_qed_fine,hinvmass_data_fine);
  
  
  const TCut ele_extrk   = ele_gsf_pt && "vSum_M > 4" && gsf_miss_hit && calocut && "vSum_Pt < 2.0 && ele_aco < 0.01";  
  const TCut ele_aco     = ele_gsf_pt && "vSum_M > 4" && gsf_miss_hit && no_extrk && calocut && "vSum_Pt < 2.0";  
  const TCut ele_sumpt   = ele_gsf_pt && "vSum_M > 4" && gsf_miss_hit && no_extrk && calocut && "ele_aco < 0.01";  

  /// N-1 plots
  
  TH1D* hextra_ntrk_data     = new TH1D("hextra_ntrk_data","",10,0,10);
  TH1D* hextra_ntrk_qed      = new TH1D("hextra_ntrk_qed","",10,0,10);
  TH1D* hacoplanarity_data   = new TH1D("hacoplanarity_data","",40,0,0.2);
  TH1D* hacoplanarity_qed    = new TH1D("hacoplanarity_qed","",40,0,0.2);

  TH1D* hsumpt_data          = new TH1D("hsumpt_data","",60,0,30);
  TH1D* hsumpt_qed           = new TH1D("hsumpt_qed" ,"",60,0,30);

  data->Project(hextra_ntrk_data->GetName(), "nextra_track",ele_extrk);
  qed->Project(hextra_ntrk_qed->GetName(),   "nextra_track",ele_extrk);


  data->Project(hacoplanarity_data->GetName(), "ele_aco",ele_aco);
  qed->Project(hacoplanarity_qed->GetName(),   "ele_aco",ele_aco);

  data->Project(hsumpt_data->GetName(), "vSum_Pt",ele_sumpt);
  qed->Project(hsumpt_qed->GetName(), "vSum_Pt",ele_sumpt);

  hextra_ntrk_qed->Scale(lumiNorm);
  hacoplanarity_qed->Scale(lumiNorm);
  hsumpt_qed->Scale(lumiNorm);


  
  int W = 700;
  int H = 600;
  
  float T = 0.08;
  float B = 0.14; 
  float L = 0.14;
  float R = 0.04;
  
  
  TCanvas* cc1 = new TCanvas("hybrid_Sum_pt","dielectron pT",254,411,639,592);
  make_canvas(cc1);
  PlotHistsAndRatio(cc1, hdiele_pt_qed, hdiele_pt_data, 0.0,2.0,0,4000,0.7,1.3,"Dielectron p_{T} (GeV)");

  TCanvas* c2 = new TCanvas("hybrid_Inv_mass","Invmass",254,411,639,592);
  make_canvas(c2);
  PlotHistsAndRatio(c2, hinvmass_qed, hinvmass_data, 0.0,50.0,0,2500,0.6,1.4,"Invariant Mass (GeV)");

  TCanvas* c3 = new TCanvas("hybrid_Rapidity","rapidity",254,411,639,592);
  make_canvas(c3);
  PlotHistsAndRatio(c3, hdiele_rapidity_qed, hdiele_rapidity_data, -2.4,2.4,0,1500,0.7,1.3,"Dielectron y");

  TCanvas* c4 = new TCanvas("hybrid_Electron_pt","Electron pT",254,411,639,592);
  make_canvas(c4);
  PlotHistsAndRatio(c4, hele_pt_qed, hele_pt_data, 0,15,0,4000,0.7,1.3,"Electron p_{T} (Gev)");

  TCanvas* c5 = new TCanvas("hybrid_Electron_eta","Electron eta",254,411,639,592);
  make_canvas(c5);
  PlotHistsAndRatio(c5, hele_eta_qed, hele_eta_data, -2.4,2.4,0,2500,0.7,1.3,"Electron #eta");

  TCanvas* c6 = new TCanvas("hybrid_Electron_phi","Electron phi",254,411,639,592);
  make_canvas(c6);
  PlotHistsAndRatio(c6, hele_phi_qed, hele_phi_data, -4,4,0,3000,0.7,1.3,"Electron #phi");



  TLegend *leg1=new TLegend(0.75,0.70,0.98,0.87);
  leg1->SetFillColor(0);
  leg1->SetBorderSize(0);
  leg1->SetFillStyle(0);
  leg1->SetTextFont(43);
  leg1->SetTextSize(24);
  leg1->AddEntry(hextra_ntrk_data,"Data ","pl");
  leg1->AddEntry(hextra_ntrk_qed,"MC","f");


  TCanvas* c8 = new TCanvas("c8","extra track n-1",50,50,W,H);
  make_canvas(c8);
  c8->cd();
  gPad->SetLogy();
  hextra_ntrk_qed->GetYaxis()->SetRangeUser(0.1,30000);
  make_hist(hextra_ntrk_qed, kYellow, 21);
  hextra_ntrk_qed->Draw("hist");   
  hextra_ntrk_qed->GetXaxis()->SetTitle("No. extra track");
  hextra_ntrk_qed->GetYaxis()->SetTitle("Events");

  make_hist(hextra_ntrk_data, kBlack, 21);
  hextra_ntrk_data->SetLineColor(kBlack);
  hextra_ntrk_data->Draw("psames");
  leg1->Draw();
  drawText("HM algo.",0.21,0.86,25);     
  //CMS_lumi( c8, 104, 10,lumi_PbPb2015 );
  c8->Print("./fig_hybrid/extra_track.gif");
  c8->Print("./fig_hybrid/extra_track.pdf");


  TCanvas* c9 = new TCanvas("c9","dielectron pT n-1",50,50,W,H);
  make_canvas(c9);
  c9->cd();
  gPad->SetLogy();
  hsumpt_qed->GetYaxis()->SetRangeUser(0.1,20000);
  make_hist(hsumpt_qed, kYellow, 21);
  hsumpt_qed->Draw("hist");   
  hsumpt_qed->GetXaxis()->SetTitle("Dielectron p_{T}");
  hsumpt_qed->GetYaxis()->SetTitle("Events");

  make_hist(hsumpt_data, kBlack, 21);
  hsumpt_data->SetLineColor(kBlack);
  hsumpt_data->Draw("psames");
  leg1->Draw();
  drawText("HM algo.",0.21,0.86,25);     
  //CMS_lumi( c9, 104, 10,lumi_PbPb2015 );
  c9->Print("./fig_hybrid/sumpt_n-1.gif");
  c9->Print("./fig_hybrid/sumpt_n-1.pdf");


  /// dielectron acoplanarity ...........................................
  TCanvas* c10 = new TCanvas("c10","acoplanarity n-1",50,50,W,H);
  make_canvas(c10);
  c10->cd();
  gPad->SetLogy();
  hacoplanarity_qed->SetMinimum(0.1);  
  make_hist(hacoplanarity_qed, kYellow, 21);
  hacoplanarity_qed->Draw("hist");   
  hacoplanarity_qed->GetXaxis()->SetTitle("e^{+}e^{-} acoplanarity");//,"dN/dp_{T}^{2} (1/GeV)^{2} 
  hacoplanarity_qed->GetXaxis()->SetNdivisions(509);
  hacoplanarity_qed->GetYaxis()->SetTitle("Events");

  make_hist(hacoplanarity_data, kBlack, 21);
  hacoplanarity_data->SetLineColor(kBlack);
  hacoplanarity_data->Draw("psame");
    
  leg1->Draw();
  drawText("HM algo.",0.21,0.86,25);     
  //CMS_lumi( c10, 104, 10,lumi_PbPb2015 );
  c10->Print("./fig_hybrid/acoplanarity_hybrid.gif");
  c10->Print("./fig_hybrid/acoplanarity_hybrid.pdf");


   // cout << " Max: " << hinvmass_data->GetXaxis()->GetXmax() << "   Min:" << hinvmass_data->GetXaxis()->GetXmin() << endl;
   cout << " invariant mass data: " << hinvmass_data->Integral() << endl;  
   cout << " invariant mass qed: " << hinvmass_qed->Integral() << endl;  
  
}


TCanvas* PlotHistsAndRatio(TCanvas* c1, TH1D* hmc, TH1D* hdata, double hxmin, double hxmax, int hymin, int hymax, double rymin , double rymax, const char *ytitle){
  
  float T = 0.08;
  float B = 0.14; 
  float L = 0.14;
  float R = 0.04;
  
  TString strRatio=hmc->GetName();
  strRatio+="_over_";
  strRatio+=hmc->GetName();
  
  TH1D* hratio=(TH1D*)hdata->Clone(strRatio);
  hratio->Sumw2();  
  
  hratio->Divide(hmc);
  
  
  c1->Divide(1,2);
  TPad* pad1 = (TPad*)c1->GetPad(1);
  TPad* pad2 = (TPad*)c1->GetPad(2);
  pad1->SetPad(0,0.3,1.0,1.0);
  pad2->SetPad(0,0,  1.0,0.28);
  pad1->SetLeftMargin(0.18);
  pad1->SetTopMargin(0.08);
  pad1->SetRightMargin(0.07);
  pad1->SetBottomMargin(0.01); // All X axis labels and titles are thus cut off
  pad2->SetLeftMargin(0.18);
  pad2->SetTopMargin(0.01);
  pad2->SetRightMargin(0.07);
  pad2->SetBottomMargin(0.45);
  
  pad1->cd();
  
  hdata->SetTitle("");
  hmc->SetTitle("");
  hmc->GetXaxis()->SetRangeUser(hxmin,hxmax);
  hmc->GetYaxis()->SetRangeUser(hymin,hymax);
  hmc->SetLabelColor(1, "XYZ");
  hmc->SetLabelFont(42, "XYZ");
  hmc->SetLabelOffset(0.007, "XYZ");
  hmc->SetLabelSize(0.06, "XYZ");
  hmc->GetYaxis()->SetTitleSize(0.07);
  hmc->GetYaxis()->SetTitle("Events");
  
  //hmc->GetXaxis()->SetRangeUser(0,2);
  hmc->SetFillColor(kYellow);
  hmc->Draw("hist");
  
  hdata->SetMarkerColor(kBlack);
  hdata->SetLineColor(kBlack);
  hdata->SetMarkerStyle(20);
  hdata->Draw("psame");
  drawText("HM algo.",0.21,0.86,25);     
  
  TLegend *leg2=new TLegend(0.75,0.70,0.98,0.87);
  leg2->SetFillColor(0);
  leg2->SetBorderSize(0);
  leg2->SetFillStyle(0);
  leg2->SetTextFont(43);
  leg2->SetTextSize(24);
  leg2->AddEntry(hdata,"Data ","pl");
  leg2->AddEntry(hmc,"MC","f");
  
  leg2->Draw();
  
  pad2->cd();
  pad2->SetGridy(1);
  hratio->SetLineWidth(2);
  //hratio->SetStats(0);
  hratio->GetXaxis()->SetRangeUser(hxmin,hxmax);
  hratio->GetYaxis()->SetRangeUser(rymin,rymax);
  //hratio->GetXaxis()->SetTitle("Dielectron p_{T}");
  hratio->GetXaxis()->SetTitle(ytitle);
  hratio->GetYaxis()->SetTitle("Data/MC");
  hratio->GetYaxis()->SetNdivisions(504);
  hratio->GetYaxis()->SetLabelSize(0.11);
  hratio->GetXaxis()->SetLabelSize(0.15);
  hratio->GetXaxis()->SetTitleSize(0.18);
  hratio->GetYaxis()->SetTitleSize(0.15);
  hratio->GetXaxis()->SetTitleOffset(1.1);
  hratio->GetYaxis()->SetTitleOffset(0.31);
  hratio->GetXaxis()->SetTickLength(0.09);
  hratio->Draw();
  
  double xmin = hmc->GetXaxis()->GetXmin();
  double xmax = hmc->GetXaxis()->GetXmax();

  TLine *l1 = new TLine(hxmin,1,hxmax,1);
  l1->SetLineColor(kRed);
  l1->Draw();



  TString cName=c1->GetName();
  cName+=".png";
  c1->SaveAs(cName);
  TString c2Name=c1->GetName();
  c2Name+=".pdf";
  c1->SaveAs(c2Name);
  return c1;
}



void make_canvas(TCanvas *& canvas){

  int W = 600;
  int H = 670;
  float T = 0.08;
  float B = 0.14; 
  float L = 0.18;
  float R = 0.04;

  canvas->SetFillColor(0);
  canvas->SetBorderMode(0);
  canvas->SetFrameFillStyle(0);
  canvas->SetFrameBorderMode(0);
  canvas->SetLeftMargin( L );
  canvas->SetRightMargin( R );
  canvas->SetTopMargin( T );
  canvas->SetBottomMargin( B );
  canvas->SetTickx(0);
  canvas->SetTicky(0);
}

void make_canvas_ratio(TCanvas *& can){

  int W = 700;
  int H = 600;
  float T = 0.08;
  float B = 0.14; 
  float L = 0.14;
  float R = 0.04;

  can->SetFillColor(0);
  can->SetBorderMode(0);
  can->SetFrameFillStyle(0);
  can->SetFrameBorderMode(0);
  can->SetLeftMargin( L );
  can->SetRightMargin( R );
  can->SetTopMargin( T );
  can->SetBottomMargin( B );
  can->SetTickx(0);
  can->SetTicky(0);
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

void make_hist_ratio(TH1D *& hist, Color_t kcolor, int kstyle){
  // For the axis labels:
  hist->SetLabelColor(1, "XYZ");
  hist->SetLabelFont(42, "XYZ");
  hist->SetLabelOffset(0.007, "XYZ");
  hist->SetLabelSize(0.1, "XYZ");

  // For the axis titles:
  hist->SetTitleFont(42, "XYZ");
  hist->SetTitleSize(0.11, "XYZ");
  hist->GetXaxis()->SetTitleOffset(1.1);
  hist->GetYaxis()->SetTitleOffset(0.4);
  hist->SetMarkerColor(kcolor);
  hist->SetFillColor(kcolor);
  hist->SetMarkerStyle(kstyle);
 
}

