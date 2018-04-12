/////////////////////////////////////////////////////////////////////////
//
// SPlot tutorial
// author: Kyle Cranmer
// date Dec. 2008
//
// This tutorial shows an example of using SPlot to unfold two distributions.
// The physics context for the example is that we want to know
// the nextra_tracks distribution for real electrons from signal events
// and fake electrons from background.  Isolation is our 'control' variable
// To unfold them, we need a model for an uncorrelated variable that
// discriminates between signal and background.  To do this, we use the invariant
// mass of two electrons.  We model the signal with a Gaussian and the background
// with a falling exponential.
//
// Note, since we don't have real data in this tutorial, we need to generate
// toy data.  To do that we need a model for the nextra_tracks variable for
// both signal and background.  This is only used to generate the toy data, and would
// not be needed if we had real data.
/////////////////////////////////////////////////////////////////////////

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooStats/SPlot.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooAddition.h"
#include "RooProduct.h"
#include "TCanvas.h"
#include "RooAbsPdf.h"
#include "RooFit.h"
#include "RooFitResult.h"
#include "RooWorkspace.h"
#include "RooConstVar.h"

// use this order for safety on library loading
using namespace RooFit ;
using namespace RooStats ;


// see below for implementation
void AddModel(RooWorkspace*);
void AddData(RooWorkspace*, const char* filename, const char* treename);
void DoSPlot(RooWorkspace*);
void MakePlots(RooWorkspace*, const char* varname);

void splot(const char* filename="output.root", const char* treename="trGED", const char* varname="nextra_track")
{

  // Create a new workspace to manage the project.
  RooWorkspace* wspace = new RooWorkspace("myWS");

  // add the signal and background models to the workspace.
  // Inside this function you will find a discription our model.
  AddModel(wspace);

  // add some toy data to the workspace
  AddData(wspace, filename, treename);

  // inspect the workspace if you wish
   wspace->Print();

  // do sPlot.
  //This wil make a new dataset with sWeights added for every event.
  DoSPlot(wspace);

  // Make some plots showing the discriminating variable and
  // the control variable after unfolding.
  MakePlots(wspace, varname);

  // cleanup
  delete wspace;

}


//____________________________________
void AddModel(RooWorkspace* ws){
   RooRealVar acop("acop","Acoplanarity",0,0.2);
   RooRealVar decayB("decayB","decay bkg",-1.2e1,-100.,0.);
   RooRealVar decayS("decayS","decay signal",-6.8e2,-1000.,-100.);
   RooRealVar NS("NS","N signal",1.5e4,0,1e6);
   RooRealVar NB("NB","N bkg",2e2,0,1e6);
   RooExponential signal("signal","signal",acop,decayS);
   RooExponential bkg("bkg","bkg",acop,decayB);
   RooAddPdf model("model","model",RooArgList(signal,bkg),RooArgList(NS,NB));


  ws->import(model);
}

