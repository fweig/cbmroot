// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmMcbm2018MonitorMuch                        -----
// -----                Created 11/05/18  by P.-A. Loizeau                 -----
// -----                Modified 11/05/18  by Ajit kumar                  -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018MonitorMuch.h"

// Data

// CbmRoot
#include "CbmHistManager.h"
#include "CbmMcbm2018MuchPar.h"

// FairRoot
#include <Logger.h>
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

// Root
#include "TClonesArray.h"
#include "THttpServer.h"
#include "TMath.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TString.h"
#include "TStyle.h"
#include <TFile.h>

// C++11
#include <bitset>

// C/C++
#include <iomanip>
#include <iostream>
#include <stdint.h>

Bool_t bMcbm2018ResetMuch     = kFALSE;
Bool_t bMcbm2018WriteMuch     = kFALSE;
Bool_t bMcbm2018ScanNoisyMuch = kFALSE;

CbmMcbm2018MonitorMuch::CbmMcbm2018MonitorMuch()
  : CbmMcbmUnpack()
  , fbMuchMode(kFALSE)
  , fvMsComponentsList()
  , fuNbCoreMsPerTs(0)
  , fuNbOverMsPerTs(0)
  ,
  //uTimeBin(1e-9),
  fbIgnoreOverlapMs(kFALSE)
  , fUnpackParMuch(NULL)
  , fuNrOfDpbs(0)
  , fDpbIdIndexMap()
  , fvbCrobActiveFlag()
  , fuNbFebs(0)
  , fuNbStsXyters(0)
  , fvdFebAdcGain()
  , fvdFebAdcOffs()
  ,
  /*
   fuNrOfDpbs(0),
   fDpbIdIndexMap(),
   fuNbStsXyters(0),
   fUnpackParMuch->GetNbChanPerAsic()(0),
   fuNbFebs(0),
*/
  fsHistoFileFullname("data/SetupHistos.root")
  , fbPrintMessages(kFALSE)
  , fPrintMessCtrl(stsxyter::MessagePrintMask::msg_print_Human)
  , fbEnableCoincidenceMaps(kFALSE)
  , fulCurrentTsIdx(0)
  , fulCurrentMsIdx(0)
  , fmMsgCounter()
  , fuCurrentEquipmentId(0)
  , fuCurrDpbId(0)
  , fuCurrDpbIdx(0)
  , fiRunStartDateTimeSec(-1)
  , fiBinSizeDatePlots(-1)
  , fvulCurrentTsMsb()
  , fvuCurrentTsMsbCycle()
  , fvuInitialHeaderDone()
  , fvuInitialTsMsbCycleHeader()
  , fvuElinkLastTsHit()
  , fvulChanLastHitTime()
  , fvdChanLastHitTime()
  , fvdPrevMsTime()
  , fvdMsTime()
  , fvuChanNbHitsInMs()
  , fvdChanLastHitTimeInMs()
  , fvusChanLastHitAdcInMs()
  ,
  //   fvmChanHitsInTs(),
  fdStartTime(-1.0)
  , prevtime(0.0)
  , fdStartTimeMsSz(-1.0)
  , ftStartTimeUnix(std::chrono::steady_clock::now())
  , fvmHitsInMs()
  , fvmAsicHitsInMs()
  , fvmFebHitsInMs()
  , fuMaxNbMicroslices(100)
  , fiTimeIntervalRateUpdate(10)
  , fviFebTimeSecLastRateUpdate()
  , fviFebCountsSinceLastRateUpdate()
  , fvdFebChanCountsSinceLastRateUpdate()
  , fbLongHistoEnable(kFALSE)
  , fuLongHistoNbSeconds(0)
  , fuLongHistoBinSizeSec(0)
  , fuLongHistoBinNb(0)
  , Counter(0)
  , Counter1(0)
  , fdCoincCenter(0.0)
  , fdCoincBorder(50.0)
  , fdCoincMin(fdCoincCenter - fdCoincBorder)
  , fdCoincMax(fdCoincCenter + fdCoincBorder)
  , fHM(new CbmHistManager())
  , fhStsMessType(NULL)
  , fhStsSysMessType(NULL)
  , fhStsFebChanAdcRaw_combined(NULL)
  , fhStsMessTypePerDpb(NULL)
  , fhStsSysMessTypePerDpb(NULL)
  , fhPulserStatusMessType(NULL)
  , fhPulserMsStatusFieldType(NULL)
  , fhStsHitsElinkPerDpb(NULL)
  , fhRate(NULL)
  , fhRateAdcCut(NULL)
  , fHistPadDistr()
  , fRealHistPadDistr()
  , fdFebChanCoincidenceLimit(100.0)
  , fhStsFebChanCntRaw()
  , fhStsFebChanCntRawGood()
  , fhStsFebChanAdcRaw()
  , fhStsFebChanAdcRawProf()
  , fhStsFebChanAdcCal()
  , fhStsFebChanAdcCalProf()
  , fhStsFebChanRawTs()
  , fhStsFebChanMissEvt()
  , fhStsFebChanMissEvtEvo()
  , fhStsFebAsicMissEvtEvo()
  , fhStsFebMissEvtEvo()
  , fhStsFebChanHitRateEvo()
  , fhStsFebChanHitRateProf()
  , fhStsFebAsicHitRateEvo()
  , fhStsFebHitRateEvo()
  , fhStsFebHitRateEvo_mskch()
  , fhStsFebHitRateEvo_mskch_adccut()
  , fhStsFebChanHitRateEvoLong()
  , fhStsFebAsicHitRateEvoLong()
  , fhStsFebHitRateEvoLong()
  , fdStsFebChanLastTimeForDist()
  , fhStsFebChanDistT()
  , fhStsFebChanDtCoinc()
  , fhStsFebChanCoinc()
  , fbSmx2ErrorUseNoiseLevels(kFALSE)
  , fdSmxErrCoincWinM07(kdSmxErrCoincWinMainM07)
  , fdSmxErrCoincWinM08(kdSmxErrCoincWinMainM08)
  , fdSmxErrCoincWinM09(kdSmxErrCoincWinMainM09)
  , fdSmxErrCoincWinM10(kdSmxErrCoincWinMainM10)
  , fdSmxErrCoincWinM11(kdSmxErrCoincWinMainM11)
  , fvdSmxErrTimeLastHits()
  , fvuSmxErrIdxFirstHitM07()
  , fvuSmxErrIdxFirstHitM08()
  , fvuSmxErrIdxFirstHitM09()
  , fvuSmxErrIdxFirstHitM10()
  , fvuSmxErrIdxFirstHitM11()
  , fvuSmxErrIdxLastHit()
  , fhStsFebSmxErrRatioEvo()
  , fhStsFebSmxErrRatioEvoAsic()
  , fhStsFebSmxErrRatioCopyEvo()
  , fhStsFebSmxErrRatioCopyEvoAsic()
  , fhStsFebSmxErrRatioCopySameAdcEvo()
  , fhStsFebSmxErrRatioCopySameAdcEvoAsic()
  , fcMsSizeAll(NULL) {}

CbmMcbm2018MonitorMuch::~CbmMcbm2018MonitorMuch() {}

Bool_t CbmMcbm2018MonitorMuch::Init() {
  LOG(info) << "Initializing flib StsXyter unpacker for STS";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) { LOG(fatal) << "No FairRootManager instance"; }

  return kTRUE;
}

void CbmMcbm2018MonitorMuch::SetParContainers() {
  LOG(info) << "Setting parameter containers for " << GetName();
  fUnpackParMuch =
    (CbmMcbm2018MuchPar*) (FairRun::Instance()->GetRuntimeDb()->getContainer(
      "CbmMcbm2018MuchPar"));
}


Bool_t CbmMcbm2018MonitorMuch::InitContainers() {
  LOG(info) << "Init parameter containers for " << GetName();

  Bool_t bInit = InitStsParameters();
  if (kTRUE == bInit) CreateHistograms();

  return bInit;
}

Bool_t CbmMcbm2018MonitorMuch::ReInitContainers() {
  LOG(info) << "ReInit parameter containers for " << GetName();

  return InitStsParameters();
}

Bool_t CbmMcbm2018MonitorMuch::InitStsParameters() {

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
  fvdFebAdcGain.resize(fuNrOfDpbs);
  fvdFebAdcOffs.resize(fuNrOfDpbs);
  for (UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb) {
    fvbCrobActiveFlag[uDpb].resize(fUnpackParMuch->GetNbCrobsPerDpb());
    fvdFebAdcGain[uDpb].resize(fUnpackParMuch->GetNbCrobsPerDpb());
    fvdFebAdcOffs[uDpb].resize(fUnpackParMuch->GetNbCrobsPerDpb());
    for (UInt_t uCrobIdx = 0; uCrobIdx < fUnpackParMuch->GetNbCrobsPerDpb();
         ++uCrobIdx) {
      fvbCrobActiveFlag[uDpb][uCrobIdx] =
        fUnpackParMuch->IsCrobActive(uDpb, uCrobIdx);
      fvdFebAdcGain[uDpb][uCrobIdx].resize(fUnpackParMuch->GetNbFebsPerCrob(),
                                           0.0);
      fvdFebAdcOffs[uDpb][uCrobIdx].resize(fUnpackParMuch->GetNbFebsPerCrob(),
                                           0.0);
    }  // for( UInt_t uCrobIdx = 0; uCrobIdx < fUnpackParMuch->GetNbCrobsPerDpb(); ++uCrobIdx )
  }    // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

  for (UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb) {
    for (UInt_t uCrobIdx = 0; uCrobIdx < fUnpackParMuch->GetNbCrobsPerDpb();
         ++uCrobIdx) {
      LOG(info) << Form("DPB #%02u CROB #%02u Active:  ", uDpb, uCrobIdx)
                << fvbCrobActiveFlag[uDpb][uCrobIdx];
    }  // for( UInt_t uCrobIdx = 0; uCrobIdx < fUnpackParMuch->GetNbCrobsPerDpb(); ++uCrobIdx )
  }    // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

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
  fvuChanNbHitsInMs.resize(fuNbStsXyters);
  fvdChanLastHitTimeInMs.resize(fuNbStsXyters);
  fvusChanLastHitAdcInMs.resize(fuNbStsXyters);
  fvmAsicHitsInMs.resize(fuNbStsXyters);

  for (UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx) {
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

  LOG(info) << "CbmMcbm2018MonitorMuch::ReInitContainers => Changed "
               "fvuChanNbHitsInMs size "
            << fvuChanNbHitsInMs.size() << " VS " << fuNbStsXyters;
  LOG(info) << "CbmMcbm2018MonitorMuch::ReInitContainers =>  Changed "
               "fvuChanNbHitsInMs size "
            << fvuChanNbHitsInMs[0].size() << " VS "
            << fUnpackParMuch->GetNbChanPerAsic();
  LOG(info) << "CbmMcbm2018MonitorMuch::ReInitContainers =>  Changed "
               "fvuChanNbHitsInMs size "
            << fvuChanNbHitsInMs[0][0].size() << " VS " << fuMaxNbMicroslices;

  fvmFebHitsInMs.resize(fuNbFebs);
  fviFebTimeSecLastRateUpdate.resize(fuNbFebs, -1);
  fviFebCountsSinceLastRateUpdate.resize(fuNbFebs, -1);
  fvdFebChanCountsSinceLastRateUpdate.resize(fuNbFebs);
  fdStsFebChanLastTimeForDist.resize(fuNbFebs);
  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    fvmFebHitsInMs[uFebIdx].clear();
    fvdFebChanCountsSinceLastRateUpdate[uFebIdx].resize(
      fUnpackParMuch->GetNbChanPerFeb(), 0.0);
    fdStsFebChanLastTimeForDist[uFebIdx].resize(
      fUnpackParMuch->GetNbChanPerFeb(), -1.0);
  }  // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )

  ///----------------- SXM 2.0 Logic Error Tagging --------------------///
  SmxErrInitializeVariables();
  ///------------------------------------------------------------------///

  return kTRUE;
}

