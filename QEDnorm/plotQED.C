#include "CMS_lumi.C"

const double xsec_3_53         = 1.086453e-01*20.6e3; // in mub
const double sf                = 0.98*0.98;
const int    ngen              = 2399759;//7929199;

void plotQED(TString algo="GED", double acop_cut=0.01, double luminosity=391, double mass_cut=0) {
   TFile *fdata = TFile::Open("outputDataAll.root");
   TTree *trdata = (TTree*) fdata->Get("tr" + algo);
   // TFile *fMC = TFile::Open("outputMCAll.root");
   TFile *fMC = TFile::Open("outputMCAll_noexcl.root");
   TTree *trMC = (TTree*) fMC->Get("tr" + algo);

   TH1F *hmass_data = new TH1F("hmass_data",";M (e^{+}e^{-}) [GeV];Entries / (2 GeV)",50,0,100);
   TH1F *hdeltapt_data = new TH1F("hdeltapt_data",";#Delta p_{T} (e^{+}e^{-}) [GeV];Entries / (1 GeV)",10,0,1);
   TH1F *hrap_data = new TH1F("hrap_data",";y (e^{+}e^{-});Entries / (0.2)",25,-2.5,2.5);
   TH1F *hpt_data = new TH1F("hpt_data",";p_{T} (e^{+}e^{-}) [GeV];Entries / (0.5 GeV)",20,0,1);
   TH1F *hacop_data = new TH1F("hacop_data",Form(";e^{+}e^{-} acoplanarity;Entries / (%.4f)",acop_cut/20.),20,0,acop_cut);

   TH1F *hmass_MC = new TH1F("hmass_MC",";M (e^{+}e^{-}) [GeV];Entries",50,0,100);
   TH1F *hdeltapt_MC = new TH1F("hdeltapt_MC",";#Delta p_{T} (e^{+}e^{-}) [GeV];Entries",10,0,1);
   TH1F *hrap_MC = new TH1F("hrap_MC",";y ();Entries",25,-2.5,2.5);
   TH1F *hpt_MC = new TH1F("hpt_MC",";p_{T} (e^{+}2^{-}) [GeV];Entries",20,0,1);
   TH1F *hacop_MC = new TH1F("hacop_MC",";e{+}e^{-} acoplanarity;Entries",20,0,acop_cut);

   trdata->Project(hmass_data->GetName(),"mass",Form("doubleEG2&&acop<%f&&mass>=%f",acop_cut,mass_cut));
   trdata->Project(hdeltapt_data->GetName(),"deltapt",Form("doubleEG2&&acop<%f&&mass>=%f",acop_cut,mass_cut));
   trdata->Project(hrap_data->GetName(),"rap",Form("doubleEG2&&acop<%f&&mass>=%f",acop_cut,mass_cut));
   trdata->Project(hpt_data->GetName(),"pt",Form("doubleEG2&&acop<%f&&mass>=%f",acop_cut,mass_cut));
   trdata->Project(hacop_data->GetName(),"acop",Form("doubleEG2&&acop<%f&&mass>=%f",acop_cut,mass_cut));
   trMC->Project(hmass_MC->GetName(),"mass",Form("doubleEG2&&acop<%f&&mass>=%f",acop_cut,mass_cut));
   trMC->Project(hdeltapt_MC->GetName(),"deltapt",Form("doubleEG2&&acop<%f&&mass>=%f",acop_cut,mass_cut));
   trMC->Project(hrap_MC->GetName(),"rap",Form("doubleEG2&&acop<%f&&mass>=%f",acop_cut,mass_cut));
   trMC->Project(hpt_MC->GetName(),"pt",Form("doubleEG2&&acop<%f&&mass>=%f",acop_cut,mass_cut));
   trMC->Project(hacop_MC->GetName(),"acop",Form("doubleEG2&&acop<%f&&mass>=%f",acop_cut,mass_cut));

   // scale MC
   double xsec = xsec_3_53;
   hmass_MC->Scale(luminosity*xsec*sf/ngen); hmass_MC->SetLineColor(kBlack); hmass_MC->SetFillColor(kYellow);
   hdeltapt_MC->Scale(luminosity*xsec*sf/ngen); hdeltapt_MC->SetLineColor(kBlack); hdeltapt_MC->SetFillColor(kYellow);
   hrap_MC->Scale(luminosity*xsec*sf/ngen); hrap_MC->SetLineColor(kBlack); hrap_MC->SetFillColor(kYellow);
   hpt_MC->Scale(luminosity*xsec*sf/ngen); hpt_MC->SetLineColor(kBlack); hpt_MC->SetFillColor(kYellow);
   hacop_MC->Scale(luminosity*xsec*sf/ngen); hacop_MC->SetLineColor(kBlack); hacop_MC->SetFillColor(kYellow);

   int W = 700;
   int H = 600;

   float T = 0.08;
   float B = 0.14;
   float L = 0.14;
   float R = 0.04;


   TCanvas* c1 = new TCanvas("c1","Acoplanarity",50,50,W,H);
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
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(0);
   TLegend *tleg = new TLegend(0.5,0.6,0.9,0.9);
   tleg->SetBorderSize(0);
   tleg->AddEntry(hmass_data,"Data","lp");
   tleg->AddEntry(hmass_MC,"#gamma#gamma #rightarrow e^{+}e^{-} (MC)","f");

   c1->SetLogy();
   hmass_data->Draw();
   hmass_data->GetYaxis()->SetTitleOffset(1.17);
   hmass_MC->Draw("same hist");
   hmass_data->Draw("same");
   tleg->Draw();
   c1->RedrawAxis();
   CMS_lumi( c1, 104, 33,lumi_PbPb2015 );
   c1->SaveAs("mass.C");
   c1->SaveAs("mass.pdf");
   c1->SetLogy(false);

   hdeltapt_data->Draw();
   hdeltapt_data->GetYaxis()->SetTitleOffset(1.17);
   hdeltapt_MC->Draw("same hist");
   hdeltapt_data->Draw("same");
   tleg->Draw();
   c1->RedrawAxis();
   CMS_lumi( c1, 104, 33,lumi_PbPb2015 );
   c1->SaveAs("deltapt.C");
   c1->SaveAs("deltapt.pdf");

   TLegend *tleg2 = new TLegend(0.16,0.6,0.56,0.9);
   tleg2->SetBorderSize(0);
   tleg2->AddEntry(hmass_data,"Data","lp");
   tleg2->AddEntry(hmass_MC,"#gamma#gamma #rightarrow e^{+}e^{-} (MC)","f");
   hrap_data->Draw();
   hrap_data->GetYaxis()->SetTitleOffset(1.17);
   hrap_data->GetYaxis()->SetRangeUser(0,1900);
   hrap_MC->Draw("same hist");
   hrap_data->Draw("same");
   tleg2->Draw();
   c1->RedrawAxis();
   CMS_lumi( c1, 104, 33,lumi_PbPb2015 );
   c1->SaveAs("rap.C");
   c1->SaveAs("rap.pdf");
   c1->SaveAs("rap.root");

   hpt_data->Draw();
   hpt_data->GetYaxis()->SetTitleOffset(1.17);
   hpt_MC->Draw("same hist");
   hpt_data->Draw("same");
   tleg->Draw();
   c1->RedrawAxis();
   CMS_lumi( c1, 104, 33,lumi_PbPb2015 );
   c1->SaveAs("pt.C");
   c1->SaveAs("pt.pdf");

   c1->SetLogy();
   hacop_data->Draw();
   hacop_data->GetYaxis()->SetTitleOffset(1.17);
   hacop_data->SetNdivisions(509);
   hacop_MC->Draw("same hist");
   hacop_data->Draw("same");
   tleg->Draw();
   c1->RedrawAxis();
   CMS_lumi( c1, 104, 33,lumi_PbPb2015 );
   c1->SaveAs("acop.C");
   c1->SaveAs("acop.pdf");
   c1->SaveAs("acop.root");
}
