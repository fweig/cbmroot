/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

void pl_raw_evt()
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas* can = new TCanvas("can", "can", 48, 56, 800, 800);
  can->Divide(2, 2, 0.01, 0.01);
  //  can->Divide(4,4,0.01,0.01);
  //  can->Divide(2,2,0,0);
  Float_t lsize = 0.01;

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(lsize, "x");
  gStyle->SetLabelSize(lsize, "y");
  gStyle->SetTitleSize(lsize, "x");
  gStyle->SetTitleSize(lsize, "y");

  //gStyle->SetStatY(0.5);
  //gStyle->SetStatX(0.5);
  gStyle->SetStatW(0.2);
  gStyle->SetStatH(0.3);

  gStyle->SetOptStat(kTRUE);
  gStyle->SetOptStat(11);
  gStyle->UseCurrentStyle();

  //gROOT->cd();
  //gROOT->SetDirLevel(2);

  TH1* h;
  TH1* h1;
  TH2* h2;
  TString hname;

  can->cd(1);
  hname = Form("hEvDetMul");
  h     = (TH1*) gROOT->FindObjectAny(hname);
  h->Draw();
  gPad->SetLogy();

  can->cd(2);
  hname = Form("hEvDigiMul");
  h     = (TH1*) gROOT->FindObjectAny(hname);
  h->GetXaxis()->SetRangeUser(0, 3000);
  h->Draw();
  gPad->SetLogy();

  can->cd(3);
  hname = Form("hDigiTdif");
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  h1->Draw("");
  gPad->SetLogz();

  can->cd(4);
  hname = Form("hEvRateIn");
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  h1->Draw("");
  gPad->SetLogz();
  hname = Form("hEvRateOut");
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  h1->SetLineColor(2);
  h1->Draw("same");
  gPad->SetLogz();


  can->SaveAs(Form("pl_raw_evt.pdf"));
}
