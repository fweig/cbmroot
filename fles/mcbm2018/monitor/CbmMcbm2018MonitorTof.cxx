// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmMcbm2018MonitorTof                          -----
// -----               Created 10.07.2018 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018MonitorTof.h"
#include "CbmMcbm2018TofPar.h"

#include "CbmFormatDecHexPrintout.h"
#include "CbmFormatMsHeaderPrintout.h"

#include "CbmHistManager.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

#include "Rtypes.h"
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "THttpServer.h"
#include "TMath.h"
#include "TPaveStats.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"
#include <TFile.h>

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <stdint.h>

Bool_t bMcbmMoniTofResetHistos       = kFALSE;
Bool_t bMcbmMoniTofSaveHistos        = kFALSE;
Bool_t bMcbmMoniTofUpdateZoomedFit   = kFALSE;
Bool_t bMcbmMoniTofRawDataPrint      = kFALSE;
Bool_t bMcbmMoniTofPrintAllHitsEna   = kFALSE;
Bool_t bMcbmMoniTofPrintAllEpochsEna = kFALSE;

CbmMcbm2018MonitorTof::CbmMcbm2018MonitorTof()
  : CbmMcbmUnpack()
  , fvMsComponentsList()
  , fuNbCoreMsPerTs(0)
  , fuNbOverMsPerTs(0)
  , fbIgnoreOverlapMs(kFALSE)
  , fuMsAcceptsPercent(100)
  , fuTotalMsNb(0)
  , fuOverlapMsNb(0)
  , fuCoreMs(0)
  , fdMsSizeInNs(0.0)
  , fdTsCoreSizeInNs(0.0)
  , fuMinNbGdpb(0)
  , fuCurrNbGdpb(0)
  , fUnpackPar()
  , fuNrOfGdpbs(0)
  , fuNrOfFeePerGdpb(0)
  , fuNrOfGet4PerFee(0)
  , fuNrOfChannelsPerGet4(0)
  , fuNrOfChannelsPerFee(0)
  , fuNrOfGet4(0)
  , fuNrOfGet4PerGdpb(0)
  , fuNrOfChannelsPerGdpb(0)
  , fuRawDataPrintMsgNb(100000)
  , fuRawDataPrintMsgIdx(fuRawDataPrintMsgNb)
  , fbPrintAllHitsEnable(kFALSE)
  , fbPrintAllEpochsEnable(kFALSE)
  , fbPulserModeEnable(kFALSE)
  , fbCoincMapsEnable(kFALSE)
  , fbOldFwData(kFALSE)
  , fuDiamondDpbIdx(10000)
  ,  // Crazy default value => should never make troubles given the price
  fsHistoFilename("data/HistosMonitorTof.root")
  , fulCurrentTsIndex(0)
  , fuCurrentMs(0)
  , fuCurrentMsSysId(0)
  , fdMsIndex(0)
  , fuGdpbId(0)
  , fuGdpbNr(0)
  , fuGet4Id(0)
  , fuGet4Nr(0)
  , fiEquipmentId(0)
  , fviMsgCounter(11, 0)
  ,  // length of enum MessageTypes initialized with 0
  fvulGdpbTsMsb()
  , fvulGdpbTsLsb()
  , fvulStarTsMsb()
  , fvulStarTsMid()
  , fvulGdpbTsFullLast()
  , fvulStarTsFullLast()
  , fvuStarTokenLast()
  , fvuStarDaqCmdLast()
  , fvuStarTrigCmdLast()
  , fvulCurrentEpoch()
  , fvbFirstEpochSeen()
  , fvulCurrentEpochCycle()
  , fvulCurrentEpochFull()
  , fulCurrentEpochTime(0)
  , fGdpbIdIndexMap()
  , fvmEpSupprBuffer()
  , fvuFeeNbHitsLastMs()
  , fdTsLastPulserHit()
  , fvuCoincNbHitsLastMs()
  , fvdCoincTsLastHit()
  , dMinDt(-1. * (kuNbBinsDt * gdpbv100::kdBinSize / 2.)
           - gdpbv100::kdBinSize / 2.)
  , dMaxDt(1. * (kuNbBinsDt * gdpbv100::kdBinSize / 2.)
           + gdpbv100::kdBinSize / 2.)
  , fuNbFeePlot(2)
  , fuNbFeePlotsPerGdpb(0)
  , fdStartTime(-1.)
  , fdStartTimeLong(-1.)
  , fdStartTimeMsSz(-1.)
  , fuHistoryHistoSize(1800)
  , fuHistoryHistoSizeLong(600)
  , fdLastRmsUpdateTime(0.0)
  , fdFitZoomWidthPs(0.0)
  , fcMsSizeAll(NULL)
  , fvhMsSzPerLink(12, NULL)
  , fvhMsSzTimePerLink(12, NULL)
  , fhMessType(NULL)
  , fhSysMessType(NULL)
  , fhGet4MessType(NULL)
  , fhGet4ChanScm(NULL)
  , fhGet4ChanErrors(NULL)
  , fhGet4EpochFlags(NULL)
  , fhGdpbMessType(NULL)
  , fhGdpbSysMessType(NULL)
  , fhGdpbSysMessPattType(NULL)
  , fhGdpbEpochFlags(NULL)
  , fhGdpbEpochSyncEvo(NULL)
  , fhGdpbEpochMissEvo(NULL)
  , fvhGdpbGet4MessType()
  , fvhGdpbGet4ChanScm()
  , fvhGdpbGet4ChanErrors()
  , fhScmScalerCounters(NULL)
  , fhScmDeadtimeCounters(NULL)
  , fhScmSeuCounters(NULL)
  , fhScmSeuCountersEvo(NULL)
  , fhPatternMissmatch(NULL)
  , fhPatternEnable(NULL)
  , fhPatternResync(NULL)
  , fvhGdpbPatternMissmatchEvo()
  , fvhGdpbPatternEnableEvo()
  , fvhGdpbPatternResyncEvo()
  , fvvbGdpbLastMissmatchPattern()
  , fvvbGdpbLastEnablePattern()
  , fvvbGdpbLastResyncPattern()
  , fvhGdpbMissmatchEvoPerTs()
  , fvhGdpbMissmatchEnaEvoPerTs()
  , fvhGdpbEnableEvoPerTs()
  , fvhGdpbResyncEvoPerTs()
  , fvhGdpbResyncEnaEvoPerTs()
  , fvhGdpbStateEvoPerTs()
  , fvhRawFt_gDPB()
  , fvhRawTot_gDPB()
  , fvhChCount_gDPB()
  , fvhChannelRate_gDPB()
  , fvhRemapTot_gDPB()
  , fvhRemapChCount_gDPB()
  , fvhRemapChRate_gDPB()
  , fvhFeeRate_gDPB()
  , fvhFeeErrorRate_gDPB()
  , fvhFeeErrorRatio_gDPB()
  , fvhFeeRateLong_gDPB()
  , fvhFeeErrorRateLong_gDPB()
  , fvhFeeErrorRatioLong_gDPB()
  ,
  /*
    fvhRemapTotSideA_mod(),
    fvhRemapTotSideB_mod(),
    fvhModRate(),
    fvhModErrorRate(),
    fvhModErrorRatio(),
*/
  fvvuChanNbHitsPerMs()
  , fhHitsPerMsFirstChan_gDPB()
  , fvhChannelRatePerMs_gDPB()
  , fvhTokenMsgType()
  , fvhTriggerRate()
  , fvhCmdDaqVsTrig()
  , fvhStarTokenEvo()
  , fvhStarTrigGdpbTsEvo()
  , fvhStarTrigStarTsEvo()
  , fvhTimeDiffPulser()
  , fhTimeMeanPulser(NULL)
  , fhTimeRmsPulser(NULL)
  , fhTimeRmsZoomFitPuls(NULL)
  , fhTimeResFitPuls(NULL)
  , fvhPulserTimeDiffEvoGbtxGbtx()
  , fvvhPulserTimeDiffEvoGdpbGdpb()
  , fvuPadiToGet4()
  , fvuGet4ToPadi()
  , fvuElinkToGet4()
  , fvuGet4ToElink()
  , fTimeLastHistoSaving() {}

CbmMcbm2018MonitorTof::~CbmMcbm2018MonitorTof() {}

Bool_t CbmMcbm2018MonitorTof::Init() {
  LOG(info) << "Initializing Get4 monitor";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }  // if( ioman == NULL )

  return kTRUE;
}

void CbmMcbm2018MonitorTof::SetParContainers() {
  LOG(info) << "Setting parameter containers for " << GetName();
  fUnpackPar =
    (CbmMcbm2018TofPar*) (FairRun::Instance()->GetRuntimeDb()->getContainer(
      "CbmMcbm2018TofPar"));
}

Bool_t CbmMcbm2018MonitorTof::InitContainers() {
  LOG(info) << "Init parameter containers for " << GetName();
  Bool_t initOK = ReInitContainers();

  CreateHistograms();

  fvulCurrentEpoch.resize(fuNrOfGdpbs * fuNrOfGet4PerGdpb);
  fvbFirstEpochSeen.resize(fuNrOfGdpbs * fuNrOfGet4PerGdpb);
  fvulCurrentEpochCycle.resize(fuNrOfGdpbs * fuNrOfGet4PerGdpb);
  fvulCurrentEpochFull.resize(fuNrOfGdpbs * fuNrOfGet4PerGdpb);
  for (UInt_t i = 0; i < fuNrOfGdpbs; ++i) {
    for (UInt_t j = 0; j < fuNrOfGet4PerGdpb; ++j) {
      fvulCurrentEpoch[GetArrayIndex(i, j)]      = 0;
      fvulCurrentEpochCycle[GetArrayIndex(i, j)] = 0;
      fvulCurrentEpochFull[GetArrayIndex(i, j)]  = 0;
    }  // for( UInt_t j = 0; j < fuNrOfGet4PerGdpb; ++j )
  }    // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )

  return initOK;
}

Bool_t CbmMcbm2018MonitorTof::ReInitContainers() {
  LOG(info) << "ReInit parameter containers for " << GetName();

  fuNrOfGdpbs = fUnpackPar->GetNrOfGdpbs();
  LOG(info) << "Nr. of Tof GDPBs: " << fuNrOfGdpbs;
  fuMinNbGdpb = fuNrOfGdpbs;

  fuNrOfFeePerGdpb = fUnpackPar->GetNrOfFeesPerGdpb();
  LOG(info) << "Nr. of FEEs per Tof GDPB: " << fuNrOfFeePerGdpb;

  fuNrOfGet4PerFee = fUnpackPar->GetNrOfGet4PerFee();
  LOG(info) << "Nr. of GET4 per Tof FEE: " << fuNrOfGet4PerFee;

  fuNrOfChannelsPerGet4 = fUnpackPar->GetNrOfChannelsPerGet4();
  LOG(info) << "Nr. of channels per GET4: " << fuNrOfChannelsPerGet4;

  fuNrOfChannelsPerFee = fuNrOfGet4PerFee * fuNrOfChannelsPerGet4;
  LOG(info) << "Nr. of channels per FEE: " << fuNrOfChannelsPerFee;

  fuNrOfGet4 = fuNrOfGdpbs * fuNrOfFeePerGdpb * fuNrOfGet4PerFee;
  LOG(info) << "Nr. of GET4s: " << fuNrOfGet4;

  fuNrOfGet4PerGdpb = fuNrOfFeePerGdpb * fuNrOfGet4PerFee;
  LOG(info) << "Nr. of GET4s per GDPB: " << fuNrOfGet4PerGdpb;

  fuNrOfChannelsPerGdpb = fuNrOfGet4PerGdpb * fuNrOfChannelsPerGet4;
  LOG(info) << "Nr. of channels per GDPB: " << fuNrOfChannelsPerGdpb;

  fGdpbIdIndexMap.clear();
  for (UInt_t i = 0; i < fuNrOfGdpbs; ++i) {
    fGdpbIdIndexMap[fUnpackPar->GetGdpbId(i)] = i;
    LOG(info) << "GDPB Id of TOF  " << i << " : " << std::hex
              << fUnpackPar->GetGdpbId(i) << std::dec;
  }  // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )

  fuNrOfGbtx = fUnpackPar->GetNrOfGbtx();
  LOG(info) << "Nr. of GBTx: " << fuNrOfGbtx;

  fuNrOfModules = fUnpackPar->GetNrOfModules();
  LOG(info) << "Nr. of GBTx: " << fuNrOfModules;

  fviRpcType.resize(fuNrOfGbtx);
  fviModuleId.resize(fuNrOfGbtx);
  fviNrOfRpc.resize(fuNrOfGbtx);
  fviRpcSide.resize(fuNrOfGbtx);
  for (UInt_t uGbtx = 0; uGbtx < fuNrOfGbtx; ++uGbtx) {
    fviNrOfRpc[uGbtx]  = fUnpackPar->GetNrOfRpc(uGbtx);
    fviRpcType[uGbtx]  = fUnpackPar->GetRpcType(uGbtx);
    fviRpcSide[uGbtx]  = fUnpackPar->GetRpcSide(uGbtx);
    fviModuleId[uGbtx] = fUnpackPar->GetModuleId(uGbtx);
  }  // for( UInt_t uGbtx = 0; uGbtx < uNrOfGbtx; ++uGbtx)

  TString sPrintoutLine = "Nr. of RPCs per GBTx: ";
  for (UInt_t uGbtx = 0; uGbtx < fuNrOfGbtx; ++uGbtx)
    sPrintoutLine += Form(" %2d", fviNrOfRpc[uGbtx]);
  LOG(info) << sPrintoutLine;

  sPrintoutLine = "RPC type per GBTx:    ";
  for (UInt_t uGbtx = 0; uGbtx < fuNrOfGbtx; ++uGbtx)
    sPrintoutLine += Form(" %2d", fviRpcType[uGbtx]);
  LOG(info) << sPrintoutLine;

  sPrintoutLine = "RPC side per GBTx:    ";
  for (UInt_t uGbtx = 0; uGbtx < fuNrOfGbtx; ++uGbtx)
    sPrintoutLine += Form(" %2d", fviRpcSide[uGbtx]);
  LOG(info) << sPrintoutLine;

  sPrintoutLine = "Module ID per GBTx:   ";
  for (UInt_t uGbtx = 0; uGbtx < fuNrOfGbtx; ++uGbtx)
    sPrintoutLine += Form(" %2d", fviModuleId[uGbtx]);
  LOG(info) << sPrintoutLine;

  fuTotalMsNb      = fUnpackPar->GetNbMsTot();
  fuOverlapMsNb    = fUnpackPar->GetNbMsOverlap();
  fuCoreMs         = fuTotalMsNb - fuOverlapMsNb;
  fdMsSizeInNs     = fUnpackPar->GetSizeMsInNs();
  fdTsCoreSizeInNs = fdMsSizeInNs * fuCoreMs;
  LOG(info) << "Timeslice parameters: " << fuTotalMsNb
            << " MS per link, of which " << fuOverlapMsNb
            << " overlap MS, each MS is " << fdMsSizeInNs << " ns";

  /// STAR Trigger decoding and monitoring
  fvulGdpbTsMsb.resize(fuNrOfGdpbs);
  fvulGdpbTsLsb.resize(fuNrOfGdpbs);
  fvulStarTsMsb.resize(fuNrOfGdpbs);
  fvulStarTsMid.resize(fuNrOfGdpbs);
  fvulGdpbTsFullLast.resize(fuNrOfGdpbs);
  fvulStarTsFullLast.resize(fuNrOfGdpbs);
  fvuStarTokenLast.resize(fuNrOfGdpbs);
  fvuStarDaqCmdLast.resize(fuNrOfGdpbs);
  fvuStarTrigCmdLast.resize(fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fvulGdpbTsMsb[uGdpb]      = 0;
    fvulGdpbTsLsb[uGdpb]      = 0;
    fvulStarTsMsb[uGdpb]      = 0;
    fvulStarTsMid[uGdpb]      = 0;
    fvulGdpbTsFullLast[uGdpb] = 0;
    fvulStarTsFullLast[uGdpb] = 0;
    fvuStarTokenLast[uGdpb]   = 0;
    fvuStarDaqCmdLast[uGdpb]  = 0;
    fvuStarTrigCmdLast[uGdpb] = 0;
  }  // for (Int_t iGdpb = 0; iGdpb < fuNrOfGdpbs; ++iGdpb)

  fvmEpSupprBuffer.resize(fuNrOfGet4);

  ///* Pulser monitoring *///
  if (kTRUE == fbPulserModeEnable) {
    fvuFeeNbHitsLastMs.resize(fuNrOfFeePerGdpb * fuNrOfGdpbs, 0);
    fdTsLastPulserHit.resize(fuNrOfFeePerGdpb * fuNrOfGdpbs, 0.0);
  }  // if( kTRUE == fbPulserModeEnable )

  ///* coincidence maps *///
  if (kTRUE == fbCoincMapsEnable) {
    fvuCoincNbHitsLastMs.resize(fuNrOfGdpbs);
    fvdCoincTsLastHit.resize(fuNrOfGdpbs);
    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
      fvuCoincNbHitsLastMs[uGdpb].resize(fuNrOfChannelsPerGdpb, 0);
      fvdCoincTsLastHit[uGdpb].resize(fuNrOfChannelsPerGdpb, 0.0);
    }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  }    // if( kTRUE == fbCoincMapsEnable )

  /// TODO: move these constants somewhere shared, e.g the parameter file
  fvuPadiToGet4.resize(fuNrOfChannelsPerFee);
  fvuGet4ToPadi.resize(fuNrOfChannelsPerFee);
  /*
   UInt_t uGet4topadi[32] = {
        4,  3,  2,  1,  // provided by Jochen
      24, 23, 22, 21,
       8,  7,  6,  5,
      28, 27, 26, 25,
      12, 11, 10,  9,
      32, 31, 30, 29,
      16, 15, 14, 13,
      20, 19, 18, 17 };
*/

  /// From NH files, for Fall 2018 detectors
  UInt_t uGet4topadi[32] = {4,  3,  2,  1,  // provided by Jochen
                            8,  7,  6,  5,  12, 11, 10, 9,  16, 15,
                            14, 13, 20, 19, 18, 17, 24, 23, 22, 21,
                            28, 27, 26, 25, 32, 31, 30, 29};


  UInt_t uPaditoget4[32] = {4,  3,  2,  1,  // provided by Jochen
                            12, 11, 10, 9,  20, 19, 18, 17, 28, 27,
                            26, 25, 32, 31, 30, 29, 8,  7,  6,  5,
                            16, 15, 14, 13, 24, 23, 22, 21};

  for (UInt_t uChan = 0; uChan < fuNrOfChannelsPerFee; ++uChan) {
    fvuPadiToGet4[uChan] = uPaditoget4[uChan] - 1;
    fvuGet4ToPadi[uChan] = uGet4topadi[uChan] - 1;
  }  // for( UInt_t uChan = 0; uChan < fuNrOfChannelsPerFee; ++uChan )


  /// TODO: move these constants somewhere shared, e.g the parameter file
  fvuElinkToGet4.resize(kuNbGet4PerGbtx);
  fvuGet4ToElink.resize(kuNbGet4PerGbtx);
  UInt_t kuElinkToGet4[kuNbGet4PerGbtx] = {
    27, 2,  7,  3,  31, 26, 30, 1,  33, 37, 32, 13, 9,  14,
    10, 15, 17, 21, 16, 35, 34, 38, 25, 24, 0,  6,  20, 23,
    18, 22, 28, 4,  29, 5,  19, 36, 39, 8,  12, 11};
  UInt_t kuGet4ToElink[kuNbGet4PerGbtx] = {
    24, 7,  1,  3,  31, 33, 25, 2,  37, 12, 14, 39, 38, 11,
    13, 15, 18, 16, 28, 34, 26, 17, 29, 27, 23, 22, 5,  0,
    30, 32, 6,  4,  10, 8,  20, 19, 35, 9,  21, 36};

  for (UInt_t uLinkAsic = 0; uLinkAsic < kuNbGet4PerGbtx; ++uLinkAsic) {
    fvuElinkToGet4[uLinkAsic] = kuElinkToGet4[uLinkAsic];
    fvuGet4ToElink[uLinkAsic] = kuGet4ToElink[uLinkAsic];
  }  // for( UInt_t uChan = 0; uChan < fuNrOfChannelsPerFee; ++uChan )

  return kTRUE;
}


void CbmMcbm2018MonitorTof::AddMsComponentToList(size_t component,
                                                 UShort_t /*usDetectorId*/) {
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Add to list
  fvMsComponentsList.push_back(component);

  /// Create MS size monitoring histos
  if (NULL == fvhMsSzPerLink[component]) {
    TString sMsSzName = Form("MsSz_link_%02lu", component);
    TString sMsSzTitle =
      Form("Size of MS from link %02lu; Ms Size [bytes]", component);
    fvhMsSzPerLink[component] =
      new TH1F(sMsSzName.Data(), sMsSzTitle.Data(), 160000, 0., 20000.);

    sMsSzName  = Form("MsSzTime_link_%02lu", component);
    sMsSzTitle = Form(
      "Size of MS vs time for gDPB of link %02lu; Time[s] ; Ms Size [bytes]",
      component);
    fvhMsSzTimePerLink[component] = new TProfile(sMsSzName.Data(),
                                                 sMsSzTitle.Data(),
                                                 100 * fuHistoryHistoSize,
                                                 0.,
                                                 2 * fuHistoryHistoSize);
    THttpServer* server           = FairRunOnline::Instance()->GetHttpServer();
    if (server) {
      server->Register("/FlibRaw", fvhMsSzPerLink[component]);
      server->Register("/FlibRaw", fvhMsSzTimePerLink[component]);
    }  // if( server )
    if (NULL != fcMsSizeAll) {
      fcMsSizeAll->cd(1 + component);
      gPad->SetLogy();
      fvhMsSzTimePerLink[component]->Draw("hist le0");
    }  // if( NULL != fcMsSizeAll )
    LOG(info) << "Added MS size histo for component (link): " << component;
  }  // if( NULL == fvhMsSzPerLink[ component ] )
}
void CbmMcbm2018MonitorTof::SetNbMsInTs(size_t uCoreMsNb, size_t uOverlapMsNb) {
  fuNbCoreMsPerTs = uCoreMsNb;
  fuNbOverMsPerTs = uOverlapMsNb;

  //   UInt_t uNbMsTotal = fuNbCoreMsPerTs + fuNbOverMsPerTs;
}

