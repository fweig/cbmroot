/* Copyright (C) 2019-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin, David Emschermann [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmMcbm2018UnpackerTaskPsd                    -----
// -----                 Created 09.10.2019 by N.Karpushkin                -----
// -----        based on CbmMcbm2018UnpackerTaskTof by P.-A. Loizeau       -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018UnpackerTaskPsd.h"

#include "CbmMcbm2018PsdPar.h"
#include "CbmMcbm2018UnpackerAlgoPsd.h"

#include "FairParGenericSet.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TH1.h"
#include "THttpServer.h"
#include "TROOT.h"
#include "TString.h"
#include <TFile.h>

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <stdint.h>

Bool_t bMcbm2018UnpackerTaskPsdResetHistos = kFALSE;

CbmMcbm2018UnpackerTaskPsd::CbmMcbm2018UnpackerTaskPsd(UInt_t /*uNbGdpb*/)
  : CbmMcbmUnpack()
  , fbMonitorMode(kFALSE)
  , fbDebugMonitorMode(kFALSE)
  , fbWriteOutput(kTRUE)
  , fbDebugWriteOutput(kFALSE)
  , fulTsCounter(0)
  , fPsdDigiVector(nullptr)
  , fPsdDspVector(nullptr)
  , fUnpackerAlgo(nullptr)
{
  LOG(info) << "CbmMcbm2018UnpackerTaskPsd::Allocate";
  fUnpackerAlgo = new CbmMcbm2018UnpackerAlgoPsd();
}

CbmMcbm2018UnpackerTaskPsd::~CbmMcbm2018UnpackerTaskPsd() { delete fUnpackerAlgo; }

Bool_t CbmMcbm2018UnpackerTaskPsd::Init()
{
  LOG(info) << "CbmMcbm2018UnpackerTaskPsd::Init";
  LOG(info) << "Initializing mCBM PSD 2018 Unpacker";
  Bool_t initOK = kTRUE;

  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman) { LOG(fatal) << "No FairRootManager instance"; }

  /// Register Digi output vector.
  fPsdDigiVector = new std::vector<CbmPsdDigi>();
  if (fPsdDigiVector) {
    ioman->RegisterAny("PsdDigi", fPsdDigiVector, fbWriteOutput);
    initOK &= fUnpackerAlgo->SetDigiOutputPointer(fPsdDigiVector);
  }
  else {
    LOG(fatal) << "fPsdDigiVector could not be registered at FairRootManager";
  }

  /// Register RawMessage output vector, if DebugWrite is enabled.
  if (fbDebugWriteOutput) {
    fPsdDspVector = new std::vector<CbmPsdDsp>();
    if (fPsdDspVector) {
      ioman->RegisterAny("PsdDsp", fPsdDspVector, fbDebugWriteOutput);
      initOK &= fUnpackerAlgo->SetDspOutputPointer(fPsdDspVector);
    }
    else {
      LOG(fatal) << "fPsdDspVector could not be registered at FairRootManager";
    }
  }

  return initOK;
}

void CbmMcbm2018UnpackerTaskPsd::SetParContainers()
{
  LOG(info) << "Setting parameter containers for " << GetName();

  TList* fParCList = fUnpackerAlgo->GetParList();

  for (Int_t iparC = 0; iparC < fParCList->GetEntries(); ++iparC) {
    FairParGenericSet* tempObj = (FairParGenericSet*) (fParCList->At(iparC));
    fParCList->Remove(tempObj);

    std::string sParamName {tempObj->GetName()};
    FairParGenericSet* newObj =
      dynamic_cast<FairParGenericSet*>(FairRun::Instance()->GetRuntimeDb()->getContainer(sParamName.data()));

    if (nullptr == newObj) {
      LOG(error) << "Failed to obtain parameter container " << sParamName << ", for parameter index " << iparC;
      return;
    }  // if( nullptr == newObj )

    fParCList->AddAt(newObj, iparC);
    //      delete tempObj;
  }  // for( Int_t iparC = 0; iparC < fParCList->GetEntries(); ++iparC )
}

Bool_t CbmMcbm2018UnpackerTaskPsd::InitContainers()
{
  LOG(info) << "Init parameter containers for " << GetName();

  /// Control flags
  CbmMcbm2018PsdPar* pUnpackPar =
    dynamic_cast<CbmMcbm2018PsdPar*>(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmMcbm2018PsdPar"));
  if (nullptr == pUnpackPar) {
    LOG(error) << "Failed to obtain parameter container CbmMcbm2018PsdPar";
    return kFALSE;
  }  // if( nullptr == pUnpackPar )

  Bool_t initOK = fUnpackerAlgo->InitContainers();

  return initOK;
}

Bool_t CbmMcbm2018UnpackerTaskPsd::ReInitContainers()
{
  LOG(info) << "ReInit parameter containers for " << GetName();
  Bool_t initOK = fUnpackerAlgo->ReInitContainers();

  return initOK;
}

void CbmMcbm2018UnpackerTaskPsd::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  fUnpackerAlgo->AddMsComponentToList(component, usDetectorId);
}

Bool_t CbmMcbm2018UnpackerTaskPsd::DoUnpack(const fles::Timeslice& ts, size_t /*component*/)
{

  if (fbMonitorMode && bMcbm2018UnpackerTaskPsdResetHistos) {
    LOG(info) << "Reset PSD unpacker histos ";
    fUnpackerAlgo->ResetHistograms();
    bMcbm2018UnpackerTaskPsdResetHistos = kFALSE;
  }  // if( fbMonitorMode && bMcbm2018UnpackerTaskPsdResetHistos )

  if (kFALSE == fUnpackerAlgo->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index() << " in unpacker algorithm class";
    return kTRUE;
  }  // if( kFALSE == fUnpackerAlgo->ProcessTs( ts ) )

  /*
   /// Sort the buffers of hits due to the time offsets applied
   => Done in the algo!!!
   sort(fPsdDigiVector->begin(), fPsdDigiVector->end(),
        [](const CbmPsdDigi & a, const CbmPsdDigi & b) -> bool
        {
          return a.GetTime() < b.GetTime();
        });
*/

  /*
*/
  if (0 == fulTsCounter % 10000) LOG(info) << "Processed " << fulTsCounter << "TS";
  fulTsCounter++;

  return kTRUE;
}

void CbmMcbm2018UnpackerTaskPsd::Reset()
{
  if (fPsdDigiVector) fPsdDigiVector->clear();
  if (fPsdDspVector) fPsdDspVector->clear();
}


void CbmMcbm2018UnpackerTaskPsd::Finish()
{
  LOG(info) << "Finish of CbmMcbm2018UnpackerTaskPsd";
  fUnpackerAlgo->Finish();
}

void CbmMcbm2018UnpackerTaskPsd::SetDspWriteMode(Bool_t bFlagIn)
{
  fbDebugWriteOutput = bFlagIn;
  fUnpackerAlgo->SetDspWriteMode(bFlagIn);
}
void CbmMcbm2018UnpackerTaskPsd::SetIgnoreOverlapMs(Bool_t bFlagIn) { fUnpackerAlgo->SetIgnoreOverlapMs(bFlagIn); }
void CbmMcbm2018UnpackerTaskPsd::SetTimeOffsetNs(Double_t dOffsetIn) { fUnpackerAlgo->SetTimeOffsetNs(dOffsetIn); }

ClassImp(CbmMcbm2018UnpackerTaskPsd)
