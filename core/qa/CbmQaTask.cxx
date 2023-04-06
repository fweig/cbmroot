/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CbmQaTask.cxx
/// \brief  A base class for CBM QA task logic (implementation)
/// \author S.Zharko <s.zharko@lx-pool.gsi.de>
/// \data   12.01.2023


#include "CbmQaTask.h"

#include "CbmQaCanvas.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairSink.h"

#include "TAxis.h"

#include <array>

ClassImp(CbmQaTask);

// ---------------------------------------------------------------------------------------------------------------------
//
CbmQaTask::CbmQaTask(const char* name, const char* prefix, int verbose, bool isMCUsed)
  : FairTask(name, verbose)
  , CbmQaIO(name, prefix)
  , fbUseMC(isMCUsed)
{
  fStoringMode = CbmQaIO::EStoringMode::kSUBDIR;  // mode of objects arrangement in the output file
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmQaTask::Exec(Option_t* /*option*/)
{
  fNofEvents.SetVal(fNofEvents.GetVal() + 1);
  LOG_IF(info, fVerbose > 1) << fName << ": event " << fNofEvents.GetVal();
  this->InitTimeSlice();
  this->FillHistograms();
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmQaTask::Finish()
{
  // Processes histograms in the end of the run
  LOG(info) << fName << ": Checking QA...";
  bool qaResult = Check();
  LOG(info) << fName << ": Checking QA: " << (qaResult ? "\033[1;32mpassed\033[0m" : "\033[1;31mfailed\033[0m");

  // Processes canvases in the end of the run
  LOG_IF(info, fVerbose > 1) << fName << ": initializing canvases";
  InitCanvases();

  // Write the root folder to sinker
  FairSink* pSink = FairRootManager::Instance()->GetSink();
  LOG_IF(fatal, !pSink) << fName << ": sink file was not found";
  pSink->WriteObject(fpFolderRoot, nullptr);
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmQaTask::Init()
{
  LOG_IF(info, fVerbose > 0) << fName << ": initializing task ...";
  InitStatus res = kSUCCESS;

  // ----- Open configuration file
  YAML::Node config;
  if (fsConfigName.Length()) {
    LOG(info) << fName << ": reading configuration from file \"" << fsConfigName << "\"";
    try {
      config        = YAML::LoadFile(fsConfigName.Data())["qa"][fName.Data()];
      fpCurrentNode = &config;
    }
    catch (const YAML::BadFile& exc) {
      LOG(fatal) << fName << ": configuration file \"" << fsConfigName << "\" does not exist";
    }
    catch (const YAML::ParserException& exc) {
      LOG(fatal) << fName << ": configuration file \"" << fsConfigName << "\" is formatted improperly";
    }
  }

  // ----- Clear map of the histograms (note)
  DeInitBase();

  // ----- Initialize data branches
  LOG_IF(info, fVerbose > 1) << fName << ": initializing input data branches";
  res = std::max(res, InitDataBranches());

  // ----- Initialize I/O
  fpFolderRoot->Add(&fNofEvents);

  // ----- Initialize histograms
  LOG_IF(info, fVerbose > 1) << fName << ": initializing histograms";
  res = std::max(res, InitHistograms());

  fNofEvents.SetVal(0);

  fpCurrentNode = nullptr;  // De-init pointer to
  return res;
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmQaTask::ReInit()
{
  LOG_IF(info, fVerbose > 2) << fName << "::DeInitBase() is called";
  return Init();
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmQaTask::DeInitBase()
{
  LOG_IF(info, fVerbose > 2) << fName << "::DeInitBase() is called";
  // De-initialize basic data members
  // ...

  // De-initialize particular QA-task implementation
  DeInit();
}
