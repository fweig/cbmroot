/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

#include "CbmTrdSpadic.h"

#include <RtypesCore.h>
#include <TH1.h>
#include <TMath.h>  // for SpadicResponse function
#include <TRandom.h>

#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>


const Double_t CbmTrdSpadic::fgClockCycleLength = CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kSPADIC);

// const Double_t CbmTrdSpadic::fgShapingTime       = 142.0;
const Double_t CbmTrdSpadic::fgShapingTime       = 120.0;
const Double_t CbmTrdSpadic::fgChargeToMaxAdcCal = 2.73;

CbmTrdSpadic::CbmTrdSpadic() {}
CbmTrdSpadic::~CbmTrdSpadic() {}

// ---- GetResponseFunc ----
std::shared_ptr<TF1> CbmTrdSpadic::GetResponseFunc()
{
  auto funcSpadicResponse = std::make_shared<TF1>(TF1("funcSpadicResponse", Response, 0, (fgNrOfAdcSamples - 1), 6));
  funcSpadicResponse->SetParNames("Shaping time", "Shaping order", "Input charge", "Bin timeshift", "Nr of presamples",
                                  "Charge Calibration");

  // Set the default parameters
  std::vector<Double_t> parvec = {fgShapingTime,          fgShapingOrder,   0.0,
                                  fgClockCycleLength / 2, fgNrOfPresamples, fgChargeToMaxAdcCal};
  funcSpadicResponse->SetParameters(parvec.data());

  return funcSpadicResponse;
}

// ---- Response ----
Double_t CbmTrdSpadic::Response(Double_t* samplenr, Double_t* par)
{

  //   par[0] = shaping time of the first shaping station
  //   par[1] = shaping order
  //   par[2] = input charge
  //   par[3] = timeshift inside the 62.5 ns bin of a CC
  //   par[4] = number of presamples
  //   par[5] = charge calibration factor

  // histo comes in cc but spadicResponse is in ns
  Double_t t = (samplenr[0] - par[4]) * fgClockCycleLength + par[3];

  Double_t response =
    t >= 0 ? ((par[2] * par[5]) * TMath::Power((t / par[0]), ((Int_t) par[1])) * TMath::Exp(-(t / par[0]))) : 0;

  return response;
}

// ---- Response ----
Double_t CbmTrdSpadic::Response(UInt_t samplenr)
{
  Double_t samplepos = samplenr;

  return Response(&samplepos, fvecResponsePars.data());
}

// ---- GetAnalogResponse ----
std::vector<std::int16_t> CbmTrdSpadic::GetAnalogResponsePulse(Double_t inputCharge, Double_t binTimeshift)
{
  // If a timeshift should be taken into account, it has to be set before calling this function via SetParameter(eResponsePars::kBinTimeShift, yourTimeShiftHere);
  std::vector<std::int16_t> pulse(CbmTrdSpadic::GetNrOfAdcSamples(), 0.0);
  SetParameter(eResponsePars::kInputCharge, inputCharge);

  SetParameter(eResponsePars::kBinTimeshift, binTimeshift);

  for (UInt_t isample = fvecResponsePars.at(static_cast<size_t>(eResponsePars::kNrPresamples)); isample < pulse.size();
       isample++) {

    auto response = Response(isample) / fMaxAdcToMipCal;
    //                                  calibrate response to MIP

    // Add baseline lvl to the response
    response += fBaselineLvl;

    // Add noise if requested (protect 0 against real numbers)
    if (fNoiseLvl > 1e-20) response += GetNoise(fNoiseLvl);

    // Check the sample for potential clipping. Since, we also have analog clipping in the same region as the digital clipping we also check for it here.
    // To deactivate simply set to an unreasonable high value
    if (response > fClippingStart) response = fClippingStart;
    pulse.at(isample) = response;
  }
  return pulse;
}

// ---- GetNoise ----
Int_t CbmTrdSpadic::GetNoise(Double_t sigmaNoise)
{
  Int_t noise = gRandom->Gaus(0, sigmaNoise);
  return noise;
}

// ---- GetAnalyticTimeshift ----
Double_t CbmTrdSpadic::GetAnalyticTimeshift(Double_t absolutetime)
{
  Double_t timeshift = (static_cast<Int_t>(absolutetime * 10) % static_cast<Int_t>(GetClockCycle() * 10)) / 10.0;

  timeshift = timeshift >= 0 ? timeshift - (GetClockCycle() / 2) : 0;

  return timeshift;
}

// ---- GetPeakingSamplePos ----
UInt_t CbmTrdSpadic::GetPeakingSamplePos()
{
  UInt_t peakbin =
    static_cast<UInt_t>(fvecResponsePars.at(static_cast<size_t>(eResponsePars::kShapingTime)) / fgClockCycleLength
                        + static_cast<UInt_t>(fvecResponsePars.at(static_cast<size_t>(eResponsePars::kNrPresamples))));
  return peakbin;
}

// ---- MaxAdcToEnergyCal ----
Float_t CbmTrdSpadic::MaxAdcToEnergyCal(Float_t maxadc)
{
  Float_t energy = maxadc * fMaxAdcToEnergyCal;

  return energy;
}

ClassImp(CbmTrdSpadic)
