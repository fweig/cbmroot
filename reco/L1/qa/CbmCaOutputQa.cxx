/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmCaOutputQa.cxx
/// @brief  Tracking output QA-task (implementation)
/// @since  24.02.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#include "CbmCaOutputQa.h"

#include "Logger.h"

#include "L1InitManager.h"

using ca::tools::Debugger;
using cbm::ca::OutputQa;

// ---------------------------------------------------------------------------------------------------------------------
//
OutputQa::OutputQa(int verbose, bool isMCUsed) : CbmQaTask("CbmCaOutputQa", "caout", verbose, isMCUsed) {}

// ---------------------------------------------------------------------------------------------------------------------
//
void OutputQa::EnableDebugger(const char* filename)
{
  if (!fpDebugger.get()) { fpDebugger = std::make_shared<Debugger>(filename); }
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus OutputQa::InitDataBranches()
{
  LOG(info) << fName << ": Initializing data branches";

  if (!fpTSReader.get()) { fpTSReader = std::make_unique<TimeSliceReader>(fTrackingMode); }


  if (!fpDataManager.get()) { fpDataManager = std::make_shared<L1IODataManager>(); }

  // Initialize time slice reader instance
  fpTSReader->SetDetector(L1DetectorID::kMvd, fbUseMvd);
  fpTSReader->SetDetector(L1DetectorID::kSts, fbUseSts);
  fpTSReader->SetDetector(L1DetectorID::kMuch, fbUseMuch);
  fpTSReader->SetDetector(L1DetectorID::kTrd, fbUseTrd);
  fpTSReader->SetDetector(L1DetectorID::kTof, fbUseTof);

  fpTSReader->RegisterIODataManager(fpDataManager);
  fpTSReader->RegisterTracksContainer(fvRecoTracks);
  fpTSReader->RegisterHitDebugInfoContainer(fvDbgHits);
  fpTSReader->RegisterHitIndexContainer(fvHitIds);

  fpTSReader->InitRun();
  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus OutputQa::InitTimeSlice()
{
  // Read hits: fill fvHitIds, fvDbgHits and fpDataManager
  fpTSReader->ReadHits();
  fpDataManager->SendInputData(fInputData);

  // Read reconstructed tracks
  fpTSReader->ReadRecoTracks();
  LOG(info) << fName << ": Time slice was read: " << fInputData.GetNhits() << " hits, " << fvRecoTracks.size()
            << " tracks";

  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void OutputQa::ReadParameters(const char* filename)
{
  fpParameters = std::make_shared<L1Parameters>();

  L1InitManager manager;
  manager.ReadParametersObject(filename);
  manager.SendParameters(*fpParameters);

  LOG(info) << fpParameters->ToString(10);
}
