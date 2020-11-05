void fit_yPos(Int_t SmT = 0, Int_t iSm = 0, Int_t iRpc = 0) {
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 800, 800);
  can->Divide(1, 2);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);
  gStyle->SetOptFit(111);

  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1* h;
  TH1* h1;
  TH2* h2;
  TH1* h2y;
  void fit_ybox(const char* hname);
  void fit_ybox(const char* hname, Double_t ysize);
  // if (h!=NULL) h->Delete();

  can->cd(1);
  gROOT->cd();
  gROOT->LoadMacro("fit_ybox.h");
   ROOT::Math::Minimizer* minimum =
      ROOT::Math::Factory::CreateMinimizer("Minuit", "Migrad");
  minimum->SetMaxFunctionCalls(100000);
  minimum->SetTolerance(0.1);
  minimum->SetPrintLevel(3);

  TFitter::SetMaxIterations(500000);
  TFitter::SetPrecision(0.1);
  TFitter::SetDefaultFitter("Minuit2");

  TString hname2 = Form("cl_SmT%d_sm%03d_rpc%03d_Pos", SmT, iSm, iRpc);
  h2             = (TH2*) gROOT->FindObjectAny(hname2);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");

    can->cd(2);
    h2y = h2->ProjectionY();
    cout << " Fit with ybox " << h2y->GetName() << endl;
    fit_ybox((const char*) (h2y->GetName()));
    if( 0) { //NULL != gMinuit ) {
      cout << "Minuit ended with " << gMinuit->fCstatu<<endl;
    }
  } else {
    cout << hname2 << " not found" << endl;
  }


  can->SaveAs(Form("Ypos%01d_%01d_%01d.pdf", SmT, iSm, iRpc));
}
