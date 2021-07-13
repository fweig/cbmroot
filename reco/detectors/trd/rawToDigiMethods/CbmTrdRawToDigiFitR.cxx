/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

#include "CbmTrdRawToDigiFitR.h"

#include "CbmTrdSpadic.h"

#include <RtypesCore.h>
#include <TH1.h>

#include <algorithm>

//---- CbmTrdRawToDigiFitR ----
CbmTrdRawToDigiFitR::CbmTrdRawToDigiFitR() : CbmTrdRawToDigiBaseR() { fixExtractionPars(); }

// ---- GetCharge ----
Float_t CbmTrdRawToDigiFitR::GetMaxAdcValue(const std::vector<std::int16_t>* /*samples*/)
{
  Double_t charge = fResponseFunc->GetParameter(static_cast<size_t>(CbmTrdSpadic::eResponsePars::kInputCharge));

  return charge > 0 ? charge : 0;
}

// ---- GetBinTimeShift ----
ULong64_t CbmTrdRawToDigiFitR::GetBinTimeShift(const std::vector<std::int16_t>* samples)
{

  fitResponse(samples);

  ULong64_t time = static_cast<ULong64_t>(
    fResponseFunc->GetParameter(static_cast<size_t>(CbmTrdSpadic::eResponsePars::kBinTimeshift)));
  return time;
}

// ---- fixExtractionPars ----
void CbmTrdRawToDigiFitR::fixExtractionPars()
{
  // Fix the parameters that are given by the spadic design
  std::vector<CbmTrdSpadic::eResponsePars> pars = {
    CbmTrdSpadic::eResponsePars::kNrPresamples, CbmTrdSpadic::eResponsePars::kShapingOrder,
    CbmTrdSpadic::eResponsePars::kShapingTime, CbmTrdSpadic::eResponsePars::kChargeToMaxAdcCal};
  for (auto ipar : pars)
    fixExtractionPar(ipar);

  // Limit the bin timeshift parameter to the clock cycle length
  fResponseFunc->SetParLimits(static_cast<size_t>(CbmTrdSpadic::eResponsePars::kBinTimeshift),
                              (-CbmTrdSpadic::GetClockCycle() / 2.0), (CbmTrdSpadic::GetClockCycle()) / 2.0);
}

// ---- fixExtractionPar ----
void CbmTrdRawToDigiFitR::fixExtractionPar(CbmTrdSpadic::eResponsePars ipar)
{
  fResponseFunc->FixParameter(static_cast<size_t>(ipar), fResponseFunc->GetParameter(static_cast<size_t>(ipar)));
}

// ---- fitResponse ----
void CbmTrdRawToDigiFitR::fitResponse(const std::vector<std::int16_t>* samples)
{
  fResponseFunc->SetParameter(static_cast<size_t>(CbmTrdSpadic::eResponsePars::kInputCharge), 0.0);
  fResponseFunc->SetParameter(static_cast<size_t>(CbmTrdSpadic::eResponsePars::kBinTimeshift), 0.0);

  TH1F hsignal("hsignal", "hsignal", samples->size(), 0, samples->size() - 1);
  Int_t ibin    = 0;
  auto baseline = GetBaseline(samples);
  for (auto isample : *samples) {
    if (isample < fSpadic->GetClippingStart()) hsignal.SetBinContent(ibin, (isample - baseline));
    ibin++;
  }
  if (fFitRangeEnd > samples->size()) return;
  // hsignal.Fit(fResponseFunc.get(), "SNQE I", "goff", fFitRangeStart, fFitRangeEnd);

  // Do not use
  // E speed improvement
  // M speed improvement
  // I it does not make sense to integrate the bin, since, we use discretised values from the adc

  // Use
  // C do not calculate chi2 to improve speed
  // S store the result to a TFitResultsPtr
  // Q we do want only minimal printout
  // N do not store the graphics function
  hsignal.Fit(fResponseFunc.get(), "SCNQ", "goff", fFitRangeStart, fFitRangeEnd);
}

ClassImp(CbmTrdRawToDigiFitR)
