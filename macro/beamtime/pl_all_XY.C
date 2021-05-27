void pl_all_XY(Int_t iNDet = 24, Int_t iMode = 0, Double_t dMax = 0.)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas* can = new TCanvas("can", "can", 0, 0, 1200, 800);
  Float_t lsize;
  if (iMode == 0) switch (iNDet) {
      case 24:
        can->Divide(5, 5, 0.01, 0.01);
        //  can->Divide(2,2,0,0);
        lsize = 0.03;
        break;
      case 9:
        can->Divide(3, 3, 0.01, 0.01);
        //  can->Divide(2,2,0,0);
        lsize = 0.03;
        break;
    }

  gROOT->cd();
  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(lsize);

  //gStyle->SetOptStat(kTRUE);
  //gROOT->cd();
  //gROOT->SetDirLevel(2);

  TH1* h;
  TH2* h2;
  // if (h!=NULL) h->Delete();
  for (Int_t iCh = 0; iCh < iNDet; iCh++) {
    gROOT->cd();
    TString hname = Form("hXY_SmT%d", iCh);
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->UseCurrentStyle();
      h2->GetYaxis()->SetLabelSize(lsize);
      if (dMax > 0) h2->SetMaximum(dMax);
      switch (iMode) {
        case 0:
          can->cd(iCh + 1);
          h2->Draw("colz");
          gPad->SetLogz();
          break;
        case 1:
          if (iCh == 0) h2->Draw("colz");
          else
            h2->Draw("colz same");
          break;
      }
    }
    else {
      cout << "Histogram " << hname << " not existing. " << endl;
    }
  }
}