void CbmMcbm2018MonitorMuch::AddMsComponentToList(size_t component,
                                                  UShort_t /*usDetectorId*/) {
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Check if this does not go above hardcoded limits
  if (kiMaxNbFlibLinks <= component) {
    LOG(error) << "CbmMcbm2018MonitorMuch::AddMsComponentToList => "
               << "Ignored the addition of component " << component
               << " as it is above the hadcoded limit of "
               << static_cast<Int_t>(kiMaxNbFlibLinks) << " !!!!!!!!! "
               << "\n"
               << "         To change this behavior check kiMaxNbFlibLinks in "
                  "CbmMcbm2018MonitorMuch.cxx";
    return;
  }  // if( kiMaxNbFlibLinks <= component  )


  /// Add to list
  fvMsComponentsList.push_back(component);
  LOG(info)
    << "CbmMcbm2018MonitorMuch::AddMsComponentToList => Added component: "
    << component;

  /// Create MS size monitoring histos
  if (NULL == fhMsSz[component]) {
    TString sMsSzName = Form("MsSz_link_%02lu", component);
    TString sMsSzTitle =
      Form("Size of MS for nDPB of link %02lu; Ms Size [bytes]", component);
    fhMsSz[component] =
      new TH1F(sMsSzName.Data(), sMsSzTitle.Data(), 160000, 0., 20000.);
    fHM->Add(sMsSzName.Data(), fhMsSz[component]);

    sMsSzName  = Form("MsSzTime_link_%02lu", component);
    sMsSzTitle = Form(
      "Size of MS vs time for gDPB of link %02lu; Time[s] ; Ms Size [bytes]",
      component);
    fhMsSzTime[component] =
      new TProfile(sMsSzName.Data(), sMsSzTitle.Data(), 15000, 0., 300.);
    fHM->Add(sMsSzName.Data(), fhMsSzTime[component]);

    if (NULL != fcMsSizeAll) {
      fcMsSizeAll->cd(1 + component);
      gPad->SetLogy();
      fhMsSzTime[component]->Draw("hist le0");
    }  // if( NULL != fcMsSizeAll )
    LOG(info) << "Added MS size histo for component: " << component << " (DPB)";

#ifdef USE_HTTP_SERVER
    THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
    if (server) {
      server->Register("/FlibRaw", fhMsSz[component]);
      server->Register("/FlibRaw", fhMsSzTime[component]);
    }  // if( server )
#endif
  }  // if( NULL == fhMsSz[ component ] )
}
void CbmMcbm2018MonitorMuch::SetNbMsInTs(size_t uCoreMsNb,
                                         size_t uOverlapMsNb) {
  fuNbCoreMsPerTs = uCoreMsNb;
  fuNbOverMsPerTs = uOverlapMsNb;
  //LOG(info) <<" fuNbCoreMsPerTs "<<fuNbCoreMsPerTs<<" fuNbOverMsPerTs "<<fuNbOverMsPerTs;
  UInt_t uNbMsTotal = fuNbCoreMsPerTs + fuNbOverMsPerTs;

  if (fuMaxNbMicroslices < uNbMsTotal) {
    fuMaxNbMicroslices = uNbMsTotal;

    fvdMsTime.resize(fuMaxNbMicroslices);
    fvuChanNbHitsInMs.resize(fuNbStsXyters);
    fvdChanLastHitTimeInMs.resize(fuNbStsXyters);
    fvusChanLastHitAdcInMs.resize(fuNbStsXyters);
    for (UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx) {
      fvuChanNbHitsInMs[uXyterIdx].resize(fUnpackParMuch->GetNbChanPerAsic());
      fvdChanLastHitTimeInMs[uXyterIdx].resize(
        fUnpackParMuch->GetNbChanPerAsic());
      fvusChanLastHitAdcInMs[uXyterIdx].resize(
        fUnpackParMuch->GetNbChanPerAsic());
      for (UInt_t uChan = 0; uChan < fUnpackParMuch->GetNbChanPerAsic();
           ++uChan) {
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
    LOG(info)
      << "CbmMcbm2018MonitorMuch::DoUnpack => Changed fvuChanNbHitsInMs size "
      << fvuChanNbHitsInMs.size() << " VS " << fuNbStsXyters;
    LOG(info)
      << "CbmMcbm2018MonitorMuch::DoUnpack =>  Changed fvuChanNbHitsInMs size "
      << fvuChanNbHitsInMs[0].size() << " VS "
      << fUnpackParMuch->GetNbChanPerAsic();
    LOG(info)
      << "CbmMcbm2018MonitorMuch::DoUnpack =>  Changed fvuChanNbHitsInMs size "
      << fvuChanNbHitsInMs[0][0].size() << " VS " << fuMaxNbMicroslices;
  }  // if( fuMaxNbMicroslices < uNbMsTotal )
}

void CbmMcbm2018MonitorMuch::SetCoincidenceBorder(Double_t dCenterPos,
                                                  Double_t dBorderVal) {
  fdCoincCenter = dCenterPos;
  fdCoincBorder = dBorderVal;
  fdCoincMin    = dCenterPos - dBorderVal;
  fdCoincMax    = dCenterPos + dBorderVal;
}

void CbmMcbm2018MonitorMuch::CreateHistograms() {
  TString sHistName {""};
  TString title {""};

  sHistName     = "hPulserMessageType";
  title         = "Nb of message for each type; Type";
  fhStsMessType = new TH1I(sHistName, title, 6, 0., 6.);
  fhStsMessType->GetXaxis()->SetBinLabel(1, "Dummy");
  fhStsMessType->GetXaxis()->SetBinLabel(2, "Hit");
  fhStsMessType->GetXaxis()->SetBinLabel(3, "TsMsb");
  fhStsMessType->GetXaxis()->SetBinLabel(4, "Epoch");
  fhStsMessType->GetXaxis()->SetBinLabel(5, "Status");
  fhStsMessType->GetXaxis()->SetBinLabel(6, "Empty");

  /* *** Missing int + MessType OP!!!! ****
   fhStsMessType->GetXaxis()->SetBinLabel(1 + stsxyter::MessType::Dummy,       "Dummy");
   fhStsMessType->GetXaxis()->SetBinLabel(1 + stsxyter::MessType::Hit,         "Hit");
   fhStsMessType->GetXaxis()->SetBinLabel(1 + stsxyter::MessType::TsMsb,       "TsMsb");
   fhStsMessType->GetXaxis()->SetBinLabel(1 + stsxyter::MessType::ReadDataAck, "ReadDataAck");
   fhStsMessType->GetXaxis()->SetBinLabel(1 + stsxyter::MessType::Ack,         "Ack");
*/

  sHistName        = "hPulserSysMessType";
  title            = "Nb of system message for each type; System Type";
  fhStsSysMessType = new TH1I(sHistName, title, 17, 0., 17.);
  /*
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   hSysMessType->GetXaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
*/
  sHistName                   = "hStsFebChanAdcRaw_combined";
  title                       = "ADC hist combined";
  fhStsFebChanAdcRaw_combined = new TH1I(sHistName,
                                         title,
                                         stsxyter::kuHitNbAdcBins,
                                         -0.5,
                                         stsxyter::kuHitNbAdcBins - 0.5);

  LOG(debug) << "Initialized 1st Histo";
  sHistName = "hPulserMessageTypePerDpb";
  title     = "Nb of message of each type for each DPB; DPB; Type";
  fhStsMessTypePerDpb =
    new TH2I(sHistName, title, fuNrOfDpbs, 0, fuNrOfDpbs, 6, 0., 6.);
  fhStsMessTypePerDpb->GetYaxis()->SetBinLabel(1, "Dummy");
  fhStsMessTypePerDpb->GetYaxis()->SetBinLabel(2, "Hit");
  fhStsMessTypePerDpb->GetYaxis()->SetBinLabel(3, "TsMsb");
  fhStsMessTypePerDpb->GetYaxis()->SetBinLabel(4, "Epoch");
  fhStsMessTypePerDpb->GetYaxis()->SetBinLabel(5, "Status");
  fhStsMessTypePerDpb->GetYaxis()->SetBinLabel(6, "Empty");
  /* *** Missing int + MessType OP!!!! ****
   fhStsMessType->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::Dummy,       "Dummy");
   fhStsMessType->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::Hit,         "Hit");
   fhStsMessType->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::TsMsb,       "TsMsb");
   fhStsMessType->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::ReadDataAck, "ReadDataAck");
   fhStsMessType->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::Ack,         "Ack");
*/


  for (UInt_t uModuleId = 0; uModuleId < 2; ++uModuleId) {
    /// Raw Ts Distribution
    sHistName = Form("HistPadDistr_Module_%01u", uModuleId);
    title     = Form("Pad distribution for, Module #%01u; ", uModuleId);

    //Below for Rectangular Module shape VS
    fHistPadDistr.push_back(
      new TH2I(sHistName, title, 23, -0.5, 22.5, 97, -0.5, 96.5));

    sHistName = Form("RealHistPadDistr_Module_%01u", uModuleId);
    title = Form("Progressive Pad distribution for, Module #%01u; ", uModuleId);
    //Below for Progressive Geometry Module shape VS
    fRealHistPadDistr.push_back(
      new TH2D(sHistName, title, 500, -0.5, 499.5, 1000, -0.5, 999.5));
  }

  sHistName = "hRate";
  title     = "Rate in kHz";
  fhRate    = new TH1I(sHistName, title, 10000, -0.5, 9999.5);

  sHistName    = "hRateAdcCut";
  title        = "Rate in kHz with Adc cut";
  fhRateAdcCut = new TH1I(sHistName, title, 10000, -0.5, 9999.5);

  sHistName = "hPulserSysMessTypePerDpb";
  title = "Nb of system message of each type for each DPB; DPB; System Type";
  fhStsSysMessTypePerDpb =
    new TH2I(sHistName, title, fuNrOfDpbs, 0, fuNrOfDpbs, 17, 0., 17.);
  /*
   hSysMessType->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   hSysMessType->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   hSysMessType->GetYaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
*/

  sHistName = "hPulserStatusMessType";
  title = "Nb of status message of each type for each DPB; ASIC; Status Type";
  fhPulserStatusMessType =
    new TH2I(sHistName, title, fuNbStsXyters, 0, fuNbStsXyters, 16, 0., 16.);
  /*
   fhPulserStatusMessType->GetYaxis()->SetBinLabel( 1, "Dummy");
   fhPulserStatusMessType->GetYaxis()->SetBinLabel( 2, "Hit");
   fhPulserStatusMessType->GetYaxis()->SetBinLabel( 3, "TsMsb");
   fhPulserStatusMessType->GetYaxis()->SetBinLabel( 4, "Epoch");
*/

  sHistName = "hPulserMsStatusFieldType";
  title =
    "For each flag in the MS header, ON/OFF counts; Flag bit []; ON/OFF; MS []";
  fhPulserMsStatusFieldType =
    new TH2I(sHistName, title, 16, -0.5, 15.5, 2, -0.5, 1.5);
  /*
   fhPulserMsStatusFieldType->GetYaxis()->SetBinLabel( 1, "Dummy");
   fhPulserMsStatusFieldType->GetYaxis()->SetBinLabel( 2, "Hit");
   fhPulserMsStatusFieldType->GetYaxis()->SetBinLabel( 3, "TsMsb");
   fhPulserMsStatusFieldType->GetYaxis()->SetBinLabel( 4, "Epoch");
*/

  sHistName = "hStsHitsElinkPerDpb";
  title = "Nb of hit messages per eLink for each DPB; DPB; eLink; Hits nb []";
  fhStsHitsElinkPerDpb =
    new TH2I(sHistName, title, fuNrOfDpbs, 0, fuNrOfDpbs, 42, 0., 42.);

  LOG(debug) << "Initialized 2nd Histo";
  /*
   // Number of rate bins =
   //      9 for the sub-unit decade
   //    + 9 for each unit of each decade * 10 for the subdecade range
   //    + 1 for the closing bin top edge
   const Int_t iNbDecadesRate    = 9;
   const Int_t iNbStepsDecade    = 9;
   const Int_t iNbSubStepsInStep = 10;
   const Int_t iNbBinsRate = iNbStepsDecade
                           + iNbStepsDecade * iNbSubStepsInStep * iNbDecadesRate
                           + 1;
   Double_t dBinsRate[iNbBinsRate];
      // First fill sub-unit decade
   for( Int_t iSubU = 0; iSubU < iNbStepsDecade; iSubU ++ )
      dBinsRate[ iSubU ] = 0.1 * ( 1 + iSubU );
   std::cout << std::endl;
      // Then fill the main decades
   Double_t dSubstepSize = 1.0 / iNbSubStepsInStep;
   for( Int_t iDecade = 0; iDecade < iNbDecadesRate; iDecade ++)
   {
      Double_t dBase = std::pow( 10, iDecade );
      Int_t iDecadeIdx = iNbStepsDecade
                       + iDecade * iNbStepsDecade * iNbSubStepsInStep;
      for( Int_t iStep = 0; iStep < iNbStepsDecade; iStep++ )
      {
         Int_t iStepIdx = iDecadeIdx + iStep * iNbSubStepsInStep;
         for( Int_t iSubStep = 0; iSubStep < iNbSubStepsInStep; iSubStep++ )
         {
            dBinsRate[ iStepIdx + iSubStep ] = dBase * (1 + iStep)
                                             + dBase * dSubstepSize * iSubStep;
         } // for( Int_t iSubStep = 0; iSubStep < iNbSubStepsInStep; iSubStep++ )
      } // for( Int_t iStep = 0; iStep < iNbStepsDecade; iStep++ )
   } // for( Int_t iDecade = 0; iDecade < iNbDecadesRate; iDecade ++)
   dBinsRate[ iNbBinsRate - 1 ] = std::pow( 10, iNbDecadesRate );
*/

  LOG(debug) << "Initialized 3rd Histo";
  ///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

  //UInt_t uAlignedLimit = fuLongHistoNbSeconds - (fuLongHistoNbSeconds % fuLongHistoBinSizeSec);
  UInt_t uAlignedLimit = 0;
  //fuLongHistoBinNb = uAlignedLimit / fuLongHistoBinSizeSec; VS & AK: Commented due to Floating Point Exception error;
  /*
   UInt_t uNbBinEvo = (32768 + 1) * 2;
   Double_t dMaxEdgeEvo = stsxyter::kdClockCycleNs
                         * static_cast< Double_t >( uNbBinEvo ) / 2.0;
   Double_t dMinEdgeEvo = dMaxEdgeEvo * -1.0;

   UInt_t uNbBinDt     = static_cast<UInt_t>( (fdCoincMax - fdCoincMin )/stsxyter::kdClockCycleNs );
*/
  ///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
  /// FEB-8 plots
  /// All histos per FEB: with channels or ASIC as axis!!
  fhStsFebChanDtCoinc.resize(fuNbFebs);
  fhStsFebChanCoinc.resize(fuNbFebs);
  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    /// Channel counts
    sHistName = Form("hStsFebChanCntRaw_%03u", uFebIdx);
    title =
      Form("Hits Count per channel, FEB #%03u; Channel; Hits []", uFebIdx);
    fhStsFebChanCntRaw.push_back(
      new TH1I(sHistName,
               title,
               fUnpackParMuch->GetNbChanPerFeb(),
               -0.5,
               fUnpackParMuch->GetNbChanPerFeb() - 0.5));

    sHistName = Form("hStsFebChanCntRawGood_%03u", uFebIdx);
    title = Form("Hits Count per channel in good MS (SX2 bug flag off), FEB "
                 "#%03u; Channel; Hits []",
                 uFebIdx);
    fhStsFebChanCntRawGood.push_back(
      new TH1I(sHistName,
               title,
               fUnpackParMuch->GetNbChanPerFeb(),
               -0.5,
               fUnpackParMuch->GetNbChanPerFeb() - 0.5));

    /// Raw Adc Distribution
    sHistName = Form("hStsFebChanAdcRaw_%03u", uFebIdx);
    title = Form("Raw Adc distribution per channel, FEB #%03u; Channel []; Adc "
                 "[]; Hits []",
                 uFebIdx);
    fhStsFebChanAdcRaw.push_back(
      new TH2I(sHistName,
               title,
               fUnpackParMuch->GetNbChanPerFeb(),
               -0.5,
               fUnpackParMuch->GetNbChanPerFeb() - 0.5,
               stsxyter::kuHitNbAdcBins,
               -0.5,
               stsxyter::kuHitNbAdcBins - 0.5));

    /// Raw Adc Distribution profile
    sHistName = Form("hStsFebChanAdcRawProfc_%03u", uFebIdx);
    title = Form("Raw Adc prodile per channel, FEB #%03u; Channel []; Adc []",
                 uFebIdx);
    fhStsFebChanAdcRawProf.push_back(
      new TProfile(sHistName,
                   title,
                   fUnpackParMuch->GetNbChanPerFeb(),
                   -0.5,
                   fUnpackParMuch->GetNbChanPerFeb() - 0.5));

    /// Cal Adc Distribution
    sHistName = Form("hStsFebChanAdcCal_%03u", uFebIdx);
    title = Form("Cal. Adc distribution per channel, FEB #%03u; Channel []; "
                 "Adc [e-]; Hits []",
                 uFebIdx);
    fhStsFebChanAdcCal.push_back(
      new TH2I(sHistName,
               title,
               fUnpackParMuch->GetNbChanPerFeb(),
               -0.5,
               fUnpackParMuch->GetNbChanPerFeb() - 0.5,
               50,
               0.,
               100000.));

    /// Cal Adc Distribution profile
    sHistName = Form("hStsFebChanAdcCalProfc_%03u", uFebIdx);
    title     = Form(
      "Cal. Adc prodile per channel, FEB #%03u; Channel []; Adc [e-]", uFebIdx);
    fhStsFebChanAdcCalProf.push_back(
      new TProfile(sHistName,
                   title,
                   fUnpackParMuch->GetNbChanPerFeb(),
                   -0.5,
                   fUnpackParMuch->GetNbChanPerFeb() - 0.5));

    /// Raw Ts Distribution
    sHistName = Form("hStsFebChanRawTs_%03u", uFebIdx);
    title = Form("Raw Timestamp distribution per channel, FEB #%03u; Channel "
                 "[]; Ts []; Hits []",
                 uFebIdx);
    fhStsFebChanRawTs.push_back(
      new TH2I(sHistName,
               title,
               fUnpackParMuch->GetNbChanPerFeb(),
               -0.5,
               fUnpackParMuch->GetNbChanPerFeb() - 0.5,
               stsxyter::kuHitNbTsBins,
               -0.5,
               stsxyter::kuHitNbTsBins - 0.5));

    /// Missed event flag
    sHistName = Form("hStsFebChanMissEvt_%03u", uFebIdx);
    title = Form("Missed Event flags per channel, FEB #%03u; Channel []; Miss "
                 "Evt []; Hits []",
                 uFebIdx);
    fhStsFebChanMissEvt.push_back(
      new TH2I(sHistName,
               title,
               fUnpackParMuch->GetNbChanPerFeb(),
               -0.5,
               fUnpackParMuch->GetNbChanPerFeb() - 0.5,
               2,
               -0.5,
               1.5));

    /// Missed event flag counts evolution
    sHistName = Form("hStsFebChanMissEvtEvo_%03u", uFebIdx);
    title     = Form("Missed Evt flags per second & channel in FEB #%03u; Time "
                 "[s]; Channel []; Missed Evt flags []",
                 uFebIdx);
    fhStsFebChanMissEvtEvo.push_back(
      new TH2I(sHistName,
               title,
               1800,
               0,
               1800,
               fUnpackParMuch->GetNbChanPerFeb(),
               -0.5,
               fUnpackParMuch->GetNbChanPerFeb() - 0.5));

    /// Missed event flag counts evolution
    sHistName = Form("hStsFebAsicMissEvtEvo_%03u", uFebIdx);
    title = Form("Missed Evt flags per second & StsXyter in FEB #%03u; Time "
                 "[s]; Asic []; Missed Evt flags []",
                 uFebIdx);
    fhStsFebAsicMissEvtEvo.push_back(
      new TH2I(sHistName,
               title,
               1800,
               0,
               1800,
               fUnpackParMuch->GetNbAsicsPerFeb(),
               -0.5,
               fUnpackParMuch->GetNbAsicsPerFeb() - 0.5));

    /// Missed event flag counts evolution
    sHistName = Form("hStsFebMissEvtEvo_%03u", uFebIdx);
    title     = Form("Missed Evt flags per second & channel in FEB #%03u; Time "
                 "[s]; Missed Evt flags []",
                 uFebIdx);
    fhStsFebMissEvtEvo.push_back(new TH1I(sHistName, title, 1800, 0, 1800));

    /// Hit rates evo per channel
    sHistName = Form("hStsFebChanRateEvo_%03u", uFebIdx);
    title     = Form(
      "Hits per second & channel in FEB #%03u; Time [s]; Channel []; Hits []",
      uFebIdx);
    fhStsFebChanHitRateEvo.push_back(
      new TH2I(sHistName,
               title,
               1800,
               0,
               1800,
               fUnpackParMuch->GetNbChanPerFeb(),
               -0.5,
               fUnpackParMuch->GetNbChanPerFeb() - 0.5));

    /// Hit rates profile per channel
    sHistName = Form("hStsFebChanRateProf_%03u", uFebIdx);
    title     = Form(
      "Hits per second for each channel in FEB #%03u; Channel []; Hits/s []",
      uFebIdx);
    fhStsFebChanHitRateProf.push_back(
      new TProfile(sHistName,
                   title,
                   fUnpackParMuch->GetNbChanPerFeb(),
                   -0.5,
                   fUnpackParMuch->GetNbChanPerFeb() - 0.5));

    /// Hit rates evo per StsXyter
    sHistName = Form("hStsFebAsicRateEvo_%03u", uFebIdx);
    title     = Form(
      "Hits per second & StsXyter in FEB #%03u; Time [s]; Asic []; Hits []",
      uFebIdx);
    fhStsFebAsicHitRateEvo.push_back(
      new TH2I(sHistName,
               title,
               1800,
               0,
               1800,
               fUnpackParMuch->GetNbAsicsPerFeb(),
               -0.5,
               fUnpackParMuch->GetNbAsicsPerFeb() - 0.5));
    /*
      /// Hit rates evo per FEB
      sHistName = Form( "hStsFebRateEvo_%03u", uFebIdx );
      title = Form( "Hits per second in FEB #%03u; Time [s]; Hits []", uFebIdx );
      fhStsFebHitRateEvo.push_back( new TH1I(sHistName, title, 1800, 0, 1800 ) );
*/
    /// Hit rates evo per FEB
    sHistName = Form("hStsFebRateEvo_%03u", uFebIdx);
    title = Form("Hits per second in FEB #%03u; Time [s]; Hits []", uFebIdx);
    //fhStsFebHitRateEvo.push_back( new TH1I(sHistName, title, 5000000, 0., 10000000. ) ); //2ms
    fhStsFebHitRateEvo.push_back(
      new TH1I(sHistName, title, 250000, 0., 1000000.));  //4ms
    //fhStsFebHitRateEvo.push_back( new TH1I(sHistName, title, 200000, 0., 1000000. ) ); //5ms
    // fhStsFebHitRateEvo.push_back( new TH1I(sHistName, title, 125000, 0., 1000000. ) ); //8ms
    // fhStsFebHitRateEvo.push_back( new TH1I(sHistName, title, 100000, 0., 1000000. ) ); //10ms
    // fhStsFebHitRateEvo.push_back( new TH1I(sHistName, title, 62500, 0., 1000000. ) ); //16ms
    // fhStsFebHitRateEvo.push_back( new TH1I(sHistName, title, 50000, 0., 1000000. ) ); //20ms
    // fhStsFebHitRateEvo.push_back( new TH1I(sHistName, title, 20000, 0., 1000000. ) ); //50ms
    // fhStsFebHitRateEvo.push_back( new TH1I(sHistName, title, 10000, 0., 1000000. ) ); //100ms
    // fhStsFebHitRateEvo.push_back( new TH1I(sHistName, title, 2000, 0., 1000000. ) ); //500ms

    /// Hit rates evo per FEB
    sHistName = Form("hStsFebRateEvo_mskch_%03u", uFebIdx);
    title = Form("Hits per second in FEB #%03u; Time [s]; Hits []", uFebIdx);
    fhStsFebHitRateEvo_mskch.push_back(
      new TH1I(sHistName, title, 1800, 0, 1800));

    /// Hit rates evo per FEB
    sHistName = Form("hStsFebRateEvo_mskch_adcut_%03u", uFebIdx);
    title = Form("Hits per second in FEB #%03u; Time [s]; Hits []", uFebIdx);
    fhStsFebHitRateEvo_mskch_adccut.push_back(
      new TH1I(sHistName, title, 1800, 0, 1800));

    /// Hit rates evo per channel, 1 minute bins, 24h
    sHistName = Form("hStsFebChanRateEvoLong_%03u", uFebIdx);
    title     = Form(
      "Hits per second & channel in FEB #%03u; Time [min]; Channel []; Hits []",
      uFebIdx);
    fhStsFebChanHitRateEvoLong.push_back(
      new TH2D(sHistName,
               title,
               fuLongHistoBinNb,
               -0.5,
               uAlignedLimit - 0.5,
               fUnpackParMuch->GetNbChanPerFeb(),
               -0.5,
               fUnpackParMuch->GetNbChanPerFeb() - 0.5));

    /// Hit rates evo per channel, 1 minute bins, 24h
    sHistName = Form("hStsFebAsicRateEvoLong_%03u", uFebIdx);
    title     = Form(
      "Hits per second & StsXyter in FEB #%03u; Time [min]; Asic []; Hits []",
      uFebIdx);
    fhStsFebAsicHitRateEvoLong.push_back(
      new TH2D(sHistName,
               title,
               fuLongHistoBinNb,
               -0.5,
               uAlignedLimit - 0.5,
               fUnpackParMuch->GetNbAsicsPerFeb(),
               -0.5,
               fUnpackParMuch->GetNbAsicsPerFeb() - 0.5));

    /// Hit rates evo per FEB, 1 minute bins, 24h
    sHistName = Form("hStsFebRateEvoLong_%03u", uFebIdx);
    title = Form("Hits per second in FEB #%03u; Time [min]; Hits []", uFebIdx);
    fhStsFebHitRateEvoLong.push_back(
      new TH1D(sHistName, title, fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5));

    /// Distance between hits on same channel
    sHistName = Form("hStsFebChanDistT_%03u", uFebIdx);
    title = Form("Time distance between hits on same channel in between FEB "
                 "#%03u; Time difference [ns]; Channel []; ",
                 uFebIdx);
    fhStsFebChanDistT.push_back(
      new TH2I(sHistName,
               title,
               1000,
               -0.5,
               6250.0 - 0.5,
               fUnpackParMuch->GetNbChanPerFeb(),
               -0.5,
               fUnpackParMuch->GetNbChanPerFeb() - 0.5));

    /// Channels coincidences between FEBs
    fhStsFebChanDtCoinc[uFebIdx].resize(fuNbFebs, nullptr);
    fhStsFebChanCoinc[uFebIdx].resize(fuNbFebs, nullptr);
    for (UInt_t uFebIdxB = uFebIdx; uFebIdxB < fuNbFebs; ++uFebIdxB) {
      sHistName = Form("hStsFebChanDtCoinc_%03u_%03u", uFebIdx, uFebIdxB);
      title = Form("Channel coincidences Time diff between FEB #%03u and  FEB "
                   "#%03u; Time difference [ns]",
                   uFebIdx,
                   uFebIdxB);
      fhStsFebChanDtCoinc[uFebIdx][uFebIdxB] =
        new TH1I(sHistName, title, 400, -1250., 1250.);

      sHistName = Form("hStsFebChanCoinc_%03u_%03u", uFebIdx, uFebIdxB);
      title     = Form("Channel coincidences between FEB #%03u and  FEB #%03u; "
                   "Channel FEB #%03u []; Channel FEB #%03u []; Coinc. []",
                   uFebIdx,
                   uFebIdxB,
                   uFebIdx,
                   uFebIdxB);
      fhStsFebChanCoinc[uFebIdx][uFebIdxB] =
        new TH2I(sHistName,
                 title,
                 fUnpackParMuch->GetNbChanPerFeb(),
                 -0.5,
                 fUnpackParMuch->GetNbChanPerFeb() - 0.5,
                 fUnpackParMuch->GetNbChanPerFeb(),
                 -0.5,
                 fUnpackParMuch->GetNbChanPerFeb() - 0.5);
    }  // for( UInt_t uFebIdxB = uFebIdx; uFebIdxB < fuNbFebs; ++uFebIdxB )

  }  // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )

  ///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

  LOG(debug) << "Initialized 4th Histo before SXM 2.0 Logic Error";
  ///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
  ///----------------- SXM 2.0 Logic Error Tagging --------------------///
  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    /// Evo of ratio MS with SMX Error over all MS
    sHistName = Form("hStsFebSmxErrRatioEvo_%03u", uFebIdx);
    title     = Form("Proportion of uS with SMX logic error in FEB #%03u; Time "
                 "[s]; Error MS fract. []",
                 uFebIdx);
    fhStsFebSmxErrRatioEvo.push_back(
      new TProfile(sHistName, title, 1800, 0, 1800));

    /// Evo of ratio MS with SMX Error per ASIC
    sHistName = Form("hStsFebSmxErrRatioEvoAsic_%03u", uFebIdx);
    title = Form("Proportion of uS with SMX logic error per ASIC in FEB #%03u; "
                 "Time [s]; ASIC []; Error MS fract. []",
                 uFebIdx);
    fhStsFebSmxErrRatioEvoAsic.push_back(
      new TProfile2D(sHistName,
                     title,
                     1800,
                     0,
                     1800,
                     fUnpackParMuch->GetNbAsicsPerFeb(),
                     -0.5,
                     fUnpackParMuch->GetNbAsicsPerFeb() - 0.5));

    /// Evo of ratio MS with hit copies over all MS
    sHistName = Form("hStsFebSmxErrRatioCopyEvo_%03u", uFebIdx);
    title     = Form("Proportion of uS with hit copies in FEB #%03u; Time [s]; "
                 "Copies MS fract. []",
                 uFebIdx);
    fhStsFebSmxErrRatioCopyEvo.push_back(
      new TProfile(sHistName, title, 1800, 0, 1800));

    /// Evo of ratio MS with hit copies per ASIC
    sHistName = Form("hStsFebSmxErrRatioCopyEvoAsic_%03u", uFebIdx);
    title = Form("Proportion of uS with hit copies per ASIC in FEB #%03u; Time "
                 "[s]; ASIC []; Copies MS fract. []",
                 uFebIdx);
    fhStsFebSmxErrRatioCopyEvoAsic.push_back(
      new TProfile2D(sHistName,
                     title,
                     1800,
                     0,
                     1800,
                     fUnpackParMuch->GetNbAsicsPerFeb(),
                     -0.5,
                     fUnpackParMuch->GetNbAsicsPerFeb() - 0.5));

    /// Evo of ratio MS with hit copies over all MS
    sHistName = Form("hStsFebSmxErrRatioCopySameAdcEvo_%03u", uFebIdx);
    title     = Form("Proportion of uS with hit full copies in FEB #%03u; Time "
                 "[s]; Copies MS fract. []",
                 uFebIdx);
    fhStsFebSmxErrRatioCopySameAdcEvo.push_back(
      new TProfile(sHistName, title, 1800, 0, 1800));

    /// Evo of ratio MS with hit copies per ASIC
    sHistName = Form("hStsFebSmxErrRatioCopySameAdcEvoAsic_%03u", uFebIdx);
    title = Form("Proportion of uS with hit full copies per ASIC in FEB #%03u; "
                 "Time [s]; ASIC []; Copies MS fract. []",
                 uFebIdx);
    fhStsFebSmxErrRatioCopySameAdcEvoAsic.push_back(
      new TProfile2D(sHistName,
                     title,
                     1800,
                     0,
                     1800,
                     fUnpackParMuch->GetNbAsicsPerFeb(),
                     -0.5,
                     fUnpackParMuch->GetNbAsicsPerFeb() - 0.5));
  }  // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )
     ///------------------------------------------------------------------///
     ///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

  // Miscroslice properties histos
  for (Int_t component = 0; component < kiMaxNbFlibLinks; component++) {
    fhMsSz[component]     = NULL;
    fhMsSzTime[component] = NULL;
  }  // for( Int_t component = 0; component < kiMaxNbFlibLinks; component ++ )

  LOG(debug) << "Initialized 6th Histo before FairRunOnlne Instance";

  // Online histo browser commands
#ifdef USE_HTTP_SERVER
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
  if (server) {
    for (UInt_t uModuleId = 0; uModuleId < 2; ++uModuleId) {
      server->Register("/StsRaw", fHistPadDistr[uModuleId]);
      server->Register("/StsRaw", fRealHistPadDistr[uModuleId]);
    }

    server->Register("/StsRaw", fhRate);
    server->Register("/StsRaw", fhRateAdcCut);
    server->Register("/StsRaw", fhStsMessType);
    server->Register("/StsRaw", fhStsSysMessType);
    server->Register("/StsRaw", fhStsMessTypePerDpb);
    server->Register("/StsRaw", fhStsSysMessTypePerDpb);
    server->Register("/StsRaw", fhPulserStatusMessType);
    server->Register("/StsRaw", fhPulserMsStatusFieldType);
    server->Register("/StsRaw", fhStsHitsElinkPerDpb);
    server->Register("/StsRaw", fhStsFebChanAdcRaw_combined);


    for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
      if (kTRUE == fUnpackParMuch->IsFebActive(uFebIdx)) {
        server->Register("/StsFeb", fhStsFebChanCntRaw[uFebIdx]);
        server->Register("/StsFeb", fhStsFebChanCntRawGood[uFebIdx]);
        server->Register("/StsFeb", fhStsFebChanAdcRaw[uFebIdx]);
        server->Register("/StsFeb", fhStsFebChanAdcRawProf[uFebIdx]);
        server->Register("/StsFeb", fhStsFebChanAdcCal[uFebIdx]);
        server->Register("/StsFeb", fhStsFebChanAdcCalProf[uFebIdx]);
        server->Register("/StsFeb", fhStsFebChanRawTs[uFebIdx]);
        server->Register("/StsFeb", fhStsFebChanMissEvt[uFebIdx]);
        server->Register("/StsFeb", fhStsFebChanMissEvtEvo[uFebIdx]);
        server->Register("/StsFeb", fhStsFebAsicMissEvtEvo[uFebIdx]);
        server->Register("/StsFeb", fhStsFebMissEvtEvo[uFebIdx]);
        server->Register("/StsFeb", fhStsFebChanHitRateEvo[uFebIdx]);
        server->Register("/StsFeb", fhStsFebChanHitRateProf[uFebIdx]);
        server->Register("/StsFeb", fhStsFebAsicHitRateEvo[uFebIdx]);
        server->Register("/StsFeb", fhStsFebHitRateEvo[uFebIdx]);
        server->Register("/StsFeb", fhStsFebHitRateEvo_mskch[uFebIdx]);
        server->Register("/StsFeb", fhStsFebHitRateEvo_mskch_adccut[uFebIdx]);
        server->Register("/StsFeb", fhStsFebChanHitRateEvoLong[uFebIdx]);
        server->Register("/StsFeb", fhStsFebAsicHitRateEvoLong[uFebIdx]);
        server->Register("/StsFeb", fhStsFebHitRateEvoLong[uFebIdx]);
        server->Register("/StsFeb", fhStsFebChanDistT[uFebIdx]);
        for (UInt_t uFebIdxB = uFebIdx; uFebIdxB < fuNbFebs; ++uFebIdxB) {
          server->Register("/StsFeb", fhStsFebChanDtCoinc[uFebIdx][uFebIdxB]);
          server->Register("/StsFeb", fhStsFebChanCoinc[uFebIdx][uFebIdxB]);
        }  // for( UInt_t uFebIdxB = uFebIdx; uFebIdxB < fuNbFebs; ++uFebIdxB )

        server->Register("/StsSmxErr", fhStsFebSmxErrRatioEvo[uFebIdx]);
        server->Register("/StsSmxErr", fhStsFebSmxErrRatioEvoAsic[uFebIdx]);
        server->Register("/StsSmxErr", fhStsFebSmxErrRatioCopyEvo[uFebIdx]);
        server->Register("/StsSmxErr", fhStsFebSmxErrRatioCopyEvoAsic[uFebIdx]);
        server->Register("/StsSmxErr",
                         fhStsFebSmxErrRatioCopySameAdcEvo[uFebIdx]);
        server->Register("/StsSmxErr",
                         fhStsFebSmxErrRatioCopySameAdcEvoAsic[uFebIdx]);
      }  // if( kTRUE == fUnpackParMuch->IsFebActive( uFebIdx ) )
    }    // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )

    server->RegisterCommand("/Reset_All", "bMcbm2018ResetMuch=kTRUE");
    server->RegisterCommand("/Write_All", "bMcbm2018WriteMuch=kTRUE");
    server->RegisterCommand("/ScanNoisyCh", "bMcbm2018ScanNoisyMuch=kTRUE");


    server->Restrict("/Reset_All", "allow=admin");
    server->Restrict("/Write_All", "allow=admin");
    server->Restrict("/ScanNoisyCh", "allow=admin");
  }  // if( server )
