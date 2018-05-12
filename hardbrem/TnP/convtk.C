void convtk()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sat May 12 00:39:56 2018) by ROOT version6.06/08
   TCanvas *c1 = new TCanvas("c1", "c1",0,0,700,500);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   c1->Range(-4.533333,0.3875,22.13333,1.1375);
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
   
   TH1F *haxes_tk__1 = new TH1F("haxes_tk__1","",1,0,20);
   haxes_tk__1->SetMinimum(0.5);
   haxes_tk__1->SetMaximum(1.1);
   haxes_tk__1->SetLineStyle(0);
   haxes_tk__1->SetMarkerStyle(20);
   haxes_tk__1->GetXaxis()->SetTitle("photon E_{T} [GeV]");
   haxes_tk__1->GetXaxis()->SetLabelFont(42);
   haxes_tk__1->GetXaxis()->SetLabelOffset(0.01);
   haxes_tk__1->GetXaxis()->SetLabelSize(0.045);
   haxes_tk__1->GetXaxis()->SetTitleSize(0.055);
   haxes_tk__1->GetXaxis()->SetTitleFont(42);
   haxes_tk__1->GetYaxis()->SetTitle("no track prob.");
   haxes_tk__1->GetYaxis()->SetLabelFont(42);
   haxes_tk__1->GetYaxis()->SetLabelOffset(0.01);
   haxes_tk__1->GetYaxis()->SetLabelSize(0.045);
   haxes_tk__1->GetYaxis()->SetTitleSize(0.055);
   haxes_tk__1->GetYaxis()->SetTitleOffset(1.3);
   haxes_tk__1->GetYaxis()->SetTitleFont(42);
   haxes_tk__1->GetZaxis()->SetLabelFont(42);
   haxes_tk__1->GetZaxis()->SetLabelSize(0.045);
   haxes_tk__1->GetZaxis()->SetTitleFont(42);
   haxes_tk__1->Draw("");
   
   TH1F *haxes_tk_copy__2 = new TH1F("haxes_tk_copy__2","",1,0,20);
   haxes_tk_copy__2->SetMinimum(0.5);
   haxes_tk_copy__2->SetMaximum(1.1);
   haxes_tk_copy__2->SetDirectory(0);
   haxes_tk_copy__2->SetLineStyle(0);
   haxes_tk_copy__2->SetMarkerStyle(20);
   haxes_tk_copy__2->GetXaxis()->SetTitle("photon E_{T} [GeV]");
   haxes_tk_copy__2->GetXaxis()->SetLabelFont(42);
   haxes_tk_copy__2->GetXaxis()->SetLabelOffset(0.01);
   haxes_tk_copy__2->GetXaxis()->SetLabelSize(0.045);
   haxes_tk_copy__2->GetXaxis()->SetTitleSize(0.055);
   haxes_tk_copy__2->GetXaxis()->SetTitleFont(42);
   haxes_tk_copy__2->GetYaxis()->SetTitle("no track prob.");
   haxes_tk_copy__2->GetYaxis()->SetLabelFont(42);
   haxes_tk_copy__2->GetYaxis()->SetLabelOffset(0.01);
   haxes_tk_copy__2->GetYaxis()->SetLabelSize(0.045);
   haxes_tk_copy__2->GetYaxis()->SetTitleSize(0.055);
   haxes_tk_copy__2->GetYaxis()->SetTitleOffset(1.3);
   haxes_tk_copy__2->GetYaxis()->SetTitleFont(42);
   haxes_tk_copy__2->GetZaxis()->SetLabelFont(42);
   haxes_tk_copy__2->GetZaxis()->SetLabelSize(0.045);
   haxes_tk_copy__2->GetZaxis()->SetTitleFont(42);
   haxes_tk_copy__2->Draw("sameaxis");
   
   Double_t divide_hnum_tk_data_by_hden_data_fx3001[4] = {
   3,
   5,
   7,
   14};
   Double_t divide_hnum_tk_data_by_hden_data_fy3001[4] = {
   0.9109589,
   0.877193,
   0.9122807,
   0.925};
   Double_t divide_hnum_tk_data_by_hden_data_felx3001[4] = {
   1,
   1,
   1,
   6};
   Double_t divide_hnum_tk_data_by_hden_data_fely3001[4] = {
   0.03020759,
   0.03044895,
   0.0549856,
   0.06760868};
   Double_t divide_hnum_tk_data_by_hden_data_fehx3001[4] = {
   1,
   1,
   1,
   6};
   Double_t divide_hnum_tk_data_by_hden_data_fehy3001[4] = {
   0.02378891,
   0.02554169,
   0.0373642,
   0.04053863};
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(4,divide_hnum_tk_data_by_hden_data_fx3001,divide_hnum_tk_data_by_hden_data_fy3001,divide_hnum_tk_data_by_hden_data_felx3001,divide_hnum_tk_data_by_hden_data_fehx3001,divide_hnum_tk_data_by_hden_data_fely3001,divide_hnum_tk_data_by_hden_data_fehy3001);
   grae->SetName("divide_hnum_tk_data_by_hden_data");
   grae->SetTitle("");

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetLineStyle(0);

   ci = TColor::GetColor("#0000ff");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_divide_hnum_tk_data_by_hden_data3001 = new TH1F("Graph_divide_hnum_tk_data_by_hden_data3001","",100,0.2,21.8);
   Graph_divide_hnum_tk_data_by_hden_data3001->SetMinimum(0.8348646);
   Graph_divide_hnum_tk_data_by_hden_data3001->SetMaximum(0.9774181);
   Graph_divide_hnum_tk_data_by_hden_data3001->SetDirectory(0);
   Graph_divide_hnum_tk_data_by_hden_data3001->SetStats(0);
   Graph_divide_hnum_tk_data_by_hden_data3001->SetLineStyle(0);
   Graph_divide_hnum_tk_data_by_hden_data3001->SetMarkerStyle(20);
   Graph_divide_hnum_tk_data_by_hden_data3001->GetXaxis()->SetLabelFont(42);
   Graph_divide_hnum_tk_data_by_hden_data3001->GetXaxis()->SetLabelOffset(0.01);
   Graph_divide_hnum_tk_data_by_hden_data3001->GetXaxis()->SetLabelSize(0.045);
   Graph_divide_hnum_tk_data_by_hden_data3001->GetXaxis()->SetTitleSize(0.055);
   Graph_divide_hnum_tk_data_by_hden_data3001->GetXaxis()->SetTitleFont(42);
   Graph_divide_hnum_tk_data_by_hden_data3001->GetYaxis()->SetLabelFont(42);
   Graph_divide_hnum_tk_data_by_hden_data3001->GetYaxis()->SetLabelOffset(0.01);
   Graph_divide_hnum_tk_data_by_hden_data3001->GetYaxis()->SetLabelSize(0.045);
   Graph_divide_hnum_tk_data_by_hden_data3001->GetYaxis()->SetTitleSize(0.055);
   Graph_divide_hnum_tk_data_by_hden_data3001->GetYaxis()->SetTitleOffset(1.3);
   Graph_divide_hnum_tk_data_by_hden_data3001->GetYaxis()->SetTitleFont(42);
   Graph_divide_hnum_tk_data_by_hden_data3001->GetZaxis()->SetLabelFont(42);
   Graph_divide_hnum_tk_data_by_hden_data3001->GetZaxis()->SetLabelSize(0.045);
   Graph_divide_hnum_tk_data_by_hden_data3001->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_divide_hnum_tk_data_by_hden_data3001);
   
   grae->Draw("p");
   
   Double_t divide_hnum_tk_mc_by_hden_mc_fx3002[4] = {
   3,
   5,
   7,
   14};
   Double_t divide_hnum_tk_mc_by_hden_mc_fy3002[4] = {
   0.9064327,
   0.920904,
   0.8978102,
   0.8947368};
   Double_t divide_hnum_tk_mc_by_hden_mc_felx3002[4] = {
   1,
   1,
   1,
   6};
   Double_t divide_hnum_tk_mc_by_hden_mc_fely3002[4] = {
   0.0278727,
   0.01706871,
   0.03280833,
   0.04780869};
   Double_t divide_hnum_tk_mc_by_hden_mc_fehx3002[4] = {
   1,
   1,
   1,
   6};
   Double_t divide_hnum_tk_mc_by_hden_mc_fehy3002[4] = {
   0.02251511,
   0.01446095,
   0.02621339,
   0.03559191};
   grae = new TGraphAsymmErrors(4,divide_hnum_tk_mc_by_hden_mc_fx3002,divide_hnum_tk_mc_by_hden_mc_fy3002,divide_hnum_tk_mc_by_hden_mc_felx3002,divide_hnum_tk_mc_by_hden_mc_fehx3002,divide_hnum_tk_mc_by_hden_mc_fely3002,divide_hnum_tk_mc_by_hden_mc_fehy3002);
   grae->SetName("divide_hnum_tk_mc_by_hden_mc");
   grae->SetTitle("");

   ci = TColor::GetColor("#ff0000");
   grae->SetLineColor(ci);
   grae->SetLineStyle(0);

   ci = TColor::GetColor("#ff0000");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_divide_hnum_tk_mc_by_hden_mc3002 = new TH1F("Graph_divide_hnum_tk_mc_by_hden_mc3002","",100,0.2,21.8);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->SetMinimum(0.8380845);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->SetMaximum(0.9442086);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->SetDirectory(0);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->SetStats(0);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->SetLineStyle(0);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->SetMarkerStyle(20);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->GetXaxis()->SetLabelFont(42);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->GetXaxis()->SetLabelOffset(0.01);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->GetXaxis()->SetLabelSize(0.045);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->GetXaxis()->SetTitleSize(0.055);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->GetXaxis()->SetTitleFont(42);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->GetYaxis()->SetLabelFont(42);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->GetYaxis()->SetLabelOffset(0.01);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->GetYaxis()->SetLabelSize(0.045);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->GetYaxis()->SetTitleSize(0.055);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->GetYaxis()->SetTitleOffset(1.3);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->GetYaxis()->SetTitleFont(42);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->GetZaxis()->SetLabelFont(42);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->GetZaxis()->SetLabelSize(0.045);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_divide_hnum_tk_mc_by_hden_mc3002);
   
   grae->Draw("p");
   
   TLegend *leg = new TLegend(0.6,0.2,0.9,0.4,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(62);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("divide_hnum_tk_data_by_hden_data","Data: 0.899^{+0.015}_{-0.017}","LP");

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#0000ff");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(62);
   entry=leg->AddEntry("divide_hnum_tk_mc_by_hden_mc","MC: 0.911^{+0.011}_{-0.012}","LP");

   ci = TColor::GetColor("#ff0000");
   entry->SetLineColor(ci);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#ff0000");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(62);
   leg->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
