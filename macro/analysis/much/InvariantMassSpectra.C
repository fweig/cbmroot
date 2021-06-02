/** @file InvariantMassSpectra.C
  * @copyright Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Anna Senger [committer] **/

//---------------------------------------------------
//
// invariant mass spectra for low-mass vector mesons, J/psi and thermal muons @ 8-10 A GeV/c
//
/**************** Full spectra corresponds NofSignals = 8 ****************/
//
/*********** If NofSignals=1 only omega meson will be used ***************/
//
// CbmAnaDimuonAnalysis has to be used with UseCuts(kTRUE)
//
// TODO update the multyplicity values for all mesons for 8 A GeV/c
// TODO macro for all SIS100 energies
//
// Anna Senger a.senger@gsi.de
//
//---------------------------------------------------

void InvariantMassSpectra(Int_t energy = 8, Int_t NofFiles = 1000, Int_t NofSignals = 7,
                          TString dir = "/lustre/cbm/prod/mc/OCT19/sis100_muon_lmvm")
{
  gStyle->SetCanvasColor(10);
  gStyle->SetFrameFillColor(10);
  gStyle->SetHistLineWidth(4);
  gStyle->SetPadColor(10);
  gStyle->SetStatColor(10);
  gStyle->SetPalette(55);
  gStyle->SetPaintTextFormat("2.1f");

  Int_t color[] = {2, 3, 4, 6, 7, 5, 14, 28};

  TString sgn[]    = {"omega", "omegaD", "eta", "etaD", "rho0", "phi", "qgp", "jpsi"};
  TString sgnLtx[] = {"#omega", "#omega_{D}", "#eta", "#eta_{D}", "#rho_{0}", "#phi", "qgp", "J/#psi"};

  Double_t sgn_mult[] = {19 * 9 * 1e-5, 19. * 1.3e-4,   16. * 5.8e-6, 16. * 3.1e-4,
                         9. * 4.55e-5,  0.12 * 2.87e-4, 3.2e-3,       0.06 * 1e-6};

  Int_t NofBins  = 100;
  Int_t NofBinsM = 400;

  Double_t minY  = -2;
  Double_t maxY  = 6;
  Double_t min   = 0;
  Double_t maxPt = 10;
  Double_t maxP  = 20;
  Double_t maxM  = 4;

  TH1D* invM_sgn[NofSignals];

  invM_sgn[0] = new TH1D("invM_omega", "invM_omega", NofBinsM, min, maxM);

  (invM_sgn[0]->GetXaxis())->SetTitle("m_{inv} (GeV/c^{2})");
  (invM_sgn[0]->GetYaxis())->SetTitle("counts/(events #times 10 MeV/c^{2})");
  invM_sgn[0]->SetLineColor(kRed);
  invM_sgn[0]->SetMarkerColor(kRed);
  invM_sgn[0]->SetMarkerStyle(20);
  invM_sgn[0]->SetMarkerSize(0.7);

  invM_sgn[0]->GetXaxis()->SetTitleSize(0.05);
  invM_sgn[0]->GetXaxis()->SetLabelSize(0.04);
  invM_sgn[0]->GetXaxis()->SetTitleOffset(0.9);
  invM_sgn[0]->GetYaxis()->SetTitleSize(0.04);
  invM_sgn[0]->GetYaxis()->SetLabelSize(0.04);
  invM_sgn[0]->GetYaxis()->SetTitleOffset(1.3);

  TString name;

  for (int i = 1; i < NofSignals; i++) {
    name        = "invM_" + sgn[i];
    invM_sgn[i] = (TH1D*) invM_sgn[0]->Clone(name);
    invM_sgn[i]->SetTitle(name);
    invM_sgn[i]->SetLineColor(color[i]);
    invM_sgn[i]->SetMarkerColor(color[i]);
  }

  TH1D* invM_MCsgn[NofSignals];

  for (int i = 0; i < NofSignals; i++) {
    name          = "invM_MC_" + sgn[i];
    invM_MCsgn[i] = (TH1D*) invM_sgn[0]->Clone(name);
    invM_MCsgn[i]->SetTitle(name);
    invM_MCsgn[i]->SetLineColor(color[i]);
    invM_MCsgn[i]->SetMarkerColor(color[i]);
  }

  TH1D* invM_bg = (TH1D*) invM_sgn[0]->Clone("invM_bg");
  invM_bg->SetTitle("invM_bg");
  invM_bg->SetLineColor(kBlack);
  invM_bg->SetMarkerColor(kBlack);

  TH1D* invM_full = (TH1D*) invM_sgn[0]->Clone("invM_full");
  invM_full->SetTitle("invM_full");
  invM_full->SetLineColor(kBlack);
  invM_full->SetMarkerColor(kBlack);

  TH1D* SBratio = (TH1D*) invM_sgn[0]->Clone("SBratio");
  SBratio->SetTitle("SBratio");
  (SBratio->GetYaxis())->SetTitle("signal-to-background ratio");

  TH2D* YPt[NofSignals];

  YPt[0] = new TH2D("YPt_omega", "YPt_omega", NofBins, minY, maxY, NofBins, min, maxPt);
  (YPt[0]->GetXaxis())->SetTitle("Y");
  (YPt[0]->GetYaxis())->SetTitle("P_{t} (GeV/c)");
  YPt[0]->GetXaxis()->SetTitleSize(0.05);
  YPt[0]->GetXaxis()->SetLabelSize(0.04);
  YPt[0]->GetXaxis()->SetTitleOffset(0.9);
  YPt[0]->GetYaxis()->SetTitleSize(0.04);
  YPt[0]->GetYaxis()->SetLabelSize(0.04);
  YPt[0]->GetYaxis()->SetTitleOffset(1.3);

  for (int i = 1; i < NofSignals; i++) {
    name   = "YPt_" + sgn[i];
    YPt[i] = (TH2D*) YPt[0]->Clone(name);
    YPt[i]->SetTitle(name);
  }

  TH3D* YPtM_sgn[NofSignals];
  YPtM_sgn[0] = new TH3D("YPtM_omega", "YPtM_omega", NofBins, minY, maxY, NofBins, min, maxPt, NofBinsM, min, maxM);
  for (int i = 1; i < NofSignals; i++) {
    name        = "YPtM_" + sgn[i];
    YPtM_sgn[i] = (TH3D*) YPtM_sgn[0]->Clone(name);
    YPtM_sgn[i]->SetTitle(name);
  }
  TH3D* YPtM_bg = (TH3D*) YPtM_sgn[0]->Clone("YPtM_bg");
  YPtM_bg->SetTitle("YPtM_bg");

  TClonesArray* MuPlus  = new TClonesArray("CbmAnaMuonCandidate");
  TClonesArray* MuMinus = new TClonesArray("CbmAnaMuonCandidate");

  TTree* InputTree;

  Double_t NofEvents[NofSignals + 1];

  for (int i = 0; i < NofSignals + 1; i++) {
    for (int k = 1; k < NofFiles + 1; k++) {
      if (i < NofSignals) name.Form("%s/%dgev/%s/%d/muons.ana.root", dir.Data(), energy, sgn[i].Data(), k);
      else
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

        NofEvents[i]++;

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
            if (i < NofSignals) {
              invM_sgn[i]->Fill(M.M(), sgn_mult[i] / 10.);  // normalized with bin value

              YPtM_sgn[i]->Fill(M.Rapidity(), M.Pt(), M.M(), sgn_mult[i]);

              if (mu_pl->GetTrueMu() == 1 && mu_mn->GetTrueMu() == 1) {
                invM_MCsgn[i]->Fill(M.M(), sgn_mult[i] / 10.);
                YPt[i]->Fill(M.Rapidity(), M.Pt());
              }
            }
            else {
              invM_bg->Fill(M.M(), 1. / 10.);
              YPtM_bg->Fill(M.Rapidity(), M.Pt(), M.M());
            }
          }
        }
      }
      f->Close();
    }
  }

  for (int i = 0; i < NofSignals + 1; i++) {
    if (i < NofSignals) {
      invM_sgn[i]->Scale(1. / NofEvents[i]);
      invM_MCsgn[i]->Scale(1. / NofEvents[i]);
      invM_full->Add(invM_sgn[i]);
      YPtM_sgn[i]->Scale(1. / NofEvents[i]);
    }
    else {
      invM_bg->Scale(1. / NofEvents[i]);
      invM_full->Add(invM_bg);
      YPtM_bg->Scale(1. / NofEvents[i]);
    }
  }

  TH1D* invM_sgnAll = (TH1D*) invM_sgn[0]->Clone("invM_sgnAll");
  for (int i = 1; i < NofSignals; i++)
    invM_sgnAll->Add(invM_sgn[i]);

  TH1D* invM_sgnAllMC = (TH1D*) invM_sgn[0]->Clone("invM_sgnAllMC");
  for (int i = 1; i < NofSignals; i++)
    invM_sgnAllMC->Add(invM_MCsgn[i]);

  for (int i = 0; i < NofBinsM; i++) {
    Double_t bg = invM_full->GetBinContent(i) - invM_sgnAllMC->GetBinContent(i);
    if (bg != 0) SBratio->SetBinContent(i, invM_sgnAllMC->GetBinContent(i) / bg);
  }

  TCanvas* c1;
  c1 = new TCanvas("c1", "Particles", 0, 0, 1200, 600);
  c1->UseCurrentStyle();

  c1->Divide(2, 1);
  c1->cd(1);
  gPad->SetLogy();

  TLegend* legend = new TLegend(0.6, 0.6, 0.89, 0.89);
  legend->SetTextSize(0.04);
  for (int i = 0; i < NofSignals; i++) {
    name = sgnLtx[i];
    legend->AddEntry(invM_MCsgn[i], name, "l");
  }
  legend->AddEntry(invM_full, "full spectrum", "l");

  invM_full->Draw();
  invM_full->SetStats(kFALSE);
  invM_full->GetXaxis()->SetRangeUser(0, 2.5);
  legend->Draw();
  for (int i = 0; i < NofSignals; i++)
    invM_MCsgn[i]->Draw("same");
  c1->cd(2);
  gPad->SetLogy();
  SBratio->SetMinimum(1e-5);
  SBratio->SetMaximum(5);
  SBratio->GetXaxis()->SetRangeUser(0, 2.5);
  SBratio->SetStats(kFALSE);
  SBratio->Draw("L HIST");

  if (NofSignals == 1) name.Form("invM_bg_omega_%dgev.root", energy);
  else
    name.Form("invM_bg_signals_%dgev.root", energy);

  TFile* FFF = new TFile(name, "recreate");
  for (int i = 0; i < NofSignals; i++) {
    invM_sgn[i]->Write();
    invM_MCsgn[i]->Write();
    YPt[i]->Write();
    YPtM_sgn[i]->Write();
  }
  invM_sgnAll->Write();
  invM_sgnAllMC->Write();
  invM_bg->Write();
  invM_full->Write();
  SBratio->Write();
  YPtM_bg->Write();
  c1->Write();
  FFF->Close();
}
