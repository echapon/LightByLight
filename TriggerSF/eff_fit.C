void eff_fit()
{
//=========Macro generated from canvas: c1/c1
//=========  (Fri May  4 17:24:03 2018) by ROOT version6.06/08
   TCanvas *c1 = new TCanvas("c1", "c1",16,47,700,500);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   c1->Range(-6.8,-0.196875,33.2,1.115625);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(0);
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
   
   TH1F *h__1 = new TH1F("h__1","",1,0,30);
   h__1->SetLineStyle(0);
   h__1->SetMarkerStyle(20);
   h__1->GetXaxis()->SetTitle("m (#gamma#gamma) [GeV]");
   h__1->GetXaxis()->SetLabelFont(42);
   h__1->GetXaxis()->SetLabelOffset(0.01);
   h__1->GetXaxis()->SetLabelSize(0.045);
   h__1->GetXaxis()->SetTitleSize(0.055);
   h__1->GetXaxis()->SetTitleFont(42);
   h__1->GetYaxis()->SetTitle("Efficiency");
   h__1->GetYaxis()->SetLabelFont(42);
   h__1->GetYaxis()->SetLabelOffset(0.01);
   h__1->GetYaxis()->SetLabelSize(0.045);
   h__1->GetYaxis()->SetTitleSize(0.055);
   h__1->GetYaxis()->SetTitleOffset(1.3);
   h__1->GetYaxis()->SetTitleFont(42);
   h__1->GetZaxis()->SetLabelFont(42);
   h__1->GetZaxis()->SetLabelSize(0.045);
   h__1->GetZaxis()->SetTitleFont(42);
   h__1->Draw("");
   
   Double_t divide_hnum_by_hden_fx3001[8] = {
   5.5,
   6.5,
   7.5,
   9,
   11,
   13.5,
   17.5,
   25};
   Double_t divide_hnum_by_hden_fy3001[8] = {
   0.1052847,
   0.2038339,
   0.2415966,
   0.2717014,
   0.2884283,
   0.3179348,
   0.3666667,
   0.3097345};
   Double_t divide_hnum_by_hden_felx3001[8] = {
   0.5,
   0.5,
   0.5,
   1,
   1,
   1.5,
   2.5,
   5};
   Double_t divide_hnum_by_hden_fely3001[8] = {
   0.00625002,
   0.01030526,
   0.01411082,
   0.01333045,
   0.01929372,
   0.0250787,
   0.03252794,
   0.04586656};
   Double_t divide_hnum_by_hden_fehx3001[8] = {
   0.5,
   0.5,
   0.5,
   1,
   1,
   1.5,
   2.5,
   5};
   Double_t divide_hnum_by_hden_fehy3001[8] = {
   0.006584014,
   0.01069547,
   0.01467388,
   0.01373972,
   0.02005725,
   0.02612179,
   0.03371035,
   0.04955542};
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(8,divide_hnum_by_hden_fx3001,divide_hnum_by_hden_fy3001,divide_hnum_by_hden_felx3001,divide_hnum_by_hden_fehx3001,divide_hnum_by_hden_fely3001,divide_hnum_by_hden_fehy3001);
   grae->SetName("divide_hnum_by_hden");
   grae->SetTitle("");
   grae->SetLineStyle(0);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_divide_hnum_by_hden3001 = new TH1F("Graph_divide_hnum_by_hden3001","",100,2.5,32.5);
   Graph_divide_hnum_by_hden3001->SetMinimum(0);
   Graph_divide_hnum_by_hden3001->SetMaximum(1);
   Graph_divide_hnum_by_hden3001->SetDirectory(0);
   Graph_divide_hnum_by_hden3001->SetStats(0);
   Graph_divide_hnum_by_hden3001->SetLineStyle(0);
   Graph_divide_hnum_by_hden3001->SetMarkerStyle(20);
   Graph_divide_hnum_by_hden3001->GetXaxis()->SetRange(0,92);
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
   TText *AText = ptstats->AddText("#chi^{2} / ndf = 2.719 / 5");
   AText = ptstats->AddText("Prob  = 0.7432");
   AText = ptstats->AddText("p0       = 0.3324 #pm 0.02476 ");
   AText = ptstats->AddText("p1       = 4.289 #pm 0.6563 ");
   AText = ptstats->AddText("p2       = -2.649 #pm 0.966 ");
   ptstats->SetOptStat(0);
   ptstats->SetOptFit(1111);
   ptstats->Draw();
   grae->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(grae->GetListOfFunctions());
   
   TF1 *f3003 = new TF1("f","[0]*(1-exp((x-[1])/[2]))",2.2,30.1);
   f3003->SetFillColor(19);
   f3003->SetFillStyle(0);
   f3003->SetMarkerStyle(20);
   f3003->SetLineWidth(3);
   f3003->SetChisquare(2.718811);
   f3003->SetNDF(5);
   f3003->GetXaxis()->SetLabelFont(42);
   f3003->GetXaxis()->SetLabelOffset(0.01);
   f3003->GetXaxis()->SetLabelSize(0.045);
   f3003->GetXaxis()->SetTitleSize(0.055);
   f3003->GetXaxis()->SetTitleFont(42);
   f3003->GetYaxis()->SetLabelFont(42);
   f3003->GetYaxis()->SetLabelOffset(0.01);
   f3003->GetYaxis()->SetLabelSize(0.045);
   f3003->GetYaxis()->SetTitleSize(0.055);
   f3003->GetYaxis()->SetTitleOffset(1.3);
   f3003->GetYaxis()->SetTitleFont(42);
   f3003->SetParameter(0,0.3324057);
   f3003->SetParError(0,0.02476384);
   f3003->SetParLimits(0,0,0);
   f3003->SetParameter(1,4.289039);
   f3003->SetParError(1,0.6563358);
   f3003->SetParLimits(1,0,0);
   f3003->SetParameter(2,-2.648502);
   f3003->SetParError(2,0.9660018);
   f3003->SetParLimits(2,0,0);
   grae->GetListOfFunctions()->Add(f3003);
   grae->Draw("p");
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