#endif

  /** Create summary Canvases for CERN 2017 **/
  Double_t w = 10;
  Double_t h = 10;
  LOG(debug) << "Initialized 7th Histo before Summary per FEB";
  // Summary per FEB
  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    if (kTRUE == fUnpackParMuch->IsFebActive(uFebIdx)) {
      TCanvas* cStsSumm =
        new TCanvas(Form("cStsSum_%03u", uFebIdx),
                    Form("Summary plots for FEB %03u", uFebIdx),
                    w,
                    h);
      cStsSumm->Divide(2, 3);

      cStsSumm->cd(1);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fhStsFebChanCntRaw[uFebIdx]->Draw();

      cStsSumm->cd(2);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fhStsFebChanHitRateProf[uFebIdx]->Draw("e0");

      cStsSumm->cd(3);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhStsFebChanAdcRaw[uFebIdx]->Draw("colz");

      cStsSumm->cd(4);
      gPad->SetGridx();
      gPad->SetGridy();
      //      gPad->SetLogy();
      fhStsFebChanAdcRawProf[uFebIdx]->Draw();

      cStsSumm->cd(5);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhStsFebChanAdcCal[uFebIdx]->Draw("colz");

      cStsSumm->cd(6);
      gPad->SetGridx();
      gPad->SetGridy();
      //      gPad->SetLogy();
      fhStsFebChanAdcCalProf[uFebIdx]->Draw();


      TCanvas* cStsSmxErr =
        new TCanvas(Form("cStsSmxErr_%03u", uFebIdx),
                    Form("SMX logic error plots for FEB %03u", uFebIdx),
                    w,
                    h);
      cStsSmxErr->Divide(2, 3);

      cStsSmxErr->cd(1);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fhStsFebSmxErrRatioEvo[uFebIdx]->Draw();

      cStsSmxErr->cd(2);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhStsFebSmxErrRatioEvoAsic[uFebIdx]->Draw("colz");

      cStsSmxErr->cd(3);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fhStsFebSmxErrRatioCopyEvo[uFebIdx]->Draw();

      cStsSmxErr->cd(4);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhStsFebSmxErrRatioCopyEvoAsic[uFebIdx]->Draw("colz");

      cStsSmxErr->cd(5);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fhStsFebSmxErrRatioCopySameAdcEvo[uFebIdx]->Draw();

      cStsSmxErr->cd(6);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhStsFebSmxErrRatioCopySameAdcEvoAsic[uFebIdx]->Draw("colz");
    }  // if( kTRUE == fUnpackParMuch->IsFebActive( uFebIdx ) )
  }    // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )

  //====================================================================//
  LOG(debug) << "Initialized Last Histo before exiting CreateHistograms";
  //====================================================================//
  /** Recovers/Create Ms Size Canvase for CERN 2016 **/
  // Try to recover canvas in case it was created already by another monitor
  // If not existing, create it
  fcMsSizeAll = dynamic_cast<TCanvas*>(gROOT->FindObject("cMsSizeAll"));
  if (NULL == fcMsSizeAll) {
    fcMsSizeAll =
      new TCanvas("cMsSizeAll", "Evolution of MS size in last 300 s", w, h);
    fcMsSizeAll->Divide(4, 4);
    LOG(info) << "Created MS size canvas in STS monitor";
  }  // if( NULL == fcMsSizeAll )
  else
    LOG(info) << "Recovered MS size canvas in STS monitor";
  //====================================================================//

  /*****************************/
}

