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
  LOG_IF(fatal, !fpParameters.get())
    << fName << ": CA parameters object is not defined. Please, provide initializer or read parameters from binary "
    << "via OutputQa::ReadParameters(filename) from the qa macro";


  // Initialize IO data manager
  if (!fpDataManager.get()) { fpDataManager = std::make_shared<L1IODataManager>(); }

  // Initialize time slice reader instance
  if (!fpTSReader.get()) { fpTSReader = std::make_unique<TimeSliceReader>(fTrackingMode); }
  fpTSReader->SetDetector(L1DetectorID::kMvd, fbUseMvd);
  fpTSReader->SetDetector(L1DetectorID::kSts, fbUseSts);
  fpTSReader->SetDetector(L1DetectorID::kMuch, fbUseMuch);
  fpTSReader->SetDetector(L1DetectorID::kTrd, fbUseTrd);
  fpTSReader->SetDetector(L1DetectorID::kTof, fbUseTof);

  fpTSReader->RegisterIODataManager(fpDataManager);
  fpTSReader->RegisterTracksContainer(fvRecoTracks);
  fpTSReader->RegisterHitDebugInfoContainer(fvDbgHits);
  //fpTSReader->RegisterHitIndexContainer(fvHitIds);

  fpTSReader->InitRun();

  // Initialize MC module
  if (IsMCUsed()) {
    if (!fpMCModule.get()) { fpMCModule = std::make_unique<CbmCaMCModule>(fVerbose, fPerformanceMode); }
    fpMCModule->SetDetector(L1DetectorID::kMvd, fbUseMvd);
    fpMCModule->SetDetector(L1DetectorID::kSts, fbUseSts);
    fpMCModule->SetDetector(L1DetectorID::kMuch, fbUseMuch);
    fpMCModule->SetDetector(L1DetectorID::kTrd, fbUseTrd);
    fpMCModule->SetDetector(L1DetectorID::kTof, fbUseTof);

    fpMCModule->RegisterInputData(fInputData);
    fpMCModule->RegisterMCData(fMCData);
    fpMCModule->RegisterRecoTrackContainer(fvRecoTracks);
    fpMCModule->RegisterHitIndexContainer(fvHitIds);
    fpMCModule->RegisterParameters(fpParameters);

    fpMCModule->InitRun();
  }


  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus OutputQa::InitTimeSlice()
{
  int nMCTracks   = -1;
  int nMCPoints   = -1;
  int nHits       = -1;
  int nRecoTracks = -1;

  // Read hits: fill fvHitIds, fvDbgHits and fpDataManager
  fpTSReader->ReadHits();
  fpDataManager->SendInputData(fInputData);

  // Read reconstructed tracks
  fpTSReader->ReadRecoTracks();

  nHits       = fInputData.GetNhits();
  nRecoTracks = fvRecoTracks.size();

  if (IsMCUsed()) {
    // Read MC information
    fpMCModule->InitEvent(nullptr);
    nMCTracks = fMCData.GetNofTracks();
    nMCPoints = fMCData.GetNofPoints();
  }

  LOG(info) << fName << ": Event or time slice consists from " << nHits << " hits, " << nRecoTracks << " reco tracks, "
            << nMCTracks << " MC tracks, " << nMCPoints << " MC points";

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