//____________________________________
void AddData(RooWorkspace* ws, const char* filename, const char* treename){
   TFile *f = TFile::Open(filename);
   TTree *tr = (TTree*) f->Get(treename);

   RooRealVar acop("acop","Acoplanarity",0,0.2);
   RooRealVar mass("mass","Mass",0,200);
   RooRealVar pt("pt","p_{T}",0,200);
   RooRealVar deltapt("deltapt","#delta p_{T}",0,5);
   RooRealVar rap("rap","Rapidity",-3,3);
   RooRealVar nextra_track("nextra_track","N(extra tracks)",0,10);
   RooRealVar nhits1("nhits1","N(hits, 1st layer)",0,10);
   RooRealVar nhits2("nhits2","N(hits, 2st layer)",0,10);
   RooRealVar nhits3("nhits3","N(hits, 3st layer)",0,10);
   RooRealVar nhits4("nhits4","N(hits, 4st layer)",0,10);
   RooRealVar nhits5("nhits5","N(hits, 5st layer)",0,10);

   // RooArgSet cols(acop,mass,pt,deltapt,rap,nextra_track,nhits1,nhits2,nhits3,nhits4,nhits5);
   RooArgSet cols(acop,mass,pt,deltapt,rap,nextra_track);
   RooArgSet cols2(nhits1,nhits2,nhits3,nhits4,nhits5);
   cols.add(cols2);

   RooDataSet data("data","data",cols) ;

   // import by hand (cannot use Import(tr) because we have integers)
   float acopF; tr->SetBranchAddress("acop",&acopF);
   float massF; tr->SetBranchAddress("mass",&massF);
   float ptF; tr->SetBranchAddress("pt",&ptF);
   float deltaptF; tr->SetBranchAddress("deltapt",&deltaptF);
   float rapF; tr->SetBranchAddress("rap",&rapF);
   int nextra_trackI; tr->SetBranchAddress("nextra_track",&nextra_trackI);
   int nhits1I; tr->SetBranchAddress("nhits1",&nhits1I);
   int nhits2I; tr->SetBranchAddress("nhits2",&nhits2I);
   int nhits3I; tr->SetBranchAddress("nhits3",&nhits3I);
   int nhits4I; tr->SetBranchAddress("nhits4",&nhits4I);
   int nhits5I; tr->SetBranchAddress("nhits5",&nhits5I);

   for (int i=0; i<tr->GetEntries(); i++) {
      tr->GetEntry(i);
      acop.setVal(acopF);
      mass.setVal(massF);
      pt.setVal(ptF);
      deltapt.setVal(deltaptF);
      rap.setVal(rapF);
      nextra_track.setVal(nextra_trackI);
      nhits1.setVal(nhits1I);
      nhits2.setVal(nhits2I);
      nhits3.setVal(nhits3I);
      nhits4.setVal(nhits4I);
      nhits5.setVal(nhits5I);
      data.add(cols);
   }

   // import data into workspace
   ws->import(data, Rename("data"));

}

//____________________________________
void DoSPlot(RooWorkspace* ws){
  std::cout << "Calculate sWeights" << std::endl;

  // get what we need out of the workspace to do the fit
  RooAbsPdf* model = ws->pdf("model");
  RooRealVar* acop = ws->var("acop");
  RooRealVar* NS = ws->var("NS");
  RooRealVar* NB = ws->var("NB");
  RooDataSet* data = (RooDataSet*) ws->data("data");

  // RooPlot *xframe = acop->frame();
  // data->plotOn(xframe);
  // xframe->Draw();

  // fit the model to the data.
  model->fitTo(*data, Extended() );

  // The sPlot technique requires that we fix the parameters
  // of the model that are not yields after doing the fit.
  RooRealVar* decayS = ws->var("decayS");
  RooRealVar* decayB = ws->var("decayB");
  decayS->setConstant();
  decayB->setConstant();



  RooMsgService::instance().setSilentMode(true);


  // Now we use the SPlot class to add SWeights to our data set
  // based on our model and our yield variables
  RooStats::SPlot* sData = new RooStats::SPlot("sData","An SPlot",
                                               *data, model, RooArgList(*NS,*NB) );


  // Check that our weights have the desired properties

  std::cout << "Check SWeights:" << std::endl;


  std::cout << std::endl <<  "Yield of signal is "
            << NS->getVal() << ".  From sWeights it is "
            << sData->GetYieldFromSWeight("NS") << std::endl;


  std::cout << "Yield of background is "
            << NB->getVal() << ".  From sWeights it is "
            << sData->GetYieldFromSWeight("NB") << std::endl
            << std::endl;

  for(Int_t i=0; i < 10; i++)
    {
      std::cout << "signal Weight   " << sData->GetSWeight(i,"NS")
                << "   background Weight   " << sData->GetSWeight(i,"NB")
                << "  Total Weight   " << sData->GetSumOfEventSWeight(i)
                << std::endl;
    }

  std::cout << std::endl;

  // import this new dataset with sWeights
 std::cout << "import new dataset with sWeights" << std::endl;
 ws->import(*data, Rename("dataWithSWeights"));


}