Bool_t CbmMcbm2018MonitorMuch::DoUnpack(const fles::Timeslice& ts,
                                        size_t component) {
  if (bMcbm2018ResetMuch) {
    ResetAllHistos();
    bMcbm2018ResetMuch = kFALSE;
  }  // if( bMcbm2018ResetMuch )
  if (bMcbm2018WriteMuch) {
    SaveAllHistos(fsHistoFileFullname);
    bMcbm2018WriteMuch = kFALSE;
  }  // if( bMcbm2018WriteMuch )
  if (bMcbm2018ScanNoisyMuch) {
    ScanForNoisyChannels();
    bMcbm2018ScanNoisyMuch = kFALSE;
  }  // if( bMcbm2018WriteMuch )

  LOG(debug) << "Timeslice contains " << ts.num_microslices(component)
             << " microslices.";
  fulCurrentTsIdx = ts.index();

  // Ignore overlap ms if flag set by user
  UInt_t uNbMsLoop = fuNbCoreMsPerTs;
  if (kFALSE == fbIgnoreOverlapMs) uNbMsLoop += fuNbOverMsPerTs;

  //LOG(info) <<" uNbMsLoop "<<uNbMsLoop;
  // Loop over core microslices (and overlap ones if chosen)
  for (UInt_t uMsIdx = 0; uMsIdx < uNbMsLoop; uMsIdx++) {
    Double_t dMsTime =
      (1e-9)
      * static_cast<double>(ts.descriptor(fvMsComponentsList[0], uMsIdx).idx);

    // Loop over registered components
    for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size();
         ++uMsCompIdx) {
      UInt_t uMsComp = fvMsComponentsList[uMsCompIdx];

      if (kFALSE == ProcessStsMs(ts, uMsComp, uMsIdx)) return kFALSE;

    }  // for( UInt_t uMsComp = 0; uMsComp < fvMsComponentsList.size(); ++uMsComp )

    /// Pulses time difference calculation and plotting
    // Sort the buffer of hits
    std::sort(fvmHitsInMs.begin(), fvmHitsInMs.end());

    // Time differences plotting using the fully time sorted hits
    if (0 < fvmHitsInMs.size()) {
      //         ULong64_t ulLastHitTime = ( *( fvmHitsInMs.rbegin() ) ).GetTs();
      std::vector<stsxyter::FinalHit>::iterator itA;
      std::vector<stsxyter::FinalHit>::iterator itB;

      //         std::chrono::steady_clock::time_point tNow = std::chrono::steady_clock::now();
      //         Double_t dUnixTimeInRun = std::chrono::duration_cast< std::chrono::seconds >(tNow - ftStartTimeUnix).count();
      //LOG(info) <<" ulLastHitTime "<<ulLastHitTime<<" dUnixTimeInRun "<<dUnixTimeInRun;
      for (
        itA = fvmHitsInMs.begin(); itA != fvmHitsInMs.end();
        //              itA != fvmHitsInMs.end() && (*itA).GetTs() < ulLastHitTime - 320; // 320 * 3.125 ns = 1000 ns
        ++itA) {
        UShort_t usAsicIdx = (*itA).GetAsic();
        //            UShort_t  usChanIdx = (*itA).GetChan();
        //            ULong64_t ulHitTs   = (*itA).GetTs();
        //            UShort_t  usHitAdc  = (*itA).GetAdc();
        UShort_t usFebIdx = usAsicIdx / fUnpackParMuch->GetNbAsicsPerFeb();
        //            UShort_t  usAsicInFeb = usAsicIdx % fUnpackParMuch->GetNbAsicsPerFeb();
        //LOG(info) <<" usAsicIdx "<<usAsicIdx<<" usChanIdx "<<usChanIdx<<" ulHitTs "<<ulHitTs<<" usHitAdc "<<usHitAdc<<" usFebIdx "<<usFebIdx<<" usAsicInFeb "<<usAsicInFeb;
        //            Double_t dTimeSinceStartSec = (ulHitTs * stsxyter::kdClockCycleNs - fdStartTime)* 1e-9;
        //LOG(info) <<" dTimeSinceStartSec "<<dTimeSinceStartSec;
        fvmAsicHitsInMs[usAsicIdx].push_back((*itA));
        fvmFebHitsInMs[usFebIdx].push_back((*itA));
        /*
            if( 1000 == fulCurrentTsIdx )
            {
               LOG(info) << Form( "FEB %02u ASIC %u Chan %03u TS %12u ADC %2u Time %8.3f",
                                  usFebIdx, usAsicInFeb, usChanIdx, ulHitTs, usHitAdc,
                                  ulHitTs* stsxyter::kdClockCycleNs );
            } // if( 0 == fulCurrentTsIdx )
*/
      }  // loop on time sorted hits and split per asic/feb

      // Remove all hits which were already used
      fvmHitsInMs.erase(fvmHitsInMs.begin(), itA);

      ///----------------- SXM 2.0 Logic Error Tagging --------------------///
      Bool_t bHitCopyInThisMs[fuNbStsXyters];
      Bool_t bHitCopySameAdcInThisMs[fuNbStsXyters];
      Bool_t bFlagOnInThisMs[fuNbStsXyters];
      for (UInt_t uAsic = 0; uAsic < fuNbStsXyters; uAsic++) {
        bHitCopyInThisMs[uAsic]        = kFALSE;
        bHitCopySameAdcInThisMs[uAsic] = kFALSE;
        bFlagOnInThisMs[uAsic]         = kFALSE;
      }  // for( UInt_t uAsic = 0; uAsic < fuNbStsXyters; uAsic++)
      ///------------------------------------------------------------------///

      for (UInt_t uAsic = 0; uAsic < fuNbStsXyters; uAsic++) {
        UInt_t uFebIdx    = uAsic / fUnpackParMuch->GetNbAsicsPerFeb();
        UInt_t uAsicInFeb = uAsic % fUnpackParMuch->GetNbAsicsPerFeb();

        std::vector<ULong64_t> vulLastHitTs(fUnpackParMuch->GetNbChanPerAsic(),
                                            0);
        std::vector<UShort_t> vusLastHitAdc(fUnpackParMuch->GetNbChanPerAsic(),
                                            0);

        for (itA = fvmAsicHitsInMs[uAsic].begin();
             itA != fvmAsicHitsInMs[uAsic].end();
             ++itA) {
          //               UShort_t usAsicIdx = (*itA).GetAsic();
          UShort_t usChanIdx = (*itA).GetChan();
          ULong64_t ulHitTs  = (*itA).GetTs();
          UShort_t usHitAdc  = (*itA).GetAdc();

          UInt_t uChanInFeb =
            usChanIdx + fUnpackParMuch->GetNbChanPerAsic() * uAsicInFeb;

          ///----------------- SXM 2.0 Logic Error Tagging --------------------///
          /// Check if hit copy
          Bool_t bIsNotCopy = kTRUE;
          if (vulLastHitTs[usChanIdx] == ulHitTs) {
            bIsNotCopy              = kFALSE;
            bHitCopyInThisMs[uAsic] = kTRUE;
            if (vusLastHitAdc[usChanIdx] == usHitAdc)
              bHitCopySameAdcInThisMs[uAsic] = kTRUE;
          }  // if( vulLastHitTs[ usChanIdx ] == ulHitTs)

          vulLastHitTs[usChanIdx]  = ulHitTs;
          vusLastHitAdc[usChanIdx] = usHitAdc;

          if (bIsNotCopy) {
            fhStsFebChanCntRawGood[uFebIdx]->Fill(uChanInFeb);
            bFlagOnInThisMs[uAsic] |= SmxErrCheckCoinc(
              uFebIdx, uAsicInFeb, ulHitTs * stsxyter::kdClockCycleNs);
          }  // if( bIsNotCopy )
          ///------------------------------------------------------------------///
        }  // for( it  = fvmAsicHitsInMs[ uAsic ].begin(); it != fvmAsicHitsInMs[ uAsic ].end(); ++it )

        /// Data in vector are not needed anymore as all possible matches are already checked
        fvmAsicHitsInMs[uAsic].clear();
      }  // for( UInt_t uAsic = 0; uAsic < fuNbStsXyters; uAsic++)

      ///----------------- SXM 2.0 Logic Error Tagging --------------------///
      std::vector<Bool_t> vbCopyOnAnyAsicMs(fuNbFebs, kFALSE);
      std::vector<Bool_t> vbCopySameAdcOnAnyAsicMs(fuNbFebs, kFALSE);
      std::vector<Bool_t> vbFlagOnAnyAsicMs(fuNbFebs, kFALSE);
      Bool_t bCopyOnAnyMs        = kFALSE;
      Bool_t bCopySameAdcOnAnyMs = kFALSE;
      Bool_t bFlagOnAnyMs        = kFALSE;
      for (UInt_t uAsic = 0; uAsic < fuNbStsXyters; uAsic++) {
        UInt_t uFebIdx    = uAsic / fUnpackParMuch->GetNbAsicsPerFeb();
        UInt_t uAsicInFeb = uAsic % fUnpackParMuch->GetNbAsicsPerFeb();

        fhStsFebSmxErrRatioCopyEvoAsic[uFebIdx]->Fill(
          dMsTime - fdStartTimeMsSz,
          uAsicInFeb,
          bHitCopyInThisMs[uAsic] ? 1.0 : 0.0);

        fhStsFebSmxErrRatioCopySameAdcEvoAsic[uFebIdx]->Fill(
          dMsTime - fdStartTimeMsSz,
          uAsicInFeb,
          bHitCopySameAdcInThisMs[uAsic] ? 1.0 : 0.0);

        fhStsFebSmxErrRatioEvoAsic[uFebIdx]->Fill(dMsTime - fdStartTimeMsSz,
                                                  uAsicInFeb,
                                                  bFlagOnInThisMs[uAsic] ? 1.0
                                                                         : 0.0);

        vbCopyOnAnyAsicMs[uFebIdx] =
          vbCopyOnAnyAsicMs[uFebIdx] || bHitCopyInThisMs[uAsic];
        vbCopySameAdcOnAnyAsicMs[uFebIdx] =
          vbCopySameAdcOnAnyAsicMs[uFebIdx] || bHitCopySameAdcInThisMs[uAsic];
        vbFlagOnAnyAsicMs[uFebIdx] =
          vbFlagOnAnyAsicMs[uFebIdx] || bFlagOnInThisMs[uAsic];

        bCopyOnAnyMs |= bHitCopyInThisMs[uAsic];
        bCopySameAdcOnAnyMs |= bHitCopySameAdcInThisMs[uAsic];
        bFlagOnAnyMs |= bFlagOnInThisMs[uAsic];
      }  // for( UInt_t uAsic = 0; uAsic < fuNbStsXyters; uAsic++)

      for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
        fhStsFebSmxErrRatioCopyEvo[uFebIdx]->Fill(
          dMsTime - fdStartTimeMsSz, vbCopyOnAnyAsicMs[uFebIdx] ? 1.0 : 0.0);
        fhStsFebSmxErrRatioCopySameAdcEvo[uFebIdx]->Fill(
          dMsTime - fdStartTimeMsSz,
          vbCopySameAdcOnAnyAsicMs[uFebIdx] ? 1.0 : 0.0);
        fhStsFebSmxErrRatioEvo[uFebIdx]->Fill(
          dMsTime - fdStartTimeMsSz, vbFlagOnAnyAsicMs[uFebIdx] ? 1.0 : 0.0);
      }  // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )
      ///------------------------------------------------------------------///

      for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
        if (kTRUE == fbEnableCoincidenceMaps) {
          //               UInt_t uFebA     =   uFebIdx  % fUnpackParMuch->GetNbFebsPerCrob();
          //               UInt_t uCrobIdxA = ( uFebIdx  / fUnpackParMuch->GetNbFebsPerCrob() ) % fUnpackParMuch->GetNbCrobsPerDpb();
          //               UInt_t uDpbIdxA  = ( uFebIdx  / fUnpackParMuch->GetNbFebsPerCrob() ) / fUnpackParMuch->GetNbCrobsPerDpb();

          for (itA = fvmFebHitsInMs[uFebIdx].begin();
               itA != fvmFebHitsInMs[uFebIdx].end();
               ++itA) {
            UShort_t usAsicIdxA = (*itA).GetAsic();
            UShort_t usAsicInFebA =
              usAsicIdxA % fUnpackParMuch->GetNbAsicsPerFeb();
            UShort_t usChanIdxA = (*itA).GetChan();
            UInt_t uChanInFebA =
              usChanIdxA + fUnpackParMuch->GetNbChanPerAsic() * usAsicInFebA;
            ULong64_t ulHitTsA = (*itA).GetTs();
            Double_t dHitTsA   = ulHitTsA * stsxyter::kdClockCycleNs;

            /// Time distance between hits on same channel
            if (-1 < fdStsFebChanLastTimeForDist[uFebIdx][uChanInFebA]) {
              fhStsFebChanDistT[uFebIdx]->Fill(
                dHitTsA - fdStsFebChanLastTimeForDist[uFebIdx][uChanInFebA],
                uChanInFebA);
            }  // if( -1 < fdStsFebChanLastTimeForDist[ uFebIdx ][ uChanInFebA ] )
            fdStsFebChanLastTimeForDist[uFebIdx][uChanInFebA] = dHitTsA;

            for (UInt_t uFebIdxB = uFebIdx; uFebIdxB < fuNbFebs; ++uFebIdxB) {
              /// Module P-N coincidences
              //Bool_t bSameModulePNsides = kFALSE;
              //                     Double_t dBestDtMatch     = 1e9; // ns
              // comment unused variable, FU, 18.01.21              std::vector<stsxyter::FinalHit>::iterator itBestMatch;
              //                     UInt_t uFebB     =   uFebIdxB % fUnpackParMuch->GetNbFebsPerCrob();
              //                     UInt_t uCrobIdxB = ( uFebIdxB / fUnpackParMuch->GetNbFebsPerCrob() ) % fUnpackParMuch->GetNbCrobsPerDpb();
              //                     UInt_t uDpbIdxB  = ( uFebIdxB / fUnpackParMuch->GetNbFebsPerCrob() ) / fUnpackParMuch->GetNbCrobsPerDpb();
              /*if( fviFebModuleIdx[ uDpbIdxA ][ uCrobIdxA ][ uFebA ]  == fviFebModuleIdx[ uDpbIdxB ][ uCrobIdxB ][ uFebB ] &&
                         fviFebModuleSide[ uDpbIdxA ][ uCrobIdxA ][ uFebA ] != fviFebModuleSide[ uDpbIdxB ][ uCrobIdxB ][ uFebB ] )
                         bSameModulePNsides = kTRUE;
					*/
              for (itB = fvmFebHitsInMs[uFebIdxB].begin();
                   itB != fvmFebHitsInMs[uFebIdxB].end();
                   ++itB) {
                UShort_t usAsicIdxB = (*itB).GetAsic();
                UShort_t usChanIdxB = (*itB).GetChan();
                UInt_t uChanInFebB =
                  usChanIdxB
                  + fUnpackParMuch->GetNbChanPerAsic()
                      * (usAsicIdxB % fUnpackParMuch->GetNbAsicsPerFeb());

                if (uFebIdx == uFebIdxB && uChanInFebA == uChanInFebB) continue;

                ULong64_t ulHitTsB = (*itB).GetTs();
                //                        Double_t  dHitTsB     = ulHitTsB * stsxyter::kdClockCycleNs;
                Double_t dDtClk = static_cast<Double_t>(ulHitTsB)
                                  - static_cast<Double_t>(ulHitTsA);
                Double_t dDt = dDtClk * stsxyter::kdClockCycleNs;

                fhStsFebChanDtCoinc[uFebIdx][uFebIdxB]->Fill(dDt);

                /// Check if we have a channel coincidence
                if (-1.0 * fdFebChanCoincidenceLimit < dDt) {
                  /// If out of coincidence, later hits will also be out => break the HitB loop
                  if (fdFebChanCoincidenceLimit < dDt) break;

                  fhStsFebChanCoinc[uFebIdx][uFebIdxB]->Fill(uChanInFebA,
                                                             uChanInFebB);

                }  // if( -1.0 * fdFebChanCoincidenceLimit < dDt )
              }  // for( itB  = fvmFebHitsInMs[ uFebIdxB ].begin(); itB != fvmFebHitsInMs[ uFebIdxB ].end(); ++itB )
            }  // for( UInt_t uFebIdxB = uFebIdx; uFebIdxB < fuNbFebs; ++uFebIdxB )
          }  // for( itA  = fvmFebHitsInMs[ uFebIdx ].begin(); itA != fvmFebHitsInMs[ uFebIdx ].end(); ++itA )
        }    // if( kTRUE == fbEnableCoincidenceMaps )

        /// Data in vector are not needed anymore as all possible matches are already checked
        fvmFebHitsInMs[uFebIdx].clear();
      }  // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )
    }    // if( 0 < fvmHitsInMs.size() )
  }      // for( UInt_t uMsIdx = 0; uMsIdx < uNbMsLoop; uMsIdx ++ )

  for (UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx) {
    fvdMsTime[uMsIdx] = 0.0;
  }  // for( UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx )

  if (0 == ts.index() % 1000) {
    for (UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb) {
      Double_t dTsMsbTime =
        static_cast<ULong64_t>(stsxyter::kuHitNbTsBins)
          * static_cast<ULong64_t>(fvulCurrentTsMsb[fuCurrDpbIdx])
        + static_cast<ULong64_t>(stsxyter::kulTsCycleNbBins)
            * static_cast<ULong64_t>(fvuCurrentTsMsbCycle[fuCurrDpbIdx]);
      dTsMsbTime *= stsxyter::kdClockCycleNs * 1e-9;

      LOG(info) << "End of TS " << std::setw(7) << ts.index() << " eDPB "
                << std::setw(2) << uDpb << " current TS MSB counter is "
                << std::setw(12) << fvulCurrentTsMsb[uDpb]
                << " current TS MSB cycle counter is " << std::setw(12)
                << fvuCurrentTsMsbCycle[uDpb] << " current TS MSB time is "
                << std::setw(12) << dTsMsbTime << " s";
    }
  }  // if( 0 == ts.index() % 1000 )

  //   if( 0 == ts.index() % 10000 )
  //SaveAllHistos( "data/PulserPeriodicHistosSave.root");

  return kTRUE;
}