void CbmMcbm2018MonitorTof::CreateHistograms() {
  LOG(info) << "create Histos for " << fuNrOfGdpbs << " gDPBs ";

  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

  TString name {""};
  TString title {""};

  // Full Fee time difference test
  UInt_t uNbBinsDt =
    kuNbBinsDt
    + 1;  // To account for extra bin due to shift by 1/2 bin of both ranges

  fuNbFeePlotsPerGdpb = fuNrOfFeePerGdpb / fuNbFeePlot
                        + (0 != fuNrOfFeePerGdpb % fuNbFeePlot ? 1 : 0);
  Double_t dBinSzG4v2 = (6250. / 112.);
  dMinDt              = -1. * (kuNbBinsDt * dBinSzG4v2 / 2.) - dBinSzG4v2 / 2.;
  dMaxDt              = 1. * (kuNbBinsDt * dBinSzG4v2 / 2.) + dBinSzG4v2 / 2.;

  /*******************************************************************/
  name  = "hMessageType";
  title = "Nb of message for each type; Type";
  // Test Big Data readout with plotting
  fhMessType = new TH1I(name,
                        title,
                        1 + gdpbv100::MSG_STAR_TRI_D,
                        0.,
                        1 + gdpbv100::MSG_STAR_TRI_D);
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_HIT, "HIT");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_EPOCH, "EPOCH");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_SLOWC, "SLOWC");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_SYST, "SYST");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_A, "TRI_A");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_B, "TRI_B");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_C, "TRI_C");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_D, "TRI_D");

  /*******************************************************************/
  name          = "hSysMessType";
  title         = "Nb of system message for each type; System Type";
  fhSysMessType = new TH1I(
    name, title, 1 + gdpbv100::SYS_PATTERN, 0., 1 + gdpbv100::SYS_PATTERN);
  fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GET4_ERROR,
                                         "GET4 ERROR");
  fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GDPB_UNKWN,
                                         "UNKW GET4 MSG");
  fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GET4_SYNC_MISS,
                                         "SYS_GET4_SYNC_MISS");
  fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_PATTERN,
                                         "SYS_PATTERN");

  /*******************************************************************/
  name           = "hGet4MessType";
  title          = "Nb of message for each type per GET4; GET4 chip # ; Type";
  fhGet4MessType = new TH2I(name, title, fuNrOfGet4, 0., fuNrOfGet4, 4, 0., 4.);
  fhGet4MessType->GetYaxis()->SetBinLabel(1, "DATA 32b");
  fhGet4MessType->GetYaxis()->SetBinLabel(2, "EPOCH");
  fhGet4MessType->GetYaxis()->SetBinLabel(3, "S.C. M");
  fhGet4MessType->GetYaxis()->SetBinLabel(4, "ERROR");
  //   fhGet4MessType->GetYaxis()->SetBinLabel( 5, "DATA 24b");
  //   fhGet4MessType->GetYaxis()->SetBinLabel( 6, "STAR Trigger");

  /*******************************************************************/
  name          = "hGet4ChanScm";
  title         = "SC messages per GET4 channel; GET4 channel # ; SC type";
  fhGet4ChanScm = new TH2I(name,
                           title,
                           2 * fuNrOfGet4 * fuNrOfChannelsPerGet4,
                           0.,
                           fuNrOfGet4 * fuNrOfChannelsPerGet4,
                           5,
                           0.,
                           5.);
  fhGet4ChanScm->GetYaxis()->SetBinLabel(1, "Hit Scal");
  fhGet4ChanScm->GetYaxis()->SetBinLabel(2, "Deadtime");
  fhGet4ChanScm->GetYaxis()->SetBinLabel(3, "SPI");
  fhGet4ChanScm->GetYaxis()->SetBinLabel(4, "SEU Scal");
  fhGet4ChanScm->GetYaxis()->SetBinLabel(5, "START");

  /*******************************************************************/
  name             = "hGet4ChanErrors";
  title            = "Error messages per GET4 channel; GET4 channel # ; Error";
  fhGet4ChanErrors = new TH2I(name,
                              title,
                              fuNrOfGet4 * fuNrOfChannelsPerGet4,
                              0.,
                              fuNrOfGet4 * fuNrOfChannelsPerGet4,
                              21,
                              0.,
                              21.);
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(1, "0x00: Readout Init    ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(2, "0x01: Sync            ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(3, "0x02: Epoch count sync");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(4, "0x03: Epoch           ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(5, "0x04: FIFO Write      ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(6, "0x05: Lost event      ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(7, "0x06: Channel state   ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(8, "0x07: Token Ring state");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(9, "0x08: Token           ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(10, "0x09: Error Readout   ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(11, "0x0a: SPI             ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(
    12, "0x0b: DLL Lock error  ");  // <- From GET4 v1.2
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(
    13, "0x0c: DLL Reset invoc.");  // <- From GET4 v1.2
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(14, "0x11: Overwrite       ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(15, "0x12: ToT out of range");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(16, "0x13: Event Discarded ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(
    17, "0x14: Add. Rising edge");  // <- From GET4 v1.3
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(
    18, "0x15: Unpaired Falling");  // <- From GET4 v1.3
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(
    19, "0x16: Sequence error  ");  // <- From GET4 v1.3
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(20, "0x7f: Unknown         ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(21, "Corrupt/unsuprtd error");

  /*******************************************************************/
  name  = "hGet4EpochFlags";
  title = "Epoch flags per GET4; GET4 chip # ; Type";
  fhGet4EpochFlags =
    new TH2I(name, title, fuNrOfGet4, 0., fuNrOfGet4, 4, 0., 4.);
  fhGet4EpochFlags->GetYaxis()->SetBinLabel(1, "SYNC");
  fhGet4EpochFlags->GetYaxis()->SetBinLabel(2, "Ep LOSS");
  fhGet4EpochFlags->GetYaxis()->SetBinLabel(3, "Da LOSS");
  fhGet4EpochFlags->GetYaxis()->SetBinLabel(4, "MISSMAT");

  /*******************************************************************/
  name  = "hGdpbMessageType";
  title = "Nb of message for each type per Gdpb; Type; Gdpb Idx []";
  // Test Big Data readout with plotting
  fhGdpbMessType = new TH2I(name,
                            title,
                            1 + gdpbv100::MSG_STAR_TRI_D,
                            0.,
                            1 + gdpbv100::MSG_STAR_TRI_D,
                            fuNrOfGdpbs,
                            -0.5,
                            fuNrOfGdpbs - 0.5);
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_HIT, "HIT");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_EPOCH, "EPOCH");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_SLOWC, "SLOWC");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_SYST, "SYST");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_A,
                                          "TRI_A");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_B,
                                          "TRI_B");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_C,
                                          "TRI_C");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_D,
                                          "TRI_D");

  /*******************************************************************/
  name = "hGdpbSysMessType";
  title =
    "Nb of system message for each type per Gdpb; System Type; Gdpb Idx []";
  fhGdpbSysMessType = new TH2I(name,
                               title,
                               1 + gdpbv100::SYS_PATTERN,
                               0.,
                               1 + gdpbv100::SYS_PATTERN,
                               fuNrOfGdpbs,
                               -0.5,
                               fuNrOfGdpbs - 0.5);
  fhGdpbSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GET4_ERROR,
                                             "GET4 ERROR");
  fhGdpbSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GDPB_UNKWN,
                                             "UNKW GET4 MSG");
  fhGdpbSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GET4_SYNC_MISS,
                                             "SYS_GET4_SYNC_MISS");
  fhGdpbSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_PATTERN,
                                             "SYS_PATTERN");

  /*******************************************************************/
  name = "hGdpbSysMessPattType";
  title =
    "Nb of pattern message for each type per Gdpb; Pattern Type; Gdpb Idx []";
  fhGdpbSysMessPattType = new TH2I(name,
                                   title,
                                   1 + gdpbv100::PATT_RESYNC,
                                   0.,
                                   1 + gdpbv100::PATT_RESYNC,
                                   fuNrOfGdpbs,
                                   -0.5,
                                   fuNrOfGdpbs - 0.5);
  fhGdpbSysMessPattType->GetXaxis()->SetBinLabel(1 + gdpbv100::PATT_MISSMATCH,
                                                 "PATT_MISSMATCH");
  fhGdpbSysMessPattType->GetXaxis()->SetBinLabel(1 + gdpbv100::PATT_ENABLE,
                                                 "PATT_ENABLE");
  fhGdpbSysMessPattType->GetXaxis()->SetBinLabel(1 + gdpbv100::PATT_RESYNC,
                                                 "PATT_RESYNC");

  /*******************************************************************/
  name  = "hGdpbEpochFlags";
  title = "Epoch flags per gDPB; gDPB # ; Type";
  fhGdpbEpochFlags =
    new TH2I(name, title, fuNrOfGdpbs, 0., fuNrOfGdpbs, 4, 0., 4.);
  fhGdpbEpochFlags->GetYaxis()->SetBinLabel(1, "SYNC");
  fhGdpbEpochFlags->GetYaxis()->SetBinLabel(2, "Ep LOSS");
  fhGdpbEpochFlags->GetYaxis()->SetBinLabel(3, "Da LOSS");
  fhGdpbEpochFlags->GetYaxis()->SetBinLabel(4, "MISSMAT");

  /*******************************************************************/
  name  = Form("hGdpbEpochSyncEvo");
  title = Form("Epoch SYNC per second and gDPB; Time[s];  gDPB #; SYNC Nb");
  fhGdpbEpochSyncEvo = new TH2D(name.Data(),
                                title.Data(),
                                fuHistoryHistoSize,
                                0,
                                fuHistoryHistoSize,
                                fuNrOfGdpbs,
                                0.,
                                fuNrOfGdpbs);

  /*******************************************************************/
  name = Form("hGdpbEpochMissEvo");
  title =
    Form("Epoch Missmatch per second and gDPB; Time[s];  gDPB #; Missmatch Nb");
  fhGdpbEpochMissEvo = new TH2D(name.Data(),
                                title.Data(),
                                fuHistoryHistoSize,
                                0,
                                fuHistoryHistoSize,
                                fuNrOfGdpbs,
                                0.,
                                fuNrOfGdpbs);

  /*******************************************************************/
  // Slow control messages analysis
  name = "hScmScalerCounters";
  title =
    "Content of Scaler counter SC messages; Scaler counter [hit]; Channel []";
  fhScmScalerCounters = new TH2I(name,
                                 title,
                                 fuNrOfGet4 * fuNrOfChannelsPerGet4 * 2,
                                 0.,
                                 fuNrOfGet4 * fuNrOfChannelsPerGet4,
                                 8192,
                                 0.,
                                 8192.);

  name  = "hScmDeadtimeCounters";
  title = "Content of Deadtime counter SC messages; Deadtime [Clk Cycles]; "
          "Channel []";
  fhScmDeadtimeCounters = new TH2I(name,
                                   title,
                                   fuNrOfGet4 * fuNrOfChannelsPerGet4 * 2,
                                   0.,
                                   fuNrOfGet4 * fuNrOfChannelsPerGet4,
                                   8192,
                                   0.,
                                   8192.);

  name             = "hScmSeuCounters";
  title            = "Content of SEU counter SC messages; SEU []; Channel []";
  fhScmSeuCounters = new TH2I(name,
                              title,
                              fuNrOfGet4 * fuNrOfChannelsPerGet4 * 2,
                              0.,
                              fuNrOfGet4 * fuNrOfChannelsPerGet4,
                              8192,
                              0.,
                              8192.);

  name = "hScmSeuCountersEvo";
  title =
    "SEU counter rate from SC messages; Time in Run [s]; Channel []; SEU []";
  fhScmSeuCountersEvo = new TH2I(name,
                                 title,
                                 fuNrOfGet4 * fuNrOfChannelsPerGet4 * 2,
                                 0.,
                                 fuNrOfGet4 * fuNrOfChannelsPerGet4,
                                 fuHistoryHistoSize,
                                 0.,
                                 fuHistoryHistoSize);

  /*******************************************************************/
  name  = "hPatternMissmatch";
  title = "Missmatch pattern integral per Gdpb; ASIC Pattern []; Gdpb Idx []";
  fhPatternMissmatch = new TH2I(name,
                                title,
                                fuNrOfGet4PerGdpb,
                                0.,
                                fuNrOfGet4PerGdpb,
                                fuNrOfGdpbs,
                                -0.5,
                                fuNrOfGdpbs - 0.5);
  name               = "hPatternEnable";
  title = "Enable pattern integral per Gdpb; ASIC Pattern []; Gdpb Idx []";
  fhPatternEnable = new TH2I(name,
                             title,
                             fuNrOfGet4PerGdpb,
                             0.,
                             fuNrOfGet4PerGdpb,
                             fuNrOfGdpbs,
                             -0.5,
                             fuNrOfGdpbs - 0.5);
  name            = "hPatternResync";
  title = "Resync pattern integral per Gdpb; ASIC Pattern []; Gdpb Idx []";
  fhPatternResync = new TH2I(name,
                             title,
                             fuNrOfGet4PerGdpb,
                             0.,
                             fuNrOfGet4PerGdpb,
                             fuNrOfGdpbs,
                             -0.5,
                             fuNrOfGdpbs - 0.5);


  /*******************************************************************/

  fvvuChanNbHitsPerMs.resize(fuNrOfGdpbs);
  fvvbGdpbLastMissmatchPattern.resize(fuNrOfGdpbs);
  fvvbGdpbLastEnablePattern.resize(fuNrOfGdpbs);
  fvvbGdpbLastResyncPattern.resize(fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {

    /*******************************************************************/
    name  = Form("hGdpbGet4MessType_%02u", uGdpb);
    title = Form(
      "Nb of message for each type per GET4 in gDPB %02u; GET4 chip # ; Type",
      uGdpb);
    fvhGdpbGet4MessType.push_back(new TH2I(
      name, title, fuNrOfGet4PerGdpb, 0., fuNrOfGet4PerGdpb, 4, 0., 4.));
    fvhGdpbGet4MessType[uGdpb]->GetYaxis()->SetBinLabel(1, "DATA 32b");
    fvhGdpbGet4MessType[uGdpb]->GetYaxis()->SetBinLabel(2, "EPOCH");
    fvhGdpbGet4MessType[uGdpb]->GetYaxis()->SetBinLabel(3, "S.C. M");
    fvhGdpbGet4MessType[uGdpb]->GetYaxis()->SetBinLabel(4, "ERROR");

    /*******************************************************************/
    name  = Form("hGdpbGet4ChanScm_%02u", uGdpb);
    title = Form(
      "SC messages per GET4 channel in gDPB %02u; GET4 channel # ; SC type",
      uGdpb);
    fvhGdpbGet4ChanScm.push_back(new TH2I(name,
                                          title,
                                          2 * fuNrOfChannelsPerGdpb,
                                          0.,
                                          fuNrOfChannelsPerGdpb,
                                          5,
                                          0.,
                                          5.));
    fvhGdpbGet4ChanScm[uGdpb]->GetYaxis()->SetBinLabel(1, "Hit Scal");
    fvhGdpbGet4ChanScm[uGdpb]->GetYaxis()->SetBinLabel(2, "Deadtime");
    fvhGdpbGet4ChanScm[uGdpb]->GetYaxis()->SetBinLabel(3, "SPI");
    fvhGdpbGet4ChanScm[uGdpb]->GetYaxis()->SetBinLabel(4, "SEU Scal");
    fvhGdpbGet4ChanScm[uGdpb]->GetYaxis()->SetBinLabel(5, "START");

    /*******************************************************************/
    name  = Form("hGdpbGet4ChanErrors_%02u", uGdpb);
    title = Form(
      "Error messages per GET4 channel in gDPB %02u; GET4 channel # ; Error",
      uGdpb);
    fvhGdpbGet4ChanErrors.push_back(new TH2I(name,
                                             title,
                                             fuNrOfChannelsPerGdpb,
                                             0.,
                                             fuNrOfChannelsPerGdpb,
                                             22,
                                             0.,
                                             22.));
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      1, "0x00: Readout Init    ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      2, "0x01: Sync            ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      3, "0x02: Epoch count sync");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      4, "0x03: Epoch           ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      5, "0x04: FIFO Write      ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      6, "0x05: Lost event      ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      7, "0x06: Channel state   ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      8, "0x07: Token Ring state");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      9, "0x08: Token           ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      10, "0x09: Error Readout   ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      11, "0x0a: SPI             ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      12, "0x0b: DLL Lock error  ");  // <- From GET4 v1.2
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      13, "0x0c: DLL Reset invoc.");  // <- From GET4 v1.2
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      14, "0x11: Overwrite       ");  // <- From GET4 v1.0 to 1.3
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      15, "0x12: ToT out of range");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      16, "0x13: Event Discarded ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      17, "0x14: Add. Rising edge");  // <- From GET4 v1.3
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      18, "0x15: Unpaired Falling");  // <- From GET4 v1.3
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      19, "0x16: Sequence error  ");  // <- From GET4 v1.3
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      20, "0x17: Epoch Overflow  ");  // <- From GET4 v2.0
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      21, "0x7f: Unknown         ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(
      22, "Corrupt/unsuprtd error");

    /*******************************************************************/
    name = Form("hGdpbPatternMissmatchEvo_%02u", uGdpb);
    title =
      Form("Missmatch pattern vs TS index in gDPB %02u; TS # ; ASIC Pattern []",
           uGdpb);
    fvhGdpbPatternMissmatchEvo.push_back(new TH2I(name,
                                                  title,
                                                  10000,
                                                  0.,
                                                  100000,
                                                  fuNrOfGet4PerGdpb,
                                                  0.,
                                                  fuNrOfGet4PerGdpb));

    name  = Form("hGdpbPatternEnableEvo_%02u", uGdpb);
    title = Form(
      "Enable pattern vs TS index in gDPB %02u; TS # ; ASIC Pattern []", uGdpb);
    fvhGdpbPatternEnableEvo.push_back(new TH2I(name,
                                               title,
                                               10000,
                                               0.,
                                               100000,
                                               fuNrOfGet4PerGdpb,
                                               0.,
                                               fuNrOfGet4PerGdpb));

    name  = Form("hGdpbPatternResyncEvo%02u", uGdpb);
    title = Form(
      "Resync pattern vs TS index in gDPB %02u; TS # ; ASIC Pattern []", uGdpb);
    fvhGdpbPatternResyncEvo.push_back(new TH2I(name,
                                               title,
                                               10000,
                                               0.,
                                               100000,
                                               fuNrOfGet4PerGdpb,
                                               0.,
                                               fuNrOfGet4PerGdpb));

    fvvbGdpbLastMissmatchPattern[uGdpb].resize(fuNrOfGet4PerGdpb, kFALSE);
    fvvbGdpbLastEnablePattern[uGdpb].resize(fuNrOfGet4PerGdpb, kTRUE);
    fvvbGdpbLastResyncPattern[uGdpb].resize(fuNrOfGet4PerGdpb, kFALSE);

    name = Form("hGdpbMissmatchEvoPerTs%02u", uGdpb);
    title =
      Form("Missmatch vs TS index in gDPB %02u; TS # ; Asic []; Missmatch? []",
           uGdpb);
    fvhGdpbMissmatchEvoPerTs.push_back(new TH2I(name,
                                                title,
                                                10000,
                                                0.,
                                                100000,
                                                fuNrOfGet4PerGdpb,
                                                0.,
                                                fuNrOfGet4PerGdpb));
    name  = Form("hGdpbMissmatchEnaEvoPerTs%02u", uGdpb);
    title = Form("Enable+Missmatch vs TS index in gDPB %02u; TS # ; Asic []; "
                 "Enabled & Missmatch? []",
                 uGdpb);
    fvhGdpbMissmatchEnaEvoPerTs.push_back(new TH2I(name,
                                                   title,
                                                   10000,
                                                   0.,
                                                   100000,
                                                   fuNrOfGet4PerGdpb,
                                                   0.,
                                                   fuNrOfGet4PerGdpb));

    name  = Form("hGdpbEnableEvoPerTs%02u", uGdpb);
    title = Form("Enable vs TS index in gDPB %02u; TS # ; Asic []; Enabled? []",
                 uGdpb);
    fvhGdpbEnableEvoPerTs.push_back(new TH2I(name,
                                             title,
                                             100000,
                                             0.,
                                             100000,
                                             fuNrOfGet4PerGdpb,
                                             0.,
                                             fuNrOfGet4PerGdpb));

    name  = Form("hGdpbResyncEvoPerTs%02u", uGdpb);
    title = Form("Resync vs TS index in gDPB %02u; TS # ; Asic []; Resync? []",
                 uGdpb);
    fvhGdpbResyncEvoPerTs.push_back(new TH2I(name,
                                             title,
                                             10000,
                                             0.,
                                             100000,
                                             fuNrOfGet4PerGdpb,
                                             0.,
                                             fuNrOfGet4PerGdpb));
    name  = Form("hGdpbResyncEnaEvoPerTs%02u", uGdpb);
    title = Form("Enable+Resync vs TS index in gDPB %02u; TS # ; Asic []; "
                 "Enabled & Resync? []",
                 uGdpb);
    fvhGdpbResyncEnaEvoPerTs.push_back(new TH2I(name,
                                                title,
                                                10000,
                                                0.,
                                                100000,
                                                fuNrOfGet4PerGdpb,
                                                0.,
                                                fuNrOfGet4PerGdpb));

    name  = Form("hGdpbStateEvoPerTs%02u", uGdpb);
    title = Form("ASIC State vs TS index in gDPB %02u; TS # ; Asic []; 0 = "
                 "Off, 1 = OK, 2 = Miss, 3 = Resync, 4 = Miss + Resync []",
                 uGdpb);
    fvhGdpbStateEvoPerTs.push_back(new TH2I(name,
                                            title,
                                            100000,
                                            0.,
                                            100000,
                                            fuNrOfGet4PerGdpb,
                                            0.,
                                            fuNrOfGet4PerGdpb));

    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
    name = Form("RawFt_gDPB_%02u", uGdpb);
    title =
      Form("Raw FineTime gDPB %02u Plot 0; channel; FineTime [bin]", uGdpb);
    fvhRawFt_gDPB.push_back(new TH2F(name.Data(),
                                     title.Data(),
                                     fuNrOfChannelsPerGdpb,
                                     0,
                                     fuNrOfChannelsPerGdpb,
                                     128,
                                     0,
                                     128));

    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
    name  = Form("RawTot_gDPB_%02u", uGdpb);
    title = Form("Raw TOT gDPB %02u; channel; TOT [bin]", uGdpb);
    fvhRawTot_gDPB.push_back(new TH2F(name.Data(),
                                      title.Data(),
                                      fuNrOfChannelsPerGdpb,
                                      0,
                                      fuNrOfChannelsPerGdpb,
                                      256,
                                      0,
                                      256));

    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
    name  = Form("ChCount_gDPB_%02u", uGdpb);
    title = Form("Channel counts gDPB %02u; channel; Hits", uGdpb);
    fvhChCount_gDPB.push_back(
      new TH1I(name.Data(),
               title.Data(),
               fuNrOfFeePerGdpb * fuNrOfChannelsPerFee,
               0,
               fuNrOfFeePerGdpb * fuNrOfChannelsPerFee));

    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
    name  = Form("ChRate_gDPB_%02u", uGdpb);
    title = Form("Channel rate gDPB %02u; Time in run [s]; channel; Rate [1/s]",
                 uGdpb);
    fvhChannelRate_gDPB.push_back(
      new TH2D(name.Data(),
               title.Data(),
               fuHistoryHistoSize,
               0,
               fuHistoryHistoSize,
               fuNrOfFeePerGdpb * fuNrOfChannelsPerFee,
               0,
               fuNrOfFeePerGdpb * fuNrOfChannelsPerFee));

    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
    name  = Form("RemapTot_gDPB_%02u", uGdpb);
    title = Form("Raw TOT gDPB %02u remapped; PADI channel; TOT [bin]", uGdpb);
    fvhRemapTot_gDPB.push_back(new TH2F(name.Data(),
                                        title.Data(),
                                        fuNrOfChannelsPerGdpb,
                                        0,
                                        fuNrOfChannelsPerGdpb,
                                        256,
                                        0,
                                        256));

    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
    name = Form("RemapChCount_gDPB_%02u", uGdpb);
    title =
      Form("Channel counts gDPB %02u remapped; PADI channel; Hits", uGdpb);
    fvhRemapChCount_gDPB.push_back(
      new TH1I(name.Data(),
               title.Data(),
               fuNrOfFeePerGdpb * fuNrOfChannelsPerFee,
               0,
               fuNrOfFeePerGdpb * fuNrOfChannelsPerFee));

    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
    name  = Form("RemapChRate_gDPB_%02u", uGdpb);
    title = Form(
      "PADI channel rate gDPB %02u; Time in run [s]; PADI channel; Rate [1/s]",
      uGdpb);
    fvhRemapChRate_gDPB.push_back(
      new TH2D(name.Data(),
               title.Data(),
               fuHistoryHistoSize,
               0,
               fuHistoryHistoSize,
               fuNrOfFeePerGdpb * fuNrOfChannelsPerFee,
               0,
               fuNrOfFeePerGdpb * fuNrOfChannelsPerFee));

    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
    for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb; ++uFee) {
      name = Form("FeeRate_gDPB_g%02u_f%1u", uGdpb, uFee);
      title =
        Form("Counts per second in Fee %1u of gDPB %02u; Time[s] ; Counts",
             uFee,
             uGdpb);
      fvhFeeRate_gDPB.push_back(new TH1D(
        name.Data(), title.Data(), fuHistoryHistoSize, 0, fuHistoryHistoSize));

      name  = Form("FeeErrorRate_gDPB_g%02u_f%1u", uGdpb, uFee);
      title = Form("Error Counts per second in Fee %1u of gDPB %02u; Time[s] ; "
                   "Error Counts",
                   uFee,
                   uGdpb);
      fvhFeeErrorRate_gDPB.push_back(new TH1D(
        name.Data(), title.Data(), fuHistoryHistoSize, 0, fuHistoryHistoSize));

      name  = Form("FeeErrorRatio_gDPB_g%02u_f%1u", uGdpb, uFee);
      title = Form("Error to data ratio per second in Fee %1u of gDPB %02u; "
                   "Time[s] ; Error ratio[]",
                   uFee,
                   uGdpb);
      fvhFeeErrorRatio_gDPB.push_back(new TProfile(
        name.Data(), title.Data(), fuHistoryHistoSize, 0, fuHistoryHistoSize));

      name = Form("FeeRateLong_gDPB_g%02u_f%1u", uGdpb, uFee);
      title =
        Form("Counts per minutes in Fee %1u of gDPB %02u; Time[min] ; Counts",
             uFee,
             uGdpb);
      fvhFeeRateLong_gDPB.push_back(new TH1D(name.Data(),
                                             title.Data(),
                                             fuHistoryHistoSizeLong,
                                             0,
                                             fuHistoryHistoSizeLong));

      name  = Form("FeeErrorRateLong_gDPB_g%02u_f%1u", uGdpb, uFee);
      title = Form("Error Counts per minutes in Fee %1u of gDPB %02u; "
                   "Time[min] ; Error Counts",
                   uFee,
                   uGdpb);
      fvhFeeErrorRateLong_gDPB.push_back(new TH1D(name.Data(),
                                                  title.Data(),
                                                  fuHistoryHistoSizeLong,
                                                  0,
                                                  fuHistoryHistoSizeLong));

      name  = Form("FeeErrorRatioLong_gDPB_g%02u_f%1u", uGdpb, uFee);
      title = Form("Error to data ratio per minutes in Fee %1u of gDPB %02u; "
                   "Time[min] ; Error ratio[]",
                   uFee,
                   uGdpb);
      fvhFeeErrorRatioLong_gDPB.push_back(new TProfile(name.Data(),
                                                       title.Data(),
                                                       fuHistoryHistoSizeLong,
                                                       0,
                                                       fuHistoryHistoSizeLong));
    }  // for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb; uFee++)

    fvvuChanNbHitsPerMs[uGdpb].resize(fuNrOfChannelsPerGdpb);
    fhHitsPerMsFirstChan_gDPB.push_back(
      new TH2D(Form("hHitsPerMsFirstChan_gDPB%02u", uGdpb),
               Form("Hit Counts per MS in first channel in gDPB %02u; TS []; "
                    "Hits/MS []; MS nb[]",
                    uGdpb),
               10000,
               0,
               10000,
               150,
               -0.5,
               149.5));
    fvhChannelRatePerMs_gDPB.push_back(
      new TProfile2D(Form("hChannelRatePerMs_gDPB%02u", uGdpb),
                     Form("Mean Hit count per MS and channel vs Time in gDPB "
                          "%02u; TS []; Channel []; <Hits/Ms> []",
                          uGdpb),
                     10000,
                     0,
                     10000,
                     fuNrOfChannelsPerGdpb,
                     -0.5,
                     fuNrOfChannelsPerGdpb - 0.5));

    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
    /// STAR Trigger decoding and monitoring
    name  = Form("hTokenMsgType_gDPB_%02u", uGdpb);
    title = Form("STAR trigger Messages type gDPB %02u; Type ; Counts", uGdpb);
    fvhTokenMsgType.push_back(new TH1F(name, title, 4, 0, 4));
    fvhTokenMsgType[uGdpb]->GetXaxis()->SetBinLabel(1, "A");  // gDPB TS high
    fvhTokenMsgType[uGdpb]->GetXaxis()->SetBinLabel(
      2, "B");  // gDPB TS low, STAR TS high
    fvhTokenMsgType[uGdpb]->GetXaxis()->SetBinLabel(3, "C");  // STAR TS mid
    fvhTokenMsgType[uGdpb]->GetXaxis()->SetBinLabel(
      4, "D");  // STAR TS low, token, CMDs
    if (server) server->Register("/StarRaw", fvhTokenMsgType[uGdpb]);

    name  = Form("hTriggerRate_gDPB_%02u", uGdpb);
    title = Form("STAR trigger signals per second gDPB %02u; Time[s] ; Counts",
                 uGdpb);
    fvhTriggerRate.push_back(
      new TH1F(name, title, fuHistoryHistoSize, 0, fuHistoryHistoSize));
    if (server) server->Register("/StarRaw", fvhTriggerRate[uGdpb]);

    name = Form("hCmdDaqVsTrig_gDPB_%02u", uGdpb);
    title =
      Form("STAR daq command VS STAR trigger command gDPB %02u; DAQ ; TRIGGER",
           uGdpb);
    fvhCmdDaqVsTrig.push_back(new TH2I(name, title, 16, 0, 16, 16, 0, 16));
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(
      1, "0x0: no-trig ");  // idle link
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(
      2, "0x1: clear   ");  // clears redundancy counters on the readout boards
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(
      3, "0x2: mast-rst");  // general reset of the whole front-end logic
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(
      4, "0x3: spare   ");  // reserved
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(
      5, "0x4: trigg. 0");  // Default physics readout, all det support required
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(6, "0x5: trigg. 1");   //
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(7, "0x6: trigg. 2");   //
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(8, "0x7: trigg. 3");   //
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(9, "0x8: puls.  0");   //
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(10, "0x9: puls.  1");  //
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(11, "0xA: puls.  2");  //
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(12, "0xB: puls.  3");  //
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(
      13,
      "0xC: config  ");  // housekeeping trigger: return geographic info of FE
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(
      14, "0xD: abort   ");  // aborts and clears an active event
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(15, "0xE: L1accept");  //
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(16, "0xF: L2accept");  //
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(
      1, "0x0:  0");  // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(
      2, "0x1:  1");  // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(
      3, "0x2:  2");  // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(
      4, "0x3:  3");  // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(
      5, "0x4:  4");  // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(
      6, "0x5:  5");  // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(
      7, "0x6:  6");  // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(
      8, "0x7:  7");  // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(
      9, "0x8:  8");  // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(
      10, "0x9:  9");  // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(
      11, "0xA: 10");  // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(
      12, "0xB: 11");  // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(
      13, "0xC: 12");  // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(
      14, "0xD: 13");  // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(
      15, "0xE: 14");  // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(
      16, "0xF: 15");  // To be filled at STAR
    if (server) server->Register("/StarRaw", fvhCmdDaqVsTrig[uGdpb]);

    name  = Form("hStarTokenEvo_gDPB_%02u", uGdpb);
    title = Form("STAR token value VS time gDPB %02u; Time in Run [s] ; STAR "
                 "Token; Counts",
                 uGdpb);
    fvhStarTokenEvo.push_back(new TH2I(
      name, title, fuHistoryHistoSize, 0, fuHistoryHistoSize, 410, 0, 4100));


    name  = Form("hStarTrigGdpbTsEvo_gDPB_%02u", uGdpb);
    title = Form(
      "gDPB TS in STAR triger tokens for gDPB %02u; Time in Run [s] ; gDPB TS;",
      uGdpb);
    fvhStarTrigGdpbTsEvo.push_back(
      new TProfile(name, title, fuHistoryHistoSize, 0, fuHistoryHistoSize));

    name  = Form("hStarTrigStarTsEvo_gDPB_%02u", uGdpb);
    title = Form(
      "STAR TS in STAR triger tokens for gDPB %02u; Time in Run [s] ; STAR TS;",
      uGdpb);
    fvhStarTrigStarTsEvo.push_back(
      new TProfile(name, title, fuHistoryHistoSize, 0, fuHistoryHistoSize));
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  /*******************************************************************/
  /// Module TOT map
  /*
   for( UInt_t uMod = 0; uMod < fuNrOfModules; uMod ++ )
   {
      name = Form("RemapTotSideA_mod_%02u", uMod);
      title = Form("Raw TOT module %02u Side A; PADI channel; TOT [bin]", uMod);
      fvhRemapTotSideA_mod.push_back(
         new TH2F(name.Data(), title.Data(),
            kuNbFeeSide * fuNrOfChannelsPerFee, 0, kuNbFeeSide * fuNrOfChannelsPerFee,
            256, 0, 256 ) );
      name = Form("RemapTotSideB_mod_%02u", uMod);
      title = Form("Raw TOT module %02u Side B; PADI channel; TOT [bin]", uMod);
      fvhRemapTotSideB_mod.push_back(
         new TH2F(name.Data(), title.Data(),
            kuNbFeeSide * fuNrOfChannelsPerFee, 0, kuNbFeeSide * fuNrOfChannelsPerFee,
            256, 0, 256 ) );

      name = Form("ModRate_gDPB_m%02u", uMod);
      title = Form( "Counts per second in Module %02u; Time[s] ; Counts", uMod);
      fvhModRate.push_back( new TH1D(name.Data(), title.Data(), fuHistoryHistoSize, 0, fuHistoryHistoSize) );

      name = Form("ModErrorRate_m%02u", uMod);
      title = Form( "Error Counts per second in Module %02u; Time[s] ; Error Counts", uMod);
      fvhModErrorRate.push_back( new TH1D(name.Data(), title.Data(), fuHistoryHistoSize, 0, fuHistoryHistoSize) );

      name = Form("ModErrorRatio_m%02u", uMod);
      title = Form( "Error to data ratio per second in Module %02u; Time[s] ; Error ratio[]", uMod);
      fvhModErrorRatio.push_back( new TProfile(name.Data(), title.Data(), fuHistoryHistoSize, 0, fuHistoryHistoSize) );
   } // for( UInt_t uMod = 0; uMod < fuNrOfModules; uMod ++ )
*/
  /*******************************************************************/
  /// FEE pulser test channels
  if (kTRUE == fbPulserModeEnable) {
    fvhTimeDiffPulser.resize(fuNrOfFeePerGdpb * fuNrOfGdpbs);
    for (UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeA++) {
      fvhTimeDiffPulser[uFeeA].resize(fuNrOfFeePerGdpb * fuNrOfGdpbs);
      for (UInt_t uFeeB = 0; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++) {
        if (uFeeA < uFeeB) {
          UInt_t uGdpbA  = uFeeA / (fuNrOfFeePerGdpb);
          UInt_t uFeeIdA = uFeeA - (fuNrOfFeePerGdpb * uGdpbA);
          UInt_t uGdpbB  = uFeeB / (fuNrOfFeePerGdpb);
          UInt_t uFeeIdB = uFeeB - (fuNrOfFeePerGdpb * uGdpbB);
          fvhTimeDiffPulser[uFeeA][uFeeB] =
            new TH1I(Form("hTimeDiffPulser_g%02u_f%1u_g%02u_f%1u",
                          uGdpbA,
                          uFeeIdA,
                          uGdpbB,
                          uFeeIdB),
                     Form("Time difference for pulser on gDPB %02u FEE %1u and "
                          "gDPB %02u FEE %1u; DeltaT [ps]; Counts",
                          uGdpbA,
                          uFeeIdA,
                          uGdpbB,
                          uFeeIdB),
                     uNbBinsDt,
                     dMinDt,
                     dMaxDt);
        }  // if( uFeeA < uFeeB )
        else
          fvhTimeDiffPulser[uFeeA][uFeeB] = NULL;
      }  // for( UInt_t uFeeB = uFeeA; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs - 1; uFeeB++)
    }    // for( UInt_t uFeeA = 0; uFeeA < kuNbChanTest - 1; uFeeA++)

    name             = "hTimeMeanPulser";
    fhTimeMeanPulser = new TH2D(
      name.Data(),
      "Time difference Mean for each FEE pairs; FEE A; FEE B ; Mean [ps]",
      fuNrOfFeePerGdpb * fuNrOfGdpbs - 1,
      -0.5,
      fuNrOfFeePerGdpb * fuNrOfGdpbs - 1.5,
      fuNrOfFeePerGdpb * fuNrOfGdpbs - 1,
      0.5,
      fuNrOfFeePerGdpb * fuNrOfGdpbs - 0.5);

    name            = "hTimeRmsPulser";
    fhTimeRmsPulser = new TH2D(
      name.Data(),
      "Time difference RMS for each FEE pairs; FEE A; FEE B ; RMS [ps]",
      fuNrOfFeePerGdpb * fuNrOfGdpbs - 1,
      -0.5,
      fuNrOfFeePerGdpb * fuNrOfGdpbs - 1.5,
      fuNrOfFeePerGdpb * fuNrOfGdpbs - 1,
      0.5,
      fuNrOfFeePerGdpb * fuNrOfGdpbs - 0.5);

    name                 = "hTimeRmsZoomFitPuls";
    fhTimeRmsZoomFitPuls = new TH2D(name.Data(),
                                    "Time difference RMS after zoom for each "
                                    "FEE pairs; FEE A; FEE B ; RMS [ps]",
                                    fuNrOfFeePerGdpb * fuNrOfGdpbs - 1,
                                    -0.5,
                                    fuNrOfFeePerGdpb * fuNrOfGdpbs - 1.5,
                                    fuNrOfFeePerGdpb * fuNrOfGdpbs - 1,
                                    0.5,
                                    fuNrOfFeePerGdpb * fuNrOfGdpbs - 0.5);

    name             = "hTimeResFitPuls";
    fhTimeResFitPuls = new TH2D(name.Data(),
                                "Time difference Res from fit for each FEE "
                                "pairs; FEE A; FEE B ; Sigma [ps]",
                                fuNrOfFeePerGdpb * fuNrOfGdpbs - 1,
                                -0.5,
                                fuNrOfFeePerGdpb * fuNrOfGdpbs - 1.5,
                                fuNrOfFeePerGdpb * fuNrOfGdpbs - 1,
                                0.5,
                                fuNrOfFeePerGdpb * fuNrOfGdpbs - 0.5);

    fvhPulserTimeDiffEvoGbtxGbtx.resize(fuNrOfGdpbs * (kuNbGbtxPerGdpb - 1));
    fvvhPulserTimeDiffEvoGdpbGdpb.resize(fuNrOfGdpbs);
    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
      for (UInt_t uGbtx = 0; uGbtx < kuNbGbtxPerGdpb - 1; ++uGbtx) {
        name =
          Form("hPulserTimeDiffEvoGdpb%02uGbtx00Gbtx%02u", uGdpb, uGbtx + 1);
        fvhPulserTimeDiffEvoGbtxGbtx[uGdpb * (kuNbGbtxPerGdpb - 1) + uGbtx] =
          new TProfile(
            name.Data(),
            Form("Time difference of the 1st FEE in the 1st GBTx of gDPB %02u "
                 "vs GBTx %02u; time in run [min]; dt [ps]",
                 uGdpb,
                 uGbtx + 1),
            fuHistoryHistoSizeLong,
            0,
            fuHistoryHistoSizeLong);
      }  // for( UInt_t uGbtx = 0; uGbtx < kuNbGbtxPerGdpb; ++uGbtx )

      fvvhPulserTimeDiffEvoGdpbGdpb[uGdpb].resize(fuNrOfGdpbs, NULL);
      for (UInt_t uGdpbB = uGdpb + 1; uGdpbB < fuNrOfGdpbs; ++uGdpbB) {
        name = Form("hPulserTimeDiffEvoGdpb%02uGdpb%02u", uGdpb, uGdpbB);
        fvvhPulserTimeDiffEvoGdpbGdpb[uGdpb][uGdpbB] =
          new TProfile(name.Data(),
                       Form("Time difference of the 1st FEE in the 1st GBTx of "
                            "gDPB %02u vs %02u; time in run [min]; dt [ps]",
                            uGdpb,
                            uGdpbB),
                       fuHistoryHistoSizeLong,
                       0,
                       fuHistoryHistoSizeLong);
      }  // for( UInt_t uGdpbB = uGdpb + 1; uGdpbB < fuNrOfGdpbs; ++uGdpbB )
    }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  }      // if( kTRUE == fbPulserModeEnable )

  /// Coincidence maps
  if (kTRUE == fbCoincMapsEnable) {
    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
      name = Form("hCoincMapAllChanGdpb%02u", uGdpb);
      fvhCoincMapAllChanGdpb.push_back(
        new TH2D(name.Data(),
                 Form("Coincidence map of all channels of gDPB %02u; Chan A "
                      "[]; Chan B[]; Coinc. []",
                      uGdpb),
                 fuNrOfChannelsPerGdpb,
                 -0.5,
                 fuNrOfChannelsPerGdpb - 0.5,
                 fuNrOfChannelsPerGdpb,
                 -0.5,
                 fuNrOfChannelsPerGdpb - 0.5));

      name = Form("hCoincMeanAllChanGdpb%02u", uGdpb);
      fvhCoincMeanAllChanGdpb.push_back(
        new TProfile2D(name.Data(),
                       Form("Coincidence mean of all channels of gDPB %02u; "
                            "Chan A []; Chan B[]; Mean dt [ps]",
                            uGdpb),
                       fuNrOfChannelsPerGdpb,
                       -0.5,
                       fuNrOfChannelsPerGdpb - 0.5,
                       fuNrOfChannelsPerGdpb,
                       -0.5,
                       fuNrOfChannelsPerGdpb - 0.5));
    }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  }    // if( kTRUE == fbCoincMapsEnable )

  if (server) {
    server->Register("/TofRaw", fhMessType);
    server->Register("/TofRaw", fhSysMessType);
    server->Register("/TofRaw", fhGet4MessType);
    server->Register("/TofRaw", fhGet4ChanScm);
    server->Register("/TofRaw", fhGet4ChanErrors);
    server->Register("/TofRaw", fhGet4EpochFlags);

    server->Register("/TofRaw", fhGdpbMessType);
    server->Register("/TofRaw", fhGdpbSysMessType);
    server->Register("/TofRaw", fhGdpbSysMessPattType);
    server->Register("/TofRaw", fhGdpbEpochFlags);
    server->Register("/TofRaw", fhGdpbEpochSyncEvo);
    server->Register("/TofRaw", fhGdpbEpochMissEvo);

    server->Register("/TofRaw", fhScmScalerCounters);
    server->Register("/TofRaw", fhScmDeadtimeCounters);
    server->Register("/TofRaw", fhScmSeuCounters);
    server->Register("/TofRaw", fhScmSeuCountersEvo);

    server->Register("/TofRaw", fhPatternMissmatch);
    server->Register("/TofRaw", fhPatternEnable);
    server->Register("/TofRaw", fhPatternResync);

    for (UInt_t uTotPlot = 0; uTotPlot < fvhRawTot_gDPB.size(); ++uTotPlot)
      server->Register("/TofRaw", fvhRawTot_gDPB[uTotPlot]);

    for (UInt_t uTotPlot = 0; uTotPlot < fvhRemapTot_gDPB.size(); ++uTotPlot)
      server->Register("/TofRaw", fvhRemapTot_gDPB[uTotPlot]);
    /*
      for( UInt_t uMod = 0; uMod < fuNrOfModules; uMod ++ )
      {
         server->Register("/TofRaw", fvhRemapTotSideA_mod[ uMod ] );
         server->Register("/TofRaw", fvhRemapTotSideB_mod[ uMod ] );
         server->Register("/TofMod", fvhModRate[ uMod ] );
         server->Register("/TofMod", fvhModErrorRate[ uMod ] );
         server->Register("/TofMod", fvhModErrorRatio[ uMod ] );
	   } // for( UInt_t uMod = 0; uMod < fuNrOfModules; uMod ++ )
*/
    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
      server->Register("/TofRaw", fvhGdpbGet4MessType[uGdpb]);
      server->Register("/TofRaw", fvhGdpbGet4ChanScm[uGdpb]);
      server->Register("/TofRaw", fvhGdpbGet4ChanErrors[uGdpb]);

      server->Register("/TofRaw", fvhGdpbPatternMissmatchEvo[uGdpb]);
      server->Register("/TofRaw", fvhGdpbPatternEnableEvo[uGdpb]);
      server->Register("/TofRaw", fvhGdpbPatternResyncEvo[uGdpb]);

      server->Register("/TofEna", fvhGdpbMissmatchEvoPerTs[uGdpb]);
      server->Register("/TofEna", fvhGdpbMissmatchEnaEvoPerTs[uGdpb]);
      server->Register("/TofEna", fvhGdpbEnableEvoPerTs[uGdpb]);
      server->Register("/TofEna", fvhGdpbResyncEvoPerTs[uGdpb]);
      server->Register("/TofEna", fvhGdpbResyncEnaEvoPerTs[uGdpb]);
      server->Register("/TofEna", fvhGdpbStateEvoPerTs[uGdpb]);

      server->Register("/TofRaw", fvhRawFt_gDPB[uGdpb]);
      server->Register("/TofRaw", fvhChCount_gDPB[uGdpb]);
      server->Register("/TofRates", fvhChannelRate_gDPB[uGdpb]);
      server->Register("/TofRaw", fvhRemapChCount_gDPB[uGdpb]);
      server->Register("/TofRates", fvhRemapChRate_gDPB[uGdpb]);

      for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb; ++uFee) {
        server->Register("/TofRates",
                         fvhFeeRate_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]);
        server->Register("/TofRates",
                         fvhFeeErrorRate_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]);
        server->Register(
          "/TofRates", fvhFeeErrorRatio_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]);
        server->Register("/TofRates",
                         fvhFeeRateLong_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]);
        server->Register(
          "/TofRates",
          fvhFeeErrorRateLong_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]);
        server->Register(
          "/TofRates",
          fvhFeeErrorRatioLong_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]);
      }  // for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb; ++ uFee)

      server->Register("/ChanFineRate", fhHitsPerMsFirstChan_gDPB[uGdpb]);
      server->Register("/ChanFineRate", fvhChannelRatePerMs_gDPB[uGdpb]);

      server->Register("/StarRaw", fvhTokenMsgType[uGdpb]);
      server->Register("/StarRaw", fvhTriggerRate[uGdpb]);
      server->Register("/StarRaw", fvhCmdDaqVsTrig[uGdpb]);
      server->Register("/StarRaw", fvhStarTokenEvo[uGdpb]);
      server->Register("/StarRaw", fvhStarTrigGdpbTsEvo[uGdpb]);
      server->Register("/StarRaw", fvhStarTrigStarTsEvo[uGdpb]);
    }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

    if (kTRUE == fbPulserModeEnable) {
      for (UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeA++)
        for (UInt_t uFeeB = 0; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++)
          if (NULL != fvhTimeDiffPulser[uFeeA][uFeeB])
            server->Register("/TofDt", fvhTimeDiffPulser[uFeeA][uFeeB]);

      server->Register("/TofRaw", fhTimeMeanPulser);
      server->Register("/TofRaw", fhTimeRmsPulser);
      server->Register("/TofRaw", fhTimeRmsZoomFitPuls);
      server->Register("/TofRaw", fhTimeResFitPuls);

      for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
        for (UInt_t uGbtx = 0; uGbtx < kuNbGbtxPerGdpb - 1; ++uGbtx)
          if (NULL
              != fvhPulserTimeDiffEvoGbtxGbtx[uGdpb * (kuNbGbtxPerGdpb - 1)
                                              + uGbtx])
            server->Register(
              "/TofDtEvo",
              fvhPulserTimeDiffEvoGbtxGbtx[uGdpb * (kuNbGbtxPerGdpb - 1)
                                           + uGbtx]);

        for (UInt_t uGdpbB = uGdpb + 1; uGdpbB < fuNrOfGdpbs; ++uGdpbB)
          if (NULL != fvvhPulserTimeDiffEvoGdpbGdpb[uGdpb][uGdpbB])
            server->Register("/TofDtEvo",
                             fvvhPulserTimeDiffEvoGdpbGdpb[uGdpb][uGdpbB]);
      }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
    }    // if( kTRUE == fbPulserModeEnable )
    if (kTRUE == fbCoincMapsEnable) {
      for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
        server->Register("/TofCoinc", fvhCoincMapAllChanGdpb[uGdpb]);
        server->Register("/TofCoinc", fvhCoincMeanAllChanGdpb[uGdpb]);
      }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
    }    // if( kTRUE == fbCoincMapsEnable )

    server->RegisterCommand("/Reset_All_eTOF", "bMcbmMoniTofResetHistos=kTRUE");
    server->RegisterCommand("/Save_All_eTof", "bMcbmMoniTofSaveHistos=kTRUE");
    server->RegisterCommand("/Update_PulsFit",
                            "bMcbmMoniTofUpdateZoomedFit=kTRUE");
    server->RegisterCommand("/Print_Raw_Data",
                            "bMcbmMoniTofRawDataPrint=kTRUE");
    server->RegisterCommand("/Print_AllHits",
                            "bMcbmMoniTofPrintAllHitsEna=kTRUE");
    server->RegisterCommand("/Print_AllEps",
                            "bMcbmMoniTofPrintAllEpochsEna=kTRUE");

    server->Restrict("/Reset_All_eTof", "allow=admin");
    server->Restrict("/Save_All_eTof", "allow=admin");
    server->Restrict("/Update_PulsFit", "allow=admin");
    server->Restrict("/Print_Raw_Data", "allow=admin");
    server->Restrict("/Print_AllHits", "allow=admin");
    server->Restrict("/Print_AllEps", "allow=admin");
  }  // if( server )

  /** Create summary Canvases for STAR 2018 **/
  Double_t w = 10;
  Double_t h = 10;
  fcSummary  = new TCanvas("cSummary", "gDPB Monitoring Summary");
  fcSummary->Divide(2, 3);

  // 1st Column: Messages types
  fcSummary->cd(1);
  gPad->SetLogy();
  fhMessType->Draw();

  fcSummary->cd(2);
  gPad->SetLogy();
  fhSysMessType->Draw();

  fcSummary->cd(3);
  gPad->SetLogz();
  fhGet4MessType->Draw("colz");

  // 2nd Column: GET4 Errors + Epoch flags + SCm
  fcSummary->cd(4);
  gPad->SetLogz();
  fhGet4ChanErrors->Draw("colz");

  fcSummary->cd(5);
  gPad->SetLogz();
  fhGet4EpochFlags->Draw("colz");

  fcSummary->cd(6);
  fhGet4ChanScm->Draw("colz");

  server->Register("/canvases", fcSummary);
  /*****************************/

  /** Create summary Canvases with plots VS Gdpb **/
  fcSummaryGdpb = new TCanvas("cSummaryGdpb", "gDPB Monitoring Summary");
  fcSummaryGdpb->Divide(2, 3);

  fcSummaryGdpb->cd(1);
  gPad->SetLogz();
  fhGdpbMessType->Draw("colz");

  fcSummaryGdpb->cd(3);
  gPad->SetLogz();
  fhGdpbSysMessType->Draw("colz");

  fcSummaryGdpb->cd(5);
  gPad->SetLogz();
  fhGdpbEpochFlags->Draw("text colz");

  fcSummaryGdpb->cd(4);
  fhGdpbEpochSyncEvo->Draw("colz");

  fcSummaryGdpb->cd(6);
  gPad->SetLogz();
  fhGdpbEpochMissEvo->Draw("colz");

  server->Register("/canvases", fcSummaryGdpb);
  /*****************************/

  /** Create summary Canvases with plots VS GET4 or channel per gDPB **/
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    TCanvas* cSumGdpbGet4 =
      new TCanvas(Form("cSumGdpbGet4_g%02u", uGdpb),
                  Form("Summary per GET4 or channel for gDPB %02u", uGdpb),
                  w,
                  h);
    cSumGdpbGet4->Divide(2, 2);

    cSumGdpbGet4->cd(1);
    gPad->SetLogz();
    fvhGdpbGet4MessType[uGdpb]->Draw("colz");

    cSumGdpbGet4->cd(2);
    gPad->SetLogz();
    fvhGdpbGet4ChanScm[uGdpb]->Draw("colz");

    cSumGdpbGet4->cd(3);
    gPad->SetLogz();
    fvhGdpbGet4ChanErrors[uGdpb]->Draw("colz");

    server->Register("/canvases", cSumGdpbGet4);
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  /*****************************/

  /** Create FEE rates Canvas for STAR 2018 **/
  TCanvas* cFeeRates =
    new TCanvas("cFeeRates", "gDPB Monitoring FEE rates", w, h);
  //   cFeeRates->Divide(fuNrOfFeePerGdpb, fuNrOfGdpbs );
  cFeeRates->Divide(kuNbFeePerGbtx, kuNbGbtxPerGdpb * fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb; ++uFee) {
      cFeeRates->cd(1 + uGdpb * fuNrOfFeePerGdpb + uFee);
      gPad->SetLogy();
      fvhFeeRate_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Draw("hist");

      fvhFeeErrorRate_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->SetLineColor(kRed);
      fvhFeeErrorRate_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Draw("same hist");
    }  // for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb; ++uFee )
  }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  server->Register("/canvases", cFeeRates);
  /*****************************/

  /** Create FEE error ratio Canvas for STAR 2018 **/
  TCanvas* cFeeErrRatio =
    new TCanvas("cFeeErrRatio", "gDPB Monitoring FEE error ratios", w, h);
  //   cFeeErrRatio->Divide(fuNrOfFeePerGdpb, fuNrOfGdpbs );
  cFeeErrRatio->Divide(kuNbFeePerGbtx, kuNbGbtxPerGdpb * fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb; ++uFee) {
      cFeeErrRatio->cd(1 + uGdpb * fuNrOfFeePerGdpb + uFee);
      gPad->SetLogy();
      fvhFeeErrorRatio_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Draw("hist le0");
    }  // for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb; ++uFee )
  }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  server->Register("/canvases", cFeeErrRatio);
  /*****************************/


  /** Create FEE rates long Canvas for STAR 2018 **/
  TCanvas* cFeeRatesLong =
    new TCanvas("cFeeRatesLong", "gDPB Monitoring FEE rates", w, h);
  //   cFeeRatesLong->Divide(fuNrOfFeePerGdpb, fuNrOfGdpbs );
  cFeeRatesLong->Divide(kuNbFeePerGbtx, kuNbGbtxPerGdpb * fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb; ++uFee) {
      cFeeRatesLong->cd(1 + uGdpb * fuNrOfFeePerGdpb + uFee);
      gPad->SetLogy();
      fvhFeeRateLong_gDPB[uGdpb]->Draw("hist");

      fvhFeeErrorRateLong_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->SetLineColor(
        kRed);
      fvhFeeErrorRateLong_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Draw(
        "same hist");
    }  // for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb; ++uFee )
  }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  server->Register("/canvases", cFeeRatesLong);
  /*****************************/

  /** Create FEE error ratio long Canvas for STAR 2018 **/
  TCanvas* cFeeErrRatioLong =
    new TCanvas("cFeeErrRatioLong", "gDPB Monitoring FEE error ratios", w, h);
  //   cFeeErrRatioLong->Divide(fuNrOfFeePerGdpb, fuNrOfGdpbs );
  cFeeErrRatioLong->Divide(kuNbFeePerGbtx, kuNbGbtxPerGdpb * fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb; ++uFee) {
      cFeeErrRatioLong->cd(1 + uGdpb * fuNrOfFeePerGdpb + uFee);
      gPad->SetLogy();
      fvhFeeErrorRatioLong_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Draw(
        "hist le0");
    }  // for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb; ++uFee )
  }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  server->Register("/canvases", cFeeErrRatioLong);
  /*****************************/

  /** Create channel count Canvas for STAR 2018 **/
  fcGdpbChannelCount =
    new TCanvas("cGdpbChannelCount", "Integrated Get4 channel counts per gDPB");
  fcGdpbChannelCount->Divide(1, fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fcGdpbChannelCount->cd(1 + uGdpb);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();
    fvhChCount_gDPB[uGdpb]->Draw();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  server->Register("/canvases", fcGdpbChannelCount);
  /*****************************/

  /** Create remapped channel count Canvas for STAR 2018 **/
  fcGdpbRemapChCount =
    new TCanvas("cGdpbRemapChCount", "Integrated PADI channel counts per gDPB");
  fcGdpbRemapChCount->Divide(1, fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fcGdpbRemapChCount->cd(1 + uGdpb);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();
    fvhRemapChCount_gDPB[uGdpb]->Draw();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  server->Register("/canvases", fcGdpbRemapChCount);
  /*****************************/

  /** Create channel rate Canvas for STAR 2018 **/
  fcGdpbChannelRate =
    new TCanvas("cGdpbChannelRate", "Get4 channel rate per gDPB");
  fcGdpbChannelRate->Divide(1, fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fcGdpbChannelRate->cd(1 + uGdpb);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhChannelRate_gDPB[uGdpb]->Draw("colz");
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  server->Register("/canvases", fcGdpbChannelRate);
  /*****************************/

  /** Create remapped rate count Canvas for STAR 2018 **/
  fcGdpbRemapChRate =
    new TCanvas("cGdpbRemapChRate", "PADI channel rate per gDPB");
  fcGdpbRemapChRate->Divide(1, fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fcGdpbRemapChRate->cd(1 + uGdpb);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhRemapChRate_gDPB[uGdpb]->Draw("colz");
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  server->Register("/canvases", fcGdpbRemapChRate);
  /*****************************/

  /** Create TOT Canvas(es) for STAR 2018 **/
  TCanvas* cTotPnt = NULL;
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    cTotPnt = new TCanvas(Form("cTot_g%02u", uGdpb),
                          Form("gDPB %02u TOT distributions", uGdpb),
                          w,
                          h);

    cTotPnt->cd();
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();

    fvhRawTot_gDPB[uGdpb]->Draw("colz");

    server->Register("/canvases", cTotPnt);
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  cTotPnt = new TCanvas("cTot_all", "TOT distributions", w, h);
  cTotPnt->Divide(fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    cTotPnt->cd(1 + uGdpb);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();

    fvhRawTot_gDPB[uGdpb]->Draw("colz");
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  server->Register("/canvases", cTotPnt);
  /**************************************************/

  /** Create PADI TOT Canvas(es) for STAR 2018 **/
  cTotPnt = NULL;
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    cTotPnt = new TCanvas(Form("cTotRemap_g%02u", uGdpb),
                          Form("PADI ch gDPB %02u TOT distributions", uGdpb),
                          w,
                          h);

    cTotPnt->cd();
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();

    fvhRemapTot_gDPB[uGdpb]->Draw("colz");

    server->Register("/canvases", cTotPnt);
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  cTotPnt = new TCanvas("cTotRemap_all", "TOT distributions", w, h);
  cTotPnt->Divide(fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    cTotPnt->cd(1 + uGdpb);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();

    fvhRemapTot_gDPB[uGdpb]->Draw("colz");
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  server->Register("/canvases", cTotPnt);
  /**************************************************/

  /** Create Side TOT Canvas(es) for STAR 2018 **/
  /*
   cTotPnt = NULL;
   for( UInt_t uMod = 0; uMod < fuNrOfModules; uMod ++ )
   {
      cTotPnt = new TCanvas( Form("cTotRemapSides_m%02u", uMod),
                             Form("Sides ch module %02u TOT distributions", uMod),
                             w, h);
      cTotPnt->Divide( 1, 2 );

      cTotPnt->cd( 1 );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();

      fvhRemapTotSideA_mod[ uMod ]->Draw( "colz" );

      cTotPnt->cd( 2 );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fvhRemapTotSideB_mod[ uMod ]->Draw( "colz" );

      server->Register("/canvases", cTotPnt );
   } // for( UInt_t uMod = 0; uMod < fuNrOfModules; uMod ++ )
*/
  /**************************************************/

  /** Create STAR token Canvas for STAR 2018 **/
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb++) {
    TCanvas* cStarToken =
      new TCanvas(Form("cStarToken_g%02u", uGdpb),
                  Form("STAR token detection info for gDPB %02u", uGdpb),
                  w,
                  h);
    cStarToken->Divide(2, 2);

    cStarToken->cd(1);
    fvhTriggerRate[uGdpb]->Draw();

    cStarToken->cd(2);
    fvhCmdDaqVsTrig[uGdpb]->Draw("colz");

    cStarToken->cd(3);
    fvhStarTokenEvo[uGdpb]->Draw();

    cStarToken->cd(4);
    fvhStarTrigGdpbTsEvo[uGdpb]->Draw("hist le0");
    fvhStarTrigStarTsEvo[uGdpb]->SetLineColor(kRed);
    fvhStarTrigStarTsEvo[uGdpb]->Draw("same hist le0");
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
  /*****************************/

  if (kTRUE == fbPulserModeEnable) {
    /** Create Pulser check Canvas for STAR 2018 **/
    TCanvas* cPulser = new TCanvas(
      "cPulser",
      "Time difference RMS for pulser channels when FEE pulser mode is ON",
      w,
      h);
    cPulser->Divide(2, 2);

    cPulser->cd(1);
    gPad->SetGridx();
    gPad->SetGridy();
    fhTimeRmsPulser->Draw("colz");

    cPulser->cd(2);
    gPad->SetGridx();
    gPad->SetGridy();
    fhTimeMeanPulser->Draw("colz");

    cPulser->cd(3);
    gPad->SetGridx();
    gPad->SetGridy();
    fhTimeRmsZoomFitPuls->Draw("colz");

    cPulser->cd(4);
    gPad->SetGridx();
    gPad->SetGridy();
    fhTimeResFitPuls->Draw("colz");

    server->Register("/canvases", cPulser);
    /*****************************/

    /** Create Pulser evo Canvas for gDPB to gDPB **/
    TCanvas* cPulserEvo = new TCanvas(
      "cPulserEvo",
      "Time difference evolution between 1st FEE of 1st GBTx of gDPB pairs",
      w,
      h);
    cPulserEvo->Divide(1, fuNrOfGdpbs - 1);
    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs - 1; uGdpb++) {
      cPulserEvo->cd(1 + uGdpb);
      gPad->SetGridx();
      gPad->SetGridy();
      if (NULL != fvvhPulserTimeDiffEvoGdpbGdpb[uGdpb][uGdpb + 1])
        fvvhPulserTimeDiffEvoGdpbGdpb[uGdpb][uGdpb + 1]->Draw();

    }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs - 1; uGdpb ++)

    server->Register("/canvases", cPulserEvo);
    /*****************************/
    /** Create Pulser evo Canvas within gDPB **/
    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb++) {
      TCanvas* cPulserEvoGbtx =
        new TCanvas(Form("cPulserEvoGbtx%02u", uGdpb),
                    Form("Time difference evolution between 1st FEE of GBTx "
                         "pairs in gDPB %02u",
                         uGdpb),
                    w,
                    h);
      cPulserEvoGbtx->Divide(1, kuNbGbtxPerGdpb - 1);

      for (UInt_t uGbtx = 0; uGbtx < kuNbGbtxPerGdpb - 1; ++uGbtx) {
        cPulserEvoGbtx->cd(1 + uGbtx);
        gPad->SetGridx();
        gPad->SetGridy();

        if (NULL
            != fvhPulserTimeDiffEvoGbtxGbtx[uGdpb * (kuNbGbtxPerGdpb - 1)
                                            + uGbtx])
          fvhPulserTimeDiffEvoGbtxGbtx[uGdpb * (kuNbGbtxPerGdpb - 1) + uGbtx]
            ->Draw();
      }  // for( UInt_t uGbtx = 0; uGbtx < kuNbGbtxPerGdpb - 1; ++uGbtx )

      server->Register("/canvases", cPulserEvoGbtx);
    }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
       /*****************************/
  }    // if( kTRUE == fbPulserModeEnable )

  /** Create Module rates Canvas **/
  /*
   for( UInt_t uMod = 0; uMod < fuNrOfModules; uMod ++ )
   {
      TCanvas* cModRates = new TCanvas( Form("cModRate_m%02u", uMod),
                                        Form( "Hit and error Rates for module %02u", uMod),
                                        w, h);
      cModRates->cd();
      gPad->SetLogy();
      fvhModRate[ uMod ]->Draw("hist");

      fvhModErrorRate[ uMod ]->SetLineColor( kRed );
      fvhModErrorRate[ uMod ]->Draw("same hist");

      server->Register("/canvases", cModRates );
   } // for( UInt_t uMod = 0; uMod < fuNrOfModules; uMod ++ )
*/
  /*****************************/

  /** Recovers/Create Ms Size Canvas for STAR 2018 **/
  // Try to recover canvas in case it was created already by another monitor
  // If not existing, create it
  fcMsSizeAll = dynamic_cast<TCanvas*>(gROOT->FindObject("cMsSizeAll"));
  if (NULL == fcMsSizeAll) {
    fcMsSizeAll =
      new TCanvas("cMsSizeAll", "Evolution of MS size in last 300 s", w, h);
    fcMsSizeAll->Divide(4, 3);
    LOG(info) << "Created MS size canvas in TOF monitor";
  }  // if( NULL == fcMsSizeAll )
  else
    LOG(info) << "Recovered MS size canvas in TOF monitor";

  server->Register("/canvases", fcMsSizeAll);

  LOG(info) << "Leaving CreateHistograms";
}

Bool_t CbmMcbm2018MonitorTof::DoUnpack(const fles::Timeslice& ts,
                                       size_t component) {
  if (bMcbmMoniTofResetHistos) {
    LOG(info) << "Reset eTOF STAR histos ";
    ResetAllHistos();
    bMcbmMoniTofResetHistos = kFALSE;
  }  // if( bMcbmMoniTofResetHistos )
  if (bMcbmMoniTofSaveHistos) {
    LOG(info) << "Start saving eTOF STAR histos ";
    SaveAllHistos("data/histos_Shift_StarTof.root");
    bMcbmMoniTofSaveHistos = kFALSE;
  }  // if( bSaveStsHistos )
  if (bMcbmMoniTofUpdateZoomedFit) {
    UpdateZoomedFit();
    bMcbmMoniTofUpdateZoomedFit = kFALSE;
  }  // if (bMcbmMoniTofUpdateZoomedFit)
  if (bMcbmMoniTofRawDataPrint) {
    fuRawDataPrintMsgIdx     = 0;
    bMcbmMoniTofRawDataPrint = kFALSE;
  }  // if( bMcbmMoniTofRawDataPrint )
  if (bMcbmMoniTofPrintAllHitsEna) {
    fbPrintAllHitsEnable        = !fbPrintAllHitsEnable;
    bMcbmMoniTofPrintAllHitsEna = kFALSE;
  }  // if( bMcbmMoniTofPrintAllHitsEna )
  if (bMcbmMoniTofPrintAllEpochsEna) {
    fbPrintAllEpochsEnable      = !fbPrintAllEpochsEnable;
    bMcbmMoniTofPrintAllHitsEna = kFALSE;
  }  // if( bMcbmMoniTofPrintAllEpochsEna )

  /// Periodically save the histograms
  /*
   std::chrono::time_point<std::chrono::system_clock> timeCurrent = std::chrono::system_clock::now();
   std::chrono::duration<double> elapsed_seconds = timeCurrent - fTimeLastHistoSaving;
   if( 0 == fTimeLastHistoSaving.time_since_epoch().count() )
   {
      fTimeLastHistoSaving = timeCurrent;
//      fulNbBuiltSubEventLastPrintout = fulNbBuiltSubEvent;
//      fulNbStarSubEventLastPrintout  = fulNbStarSubEvent;
   } // if( 0 == fTimeLastHistoSaving.time_since_epoch().count() )
   else if( 300 < elapsed_seconds.count() )
   {
      std::time_t cTimeCurrent = std::chrono::system_clock::to_time_t( timeCurrent );
      char tempBuff[80];
      std::strftime( tempBuff, 80, "%F %T", localtime (&cTimeCurrent) );
      fTimeLastHistoSaving = timeCurrent;
      SaveAllHistos( "data/histos_shift.root" );
   } // else if( 300 < elapsed_seconds.count() )
*/
  LOG(debug1) << "Timeslice contains " << ts.num_microslices(component)
              << "microslices.";

  /// Ignore First TS as first MS is typically corrupt
  if (0 == ts.index()) return kTRUE;

  /// Ignore overlap ms if flag set by user
  UInt_t uNbMsLoop = fuNbCoreMsPerTs;
  if (kFALSE == fbIgnoreOverlapMs) uNbMsLoop += fuNbOverMsPerTs;

  Int_t messageType     = -111;
  Double_t dTsStartTime = -1;

  /// Loop over core microslices (and overlap ones if chosen)
  for (UInt_t uMsIdx = 0; uMsIdx < uNbMsLoop; uMsIdx++) {
    if (fuMsAcceptsPercent < uMsIdx) continue;

    fuCurrentMs = uMsIdx;

    if (0 == fulCurrentTsIndex && 0 == uMsIdx) {
      for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size();
           ++uMsCompIdx) {
        UInt_t uMsComp    = fvMsComponentsList[uMsCompIdx];
        auto msDescriptor = ts.descriptor(uMsComp, uMsIdx);
        /*
            LOG(info) << "hi hv eqid flag si sv idx/start        crc      size     offset";
            LOG(info) << Form( "%02x %02x %04x %04x %02x %02x %016llx %08x %08x %016llx",
                            static_cast<unsigned int>(msDescriptor.hdr_id),
                            static_cast<unsigned int>(msDescriptor.hdr_ver), msDescriptor.eq_id, msDescriptor.flags,
                            static_cast<unsigned int>(msDescriptor.sys_id),
                            static_cast<unsigned int>(msDescriptor.sys_ver), msDescriptor.idx, msDescriptor.crc,
                            msDescriptor.size, msDescriptor.offset );
*/
        LOG(info) << FormatMsHeaderPrintout(msDescriptor);
      }  // for( UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx )
    }    // if( 0 == fulCurrentTsIndex && 0 == uMsIdx )

    /// Loop over registered components
    for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size();
         ++uMsCompIdx) {
      constexpr uint32_t kuBytesPerMessage = 8;

      UInt_t uMsComp    = fvMsComponentsList[uMsCompIdx];
      auto msDescriptor = ts.descriptor(uMsComp, uMsIdx);
      fiEquipmentId     = msDescriptor.eq_id;
      fdMsIndex         = static_cast<double>(msDescriptor.idx);
      fuCurrentMsSysId  = static_cast<unsigned int>(msDescriptor.sys_id);
      const uint8_t* msContent =
        reinterpret_cast<const uint8_t*>(ts.content(uMsComp, uMsIdx));

      uint32_t size = msDescriptor.size;
      //    fulLastMsIdx = msDescriptor.idx;
      if (size > 0)
        LOG(debug) << "Microslice: " << msDescriptor.idx
                   << " has size: " << size;
      /*
         if( numCompMsInTs - fuOverlapMsNb <= m )
         {
//         LOG(info) << "Ignore overlap Microslice: " << msDescriptor.idx;
            continue;
         } // if( numCompMsInTs - fuOverlapMsNb <= m )
*/
      if (0 == uMsIdx && 0 == uMsCompIdx) dTsStartTime = (1e-9) * fdMsIndex;

      if (fdStartTimeMsSz < 0) fdStartTimeMsSz = (1e-9) * fdMsIndex;
      fvhMsSzPerLink[uMsComp]->Fill(size);
      if (2 * fuHistoryHistoSize < (1e-9) * fdMsIndex - fdStartTimeMsSz) {
        // Reset the evolution Histogram and the start time when we reach the end of the range
        fvhMsSzTimePerLink[uMsComp]->Reset();
        fdStartTimeMsSz = (1e-9) * fdMsIndex;
      }  // if( 2 * fuHistoryHistoSize < (1e-9) * fdMsIndex - fdStartTimeMsSz )
      fvhMsSzTimePerLink[uMsComp]->Fill((1e-9) * fdMsIndex - fdStartTimeMsSz,
                                        size);

      // If not integer number of message in input buffer, print warning/error
      if (0 != (size % kuBytesPerMessage))
        LOG(error) << "The input microslice buffer does NOT "
                   << "contain only complete nDPB messages!";

      // Compute the number of complete messages in the input microslice buffer
      uint32_t uNbMessages =
        (size - (size % kuBytesPerMessage)) / kuBytesPerMessage;

      // Get the gDPB ID from the MS header
      fuGdpbId = fiEquipmentId;

      /// Check if this gDPB ID was declared in parameter file and stop there if not
      auto it = fGdpbIdIndexMap.find(fuGdpbId);
      if (it == fGdpbIdIndexMap.end()) {
        LOG(info)
          << "---------------------------------------------------------------";
        /*
             LOG(info) << "hi hv eqid flag si sv idx/start        crc      size     offset";
             LOG(info) << Form( "%02x %02x %04x %04x %02x %02x %016llx %08x %08x %016llx",
                               static_cast<unsigned int>(msDescriptor.hdr_id),
                               static_cast<unsigned int>(msDescriptor.hdr_ver), msDescriptor.eq_id, msDescriptor.flags,
                               static_cast<unsigned int>(msDescriptor.sys_id),
                               static_cast<unsigned int>(msDescriptor.sys_ver), msDescriptor.idx, msDescriptor.crc,
                               msDescriptor.size, msDescriptor.offset );
*/
        LOG(info) << FormatMsHeaderPrintout(msDescriptor);
        LOG(error) << "Could not find the gDPB index for AFCK id 0x" << std::hex
                   << fuGdpbId << std::dec << " in timeslice "
                   << fulCurrentTsIndex << " in microslice " << fdMsIndex
                   << " component " << uMsCompIdx << "\n"
                   << "If valid this index has to be added in the TOF "
                      "parameter file in the RocIdArray field";
        continue;
      }  // if( it == fGdpbIdIndexMap.end() )
      else
        fuGdpbNr = fGdpbIdIndexMap[fuGdpbId];

      // Prepare variables for the loop on contents
      const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>(msContent);
      for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++) {
        // Fill message
        uint64_t ulData = static_cast<uint64_t>(pInBuff[uIdx]);

        /// Catch the Epoch cycle block which is always the first 64b of the MS
        if (0 == uIdx && kFALSE == fbOldFwData) {
          ProcessEpochCycle(ulData);
          continue;
        }  // if( 0 == uIdx && kFALSE == fbOldFwData )

        gdpbv100::Message mess(ulData);

        if (fuRawDataPrintMsgIdx < fuRawDataPrintMsgNb
            || gLogger->IsLogNeeded(fair::Severity::debug2)) {
          mess.printDataCout();
          fuRawDataPrintMsgIdx++;
        }  // if( fuRawDataPrintMsgIdx < fuRawDataPrintMsgNb || gLogger->IsLogNeeded( fair::Severity::debug2 ) )

        // Increment counter for different message types
        // and fill the corresponding histogram
        messageType = mess.getMessageType();
        fviMsgCounter[messageType]++;
        fhMessType->Fill(messageType);
        fhGdpbMessType->Fill(messageType, fuGdpbNr);

        ///         fuGet4Id = mess.getGdpbGenChipId();
        fuGet4Id = ConvertElinkToGet4(mess.getGdpbGenChipId());
        /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
        if (fuGdpbNr == fuDiamondDpbIdx || 0x90 == fuCurrentMsSysId)
          fuGet4Id = mess.getGdpbGenChipId();
        fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;

        if (fuNrOfGet4PerGdpb <= fuGet4Id && !mess.isStarTrigger()
            && (gdpbv100::kuChipIdMergedEpoch != fuGet4Id))
          LOG(warning) << "Message with Get4 ID too high: " << fuGet4Id
                       << " VS " << fuNrOfGet4PerGdpb << " set in parameters.";

        switch (messageType) {
          case gdpbv100::MSG_HIT: {
            if (mess.getGdpbHitIs24b()) {
              fhGet4MessType->Fill(fuGet4Nr, 4);
              fvhGdpbGet4MessType[fuGdpbNr]->Fill(fuGet4Id, 4);
              PrintGenInfo(mess);
            }  // if( getGdpbHitIs24b() )
            else {
              fhGet4MessType->Fill(fuGet4Nr, 0);
              fvhGdpbGet4MessType[fuGdpbNr]->Fill(fuGet4Id, 0);
              fvmEpSupprBuffer[fuGet4Nr].push_back(mess);
            }  // else of if( getGdpbHitIs24b() )
            break;
          }  // case gdpbv100::MSG_HIT:
          case gdpbv100::MSG_EPOCH: {
            if (gdpbv100::kuChipIdMergedEpoch == fuGet4Id) {
              if (1 == mess.getGdpbEpSync()) {
                fhGdpbEpochFlags->Fill(fuGdpbNr, 0);
                fhGdpbEpochSyncEvo->Fill((1e-9) * fdMsIndex - fdStartTimeMsSz,
                                         fuGdpbNr);
              }  // if (1 == mess.getGdpbEpSync())

              if (1 == mess.getGdpbEpDataLoss())
                fhGdpbEpochFlags->Fill(fuGdpbNr, 1);

              if (1 == mess.getGdpbEpEpochLoss())
                fhGdpbEpochFlags->Fill(fuGdpbNr, 2);

              if (1 == mess.getGdpbEpMissmatch()) {
                fhGdpbEpochFlags->Fill(fuGdpbNr, 3);
                fhGdpbEpochMissEvo->Fill((1e-9) * fdMsIndex - fdStartTimeMsSz,
                                         fuGdpbNr);
              }  // if (1 == mess.getGdpbEpMissmatch())

              for (uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb;
                   uGet4Index++) {
                fuGet4Id = uGet4Index;
                fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;
                gdpbv100::Message tmpMess(mess);
                tmpMess.setGdpbGenChipId(uGet4Index);

                fhGet4MessType->Fill(fuGet4Nr, 1);
                fvhGdpbGet4MessType[fuGdpbNr]->Fill(fuGet4Id, 1);
                FillEpochInfo(tmpMess);
              }  // for( uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGetIndex ++ )

              if (kTRUE == fbPrintAllEpochsEnable) {
                LOG(info) << "Epoch: " << Form("0x%08x ", fuGdpbId) << ", Merg"
                          << ", Link " << std::setw(1) << mess.getGdpbEpLinkId()
                          << ", epoch " << std::setw(8)
                          << mess.getGdpbEpEpochNb() << ", Sync "
                          << std::setw(1) << mess.getGdpbEpSync()
                          << ", Data loss " << std::setw(1)
                          << mess.getGdpbEpDataLoss() << ", Epoch loss "
                          << std::setw(1) << mess.getGdpbEpEpochLoss()
                          << ", Epoch miss " << std::setw(1)
                          << mess.getGdpbEpMissmatch();
              }  // if( kTRUE == fbPrintAllEpochsEnable )
            }    // if this epoch message is a merged one valid for all chips
            else {
              fhGet4MessType->Fill(fuGet4Nr, 1);
              fvhGdpbGet4MessType[fuGdpbNr]->Fill(fuGet4Id, 1);
              FillEpochInfo(mess);

              if (kTRUE == fbPrintAllEpochsEnable) {
                LOG(info) << "Epoch: " << Form("0x%08x ", fuGdpbId) << ", "
                          << std::setw(4) << fuGet4Nr << ", Link "
                          << std::setw(1) << mess.getGdpbEpLinkId()
                          << ", epoch " << std::setw(8)
                          << mess.getGdpbEpEpochNb() << ", Sync "
                          << std::setw(1) << mess.getGdpbEpSync()
                          << ", Data loss " << std::setw(1)
                          << mess.getGdpbEpDataLoss() << ", Epoch loss "
                          << std::setw(1) << mess.getGdpbEpEpochLoss()
                          << ", Epoch miss " << std::setw(1)
                          << mess.getGdpbEpMissmatch();
              }  // if( kTRUE == fbPrintAllEpochsEnable )
            }    // if single chip epoch message
            break;
          }  // case gdpbv100::MSG_EPOCH:
          case gdpbv100::MSG_SLOWC: {
            fhGet4MessType->Fill(fuGet4Nr, 2);
            fvhGdpbGet4MessType[fuGdpbNr]->Fill(fuGet4Id, 2);
            PrintSlcInfo(mess);
            break;
          }  // case gdpbv100::MSG_SLOWC:
          case gdpbv100::MSG_SYST: {
            fhSysMessType->Fill(mess.getGdpbSysSubType());
            fhGdpbSysMessType->Fill(mess.getGdpbSysSubType(), fuGdpbNr);
            if (gdpbv100::SYS_GET4_ERROR == mess.getGdpbSysSubType()) {
              fhGet4MessType->Fill(fuGet4Nr, 3);
              fvhGdpbGet4MessType[fuGdpbNr]->Fill(fuGet4Id, 3);

              UInt_t uFeeNr = (fuGet4Id / fuNrOfGet4PerFee);
              if (0 <= fdStartTime) {
                fvhFeeErrorRate_gDPB[(fuGdpbNr * fuNrOfFeePerGdpb) + uFeeNr]
                  ->Fill(1e-9
                         * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr])
                            - fdStartTime));
                fvhFeeErrorRatio_gDPB[(fuGdpbNr * fuNrOfFeePerGdpb) + uFeeNr]
                  ->Fill(1e-9
                           * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr])
                              - fdStartTime),
                         1,
                         1);

                //                        UInt_t uGbtxNr      = (uFeeNr / kuNbFeePerGbtx);
                //                        UInt_t uGbtxNrInSys = fuGdpbNr * kuNbGbtxPerGdpb + uGbtxNr;
                /*
                        fvhModErrorRate[ fviModuleId[ uGbtxNrInSys ] ]->Fill(
                           1e-9 * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr]) - fdStartTime));
                        fvhModErrorRatio[ fviModuleId[ uGbtxNrInSys ] ]->Fill(
                           1e-9 * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr]) - fdStartTime), 1, 1);
*/
              }  // if (0 <= fdStartTime)
              if (0 <= fdStartTimeLong) {
                fvhFeeErrorRateLong_gDPB[(fuGdpbNr * fuNrOfFeePerGdpb) + uFeeNr]
                  ->Fill(1e-9 / 60.0
                           * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr])
                              - fdStartTimeLong),
                         1 / 60.0);
                fvhFeeErrorRatioLong_gDPB[(fuGdpbNr * fuNrOfFeePerGdpb)
                                          + uFeeNr]
                  ->Fill(1e-9 / 60.0
                           * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr])
                              - fdStartTimeLong),
                         1,
                         1 / 60.0);
              }  // if (0 <= fdStartTime)

              Int_t dGdpbChId =
                fuGet4Id * fuNrOfChannelsPerGet4 + mess.getGdpbSysErrChanId();
              Int_t dFullChId =
                fuGet4Nr * fuNrOfChannelsPerGet4 + mess.getGdpbSysErrChanId();
              switch (mess.getGdpbSysErrData()) {
                case gdpbv100::GET4_V2X_ERR_READ_INIT:
                  fhGet4ChanErrors->Fill(dFullChId, 0);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 0);
                  break;
                case gdpbv100::GET4_V2X_ERR_SYNC:
                  fhGet4ChanErrors->Fill(dFullChId, 1);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 1);
                  break;
                case gdpbv100::GET4_V2X_ERR_EP_CNT_SYNC:
                  fhGet4ChanErrors->Fill(dFullChId, 2);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 2);
                  break;
                case gdpbv100::GET4_V2X_ERR_EP:
                  fhGet4ChanErrors->Fill(dFullChId, 3);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 3);
                  break;
                case gdpbv100::GET4_V2X_ERR_FIFO_WRITE:
                  fhGet4ChanErrors->Fill(dFullChId, 4);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 4);
                  break;
                case gdpbv100::GET4_V2X_ERR_LOST_EVT:
                  fhGet4ChanErrors->Fill(dFullChId, 5);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 5);
                  break;
                case gdpbv100::GET4_V2X_ERR_CHAN_STATE:
                  fhGet4ChanErrors->Fill(dFullChId, 6);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 6);
                  break;
                case gdpbv100::GET4_V2X_ERR_TOK_RING_ST:
                  fhGet4ChanErrors->Fill(dFullChId, 7);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 7);
                  break;
                case gdpbv100::GET4_V2X_ERR_TOKEN:
                  fhGet4ChanErrors->Fill(dFullChId, 8);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 8);
                  break;
                case gdpbv100::GET4_V2X_ERR_READOUT_ERR:
                  fhGet4ChanErrors->Fill(dFullChId, 9);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 9);
                  break;
                case gdpbv100::GET4_V2X_ERR_SPI:
                  fhGet4ChanErrors->Fill(dFullChId, 10);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 10);
                  break;
                case gdpbv100::GET4_V2X_ERR_DLL_LOCK:
                  fhGet4ChanErrors->Fill(dFullChId, 11);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 11);
                  break;
                case gdpbv100::GET4_V2X_ERR_DLL_RESET:
                  fhGet4ChanErrors->Fill(dFullChId, 12);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 12);
                  break;
                case gdpbv100::GET4_V2X_ERR_TOT_OVERWRT:
                  fhGet4ChanErrors->Fill(dFullChId, 13);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 13);
                  break;
                case gdpbv100::GET4_V2X_ERR_TOT_RANGE:
                  fhGet4ChanErrors->Fill(dFullChId, 14);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 14);
                  break;
                case gdpbv100::GET4_V2X_ERR_EVT_DISCARD:
                  fhGet4ChanErrors->Fill(dFullChId, 15);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 15);
                  break;
                case gdpbv100::GET4_V2X_ERR_ADD_RIS_EDG:
                  fhGet4ChanErrors->Fill(dFullChId, 16);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 16);
                  break;
                case gdpbv100::GET4_V2X_ERR_UNPAIR_FALL:
                  fhGet4ChanErrors->Fill(dFullChId, 17);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 17);
                  break;
                case gdpbv100::GET4_V2X_ERR_SEQUENCE_ER:
                  fhGet4ChanErrors->Fill(dFullChId, 18);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 18);
                  break;
                case gdpbv100::GET4_V2X_ERR_EPOCH_OVERF:
                  fhGet4ChanErrors->Fill(dFullChId, 19);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 19);
                  break;
                case gdpbv100::GET4_V2X_ERR_UNKNOWN:
                  fhGet4ChanErrors->Fill(dFullChId, 20);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 20);
                  break;
                default:  // Corrupt error or not yet supported error
                  fhGet4ChanErrors->Fill(dFullChId, 21);
                  fvhGdpbGet4ChanErrors[fuGdpbNr]->Fill(dGdpbChId, 21);
                  break;
              }  // Switch( mess.getGdpbSysErrData() )
            }  // if( gdpbv100::SYSMSG_GET4_EVENT == mess.getGdpbSysSubType() )
            if (gdpbv100::SYS_PATTERN == mess.getGdpbSysSubType()) {
              fhGdpbSysMessPattType->Fill(mess.getGdpbSysPattType(), fuGdpbNr);
              FillPattInfo(mess);
            }  // if( gdpbv100::SYS_PATTERN == mess.getGdpbSysSubType() )
            PrintSysInfo(mess);
            break;
          }  // case gdpbv100::MSG_SYST:
          case gdpbv100::MSG_STAR_TRI_A:
          case gdpbv100::MSG_STAR_TRI_B:
          case gdpbv100::MSG_STAR_TRI_C:
          case gdpbv100::MSG_STAR_TRI_D:
            //                  fhGet4MessType->Fill(fuGet4Nr, 5);
            FillStarTrigInfo(mess);
            break;
          default:
            LOG(error) << "Message type " << std::hex << std::setw(2)
                       << static_cast<uint16_t>(messageType)
                       << " not included in Get4 unpacker.";
        }  // switch( mess.getMessageType() )
      }    // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
    }  // for( UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx )

    ///* Pulser monitoring *///
    if (kTRUE == fbPulserModeEnable) {
      /// Fill the corresponding histos if the time difference is reasonnable
      for (UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; ++uFeeA) {
        if (1 != fvuFeeNbHitsLastMs[uFeeA]) {
          /// Pulser should lead to single hit in a MS, so ignore this time
          fvuFeeNbHitsLastMs[uFeeA] = 0;
          continue;
        }  // if( 1 != fvuFeeNbHitsLastMs[ uFeeA ] )

        UInt_t uGdpbNr = uFeeA / fuNrOfFeePerGdpb;
        UInt_t uGbtxNr = uFeeA / kuNbFeePerGbtx;
        /// Update the difference to all other FEE with higher indices
        for (UInt_t uFeeB = uFeeA + 1; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs;
             ++uFeeB) {
          if (1 != fvuFeeNbHitsLastMs[uFeeB]) continue;

          if (NULL != fvhTimeDiffPulser[uFeeA][uFeeB]) {
            Double_t dTimeDiff =
              1e3 * (fdTsLastPulserHit[uFeeB] - fdTsLastPulserHit[uFeeA]);
            if (TMath::Abs(dTimeDiff) < kdMaxDtPulserPs) {
              fvhTimeDiffPulser[uFeeA][uFeeB]->Fill(dTimeDiff);

              /// Dt Evo plots only for first FEE in each GBTx
              if (0 == uFeeA % kuNbFeePerGbtx && 0 == uFeeB % kuNbFeePerGbtx) {
                /// Evo of GBTx inside same DPB
                if (uGdpbNr == uFeeB / fuNrOfFeePerGdpb) {
                  if (0 == uGbtxNr) {
                    UInt_t uPlotIdx =
                      uGdpbNr * (kuNbGbtxPerGdpb - 1)
                      + (uFeeB - uGdpbNr * fuNrOfFeePerGdpb) / kuNbFeePerGbtx
                      - 1;
                    fvhPulserTimeDiffEvoGbtxGbtx[uPlotIdx]->Fill(
                      1e-9 / 60.0 * (fdTsLastPulserHit[uFeeA] - fdStartTime),
                      dTimeDiff);
                  }   // if( 0 == uGbtxNr  )
                }     // if( uGdpbNr == uFeeB / fuNrOfFeePerGdpb )
                else  // if( NULL != fvvhPulserTimeDiffEvoGdpbGdpb[ uFeeB / fuNrOfFeePerGdpb ][ uGdpbNr ] )
                {
                  /// Evo of DPBs if both first FEE in First GBTx
                  if (0 == uGbtxNr && 0 == uFeeB / kuNbFeePerGbtx)
                    fvvhPulserTimeDiffEvoGdpbGdpb[uGdpbNr][uFeeB
                                                           / fuNrOfFeePerGdpb]
                      ->Fill(1e-9 / 60.0
                               * (fdTsLastPulserHit[uFeeA] - fdStartTime),
                             dTimeDiff);
                }  // else of if( uGdpbNr == uFeeB / fuNrOfFeePerGdpb )
              }  // if( 0 == uFeeA % kuNbFeePerGbtx && 0 == uFeeB % kuNbFeePerGbtx )
            }  // if( TMath::Abs( dTimeDiff ) < kdMaxDtPulserPs )
               //               else if( 10 == uFeeA && 20 == uFeeB )
               //                  LOG(info) << "new in 10 " << dTimeDiff;
          }    // if( NULL != fvhTimeDiffPulser[uFeeA][uFeeB] )
        }  // for( UInt_t uFeeB = uFeeA + 1; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; ++uFeeB)

        /// Done with this FEE, we can reset the hit counter
        fvuFeeNbHitsLastMs[uFeeA] = 0;
      }  // for( UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; ++uFeeA)
    }  // if( kTRUE == fbPulserModeEnable )

    if (kTRUE == fbCoincMapsEnable) {
      for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
        for (UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerGdpb; ++uChanA) {
          if (0 == fvuCoincNbHitsLastMs[uGdpb][uChanA]) {
            /// Coinc need a hit in a MS, so ignore this time
            continue;
          }  // if( 0 == fvuCoincNbHitsLastMs[ uGdpb ][ uChanA ] )

          for (UInt_t uChanB = uChanA + 1; uChanB < fuNrOfChannelsPerGdpb;
               ++uChanB) {
            if (0 == fvuCoincNbHitsLastMs[uGdpb][uChanB]) {
              /// Coinc need a hit in a MS, so ignore this time
              continue;
            }  // if( 0 == fvuCoincNbHitsLastMs[ uGdpb ][ uChanB ] )

            Double_t dTimeDiff = 1e3
                                 * (fvdCoincTsLastHit[uGdpb][uChanB]
                                    - fvdCoincTsLastHit[uGdpb][uChanA]);

            fvhCoincMeanAllChanGdpb[uGdpb]->Fill(uChanA, uChanB, dTimeDiff);

            if (TMath::Abs(dTimeDiff) < kdMaxDtPulserPs) {
              fvhCoincMapAllChanGdpb[uGdpb]->Fill(uChanA, uChanB);
            }  // if( TMath::Abs( dTimeDiff ) < kdMaxDtPulserPs )
          }  // for( UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerGdpb; ++uChan A )

          /// Done with this channel, we can reset the hit counter
          fvuCoincNbHitsLastMs[uGdpb][uChanA] = 0;
        }  // for( UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerGdpb; ++uChan A )
      }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
    }      // if( kTRUE == fbCoincMapsEnable )

    //      UInt_t uTsIndexHeader = ts.index();
    UInt_t uTsIndexHeader = fulCurrentTsIndex;
    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
      fhHitsPerMsFirstChan_gDPB[uGdpb]->Fill(uTsIndexHeader,
                                             fvvuChanNbHitsPerMs[uGdpb][0]);
      for (UInt_t uChan = 0; uChan < fuNrOfChannelsPerGdpb; ++uChan) {
        fvhChannelRatePerMs_gDPB[uGdpb]->Fill(
          uTsIndexHeader, uChan, 1.0 * fvvuChanNbHitsPerMs[uGdpb][uChan]);
        fvvuChanNbHitsPerMs[uGdpb][uChan] = 0;
      }  // for( UInt_t uChan = 0; uChan < fuNrOfChannelsPerGdpb; ++uChan )
    }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  }      // for( UInt_t uMsIdx = 0; uMsIdx < uNbMsLoop; uMsIdx ++ )

  /// Fill plot of Enable flag state per TS
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb)
    for (UInt_t uAsic = 0; uAsic < fuNrOfGet4PerGdpb; ++uAsic) {
      if (fvvbGdpbLastMissmatchPattern[uGdpb][uAsic]) {
        fvhGdpbMissmatchEvoPerTs[uGdpb]->Fill(fulCurrentTsIndex, uAsic);
      }  // if( fvvbGdpbLastMissmatchPattern[ uGdpb ][ uAsic ] )

      if (fvvbGdpbLastEnablePattern[uGdpb][uAsic]) {
        fvhGdpbEnableEvoPerTs[uGdpb]->Fill(fulCurrentTsIndex, uAsic);

        if (fvvbGdpbLastMissmatchPattern[uGdpb][uAsic]) {
          fvhGdpbMissmatchEnaEvoPerTs[uGdpb]->Fill(fulCurrentTsIndex, uAsic);
        }  // if( fvvbGdpbLastMissmatchPattern[ uGdpb ][ uAsic ] )

        if (fvvbGdpbLastResyncPattern[uGdpb][uAsic]) {
          fvhGdpbResyncEnaEvoPerTs[uGdpb]->Fill(fulCurrentTsIndex, uAsic);
        }  // if( fvvbGdpbLastResyncPattern[ uGdpb ][ uAsic ] )

        /// State evolution
        if (fvvbGdpbLastMissmatchPattern[uGdpb][uAsic]) {
          if (fvvbGdpbLastResyncPattern[uGdpb][uAsic]) {
            fvhGdpbStateEvoPerTs[uGdpb]->Fill(fulCurrentTsIndex, uAsic, 4);
          }  // if( fvvbGdpbLastResyncPattern[ uGdpb ][ uAsic ] )
          else
            fvhGdpbStateEvoPerTs[uGdpb]->Fill(fulCurrentTsIndex, uAsic, 2);
        }  // if( fvvbGdpbLastMissmatchPattern[ uGdpb ][ uAsic ] )
        else if (fvvbGdpbLastResyncPattern[uGdpb][uAsic]) {
          fvhGdpbStateEvoPerTs[uGdpb]->Fill(fulCurrentTsIndex, uAsic, 3);
        }  // else if( fvvbGdpbLastResyncPattern[ uGdpb ][ uAsic ] )
        else
          fvhGdpbStateEvoPerTs[uGdpb]->Fill(fulCurrentTsIndex, uAsic, 1);
      }  // if( fvvbGdpbLastEnablePattern[ uGdpb ][ uAsic ] )
      else
        fvhGdpbStateEvoPerTs[uGdpb]->Fill(fulCurrentTsIndex, uAsic, 0);

      if (fvvbGdpbLastResyncPattern[uGdpb][uAsic]) {
        fvhGdpbResyncEvoPerTs[uGdpb]->Fill(fulCurrentTsIndex, uAsic);
      }  // if( fvvbGdpbLastResyncPattern[ uGdpb ][ uAsic ] )
    }    // Loop on gDPB and ASICs

  // Update RMS plots only every 10s in data
  if (kTRUE == fbPulserModeEnable) {
    if (10.0 < dTsStartTime - fdLastRmsUpdateTime) {
      // Reset summary histograms for safety
      fhTimeMeanPulser->Reset();
      fhTimeRmsPulser->Reset();

      for (UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeA++)
        for (UInt_t uFeeB = 0; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++)
          if (NULL != fvhTimeDiffPulser[uFeeA][uFeeB]) {
            fhTimeMeanPulser->Fill(
              uFeeA, uFeeB, fvhTimeDiffPulser[uFeeA][uFeeB]->GetMean());
            fhTimeRmsPulser->Fill(
              uFeeA, uFeeB, fvhTimeDiffPulser[uFeeA][uFeeB]->GetRMS());
          }  // for( UInt_t uChan = 0; uChan < kuNbChanTest - 1; uChan++)
      fdLastRmsUpdateTime = dTsStartTime;
    }  // if( 10.0 < dTsStartTime - fdLastRmsUpdateTime )
  }    // if( kTRUE == fbPulserModeEnable )

  fulCurrentTsIndex++;

  return kTRUE;
}

