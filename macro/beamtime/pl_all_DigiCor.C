void pl_all_DigiCor(Int_t iNDet = 1) {
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 48, 55, 900, 900);
  //TCanvas *can = new TCanvas("can","can",48,56,900,700);
  //can->Divide(4,4,0.01,0.01);
  //  can->Divide(2,3,0.01,0.01);
  can->Divide(5, 5, 0.01, 0.01);
  Float_t lsize = 0.07;

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(lsize);

  //gStyle->SetOptStat(kTRUE);
  //gROOT->cd();
  //gROOT->SetDirLevel(2);

  TH1* h;
  TH2* h2;
  Int_t iType[6]   = {0, 1, 6, 5, 9, 8};
  Int_t iNumSm[6]  = {5, 3, 1, 1, 3, 2};
  Int_t iNumRpc[6] = {5, 3, 2, 1, 2, 1};
  Int_t iCanv      = 0;
  // if (h!=NULL) h->Delete();

  for (Int_t iCh = 0; iCh < iNDet; iCh++) {
    for (Int_t iSm = 0; iSm < iNumSm[iCh]; iSm++) {
      for (Int_t iRpc = 0; iRpc < iNumRpc[iCh]; iRpc++) {
        can->cd(iCanv + 1);
        iCanv++;
        gROOT->cd();
        TString hname =
          Form("cl_SmT%01d_sm%03d_rpc%03d_DigiCor", iType[iCh], iSm, iRpc);
        h2 = (TH2*) gROOT->FindObjectAny(hname);
        if (h2 != NULL) {
          h2->Draw("colz");
          //     gPad->SetLogy();
        } else {
          cout << "Histogram " << hname << " not existing. " << endl;
        }
      }
    }
  }
  can->SaveAs(Form("pl_all_DigiCor.pdf"));
}
