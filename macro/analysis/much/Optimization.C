/* Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Anna Senger [committer] */

//---------------------------------------------------
//
// invariant mass spectra for omega mesons @ 8-10 A GeV/c
//
// CbmAnaDimuonAnalysis has to be used:
// - with UseCuts(kFALSE) for type 0 (cuts optimization)
// - with UseCuts(kTRUE)  for type 1 (geometry optimization)
//
// Invariant mass spectrum for background is calculated using super-event technics
//
// Anna Senger a.senger@gsi.de
//
//---------------------------------------------------

void Optimization(Int_t energy = 8, Int_t NofFiles = 1000, Int_t type = 1, TString version = "v19c",
                  TString dir = "../../much/data/sis100_muon_lmvm")
{
  gStyle->SetCanvasColor(10);
  gStyle->SetFrameFillColor(10);
  gStyle->SetHistLineWidth(4);
  gStyle->SetPadColor(10);
  gStyle->SetStatColor(10);
  gStyle->SetPalette(55);
  gStyle->SetPaintTextFormat("2.1f");

  Double_t sgn_mult = 19 * 9 * 1e-5;

  TH1D* invM_sgn = new TH1D("invM_omega", "invM_omega", 400, 0, 4);

  (invM_sgn->GetXaxis())->SetTitle("m_{inv} (GeV/c^{2})");
  (invM_sgn->GetYaxis())->SetTitle("counts/(events #times 10 MeV/c^{2})");
  invM_sgn->SetLineColor(kRed);
  invM_sgn->SetMarkerColor(kRed);
  invM_sgn->SetMarkerStyle(20);

  TH1D* invM_bg = (TH1D*) invM_sgn->Clone("invM_bg");
  invM_bg->SetTitle("invM_bg");
  invM_bg->SetLineColor(kBlack);
  invM_bg->SetMarkerColor(kBlack);

  TH1D* invM_full = (TH1D*) invM_sgn->Clone("invM_full");
  invM_full->SetTitle("invM_full");
  invM_full->SetLineColor(kBlack);
  invM_full->SetMarkerColor(kBlack);

  TClonesArray* MuPlus  = new TClonesArray("CbmAnaMuonCandidate");
  TClonesArray* MuMinus = new TClonesArray("CbmAnaMuonCandidate");
  TTree* InputTree;

  Double_t NofEvents[2];

  //-------------- cuts ---------------------------------

  Int_t MUCHhits = 11;
  Int_t STShits  = 7;
  Int_t TRDhits  = 1;
  Int_t TOFhits  = 1;

  Double_t MUCHchi2 = 2.;
  Double_t STSchi2  = 2.;
  Double_t Vchi2    = 2.;

  Int_t sigmaTOFCut = 2;

  TString dir_par = getenv("VMCWORKDIR");
  TString name =
    dir_par + "/parameters/much/TOF8gev_fitParam_sigma" + std::to_string(sigmaTOFCut) + ".sis100_muon_lmvm.root";

  Double_t p0min, p1min, p2min;
  Double_t p0max, p1max, p2max;

  TFile* FF       = new TFile(name);
  TTree* MinParam = (TTree*) FF->Get("MinParam");
  MinParam->SetBranchAddress("p0", &p0min);
  MinParam->SetBranchAddress("p1", &p1min);
  MinParam->SetBranchAddress("p2", &p2min);
  MinParam->GetEntry(0);

  TTree* MaxParam = (TTree*) FF->Get("MaxParam");
  MaxParam->SetBranchAddress("p0", &p0max);
  MaxParam->SetBranchAddress("p1", &p1max);
  MaxParam->SetBranchAddress("p2", &p2max);
  MaxParam->GetEntry(0);

  //--------------------------------------------------------

  for (int k = 1; k < NofFiles + 1; k++) {
    name.Form("%s/%dgev/omega/%d/muons.ana.root", dir.Data(), energy, k);

    TFile* f = new TFile(name);
    if (f->IsZombie() || f->GetNkeys() < 1 || f->TestBit(TFile::kRecovered)) {
      f->Close();
      continue;
    }

    if (k % 100 == 0) cout << "Input File " << k << " : " << f->GetName() << endl;

    InputTree = (TTree*) f->Get("cbmsim");
    if (!InputTree) {
      f->Close();
      continue;
    }

    InputTree->SetBranchAddress("MuPlus", &MuPlus);
    InputTree->SetBranchAddress("MuMinus", &MuMinus);

    //-----------------------------------------------------------

    for (int iEvent = 0; iEvent < InputTree->GetEntries(); iEvent++) {
      InputTree->GetEntry(iEvent);

      NofEvents[0]++;

      int NofPlus  = MuPlus->GetEntriesFast();
      int NofMinus = MuMinus->GetEntriesFast();

      if (NofPlus == 0 || NofMinus == 0) continue;

      for (int iPart = 0; iPart < NofPlus; iPart++) {
        CbmAnaMuonCandidate* mu_pl = (CbmAnaMuonCandidate*) MuPlus->At(iPart);
        if (type == 0) {
          if (mu_pl->GetNStsHits() < STShits) continue;
          if (mu_pl->GetNMuchHits() < MUCHhits) continue;
          if (mu_pl->GetNTrdHits() < TRDhits) continue;
          if (mu_pl->GetNTofHits() < TOFhits) continue;

          if (mu_pl->GetChiToVertex() > Vchi2) continue;
          if (mu_pl->GetChiSts() > STSchi2) continue;
          if (mu_pl->GetChiMuch() > MUCHchi2) continue;
        }

        TLorentzVector* P_pl = mu_pl->GetMomentum();

        Double_t mass = mu_pl->GetTofM();
        Double_t mom  = P_pl->P();

        if (type == 0)
          if (mass < (p0min + p1min * mom + p2min * mom * mom) || mass > (p0max + p1max * mom + p2max * mom * mom))
            continue;

        for (int jPart = 0; jPart < NofMinus; jPart++) {
          CbmAnaMuonCandidate* mu_mn = (CbmAnaMuonCandidate*) MuMinus->At(jPart);
          if (type == 0) {
            if (mu_mn->GetNStsHits() < STShits) continue;
            if (mu_mn->GetNMuchHits() < MUCHhits) continue;
            if (mu_mn->GetNTrdHits() < TRDhits) continue;
            if (mu_mn->GetNTofHits() < TOFhits) continue;

            if (mu_mn->GetChiToVertex() > Vchi2) continue;
            if (mu_mn->GetChiSts() > STSchi2) continue;
            if (mu_mn->GetChiMuch() > MUCHchi2) continue;
          }
          TLorentzVector* P_mn = mu_mn->GetMomentum();

          mass = mu_mn->GetTofM();
          mom  = P_mn->P();

          if (type == 0)
            if (mass < (p0min + p1min * mom + p2min * mom * mom) || mass > (p0max + p1max * mom + p2max * mom * mom))
              continue;

          TLorentzVector M(*P_pl + *P_mn);
          invM_sgn->Fill(M.M(), sgn_mult / 10.);  // normalized with bin value
        }
      }
    }
    f->Close();
  }

  TLorentzVector P1, P2, M;
  TTree* Plus = new TTree("Plus", "part-");
  Plus->Branch("P0", &P1(0), "Px/D:Py:Pz:E", 10000000);

  TTree* Minus = new TTree("Minus", "part+");
  Minus->Branch("P0", &P2(0), "Px/D:Py:Pz:E", 10000000);

  for (int k = 1; k < NofFiles + 1; k++) {
    name.Form("%s/%dgev/centr/%d/muons.ana.root", dir.Data(), energy, k);

    TFile* f = new TFile(name);
    if (f->IsZombie() || f->GetNkeys() < 1 || f->TestBit(TFile::kRecovered)) {
      f->Close();
      continue;
    }

    if (k % 100 == 0) cout << "Input File " << k << " : " << f->GetName() << endl;

    InputTree = (TTree*) f->Get("cbmsim");
    if (!InputTree) {
      f->Close();
      continue;
    }

    InputTree->SetBranchAddress("MuPlus", &MuPlus);
    InputTree->SetBranchAddress("MuMinus", &MuMinus);

    //-----------------------------------------------------------

    for (int iEvent = 0; iEvent < InputTree->GetEntries(); iEvent++) {
      InputTree->GetEntry(iEvent);

      NofEvents[1]++;

      int NofPlus  = MuPlus->GetEntriesFast();
      int NofMinus = MuMinus->GetEntriesFast();

      for (int iPart = 0; iPart < NofPlus; iPart++) {
        CbmAnaMuonCandidate* mu_pl = (CbmAnaMuonCandidate*) MuPlus->At(iPart);
        if (type == 0) {
          if (mu_pl->GetNStsHits() < STShits) continue;
          if (mu_pl->GetNMuchHits() < MUCHhits) continue;
          if (mu_pl->GetNTrdHits() < TRDhits) continue;
          if (mu_pl->GetNTofHits() < TOFhits) continue;

          if (mu_pl->GetChiToVertex() > Vchi2) continue;
          if (mu_pl->GetChiSts() > STSchi2) continue;
          if (mu_pl->GetChiMuch() > MUCHchi2) continue;
        }
        TLorentzVector* P_pl = mu_pl->GetMomentum();

        Double_t mass = mu_pl->GetTofM();
        Double_t mom  = P_pl->P();

        if (type == 0)
          if (mass < (p0min + p1min * mom + p2min * mom * mom) || mass > (p0max + p1max * mom + p2max * mom * mom))
            continue;
        P1 = *mu_pl->GetMomentum();
        Plus->Fill();
      }
      for (int jPart = 0; jPart < NofMinus; jPart++) {
        CbmAnaMuonCandidate* mu_mn = (CbmAnaMuonCandidate*) MuMinus->At(jPart);
        if (type == 0) {
          if (mu_mn->GetNStsHits() < STShits) continue;
          if (mu_mn->GetNMuchHits() < MUCHhits) continue;
          if (mu_mn->GetNTrdHits() < TRDhits) continue;
          if (mu_mn->GetNTofHits() < TOFhits) continue;

          if (mu_mn->GetChiToVertex() > Vchi2) continue;
          if (mu_mn->GetChiSts() > STSchi2) continue;
          if (mu_mn->GetChiMuch() > MUCHchi2) continue;
        }
        TLorentzVector* P_mn = mu_mn->GetMomentum();

        Double_t mass = mu_mn->GetTofM();
        Double_t mom  = P_mn->P();

        if (type == 0)
          if (mass < (p0min + p1min * mom + p2min * mom * mom) || mass > (p0max + p1max * mom + p2max * mom * mom))
            continue;
        P2 = *mu_mn->GetMomentum();
        Minus->Fill();
      }
    }
    f->Close();
    if (Plus->GetEntries() > 10000) break;  // speed up of calculation
  }

  Plus->SetBranchAddress("P0", &P1(0));
  Minus->SetBranchAddress("P0", &P2(0));

  Double_t entriesPlus  = Plus->GetEntries();
  Double_t entriesMinus = Minus->GetEntries();

  for (int jPart = 0; jPart < entriesPlus; jPart++) {
    if (jPart % 1000 == 0) cout << entriesPlus << " ----- " << jPart << endl;
    Plus->GetEntry(jPart);
    for (int iPart = 0; iPart < entriesMinus; iPart++) {
      Minus->GetEntry(iPart);
      M = P1 + P2;
      invM_bg->Fill(M.M(), 1. / 10.);
    }
  }

  invM_sgn->Scale(1. / NofEvents[0]);
  invM_full->Add(invM_sgn);

  invM_bg->Scale(1. / NofEvents[1] / NofEvents[1]);
  invM_full->Add(invM_bg);

  Double_t initPar[] = {0.782, 0.02};  // omega mass and sigma for fit initialization

  TF1* fit = new TF1("fullFit", "gaus(0)+pol2(3)", 0.60, 0.90);

  fit->SetParNames("Const", "Mass", "Sigma", "a0", "a1", "a2");
  fit->SetNpx(10000);
  fit->SetLineColor(2);
  fit->SetParameter(1, (0.60 + 0.90) / 2);
  fit->SetParameter(2, (0.90 - 0.60) / 4);
  fit->SetParLimits(2, 0, 1);

  fit->FixParameter(1, initPar[0]);
  fit->FixParameter(2, initPar[1]);
  invM_full->Fit("fullFit", "QRN", "", 0.60, 0.90);

  fit->ReleaseParameter(1);
  fit->ReleaseParameter(2);
  invM_full->Fit("fullFit", "QRN", "", 0.60, 0.90);

  float start = fit->GetParameter(1) - 2 * fit->GetParameter(2);
  float end   = fit->GetParameter(1) + 2 * fit->GetParameter(2);

  invM_full->Fit("fullFit", "", "", 0.60, 0.90);

  TF1* fitSgn = new TF1("sgnFit", "gaus", 0.60, 0.90);
  for (int iPar = 0; iPar < 3; iPar++)
    fitSgn->SetParameter(iPar, fit->GetParameter(iPar));

  TF1* fitBg = new TF1("bgFit", "pol2", 0.60, 0.90);
  for (int iPar = 3; iPar < 6; iPar++)
    fitBg->SetParameter(iPar - 3, fit->GetParameter(iPar));

  Double_t min_histo = (invM_sgn->GetXaxis())->GetXmin();
  Double_t max_histo = (invM_sgn->GetXaxis())->GetXmax();
  Double_t bin_histo = (max_histo - min_histo) / invM_sgn->GetNbinsX();

  float S       = fitSgn->Integral(start, end) / bin_histo;
  float B       = fitBg->Integral(start, end) / bin_histo;
  float SBratio = S / B;

  Double_t eff = S / sgn_mult * 10. * 100.;

  char tmpString[5];
  sprintf(tmpString, "%4.2f", (S / B));

  TString LegendEntry[6];
  LegendEntry[0] = "#omega(782)";
  LegendEntry[1] = "S/B = ";
  LegendEntry[1] += tmpString;
  LegendEntry[2] = "#varepsilon = ";
  sprintf(tmpString, "%2.2f ", eff);
  LegendEntry[2] += tmpString;
  LegendEntry[2] += "%";
  LegendEntry[3] = "#frac{S}{#sqrt{S+B}} = ";
  sprintf(tmpString, "%1.2e", S / sqrt(S + B));
  LegendEntry[3] += tmpString;
  LegendEntry[4] = "   ";

  TCanvas* c1;
  c1 = new TCanvas("c1", "Particles", 0, 0, 1200, 600);
  c1->UseCurrentStyle();
  gPad->SetFixedAspectRatio(kFALSE);

  TLegend* legend = legend = new TLegend(0.60, 0.55, .99, .99);
  legend->SetFillColor(0);
  legend->SetTextSize(0.05);
  legend->SetMargin(0.1);

  TGaxis::SetMaxDigits(3);

  invM_full->GetXaxis()->SetTitleSize(0.05);
  invM_full->GetXaxis()->SetLabelSize(0.04);
  invM_full->GetXaxis()->SetTitleOffset(0.9);
  invM_full->GetXaxis()->SetRangeUser(0.6, 1.2);

  invM_full->GetYaxis()->SetTitleSize(0.04);
  invM_full->GetYaxis()->SetLabelSize(0.04);
  invM_full->GetYaxis()->SetTitleOffset(1.3);

  invM_full->SetLineColor(kBlack);
  invM_full->SetMarkerStyle(20);
  invM_full->SetMarkerColor(kBlack);
  invM_full->SetMarkerSize(0.7);
  legend->AddEntry(invM_full, LegendEntry[0].Data(), "l");
  legend->AddEntry((TObject*) 0, LegendEntry[1].Data(), "");
  legend->AddEntry((TObject*) 0, LegendEntry[2].Data(), "");
  legend->AddEntry((TObject*) 0, LegendEntry[3].Data(), "");
  legend->AddEntry((TObject*) 0, LegendEntry[4].Data(), "");

  if (type == 0) {
    c1->Divide(2, 1);
    c1->cd(1);

    invM_full->Draw("pe");
    legend->Draw();

    c1->cd(2);

    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);

    name = "Cuts:";
    Tl.DrawLatex(0.3, 0.8, name);
    name.Form("N of STS hits #geq %d", STShits);
    Tl.DrawLatex(0.3, 0.7, name);
    name.Form("N of MUCH hits #geq %d", MUCHhits);
    Tl.DrawLatex(0.3, 0.6, name);
    name.Form("N of TRD hits #geq %d", TRDhits);
    Tl.DrawLatex(0.3, 0.5, name);
    name.Form("#chi^{2}_{vertex} #leq %1.1f", Vchi2);
    Tl.DrawLatex(0.3, 0.4, name);
    name.Form("#chi^{2}_{STS} #leq %1.1f", STSchi2);
    Tl.DrawLatex(0.3, 0.3, name);
    name.Form("#chi^{2}_{MUCH} #leq %1.1f", MUCHchi2);
    Tl.DrawLatex(0.3, 0.2, name);
    name.Form("%d#sigma cut in TOF", sigmaTOFCut);
    Tl.DrawLatex(0.3, 0.1, name);
  }
  else {
    invM_full->Draw("pe");
    legend->Draw();
  }

  if (type == 0) name.Form("invM_bg_omega_%dgev.cuts_", energy);
  else
    name.Form("invM_bg_omega_%dgev.geo_", energy);
  name += version + ".root";

  TFile* FFF = new TFile(name, "recreate");
  c1->Write();
  FFF->Close();

  name += ".ps";
  c1->Print(name);
}