void CbmMcbm2018MonitorTof::ProcessEpochCycle(uint64_t ulCycleData) {
  uint64_t ulEpochCycleVal = ulCycleData & gdpbv100::kulEpochCycleFieldSz;

  if (fuRawDataPrintMsgIdx < fuRawDataPrintMsgNb
      || gLogger->IsLogNeeded(fair::Severity::debug2)) {
    LOG(info)
      << "CbmMcbm2018MonitorTof::ProcessEpochCyle => "
      //                 << Form( " TS %5llu MS %3lu In data 0x%016llX Cycle 0x%016llX",
      //                           fulCurrentTsIndex, fuCurrentMs, ulCycleData, ulEpochCycleVal );
      << " TS " << FormatDecPrintout(fulCurrentTsIndex, 5) << " MS "
      << FormatDecPrintout(fuCurrentMs, 3) << " In data 0x"
      << FormatHexPrintout(ulCycleData, 16, '0', true) << " Cycle 0x"
      << FormatHexPrintout(ulEpochCycleVal, 16, '0', true);
    fuRawDataPrintMsgIdx++;
  }  // if( fuRawDataPrintMsgIdx < fuRawDataPrintMsgNb || gLogger->IsLogNeeded( fair::Severity::debug2 ) )

  for (uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGet4Index++) {
    fuGet4Id = uGet4Index;
    fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;
    /*
      if( !( ulEpochCycleVal == fvulCurrentEpochCycle[fuGet4Nr] ||
             ulEpochCycleVal == fvulCurrentEpochCycle[fuGet4Nr] + 1 ) )
         LOG(error) << "CbmMcbm2018MonitorTof::ProcessEpochCyle => "
                    << " Missmatch in epoch cycles detected, probably fake cycles due to epoch index corruption! "
                    << Form( " Current cycle 0x%09X New cycle 0x%09X", fvulCurrentEpochCycle[fuGet4Nr], ulEpochCycleVal );
*/
    fvulCurrentEpochCycle[fuGet4Nr] = ulEpochCycleVal;
  }  // for( uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGet4Index ++ )
  return;
}

