void convel()
{
//=========Macro generated from canvas: c1/c1
//=========  (Fri May 11 15:14:57 2018) by ROOT version6.02/05
   TCanvas *c1 = new TCanvas("c1", "c1",1,1,700,476);
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
   
   TH1F *haxes_el3 = new TH1F("haxes_el3","",1,0,20);
   haxes_el3->SetMinimum(0);
   haxes_el3->SetMaximum(1);
   haxes_el3->SetLineStyle(0);
   haxes_el3->SetMarkerStyle(20);
   haxes_el3->GetXaxis()->SetTitle("photon E_{T} [GeV]");
   haxes_el3->GetXaxis()->SetLabelFont(42);
   haxes_el3->GetXaxis()->SetLabelOffset(0.01);
   haxes_el3->GetXaxis()->SetLabelSize(0.045);
   haxes_el3->GetXaxis()->SetTitleSize(0.055);
   haxes_el3->GetXaxis()->SetTitleFont(42);
   haxes_el3->GetYaxis()->SetTitle("electron finding prob.");
   haxes_el3->GetYaxis()->SetLabelFont(42);
   haxes_el3->GetYaxis()->SetLabelOffset(0.01);
   haxes_el3->GetYaxis()->SetLabelSize(0.045);
   haxes_el3->GetYaxis()->SetTitleSize(0.055);
   haxes_el3->GetYaxis()->SetTitleOffset(1.3);
   haxes_el3->GetYaxis()->SetTitleFont(42);
   haxes_el3->GetZaxis()->SetLabelFont(42);
   haxes_el3->GetZaxis()->SetLabelSize(0.045);
   haxes_el3->GetZaxis()->SetTitleFont(42);
   haxes_el3->Draw("");
   
   TH1F *haxes_el_copy4 = new TH1F("haxes_el_copy4","",1,0,20);
   haxes_el_copy4->SetMinimum(0);
   haxes_el_copy4->SetMaximum(1);
   haxes_el_copy4->SetDirectory(0);
   haxes_el_copy4->SetLineStyle(0);
   haxes_el_copy4->SetMarkerStyle(20);
   haxes_el_copy4->GetXaxis()->SetTitle("photon E_{T} [GeV]");
   haxes_el_copy4->GetXaxis()->SetLabelFont(42);
   haxes_el_copy4->GetXaxis()->SetLabelOffset(0.01);
   haxes_el_copy4->GetXaxis()->SetLabelSize(0.045);
   haxes_el_copy4->GetXaxis()->SetTitleSize(0.055);
   haxes_el_copy4->GetXaxis()->SetTitleFont(42);
   haxes_el_copy4->GetYaxis()->SetTitle("electron finding prob.");
   haxes_el_copy4->GetYaxis()->SetLabelFont(42);
   haxes_el_copy4->GetYaxis()->SetLabelOffset(0.01);
   haxes_el_copy4->GetYaxis()->SetLabelSize(0.045);
   haxes_el_copy4->GetYaxis()->SetTitleSize(0.055);
   haxes_el_copy4->GetYaxis()->SetTitleOffset(1.3);
   haxes_el_copy4->GetYaxis()->SetTitleFont(42);
   haxes_el_copy4->GetZaxis()->SetLabelFont(42);
   haxes_el_copy4->GetZaxis()->SetLabelSize(0.045);
   haxes_el_copy4->GetZaxis()->SetTitleFont(42);
   haxes_el_copy4->Draw("sameaxis");
   
   Double_t divide_hnum_el_data_by_hden_data_fx3003[4] = {
   3,
   5,
   7,
   14};
   Double_t divide_hnum_el_data_by_hden_data_fy3003[4] = {
   0.6290323,
   0.3770492,
   0.2619048,
   0.2};
   Double_t divide_hnum_el_data_by_hden_data_felx3003[4] = {
   1,
   1,
   1,
   6};
   Double_t divide_hnum_el_data_by_hden_data_fely3003[4] = {
   0.07113811,
   0.04667369,
   0.07223922,
   0.09312246};
   Double_t divide_hnum_el_data_by_hden_data_fehx3003[4] = {
   1,
   1,
   1,
   6};
   Double_t divide_hnum_el_data_by_hden_data_fehy3003[4] = {
   0.06647884,
   0.04886589,
   0.08535128,
   0.1301189};
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(4,divide_hnum_el_data_by_hden_data_fx3003,divide_hnum_el_data_by_hden_data_fy3003,divide_hnum_el_data_by_hden_data_felx3003,divide_hnum_el_data_by_hden_data_fehx3003,divide_hnum_el_data_by_hden_data_fely3003,divide_hnum_el_data_by_hden_data_fehy3003);
   grae->SetName("divide_hnum_el_data_by_hden_data");
   grae->SetTitle("");

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   grae->SetLineColor(ci);
   grae->SetLineStyle(0);

   ci = TColor::GetColor("#0000ff");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_divide_hnum_el_data_by_hden_data3003 = new TH1F("Graph_divide_hnum_el_data_by_hden_data3003","",100,0.2,21.8);
   Graph_divide_hnum_el_data_by_hden_data3003->SetMinimum(0.04801418);
   Graph_divide_hnum_el_data_by_hden_data3003->SetMaximum(0.7543745);
   Graph_divide_hnum_el_data_by_hden_data3003->SetDirectory(0);
   Graph_divide_hnum_el_data_by_hden_data3003->SetStats(0);
   Graph_divide_hnum_el_data_by_hden_data3003->SetLineStyle(0);
   Graph_divide_hnum_el_data_by_hden_data3003->SetMarkerStyle(20);
   Graph_divide_hnum_el_data_by_hden_data3003->GetXaxis()->SetLabelFont(42);
   Graph_divide_hnum_el_data_by_hden_data3003->GetXaxis()->SetLabelOffset(0.01);
   Graph_divide_hnum_el_data_by_hden_data3003->GetXaxis()->SetLabelSize(0.045);
   Graph_divide_hnum_el_data_by_hden_data3003->GetXaxis()->SetTitleSize(0.055);
   Graph_divide_hnum_el_data_by_hden_data3003->GetXaxis()->SetTitleFont(42);
   Graph_divide_hnum_el_data_by_hden_data3003->GetYaxis()->SetLabelFont(42);
   Graph_divide_hnum_el_data_by_hden_data3003->GetYaxis()->SetLabelOffset(0.01);
   Graph_divide_hnum_el_data_by_hden_data3003->GetYaxis()->SetLabelSize(0.045);
   Graph_divide_hnum_el_data_by_hden_data3003->GetYaxis()->SetTitleSize(0.055);
   Graph_divide_hnum_el_data_by_hden_data3003->GetYaxis()->SetTitleOffset(1.3);
   Graph_divide_hnum_el_data_by_hden_data3003->GetYaxis()->SetTitleFont(42);
   Graph_divide_hnum_el_data_by_hden_data3003->GetZaxis()->SetLabelFont(42);
   Graph_divide_hnum_el_data_by_hden_data3003->GetZaxis()->SetLabelSize(0.045);
   Graph_divide_hnum_el_data_by_hden_data3003->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_divide_hnum_el_data_by_hden_data3003);
   
   grae->Draw("p");
   
   Double_t divide_hnum_el_mc_by_hden_mc_fx3004[4] = {
   3,
   5,
   7,
   14};
   Double_t divide_hnum_el_mc_by_hden_mc_fy3004[4] = {
   0.6691729,
   0.4083045,
   0.1071429,
   0.05555556};
   Double_t divide_hnum_el_mc_by_hden_mc_felx3004[4] = {
   1,
   1,
   1,
   6};
   Double_t divide_hnum_el_mc_by_hden_mc_fely3004[4] = {
   0.04577112,
   0.03025257,
   0.02963331,
   0.03008345};
   Double_t divide_hnum_el_mc_by_hden_mc_fehx3004[4] = {
   1,
   1,
   1,
   6};
   Double_t divide_hnum_el_mc_by_hden_mc_fehy3004[4] = {
   0.04300618,
   0.03092365,
   0.03767231,
   0.05110014};
   grae = new TGraphAsymmErrors(4,divide_hnum_el_mc_by_hden_mc_fx3004,divide_hnum_el_mc_by_hden_mc_fy3004,divide_hnum_el_mc_by_hden_mc_felx3004,divide_hnum_el_mc_by_hden_mc_fehx3004,divide_hnum_el_mc_by_hden_mc_fely3004,divide_hnum_el_mc_by_hden_mc_fehy3004);
   grae->SetName("divide_hnum_el_mc_by_hden_mc");
   grae->SetTitle("");

   ci = TColor::GetColor("#ff0000");
   grae->SetLineColor(ci);
   grae->SetLineStyle(0);

   ci = TColor::GetColor("#ff0000");
   grae->SetMarkerColor(ci);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_divide_hnum_el_mc_by_hden_mc3004 = new TH1F("Graph_divide_hnum_el_mc_by_hden_mc3004","",100,0.2,21.8);
   Graph_divide_hnum_el_mc_by_hden_mc3004->SetMinimum(0);
   Graph_divide_hnum_el_mc_by_hden_mc3004->SetMaximum(0.7808498);
   Graph_divide_hnum_el_mc_by_hden_mc3004->SetDirectory(0);
   Graph_divide_hnum_el_mc_by_hden_mc3004->SetStats(0);
   Graph_divide_hnum_el_mc_by_hden_mc3004->SetLineStyle(0);
   Graph_divide_hnum_el_mc_by_hden_mc3004->SetMarkerStyle(20);
   Graph_divide_hnum_el_mc_by_hden_mc3004->GetXaxis()->SetLabelFont(42);
   Graph_divide_hnum_el_mc_by_hden_mc3004->GetXaxis()->SetLabelOffset(0.01);
   Graph_divide_hnum_el_mc_by_hden_mc3004->GetXaxis()->SetLabelSize(0.045);
   Graph_divide_hnum_el_mc_by_hden_mc3004->GetXaxis()->SetTitleSize(0.055);
   Graph_divide_hnum_el_mc_by_hden_mc3004->GetXaxis()->SetTitleFont(42);
   Graph_divide_hnum_el_mc_by_hden_mc3004->GetYaxis()->SetLabelFont(42);
   Graph_divide_hnum_el_mc_by_hden_mc3004->GetYaxis()->SetLabelOffset(0.01);
   Graph_divide_hnum_el_mc_by_hden_mc3004->GetYaxis()->SetLabelSize(0.045);
   Graph_divide_hnum_el_mc_by_hden_mc3004->GetYaxis()->SetTitleSize(0.055);
   Graph_divide_hnum_el_mc_by_hden_mc3004->GetYaxis()->SetTitleOffset(1.3);
   Graph_divide_hnum_el_mc_by_hden_mc3004->GetYaxis()->SetTitleFont(42);
   Graph_divide_hnum_el_mc_by_hden_mc3004->GetZaxis()->SetLabelFont(42);
   Graph_divide_hnum_el_mc_by_hden_mc3004->GetZaxis()->SetLabelSize(0.045);
   Graph_divide_hnum_el_mc_by_hden_mc3004->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_divide_hnum_el_mc_by_hden_mc3004);
   
   grae->Draw("p");
   
   TLegend *leg = new TLegend(0.6,0.7,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(62);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("divide_hnum_el_data_by_hden_data","Data: 0.41^{+0.03}_{-0.03}","LP");

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#0000ff");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(62);
   entry=leg->AddEntry("divide_hnum_el_mc_by_hden_mc","MC: 0.38^{+0.02}_{-0.02}","LP");

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
