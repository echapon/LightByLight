void uncert_gg_ee() {
   TFile *fgg = TFile::Open("scged_photon_efficiency_gen_reco_match_scale_factor.root");
   TFile *fee = TFile::Open("histos_ee.root");
   TH1D *hnum_gg = (TH1D*) fgg->Get("hnum");
   TH1D *hnum_wt_gg = (TH1D*) fgg->Get("hnum_wt");
   TH1D **hnum_uncert_gg = new TH1D*[14];
   for (int i=0; i<14; i++) hnum_uncert_gg[i] = (TH1D*) fgg->Get(Form("hnum_uncert%d",i));
   TH1D *hnum_ee = (TH1D*) fee->Get("hnum");
   TH1D *hnum_wt_ee = (TH1D*) fee->Get("hnum_wt");
   TH1D **hnum_uncert_ee = new TH1D*[14];
   for (int i=0; i<14; i++) hnum_uncert_ee[i] = (TH1D*) fee->Get(Form("hnum_uncert%d",i));

   double ratio[14];

   ratio[0] = (hnum_wt_gg->Integral()/hnum_gg->Integral()) / (hnum_wt_ee->Integral()/hnum_ee->Integral());
   cout << hnum_wt_gg->Integral()/hnum_gg->Integral() << " " <<  (hnum_wt_ee->Integral()/hnum_ee->Integral()) << endl;
   for (int i=1; i<14; i++) {
      ratio[i] = (hnum_uncert_gg[i]->Integral()/hnum_gg->Integral()) / (hnum_uncert_ee[i]->Integral()/hnum_ee->Integral());
      cout << (hnum_uncert_gg[i]->Integral()/hnum_gg->Integral()) << " " <<  (hnum_uncert_ee[i]->Integral()/hnum_ee->Integral()) << " ";
      cout << ratio[i] << endl;
   }

   double uncert=0;
   for (int i=1; i<14; i++) uncert += pow(ratio[i]-ratio[0],2);
   uncert = sqrt(uncert);

   cout << ratio[0] << "+/-" << uncert << " (" << uncert/ratio[0]*100. << "%)" << endl;

}
