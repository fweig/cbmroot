/* Copyright (C) 2019-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Egor Ovcharenko [committer], Pierre-Alain Loizeau */

/**
 * CbmMcbm2018UnpackerTaskRich2020
 * E. Ovcharenko, Mar 2019
 * based on other detectors' classes by P.-A. Loizeau
 */

//TODO: check that all data members are initialized in the constructor

#include "CbmMcbm2018UnpackerTaskRich2020.h"

// ROOT
#include <TCanvas.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <THttpServer.h>
#include <TROOT.h>

// FairRoot
#include <FairParGenericSet.h>
#include <FairRootManager.h>
#include <FairRun.h>
#include <FairRunOnline.h>
#include <FairRuntimeDb.h>
#include <Logger.h>

// CbmRoot
#include "CbmMcbm2018RichPar.h"
#include "CbmMcbm2018UnpackerAlgoRich2020.h"

#include <thread>

#include <chrono>

//TODO global variable, really?
Bool_t bMcbm2018UnpackerTaskRichResetHistos2020 = kFALSE;

CbmMcbm2018UnpackerTaskRich2020::CbmMcbm2018UnpackerTaskRich2020()
  : CbmMcbmUnpack()
  , fbMonitorMode(kFALSE)
  , fbDebugMonitorMode(kFALSE)
  , fbWriteOutput(kTRUE)
  , fUnpackerAlgo(new CbmMcbm2018UnpackerAlgoRich2020())
{
}

CbmMcbm2018UnpackerTaskRich2020::~CbmMcbm2018UnpackerTaskRich2020()
{
  if (fUnpackerAlgo) {
    delete fUnpackerAlgo;
    fUnpackerAlgo = nullptr;
  }
}

Bool_t CbmMcbm2018UnpackerTaskRich2020::Init()
{
  LOG(info) << "CbmMcbm2018UnpackerTaskRich2020::Init";
  LOG(info) << "Initializing mCBM RICH 2018 Unpacker";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) { LOG(fatal) << "No FairRootManager instance"; }

  /// Get address of vector from algo
  fpvDigiRich = &(fUnpackerAlgo->GetVector());
  ioman->RegisterAny("RichDigi", fpvDigiRich, fbWriteOutput);

  return kTRUE;
}

Bool_t CbmMcbm2018UnpackerTaskRich2020::DoUnpack(const fles::Timeslice& ts, size_t component)
{
  //std::this_thread::sleep_for(std::chrono::milliseconds(10));
  if (fbMonitorMode && bMcbm2018UnpackerTaskRichResetHistos2020) {
    LOG(info) << "Reset RICH unpacker histos ";
    fUnpackerAlgo->ResetHistograms();
    bMcbm2018UnpackerTaskRichResetHistos2020 = kFALSE;
  }  // if( fbMonitorMode && bMcbm2018UnpackerTaskRichResetHistos2020 )

  if (kFALSE == fUnpackerAlgo->ProcessTs(ts, component)) {
    LOG(error) << "Failed processing TS " << ts.index() << " in mRICH unpacker algorithm class";
    return kTRUE;
  }  // if( kFALSE == fUnpackerAlgo->ProcessTs( ts, component ) )

  /*
   /// Sort the buffers of hits due to the time offsets applied
   //=> Done in the algo!!!
   sort(fpvDigiRich->begin(), fpvDigiRich->end(),
        [](const CbmRichDigi & a, const CbmRichDigi & b) -> bool
        {
          return a.GetTime() < b.GetTime();
        });
*/

  if (0 == fulTsCounter % 10000) LOG(info) << "Processed " << fulTsCounter << "TS";
  fulTsCounter++;

  return kTRUE;
}

void CbmMcbm2018UnpackerTaskRich2020::Reset() { fUnpackerAlgo->ClearVector(); }

/**
  Copied from the CbmMcbm2018UnpackerTaskSts class without giving any thinking...
*/
void CbmMcbm2018UnpackerTaskRich2020::Finish()
{
  /// If monitor mode enabled, trigger histos creation, obtain pointer on them and add them to the HTTP server
  if (kTRUE == fbMonitorMode) {
    /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
    std::vector<std::pair<TNamed*, std::string>> vHistos = fUnpackerAlgo->GetHistoVector();

    /// Save old global file and folder pointer to avoid messing with FairRoot
    TFile* oldFile     = gFile;
    TDirectory* oldDir = gDirectory;

    TFile* histoFile = nullptr;

    // open separate histo file in recreate mode
    histoFile = new TFile("data/HistosUnpackerRich.root", "RECREATE");
    histoFile->cd();

    /// Register the histos in the HTTP server
    for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
      /// Make sure we end up in chosen folder
      TString sFolder = vHistos[uHisto].second.data();
      if (nullptr == gDirectory->Get(sFolder)) gDirectory->mkdir(sFolder);
      gDirectory->cd(sFolder);

      /// Write plot
      vHistos[uHisto].first->Write();

      histoFile->cd();
    }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

    /// Restore old global file and folder pointer to avoid messing with FairRoot
    gFile      = oldFile;
    gDirectory = oldDir;

    histoFile->Close();
  }  // if( kTRUE == fbMonitorMode )
}

