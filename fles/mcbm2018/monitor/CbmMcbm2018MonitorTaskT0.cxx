// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmMcbm2018MonitorTaskT0                      -----
// -----               Created 10.02.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018MonitorTaskT0.h"

#include "CbmMcbm2018MonitorAlgoT0.h"
#include "CbmMcbm2018TofPar.h"

#include "FairLogger.h"
#include "FairParGenericSet.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

#include "TCanvas.h"
#include "THttpServer.h"
#include "TList.h"
#include "TROOT.h"
#include "TString.h"
#include <TFile.h>

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdint.h>

Bool_t bMcbm2018MonitorTaskT0ResetHistos = kFALSE;

CbmMcbm2018MonitorTaskT0::CbmMcbm2018MonitorTaskT0()
  : CbmMcbmUnpack()
  , fbMonitorMode(kTRUE)
  , fbDebugMonitorMode(kFALSE)
  , fuHistoryHistoSize(3600)
  , fsHistoFileName("data/HistosMonitorT0.root")
  , fuMinTotPulser(90)
  , fuMaxTotPulser(100)
  , fuOffSpillCountLimit(200)
  , fulTsCounter(0)
  , fMonitorAlgo(nullptr) {
  fMonitorAlgo = new CbmMcbm2018MonitorAlgoT0();
}

CbmMcbm2018MonitorTaskT0::~CbmMcbm2018MonitorTaskT0() { delete fMonitorAlgo; }

Bool_t CbmMcbm2018MonitorTaskT0::Init() {
  LOG(info) << "CbmMcbm2018MonitorTaskT0::Init";
  LOG(info) << "Initializing mCBM T0 2019 Monitor";

  return kTRUE;
}

void CbmMcbm2018MonitorTaskT0::SetParContainers() {
  LOG(info) << "Setting parameter containers for " << GetName();

  TList* parCList = fMonitorAlgo->GetParList();

  for (Int_t iparC = 0; iparC < parCList->GetEntries(); ++iparC) {
    FairParGenericSet* tempObj = (FairParGenericSet*) (parCList->At(iparC));
    parCList->Remove(tempObj);

    std::string sParamName {tempObj->GetName()};
    FairParGenericSet* newObj = dynamic_cast<FairParGenericSet*>(
      FairRun::Instance()->GetRuntimeDb()->getContainer(sParamName.data()));

    if (nullptr == newObj) {
      LOG(error) << "Failed to obtain parameter container " << sParamName
                 << ", for parameter index " << iparC;
      return;
    }  // if( nullptr == newObj )

    parCList->AddAt(newObj, iparC);
    //      delete tempObj;
  }  // for( Int_t iparC = 0; iparC < parCList->GetEntries(); ++iparC )
}

Bool_t CbmMcbm2018MonitorTaskT0::InitContainers() {
  LOG(info) << "Init parameter containers for " << GetName();

  /// Control flags
  CbmMcbm2018TofPar* pUnpackPar = dynamic_cast<CbmMcbm2018TofPar*>(
    FairRun::Instance()->GetRuntimeDb()->getContainer("CbmMcbm2018TofPar"));
  if (nullptr == pUnpackPar) {
    LOG(error) << "Failed to obtain parameter container CbmMcbm2018TofPar";
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
  Bool_t initOK = fMonitorAlgo->InitContainers();

  /// Transfer parameter values set from calling macro
  fMonitorAlgo->SetMonitorMode(fbMonitorMode);
  fMonitorAlgo->SetHistoryHistoSize(fuHistoryHistoSize);
  fMonitorAlgo->SetPulserTotLimits(fuMinTotPulser, fuMaxTotPulser);
  fMonitorAlgo->SetSpillThreshold(fuOffSpillCountLimit);

  /// Histos creation, obtain pointer on them and add them to the HTTP server
  /// Trigger histo creation on all associated algos
  initOK &= fMonitorAlgo->CreateHistograms();

  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos =
    fMonitorAlgo->GetHistoVector();
  /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
  std::vector<std::pair<TCanvas*, std::string>> vCanvases =
    fMonitorAlgo->GetCanvasVector();

  /// Register the histos in the HTTP server
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
  if (nullptr != server) {
    for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
      //         LOG(info) << "Registering  " << vHistos[ uHisto ].first->GetName()
      //                   << " in " << vHistos[ uHisto ].second.data();
      server->Register(Form("/%s", vHistos[uHisto].second.data()),
                       vHistos[uHisto].first);
    }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

    for (UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv) {
      //         LOG(info) << "Registering  " << vCanvases[ uCanv ].first->GetName()
      //                   << " in " << vCanvases[ uCanv ].second.data();
      server->Register(Form("/%s", vCanvases[uCanv].second.data()),
                       gROOT->FindObject((vCanvases[uCanv].first)->GetName()));
    }  //  for( UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv )

    server->RegisterCommand("/Reset_MoniT0_Hist",
                            "bMcbm2018MonitorTaskT0ResetHistos=kTRUE");
    server->Restrict("/Reset_MoniT0_Hist", "allow=admin");
  }  // if( nullptr != server )

  return initOK;
}