void CbmMcbm2018MonitorTof::FillHitInfo(gdpbv100::Message mess) {
  UInt_t uChannel = mess.getGdpbHitChanId();
  UInt_t uTot     = mess.getGdpbHit32Tot();
  UInt_t uFts     = mess.getGdpbHitFineTs();

  ULong64_t ulCurEpochGdpbGet4 = fvulCurrentEpoch[fuGet4Nr];

  // In Ep. Suppr. Mode, receive following epoch instead of previous
  if (0 < ulCurEpochGdpbGet4)
    ulCurEpochGdpbGet4--;
  else
    ulCurEpochGdpbGet4 = gdpbv100::kuEpochCounterSz;  // Catch epoch cycle!

  UInt_t uChannelNr = fuGet4Id * fuNrOfChannelsPerGet4 + uChannel;
  UInt_t uChannelNrInFee =
    (fuGet4Id % fuNrOfGet4PerFee) * fuNrOfChannelsPerGet4 + uChannel;
  UInt_t uFeeNr      = (fuGet4Id / fuNrOfGet4PerFee);
  UInt_t uFeeNrInSys = fuGdpbNr * fuNrOfFeePerGdpb + uFeeNr;
  UInt_t uRemappedChannelNr =
    uFeeNr * fuNrOfChannelsPerFee + fvuGet4ToPadi[uChannelNrInFee];
  /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
  if (fuGdpbNr == fuDiamondDpbIdx || 0x90 == fuCurrentMsSysId)
    uRemappedChannelNr = uChannelNr;
  //   UInt_t uGbtxNr      = (uFeeNr / kuNbFeePerGbtx);
  //   UInt_t uFeeInGbtx  = (uFeeNr % kuNbFeePerGbtx);
  //   UInt_t uGbtxNrInSys = fuGdpbNr * kuNbGbtxPerGdpb + uGbtxNr;

  ULong_t ulHitTime = mess.getMsgFullTime(ulCurEpochGdpbGet4);
  Double_t dHitTime = mess.getMsgFullTimeD(ulCurEpochGdpbGet4);

  // In 32b mode the coarse counter is already computed back to 112 FTS bins
  // => need to hide its contribution from the Finetime
  // => FTS = Fullt TS modulo 112
  uFts = mess.getGdpbHitFullTs() % 112;

  fvvuChanNbHitsPerMs[fuGdpbNr][uChannelNr]++;
  fvhChCount_gDPB[fuGdpbNr]->Fill(uChannelNr);
  fvhRawFt_gDPB[fuGdpbNr]->Fill(uChannelNr, uFts);
  fvhRawTot_gDPB[fuGdpbNr]->Fill(uChannelNr, uTot);

  /// Remapped for PADI to GET4
  fvhRemapChCount_gDPB[fuGdpbNr]->Fill(uRemappedChannelNr);
  fvhRemapTot_gDPB[fuGdpbNr]->Fill(uRemappedChannelNr, uTot);
  /*
   if( uGbtxNrInSys < fuNrOfGbtx )
   {
      UInt_t uOffset = uGbtxNrInSys * kuNbFeeSide * fuNrOfChannelsPerFee;
      if( fviRpcSide[ uGbtxNrInSys ] )
         fvhRemapTotSideB_mod[ fviModuleId[ uGbtxNrInSys ] ]->Fill( uRemappedChannelNr - uOffset, uTot);
         else fvhRemapTotSideA_mod[ fviModuleId[ uGbtxNrInSys ] ]->Fill( uRemappedChannelNr - uOffset, uTot);
   } // if( uGbtxNrInSys < fuNrOfGbtx )
*/
  /*
   switch( ( uRemappedChannelNr / fuNrOfChannelsPerFee ) / kuNbFeeSide )
   {
      case 0: // Module 1 Side A
         fvhRemapTotSideA_mod[ fuGdpbNr ]->Fill(  uRemappedChannelNr , uTot);
      case 1: // Module 1 Side B
         fvhRemapTotSideB_mod[ fuGdpbNr ]->Fill(  uRemappedChannelNr - 160 , uTot);
      case 2: // Module 2 Side A
         fvhRemapTotSideA_mod[ fuGdpbNr ]->Fill(  uRemappedChannelNr - 160 , uTot);
      case 3: // Module 2 Side B
         fvhRemapTotSideB_mod[ fuGdpbNr ]->Fill(  uRemappedChannelNr - 320, uTot);
   } // switch( ( uRemappedChannelNr / fuNrOfChannelsPerFee ) % kuNbFeeSide )
*/
  // In Run rate evolution
  if (fdStartTime < 0) fdStartTime = dHitTime;

  // Reset the evolution Histogram and the start time when we reach the end of the range
  if (fuHistoryHistoSize < 1e-9 * (dHitTime - fdStartTime)) {
    ResetEvolutionHistograms();
    fdStartTime = dHitTime;
  }  // if( fuHistoryHistoSize < 1e-9 * (dHitTime - fdStartTime) )

  // In Run rate evolution
  if (fdStartTimeLong < 0) fdStartTimeLong = dHitTime;

  // Reset the evolution Histogram and the start time when we reach the end of the range
  if (fuHistoryHistoSizeLong < 1e-9 * (dHitTime - fdStartTimeLong) / 60.0) {
    ResetLongEvolutionHistograms();
    fdStartTimeLong = dHitTime;
  }  // if( fuHistoryHistoSize < 1e-9 * (dHitTime - fdStartTime) / 60.0 )

  ///* Pulser monitoring *///
  if (kTRUE == fbPulserModeEnable) {
    /// Save last hist time if pulser channel
    /// Fill the corresponding histos if the time difference is reasonnable )
    if (gdpbv100::kuFeePulserChannel == uChannelNrInFee
        && fuGdpbNr != fuDiamondDpbIdx) {
      fdTsLastPulserHit[uFeeNrInSys] = dHitTime;
      fvuFeeNbHitsLastMs[uFeeNrInSys]++;
      /*
         /// Update the difference to all other FEE with lower indices
         for( UInt_t uFeeB = 0; uFeeB < uFeeNrInSys; uFeeB++)
            if( NULL != fvhTimeDiffPulser[uFeeB][uFeeNrInSys] )
            {
               Double_t dTimeDiff = 1e3 * ( fdTsLastPulserHit[ uFeeNrInSys ] - fdTsLastPulserHit[ uFeeB ] );
               if( TMath::Abs( dTimeDiff ) < kdMaxDtPulserPs )
               {
                  fvhTimeDiffPulser[uFeeB][uFeeNrInSys]->Fill( dTimeDiff );

                  /// Dt Evo plots only for first FEE in each GBTx
                  if( 0 == uFeeInGbtx && 0 == uFeeB % kuNbFeePerGbtx )
                  {
                     /// Evo of GBTx inside same DPB
                     if( fuGdpbNr == uFeeB / fuNrOfFeePerGdpb )
                     {
                        if( 0 == uFeeB / kuNbFeePerGbtx )
                        {
                           UInt_t uPlotIdx = fuGdpbNr * ( kuNbGbtxPerGdpb - 1) + uGbtxNr - 1;
                           fvhPulserTimeDiffEvoGbtxGbtx[ uPlotIdx ]->Fill( 1e-9 / 60.0 * (dHitTime - fdStartTime), dTimeDiff );
                        }
                     } // if( fuGdpbNr == uFeeB / fuNrOfFeePerGdpb )
                        else // if( NULL != fvvhPulserTimeDiffEvoGdpbGdpb[ uFeeB / fuNrOfFeePerGdpb ][ fuGdpbNr ] )
                        {
                           /// Evo of DPBs if both first FEE in First GBTx
                           if( 0 == uGbtxNr && 0 == uFeeB / kuNbFeePerGbtx )
                              fvvhPulserTimeDiffEvoGdpbGdpb[ uFeeB / fuNrOfFeePerGdpb ][ fuGdpbNr ]->Fill(
                                 1e-9 / 60.0 * (dHitTime - fdStartTime), dTimeDiff );
                        } // else of if( fuGdpbNr == uFeeB / fuNrOfFeePerGdpb )
                  } // if( 0 == uFeeInGbtx && 0 == uFeeB % kuNbFeePerGbtx )
               } // if( TMath::Abs( dTimeDiff ) < kdMaxDtPulserPs )
   //               else if( 10 == uFeeB && 20 == uFeeNrInSys )
   //                  LOG(info) << "new in 20 " << dTimeDiff;
            } // if( NULL != fvhTimeDiffPulser[uFeeB][uFeeB] )

         /// Update the difference to all other FEE with higher indices
         for( UInt_t uFeeB = uFeeNrInSys + 1; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++)
            if( NULL != fvhTimeDiffPulser[uFeeNrInSys][uFeeB] )
            {
               Double_t dTimeDiff = 1e3 * ( fdTsLastPulserHit[ uFeeB ] - fdTsLastPulserHit[ uFeeNrInSys ] );
               if( TMath::Abs( dTimeDiff ) < kdMaxDtPulserPs )
               {
                  fvhTimeDiffPulser[uFeeNrInSys][uFeeB]->Fill( dTimeDiff );

                  /// Dt Evo plots only for first FEE in each GBTx
                  if( 0 == uFeeInGbtx && 0 == uFeeB % kuNbFeePerGbtx )
                  {
                     /// Evo of GBTx inside same DPB
                     if( fuGdpbNr == uFeeB / fuNrOfFeePerGdpb )
                     {
                        if( 0 == uGbtxNr  )
                        {
                           UInt_t uPlotIdx = fuGdpbNr * ( kuNbGbtxPerGdpb - 1)
                                            + ( uFeeB - fuGdpbNr * fuNrOfFeePerGdpb) / kuNbFeePerGbtx - 1;
                           fvhPulserTimeDiffEvoGbtxGbtx[ uPlotIdx ]->Fill( 1e-9 / 60.0 * (dHitTime - fdStartTime), dTimeDiff );
                        } // if( 0 == uGbtxNr  )
                     } // if( fuGdpbNr == uFeeB / fuNrOfFeePerGdpb )
                        else // if( NULL != fvvhPulserTimeDiffEvoGdpbGdpb[ uFeeB / fuNrOfFeePerGdpb ][ fuGdpbNr ] )
                        {
                           /// Evo of DPBs if both first FEE in First GBTx
                           if( 0 == uGbtxNr && 0 == uFeeB / kuNbFeePerGbtx )
                              fvvhPulserTimeDiffEvoGdpbGdpb[ fuGdpbNr ][ uFeeB / fuNrOfFeePerGdpb ]->Fill(
                                 1e-9 / 60.0 * (dHitTime - fdStartTime), dTimeDiff );
                        } // else of if( fuGdpbNr == uFeeB / fuNrOfFeePerGdpb )
                  } // if( 0 == uFeeInGbtx && 0 == uFeeB % kuNbFeePerGbtx )
               } // if( TMath::Abs( dTimeDiff ) < kdMaxDtPulserPs )
   //               else if( 10 == uFeeNrInSys && 20 == uFeeB )
   //                  LOG(info) << "new in 10 " << dTimeDiff;
            } // if( NULL != fvhTimeDiffPulser[uFeeNrInSys][uFeeB] )
*/
    }  // if( gdpbv100::kuFeePulserChannel == uChannelNrInFee )
       /// Diamond FEE have pulser on channel 0!
    else if ((fuGdpbNr == fuDiamondDpbIdx || 0x90 == fuCurrentMsSysId)
             && 0 == uChannelNrInFee) {
      fdTsLastPulserHit[uFeeNrInSys] = dHitTime;
      fvuFeeNbHitsLastMs[uFeeNrInSys]++;
    }  // if( fuGdpbNr == fuDiamondDpbIdx && 0 == uChannelNrInFee )
  }    // if( kTRUE == fbPulserModeEnable )

  /// Coincidence maps
  if (kTRUE == fbCoincMapsEnable) {
    fvdCoincTsLastHit[fuGdpbNr][uRemappedChannelNr] = dHitTime;
    fvuCoincNbHitsLastMs[fuGdpbNr][uRemappedChannelNr]++;
  }  // if( kTRUE == fbCoincMapsEnable )

  if (0 <= fdStartTime) {
    fvhChannelRate_gDPB[fuGdpbNr]->Fill(1e-9 * (dHitTime - fdStartTime),
                                        uChannelNr);
    fvhRemapChRate_gDPB[fuGdpbNr]->Fill(1e-9 * (dHitTime - fdStartTime),
                                        uRemappedChannelNr);
    fvhFeeRate_gDPB[(fuGdpbNr * fuNrOfFeePerGdpb) + uFeeNr]->Fill(
      1e-9 * (dHitTime - fdStartTime));
    fvhFeeErrorRatio_gDPB[(fuGdpbNr * fuNrOfFeePerGdpb) + uFeeNr]->Fill(
      1e-9 * (dHitTime - fdStartTime), 0, 1);

    /*
      fvhModRate[ fviModuleId[ uGbtxNrInSys ] ]->Fill( 1e-9 * (dHitTime - fdStartTime));
      fvhModErrorRatio[ fviModuleId[ uGbtxNrInSys ] ]->Fill( 1e-9 * (dHitTime - fdStartTime), 0, 1);
*/
  }  // if (0 <= fdStartTime)

  if (0 <= fdStartTimeLong) {
    fvhFeeRateLong_gDPB[(fuGdpbNr * fuNrOfFeePerGdpb) + uFeeNr]->Fill(
      1e-9 / 60.0 * (dHitTime - fdStartTimeLong), 1 / 60.0);
    fvhFeeErrorRatioLong_gDPB[(fuGdpbNr * fuNrOfFeePerGdpb) + uFeeNr]->Fill(
      1e-9 / 60.0 * (dHitTime - fdStartTimeLong), 0, 1 / 60.0);
  }  // if (0 <= fdStartTimeLong)

  if (kTRUE == fbPrintAllHitsEnable) {
    LOG(info) << "Hit: " << Form("0x%08x ", fuGdpbId) << ", " << std::setw(2)
              << fuGet4Nr << ", " << std::setw(3) << uChannel << ", "
              << std::setw(3) << uTot << ", epoch " << std::setw(3)
              << ulCurEpochGdpbGet4 << ", FullTime Clk "
              << Form("%12lu ", ulHitTime) << ", FullTime s "
              << Form("%12.9f ", dHitTime / 1e9) << ", FineTime " << uFts;
  }  // if( kTRUE == fbPrintAllHitsEnable )
}

