#include "CbmMcbm2018MonitorTaskMuchLite.h"

#include "CbmMcbm2018MonitorAlgoMuchLite.h"
#include "CbmMcbm2018MuchPar.h"

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

Bool_t bMcbm2018ResetTaskMuchLite     = kFALSE;
Bool_t bMcbm2018WriteTaskMuchLite     = kFALSE;
Bool_t bMcbm2018ScanNoisyTaskMuchLite = kFALSE;

CbmMcbm2018MonitorTaskMuchLite::CbmMcbm2018MonitorTaskMuchLite()
  : CbmMcbmUnpack()
  , fbMonitorMode(kTRUE)
  , fbDebugMonitorMode(kFALSE)
  , fuHistoryHistoSize(3600)
  , fsHistoFilename("data/HistosMonitorMuch.root")
  , fuOffSpillCountLimit(200)
  , fulTsCounter(0)
  , fMonitorAlgo(nullptr)
{
  fMonitorAlgo = new CbmMcbm2018MonitorAlgoMuchLite();
}

CbmMcbm2018MonitorTaskMuchLite::~CbmMcbm2018MonitorTaskMuchLite() { delete fMonitorAlgo; }

Bool_t CbmMcbm2018MonitorTaskMuchLite::Init()
{
  LOG(info) << "CbmMcbm2018MonitorTaskMuchLite::Init";
  LOG(info) << "Initializing Much  2019 Monitor";

  return kTRUE;
}

void CbmMcbm2018MonitorTaskMuchLite::SetParContainers()
{
  LOG(info) << "Setting parameter containers for " << GetName();

  TList* parCList = fMonitorAlgo->GetParList();

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

  /*fUnpackParMuch =
  (CbmMcbm2018MuchPar*) (FairRun::Instance()->GetRuntimeDb()->getContainer(
  "CbmMcbm2018MuchPar")); */
}

