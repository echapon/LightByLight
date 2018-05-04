void eff_fit()
{
//=========Macro generated from canvas: c1/c1
//=========  (Thu May  3 18:52:09 2018) by ROOT version6.06/08
   TCanvas *c1 = new TCanvas("c1", "c1",16,97,700,523);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   c1->ToggleEventStatus();
   c1->Range(0.2705489,-0.09375,2.521512,0.53125);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(0);
   c1->SetLogx();
   c1->SetTickx(1);
   c1->SetTicky(1);
   c1->SetLeftMargin(0.17);
   c1->SetRightMargin(0.08);
   c1->SetTopMargin(0.05);
   c1->SetBottomMargin(0.15);
   c1->SetFrameLineColor(0);
   c1->SetFrameBorderMode(0);
   c1->SetFrameLineColor(0);
   c1->SetFrameBorderMode(0);
   
   Double_t divide_hnum_by_hden_fx3001[7] = {
   6.25,
   8.75,
   11.25,
   13.75,
   16.25,
   18.75,
   110};
   Double_t divide_hnum_by_hden_fy3001[7] = {
   0.1567591,
   0.2742347,
   0.3016345,
   0.3284672,
   0.3797468,
   0.3170732,
   0.3489933};
   Double_t divide_hnum_by_hden_felx3001[7] = {
   1.25,
   1.25,
   1.25,
   1.25,
   1.25,
   1.25,
   90};
   Double_t divide_hnum_by_hden_fely3001[7] = {
   0.005427432,
   0.01143412,
   0.01811536,
   0.02947902,
   0.04081707,
   0.0546516,
   0.04116337};
   Double_t divide_hnum_by_hden_fehx3001[7] = {
   1.25,
   1.25,
   1.25,
   1.25,
   1.25,
   1.25,
   90};
   Double_t divide_hnum_by_hden_fehy3001[7] = {
   0.005581638,
   0.01173012,
   0.01872905,
   0.03080855,
   0.04245736,
   0.05960279,
   0.04335503};
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(7,divide_hnum_by_hden_fx3001,divide_hnum_by_hden_fy3001,divide_hnum_by_hden_felx3001,divide_hnum_by_hden_fehx3001,divide_hnum_by_hden_fely3001,divide_hnum_by_hden_fehy3001);
   grae->SetName("divide_hnum_by_hden");
   grae->SetTitle("");
   grae->SetLineStyle(0);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_divide_hnum_by_hden3001 = new TH1F("Graph_divide_hnum_by_hden3001","",100,4.5,219.5);
   Graph_divide_hnum_by_hden3001->SetMinimum(0);
   Graph_divide_hnum_by_hden3001->SetMaximum(0.5);
   Graph_divide_hnum_by_hden3001->SetDirectory(0);
   Graph_divide_hnum_by_hden3001->SetStats(0);
   Graph_divide_hnum_by_hden3001->SetLineStyle(0);
   Graph_divide_hnum_by_hden3001->SetMarkerStyle(20);
   Graph_divide_hnum_by_hden3001->GetXaxis()->SetTitle("M^{#gamma#gamma} [GeV]");
   Graph_divide_hnum_by_hden3001->GetXaxis()->SetLabelFont(42);
   Graph_divide_hnum_by_hden3001->GetXaxis()->SetLabelOffset(0.01);
   Graph_divide_hnum_by_hden3001->GetXaxis()->SetLabelSize(0.045);
   Graph_divide_hnum_by_hden3001->GetXaxis()->SetTitleSize(0.055);
   Graph_divide_hnum_by_hden3001->GetXaxis()->SetTitleFont(42);
   Graph_divide_hnum_by_hden3001->GetYaxis()->SetTitle("Efficiency");
   Graph_divide_hnum_by_hden3001->GetYaxis()->SetLabelFont(42);
   Graph_divide_hnum_by_hden3001->GetYaxis()->SetLabelOffset(0.01);
   Graph_divide_hnum_by_hden3001->GetYaxis()->SetLabelSize(0.045);
   Graph_divide_hnum_by_hden3001->GetYaxis()->SetTitleSize(0.055);
   Graph_divide_hnum_by_hden3001->GetYaxis()->SetTitleOffset(1.3);
   Graph_divide_hnum_by_hden3001->GetYaxis()->SetTitleFont(42);
   Graph_divide_hnum_by_hden3001->GetZaxis()->SetLabelFont(42);
   Graph_divide_hnum_by_hden3001->GetZaxis()->SetLabelSize(0.045);
   Graph_divide_hnum_by_hden3001->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_divide_hnum_by_hden3001);
   
   
   TPaveStats *ptstats = new TPaveStats(0.62,0.755,0.98,0.995,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   TText *AText = ptstats->AddText("#chi^{2} / ndf = 1.322 / 4");
   AText = ptstats->AddText("Prob  = 0.8576");
   AText = ptstats->AddText("p0       = 0.3538 #pm 0.03059 ");
   AText = ptstats->AddText("p1       = 4.197 #pm 1.855 ");
   AText = ptstats->AddText("p2       = -3.373 #pm 1.813 ");
   ptstats->SetOptStat(0);
   ptstats->SetOptFit(1111);
   ptstats->Draw();
   grae->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(grae->GetListOfFunctions());
   
   TF1 *PrevFitTMP3003 = new TF1("PrevFitTMP","[0]*(1-exp((x-[1])/[2]))",4.5,219.5);
   PrevFitTMP3003->SetFillColor(19);
   PrevFitTMP3003->SetFillStyle(0);
   PrevFitTMP3003->SetMarkerStyle(20);
   PrevFitTMP3003->SetLineWidth(3);
   PrevFitTMP3003->SetChisquare(1.322113);
   PrevFitTMP3003->SetNDF(4);
   PrevFitTMP3003->GetXaxis()->SetLabelFont(42);
   PrevFitTMP3003->GetXaxis()->SetLabelOffset(0.01);
   PrevFitTMP3003->GetXaxis()->SetLabelSize(0.045);
   PrevFitTMP3003->GetXaxis()->SetTitleSize(0.055);
   PrevFitTMP3003->GetXaxis()->SetTitleFont(42);
   PrevFitTMP3003->GetYaxis()->SetLabelFont(42);
   PrevFitTMP3003->GetYaxis()->SetLabelOffset(0.01);
   PrevFitTMP3003->GetYaxis()->SetLabelSize(0.045);
   PrevFitTMP3003->GetYaxis()->SetTitleSize(0.055);
   PrevFitTMP3003->GetYaxis()->SetTitleOffset(1.3);
   PrevFitTMP3003->GetYaxis()->SetTitleFont(42);
   PrevFitTMP3003->SetParameter(0,0.3538181);
   PrevFitTMP3003->SetParError(0,0.03059186);
   PrevFitTMP3003->SetParLimits(0,0,0);
   PrevFitTMP3003->SetParameter(1,4.196903);
   PrevFitTMP3003->SetParError(1,1.855059);
   PrevFitTMP3003->SetParLimits(1,0,0);
   PrevFitTMP3003->SetParameter(2,-3.372556);
   PrevFitTMP3003->SetParError(2,1.812515);
   PrevFitTMP3003->SetParLimits(2,0,0);
   grae->GetListOfFunctions()->Add(PrevFitTMP3003);
   grae->Draw("ap");
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
