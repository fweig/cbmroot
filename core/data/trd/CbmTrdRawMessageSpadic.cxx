/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "CbmTrdRawMessageSpadic.h"

#include <Logger.h>  // for LOG

#include <algorithm>  // for max_element
#include <stdexcept>  // for range_error

// -------  Default Constructor  ----------------
CbmTrdRawMessageSpadic::CbmTrdRawMessageSpadic()
  : fChannelID()
  , fElinkID()
  , fCrobId()
  , fCriId()
  , fHitType()
  , fNrSamples()
  , fMultiHit()
  , fFullTime()
  , fSamples()
{
}

// -------          Constructor  ----------------
CbmTrdRawMessageSpadic::CbmTrdRawMessageSpadic(std::uint8_t channelId, std::uint8_t elinkId, std::uint8_t crobId,
                                               std::uint16_t criId, std::uint8_t hitType, std::uint8_t nrSamples,
                                               bool multiHit, std::uint64_t fullTime, std::vector<std::int16_t> samples)
  : fChannelID {channelId}
  , fElinkID {elinkId}
  , fCrobId(crobId)
  , fCriId {criId}
  , fHitType {hitType}
  , fNrSamples {nrSamples}
  , fMultiHit {multiHit}
  , fFullTime {fullTime}
  , fSamples(samples)
{
  // throw an exception if samples has more than 32 elements.
  if (fSamples.size() > 32) {
    LOG(error) << "CbmTrdRawMessageSpadic: too many samples in message!";
    throw std::range_error("CbmTrdRawMessageSpadic: too many samples in message!");
  }
}

// -------  copy    Constructor  ----------------
CbmTrdRawMessageSpadic::CbmTrdRawMessageSpadic(const CbmTrdRawMessageSpadic& old)
  : fChannelID {old.fChannelID}
  , fElinkID {old.fElinkID}
  , fCrobId(old.fCrobId)
  , fCriId {old.fCriId}
  , fHitType {old.fHitType}
  , fNrSamples {old.fNrSamples}
  , fMultiHit {old.fMultiHit}
  , fFullTime {old.fFullTime}
  , fSamples(old.fSamples)
{
}

// -------           Destructor  ----------------
CbmTrdRawMessageSpadic::~CbmTrdRawMessageSpadic() {}

int16_t CbmTrdRawMessageSpadic::GetMaxAdc()
{
  int16_t maxADC = *std::max_element(fSamples.begin(), fSamples.end());

  return maxADC;
}

void CbmTrdRawMessageSpadic::SetSample(std::int16_t value, std::uint8_t pos)
{
  if (pos > 31 || value < -256 || value > 256 || pos >= fNrSamples) {
    LOG(error) << "CbmTrdRawMessageSpadic::SetSample() Out of range!";
    return;
  }
  if ((std::uint8_t)(pos + 1) > fSamples.size()) { fSamples.resize(pos + 1); }
  fSamples[pos] = value;

  return;
}

ClassImp(CbmTrdRawMessageSpadic)
