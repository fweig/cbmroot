/* Copyright (C) 2020 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void pl_all_DigiTot(Int_t iNDet = 2)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 48, 55, 900, 900);
  //TCanvas *can = new TCanvas("can","can",48,56,900,700);
  //can->Divide(4,4,0.01,0.01);
  //  can->Divide(2,3,0.01,0.01);
  can->Divide(4, 3, 0.01, 0.01);
  Float_t lsize = 0.07;

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(lsize);

  //gStyle->SetOptStat(kTRUE);
  //gROOT->cd();
  //gROOT->SetDirLevel(2);

  TH1* h;
  TH2* h2;
  const Int_t iType[6]   = {9, 6, 7, 8, 6, 8};
  const Int_t iSmNum[6]  = {3, 2, 1, 1, 1, 1};
  const Int_t iRpcNum[6] = {2, 2, 1, 8, 2, 8};

  Int_t iCanv = 0;
  // if (h!=NULL) h->Delete();

  for (Int_t iCh = 0; iCh < iNDet; iCh++) {
    for (Int_t iSm = 0; iSm < iSmNum[iCh]; iSm++) {
      for (Int_t iRpc = 0; iRpc < iRpcNum[iCh]; iRpc++) {
        can->cd(iCanv + 1);
        iCanv++;
        gROOT->cd();
        TString hname = Form("hDigiTot_SmT%01d_sm%03d_rpc%03d", iType[iCh], iSm, iRpc);
        h2            = (TH2*) gROOT->FindObjectAny(hname);
        if (h2 != NULL) {
          h2->Draw("colz");
          gPad->SetLogz();
        }
        else {
          cout << "Histogram " << hname << " not existing. " << endl;
        }
      }
    }
  }
  can->SaveAs(Form("pl_all_DigiTot.pdf"));
}
