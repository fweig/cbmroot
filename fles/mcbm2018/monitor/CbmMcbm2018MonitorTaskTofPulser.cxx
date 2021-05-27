// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmMcbm2018MonitorTaskTofPulser                  -----
// -----               Created 27.11.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018MonitorTaskTofPulser.h"

#include "CbmMcbm2018MonitorAlgoTofPulser.h"
#include "CbmMcbm2018TofPar.h"

#include "FairParGenericSet.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

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

Bool_t bMcbm2018MonitorTaskTofPulserResetHistos = kFALSE;
Bool_t bMcbm2018MonitorTaskTofPulserSaveHistos  = kFALSE;

CbmMcbm2018MonitorTaskTofPulser::CbmMcbm2018MonitorTaskTofPulser()
  : CbmMcbmUnpack()
  , fsHistoFileName("data/HistosMonitorPulser.root")
  , fuUpdateFreqTs(100)
  , fuPulserMinTot(90)
  , fuPulserMaxTot(110)
  , fuPulserChannel(3)
  , fiGdpbIndex(-1)
  , fuHistoryHistoSize(1800)
  , fulTsCounter(0)
  , fMonitorPulserAlgo(nullptr)
{
  fMonitorPulserAlgo = new CbmMcbm2018MonitorAlgoTofPulser();
}

CbmMcbm2018MonitorTaskTofPulser::~CbmMcbm2018MonitorTaskTofPulser() { delete fMonitorPulserAlgo; }

Bool_t CbmMcbm2018MonitorTaskTofPulser::Init()
{
  LOG(info) << "CbmMcbm2018MonitorTaskTofPulser::Init";
  LOG(info) << "Initializing mCBM 2019 Monitor task";

  return kTRUE;
}

void CbmMcbm2018MonitorTaskTofPulser::SetParContainers()
{
  LOG(info) << "Setting parameter containers for " << GetName();

  TList* parCList = fMonitorPulserAlgo->GetParList();

  for (Int_t iparC = 0; iparC < parCList->GetEntries(); ++iparC) {
    FairParGenericSet* tempObj = (FairParGenericSet*) (parCList->At(iparC));
    parCList->Remove(tempObj);

    std::string sParamName {tempObj->GetName()};
    FairParGenericSet* newObj =
      dynamic_cast<FairParGenericSet*>(FairRun::Instance()->GetRuntimeDb()->getContainer(sParamName.data()));

    if (nullptr == newObj) {
      LOG(error) << "Failed to obtain parameter container " << sParamName << ", for parameter index " << iparC;
      return;
    }  // if( nullptr == newObj )

    parCList->AddAt(newObj, iparC);
    //      delete tempObj;
  }  // for( Int_t iparC = 0; iparC < parCList->GetEntries(); ++iparC )
}

Bool_t CbmMcbm2018MonitorTaskTofPulser::InitContainers()
{
  LOG(info) << "Init parameter containers for " << GetName();

  /// Control flags
  CbmMcbm2018TofPar* pUnpackPar =
    dynamic_cast<CbmMcbm2018TofPar*>(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmMcbm2018TofPar"));
  if (nullptr == pUnpackPar) {
    LOG(error) << "Failed to obtain parameter container CbmMcbm2018TofPar";
    return kFALSE;
  }  // if( nullptr == pUnpackPar )

  /// Transfer parameter values set from calling macro
  fMonitorPulserAlgo->SetGdpbIndex(fiGdpbIndex);
  fMonitorPulserAlgo->SetUpdateFreqTs(fuUpdateFreqTs);
  fMonitorPulserAlgo->SetPulserTotLimits(fuPulserMinTot, fuPulserMaxTot);
  fMonitorPulserAlgo->SetPulserChannel(fuPulserChannel);

  Bool_t initOK = fMonitorPulserAlgo->InitContainers();

  /// Histos creation, obtain pointer on them and add them to the HTTP server
  /// Trigger histo creation on all associated algos
  initOK &= fMonitorPulserAlgo->CreateHistograms();

  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos = fMonitorPulserAlgo->GetHistoVector();
  /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
  std::vector<std::pair<TCanvas*, std::string>> vCanvases = fMonitorPulserAlgo->GetCanvasVector();

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

    server->RegisterCommand("/Reset_Pulser_Hist", "bMcbm2018MonitorTaskTofPulserResetHistos=kTRUE");
    server->RegisterCommand("/Save_Pulser_Hist", "bMcbm2018MonitorTaskTofPulserSaveHistos=kTRUE");

    server->Restrict("/Reset_Moni_Hist", "allow=admin");
  }  // if( nullptr != server )

  return initOK;
}

