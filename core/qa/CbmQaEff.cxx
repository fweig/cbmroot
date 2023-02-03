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
CbmQaEff* CbmQaEff::Integral(double lo, double hi)
{
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

  LOG(info) << "DEBUG: " << this->GetName() << ": passed: " << pPassed->GetEntries();
  LOG(info) << "DEBUG: " << this->GetName() << ": total: " << pTotal->GetEntries();
  LOG(info) << "DEBUG: " << this->GetName() << ": passed: " << histPassedReb.GetBinContent(1) << ' '
            << histPassedReb.GetBinError(1);
  LOG(info) << "DEBUG: " << this->GetName() << ": total: " << histTotalReb.GetBinContent(1) << ' '
            << histPassedReb.GetBinError(1);

  TString sName = Form("%s_integrated", this->GetName());

  // New efficiency
  auto* pIntEff = new CbmQaEff(TEfficiency(histPassedReb, histTotalReb));
  pIntEff->SetName(sName);
  return pIntEff;
}