/**
  Copied from the CbmMcbm2018UnpackerTaskTof class without giving any thinking...
*/
void CbmMcbm2018UnpackerTaskRich2020::SetParContainers()
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

/**
  Copied from the CbmMcbm2018UnpackerTaskTof class without giving any thinking...
*/
Bool_t CbmMcbm2018UnpackerTaskRich2020::InitContainers()
{
  LOG(info) << "Init parameter containers for " << GetName();

  /// Control flags
  CbmMcbm2018RichPar* pUnpackPar =
    dynamic_cast<CbmMcbm2018RichPar*>(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmMcbm2018RichPar"));
  if (nullptr == pUnpackPar) {
    LOG(error) << "Failed to obtain parameter container CbmMcbm2018RichPar";
    return kFALSE;
  }  // if( nullptr == pUnpackPar )
     /*
   fbMonitorMode = pUnpackPar->GetMonitorMode();
   LOG(info) << "Monitor mode:       "
             << ( fbMonitorMode ? "ON" : "OFF" );

   fbDebugMonitorMode = pUnpackPar->GetDebugMonitorMode();
   LOG(info) << "Debug Monitor mode: "
             << ( fbDebugMonitorMode ? "ON" : "OFF" );
*/
  Bool_t initOK = fUnpackerAlgo->InitContainers();

  /// If monitor mode enabled, trigger histos creation, obtain pointer on them and add them to the HTTP server
  if (kTRUE == fbMonitorMode) {
    /// Trigger histo creation on all associated algos
    initOK &= fUnpackerAlgo->CreateHistograms();

    /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
    std::vector<std::pair<TNamed*, std::string>> vHistos = fUnpackerAlgo->GetHistoVector();
    /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
    std::vector<std::pair<TCanvas*, std::string>> vCanvases = fUnpackerAlgo->GetCanvasVector();

    /// Register the histos in the HTTP server
    THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
    if (nullptr != server) {
      for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
        server->Register(Form("/rich/%s", vHistos[uHisto].second.data()), vHistos[uHisto].first);
      }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )
      for (UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv) {
        server->Register(Form("/rich/%s", vCanvases[uCanv].second.data()),
                         gROOT->FindObject((vCanvases[uCanv].first)->GetName()));
      }  //  for( UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv )


      server->RegisterCommand("/Reset_UnpRich_Hist", "bMcbm2018UnpackerTaskRichResetHistos2020=kTRUE");
      server->Restrict("/Reset_UnpRich_Hist", "allow=admin");
    }  // if( nullptr != server )

  }  // if( kTRUE == fbMonitorMode )

  fUnpackerAlgo->SetMonitorMode(fbMonitorMode);

  return initOK;
}

/**
  Copied from the CbmMcbm2018UnpackerTaskTof class without giving any thinking...
*/
Bool_t CbmMcbm2018UnpackerTaskRich2020::ReInitContainers()
{
  LOG(info) << "ReInit parameter containers for " << GetName();
  Bool_t initOK = fUnpackerAlgo->ReInitContainers();

  return initOK;
}

/**
  Copied from other detectors without any brain effort...
**/
void CbmMcbm2018UnpackerTaskRich2020::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  fUnpackerAlgo->AddMsComponentToList(component, usDetectorId);
}

void CbmMcbm2018UnpackerTaskRich2020::SetNbMsInTs(size_t /*uCoreMsNb*/, size_t /*uOverlapMsNb*/) {}

void CbmMcbm2018UnpackerTaskRich2020::SetIgnoreOverlapMs(Bool_t bFlagIn) { fUnpackerAlgo->SetIgnoreOverlapMs(bFlagIn); }

void CbmMcbm2018UnpackerTaskRich2020::SetTimeOffsetNs(Double_t dOffsetIn) { fUnpackerAlgo->SetTimeOffsetNs(dOffsetIn); }

void CbmMcbm2018UnpackerTaskRich2020::DoTotCorr(Bool_t bDoToTCorr) { fUnpackerAlgo->DoTotCorr(bDoToTCorr); }

ClassImp(CbmMcbm2018UnpackerTaskRich2020)
