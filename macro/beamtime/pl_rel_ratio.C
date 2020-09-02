
void set_plot_style() {
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = {0.00, 0.34, 0.61, 0.84, 1.00};
  Double_t red[NRGBs]   = {0.00, 0.00, 0.87, 1.00, 0.51};
  Double_t green[NRGBs] = {0.00, 0.81, 1.00, 0.20, 0.00};
  Double_t blue[NRGBs]  = {0.51, 1.00, 0.12, 0.00, 0.00};
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
  cout << "set_plot_style finished" << endl;
}

void pl_rel_ratio(TString hname    = "hDutDTLH_DD_Found_911",
                  Int_t iRefBin    = 1,
                  Int_t iDim       = 0,
                  Int_t iDisBin    = 0xFF,
                  Double_t yRange  = 1.,
                  Double_t dYShift = 0.1,
                  Double_t Xmin    = 0.,
                  Double_t Xmax    = 10.) {
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 600, 800);
  can->Divide(1, 2);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  set_plot_style();
  gStyle->SetOptStat(kFALSE);
  //TExec *ex1 = new TExec("ex1","Pal1();");

  // gROOT->cd();
  // gROOT->SetDirLevel(1);

  TH1* h;
  TH1* h1;
  TH2* h2;
  TH2* h2C;
  // if (h!=NULL) h->Delete();
  Int_t iNBins = 0;

  can->cd(1);
  // gROOT->cd();
  gDirectory->pwd();
  h2 = (TH2*) gDirectory->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
  } else {
    cout << hname << " not found" << endl;
    return;
  }

  can->cd(2);

  TH1D* href;
  TH1D* hbin;
  switch (iDim) {
    case 0:
      iNBins = h2->GetNbinsX();
      href =
        h2->ProjectionX(Form("%sRef", hname.Data()), iRefBin + 1, iRefBin + 1);
      href->Scale(1. / href->GetEntries());
      break;
    case 1: iNBins = h2->GetNbinsY(); break;
  }
  href->Draw();

  for (Int_t iBin = 0; iBin < iNBins; iBin++) {
    hbin =
      h2->ProjectionX(Form("%sBin%d", hname.Data(), iBin), iBin + 1, iBin + 1);
    hbin->Scale(1. / hbin->GetEntries());
    hbin->Divide(hbin, href, 1, 1, "E");
    hbin->SetLineColor(iBin + 1);
    if (iBin == 0) {
      hbin->Draw("Lhist");
      hbin->SetMinimum(0.);
      hbin->SetMaximum(yRange + iNBins * dYShift);
      hbin->GetXaxis()->SetRangeUser(Xmin, Xmax);
    } else {
      Int_t bit = 1;
      if (iDisBin & (bit <<= iBin)) hbin->Draw("same Lhist");
    }
    //  h2->UseCurrentStyle();
    //  gPad->SetLogz();
  }

  /*
can->cd(2);
 for (Int_t iCh=0; iCh<iNch; iCh++){
   TString hname=Form("Cor_SmT%d_sm%03d_rpc%03d_Ch%03d_S1_Walk_px",SmT,iSm,iRpc,iCh);
 h2=(TH2 *)gDirectory->FindObjectAny(hname);
 if (h2!=NULL) {
  h2->SetLineColor(iCh+1);
  if (iCh==0) {
    h2->Draw("Lhist");
    h2->SetMinimum(-yRange);
    h2->SetMaximum(yRange+iNch*dYShift);
    h2->GetXaxis()->SetRangeUser(Xmin,Xmax);
  }else{
    h2C=(TH2 *)h2->Clone();
    Int_t iNB=h2C->GetNbinsX();
    for (Int_t iB=0; iB<iNB; iB++){
      h2C->SetBinContent(iB+1,h2C->GetBinContent(iB+1)+dYShift*iCh);
    }
    h2C->Draw("same Lhist");
  }
  //  gPad->SetLogz();
 }else { cout << hname << " not found" << endl;  }
 }
 */
}
