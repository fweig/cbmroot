// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmMcbm2018UnpackerTaskSts                    -----
// -----               Created 26.01.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018UnpackerTaskSts.h"

#include "CbmMcbm2018StsPar.h"
#include "CbmMcbm2018UnpackerAlgoSts.h"

#include "FairLogger.h"
#include "FairParGenericSet.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

#include "TClonesArray.h"
#include "THttpServer.h"
#include "TROOT.h"
#include "TString.h"
#include <TFile.h>

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdint.h>

Bool_t bMcbm2018UnpackerTaskStsResetHistos = kFALSE;

CbmMcbm2018UnpackerTaskSts::CbmMcbm2018UnpackerTaskSts(UInt_t /*uNbGdpb*/)
  : CbmMcbmUnpack()
  , fbMonitorMode(kFALSE)
  , fbWriteOutput(kTRUE)
  , fvChanMasks()
  , fulTsCounter(0)
  , fUnpackerAlgo(nullptr) {
  fUnpackerAlgo = new CbmMcbm2018UnpackerAlgoSts();
}

CbmMcbm2018UnpackerTaskSts::~CbmMcbm2018UnpackerTaskSts() {
  delete fUnpackerAlgo;
}

Bool_t CbmMcbm2018UnpackerTaskSts::Init() {
  LOG(info) << "CbmMcbm2018UnpackerTaskSts::Init";
  LOG(info) << "Initializing mCBM STS 2018 Unpacker";

  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman) { LOG(fatal) << "No FairRootManager instance"; }

  /// WARNING: incompatible with the hodoscopes unpacker!!!!
  if (nullptr != ioman->InitObjectAs<std::vector<CbmStsDigi> const*>("StsDigi"))
    LOG(fatal)
      << "CbmMcbm2018UnpackerTaskSts::Init => output vector already registered,"
      << " probably by CbmMcbm2018UnpackerTaskHodo" << std::endl
      << " THESE TWO CLASSES ARE INCOMPATIBLE!";

  /// Get address of vector from algo
  fpvDigiSts = &(fUnpackerAlgo->GetVector());
  ioman->RegisterAny("StsDigi", fpvDigiSts, fbWriteOutput);

  /// Get address of error vector from algo
  fpvErrorSts = &(fUnpackerAlgo->GetErrorVector());
  ioman->RegisterAny("CbmStsError", fpvErrorSts, fbWriteOutput);

  return kTRUE;
}

void CbmMcbm2018UnpackerTaskSts::SetParContainers() {
  LOG(info) << "Setting parameter containers for " << GetName();

  TList* fParCList = fUnpackerAlgo->GetParList();

  for (Int_t iparC = 0; iparC < fParCList->GetEntries(); ++iparC) {
    FairParGenericSet* tempObj = (FairParGenericSet*) (fParCList->At(iparC));
    fParCList->Remove(tempObj);

    std::string sParamName {tempObj->GetName()};
    FairParGenericSet* newObj = dynamic_cast<FairParGenericSet*>(
      FairRun::Instance()->GetRuntimeDb()->getContainer(sParamName.data()));

    if (nullptr == newObj) {
      LOG(error) << "Failed to obtain parameter container " << sParamName
                 << ", for parameter index " << iparC;
      return;
    }  // if( nullptr == newObj )

    fParCList->AddAt(newObj, iparC);
    //      delete tempObj;
  }  // for( Int_t iparC = 0; iparC < fParCList->GetEntries(); ++iparC )
}

Bool_t CbmMcbm2018UnpackerTaskSts::InitContainers() {
  LOG(info) << "Init parameter containers for " << GetName();

  /// Control flags
  CbmMcbm2018StsPar* pUnpackPar = dynamic_cast<CbmMcbm2018StsPar*>(
    FairRun::Instance()->GetRuntimeDb()->getContainer("CbmMcbm2018StsPar"));
  if (nullptr == pUnpackPar) {
    LOG(error) << "Failed to obtain parameter container CbmMcbm2018StsPar";
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
    std::vector<std::pair<TNamed*, std::string>> vHistos =
      fUnpackerAlgo->GetHistoVector();

    /// Register the histos in the HTTP server
    THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
    if (nullptr != server) {
      for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
        server->Register(Form("/%s", vHistos[uHisto].second.data()),
                         vHistos[uHisto].first);
      }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

      server->RegisterCommand("/Reset_UnpSts_Hist",
                              "bMcbm2018UnpackerTaskStsResetHistos=kTRUE");
      server->Restrict("/Reset_UnpSts_Hist", "allow=admin");
    }  // if( nullptr != server )

  }  // if( kTRUE == fbMonitorMode )

  fUnpackerAlgo->SetMonitorMode(fbMonitorMode);
  for (std::vector<FebChanMask>::iterator it = fvChanMasks.begin();
       it < fvChanMasks.end();
       ++it)
    fUnpackerAlgo->MaskNoisyChannel((*it).uFeb, (*it).uChan, (*it).bMasked);

  return initOK;
}

