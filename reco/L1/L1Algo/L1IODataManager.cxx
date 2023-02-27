/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file L1IODataManager.h
/// \brief Input-output data manager for L1 tracking algorithm
/// \since 08.08.2022
/// \author S.Zharko <s.zharko@gsi.de>

#include "L1IODataManager.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <fstream>

#include "L1Algo.h"

// ---------------------------------------------------------------------------------------------------------------------
//
L1IODataManager::L1IODataManager(const L1Parameters* pParameters) : fpParameters(pParameters) {}

// ---------------------------------------------------------------------------------------------------------------------
//
bool L1IODataManager::SendInputData(L1Algo* pAlgo)
{
  // Set boundary hit indexes
  InitData();

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
void L1IODataManager::ReadInputData(const std::string& fileName)
{
  // Reset input data object
  this->ResetInputData();
  LOG(info) << "L1: Input data will be read from file \"" << fileName << "\"";

  // Open input binary file
  std::ifstream ifs(fileName, std::ios::binary);
  if (!ifs) { LOG(fatal) << "L1: input data reader: data file \"" << fileName << "\" was not found"; }

  // Get L1InputData object
  try {
    boost::archive::binary_iarchive ia(ifs);
    ia >> fInputData;
  }
  catch (const std::exception&) {
    LOG(fatal) << "L1: input data reader: data file \"" << fileName << "\" has incorrect data format or was corrupted";
  }
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
void L1IODataManager::InitData()
{
  // Sort hits within stations
  L1Vector<L1Hit> fvHitsNew;
  fvHitsNew.reset(fInputData.fvHits.size());

  std::fill(fvHitIndex.begin(), fvHitIndex.end(), 0);
  std::fill(fInputData.fvStartHitIndexes.begin(), fInputData.fvStartHitIndexes.end(), 0);

  // Count number of hits in each station
  for (const auto& hit : fInputData.fvHits) {
    ++fInputData.fvStartHitIndexes[hit.iSt + 1];
  }

  // Fill bordering numbers of hits for each station
  for (int iSt = 0; iSt < fpParameters->GetNstationsActive(); ++iSt) {
    fInputData.fvStartHitIndexes[iSt + 1] += fInputData.fvStartHitIndexes[iSt];
  }

  // Save ordered hits to new vector
  for (const auto& hit : fInputData.fvHits) {
    int iSt                                                            = hit.iSt;
    fvHitsNew[fInputData.fvStartHitIndexes[iSt] + (fvHitIndex[iSt]++)] = hit;
  }

  // Swap contents of old and new hits vector
  std::swap(fvHitsNew, fInputData.fvHits);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1IODataManager::WriteInputData(const std::string& fileName) const
{
  // Check current data object for consistency
  if (!CheckInputData<L1Constants::control::kInputDataQaLevel>()) {
    LOG(error) << "L1: input data writer: attempt to write invalid input data object to file \"" << fileName << "\"";
    return;
  }

  // Open output binary file
  std::ofstream ofs(fileName, std::ios::binary);
  if (!ofs) {
    LOG(error) << "L1: input data writer: failed opening file \"" << fileName << " for writing input data\"";
    return;
  }

  // Serialize L1InputData object and write
  boost::archive::binary_oarchive oa(ofs);
  oa << fInputData;
}