Bool_t CbmMcbm2018MonitorTaskMuchLite::InitContainers()
{
  LOG(info) << "Init parameter containers for " << GetName();

  /// Control flags
  CbmMcbm2018MuchPar* pUnpackPar =
    dynamic_cast<CbmMcbm2018MuchPar*>(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmMcbm2018MuchPar"));
  if (nullptr == pUnpackPar) {
    LOG(error) << "Failed to obtain parameter container CbmMcbm2018MuchPar";
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
  LOG(info) << "after initOK";

  /// Transfer parameter values set from calling macro
  fMonitorAlgo->SetMonitorMode(fbMonitorMode);
  fMonitorAlgo->SetHistoryHistoSize(fuHistoryHistoSize);
  //fMonitorAlgo->SetPulserTotLimits(fuMinTotPulser, fuMaxTotPulser);
  fMonitorAlgo->SetSpillThreshold(fuOffSpillCountLimit);

  /// Histos creation, obtain pointer on them and add them to the HTTP server
  /// Trigger histo creation on all associated algos
  LOG(info) << "before creating histogram";
  initOK &= fMonitorAlgo->CreateHistograms();
  LOG(info) << "created histograms";
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

    server->RegisterCommand("/Reset_All", "bMcbm2018ResetTaskMuchLite=kTRUE");
    server->RegisterCommand("/Write_All", "bMcbm2018WriteTaskMuchLite=kTRUE");
    server->RegisterCommand("/ScanNoisyCh", "bMcbm2018ScanNoisyTaskMuchLite=kTRUE");
    server->Restrict("/Reset_All", "allow=admin");
    server->Restrict("/Write_All", "allow=admin");
    server->Restrict("/ScanNoisyCh", "allow=admin");

  }  // if( nullptr != server )

  return initOK;
}

Bool_t CbmMcbm2018MonitorTaskMuchLite::ReInitContainers()
{
  LOG(info) << "ReInit parameter containers for " << GetName();
  Bool_t initOK = fMonitorAlgo->ReInitContainers();

  return initOK;
}

/* Bool_t CbmMcbm2018MonitorTaskMuchLite::InitMuchParameters() {

  fuNrOfDpbs = fUnpackParMuch->GetNrOfDpbs();
  LOG(info) << "Nr. of MUCH DPBs:       " << fuNrOfDpbs;

  fDpbIdIndexMap.clear();
  for (UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb) {
    fDpbIdIndexMap[fUnpackParMuch->GetDpbId(uDpb)] = uDpb;
    LOG(info) << "Eq. ID for DPB #" << std::setw(2) << uDpb << " = 0x"
              << std::setw(4) << std::hex << fUnpackParMuch->GetDpbId(uDpb)
              << std::dec << " => "
              << fDpbIdIndexMap[fUnpackParMuch->GetDpbId(uDpb)];
  }  // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

  fuNbFebs = fUnpackParMuch->GetNrOfFebs();
  LOG(info) << "Nr. of FEBs:           " << fuNbFebs;

  fuNbStsXyters = fUnpackParMuch->GetNrOfAsics();
  LOG(info) << "Nr. of StsXyter ASICs: " << fuNbStsXyters;

  fvbCrobActiveFlag.resize(fuNrOfDpbs);
  //fvdFebAdcGain.resize(     fuNrOfDpbs );
  //fvdFebAdcOffs.resize(     fuNrOfDpbs );
  for (UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb) {
    fvbCrobActiveFlag[uDpb].resize(fUnpackParMuch->GetNbCrobsPerDpb());
    //fvdFebAdcGain[ uDpb ].resize(        fUnpackParMuch->GetNbCrobsPerDpb() );
    //fvdFebAdcOffs[ uDpb ].resize(        fUnpackParMuch->GetNbCrobsPerDpb() );
    for (UInt_t uCrobIdx = 0; uCrobIdx < fUnpackParMuch->GetNbCrobsPerDpb();
         ++uCrobIdx) {
      fvbCrobActiveFlag[uDpb][uCrobIdx] =
        fUnpackParMuch->IsCrobActive(uDpb, uCrobIdx);
      // fvdFebAdcGain[ uDpb ][ uCrobIdx ].resize(     fUnpackParMuch->GetNbFebsPerCrob(), 0.0 );
      //fvdFebAdcOffs[ uDpb ][ uCrobIdx ].resize(     fUnpackParMuch->GetNbFebsPerCrob(), 0.0 );
    }  // for( UInt_t uCrobIdx = 0; uCrobIdx < fUnpackParMuch->GetNbCrobsPerDpb(); ++uCrobIdx )
  }    // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

  for (UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb) {
    for (UInt_t uCrobIdx = 0; uCrobIdx < fUnpackParMuch->GetNbCrobsPerDpb();
         ++uCrobIdx) {
      LOG(info) << Form("DPB #%02u CROB #%02u Active:  ", uDpb, uCrobIdx)
                << fvbCrobActiveFlag[uDpb][uCrobIdx];
    }  // for( UInt_t uCrobIdx = 0; uCrobIdx < fUnpackParMuch->GetNbCrobsPerDpb(); ++uCrobIdx )
  }    // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

  if (fbBinningFw)
    LOG(info) << "Unpacking data in bin sorter FW mode";
  else
    LOG(info) << "Unpacking data in full time sorter FW mode (legacy)";

  // Internal status initialization
  fvulCurrentTsMsb.resize(fuNrOfDpbs);
  fvuCurrentTsMsbCycle.resize(fuNrOfDpbs);
  fvuInitialHeaderDone.resize(fuNrOfDpbs);
  fvuInitialTsMsbCycleHeader.resize(fuNrOfDpbs);
  fvuElinkLastTsHit.resize(fuNrOfDpbs);
  for (UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb) {
    fvulCurrentTsMsb[uDpb]           = 0;
    fvuCurrentTsMsbCycle[uDpb]       = 0;
    fvuInitialHeaderDone[uDpb]       = kFALSE;
    fvuInitialTsMsbCycleHeader[uDpb] = 0;
  }  // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

  fvdPrevMsTime.resize(kiMaxNbFlibLinks);
  fvulChanLastHitTime.resize(fuNbStsXyters);
  fvdChanLastHitTime.resize(fuNbStsXyters);
  fvdMsTime.resize(fuMaxNbMicroslices);
  fvuChanNbHitsInMs.resize(fuNbFebs);
  fvdChanLastHitTimeInMs.resize(fuNbFebs);
  fvusChanLastHitAdcInMs.resize(fuNbFebs);
  fvmAsicHitsInMs.resize(fuNbFebs);

  //fvdMsTime.resize( fuMaxNbMicroslices );
  //fvuChanNbHitsInMs.resize( fuNbStsXyters );
  //fvdChanLastHitTimeInMs.resize( fuNbStsXyters );
  //fvusChanLastHitAdcInMs.resize( fuNbStsXyters );
  //fvmAsicHitsInMs.resize( fuNbStsXyters );

  for (UInt_t uXyterIdx = 0; uXyterIdx < fuNbFebs; ++uXyterIdx) {
    fvulChanLastHitTime[uXyterIdx].resize(fUnpackParMuch->GetNbChanPerAsic());
    fvdChanLastHitTime[uXyterIdx].resize(fUnpackParMuch->GetNbChanPerAsic());
    fvuChanNbHitsInMs[uXyterIdx].resize(fUnpackParMuch->GetNbChanPerAsic());
    fvdChanLastHitTimeInMs[uXyterIdx].resize(
      fUnpackParMuch->GetNbChanPerAsic());
    fvusChanLastHitAdcInMs[uXyterIdx].resize(
      fUnpackParMuch->GetNbChanPerAsic());
    fvmAsicHitsInMs[uXyterIdx].clear();

    for (UInt_t uChan = 0; uChan < fUnpackParMuch->GetNbChanPerAsic();
         ++uChan) {
      fvulChanLastHitTime[uXyterIdx][uChan] = 0;
      fvdChanLastHitTime[uXyterIdx][uChan]  = -1.0;

      fvuChanNbHitsInMs[uXyterIdx][uChan].resize(fuMaxNbMicroslices);
      fvdChanLastHitTimeInMs[uXyterIdx][uChan].resize(fuMaxNbMicroslices);
      fvusChanLastHitAdcInMs[uXyterIdx][uChan].resize(fuMaxNbMicroslices);
      for (UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx) {
        fvuChanNbHitsInMs[uXyterIdx][uChan][uMsIdx]      = 0;
        fvdChanLastHitTimeInMs[uXyterIdx][uChan][uMsIdx] = -1.0;
        fvusChanLastHitAdcInMs[uXyterIdx][uChan][uMsIdx] = 0;
      }  // for( UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx )
    }  // for( UInt_t uChan = 0; uChan < fUnpackParMuch->GetNbChanPerAsic(); ++uChan )
  }  // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

  LOG(info) << "CbmMcbm2018MonitorTaskMuchLite::ReInitContainers => Changed "
               "fvuChanNbHitsInMs size "
            << fvuChanNbHitsInMs.size() << " VS " << fuNbFebs;
  LOG(info) << "CbmMcbm2018MonitorTaskMuchLite::ReInitContainers =>  Changed "
               "fvuChanNbHitsInMs size "
            << fvuChanNbHitsInMs[0].size() << " VS "
            << fUnpackParMuch->GetNbChanPerAsic();
  LOG(info) << "CbmMcbm2018MonitorTaskMuchLite::ReInitContainers =>  Changed "
               "fvuChanNbHitsInMs size "
            << fvuChanNbHitsInMs[0][0].size() << " VS " << fuMaxNbMicroslices;

  fvmFebHitsInMs.resize(fuNbFebs);
  fviFebTimeSecLastRateUpdate.resize(fuNbFebs, -1);
  fviFebCountsSinceLastRateUpdate.resize(fuNbFebs, -1);
  fvdFebChanCountsSinceLastRateUpdate.resize(fuNbFebs);
  fdMuchFebChanLastTimeForDist.resize(fuNbFebs);
  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    fvmFebHitsInMs[uFebIdx].clear();
    fvdFebChanCountsSinceLastRateUpdate[uFebIdx].resize(
      fUnpackParMuch->GetNbChanPerFeb(), 0.0);
    fdMuchFebChanLastTimeForDist[uFebIdx].resize(
      fUnpackParMuch->GetNbChanPerFeb(), -1.0);
  }  // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )

  ///----------------- SXM 2.0 Logic Error Tagging --------------------///
  //   SmxErrInitializeVariables();
  ///------------------------------------------------------------------///

  return kTRUE;
} */

void CbmMcbm2018MonitorTaskMuchLite::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  fMonitorAlgo->AddMsComponentToList(component, usDetectorId);
}