Bool_t CbmMcbm2018MonitorTaskT0::ReInitContainers() {
  LOG(info) << "ReInit parameter containers for " << GetName();
  Bool_t initOK = fMonitorAlgo->ReInitContainers();

  return initOK;
}

void CbmMcbm2018MonitorTaskT0::AddMsComponentToList(size_t component,
                                                    UShort_t usDetectorId) {
  fMonitorAlgo->AddMsComponentToList(component, usDetectorId);
}

Bool_t CbmMcbm2018MonitorTaskT0::DoUnpack(const fles::Timeslice& ts,
                                          size_t /*component*/) {
  if (fbMonitorMode && bMcbm2018MonitorTaskT0ResetHistos) {
    LOG(info) << "Reset T0 Monitor histos ";
    fMonitorAlgo->ResetHistograms();
    bMcbm2018MonitorTaskT0ResetHistos = kFALSE;
  }  // if( fbMonitorMode && bMcbm2018MonitorTaskT0ResetHistos )

  if (kFALSE == fMonitorAlgo->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index()
               << " in unpacker algorithm class";
    return kTRUE;
  }  // if( kFALSE == fMonitorAlgo->ProcessTs( ts ) )

  /// Cleqr the digis vector in case it was filled
  std::vector<CbmTofDigi> vDigi = fMonitorAlgo->GetVector();
  fMonitorAlgo->ClearVector();

  if (0 == fulTsCounter % 10000)
    LOG(info) << "Processed " << fulTsCounter << "TS";
  fulTsCounter++;

  return kTRUE;
}

void CbmMcbm2018MonitorTaskT0::Reset() {}

void CbmMcbm2018MonitorTaskT0::Finish() {
  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos =
    fMonitorAlgo->GetHistoVector();

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* histoFile = nullptr;

  // open separate histo file in recreate mode
  histoFile = new TFile(fsHistoFileName, "RECREATE");
  histoFile->cd();

  /// Save the histograms in a file
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
}

void CbmMcbm2018MonitorTaskT0::SetIgnoreOverlapMs(Bool_t bFlagIn) {
  fMonitorAlgo->SetIgnoreOverlapMs(bFlagIn);
}

void CbmMcbm2018MonitorTaskT0::SetChannelMap(UInt_t uChan0,
                                             UInt_t uChan1,
                                             UInt_t uChan2,
                                             UInt_t uChan3,
                                             UInt_t uChan4,
                                             UInt_t uChan5,
                                             UInt_t uChan6,
                                             UInt_t uChan7) {
  fMonitorAlgo->SetChannelMap(
    uChan0, uChan1, uChan2, uChan3, uChan4, uChan5, uChan6, uChan7);
}

ClassImp(CbmMcbm2018MonitorTaskT0)
