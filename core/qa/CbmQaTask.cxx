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
  , fsPrefix(prefix)
  , fbUseMC(isMCUsed)
{
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmQaTask::Exec(Option_t* /*option*/)
{
  fNofEvents.SetVal(fNofEvents.GetVal() + 1);
  LOG_IF(info, fVerbose > 1) << fName << ": event " << fNofEvents.GetVal();
  FillHistograms();
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
  pSink->WriteObject(fpFolderRoot.get(), nullptr);
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmQaTask::Init()
{
  LOG_IF(info, fVerbose > 0) << fName << ": initializing task ...";

  InitStatus res = kSUCCESS;

  // ----- Clear map of the histograms (note)
  DeInitBase();

  // ----- Initialize data branches
  LOG_IF(info, fVerbose > 1) << fName << ": initializing input data branches";
  res = std::max(res, InitDataBranches());

  // ----- Initialize I/O
  fpFolderRoot = std::make_unique<TFolder>(fName, fName);  // The name of the folder follows the name of the task
  fpFolderRoot->SetOwner(true);                            // When true, TFolder owns all added objects
  fpFolderRoot->Add(&fNofEvents);

  // ----- Initialize histograms and canvases
  LOG_IF(info, fVerbose > 1) << fName << ": initializing histograms";
  res = std::max(res, InitHistograms());

  fNofEvents.SetVal(0);

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

  delete fpFolderRoot.get();
  fpFolderRoot = nullptr;

  // De-initialize particular QA-task implementation
  DeInit();
}


// ---------------------------------------------------------------------------------------------------------------------
// NOTE: Example
void CbmQaTask::SetHistoProperties(TH1* pHisto)
{
  constexpr double kHdefTextSize = 0.04;

  pHisto->SetStats(kTRUE);
  pHisto->Sumw2();

  // Axis property settings
  std::array<TAxis*, 3> vpAxes = {pHisto->GetXaxis(), pHisto->GetYaxis(), pHisto->GetZaxis()};
  for (auto* pAxis : vpAxes) {
    pAxis->SetTitleSize(kHdefTextSize);
    pAxis->SetLabelSize(kHdefTextSize);
  }
  vpAxes[0]->SetNdivisions(504);
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmQaTask::InitDataBranches()
{
  LOG_IF(info, fVerbose > 1) << fName << ": data branches initialization function is not defined";
  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmQaTask::InitHistograms()
{
  LOG_IF(info, fVerbose > 1) << fName << ": histogram initialization function is not defined";
  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmQaTask::FillHistograms()
{
  LOG_IF(info, fVerbose > 1) << fName << ": histogram filling function is not defined";
}

// ---------------------------------------------------------------------------------------------------------------------
//
InitStatus CbmQaTask::InitCanvases()
{
  LOG_IF(info, fVerbose > 1) << fName << ": no initialization of canvases is provided";
  return kSUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmQaTask::DeInit() { LOG_IF(info, fVerbose > 1) << fName << ": no extra de-initialization is provided"; }