Bool_t CbmMcbm2018MonitorMuch::ProcessStsMs(const fles::Timeslice& ts,
                                            size_t uMsComp,
                                            UInt_t uMsIdx) {
  auto msDescriptor    = ts.descriptor(uMsComp, uMsIdx);
  fuCurrentEquipmentId = msDescriptor.eq_id;
  //LOG(info) << " fuCurrentEquipmentId "<<fuCurrentEquipmentId;
  const uint8_t* msContent =
    reinterpret_cast<const uint8_t*>(ts.content(uMsComp, uMsIdx));

  uint32_t uSize   = msDescriptor.size;
  fulCurrentMsIdx  = msDescriptor.idx;
  Double_t dMsTime = (1e-9) * static_cast<double>(fulCurrentMsIdx);
  LOG(debug) << "Microslice: " << fulCurrentMsIdx << " from EqId " << std::hex
             << fuCurrentEquipmentId << std::dec << " has size: " << uSize;

  fuCurrDpbId  = static_cast<uint32_t>(fuCurrentEquipmentId & 0xFFFF);
  fuCurrDpbIdx = fDpbIdIndexMap[fuCurrDpbId];
  //LOG(info) <<" fuCurrDpbIdx "<<fuCurrDpbIdx<<" fuCurrDpbId "<<fuCurrDpbId;

  if (uMsComp < kiMaxNbFlibLinks) {
    if (fdStartTimeMsSz < 0) fdStartTimeMsSz = dMsTime;
    fhMsSz[uMsComp]->Fill(uSize);
    fhMsSzTime[uMsComp]->Fill(dMsTime - fdStartTimeMsSz, uSize);
  }  // if( uMsComp < kiMaxNbFlibLinks )

  /// Plots in [X/s] update
  if (static_cast<Int_t>(fvdPrevMsTime[uMsComp])
      < static_cast<Int_t>(dMsTime)) {
    /// "new second"
    UInt_t uFebIdxOffset = fUnpackParMuch->GetNbFebsPerDpb() * fuCurrDpbIdx;
    for (UInt_t uFebIdx = 0; uFebIdx < fUnpackParMuch->GetNbFebsPerDpb();
         ++uFebIdx) {
      UInt_t uFebIdxInSyst = uFebIdxOffset + uFebIdx;

      /// Ignore first interval is not clue how late the data taking was started
      if (0 == fviFebTimeSecLastRateUpdate[uFebIdxInSyst]) {
        fviFebTimeSecLastRateUpdate[uFebIdxInSyst] =
          static_cast<Int_t>(dMsTime);
        fviFebCountsSinceLastRateUpdate[uFebIdxInSyst] = 0;
        for (UInt_t uChan = 0; uChan < fUnpackParMuch->GetNbChanPerFeb();
             ++uChan)
          fvdFebChanCountsSinceLastRateUpdate[uFebIdxInSyst][uChan] = 0.0;
        continue;
      }  // if( 0 == fviFebTimeSecLastRateUpdate[uFebIdxInSyst] )

      Int_t iTimeInt = static_cast<Int_t>(dMsTime)
                       - fviFebTimeSecLastRateUpdate[uFebIdxInSyst];
      if (fiTimeIntervalRateUpdate <= iTimeInt) {
        /// Jump empty FEBs without looping over channels
        if (0 == fviFebCountsSinceLastRateUpdate[uFebIdxInSyst]) {
          fviFebTimeSecLastRateUpdate[uFebIdxInSyst] =
            static_cast<Int_t>(dMsTime);
          continue;
        }  // if( 0 == fviFebCountsSinceLastRateUpdate[uFebIdxInSyst] )

        for (UInt_t uChan = 0; uChan < fUnpackParMuch->GetNbChanPerFeb();
             ++uChan) {
          fhStsFebChanHitRateProf[uFebIdxInSyst]->Fill(
            uChan,
            fvdFebChanCountsSinceLastRateUpdate[uFebIdxInSyst][uChan]
              / iTimeInt);
          fvdFebChanCountsSinceLastRateUpdate[uFebIdxInSyst][uChan] = 0.0;
        }  // for( UInt_t uChan = 0; uChan < fUnpackParMuch->GetNbChanPerFeb(); ++uChan )

        fviFebTimeSecLastRateUpdate[uFebIdxInSyst] =
          static_cast<Int_t>(dMsTime);
        fviFebCountsSinceLastRateUpdate[uFebIdxInSyst] = 0;
      }  // if( fiTimeIntervalRateUpdate <= iTimeInt )
    }  // for( UInt_t uFebIdx = 0; uFebIdx < fUnpackParMuch->GetNbFebsPerDpb(); ++uFebIdx )
  }  // if( static_cast<Int_t>( fvdMsTime[ uMsCompIdx ] ) < static_cast<Int_t>( dMsTime )  )

  // Store MS time for coincidence plots
  fvdPrevMsTime[uMsComp] = dMsTime;

  /// Check Flags field of MS header
  uint16_t uMsHeaderFlags = msDescriptor.flags;
  for (UInt_t uBit = 0; uBit < 16; ++uBit)
    fhPulserMsStatusFieldType->Fill(uBit, (uMsHeaderFlags >> uBit) & 0x1);

  /** Check the current TS_MSb cycle and correct it if wrong **/
  UInt_t uTsMsbCycleHeader =
    std::floor(fulCurrentMsIdx
               / (stsxyter::kulTsCycleNbBins * stsxyter::kdClockCycleNs))
    - fvuInitialTsMsbCycleHeader[fuCurrDpbIdx];
  if (kFALSE == fvuInitialHeaderDone[fuCurrDpbIdx]) {
    fvuInitialTsMsbCycleHeader[fuCurrDpbIdx] = uTsMsbCycleHeader;
    fvuInitialHeaderDone[fuCurrDpbIdx]       = kTRUE;
  }  // if( kFALSE == fvuInitialHeaderDone[ fuCurrDpbIdx ] )
  else if (uTsMsbCycleHeader != fvuCurrentTsMsbCycle[fuCurrDpbIdx]
           && 4194303 != fvulCurrentTsMsb[fuCurrDpbIdx]) {
    /*
      LOG(warning) << "TS MSB cycle from MS header does not match current cycle from data "
                    << "for TS " << std::setw( 12 ) << fulCurrentTsIdx
                    << " MS " << std::setw( 12 ) << fulCurrentMsIdx
                    << " MsInTs " << std::setw( 3 ) << uMsIdx
                    << " ====> " << fvuCurrentTsMsbCycle[ fuCurrDpbIdx ]
                    << " VS " << uTsMsbCycleHeader;
*/
    fvuCurrentTsMsbCycle[fuCurrDpbIdx] = uTsMsbCycleHeader;
  }

  // If not integer number of message in input buffer, print warning/error
  if (0 != (uSize % kuBytesPerMessage))
    LOG(error) << "The input microslice buffer does NOT "
               << "contain only complete nDPB messages!";

  // Compute the number of complete messages in the input microslice buffer
  uint32_t uNbMessages =
    (uSize - (uSize % kuBytesPerMessage)) / kuBytesPerMessage;

  // Prepare variables for the loop on contents
  const uint32_t* pInBuff = reinterpret_cast<const uint32_t*>(msContent);

  for (uint32_t uIdx = 0; uIdx < uNbMessages; ++uIdx) {
    // Fill message
    uint32_t ulData = static_cast<uint32_t>(pInBuff[uIdx]);

    stsxyter::Message mess(static_cast<uint32_t>(ulData & 0xFFFFFFFF));

    // Print message if requested
    if (fbPrintMessages) mess.PrintMess(std::cout, fPrintMessCtrl);
    /*
      if( 1000 == fulCurrentTsIdx )
      {
         mess.PrintMess( std::cout, fPrintMessCtrl );
      } // if( 0 == fulCurrentTsIdx )
*/
    stsxyter::MessType typeMess = mess.GetMessType();
    fmMsgCounter[typeMess]++;
    fhStsMessType->Fill(static_cast<uint16_t>(typeMess));
    fhStsMessTypePerDpb->Fill(fuCurrDpbIdx, static_cast<uint16_t>(typeMess));

    switch (typeMess) {
      case stsxyter::MessType::Hit: {
        // Extract the eLink and Asic indices => Should GO IN the fill method now that obly hits are link/asic specific!
        UShort_t usElinkIdx = mess.GetLinkIndex();
        UInt_t uCrobIdx     = usElinkIdx / fUnpackParMuch->GetNbElinkPerCrob();
        Int_t uFebIdx       = fUnpackParMuch->ElinkIdxToFebIdx(usElinkIdx);
        //            if(usElinkIdx!=0)
        //LOG(info) <<" usElinkIdx "<<usElinkIdx<<" uCrobIdx "<<uCrobIdx<<" uFebIdx "<<uFebIdx;
        if (kTRUE == fbMuchMode) uFebIdx = usElinkIdx;
        fhStsHitsElinkPerDpb->Fill(fuCurrDpbIdx, usElinkIdx);
        if (-1 == uFebIdx) {
          LOG(warning) << "CbmMcbm2018MonitorMuch::DoUnpack => "
                       << "Wrong elink Idx! Elink raw "
                       << Form("%d remap %d", usElinkIdx, uFebIdx);
          continue;
        }  // if( -1 == uFebIdx )
        //LOG(info) << " uCrobIdx "<<uCrobIdx<<" fUnpackParMuch->ElinkIdxToAsicIdx( usElinkIdx ) "<<fUnpackParMuch->ElinkIdxToAsicIdx( usElinkIdx )<<" usElinkIdx "<<usElinkIdx;
        UInt_t uAsicIdx =
          (fuCurrDpbIdx * fUnpackParMuch->GetNbCrobsPerDpb() + uCrobIdx)
            * fUnpackParMuch->GetNbAsicsPerCrob()
          + fUnpackParMuch->ElinkIdxToAsicIdx(usElinkIdx);

        FillHitInfo(mess, usElinkIdx, uAsicIdx, uMsIdx);
        break;
      }  // case stsxyter::MessType::Hit :
      case stsxyter::MessType::TsMsb: {
        FillTsMsbInfo(mess, uIdx, uMsIdx);
        break;
      }  // case stsxyter::MessType::TsMsb :
      case stsxyter::MessType::Epoch: {
        // The first message in the TS is a special ones: EPOCH
        FillEpochInfo(mess);

        if (0 < uIdx)
          LOG(info) << "CbmMcbm2018MonitorMuch::DoUnpack => "
                    << "EPOCH message at unexpected position in MS: message "
                    << uIdx << " VS message 0 expected!";
        break;
      }  // case stsxyter::MessType::TsMsb :
      case stsxyter::MessType::Status: {
        UShort_t usElinkIdx = mess.GetStatusLink();
        UInt_t uCrobIdx     = usElinkIdx / fUnpackParMuch->GetNbElinkPerCrob();
        //            Int_t   uFebIdx    = fUnpackParMuch->ElinkIdxToFebIdx( usElinkIdx );
        UInt_t uAsicIdx =
          (fuCurrDpbIdx * fUnpackParMuch->GetNbCrobsPerDpb() + uCrobIdx)
            * fUnpackParMuch->GetNbAsicsPerCrob()
          + fUnpackParMuch->ElinkIdxToAsicIdx(usElinkIdx);

        UShort_t usStatusField = mess.GetStatusStatus();

        fhPulserStatusMessType->Fill(uAsicIdx, usStatusField);
        /// Always print status messages... or not?
        if (fbPrintMessages) {
          std::cout << Form("DPB %2u TS %12llu MS %12llu mess %5u ",
                            fuCurrDpbIdx,
                            fulCurrentTsIdx,
                            fulCurrentMsIdx,
                            uIdx);
          mess.PrintMess(std::cout, fPrintMessCtrl);
        }  // if( fbPrintMessages )
           //                   FillTsMsbInfo( mess );
        break;
      }  // case stsxyter::MessType::Status
      case stsxyter::MessType::Empty: {
        //                   FillTsMsbInfo( mess );
        break;
      }  // case stsxyter::MessType::Empty :
      case stsxyter::MessType::Dummy: {
        break;
      }  // case stsxyter::MessType::Dummy / ReadDataAck / Ack :
      default: {
        LOG(fatal) << "CbmMcbm2018MonitorMuch::DoUnpack => "
                   << "Unknown message type, should never happen, stopping "
                      "here! Type found was: "
                   << static_cast<int>(typeMess);
      }
    }  // switch( mess.GetMessType() )
  }    // for( uint32_t uIdx = 0; uIdx < uNbMessages; ++uIdx )

  return kTRUE;
}

