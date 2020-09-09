// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmStar2019EventBuilderEtof                   -----
// -----               Created 14.11.2018 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmStar2019EventBuilderEtof.h"

#include "CbmStar2019EventBuilderEtofAlgo.h"
#include "CbmStar2019TofPar.h"

#include <Logger.h>
#include "FairParGenericSet.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

#include "TCanvas.h"
#include "TH2.h"
#include "THttpServer.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TString.h"
#include <TFile.h>

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdint.h>

Bool_t bStarEtof2019EventBuilderResetHistos = kFALSE;

CbmStar2019EventBuilderEtof::CbmStar2019EventBuilderEtof(UInt_t /*uNbGdpb*/)
  : CbmMcbmUnpack()
  , fbMonitorMode(kFALSE)
  , fbDebugMonitorMode(kFALSE)
  , fbStoreLostEventMsg(kFALSE)
  , fbAddStatusToEvent(kTRUE)
  , fbSandboxMode(kFALSE)
  , fbEventDumpEna(kFALSE)
  , fParCList(nullptr)
  , fuMinTotPulser(90)
  , fuMaxTotPulser(110)
  , fsHistoFileName("data/eventBuilderMonHist.root")
  , fulTsCounter(0)
  , fEventBuilderAlgo(nullptr)
  , fdMsSizeInNs(-1.0)
  , fdTsCoreSizeInSec(-1.0)
  , fTimer()
  , fdRealTime(0.0)
  , fdRealTimeMin(1e6)
  , fdRealTimeMax(0.0)
  , fdRealTimeAll(0.0)
  , fdRealTimeMinAll(1e6)
  , fdRealTimeMaxAll(0.0)
  , fulNbEvents(0)
  , fulNbEventsSinceLastPrintout(0)
  , fhRealTimeDistr(nullptr)
  , fhRealTimeEvo(nullptr)
  , fhMeanRealTimeEvo(nullptr)
  , fpBinDumpFile(nullptr) {
  fEventBuilderAlgo = new CbmStar2019EventBuilderEtofAlgo();
}

CbmStar2019EventBuilderEtof::~CbmStar2019EventBuilderEtof() {
  delete fEventBuilderAlgo;
}

Bool_t CbmStar2019EventBuilderEtof::Init() {
  LOG(info) << "CbmStar2019EventBuilderEtof::Init";
  LOG(info) << "Initializing STAR eTOF 2018 Event Builder";

  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman) { LOG(fatal) << "No FairRootManager instance"; }

  return kTRUE;
}