void CbmMcbm2018MonitorTof::FillEpochInfo(gdpbv100::Message mess) {
  ULong64_t ulEpochNr = mess.getGdpbEpEpochNb();
  /*
   if( fvulCurrentEpoch[fuGet4Nr] < ulEpochNr )
      fvulCurrentEpochCycle[fuGet4Nr]++;
*/
  fvulCurrentEpoch[fuGet4Nr] = ulEpochNr;
  fvulCurrentEpochFull[fuGet4Nr] =
    ulEpochNr + gdpbv100::kulEpochCycleBins * fvulCurrentEpochCycle[fuGet4Nr];

  if (1 == mess.getGdpbEpSync()) fhGet4EpochFlags->Fill(fuGet4Nr, 0);
  if (1 == mess.getGdpbEpDataLoss()) fhGet4EpochFlags->Fill(fuGet4Nr, 1);
  if (1 == mess.getGdpbEpEpochLoss()) fhGet4EpochFlags->Fill(fuGet4Nr, 2);
  if (1 == mess.getGdpbEpMissmatch()) fhGet4EpochFlags->Fill(fuGet4Nr, 3);

  fulCurrentEpochTime = mess.getMsgFullTime(ulEpochNr);

  /// Re-align the epoch number of the message in case it will be used later:
  /// We received the epoch after the data instead of the one before!
  if (0 < ulEpochNr)
    mess.setGdpbEpEpochNb(ulEpochNr - 1);
  else
    mess.setGdpbEpEpochNb(gdpbv100::kuEpochCounterSz);

  Int_t iBufferSize = fvmEpSupprBuffer[fuGet4Nr].size();
  if (0 < iBufferSize) {
    LOG(debug) << "Now processing stored messages for for get4 " << fuGet4Nr
               << " with epoch number " << (fvulCurrentEpoch[fuGet4Nr] - 1);

    /// Data are sorted between epochs, not inside => Epoch level ordering
    /// Sorting at lower bin precision level
    std::stable_sort(fvmEpSupprBuffer[fuGet4Nr].begin(),
                     fvmEpSupprBuffer[fuGet4Nr].begin());

    for (Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++) {
      FillHitInfo(fvmEpSupprBuffer[fuGet4Nr][iMsgIdx]);
    }  // for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )

    fvmEpSupprBuffer[fuGet4Nr].clear();
  }  // if( 0 < fvmEpSupprBuffer[fuGet4Nr] )
}