Bool_t CbmMcbm2018UnpackerTaskSts::ReInitContainers() {
  LOG(info) << "ReInit parameter containers for " << GetName();
  Bool_t initOK = fUnpackerAlgo->ReInitContainers();

  return initOK;
}

void CbmMcbm2018UnpackerTaskSts::AddMsComponentToList(size_t component,
                                                      UShort_t usDetectorId) {
  fUnpackerAlgo->AddMsComponentToList(component, usDetectorId);
}

Bool_t CbmMcbm2018UnpackerTaskSts::DoUnpack(const fles::Timeslice& ts,
                                            size_t /*component*/) {
  if (fbMonitorMode && bMcbm2018UnpackerTaskStsResetHistos) {
    LOG(info) << "Reset STS unpacker histos ";
    fUnpackerAlgo->ResetHistograms();
    bMcbm2018UnpackerTaskStsResetHistos = kFALSE;
  }  // if( fbMonitorMode && bMcbm2018UnpackerTaskStsResetHistos )

  if (kFALSE == fUnpackerAlgo->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index()
               << " in unpacker algorithm class";
    return kTRUE;
  }  // if( kFALSE == fUnpackerAlgo->ProcessTs( ts ) )

  /*
   /// Sort the buffers of hits due to the time offsets applied
   => Done in the algo!!!
   sort(fpvDigiSts->begin(), fpvDigiSts->end(),
        [](const CbmStsDigi & a, const CbmStsDigi & b) -> bool
        {
          return a.GetTime() < b.GetTime();
        });
*/

  if (0 == fulTsCounter % 10000)
    LOG(info) << "Processed " << fulTsCounter << "TS";
  fulTsCounter++;

  return kTRUE;
}

void CbmMcbm2018UnpackerTaskSts::Reset() {
  fUnpackerAlgo->ClearVector();
  fUnpackerAlgo->ClearErrorVector();
}

void CbmMcbm2018UnpackerTaskSts::Finish() {
  /// If monitor mode enabled, trigger histos creation, obtain pointer on them and add them to the HTTP server
  if (kTRUE == fbMonitorMode) {
    /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
    std::vector<std::pair<TNamed*, std::string>> vHistos =
      fUnpackerAlgo->GetHistoVector();

    /// Save old global file and folder pointer to avoid messing with FairRoot
    TFile* oldFile     = gFile;
    TDirectory* oldDir = gDirectory;

    TFile* histoFile = nullptr;

    // open separate histo file in recreate mode
    histoFile = new TFile("data/HistosUnpackerSts.root", "RECREATE");
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

void CbmMcbm2018UnpackerTaskSts::SetIgnoreOverlapMs(Bool_t bFlagIn) {
  fUnpackerAlgo->SetIgnoreOverlapMs(bFlagIn);
}

void CbmMcbm2018UnpackerTaskSts::SetTimeOffsetNs(Double_t dOffsetIn) {
  fUnpackerAlgo->SetTimeOffsetNs(dOffsetIn);
}

void CbmMcbm2018UnpackerTaskSts::SetTimeOffsetNsAsic(UInt_t uAsicIdx,
                                                     Double_t dOffsetIn) {
  fUnpackerAlgo->SetTimeOffsetNsAsic(uAsicIdx, dOffsetIn);
}

void CbmMcbm2018UnpackerTaskSts::MaskNoisyChannel(UInt_t uFeb,
                                                  UInt_t uChan,
                                                  Bool_t bMasked) {
  fvChanMasks.push_back(FebChanMask {uFeb, uChan, bMasked});
}

void CbmMcbm2018UnpackerTaskSts::SetAdcCut(UInt_t uAdc) {
  fUnpackerAlgo->SetAdcCut(uAdc);
}

void CbmMcbm2018UnpackerTaskSts::SetBinningFwFlag(Bool_t bEnable) {
  fUnpackerAlgo->SetBinningFwFlag(bEnable);
}


ClassImp(CbmMcbm2018UnpackerTaskSts)
