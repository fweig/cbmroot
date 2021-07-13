/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

#include "CbmTrdRawToDigiBaseR.h"

#include "CbmTrdDigi.h"
#include "CbmTrdRawMessageSpadic.h"

#include <Rtypes.h>


//---- CbmTrdRawToDigiBaseR ----
CbmTrdRawToDigiBaseR::CbmTrdRawToDigiBaseR() : TObject() {}

// ---- GetBaseline ----
Float_t CbmTrdRawToDigiBaseR::GetBaseline(const std::vector<std::int16_t>* samples)
{
  // The spadic 2.2 has a functionality that an average baseline can be written to the first sample. So first we have to check if this is active.
  if (fSpadic->GetUseBaselineAvg()) return samples->at(0);
  else {
    Float_t baseline = 0.0;
    auto itend       = samples->begin() + fNrOfPresamples;
    if (itend > samples->end()) itend = samples->end();
    for (auto isample = samples->begin(); isample < itend; isample++) {
      baseline += *isample;
    }
    baseline /= fNrOfPresamples;

    return baseline;
  }
}

// ---- GetDigiTriggerType ----
CbmTrdDigi::eTriggerType CbmTrdRawToDigiBaseR::GetDigiTriggerType(Spadic::eTriggerType tt)
{
  auto digiTriggerType = CbmTrdDigi::eTriggerType::kNTrg;
  // Shift self trigger to digi selftrigger
  // Shift neighbour trigger to digi neighbour
  // Hide spadic kSandN in Self
  switch (tt) {
    case Spadic::eTriggerType::kGlobal: digiTriggerType = CbmTrdDigi::eTriggerType::kNTrg; break;
    case Spadic::eTriggerType::kSelf: digiTriggerType = CbmTrdDigi::eTriggerType::kSelf; break;
    case Spadic::eTriggerType::kNeigh: digiTriggerType = CbmTrdDigi::eTriggerType::kNeighbor; break;
    case Spadic::eTriggerType::kSandN: digiTriggerType = CbmTrdDigi::eTriggerType::kSelf; break;
  }
  return digiTriggerType;
}

// ---- MakeDigi ----
std::unique_ptr<CbmTrdDigi> CbmTrdRawToDigiBaseR::MakeDigi(const std::vector<std::int16_t>* samples, Int_t padChNr,
                                                           Int_t uniqueModuleId, ULong64_t time,
                                                           CbmTrdDigi::eTriggerType triggerType, Int_t errClass)
{
  // Get the timeshift and set the member, which is required for some of the rtd methods
  fCurrentTimeshift = GetBinTimeShift(samples);

  // In this case of CbmTrdRawToDigi GetCharge calls GetBinTimeshift, since the information is needed. The shift is stored in fCurrentTimeshift
  // Hence, the order of charge and time assignement here makes a difference!
  auto maxadc = GetMaxAdcValue(samples);

  // Get energy from maxadc value
  auto energy = fSpadic->MaxAdcToEnergyCal(maxadc);

  // In simulation since we often start at time = 0 there is a non negligible chance that a time < 0 is extracted. Since, this is not allowed in the code we set it to 0 for these cases
  time = time > fCurrentTimeshift ? time - fCurrentTimeshift : 0;

  auto digi = std::unique_ptr<CbmTrdDigi>(new CbmTrdDigi(padChNr, uniqueModuleId, energy, time, triggerType, errClass));
  return digi;
}

ClassImp(CbmTrdRawToDigiBaseR)