Bool_t CbmMcbm2018MonitorTaskTofPulser::ReInitContainers()
{
  LOG(info) << "ReInit parameter containers for " << GetName();
  Bool_t initOK = fMonitorPulserAlgo->ReInitContainers();

  return initOK;
}

void CbmMcbm2018MonitorTaskTofPulser::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  fMonitorPulserAlgo->AddMsComponentToList(component, usDetectorId);
}

Bool_t CbmMcbm2018MonitorTaskTofPulser::DoUnpack(const fles::Timeslice& ts, size_t /*component*/)
{
  if (bMcbm2018MonitorTaskTofPulserResetHistos) {
    LOG(info) << "Reset Monitor histos ";
    fMonitorPulserAlgo->ResetHistograms();
    bMcbm2018MonitorTaskTofPulserResetHistos = kFALSE;
  }  // if( bMcbm2018MonitorTaskTofPulserResetHistos )

  if (bMcbm2018MonitorTaskTofPulserSaveHistos) {
    LOG(info) << "Save Monitor histos & canvases";
    SaveHistograms();
    bMcbm2018MonitorTaskTofPulserSaveHistos = kFALSE;
  }  // if( bMcbm2018MonitorTaskTofPulserSaveHistos )

  if (kFALSE == fMonitorPulserAlgo->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index() << " in unpacker algorithm class";
    return kTRUE;
  }  // if( kFALSE == fMonitorPulserAlgo->ProcessTs( ts ) )

  /// Cleqr the digis vector in case it was filled
  std::vector<CbmTofDigi> vDigi = fMonitorPulserAlgo->GetVector();
  fMonitorPulserAlgo->ClearVector();

  if (0 == fulTsCounter % 10000) LOG(info) << "Processed " << fulTsCounter << "TS";
  fulTsCounter++;

  return kTRUE;
}

void CbmMcbm2018MonitorTaskTofPulser::Reset() {}

void CbmMcbm2018MonitorTaskTofPulser::Finish()
{
  fMonitorPulserAlgo->UpdateStats();
  SaveHistograms();
}

void CbmMcbm2018MonitorTaskTofPulser::SetIgnoreOverlapMs(Bool_t bFlagIn)
{
  fMonitorPulserAlgo->SetIgnoreOverlapMs(bFlagIn);
}

Bool_t CbmMcbm2018MonitorTaskTofPulser::SaveHistograms()
{
  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos  = fMonitorPulserAlgo->GetHistoVector();
  std::vector<std::pair<TCanvas*, std::string>> vCanvas = fMonitorPulserAlgo->GetCanvasVector();

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* histoFile = nullptr;

  // open separate histo file in recreate mode
  histoFile = new TFile(fsHistoFileName, "RECREATE");

  if (nullptr == histoFile) return kFALSE;

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

  for (UInt_t uCanvas = 0; uCanvas < vCanvas.size(); ++uCanvas) {
    /// Make sure we end up in chosen folder
    TString sFolder = vCanvas[uCanvas].second.data();
    if (nullptr == gDirectory->Get(sFolder)) gDirectory->mkdir(sFolder);
    gDirectory->cd(sFolder);

    /// Write plot
    vCanvas[uCanvas].first->Write();

    histoFile->cd();
  }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  histoFile->Close();

  return kTRUE;
}
ClassImp(CbmMcbm2018MonitorTaskTofPulser)