void CbmMcbm2018MonitorMuch::FillHitInfo(stsxyter::Message mess,
                                         const UShort_t& usElinkIdx,
                                         const UInt_t& uAsicIdx,
                                         const UInt_t& uMsIdx) {
  UShort_t usChan   = mess.GetHitChannel();
  UShort_t usRawAdc = mess.GetHitAdc();
  //   UShort_t usFullTs = mess.GetHitTimeFull();
  //   UShort_t usTsOver = mess.GetHitTimeOver();
  UShort_t usRawTs = mess.GetHitTime();

  /// Cheat needed only for modules with FEB at bottom of module or the Test module
  //   usChan = 127 - usChan;

  /*
   fhStsChanCntRaw[  uAsicIdx ]->Fill( usChan );
   fhStsChanAdcRaw[  uAsicIdx ]->Fill( usChan, usRawAdc );
   fhStsChanAdcRawProf[  uAsicIdx ]->Fill( usChan, usRawAdc );
   fhStsChanRawTs[   uAsicIdx ]->Fill( usChan, usRawTs );
   fhStsChanMissEvt[ uAsicIdx ]->Fill( usChan, mess.IsHitMissedEvts() );
*/
  //LOG(info) <<" uAsicIdx "<<uAsicIdx<<" usChan "<<usChan;
  Int_t FebId = fUnpackParMuch->GetFebId(uAsicIdx);

  UInt_t uFebIdx    = uAsicIdx / fUnpackParMuch->GetNbAsicsPerFeb();
  UInt_t uCrobIdx   = usElinkIdx / fUnpackParMuch->GetNbElinkPerCrob();
  UInt_t uAsicInFeb = uAsicIdx % fUnpackParMuch->GetNbAsicsPerFeb();
  UInt_t uChanInFeb = usChan
                      + fUnpackParMuch->GetNbChanPerAsic()
                          * (uAsicIdx % fUnpackParMuch->GetNbAsicsPerFeb());
  Int_t sector  = fUnpackParMuch->GetPadXA(FebId, usChan);
  Int_t channel = fUnpackParMuch->GetPadYA(FebId, usChan);
  //Convert into Real X Y Position
  Double_t ActualX = (fUnpackParMuch->GetRealPadSize(channel) * sector);
  Double_t ActualY = fUnpackParMuch->GetRealX(channel);
  //LOG(info) <<" sector "<<sector<<" channel "<<channel<<" FebId "<<FebId<<" usChan "<<usChan;
  Int_t ModuleNr = fUnpackParMuch->GetModule(uAsicIdx);
  fHistPadDistr[ModuleNr]->Fill(sector, channel);
  fRealHistPadDistr[ModuleNr]->Fill(ActualY, ActualX);

  //LOG(info) <<" usElinkIdx "<<usElinkIdx<<" uAsicIdx "<<uAsicIdx<<" uMsIdx "<<uMsIdx<<" uCrobIdx "<<uCrobIdx<<" uFebIdx "<<uFebIdx<<" uAsicInFeb "<<uAsicInFeb<<" uChanInFeb "<<uChanInFeb<<" usChan "<<usChan;

  Double_t dCalAdc =
    fvdFebAdcOffs[fuCurrDpbIdx][uCrobIdx][uFebIdx]
    + (usRawAdc - 1) * fvdFebAdcGain[fuCurrDpbIdx][uCrobIdx][uFebIdx];
  //if(uChanInFeb!=37 && uChanInFeb!=58 && uChanInFeb!=68 && uChanInFeb!=69 && uChanInFeb!=101 && uChanInFeb!=109 ){
  fhStsFebChanCntRaw[uFebIdx]->Fill(uChanInFeb);
  fhStsFebChanAdcRaw[uFebIdx]->Fill(uChanInFeb, usRawAdc);
  fhStsFebChanAdcRawProf[uFebIdx]->Fill(uChanInFeb, usRawAdc);
  fhStsFebChanAdcCal[uFebIdx]->Fill(uChanInFeb, dCalAdc);
  fhStsFebChanAdcCalProf[uFebIdx]->Fill(uChanInFeb, dCalAdc);
  fhStsFebChanRawTs[uFebIdx]->Fill(usChan, usRawTs);
  fhStsFebChanMissEvt[uFebIdx]->Fill(usChan, mess.IsHitMissedEvts());
  fhStsFebChanAdcRaw_combined->Fill(usRawAdc);
  //}
  // Compute the Full time stamp
  //   Long64_t ulOldHitTime = fvulChanLastHitTime[ uAsicIdx ][ usChan ];
  //Double_t dOldHitTime  = fvdChanLastHitTime[ uAsicIdx ][ usChan ];
  //   Long64_t dOldHitTime  = fvdChanLastHitTime[ uAsicIdx ][ usChan ];

  // Use TS w/o overlap bits as they will anyway come from the TS_MSB
  fvulChanLastHitTime[uAsicIdx][usChan] = usRawTs;

  fvulChanLastHitTime[uAsicIdx][usChan] +=
    static_cast<ULong64_t>(stsxyter::kuHitNbTsBins)
      * static_cast<ULong64_t>(fvulCurrentTsMsb[fuCurrDpbIdx])
    + static_cast<ULong64_t>(stsxyter::kulTsCycleNbBins)
        * static_cast<ULong64_t>(fvuCurrentTsMsbCycle[fuCurrDpbIdx]);

  //   fvuElinkLastTsHit[fuCurrDpbIdx] = usRawTs;

  // Convert the Hit time in bins to Hit time in ns
  Long64_t dHitTimeNs =
    fvulChanLastHitTime[uAsicIdx][usChan] * stsxyter::kdClockCycleNs;
  //Double_t dHitTimeNs = fvulChanLastHitTime[ uAsicIdx ][ usChan ] * stsxyter::kdClockCycleNs;

  /*
   // If needed fill the hit interval plots
   if( fbChanHitDtEna )
   {
      Double_t dDeltaT = dHitTimeNs - fvdChanLastHitTime[ uAsicIdx ][ usChan ];
      if( 0 == dDeltaT )
         fhStsChanHitDtNeg[ uAsicIdx ]->Fill( 1, usChan );
         else if( 0 < dDeltaT )
            fhStsChanHitDt[ uAsicIdx ]->Fill( dDeltaT, usChan );
         else fhStsChanHitDtNeg[ uAsicIdx ]->Fill( -dDeltaT, usChan );
   } // if( fbChanHitDtEna )
*/
  // Store new value of Hit time in ns
  fvdChanLastHitTime[uAsicIdx][usChan] =
    fvulChanLastHitTime[uAsicIdx][usChan] * stsxyter::kdClockCycleNs;
  /*
   LOG(info) << " Asic " << std::setw( 2 ) << uAsicIdx
             << " Channel " << std::setw( 3 ) << usChan
             << " Diff to last hit " << std::setw( 12 ) << ( fvulChanLastHitTime[ uAsicIdx ][ usChan ] - ulOldHitTime)
             << " in s " << std::setw( 12 ) << ( fvdChanLastHitTime[ uAsicIdx ][ usChan ] - dOldHitTime) * 1e-9;
*/
  // Pulser and MS
  fvuChanNbHitsInMs[uAsicIdx][usChan][uMsIdx]++;
  fvdChanLastHitTimeInMs[uAsicIdx][usChan][uMsIdx] = dHitTimeNs;
  fvusChanLastHitAdcInMs[uAsicIdx][usChan][uMsIdx] = usRawAdc;
  /*
   fvmChanHitsInTs[        uAsicIdx ][ usChan ].insert( stsxyter::FinalHit( fvulChanLastHitTime[ uAsicIdx ][ usChan ],
                                                                            usRawAdc, uAsicIdx, usChan ) );
*/
  fvmHitsInMs.push_back(stsxyter::FinalHit(
    fvulChanLastHitTime[uAsicIdx][usChan], usRawAdc, uAsicIdx, usChan));

  // Check Starting point of histos with time as X axis
  if (-1 == fdStartTime) fdStartTime = fvdChanLastHitTime[uAsicIdx][usChan];

  int64_t constime = (fvdChanLastHitTime[uAsicIdx][usChan] - prevtime);

  if (constime < 1000000) {
    if (usRawAdc > 1) { Counter1++; }
    Counter++;
  } else {
    fhRate->Fill(Counter);
    fhRateAdcCut->Fill(Counter1);
    Counter  = 0;
    Counter1 = 0;
  }

  prevtime = fvdChanLastHitTime[uAsicIdx][usChan];

  // Fill histos with time as X axis
  Double_t dTimeSinceStartSec =
    (fvdChanLastHitTime[uAsicIdx][usChan] - fdStartTime) * 1e-6;  //uTimeBin
  //   Double_t SpillTimeMs = dTimeSinceStartSec/1000000;
  //Double_t dTimeSinceStartSec = (fvdChanLastHitTime[ uAsicIdx ][ usChan ] - fdStartTime)* uTimeBin; //uTimeBin
  Double_t dTimeSinceStartMin = dTimeSinceStartSec / 60.0;

  //LOG(info)<<" full time "<<fvdChanLastHitTime[ uAsicIdx ][ usChan ]<<" start time "<<fdStartTime<<" dTimeSinceStartSec "<<dTimeSinceStartSec<<" dTimeSinceStartSec*1e-9 "<<dTimeSinceStartSec*1e+9<< " fvulChanLastHitTime[ uAsicIdx ][ usChan ] "<<fvulChanLastHitTime[ uAsicIdx ][ usChan ]<< " usChan "<<usChan<<" usrawadc "<<usRawAdc;
  fviFebCountsSinceLastRateUpdate[uFebIdx]++;
  fvdFebChanCountsSinceLastRateUpdate[uFebIdx][uChanInFeb] += 1;
  //LOG(info)<<" fviFebCountsSinceLastRateUpdate "<<fviFebCountsSinceLastRateUpdate[uFebIdx]<<" fvdFebChanCountsSinceLastRateUpdate "<<fvdFebChanCountsSinceLastRateUpdate[uFebIdx][uChanInFeb] <<" uFebIdx " << uFebIdx;
  //if((fvdChanLastHitTime[ uAsicIdx ][ usChan ] - fdStartTime) < 60.0)
  //fviFebCountsSinceLastRateUpdate[uFebIdx]++;
  fhStsFebChanHitRateEvo[uFebIdx]->Fill(dTimeSinceStartSec, uChanInFeb);
  fhStsFebAsicHitRateEvo[uFebIdx]->Fill(dTimeSinceStartSec, uAsicInFeb);
  fhStsFebHitRateEvo[uFebIdx]->Fill(dTimeSinceStartSec);
  // fhStsFebHitRateEvo[ uFebIdx ]->Fill( 10.0*SpillTimeMs );
  fhStsFebHitRateEvo_mskch[uFebIdx]->Fill(dTimeSinceStartSec);
  if (usRawAdc > 1)
    fhStsFebHitRateEvo_mskch_adccut[uFebIdx]->Fill(dTimeSinceStartSec);
  fhStsFebChanHitRateEvoLong[uFebIdx]->Fill(
    dTimeSinceStartMin, uChanInFeb, 1.0 / 60.0);
  fhStsFebAsicHitRateEvoLong[uFebIdx]->Fill(
    dTimeSinceStartMin, uAsicInFeb, 1.0 / 60.0);
  fhStsFebHitRateEvoLong[uFebIdx]->Fill(dTimeSinceStartMin, 1.0 / 60.0);
  if (mess.IsHitMissedEvts()) {
    fhStsFebChanMissEvtEvo[uFebIdx]->Fill(dTimeSinceStartSec, uChanInFeb);
    fhStsFebAsicMissEvtEvo[uFebIdx]->Fill(dTimeSinceStartSec, uAsicInFeb);
    fhStsFebMissEvtEvo[uFebIdx]->Fill(dTimeSinceStartSec);
  }  // if( mess.IsHitMissedEvts() )
}

