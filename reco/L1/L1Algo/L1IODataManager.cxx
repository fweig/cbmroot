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
bool L1IODataManager::SendInputData(L1InputData& destination)
{
  // Set boundary hit indexes
  InitData();

  // Check data before input
  if (CheckInputData<L1Constants::control::kInputDataQaLevel>()) {
    destination = std::move(fInputData);
    assert(this->GetNofHits() == 0);
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
  ResetInputData();
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
  fLastStreamId = -1;
  fInputData.fStreamStartIndices.reserve(2000);
  fInputData.fStreamStopIndices.reserve(2000);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1IODataManager::InitData()
{
  // set the end pointers to data streams

  //std::cout << "N data streams: " << fInputData.fStreamStartIndices.size() << std::endl;

  if (fInputData.fStreamStartIndices.size() > 3000) {
    LOG(warning) << "L1: unexpected order of input data: too many data streams!!! ";
    fInputData.fStreamStartIndices.reduce(3000);
  }
  int nStreams = fInputData.fStreamStartIndices.size();
  fInputData.fStreamStopIndices.reset(nStreams);
  for (int i = 0; i < nStreams - 1; i++) {
    fInputData.fStreamStopIndices[i] = fInputData.fStreamStartIndices[i + 1];
  }
  if (nStreams > 0) { fInputData.fStreamStopIndices[nStreams - 1] = fInputData.fHits.size(); }
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
