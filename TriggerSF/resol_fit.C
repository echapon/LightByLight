void resol_fit() {
   double x[6], dx[6], mean[6], resol[6], nevt[6], dmean[6], dresol[6];
   for (int i=0; i<6; i++) dx[i] = 1.25;
   x[0] = 6.25; mean[0] = 0.9893; resol[0] = 0.06092; nevt[0] = 4542;
   x[1] = 8.75; mean[1] = 0.9575; resol[1] = 0.07542; nevt[1] = 1568;
   x[2] = 11.25; mean[2] = 0.9372; resol[2] = 0.0891; nevt[2] = 673;
   x[3] = 13.75; mean[3] = 0.9286; resol[3] = 0.0846; nevt[3] = 274;
   x[4] = 16.25; mean[4] = 0.9286; resol[4] = 0.08925; nevt[4] = 158;
   x[5] = 18.75; mean[5] = 0.9269; resol[5] = 0.09091; nevt[5] = 82;

   for (int i=0; i<6; i++) {
      dmean[i] = resol[i]/sqrt(nevt[i]);
      dresol[i] = 0;
   }

   TF1 *fm = new TF1("fm","[0]+[1]*exp([2]*x)",5,20);
   fm->SetParameters(0.93,0.4,-0.3);
   TF1 *fr = new TF1("fr","[0]+[1]*exp([2]*x)",5,20);
   fr->SetParameters(0.1,-0.2,-0.2);

   TCanvas *c1 = new TCanvas();
   TH1F *haxes1 = new TH1F("haxes1",";m (#gamma#gamma) [GeV];Relative mass scale",1,0,20);
   TGraphErrors *g1 = new TGraphErrors(6,x,mean,dx,dmean);
   haxes1->Draw();
   haxes1->GetYaxis()->SetRangeUser(0.9,1.05);
   g1->Draw("P");
   g1->Fit(fm);

   TCanvas *c2 = new TCanvas();
   TH1F *haxes2 = new TH1F("haxes2",";m (#gamma#gamma) [GeV];Mass resolution",1,0,20);
   TGraphErrors *g2 = new TGraphErrors(6,x,resol,dx,dresol);
   haxes2->Draw();
   haxes2->GetYaxis()->SetRangeUser(0.05,0.12);
   g2->Draw("P");
   g2->Fit(fr,"W");
}
