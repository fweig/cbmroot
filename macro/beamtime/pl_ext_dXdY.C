/* Copyright (C) 2021 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void pl_ext_dXdY(Int_t iLev = 0, Int_t NSt = 12)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 1200, 800);
  can->Divide(4, 3);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1* h;
  TH1* h1;
  TH2* h2;
  TString hname = "";

  // if (h!=NULL) h->Delete();

  Int_t iPad = 0;
  for (Int_t iSt = 0; iSt < NSt; iSt++) {
    iPad++;
    can->cd(iPad);
    gROOT->cd();
    hname = Form("hTrkPosCor%d_St%d", iLev, iSt);
    h2    = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
      h2->ProfileX()->Draw("same");
    }
    else {
      cout << hname << " not found" << endl;
    }
  }


  can->SaveAs("pl_ext_dXdY.pdf");
}
