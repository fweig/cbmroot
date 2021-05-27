// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmStar2019MonitorTask                        -----
// -----               Created 12.10.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmStar2019MonitorTask.h"

#include "CbmStar2019MonitorAlgo.h"
#include "CbmStar2019TofPar.h"

#include "FairParGenericSet.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

#include "TCanvas.h"
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

Bool_t bStar2019MonitorTaskResetHistos = kFALSE;
Bool_t bStar2019MonitorTaskSaveHistos  = kFALSE;

CbmStar2019MonitorTask::CbmStar2019MonitorTask()
  : CbmMcbmUnpack()
  , fbDebugMonitorMode(kFALSE)
  , fbIgnoreCriticalErrors(kFALSE)
  , fuHistoryHistoSize(3600)
  , fsHistoFileName("data/HistosMonitor.root")
  , fuMinTotPulser(90)
  , fuMaxTotPulser(100)
  , fiSectorIndex(-1)
  , fParCList(nullptr)
  , fulTsCounter(0)
  , fMonitorAlgo(nullptr)
{
  fMonitorAlgo = new CbmStar2019MonitorAlgo();
}

CbmStar2019MonitorTask::~CbmStar2019MonitorTask() { delete fMonitorAlgo; }

Bool_t CbmStar2019MonitorTask::Init()
{
  LOG(info) << "CbmStar2019MonitorTask::Init";
  LOG(info) << "Initializing eTOF 2019 Monitor task";

  return kTRUE;
}

void CbmStar2019MonitorTask::SetParContainers()
{
  LOG(info) << "Setting parameter containers for " << GetName();

  fParCList = fMonitorAlgo->GetParList();

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

Bool_t CbmStar2019MonitorTask::InitContainers()
{
  LOG(info) << "Init parameter containers for " << GetName();

  /// Control flags
  CbmStar2019TofPar* pUnpackPar =
    dynamic_cast<CbmStar2019TofPar*>(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmStar2019TofPar"));
  if (nullptr == pUnpackPar) {
    LOG(error) << "Failed to obtain parameter container CbmStar2019TofPar";
    return kFALSE;
  }  // if( nullptr == pUnpackPar )

  /// Transfer parameter values set from calling macro
  fMonitorAlgo->SetDebugMonitorMode(fbDebugMonitorMode);
  fMonitorAlgo->SetIgnoreCriticalErrors(fbIgnoreCriticalErrors);
  fMonitorAlgo->SetHistoryHistoSize(fuHistoryHistoSize);
  fMonitorAlgo->SetPulserTotLimits(fuMinTotPulser, fuMaxTotPulser);
  fMonitorAlgo->SetSectorIndex(fiSectorIndex);

  Bool_t initOK = fMonitorAlgo->InitContainers();

  /// Histos creation, obtain pointer on them and add them to the HTTP server
  /// Trigger histo creation on all associated algos
  initOK &= fMonitorAlgo->CreateHistograms();

  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos = fMonitorAlgo->GetHistoVector();
  /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
  std::vector<std::pair<TCanvas*, std::string>> vCanvases = fMonitorAlgo->GetCanvasVector();

  /// Register the histos in the HTTP server
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
  if (nullptr != server) {
    for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
      //         LOG(info) << "Registering  " << vHistos[ uHisto ].first->GetName()
      //                   << " in " << vHistos[ uHisto ].second.data();
      server->Register(Form("/%s", vHistos[uHisto].second.data()), vHistos[uHisto].first);
    }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

    for (UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv) {
      //         LOG(info) << "Registering  " << vCanvases[ uCanv ].first->GetName()
      //                   << " in " << vCanvases[ uCanv ].second.data();
      server->Register(Form("/%s", vCanvases[uCanv].second.data()),
                       gROOT->FindObject((vCanvases[uCanv].first)->GetName()));
    }  //  for( UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv )

    server->RegisterCommand("/Reset_Moni_Hist", "bStar2019MonitorTaskResetHistos=kTRUE");
    server->RegisterCommand("/Save_Moni_Hist", "bStar2019MonitorTaskSaveHistos=kTRUE");

    server->Restrict("/Reset_Moni_Hist", "allow=admin");
  }  // if( nullptr != server )

  return initOK;
}

Bool_t CbmStar2019MonitorTask::ReInitContainers()
{
  LOG(info) << "ReInit parameter containers for " << GetName();
  Bool_t initOK = fMonitorAlgo->ReInitContainers();

  return initOK;
}

void CbmStar2019MonitorTask::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  fMonitorAlgo->AddMsComponentToList(component, usDetectorId);
}

Bool_t CbmStar2019MonitorTask::DoUnpack(const fles::Timeslice& ts, size_t /*component*/)
{
  if (bStar2019MonitorTaskResetHistos) {
    LOG(info) << "Reset Monitor histos ";
    fMonitorAlgo->ResetHistograms();
    bStar2019MonitorTaskResetHistos = kFALSE;
  }  // if( bStar2019MonitorTaskResetHistos )

  if (bStar2019MonitorTaskSaveHistos) {
    LOG(info) << "Save Monitor histos & canvases";
    SaveHistograms();
    bStar2019MonitorTaskSaveHistos = kFALSE;
  }  // if( bStar2019MonitorTaskSaveHistos )

  if (kFALSE == fMonitorAlgo->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index() << " in unpacker algorithm class";
    return kTRUE;
  }  // if( kFALSE == fMonitorAlgo->ProcessTs( ts ) )

  /// Cleqr the digis vector in case it was filled
  std::vector<CbmTofDigi> vDigi = fMonitorAlgo->GetVector();
  fMonitorAlgo->ClearVector();

  if (0 == fulTsCounter % 10000) LOG(info) << "Processed " << fulTsCounter << "TS";
  fulTsCounter++;

  return kTRUE;
}

void CbmStar2019MonitorTask::Reset() {}

void CbmStar2019MonitorTask::Finish() { SaveHistograms(); }

void CbmStar2019MonitorTask::SetIgnoreOverlapMs(Bool_t bFlagIn) { fMonitorAlgo->SetIgnoreOverlapMs(bFlagIn); }

Bool_t CbmStar2019MonitorTask::SaveLatencyHistograms(TString sFilename)
{
  return fMonitorAlgo->SaveLatencyHistograms(sFilename);
}

Bool_t CbmStar2019MonitorTask::SaveHistograms()
{
  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos  = fMonitorAlgo->GetHistoVector();
  std::vector<std::pair<TCanvas*, std::string>> vCanvas = fMonitorAlgo->GetCanvasVector();

  /// (Re-)Create ROOT file to store the histos
  TDirectory* oldDir = NULL;
  TFile* histoFile   = NULL;
  // Store current directory position to allow restore later
  oldDir = gDirectory;
  // open separate histo file in recreate mode
  histoFile = new TFile(fsHistoFileName, "RECREATE");

  if (nullptr == histoFile) return kFALSE;

  LOG(info) << "Saving " << vHistos.size() << " monitor histograms ";

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

  if (0)  // attempt to speed up, nh, 20200125
    for (UInt_t uCanvas = 0; uCanvas < vCanvas.size(); ++uCanvas) {
      /// Make sure we end up in chosen folder
      TString sFolder = vCanvas[uCanvas].second.data();
      if (nullptr == gDirectory->Get(sFolder)) gDirectory->mkdir(sFolder);
      gDirectory->cd(sFolder);

      /// Write plot
      vCanvas[uCanvas].first->Write();

      histoFile->cd();
    }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

  // Restore original directory position
  oldDir->cd();
  histoFile->Close();

  return kTRUE;
}
ClassImp(CbmStar2019MonitorTask)
