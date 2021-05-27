/** @file pl_XY_3D.C
  * @copyright Copyright (C) 2017-2018 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann [orginator] **/

void pl_XY_3D(Int_t iNSt = 4, Int_t iOpt = 0, Int_t i2D = 0, Int_t i1D = 0, Double_t dYmax = 0.)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 800, 800);
  can->Divide(1, 1);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kFALSE);

  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1* h;
  TH1* h1;
  TH2* h2;
  TH3* h3;
  TH3* h3f;

  TString hname;
  TString cOpt;
  TString c2D;
  TString c1D;

  const Int_t ColMap[10] = {1, 2, 3, 4, 6, 7, 8, 9, 41, 46};

  Int_t iCan = 1;
  gROOT->cd();
  switch (iOpt) {
    case 0: cOpt = "DX"; break;
    case 1: cOpt = "DY"; break;
    case 2: cOpt = "DT"; break;
    case 3: cOpt = "TOT"; break;
    case 4: cOpt = "CSZ"; break;
  }

  switch (i2D) {
    case 0: c2D = "yx"; break;
    case 1: c2D = "zx"; break;
    default: cout << "i2D mode not implemented " << endl; return;
  }

  can->cd(iCan);

  for (Int_t iSt = 0; iSt < iNSt; iSt++) {
    hname = Form("hXY_%s_%d", cOpt.Data(), iSt);
    h3f   = (TH3*) gROOT->FindObjectAny(hname);
    cout << hname.Data() << " with pointer  " << h3f << " at iCan = " << iCan << endl;
    if (h3f != NULL) {
      switch (i1D) {
        case 0:
          h = (TH1*) h3f->Project3DProfile(c2D.Data())->ProfileX();
          h->SetTitle(Form("%s", h->GetName()));
          //h->GetXaxis()->SetTitle("???");
          break;
        case 1:
          h = (TH1*) h3f->Project3DProfile(c2D.Data())->ProfileY();
          h->SetTitle(Form("%s", h->GetName()));
          break;
      }
      h->SetLineColor(ColMap[iSt]);
      if (iSt == 0) {
        h->Draw();
        if (dYmax > 0.) {
          h->SetMinimum(-dYmax);
          h->SetMaximum(dYmax);
        }
      }
      else {
        h->Draw("same");
      }
      gPad->SetGridx();
      gPad->SetGridy();
      //gPad->SetLogy();
    }
    else
      cout << hname << " not found" << endl;
  }
  can->SaveAs("pl_XY_3D.pdf");
}