void CbmMcbm2018MonitorTof::PrintSlcInfo(gdpbv100::Message mess) {
  if (fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find(fuGdpbId)) {
    //      UInt_t uChip = mess.getGdpbGenChipId();
    //      UInt_t uChan = mess.getGdpbSlcChan();
    //      UInt_t uEdge = mess.getGdpbSlcEdge();
    UInt_t uData       = mess.getGdpbSlcData();
    UInt_t uType       = mess.getGdpbSlcType();
    Double_t dGdpbChId = fuGet4Id * fuNrOfChannelsPerGet4
                         + mess.getGdpbSlcChan() + 0.5 * mess.getGdpbSlcEdge();
    Double_t dFullChId = fuGet4Nr * fuNrOfChannelsPerGet4
                         + mess.getGdpbSlcChan() + 0.5 * mess.getGdpbSlcEdge();
    Double_t dMessTime = static_cast<Double_t>(fulCurrentEpochTime) * 1.e-9;

    switch (uType) {
      case 0:  // Scaler counter
      {
        fhGet4ChanScm->Fill(dFullChId, uType);
        fvhGdpbGet4ChanScm[fuGdpbNr]->Fill(dGdpbChId, uType);
        fhScmScalerCounters->Fill(uData, dFullChId);
        break;
      }
      case 1:  // Deadtime counter
      {
        fhGet4ChanScm->Fill(dFullChId, uType);
        fvhGdpbGet4ChanScm[fuGdpbNr]->Fill(dGdpbChId, uType);
        fhScmDeadtimeCounters->Fill(uData, dFullChId);
        break;
      }
      case 2:  // SPI message
      {
        /*
            LOG(info) << "GET4 Slow Control message, epoch "
                       << static_cast<Int_t>(fvulCurrentEpoch[fuGet4Nr]) << ", time " << std::setprecision(9)
                       << std::fixed << dMessTime << " s "
                       << " for board ID " << std::hex << std::setw(4) << fuGdpbId
                       << std::dec << "\n"
                       << " +++++++ > Chip = "
                       << std::setw(2) << uChip << ", Chan = "
                       << std::setw(1) << uChan << ", Edge = "
                       << std::setw(1) << uEdge << ", Type = "
                       << std::setw(1) << mess.getGdpbSlcType() << ", Data = 0x"
//                 << std::hex << std::setw(6) << mess.getGdpbSlcData()
                       << Form( "%06x", uData )
                       << std::dec << ", CRC = " << uCRC
//                 << " RAW: " << Form( "%08x", mess.getGdpbSlcMess() );
*/
        fhGet4ChanScm->Fill(dFullChId, uType);
        fvhGdpbGet4ChanScm[fuGdpbNr]->Fill(dGdpbChId, uType);
        break;
      }
      case 3:  // Start message or SEU counter
      {
        if (0 == mess.getGdpbSlcChan()
            && 0 == mess.getGdpbSlcEdge())  // START message
        {
          /*
               LOG(info) << std::setprecision(9)
                             << std::fixed << dMessTime << " s ";
            LOG(info) << "GET4 Slow Control message, epoch "
                    << static_cast<Int_t>(fvulCurrentEpoch[fuGet4Nr]) << ", time " << std::setprecision(9)
                    << std::fixed << dMessTime << " s "
                    << " for board ID " << std::hex << std::setw(4) << fuGdpbId
                    << std::dec << "\n"
                    << " +++++++ > Chip = "
                    << std::setw(2) << mess.getGdpbGenChipId() << ", Chan = "
                    << std::setw(1) << mess.getGdpbSlcChan() << ", Edge = "
                    << std::setw(1) << mess.getGdpbSlcEdge() << ", Type = "
                    << std::setw(1) << mess.getGdpbSlcType() << ", Data = 0x"
   //                 << std::hex << std::setw(6) << mess.getGdpbSlcData()
                    << Form( "%06x", mess.getGdpbSlcData() )
                    << std::dec << ", CRC = " << mess.getGdpbSlcCrc();
*/
          fhGet4ChanScm->Fill(dFullChId, uType + 1);
          fvhGdpbGet4ChanScm[fuGdpbNr]->Fill(dGdpbChId, uType + 1);
        }  // if( 0 == mess.getGdpbSlcChan() && 0 == mess.getGdpbSlcEdge() )
        else if (0 == mess.getGdpbSlcChan()
                 && 1 == mess.getGdpbSlcEdge())  // SEU counter message
        {
          /*
         LOG(info) << "GET4 Slow Control message, epoch "
                 << static_cast<Int_t>(fvulCurrentEpoch[fuGet4Nr]) << ", time " << std::setprecision(9)
                 << std::fixed << dMessTime << " s "
                 << " for board ID " << std::hex << std::setw(4) << fuGdpbId
                 << std::dec << "\n"
                 << " +++++++ > Chip = "
                 << std::setw(2) << mess.getGdpbGenChipId() << ", Chan = "
                 << std::setw(1) << mess.getGdpbSlcChan() << ", Edge = "
                 << std::setw(1) << mess.getGdpbSlcEdge() << ", Type = "
                 << std::setw(1) << mess.getGdpbSlcType() << ", Data = 0x"
//                 << std::hex << std::setw(6) << mess.getGdpbSlcData()
                 << Form( "%06x", mess.getGdpbSlcData() )
                 << std::dec << ", CRC = " << mess.getGdpbSlcCrc();
*/
          fhGet4ChanScm->Fill(dFullChId, uType);
          fvhGdpbGet4ChanScm[fuGdpbNr]->Fill(dGdpbChId, uType);
          fhScmSeuCounters->Fill(uData, dFullChId);
          fhScmSeuCountersEvo->Fill(
            dMessTime - fdStartTime * 1.e-9, uData, dFullChId);
        }  // else if( 0 == mess.getGdpbSlcChan() && 1 == mess.getGdpbSlcEdge() )
        break;
      }
      default:  // Should never happen
        break;
    }  // switch( mess.getGdpbSlcType() )
  }
}

