#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"

const double excleff = 0.984;
const double excleff_err = 0.01;

void qedNorm() {
   TFile *fdata = TFile::Open("outputData.root");
   TFile *fmc = TFile::Open("output.root");

   // estimate the purity in data
   TH1F *hacop_data = (TH1F*) fdata->Get("haco_recoGEDnoaco_passTrigDouble_recoGED");
   TH1F *hacop_mc = (TH1F*) fmc->Get("haco_recoGEDpassTrigDouble_recoGED");

   TF1 *fexp = new TF1("fexp","[0]*exp(-[1]*x) + [2]*exp(-[3]*x)",0,0.06);
   fexp->SetParNames("Norm_sig","Decay_sig","Norm_bkg","Decay_bkg");
   fexp->SetParameters(1.5e4,6.8e2,2e2,1.2e1);

   // normalise MC: force the data and MC integrals to be the same within acop<0.01
   double norm_mc_err,norm_data_err;
   double norm_mc = hacop_mc->IntegralAndError(1,5,norm_mc_err,"width");
   double norm_data = hacop_data->IntegralAndError(1,5,norm_data_err,"width");
   cout << "Scaling MC by " << norm_mc << " / " << norm_data << " = " << norm_mc/norm_data << endl;
   hacop_mc->Scale(norm_data/norm_mc);
   hacop_mc->SetFillColor(kYellow+1);

   TCanvas *c_aco = new TCanvas("c_aco","Acoplanarity");
   c_aco->SetLogy();
   hacop_data->Draw();
   hacop_mc->Draw("hist same");
   hacop_data->Draw("same");
   TFitResultPtr r = hacop_data->Fit(fexp,"ILEMS");
   c_aco->RedrawAxis();

   const double *params = r->GetParams();
   // get the total
   params = r->GetParams();
   for (int i=0; i<4; i++) fexp->SetParameter(i,params[i]);
   double inttot = fexp->Integral(0,0.01);
   double inttot_err = fexp->IntegralError(0,0.01,params,r->GetCovarianceMatrix().GetMatrixArray());
   // set the bkg part to 0, to get the sig part
   double params2[4];
   params2[0] = params[0];
   params2[1] = params[1];
   params2[2] = 0;
   params2[3] = 0;
   for (int i=0; i<4; i++) fexp->SetParameter(i,params2[i]);
   double intsig = fexp->Integral(0,0.01);
   double intsig_err = fexp->IntegralError(0,0.01,params2,r->GetCovarianceMatrix().GetMatrixArray());

   // get the purity
   // according to the fit
   double purity_fit = intsig / inttot;
   double purity_fit_err = purity_fit * sqrt(pow(intsig_err/intsig,2)+pow(inttot_err/inttot,2)-2.*pow(inttot_err,2)/(intsig*inttot));
   // cout << intsig << " " << inttot << endl;
   // cout << intsig_err << " " << inttot_err << endl;
   cout << "Purity according to the fit: " << purity_fit << " +/- " << purity_fit_err << endl;

   // according to counting
   double purity_cnt = intsig / norm_data;
   double purity_cnt_err = purity_cnt * sqrt(pow(intsig_err/intsig,2)+pow(norm_data_err/norm_data,2)-2.*pow(intsig_err,2)/(intsig*norm_data));
   // cout << norm_data << " " << inttot << endl;
   cout << "Purity according to the counting: " << purity_cnt << " +/- " << purity_cnt_err << endl;

}
