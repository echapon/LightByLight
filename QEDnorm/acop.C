void acop()
{
//=========Macro generated from canvas: acop/
//=========  (Mon Sep 17 22:39:36 2018) by ROOT version6.06/08
   TCanvas *acop = new TCanvas("acop", "",0,0,700,600);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   acop->Range(0,0,1,1);
   acop->SetFillColor(0);
   acop->SetBorderMode(0);
   acop->SetBorderSize(2);
   acop->SetLeftMargin(0.14);
   acop->SetRightMargin(0.04);
   acop->SetTopMargin(0.08);
   acop->SetBottomMargin(0.14);
   acop->SetFrameFillStyle(0);
   acop->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: TopPad
   TPad *TopPad = new TPad("TopPad", "TopPad",0.01,0.26,1,1);
   TopPad->Draw();
   TopPad->cd();
   TopPad->Range(-0.001510791,0.06533885,0.01047962,4.425625);
   TopPad->SetFillColor(0);
   TopPad->SetBorderMode(0);
   TopPad->SetBorderSize(2);
   TopPad->SetLogy();
   TopPad->SetTickx(1);
   TopPad->SetTicky(1);
   TopPad->SetLeftMargin(0.126);
   TopPad->SetRightMargin(0.04);
   TopPad->SetBottomMargin(0.05405406);
   TopPad->SetFrameFillStyle(0);
   TopPad->SetFrameBorderMode(0);
   TopPad->SetFrameFillStyle(0);
   TopPad->SetFrameBorderMode(0);
   
   TH1D *hacop_data__37 = new TH1D("hacop_data__37","",20,0,0.01);
   hacop_data__37->SetBinContent(1,2509);
   hacop_data__37->SetBinContent(2,1987);
   hacop_data__37->SetBinContent(3,1456);
   hacop_data__37->SetBinContent(4,1007);
   hacop_data__37->SetBinContent(5,661);
   hacop_data__37->SetBinContent(6,496);
   hacop_data__37->SetBinContent(7,335);
   hacop_data__37->SetBinContent(8,261);
   hacop_data__37->SetBinContent(9,191);
   hacop_data__37->SetBinContent(10,142);
   hacop_data__37->SetBinContent(11,109);
   hacop_data__37->SetBinContent(12,92);
   hacop_data__37->SetBinContent(13,57);
   hacop_data__37->SetBinContent(14,77);
   hacop_data__37->SetBinContent(15,58);
   hacop_data__37->SetBinContent(16,30);
   hacop_data__37->SetBinContent(17,32);
   hacop_data__37->SetBinContent(18,22);
   hacop_data__37->SetBinContent(19,24);
   hacop_data__37->SetBinContent(20,24);
   hacop_data__37->SetBinError(1,50.08992);
   hacop_data__37->SetBinError(2,44.57578);
   hacop_data__37->SetBinError(3,38.15757);
   hacop_data__37->SetBinError(4,31.73326);
   hacop_data__37->SetBinError(5,25.70992);
   hacop_data__37->SetBinError(6,22.27106);
   hacop_data__37->SetBinError(7,18.30301);
   hacop_data__37->SetBinError(8,16.15549);
   hacop_data__37->SetBinError(9,13.82027);
   hacop_data__37->SetBinError(10,11.91638);
   hacop_data__37->SetBinError(11,10.44031);
   hacop_data__37->SetBinError(12,9.591663);
   hacop_data__37->SetBinError(13,7.549834);
   hacop_data__37->SetBinError(14,8.774964);
   hacop_data__37->SetBinError(15,7.615773);
   hacop_data__37->SetBinError(16,5.477226);
   hacop_data__37->SetBinError(17,5.656854);
   hacop_data__37->SetBinError(18,4.690416);
   hacop_data__37->SetBinError(19,4.898979);
   hacop_data__37->SetBinError(20,4.898979);
   hacop_data__37->SetMinimum(2);
   hacop_data__37->SetMaximum(9000);
   hacop_data__37->SetEntries(9570);
   hacop_data__37->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = 1639;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   hacop_data__37->SetFillColor(ci);
   hacop_data__37->SetLineStyle(0);
   hacop_data__37->SetMarkerStyle(20);
   hacop_data__37->GetXaxis()->SetTitle("Dielectron A_{#phi}");
   hacop_data__37->GetXaxis()->SetMoreLogLabels();
   hacop_data__37->GetXaxis()->SetNoExponent();
   hacop_data__37->GetXaxis()->SetNdivisions(509);
   hacop_data__37->GetXaxis()->SetLabelFont(42);
   hacop_data__37->GetXaxis()->SetLabelOffset(0.01);
   hacop_data__37->GetXaxis()->SetLabelSize(0);
   hacop_data__37->GetXaxis()->SetTitleSize(0);
   hacop_data__37->GetXaxis()->SetTitleFont(42);
   hacop_data__37->GetYaxis()->SetTitle("Entries / (0.0005)");
   hacop_data__37->GetYaxis()->SetLabelFont(42);
   hacop_data__37->GetYaxis()->SetLabelOffset(0.01);
   hacop_data__37->GetYaxis()->SetLabelSize(0.06081081);
   hacop_data__37->GetYaxis()->SetTitleSize(0.06756756);
   hacop_data__37->GetYaxis()->SetTitleFont(42);
   hacop_data__37->GetZaxis()->SetLabelFont(42);
   hacop_data__37->GetZaxis()->SetLabelSize(0.045);
   hacop_data__37->GetZaxis()->SetTitleFont(42);
   hacop_data__37->Draw("EP");
   
   TH1D *hacop_MC__38 = new TH1D("hacop_MC__38","",20,0,0.01);
   hacop_MC__38->SetBinContent(1,2376.45);
   hacop_MC__38->SetBinContent(2,1886.078);
   hacop_MC__38->SetBinContent(3,1372.672);
   hacop_MC__38->SetBinContent(4,982.5935);
   hacop_MC__38->SetBinContent(5,685.1589);
   hacop_MC__38->SetBinContent(6,482.4696);
   hacop_MC__38->SetBinContent(7,358.3843);
   hacop_MC__38->SetBinContent(8,269.0194);
   hacop_MC__38->SetBinContent(9,185.9597);
   hacop_MC__38->SetBinContent(10,139.8901);
   hacop_MC__38->SetBinContent(11,107.6918);
   hacop_MC__38->SetBinContent(12,77.00682);
   hacop_MC__38->SetBinContent(13,57.50291);
   hacop_MC__38->SetBinContent(14,44.89262);
   hacop_MC__38->SetBinContent(15,29.76028);
   hacop_MC__38->SetBinContent(16,25.72498);
   hacop_MC__38->SetBinContent(17,19.67205);
   hacop_MC__38->SetBinContent(18,17.57033);
   hacop_MC__38->SetBinContent(19,11.93774);
   hacop_MC__38->SetBinContent(20,9.247543);
   hacop_MC__38->SetBinError(1,257.8496);
   hacop_MC__38->SetBinError(2,212.8969);
   hacop_MC__38->SetBinError(3,162.5675);
   hacop_MC__38->SetBinError(4,126.2795);
   hacop_MC__38->SetBinError(5,90.46869);
   hacop_MC__38->SetBinError(6,68.91906);
   hacop_MC__38->SetBinError(7,55.9773);
   hacop_MC__38->SetBinError(8,42.88586);
   hacop_MC__38->SetBinError(9,33.08544);
   hacop_MC__38->SetBinError(10,27.16515);
   hacop_MC__38->SetBinError(11,21.24672);
   hacop_MC__38->SetBinError(12,15.14531);
   hacop_MC__38->SetBinError(13,12.12904);
   hacop_MC__38->SetBinError(14,9.148249);
   hacop_MC__38->SetBinError(15,6.366905);
   hacop_MC__38->SetBinError(16,5.2599);
   hacop_MC__38->SetBinError(17,4.443407);
   hacop_MC__38->SetBinError(18,4.050016);
   hacop_MC__38->SetBinError(19,2.69184);
   hacop_MC__38->SetBinError(20,1.833019);
   hacop_MC__38->SetEntries(108717);
   hacop_MC__38->SetStats(0);

   ci = TColor::GetColor("#ffff00");
   hacop_MC__38->SetFillColor(ci);
   hacop_MC__38->SetLineStyle(0);

   ci = TColor::GetColor("#ffff00");
   hacop_MC__38->SetMarkerColor(ci);
   hacop_MC__38->SetMarkerStyle(20);
   hacop_MC__38->GetXaxis()->SetLabelFont(42);
   hacop_MC__38->GetXaxis()->SetLabelOffset(0.01);
   hacop_MC__38->GetXaxis()->SetLabelSize(0.045);
   hacop_MC__38->GetXaxis()->SetTitleSize(0.055);
   hacop_MC__38->GetXaxis()->SetTitleFont(42);
   hacop_MC__38->GetYaxis()->SetLabelFont(42);
   hacop_MC__38->GetYaxis()->SetLabelOffset(0.01);
   hacop_MC__38->GetYaxis()->SetLabelSize(0.045);
   hacop_MC__38->GetYaxis()->SetTitleSize(0.055);
   hacop_MC__38->GetYaxis()->SetTitleOffset(1.3);
   hacop_MC__38->GetYaxis()->SetTitleFont(42);
   hacop_MC__38->GetZaxis()->SetLabelFont(42);
   hacop_MC__38->GetZaxis()->SetLabelSize(0.045);
   hacop_MC__38->GetZaxis()->SetTitleFont(42);
   hacop_MC__38->Draw("hist SAME");
   
   TH1D *hacop_MC__39 = new TH1D("hacop_MC__39","",20,0,0.01);
   hacop_MC__39->SetBinContent(1,2376.45);
   hacop_MC__39->SetBinContent(2,1886.078);
   hacop_MC__39->SetBinContent(3,1372.672);
   hacop_MC__39->SetBinContent(4,982.5935);
   hacop_MC__39->SetBinContent(5,685.1589);
   hacop_MC__39->SetBinContent(6,482.4696);
   hacop_MC__39->SetBinContent(7,358.3843);
   hacop_MC__39->SetBinContent(8,269.0194);
   hacop_MC__39->SetBinContent(9,185.9597);
   hacop_MC__39->SetBinContent(10,139.8901);
   hacop_MC__39->SetBinContent(11,107.6918);
   hacop_MC__39->SetBinContent(12,77.00682);
   hacop_MC__39->SetBinContent(13,57.50291);
   hacop_MC__39->SetBinContent(14,44.89262);
   hacop_MC__39->SetBinContent(15,29.76028);
   hacop_MC__39->SetBinContent(16,25.72498);
   hacop_MC__39->SetBinContent(17,19.67205);
   hacop_MC__39->SetBinContent(18,17.57033);
   hacop_MC__39->SetBinContent(19,11.93774);
   hacop_MC__39->SetBinContent(20,9.247543);
   hacop_MC__39->SetBinError(1,257.8496);
   hacop_MC__39->SetBinError(2,212.8969);
   hacop_MC__39->SetBinError(3,162.5675);
   hacop_MC__39->SetBinError(4,126.2795);
   hacop_MC__39->SetBinError(5,90.46869);
   hacop_MC__39->SetBinError(6,68.91906);
   hacop_MC__39->SetBinError(7,55.9773);
   hacop_MC__39->SetBinError(8,42.88586);
   hacop_MC__39->SetBinError(9,33.08544);
   hacop_MC__39->SetBinError(10,27.16515);
   hacop_MC__39->SetBinError(11,21.24672);
   hacop_MC__39->SetBinError(12,15.14531);
   hacop_MC__39->SetBinError(13,12.12904);
   hacop_MC__39->SetBinError(14,9.148249);
   hacop_MC__39->SetBinError(15,6.366905);
   hacop_MC__39->SetBinError(16,5.2599);
   hacop_MC__39->SetBinError(17,4.443407);
   hacop_MC__39->SetBinError(18,4.050016);
   hacop_MC__39->SetBinError(19,2.69184);
   hacop_MC__39->SetBinError(20,1.833019);
   hacop_MC__39->SetEntries(108717);
   hacop_MC__39->SetFillColor(1);
   hacop_MC__39->SetFillStyle(3005);
   hacop_MC__39->SetLineStyle(0);
   hacop_MC__39->SetMarkerStyle(20);
   hacop_MC__39->SetMarkerSize(0);
   hacop_MC__39->GetXaxis()->SetLabelFont(42);
   hacop_MC__39->GetXaxis()->SetLabelOffset(0.01);
   hacop_MC__39->GetXaxis()->SetLabelSize(0.045);
   hacop_MC__39->GetXaxis()->SetTitleSize(0.055);
   hacop_MC__39->GetXaxis()->SetTitleFont(42);
   hacop_MC__39->GetYaxis()->SetLabelFont(42);
   hacop_MC__39->GetYaxis()->SetLabelOffset(0.01);
   hacop_MC__39->GetYaxis()->SetLabelSize(0.045);
   hacop_MC__39->GetYaxis()->SetTitleSize(0.055);
   hacop_MC__39->GetYaxis()->SetTitleOffset(1.3);
   hacop_MC__39->GetYaxis()->SetTitleFont(42);
   hacop_MC__39->GetZaxis()->SetLabelFont(42);
   hacop_MC__39->GetZaxis()->SetLabelSize(0.045);
   hacop_MC__39->GetZaxis()->SetTitleFont(42);
   hacop_MC__39->Draw("E2 same");
   
   TH1D *hacop_data__40 = new TH1D("hacop_data__40","",20,0,0.01);
   hacop_data__40->SetBinContent(1,2509);
   hacop_data__40->SetBinContent(2,1987);
   hacop_data__40->SetBinContent(3,1456);
   hacop_data__40->SetBinContent(4,1007);
   hacop_data__40->SetBinContent(5,661);
   hacop_data__40->SetBinContent(6,496);
   hacop_data__40->SetBinContent(7,335);
   hacop_data__40->SetBinContent(8,261);
   hacop_data__40->SetBinContent(9,191);
   hacop_data__40->SetBinContent(10,142);
   hacop_data__40->SetBinContent(11,109);
   hacop_data__40->SetBinContent(12,92);
   hacop_data__40->SetBinContent(13,57);
   hacop_data__40->SetBinContent(14,77);
   hacop_data__40->SetBinContent(15,58);
   hacop_data__40->SetBinContent(16,30);
   hacop_data__40->SetBinContent(17,32);
   hacop_data__40->SetBinContent(18,22);
   hacop_data__40->SetBinContent(19,24);
   hacop_data__40->SetBinContent(20,24);
   hacop_data__40->SetBinError(1,50.08992);
   hacop_data__40->SetBinError(2,44.57578);
   hacop_data__40->SetBinError(3,38.15757);
   hacop_data__40->SetBinError(4,31.73326);
   hacop_data__40->SetBinError(5,25.70992);
   hacop_data__40->SetBinError(6,22.27106);
   hacop_data__40->SetBinError(7,18.30301);
   hacop_data__40->SetBinError(8,16.15549);
   hacop_data__40->SetBinError(9,13.82027);
   hacop_data__40->SetBinError(10,11.91638);
   hacop_data__40->SetBinError(11,10.44031);
   hacop_data__40->SetBinError(12,9.591663);
   hacop_data__40->SetBinError(13,7.549834);
   hacop_data__40->SetBinError(14,8.774964);
   hacop_data__40->SetBinError(15,7.615773);
   hacop_data__40->SetBinError(16,5.477226);
   hacop_data__40->SetBinError(17,5.656854);
   hacop_data__40->SetBinError(18,4.690416);
   hacop_data__40->SetBinError(19,4.898979);
   hacop_data__40->SetBinError(20,4.898979);
   hacop_data__40->SetMinimum(2);
   hacop_data__40->SetMaximum(9000);
   hacop_data__40->SetEntries(9570);
   hacop_data__40->SetStats(0);

   ci = 1639;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   hacop_data__40->SetFillColor(ci);
   hacop_data__40->SetLineStyle(0);
   hacop_data__40->SetMarkerStyle(20);
   hacop_data__40->GetXaxis()->SetTitle("Dielectron A_{#phi}");
   hacop_data__40->GetXaxis()->SetMoreLogLabels();
   hacop_data__40->GetXaxis()->SetNoExponent();
   hacop_data__40->GetXaxis()->SetNdivisions(509);
   hacop_data__40->GetXaxis()->SetLabelFont(42);
   hacop_data__40->GetXaxis()->SetLabelOffset(0.01);
   hacop_data__40->GetXaxis()->SetLabelSize(0);
   hacop_data__40->GetXaxis()->SetTitleSize(0);
   hacop_data__40->GetXaxis()->SetTitleFont(42);
   hacop_data__40->GetYaxis()->SetTitle("Entries / (0.0005)");
   hacop_data__40->GetYaxis()->SetLabelFont(42);
   hacop_data__40->GetYaxis()->SetLabelOffset(0.01);
   hacop_data__40->GetYaxis()->SetLabelSize(0.06081081);
   hacop_data__40->GetYaxis()->SetTitleSize(0.06756756);
   hacop_data__40->GetYaxis()->SetTitleFont(42);
   hacop_data__40->GetZaxis()->SetLabelFont(42);
   hacop_data__40->GetZaxis()->SetLabelSize(0.045);
   hacop_data__40->GetZaxis()->SetTitleFont(42);
   hacop_data__40->Draw("EPSAME");
   
   TLegend *leg = new TLegend(0.3,0.68,0.7,0.85,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(62);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry("hacop_data","Data","lpf");

   ci = 1639;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(62);
   entry=leg->AddEntry("hacop_MC","QED #gamma#gamma #rightarrow e^{+}e^{-} (MC)","f");

   ci = TColor::GetColor("#ffff00");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(62);
   leg->Draw();
   
   TH1D *hacop_data_copy__41 = new TH1D("hacop_data_copy__41","",20,0,0.01);
   hacop_data_copy__41->SetBinContent(1,2509);
   hacop_data_copy__41->SetBinContent(2,1987);
   hacop_data_copy__41->SetBinContent(3,1456);
   hacop_data_copy__41->SetBinContent(4,1007);
   hacop_data_copy__41->SetBinContent(5,661);
   hacop_data_copy__41->SetBinContent(6,496);
   hacop_data_copy__41->SetBinContent(7,335);
   hacop_data_copy__41->SetBinContent(8,261);
   hacop_data_copy__41->SetBinContent(9,191);
   hacop_data_copy__41->SetBinContent(10,142);
   hacop_data_copy__41->SetBinContent(11,109);
   hacop_data_copy__41->SetBinContent(12,92);
   hacop_data_copy__41->SetBinContent(13,57);
   hacop_data_copy__41->SetBinContent(14,77);
   hacop_data_copy__41->SetBinContent(15,58);
   hacop_data_copy__41->SetBinContent(16,30);
   hacop_data_copy__41->SetBinContent(17,32);
   hacop_data_copy__41->SetBinContent(18,22);
   hacop_data_copy__41->SetBinContent(19,24);
   hacop_data_copy__41->SetBinContent(20,24);
   hacop_data_copy__41->SetBinError(1,50.08992);
   hacop_data_copy__41->SetBinError(2,44.57578);
   hacop_data_copy__41->SetBinError(3,38.15757);
   hacop_data_copy__41->SetBinError(4,31.73326);
   hacop_data_copy__41->SetBinError(5,25.70992);
   hacop_data_copy__41->SetBinError(6,22.27106);
   hacop_data_copy__41->SetBinError(7,18.30301);
   hacop_data_copy__41->SetBinError(8,16.15549);
   hacop_data_copy__41->SetBinError(9,13.82027);
   hacop_data_copy__41->SetBinError(10,11.91638);
   hacop_data_copy__41->SetBinError(11,10.44031);
   hacop_data_copy__41->SetBinError(12,9.591663);
   hacop_data_copy__41->SetBinError(13,7.549834);
   hacop_data_copy__41->SetBinError(14,8.774964);
   hacop_data_copy__41->SetBinError(15,7.615773);
   hacop_data_copy__41->SetBinError(16,5.477226);
   hacop_data_copy__41->SetBinError(17,5.656854);
   hacop_data_copy__41->SetBinError(18,4.690416);
   hacop_data_copy__41->SetBinError(19,4.898979);
   hacop_data_copy__41->SetBinError(20,4.898979);
   hacop_data_copy__41->SetMinimum(2);
   hacop_data_copy__41->SetMaximum(9000);
   hacop_data_copy__41->SetEntries(9570);
   hacop_data_copy__41->SetDirectory(0);
   hacop_data_copy__41->SetStats(0);

   ci = 1639;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   hacop_data_copy__41->SetFillColor(ci);
   hacop_data_copy__41->SetLineStyle(0);
   hacop_data_copy__41->SetMarkerStyle(20);
   hacop_data_copy__41->GetXaxis()->SetTitle("Dielectron A_{#phi}");
   hacop_data_copy__41->GetXaxis()->SetMoreLogLabels();
   hacop_data_copy__41->GetXaxis()->SetNoExponent();
   hacop_data_copy__41->GetXaxis()->SetNdivisions(509);
   hacop_data_copy__41->GetXaxis()->SetLabelFont(42);
   hacop_data_copy__41->GetXaxis()->SetLabelOffset(0.01);
   hacop_data_copy__41->GetXaxis()->SetLabelSize(0);
   hacop_data_copy__41->GetXaxis()->SetTitleSize(0);
   hacop_data_copy__41->GetXaxis()->SetTitleFont(42);
   hacop_data_copy__41->GetYaxis()->SetTitle("Entries / (0.0005)");
   hacop_data_copy__41->GetYaxis()->SetLabelFont(42);
   hacop_data_copy__41->GetYaxis()->SetLabelOffset(0.01);
   hacop_data_copy__41->GetYaxis()->SetLabelSize(0.06081081);
   hacop_data_copy__41->GetYaxis()->SetTitleSize(0.06756756);
   hacop_data_copy__41->GetYaxis()->SetTitleFont(42);
   hacop_data_copy__41->GetZaxis()->SetLabelFont(42);
   hacop_data_copy__41->GetZaxis()->SetLabelSize(0.045);
   hacop_data_copy__41->GetZaxis()->SetTitleFont(42);
   hacop_data_copy__41->Draw("sameaxis");
   TLatex *   tex = new TLatex(0.96,0.9221622,"PbPb 390 #mub^{-1} (5.02 TeV)");
tex->SetNDC();
   tex->SetTextAlign(31);
   tex->SetTextFont(42);
   tex->SetTextSize(0.05405405);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.126,0.9221622,"");
tex->SetNDC();
   tex->SetTextFont(42);
   tex->SetTextSize(0.05405405);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.91247,0.8441892,"CMS");
