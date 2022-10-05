/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CbmCaPerformance.cxx
/// \brief  CA Tracking performance interface for CBM (implementation)
/// \since  23.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#include "CbmCaPerformance.h"

#include "CbmEvent.h"
#include "CbmL1.h"  // for L1DetectorID
#include "CbmMCDataManager.h"

#include "FairLogger.h"
#include "FairRootManager.h"

#include <cassert>
#include <stdexcept>  // for std::logic_error


// *********************************
// ** Action definition functions **
// *********************************

// ---------------------------------------------------------------------------------------------------------------------
//
bool CbmCaPerformance::Init()
try {
  LOG(info) << "Initializing CA tracking Monte-Carlo module... ";

  auto fairManager = FairRootManager::Instance();
  assert(fairManager);

  auto mcManager = dynamic_cast<CbmMCDataManager*>(fairManager->GetObject("MCDataManager"));
  assert(mcManager);

  fpMvdPoints  = nullptr;
  fpStsPoints  = nullptr;
  fpMuchPoints = nullptr;
  fpTrdPoints  = nullptr;
  fpTofPoints  = nullptr;

  if (fbUseMvd) {
    LOG(info) << "CA MC Module: initializing branches for MVD";
    fpMvdPoints = mcManager->InitBranch("MvdPoint");
  }

  if (fbUseSts) {
    LOG(info) << "CA MC Module: initializing branches for STS";
    fpStsPoints = mcManager->InitBranch("StsPoint");
  }

  if (fbUseMuch) {
    LOG(info) << "CA MC Module: initializing branches for MuCh";
    fpMuchPoints = mcManager->InitBranch("MuchPoint");
  }

  if (fbUseTrd) {
    LOG(info) << "CA MC Module: initializing branches for TRD";
    fpTrdPoints = mcManager->InitBranch("TrdPoint");
  }

  if (fbUseTof) {
    LOG(info) << "CA MC Module: initializing branches for TOF";
    fpTofPoints = mcManager->InitBranch("TofPoint");
  }

  // Check initialization
  this->CheckInit();

  LOG(info) << "Initializing CA tracking Monte-Carlo module... \033[1;32mDone!\033[0m";
  return true;
}
catch (const std::logic_error& error) {
  LOG(info) << "Initializing CA tracking Monte-Carlo module... \033[1;31mFailed\033[0m\nReason: " << error.what();
  return false;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaPerformance::ProcessEvent(CbmEvent* pEvent)
{
  assert(pEvent);
  std::cout << "\033[1;32mProcessing performance event\033[0m\n";
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaPerformance::Finish() { std::cout << "\033[1;32mFinishing performance\033[0m\n"; }


// ***********************
// **     Accessors     **
// ***********************

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaPerformance::SetDetector(L1DetectorID detID, bool flag)
{
  switch (detID) {
    case L1DetectorID::kMvd: fbUseMvd = flag; break;
    case L1DetectorID::kSts: fbUseSts = flag; break;
    case L1DetectorID::kMuch: fbUseMuch = flag; break;
    case L1DetectorID::kTrd: fbUseTrd = flag; break;
    case L1DetectorID::kTof: fbUseTof = flag; break;
  }
}


// *******************************
// **     Utility functions     **
// *******************************

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaPerformance::CheckInit() const
{
  // Check parameters
  if (!fpParameters) { throw std::logic_error("Tracking parameters object was not defined"); }

  // Check detectors initialization
  if (fbUseMvd) {
    if (!fpMvdPoints) { throw std::logic_error("MC points unavailable for MVD"); }
  }

  if (fbUseSts) {
    if (!fpStsPoints) { throw std::logic_error("MC points unavailable for STS"); }
  }

  if (fbUseMuch) {
    if (!fpMuchPoints) { throw std::logic_error("MC points unavailable for MuCh"); }
  }

  if (fbUseTrd) {
    if (!fpTrdPoints) { throw std::logic_error("MC points unavailable for TRD"); }
  }

  if (fbUseTof) {
    if (!fpTofPoints) { throw std::logic_error("MC points unavailable for TOF"); }
  }
}
