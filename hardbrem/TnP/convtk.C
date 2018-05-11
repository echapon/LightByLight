void convtk()
{
//=========Macro generated from canvas: c1/c1
//=========  (Fri May 11 15:14:57 2018) by ROOT version6.02/05
   TCanvas *c1 = new TCanvas("c1", "c1",0,0,700,500);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   c1->Range(-4.533333,-0.1875,22.13333,1.0625);
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
   
   TH1F *haxes_tk1 = new TH1F("haxes_tk1","",1,0,20);
   haxes_tk1->SetMinimum(0);
   haxes_tk1->SetMaximum(1);
   haxes_tk1->SetLineStyle(0);
   haxes_tk1->SetMarkerStyle(20);
   haxes_tk1->GetXaxis()->SetTitle("photon E_{T} [GeV]");
   haxes_tk1->GetXaxis()->SetLabelFont(42);
   haxes_tk1->GetXaxis()->SetLabelOffset(0.01);
   haxes_tk1->GetXaxis()->SetLabelSize(0.045);
   haxes_tk1->GetXaxis()->SetTitleSize(0.055);
   haxes_tk1->GetXaxis()->SetTitleFont(42);
   haxes_tk1->GetYaxis()->SetTitle("track finding prob.");
   haxes_tk1->GetYaxis()->SetLabelFont(42);
   haxes_tk1->GetYaxis()->SetLabelOffset(0.01);
   haxes_tk1->GetYaxis()->SetLabelSize(0.045);
   haxes_tk1->GetYaxis()->SetTitleSize(0.055);
   haxes_tk1->GetYaxis()->SetTitleOffset(1.3);
   haxes_tk1->GetYaxis()->SetTitleFont(42);
   haxes_tk1->GetZaxis()->SetLabelFont(42);
   haxes_tk1->GetZaxis()->SetLabelSize(0.045);
   haxes_tk1->GetZaxis()->SetTitleFont(42);
   haxes_tk1->Draw("");
   
   TH1F *haxes_tk_copy2 = new TH1F("haxes_tk_copy2","",1,0,20);
   haxes_tk_copy2->SetMinimum(0);
   haxes_tk_copy2->SetMaximum(1);
   haxes_tk_copy2->SetDirectory(0);
   haxes_tk_copy2->SetLineStyle(0);
   haxes_tk_copy2->SetMarkerStyle(20);
   haxes_tk_copy2->GetXaxis()->SetTitle("photon E_{T} [GeV]");
   haxes_tk_copy2->GetXaxis()->SetLabelFont(42);
   haxes_tk_copy2->GetXaxis()->SetLabelOffset(0.01);
   haxes_tk_copy2->GetXaxis()->SetLabelSize(0.045);
   haxes_tk_copy2->GetXaxis()->SetTitleSize(0.055);
   haxes_tk_copy2->GetXaxis()->SetTitleFont(42);
   haxes_tk_copy2->GetYaxis()->SetTitle("track finding prob.");
   haxes_tk_copy2->GetYaxis()->SetLabelFont(42);
   haxes_tk_copy2->GetYaxis()->SetLabelOffset(0.01);
   haxes_tk_copy2->GetYaxis()->SetLabelSize(0.045);
   haxes_tk_copy2->GetYaxis()->SetTitleSize(0.055);
   haxes_tk_copy2->GetYaxis()->SetTitleOffset(1.3);
   haxes_tk_copy2->GetYaxis()->SetTitleFont(42);
   haxes_tk_copy2->GetZaxis()->SetLabelFont(42);
   haxes_tk_copy2->GetZaxis()->SetLabelSize(0.045);
   haxes_tk_copy2->GetZaxis()->SetTitleFont(42);
   haxes_tk_copy2->Draw("sameaxis");
   
   Double_t divide_hnum_tk_data_by_hden_data_fx3001[4] = {
   3,
   5,
   7,
   14};
   Double_t divide_hnum_tk_data_by_hden_data_fy3001[4] = {
   0.6451613,
   0.557377,
   0.4285714,
   0.5};
   Double_t divide_hnum_tk_data_by_hden_data_felx3001[4] = {
   1,
   1,
   1,
   6};
   Double_t divide_hnum_tk_data_by_hden_data_fely3001[4] = {
   0.07085901,
   0.04936077,
   0.0851435,
   0.1327325};
   Double_t divide_hnum_tk_data_by_hden_data_fehx3001[4] = {
   1,
   1,
   1,
   6};
   Double_t divide_hnum_tk_data_by_hden_data_fehy3001[4] = {
   0.0656115,
   0.04833986,
   0.08901242,
   0.1327325};
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
   Graph_divide_hnum_tk_data_by_hden_data3001->SetMinimum(0.3066934);
   Graph_divide_hnum_tk_data_by_hden_data3001->SetMaximum(0.7475073);
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
   0.6616541,
   0.5847751,
   0.4732143,
   0.4259259};
   Double_t divide_hnum_tk_mc_by_hden_mc_felx3002[4] = {
   1,
   1,
   1,
   6};
   Double_t divide_hnum_tk_mc_by_hden_mc_fely3002[4] = {
   0.04593557,
   0.03097097,
   0.05113093,
   0.07421905};
   Double_t divide_hnum_tk_mc_by_hden_mc_fehx3002[4] = {
   1,
   1,
   1,
   6};
   Double_t divide_hnum_tk_mc_by_hden_mc_fehy3002[4] = {
   0.04329477,
   0.03035063,
   0.05165149,
   0.07730169};
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
   Graph_divide_hnum_tk_mc_by_hden_mc3002->SetMinimum(0.3163827);
   Graph_divide_hnum_tk_mc_by_hden_mc3002->SetMaximum(0.7402731);
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
   
   TLegend *leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(62);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("divide_hnum_tk_data_by_hden_data","Data: 0.55^{+0.03}_{-0.03}","LP");

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#0000ff");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(62);
   entry=leg->AddEntry("divide_hnum_tk_mc_by_hden_mc","MC: 0.57^{+0.02}_{-0.02}","LP");

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