tex->SetNDC();
   tex->SetTextAlign(33);
   tex->SetTextFont(61);
   tex->SetTextSize(0.08108108);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.91247,0.7468919,"");
tex->SetNDC();
   tex->SetTextAlign(33);
   tex->SetTextFont(52);
   tex->SetTextSize(0.06162162);
   tex->SetLineWidth(2);
   tex->Draw();
   TopPad->Modified();
   acop->cd();
  
// ------------>Primitives in pad: BottomPad
   TPad *BottomPad = new TPad("BottomPad", "BottomPad",0.01,0.01,0.99,0.3);
   BottomPad->Draw();
   BottomPad->cd();
   BottomPad->Range(-0.001512346,-0.8494909,0.0103585,1.9);
   BottomPad->SetFillColor(0);
   BottomPad->SetFillStyle(4000);
   BottomPad->SetBorderMode(0);
   BottomPad->SetBorderSize(2);
   BottomPad->SetTickx(1);
   BottomPad->SetTicky(1);
   BottomPad->SetLeftMargin(0.1274);
   BottomPad->SetRightMargin(0.0302);
   BottomPad->SetTopMargin(0);
   BottomPad->SetBottomMargin(0.3453333);
   BottomPad->SetFrameFillStyle(0);
   BottomPad->SetFrameBorderMode(0);
   BottomPad->SetFrameFillStyle(0);
   BottomPad->SetFrameBorderMode(0);
   
   TH1D *h_ratio__42 = new TH1D("h_ratio__42","",20,0,0.01);
   h_ratio__42->SetBinContent(1,1.055776);
   h_ratio__42->SetBinContent(2,1.053509);
   h_ratio__42->SetBinContent(3,1.060705);
   h_ratio__42->SetBinContent(4,1.024839);
   h_ratio__42->SetBinContent(5,0.9647397);
   h_ratio__42->SetBinContent(6,1.028044);
   h_ratio__42->SetBinContent(7,0.9347507);
   h_ratio__42->SetBinContent(8,0.9701901);
   h_ratio__42->SetBinContent(9,1.027104);
   h_ratio__42->SetBinContent(10,1.015082);
   h_ratio__42->SetBinContent(11,1.012147);
   h_ratio__42->SetBinContent(12,1.194699);
   h_ratio__42->SetBinContent(13,0.9912543);
   h_ratio__42->SetBinContent(14,1.715204);
   h_ratio__42->SetBinContent(15,1.948907);
   h_ratio__42->SetBinContent(16,1.166181);
   h_ratio__42->SetBinContent(17,1.626674);
   h_ratio__42->SetBinContent(18,1.252111);
   h_ratio__42->SetBinContent(19,2.010431);
   h_ratio__42->SetBinContent(20,2.595284);
   h_ratio__42->SetBinError(1,0.02107762);
   h_ratio__42->SetBinError(2,0.02363411);
   h_ratio__42->SetBinError(3,0.02779803);
   h_ratio__42->SetBinError(4,0.03229541);
   h_ratio__42->SetBinError(5,0.03752403);
   h_ratio__42->SetBinError(6,0.04616054);
   h_ratio__42->SetBinError(7,0.05107088);
   h_ratio__42->SetBinError(8,0.06005326);
   h_ratio__42->SetBinError(9,0.07431866);
   h_ratio__42->SetBinError(10,0.08518383);
   h_ratio__42->SetBinError(11,0.09694612);
   h_ratio__42->SetBinError(12,0.124556);
   h_ratio__42->SetBinError(13,0.1312948);
   h_ratio__42->SetBinError(14,0.1954656);
   h_ratio__42->SetBinError(15,0.255904);
   h_ratio__42->SetBinError(16,0.2129146);
   h_ratio__42->SetBinError(17,0.287558);
   h_ratio__42->SetBinError(18,0.2669509);
   h_ratio__42->SetBinError(19,0.4103775);
   h_ratio__42->SetBinError(20,0.5297601);
   h_ratio__42->SetMinimum(0.1);
   h_ratio__42->SetMaximum(1.9);
   h_ratio__42->SetEntries(802.6498);
   h_ratio__42->SetStats(0);

   ci = 1639;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   h_ratio__42->SetFillColor(ci);
   h_ratio__42->SetLineStyle(0);
   h_ratio__42->SetMarkerStyle(20);
   h_ratio__42->GetXaxis()->SetTitle("Dielectron A_{#phi}");
   h_ratio__42->GetXaxis()->SetMoreLogLabels();
   h_ratio__42->GetXaxis()->SetNoExponent();
   h_ratio__42->GetXaxis()->SetNdivisions(509);
   h_ratio__42->GetXaxis()->SetLabelFont(42);
   h_ratio__42->GetXaxis()->SetLabelOffset(0.007);
   h_ratio__42->GetXaxis()->SetLabelSize(0.15);
   h_ratio__42->GetXaxis()->SetTitleSize(0.1666667);
   h_ratio__42->GetXaxis()->SetTickLength(0.08571428);
   h_ratio__42->GetXaxis()->SetTitleOffset(0.92);
   h_ratio__42->GetXaxis()->SetTitleFont(42);
   h_ratio__42->GetYaxis()->SetTitle("Data/MC");
   h_ratio__42->GetYaxis()->SetNdivisions(505);
   h_ratio__42->GetYaxis()->SetLabelFont(42);
   h_ratio__42->GetYaxis()->SetLabelOffset(0.01);
   h_ratio__42->GetYaxis()->SetLabelSize(0.15);
   h_ratio__42->GetYaxis()->SetTitleSize(0.1666667);
   h_ratio__42->GetYaxis()->SetTitleOffset(0.4125);
   h_ratio__42->GetYaxis()->SetTitleFont(42);
   h_ratio__42->GetZaxis()->SetLabelFont(42);
   h_ratio__42->GetZaxis()->SetLabelSize(0.045);
   h_ratio__42->GetZaxis()->SetTitleFont(42);
   h_ratio__42->Draw("EP");
   
   TH1D *hacop_MC__43 = new TH1D("hacop_MC__43","",20,0,0.01);
   hacop_MC__43->SetBinContent(1,1);
   hacop_MC__43->SetBinContent(2,1);
   hacop_MC__43->SetBinContent(3,1);
   hacop_MC__43->SetBinContent(4,1);
   hacop_MC__43->SetBinContent(5,1);
   hacop_MC__43->SetBinContent(6,1);
   hacop_MC__43->SetBinContent(7,1);
   hacop_MC__43->SetBinContent(8,1);
   hacop_MC__43->SetBinContent(9,1);
   hacop_MC__43->SetBinContent(10,1);
   hacop_MC__43->SetBinContent(11,1);
   hacop_MC__43->SetBinContent(12,1);
   hacop_MC__43->SetBinContent(13,1);
   hacop_MC__43->SetBinContent(14,1);
   hacop_MC__43->SetBinContent(15,1);
   hacop_MC__43->SetBinContent(16,1);
   hacop_MC__43->SetBinContent(17,1);
   hacop_MC__43->SetBinContent(18,1);
   hacop_MC__43->SetBinContent(19,1);
   hacop_MC__43->SetBinContent(20,1);
   hacop_MC__43->SetBinError(1,0.108502);
   hacop_MC__43->SetBinError(2,0.1128781);
   hacop_MC__43->SetBinError(3,0.1184315);
   hacop_MC__43->SetBinError(4,0.1285165);
   hacop_MC__43->SetBinError(5,0.1320405);
   hacop_MC__43->SetBinError(6,0.1428464);
   hacop_MC__43->SetBinError(7,0.1561935);
   hacop_MC__43->SetBinError(8,0.1594155);
   hacop_MC__43->SetBinError(9,0.1779173);
   hacop_MC__43->SetBinError(10,0.1941892);
   hacop_MC__43->SetBinError(11,0.1972918);
   hacop_MC__43->SetBinError(12,0.1966749);
   hacop_MC__43->SetBinError(13,0.2109292);
   hacop_MC__43->SetBinError(14,0.2037807);
   hacop_MC__43->SetBinError(15,0.2139397);
   hacop_MC__43->SetBinError(16,0.2044666);
   hacop_MC__43->SetBinError(17,0.2258742);
   hacop_MC__43->SetBinError(18,0.2305031);
   hacop_MC__43->SetBinError(19,0.22549);
   hacop_MC__43->SetBinError(20,0.1982169);
   hacop_MC__43->SetEntries(108737);
   hacop_MC__43->SetStats(0);
   hacop_MC__43->SetFillColor(1);
   hacop_MC__43->SetFillStyle(3005);
   hacop_MC__43->SetLineStyle(0);

   ci = TColor::GetColor("#ffff00");
   hacop_MC__43->SetMarkerColor(ci);
   hacop_MC__43->SetMarkerStyle(20);
   hacop_MC__43->SetMarkerSize(0);
   hacop_MC__43->GetXaxis()->SetLabelFont(42);
   hacop_MC__43->GetXaxis()->SetLabelOffset(0.01);
   hacop_MC__43->GetXaxis()->SetLabelSize(0.045);
   hacop_MC__43->GetXaxis()->SetTitleSize(0.055);
   hacop_MC__43->GetXaxis()->SetTitleFont(42);
   hacop_MC__43->GetYaxis()->SetLabelFont(42);
   hacop_MC__43->GetYaxis()->SetLabelOffset(0.01);
   hacop_MC__43->GetYaxis()->SetLabelSize(0.045);
   hacop_MC__43->GetYaxis()->SetTitleSize(0.055);
   hacop_MC__43->GetYaxis()->SetTitleOffset(1.3);
   hacop_MC__43->GetYaxis()->SetTitleFont(42);
   hacop_MC__43->GetZaxis()->SetLabelFont(42);
   hacop_MC__43->GetZaxis()->SetLabelSize(0.045);
   hacop_MC__43->GetZaxis()->SetTitleFont(42);
   hacop_MC__43->Draw("E2 same");
   
   TF1 *f_line5 = new TF1("f_line","1.000000",-10000,10000);
   f_line5->SetFillColor(19);
   f_line5->SetFillStyle(0);
   f_line5->SetMarkerStyle(20);

   ci = TColor::GetColor("#ff0000");
   f_line5->SetLineColor(ci);
   f_line5->SetLineWidth(1);
   f_line5->GetXaxis()->SetLabelFont(42);
   f_line5->GetXaxis()->SetLabelOffset(0.007);
   f_line5->GetXaxis()->SetLabelSize(0.05);
   f_line5->GetXaxis()->SetTitleSize(0.06);
   f_line5->GetXaxis()->SetTitleOffset(0.9);
   f_line5->GetXaxis()->SetTitleFont(42);
   f_line5->GetYaxis()->SetLabelFont(42);
   f_line5->GetYaxis()->SetLabelOffset(0.007);
   f_line5->GetYaxis()->SetLabelSize(0.05);
   f_line5->GetYaxis()->SetTitleSize(0.06);
   f_line5->GetYaxis()->SetTitleOffset(1.25);
   f_line5->GetYaxis()->SetTitleFont(42);
   f_line5->Draw("SAME");
   
   TH1D *h_ratio__44 = new TH1D("h_ratio__44","",20,0,0.01);
   h_ratio__44->SetBinContent(1,1.055776);
   h_ratio__44->SetBinContent(2,1.053509);
   h_ratio__44->SetBinContent(3,1.060705);
   h_ratio__44->SetBinContent(4,1.024839);
   h_ratio__44->SetBinContent(5,0.9647397);
   h_ratio__44->SetBinContent(6,1.028044);
   h_ratio__44->SetBinContent(7,0.9347507);
   h_ratio__44->SetBinContent(8,0.9701901);
   h_ratio__44->SetBinContent(9,1.027104);
   h_ratio__44->SetBinContent(10,1.015082);
   h_ratio__44->SetBinContent(11,1.012147);
   h_ratio__44->SetBinContent(12,1.194699);
   h_ratio__44->SetBinContent(13,0.9912543);
   h_ratio__44->SetBinContent(14,1.715204);
   h_ratio__44->SetBinContent(15,1.948907);
   h_ratio__44->SetBinContent(16,1.166181);
   h_ratio__44->SetBinContent(17,1.626674);
   h_ratio__44->SetBinContent(18,1.252111);
   h_ratio__44->SetBinContent(19,2.010431);
   h_ratio__44->SetBinContent(20,2.595284);
   h_ratio__44->SetBinError(1,0.02107762);
   h_ratio__44->SetBinError(2,0.02363411);
   h_ratio__44->SetBinError(3,0.02779803);
   h_ratio__44->SetBinError(4,0.03229541);
   h_ratio__44->SetBinError(5,0.03752403);
   h_ratio__44->SetBinError(6,0.04616054);
   h_ratio__44->SetBinError(7,0.05107088);
   h_ratio__44->SetBinError(8,0.06005326);
   h_ratio__44->SetBinError(9,0.07431866);
   h_ratio__44->SetBinError(10,0.08518383);
   h_ratio__44->SetBinError(11,0.09694612);
   h_ratio__44->SetBinError(12,0.124556);
   h_ratio__44->SetBinError(13,0.1312948);
   h_ratio__44->SetBinError(14,0.1954656);
   h_ratio__44->SetBinError(15,0.255904);
   h_ratio__44->SetBinError(16,0.2129146);
   h_ratio__44->SetBinError(17,0.287558);
   h_ratio__44->SetBinError(18,0.2669509);
   h_ratio__44->SetBinError(19,0.4103775);
   h_ratio__44->SetBinError(20,0.5297601);
   h_ratio__44->SetMinimum(0.1);
   h_ratio__44->SetMaximum(1.9);
   h_ratio__44->SetEntries(802.6498);
   h_ratio__44->SetStats(0);

   ci = 1639;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   h_ratio__44->SetFillColor(ci);
   h_ratio__44->SetLineStyle(0);
   h_ratio__44->SetMarkerStyle(20);
   h_ratio__44->GetXaxis()->SetTitle("Dielectron A_{#phi}");
   h_ratio__44->GetXaxis()->SetMoreLogLabels();
   h_ratio__44->GetXaxis()->SetNoExponent();
   h_ratio__44->GetXaxis()->SetNdivisions(509);
   h_ratio__44->GetXaxis()->SetLabelFont(42);
   h_ratio__44->GetXaxis()->SetLabelOffset(0.007);
   h_ratio__44->GetXaxis()->SetLabelSize(0.15);
   h_ratio__44->GetXaxis()->SetTitleSize(0.1666667);
   h_ratio__44->GetXaxis()->SetTickLength(0.08571428);
   h_ratio__44->GetXaxis()->SetTitleOffset(0.92);
   h_ratio__44->GetXaxis()->SetTitleFont(42);
   h_ratio__44->GetYaxis()->SetTitle("Data/MC");
   h_ratio__44->GetYaxis()->SetNdivisions(505);
   h_ratio__44->GetYaxis()->SetLabelFont(42);
   h_ratio__44->GetYaxis()->SetLabelOffset(0.01);
   h_ratio__44->GetYaxis()->SetLabelSize(0.15);
   h_ratio__44->GetYaxis()->SetTitleSize(0.1666667);
   h_ratio__44->GetYaxis()->SetTitleOffset(0.4125);
   h_ratio__44->GetYaxis()->SetTitleFont(42);
   h_ratio__44->GetZaxis()->SetLabelFont(42);
   h_ratio__44->GetZaxis()->SetLabelSize(0.045);
   h_ratio__44->GetZaxis()->SetTitleFont(42);
   h_ratio__44->Draw("EPSAME");
   
   TH1D *h_ratio_copy__45 = new TH1D("h_ratio_copy__45","",20,0,0.01);
   h_ratio_copy__45->SetBinContent(1,1.055776);
   h_ratio_copy__45->SetBinContent(2,1.053509);
   h_ratio_copy__45->SetBinContent(3,1.060705);
   h_ratio_copy__45->SetBinContent(4,1.024839);
   h_ratio_copy__45->SetBinContent(5,0.9647397);
   h_ratio_copy__45->SetBinContent(6,1.028044);
   h_ratio_copy__45->SetBinContent(7,0.9347507);
   h_ratio_copy__45->SetBinContent(8,0.9701901);
   h_ratio_copy__45->SetBinContent(9,1.027104);
   h_ratio_copy__45->SetBinContent(10,1.015082);
   h_ratio_copy__45->SetBinContent(11,1.012147);
   h_ratio_copy__45->SetBinContent(12,1.194699);
   h_ratio_copy__45->SetBinContent(13,0.9912543);
   h_ratio_copy__45->SetBinContent(14,1.715204);
   h_ratio_copy__45->SetBinContent(15,1.948907);
   h_ratio_copy__45->SetBinContent(16,1.166181);
   h_ratio_copy__45->SetBinContent(17,1.626674);
   h_ratio_copy__45->SetBinContent(18,1.252111);
   h_ratio_copy__45->SetBinContent(19,2.010431);
   h_ratio_copy__45->SetBinContent(20,2.595284);
   h_ratio_copy__45->SetBinError(1,0.02107762);
   h_ratio_copy__45->SetBinError(2,0.02363411);
   h_ratio_copy__45->SetBinError(3,0.02779803);
   h_ratio_copy__45->SetBinError(4,0.03229541);
   h_ratio_copy__45->SetBinError(5,0.03752403);
   h_ratio_copy__45->SetBinError(6,0.04616054);
   h_ratio_copy__45->SetBinError(7,0.05107088);
   h_ratio_copy__45->SetBinError(8,0.06005326);
   h_ratio_copy__45->SetBinError(9,0.07431866);
   h_ratio_copy__45->SetBinError(10,0.08518383);
   h_ratio_copy__45->SetBinError(11,0.09694612);
   h_ratio_copy__45->SetBinError(12,0.124556);
   h_ratio_copy__45->SetBinError(13,0.1312948);
   h_ratio_copy__45->SetBinError(14,0.1954656);
   h_ratio_copy__45->SetBinError(15,0.255904);
   h_ratio_copy__45->SetBinError(16,0.2129146);
   h_ratio_copy__45->SetBinError(17,0.287558);
   h_ratio_copy__45->SetBinError(18,0.2669509);
   h_ratio_copy__45->SetBinError(19,0.4103775);
   h_ratio_copy__45->SetBinError(20,0.5297601);
   h_ratio_copy__45->SetMinimum(0.1);
   h_ratio_copy__45->SetMaximum(1.9);
   h_ratio_copy__45->SetEntries(802.6498);
   h_ratio_copy__45->SetDirectory(0);
   h_ratio_copy__45->SetStats(0);

   ci = 1639;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   h_ratio_copy__45->SetFillColor(ci);
   h_ratio_copy__45->SetLineStyle(0);
   h_ratio_copy__45->SetMarkerStyle(20);
   h_ratio_copy__45->GetXaxis()->SetTitle("Dielectron A_{#phi}");
   h_ratio_copy__45->GetXaxis()->SetMoreLogLabels();
   h_ratio_copy__45->GetXaxis()->SetNoExponent();
   h_ratio_copy__45->GetXaxis()->SetNdivisions(509);
   h_ratio_copy__45->GetXaxis()->SetLabelFont(42);
   h_ratio_copy__45->GetXaxis()->SetLabelOffset(0.007);
   h_ratio_copy__45->GetXaxis()->SetLabelSize(0.15);
   h_ratio_copy__45->GetXaxis()->SetTitleSize(0.1666667);
   h_ratio_copy__45->GetXaxis()->SetTickLength(0.08571428);
   h_ratio_copy__45->GetXaxis()->SetTitleOffset(0.92);
   h_ratio_copy__45->GetXaxis()->SetTitleFont(42);
   h_ratio_copy__45->GetYaxis()->SetTitle("Data/MC");
   h_ratio_copy__45->GetYaxis()->SetNdivisions(505);
   h_ratio_copy__45->GetYaxis()->SetLabelFont(42);
   h_ratio_copy__45->GetYaxis()->SetLabelOffset(0.01);
   h_ratio_copy__45->GetYaxis()->SetLabelSize(0.15);
   h_ratio_copy__45->GetYaxis()->SetTitleSize(0.1666667);
   h_ratio_copy__45->GetYaxis()->SetTitleOffset(0.4125);
   h_ratio_copy__45->GetYaxis()->SetTitleFont(42);
   h_ratio_copy__45->GetZaxis()->SetLabelFont(42);
   h_ratio_copy__45->GetZaxis()->SetLabelSize(0.045);
   h_ratio_copy__45->GetZaxis()->SetTitleFont(42);
   h_ratio_copy__45->Draw("sameaxis");
   BottomPad->Modified();
   acop->cd();
   acop->Modified();
   acop->cd();
   acop->SetSelected(acop);
}
