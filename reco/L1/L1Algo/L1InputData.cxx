/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   L1InputData.cxx
/// \brief  Structure for input data to the L1 tracking algorithm (implementation)
/// \since  08.08.2022
/// \author Sergei Zharko <s.zharko@gsi.de>

#include "L1InputData.h"


// ---------------------------------------------------------------------------------------------------------------------
//
L1InputData::L1InputData() {}

// ---------------------------------------------------------------------------------------------------------------------
//
L1InputData::L1InputData(const L1InputData& other)
  : fHits(other.fHits)
  , fStreamStartIndices(other.fStreamStartIndices)
  , fStreamStopIndices(other.fStreamStopIndices)
  , fNhitKeys(other.fNhitKeys)
{
}

// ---------------------------------------------------------------------------------------------------------------------
//
L1InputData::L1InputData(L1InputData&& other) noexcept { this->Swap(other); }

// ---------------------------------------------------------------------------------------------------------------------
//
L1InputData& L1InputData::operator=(const L1InputData& other)
{
  if (this != &other) { L1InputData(other).Swap(*this); }
  return *this;
}

// ---------------------------------------------------------------------------------------------------------------------
//
L1InputData& L1InputData::operator=(L1InputData&& other) noexcept
{
  if (this != &other) {
    L1InputData tmp(std::move(other));
    this->Swap(tmp);
  }
  return *this;
}