Bool_t CbmMcbm2018MonitorTaskMuchLite::DoUnpack(const fles::Timeslice& ts, size_t /*component*/)
{
  if (bMcbm2018ResetTaskMuchLite) {
    fMonitorAlgo->ResetAllHistos();
    bMcbm2018ResetTaskMuchLite = kFALSE;
  }  // if( bMcbm2018ResetMuchLite )
  if (bMcbm2018WriteTaskMuchLite) {
    fMonitorAlgo->SaveAllHistos(fsHistoFilename);
    bMcbm2018WriteTaskMuchLite = kFALSE;
  }  // if( bMcbm2018WriteMuchLite )
  if (bMcbm2018ScanNoisyTaskMuchLite) {
    fMonitorAlgo->ScanForNoisyChannels();
    bMcbm2018ScanNoisyTaskMuchLite = kFALSE;
  }  // if( bMcbm2018WriteMuchLite )


  /* if (fbMonitorMode && bMcbm2018MonitorTaskMuchResetHistos) {
    LOG(info) << "Reset Much Monitor histos ";
    fMonitorAlgo->ResetAllHistos();
    bMcbm2018MonitorTaskMuchResetHistos = kFALSE;
  }  // if( fbMonitorMode && bMcbm2018MonitorTaskT0ResetHistos ) */ //closed by me

  if (kFALSE == fMonitorAlgo->ProcessTs(ts)) {
    //if (kFALSE == fMonitorAlgo->ProcessMuchMs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index() << " in unpacker algorithm class";
    return kTRUE;
  }  // if( kFALSE == fMonitorAlgo->ProcessTs( ts ) )

  /// Cleqr the digis vector in case it was filled
  std::vector<CbmMuchBeamTimeDigi> vDigi = fMonitorAlgo->GetVector();
  fMonitorAlgo->ClearVector();

  if (0 == fulTsCounter % 10000) LOG(info) << "Processed " << fulTsCounter << "TS";
  fulTsCounter++;

  return kTRUE;
}


void CbmMcbm2018MonitorTaskMuchLite::Reset() {}

void CbmMcbm2018MonitorTaskMuchLite::Finish()
{
  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos = fMonitorAlgo->GetHistoVector();

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* histoFile = nullptr;

  // open separate histo file in recreate mode
  histoFile = new TFile(fsHistoFilename, "RECREATE");
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

void CbmMcbm2018MonitorTaskMuchLite::SetIgnoreOverlapMs(Bool_t bFlagIn) { fMonitorAlgo->SetIgnoreOverlapMs(bFlagIn); }

/*void CbmMcbm2018MonitorTaskMuchLite::SetChannelMap(UInt_t uChan0,
                                             UInt_t uChan1,
                                             UInt_t uChan2,
                                             UInt_t uChan3,
                                             UInt_t uChan4,
                                             UInt_t uChan5,
                                             UInt_t uChan6,
                                             UInt_t uChan7) {
  fMonitorAlgo->SetChannelMap(
    uChan0, uChan1, uChan2, uChan3, uChan4, uChan5, uChan6, uChan7);
} */

ClassImp(CbmMcbm2018MonitorTaskMuchLite)
