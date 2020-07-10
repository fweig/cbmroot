// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmMcbm2018UnpackerTaskHodo                     -----
// -----              Created 31/07/19  by P.-A. Loizeau                   -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018UnpackerTaskHodo.h"

#include "CbmMcbm2018HodoPar.h"
#include "CbmMcbm2018UnpackerAlgoHodo.h"

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

Bool_t bMcbm2018UnpackerTaskHodoResetHistos = kFALSE;

CbmMcbm2018UnpackerTaskHodo::CbmMcbm2018UnpackerTaskHodo(UInt_t /*uNbGdpb*/)
  : CbmMcbmUnpack()
  , fbMonitorMode(kFALSE)
  , fbWriteOutput(kTRUE)
  , fvChanMasks()
  , fulTsCounter(0)
  , fUnpackerAlgo(nullptr) {
  fUnpackerAlgo    = new CbmMcbm2018UnpackerAlgoHodo();
  fUnpackerAlgoSts = new CbmMcbm2018UnpackerAlgoSts();
}

CbmMcbm2018UnpackerTaskHodo::~CbmMcbm2018UnpackerTaskHodo() {
  delete fUnpackerAlgo;
  delete fUnpackerAlgoSts;
}

Bool_t CbmMcbm2018UnpackerTaskHodo::Init() {
  LOG(info) << "CbmMcbm2018UnpackerTaskHodo::Init";
  LOG(info) << "Initializing mCBM STS 2018 Unpacker";

  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman) { LOG(fatal) << "No FairRootManager instance"; }
  /// WARNING: incompatible with the hodoscopes unpacker!!!!
  if (nullptr != ioman->InitObjectAs<std::vector<CbmStsDigi> const*>("StsDigi"))
    LOG(fatal) << "CbmMcbm2018UnpackerTaskHodo::Init => output vector already "
                  "registered,"
               << " probably by CbmMcbm2018UnpackerTaskSts" << std::endl
               << " THESE TWO CLASSES ARE INCOMPATIBLE!";

  /// Create output vector and register it
  fpvDigiSts = new std::vector<CbmStsDigi>();
  ioman->RegisterAny("StsDigi", fpvDigiSts, fbWriteOutput);

  /// create output error vector and register it
  fpvErrorSts = new std::vector<CbmErrorMessage>();
  ioman->RegisterAny("CbmStsError", fpvErrorSts, fbWriteOutput);

  return kTRUE;
}

