/* Copyright (C) 2016-2018 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void pl_over_MatD4sel(TString sysinfo = "")
{
  gROOT->LoadMacro(((TString) gSystem->Getenv("VMCWORKDIR") + "/macro/beamtime/pl_Datime.C").Data());

  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 800, 800);
  can->Divide(3, 4);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);
  gStyle->SetOptFit(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);
  cout << " DirLevel " << gROOT->GetDirLevel() << endl;

  TH1* h;
  TH1* h1;
  TH1* h12;
  TH2* h2;
  TH2F* h2f;

  Double_t NEvents;
  Double_t NFinalHits;
  Double_t dTRes;
  Double_t dTRMS;

  // if (hPla!=NULL) hPla->Delete();
  can->cd(1);
  gROOT->cd();
  TString hname1 = "hBRefMul";
  h1             = (TH1*) gROOT->FindObjectAny(hname1);
  if (h1 != NULL) {
    h1->Draw();
    gPad->SetLogy();
  }
  else {
    cout << hname1 << " not found" << endl;
  }


  can->cd(2);
  gROOT->cd();
  TString hname2 = "hDTD4";
  h1             = (TH1*) gROOT->FindObjectAny(hname2);
  if (h1 != NULL) {
    h1->Draw();
    gPad->SetLogy();
  }
  else {
    cout << hname2 << " not found" << endl;
  }

  can->cd(3);
  gROOT->cd();
  TString hname3 = "hNMatch04";
  h1             = (TH1*) gROOT->FindObjectAny(hname3);
  if (h1 != NULL) {
    h1->Draw();
    gPad->SetLogy();
  }
  else {
    cout << hname3 << " not found" << endl;
  }

  TString hname4 = "hNMatchD4sel";
  h1             = (TH1*) gROOT->FindObjectAny(hname4);
  if (h1 != NULL) {
    h1->Draw("same");
    h1->SetLineColor(2);
  }
  else {
    cout << hname4 << " not found" << endl;
  }

  can->cd(4);
  gROOT->cd();
  TString hname5 = "hChiSel24";
  h1             = (TH1*) gROOT->FindObjectAny(hname5);
  if (h1 != NULL) { h1->Draw(); }
  else {
    cout << hname5 << " not found" << endl;
  }

  can->cd(5);
  {
    gROOT->cd();
    TString hname = "hDXSel24";
    h1            = (TH1*) gROOT->FindObjectAny(hname);
    if (h1 != NULL) { h1->Draw(); }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(6);
  {
    gROOT->cd();
    TString hname = "hDYSel24";
    h1            = (TH1*) gROOT->FindObjectAny(hname);
    if (h1 != NULL) { h1->Draw(); }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(7);
  {
    gROOT->cd();
    TString hname = "hDTSel24";
    h1            = (TH1*) gROOT->FindObjectAny(hname);
    if (h1 != NULL) {
      h1->Draw();
      h1->Fit("gaus");
      gPad->SetLogy();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(8);
  {
    gROOT->cd();
    TString hname = "hTofSel24";
    h1            = (TH1*) gROOT->FindObjectAny(hname);
    if (h1 != NULL) {
      h1->Draw();
      gPad->SetLogy();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(9);
  {
    gROOT->cd();
    TString hname = "hDTD4sel";
    h1            = (TH1*) gROOT->FindObjectAny(hname);
    if (h1 != NULL) {
      h1->Draw();
      NEvents = h1->GetEntries();
      h1->SetLineColor(4);
      gPad->SetLogy();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  {
    TString hname2 = "hTofD4sel";
    h12            = (TH1*) gROOT->FindObjectAny(hname2);
    if (h12 != NULL) {
      h12->Draw("same");
      h12->SetLineColor(2);
      NEvents = h1->GetEntries();
    }
    else {
      cout << hname2 << " not found" << endl;
    }
  }

  can->cd(10);
  {
    gROOT->cd();
    TString hname = "hXYSel2D4sel";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) { h2->Draw("colz"); }
    else {
      cout << hname << " not found" << endl;
    }
  }
  can->cd(11);
  {
    gROOT->cd();
    TString hname = "hXY4D4sel";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) { h2->Draw("colz"); }
    else {
      cout << hname << " not found" << endl;
    }
  }
  can->cd(12);
  {
    gROOT->cd();
    TString hname = "hXY0D4sel";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) { h2->Draw("colz"); }
    else {
      cout << hname << " not found" << endl;
    }
  }

  TString FADD = Form("pl_Datime(\"%s\")", sysinfo.Data());
  if (gROOT->IsBatch()) { gInterpreter->ProcessLine(FADD.Data()); }

  //report summary
  can->SaveAs("pl_over_MatD4sel.pdf");
}