void CbmMcbm2018MonitorMuch::FillTsMsbInfo(stsxyter::Message mess,
                                           UInt_t uMessIdx,
                                           UInt_t /*uMsIdx*/) {
  UInt_t uVal = mess.GetTsMsbVal();
  /*
   if( ( 419369 < fulCurrentTsIdx && fulCurrentTsIdx < 419371 ) )
      LOG(info) << " TS " << std::setw( 12 ) << fulCurrentTsIdx
                << " MS " << std::setw( 12 ) << fulCurrentMsIdx
                << " MsInTs " << std::setw( 3 ) << uMsIdx
                << " DPB "    << std::setw( 2 ) << fuCurrDpbIdx
                << " Mess "   << std::setw( 5 ) << uMessIdx
                << " TsMsb "  << std::setw( 5 ) << uVal;
*/
  /*
   if( (uVal != fvulCurrentTsMsb[fuCurrDpbIdx] + 1) && 0 < uVal  &&
       !( 1 == uMessIdx && usVal == fvulCurrentTsMsb[fuCurrDpbIdx] ) ) // 1st TS_MSB in MS is always a repeat of the last one in previous MS!
   {
      LOG(info) << "TS MSB not increasing by 1!  TS " << std::setw( 12 ) << fulCurrentTsIdx
                << " MS " << std::setw( 12 ) << fulCurrentMsIdx
                << " MsInTs " << std::setw( 3 ) << uMsIdx
                << " DPB " << std::setw( 2 ) << fuCurrDpbIdx
                << " Mess " << std::setw( 5 ) << uMessIdx
                << " Old TsMsb " << std::setw( 5 ) << fvulCurrentTsMsb[fuCurrDpbIdx]
                << " new TsMsb " << std::setw( 5 ) << uVal
                << " Diff " << std::setw( 5 ) << uVal - fvulCurrentTsMsb[fuCurrDpbIdx]
                << " Old MsbCy " << std::setw( 5 ) << fvuCurrentTsMsbCycle[fuCurrDpbIdx];
   } // if( (uVal != fvulCurrentTsMsb[fuCurrDpbIdx] + 1) && 0 < uVal )
*/

  // Update Status counters
  if (uVal < fvulCurrentTsMsb[fuCurrDpbIdx]) {
    /*
      LOG(info) << " TS " << std::setw( 12 ) << fulCurrentTsIdx
                << " MS " << std::setw( 12 ) << fulCurrentMsIdx
                << " MS Idx " << std::setw( 4 ) << uMsIdx
                << " Msg Idx " << std::setw( 5 ) << uMessIdx
                << " DPB " << std::setw( 2 ) << fuCurrDpbIdx
                << " Old TsMsb " << std::setw( 5 ) << fvulCurrentTsMsb[fuCurrDpbIdx]
                << " Old MsbCy " << std::setw( 5 ) << fvuCurrentTsMsbCycle[fuCurrDpbIdx]
                << " new TsMsb " << std::setw ( 5 ) << uVal;
*/

    fvuCurrentTsMsbCycle[fuCurrDpbIdx]++;
  }  // if( uVal < fvulCurrentTsMsb[fuCurrDpbIdx] )
  if (uVal != fvulCurrentTsMsb[fuCurrDpbIdx] + 1 && 0 != uVal
      && 4194303 != fvulCurrentTsMsb[fuCurrDpbIdx] && 1 != uMessIdx) {
    /*
      LOG(info) << "TS MSb Jump in "
                << " TS " << std::setw( 12 ) << fulCurrentTsIdx
                << " MS " << std::setw( 12 ) << fulCurrentMsIdx
                << " MS Idx " << std::setw( 4 ) << uMsIdx
                << " Msg Idx " << std::setw( 5 ) << uMessIdx
                << " DPB " << std::setw( 2 ) << fuCurrDpbIdx
                << " => Old TsMsb " << std::setw( 5 ) << fvulCurrentTsMsb[fuCurrDpbIdx]
                << " new TsMsb " << std::setw ( 5 ) << uVal;
*/
  }  // if( uVal + 1 != fvulCurrentTsMsb[fuCurrDpbIdx] && 4194303 != uVal && 0 != fvulCurrentTsMsb[fuCurrDpbIdx] )
  fvulCurrentTsMsb[fuCurrDpbIdx] = uVal;
  /*
   ULong64_t ulNewTsMsbTime =  static_cast< ULong64_t >( stsxyter::kuHitNbTsBins )
                             * static_cast< ULong64_t >( fvulCurrentTsMsb[fuCurrDpbIdx])
                             + static_cast< ULong64_t >( stsxyter::kulTsCycleNbBins )
                             * static_cast< ULong64_t >( fvuCurrentTsMsbCycle[fuCurrDpbIdx] );
*/
}

void CbmMcbm2018MonitorMuch::FillEpochInfo(stsxyter::Message /*mess*/) {
  //   UInt_t uVal    = mess.GetEpochVal();
  //   UInt_t uCurrentCycle = uVal % stsxyter::kulTsCycleNbBins;
}

void CbmMcbm2018MonitorMuch::Reset() {}

void CbmMcbm2018MonitorMuch::Finish() {

  LOG(info) << "-------------------------------------";
  LOG(info) << "CbmMcbm2018MonitorMuch statistics are ";
  LOG(info) << " Hit      messages: " << fmMsgCounter[stsxyter::MessType::Hit]
            << "\n"
            << " Ts MSB   messages: " << fmMsgCounter[stsxyter::MessType::TsMsb]
            << "\n"
            << " Dummy    messages: " << fmMsgCounter[stsxyter::MessType::Dummy]
            << "\n"
            << " Epoch    messages: " << fmMsgCounter[stsxyter::MessType::Epoch]
            << "\n"
            << " Empty    messages: "
            << fmMsgCounter[stsxyter::MessType::Empty];

  LOG(info) << "-------------------------------------";

  SaveAllHistos(fsHistoFileFullname);
  //SaveAllHistos();
}


void CbmMcbm2018MonitorMuch::SaveAllHistos(TString sFileName) {
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* histoFile   = NULL;
  if ("" != sFileName) {
    // open separate histo file in recreate mode
    histoFile = new TFile(sFileName, "RECREATE");
    histoFile->cd();
  }  // if( "" != sFileName )

  /***************************/
  gDirectory->mkdir("Sts_Raw");
  gDirectory->cd("Sts_Raw");

  for (UInt_t uModuleId = 0; uModuleId < 2; ++uModuleId) {
    fHistPadDistr[uModuleId]->Write();
    fRealHistPadDistr[uModuleId]->Write();
  }
  fhRate->Write();
  fhRateAdcCut->Write();
  fhStsMessType->Write();
  fhStsSysMessType->Write();
  fhStsMessTypePerDpb->Write();
  fhStsSysMessTypePerDpb->Write();
  fhPulserStatusMessType->Write();
  fhPulserMsStatusFieldType->Write();
  fhStsHitsElinkPerDpb->Write();
  fhStsFebChanAdcRaw_combined->Write();
  gDirectory->cd("..");
  /***************************/

  /***************************/
  gDirectory->mkdir("Sts_Feb");
  gDirectory->cd("Sts_Feb");
  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    if (kTRUE == fUnpackParMuch->IsFebActive(uFebIdx)) {
      fhStsFebChanCntRaw[uFebIdx]->Write();
      fhStsFebChanCntRawGood[uFebIdx]->Write();
      fhStsFebChanAdcRaw[uFebIdx]->Write();
      fhStsFebChanAdcRawProf[uFebIdx]->Write();
      fhStsFebChanAdcCal[uFebIdx]->Write();
      fhStsFebChanAdcCalProf[uFebIdx]->Write();
      fhStsFebChanRawTs[uFebIdx]->Write();
      fhStsFebChanMissEvt[uFebIdx]->Write();
      fhStsFebChanMissEvtEvo[uFebIdx]->Write();
      fhStsFebAsicMissEvtEvo[uFebIdx]->Write();
      fhStsFebMissEvtEvo[uFebIdx]->Write();
      fhStsFebChanHitRateEvo[uFebIdx]->Write();
      fhStsFebChanHitRateProf[uFebIdx]->Write();
      fhStsFebAsicHitRateEvo[uFebIdx]->Write();
      fhStsFebHitRateEvo[uFebIdx]->Write();
      fhStsFebHitRateEvo_mskch[uFebIdx]->Write();
      fhStsFebHitRateEvo_mskch_adccut[uFebIdx]->Write();
      fhStsFebChanHitRateEvoLong[uFebIdx]->Write();
      fhStsFebAsicHitRateEvoLong[uFebIdx]->Write();
      fhStsFebHitRateEvoLong[uFebIdx]->Write();
      fhStsFebChanDistT[uFebIdx]->Write();
      for (UInt_t uFebIdxB = uFebIdx; uFebIdxB < fuNbFebs; ++uFebIdxB) {
        fhStsFebChanDtCoinc[uFebIdx][uFebIdxB]->Write();
        fhStsFebChanCoinc[uFebIdx][uFebIdxB]->Write();
      }  // for( UInt_t uFebIdxB = uFebIdx; uFebIdxB < fuNbFebs; ++uFebIdxB )
    }    // if( kTRUE == fUnpackParMuch->IsFebActive( uFebIdx ) )
  }      // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )
  gDirectory->cd("..");
  /***************************/


  ///----------------- SXM 2.0 Logic Error Tagging --------------------///
  gDirectory->mkdir("Sts_SmxErr");
  gDirectory->cd("Sts_SmxErr");
  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    if (kTRUE == fUnpackParMuch->IsFebActive(uFebIdx)) {
      fhStsFebSmxErrRatioEvo[uFebIdx]->Write();
      fhStsFebSmxErrRatioEvoAsic[uFebIdx]->Write();
      fhStsFebSmxErrRatioCopyEvo[uFebIdx]->Write();
      fhStsFebSmxErrRatioCopyEvoAsic[uFebIdx]->Write();
      fhStsFebSmxErrRatioCopySameAdcEvo[uFebIdx]->Write();
      fhStsFebSmxErrRatioCopySameAdcEvoAsic[uFebIdx]->Write();
    }  // if( kTRUE == fUnpackParMuch->IsFebActive( uFebIdx ) )
  }    // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )
  gDirectory->cd("..");
  ///------------------------------------------------------------------///

  /***************************/
  // Flib Histos
  gDirectory->mkdir("Flib_Raw");
  gDirectory->cd("Flib_Raw");
  for (UInt_t uLinks = 0; uLinks < kiMaxNbFlibLinks; uLinks++) {
    TString sMsSzName = Form("MsSz_link_%02u", uLinks);
    if (fHM->Exists(sMsSzName.Data())) fHM->H1(sMsSzName.Data())->Write();

    sMsSzName = Form("MsSzTime_link_%02u", uLinks);
    if (fHM->Exists(sMsSzName.Data())) fHM->P1(sMsSzName.Data())->Write();
  }  // for( UInt_t uLinks = 0; uLinks < 16; uLinks ++)

  TH1* pMissedTsH1 = dynamic_cast<TH1*>(gROOT->FindObjectAny("Missed_TS"));
  if (NULL != pMissedTsH1) pMissedTsH1->Write();

  TProfile* pMissedTsEvoP =
    dynamic_cast<TProfile*>(gROOT->FindObjectAny("Missed_TS_Evo"));
  if (NULL != pMissedTsEvoP) pMissedTsEvoP->Write();

  gDirectory->cd("..");
  /***************************/

  if ("" != sFileName) {
    // Restore original directory position
    histoFile->Close();
  }  // if( "" != sFileName )

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}
void CbmMcbm2018MonitorMuch::ResetAllHistos() {
  LOG(info) << "Reseting all STS histograms.";

  for (UInt_t uModuleId = 0; uModuleId < 2; ++uModuleId) {
    fHistPadDistr[uModuleId]->Reset();
    fRealHistPadDistr[uModuleId]->Reset();
  }

  fhRate->Reset();
  fhRateAdcCut->Reset();
  fhStsMessType->Reset();
  fhStsSysMessType->Reset();
  fhStsMessTypePerDpb->Reset();
  fhStsSysMessTypePerDpb->Reset();
  fhPulserStatusMessType->Reset();
  fhPulserMsStatusFieldType->Reset();
  fhStsHitsElinkPerDpb->Reset();
  fhStsFebChanAdcRaw_combined->Reset();

  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    if (kTRUE == fUnpackParMuch->IsFebActive(uFebIdx)) {
      fhStsFebChanCntRaw[uFebIdx]->Reset();
      fhStsFebChanCntRawGood[uFebIdx]->Reset();
      fhStsFebChanAdcRaw[uFebIdx]->Reset();
      fhStsFebChanAdcRawProf[uFebIdx]->Reset();
      fhStsFebChanAdcCal[uFebIdx]->Reset();
      fhStsFebChanAdcCalProf[uFebIdx]->Reset();
      fhStsFebChanRawTs[uFebIdx]->Reset();
      fhStsFebChanMissEvt[uFebIdx]->Reset();
      fhStsFebChanMissEvtEvo[uFebIdx]->Reset();
      fhStsFebAsicMissEvtEvo[uFebIdx]->Reset();
      fhStsFebMissEvtEvo[uFebIdx]->Reset();
      fhStsFebChanHitRateEvo[uFebIdx]->Reset();
      fhStsFebChanHitRateProf[uFebIdx]->Reset();
      fhStsFebAsicHitRateEvo[uFebIdx]->Reset();
      fhStsFebHitRateEvo[uFebIdx]->Reset();
      fhStsFebHitRateEvo_mskch[uFebIdx]->Reset();
      fhStsFebHitRateEvo_mskch_adccut[uFebIdx]->Reset();
      fhStsFebChanHitRateEvoLong[uFebIdx]->Reset();
      fhStsFebAsicHitRateEvoLong[uFebIdx]->Reset();
      fhStsFebHitRateEvoLong[uFebIdx]->Reset();
      fhStsFebChanDistT[uFebIdx]->Reset();
      for (UInt_t uFebIdxB = uFebIdx; uFebIdxB < fuNbFebs; ++uFebIdxB) {
        fhStsFebChanDtCoinc[uFebIdx][uFebIdxB]->Reset();
        fhStsFebChanCoinc[uFebIdx][uFebIdxB]->Reset();
      }  // for( UInt_t uFebIdxB = uFebIdx; uFebIdxB < fuNbFebs; ++uFebIdxB )

      ///----------------- SXM 2.0 Logic Error Tagging --------------------///
      fhStsFebSmxErrRatioEvo[uFebIdx]->Reset();
      fhStsFebSmxErrRatioEvoAsic[uFebIdx]->Reset();
      fhStsFebSmxErrRatioCopyEvo[uFebIdx]->Reset();
      fhStsFebSmxErrRatioCopyEvoAsic[uFebIdx]->Reset();
      fhStsFebSmxErrRatioCopySameAdcEvo[uFebIdx]->Reset();
      fhStsFebSmxErrRatioCopySameAdcEvoAsic[uFebIdx]->Reset();
      ///------------------------------------------------------------------///
    }  // if( kTRUE == fUnpackParMuch->IsFebActive( uFebIdx ) )
  }    // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )

  for (UInt_t uLinks = 0; uLinks < kiMaxNbFlibLinks; ++uLinks) {
    TString sMsSzName = Form("MsSz_link_%02u", uLinks);
    if (fHM->Exists(sMsSzName.Data())) fHM->H1(sMsSzName.Data())->Reset();

    sMsSzName = Form("MsSzTime_link_%02u", uLinks);
    if (fHM->Exists(sMsSzName.Data())) fHM->P1(sMsSzName.Data())->Reset();
  }  // for( UInt_t uLinks = 0; uLinks < kiMaxNbFlibLinks; ++uLinks )

  fdStartTime     = -1;
  fdStartTimeMsSz = -1;
}

