const double xsec_3_53         = 1.086453e-01*20.6e3; // in mub
const double sf                = 0.98*0.98;
const int    ngen              = 2399759;//7929199;

void plotQED(TString algo="GED", double acop_cut=0.01, double luminosity=391, double mass_cut=0) {
   TFile *fdata = TFile::Open("outputDataAll.root");
   TTree *trdata = (TTree*) fdata->Get("tr" + algo);
   TFile *fMC = TFile::Open("outputMCAll.root");
   TTree *trMC = (TTree*) fMC->Get("tr" + algo);

   TH1F *hmass_data = new TH1F("hmass_data",";M (ee) [GeV];Entries / (2 GeV)",50,0,100);
   TH1F *hdeltapt_data = new TH1F("hdeltapt_data",";#Delta p_{T} (ee) [GeV];Entries / (1 GeV)",10,0,1);
   TH1F *hrap_data = new TH1F("hrap_data",";y (ee);Entries / (0.2)",25,-2.5,2.5);
   TH1F *hpt_data = new TH1F("hpt_data",";p_{T} (ee) [GeV];Entries / (0.5 GeV)",20,0,1);
   TH1F *hacop_data = new TH1F("hacop_data",Form(";Acoplanarity;Entries / (%.4f)",acop_cut/20.),20,0,acop_cut);

   TH1F *hmass_MC = new TH1F("hmass_MC",";M (ee) [GeV];Entries",50,0,100);
   TH1F *hdeltapt_MC = new TH1F("hdeltapt_MC",";#Delta p_{T} (ee) [GeV];Entries",10,0,1);
   TH1F *hrap_MC = new TH1F("hrap_MC",";y (ee);Entries",25,-2.5,2.5);
   TH1F *hpt_MC = new TH1F("hpt_MC",";p_{T} (ee) [GeV];Entries",20,0,1);
   TH1F *hacop_MC = new TH1F("hacop_MC",";Acoplanarity;Entries",20,0,acop_cut);

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
   hmass_MC->Scale(luminosity*xsec*sf/ngen); hmass_MC->SetLineColor(kBlack); hmass_MC->SetFillColor(kYellow+1);
   hdeltapt_MC->Scale(luminosity*xsec*sf/ngen); hdeltapt_MC->SetLineColor(kBlack); hdeltapt_MC->SetFillColor(kYellow+1);
   hrap_MC->Scale(luminosity*xsec*sf/ngen); hrap_MC->SetLineColor(kBlack); hrap_MC->SetFillColor(kYellow+1);
   hpt_MC->Scale(luminosity*xsec*sf/ngen); hpt_MC->SetLineColor(kBlack); hpt_MC->SetFillColor(kYellow+1);
   hacop_MC->Scale(luminosity*xsec*sf/ngen); hacop_MC->SetLineColor(kBlack); hacop_MC->SetFillColor(kYellow+1);

   TCanvas *c1 = new TCanvas();
   TLegend *tleg = new TLegend(0.7,0.7,0.9,0.9);
   tleg->SetBorderSize(0);
   tleg->AddEntry(hmass_data,"Data","lp");
   tleg->AddEntry(hmass_MC,"#gamma#gamma #rightarrow e^{+}e^{-}","f");

   c1->SetLogy();
   hmass_data->Draw();
   hmass_MC->Draw("same hist");
   hmass_data->Draw("same");
   tleg->Draw();
   c1->RedrawAxis();
   c1->SaveAs("mass.pdf");
   c1->SetLogy(false);

   hdeltapt_data->Draw();
   hdeltapt_MC->Draw("same hist");
   hdeltapt_data->Draw("same");
   tleg->Draw();
   c1->RedrawAxis();
   c1->SaveAs("deltapt.pdf");

   hrap_data->Draw();
   hrap_MC->Draw("same hist");
   hrap_data->Draw("same");
   tleg->Draw();
   c1->RedrawAxis();
   c1->SaveAs("rap.pdf");

   hpt_data->Draw();
   hpt_MC->Draw("same hist");
   hpt_data->Draw("same");
   tleg->Draw();
   c1->RedrawAxis();
   c1->SaveAs("pt.pdf");

   hacop_data->Draw();
   hacop_MC->Draw("same hist");
   hacop_data->Draw("same");
   tleg->Draw();
   c1->RedrawAxis();
   c1->SaveAs("acop.pdf");
}