void CbmMcbm2018MonitorTof::PrintGenInfo(gdpbv100::Message mess) {
  Int_t mType    = mess.getMessageType();
  Int_t channel  = mess.getGdpbHitChanId();
  uint64_t uData = mess.getData();

  LOG(debug) << "Get4 MSG type " << mType << " from gdpbId " << fuGdpbId
             << ", getId " << fuGet4Id << ", (hit channel) " << channel
             << " data " << std::hex << uData;
}

void CbmMcbm2018MonitorTof::PrintSysInfo(gdpbv100::Message mess) {
  if (fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find(fuGdpbId))
    LOG(debug) << "GET4 System message,       epoch "
               << static_cast<Int_t>(fvulCurrentEpoch[fuGet4Nr]) << ", time "
               << std::setprecision(9) << std::fixed
               << Double_t(fulCurrentEpochTime) * 1.e-9 << " s "
               << " for board ID " << std::hex << std::setw(4) << fuGdpbId
               << std::dec;

  switch (mess.getGdpbSysSubType()) {
    case gdpbv100::SYS_GET4_ERROR: {
      uint32_t uData = mess.getGdpbSysErrData();
      if (gdpbv100::GET4_V2X_ERR_TOT_OVERWRT == uData
          || gdpbv100::GET4_V2X_ERR_TOT_RANGE == uData
          || gdpbv100::GET4_V2X_ERR_EVT_DISCARD == uData
          || gdpbv100::GET4_V2X_ERR_ADD_RIS_EDG == uData
          || gdpbv100::GET4_V2X_ERR_UNPAIR_FALL == uData
          || gdpbv100::GET4_V2X_ERR_SEQUENCE_ER == uData)
        LOG(debug) << " +++++++ > gDPB: " << std::hex << std::setw(4)
                   << fuGdpbId << std::dec << ", Chip = " << std::setw(2)
                   << mess.getGdpbGenChipId() << ", Chan = " << std::setw(1)
                   << mess.getGdpbSysErrChanId() << ", Edge = " << std::setw(1)
                   << mess.getGdpbSysErrEdge() << ", Empt = " << std::setw(1)
                   << mess.getGdpbSysErrUnused() << ", Data = " << std::hex
                   << std::setw(2) << uData << std::dec
                   << " -- GET4 V1 Error Event";
      else
        LOG(debug) << " +++++++ >gDPB: " << std::hex << std::setw(4) << fuGdpbId
                   << std::dec << ", Chip = " << std::setw(2)
                   << mess.getGdpbGenChipId() << ", Chan = " << std::setw(1)
                   << mess.getGdpbSysErrChanId() << ", Edge = " << std::setw(1)
                   << mess.getGdpbSysErrEdge() << ", Empt = " << std::setw(1)
                   << mess.getGdpbSysErrUnused() << ", Data = " << std::hex
                   << std::setw(2) << uData << std::dec
                   << " -- GET4 V1 Error Event ";
      break;
    }  // case gdpbv100::SYSMSG_GET4_EVENT
    case gdpbv100::SYS_GDPB_UNKWN: {
      LOG(debug) << "Unknown GET4 message, data: " << std::hex << std::setw(8)
                 << mess.getGdpbSysUnkwData() << std::dec
                 << " Full message: " << std::hex << std::setw(16)
                 << mess.getData() << std::dec;
      break;
    }  // case gdpbv100::SYS_GDPB_UNKWN:
    case gdpbv100::SYS_GET4_SYNC_MISS: {
      if (mess.getGdpbSysFwErrResync())
        LOG(info) << Form("GET4 Resynchronization: Get4:0x%04x ",
                          mess.getGdpbGenChipId())
                  << std::hex << std::setw(4) << fuGdpbId << std::dec;
      else
        LOG(info) << "GET4 synchronization pulse missing in gDPB " << std::hex
                  << std::setw(4) << fuGdpbId << std::dec;
      break;
    }  // case gdpbv100::SYS_GET4_SYNC_MISS:
    case gdpbv100::SYS_PATTERN: {
      LOG(debug) << "ASIC pattern for missmatch, disable or resync";
      break;
    }  // case gdpbv100::SYS_PATTERN:
    default: {
      LOG(debug) << "Crazy system message, subtype "
                 << mess.getGdpbSysSubType();
      break;
    }  // default

  }  // switch( getGdpbSysSubType() )
}

void CbmMcbm2018MonitorTof::FillPattInfo(gdpbv100::Message mess) {
  uint16_t usType   = mess.getGdpbSysPattType();
  uint16_t usIndex  = mess.getGdpbSysPattIndex();
  uint32_t uPattern = mess.getGdpbSysPattPattern();

  switch (usType) {
    case gdpbv100::PATT_MISSMATCH: {
      LOG(debug) << Form(
        "Missmatch pattern message => Type %d, Index %2d, Pattern 0x%08X",
        usType,
        usIndex,
        uPattern);
      for (UInt_t uBit = 0; uBit < 32; ++uBit) {
        UInt_t uBadAsic = ConvertElinkToGet4(32 * usIndex + uBit);
        /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
        if (fuGdpbNr == fuDiamondDpbIdx || 0x90 == fuCurrentMsSysId)
          uBadAsic = 32 * usIndex + uBit;

        if ((uPattern >> uBit) & 0x1) {
          fhPatternMissmatch->Fill(uBadAsic, fuGdpbNr);
          fvhGdpbPatternMissmatchEvo[fuGdpbNr]->Fill(fulCurrentTsIndex,
                                                     uBadAsic);
          fvvbGdpbLastMissmatchPattern[fuGdpbNr][uBadAsic] = kTRUE;
        }  // if( ( uPattern >> uBit ) & 0x1 )
        else
          fvvbGdpbLastMissmatchPattern[fuGdpbNr][uBadAsic] = kFALSE;

      }  // for( UInt_t uBit = 0; uBit < 32; ++uBit )
      break;
    }  // case gdpbv100::PATT_MISSMATCH:
    case gdpbv100::PATT_ENABLE: {
      for (UInt_t uBit = 0; uBit < 32; ++uBit) {
        UInt_t uAsic = ConvertElinkToGet4(32 * usIndex + uBit);
        /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
        if (fuGdpbNr == fuDiamondDpbIdx || 0x90 == fuCurrentMsSysId)
          uAsic = 32 * usIndex + uBit;

        if ((uPattern >> uBit) & 0x1) {
          fhPatternEnable->Fill(uAsic, fuGdpbNr);
          fvhGdpbPatternEnableEvo[fuGdpbNr]->Fill(fulCurrentTsIndex, uAsic);
          fvvbGdpbLastEnablePattern[fuGdpbNr][uAsic] = kFALSE;
        }  // if( ( uPattern >> uBit ) & 0x1 )
        else
          fvvbGdpbLastEnablePattern[fuGdpbNr][uAsic] = kTRUE;

      }  // for( UInt_t uBit = 0; uBit < 32; ++uBit )
      break;
    }  // case gdpbv100::PATT_ENABLE:
    case gdpbv100::PATT_RESYNC: {
      LOG(debug) << Form(
        "RESYNC pattern message => Type %d, Index %2d, Pattern 0x%08X",
        usType,
        usIndex,
        uPattern);

      for (UInt_t uBit = 0; uBit < 32; ++uBit) {
        UInt_t uBadAsic = ConvertElinkToGet4(32 * usIndex + uBit);
        /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
        if (fuGdpbNr == fuDiamondDpbIdx || 0x90 == fuCurrentMsSysId)
          uBadAsic = 32 * usIndex + uBit;

        if ((uPattern >> uBit) & 0x1) {
          fhPatternResync->Fill(uBadAsic, fuGdpbNr);
          fvhGdpbPatternResyncEvo[fuGdpbNr]->Fill(fulCurrentTsIndex, uBadAsic);
          fvvbGdpbLastResyncPattern[fuGdpbNr][uBadAsic] = kTRUE;
        }  // if( ( uPattern >> uBit ) & 0x1 )
        else
          fvvbGdpbLastResyncPattern[fuGdpbNr][uBadAsic] = kFALSE;

      }  // for( UInt_t uBit = 0; uBit < 32; ++uBit )
      break;
    }  // case gdpbv100::PATT_RESYNC:
    default: {
      LOG(debug) << "Crazy pattern message, subtype " << usType;
      break;
    }  // default
  }    // switch( usType )

  return;
}

void CbmMcbm2018MonitorTof::FillStarTrigInfo(gdpbv100::Message mess) {
  Int_t iMsgIndex = mess.getStarTrigMsgIndex();

  switch (iMsgIndex) {
    case 0:
      fvhTokenMsgType[fuGdpbNr]->Fill(0);
      fvulGdpbTsMsb[fuGdpbNr] = mess.getGdpbTsMsbStarA();
      break;
    case 1:
      fvhTokenMsgType[fuGdpbNr]->Fill(1);
      fvulGdpbTsLsb[fuGdpbNr] = mess.getGdpbTsLsbStarB();
      fvulStarTsMsb[fuGdpbNr] = mess.getStarTsMsbStarB();
      break;
    case 2:
      fvhTokenMsgType[fuGdpbNr]->Fill(2);
      fvulStarTsMid[fuGdpbNr] = mess.getStarTsMidStarC();
      break;
    case 3: {
      fvhTokenMsgType[fuGdpbNr]->Fill(3);

      ULong64_t ulNewGdpbTsFull =
        (fvulGdpbTsMsb[fuGdpbNr] << 24) + (fvulGdpbTsLsb[fuGdpbNr]);
      ULong64_t ulNewStarTsFull = (fvulStarTsMsb[fuGdpbNr] << 48)
                                  + (fvulStarTsMid[fuGdpbNr] << 8)
                                  + mess.getStarTsLsbStarD();
      UInt_t uNewToken   = mess.getStarTokenStarD();
      UInt_t uNewDaqCmd  = mess.getStarDaqCmdStarD();
      UInt_t uNewTrigCmd = mess.getStarTrigCmdStarD();

      if ((uNewToken == fvuStarTokenLast[fuGdpbNr])
          && (ulNewGdpbTsFull == fvulGdpbTsFullLast[fuGdpbNr])
          && (ulNewStarTsFull == fvulStarTsFullLast[fuGdpbNr])
          && (uNewDaqCmd == fvuStarDaqCmdLast[fuGdpbNr])
          && (uNewTrigCmd == fvuStarTrigCmdLast[fuGdpbNr])) {
        UInt_t uTrigWord = ((fvuStarTrigCmdLast[fuGdpbNr] & 0x00F) << 16)
                           + ((fvuStarDaqCmdLast[fuGdpbNr] & 0x00F) << 12)
                           + ((fvuStarTokenLast[fuGdpbNr] & 0xFFF));
        LOG(warning) << "Possible error: identical STAR tokens found twice in "
                        "a row => ignore 2nd! "
                     << " TS " << fulCurrentTsIndex << " gDBB #" << fuGdpbNr
                     << " " << Form("token = %5u ", fvuStarTokenLast[fuGdpbNr])
                     << Form("gDPB ts  = %12llu ", fvulGdpbTsFullLast[fuGdpbNr])
                     << Form("STAR ts = %12llu ", fvulStarTsFullLast[fuGdpbNr])
                     << Form("DAQ cmd = %2u ", fvuStarDaqCmdLast[fuGdpbNr])
                     << Form("TRG cmd = %2u ", fvuStarTrigCmdLast[fuGdpbNr])
                     << Form("TRG Wrd = %5x ", uTrigWord);
        return;
      }  // if exactly same message repeated
         /*
         if( (uNewToken != fuStarTokenLast[fuGdpbNr] + 1) &&
             0 < fvulGdpbTsFullLast[fuGdpbNr] && 0 < fvulStarTsFullLast[fuGdpbNr] &&
             ( 4095 != fvuStarTokenLast[fuGdpbNr] || 1 != uNewToken)  )
            LOG(warning) << "Possible error: STAR token did not increase by exactly 1! "
                         << " gDBB #" << fuGdpbNr << " "
                         << Form("old = %5u vs new = %5u ", fvuStarTokenLast[fuGdpbNr],   uNewToken)
                         << Form("old = %12llu vs new = %12llu ", fvulGdpbTsFullLast[fuGdpbNr], ulNewGdpbTsFull)
                         << Form("old = %12llu vs new = %12llu ", fvulStarTsFullLast[fuGdpbNr], ulNewStarTsFull)
                         << Form("old = %2u vs new = %2u ", fvuStarDaqCmdLast[fuGdpbNr],  uNewDaqCmd)
                         << Form("old = %2u vs new = %2u ", fvuStarTrigCmdLast[fuGdpbNr], uNewTrigCmd);
*/
      // STAR TS counter reset detection
      if (ulNewStarTsFull < fvulStarTsFullLast[fuGdpbNr])
        LOG(debug) << "Probable reset of the STAR TS: old = "
                   << Form("%16llu", fvulStarTsFullLast[fuGdpbNr])
                   << " new = " << Form("%16llu", ulNewStarTsFull)
                   << " Diff = -"
                   << Form("%8llu",
                           fvulStarTsFullLast[fuGdpbNr] - ulNewStarTsFull);

      /*
         LOG(info) << "Updating  trigger token for " << fuGdpbNr
                   << " " << fuStarTokenLast[fuGdpbNr] << " " << uNewToken;
*/
      //         ULong64_t ulGdpbTsDiff = ulNewGdpbTsFull - fvulGdpbTsFullLast[fuGdpbNr];
      fvulGdpbTsFullLast[fuGdpbNr] = ulNewGdpbTsFull;
      fvulStarTsFullLast[fuGdpbNr] = ulNewStarTsFull;
      fvuStarTokenLast[fuGdpbNr]   = uNewToken;
      fvuStarDaqCmdLast[fuGdpbNr]  = uNewDaqCmd;
      fvuStarTrigCmdLast[fuGdpbNr] = uNewTrigCmd;

      /// Histograms filling only in core MS
      if (fuCurrentMs < fuCoreMs) {
        /// In Run rate evolution
        if (0 <= fdStartTime) {
          /// Reset the evolution Histogram and the start time when we reach the end of the range
          if (fuHistoryHistoSize
              < 1e-9
                  * (fvulGdpbTsFullLast[fuGdpbNr] * gdpbv100::kdClockCycleSizeNs
                     - fdStartTime)) {
            ResetEvolutionHistograms();
            fdStartTime =
              fvulGdpbTsFullLast[fuGdpbNr] * gdpbv100::kdClockCycleSizeNs;
          }  // if( fuHistoryHistoSize < 1e-9 * (fulGdpbTsFullLast * gdpbv100::kdClockCycleSizeNs - fdStartTime) )

          fvhTriggerRate[fuGdpbNr]->Fill(
            1e-9
            * (fvulGdpbTsFullLast[fuGdpbNr] * gdpbv100::kdClockCycleSizeNs
               - fdStartTime));
          fvhStarTokenEvo[fuGdpbNr]->Fill(
            1e-9
              * (fvulGdpbTsFullLast[fuGdpbNr] * gdpbv100::kdClockCycleSizeNs
                 - fdStartTime),
            fvuStarTokenLast[fuGdpbNr]);
          fvhStarTrigGdpbTsEvo[fuGdpbNr]->Fill(
            1e-9
              * (fvulGdpbTsFullLast[fuGdpbNr] * gdpbv100::kdClockCycleSizeNs
                 - fdStartTime),
            fvulGdpbTsFullLast[fuGdpbNr]);
          fvhStarTrigStarTsEvo[fuGdpbNr]->Fill(
            1e-9
              * (fvulGdpbTsFullLast[fuGdpbNr] * gdpbv100::kdClockCycleSizeNs
                 - fdStartTime),
            fvulStarTsFullLast[fuGdpbNr]);
        }  // if( 0 < fdStartTime )
        else
          fdStartTime =
            fvulGdpbTsFullLast[fuGdpbNr] * gdpbv100::kdClockCycleSizeNs;
        fvhCmdDaqVsTrig[fuGdpbNr]->Fill(fvuStarDaqCmdLast[fuGdpbNr],
                                        fvuStarTrigCmdLast[fuGdpbNr]);
      }  // if( fuCurrentMs < fuCoreMs  )

      break;
    }  // case 3
    default: LOG(error) << "Unknown Star Trigger messageindex: " << iMsgIndex;
  }  // switch( iMsgIndex )
}

void CbmMcbm2018MonitorTof::Reset() {}

void CbmMcbm2018MonitorTof::Finish() {
  // Printout some stats on what was unpacked
  TString message_type;
  for (unsigned int i = 0; i < fviMsgCounter.size(); ++i) {
    switch (i) {
      case 0: message_type = "NOP"; break;
      case 1: message_type = "HIT"; break;
      case 2: message_type = "EPOCH"; break;
      case 3: message_type = "SYNC"; break;
      case 4: message_type = "AUX"; break;
      case 5: message_type = "EPOCH2"; break;
      case 6: message_type = "GET4"; break;
      case 7: message_type = "SYS"; break;
      case 8: message_type = "GET4_SLC"; break;
      case 9: message_type = "GET4_32B"; break;
      case 10: message_type = "GET4_SYS"; break;
      default: message_type = "UNKNOWN"; break;
    }  // switch(i)
    LOG(info) << message_type << " messages: " << fviMsgCounter[i];
  }  // for (unsigned int i=0; i< fviMsgCounter.size(); ++i)

  LOG(info) << "-------------------------------------";
  for (UInt_t i = 0; i < fuNrOfGdpbs; ++i) {
    for (UInt_t j = 0; j < fuNrOfGet4PerGdpb; ++j) {
      LOG(info) << "Last epoch for gDPB: " << std::hex << std::setw(4) << i
                << std::dec << " , GET4  " << std::setw(4) << j << " => "
                << fvulCurrentEpoch[GetArrayIndex(i, j)];
    }  // for (UInt_t j = 0; j < fuNrOfGet4PerGdpb; ++j)
  }    // for (UInt_t i = 0; i < fuNrOfGdpbs; ++i)
  LOG(info) << "-------------------------------------";


  /// Update RMS plots
  if (kTRUE == fbPulserModeEnable) {
    /// Reset summary histograms for safety
    fhTimeMeanPulser->Reset();
    fhTimeRmsPulser->Reset();

    for (UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeA++)
      for (UInt_t uFeeB = 0; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++)
        if (NULL != fvhTimeDiffPulser[uFeeA][uFeeB]) {
          fhTimeMeanPulser->Fill(
            uFeeA, uFeeB, fvhTimeDiffPulser[uFeeA][uFeeB]->GetMean());
          fhTimeRmsPulser->Fill(
            uFeeA, uFeeB, fvhTimeDiffPulser[uFeeA][uFeeB]->GetRMS());
        }  // for( UInt_t uChan = 0; uChan < kuNbChanTest - 1; uChan++)

    /// Update zoomed RMS and pulser fit plots
    UpdateZoomedFit();
  }  // if( kTRUE == fbPulserModeEnable )

  SaveAllHistos(fsHistoFilename);
}