void CbmMcbm2018MonitorMuch::SetRunStart(Int_t dateIn,
                                         Int_t timeIn,
                                         Int_t iBinSize) {
  TDatime* fRunStartDateTime = new TDatime(dateIn, timeIn);
  fiRunStartDateTimeSec      = fRunStartDateTime->Convert();
  fiBinSizeDatePlots         = iBinSize;

  LOG(info) << "Assigned new MUCH Run Start Date-Time: "
            << fRunStartDateTime->AsString();
}

void CbmMcbm2018MonitorMuch::SetLongDurationLimits(UInt_t uDurationSeconds,
                                                   UInt_t uBinSize) {
  fbLongHistoEnable     = kTRUE;
  fuLongHistoNbSeconds  = uDurationSeconds;
  fuLongHistoBinSizeSec = uBinSize;
}
///------------------------------------------------------------------///
///----------------- SXM 2.0 Logic Error Tagging --------------------///
///------------------------------------------------------------------///
void CbmMcbm2018MonitorMuch::SmxErrInitializeVariables() {

  if (kTRUE == fbSmx2ErrorUseNoiseLevels) {
    fdSmxErrCoincWinM07 = kdSmxErrCoincWinNoiseM07;
    fdSmxErrCoincWinM08 = kdSmxErrCoincWinNoiseM08;
    fdSmxErrCoincWinM09 = kdSmxErrCoincWinNoiseM09;
    fdSmxErrCoincWinM10 = kdSmxErrCoincWinNoiseM10;
    fdSmxErrCoincWinM11 = kdSmxErrCoincWinNoiseM11;
  }  // if( kTRUE == fbSmx2ErrorUseNoiseLevels )

  fvdSmxErrTimeLastHits.resize(fuNbFebs);
  fvuSmxErrIdxFirstHitM07.resize(fuNbFebs);
  fvuSmxErrIdxFirstHitM08.resize(fuNbFebs);
  fvuSmxErrIdxFirstHitM09.resize(fuNbFebs);
  fvuSmxErrIdxFirstHitM10.resize(fuNbFebs);
  fvuSmxErrIdxFirstHitM11.resize(fuNbFebs);
  fvuSmxErrIdxLastHit.resize(fuNbFebs);
  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    fvdSmxErrTimeLastHits[uFebIdx].resize(fUnpackParMuch->GetNbAsicsPerFeb());
    fvuSmxErrIdxFirstHitM07[uFebIdx].resize(fUnpackParMuch->GetNbAsicsPerFeb());
    fvuSmxErrIdxFirstHitM08[uFebIdx].resize(fUnpackParMuch->GetNbAsicsPerFeb());
    fvuSmxErrIdxFirstHitM09[uFebIdx].resize(fUnpackParMuch->GetNbAsicsPerFeb());
    fvuSmxErrIdxFirstHitM10[uFebIdx].resize(fUnpackParMuch->GetNbAsicsPerFeb());
    fvuSmxErrIdxFirstHitM11[uFebIdx].resize(fUnpackParMuch->GetNbAsicsPerFeb());
    fvuSmxErrIdxLastHit[uFebIdx].resize(fUnpackParMuch->GetNbAsicsPerFeb());
    for (UInt_t uAsicIdx = 0; uAsicIdx < fUnpackParMuch->GetNbAsicsPerFeb();
         ++uAsicIdx) {
      //fvulLastHitTs[ uXyterIdx ].resize( fuNbChanPerAsic, 0 );
      fvdSmxErrTimeLastHits[uFebIdx][uAsicIdx].resize(kuSmxErrCoincWinNbHits,
                                                      -1.0);
      fvuSmxErrIdxFirstHitM07[uFebIdx][uAsicIdx] = kuSmxErrCoincWinNbHits;
      fvuSmxErrIdxFirstHitM08[uFebIdx][uAsicIdx] = kuSmxErrCoincWinNbHits;
      fvuSmxErrIdxFirstHitM09[uFebIdx][uAsicIdx] = kuSmxErrCoincWinNbHits;
      fvuSmxErrIdxFirstHitM10[uFebIdx][uAsicIdx] = kuSmxErrCoincWinNbHits;
      fvuSmxErrIdxFirstHitM11[uFebIdx][uAsicIdx] = kuSmxErrCoincWinNbHits;
      fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx]     = kuSmxErrCoincWinNbHits;
    }  // for( UInt_t uAsicIdx = 0; uAsicIdx < fUnpackParMuch->GetNbAsicsPerFeb(); ++uAsicIdx )
  }    // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )
}

Bool_t CbmMcbm2018MonitorMuch::SmxErrCheckCoinc(UInt_t uFebIdx,
                                                UInt_t uAsicIdx,
                                                Double_t dNewHitTime) {
  if (kuSmxErrCoincWinNbHits == fvuSmxErrIdxFirstHitM07[uFebIdx][uAsicIdx]
      && kuSmxErrCoincWinNbHits == fvuSmxErrIdxFirstHitM08[uFebIdx][uAsicIdx]
      && kuSmxErrCoincWinNbHits == fvuSmxErrIdxFirstHitM09[uFebIdx][uAsicIdx]
      && kuSmxErrCoincWinNbHits == fvuSmxErrIdxFirstHitM10[uFebIdx][uAsicIdx]
      && kuSmxErrCoincWinNbHits == fvuSmxErrIdxFirstHitM11[uFebIdx][uAsicIdx]
      && kuSmxErrCoincWinNbHits == fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx]) {
    /// 1st iteration => Initialize both indices on first slot in array
    fvuSmxErrIdxFirstHitM07[uFebIdx][uAsicIdx] = 4;
    fvuSmxErrIdxFirstHitM08[uFebIdx][uAsicIdx] = 3;
    fvuSmxErrIdxFirstHitM09[uFebIdx][uAsicIdx] = 2;
    fvuSmxErrIdxFirstHitM10[uFebIdx][uAsicIdx] = 1;
    fvuSmxErrIdxFirstHitM11[uFebIdx][uAsicIdx] = 0;
    fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx]     = 0;
    fvdSmxErrTimeLastHits[uFebIdx][uAsicIdx]
                         [fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx]] = dNewHitTime;

    /// Exit as we have less hits than needed to trigger the bug
    return kFALSE;
  }  /// if both indices are 0 => 1st hit in this ASIC
  else if (kuSmxErrCoincWinNbHits - 1
             == fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx]
                  - fvuSmxErrIdxFirstHitM11[uFebIdx][uAsicIdx]
           || fvuSmxErrIdxFirstHitM11[uFebIdx][uAsicIdx] - 1
                == fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx]) {
    /// Running loop once the array is full => shift indices for both first and last hit
    fvuSmxErrIdxFirstHitM07[uFebIdx][uAsicIdx] =
      (fvuSmxErrIdxFirstHitM07[uFebIdx][uAsicIdx] + 1) % kuSmxErrCoincWinNbHits;
    fvuSmxErrIdxFirstHitM08[uFebIdx][uAsicIdx] =
      (fvuSmxErrIdxFirstHitM08[uFebIdx][uAsicIdx] + 1) % kuSmxErrCoincWinNbHits;
    fvuSmxErrIdxFirstHitM09[uFebIdx][uAsicIdx] =
      (fvuSmxErrIdxFirstHitM09[uFebIdx][uAsicIdx] + 1) % kuSmxErrCoincWinNbHits;
    fvuSmxErrIdxFirstHitM10[uFebIdx][uAsicIdx] =
      (fvuSmxErrIdxFirstHitM10[uFebIdx][uAsicIdx] + 1) % kuSmxErrCoincWinNbHits;
    fvuSmxErrIdxFirstHitM11[uFebIdx][uAsicIdx] =
      (fvuSmxErrIdxFirstHitM11[uFebIdx][uAsicIdx] + 1) % kuSmxErrCoincWinNbHits;
    fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx] =
      (fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx] + 1) % kuSmxErrCoincWinNbHits;
    fvdSmxErrTimeLastHits[uFebIdx][uAsicIdx]
                         [fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx]] = dNewHitTime;
  }  /// If we had already at least kuNbHitCoincWinBug hits
  else {
    /// Still filling the array => shift only the index for last hit
    fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx] =
      fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx] + 1;
    fvdSmxErrTimeLastHits[uFebIdx][uAsicIdx]
                         [fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx]] = dNewHitTime;

    /// Exit as we have less hits than needed to trigger the bug
    return kFALSE;
  }  /// less than kuSmxErrCoincWinNbHits hits

  /// Compute the time difference between 1st and last for each condition
  Double_t dTimeDiff07 =
    fvdSmxErrTimeLastHits[uFebIdx][uAsicIdx]
                         [fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx]]
    - fvdSmxErrTimeLastHits[uFebIdx][uAsicIdx]
                           [fvuSmxErrIdxFirstHitM07[uFebIdx][uAsicIdx]];
  Double_t dTimeDiff08 =
    fvdSmxErrTimeLastHits[uFebIdx][uAsicIdx]
                         [fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx]]
    - fvdSmxErrTimeLastHits[uFebIdx][uAsicIdx]
                           [fvuSmxErrIdxFirstHitM08[uFebIdx][uAsicIdx]];
  Double_t dTimeDiff09 =
    fvdSmxErrTimeLastHits[uFebIdx][uAsicIdx]
                         [fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx]]
    - fvdSmxErrTimeLastHits[uFebIdx][uAsicIdx]
                           [fvuSmxErrIdxFirstHitM09[uFebIdx][uAsicIdx]];
  Double_t dTimeDiff10 =
    fvdSmxErrTimeLastHits[uFebIdx][uAsicIdx]
                         [fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx]]
    - fvdSmxErrTimeLastHits[uFebIdx][uAsicIdx]
                           [fvuSmxErrIdxFirstHitM10[uFebIdx][uAsicIdx]];
  Double_t dTimeDiff11 =
    fvdSmxErrTimeLastHits[uFebIdx][uAsicIdx]
                         [fvuSmxErrIdxLastHit[uFebIdx][uAsicIdx]]
    - fvdSmxErrTimeLastHits[uFebIdx][uAsicIdx]
                           [fvuSmxErrIdxFirstHitM11[uFebIdx][uAsicIdx]];


  /// Check conditions and set flag
  if ((kdSmxErrCoincWinBeg <= dTimeDiff07 && dTimeDiff07 <= fdSmxErrCoincWinM07)
      || (kdSmxErrCoincWinBeg <= dTimeDiff08
          && dTimeDiff08 <= fdSmxErrCoincWinM08)
      || (kdSmxErrCoincWinBeg <= dTimeDiff09
          && dTimeDiff09 <= fdSmxErrCoincWinM09)
      || (kdSmxErrCoincWinBeg <= dTimeDiff10
          && dTimeDiff10 <= fdSmxErrCoincWinM10)
      || (kdSmxErrCoincWinBeg <= dTimeDiff11
          && dTimeDiff11 <= fdSmxErrCoincWinM11)) {
    return kTRUE;
  }  /// If we are within at least one of the coincidence windows
  else {
    return kFALSE;
  }  /// Out of all coincidence windows, we are safe!
}
///------------------------------------------------------------------///
Bool_t CbmMcbm2018MonitorMuch::ScanForNoisyChannels(Double_t dNoiseThreshold) {
  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    if (kTRUE == fUnpackParMuch->IsFebActive(uFebIdx)) {
      LOG(info) << Form(
        " ------------------ Noisy channels scan for FEB %2d ------------",
        uFebIdx);
      for (UInt_t uAsicIdx = 0; uAsicIdx < fUnpackParMuch->GetNbAsicsPerFeb();
           ++uAsicIdx)
        for (UInt_t uChanIdx = 0; uChanIdx < fUnpackParMuch->GetNbChanPerAsic();
             ++uChanIdx) {
          UInt_t uChanInFeb =
            uAsicIdx * fUnpackParMuch->GetNbChanPerAsic() + uChanIdx;
          if (dNoiseThreshold
              < fhStsFebChanHitRateProf[uFebIdx]->GetBinContent(1 + uChanInFeb))
            LOG(info) << Form(
              "Noisy Channel ASIC %d channel %3d (%4d) level %6.0f",
              uAsicIdx,
              uChanIdx,
              uChanInFeb,
              fhStsFebChanHitRateProf[uFebIdx]->GetBinContent(1 + uChanInFeb));

        }  // for( UInt_t uChanIdx = 0; uChanIdx < fUnpackParMuch->GetNbChanPerAsic(); ++uChanIdx )

      LOG(info)
        << " ---------------------------------------------------------------";
    }  // if( kTRUE == fUnpackParMuch->IsFebActive( uFebIdx ) )
  }    // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )
  return kTRUE;
}
///------------------------------------------------------------------///

ClassImp(CbmMcbm2018MonitorMuch)