void CbmMcbm2018UnpackerTaskHodo::SetParContainers() {
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

  fParCList = fUnpackerAlgoSts->GetParList();

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

Bool_t CbmMcbm2018UnpackerTaskHodo::InitContainers() {
  LOG(info) << "Init parameter containers for " << GetName();

  /// Control flags
  CbmMcbm2018HodoPar* pUnpackPar = dynamic_cast<CbmMcbm2018HodoPar*>(
    FairRun::Instance()->GetRuntimeDb()->getContainer("CbmMcbm2018HodoPar"));
  if (nullptr == pUnpackPar) {
    LOG(error) << "Failed to obtain parameter container CbmMcbm2018HodoPar";
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
  initOK &= fUnpackerAlgoSts->InitContainers();

  /// If monitor mode enabled, trigger histos creation, obtain pointer on them and add them to the HTTP server
  if (kTRUE == fbMonitorMode) {
    /// Trigger histo creation on all associated algos
    initOK &= fUnpackerAlgo->CreateHistograms();
    initOK &= fUnpackerAlgoSts->CreateHistograms();

    /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
    std::vector<std::pair<TNamed*, std::string>> vHistos =
      fUnpackerAlgo->GetHistoVector();
    std::vector<std::pair<TNamed*, std::string>> vHistosSts =
      fUnpackerAlgoSts->GetHistoVector();
    vHistos.insert(vHistos.end(), vHistosSts.begin(), vHistosSts.end());

    /// Register the histos in the HTTP server
    THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
    if (nullptr != server) {
      for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
        server->Register(Form("/%s", vHistos[uHisto].second.data()),
                         vHistos[uHisto].first);
      }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

      server->RegisterCommand("/Reset_UnpHodo_Hist",
                              "bMcbm2018UnpackerTaskHodoResetHistos=kTRUE");
      server->Restrict("/Reset_UnpHodo_Hist", "allow=admin");
    }  // if( nullptr != server )

  }  // if( kTRUE == fbMonitorMode )

  fUnpackerAlgo->SetMonitorMode(fbMonitorMode);

  fUnpackerAlgoSts->SetMonitorMode(fbMonitorMode);
  for (std::vector<FebChanMaskSts>::iterator it = fvChanMasks.begin();
       it < fvChanMasks.end();
       ++it)
    fUnpackerAlgoSts->MaskNoisyChannel((*it).uFeb, (*it).uChan, (*it).bMasked);

  return initOK;
}

Bool_t CbmMcbm2018UnpackerTaskHodo::ReInitContainers() {
  LOG(info) << "ReInit parameter containers for " << GetName();
  Bool_t initOK = fUnpackerAlgo->ReInitContainers();
  initOK &= fUnpackerAlgoSts->ReInitContainers();

  return initOK;
}

void CbmMcbm2018UnpackerTaskHodo::AddMsComponentToList(size_t component,
                                                       UShort_t usDetectorId) {
  fUnpackerAlgo->AddMsComponentToList(component, usDetectorId);
  fUnpackerAlgoSts->AddMsComponentToList(component, usDetectorId);
}

Bool_t CbmMcbm2018UnpackerTaskHodo::DoUnpack(const fles::Timeslice& ts,
                                             size_t /*component*/) {
  if (fbMonitorMode && bMcbm2018UnpackerTaskHodoResetHistos) {
    LOG(info) << "Reset Hodo + STS unpacker histos ";
    fUnpackerAlgo->ResetHistograms();
    fUnpackerAlgoSts->ResetHistograms();
    bMcbm2018UnpackerTaskHodoResetHistos = kFALSE;
  }  // if( fbMonitorMode && bMcbm2018UnpackerTaskHodoResetHistos )

  if (kFALSE == fUnpackerAlgo->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index()
               << " in HODO unpacker algorithm class";
    return kTRUE;
  }  // if( kFALSE == fUnpackerAlgo->ProcessTs( ts ) )

  if (kFALSE == fUnpackerAlgoSts->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index()
               << " in STS unpacker algorithm class";
    return kTRUE;
  }  // if( kFALSE == fUnpackerAlgoSts->ProcessTs( ts ) )

  /// Copy the digis in the output Buffer
  fpvDigiSts->insert(fpvDigiSts->end(),
                     fUnpackerAlgo->GetVector().begin(),
                     fUnpackerAlgo->GetVector().end());
  fpvDigiSts->insert(fpvDigiSts->end(),
                     fUnpackerAlgoSts->GetVector().begin(),
                     fUnpackerAlgoSts->GetVector().end());

  /// Sort the buffers of hits due to the time offsets applied
  sort(fpvDigiSts->begin(),
       fpvDigiSts->end(),
       [](const CbmStsDigi& a, const CbmStsDigi& b) -> bool {
         return a.GetTime() < b.GetTime();
       });

  fUnpackerAlgo->ClearVector();
  fUnpackerAlgoSts->ClearVector();

  /// Copy the errors in the output Buffer
  /// Cannot be done in algo as merging outputs of 2 different ones!
  /// => Less efficient than STS only due to double sorting
  fpvErrorSts->insert(fpvErrorSts->end(),
                      fUnpackerAlgo->GetErrorVector().begin(),
                      fUnpackerAlgo->GetErrorVector().end());
  fpvErrorSts->insert(fpvErrorSts->end(),
                      fUnpackerAlgoSts->GetErrorVector().begin(),
                      fUnpackerAlgoSts->GetErrorVector().end());

  fUnpackerAlgo->ClearErrorVector();
  fUnpackerAlgoSts->ClearErrorVector();

  if (0 == fulTsCounter % 10000)
    LOG(info) << "Processed " << fulTsCounter << "TS";
  fulTsCounter++;

  return kTRUE;
}

void CbmMcbm2018UnpackerTaskHodo::Reset() {
  fpvDigiSts->clear();
  fpvErrorSts->clear();
}

void CbmMcbm2018UnpackerTaskHodo::Finish() {
  /// If monitor mode enabled, trigger histos creation, obtain pointer on them and add them to the HTTP server
  if (kTRUE == fbMonitorMode) {
    /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
    std::vector<std::pair<TNamed*, std::string>> vHistos =
      fUnpackerAlgo->GetHistoVector();
    std::vector<std::pair<TNamed*, std::string>> vHistosSts =
      fUnpackerAlgoSts->GetHistoVector();
    vHistos.insert(vHistos.end(), vHistosSts.begin(), vHistosSts.end());

    /// (Re-)Create ROOT file to store the histos
    TDirectory* oldDir = NULL;
    TFile* histoFile   = NULL;
    // Store current directory position to allow restore later
    oldDir = gDirectory;
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

    // Restore original directory position
    oldDir->cd();
    histoFile->Close();
  }  // if( kTRUE == fbMonitorMode )
}

void CbmMcbm2018UnpackerTaskHodo::SetIgnoreOverlapMs(Bool_t bFlagIn) {
  fUnpackerAlgo->SetIgnoreOverlapMs(bFlagIn);
  fUnpackerAlgoSts->SetIgnoreOverlapMs(bFlagIn);
}

void CbmMcbm2018UnpackerTaskHodo::SetTimeOffsetNs(Double_t dOffsetIn) {
  fUnpackerAlgo->SetTimeOffsetNs(dOffsetIn);
}

void CbmMcbm2018UnpackerTaskHodo::SetTimeOffsetNsSts(Double_t dOffsetIn) {
  fUnpackerAlgoSts->SetTimeOffsetNs(dOffsetIn);
}

void CbmMcbm2018UnpackerTaskHodo::SetTimeOffsetNsAsic(UInt_t uAsicIdx,
                                                      Double_t dOffsetIn) {
  fUnpackerAlgo->SetTimeOffsetNsAsic(uAsicIdx, dOffsetIn);
}

void CbmMcbm2018UnpackerTaskHodo::SetTimeOffsetNsAsicSts(UInt_t uAsicIdx,
                                                         Double_t dOffsetIn) {
  fUnpackerAlgoSts->SetTimeOffsetNsAsic(uAsicIdx, dOffsetIn);
}

void CbmMcbm2018UnpackerTaskHodo::MaskNoisyChannelSts(UInt_t uFeb,
                                                      UInt_t uChan,
                                                      Bool_t bMasked) {
  fvChanMasks.push_back(FebChanMaskSts {uFeb, uChan, bMasked});
}

void CbmMcbm2018UnpackerTaskHodo::SetAdcCutSts(UInt_t uAdc) {
  fUnpackerAlgoSts->SetAdcCut(uAdc);
}

ClassImp(CbmMcbm2018UnpackerTaskHodo)