void MakePlots(RooWorkspace* ws, const char* varname){

  // Here we make plots of the discriminating variable (acop) after the fit
  // and of the control variable (nextra_tracks) after unfolding with sPlot.
  std::cout << "make plots" << std::endl;

  // make our canvas
  TCanvas* cdata = new TCanvas("sPlot","sPlot demo", 1600, 400);
  cdata->Divide(3,1);

  // get what we need out of the workspace
  RooAbsPdf* model = ws->pdf("model");
  RooAbsPdf* signal = ws->pdf("signal");
  RooAbsPdf* bkg = ws->pdf("bkg");

  RooRealVar* exclvar = ws->var(varname);
  RooRealVar* acop = ws->var("acop");

  // note, we get the dataset with sWeights
  RooDataSet* data = (RooDataSet*) ws->data("dataWithSWeights");

  // this shouldn't be necessary, need to fix something with workspace
  // do this to set parameters back to their fitted values.
  model->fitTo(*data, Extended() );

  //plot acop for data with full model and individual componenets overlayed
  //  TCanvas* cdata = new TCanvas();
  cdata->cd(1);
  gPad->SetLogy();
  RooPlot* frame = acop->frame(Range(0,0.06),Bins(30)) ;
  data->plotOn(frame ) ;
  model->plotOn(frame) ;
  model->plotOn(frame,Components(*signal),LineStyle(kDashed), LineColor(kRed)) ;
  model->plotOn(frame,Components(*bkg),LineStyle(kDashed),LineColor(kGreen)) ;

  frame->SetTitle("Fit of model to discriminating variable");
  frame->Draw() ;


  // Now use the sWeights to show exclvar distribution for signal and background.
  // The SPlot class can make this easier, but here we demonstrait in more
  // detail how the sWeights are used.  The SPlot class should make this
  // very easy and needs some more development.

  // Plot exclvar for signal component.
  // Do this by plotting all events weighted by the sWeight for the signal component.
  // The SPlot class adds a new variable that has the name of the corresponding
  // yield + "_sw".
  cdata->cd(2);

  // create weightfed data set
  RooDataSet * dataw_sig = new RooDataSet(data->GetName(),data->GetTitle(),data,*data->get(),0,"NS_sw") ;

  RooPlot* frame2 = exclvar->frame(Bins(10)) ;
  dataw_sig->plotOn(frame2, DataError(RooAbsData::SumW2) ) ;

  frame2->SetTitle(Form("%s distribution for signal",varname));
  frame2->Draw() ;
  // frame2->getHist()->SaveAs(Form("%s_sOnly.root",varname));

  // Plot exclvar for background component.
  // Eg. plot all events weighted by the sWeight for the background component.
  // The SPlot class adds a new variable that has the name of the corresponding
  // yield + "_sw".
  cdata->cd(3);
  RooDataSet * dataw_bkg = new RooDataSet(data->GetName(),data->GetTitle(),data,*data->get(),0,"NB_sw") ;
  RooPlot* frame3 = exclvar->frame(Bins(10)) ;
  dataw_bkg->plotOn(frame3,DataError(RooAbsData::SumW2) ) ;

  frame3->SetTitle(Form("%s distribution for background",varname));
  frame3->Draw() ;

  //  cdata->SaveAs("SPlot.gif");

  // charged track exclusivity
  if (TString(varname)=="nextra_track") {
     double ntot = dataw_sig->sumEntries();
     double npass = dataw_sig->sumEntries("nextra_track==0");
     double ntot2 = data->sumEntries();
     double npass2 = data->sumEntries("nextra_track==0");
     double eff = npass/ntot;
     double eff2 = npass2/ntot2;
     double eff_err_up = TEfficiency::ClopperPearson(ntot2,npass2, 0.683, true)-eff2;
     double eff_err_down = eff2-TEfficiency::ClopperPearson(ntot2,npass2, 0.683, false);
     cout << "nextra_track==0 efficiency: " << eff << " +" << eff_err_up << " -" << eff_err_down << endl;

     // cut based
     double ntot3 = data->sumEntries("acop<0.005");
     double npass3 = data->sumEntries("acop<0.005&&nextra_track==0");
     double eff3 = npass3/ntot3;
     double eff3_err_up = TEfficiency::ClopperPearson(ntot3,npass3, 0.683, true)-eff3;
     double eff3_err_down = eff3-TEfficiency::ClopperPearson(ntot3,npass3, 0.683, false);
     cout << "nextra_track==0 efficiency (cut-based): " << eff3 << " +" << eff3_err_up << " -" << eff3_err_down << endl;
  }
}
