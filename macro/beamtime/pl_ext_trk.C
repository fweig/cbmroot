/* Copyright (C) 2021 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void pl_ext_trk(Int_t iLev = 1)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 800, 800);
  can->Divide(4, 4);

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

  can->cd(1);
  gROOT->cd();
  hname = Form("hTrkStationDX%d", iLev);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(2);
  gROOT->cd();
  hname = Form("hTrkStationDY%d", iLev);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(3);
  gROOT->cd();
  hname = Form("hTrkStationDT%d", iLev);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(4);
  gROOT->cd();
  hname = Form("hTrkStationNHits%d", iLev);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(5);
  gROOT->cd();
  hname = Form("hTrkPullDX%d", iLev);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(6);
  gROOT->cd();
  hname = Form("hTrkPullDY%d", iLev);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(7);
  gROOT->cd();
  hname = Form("hTrkPullDT%d", iLev);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(8);
  gROOT->cd();
  hname = Form("hExt_TrkSizVel%d", iLev);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(9);
  gROOT->cd();
  hname = Form("hExt_Xoff");
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) { h1->Draw(""); }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(10);
  gROOT->cd();
  hname = Form("hExt_Yoff");
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) { h1->Draw(""); }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(11);
  gROOT->cd();
  hname = Form("hExt_Toff");
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) { h1->Draw(""); }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(12);
  gROOT->cd();
  hname = Form("hExt_TrkSizChiSq%d", iLev);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    h2->ProfileX()->Draw("same");
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(13);
  gROOT->cd();
  hname = Form("hExt_Xsig");
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) { h1->Draw(""); }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(14);
  gROOT->cd();
  hname = Form("hExt_Ysig");
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) { h1->Draw(""); }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(15);
  gROOT->cd();
  hname = Form("hExt_Tsig");
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) { h1->Draw(""); }
  else {
    cout << hname << " not found" << endl;
  }

  can->SaveAs("pl_ext_trk.pdf");
}
