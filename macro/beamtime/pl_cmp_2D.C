/* Copyright (C) 2022 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void pl_cmp_2D(TString hname = "cl_SmT0_sm003_rpc001_Tot", Int_t iOpt = 1,
               TString fName1 = "1091.50.0.0/CluStatus42042042_-1_Cal_1044.50.4.0_set031041500.hst.root",
               TString fName2 = "1111.50.0.0/CluStatus42042042_-1_Cal_1044.50.4.0_set031041500.hst.root")
{
  //plot initialisation
  TCanvas* can = new TCanvas("can", "can", 50, 50, 686, 686);
  can->Divide(1, 1, 0.01, 0.01);
  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);
  gStyle->SetOptStat(" ");
  gStyle->SetOptStat(10);
  //cout << "gStyle label size: "<<  gStyle->GetLabelSize() << endl;

  // file opening

  TFile* f1 = new TFile(fName1.Data());
  TH2* h21  = (TH2*) f1->Get(hname.Data())->Clone();

  TFile* f2 = new TFile(fName2.Data());
  TH2* h22  = (TH2*) f2->Get(hname.Data())->Clone();
  //  f2->Close();
  gDirectory->cd();
  //  cout << Form("h21 %p, h22 %p",h21,h22)<<endl;

  TH1* h1;
  TH1* h2;

  switch (iOpt % 10) {
    case 0:  // x-projection
      h1 = (TH1*) h21->Clone();
      h2 = (TH1*) h22->Clone();
      ;
      break;

    case 10:
    case 1:  // y- projections
      h1 = (TH1*) h21->ProjectionY(Form("%s_py1", h21->GetName()));
      h2 = (TH1*) h22->ProjectionY(Form("%s_py2", h22->GetName()));
      break;

    case 2:  // x-projection with box fit
      ;
      break;

    default: cout << "Option " << iOpt << " not available " << endl; return;
  }

  can->cd(1);

  h1->Draw("");
  h1->SetLineColor(kRed);
  h2->SetLineColor(kBlue);
  Double_t N1 = h1->GetEntries();
  Double_t N2 = h2->GetEntries();
  TH1* h1s    = (TH1*) h1->Clone();
  cout << "Scale " << h1s->GetName() << " by " << N2 / N1 << endl;
  h1s->Scale(N2 / N1);
  h2->SetMaximum(TMath::Max(h1s->GetMaximum(), h2->GetMaximum()) * 1.2);
  h2->Draw("he");
  h1s->Draw("samehe");

  TLegend* leg = new TLegend(0.2, 0.82, 0.85, 0.9);
  leg->SetTextSize(0.015);
  leg->AddEntry(h1s, fName1, "l");
  leg->AddEntry(h2, fName2, "l");
  leg->Draw();
  if (iOpt > 9) { gPad->SetLogy(); }
}
