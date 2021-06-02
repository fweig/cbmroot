/** @file Add_histo.C
  * @copyright Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Anna Senger [committer] **/

//---------------------------------------------------
//
// combine invariant mass spectra for background @ 8-10 A GeV/c
// into one histogram
//
// Anna Senger a.senger@gsi.de
//
//---------------------------------------------------

void Add_histo(Int_t energy = 8, TString type = "invM_mix", Int_t NofFiles = 4000)
{
  gStyle->SetCanvasColor(10);
  gStyle->SetFrameFillColor(10);
  gStyle->SetHistLineWidth(6);
  gStyle->SetPadColor(10);
  gStyle->SetStatColor(10);
  gStyle->SetPalette(55);

  TString name;

  name.Form("%s_%dgev_0.root", type.Data(), energy);
  TFile* f1    = new TFile(name);
  TH1D* h_bg   = (TH1D*) f1->Get(type.Data());
  TH3D* 3Dh_bg = (TH3D*) f1->Get("YPtM_bg");

  Double_t N = 1;

  for (int k = 1; k < NofFiles + 1; k++) {
    name.Form("%s_%dgev_%d.root", type.Data(), energy, k);
    if (k % 100 == 0) cout << k << " - " << name << endl;

    TFile* f1 = new TFile(name);
    if (!f1 || f1->IsZombie() || f1->GetNkeys() < 1 || f1->TestBit(TFile::kRecovered)) {
      f1->Close();
      continue;
    }
    N++;
    h_bg->Add((TH1D*) f1->Get(type.Data()));
    3Dh_bg->Add((TH3D*) f1->Get("YPtM_bg"));

    f1->Close();
  }
  h_bg->Scale(1. / N);
  3Dh_bg->Scale(1. / N);

  name.Form("%s_%dgev_big.root", type.Data(), energy);
  TFile* FFF = new TFile(name, "recreate");
  h_bg->Write();
  3Dh_bg->Write();
  FFF->Close();
}