void CbmStar2019EventBuilderEtof::SetParContainers() {
  LOG(info) << "Setting parameter containers for " << GetName();

  fParCList = fEventBuilderAlgo->GetParList();

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

Bool_t CbmStar2019EventBuilderEtof::InitContainers() {
  LOG(info) << "Init parameter containers for " << GetName();

  /// Control flags
  CbmStar2019TofPar* pUnpackPar = dynamic_cast<CbmStar2019TofPar*>(
    FairRun::Instance()->GetRuntimeDb()->getContainer("CbmStar2019TofPar"));
  if (nullptr == pUnpackPar) {
    LOG(error) << "Failed to obtain parameter container CbmStar2019TofPar";
    return kFALSE;
  }  // if( nullptr == pUnpackPar )

  //   fbMonitorMode = pUnpackPar->GetMonitorMode();
  LOG(info) << "Monitor mode:       " << (fbMonitorMode ? "ON" : "OFF");

  //   fbDebugMonitorMode = pUnpackPar->GetDebugMonitorMode();
  LOG(info) << "Debug Monitor mode: " << (fbDebugMonitorMode ? "ON" : "OFF");

  /// Parameters for FLES containers processing
  fdMsSizeInNs = pUnpackPar->GetSizeMsInNs();
  LOG(info) << "Timeslice parameters: each MS is " << fdMsSizeInNs << " ns";

  fEventBuilderAlgo->SetMonitorMode(fbMonitorMode);
  fEventBuilderAlgo->SetDebugMonitorMode(fbDebugMonitorMode);
  fEventBuilderAlgo->SetStoreLostEventMsg(fbStoreLostEventMsg);
  fEventBuilderAlgo->SetAddStatusToEvent(fbAddStatusToEvent);
  fEventBuilderAlgo->SetPulserTotLimits(fuMinTotPulser, fuMaxTotPulser);

  Bool_t initOK = fEventBuilderAlgo->InitContainers();

  /// If monitor mode enabled, trigger histos creation, obtain pointer on them and add them to the HTTP server
  if (kTRUE == fbMonitorMode) {
    /// Trigger histo creation on all associated algos
    initOK &= fEventBuilderAlgo->CreateHistograms();

    /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
    std::vector<std::pair<TNamed*, std::string>> vHistos =
      fEventBuilderAlgo->GetHistoVector();
    /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
    std::vector<std::pair<TCanvas*, std::string>> vCanvases =
      fEventBuilderAlgo->GetCanvasVector();

    /// Register the histos in the HTTP server
    THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
    for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
      server->Register(Form("/%s", vHistos[uHisto].second.data()),
                       vHistos[uHisto].first);
    }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

    for (UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv) {
      //         LOG(info) << "Registering  " << vCanvases[ uCanv ].first->GetName()
      //                   << " in " << vCanvases[ uCanv ].second.data();
      server->Register(Form("/%s", vCanvases[uCanv].second.data()),
                       gROOT->FindObject((vCanvases[uCanv].first)->GetName()));
    }  //  for( UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv )

    server->RegisterCommand("/Reset_EvtBuild_Hist",
                            "bStarEtof2019EventBuilderResetHistos=kTRUE");
    server->Restrict("/Reset_EvtBuild_Hist", "allow=admin");

    if (fbDebugMonitorMode) {
      fhRealTimeDistr = new TH1I("hEvtBuildRealTimeDistr",
                                 "Realtime for processing a TS in event "
                                 "builder; Realtime [ms]; TS nb []",
                                 100000,
                                 0.0,
                                 100.0);
      fhRealTimeEvo   = new TH2I(
        "hEvtBuildRealTimeEvo",
        "Realtime Processing to duration ratio for processing a TS in event "
        "builder vs TS index; TS []; Realtime ratio []; TS Nb []",
        1000,
        0,
        100000,
        10000,
        0.0,
        100.0);

      fhMeanRealTimeEvo = new TProfile(
        "hEvtBuildMeanRealTimeEvo",
        "Mean Realtime Processing to duration ratio for processing a TS in "
        "event builder vs TS index; TS []; Mean Realtime ratio []; TS Nb []",
        1000,
        0,
        100000);

      server->Register("/EvtBuildTime", fhRealTimeDistr);
      server->Register("/EvtBuildTime", fhRealTimeEvo);
      server->Register("/EvtBuildTime", fhMeanRealTimeEvo);
    }  // if( fbDebugMonitorMode )

  }  // if( kTRUE == fbMonitorMode )

  return initOK;
}

Bool_t CbmStar2019EventBuilderEtof::ReInitContainers() {
  LOG(info) << "ReInit parameter containers for " << GetName();
  Bool_t initOK = fEventBuilderAlgo->ReInitContainers();

  return initOK;
}

void CbmStar2019EventBuilderEtof::SetEventDumpEnable(Bool_t bDumpEna) {
  if (fbEventDumpEna != bDumpEna) {
    if (bDumpEna) {
      LOG(info) << "Enabling event dump to binary file which was disabled. "
                   "File will be opened.";

      std::time_t cTimeCurrent =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      char tempBuff[80];
      std::strftime(
        tempBuff, 80, "%Y_%m_%d_%H_%M_%S", localtime(&cTimeCurrent));
      TString sFileName = Form("event_dump_%s.bin", tempBuff);
      fpBinDumpFile =
        new std::fstream(sFileName, std::ios::out | std::ios::binary);

      if (NULL == fpBinDumpFile) {
        LOG(fatal) << "Failed to open new binary file for event dump at "
                   << sFileName;
      }  // if( NULL == fpBinDumpFile )
      else
        LOG(info) << "Opened binary dump file at " << sFileName;
    }  // if( bDumpEna )
    else {
      LOG(info) << "Disabling event dump to binary file which was enabled. "
                   "File will be closed.";

      if (NULL != fpBinDumpFile) fpBinDumpFile->close();
    }  // else of if( bDumpEna )
  }    // if( fbEventDumpEna != bDumpEna )

  fbEventDumpEna = bDumpEna;
  if (fbEventDumpEna)
    LOG(info) << "Event dump to binary file is now ENABLED";
  else
    LOG(info) << "Event dump to binary file is now DISABLED";
}

