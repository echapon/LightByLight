#include "TFile.h"
#include "TF1.h"
#include "Fit/Fitter.h"
#include "Fit/BinData.h"
#include "Fit/Chi2FCN.h"
#include "TH1.h"
#include "TList.h"
#include "Math/WrappedMultiTF1.h"
#include "HFitInterface.h"

// simultaneous fit inspired by $ROOTSYS/tutorials/fit/combinedFit.C

// definition of shared parameters
// "pass" function
int iparP[4] = { 0,      // Norm signal pass
                 1,      // Decay signal
                 2,      // Norm bkg pass
                 3       // Decay bkg
};

// "all" function
int iparA[6] = {  0,     // Norm signal pass
                 1,      // Decay signal
                 2,      // Norm bkg pass
                 3 ,     // Decay bkg
                 4,      // Norm signal fail
                 5       // Norm bkg fail
};

struct GlobalChi2 {
   GlobalChi2(  ROOT::Math::IMultiGenFunction & f1,
                ROOT::Math::IMultiGenFunction & f2) :
      fChi2_1(&f1), fChi2_2(&f2) {}

   // parameter vector is first pass (in common 1 and 2)
   // and then is pass and fail (only in 2)
   double operator() (const double *par) const {
      double p1[4];
      for (int i = 0; i < 4; ++i) p1[i] = par[iparP[i] ];

      double p2[6];
      for (int i = 0; i < 6; ++i) p2[i] = par[iparA[i] ];

      return (*fChi2_1)(p1) + (*fChi2_2)(p2);
   }

   const  ROOT::Math::IMultiGenFunction * fChi2_1;
   const  ROOT::Math::IMultiGenFunction * fChi2_2;
};

void calcEff(TH1F *hpass, TH1F *hall, double &eff, double &eff_err, const char* cname);

void triggerEff(const char* type = "GED") {
   TFile *fdata = TFile::Open("outputData.root");
   TFile *fmc = TFile::Open("output.root");

   TH2F *hacop_pass_EB = (TH2F*) fdata->Get(Form("hacop_pass_EB_%s",type));
   TH1F *hacop_pass_EB_1D = (TH1F*) hacop_pass_EB->ProjectionY();
   TH2F *hacop_all_EB = (TH2F*) fdata->Get(Form("hacop_all_EB_%s",type));
   TH1F *hacop_all_EB_1D = (TH1F*)  hacop_all_EB->ProjectionY();

   double eff, eff_err;
   calcEff(hacop_pass_EB_1D, hacop_all_EB_1D, eff, eff_err, "test.pdf");
   cout << eff << " " << eff_err << endl;
}

void calcEff(TH1F *hpass, TH1F *hall, double &eff, double &eff_err, const char* cname) {
   TF1 *fexp = new TF1("fexp","[0]*exp(-[1]*x) + [2]*exp(-[3]*x)",0,0.06);
   fexp->SetParNames("Norm_sig","Decay_sig","Norm_bkg","Decay_bkg");
   fexp->SetParameters(1.5e4,6.8e2,2e2,1.2e1);
   TF1 *fexp_all = new TF1("fexp_all","[0]*exp(-[1]*x) + [2]*exp(-[3]*x) + [4]*exp(-[1]*x) + [5]*exp(-[3]*x)",0,0.06);
   fexp->SetParNames("Norm_sig","Decay_sig","Norm_bkg","Decay_bkg","Norm_sig_fail","Norm_bkg_fail");
   fexp->SetParameters(1.5e4,6.8e2,2e2,1.2e1,1e2,1);

   // normalise MC: force the data and MC integrals to be the same within acop<0.01
   double norm_all_err,norm_pass_err;
   double norm_all = hall->IntegralAndError(1,5,norm_all_err,"width");
   double norm_pass = hpass->IntegralAndError(1,5,norm_pass_err,"width");
   cout << "Scaling MC by " << norm_all << " / " << norm_pass << " = " << norm_all/norm_pass << endl;
   hall->Scale(norm_pass/norm_all);
   hall->SetFillColor(kYellow+1);

   TCanvas *c_aco = new TCanvas("c_aco","Acoplanarity");

   // do the fit
   ROOT::Math::WrappedMultiTF1 wfP(*fexp,1);
   ROOT::Math::WrappedMultiTF1 wfA(*fexp_all,1);

   ROOT::Fit::DataOptions opt;
   ROOT::Fit::DataRange rangeP;
   // set the data range
   rangeP.SetRange(0,0.06);
   ROOT::Fit::BinData dataP(opt,rangeP);
   ROOT::Fit::FillData(dataP, hpass);

   ROOT::Fit::DataRange rangeA;
   rangeA.SetRange(0,0.06);
   ROOT::Fit::BinData dataA(opt,rangeA);
   ROOT::Fit::FillData(dataA, hall);

   ROOT::Fit::Chi2Function chi2_P(dataP, wfP);
   ROOT::Fit::Chi2Function chi2_A(dataA, wfA);

   GlobalChi2 globalChi2(chi2_P, chi2_A);

   ROOT::Fit::Fitter fitter;

   const int Npar = 6;
   double par0[Npar] = { 1.5e4,6.8e2,2e2,1.2e1,1e2,1 };

   // create before the parameter settings in order to fix or set range on them
   fitter.Config().SetParamsSettings(6,par0);

   fitter.Config().MinimizerOptions().SetPrintLevel(0);
   fitter.Config().SetMinimizer("Minuit2","Migrad");

   // fit FCN function directly
   // (specify optionally data size and flag to indicate that is a chi2 fit)
   fitter.FitFCN(6,globalChi2,0,dataP.Size()+dataA.Size(),true);
   ROOT::Fit::FitResult result = fitter.Result();
   result.Print(std::cout);

   const double *params = result.GetParams();

   // return the efficiency and error
   double p = params[0];
   double f = params[4];
   eff = p/(p+f);
   eff_err = sqrt(p*p*result.CovMatrix(0,0)/pow(p+f,4) + f*f*result.CovMatrix(4,4)/pow(p+f,4) - 2.*p*f*result.CovMatrix(0,4)/pow(p+f,4));

   c_aco->Divide(1,2);
   c_aco->cd(1);
   gPad->SetLogy();
   gStyle->SetOptFit(1111);

   fexp->SetFitResult( result, iparP);
   fexp->SetRange(rangeP().first, rangeP().second);
   fexp->SetLineColor(kBlue);
   hpass->GetListOfFunctions()->Add(fexp);
   hpass->Draw();

   c_aco->cd(2);
   gPad->SetLogy();
   fexp_all->SetFitResult( result, iparA);
   fexp_all->SetRange(rangeA().first, rangeA().second);
   fexp_all->SetLineColor(kRed);
   hall->GetListOfFunctions()->Add(fexp_all);
   hall->Draw();
   c_aco->SaveAs(cname);

   delete c_aco;
   delete fexp;
   delete fexp_all;
}
