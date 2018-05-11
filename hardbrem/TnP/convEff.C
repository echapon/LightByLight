void convEff() {
   TFile *fdata = TFile::Open("outputDataAll.root");
   TFile *fmc = TFile::Open("outputMCAll.root");

   TTree *trdata = (TTree*) fdata->Get("tpTree_conv/fitter_tree");
   TTree *trmc = (TTree*) fmc->Get("tpTree_conv/fitter_tree");

   nbins = 4;
   double bins[5] = {2,4,6,8,20};

   // book histos
   TH1F *haxes_tk = new TH1F("haxes_tk",";photon E_{T} [GeV];track finding prob.",1,0,20);
   TH1F *haxes_el = new TH1F("haxes_el",";photon E_{T} [GeV];electron finding prob.",1,0,20);
   TH1F *hnum_tk_data = new TH1F("hnum_tk_data",";photon E_{T} [GeV];track finding prob.",nbins,bins);
   TH1F *hden_data = new TH1F("hden_data",";photon E_{T} [GeV];track finding prob.",nbins,bins);
   TH1F *hnum_el_data = new TH1F("hnum_el_data",";photon E_{T} [GeV];electron finding prob.",nbins,bins);
   TH1F *hnum_tk_mc = new TH1F("hnum_tk_mc",";photon E_{T} [GeV];track finding prob.",nbins,bins);
   TH1F *hden_mc = new TH1F("hden_mc",";photon E_{T} [GeV];track finding prob.",nbins,bins);
   TH1F *hnum_el_mc = new TH1F("hnum_el_mc",";photon E_{T} [GeV];electron finding prob.",nbins,bins);

   // fill them
   trdata->Project("hnum_tk_data","phoSCEt_notag","nmatchtrk>0&&abs(elePt-probetkPt-phoSCEt_notag)<2&&phoSCEt_notag>2&&probetkPt<2&&(tagCharge!=probetkCharge)&&probetkAco<0.1&&phoAco<0.1");
   trdata->Project("hnum_el_data","phoSCEt_notag","nmatchele>0&&abs(elePt-probetkPt-phoSCEt_notag)<2&&phoSCEt_notag>2&&probetkPt<2&&(tagCharge!=probetkCharge)&&probetkAco<0.1&&phoAco<0.1");
   trdata->Project("hden_data","phoSCEt_notag","abs(elePt-probetkPt-phoSCEt_notag)<2&&phoSCEt_notag>2&&probetkPt<2&&(tagCharge!=probetkCharge)&&probetkAco<0.1&&phoAco<0.1");
   trmc->Project("hnum_tk_mc","phoSCEt_notag","nmatchtrk>0&&abs(elePt-probetkPt-phoSCEt_notag)<2&&phoSCEt_notag>2&&probetkPt<2&&(tagCharge!=probetkCharge)&&probetkAco<0.1&&phoAco<0.1");
   trmc->Project("hnum_el_mc","phoSCEt_notag","nmatchele>0&&abs(elePt-probetkPt-phoSCEt_notag)<2&&phoSCEt_notag>2&&probetkPt<2&&(tagCharge!=probetkCharge)&&probetkAco<0.1&&phoAco<0.1");
   trmc->Project("hden_mc","phoSCEt_notag","abs(elePt-probetkPt-phoSCEt_notag)<2&&phoSCEt_notag>2&&probetkPt<2&&(tagCharge!=probetkCharge)&&probetkAco<0.1&&phoAco<0.1");

   double efftkdata =  hnum_tk_data->GetEntries() / hden_data->GetEntries();
   double efftkdataup = TEfficiency::ClopperPearson(hden_data->GetEntries(), hnum_tk_data->GetEntries(), 0.683, true)-efftkdata;
   double efftkdatadown = TEfficiency::ClopperPearson(hden_data->GetEntries(), hnum_tk_data->GetEntries(), 0.683, false)-efftkdata;
   double effeldata =  hnum_el_data->GetEntries() / hden_data->GetEntries();
   double effeldataup = TEfficiency::ClopperPearson(hden_data->GetEntries(), hnum_el_data->GetEntries(), 0.683, true)-effeldata;
   double effeldatadown = TEfficiency::ClopperPearson(hden_data->GetEntries(), hnum_el_data->GetEntries(), 0.683, false)-effeldata;
   double efftkmc =  hnum_tk_mc->GetEntries() / hden_mc->GetEntries();
   double efftkmcup = TEfficiency::ClopperPearson(hden_mc->GetEntries(), hnum_tk_mc->GetEntries(), 0.683, true)-efftkmc;
   double efftkmcdown = TEfficiency::ClopperPearson(hden_mc->GetEntries(), hnum_tk_mc->GetEntries(), 0.683, false)-efftkmc;
   double effelmc =  hnum_el_mc->GetEntries() / hden_mc->GetEntries();
   double effelmcup = TEfficiency::ClopperPearson(hden_mc->GetEntries(), hnum_el_mc->GetEntries(), 0.683, true)-effelmc;
   double effelmcdown = TEfficiency::ClopperPearson(hden_mc->GetEntries(), hnum_el_mc->GetEntries(), 0.683, false)-effelmc;

   cout << hnum_tk_data->GetEntries() << "/" << hden_data->GetEntries() << " = " << efftkdata << 
      " +" << efftkdataup <<
      " " << efftkdatadown << endl;
   cout << hnum_el_data->GetEntries() << "/" << hden_data->GetEntries() << " = " << effeldata << 
      " +" << effeldataup <<
      " " << effeldatadown << endl;
   cout << hnum_tk_mc->GetEntries() << "/" << hden_mc->GetEntries() << " = " << efftkmc << 
      " +" << efftkmcup <<
      " " << efftkmcdown << endl;
   cout << hnum_el_mc->GetEntries() << "/" << hden_mc->GetEntries() << " = " << effelmc << 
      " +" << effelmcup <<
      " " << effelmcdown << endl;

   // efficiencies
   TGraphAsymmErrors *gtkdata = new TGraphAsymmErrors(hnum_tk_data,hden_data);
   TGraphAsymmErrors *geldata = new TGraphAsymmErrors(hnum_el_data,hden_data);
   TGraphAsymmErrors *gtkmc = new TGraphAsymmErrors(hnum_tk_mc,hden_mc);
   TGraphAsymmErrors *gelmc = new TGraphAsymmErrors(hnum_el_mc,hden_mc);
   gtkdata->SetMarkerColor(kBlue);
   gtkdata->SetLineColor(kBlue);
   gtkmc->SetMarkerColor(kRed);
   gtkmc->SetLineColor(kRed);
   geldata->SetMarkerColor(kBlue);
   geldata->SetLineColor(kBlue);
   gelmc->SetMarkerColor(kRed);
   gelmc->SetLineColor(kRed);

   TLegend *tleg = new TLegend(0.6,0.7,0.9,0.9);
   tleg->SetBorderSize(0);
   tleg->AddEntry(gtkdata,Form("Data: %.2f^{+%.2f}_{%.2f}",efftkdata,efftkdataup,efftkdatadown),"LP");
   tleg->AddEntry(gtkmc,Form("MC: %.2f^{+%.2f}_{%.2f}",efftkmc,efftkmcup,efftkmcdown),"LP");

   TCanvas *c1 = new TCanvas();
   haxes_tk->Draw();
   haxes_tk->GetYaxis()->SetRangeUser(0,1);
   c1->RedrawAxis();
   gtkdata->Draw("P");
   gtkmc->Draw("P");
   tleg->Draw();
   c1->SaveAs("convtk.pdf");
   c1->SaveAs("convtk.C");


   TLegend *tleg2 = new TLegend(0.6,0.7,0.9,0.9);
   tleg2->SetBorderSize(0);
   tleg2->AddEntry(geldata,Form("Data: %.2f^{+%.2f}_{%.2f}",effeldata,effeldataup,effeldatadown),"LP");
   tleg2->AddEntry(gelmc,Form("MC: %.2f^{+%.2f}_{%.2f}",effelmc,effelmcup,effelmcdown),"LP");

   haxes_el->Draw();
   haxes_el->GetYaxis()->SetRangeUser(0,1);
   c1->RedrawAxis();
   geldata->Draw("P");
   gelmc->Draw("P");
   tleg2->Draw();
   c1->SaveAs("convel.pdf");
   c1->SaveAs("convel.C");
}
