/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file L1IODataManager.h
/// \brief Input-output data manager for L1 tracking algorithm
/// \since 08.08.2022
/// \author S.Zharko <s.zharko@gsi.de>

#include "L1IODataManager.h"

#include "L1Algo.h"

// ---------------------------------------------------------------------------------------------------------------------
//
L1IODataManager::L1IODataManager(const L1Parameters* pParameters) : fpParameters(pParameters) {}

// ---------------------------------------------------------------------------------------------------------------------
//
bool L1IODataManager::SendInputData(L1Algo* pAlgo)
{
  // Set boundary hit indexes
  SetStartStopHitIndexes();

  // Check data before input
  if (CheckInputData<L1Constants::control::kInputDataQaLevel>()) {
    assert(pAlgo);
    pAlgo->ReceiveInputData(std::move(fInputData));
    return true;
  }
  LOG(error) << "L1: Attempt to set up inconsistent input data";
  return false;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1IODataManager::ResetInputData() noexcept
{
  L1InputData tmp;
  fInputData.Swap(tmp);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1IODataManager::SetStartStopHitIndexes()
{
  // TODO: probably, it is better to loop before the actual number of stations
  for (int iStation = 0; iStation < fpParameters->GetNstationsActive(); ++iStation) {
    fInputData.fvStartHitIndexes[iStation] =
      std::lower_bound(fInputData.fvHits.begin(), fInputData.fvHits.end(), iStation,
                       [](const L1Hit& hit, int ist) { return hit.iSt < ist; })
      - fInputData.fvHits.begin();

    fInputData.fvStopHitIndexes[iStation] =
      std::upper_bound(fInputData.fvHits.begin(), fInputData.fvHits.end(), iStation,
                       [](int ist, const L1Hit& hit) { return hit.iSt > ist; })
      - fInputData.fvHits.begin();

    // Account for stations with no hits
    if (fInputData.fvStartHitIndexes[iStation] == fInputData.fvStopHitIndexes[iStation]) {
      fInputData.fvStartHitIndexes[iStation] = 0;
      fInputData.fvStopHitIndexes[iStation]  = 0;
    }
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
