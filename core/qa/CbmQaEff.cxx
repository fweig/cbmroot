/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CbmQaEff.cxx
/// \date   20.01.2023
/// \author S.Zharko <s.zharko@gsi.de>
/// \brief  Implementation of CbmQaEff class

#include "CbmQaEff.h"

#include "Logger.h"

#include "TString.h"

// ---------------------------------------------------------------------------------------------------------------------
//
CbmQaEff::CbmQaEff() : TEfficiency() {}

// ---------------------------------------------------------------------------------------------------------------------
//
CbmQaEff::CbmQaEff(const CbmQaEff& other) : TEfficiency(other) {}

// ---------------------------------------------------------------------------------------------------------------------
//
std::tuple<double, double, double> CbmQaEff::GetTotalEfficiency() const
{
  // The efficiency is always calculated based on the number of passed events and total events, which are stored in the
  // corresponding histograms inside the TEfficiency object. To get an integrated efficiency one needs just carefully
  // re-bin these histograms and pass them to the TEfficiency ctor.
  auto pHistP = std::unique_ptr<TH1>((TH1*) this->GetPassedHistogram()->Clone());
  auto pHistT = std::unique_ptr<TH1>((TH1*) this->GetTotalHistogram()->Clone());

  // X-axis range to calculate integrated efficiency
  double range[2] = {pHistP->GetXaxis()->GetXmin(), pHistP->GetXaxis()->GetXmax()};

  // Define the re-binned histograms
  std::unique_ptr<TH1> pHistPR = nullptr;  // re-binned histogram of passed events
  std::unique_ptr<TH1> pHistTR = nullptr;  // re-binned histogram of total events

  if (GetDimension() == 1) {
    pHistPR = std::unique_ptr<TH1>(pHistP->Rebin(1, "tmp_passed", range));
    pHistTR = std::unique_ptr<TH1>(pHistT->Rebin(1, "tmp_total", range));
  }
  if (GetDimension() == 2) {
    auto pHistPProj = std::unique_ptr<TH1>(static_cast<TH2*>(pHistP.get())->ProjectionX());
    auto pHistTProj = std::unique_ptr<TH1>(static_cast<TH2*>(pHistT.get())->ProjectionX());

    pHistPR = std::unique_ptr<TH1>(pHistPProj->Rebin(1, "tmp_passed", range));
    pHistTR = std::unique_ptr<TH1>(pHistTProj->Rebin(1, "tmp_total", range));
  }

  // Define temporary efficiency, copy all the attributes
  auto pIntEff = std::make_unique<TEfficiency>(*pHistPR, *pHistTR);
  pIntEff->SetBetaAlpha(fBeta_alpha);
  pIntEff->SetBetaBeta(fBeta_beta);
  pIntEff->SetConfidenceLevel(fConfLevel);
  pIntEff->SetWeight(fWeight);
  pIntEff->SetStatisticOption(fStatisticOption);
  pIntEff->SetPosteriorMode(this->UsesPosteriorMode());
  pIntEff->SetCentralInterval(this->UsesCentralInterval());

  double effV = pIntEff->GetEfficiency(1);
  double effL = pIntEff->GetEfficiencyErrorLow(1);
  double effU = pIntEff->GetEfficiencyErrorUp(1);

  return {effV, effL, effU};
}


// ---------------------------------------------------------------------------------------------------------------------
//
CbmQaEff* CbmQaEff::Integral(double lo, double hi)
{
  if (GetDimension() != 1) { return nullptr; }  // For now efficiency integration works only for 1D

  // Underlying histograms with passed and total events
  auto* pPassed = (TH1D*) (this->GetPassedHistogram());
  auto* pTotal  = (TH1D*) (this->GetTotalHistogram());

  if (!pPassed) { return nullptr; }

  // Integration range
  double range[2] = {0};
  if (lo < hi) {
    range[0] = lo;
    range[1] = hi;
  }
  else {
    range[0] = pPassed->GetXaxis()->GetXmin();
    range[1] = pPassed->GetXaxis()->GetXmax();
  }

  // Re-binned histograms for a selected integration range
  auto& histPassedReb = *(pPassed->Rebin(1, "ptmp", range));
  auto& histTotalReb  = *(pTotal->Rebin(1, "ttmp", range));

  TString sName = Form("%s_integrated", this->GetName());

  // New efficiency
  auto* pIntEff = new CbmQaEff(TEfficiency(histPassedReb, histTotalReb));

  pIntEff->SetName(sName);
  return pIntEff;
}

// ---------------------------------------------------------------------------------------------------------------------
//
CbmQaEff* CbmQaEff::DrawCopy(Option_t* opt, const char* postfix) const
{
  TString option = opt;
  option.ToLower();
  if (gPad && !option.Contains("same")) { gPad->Clear(); }
  TString newName = "";
  if (postfix) { newName.Form("%s%s", GetName(), postfix); }
  CbmQaEff* pNewEff = (CbmQaEff*) Clone(newName.Data());
  pNewEff->SetDirectory(nullptr);
  pNewEff->SetBit(kCanDelete);
  if (gPad) gPad->IncrementPaletteColor(1, option);
  pNewEff->AppendPad(option);
  return pNewEff;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmQaEff::SetStats()
{
  if (!fpStats) {
    fpStats = new TPaveText(0.20, 0.17, 0.80, 0.24, "NDC");
    fpStats->SetFillColor(0);

    // Add integrated efficiency
    auto [effVal, effLErr, effUErr] = this->GetTotalEfficiency();

    fpStats->SetTextFont(42);
    fpStats->SetTextSize(0.05);
    fpStats->SetBorderSize(0);
    fpStats->SetFillColor(0);
    fpStats->AddText(0, 0,
                     Form("#epsilon_{tot} = %.4f_{-%.4f}^{+%.4f} (CL %3.3f)", effVal, effLErr, effUErr, fConfLevel));
  }
}
