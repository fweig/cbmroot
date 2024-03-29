/* Copyright (C) 2016 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can23", "can23", 48, 55, 700, 900);
  can->Divide(2, 4);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);
  cout << " DirLevel " << gROOT->GetDirLevel() << endl;

  TH1* h;
  TH2* h2;
  // if (hPla!=NULL) hPla->Delete();

  can->cd(1);
  gROOT->cd();
  TString hname = "hDXDY24";
  h2            = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) { h2->Draw("colz"); }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(2);
  gROOT->cd();
  TString hname = "hDXDT24";
  h2            = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) { h2->Draw("colz"); }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(3);
  gROOT->cd();
  TString hname = "hDYDT24";
  h2            = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) { h2->Draw("colz"); }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(4);
  gROOT->cd();
  TString hname = "hChi24";
  h1            = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) { h1->Draw(); }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(5);
  gROOT->cd();
  TString hname = "hYY24";
  h2            = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) { h2->Draw("colz"); }
  else {
    cout << hname << " not found" << endl;
  }


  can->cd(6);
  gROOT->cd();
  TString hname = "hNMatch24";
  h1            = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    h1->Draw();
    gPad->SetLogy();
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(7);
  gROOT->cd();
  TString hname = "hDT24sel";
  h1            = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    h1->Draw();
    gPad->SetLogy();
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(8);
  gROOT->cd();
  TString hname = "hDT224sel";
  h1            = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    h1->Draw();
    h1->SetLineColor(2);
    gPad->SetLogy();
  }
  else {
    cout << hname << " not found" << endl;
  }
}
