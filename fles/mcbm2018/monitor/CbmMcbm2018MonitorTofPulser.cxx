// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmMcbm2018MonitorTofPulser                          -----
// -----               Created 10.07.2018 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018MonitorTofPulser.h"
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

Bool_t bMcbmMoniTofPulserResetHistos       = kFALSE;
Bool_t bMcbmMoniTofPulserSaveHistos        = kFALSE;
Bool_t bMcbmMoniTofPulserUpdateZoomedFit   = kFALSE;
Bool_t bMcbmMoniTofPulserUpdateDnlInl      = kFALSE;
Bool_t bMcbmMoniTofPulserRawDataPrint      = kFALSE;
Bool_t bMcbmMoniTofPulserPrintAllHitsEna   = kFALSE;
Bool_t bMcbmMoniTofPulserPrintAllEpochsEna = kFALSE;

CbmMcbm2018MonitorTofPulser::CbmMcbm2018MonitorTofPulser()
  : CbmMcbmUnpack()
  , fvMsComponentsList()
  , fuNbCoreMsPerTs(0)
  , fuNbOverMsPerTs(0)
  , fbIgnoreOverlapMs(kFALSE)
  , fsHistoFileFullname("data/TofPulserHistos.root")
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
  , fbOldFwData(kFALSE)
  , fuDiamondDpbIdx(10000)
  ,  // Crazy default value => should never make troubles given the price
  fulCurrentTsIndex(0)
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
  , fvhTimeDiffPulser()
  , fhTimeMeanPulser(NULL)
  , fhTimeRmsPulser(NULL)
  , fhTimeRmsZoomFitPuls(NULL)
  , fhTimeResFitPuls(NULL)
  , fvhPulserCountEvoPerFeeGdpb()
  , fvhPulserTimeDiffEvoGbtxGbtx()
  , fvvhPulserTimeDiffEvoGdpbGdpb()
  , fvvhPulserTimeDiffEvoFeeFee()
  , fvuPadiToGet4()
  , fvuGet4ToPadi()
  , fvuElinkToGet4()
  , fvuGet4ToElink()
  , fTimeLastHistoSaving() {}

CbmMcbm2018MonitorTofPulser::~CbmMcbm2018MonitorTofPulser() {}

Bool_t CbmMcbm2018MonitorTofPulser::Init() {
  LOG(info) << "Initializing Get4 monitor";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }  // if( ioman == NULL )

  return kTRUE;
}

void CbmMcbm2018MonitorTofPulser::SetParContainers() {
  LOG(info) << "Setting parameter containers for " << GetName();
  fUnpackPar =
    (CbmMcbm2018TofPar*) (FairRun::Instance()->GetRuntimeDb()->getContainer(
      "CbmMcbm2018TofPar"));
}

