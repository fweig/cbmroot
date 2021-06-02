/** @file CbmMcbm2018TofTestFee.cxx
  * @copyright Copyright (C) 2018-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmMcbm2018TofTestFee                          -----
// -----               Created 10.07.2018 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018TofTestFee.h"

#include "CbmFlesHistosTools.h"
#include "CbmFormatDecHexPrintout.h"
#include "CbmFormatMsHeaderPrintout.h"
#include "CbmHistManager.h"
#include "CbmMcbm2018TofPar.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

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

Bool_t bMcbmTofTestFeeResetHistos       = kFALSE;
Bool_t bMcbmTofTestFeeSaveHistos        = kFALSE;
Bool_t bMcbmTofTestFeeUpdateZoomedFit   = kFALSE;
Bool_t bMcbmTofTestFeeRawDataPrint      = kFALSE;
Bool_t bMcbmTofTestFeePrintAllHitsEna   = kFALSE;
Bool_t bMcbmTofTestFeePrintAllEpochsEna = kFALSE;

CbmMcbm2018TofTestFee::CbmMcbm2018TofTestFee()
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
  , fvuFeeChanNbHitsLastMs()
  , fvdFeeChanMsLastPulserHit()
  , dMinDt(-1. * (kuNbBinsDt * gdpbv100::kdBinSize / 2.) - gdpbv100::kdBinSize / 2.)
  , dMaxDt(1. * (kuNbBinsDt * gdpbv100::kdBinSize / 2.) + gdpbv100::kdBinSize / 2.)
  , fuNbFeePlot(2)
  , fuNbFeePlotsPerGdpb(0)
  , fdStartTime(-1.)
  , fdStartTimeLong(-1.)
  , fdStartTimeMsSz(-1.)
  , fuHistoryHistoSize(1800)
  , fdLastRmsUpdateTime(0.0)
  , fdFitZoomWidthPs(0.0)
  , fcMsSizeAll(NULL)
  , fvhMsSzPerLink(12, NULL)
  , fvhMsSzTimePerLink(12, NULL)
  , fuGdpbA(0)
  , fuGbtxA(0)
  , fuFeeA(0)
  , fuGlobalIdxFeeA(0)
  , fuGdpbB(0)
  , fuGbtxB(0)
  , fuFeeB(1)
  , fuGlobalIdxFeeB(1)
  , fvhTimeDiffPulserFeeA()
  , fhTimeMeanPulserFeeA(NULL)
  , fhTimeRmsPulserFeeA(NULL)
  , fhTimeRmsZoomFitPulsFeeA(NULL)
  , fhTimeResFitPulsFeeA(NULL)
  , fvhTimeDiffPulserFeeB()
  , fhTimeMeanPulserFeeB(NULL)
  , fhTimeRmsPulserFeeB(NULL)
  , fhTimeRmsZoomFitPulsFeeB(NULL)
  , fhTimeResFitPulsFeeB(NULL)
  , fvhTimeDiffPulserFeeFee()
  , fhTimeMeanPulserFeeFee(NULL)
  , fhTimeRmsPulserFeeFee(NULL)
  , fhTimeRmsZoomFitPulsFeeFee(NULL)
  , fhTimeResFitPulsFeeFee(NULL)
  , fhChanTotFeeA(NULL)
  , fhChanTotFeeB(NULL)
  , fhChanPulseIntervalFeeA(NULL)
  , fhChanPulseIntervalFeeB(NULL)
  , fvhPulserCountEvoPerFeeGdpb()
  , fcPulserFeeA(NULL)
  , fcPulserFeeB(NULL)
  , fcPulserFeeFee(NULL)
  , fcPulseProp(NULL)
  , fvuPadiToGet4()
  , fvuGet4ToPadi()
  , fvuElinkToGet4()
  , fvuGet4ToElink()
  , fTimeLastHistoSaving()
{
}

CbmMcbm2018TofTestFee::~CbmMcbm2018TofTestFee() {}

Bool_t CbmMcbm2018TofTestFee::Init()
{
  LOG(info) << "Initializing Get4 monitor";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) { LOG(fatal) << "No FairRootManager instance"; }  // if( ioman == NULL )

  return kTRUE;
}

void CbmMcbm2018TofTestFee::SetParContainers()
{
  LOG(info) << "Setting parameter containers for " << GetName();
  fUnpackPar = (CbmMcbm2018TofPar*) (FairRun::Instance()->GetRuntimeDb()->getContainer("CbmMcbm2018TofPar"));
}

Bool_t CbmMcbm2018TofTestFee::InitContainers()
{
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

Bool_t CbmMcbm2018TofTestFee::ReInitContainers()
{
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
    LOG(info) << "GDPB Id of TOF  " << i << " : " << std::hex << fUnpackPar->GetGdpbId(i) << std::dec;
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
  LOG(info) << "Timeslice parameters: " << fuTotalMsNb << " MS per link, of which " << fuOverlapMsNb
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
  fuGlobalIdxFeeA = fuNrOfFeePerGdpb * fuGdpbA + fUnpackPar->GetNrOfFeePerGbtx() * fuGbtxA + fuFeeA;
  fuGlobalIdxFeeB = fuNrOfFeePerGdpb * fuGdpbB + fUnpackPar->GetNrOfFeePerGbtx() * fuGbtxB + fuFeeB;

  fvuFeeChanNbHitsLastMs.resize(fuNrOfFeePerGdpb * fuNrOfGdpbs);
  fvdFeeChanMsLastPulserHit.resize(fuNrOfFeePerGdpb * fuNrOfGdpbs);
  for (UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb * fuNrOfGdpbs; ++uFee) {
    fvuFeeChanNbHitsLastMs[uFee].resize(fuNrOfChannelsPerFee, 0);
    fvdFeeChanMsLastPulserHit[uFee].resize(fuNrOfChannelsPerFee, 0.0);
  }  // for( UInt_t uFee = 0; uFee < fuNrOfFeePerGdpb * fuNrOfGdpbs; ++uFee );

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
                            8,  7,  6,  5,  12, 11, 10, 9,  16, 15, 14, 13, 20, 19,
                            18, 17, 24, 23, 22, 21, 28, 27, 26, 25, 32, 31, 30, 29};

  UInt_t uPaditoget4[32] = {4,  3,  2,  1,  // provided by Jochen
                            12, 11, 10, 9, 20, 19, 18, 17, 28, 27, 26, 25, 32, 31,
                            30, 29, 8,  7, 6,  5,  16, 15, 14, 13, 24, 23, 22, 21};

  for (UInt_t uChan = 0; uChan < fuNrOfChannelsPerFee; ++uChan) {
    fvuPadiToGet4[uChan] = uPaditoget4[uChan] - 1;
    fvuGet4ToPadi[uChan] = uGet4topadi[uChan] - 1;
  }  // for( UInt_t uChan = 0; uChan < fuNrOfChannelsPerFee; ++uChan )


  /// TODO: move these constants somewhere shared, e.g the parameter file
  fvuElinkToGet4.resize(kuNbGet4PerGbtx);
  fvuGet4ToElink.resize(kuNbGet4PerGbtx);
  UInt_t kuElinkToGet4[kuNbGet4PerGbtx] = {27, 2,  7,  3,  31, 26, 30, 1,  33, 37, 32, 13, 9,  14,
                                           10, 15, 17, 21, 16, 35, 34, 38, 25, 24, 0,  6,  20, 23,
                                           18, 22, 28, 4,  29, 5,  19, 36, 39, 8,  12, 11};
  UInt_t kuGet4ToElink[kuNbGet4PerGbtx] = {24, 7,  1,  3,  31, 33, 25, 2,  37, 12, 14, 39, 38, 11,
                                           13, 15, 18, 16, 28, 34, 26, 17, 29, 27, 23, 22, 5,  0,
                                           30, 32, 6,  4,  10, 8,  20, 19, 35, 9,  21, 36};

  for (UInt_t uLinkAsic = 0; uLinkAsic < kuNbGet4PerGbtx; ++uLinkAsic) {
    fvuElinkToGet4[uLinkAsic] = kuElinkToGet4[uLinkAsic];
    fvuGet4ToElink[uLinkAsic] = kuGet4ToElink[uLinkAsic];
  }  // for( UInt_t uChan = 0; uChan < fuNrOfChannelsPerFee; ++uChan )

  return kTRUE;
}


void CbmMcbm2018TofTestFee::AddMsComponentToList(size_t component, UShort_t /*usDetectorId*/)
{
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Add to list
  fvMsComponentsList.push_back(component);

  /// Create MS size monitoring histos
  if (NULL == fvhMsSzPerLink[component]) {
    TString sMsSzName         = Form("MsSz_link_%02lu", component);
    TString sMsSzTitle        = Form("Size of MS from link %02lu; Ms Size [bytes]", component);
    fvhMsSzPerLink[component] = new TH1F(sMsSzName.Data(), sMsSzTitle.Data(), 160000, 0., 20000.);

    sMsSzName  = Form("MsSzTime_link_%02lu", component);
    sMsSzTitle = Form("Size of MS vs time for gDPB of link %02lu; Time[s] ; Ms Size [bytes]", component);
    fvhMsSzTimePerLink[component] =
      new TProfile(sMsSzName.Data(), sMsSzTitle.Data(), 100 * fuHistoryHistoSize, 0., 2 * fuHistoryHistoSize);
    THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
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
void CbmMcbm2018TofTestFee::SetNbMsInTs(size_t uCoreMsNb, size_t uOverlapMsNb)
{
  fuNbCoreMsPerTs = uCoreMsNb;
  fuNbOverMsPerTs = uOverlapMsNb;

  //   UInt_t uNbMsTotal = fuNbCoreMsPerTs + fuNbOverMsPerTs;
}

void CbmMcbm2018TofTestFee::CreateHistograms()
{
  LOG(info) << "create Histos for " << fuNrOfGdpbs << " gDPBs ";

  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

  TString name {""};
  TString title {""};

  // Full Fee time difference test
  UInt_t uNbBinsDt = kuNbBinsDt + 1;  // To account for extra bin due to shift by 1/2 bin of both ranges

  fuNbFeePlotsPerGdpb = fuNrOfFeePerGdpb / fuNbFeePlot + (0 != fuNrOfFeePerGdpb % fuNbFeePlot ? 1 : 0);
  Double_t dBinSzG4v2 = (6250. / 112.);
  dMinDt              = -1. * (kuNbBinsDt * dBinSzG4v2 / 2.) - dBinSzG4v2 / 2.;
  dMaxDt              = 1. * (kuNbBinsDt * dBinSzG4v2 / 2.) + dBinSzG4v2 / 2.;


  /*******************************************************************/
  /// FEE pulser test channels
  fvhTimeDiffPulserFeeA.resize(fuNrOfChannelsPerFee);
  fvhTimeDiffPulserFeeB.resize(fuNrOfChannelsPerFee);
  fvhTimeDiffPulserFeeFee.resize(fuNrOfChannelsPerFee);
  for (UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerFee; uChanA++) {
    fvhTimeDiffPulserFeeA[uChanA].resize(fuNrOfChannelsPerFee);
    fvhTimeDiffPulserFeeB[uChanA].resize(fuNrOfChannelsPerFee);
    fvhTimeDiffPulserFeeFee[uChanA].resize(fuNrOfChannelsPerFee);
    for (UInt_t uChanB = 0; uChanB < fuNrOfChannelsPerFee; uChanB++) {
      if (uChanA < uChanB) {
        fvhTimeDiffPulserFeeA[uChanA][uChanB] =
          new TH1I(Form("hTimeDiffPulser_g%02u_gbt%1u_f%1u_ch%02u_ch%02u", fuGdpbA, fuGbtxA, fuFeeA, uChanA, uChanB),
                   Form("Time difference for pulser on gDPB %02u GBTx %02u FEE "
                        "%1u channels %02u and %02u; DeltaT [ps]; Counts",
                        fuGdpbA, fuGbtxA, fuFeeA, uChanA, uChanB),
                   uNbBinsDt, dMinDt, dMaxDt);

        fvhTimeDiffPulserFeeB[uChanA][uChanB] =
          new TH1I(Form("hTimeDiffPulser_g%02u_gbt%1u_f%1u_ch%02u_ch%02u", fuGdpbB, fuGbtxB, fuFeeB, uChanA, uChanB),
                   Form("Time difference for pulser on gDPB %02u GBTx %02u FEE "
                        "%1u channels %02u and %02u; DeltaT [ps]; Counts",
                        fuGdpbB, fuGbtxB, fuFeeB, uChanA, uChanB),
                   uNbBinsDt, dMinDt, dMaxDt);
      }  // if( uChanA < uFeeB )
      else {
        fvhTimeDiffPulserFeeA[uChanA][uChanB] = NULL;
        fvhTimeDiffPulserFeeB[uChanA][uChanB] = NULL;
      }  // else of if( uChanA < uChanB )

      fvhTimeDiffPulserFeeFee[uChanA][uChanB] =
        new TH1I(Form("hTimeDiffPulser_g%02u_gbt%1u_f%1u_ch%02u_g%02u_gbt%1u_f%1u_ch%02u", fuGdpbA, fuGbtxA, fuFeeA,
                      uChanA, fuGdpbB, fuGbtxB, fuFeeB, uChanB),
                 Form("Time difference for pulser on gDPB %02u GBTx %02u FEE %1u "
                      "channel %02u and gDPB %02u GBTx %02u FEE %1u channel %02u; "
                      "DeltaT [ps]; Counts",
                      fuGdpbA, fuGbtxA, fuFeeA, uChanA, fuGdpbB, fuGbtxB, fuFeeB, uChanB),
                 uNbBinsDt, dMinDt, dMaxDt);
    }  // for( UInt_t uChanB = 0; uChanB < fuNrOfChannelsPerFee; uChanB++)
  }    // for( UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerFee; uChanA++)

  /// FEE A
  name                 = "hTimeMeanPulserFeeA";
  fhTimeMeanPulserFeeA = new TH2D(name.Data(),
                                  "Time difference Mean for each channel pairs "
                                  "in FEE A; Chan A; Chan B ; Mean [ps]",
                                  fuNrOfChannelsPerFee - 1, -0.5, fuNrOfChannelsPerFee - 1.5, fuNrOfChannelsPerFee - 1,
                                  0.5, fuNrOfChannelsPerFee - 0.5);

  name                = "hTimeRmsPulserFeeA";
  fhTimeRmsPulserFeeA = new TH2D(name.Data(),
                                 "Time difference RMS for each channel pairs "
                                 "in FEE A; Chan A; Chan B; RMS [ps]",
                                 fuNrOfChannelsPerFee - 1, -0.5, fuNrOfChannelsPerFee - 1.5, fuNrOfChannelsPerFee - 1,
                                 0.5, fuNrOfChannelsPerFee - 0.5);

  name                     = "hTimeRmsZoomFitPulsFeeA";
  fhTimeRmsZoomFitPulsFeeA = new TH2D(name.Data(),
                                      "Time difference RMS after zoom for each channel pairs in FEE A; "
                                      "Chan A; Chan B; RMS [ps]",
                                      fuNrOfChannelsPerFee - 1, -0.5, fuNrOfChannelsPerFee - 1.5,
                                      fuNrOfChannelsPerFee - 1, 0.5, fuNrOfChannelsPerFee - 0.5);

  name                 = "hTimeResFitPulsFeeA";
  fhTimeResFitPulsFeeA = new TH2D(name.Data(),
                                  "Time difference Res from fit for each channel pairs in FEE A; "
                                  "Chan A; Chan B; Sigma [ps]",
                                  fuNrOfChannelsPerFee - 1, -0.5, fuNrOfChannelsPerFee - 1.5, fuNrOfChannelsPerFee - 1,
                                  0.5, fuNrOfChannelsPerFee - 0.5);

  /// FEE B
  name                 = "hTimeMeanPulserFeeB";
  fhTimeMeanPulserFeeB = new TH2D(name.Data(),
                                  "Time difference Mean for each channel pairs "
                                  "in FEE B; Chan A; Chan B ; Mean [ps]",
                                  fuNrOfChannelsPerFee - 1, -0.5, fuNrOfChannelsPerFee - 1.5, fuNrOfChannelsPerFee - 1,
                                  0.5, fuNrOfChannelsPerFee - 0.5);

  name                = "hTimeRmsPulserFeeB";
  fhTimeRmsPulserFeeB = new TH2D(name.Data(),
                                 "Time difference RMS for each channel pairs "
                                 "in FEE B; Chan A; Chan B; RMS [ps]",
                                 fuNrOfChannelsPerFee - 1, -0.5, fuNrOfChannelsPerFee - 1.5, fuNrOfChannelsPerFee - 1,
                                 0.5, fuNrOfChannelsPerFee - 0.5);

  name                     = "hTimeRmsZoomFitPulsFeeB";
  fhTimeRmsZoomFitPulsFeeB = new TH2D(name.Data(),
                                      "Time difference RMS after zoom for each channel pairs in FEE B; "
                                      "Chan A; Chan B; RMS [ps]",
                                      fuNrOfChannelsPerFee - 1, -0.5, fuNrOfChannelsPerFee - 1.5,
                                      fuNrOfChannelsPerFee - 1, 0.5, fuNrOfChannelsPerFee - 0.5);

  name                 = "hTimeResFitPulsFeeB";
  fhTimeResFitPulsFeeB = new TH2D(name.Data(),
                                  "Time difference Res from fit for each channel pairs in FEE B; "
                                  "Chan A; Chan B; Sigma [ps]",
                                  fuNrOfChannelsPerFee - 1, -0.5, fuNrOfChannelsPerFee - 1.5, fuNrOfChannelsPerFee - 1,
                                  0.5, fuNrOfChannelsPerFee - 0.5);

  /// FEE-FEE
  name                   = "hTimeMeanPulserFeeFee";
  fhTimeMeanPulserFeeFee = new TH2D(name.Data(),
                                    "Time difference Mean for each channel pairs in FEE A & B; Chan "
                                    "FEE A; Chan FEE B ; Mean [ps]",
                                    fuNrOfChannelsPerFee, -0.5, fuNrOfChannelsPerFee - 0.5, fuNrOfChannelsPerFee, -0.5,
                                    fuNrOfChannelsPerFee - 0.5);

  name                  = "hTimeRmsPulserFeeFee";
  fhTimeRmsPulserFeeFee = new TH2D(name.Data(),
                                   "Time difference RMS for each channel pairs in FEE A & B; Chan "
                                   "FEE A; Chan FEE B; RMS [ps]",
                                   fuNrOfChannelsPerFee, -0.5, fuNrOfChannelsPerFee - 0.5, fuNrOfChannelsPerFee, -0.5,
                                   fuNrOfChannelsPerFee - 0.5);

  name                       = "hTimeRmsZoomFitPulsFeeFee";
  fhTimeRmsZoomFitPulsFeeFee = new TH2D(name.Data(),
                                        "Time difference RMS after zoom for each channel pairs in FEE A & "
                                        "B; Chan FEE A; Chan FEE B; RMS [ps]",
                                        fuNrOfChannelsPerFee, -0.5, fuNrOfChannelsPerFee - 0.5, fuNrOfChannelsPerFee,
                                        -0.5, fuNrOfChannelsPerFee - 0.5);

  name                   = "hTimeResFitPulsFeeFee";
  fhTimeResFitPulsFeeFee = new TH2D(name.Data(),
                                    "Time difference Res from fit for each channel pairs in FEE A & "
                                    "B; Chan FEE A; Chan FEE B; Sigma [ps]",
                                    fuNrOfChannelsPerFee, -0.5, fuNrOfChannelsPerFee - 0.5, fuNrOfChannelsPerFee, -0.5,
                                    fuNrOfChannelsPerFee - 0.5);

  /// TOT plots
  name          = "hChanTotFeeA";
  fhChanTotFeeA = new TH2D(name.Data(), "TOT distribution per channel in FEE A; Chan FEE A; TOT [bin]; Counts []",
                           fuNrOfChannelsPerFee, -0.5, fuNrOfChannelsPerFee - 0.5, gdpbv100::kuTotCounterSize, -0.5,
                           gdpbv100::kuTotCounterSize - 0.5);
  name          = "hChanTotFeeB";
  fhChanTotFeeB = new TH2D(name.Data(), "TOT distribution per channel in FEE B; Chan FEE B; TOT [bin]; Counts []",
                           fuNrOfChannelsPerFee, -0.5, fuNrOfChannelsPerFee - 0.5, gdpbv100::kuTotCounterSize, -0.5,
                           gdpbv100::kuTotCounterSize - 0.5);

  /// Pulse distance plots
  /// Logarithmic bining
  uint32_t iNbBinsLog = 0;
  /// Parameters are NbDecadesLog, NbStepsDecade, NbSubStepsInStep
  std::vector<double> dBinsLogVector = GenerateLogBinArray(9, 9, 10, iNbBinsLog);
  double* dBinsLog                   = dBinsLogVector.data();
  //   double * dBinsLog = GenerateLogBinArray( 9, 9, 10, iNbBinsLog );

  name                    = "hChanPulseIntervalFeeA";
  fhChanPulseIntervalFeeA = new TH2D(name.Data(),
                                     "Pulses time interval per channel in FEE A; Time interval [ns]; "
                                     "Chan FEE A; Counts []",
                                     iNbBinsLog, dBinsLog, fuNrOfChannelsPerFee, -0.5, fuNrOfChannelsPerFee - 0.5);
  name                    = "hChanPulseIntervalFeeB";
  fhChanPulseIntervalFeeB = new TH2D(name.Data(),
                                     "Pulses time interval per channel in FEE B; Time interval [ns]; "
                                     "Chan FEE B; Counts []",
                                     iNbBinsLog, dBinsLog, fuNrOfChannelsPerFee, -0.5, fuNrOfChannelsPerFee - 0.5);

  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    name = Form("hPulserCountEvoPerFeeGdpb%02u", uGdpb);
    fvhPulserCountEvoPerFeeGdpb.push_back(
      new TH2D(name.Data(), Form("Pulser count per FEE in gDPB %02u; time in run [s]; FEE []", uGdpb),
               fuHistoryHistoSize, 0, fuHistoryHistoSize, fuNrOfFeePerGdpb, -0.5, fuNrOfFeePerGdpb));
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  /// Cleanup array of log bins
  //   delete dBinsLog;

  if (server) {
    for (UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerFee; uChanA++)
      for (UInt_t uChanB = 0; uChanB < fuNrOfChannelsPerFee; uChanB++) {
        if (NULL != fvhTimeDiffPulserFeeA[uChanA][uChanB])
          server->Register("/TofDtFeeA", fvhTimeDiffPulserFeeA[uChanA][uChanB]);
        if (NULL != fvhTimeDiffPulserFeeB[uChanA][uChanB])
          server->Register("/TofDtFeeB", fvhTimeDiffPulserFeeB[uChanA][uChanB]);

        server->Register("/TofDtFeeFee", fvhTimeDiffPulserFeeFee[uChanA][uChanB]);
      }  // Loop anc channels A and B

    server->Register("/TofDt", fhTimeMeanPulserFeeA);
    server->Register("/TofDt", fhTimeRmsPulserFeeA);
    server->Register("/TofDt", fhTimeRmsZoomFitPulsFeeA);
    server->Register("/TofDt", fhTimeResFitPulsFeeA);

    server->Register("/TofDt", fhTimeMeanPulserFeeB);
    server->Register("/TofDt", fhTimeRmsPulserFeeB);
    server->Register("/TofDt", fhTimeRmsZoomFitPulsFeeB);
    server->Register("/TofDt", fhTimeResFitPulsFeeB);

    server->Register("/TofDt", fhTimeMeanPulserFeeFee);
    server->Register("/TofDt", fhTimeRmsPulserFeeFee);
    server->Register("/TofDt", fhTimeRmsZoomFitPulsFeeFee);
    server->Register("/TofDt", fhTimeResFitPulsFeeFee);

    server->Register("/TofPulse", fhChanTotFeeA);
    server->Register("/TofPulse", fhChanTotFeeB);
    server->Register("/TofPulse", fhChanPulseIntervalFeeA);
    server->Register("/TofPulse", fhChanPulseIntervalFeeB);

    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
      server->Register("/TofCnt", fvhPulserCountEvoPerFeeGdpb[uGdpb]);

    }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

    server->RegisterCommand("/Reset_All_eTOF", "bMcbmTofTestFeeResetHistos=kTRUE");
    server->RegisterCommand("/Save_All_eTof", "bMcbmTofTestFeeSaveHistos=kTRUE");
    server->RegisterCommand("/Update_PulsFit", "bMcbmTofTestFeeUpdateZoomedFit=kTRUE");
    server->RegisterCommand("/Print_Raw_Data", "bMcbmTofTestFeeRawDataPrint=kTRUE");
    server->RegisterCommand("/Print_AllHits", "bMcbmTofTestFeePrintAllHitsEna=kTRUE");
    server->RegisterCommand("/Print_AllEps", "bMcbmTofTestFeePrintAllEpochsEna=kTRUE");

    server->Restrict("/Reset_All_eTof", "allow=admin");
    server->Restrict("/Save_All_eTof", "allow=admin");
    server->Restrict("/Update_PulsFit", "allow=admin");
    server->Restrict("/Print_Raw_Data", "allow=admin");
    server->Restrict("/Print_AllHits", "allow=admin");
    server->Restrict("/Print_AllEps", "allow=admin");
  }  // if( server )

  /*****************************/
  Double_t w = 600;
  Double_t h = 480;

  /** Create Pulser check Canvas for FEE A **/
  fcPulserFeeA = new TCanvas("cPulserFeeA", "Time difference RMS for channels on FEE A", w, h);
  fcPulserFeeA->Divide(2, 2);

  fcPulserFeeA->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  fhTimeRmsPulserFeeA->Draw("colz");

  fcPulserFeeA->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  fhTimeMeanPulserFeeA->Draw("colz");

  fcPulserFeeA->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  fhTimeRmsZoomFitPulsFeeA->Draw("colz");

  fcPulserFeeA->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  fhTimeResFitPulsFeeA->Draw("colz");

  server->Register("/canvases", fcPulserFeeA);
  /*****************************/

  /** Create Pulser check Canvas for FEE B **/
  fcPulserFeeB = new TCanvas("cPulserFeeB", "Time difference RMS for channels on FEE A", w, h);
  fcPulserFeeB->Divide(2, 2);

  fcPulserFeeB->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  fhTimeRmsPulserFeeB->Draw("colz");

  fcPulserFeeB->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  fhTimeMeanPulserFeeB->Draw("colz");

  fcPulserFeeB->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  fhTimeRmsZoomFitPulsFeeB->Draw("colz");

  fcPulserFeeB->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  fhTimeResFitPulsFeeB->Draw("colz");

  server->Register("/canvases", fcPulserFeeB);
  /*****************************/

  /** Create Pulser check Canvas for FEE A VS B **/
  fcPulserFeeFee = new TCanvas("cPulserFeeFee", "Time difference RMS for channels on FEE A VS FEE B", w, h);
  fcPulserFeeFee->Divide(2, 2);

  fcPulserFeeFee->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  fhTimeRmsPulserFeeFee->Draw("colz");

  fcPulserFeeFee->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  fhTimeMeanPulserFeeFee->Draw("colz");

  fcPulserFeeFee->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  fhTimeRmsZoomFitPulsFeeFee->Draw("colz");

  fcPulserFeeFee->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  fhTimeResFitPulsFeeFee->Draw("colz");

  server->Register("/canvases", fcPulserFeeFee);
  /*****************************/

  /** Create Pulse properties Canvas for FEE A & B **/
  fcPulseProp = new TCanvas("cPulseProp", "Pulse properties for each channel on FEE A and FEE B", w, h);
  fcPulseProp->Divide(2, 2);

  fcPulseProp->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhChanTotFeeA->Draw("colz");

  fcPulseProp->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhChanTotFeeB->Draw("colz");

  fcPulseProp->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogx();
  gPad->SetLogz();
  fhChanPulseIntervalFeeA->Draw("colz");

  fcPulseProp->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogx();
  gPad->SetLogz();
  fhChanPulseIntervalFeeB->Draw("colz");

  server->Register("/canvases", fcPulseProp);
  /*****************************/

  /** Recovers/Create Ms Size Canvas for STAR 2018 **/
  // Try to recover canvas in case it was created already by another monitor
  // If not existing, create it
  fcMsSizeAll = dynamic_cast<TCanvas*>(gROOT->FindObject("cMsSizeAll"));
  if (NULL == fcMsSizeAll) {
    fcMsSizeAll = new TCanvas("cMsSizeAll", "Evolution of MS size in last 300 s", w, h);
    fcMsSizeAll->Divide(4, 3);
    LOG(info) << "Created MS size canvas in TOF monitor";
  }  // if( NULL == fcMsSizeAll )
  else
    LOG(info) << "Recovered MS size canvas in TOF monitor";

  LOG(info) << "Leaving CreateHistograms";
}

Bool_t CbmMcbm2018TofTestFee::DoUnpack(const fles::Timeslice& ts, size_t component)
{
  if (bMcbmTofTestFeeResetHistos) {
    LOG(info) << "Reset eTOF STAR histos ";
    ResetAllHistos();
    bMcbmTofTestFeeResetHistos = kFALSE;
  }  // if( bMcbmTofTestFeeResetHistos )
  if (bMcbmTofTestFeeSaveHistos) {
    LOG(info) << "Start saving eTOF STAR histos ";
    SaveAllHistos("data/histos_Shift_StarTof.root");
    bMcbmTofTestFeeSaveHistos = kFALSE;
  }  // if( bSaveStsHistos )
  if (bMcbmTofTestFeeUpdateZoomedFit) {
    UpdateZoomedFit(fvhTimeDiffPulserFeeA, fhTimeRmsZoomFitPulsFeeA, fhTimeResFitPulsFeeA);
    UpdateZoomedFit(fvhTimeDiffPulserFeeB, fhTimeRmsZoomFitPulsFeeB, fhTimeResFitPulsFeeB);
    UpdateZoomedFit(fvhTimeDiffPulserFeeFee, fhTimeRmsZoomFitPulsFeeFee, fhTimeResFitPulsFeeFee);
    bMcbmTofTestFeeUpdateZoomedFit = kFALSE;
  }  // if (bMcbmTofTestFeeUpdateZoomedFit)
  if (bMcbmTofTestFeeRawDataPrint) {
    fuRawDataPrintMsgIdx        = 0;
    bMcbmTofTestFeeRawDataPrint = kFALSE;
  }  // if( bMcbmTofTestFeeRawDataPrint )
  if (bMcbmTofTestFeePrintAllHitsEna) {
    fbPrintAllHitsEnable           = !fbPrintAllHitsEnable;
    bMcbmTofTestFeePrintAllHitsEna = kFALSE;
  }  // if( bMcbmTofTestFeePrintAllHitsEna )
  if (bMcbmTofTestFeePrintAllEpochsEna) {
    fbPrintAllEpochsEnable         = !fbPrintAllEpochsEnable;
    bMcbmTofTestFeePrintAllHitsEna = kFALSE;
  }  // if( bMcbmTofTestFeePrintAllEpochsEna )

  /// Periodically save the histograms
  std::chrono::time_point<std::chrono::system_clock> timeCurrent = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds                  = timeCurrent - fTimeLastHistoSaving;
  if (0 == fTimeLastHistoSaving.time_since_epoch().count()) {
    fTimeLastHistoSaving = timeCurrent;
    //      fulNbBuiltSubEventLastPrintout = fulNbBuiltSubEvent;
    //      fulNbStarSubEventLastPrintout  = fulNbStarSubEvent;
  }  // if( 0 == fTimeLastHistoSaving.time_since_epoch().count() )
  else if (300 < elapsed_seconds.count()) {
    std::time_t cTimeCurrent = std::chrono::system_clock::to_time_t(timeCurrent);
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

  LOG(debug1) << "Timeslice contains " << ts.num_microslices(component) << "microslices.";

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
      for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx) {
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
    for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx) {
      constexpr uint32_t kuBytesPerMessage = 8;

      UInt_t uMsComp           = fvMsComponentsList[uMsCompIdx];
      auto msDescriptor        = ts.descriptor(uMsComp, uMsIdx);
      fiEquipmentId            = msDescriptor.eq_id;
      fdMsIndex                = static_cast<double>(msDescriptor.idx);
      fuCurrentMsSysId         = static_cast<unsigned int>(msDescriptor.sys_id);
      const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(uMsComp, uMsIdx));

      uint32_t size = msDescriptor.size;
      //    fulLastMsIdx = msDescriptor.idx;
      if (size > 0) LOG(debug) << "Microslice: " << msDescriptor.idx << " has size: " << size;
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
      fvhMsSzTimePerLink[uMsComp]->Fill((1e-9) * fdMsIndex - fdStartTimeMsSz, size);

      // If not integer number of message in input buffer, print warning/error
      if (0 != (size % kuBytesPerMessage))
        LOG(error) << "The input microslice buffer does NOT "
                   << "contain only complete nDPB messages!";

      // Compute the number of complete messages in the input microslice buffer
      uint32_t uNbMessages = (size - (size % kuBytesPerMessage)) / kuBytesPerMessage;

      // Get the gDPB ID from the MS header
      fuGdpbId = fiEquipmentId;

      /// Check if this gDPB ID was declared in parameter file and stop there if not
      auto it = fGdpbIdIndexMap.find(fuGdpbId);
      if (it == fGdpbIdIndexMap.end()) {
        LOG(info) << "---------------------------------------------------------------";
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
        LOG(warning) << "Could not find the gDPB index for AFCK id 0x" << std::hex << fuGdpbId << std::dec
                     << " in timeslice " << fulCurrentTsIndex << " in microslice " << fdMsIndex << " component "
                     << uMsCompIdx << "\n"
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
        if (0 == uIdx) {
          ProcessEpochCycle(ulData);
          continue;
        }  // if( 0 == uIdx )

        gdpbv100::Message mess(ulData);

        if (fuRawDataPrintMsgIdx < fuRawDataPrintMsgNb || fair::Logger::Logging(fair::Severity::debug2)) {
          mess.printDataCout();
          fuRawDataPrintMsgIdx++;
        }  // if( fuRawDataPrintMsgIdx < fuRawDataPrintMsgNb || fair::Logger::Logging( fair::Severity::debug2 ) )

        // Increment counter for different message types
        // and fill the corresponding histogram
        messageType = mess.getMessageType();
        fviMsgCounter[messageType]++;

        ///         fuGet4Id = mess.getGdpbGenChipId();
        fuGet4Id = ConvertElinkToGet4(mess.getGdpbGenChipId());
        /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
        if (0x90 == fuCurrentMsSysId) fuGet4Id = mess.getGdpbGenChipId();
        fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;

        if (fuNrOfGet4PerGdpb <= fuGet4Id && !mess.isStarTrigger() && (gdpbv100::kuChipIdMergedEpoch != fuGet4Id))
          LOG(warning) << "Message with Get4 ID too high: " << fuGet4Id << " VS " << fuNrOfGet4PerGdpb
                       << " set in parameters.";

        switch (messageType) {
          case gdpbv100::MSG_HIT: {
            if (mess.getGdpbHitIs24b()) { PrintGenInfo(mess); }  // if( getGdpbHitIs24b() )
            else {
              fvmEpSupprBuffer[fuGet4Nr].push_back(mess);
            }  // else of if( getGdpbHitIs24b() )
            break;
          }  // case gdpbv100::MSG_HIT:
          case gdpbv100::MSG_EPOCH: {
            if (gdpbv100::kuChipIdMergedEpoch == fuGet4Id) {
              for (uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGet4Index++) {
                fuGet4Id = uGet4Index;
                fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;
                gdpbv100::Message tmpMess(mess);
                tmpMess.setGdpbGenChipId(uGet4Index);

                FillEpochInfo(tmpMess);
              }  // for( uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGetIndex ++ )

              if (kTRUE == fbPrintAllEpochsEnable) {
                LOG(info) << "Epoch: " << Form("0x%08x ", fuGdpbId) << ", Merg"
                          << ", Link " << std::setw(1) << mess.getGdpbEpLinkId() << ", epoch " << std::setw(8)
                          << mess.getGdpbEpEpochNb() << ", Sync " << std::setw(1) << mess.getGdpbEpSync()
                          << ", Data loss " << std::setw(1) << mess.getGdpbEpDataLoss() << ", Epoch loss "
                          << std::setw(1) << mess.getGdpbEpEpochLoss() << ", Epoch miss " << std::setw(1)
                          << mess.getGdpbEpMissmatch();
              }  // if( kTRUE == fbPrintAllEpochsEnable )
            }    // if this epoch message is a merged one valid for all chips
            else {
              FillEpochInfo(mess);

              if (kTRUE == fbPrintAllEpochsEnable) {
                LOG(info) << "Epoch: " << Form("0x%08x ", fuGdpbId) << ", " << std::setw(4) << fuGet4Nr << ", Link "
                          << std::setw(1) << mess.getGdpbEpLinkId() << ", epoch " << std::setw(8)
                          << mess.getGdpbEpEpochNb() << ", Sync " << std::setw(1) << mess.getGdpbEpSync()
                          << ", Data loss " << std::setw(1) << mess.getGdpbEpDataLoss() << ", Epoch loss "
                          << std::setw(1) << mess.getGdpbEpEpochLoss() << ", Epoch miss " << std::setw(1)
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
            LOG(error) << "Message type " << std::hex << std::setw(2) << static_cast<uint16_t>(messageType)
                       << " not included in Get4 unpacker.";
        }  // switch( mess.getMessageType() )
      }    // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
    }      // for( UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx )

    ///* Pulser monitoring *///
    /// Fill the corresponding histos if the time difference is reasonnable
    /// Loop on Channel A
    for (UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerFee; uChanA++) {
      /// Pulser should lead to single hit in a MS, so ignore if not single
      Bool_t bChanOkFeeA = kFALSE;
      Bool_t bChanOkFeeB = kFALSE;
      if (1 == fvuFeeChanNbHitsLastMs[fuGlobalIdxFeeA][uChanA]) bChanOkFeeA = kTRUE;
      if (1 == fvuFeeChanNbHitsLastMs[fuGlobalIdxFeeB][uChanA]) bChanOkFeeB = kTRUE;

      /// Loop on Channel B
      for (UInt_t uChanB = 0; uChanB < fuNrOfChannelsPerFee; uChanB++) {
        if (bChanOkFeeA) {
          /// Fill FEE A plots
          if (uChanA < uChanB && 1 == fvuFeeChanNbHitsLastMs[fuGlobalIdxFeeA][uChanB]) {
            Double_t dTimeDiff = 1e3
                                 * (fvdFeeChanMsLastPulserHit[fuGlobalIdxFeeA][uChanB]
                                    - fvdFeeChanMsLastPulserHit[fuGlobalIdxFeeA][uChanA]);
            if (TMath::Abs(dTimeDiff) < kdMaxDtPulserPs) fvhTimeDiffPulserFeeA[uChanA][uChanB]->Fill(dTimeDiff);
          }  // if( uChanA < uChanB && 1 == fvuFeeChanNbHitsLastMs[ fuGlobalIdxFeeA ][ uChanB ] )

          /// Fill FEE A VS FEE B plots
          if (1 == fvuFeeChanNbHitsLastMs[fuGlobalIdxFeeB][uChanB]) {
            Double_t dTimeDiff = 1e3
                                 * (fvdFeeChanMsLastPulserHit[fuGlobalIdxFeeB][uChanB]
                                    - fvdFeeChanMsLastPulserHit[fuGlobalIdxFeeA][uChanA]);
            if (TMath::Abs(dTimeDiff) < kdMaxDtPulserPs) fvhTimeDiffPulserFeeFee[uChanA][uChanB]->Fill(dTimeDiff);
          }  // if( 1 == fvuFeeChanNbHitsLastMs[ fuGlobalIdxFeeB ][ uChanB ] )
        }    // if( bChanOkFeeA )

        if (bChanOkFeeB) {
          /// Fill FEE B plots
          if (uChanA < uChanB && 1 == fvuFeeChanNbHitsLastMs[fuGlobalIdxFeeB][uChanB]) {
            Double_t dTimeDiff = 1e3
                                 * (fvdFeeChanMsLastPulserHit[fuGlobalIdxFeeB][uChanB]
                                    - fvdFeeChanMsLastPulserHit[fuGlobalIdxFeeB][uChanA]);
            if (TMath::Abs(dTimeDiff) < kdMaxDtPulserPs) fvhTimeDiffPulserFeeB[uChanA][uChanB]->Fill(dTimeDiff);
          }  // if( uChanA < uChanB && 1 == fvuFeeChanNbHitsLastMs[ fuGlobalIdxFeeB ][ uChanB ] )
        }    // if( bChanOkFeeB )
      }      // for( UInt_t uChanB = 0; uChanB < fuNrOfChannelsPerFee; uChanB++)
    }        // for( UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerFee; uChanA++)

    /// Done with all FEEs and chans, we can reset the hit counters
    for (UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; ++uFeeA)
      for (UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerFee; uChanA++)
        fvuFeeChanNbHitsLastMs[uFeeA][uChanA] = 0;
  }  // for( UInt_t uMsIdx = 0; uMsIdx < uNbMsLoop; uMsIdx ++ )

  // Update RMS plots only every 10s in data
  if (10.0 < dTsStartTime - fdLastRmsUpdateTime) {
    // Reset summary histograms for safety
    fhTimeMeanPulserFeeA->Reset();
    fhTimeRmsPulserFeeA->Reset();

    fhTimeMeanPulserFeeB->Reset();
    fhTimeRmsPulserFeeB->Reset();

    fhTimeMeanPulserFeeFee->Reset();
    fhTimeRmsPulserFeeFee->Reset();

    for (UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerFee; uChanA++)
      for (UInt_t uChanB = 0; uChanB < fuNrOfChannelsPerFee; uChanB++) {
        if (NULL != fvhTimeDiffPulserFeeA[uChanA][uChanB]) {
          fhTimeMeanPulserFeeA->Fill(uChanA, uChanB, fvhTimeDiffPulserFeeA[uChanA][uChanB]->GetMean());
          fhTimeRmsPulserFeeA->Fill(uChanA, uChanB, fvhTimeDiffPulserFeeA[uChanA][uChanB]->GetRMS());
        }  // if( NULL != fvhTimeDiffPulserFeeA[ uChanA ][ uChanB ] )

        if (NULL != fvhTimeDiffPulserFeeB[uChanA][uChanB]) {
          fhTimeMeanPulserFeeB->Fill(uChanA, uChanB, fvhTimeDiffPulserFeeB[uChanA][uChanB]->GetMean());
          fhTimeRmsPulserFeeB->Fill(uChanA, uChanB, fvhTimeDiffPulserFeeB[uChanA][uChanB]->GetRMS());
        }  // if( NULL != fvhTimeDiffPulserFeeB[ uChanA ][ uChanB ] )

        if (NULL != fvhTimeDiffPulserFeeFee[uChanA][uChanB]) {
          fhTimeMeanPulserFeeFee->Fill(uChanA, uChanB, fvhTimeDiffPulserFeeFee[uChanA][uChanB]->GetMean());
          fhTimeRmsPulserFeeFee->Fill(uChanA, uChanB, fvhTimeDiffPulserFeeFee[uChanA][uChanB]->GetRMS());
        }  // if( NULL != fvhTimeDiffPulserFeeFee[ uChanA ][ uChanB ] )
      }    // Loop on both channels
    fdLastRmsUpdateTime = dTsStartTime;
  }  // if( 10.0 < dTsStartTime - fdLastRmsUpdateTime )

  fulCurrentTsIndex++;

  return kTRUE;
}

void CbmMcbm2018TofTestFee::ProcessEpochCycle(uint64_t ulCycleData)
{
  uint64_t ulEpochCycleVal = ulCycleData & gdpbv100::kulEpochCycleFieldSz;

  if (fuRawDataPrintMsgIdx < fuRawDataPrintMsgNb || fair::Logger::Logging(fair::Severity::debug2)) {
    LOG(info) << "CbmMcbm2018TofTestFee::ProcessEpochCyle => "
              //                 << Form( " TS %5llu MS %3lu In data 0x%016llX Cycle 0x%016llX",
              //                           fulCurrentTsIndex, fuCurrentMs, ulCycleData, ulEpochCycleVal );
              << " TS " << FormatDecPrintout(fulCurrentTsIndex, 5) << " MS " << FormatDecPrintout(fuCurrentMs, 3)
              << " In data 0x" << FormatHexPrintout(ulCycleData, 16, '0', true) << " Cycle 0x"
              << FormatHexPrintout(ulEpochCycleVal, 16, '0', true);
    fuRawDataPrintMsgIdx++;
  }  // if( fuRawDataPrintMsgIdx < fuRawDataPrintMsgNb || fair::Logger::Logging( fair::Severity::debug2 ) )

  for (uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGet4Index++) {
    fuGet4Id = uGet4Index;
    fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;
    /*
      if( !( ulEpochCycleVal == fvulCurrentEpochCycle[fuGet4Nr] ||
             ulEpochCycleVal == fvulCurrentEpochCycle[fuGet4Nr] + 1 ) )
         LOG(error) << "CbmMcbm2018TofTestFee::ProcessEpochCyle => "
                    << " Missmatch in epoch cycles detected, probably fake cycles due to epoch index corruption! "
                    << Form( " Current cycle 0x%09X New cycle 0x%09X", fvulCurrentEpochCycle[fuGet4Nr], ulEpochCycleVal );
*/
    fvulCurrentEpochCycle[fuGet4Nr] = ulEpochCycleVal;
  }  // for( uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGet4Index ++ )
  return;
}

void CbmMcbm2018TofTestFee::FillHitInfo(gdpbv100::Message mess)
{
  UInt_t uChannel = mess.getGdpbHitChanId();
  UInt_t uTot     = mess.getGdpbHit32Tot();
  UInt_t uFts     = mess.getGdpbHitFineTs();

  ULong64_t ulCurEpochGdpbGet4 = fvulCurrentEpoch[fuGet4Nr];

  // In Ep. Suppr. Mode, receive following epoch instead of previous
  if (0 < ulCurEpochGdpbGet4) ulCurEpochGdpbGet4--;
  else
    ulCurEpochGdpbGet4 = gdpbv100::kuEpochCounterSz;  // Catch epoch cycle!

  //   UInt_t uChannelNr   = fuGet4Id * fuNrOfChannelsPerGet4 + uChannel;
  UInt_t uChannelNrInFee = (fuGet4Id % fuNrOfGet4PerFee) * fuNrOfChannelsPerGet4 + uChannel;
  UInt_t uFeeNr          = (fuGet4Id / fuNrOfGet4PerFee);
  UInt_t uFeeNrInSys     = fuGdpbNr * fuNrOfFeePerGdpb + uFeeNr;
  //   UInt_t uRemappedChannelNr = uFeeNr * fuNrOfChannelsPerFee + fvuGet4ToPadi[ uChannelNrInFee ];
  UInt_t uRemappedChannelNrInFee = fvuGet4ToPadi[uChannelNrInFee];
  /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
  if (0x90 == fuCurrentMsSysId) {
    //      uRemappedChannelNr = uChannelNr;
    uRemappedChannelNrInFee = uChannelNrInFee;
  }  // if( 0x90 == fuCurrentMsSysId )
     //   UInt_t uGbtxNr      = (uFeeNr / kuNbFeePerGbtx);
     //   UInt_t uFeeInGbtx  = (uFeeNr % kuNbFeePerGbtx);
     //   UInt_t uGbtxNrInSys = fuGdpbNr * kuNbGbtxPerGdpb + uGbtxNr;

  ULong_t ulHitTime = mess.getMsgFullTime(ulCurEpochGdpbGet4);
  Double_t dHitTime = mess.getMsgFullTimeD(ulCurEpochGdpbGet4);

  // In 32b mode the coarse counter is already computed back to 112 FTS bins
  // => need to hide its contribution from the Finetime
  // => FTS = Fullt TS modulo 112
  uFts = mess.getGdpbHitFullTs() % 112;

  // In Run rate evolution
  if (fdStartTime < 0) fdStartTime = dHitTime;

  // Reset the evolution Histogram and the start time when we reach the end of the range
  if (fuHistoryHistoSize < 1e-9 * (dHitTime - fdStartTime)) {
    ResetEvolutionHistograms();
    fdStartTime = dHitTime;
  }  // if( fuHistoryHistoSize < 1e-9 * (dHitTime - fdStartTime) )

  // In Run rate evolution
  if (fdStartTimeLong < 0) fdStartTimeLong = dHitTime;

  ///* Pulser monitoring *///
  /// Save last hist time
  if (fuGlobalIdxFeeA == uFeeNrInSys) {
    fhChanTotFeeA->Fill(uRemappedChannelNrInFee, uTot);
    fhChanPulseIntervalFeeA->Fill(dHitTime - fvdFeeChanMsLastPulserHit[uFeeNrInSys][uRemappedChannelNrInFee],
                                  uRemappedChannelNrInFee);
  }  // if( fuGlobalIdxFeeA == uFeeNrInSys )
  else if (fuGlobalIdxFeeB == uFeeNrInSys) {
    fhChanTotFeeB->Fill(uRemappedChannelNrInFee, uTot);
    fhChanPulseIntervalFeeB->Fill(dHitTime - fvdFeeChanMsLastPulserHit[uFeeNrInSys][uRemappedChannelNrInFee],
                                  uRemappedChannelNrInFee);
  }  // if( fuGlobalIdxFeeB == uFeeNrInSys )
  fvdFeeChanMsLastPulserHit[uFeeNrInSys][uRemappedChannelNrInFee] = dHitTime;
  fvuFeeChanNbHitsLastMs[uFeeNrInSys][uRemappedChannelNrInFee]++;

  if (0 <= fdStartTime) fvhPulserCountEvoPerFeeGdpb[fuGdpbNr]->Fill(1e-9 * (dHitTime - fdStartTime), uFeeNr);

  if (kTRUE == fbPrintAllHitsEnable) {
    LOG(info) << "Hit: " << Form("0x%08x ", fuGdpbId) << ", " << std::setw(2) << fuGet4Nr << ", " << std::setw(3)
              << uChannel << ", " << std::setw(3) << uTot << ", epoch " << std::setw(3) << ulCurEpochGdpbGet4
              << ", FullTime Clk " << Form("%12lu ", ulHitTime) << ", FullTime s " << Form("%12.9f ", dHitTime / 1e9)
              << ", FineTime " << uFts;
  }  // if( kTRUE == fbPrintAllHitsEnable )
}

void CbmMcbm2018TofTestFee::FillEpochInfo(gdpbv100::Message mess)
{
  ULong64_t ulEpochNr = mess.getGdpbEpEpochNb();
  /*
   if( fvulCurrentEpoch[fuGet4Nr] < ulEpochNr )
      fvulCurrentEpochCycle[fuGet4Nr]++;
*/
  fvulCurrentEpoch[fuGet4Nr]     = ulEpochNr;
  fvulCurrentEpochFull[fuGet4Nr] = ulEpochNr + gdpbv100::kulEpochCycleBins * fvulCurrentEpochCycle[fuGet4Nr];

  fulCurrentEpochTime = mess.getMsgFullTime(ulEpochNr);

  /// Re-align the epoch number of the message in case it will be used later:
  /// We received the epoch after the data instead of the one before!
  if (0 < ulEpochNr) mess.setGdpbEpEpochNb(ulEpochNr - 1);
  else
    mess.setGdpbEpEpochNb(gdpbv100::kuEpochCounterSz);

  Int_t iBufferSize = fvmEpSupprBuffer[fuGet4Nr].size();
  if (0 < iBufferSize) {
    LOG(debug) << "Now processing stored messages for for get4 " << fuGet4Nr << " with epoch number "
               << (fvulCurrentEpoch[fuGet4Nr] - 1);

    /// Data are sorted between epochs, not inside => Epoch level ordering
    /// Sorting at lower bin precision level
    std::stable_sort(fvmEpSupprBuffer[fuGet4Nr].begin(), fvmEpSupprBuffer[fuGet4Nr].begin());

    for (Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++) {
      FillHitInfo(fvmEpSupprBuffer[fuGet4Nr][iMsgIdx]);
    }  // for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )

    fvmEpSupprBuffer[fuGet4Nr].clear();
  }  // if( 0 < fvmEpSupprBuffer[fuGet4Nr] )
}

void CbmMcbm2018TofTestFee::PrintSlcInfo(gdpbv100::Message /*mess*/)
{
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

      /// Printout if SPI message!

   }
*/
}

void CbmMcbm2018TofTestFee::PrintGenInfo(gdpbv100::Message mess)
{
  Int_t mType    = mess.getMessageType();
  Int_t channel  = mess.getGdpbHitChanId();
  uint64_t uData = mess.getData();

  LOG(debug) << "Get4 MSG type " << mType << " from gdpbId " << fuGdpbId << ", getId " << fuGet4Id << ", (hit channel) "
             << channel << " data " << std::hex << uData;
}

void CbmMcbm2018TofTestFee::PrintSysInfo(gdpbv100::Message mess)
{
  if (fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find(fuGdpbId))
    LOG(debug) << "GET4 System message,       epoch " << static_cast<Int_t>(fvulCurrentEpoch[fuGet4Nr]) << ", time "
               << std::setprecision(9) << std::fixed << Double_t(fulCurrentEpochTime) * 1.e-9 << " s "
               << " for board ID " << std::hex << std::setw(4) << fuGdpbId << std::dec;

  switch (mess.getGdpbSysSubType()) {
    case gdpbv100::SYS_GET4_ERROR: {
      uint32_t uData = mess.getGdpbSysErrData();
      if (gdpbv100::GET4_V2X_ERR_TOT_OVERWRT == uData || gdpbv100::GET4_V2X_ERR_TOT_RANGE == uData
          || gdpbv100::GET4_V2X_ERR_EVT_DISCARD == uData || gdpbv100::GET4_V2X_ERR_ADD_RIS_EDG == uData
          || gdpbv100::GET4_V2X_ERR_UNPAIR_FALL == uData || gdpbv100::GET4_V2X_ERR_SEQUENCE_ER == uData)
        LOG(debug) << " +++++++ > gDPB: " << std::hex << std::setw(4) << fuGdpbId << std::dec
                   << ", Chip = " << std::setw(2) << mess.getGdpbGenChipId() << ", Chan = " << std::setw(1)
                   << mess.getGdpbSysErrChanId() << ", Edge = " << std::setw(1) << mess.getGdpbSysErrEdge()
                   << ", Empt = " << std::setw(1) << mess.getGdpbSysErrUnused() << ", Data = " << std::hex
                   << std::setw(2) << uData << std::dec << " -- GET4 V1 Error Event";
      else
        LOG(debug) << " +++++++ >gDPB: " << std::hex << std::setw(4) << fuGdpbId << std::dec
                   << ", Chip = " << std::setw(2) << mess.getGdpbGenChipId() << ", Chan = " << std::setw(1)
                   << mess.getGdpbSysErrChanId() << ", Edge = " << std::setw(1) << mess.getGdpbSysErrEdge()
                   << ", Empt = " << std::setw(1) << mess.getGdpbSysErrUnused() << ", Data = " << std::hex
                   << std::setw(2) << uData << std::dec << " -- GET4 V1 Error Event ";
      break;
    }  // case gdpbv100::SYSMSG_GET4_EVENT
    case gdpbv100::SYS_GDPB_UNKWN: {
      LOG(debug) << "Unknown GET4 message, data: " << std::hex << std::setw(8) << mess.getGdpbSysUnkwData() << std::dec
                 << " Full message: " << std::hex << std::setw(16) << mess.getData() << std::dec;
      break;
    }  // case gdpbv100::SYS_GDPB_UNKWN:
    case gdpbv100::SYS_GET4_SYNC_MISS: {
      if (mess.getGdpbSysFwErrResync())
        LOG(info) << Form("GET4 Resynchronization: Get4:0x%04x ", mess.getGdpbGenChipId()) << std::hex << std::setw(4)
                  << fuGdpbId << std::dec;
      else
        LOG(info) << "GET4 synchronization pulse missing in gDPB " << std::hex << std::setw(4) << fuGdpbId << std::dec;
      break;
    }  // case gdpbv100::SYS_GET4_SYNC_MISS:
    case gdpbv100::SYS_PATTERN: {
      LOG(debug) << "ASIC pattern for missmatch, disable or resync";
      break;
    }  // case gdpbv100::SYS_PATTERN:
    default: {
      LOG(debug) << "Crazy system message, subtype " << mess.getGdpbSysSubType();
      break;
    }  // default

  }  // switch( getGdpbSysSubType() )
}

void CbmMcbm2018TofTestFee::FillPattInfo(gdpbv100::Message /*mess*/)
{
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
               if( 0x90 == fuCurrentMsSysId )
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
               if( 0x90 == fuCurrentMsSysId )
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
               if( 0x90 == fuCurrentMsSysId )
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

void CbmMcbm2018TofTestFee::FillStarTrigInfo(gdpbv100::Message mess)
{
  Int_t iMsgIndex = mess.getStarTrigMsgIndex();

  switch (iMsgIndex) {
    case 0: fvulGdpbTsMsb[fuGdpbNr] = mess.getGdpbTsMsbStarA(); break;
    case 1:
      fvulGdpbTsLsb[fuGdpbNr] = mess.getGdpbTsLsbStarB();
      fvulStarTsMsb[fuGdpbNr] = mess.getStarTsMsbStarB();
      break;
    case 2: fvulStarTsMid[fuGdpbNr] = mess.getStarTsMidStarC(); break;
    case 3: {

      ULong64_t ulNewGdpbTsFull = (fvulGdpbTsMsb[fuGdpbNr] << 24) + (fvulGdpbTsLsb[fuGdpbNr]);
      ULong64_t ulNewStarTsFull =
        (fvulStarTsMsb[fuGdpbNr] << 48) + (fvulStarTsMid[fuGdpbNr] << 8) + mess.getStarTsLsbStarD();
      UInt_t uNewToken   = mess.getStarTokenStarD();
      UInt_t uNewDaqCmd  = mess.getStarDaqCmdStarD();
      UInt_t uNewTrigCmd = mess.getStarTrigCmdStarD();

      if ((uNewToken == fvuStarTokenLast[fuGdpbNr]) && (ulNewGdpbTsFull == fvulGdpbTsFullLast[fuGdpbNr])
          && (ulNewStarTsFull == fvulStarTsFullLast[fuGdpbNr]) && (uNewDaqCmd == fvuStarDaqCmdLast[fuGdpbNr])
          && (uNewTrigCmd == fvuStarTrigCmdLast[fuGdpbNr])) {
        UInt_t uTrigWord = ((fvuStarTrigCmdLast[fuGdpbNr] & 0x00F) << 16)
                           + ((fvuStarDaqCmdLast[fuGdpbNr] & 0x00F) << 12) + ((fvuStarTokenLast[fuGdpbNr] & 0xFFF));
        LOG(warning) << "Possible error: identical STAR tokens found twice in "
                        "a row => ignore 2nd! "
                     << " TS " << fulCurrentTsIndex << " gDBB #" << fuGdpbNr << " "
                     << Form("token = %5u ", fvuStarTokenLast[fuGdpbNr])
                     << Form("gDPB ts  = %12llu ", fvulGdpbTsFullLast[fuGdpbNr])
                     << Form("STAR ts = %12llu ", fvulStarTsFullLast[fuGdpbNr])
                     << Form("DAQ cmd = %2u ", fvuStarDaqCmdLast[fuGdpbNr])
                     << Form("TRG cmd = %2u ", fvuStarTrigCmdLast[fuGdpbNr]) << Form("TRG Wrd = %5x ", uTrigWord);
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
        LOG(debug) << "Probable reset of the STAR TS: old = " << Form("%16llu", fvulStarTsFullLast[fuGdpbNr])
                   << " new = " << Form("%16llu", ulNewStarTsFull) << " Diff = -"
                   << Form("%8llu", fvulStarTsFullLast[fuGdpbNr] - ulNewStarTsFull);

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

void CbmMcbm2018TofTestFee::Reset() {}

void CbmMcbm2018TofTestFee::Finish()
{
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

  /*
   LOG(info) << "-------------------------------------";
   for (UInt_t i = 0; i < fuNrOfGdpbs; ++i)
   {
      for (UInt_t j = 0; j < fuNrOfGet4PerGdpb; ++j)
      {
         LOG(info) << "Last epoch for gDPB: " << std::hex << std::setw(4) << i
                   << std::dec << " , GET4  " << std::setw(4) << j << " => "
                   << fvulCurrentEpoch[GetArrayIndex(i, j)];
      } // for (UInt_t j = 0; j < fuNrOfGet4PerGdpb; ++j)
   } // for (UInt_t i = 0; i < fuNrOfGdpbs; ++i)
   LOG(info) << "-------------------------------------";
*/

  /// Update RMS plots
  /// Reset summary histograms for safety
  fhTimeMeanPulserFeeA->Reset();
  fhTimeRmsPulserFeeA->Reset();

  fhTimeMeanPulserFeeB->Reset();
  fhTimeRmsPulserFeeB->Reset();

  fhTimeMeanPulserFeeFee->Reset();
  fhTimeRmsPulserFeeFee->Reset();

  for (UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerFee; uChanA++)
    for (UInt_t uChanB = 0; uChanB < fuNrOfChannelsPerFee; uChanB++) {
      if (NULL != fvhTimeDiffPulserFeeA[uChanA][uChanB]) {
        fhTimeMeanPulserFeeA->Fill(uChanA, uChanB, fvhTimeDiffPulserFeeA[uChanA][uChanB]->GetMean());
        fhTimeRmsPulserFeeA->Fill(uChanA, uChanB, fvhTimeDiffPulserFeeA[uChanA][uChanB]->GetRMS());
      }  // if( NULL != fvhTimeDiffPulserFeeA[ uChanA ][ uChanB ] )

      if (NULL != fvhTimeDiffPulserFeeB[uChanA][uChanB]) {
        fhTimeMeanPulserFeeB->Fill(uChanA, uChanB, fvhTimeDiffPulserFeeB[uChanA][uChanB]->GetMean());
        fhTimeRmsPulserFeeB->Fill(uChanA, uChanB, fvhTimeDiffPulserFeeB[uChanA][uChanB]->GetRMS());
      }  // if( NULL != fvhTimeDiffPulserFeeB[ uChanA ][ uChanB ] )

      if (NULL != fvhTimeDiffPulserFeeFee[uChanA][uChanB]) {
        fhTimeMeanPulserFeeFee->Fill(uChanA, uChanB, fvhTimeDiffPulserFeeFee[uChanA][uChanB]->GetMean());
        fhTimeRmsPulserFeeFee->Fill(uChanA, uChanB, fvhTimeDiffPulserFeeFee[uChanA][uChanB]->GetRMS());
      }  // if( NULL != fvhTimeDiffPulserFeeFee[ uChanA ][ uChanB ] )
    }    // Loop on both channels

  /// Update zoomed RMS and pulser fit plots
  UpdateZoomedFit(fvhTimeDiffPulserFeeA, fhTimeRmsZoomFitPulsFeeA, fhTimeResFitPulsFeeA);
  UpdateZoomedFit(fvhTimeDiffPulserFeeB, fhTimeRmsZoomFitPulsFeeB, fhTimeResFitPulsFeeB);
  UpdateZoomedFit(fvhTimeDiffPulserFeeFee, fhTimeRmsZoomFitPulsFeeFee, fhTimeResFitPulsFeeFee);

  SaveAllHistos(fsHistoFileFullname);
  //   SaveAllHistos();
}

void CbmMcbm2018TofTestFee::SaveAllHistos(TString sFileName)
{
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* histoFile = NULL;
  if ("" != sFileName) {
    // open separate histo file in recreate mode
    histoFile = new TFile(sFileName, "RECREATE");
    histoFile->cd();
  }  // if( "" != sFileName )

  gDirectory->mkdir("TofDt");
  gDirectory->cd("TofDt");
  fhTimeMeanPulserFeeA->Write();
  fhTimeRmsPulserFeeA->Write();
  fhTimeRmsZoomFitPulsFeeA->Write();
  fhTimeResFitPulsFeeA->Write();

  fhTimeMeanPulserFeeB->Write();
  fhTimeRmsPulserFeeB->Write();
  fhTimeRmsZoomFitPulsFeeB->Write();
  fhTimeResFitPulsFeeB->Write();

  fhTimeMeanPulserFeeFee->Write();
  fhTimeRmsPulserFeeFee->Write();
  fhTimeRmsZoomFitPulsFeeFee->Write();
  fhTimeResFitPulsFeeFee->Write();

  gDirectory->cd("..");


  ///* Pulser monitoring *///
  gDirectory->mkdir("TofDtFeeA");
  gDirectory->cd("TofDtFeeA");
  for (UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerFee; uChanA++)
    for (UInt_t uChanB = 0; uChanB < fuNrOfChannelsPerFee; uChanB++)
      if (NULL != fvhTimeDiffPulserFeeA[uChanA][uChanB]) fvhTimeDiffPulserFeeA[uChanA][uChanB]->Write();
  gDirectory->cd("..");

  gDirectory->mkdir("TofDtFeeB");
  gDirectory->cd("TofDtFeeB");
  for (UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerFee; uChanA++)
    for (UInt_t uChanB = 0; uChanB < fuNrOfChannelsPerFee; uChanB++)
      if (NULL != fvhTimeDiffPulserFeeB[uChanA][uChanB]) fvhTimeDiffPulserFeeB[uChanA][uChanB]->Write();
  gDirectory->cd("..");

  gDirectory->mkdir("TofDtFeeFee");
  gDirectory->cd("TofDtFeeFee");
  for (UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerFee; uChanA++)
    for (UInt_t uChanB = 0; uChanB < fuNrOfChannelsPerFee; uChanB++)
      if (NULL != fvhTimeDiffPulserFeeFee[uChanA][uChanB]) fvhTimeDiffPulserFeeFee[uChanA][uChanB]->Write();
  gDirectory->cd("..");

  gDirectory->mkdir("TofPulse");
  gDirectory->cd("TofPulse");
  fhChanTotFeeA->Write();
  fhChanTotFeeB->Write();
  fhChanPulseIntervalFeeA->Write();
  fhChanPulseIntervalFeeB->Write();
  gDirectory->cd("..");

  gDirectory->mkdir("TofCnt");
  gDirectory->cd("TofCnt");
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb)
    fvhPulserCountEvoPerFeeGdpb[uGdpb]->Write();
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

  TProfile* pMissedTsEvoP = dynamic_cast<TProfile*>(gROOT->FindObjectAny("Missed_TS_Evo"));
  if (NULL != pMissedTsEvoP) pMissedTsEvoP->Write();

  gDirectory->cd("..");

  fcPulserFeeA->Write();
  fcPulserFeeB->Write();
  fcPulserFeeFee->Write();
  fcPulseProp->Write();


  if ("" != sFileName) {
    // Restore original directory position
    histoFile->Close();
  }  // if( "" != sFileName )

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

void CbmMcbm2018TofTestFee::ResetAllHistos()
{
  LOG(info) << "Reseting all TOF histograms.";

  ///* Pulser monitoring *///
  for (UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerFee; uChanA++)
    for (UInt_t uChanB = 0; uChanB < fuNrOfChannelsPerFee; uChanB++)
      if (NULL != fvhTimeDiffPulserFeeA[uChanA][uChanB]) fvhTimeDiffPulserFeeA[uChanA][uChanB]->Reset();

  for (UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerFee; uChanA++)
    for (UInt_t uChanB = 0; uChanB < fuNrOfChannelsPerFee; uChanB++)
      if (NULL != fvhTimeDiffPulserFeeB[uChanA][uChanB]) fvhTimeDiffPulserFeeB[uChanA][uChanB]->Reset();

  for (UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerFee; uChanA++)
    for (UInt_t uChanB = 0; uChanB < fuNrOfChannelsPerFee; uChanB++)
      if (NULL != fvhTimeDiffPulserFeeFee[uChanA][uChanB]) fvhTimeDiffPulserFeeFee[uChanA][uChanB]->Reset();

  fhTimeMeanPulserFeeA->Reset();
  fhTimeRmsPulserFeeA->Reset();
  fhTimeRmsZoomFitPulsFeeA->Reset();
  fhTimeResFitPulsFeeA->Reset();

  fhTimeMeanPulserFeeB->Reset();
  fhTimeRmsPulserFeeB->Reset();
  fhTimeRmsZoomFitPulsFeeB->Reset();
  fhTimeResFitPulsFeeB->Reset();

  fhTimeMeanPulserFeeFee->Reset();
  fhTimeRmsPulserFeeFee->Reset();
  fhTimeRmsZoomFitPulsFeeFee->Reset();
  fhTimeResFitPulsFeeFee->Reset();

  fhChanTotFeeA->Reset();
  fhChanTotFeeB->Reset();
  fhChanPulseIntervalFeeA->Reset();
  fhChanPulseIntervalFeeB->Reset();

  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb)
    fvhPulserCountEvoPerFeeGdpb[uGdpb]->Reset();

  for (UInt_t uLinks = 0; uLinks < fvhMsSzPerLink.size(); uLinks++) {
    if (NULL == fvhMsSzPerLink[uLinks]) continue;

    fvhMsSzPerLink[uLinks]->Reset();
    fvhMsSzTimePerLink[uLinks]->Reset();
  }  // for( UInt_t uLinks = 0; uLinks < fvhMsSzPerLink.size(); uLinks++ )

  fdStartTime     = -1;
  fdStartTimeMsSz = -1;
}
void CbmMcbm2018TofTestFee::ResetEvolutionHistograms() { fdStartTime = -1; }

void CbmMcbm2018TofTestFee::UpdateZoomedFit(std::vector<std::vector<TH1*>> phTimeDiff, TH2* phTimeRmsZoom,
                                            TH2* phTimeResFit)
{
  // Only do something if the user defined the width he wants for the zoom
  if (0.0 < fdFitZoomWidthPs) {
    // Reset summary histograms for safety
    phTimeRmsZoom->Reset();
    phTimeResFit->Reset();

    Double_t dRes = 0;
    TF1* fitFuncPairs[fuNrOfChannelsPerFee][fuNrOfChannelsPerFee];

    for (UInt_t uChanA = 0; uChanA < fuNrOfChannelsPerFee; uChanA++)
      for (UInt_t uChanB = 0; uChanB < fuNrOfChannelsPerFee; uChanB++)
        if (NULL != phTimeDiff[uChanA][uChanB]) {
          // Check that we have at least 1 entry
          if (0 == phTimeDiff[uChanA][uChanB]->GetEntries()) {
            phTimeRmsZoom->Fill(uChanA, uChanB, 0.0);
            phTimeResFit->Fill(uChanA, uChanB, 0.0);
            LOG(debug) << "CbmMcbm2018TofTestFee::UpdateZoomedFit => Empty input "
                       << "for Chan pair " << uChanA << " and " << uChanB << " !!! ";
            continue;
          }  // if( 0 == phTimeDiff[ uChanA ][ uChanB ]->GetEntries() )

          // Read the peak position (bin with max counts) + total nb of entries
          Int_t iBinWithMax  = phTimeDiff[uChanA][uChanB]->GetMaximumBin();
          Double_t dNbCounts = phTimeDiff[uChanA][uChanB]->Integral();

          // Zoom the X axis to +/- ZoomWidth around the peak position
          Double_t dPeakPos = phTimeDiff[uChanA][uChanB]->GetXaxis()->GetBinCenter(iBinWithMax);
          phTimeDiff[uChanA][uChanB]->GetXaxis()->SetRangeUser(dPeakPos - fdFitZoomWidthPs,
                                                               dPeakPos + fdFitZoomWidthPs);

          // Read integral and check how much we lost due to the zoom (% loss allowed)
          Double_t dZoomCounts = phTimeDiff[uChanA][uChanB]->Integral();
          if ((dZoomCounts / dNbCounts) < 0.99) {
            phTimeRmsZoom->Fill(uChanA, uChanB, 0.0);
            phTimeResFit->Fill(uChanA, uChanB, 0.0);
            LOG(warning) << "CbmMcbm2018TofTestFee::UpdateZoomedFit => Zoom too strong, "
                         << "more than 1% loss for Chan pair " << uChanA << " and " << uChanB << " !!! ";
            continue;
          }  // if( ( dZoomCounts / dNbCounts ) < 0.99 )

          // Fill new RMS after zoom into summary histo
          phTimeRmsZoom->Fill(uChanA, uChanB, phTimeDiff[uChanA][uChanB]->GetRMS());


          // Fit using zoomed boundaries + starting gaussian width, store into summary histo
          dRes                         = 0;
          fitFuncPairs[uChanA][uChanB] = new TF1(Form("fPair_%02d_%02d", uChanA, uChanB), "gaus",
                                                 dPeakPos - fdFitZoomWidthPs, dPeakPos + fdFitZoomWidthPs);
          // Fix the Mean fit value around the Histogram Mean
          fitFuncPairs[uChanA][uChanB]->SetParameter(0, dZoomCounts);
          fitFuncPairs[uChanA][uChanB]->SetParameter(1, dPeakPos);
          fitFuncPairs[uChanA][uChanB]->SetParameter(2, 200.0);  // Hardcode start with ~4*BinWidth, do better later
          // Using integral instead of bin center seems to lead to unrealistic values => no "I"
          phTimeDiff[uChanA][uChanB]->Fit(Form("fPair_%02d_%02d", uChanA, uChanB), "QRM0");
          // Get Sigma
          dRes = fitFuncPairs[uChanA][uChanB]->GetParameter(2);
          // Cleanup memory
          delete fitFuncPairs[uChanA][uChanB];
          // Fill summary
          phTimeResFit->Fill(uChanA, uChanB, dRes / TMath::Sqrt2());


          LOG(debug) << "CbmMcbm2018TofTestFee::UpdateZoomedFit => "
                     << "For chan pair " << uChanA << " and " << uChanB
                     << " we have zoomed RMS = " << phTimeDiff[uChanA][uChanB]->GetRMS() << " and a resolution of "
                     << dRes / TMath::Sqrt2();

          // Restore original axis state?
          phTimeDiff[uChanA][uChanB]->GetXaxis()->UnZoom();
        }  // loop on chanA and chanB + check if corresponding fvhTimeDiffPulser exists
  }        // if( 0.0 < fdFitZoomWidthPs )
  else {
    LOG(error) << "CbmMcbm2018TofTestFee::UpdateZoomedFit => Zoom width not defined, "
               << "please use SetFitZoomWidthPs, e.g. in macro, before trying this "
                  "update !!!";
  }  // else of if( 0.0 < fdFitZoomWidthPs )
}

ClassImp(CbmMcbm2018TofTestFee)
