void convel()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sat May 12 00:39:56 2018) by ROOT version6.06/08
   TCanvas *c1 = new TCanvas("c1", "c1",1,1,700,476);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   c1->Range(-4.533333,-0.09375,22.13333,0.53125);
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
   
   TH1F *haxes_el__3 = new TH1F("haxes_el__3","",1,0,20);
   haxes_el__3->SetMinimum(0);
   haxes_el__3->SetMaximum(0.5);
   haxes_el__3->SetLineStyle(0);
   haxes_el__3->SetMarkerStyle(20);
   haxes_el__3->GetXaxis()->SetTitle("photon E_{T} [GeV]");
   haxes_el__3->GetXaxis()->SetLabelFont(42);
   haxes_el__3->GetXaxis()->SetLabelOffset(0.01);
   haxes_el__3->GetXaxis()->SetLabelSize(0.045);
   haxes_el__3->GetXaxis()->SetTitleSize(0.055);
   haxes_el__3->GetXaxis()->SetTitleFont(42);
   haxes_el__3->GetYaxis()->SetTitle("electron finding prob.");
   haxes_el__3->GetYaxis()->SetLabelFont(42);
   haxes_el__3->GetYaxis()->SetLabelOffset(0.01);
   haxes_el__3->GetYaxis()->SetLabelSize(0.045);
   haxes_el__3->GetYaxis()->SetTitleSize(0.055);
   haxes_el__3->GetYaxis()->SetTitleOffset(1.3);
   haxes_el__3->GetYaxis()->SetTitleFont(42);
   haxes_el__3->GetZaxis()->SetLabelFont(42);
   haxes_el__3->GetZaxis()->SetLabelSize(0.045);
   haxes_el__3->GetZaxis()->SetTitleFont(42);
   haxes_el__3->Draw("");
   
   TH1F *haxes_el_copy__4 = new TH1F("haxes_el_copy__4","",1,0,20);
   haxes_el_copy__4->SetMinimum(0);
   haxes_el_copy__4->SetMaximum(0.5);
   haxes_el_copy__4->SetDirectory(0);
   haxes_el_copy__4->SetLineStyle(0);
   haxes_el_copy__4->SetMarkerStyle(20);
   haxes_el_copy__4->GetXaxis()->SetTitle("photon E_{T} [GeV]");
   haxes_el_copy__4->GetXaxis()->SetLabelFont(42);
   haxes_el_copy__4->GetXaxis()->SetLabelOffset(0.01);
   haxes_el_copy__4->GetXaxis()->SetLabelSize(0.045);
   haxes_el_copy__4->GetXaxis()->SetTitleSize(0.055);
   haxes_el_copy__4->GetXaxis()->SetTitleFont(42);
   haxes_el_copy__4->GetYaxis()->SetTitle("electron finding prob.");
   haxes_el_copy__4->GetYaxis()->SetLabelFont(42);
   haxes_el_copy__4->GetYaxis()->SetLabelOffset(0.01);
   haxes_el_copy__4->GetYaxis()->SetLabelSize(0.045);
   haxes_el_copy__4->GetYaxis()->SetTitleSize(0.055);
   haxes_el_copy__4->GetYaxis()->SetTitleOffset(1.3);
   haxes_el_copy__4->GetYaxis()->SetTitleFont(42);
   haxes_el_copy__4->GetZaxis()->SetLabelFont(42);
   haxes_el_copy__4->GetZaxis()->SetLabelSize(0.045);
   haxes_el_copy__4->GetZaxis()->SetTitleFont(42);
   haxes_el_copy__4->Draw("sameaxis");
   
   Double_t divide_hnum_el_data_by_hden_data_fx3003[4] = {
   3,
   5,
   7,
   14};
   Double_t divide_hnum_el_data_by_hden_data_fy3003[4] = {
   0,
   0,
   0,
   0};
   Double_t divide_hnum_el_data_by_hden_data_felx3003[4] = {
   1,
   1,
   1,
   6};
   Double_t divide_hnum_el_data_by_hden_data_fely3003[4] = {
   0,
   0,
   0,
   0};
   Double_t divide_hnum_el_data_by_hden_data_fehx3003[4] = {
   1,
   1,
   1,
   6};
   Double_t divide_hnum_el_data_by_hden_data_fehy3003[4] = {
   0.01253057,
   0.01070846,
   0.0317826,
   0.04498243};
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
   Graph_divide_hnum_el_data_by_hden_data3003->SetMinimum(0);
   Graph_divide_hnum_el_data_by_hden_data3003->SetMaximum(0.04948067);
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
   0,
   0,
   0,
   0};
   Double_t divide_hnum_el_mc_by_hden_mc_felx3004[4] = {
   1,
   1,
   1,
   6};
   Double_t divide_hnum_el_mc_by_hden_mc_fely3004[4] = {
   0,
   0,
   0,
   0};
   Double_t divide_hnum_el_mc_by_hden_mc_fehx3004[4] = {
   1,
   1,
   1,
   6};
   Double_t divide_hnum_el_mc_by_hden_mc_fehy3004[4] = {
   0.01070846,
   0.005187126,
   0.01334823,
   0.02393292};
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
   Graph_divide_hnum_el_mc_by_hden_mc3004->SetMaximum(0.02632622);
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
   TLegendEntry *entry=leg->AddEntry("divide_hnum_el_data_by_hden_data","Data: 0.000^{+0.004}_{0.000}","LP");

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#0000ff");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(62);
   entry=leg->AddEntry("divide_hnum_el_mc_by_hden_mc","MC: 0.000^{+0.002}_{0.000}","LP");

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
