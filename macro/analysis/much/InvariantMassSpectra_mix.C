/** @file InvariantMassSpectra_mix.C
  * @copyright Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Anna Senger [committer] **/

//---------------------------------------------------
//
// invariant mass spectra for background
// using Mix Event technics
//
// CbmAnaDimuonAnalysis has to be used with UseCuts(kTRUE)
//
// Anna Senger a.senger@gsi.de
//
//---------------------------------------------------

void InvariantMassSpectra_mix(Int_t energy = 8, TString type = "invM_mix", Int_t NofFiles = 5000, Int_t NofEvents = 1e8,
                              Int_t index = 0)
{
  gStyle->SetCanvasColor(10);
  gStyle->SetFrameFillColor(10);
  gStyle->SetHistLineWidth(4);
  gStyle->SetPadColor(10);
  gStyle->SetStatColor(10);
  gStyle->SetPalette(55);
  gStyle->SetPaintTextFormat("2.1f");

  Int_t NofBins  = 100;
  Int_t NofBinsM = 400;

  Double_t minY  = -2;
  Double_t maxY  = 6;
  Double_t min   = 0;
  Double_t maxPt = 10;
  Double_t maxM  = 4;

  TH1D* invM_bg = new TH1D(type.Data(), type.Data(), NofBinsM, min, maxM);
  (invM_bg->GetYaxis())->SetTitle("counts/(events #times 10 MeV/c^{2})");
  (invM_bg->GetXaxis())->SetTitle("m_{inv} (GeV/c^{2})");
  invM_bg->SetLineColor(kBlack);
  invM_bg->SetMarkerColor(kBlack);
  invM_bg->SetMarkerStyle(20);

  TH3D* YPtM_bg = new TH3D("YPtM_bg", "YPtM_bg", NofBins, minY, maxY, NofBins, min, maxPt, NofBinsM, min, maxM);

  TString name;

  TClonesArray* MuPlus  = new TClonesArray("CbmAnaMuonCandidate");
  TClonesArray* MuMinus = new TClonesArray("CbmAnaMuonCandidate");
  TTree* InputTree1;
  TTree* InputTree2;

  Int_t events = 0;

  while (events < NofEvents) {
    Int_t k1 = rand() % NofFiles + 1;
    Int_t k2 = rand() % NofFiles + 1;

    name.Form("/lustre/cbm/prod/mc/OCT19/sis100_muon_lmvm/%dgev/centr/%d/"
              "muons.ana.root",
              energy, k1);

    TFile* f1 = new TFile(name);
    if (f1->IsZombie() || f1->GetNkeys() < 1 || f1->TestBit(TFile::kRecovered)) {
      f1->Close();
      continue;
    }

    name.Form("/lustre/cbm/prod/mc/OCT19/sis100_muon_lmvm/%dgev/centr/%d/"
              "muons.ana.root",
              energy, k2);

    TFile* f2 = new TFile(name);
    if (f2->IsZombie() || f2->GetNkeys() < 1 || f2->TestBit(TFile::kRecovered)) {
      f2->Close();
      continue;
    }

    if (events % 1000000 == 0) cout << events / 1000000 << "e6" << endl;


    InputTree1 = (TTree*) f1->Get("cbmsim");
    if (!InputTree1) {
      f1->Close();
      continue;
    }

    InputTree1->SetBranchAddress("MuPlus", &MuPlus);

    InputTree2 = (TTree*) f2->Get("cbmsim");
    if (!InputTree2) {
      f2->Close();
      continue;
    }

    InputTree2->SetBranchAddress("MuMinus", &MuMinus);

    //-----------------------------------------------------------

    for (int iEvent = 0; iEvent < InputTree1->GetEntries(); iEvent++) {
      InputTree1->GetEntry(iEvent);
      InputTree2->GetEntry(iEvent);

      events++;

      int NofPlus  = MuPlus->GetEntriesFast();
      int NofMinus = MuMinus->GetEntriesFast();

      if (NofPlus == 0 || NofMinus == 0) continue;

      for (int iPart = 0; iPart < NofPlus; iPart++) {
        CbmAnaMuonCandidate* mu_pl = (CbmAnaMuonCandidate*) MuPlus->At(iPart);
        TLorentzVector* P_pl       = mu_pl->GetMomentum();
        for (int jPart = 0; jPart < NofMinus; jPart++) {
          CbmAnaMuonCandidate* mu_mn = (CbmAnaMuonCandidate*) MuMinus->At(jPart);
          TLorentzVector* P_mn       = mu_mn->GetMomentum();
          TLorentzVector M(*P_pl + *P_mn);
          invM_bg->Fill(M.M(), 1. / 10.);  // normalized with bin value
          YPtM_bg->Fill(M.Rapidity(), M.Pt(), M.M());
        }
      }
    }
    f1->Close();
    f2->Close();
  }

  invM_bg->Scale(1. / Double_t(events));
  YPtM_bg->Scale(1. / Double_t(events));
  name.Form("%s_1e8events_%d.root", type.Data(), index);
  TFile* FFF = new TFile(name, "recreate");
  invM_bg->Write();
  YPtM_bg->Write();
  FFF->Close();
}