void CbmMcbm2018MonitorTof::SaveAllHistos(TString sFileName) {
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* histoFile   = NULL;
  if ("" != sFileName) {
    // open separate histo file in recreate mode
    histoFile = new TFile(sFileName, "RECREATE");
    histoFile->cd();
  }  // if( "" != sFileName )

  gDirectory->mkdir("Tof_Raw_gDPB");
  gDirectory->cd("Tof_Raw_gDPB");

  fhMessType->Write();
  fhSysMessType->Write();
  fhGet4MessType->Write();
  fhGet4ChanScm->Write();
  fhGet4ChanErrors->Write();
  fhGet4EpochFlags->Write();

  fhGdpbMessType->Write();
  fhGdpbSysMessType->Write();
  fhGdpbSysMessPattType->Write();
  fhGdpbEpochFlags->Write();
  fhGdpbEpochSyncEvo->Write();
  fhGdpbEpochMissEvo->Write();

  fhScmScalerCounters->Write();
  fhScmDeadtimeCounters->Write();
  fhScmSeuCounters->Write();
  fhScmSeuCountersEvo->Write();

  fhPatternMissmatch->Write();
  fhPatternEnable->Write();
  fhPatternResync->Write();

  for (UInt_t uTotPlot = 0; uTotPlot < fvhRawTot_gDPB.size(); ++uTotPlot)
    fvhRawTot_gDPB[uTotPlot]->Write();

  for (UInt_t uTotPlot = 0; uTotPlot < fvhRemapTot_gDPB.size(); ++uTotPlot)
    fvhRemapTot_gDPB[uTotPlot]->Write();
  /*
   for( UInt_t uMod = 0; uMod < fuNrOfModules; uMod ++ )
   {
      fvhRemapTotSideA_mod[ uMod ]->Write();
      fvhRemapTotSideB_mod[ uMod ]->Write();

      fvhModRate[ uMod ]->Write();
      fvhModErrorRate[ uMod ]->Write();
      fvhModErrorRatio[ uMod ]->Write();
   } // for( UInt_t uMod = 0; uMod < fuNrOfModules; uMod ++ )
*/
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fvhGdpbGet4MessType[uGdpb]->Write();
    fvhGdpbGet4ChanScm[uGdpb]->Write();
    fvhGdpbGet4ChanErrors[uGdpb]->Write();

    fvhGdpbPatternMissmatchEvo[uGdpb]->Write();
    fvhGdpbPatternEnableEvo[uGdpb]->Write();
    fvhGdpbPatternResyncEvo[uGdpb]->Write();

    fvhRawFt_gDPB[uGdpb]->Write();
    fvhChCount_gDPB[uGdpb]->Write();
    fvhChannelRate_gDPB[uGdpb]->Write();
    fvhRemapChCount_gDPB[uGdpb]->Write();
    fvhRemapChRate_gDPB[uGdpb]->Write();

    for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb; ++uFee) {
      fvhFeeRate_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Write();
      fvhFeeErrorRate_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Write();
      fvhFeeErrorRatio_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Write();
      fvhFeeRateLong_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Write();
      fvhFeeErrorRateLong_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Write();
      fvhFeeErrorRatioLong_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Write();
    }  // for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb; ++ uFee)

  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  if (kTRUE == fbPulserModeEnable) {
    fhTimeMeanPulser->Write();
    fhTimeRmsPulser->Write();
    fhTimeRmsZoomFitPuls->Write();
    fhTimeResFitPuls->Write();
  }  // if( kTRUE == fbPulserModeEnable )
  gDirectory->cd("..");

  gDirectory->mkdir("Tof_Chan_FineCount");
  gDirectory->cd("Tof_Chan_FineCount");
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fhHitsPerMsFirstChan_gDPB[uGdpb]->Write();
    fvhChannelRatePerMs_gDPB[uGdpb]->Write();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  gDirectory->cd("..");

  gDirectory->mkdir("Tof_Ena");
  gDirectory->cd("Tof_Ena");
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fvhGdpbMissmatchEvoPerTs[uGdpb]->Write();
    fvhGdpbMissmatchEnaEvoPerTs[uGdpb]->Write();
    fvhGdpbEnableEvoPerTs[uGdpb]->Write();
    fvhGdpbResyncEvoPerTs[uGdpb]->Write();
    fvhGdpbResyncEnaEvoPerTs[uGdpb]->Write();
    fvhGdpbStateEvoPerTs[uGdpb]->Write();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  gDirectory->cd("..");

  ///* STAR event building/cutting *///
  gDirectory->mkdir("Star_Raw");
  gDirectory->cd("Star_Raw");
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fvhTokenMsgType[uGdpb]->Write();
    fvhTriggerRate[uGdpb]->Write();
    fvhCmdDaqVsTrig[uGdpb]->Write();
    fvhStarTokenEvo[uGdpb]->Write();
    fvhStarTrigGdpbTsEvo[uGdpb]->Write();
    fvhStarTrigStarTsEvo[uGdpb]->Write();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  gDirectory->cd("..");

  ///* Pulser monitoring *///
  if (kTRUE == fbPulserModeEnable) {
    gDirectory->mkdir("TofDt");
    gDirectory->cd("TofDt");
    for (UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeA++)
      for (UInt_t uFeeB = 0; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++)
        if (NULL != fvhTimeDiffPulser[uFeeA][uFeeB])
          fvhTimeDiffPulser[uFeeA][uFeeB]->Write();
    gDirectory->cd("..");

    ///* Pulser evolution monitoring *///
    gDirectory->mkdir("TofDtEvo");
    gDirectory->cd("TofDtEvo");
    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
      for (UInt_t uGbtx = 0; uGbtx < kuNbGbtxPerGdpb - 1; ++uGbtx)
        fvhPulserTimeDiffEvoGbtxGbtx[uGdpb * (kuNbGbtxPerGdpb - 1) + uGbtx]
          ->Write();

      for (UInt_t uGdpbB = uGdpb + 1; uGdpbB < fuNrOfGdpbs; ++uGdpbB)
        fvvhPulserTimeDiffEvoGdpbGdpb[uGdpb][uGdpbB]->Write();
    }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
    gDirectory->cd("..");
  }  // if( kTRUE == fbPulserModeEnable )

  ///* Coincidence maps *///
  if (kTRUE == fbCoincMapsEnable) {
    gDirectory->mkdir("TofCoinc");
    gDirectory->cd("TofCoinc");
    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
      fvhCoincMapAllChanGdpb[uGdpb]->Write();
      fvhCoincMeanAllChanGdpb[uGdpb]->Write();
    }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
    gDirectory->cd("..");
  }  // if( kTRUE == fbCoincMapsEnable )

  gDirectory->mkdir("Flib_Raw");
  gDirectory->cd("Flib_Raw");
  for (UInt_t uLinks = 0; uLinks < fvhMsSzPerLink.size(); uLinks++) {
    if (NULL == fvhMsSzPerLink[uLinks]) continue;

    fvhMsSzPerLink[uLinks]->Write();
    fvhMsSzTimePerLink[uLinks]->Write();
  }  // for( UInt_t uLinks = 0; uLinks < fvhMsSzPerLink.size(); uLinks++ )

  TH1* pMissedTsH1 = dynamic_cast<TH1*>(gROOT->FindObjectAny("Missed_TS"));
  if (NULL != pMissedTsH1) pMissedTsH1->Write();

  TProfile* pMissedTsEvoP =
    dynamic_cast<TProfile*>(gROOT->FindObjectAny("Missed_TS_Evo"));
  if (NULL != pMissedTsEvoP) pMissedTsEvoP->Write();

  gDirectory->cd("..");

  gDirectory->mkdir("canvases");
  gDirectory->cd("canvases");
  fcSummary->Write();
  fcSummaryGdpb->Write();
  fcGdpbChannelCount->Write();
  fcGdpbRemapChCount->Write();
  fcGdpbChannelRate->Write();
  fcGdpbRemapChRate->Write();
  gDirectory->cd("..");


  if ("" != sFileName) {
    // Restore original directory position
    histoFile->Close();
    /// Restore old global file and folder pointer to avoid messing with FairRoot
    gFile      = oldFile;
    gDirectory = oldDir;
  }  // if( "" != sFileName )
}

void CbmMcbm2018MonitorTof::ResetAllHistos() {
  LOG(info) << "Reseting all TOF histograms.";

  fhMessType->Reset();
  fhSysMessType->Reset();
  fhGet4MessType->Reset();
  fhGet4ChanScm->Reset();
  fhGet4ChanErrors->Reset();
  fhGet4EpochFlags->Reset();

  fhGdpbMessType->Reset();
  fhGdpbSysMessType->Reset();
  fhGdpbSysMessPattType->Reset();
  fhGdpbEpochFlags->Reset();
  fhGdpbEpochSyncEvo->Reset();
  fhGdpbEpochMissEvo->Reset();

  fhScmScalerCounters->Reset();
  fhScmDeadtimeCounters->Reset();
  fhScmSeuCounters->Reset();
  fhScmSeuCountersEvo->Reset();

  fhPatternMissmatch->Reset();
  fhPatternEnable->Reset();
  fhPatternResync->Reset();

  for (UInt_t uTotPlot = 0; uTotPlot < fvhRawTot_gDPB.size(); ++uTotPlot)
    fvhRawTot_gDPB[uTotPlot]->Reset();

  for (UInt_t uTotPlot = 0; uTotPlot < fvhRemapTot_gDPB.size(); ++uTotPlot)
    fvhRemapTot_gDPB[uTotPlot]->Reset();
  /*
   for( UInt_t uMod = 0; uMod < fuNrOfModules; uMod ++ )
   {
      fvhRemapTotSideA_mod[ uMod ]->Reset();
      fvhRemapTotSideB_mod[ uMod ]->Reset();

      fvhModRate[ uMod ]->Reset();
      fvhModErrorRate[ uMod ]->Reset();
      fvhModErrorRatio[ uMod ]->Reset();
   } // for( UInt_t uMod = 0; uMod < fuNrOfModules; uMod ++ )
*/
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fvhGdpbGet4MessType[uGdpb]->Reset();
    fvhGdpbGet4ChanScm[uGdpb]->Reset();
    fvhGdpbGet4ChanErrors[uGdpb]->Reset();

    fvhGdpbPatternMissmatchEvo[uGdpb]->Reset();
    fvhGdpbPatternEnableEvo[uGdpb]->Reset();
    fvhGdpbPatternResyncEvo[uGdpb]->Reset();

    fvhRawFt_gDPB[uGdpb]->Reset();
    fvhChCount_gDPB[uGdpb]->Reset();
    fvhChannelRate_gDPB[uGdpb]->Reset();
    fvhRemapChCount_gDPB[uGdpb]->Reset();
    fvhRemapChRate_gDPB[uGdpb]->Reset();

    for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb; ++uFee) {
      fvhFeeRate_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Reset();
      fvhFeeErrorRate_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Reset();
      fvhFeeErrorRatio_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Reset();
      fvhFeeRateLong_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Reset();
      fvhFeeErrorRateLong_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Reset();
      fvhFeeErrorRatioLong_gDPB[uGdpb * fuNrOfFeePerGdpb + uFee]->Reset();
    }  // for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb; ++ uFee)

  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fhHitsPerMsFirstChan_gDPB[uGdpb]->Reset();
    fvhChannelRatePerMs_gDPB[uGdpb]->Reset();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  ///* STAR event building/cutting *///
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fvhTokenMsgType[uGdpb]->Reset();
    fvhTriggerRate[uGdpb]->Reset();
    fvhCmdDaqVsTrig[uGdpb]->Reset();
    fvhStarTokenEvo[uGdpb]->Reset();
    fvhStarTrigGdpbTsEvo[uGdpb]->Reset();
    fvhStarTrigStarTsEvo[uGdpb]->Reset();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  ///* Pulser monitoring *///
  if (kTRUE == fbPulserModeEnable) {
    for (UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeA++)
      for (UInt_t uFeeB = 0; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++)
        if (NULL != fvhTimeDiffPulser[uFeeA][uFeeB])
          fvhTimeDiffPulser[uFeeA][uFeeB]->Reset();

    fhTimeMeanPulser->Reset();
    fhTimeRmsPulser->Reset();
    fhTimeRmsZoomFitPuls->Reset();
    fhTimeResFitPuls->Reset();

    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
      for (UInt_t uGbtx = 0; uGbtx < kuNbGbtxPerGdpb - 1; ++uGbtx)
        fvhPulserTimeDiffEvoGbtxGbtx[uGdpb * (kuNbGbtxPerGdpb - 1) + uGbtx]
          ->Reset();

      for (UInt_t uGdpbB = uGdpb + 1; uGdpbB < fuNrOfGdpbs; ++uGdpbB)
        fvvhPulserTimeDiffEvoGdpbGdpb[uGdpb][uGdpbB]->Reset();
    }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  }    // if( kTRUE == fbPulserModeEnable )

  ///* Coincidence maps *///
  if (kTRUE == fbCoincMapsEnable) {
    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
      fvhCoincMapAllChanGdpb[uGdpb]->Reset();
      fvhCoincMeanAllChanGdpb[uGdpb]->Reset();
    }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  }    // if( kTRUE == fbCoincMapsEnable )

  for (UInt_t uLinks = 0; uLinks < fvhMsSzPerLink.size(); uLinks++) {
    if (NULL == fvhMsSzPerLink[uLinks]) continue;

    fvhMsSzPerLink[uLinks]->Reset();
    fvhMsSzTimePerLink[uLinks]->Reset();
  }  // for( UInt_t uLinks = 0; uLinks < fvhMsSzPerLink.size(); uLinks++ )

  fdStartTime     = -1;
  fdStartTimeLong = -1;
  fdStartTimeMsSz = -1;
}
void CbmMcbm2018MonitorTof::ResetEvolutionHistograms() {
  for (UInt_t uGdpbLoop = 0; uGdpbLoop < fuNrOfGdpbs; ++uGdpbLoop) {
    fvhChannelRate_gDPB[uGdpbLoop]->Reset();
    fvhRemapChRate_gDPB[uGdpbLoop]->Reset();
    for (UInt_t uFeeLoop = 0; uFeeLoop < fuNrOfFeePerGdpb; ++uFeeLoop) {
      fvhFeeRate_gDPB[(uGdpbLoop * fuNrOfFeePerGdpb) + uFeeLoop]->Reset();
      fvhFeeErrorRate_gDPB[(uGdpbLoop * fuNrOfFeePerGdpb) + uFeeLoop]->Reset();
      fvhFeeErrorRatio_gDPB[(uGdpbLoop * fuNrOfFeePerGdpb) + uFeeLoop]->Reset();
    }  // for( UInt_t uFeeLoop = 0; uFeeLoop < fuNrOfFeePerGdpb; ++uFeeLoop )
    fvhTriggerRate[uGdpbLoop]->Reset();
    fvhStarTokenEvo[uGdpbLoop]->Reset();
    fvhStarTrigGdpbTsEvo[uGdpbLoop]->Reset();
    fvhStarTrigStarTsEvo[uGdpbLoop]->Reset();
  }  // for( UInt_t uGdpbLoop = 0; uGdpbLoop < fuNrOfGdpbs; ++uGdpbLoop )
     /*
   for( UInt_t uMod = 0; uMod < fuNrOfModules; uMod ++ )
   {
      fvhModRate[ uMod ]->Reset();
      fvhModErrorRate[ uMod ]->Reset();
      fvhModErrorRatio[ uMod ]->Reset();
   } // for( UInt_t uMod = 0; uMod < fuNrOfModules; uMod ++ )
*/
  fdStartTime = -1;
}
void CbmMcbm2018MonitorTof::ResetLongEvolutionHistograms() {
  for (UInt_t uGdpbLoop = 0; uGdpbLoop < fuNrOfGdpbs; uGdpbLoop++) {
    for (UInt_t uFeeLoop = 0; uFeeLoop < fuNrOfFeePerGdpb; uFeeLoop++) {
      fvhFeeRateLong_gDPB[(uGdpbLoop * fuNrOfFeePerGdpb) + uFeeLoop]->Reset();
      fvhFeeErrorRateLong_gDPB[(uGdpbLoop * fuNrOfFeePerGdpb) + uFeeLoop]
        ->Reset();
      fvhFeeErrorRatioLong_gDPB[(uGdpbLoop * fuNrOfFeePerGdpb) + uFeeLoop]
        ->Reset();
    }  // for (UInt_t uFeeLoop = 0; uFeeLoop < fuNrOfFeePerGdpb; uFeeLoop++)
  }    // for (UInt_t uFeeLoop = 0; uFeeLoop < fuNrOfFeePerGdpb; uFeeLoop++)

  fdStartTimeLong = -1;
}

void CbmMcbm2018MonitorTof::UpdateZoomedFit() {
  if (kFALSE == fbPulserModeEnable) {
    LOG(error) << "CbmMcbm2018MonitorTof::UpdateZoomedFit => "
               << "Pulser mode not enabled in root macro, doinb nothing !!! ";
    return;
  }  // if( kFALSE == fbPulserModeEnable )

  // Only do something is the user defined the width he want for the zoom
  if (0.0 < fdFitZoomWidthPs) {
    // Reset summary histograms for safety
    fhTimeRmsZoomFitPuls->Reset();
    fhTimeResFitPuls->Reset();

    Double_t dRes = 0;
    TF1* fitFuncPairs[fuNrOfFeePerGdpb * fuNrOfGdpbs]
                     [fuNrOfFeePerGdpb * fuNrOfGdpbs];

    for (UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeA++)
      for (UInt_t uFeeB = 0; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++)
        if (NULL != fvhTimeDiffPulser[uFeeA][uFeeB]) {
          // Check that we have at least 1 entry
          if (0 == fvhTimeDiffPulser[uFeeA][uFeeB]->GetEntries()) {
            fhTimeRmsZoomFitPuls->Fill(uFeeA, uFeeB, 0.0);
            fhTimeResFitPuls->Fill(uFeeA, uFeeB, 0.0);
            LOG(info)
              << "CbmMcbm2018MonitorTof::UpdateZoomedFit => Empty input "
              << "for FEE pair " << uFeeA << " and " << uFeeB << " !!! ";
            continue;
          }  // if( 0 == fvhTimeDiffPulser[uFeeA][uFeeB]->GetEntries() )

          // Read the peak position (bin with max counts) + total nb of entries
          Int_t iBinWithMax  = fvhTimeDiffPulser[uFeeA][uFeeB]->GetMaximumBin();
          Double_t dNbCounts = fvhTimeDiffPulser[uFeeA][uFeeB]->Integral();

          // Zoom the X axis to +/- ZoomWidth around the peak position
          Double_t dPeakPos =
            fvhTimeDiffPulser[uFeeA][uFeeB]->GetXaxis()->GetBinCenter(
              iBinWithMax);
          fvhTimeDiffPulser[uFeeA][uFeeB]->GetXaxis()->SetRangeUser(
            dPeakPos - fdFitZoomWidthPs, dPeakPos + fdFitZoomWidthPs);

          // Read integral and check how much we lost due to the zoom (% loss allowed)
          Double_t dZoomCounts = fvhTimeDiffPulser[uFeeA][uFeeB]->Integral();
          if ((dZoomCounts / dNbCounts) < 0.99) {
            fhTimeRmsZoomFitPuls->Fill(uFeeA, uFeeB, 0.0);
            fhTimeResFitPuls->Fill(uFeeA, uFeeB, 0.0);
            LOG(warning)
              << "CbmMcbm2018MonitorTof::UpdateZoomedFit => Zoom too strong, "
              << "more than 1% loss for FEE pair " << uFeeA << " and " << uFeeB
              << " !!! ";
            continue;
          }  // if( ( dZoomCounts / dNbCounts ) < 0.99 )

          // Fill new RMS after zoom into summary histo
          fhTimeRmsZoomFitPuls->Fill(
            uFeeA, uFeeB, fvhTimeDiffPulser[uFeeA][uFeeB]->GetRMS());


          // Fit using zoomed boundaries + starting gaussian width, store into summary histo
          dRes = 0;
          fitFuncPairs[uFeeA][uFeeB] =
            new TF1(Form("fPair_%02d_%02d", uFeeA, uFeeB),
                    "gaus",
                    dPeakPos - fdFitZoomWidthPs,
                    dPeakPos + fdFitZoomWidthPs);
          // Fix the Mean fit value around the Histogram Mean
          fitFuncPairs[uFeeA][uFeeB]->SetParameter(0, dZoomCounts);
          fitFuncPairs[uFeeA][uFeeB]->SetParameter(1, dPeakPos);
          fitFuncPairs[uFeeA][uFeeB]->SetParameter(
            2, 200.0);  // Hardcode start with ~4*BinWidth, do better later
          // Using integral instead of bin center seems to lead to unrealistic values => no "I"
          fvhTimeDiffPulser[uFeeA][uFeeB]->Fit(
            Form("fPair_%02d_%02d", uFeeA, uFeeB), "QRM0");
          // Get Sigma
          dRes = fitFuncPairs[uFeeA][uFeeB]->GetParameter(2);
          // Cleanup memory
          delete fitFuncPairs[uFeeA][uFeeB];
          // Fill summary
          fhTimeResFitPuls->Fill(uFeeA, uFeeB, dRes / TMath::Sqrt2());


          LOG(info) << "CbmMcbm2018MonitorTof::UpdateZoomedFit => "
                    << "For FEE pair " << uFeeA << " and " << uFeeB
                    << " we have zoomed RMS = "
                    << fvhTimeDiffPulser[uFeeA][uFeeB]->GetRMS()
                    << " and a resolution of " << dRes / TMath::Sqrt2();

          // Restore original axis state?
          fvhTimeDiffPulser[uFeeA][uFeeB]->GetXaxis()->UnZoom();
        }  // loop on uFeeA and uFeeB + check if corresponding fvhTimeDiffPulser exists
  }  // if( 0.0 < fdFitZoomWidthPs )
  else {
    LOG(error)
      << "CbmMcbm2018MonitorTof::UpdateZoomedFit => Zoom width not defined, "
      << "please use SetFitZoomWidthPs, e.g. in macro, before trying this "
         "update !!!";
  }  // else of if( 0.0 < fdFitZoomWidthPs )
}

ClassImp(CbmMcbm2018MonitorTof)