Bool_t CbmMcbm2018MonitorTofPulser::InitContainers() {
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

Bool_t CbmMcbm2018MonitorTofPulser::ReInitContainers() {
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

  LOG(info) << "Nr. of RPCs per GBTx: ";
  std::stringstream ss;
  for (UInt_t uGbtx = 0; uGbtx < fuNrOfGbtx; ++uGbtx)
    ss << Form(" %2d", fviNrOfRpc[uGbtx]);
  LOG(info) << ss.str();

  LOG(info) << "RPC type per GBTx:    ";
  ss.clear();
  for (UInt_t uGbtx = 0; uGbtx < fuNrOfGbtx; ++uGbtx)
    ss << Form(" %2d", fviRpcType[uGbtx]);
  LOG(info) << ss.str();

  LOG(info) << "RPC side per GBTx:    ";
  ss.clear();
  for (UInt_t uGbtx = 0; uGbtx < fuNrOfGbtx; ++uGbtx)
    ss << Form(" %2d", fviRpcSide[uGbtx]);
  LOG(info) << ss.str();

  LOG(info) << "Module ID per GBTx:   ";
  ss.clear();
  for (UInt_t uGbtx = 0; uGbtx < fuNrOfGbtx; ++uGbtx)
    ss << Form(" %2d", fviModuleId[uGbtx]);
  LOG(info) << ss.str();

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
  fvuFeeNbHitsLastMs.resize(fuNrOfFeePerGdpb * fuNrOfGdpbs, 0);
  fdTsLastPulserHit.resize(fuNrOfFeePerGdpb * fuNrOfGdpbs, 0.0);

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


void CbmMcbm2018MonitorTofPulser::AddMsComponentToList(
  size_t component,
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
void CbmMcbm2018MonitorTofPulser::SetNbMsInTs(size_t uCoreMsNb,
                                              size_t uOverlapMsNb) {
  fuNbCoreMsPerTs = uCoreMsNb;
  fuNbOverMsPerTs = uOverlapMsNb;

  //   UInt_t uNbMsTotal = fuNbCoreMsPerTs + fuNbOverMsPerTs;
}

void CbmMcbm2018MonitorTofPulser::CreateHistograms() {
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
  /// FIXME: Disable clang formatting to keep readable histo creation
  /* clang-format off */

  /// FEE pulser test channels
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
                        uGdpbA, uFeeIdA, uGdpbB, uFeeIdB),
                   Form("Time difference for pulser on gDPB %02u FEE %1u and "
                        "gDPB %02u FEE %1u; DeltaT [ps]; Counts",
                        uGdpbA, uFeeIdA, uGdpbB, uFeeIdB),
                   uNbBinsDt, dMinDt, dMaxDt);
      }  // if( uFeeA < uFeeB )
      else
        fvhTimeDiffPulser[uFeeA][uFeeB] = NULL;
    }  // for( UInt_t uFeeB = uFeeA; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs - 1; uFeeB++)
  }    // for( UInt_t uFeeA = 0; uFeeA < kuNbChanTest - 1; uFeeA++)

  name             = "hTimeMeanPulser";
  fhTimeMeanPulser = new TH2D(
    name.Data(),
    "Time difference Mean for each FEE pairs; FEE A; FEE B ; Mean [ps]",
    fuNrOfFeePerGdpb * fuNrOfGdpbs - 1, -0.5, fuNrOfFeePerGdpb * fuNrOfGdpbs - 1.5,
    fuNrOfFeePerGdpb * fuNrOfGdpbs - 1,  0.5, fuNrOfFeePerGdpb * fuNrOfGdpbs - 0.5);

  name = "hTimeRmsPulser";
  fhTimeRmsPulser = new TH2D(
    name.Data(),
    "Time difference RMS for each FEE pairs; FEE A; FEE B ; RMS [ps]",
    fuNrOfFeePerGdpb * fuNrOfGdpbs - 1, -0.5, fuNrOfFeePerGdpb * fuNrOfGdpbs - 1.5,
    fuNrOfFeePerGdpb * fuNrOfGdpbs - 1,  0.5, fuNrOfFeePerGdpb * fuNrOfGdpbs - 0.5);

  name                 = "hTimeRmsZoomFitPuls";
  fhTimeRmsZoomFitPuls = new TH2D(
    name.Data(),
    "Time difference RMS after zoom for each FEE "
    "pairs; FEE A; FEE B ; RMS [ps]",
    fuNrOfFeePerGdpb * fuNrOfGdpbs - 1, -0.5, fuNrOfFeePerGdpb * fuNrOfGdpbs - 1.5,
    fuNrOfFeePerGdpb * fuNrOfGdpbs - 1, 0.5, fuNrOfFeePerGdpb * fuNrOfGdpbs - 0.5);

  name             = "hTimeResFitPuls";
  fhTimeResFitPuls = new TH2D(
    name.Data(),
    "Time difference Res from fit for each FEE pairs; FEE A; FEE B ; Sigma [ps]",
    fuNrOfFeePerGdpb * fuNrOfGdpbs - 1, -0.5, fuNrOfFeePerGdpb * fuNrOfGdpbs - 1.5,
    fuNrOfFeePerGdpb * fuNrOfGdpbs - 1,  0.5, fuNrOfFeePerGdpb * fuNrOfGdpbs - 0.5);

  fvhPulserTimeDiffEvoGbtxGbtx.resize(fuNrOfGdpbs * (kuNbGbtxPerGdpb - 1));
  fvvhPulserTimeDiffEvoGdpbGdpb.resize(fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {

    name = Form("hPulserCountEvoPerFeeGdpb%02u", uGdpb);
    fvhPulserCountEvoPerFeeGdpb.push_back(new TH2D(
      name.Data(),
      Form("Pulser count per FEE in gDPB %02u; time in run [s]; dt [ps]",
           uGdpb),
      fuHistoryHistoSize, 0, fuHistoryHistoSize,
      fuNrOfFeePerGdpb, -0.5, fuNrOfFeePerGdpb));

    for (UInt_t uGbtx = 0; uGbtx < kuNbGbtxPerGdpb - 1; ++uGbtx) {
      name = Form("hPulserTimeDiffEvoGdpb%02uGbtx00Gbtx%02u", uGdpb, uGbtx + 1);
      fvhPulserTimeDiffEvoGbtxGbtx[uGdpb * (kuNbGbtxPerGdpb - 1) + uGbtx] =
        new TProfile(name.Data(),
                     Form("Time difference of the 1st FEE in the 1st GBTx of "
                          "gDPB %02u vs GBTx %02u; time in run [s]; dt [ps]",
                          uGdpb,
                          uGbtx + 1),
                     fuHistoryHistoSize, 0, fuHistoryHistoSize);
    }  // for( UInt_t uGbtx = 0; uGbtx < kuNbGbtxPerGdpb; ++uGbtx )

    fvvhPulserTimeDiffEvoGdpbGdpb[uGdpb].resize(fuNrOfGdpbs, NULL);
    for (UInt_t uGdpbB = uGdpb + 1; uGdpbB < fuNrOfGdpbs; ++uGdpbB) {
      name = Form("hPulserTimeDiffEvoGdpb%02uGdpb%02u", uGdpb, uGdpbB);
      fvvhPulserTimeDiffEvoGdpbGdpb[uGdpb][uGdpbB] =
        new TProfile(name.Data(),
                     Form("Time difference of the 1st FEE in the 1st GBTx of "
                          "gDPB %02u vs %02u; time in run [s]; dt [ps]",
                          uGdpb,
                          uGdpbB),
                     fuHistoryHistoSize,  0, fuHistoryHistoSize);
    }  // for( UInt_t uGdpbB = uGdpb + 1; uGdpbB < fuNrOfGdpbs; ++uGdpbB )
  }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  fvvhPulserTimeDiffEvoFeeFee.resize(kuNbRefFeeEvo);
  for (UInt_t uFeeRef = 0; uFeeRef < kuNbRefFeeEvo; ++uFeeRef) {
    UInt_t uGdpbRef  = kuRefFeeEvoIdx[uFeeRef] / (fuNrOfFeePerGdpb);
    UInt_t uFeeIdRef = kuRefFeeEvoIdx[uFeeRef] - (fuNrOfFeePerGdpb * uGdpbRef);

    fvvhPulserTimeDiffEvoFeeFee[uFeeRef].resize(fuNrOfFeePerGdpb * fuNrOfGdpbs);
    for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFee++) {
      UInt_t uGdpb  = uFee / (fuNrOfFeePerGdpb);
      UInt_t uFeeId = uFee - (fuNrOfFeePerGdpb * uGdpb);

      fvvhPulserTimeDiffEvoFeeFee[uFeeRef][uFee] =
        new TProfile(Form("hTimeDiffEvoFeeFee_g%02u_f%02u_g%02u_f%02u",
                          uGdpbRef, uFeeIdRef, uGdpb, uFeeId),
                     Form("Time difference for pulser on gDPB %02u FEE %1u and "
                          "gDPB %02u FEE %02u; time in run [s]; DeltaT [ps]",
                          uGdpbRef, uFeeIdRef, uGdpb, uFeeId),
                     fuHistoryHistoSize / 2, 0, fuHistoryHistoSize);
    }  // for( UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFee++)
  }    // for( UInt_t uFeeRef = 0; uFeeRef < kuNbRefFeeEvo; ++uFeeRef )

   fhFeeFtDistribPerCh.resize( fuNrOfFeePerGdpb * fuNrOfGdpbs );
   fhFeeFtNormDnl.resize( fuNrOfFeePerGdpb * fuNrOfGdpbs );
   fhFeeFtNormInl.resize( fuNrOfFeePerGdpb * fuNrOfGdpbs );
   for( UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFee++)
   {
      UInt_t uGdpb  = uFee / ( fuNrOfFeePerGdpb );
      UInt_t uFeeId = uFee - ( fuNrOfFeePerGdpb * uGdpb );
      fhFeeFtDistribPerCh[ uFee ] = new TH2D(
            Form( "hFeeFtDistribPerCh_g%02u_f%02u", uGdpb, uFeeId ),
            Form( "FT distribution per channel for gDPB %02u FEE %02u", uGdpb, uFeeId ),
            gdpbv100::kdFtBinsNb, 0, gdpbv100::kdFtBinsNb,
            fuNrOfChannelsPerFee, -0.5, fuNrOfChannelsPerFee - 0.5 );
      fhFeeFtNormDnl[ uFee ] = new TH2D(
            Form( "hFeeFtNormDnl_g%02u_f%02u", uGdpb, uFeeId ),
            Form( "Normalized DNL per channel for gDPB %02u FEE %02u", uGdpb, uFeeId ),
            gdpbv100::kdFtBinsNb, 0, gdpbv100::kdFtBinsNb,
            fuNrOfChannelsPerFee, -0.5, fuNrOfChannelsPerFee - 0.5 );
      fhFeeFtNormInl[ uFee ] = new TH2D(
            Form( "hFeeFtNormInl_g%02u_f%02u", uGdpb, uFeeId ),
            Form( "Normalized INL per channel for gDPB %02u FEE %02u", uGdpb, uFeeId ),
            gdpbv100::kdFtBinsNb, 0, gdpbv100::kdFtBinsNb,
            fuNrOfChannelsPerFee, -0.5, fuNrOfChannelsPerFee - 0.5 );
   } // for( UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFee++)

  /// FIXME: Re-enable clang formatting after histo creation
  /* clang-format on */

  if (server) {
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

    for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFee++) {
      server->Register("/DnlInl", fhFeeFtDistribPerCh[uFee]);
      server->Register("/DnlInl", fhFeeFtNormDnl[uFee]);
      server->Register("/DnlInl", fhFeeFtNormInl[uFee]);
    }  // for( UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFee++)

    server->RegisterCommand("/Reset_All_eTOF",
                            "bMcbmMoniTofPulserResetHistos=kTRUE");
    server->RegisterCommand("/Save_All_eTof",
                            "bMcbmMoniTofPulserSaveHistos=kTRUE");
    server->RegisterCommand("/Update_PulsFit",
                            "bMcbmMoniTofPulserUpdateZoomedFit=kTRUE");
    server->RegisterCommand("/Print_Raw_Data",
                            "bMcbmMoniTofPulserRawDataPrint=kTRUE");
    server->RegisterCommand("/Print_AllHits",
                            "bMcbmMoniTofPulserPrintAllHitsEna=kTRUE");
    server->RegisterCommand("/Print_AllEps",
                            "bMcbmMoniTofPulserPrintAllEpochsEna=kTRUE");

    server->Restrict("/Reset_All_eTof", "allow=admin");
    server->Restrict("/Save_All_eTof", "allow=admin");
    server->Restrict("/Update_PulsFit", "allow=admin");
    server->Restrict("/Update_DnlInl", "allow=admin");
    server->Restrict("/Print_Raw_Data", "allow=admin");
    server->Restrict("/Print_AllHits", "allow=admin");
    server->Restrict("/Print_AllEps", "allow=admin");
  }  // if( server )

  /*****************************/
  Double_t w = 10;
  Double_t h = 10;

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
  /*****************************/
  /** Create Pulser evo Canvas within gDPB **/
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb++) {
    TCanvas* cPulserEvoGbtx = new TCanvas(
      Form("cPulserEvoGbtx%02u", uGdpb),
      Form(
        "Time difference evolution between 1st FEE of GBTx pairs in gDPB %02u",
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
  }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
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

  LOG(info) << "Leaving CreateHistograms";
}

Bool_t CbmMcbm2018MonitorTofPulser::DoUnpack(const fles::Timeslice& ts,
                                             size_t component) {
  if (bMcbmMoniTofPulserResetHistos) {
    LOG(info) << "Reset eTOF STAR histos ";
    ResetAllHistos();
    bMcbmMoniTofPulserResetHistos = kFALSE;
  }  // if( bMcbmMoniTofPulserResetHistos )
  if (bMcbmMoniTofPulserSaveHistos) {
    LOG(info) << "Start saving eTOF STAR histos ";
    SaveAllHistos("data/histos_Shift_StarTof.root");
    bMcbmMoniTofPulserSaveHistos = kFALSE;
  }  // if( bSaveStsHistos )
  if (bMcbmMoniTofPulserUpdateZoomedFit) {
    UpdateZoomedFit();
    bMcbmMoniTofPulserUpdateZoomedFit = kFALSE;
  }  // if (bMcbmMoniTofPulserUpdateZoomedFit)
  if (bMcbmMoniTofPulserUpdateDnlInl) {
    UpdateNormedDnlInl();
    bMcbmMoniTofPulserUpdateDnlInl = kFALSE;
  }  // if (bMcbmMoniTofPulserUpdateZoomedFit)
  if (bMcbmMoniTofPulserRawDataPrint) {
    fuRawDataPrintMsgIdx           = 0;
    bMcbmMoniTofPulserRawDataPrint = kFALSE;
  }  // if( bMcbmMoniTofPulserRawDataPrint )
  if (bMcbmMoniTofPulserPrintAllHitsEna) {
    fbPrintAllHitsEnable              = !fbPrintAllHitsEnable;
    bMcbmMoniTofPulserPrintAllHitsEna = kFALSE;
  }  // if( bMcbmMoniTofPulserPrintAllHitsEna )
  if (bMcbmMoniTofPulserPrintAllEpochsEna) {
    fbPrintAllEpochsEnable            = !fbPrintAllEpochsEnable;
    bMcbmMoniTofPulserPrintAllHitsEna = kFALSE;
  }  // if( bMcbmMoniTofPulserPrintAllEpochsEna )

  /// Periodically save the histograms
  std::chrono::time_point<std::chrono::system_clock> timeCurrent =
    std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds =
    timeCurrent - fTimeLastHistoSaving;
  if (0 == fTimeLastHistoSaving.time_since_epoch().count()) {
    fTimeLastHistoSaving = timeCurrent;
    //      fulNbBuiltSubEventLastPrintout = fulNbBuiltSubEvent;
    //      fulNbStarSubEventLastPrintout  = fulNbStarSubEvent;
  }  // if( 0 == fTimeLastHistoSaving.time_since_epoch().count() )
  else if (300 < elapsed_seconds.count()) {
    std::time_t cTimeCurrent =
      std::chrono::system_clock::to_time_t(timeCurrent);
    char tempBuff[80];
    std::strftime(tempBuff, 80, "%F %T", localtime(&cTimeCurrent));
    /*
      LOG(info) << "CbmTofStarEventBuilder2018::DoUnpack => " << tempBuff
               << " Total number of Built events: " << std::setw(9) << fulNbBuiltSubEvent
               << ", " << std::setw(9) << (fulNbBuiltSubEvent - fulNbBuiltSubEventLastPrintout)
               << " events in last " << std::setw(4) << elapsed_seconds.count() << " s";
      fTimeLastPrintoutNbStarEvent = timeCurrent;
      fulNbBuiltSubEventLastPrintout   = fulNbBuiltSubEvent;
*/
    fTimeLastHistoSaving = timeCurrent;
    SaveAllHistos("data/histos_tof_pulser.root");
  }  // else if( 300 < elapsed_seconds.count() )

  LOG(debug1) << "Timeslice contains " << ts.num_microslices(component)
              << "microslices.";

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
        LOG(warning) << "Could not find the gDPB index for AFCK id 0x"
                     << std::hex << fuGdpbId << std::dec << " in timeslice "
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
              PrintGenInfo(mess);
            }  // if( getGdpbHitIs24b() )
            else {
              fvmEpSupprBuffer[fuGet4Nr].push_back(mess);
            }  // else of if( getGdpbHitIs24b() )
            break;
          }  // case gdpbv100::MSG_HIT:
          case gdpbv100::MSG_EPOCH: {
            if (gdpbv100::kuChipIdMergedEpoch == fuGet4Id) {
              for (uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb;
                   uGet4Index++) {
                fuGet4Id = uGet4Index;
                fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;
                gdpbv100::Message tmpMess(mess);
                tmpMess.setGdpbGenChipId(uGet4Index);

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
            PrintSlcInfo(mess);
            break;
          }  // case gdpbv100::MSG_SLOWC:
          case gdpbv100::MSG_SYST: {
            if (gdpbv100::SYS_GET4_ERROR == mess.getGdpbSysSubType()) {

              //                     UInt_t uFeeNr   = (fuGet4Id / fuNrOfGet4PerFee);
              /*
                     if (0 <= fdStartTime)
                     {
                        fvhFeeErrorRate_gDPB[(fuGdpbNr * fuNrOfFeePerGdpb) + uFeeNr]->Fill(
                           1e-9 * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr]) - fdStartTime));
                        fvhFeeErrorRatio_gDPB[(fuGdpbNr * fuNrOfFeePerGdpb) + uFeeNr]->Fill(
                           1e-9 * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr]) - fdStartTime), 1, 1 );

                        UInt_t uGbtxNr      = (uFeeNr / kuNbFeePerGbtx);
                        UInt_t uGbtxNrInSys = fuGdpbNr * kuNbGbtxPerGdpb + uGbtxNr;
                        fvhModErrorRate[ fviModuleId[ uGbtxNrInSys ] ]->Fill(
                           1e-9 * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr]) - fdStartTime));
                        fvhModErrorRatio[ fviModuleId[ uGbtxNrInSys ] ]->Fill(
                           1e-9 * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr]) - fdStartTime), 1, 1);
                     } // if (0 <= fdStartTime)
                     if (0 <= fdStartTimeLong)
                     {
                        fvhFeeErrorRateLong_gDPB[(fuGdpbNr * fuNrOfFeePerGdpb) + uFeeNr]->Fill(
                           1e-9 / 60.0 * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr]) - fdStartTimeLong), 1 / 60.0);
                        fvhFeeErrorRatioLong_gDPB[(fuGdpbNr * fuNrOfFeePerGdpb) + uFeeNr]->Fill(
                           1e-9 / 60.0 * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr]) - fdStartTimeLong), 1, 1 / 60.0);
                     } // if (0 <= fdStartTime)
*/
              //                     Int_t dGdpbChId =  fuGet4Id * fuNrOfChannelsPerGet4 + mess.getGdpbSysErrChanId();
              //                     Int_t dFullChId =  fuGet4Nr * fuNrOfChannelsPerGet4 + mess.getGdpbSysErrChanId();
            }  // if( gdpbv100::SYSMSG_GET4_EVENT == mess.getGdpbSysSubType() )
            if (gdpbv100::SYS_PATTERN == mess.getGdpbSysSubType()) {
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
    /// Fill the corresponding histos if the time difference is reasonnable
    for (UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; ++uFeeA) {
      if (1 != fvuFeeNbHitsLastMs[uFeeA]) {
        /// Pulser should lead to single hit in a MS, so ignore this time
        fvuFeeNbHitsLastMs[uFeeA] = 0;
        continue;
      }  // if( 1 != fvuFeeNbHitsLastMs[ uFeeA ] )

      UInt_t uGdpbNr = uFeeA / fuNrOfFeePerGdpb;
      UInt_t uGbtxNr = (uFeeA % fuNrOfFeePerGdpb) / kuNbFeePerGbtx;

      UInt_t uFeeRefIndexA = kuNbRefFeeEvo;
      for (UInt_t uFeeRef = 0; uFeeRef < kuNbRefFeeEvo; ++uFeeRef)
        if (kuRefFeeEvoIdx[uFeeRef] == uFeeA) uFeeRefIndexA = uFeeRef;

      /// Update the difference to all other FEE with higher indices
      for (UInt_t uFeeB = uFeeA + 1; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs;
           ++uFeeB) {
        if (1 != fvuFeeNbHitsLastMs[uFeeB]) continue;

        if (NULL != fvhTimeDiffPulser[uFeeA][uFeeB]) {
          UInt_t uFeeRefIndexB = kuNbRefFeeEvo;
          for (UInt_t uFeeRef = 0; uFeeRef < kuNbRefFeeEvo; ++uFeeRef)
            if (kuRefFeeEvoIdx[uFeeRef] == uFeeB) uFeeRefIndexB = uFeeRef;

          Double_t dTimeDiff =
            1e3 * (fdTsLastPulserHit[uFeeB] - fdTsLastPulserHit[uFeeA]);
          if (TMath::Abs(dTimeDiff) < kdMaxDtPulserPs) {
            fvhTimeDiffPulser[uFeeA][uFeeB]->Fill(dTimeDiff);

            if (uFeeRefIndexA < kuNbRefFeeEvo)
              fvvhPulserTimeDiffEvoFeeFee[uFeeRefIndexA][uFeeB]->Fill(
                1e-9 * (fdTsLastPulserHit[uFeeA] - fdStartTime), dTimeDiff);

            if (uFeeRefIndexB < kuNbRefFeeEvo)
              fvvhPulserTimeDiffEvoFeeFee[uFeeRefIndexB][uFeeA]->Fill(
                1e-9 * (fdTsLastPulserHit[uFeeB] - fdStartTime),
                -1.0 * dTimeDiff);

            /// Dt Evo plots only for first FEE in each GBTx
            if (0 == uFeeA % kuNbFeePerGbtx && 0 == uFeeB % kuNbFeePerGbtx) {
              UInt_t uGdpbNrB = uFeeB / fuNrOfFeePerGdpb;
              UInt_t uGbtxNrB = (uFeeB % fuNrOfFeePerGdpb) / kuNbFeePerGbtx;

              /// Evo of GBTx inside same DPB
              if (uGdpbNr == uGdpbNrB) {
                if (0 == uGbtxNr) {
                  UInt_t uPlotIdx =
                    uGdpbNr * (kuNbGbtxPerGdpb - 1) + uGbtxNrB - 1;
                  fvhPulserTimeDiffEvoGbtxGbtx[uPlotIdx]->Fill(
                    1e-9 * (fdTsLastPulserHit[uFeeA] - fdStartTime), dTimeDiff);
                }   // if( 0 == uGbtxNr  )
              }     // if( uGdpbNr == uFeeB / fuNrOfFeePerGdpb )
              else  // if( NULL != fvvhPulserTimeDiffEvoGdpbGdpb[ uFeeB / fuNrOfFeePerGdpb ][ uGdpbNr ] )
              {
                /// Evo of DPBs if both first FEE in First GBTx
                if (0 == uGbtxNr && 0 == uGbtxNrB)
                  fvvhPulserTimeDiffEvoGdpbGdpb[uGdpbNr][uFeeB
                                                         / fuNrOfFeePerGdpb]
                    ->Fill(1e-9 * (fdTsLastPulserHit[uFeeA] - fdStartTime),
                           dTimeDiff);
              }  // else of if( uGdpbNr == uFeeB / fuNrOfFeePerGdpb )
            }  // if( 0 == uFeeA % kuNbFeePerGbtx && 0 == uFeeB % kuNbFeePerGbtx )
          }    // if( TMath::Abs( dTimeDiff ) < kdMaxDtPulserPs )
               //               else if( 10 == uFeeA && 20 == uFeeB )
               //                  LOG(info) << "new in 10 " << dTimeDiff;
        }      // if( NULL != fvhTimeDiffPulser[uFeeA][uFeeB] )
      }  // for( UInt_t uFeeB = uFeeA + 1; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; ++uFeeB)

      /// Done with this FEE, we can reset the hit counter
      fvuFeeNbHitsLastMs[uFeeA] = 0;
    }  // for( UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; ++uFeeA)
  }    // for( UInt_t uMsIdx = 0; uMsIdx < uNbMsLoop; uMsIdx ++ )

  // Update RMS plots only every 10s in data
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

  fulCurrentTsIndex++;

  return kTRUE;
}

void CbmMcbm2018MonitorTofPulser::ProcessEpochCycle(uint64_t ulCycleData) {
  uint64_t ulEpochCycleVal = ulCycleData & gdpbv100::kulEpochCycleFieldSz;

  if (fuRawDataPrintMsgIdx < fuRawDataPrintMsgNb
      || gLogger->IsLogNeeded(fair::Severity::debug2)) {
    LOG(info)
      << "CbmMcbm2018MonitorTofPulser::ProcessEpochCyle => "
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
         LOG(error) << "CbmMcbm2018MonitorTofPulser::ProcessEpochCyle => "
                    << " Missmatch in epoch cycles detected, probably fake cycles due to epoch index corruption! "
                    << Form( " Current cycle 0x%09X New cycle 0x%09X", fvulCurrentEpochCycle[fuGet4Nr], ulEpochCycleVal );
*/
    fvulCurrentEpochCycle[fuGet4Nr] = ulEpochCycleVal;
  }  // for( uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGet4Index ++ )
  return;
}

void CbmMcbm2018MonitorTofPulser::FillHitInfo(gdpbv100::Message mess) {
  UInt_t uChannel = mess.getGdpbHitChanId();
  UInt_t uTot     = mess.getGdpbHit32Tot();
  UInt_t uFts     = mess.getGdpbHitFineTs();

  ULong64_t ulCurEpochGdpbGet4 = fvulCurrentEpoch[fuGet4Nr];

  // In Ep. Suppr. Mode, receive following epoch instead of previous
  if (0 < ulCurEpochGdpbGet4)
    ulCurEpochGdpbGet4--;
  else
    ulCurEpochGdpbGet4 = gdpbv100::kuEpochCounterSz;  // Catch epoch cycle!

  //   UInt_t uChannelNr   = fuGet4Id * fuNrOfChannelsPerGet4 + uChannel;
  UInt_t uChannelNrInFee =
    (fuGet4Id % fuNrOfGet4PerFee) * fuNrOfChannelsPerGet4 + uChannel;
  UInt_t uFeeNr      = (fuGet4Id / fuNrOfGet4PerFee);
  UInt_t uFeeNrInSys = fuGdpbNr * fuNrOfFeePerGdpb + uFeeNr;
  //   UInt_t uRemappedChannelNr = uFeeNr * fuNrOfChannelsPerFee + fvuGet4ToPadi[ uChannelNrInFee ];
  /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
  //   if( fuGdpbNr == fuDiamondDpbIdx || 0x90 == fuCurrentMsSysId )
  //      uRemappedChannelNr = uChannelNr;
  //   UInt_t uGbtxNr      = (uFeeNr / kuNbFeePerGbtx);
  //   UInt_t uFeeInGbtx  = (uFeeNr % kuNbFeePerGbtx);
  //   UInt_t uGbtxNrInSys = fuGdpbNr * kuNbGbtxPerGdpb + uGbtxNr;

  ULong_t ulHitTime = mess.getMsgFullTime(ulCurEpochGdpbGet4);
  Double_t dHitTime = mess.getMsgFullTimeD(ulCurEpochGdpbGet4);

  // In 32b mode the coarse counter is already computed back to 112 FTS bins
  // => need to hide its contribution from the Finetime
  // => FTS = Fullt TS modulo 112
  uFts = mess.getGdpbHitFullTs() % 112;

  /// FT/DNL/INL monitoring
  fhFeeFtDistribPerCh[uFeeNrInSys]->Fill(uFts, uChannelNrInFee);

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
  /// Save last hist time if pulser channel
  /// Fill the corresponding histos if the time difference is reasonnable )
  if (92 < uTot && uTot < 95) {
    if (gdpbv100::kuFeePulserChannel == uChannelNrInFee
        && fuGdpbNr != fuDiamondDpbIdx && 0x90 != fuCurrentMsSysId) {
      fdTsLastPulserHit[uFeeNrInSys] = dHitTime;
      fvuFeeNbHitsLastMs[uFeeNrInSys]++;
      fvhPulserCountEvoPerFeeGdpb[fuGdpbNr]->Fill(dHitTime * 1e-9, uFeeNr);
    }  // if( gdpbv100::kuFeePulserChannel == uChannelNrInFee && fuGdpbNr != fuDiamondDpbIdx && 0x90 != fuCurrentMsSysId )
       /// Diamond FEE have pulser on channel 0!
    else if ((fuGdpbNr == fuDiamondDpbIdx || 0x90 == fuCurrentMsSysId)
             && 0 == uChannelNrInFee) {
      fdTsLastPulserHit[uFeeNrInSys] = dHitTime;
      fvuFeeNbHitsLastMs[uFeeNrInSys]++;
      fvhPulserCountEvoPerFeeGdpb[fuGdpbNr]->Fill(dHitTime * 1e-9, uFeeNr);
    }  // if( ( fuGdpbNr == fuDiamondDpbIdx || 0x90 == fuCurrentMsSysId ) && 0 == uChannelNrInFee )
  }    // if( 92 < uTot && uTot < 95 )

  /*
   if (0 <= fdStartTime)
   {
      fvhChannelRate_gDPB[ fuGdpbNr ]->Fill( 1e-9 * (dHitTime - fdStartTime), uChannelNr );
      fvhRemapChRate_gDPB[ fuGdpbNr ]->Fill( 1e-9 * (dHitTime - fdStartTime), uRemappedChannelNr );
      fvhFeeRate_gDPB[(fuGdpbNr * fuNrOfFeePerGdpb) + uFeeNr]->Fill( 1e-9 * (dHitTime - fdStartTime));
      fvhFeeErrorRatio_gDPB[(fuGdpbNr * fuNrOfFeePerGdpb) + uFeeNr]->Fill( 1e-9 * (dHitTime - fdStartTime), 0, 1);

      fvhModRate[ fviModuleId[ uGbtxNrInSys ] ]->Fill( 1e-9 * (dHitTime - fdStartTime));
      fvhModErrorRatio[ fviModuleId[ uGbtxNrInSys ] ]->Fill( 1e-9 * (dHitTime - fdStartTime), 0, 1);
   } // if (0 <= fdStartTime)

   if (0 <= fdStartTimeLong)
   {
      fvhFeeRateLong_gDPB[(fuGdpbNr * fuNrOfFeePerGdpb) + uFeeNr]->Fill(
            1e-9 / 60.0 * (dHitTime - fdStartTimeLong), 1 / 60.0 );
      fvhFeeErrorRatioLong_gDPB[(fuGdpbNr * fuNrOfFeePerGdpb) + uFeeNr]->Fill(
            1e-9 / 60.0 * (dHitTime - fdStartTimeLong), 0, 1 / 60.0 );
   } // if (0 <= fdStartTimeLong)
*/
  if (kTRUE == fbPrintAllHitsEnable) {
    LOG(info) << "Hit: " << Form("0x%08x ", fuGdpbId) << ", " << std::setw(2)
              << fuGet4Nr << ", " << std::setw(3) << uChannel << ", "
              << std::setw(3) << uTot << ", epoch " << std::setw(3)
              << ulCurEpochGdpbGet4 << ", FullTime Clk "
              << Form("%12lu ", ulHitTime) << ", FullTime s "
              << Form("%12.9f ", dHitTime / 1e9) << ", FineTime " << uFts;
  }  // if( kTRUE == fbPrintAllHitsEnable )
}

void CbmMcbm2018MonitorTofPulser::FillEpochInfo(gdpbv100::Message mess) {
  ULong64_t ulEpochNr = mess.getGdpbEpEpochNb();
  /*
   if( fvulCurrentEpoch[fuGet4Nr] < ulEpochNr )
      fvulCurrentEpochCycle[fuGet4Nr]++;
*/
  fvulCurrentEpoch[fuGet4Nr] = ulEpochNr;
  fvulCurrentEpochFull[fuGet4Nr] =
    ulEpochNr + gdpbv100::kulEpochCycleBins * fvulCurrentEpochCycle[fuGet4Nr];

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

void CbmMcbm2018MonitorTofPulser::PrintSlcInfo(gdpbv100::Message /*mess*/) {
  /*
   if (fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find(fuGdpbId))
   {
      UInt_t uChip = mess.getGdpbGenChipId();
      UInt_t uChan = mess.getGdpbSlcChan();
      UInt_t uEdge = mess.getGdpbSlcEdge();
      UInt_t uData = mess.getGdpbSlcData();
      UInt_t uType = mess.getGdpbSlcType();
      Double_t dGdpbChId =  fuGet4Id * fuNrOfChannelsPerGet4 + mess.getGdpbSlcChan() + 0.5 * mess.getGdpbSlcEdge();
      Double_t dFullChId =  fuGet4Nr * fuNrOfChannelsPerGet4 + mess.getGdpbSlcChan() + 0.5 * mess.getGdpbSlcEdge();
      Double_t dMessTime = static_cast< Double_t>( fulCurrentEpochTime ) * 1.e-9;


   }
*/
}

void CbmMcbm2018MonitorTofPulser::PrintGenInfo(gdpbv100::Message mess) {
  Int_t mType    = mess.getMessageType();
  Int_t channel  = mess.getGdpbHitChanId();
  uint64_t uData = mess.getData();

  LOG(debug) << "Get4 MSG type " << mType << " from gdpbId " << fuGdpbId
             << ", getId " << fuGet4Id << ", (hit channel) " << channel
             << " data " << std::hex << uData;
}

void CbmMcbm2018MonitorTofPulser::PrintSysInfo(gdpbv100::Message mess) {
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

void CbmMcbm2018MonitorTofPulser::FillPattInfo(gdpbv100::Message /*mess*/) {
  /*
   uint16_t usType   = mess.getGdpbSysPattType();
   uint16_t usIndex  = mess.getGdpbSysPattIndex();
   uint32_t uPattern = mess.getGdpbSysPattPattern();

   switch( usType )
   {
      case gdpbv100::PATT_MISSMATCH:
      {
         LOG(debug) << Form( "Missmatch pattern message => Type %d, Index %2d, Pattern 0x%08X", usType, usIndex, uPattern );
         for( UInt_t uBit = 0; uBit < 32; ++uBit )
            if( ( uPattern >> uBit ) & 0x1 )
            {
               UInt_t uBadAsic = ConvertElinkToGet4( 32 * usIndex + uBit );
                  /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
               if( fuGdpbNr == fuDiamondDpbIdx || 0x90 == fuCurrentMsSysId )
                  uBadAsic = 32 * usIndex + uBit;
            } // if( ( uPattern >> uBit ) & 0x1 )

         break;
      } // case gdpbv100::PATT_MISSMATCH:
      case gdpbv100::PATT_ENABLE:
      {
         for( UInt_t uBit = 0; uBit < 32; ++uBit )
            if( ( uPattern >> uBit ) & 0x1 )
            {
               UInt_t uBadAsic = ConvertElinkToGet4( 32 * usIndex + uBit );
                  /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
               if( fuGdpbNr == fuDiamondDpbIdx || 0x90 == fuCurrentMsSysId )
                  uBadAsic = 32 * usIndex + uBit;
            } // if( ( uPattern >> uBit ) & 0x1 )

         break;
      } // case gdpbv100::PATT_ENABLE:
      case gdpbv100::PATT_RESYNC:
      {
         LOG(debug) << Form( "RESYNC pattern message => Type %d, Index %2d, Pattern 0x%08X", usType, usIndex, uPattern );

         for( UInt_t uBit = 0; uBit < 32; ++uBit )
            if( ( uPattern >> uBit ) & 0x1 )
            {
               UInt_t uBadAsic = ConvertElinkToGet4( 32 * usIndex + uBit );
                  /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
               if( fuGdpbNr == fuDiamondDpbIdx || 0x90 == fuCurrentMsSysId )
                  uBadAsic = 32 * usIndex + uBit;
            } // if( ( uPattern >> uBit ) & 0x1 )

         break;
      } // case gdpbv100::PATT_RESYNC:
      default:
      {
         LOG(debug) << "Crazy pattern message, subtype " << usType;
         break;
      } // default
   } // switch( usType )
*/
  return;
}

void CbmMcbm2018MonitorTofPulser::FillStarTrigInfo(gdpbv100::Message mess) {
  Int_t iMsgIndex = mess.getStarTrigMsgIndex();

  switch (iMsgIndex) {
    case 0: fvulGdpbTsMsb[fuGdpbNr] = mess.getGdpbTsMsbStarA(); break;
    case 1:
      fvulGdpbTsLsb[fuGdpbNr] = mess.getGdpbTsLsbStarB();
      fvulStarTsMsb[fuGdpbNr] = mess.getStarTsMsbStarB();
      break;
    case 2: fvulStarTsMid[fuGdpbNr] = mess.getStarTsMidStarC(); break;
    case 3: {

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
      /*
         /// Histograms filling only in core MS
         if( fuCurrentMs < fuCoreMs  )
         {
            /// In Run rate evolution
            if( 0 <= fdStartTime )
            {
               /// Reset the evolution Histogram and the start time when we reach the end of the range
               if( fuHistoryHistoSize < 1e-9 * (fvulGdpbTsFullLast[fuGdpbNr] * gdpbv100::kdClockCycleSizeNs - fdStartTime) )
               {
                  ResetEvolutionHistograms();
                  fdStartTime = fvulGdpbTsFullLast[fuGdpbNr] * gdpbv100::kdClockCycleSizeNs;
               } // if( fuHistoryHistoSize < 1e-9 * (fulGdpbTsFullLast * gdpbv100::kdClockCycleSizeNs - fdStartTime) )

               fvhTriggerRate[fuGdpbNr]->Fill( 1e-9 * ( fvulGdpbTsFullLast[fuGdpbNr] * gdpbv100::kdClockCycleSizeNs - fdStartTime ) );
               fvhStarTokenEvo[fuGdpbNr]->Fill( 1e-9 * ( fvulGdpbTsFullLast[fuGdpbNr] * gdpbv100::kdClockCycleSizeNs - fdStartTime ),
                                               fvuStarTokenLast[fuGdpbNr] );
               fvhStarTrigGdpbTsEvo[fuGdpbNr]->Fill( 1e-9 * ( fvulGdpbTsFullLast[fuGdpbNr] * gdpbv100::kdClockCycleSizeNs - fdStartTime ),
                                                     fvulGdpbTsFullLast[fuGdpbNr] );
               fvhStarTrigStarTsEvo[fuGdpbNr]->Fill( 1e-9 * ( fvulGdpbTsFullLast[fuGdpbNr] * gdpbv100::kdClockCycleSizeNs - fdStartTime ),
                                                     fvulStarTsFullLast[fuGdpbNr] );
            } // if( 0 < fdStartTime )
               else fdStartTime = fvulGdpbTsFullLast[fuGdpbNr] * gdpbv100::kdClockCycleSizeNs;
            fvhCmdDaqVsTrig[fuGdpbNr]->Fill( fvuStarDaqCmdLast[fuGdpbNr], fvuStarTrigCmdLast[fuGdpbNr] );
         } // if( fuCurrentMs < fuCoreMs  )
*/
      break;
    }  // case 3
    default: LOG(error) << "Unknown Star Trigger messageindex: " << iMsgIndex;
  }  // switch( iMsgIndex )
}

void CbmMcbm2018MonitorTofPulser::Reset() {}

void CbmMcbm2018MonitorTofPulser::Finish() {
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

  /// Update the DNL/INL plots
  UpdateNormedDnlInl();

  SaveAllHistos(fsHistoFileFullname);
  //   SaveAllHistos();
}

void CbmMcbm2018MonitorTofPulser::FillOutput(CbmDigi* /*digi*/) {}

void CbmMcbm2018MonitorTofPulser::SaveAllHistos(TString sFileName) {
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
  fhTimeMeanPulser->Write();
  fhTimeRmsPulser->Write();
  fhTimeRmsZoomFitPuls->Write();
  fhTimeResFitPuls->Write();

  gDirectory->cd("..");


  ///* Pulser monitoring *///
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
    fvhPulserCountEvoPerFeeGdpb[uGdpb]->Write();
    for (UInt_t uGbtx = 0; uGbtx < kuNbGbtxPerGdpb - 1; ++uGbtx)
      fvhPulserTimeDiffEvoGbtxGbtx[uGdpb * (kuNbGbtxPerGdpb - 1) + uGbtx]
        ->Write();

    for (UInt_t uGdpbB = uGdpb + 1; uGdpbB < fuNrOfGdpbs; ++uGdpbB)
      fvvhPulserTimeDiffEvoGdpbGdpb[uGdpb][uGdpbB]->Write();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  for (UInt_t uFeeRef = 0; uFeeRef < kuNbRefFeeEvo; ++uFeeRef) {
    for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFee++) {
      fvvhPulserTimeDiffEvoFeeFee[uFeeRef][uFee]->Write();
    }  // for( UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFee++)
  }    // for( UInt_t uFeeRef = 0; uFeeRef < kuNbRefFeeEvo; ++uFeeRef )

  gDirectory->cd("..");

  gDirectory->mkdir("TofDnlInl");
  gDirectory->cd("TofDnlInl");
  for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFee++) {
    fhFeeFtDistribPerCh[uFee]->Write();
    fhFeeFtNormDnl[uFee]->Write();
    fhFeeFtNormInl[uFee]->Write();
  }  // for( UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFee++)

  gDirectory->cd("..");

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


  if ("" != sFileName) {
    // Restore original directory position
    histoFile->Close();
  }  // if( "" != sFileName )

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

void CbmMcbm2018MonitorTofPulser::ResetAllHistos() {
  LOG(info) << "Reseting all TOF histograms.";


  ///* Pulser monitoring *///
  for (UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeA++)
    for (UInt_t uFeeB = 0; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++)
      if (NULL != fvhTimeDiffPulser[uFeeA][uFeeB])
        fvhTimeDiffPulser[uFeeA][uFeeB]->Reset();

  fhTimeMeanPulser->Reset();
  fhTimeRmsPulser->Reset();
  fhTimeRmsZoomFitPuls->Reset();
  fhTimeResFitPuls->Reset();

  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fvhPulserCountEvoPerFeeGdpb[uGdpb]->Reset();
    for (UInt_t uGbtx = 0; uGbtx < kuNbGbtxPerGdpb - 1; ++uGbtx)
      fvhPulserTimeDiffEvoGbtxGbtx[uGdpb * (kuNbGbtxPerGdpb - 1) + uGbtx]
        ->Reset();

    for (UInt_t uGdpbB = uGdpb + 1; uGdpbB < fuNrOfGdpbs; ++uGdpbB)
      fvvhPulserTimeDiffEvoGdpbGdpb[uGdpb][uGdpbB]->Reset();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  for (UInt_t uFeeRef = 0; uFeeRef < kuNbRefFeeEvo; ++uFeeRef) {
    for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFee++) {
      fvvhPulserTimeDiffEvoFeeFee[uFeeRef][uFee]->Reset();
    }  // for( UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFee++)
  }    // for( UInt_t uFeeRef = 0; uFeeRef < kuNbRefFeeEvo; ++uFeeRef )

  for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFee++) {
    fhFeeFtDistribPerCh[uFee]->Reset();
    fhFeeFtNormDnl[uFee]->Reset();
    fhFeeFtNormInl[uFee]->Reset();
  }  // for( UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFee++)

  for (UInt_t uLinks = 0; uLinks < fvhMsSzPerLink.size(); uLinks++) {
    if (NULL == fvhMsSzPerLink[uLinks]) continue;

    fvhMsSzPerLink[uLinks]->Reset();
    fvhMsSzTimePerLink[uLinks]->Reset();
  }  // for( UInt_t uLinks = 0; uLinks < fvhMsSzPerLink.size(); uLinks++ )

  fdStartTime     = -1;
  fdStartTimeLong = -1;
  fdStartTimeMsSz = -1;
}
void CbmMcbm2018MonitorTofPulser::ResetEvolutionHistograms() {

  fdStartTime = -1;
}
void CbmMcbm2018MonitorTofPulser::ResetLongEvolutionHistograms() {

  fdStartTimeLong = -1;
}

void CbmMcbm2018MonitorTofPulser::UpdateZoomedFit() {
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
              << "CbmMcbm2018MonitorTofPulser::UpdateZoomedFit => Empty input "
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
            LOG(warning) << "CbmMcbm2018MonitorTofPulser::UpdateZoomedFit => "
                            "Zoom too strong, "
                         << "more than 1% loss for FEE pair " << uFeeA
                         << " and " << uFeeB << " !!! ";
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


          LOG(info) << "CbmMcbm2018MonitorTofPulser::UpdateZoomedFit => "
                    << "For FEE pair " << uFeeA << " and " << uFeeB
                    << " we have zoomed RMS = "
                    << fvhTimeDiffPulser[uFeeA][uFeeB]->GetRMS()
                    << " and a resolution of " << dRes / TMath::Sqrt2();

          // Restore original axis state?
          fvhTimeDiffPulser[uFeeA][uFeeB]->GetXaxis()->UnZoom();
        }  // loop on uFeeA and uFeeB + check if corresponding fvhTimeDiffPulser exists
  }  // if( 0.0 < fdFitZoomWidthPs )
  else {
    LOG(error) << "CbmMcbm2018MonitorTofPulser::UpdateZoomedFit => Zoom width "
                  "not defined, "
               << "please use SetFitZoomWidthPs, e.g. in macro, before trying "
                  "this update !!!";
  }  // else of if( 0.0 < fdFitZoomWidthPs )
}
void CbmMcbm2018MonitorTofPulser::UpdateNormedDnlInl()
{
  TDirectory* oldDir = gDirectory;

  gROOT->cd();
  TF1* constantVal    = new TF1("constant", "1", 0, gdpbv100::kdFtBinsNb);
  for (UInt_t uFeeNrInSys = 0; uFeeNrInSys < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeNrInSys++) {
    fhFeeFtNormDnl[uFeeNrInSys]->Reset();
    fhFeeFtNormInl[uFeeNrInSys]->Reset();

    for (UInt_t uChannel = 0; uChannel < fuNrOfChannelsPerFee; uChannel++) {
      // Rising edge
      TH1* pFtSelChSlice =
        fhFeeFtDistribPerCh[uFeeNrInSys]->ProjectionX("temp_pFtSelChSlice", 1 + uChannel, 1 + uChannel);
      if (0 < pFtSelChSlice->GetEntries()) {
        Double_t dNormFactRise = pFtSelChSlice->GetEntries() / gdpbv100::kdFtBinsNb;
        pFtSelChSlice->Scale(1.0 / dNormFactRise);
        pFtSelChSlice->Add(constantVal, -1.);
      }  // if( 0 < pFtSelChSliceRise->GetEntries() )

      // INLs + storage
      Double_t dDnl = 0.0;
      Double_t dInl = 0.0;
      for (UInt_t uFtBin = 0; uFtBin < gdpbv100::kdFtBinsNb; uFtBin++) {
        dDnl = pFtSelChSlice->GetBinContent(1 + uFtBin);
        dInl += dDnl;
        fhFeeFtNormDnl[uFeeNrInSys]->Fill(uFtBin, uChannel, dDnl);
        fhFeeFtNormInl[uFeeNrInSys]->Fill(uFtBin, uChannel, dInl);
      }  // for( UInt_t uFtBin = 0; uFtBin < gdpbv100::kuFineCounterSize; uFtBin++)

      delete pFtSelChSlice;
    }  // for( UInt_t uChannel = 0; uChannel < fuNrOfChannelsPerFee; uChannel++ )
  }    // for( UInt_t uFeeNrInSys = 0; uFeeNrInSys < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeNrInSys++ )
  delete constantVal;

  oldDir->cd();
}

ClassImp(CbmMcbm2018MonitorTofPulser)