void CbmStar2019EventBuilderEtof::AddMsComponentToList(size_t component,
                                                       UShort_t usDetectorId) {
  fEventBuilderAlgo->AddMsComponentToList(component, usDetectorId);
}

Bool_t CbmStar2019EventBuilderEtof::DoUnpack(const fles::Timeslice& ts,
                                             size_t /*component*/) {
  fTimer.Start();

  /// On first TS, extract the TS parameters from header (by definition stable over time)
  if (-1.0 == fdTsCoreSizeInSec) {
    fdTsCoreSizeInSec = fdMsSizeInNs * ts.num_core_microslices() / 1e9;
  }  // if( -1.0 == fdTsCoreSizeInSec )

  if (0 == fulTsCounter) {
    LOG(info) << "FIXME ===> Jumping 1st TS as corrupted with current FW + "
                 "FLESNET combination";
    fulTsCounter++;
    return kTRUE;
  }  // if( 0 == fulTsCounter )

  if (fbMonitorMode && bStarEtof2019EventBuilderResetHistos) {
    LOG(info) << "Reset eTOF STAR histos ";
    fEventBuilderAlgo->ResetHistograms();
    bStarEtof2019EventBuilderResetHistos = kFALSE;

    if (fbDebugMonitorMode) {
      fhRealTimeDistr->Reset();
      fhRealTimeEvo->Reset();
      fhMeanRealTimeEvo->Reset();
    }  // if( fbDebugMonitorMode )
  }    // if( fbMonitorMode && bStarEtof2019EventBuilderResetHistos )

  if (kFALSE == fEventBuilderAlgo->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index()
               << " in event builder algorithm class";
    return kTRUE;
  }  // if( kFALSE == fEventBuilderAlgo->ProcessTs( ts ) )

  std::vector<CbmTofStarSubevent2019>& eventBuffer =
    fEventBuilderAlgo->GetEventBuffer();

  for (UInt_t uEvent = 0; uEvent < eventBuffer.size(); ++uEvent) {
    /// Send the sub-event to the STAR systems
    Int_t iBuffSzByte = 0;
    void* pDataBuff   = eventBuffer[uEvent].BuildOutput(iBuffSzByte);
    if (NULL != pDataBuff) {
      /// Valid output, do stuff with it!
      //         Bool_t fbSendEventToStar = kFALSE;
      if (kFALSE == fbSandboxMode) {
        /*
             ** Function to send sub-event block to the STAR DAQ system
             *       trg_word received is packed as:
             *
             *       trg_cmd|daq_cmd|tkn_hi|tkn_mid|tkn_lo
             */
        star_rhicf_write(eventBuffer[uEvent].GetTrigger().GetStarTrigerWord(),
                         pDataBuff,
                         iBuffSzByte);
      }  // if( kFALSE == fbSandboxMode )

      LOG(debug) << "Sent STAR event with size " << iBuffSzByte << " Bytes"
                 << " and token "
                 << eventBuffer[uEvent].GetTrigger().GetStarToken();

      if (kTRUE == fbEventDumpEna) {
        fpBinDumpFile->write(reinterpret_cast<const char*>(&kuBinDumpBegWord),
                             sizeof(UInt_t));
        fpBinDumpFile->write(reinterpret_cast<const char*>(&iBuffSzByte),
                             sizeof(Int_t));
        fpBinDumpFile->write(reinterpret_cast<const char*>(pDataBuff),
                             iBuffSzByte);
        fpBinDumpFile->write(reinterpret_cast<const char*>(&kuBinDumpEndWord),
                             sizeof(UInt_t));
      }  // if( kTRUE == fbEventDumpEna )
    }    // if( NULL != pDataBuff )
    else
      LOG(error) << "Invalid STAR SubEvent Output, can only happen if trigger "
                 << " object was not set => Do Nothing more with it!!! ";
  }  // for( UInt_t uEvent = 0; uEvent < eventBuffer.size(); ++uEvent )
  fTimer.Stop();
  Double_t dRealT = fTimer.RealTime();
  if (fbDebugMonitorMode) {
    LOG(debug2) << Form("Real time TS %12lu, Realtime: %12f ns",
                        static_cast<unsigned long>(fulTsCounter),
                        dRealT * 1e9);
    fhRealTimeDistr->Fill(dRealT * 1e3);
    fhRealTimeEvo->Fill(fulTsCounter, dRealT / fdTsCoreSizeInSec);
    fhMeanRealTimeEvo->Fill(fulTsCounter, dRealT / fdTsCoreSizeInSec);
  }  // if( fbDebugMonitorMode )
  fdRealTime += dRealT;
  if (dRealT < fdRealTimeMin) {
    if (fbDebugMonitorMode)
      LOG(debug) << Form("New min Real time TS %12lu, Real time: %9.6f ms Old "
                         "Min %9.6f Diff %.9f",
                         static_cast<unsigned long>(fulTsCounter),
                         dRealT,
                         fdRealTimeMin,
                         fdRealTimeMin - dRealT);
    fdRealTimeMin = dRealT;
  }  // if( dRealT < fdRealTimeMin )
  if (fdRealTimeMax < dRealT) {
    if (fbDebugMonitorMode)
      LOG(debug) << Form("New max Real time TS %12lu, Real time: %9.6f ms Old "
                         "Max %9.6f Diff %.9f",
                         static_cast<unsigned long>(fulTsCounter),
                         dRealT,
                         fdRealTimeMax,
                         fdRealTimeMax - dRealT);
    fdRealTimeMax = dRealT;
  }  // if( fdRealTimeMax < dRealT )
  fulNbEvents += eventBuffer.size();
  fulNbEventsSinceLastPrintout += eventBuffer.size();

  if (0 == fulTsCounter % 10000) {
    fdRealTime /= 10000;
    LOG(info) << Form("Processed %12lu TS, Real time: %6.3f ms/TS (Min %6.3f, "
                      "Max %6.3f), Events: %12lu (%9lu since last print)",
                      static_cast<unsigned long>(fulTsCounter),
                      fdRealTime,
                      fdRealTimeMin,
                      fdRealTimeMax,
                      static_cast<unsigned long>(fulNbEvents),
                      static_cast<unsigned long>(fulNbEventsSinceLastPrintout));
    fdRealTime                   = 0.0;
    fdRealTimeMin                = 1e6;
    fdRealTimeMax                = 0.0;
    fulNbEventsSinceLastPrintout = 0;
  }  // if( 0 == fulTsCounter % 10000 )
  fulTsCounter++;

  return kTRUE;
}

void CbmStar2019EventBuilderEtof::Reset() {}

void CbmStar2019EventBuilderEtof::Finish() {
  if (NULL != fpBinDumpFile) {
    LOG(info) << "Closing binary file used for event dump.";
    fpBinDumpFile->close();
  }  // if( NULL != fpBinDumpFile )

  /// If monitor mode enabled, trigger histos creation, obtain pointer on them and add them to the HTTP server
  if (kTRUE == fbMonitorMode) {

    SaveHistograms();
  }  // if( kTRUE == fbMonitorMode )
}

bool CbmStar2019EventBuilderEtof::SaveHistograms() {
  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos =
    fEventBuilderAlgo->GetHistoVector();
  std::vector<std::pair<TCanvas*, std::string>> vCanvas =
    fEventBuilderAlgo->GetCanvasVector();

  /// (Re-)Create ROOT file to store the histos
  TDirectory* oldDir = NULL;
  TFile* histoFile   = NULL;
  // Store current directory position to allow restore later
  oldDir = gDirectory;
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

  // Restore original directory position
  oldDir->cd();
  histoFile->Close();

  return kTRUE;
}

ClassImp(CbmStar2019EventBuilderEtof)
