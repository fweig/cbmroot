// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmStar2019EventBuilderEtofAlgo                  -----
// -----               Created 03.11.2018 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmStar2019EventBuilderEtofAlgo.h"

#include "CbmHistManager.h"
#include "CbmStar2019TofPar.h"

#include <Logger.h>
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TList.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TString.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdint.h>

// -------------------------------------------------------------------------
CbmStar2019EventBuilderEtofAlgo::CbmStar2019EventBuilderEtofAlgo()
  : CbmStar2019Algo()
  ,
  /// From the class itself
  fbMonitorMode(kFALSE)
  , fbDebugMonitorMode(kFALSE)
  , fbStoreLostEventMsg(kFALSE)
  , fbAddStatusToEvent(kTRUE)
  , fUnpackPar(nullptr)
  , fuNrOfGdpbs(0)
  , fGdpbIdIndexMap()
  , fuNrOfFeePerGdpb(0)
  , fuNrOfGet4PerFee(0)
  , fuNrOfChannelsPerGet4(0)
  , fuNrOfChannelsPerFee(0)
  , fuNrOfGet4(0)
  , fuNrOfGet4PerGdpb(0)
  , fuNrOfChannelsPerGdpb(0)
  , fuNrOfGbtx(0)
  , fuNrOfModules(0)
  , fviNrOfRpc()
  , fviRpcType()
  , fviRpcSide()
  , fviModuleId()
  , fdAllowedTriggersSpread(-1.0)
  , fdStarTriggerDeadtime()
  , fdStarTriggerDelay()
  , fdStarTriggerWinSize()
  , fuMinTotPulser(90)
  , fuMaxTotPulser(110)
  , fulCurrentTsIndex(0)
  , fdTsStartTime(-1.0)
  , fdTsStopTimeCore(-1.0)
  , fuCurrentMs(0)
  , fdMsTime(-1.0)
  , fuMsIndex(0)
  , fuGdpbId(0)
  , fuGdpbNr(0)
  , fuGet4Id(0)
  , fuGet4Nr(0)
  , fiEquipmentId(0)
  , fvulCurrentEpoch()
  , fvulCurrentEpochCycle()
  , fvulCurrentEpochFull()
  , fvvmEpSupprBuffer()
  , fvvBufferMajorAsicErrors()
  , fvvBufferMessages()
  , fvvBufferTriggers()
  , fbTriggerFoundA(kFALSE)
  , fbTriggerFoundB(kFALSE)
  , fbTriggerFoundC(kFALSE)
  , fvulGdpbTsMsb()
  , fvulGdpbTsLsb()
  , fvulStarTsMsb()
  , fvulStarTsMid()
  , fvulGdpbTsFullLast()
  , fvulStarTsFullLast()
  , fvuStarTokenLast()
  , fvuStarDaqCmdLast()
  , fvuStarTrigCmdLast()
  , fvulGdpbTsFullLastCore()
  , fvulStarTsFullLastCore()
  , fvuStarTokenLastCore()
  , fvuStarDaqCmdLastCore()
  , fvuStarTrigCmdLastCore()
  , fvdMessCandidateTimeStart()
  , fvdMessCandidateTimeStop()
  , fvdTrigCandidateTimeStart()
  , fvdTrigCandidateTimeStop()
  , fbEpochAfterCandWinFound(kFALSE)
  , fbTriggerAfterCandWinFound(kFALSE)
  , fvbGdpbLastMissmatchPattern()
  , fvbGdpbLastEnablePattern()
  , fvbGdpbLastResyncPattern()
  , fvSectorStatusPattern()
  , fvhHitsTimeToTriggerRaw()
  , fvhHitsTimeToTriggerRawPulser()
  , fvhHitsTimeToTriggerSel()
  , fvhHitsTimeToTriggerSelVsDaq()
  , fvhHitsTimeToTriggerSelVsTrig()
  , fvhTriggerDt()
  , fvhTriggerDistributionInTs()
  , fvhTriggerDistributionInMs()
  , fvhMessDistributionInMs()
  , fhEventSizeDistribution(nullptr)
  , fhEventSizeEvolution(nullptr)
  , fhEventNbEvolution(nullptr)
  , fhEventNbDistributionInTs(nullptr)
  , fhEventSizeDistributionInTs(nullptr)
  , fhRawTriggersStats(nullptr)
  , fhRawTriggersStatsCore(nullptr)
  , fhRawTriggersStatsOver(nullptr)
  , fhRawTriggersStatsSel(nullptr)
  , fhMissingTriggersEvolution(nullptr)
  , fcTimeToTrigRaw(nullptr)
  , fcTimeToTrigSel(nullptr)
  , fcTrigDistMs(nullptr)
  , fcMessDistMs(nullptr)
  , fcEventBuildStats(nullptr)
  , fcTriggerStats(nullptr) {}
CbmStar2019EventBuilderEtofAlgo::~CbmStar2019EventBuilderEtofAlgo() {
  /// Clear buffers
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fvvmEpSupprBuffer[uGdpb].clear();
    fvvBufferMajorAsicErrors[uGdpb].clear();
    fvvBufferMessages[uGdpb].clear();
    fvvBufferTriggers[uGdpb].clear();
  }  // for (Int_t iGdpb = 0; iGdpb < fuNrOfGdpbs; ++iGdpb)
}

// -------------------------------------------------------------------------
Bool_t CbmStar2019EventBuilderEtofAlgo::Init() {
  LOG(info) << "Initializing STAR eTOF 2019 event builder algo";

  return kTRUE;
}
void CbmStar2019EventBuilderEtofAlgo::Reset() {}
void CbmStar2019EventBuilderEtofAlgo::Finish() {
  /// Printout Goodbye message and stats

  /// Write Output histos
}

// -------------------------------------------------------------------------
Bool_t CbmStar2019EventBuilderEtofAlgo::InitContainers() {
  LOG(info) << "Init parameter containers for CbmStar2019EventBuilderEtofAlgo";
  Bool_t initOK = ReInitContainers();

  return initOK;
}
Bool_t CbmStar2019EventBuilderEtofAlgo::ReInitContainers() {
  LOG(info) << "**********************************************";
  LOG(info)
    << "ReInit parameter containers for CbmStar2019EventBuilderEtofAlgo";

  fUnpackPar = (CbmStar2019TofPar*) fParCList->FindObject("CbmStar2019TofPar");
  if (nullptr == fUnpackPar) return kFALSE;

  Bool_t initOK = InitParameters();

  return initOK;
}
TList* CbmStar2019EventBuilderEtofAlgo::GetParList() {
  if (nullptr == fParCList) fParCList = new TList();
  fUnpackPar = new CbmStar2019TofPar("CbmStar2019TofPar");
  fParCList->Add(fUnpackPar);

  return fParCList;
}
Bool_t CbmStar2019EventBuilderEtofAlgo::InitParameters() {
  /// Control flags => Controlled from the calling task
  //   fbMonitorMode = fUnpackPar->GetMonitorMode();
  LOG(info) << "Monitor mode:       " << (fbMonitorMode ? "ON" : "OFF");

  //   fbDebugMonitorMode = fUnpackPar->GetDebugMonitorMode();
  LOG(info) << "Debug Monitor mode: " << (fbDebugMonitorMode ? "ON" : "OFF");

  LOG(info) << "Store Lost Event Msg in event: "
            << (fbStoreLostEventMsg ? "ON" : "OFF");

  LOG(info) << "Add status pattern to event: "
            << (fbAddStatusToEvent ? "ON" : "OFF");

  /// Parameters for variables dimensionning
  fuNrOfGdpbs = fUnpackPar->GetNrOfGdpbs();
  LOG(info) << "Nr. of Tof GDPBs: " << fuNrOfGdpbs;

  fuNrOfFeePerGdpb = fUnpackPar->GetNrOfFeePerGdpb();
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
    LOG(info) << "GDPB Id of TOF  " << Form("%02d", i) << " : " << std::hex
              << Form("0x%04x", fUnpackPar->GetGdpbId(i)) << std::dec;
  }  // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )

  fuNrOfGbtx = fUnpackPar->GetNrOfGbtx();
  LOG(info) << "Nr. of GBTx: " << fuNrOfGbtx;

  fuNrOfModules = fUnpackPar->GetNrOfModules();
  LOG(info) << "Nr. of GBTx: " << fuNrOfModules;

  /// Parameters for detector mapping
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

  /// Parameters for FLES containers processing
  fdMsSizeInNs = fUnpackPar->GetSizeMsInNs();
  LOG(info) << "Timeslice parameters: each MS is " << fdMsSizeInNs << " ns";

  /// Parameters for STAR trigger & event building
  fdAllowedTriggersSpread = fUnpackPar->GetStarTriggAllowedSpread();
  fdStarTriggerDeadtime.resize(fuNrOfGdpbs, 0.0);
  fdStarTriggerDelay.resize(fuNrOfGdpbs, 0.0);
  fdStarTriggerWinSize.resize(fuNrOfGdpbs, 0.0);
  fvdMessCandidateTimeStart.resize(fuNrOfGdpbs, 0.0);
  fvdMessCandidateTimeStop.resize(fuNrOfGdpbs, 0.0);
  fvdTrigCandidateTimeStart.resize(fuNrOfGdpbs, 0.0);
  fvdTrigCandidateTimeStop.resize(fuNrOfGdpbs, 0.0);
  fvbGdpbLastMissmatchPattern.resize(fuNrOfGdpbs, 0);
  fvbGdpbLastEnablePattern.resize(fuNrOfGdpbs, 0);
  fvbGdpbLastResyncPattern.resize(fuNrOfGdpbs, 0);
  fvSectorStatusPattern.resize(fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fdStarTriggerDeadtime[uGdpb] = fUnpackPar->GetStarTriggDeadtime(uGdpb);
    fdStarTriggerDelay[uGdpb]    = fUnpackPar->GetStarTriggDelay(uGdpb);
    fdStarTriggerWinSize[uGdpb]  = fUnpackPar->GetStarTriggWinSize(uGdpb);
    LOG(info) << Form("Trigger window parameters for gDPB %2u are: ", uGdpb)
              << fdStarTriggerDeadtime[uGdpb] << " ns deadtime, "
              << fdStarTriggerDelay[uGdpb] << " ns delay, "
              << fdStarTriggerWinSize[uGdpb] << " ns window width, "
              << fdAllowedTriggersSpread << " ns allowed spread";
  }  // for (Int_t iGdpb = 0; iGdpb < fuNrOfGdpbs; ++iGdpb)

  ///
  fvulCurrentEpoch.resize(fuNrOfGdpbs, 0);
  fvulCurrentEpochCycle.resize(fuNrOfGdpbs, 0);
  fvulCurrentEpochFull.resize(fuNrOfGdpbs, 0);

  ///
  fvvmEpSupprBuffer.resize(fuNrOfGdpbs);
  fvvBufferMajorAsicErrors.resize(fuNrOfGdpbs);
  fvvBufferMessages.resize(fuNrOfGdpbs);
  fvvBufferTriggers.resize(fuNrOfGdpbs);

  /// STAR Trigger decoding and monitoring
  fvulGdpbTsMsb.resize(fuNrOfGdpbs, 0);
  fvulGdpbTsLsb.resize(fuNrOfGdpbs, 0);
  fvulStarTsMsb.resize(fuNrOfGdpbs, 0);
  fvulStarTsMid.resize(fuNrOfGdpbs, 0);
  fvulGdpbTsFullLast.resize(fuNrOfGdpbs, 0);
  fvulStarTsFullLast.resize(fuNrOfGdpbs, 0);
  fvuStarTokenLast.resize(fuNrOfGdpbs, 0);
  fvuStarDaqCmdLast.resize(fuNrOfGdpbs, 0);
  fvuStarTrigCmdLast.resize(fuNrOfGdpbs, 0);
  fvulGdpbTsFullLastCore.resize(fuNrOfGdpbs, 0);
  fvulStarTsFullLastCore.resize(fuNrOfGdpbs, 0);
  fvuStarTokenLastCore.resize(fuNrOfGdpbs, 0);
  fvuStarDaqCmdLastCore.resize(fuNrOfGdpbs, 0);
  fvuStarTrigCmdLastCore.resize(fuNrOfGdpbs, 0);

  return kTRUE;
}
// -------------------------------------------------------------------------

void CbmStar2019EventBuilderEtofAlgo::AddMsComponentToList(
  size_t component,
  UShort_t usDetectorId) {
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Add to list
  fvMsComponentsList.push_back(component);

  LOG(info)
    << "CbmStar2019EventBuilderEtofAlgo::AddMsComponentToList => Component "
    << component << " with detector ID 0x" << std::hex << usDetectorId
    << std::dec << " added to list";
}
// -------------------------------------------------------------------------

Bool_t CbmStar2019EventBuilderEtofAlgo::ProcessTs(const fles::Timeslice& ts) {
  fulCurrentTsIndex = ts.index();
  fdTsStartTime     = static_cast<Double_t>(ts.descriptor(0, 0).idx);

  /// On first TS, extract the TS parameters from header (by definition stable over time)
  if (-1.0 == fdTsCoreSizeInNs) {
    fuNbCoreMsPerTs  = ts.num_core_microslices();
    fuNbOverMsPerTs  = ts.num_microslices(0) - ts.num_core_microslices();
    fdTsCoreSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs);
    fdTsFullSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs + fuNbOverMsPerTs);
    LOG(info) << "Timeslice parameters: each TS has " << fuNbCoreMsPerTs
              << " Core MS and " << fuNbOverMsPerTs
              << " Overlap MS, for a core duration of " << fdTsCoreSizeInNs
              << " ns and a full duration of " << fdTsFullSizeInNs << " ns";

    /// Ignore overlap ms if flag set by user
    fuNbMsLoop = fuNbCoreMsPerTs;
    if (kFALSE == fbIgnoreOverlapMs) fuNbMsLoop += fuNbOverMsPerTs;
    LOG(info) << "In each TS " << fuNbMsLoop << " MS will be looped over";
  }  // if( -1.0 == fdTsCoreSizeInNs )

  /// Compute time of TS core end
  fdTsStopTimeCore = fdTsStartTime + fdTsCoreSizeInNs;
  //      LOG(info) << Form( "TS %5d Start %12f Stop %12f", fulCurrentTsIndex, fdTsStartTime, fdTsStopTimeCore );

  /// Compute the limits for accepting hits and trigger in this TS, for each gDPB/sector
  /// => Avoid special cases and buffering for the overlap MS
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    if (fdStarTriggerDelay[uGdpb] < 0.0) {
      /// Event window for this gDPB starts before the trigger
      if (fdStarTriggerDelay[uGdpb] + fdStarTriggerWinSize[uGdpb] < 0.0) {
        /// Event window for this gDPB is fully before the trigger
        fvdMessCandidateTimeStart[uGdpb] = fdTsStartTime;
        // << Accept more than needed as this should be safer and small amounts >>
        fvdMessCandidateTimeStop[uGdpb] =
          fdTsStopTimeCore + fdAllowedTriggersSpread
          + 2.0
              * fdStarTriggerWinSize
                [uGdpb];  // + fdStarTriggerWinSize[ uGdpb ];
        fvdTrigCandidateTimeStart[uGdpb] =
          fdTsStartTime + fdAllowedTriggersSpread - fdStarTriggerDelay[uGdpb];
        fvdTrigCandidateTimeStop[uGdpb] = fdTsStopTimeCore
                                          + fdAllowedTriggersSpread
                                          - fdStarTriggerDelay[uGdpb];

      }  // if( fdStarTriggerDelay[ uGdpb ] + fdStarTriggerWinSize[ uGdpb ] < 0.0 )
      else {
        /// Event window for this gDPB is on both sides of the trigger
        fvdMessCandidateTimeStart[uGdpb] = fdTsStartTime;
        // << Accept more than needed as this should be safer and small amounts >>
        fvdMessCandidateTimeStop[uGdpb] =
          fdTsStopTimeCore + fdAllowedTriggersSpread
          + 2.0
              * fdStarTriggerWinSize[uGdpb];  // + fdStarTriggerDelay[ uGdpb ];
        fvdTrigCandidateTimeStart[uGdpb] =
          fdTsStartTime + fdAllowedTriggersSpread - fdStarTriggerDelay[uGdpb];
        fvdTrigCandidateTimeStop[uGdpb] = fdTsStopTimeCore
                                          + fdAllowedTriggersSpread
                                          - fdStarTriggerDelay[uGdpb];
      }  // else of if( fdStarTriggerDelay[ uGdpb ] + fdStarTriggerWinSize[ uGdpb ] < 0.0 )
    }    // if( fdStarTriggerDeadtime[ uGdpb ] < 0.0 )
    else {
      /// Event window for this gDPB starts after the trigger => fully after
      // << Accept more than needed as this should be safer and small amounts >>
      fvdMessCandidateTimeStart[uGdpb] =
        fdTsStartTime;  // - fdAllowedTriggersSpread + fdStarTriggerDelay[ uGdpb ];
      fvdMessCandidateTimeStop[uGdpb] =
        fdTsStopTimeCore + fdAllowedTriggersSpread + fdStarTriggerDelay[uGdpb]
        + fdStarTriggerWinSize[uGdpb];
      fvdTrigCandidateTimeStart[uGdpb] =
        fdTsStartTime + fdAllowedTriggersSpread;
      fvdTrigCandidateTimeStop[uGdpb] =
        fdTsStopTimeCore + fdAllowedTriggersSpread;
    }  // else of if( fdStarTriggerDelay[ uGdpb ] < 0.0 )
  }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  /// Loop over registered components
  for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size();
       ++uMsCompIdx) {
    UInt_t uMsComp = fvMsComponentsList[uMsCompIdx];

    /// Reset the flags for incomplete triggers rejection at beginning of TS
    fbTriggerFoundA = kFALSE;
    fbTriggerFoundB = kFALSE;
    fbTriggerFoundC = kFALSE;

    /// Reset the flags used to detect the end of useful data in the overlap MS
    fbEpochAfterCandWinFound   = kFALSE;
    fbTriggerAfterCandWinFound = kFALSE;

    /// Loop over core microslices (and overlap ones if chosen)
    for (fuMsIndex = 0; fuMsIndex < fuNbMsLoop; fuMsIndex++) {
      if (kFALSE == ProcessMs(ts, uMsComp, fuMsIndex)) {
        LOG(error) << "Failed to process ts " << fulCurrentTsIndex << " MS "
                   << fuMsIndex << " for component " << uMsComp;
        return kFALSE;
      }  // if( kFALSE == ProcessMs( ts, uMsCompIdx, fuMsIndex ) )
    }    // for( fuMsIndex = 0; fuMsIndex < uNbMsLoop; fuMsIndex ++ )
  }  // for( UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx )

  /// Clear event buffer from previous TS
  /// ... Hopefully whatever had to be done with it was done before calling Process TS again ^^'
  fvEventsBuffer.clear();

  /// Build events from all triggers and data found in this TS core MS + part of the overlap MS
  if (kFALSE == BuildEvents()) {
    LOG(error) << "Failed to build events in ts " << fulCurrentTsIndex;
    return kFALSE;
  }  // if( kFALSE == BuildEvents() )

  /// Clear buffers to prepare for the next TS
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fvvmEpSupprBuffer[uGdpb].clear();
    fvvBufferMajorAsicErrors[uGdpb].clear();
    fvvBufferMessages[uGdpb].clear();
    fvvBufferTriggers[uGdpb].clear();

    fvbGdpbLastMissmatchPattern[uGdpb] = 0;
    fvbGdpbLastEnablePattern[uGdpb]    = 0;
    fvbGdpbLastResyncPattern[uGdpb]    = 0;
    fvSectorStatusPattern[uGdpb].clear();
  }  // for (Int_t iGdpb = 0; iGdpb < fuNrOfGdpbs; ++iGdpb)

  /// Fill plots if in monitor mode
  if (fbMonitorMode) {
    if (kFALSE == FillHistograms()) {
      LOG(error) << "Failed to fill histos in ts " << fulCurrentTsIndex;
      return kFALSE;
    }  // if( kFALSE == FillHistograms() )
  }    // if( fbMonitorMode )

  return kTRUE;
}

static Int_t iWarn     = 0;
static Int_t iWarnMess = 0;

Bool_t CbmStar2019EventBuilderEtofAlgo::ProcessMs(const fles::Timeslice& ts,
                                                  size_t uMsCompIdx,
                                                  size_t uMsIdx) {
  UInt_t uMsComp    = fvMsComponentsList[uMsCompIdx];
  auto msDescriptor = ts.descriptor(uMsComp, uMsIdx);

  /// Store MS/TS information
  fuCurrentMs   = uMsIdx;
  fiEquipmentId = msDescriptor.eq_id;
  fdMsTime      = static_cast<double>(msDescriptor.idx);
  uint32_t size = msDescriptor.size;
  //    fulLastMsIdx = msDescriptor.idx;
  if (size > 0)
    LOG(debug) << "Microslice: " << msDescriptor.idx << " has size: " << size;

  /// Get pointer on MS data
  const uint8_t* msContent =
    reinterpret_cast<const uint8_t*>(ts.content(uMsComp, uMsIdx));

  /*
   if( fdStartTimeMsSz < 0 )
      fdStartTimeMsSz = (1e-9) * fdMsTime;

   fvhMsSzPerLink[ uMsComp ]->Fill(size);
   if( 2 * fuHistoryHistoSize < (1e-9) * fdMsTime - fdStartTimeMsSz )
   {
      // Reset the evolution Histogram and the start time when we reach the end of the range
      fvhMsSzTimePerLink[ uMsComp ]->Reset();
      fdStartTimeMsSz = (1e-9) * fdMsTime;
   } // if( 2 * fuHistoryHistoSize < (1e-9) * fdMsTime - fdStartTimeMsSz )
   fvhMsSzTimePerLink[ uMsComp ]->Fill((1e-9) * fdMsTime - fdStartTimeMsSz, size);
*/

  /// If not integer number of message in input buffer, print warning/error
  if (0 != (size % fUnpackPar->GetNbByteMessage()))
    LOG(error) << "The input microslice buffer does NOT "
               << "contain only complete nDPB messages!";

  /// Compute the number of complete messages in the input microslice buffer
  uint32_t uNbMessages = (size - (size % fUnpackPar->GetNbByteMessage()))
                         / fUnpackPar->GetNbByteMessage();

  /// Get the gDPB ID from the MS header
  fuGdpbId = fiEquipmentId;

  /// Check if this gDPB ID was declared in parameter file and stop there if not
  auto it = fGdpbIdIndexMap.find(fuGdpbId);
  if (it == fGdpbIdIndexMap.end()) {
    iWarn++;
    LOG(warning) << "Could not find the gDPB index for AFCK id 0x" << std::hex
                 << fuGdpbId << std::dec << " in microslice " << fdMsTime
                 << "\n"
                 << "If valid this index has to be added in the TOF parameter "
                    "file in the RocIdArray field";
    if (iWarn == 100) LOG(fatal) << "Got max number of Warnings!";
    return kFALSE;
  }  // if( it == fGdpbIdIndexMap.end() )
  else
    fuGdpbNr = fGdpbIdIndexMap[fuGdpbId];

  /// Store the last STAR trigger values for the core MS when reaching the first overlap MS, needed to check for clones
  if (0 < fuNbOverMsPerTs && fuNbCoreMsPerTs == fuCurrentMs) {
    fvulGdpbTsFullLastCore[fuGdpbNr] = fvulGdpbTsFullLast[fuGdpbNr];
    fvulStarTsFullLastCore[fuGdpbNr] = fvulStarTsFullLast[fuGdpbNr];
    fvuStarTokenLastCore[fuGdpbNr]   = fvuStarTokenLast[fuGdpbNr];
    fvuStarDaqCmdLastCore[fuGdpbNr]  = fvuStarDaqCmdLast[fuGdpbNr];
    fvuStarTrigCmdLastCore[fuGdpbNr] = fvuStarTrigCmdLast[fuGdpbNr];
  }  // if( 0 < fuNbOverMsPerTs && fuNbCoreMsPerTs == fuCurrentMs )

  /// Restore the last STAR trigger values for the core MS when reaching the first core MS, needed to check for clones
  if (0 < fuNbOverMsPerTs && 0 == fuCurrentMs) {
    fvulGdpbTsFullLast[fuGdpbNr] = fvulGdpbTsFullLastCore[fuGdpbNr];
    fvulStarTsFullLast[fuGdpbNr] = fvulStarTsFullLastCore[fuGdpbNr];
    fvuStarTokenLast[fuGdpbNr]   = fvuStarTokenLastCore[fuGdpbNr];
    fvuStarDaqCmdLast[fuGdpbNr]  = fvuStarDaqCmdLastCore[fuGdpbNr];
    fvuStarTrigCmdLast[fuGdpbNr] = fvuStarTrigCmdLastCore[fuGdpbNr];
  }  // if( 0 < fuNbOverMsPerTs && 0 == fuCurrentMs )

  // Prepare variables for the loop on contents
  Int_t messageType       = -111;
  const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>(msContent);
  for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++) {
    /// Escape loop if in overlap MS and flags used to detect end of usefull data are both up
    if (fuNbCoreMsPerTs <= fuCurrentMs && kTRUE == fbEpochAfterCandWinFound
        && kTRUE == fbTriggerAfterCandWinFound)
      break;

    // Fill message
    uint64_t ulData = static_cast<uint64_t>(pInBuff[uIdx]);

    /// Catch the Epoch cycle block which is always the first 64b of the MS
    if (0 == uIdx) {
      //         std::cout << Form( "gDPB %2d", fuGdpbNr) << " Epoch cycle " << Form( "0x%012lx", ulData ) << std::endl;
      ProcessEpochCycle(ulData);
      continue;
    }  // if( 0 == uIdx )

    gdpbv100::Message mess(ulData);
    /// Get message type
    messageType = mess.getMessageType();
    if (fUnpackPar->GetNrOfGet4PerGdpb() <= mess.getGdpbGenChipId()
        && 255 > mess.getGdpbGenChipId()) {  //FIXME
      if (iWarnMess < 100)
        LOG(warn) << iWarnMess << ": Invalid ChipID " << mess.getGdpbGenChipId()
                  << " in messageType " << messageType;
      iWarnMess++;
      continue;
    }

    fuGet4Id = fUnpackPar->ElinkIdxToGet4Idx(mess.getGdpbGenChipId());
    fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;
    /*
      if( 5916 == fulCurrentTsIndex && gdpbv100::MSG_STAR_TRI_A <= messageType)
         mess.printDataCout();
*/
    /*
      if( gdpbv100::MSG_STAR_TRI_A <= messageType )
         mess.printDataCout();
         continue;
*/

    if (fuNrOfGet4PerGdpb <= fuGet4Id && !mess.isStarTrigger()
        && (gdpbv100::kuChipIdMergedEpoch != fuGet4Id))
      LOG(warning) << "Message with Get4 ID too high: " << fuGet4Id << " VS "
                   << fuNrOfGet4PerGdpb << " set in parameters.";

    switch (messageType) {
      case gdpbv100::MSG_HIT: {
        if (mess.getGdpbHitIs24b()) {
          LOG(error)
            << "This event builder does not support 24b hit message!!!.";
          continue;
        }  // if( getGdpbHitIs24b() )
        else {
          fvvmEpSupprBuffer[fuGdpbNr].push_back(mess);
        }  // else of if( getGdpbHitIs24b() )
        break;
      }  // case gdpbv100::MSG_HIT:
      case gdpbv100::MSG_EPOCH: {
        if (gdpbv100::kuChipIdMergedEpoch == fuGet4Id) {
          ProcessEpoch(mess);
          /*
               if( kTRUE == fbPrintAllEpochsEnable )
               {
                  LOG(info) << "Epoch: " << Form("0x%08x ", fuGdpbId)
                            << ", Merg"
                            << ", Link " << std::setw(1) << mess.getGdpbEpLinkId()
                            << ", epoch " << std::setw(8) << mess.getGdpbEpEpochNb()
                            << ", Sync " << std::setw(1) << mess.getGdpbEpSync()
                            << ", Data loss " << std::setw(1) << mess.getGdpbEpDataLoss()
                            << ", Epoch loss " << std::setw(1) << mess.getGdpbEpEpochLoss()
                            << ", Epoch miss " << std::setw(1) << mess.getGdpbEpMissmatch();
               } // if( kTRUE == fbPrintAllEpochsEnable )
*/
        }  // if this epoch message is a merged one valid for all chips
        else {
          /// Should be checked in monitor task, here we just jump it
          LOG(debug2) << "This event builder does not support unmerged epoch "
                         "messages!!!.";
          continue;
        }  // if single chip epoch message
        break;
      }  // case gdpbv100::MSG_EPOCH:
      case gdpbv100::MSG_SLOWC: {
        fvvmEpSupprBuffer[fuGdpbNr].push_back(mess);
        break;
      }  // case gdpbv100::MSG_SLOWC:
      case gdpbv100::MSG_SYST: {
        fvvmEpSupprBuffer[fuGdpbNr].push_back(mess);
        break;
      }  // case gdpbv100::MSG_SYST:
      case gdpbv100::MSG_STAR_TRI_A:
      case gdpbv100::MSG_STAR_TRI_B:
      case gdpbv100::MSG_STAR_TRI_C:
      case gdpbv100::MSG_STAR_TRI_D: {
        ProcessStarTrigger(mess);

        /// If A message, check that the following ones are B, C, D
        /// ==> TBD only if necessary
        /*
            if( gdpbv100::MSG_STAR_TRI_A == messageType )
            {
            } // if( gdpbv100::MSG_STAR_TRI_A == messageType )
*/
        break;
      }  // case gdpbv100::MSG_STAR_TRI_A-D
      default:
        LOG(error) << "Message type " << std::hex << std::setw(2)
                   << static_cast<uint16_t>(messageType)
                   << " not included in Get4 unpacker.";
    }  // switch( mess.getMessageType() )
  }    // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)

  return kTRUE;
}

// -------------------------------------------------------------------------
void CbmStar2019EventBuilderEtofAlgo::ProcessEpochCycle(uint64_t ulCycleData) {
  ULong64_t ulEpochCycleVal = ulCycleData & gdpbv100::kulEpochCycleFieldSz;
  /*
   if( fuRawDataPrintMsgIdx < fuRawDataPrintMsgNb || fair::Logger::Logging(fair::Severity::debug2) )
   {
      LOG(info) << "CbmMcbm2018MonitorTof::ProcessEpochCyle => "
                 << Form( " TS %5d MS %3d In data 0x%016X Cycle 0x%016X",
                           fulCurrentTsIndex, fuCurrentMs, ulCycleData, ulEpochCycleVal );
      fuRawDataPrintMsgIdx ++;
   }
*/
  if (!(ulEpochCycleVal == fvulCurrentEpochCycle[fuGdpbNr]
        || ulEpochCycleVal == fvulCurrentEpochCycle[fuGdpbNr] + 1)) {
    LOG(warning) << "CbmStar2019EventBuilderEtofAlgo::ProcessEpochCycle => "
                 << " Missmatch in epoch cycles detected for Gdpb " << fuGdpbNr
                 << ", probably fake cycles due to epoch index corruption! "
                 << Form(" Current cycle 0x%09llX New cycle 0x%09llX",
                         fvulCurrentEpochCycle[fuGdpbNr],
                         ulEpochCycleVal);
  }
  if (ulEpochCycleVal != fvulCurrentEpochCycle[fuGdpbNr]) {
    LOG(info) << "CbmStar2019EventBuilderEtofAlgo::ProcessEpochCycle => "
              << " New epoch cycle for Gdpb " << fuGdpbNr
              << Form(": Current cycle 0x%09llX New cycle 0x%09llX",
                      fvulCurrentEpochCycle[fuGdpbNr],
                      ulEpochCycleVal);
  }  // if( ulEpochCycleVal != fvulCurrentEpochCycle[fuGdpbNr] )
  fvulCurrentEpochCycle[fuGdpbNr] = ulEpochCycleVal;

  return;
}

void CbmStar2019EventBuilderEtofAlgo::ProcessEpoch(gdpbv100::Message mess) {
  ULong64_t ulEpochNr = mess.getGdpbEpEpochNb();
  /*
 * /// FIXME: Need proper handling of overlap MS
   if( 0 < fvulCurrentEpoch[ fuGdpbNr ] && ulEpochNr < fvulCurrentEpoch[ fuGdpbNr ] )
   {
      std::cout << Form( "gDPB %2d", fuGdpbNr) << " New Epoch cycle "
                << Form( "0x%012llx old Ep %08llx new Ep %08llx", fvulCurrentEpochCycle[ fuGdpbNr ], fvulCurrentEpoch[ fuGdpbNr ], ulEpochNr )
                << std::endl;
      fvulCurrentEpochCycle[ fuGdpbNr ]++;
   } // if( 0 < fvulCurrentEpoch[ fuGdpbNr ] && ulEpochNr < fvulCurrentEpoch[ fuGdpbNr ] )
*/
  fvulCurrentEpoch[fuGdpbNr] = ulEpochNr;
  fvulCurrentEpochFull[fuGdpbNr] =
    ulEpochNr
    + (gdpbv100::kuEpochCounterSz + 1) * fvulCurrentEpochCycle[fuGdpbNr];

  //   fulCurrentEpochTime = mess.getMsgFullTime(ulEpochNr);

  /// Re-align the epoch number of the message in case it will be used later:
  /// We received the epoch after the data instead of the one before!
  if (0 < ulEpochNr)
    mess.setGdpbEpEpochNb(ulEpochNr - 1);
  else
    mess.setGdpbEpEpochNb(gdpbv100::kuEpochCounterSz);

  /// Process the corresponding messages buffer
  ProcessEpSupprBuffer(fuGdpbNr);

  /// Detecting of the end of useful data in the overlap MS
  if (fuNbCoreMsPerTs <= fuCurrentMs
      && fvdMessCandidateTimeStop[fuGdpbNr]
           < fvulCurrentEpochFull[fuGdpbNr] * gdpbv100::kdEpochInNs) {
    fbEpochAfterCandWinFound = kTRUE;
  }  // if( fuNbCoreMsPerTs <= fuCurrentMs && fvdMessCandidateTimeStop[ fuGdpbNr ] < fvulCurrentEpochFull[ fuGdpbNr ] * gdpbv100::kdEpochInNs )
}

void CbmStar2019EventBuilderEtofAlgo::ProcessStarTrigger(
  gdpbv100::Message mess) {
  Int_t iMsgIndex = mess.getStarTrigMsgIndex();

  switch (iMsgIndex) {
    case 0: {
      fvulGdpbTsMsb[fuGdpbNr] = mess.getGdpbTsMsbStarA();

      /// Incomplete trigger detection flag
      if (fbMonitorMode && fbDebugMonitorMode) {
        fhRawTriggersStats->Fill(
          0., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);
        if (fuCurrentMs < fuNbCoreMsPerTs)
          fhRawTriggersStatsCore->Fill(
            0., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);
        else
          fhRawTriggersStatsOver->Fill(
            0., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);

        if (kTRUE == fbTriggerFoundA)
          LOG(info) << Form(
            "Problem: Overwritting a A trigger in TS %12llu MS %3u Core? %u",
            fulCurrentTsIndex,
            fuMsIndex,
            fuCurrentMs < fuNbCoreMsPerTs)
                    << Form(" (%u %u %u)",
                            fbTriggerFoundA,
                            fbTriggerFoundB,
                            fbTriggerFoundC);
      }  // if( fbMonitorMode && fbDebugMonitorMode )
      fbTriggerFoundA = kTRUE;

      break;
    }  // 1st trigger message
    case 1: {
      fvulGdpbTsLsb[fuGdpbNr] = mess.getGdpbTsLsbStarB();
      fvulStarTsMsb[fuGdpbNr] = mess.getStarTsMsbStarB();

      /// Incomplete trigger detection flag
      if (fbMonitorMode && fbDebugMonitorMode) {
        fhRawTriggersStats->Fill(
          1., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);
        if (fuCurrentMs < fuNbCoreMsPerTs)
          fhRawTriggersStatsCore->Fill(
            1., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);
        else
          fhRawTriggersStatsOver->Fill(
            1., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);

        if (kFALSE == fbTriggerFoundA)
          LOG(info) << Form(
            "Problem: Found B trigger before A in TS %12llu MS %3u",
            fulCurrentTsIndex,
            fuMsIndex);

        if (kTRUE == fbTriggerFoundB)
          LOG(info) << Form(
            "Problem: Overwritting a B trigger in TS %12llu MS %3u Core? %u",
            fulCurrentTsIndex,
            fuMsIndex,
            fuCurrentMs < fuNbCoreMsPerTs)
                    << Form(" (%u %u %u)",
                            fbTriggerFoundA,
                            fbTriggerFoundB,
                            fbTriggerFoundC);
      }  // if( fbMonitorMode && fbDebugMonitorMode )
      fbTriggerFoundB = kTRUE;

      break;
    }  // 2nd trigger message
    case 2: {
      fvulStarTsMid[fuGdpbNr] = mess.getStarTsMidStarC();

      /// Incomplete trigger detection flag
      if (fbMonitorMode && fbDebugMonitorMode) {
        fhRawTriggersStats->Fill(
          2., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);
        if (fuCurrentMs < fuNbCoreMsPerTs)
          fhRawTriggersStatsCore->Fill(
            2., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);
        else
          fhRawTriggersStatsOver->Fill(
            2., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);

        if (kFALSE == fbTriggerFoundB || kFALSE == fbTriggerFoundA)
          LOG(info) << Form(
            "Problem: Found C trigger before A or B in TS %12llu MS %3u",
            fulCurrentTsIndex,
            fuMsIndex);

        if (kTRUE == fbTriggerFoundC)
          LOG(info) << Form(
            "Problem: Overwritting a C trigger in TS %12llu MS %3u Core? %u",
            fulCurrentTsIndex,
            fuMsIndex,
            fuCurrentMs < fuNbCoreMsPerTs)
                    << Form(" (%u %u %u)",
                            fbTriggerFoundA,
                            fbTriggerFoundB,
                            fbTriggerFoundC);
      }  // if( fbMonitorMode && fbDebugMonitorMode )
      fbTriggerFoundC = kTRUE;

      break;
    }  // 3rd trigger message
    case 3: {
      ULong64_t ulNewGdpbTsFull =
        (fvulGdpbTsMsb[fuGdpbNr] << 24) + (fvulGdpbTsLsb[fuGdpbNr]);
      ULong64_t ulNewStarTsFull = (fvulStarTsMsb[fuGdpbNr] << 48)
                                  + (fvulStarTsMid[fuGdpbNr] << 8)
                                  + mess.getStarTsLsbStarD();
      UInt_t uNewToken   = mess.getStarTokenStarD();
      UInt_t uNewDaqCmd  = mess.getStarDaqCmdStarD();
      UInt_t uNewTrigCmd = mess.getStarTrigCmdStarD();

      /// Detect and reject incomplete triggers: they should always come in blocks of 4,
      /// ===> Typically this happen at the beginning of the TS and the same full trigger
      ///      is found in the overlap of the previous TS
      if (kFALSE == fbTriggerFoundC || kFALSE == fbTriggerFoundB
          || kFALSE == fbTriggerFoundA) {
        if (fbMonitorMode && fbDebugMonitorMode)
          LOG(info) << Form("Problem: Found D trigger before A or B or C in TS "
                            "%12llu MS %3u Core? %u",
                            fulCurrentTsIndex,
                            fuMsIndex,
                            fuCurrentMs < fuNbCoreMsPerTs)
                    << Form(" (%u %u %u) token %5u",
                            fbTriggerFoundA,
                            fbTriggerFoundB,
                            fbTriggerFoundC,
                            uNewToken);

        /// Reset Incomplete trigger detection flag for next trigger
        fbTriggerFoundA = kFALSE;
        fbTriggerFoundB = kFALSE;
        fbTriggerFoundC = kFALSE;

        /// Return only if in first MS
        if (0 == fuCurrentMs) return;
      }  // if( kFALSE == fbTriggerFoundC || kFALSE == fbTriggerFoundB || kFALSE == fbTriggerFoundA )

      /// Reset Incomplete trigger detection flag for next trigger
      fbTriggerFoundA = kFALSE;
      fbTriggerFoundB = kFALSE;
      fbTriggerFoundC = kFALSE;

      if (fbMonitorMode && fbDebugMonitorMode) {
        fhRawTriggersStats->Fill(
          3., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);
        if (fuCurrentMs < fuNbCoreMsPerTs)
          fhRawTriggersStatsCore->Fill(
            3., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);
        else
          fhRawTriggersStatsOver->Fill(
            3., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);
      }  // if( fbMonitorMode && fbDebugMonitorMode )

      /*
         UInt_t uNewTrigWord =  ( (uNewTrigCmd & 0x00F) << 16 )
                  + ( (uNewDaqCmd   & 0x00F) << 12 )
                  + ( (uNewToken    & 0xFFF)       );
         LOG(info) << "New STAR trigger "
                   << " TS " << fulCurrentTsIndex
                   << " gDBB #" << fuGdpbNr << " "
                   << Form("token = %5u ", uNewToken )
                   << Form("gDPB ts  = %12llu ", ulNewGdpbTsFull )
                   << Form("STAR ts = %12llu ", ulNewStarTsFull )
                   << Form("DAQ cmd = %2u ", uNewDaqCmd )
                   << Form("TRG cmd = %2u ", uNewTrigCmd )
                   << Form("TRG Wrd = %5x ", uNewTrigWord );
*/

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

      // GDPB TS counter reset detection
      if (ulNewGdpbTsFull < fvulGdpbTsFullLast[fuGdpbNr])
        LOG(debug) << "Probable reset of the GDPB TS: old = "
                   << Form("%16llu", fvulGdpbTsFullLast[fuGdpbNr])
                   << " new = " << Form("%16llu", ulNewGdpbTsFull)
                   << " Diff = -"
                   << Form("%8llu",
                           fvulGdpbTsFullLast[fuGdpbNr] - ulNewGdpbTsFull)
                   << " GDPB #" << Form("%2u", fuGdpbNr);

      // STAR TS counter reset detection
      if (ulNewStarTsFull < fvulStarTsFullLast[fuGdpbNr])
        LOG(debug) << "Probable reset of the STAR TS: old = "
                   << Form("%16llu", fvulStarTsFullLast[fuGdpbNr])
                   << " new = " << Form("%16llu", ulNewStarTsFull)
                   << " Diff = -"
                   << Form("%8llu",
                           fvulStarTsFullLast[fuGdpbNr] - ulNewStarTsFull)
                   << " GDPB #" << Form("%2u", fuGdpbNr);

      /*
         LOG(info) << "Updating  trigger token for " << fuGdpbNr
                   << " " << fvuStarTokenLast[fuGdpbNr] << " " << uNewToken;
*/

      /// Generate Trigger object and store it for event building ///
      CbmTofStarTrigger2019 newTrig(ulNewGdpbTsFull,
                                    ulNewStarTsFull,
                                    uNewToken,
                                    uNewDaqCmd,
                                    uNewTrigCmd,
                                    fuGdpbId);
      Double_t dTriggerTime =
        newTrig.GetFullGdpbTs() * gdpbv100::kdClockCycleSizeNs;
      if (fvdTrigCandidateTimeStart[fuGdpbNr] < dTriggerTime
          && dTriggerTime < fvdTrigCandidateTimeStop[fuGdpbNr]) {
        fvvBufferTriggers[fuGdpbNr].push_back(newTrig);

        if (fbMonitorMode && fbDebugMonitorMode) {
          fhRawTriggersStatsSel->Fill(
            0., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);
          if (fuCurrentMs < fuNbCoreMsPerTs)
            fhRawTriggersStatsSel->Fill(
              1., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);
          else
            fhRawTriggersStatsSel->Fill(
              2., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);
        }  // if( fbMonitorMode && fbDebugMonitorMode )
      }  // if( fvdTrigCandidateTimeStart[ fuGdpbNr ] < dTriggerTime && dTriggerTime < fvdTrigCandidateTimeStop[ fuGdpbNr ] )
      else if (fuNbCoreMsPerTs <= fuCurrentMs
               && fvdTrigCandidateTimeStop[fuGdpbNr] < dTriggerTime) {
        /// Detected the end of useful data in the overlap MS
        fbTriggerAfterCandWinFound = kTRUE;
      }  // else if( fuNbCoreMsPerTs <= fuCurrentMs && fvdTrigCandidateTimeStop[ fuGdpbNr ] < dTriggerTime )

      //         if( fuCurrentMs < fuNbCoreMsPerTs )
      {
        //            ULong64_t ulGdpbTsDiff = ulNewGdpbTsFull - fvulGdpbTsFullLast[fuGdpbNr];
        fvulGdpbTsFullLast[fuGdpbNr] = ulNewGdpbTsFull;
        fvulStarTsFullLast[fuGdpbNr] = ulNewStarTsFull;
        fvuStarTokenLast[fuGdpbNr]   = uNewToken;
        fvuStarDaqCmdLast[fuGdpbNr]  = uNewDaqCmd;
        fvuStarTrigCmdLast[fuGdpbNr] = uNewTrigCmd;
      }
      ///---------------------------------------------------------///
      /// Full Trigger only if we reached this point
      if (fbMonitorMode && fbDebugMonitorMode) {
        fhRawTriggersStats->Fill(
          4., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);
        if (fuCurrentMs < fuNbCoreMsPerTs)
          fhRawTriggersStatsCore->Fill(
            4., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);
        else
          fhRawTriggersStatsOver->Fill(
            4., fUnpackPar->GetGdpbToSectorOffset() + fuGdpbNr);
        fvhTriggerDistributionInTs[fuGdpbNr]->Fill(
          (dTriggerTime - fdTsStartTime) / 1000.0);
        fvhTriggerDistributionInMs[fuGdpbNr]->Fill((dTriggerTime - fdMsTime)
                                                   / 1000.0);

        if ((dTriggerTime - fdMsTime) / 1000.0 < -1000) {
          LOG(info) << Form("Trigger in wrong MS TS %6llu", fulCurrentTsIndex)
                    << Form(" MS %3u ", fuMsIndex)
                    << Form(" Sector %3u ", fuGdpbNr + 13)
                    << Form(" Ttrig %15.2f", dTriggerTime)
                    << Form(" Tms %15.2f", fdMsTime)
                    << Form(" dT %15.5f", ((dTriggerTime - fdMsTime) / 1000.0));
          LOG(info) << Form("Full token, gDPB TS LSB bits: 0x%16llx, STAR TS "
                            "MSB bits: 0x%16llx, token is %4u",
                            ulNewGdpbTsFull,
                            ulNewStarTsFull,
                            uNewToken);
        }  // if( (dTriggerTime - fdMsTime) / 1000.0 < -1000 )
      }    // if( fbMonitorMode && fbDebugMonitorMode )

      //         LOG(info) << "First full trigger in TS " << fulCurrentTsIndex;

      break;
    }  // case 3
    default: LOG(error) << "Unknown Star Trigger messageindex: " << iMsgIndex;
  }  // switch( iMsgIndex )
}

// -------------------------------------------------------------------------
void CbmStar2019EventBuilderEtofAlgo::ProcessEpSupprBuffer(
  uint32_t /*uGdpbNr*/) {
  Int_t iBufferSize = fvvmEpSupprBuffer[fuGdpbNr].size();

  if (0 == iBufferSize) return;

  LOG(debug) << "Now processing stored messages for for gDPB " << fuGdpbNr
             << " with epoch number " << (fvulCurrentEpoch[fuGdpbNr] - 1);

  /// Data are sorted between epochs, not inside => Epoch level ordering
  /// Sorting at lower bin precision level
  std::stable_sort(fvvmEpSupprBuffer[fuGdpbNr].begin(),
                   fvvmEpSupprBuffer[fuGdpbNr].begin());

  /// Compute original epoch index before epoch suppression
  ULong64_t ulCurEpochGdpbGet4 = fvulCurrentEpochFull[fuGdpbNr];

  /// Ignore the first epoch as it should never appear (start delay!!)
  if (0 == ulCurEpochGdpbGet4) return;

  /// In Ep. Suppr. Mode, receive following epoch instead of previous
  ulCurEpochGdpbGet4--;

  Int_t messageType = -111;
  for (Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++) {

    messageType = fvvmEpSupprBuffer[fuGdpbNr][iMsgIdx].getMessageType();

    fuGet4Id = fUnpackPar->ElinkIdxToGet4Idx(
      fvvmEpSupprBuffer[fuGdpbNr][iMsgIdx].getGdpbGenChipId());
    fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;

    /// Store the full message in the proper buffer
    gdpbv100::FullMessage fullMess(fvvmEpSupprBuffer[fuGdpbNr][iMsgIdx],
                                   ulCurEpochGdpbGet4);

    /// Do other actions on it if needed
    switch (messageType) {
      case gdpbv100::MSG_HIT: {
        //            ProcessHit( fullMess, ulCurEpochGdpbGet4 );
        StoreMessageInBuffer(fullMess, fuGdpbNr);
        break;
      }  // case gdpbv100::MSG_HIT:
      case gdpbv100::MSG_SLOWC: {
        //            ProcessSlCtrl( fullMess, ulCurEpochGdpbGet4 );
        StoreMessageInBuffer(fullMess, fuGdpbNr);
        break;
      }  // case gdpbv100::MSG_SLOWC:
      case gdpbv100::MSG_SYST: {
        if (gdpbv100::SYS_PATTERN == fullMess.getGdpbSysSubType()) {
          if (fbAddStatusToEvent) ProcessPattern(fullMess, ulCurEpochGdpbGet4);
        }  // if( gdpbv100::SYS_PATTERN == fvvmEpSupprBuffer[ fuGdpbNr ][ iMsgIdx ].getGdpbSysSubType() )
           /*
               else ProcessSysMess( fullMess, ulCurEpochGdpbGet4 );
*/
        else
          StoreMessageInBuffer(fullMess, fuGdpbNr);

        break;
      }  // case gdpbv100::MSG_SYST:
      case gdpbv100::MSG_EPOCH:
      case gdpbv100::MSG_STAR_TRI_A:
      case gdpbv100::MSG_STAR_TRI_B:
      case gdpbv100::MSG_STAR_TRI_C:
      case gdpbv100::MSG_STAR_TRI_D: break;
      default:
        LOG(error) << "Message type " << std::hex << std::setw(2)
                   << static_cast<uint16_t>(messageType)
                   << " not included in Get4 unpacker.";
    }  // switch( mess.getMessageType() )
  }    // for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )

  fvvmEpSupprBuffer[fuGdpbNr].clear();
}
void CbmStar2019EventBuilderEtofAlgo::StoreMessageInBuffer(
  gdpbv100::FullMessage fullMess,
  uint32_t /*uGdpbNr*/) {
  /// Store in the major error buffer only if GET4 error not channel related
  uint16_t usG4ErrorType = fullMess.getGdpbSysErrData();
  if (gdpbv100::MSG_SYST == fullMess.getMessageType()
      && gdpbv100::SYS_GET4_ERROR == fullMess.getGdpbSysSubType()
      && ((usG4ErrorType < gdpbv100::GET4_V2X_ERR_TOT_OVERWRT
           && gdpbv100::GET4_V2X_ERR_LOST_EVT != usG4ErrorType)
          || usG4ErrorType > gdpbv100::GET4_V2X_ERR_SEQUENCE_ER)) {
    fvvBufferMajorAsicErrors[fuGdpbNr].push_back(fullMess);
    return;
  }  // if GET4 error out of TOT/hit building error range
  else if (fbStoreLostEventMsg
           && gdpbv100::GET4_V2X_ERR_LOST_EVT == usG4ErrorType) {
    fvvBufferMajorAsicErrors[fuGdpbNr].push_back(fullMess);
    return;
  }  // else if( fbStoreLostEventMsg && gdpbv100::GET4_V2X_ERR_LOST_EVT == usG4ErrorType )

  if (fbMonitorMode && fbDebugMonitorMode
      && gdpbv100::MSG_HIT == fullMess.getMessageType())
    fvhMessDistributionInMs[fuGdpbNr]->Fill(
      (fullMess.GetFullTimeNs() - fdMsTime) / 1000.0);
  /*
   LOG(info) << Form( "Message Full Time ns: %f MS time ns: %f diff: %f Start %f Stop %f",
                      fullMess.GetFullTimeNs(), fdMsTime, fullMess.GetFullTimeNs() - fdMsTime,
                      fvdMessCandidateTimeStart[ fuGdpbNr ], fvdMessCandidateTimeStop[ fuGdpbNr ] );
   LOG(info) << Form( "Current epoch %llu Current cycle %llu Current Full epoch %llu",
                      fvulCurrentEpoch[ fuGdpbNr ], fvulCurrentEpochCycle[ fuGdpbNr ], fvulCurrentEpochFull[ fuGdpbNr ] );
*/
  /// All other messages (including non "critical" errors) get the same treatment: enter event only if in event window
  /// => These errors are probably added only if the event window is across the previous epoch!
  if (fvdMessCandidateTimeStart[fuGdpbNr] < fullMess.GetFullTimeNs()
      && fullMess.GetFullTimeNs() < fvdMessCandidateTimeStop[fuGdpbNr]) {
    fvvBufferMessages[fuGdpbNr].push_back(fullMess);
  }  // if in limits
}

// -------------------------------------------------------------------------
void CbmStar2019EventBuilderEtofAlgo::ProcessHit(
  gdpbv100::Message mess,
  uint64_t /*ulCurEpochGdpbGet4*/) {
  UInt_t uChannel = mess.getGdpbHitChanId();
  //   UInt_t uTot     = mess.getGdpbHit32Tot();

  // In 32b mode the coarse counter is already computed back to 112 FTS bins
  // => need to hide its contribution from the Finetime
  // => FTS = Fullt TS modulo 112
  //   UInt_t uFts     = mess.getGdpbHitFullTs() % 112;

  //   UInt_t uChannelNr         = fuGet4Id * fuNrOfChannelsPerGet4 + uChannel;
  //   UInt_t uChannelNrInFee    = (fuGet4Id % fuNrOfGet4PerFee) * fuNrOfChannelsPerGet4 + uChannel;
  //   UInt_t uFeeNr             = (fuGet4Id / fuNrOfGet4PerFee);
  //   UInt_t uFeeNrInSys        = fuGdpbNr * fuNrOfFeePerGdpb + uFeeNr;
  //   UInt_t uRemappedChannelNr = uFeeNr * fuNrOfChannelsPerFee + fUnpackPar->Get4ChanToPadiChan( uChannelNrInFee );
  //   UInt_t uGbtxNr            = (uFeeNr / fUnpackPar->GetNrOfFeePerGbtx());
  //   UInt_t uFeeInGbtx         = (uFeeNr % fUnpackPar->GetNrOfFeePerGbtx());
  //   UInt_t uGbtxNrInSys       = fuGdpbNr * fUnpackPar->GetNrOfGbtxPerGdpb() + uGbtxNr;

  //   ULong_t  ulhitTime = mess.getMsgFullTime(  ulCurEpochGdpbGet4 );
  //   Double_t dHitTime  = mess.getMsgFullTimeD( ulCurEpochGdpbGet4 );
  //   Double_t dHitTot   = uTot;     // in bins

  //   UInt_t uFebIdx     = (fuGet4Id / fUnpackPar->GetNrOfGet4PerFee());
  //   UInt_t uFullFebIdx = (fuGdpbNr * fUnpackPar->GetNrOfFeePerGdpb()) + uFebIdx;

  UInt_t uChanInGdpb = fuGet4Id * fuNrOfChannelsPerGet4 + uChannel;
  UInt_t uChanInSyst = fuGdpbNr * fuNrOfChannelsPerGdpb + uChanInGdpb;
  if (fUnpackPar->GetNumberOfChannels() < uChanInSyst) {
    LOG(error) << "Invalid mapping index " << uChanInSyst << " VS "
               << fUnpackPar->GetNumberOfChannels() << ", from " << fuGdpbNr
               << ", " << fuGet4Id << ", " << uChannel;
    return;
  }  // if( fUnpackPar->GetNumberOfChannels() < uChanUId )
}

void CbmStar2019EventBuilderEtofAlgo::ProcessSlCtrl(
  gdpbv100::Message /*mess*/,
  uint64_t /*ulCurEpochGdpbGet4*/) {}

void CbmStar2019EventBuilderEtofAlgo::ProcessSysMess(
  gdpbv100::Message mess,
  uint64_t /*ulCurEpochGdpbGet4*/) {
  switch (mess.getGdpbSysSubType()) {
    case gdpbv100::SYS_GET4_ERROR: {
      uint32_t uData = mess.getGdpbSysErrData();
      if (gdpbv100::GET4_V2X_ERR_TOT_OVERWRT == uData
          || gdpbv100::GET4_V2X_ERR_TOT_RANGE == uData
          || gdpbv100::GET4_V2X_ERR_EVT_DISCARD == uData
          || gdpbv100::GET4_V2X_ERR_ADD_RIS_EDG == uData
          || gdpbv100::GET4_V2X_ERR_UNPAIR_FALL == uData
          || gdpbv100::GET4_V2X_ERR_SEQUENCE_ER == uData
          || gdpbv100::GET4_V2X_ERR_LOST_EVT == uData)
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
      LOG(debug) << "GET4 synchronization pulse missing";
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
// -------------------------------------------------------------------------
void CbmStar2019EventBuilderEtofAlgo::ProcessPattern(
  gdpbv100::Message mess,
  uint64_t /*ulCurEpochGdpbGet4*/) {
  uint16_t usType   = mess.getGdpbSysPattType();
  uint16_t usIndex  = mess.getGdpbSysPattIndex();
  uint32_t uPattern = mess.getGdpbSysPattPattern();
  //   std::bitset< 32 > bPattern( mess.getGdpbSysPattPattern() );
  UInt_t uNbBits = (kuNbMsgPerPattern - 1 == usIndex ? 16 : 32);

  switch (usType) {
    case gdpbv100::PATT_MISSMATCH: {
      //         LOG(debug) << Form( "Missmatch pattern message => Type %d, Index %2d, Pattern 0x%08X", usType, usIndex, uPattern );
      for (UInt_t uBit = 0; uBit < uNbBits; ++uBit) {
        fvbGdpbLastMissmatchPattern[fuGdpbNr][32 * usIndex + uBit] =
          (uPattern >> uBit) & 0x1;
        //            fvbGdpbLastMissmatchPattern[ fuGdpbNr ][ 32 * usIndex + uBit ] = bPattern[ uBit ];
      }  // for( UInt_t uBit = 0; uBit < uNbBits; ++uBit )
      break;
    }  // case gdpbv100::PATT_MISSMATCH:
    case gdpbv100::PATT_ENABLE: {
      //         LOG(debug2) << Form( "Enable pattern message => Type %d, Index %2d, Pattern 0x%08X", usType, usIndex, uPattern );
      for (UInt_t uBit = 0; uBit < uNbBits; ++uBit) {
        fvbGdpbLastEnablePattern[fuGdpbNr][32 * usIndex + uBit] =
          (uPattern >> uBit) & 0x1;
        //            fvbGdpbLastEnablePattern[ fuGdpbNr ][ 32 * usIndex + uBit ] = bPattern[ uBit ];
      }  // for( UInt_t uBit = 0; uBit < uNbBits; ++uBit )
      break;
    }  // case gdpbv100::PATT_ENABLE:
    case gdpbv100::PATT_RESYNC: {
      //         LOG(debug) << Form( "RESYNC pattern message => Type %d, Index %2d, Pattern 0x%08X", usType, usIndex, uPattern );
      for (UInt_t uBit = 0; uBit < uNbBits; ++uBit) {
        fvbGdpbLastResyncPattern[fuGdpbNr][32 * usIndex + uBit] =
          (uPattern >> uBit) & 0x1;
        //            fvbGdpbLastResyncPattern[ fuGdpbNr ][ 32 * usIndex + uBit ] = bPattern[ uBit ];
      }  // for( UInt_t uBit = 0; uBit < uNbBits; ++uBit )
      break;
    }  // case gdpbv100::PATT_RESYNC:
    default: {
      LOG(debug) << "Crazy pattern message, subtype " << usType;
      break;
    }  // default
  }    // switch( usType )

  if (kuNbMsgPerPattern - 1 == usIndex) UpdateStatusPatternCurrGdpb();

  return;
}
void CbmStar2019EventBuilderEtofAlgo::UpdateStatusPatternCurrGdpb() {
  /// Add a status update with the missmatch pattern as initial value
  /// Optimization: use emplace?
  fvSectorStatusPattern[fuGdpbNr].push_back(
    std::pair<uint64_t, std::bitset<kuNbAsicPerGdpb>>(
      fvulCurrentEpochFull[fuGdpbNr], fvbGdpbLastMissmatchPattern[fuGdpbNr]));

  /// Generate a status pattern by performing an OR with the enable and resync patterns
  fvSectorStatusPattern[fuGdpbNr].back().second |=
    fvbGdpbLastEnablePattern[fuGdpbNr];
  fvSectorStatusPattern[fuGdpbNr].back().second |=
    fvbGdpbLastResyncPattern[fuGdpbNr];
}
gdpbv100::FullMessage CbmStar2019EventBuilderEtofAlgo::CreateStatusMessage(
  uint16_t uGdpbId,
  uint32_t uIndex,
  std::pair<uint64_t, std::bitset<kuNbAsicPerGdpb>> statusIn) {
  /// Mask for extraction of pattern blocks for each msg index
  static constexpr std::bitset<kuNbAsicPerGdpb> kbMask32 {0xFFFFFFFF};

  /// Create message and set the full epoch
  gdpbv100::FullMessage outMess(0, statusIn.first);

  /// Extract block corresponding to this index
  const std::bitset<kuNbAsicPerGdpb> bPatterBlock {
    (statusIn.second >> (32 * uIndex)) & kbMask32};

  /// Set the messages fields
  outMess.setGdpbGenGdpbId(uGdpbId);
  outMess.setMessageType(gdpbv100::MSG_SYST);
  outMess.setGdpbSysSubType(gdpbv100::SYS_PATTERN);
  outMess.setGdpbSysPattType(gdpbv100::PATT_STATUS);
  outMess.setGdpbSysPattIndex(uIndex);
  outMess.setGdpbSysPattPattern(
    bPatterBlock.to_ulong());  // Safe as we masked with 32b

  return outMess;
}
// -------------------------------------------------------------------------

Bool_t CbmStar2019EventBuilderEtofAlgo::BuildEvents() {
  /// Get an iterator to the 1st trigger of each gDPB/sector
  /// + Use iterators to keep track of where we stand in the message and error buffers
  /// + Same principle for the Status updates: either the same for consecutive triggs or the next!
  /// => Allow to avoid needless looping and to properly deal with user defined deadtime (double counting)
  std::vector<std::vector<CbmTofStarTrigger2019>::iterator>
    itTrigger;  /// [sector]
  std::vector<std::vector<gdpbv100::FullMessage>::iterator>
    itErrorMessStart;  /// [sector]
  std::vector<std::vector<gdpbv100::FullMessage>::iterator>
    itMessStart;  /// [sector]
  std::vector<
    std::vector<std::pair<uint64_t, std::bitset<kuNbAsicPerGdpb>>>::iterator>
    itStatUpdtStart;  /// [sector]
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    itTrigger.push_back(fvvBufferTriggers[uGdpb].begin());
    itErrorMessStart.push_back(fvvBufferMajorAsicErrors[uGdpb].begin());
    itMessStart.push_back(fvvBufferMessages[uGdpb].begin());
    itStatUpdtStart.push_back(fvSectorStatusPattern[uGdpb].begin());
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  /// Check that all gDPB/sectors are not straight at end of their trigger buffer (TS w/o triggers)
  Bool_t bAllSectAllTriggDone = kTRUE;
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    /// => Check can be stopped ad soon as a gDPB/sector not at end is found!
    if (fvvBufferTriggers[uGdpb].end() != itTrigger[uGdpb]) {
      bAllSectAllTriggDone = kFALSE;
      break;
    }  // if( fvvBufferTriggers[ uGdpb ].end() != (*itTrigger[ uGdpb ]) )
  }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  /// while at least one gDPB not at end of its triger buffer
  /// ==> Need to include a TS edge check? Should be taken care of be the fdAllowedTriggersSpread
  ///     but check here if problem of split events on TS edge
  while (kFALSE == bAllSectAllTriggDone) {
    /// Check if all @ same trigger ID and CMD fields
    Bool_t bAllSectorsMatch     = kTRUE;
    UInt_t uFirstStarToken      = 0;
    UShort_t usFirstStarDaqCmd  = 0;
    UShort_t usFirstStarTrigCmd = 0;
    ULong64_t ulFirstFullGdpbTs = 0;

    /// If first sector has no remaining trigger, we cannot have same trigger in all!
    /// => Prevent seg fault if first sector goes missing
    if (0 == fvvBufferTriggers[0].size()) {
      bAllSectorsMatch = kFALSE;
    }  // if( 0 == fvvBufferTriggers[ 0 ].size() )
    else {
      /// If first sector still has a trigger, use it as reference to check if the
      /// first remaining trigger in all sectors is the same
      uFirstStarToken    = (*itTrigger[0]).GetStarToken();
      usFirstStarDaqCmd  = (*itTrigger[0]).GetStarDaqCmd();
      usFirstStarTrigCmd = (*itTrigger[0]).GetStarTrigCmd();
      ulFirstFullGdpbTs  = (*itTrigger[0]).GetFullGdpbTs();
      for (UInt_t uGdpb = 1; uGdpb < fuNrOfGdpbs; ++uGdpb) {
        if (itTrigger[uGdpb] == fvvBufferTriggers[uGdpb].end()
            || (*itTrigger[uGdpb]).GetStarToken() != uFirstStarToken
            || (*itTrigger[uGdpb]).GetStarDaqCmd() != usFirstStarDaqCmd
            || (*itTrigger[uGdpb]).GetStarTrigCmd() != usFirstStarTrigCmd) {
          bAllSectorsMatch = kFALSE;
          break;
        }  // If end of buffer or any field differs for any gDPB/sector current trigger, not all matched!
      }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
    }      // else of if( 0 == itTrigger[ uGdpb ].size() )

    if (kTRUE == bAllSectorsMatch) {
      /// Yes = Complete event
      /// Create event
      CbmTofStarSubevent2019
        starSubEvent;  /// Mean trigger is set latter, SourceId 0 for the full wheel!

      /// Loop gDPB/sector
      Double_t dMeanTriggerGdpbTs = 0;
      Double_t dMeanTriggerStarTs = 0;
      for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
        /// Prepare mean trigger for event header
        dMeanTriggerGdpbTs += (*itTrigger[uGdpb]).GetFullGdpbTs();
        dMeanTriggerStarTs += (*itTrigger[uGdpb]).GetFullStarTs();

        if (fbMonitorMode && fbDebugMonitorMode) {
          Long64_t ilTriggerDt =
            static_cast<Long64_t>((*itTrigger[uGdpb]).GetFullGdpbTs())
            - static_cast<Long64_t>(ulFirstFullGdpbTs);
          fvhTriggerDt[uGdpb]->Fill(ilTriggerDt);
        }  // if( fbMonitorMode && fbDebugMonitorMode )

        /// Convert STAR trigger to 4 full messages and insert in buffer
        std::vector<gdpbv100::FullMessage> vTrigMess =
          (*itTrigger[uGdpb]).GetGdpbMessages();
        for (std::vector<gdpbv100::FullMessage>::iterator itMess =
               vTrigMess.begin();
             itMess != vTrigMess.end();
             ++itMess) {
          starSubEvent.AddMsg((*itMess));
        }  // for( std::vector< gdpbv100::FullMessage >::iterator itMess = vTrigMess.begin(); itMess != vTrigMess.end(); ++ itMess )

        if (fbAddStatusToEvent) {
          /*
               LOG(info) << Form( "Trying to add status to event, status buffer size is %d, first test status time is %llu, trigger time is %llu Evt size is %u",
                                   fvSectorStatusPattern[ uGdpb ].size(),
                                   (*itStatUpdtStart[ uGdpb ]).first * gdpbv100::kuCoarseCounterSize,
                                   (*itTrigger[ uGdpb ]).GetFullGdpbTs(),
                                   starSubEvent.GetMsgBuffSize() );
*/
          /// Find Last status field update before the trigger, convert it to 7 status pattern messages and insert in buffer
          /// Iterator may reach the end if last update before trigger is also last update in general
          /// Used proper order of conditions to avoid segfault
          /// Loop until we find first update not matching or the end of the vector
          while (fvSectorStatusPattern[uGdpb].end() != itStatUpdtStart[uGdpb]
                 && ((*itStatUpdtStart[uGdpb]).first)
                        * gdpbv100::kuCoarseCounterSize
                      <= (*itTrigger[uGdpb]).GetFullGdpbTs()) {
            ++itStatUpdtStart[uGdpb];
          }  // while( not at the end of updates vector AND Update time under trigger time )
          /// Check that there was at least one update before this trigger
          if (fvSectorStatusPattern[uGdpb].begin() != itStatUpdtStart[uGdpb]) {
            /// Go back to last matching update
            --itStatUpdtStart[uGdpb];
            /// Generate and insert the messages
            uint16_t usGdpbId = fUnpackPar->GetGdpbId(uGdpb);
            for (uint32_t uIdx = 0; uIdx < kuNbMsgPerPattern; ++uIdx)
              starSubEvent.AddMsg(
                CreateStatusMessage(usGdpbId, uIdx, (*itStatUpdtStart[uGdpb])));
            /*
                  LOG(info) << Form( "found status time is %llu, diff to trigger time is %llu  Evt size is %u",
                                      ( (*itStatUpdtStart[ uGdpb ]).first ) * gdpbv100::kuCoarseCounterSize,
                                      (*itTrigger[ uGdpb ]).GetFullGdpbTs() - ( (*itStatUpdtStart[ uGdpb ]).first ) * gdpbv100::kuCoarseCounterSize,
                                   starSubEvent.GetMsgBuffSize() );
*/
          }  // if( fvSectorStatusPattern[ uGdpb ].begin() != itStatUpdtStart[ uGdpb ] )
        }  // if( fbAddStatusToEvent )

        /// Prepare trigger window limits in ns for data selection
        Double_t dWinBeg =
          gdpbv100::kdClockCycleSizeNs * (*itTrigger[uGdpb]).GetFullGdpbTs()
          + fdStarTriggerDelay[uGdpb];
        Double_t dWinEnd =
          gdpbv100::kdClockCycleSizeNs * (*itTrigger[uGdpb]).GetFullGdpbTs()
          + fdStarTriggerDelay[uGdpb] + fdStarTriggerWinSize[uGdpb];
        Double_t dDeadEnd =
          gdpbv100::kdClockCycleSizeNs * (*itTrigger[uGdpb]).GetFullGdpbTs()
          + fdStarTriggerDeadtime[uGdpb];
        Double_t dWinBegErrors = dWinBeg - 2 * gdpbv100::kdEpochInNs;

        /// Loop on important errors buffer and select all from "last event" or 2 Epoch before the trigger window to "end of trigger window"
        UInt_t uNbErrorsInEventGdpb = 0;
        while (itErrorMessStart[uGdpb] != fvvBufferMajorAsicErrors[uGdpb].end()
               && (*itErrorMessStart[uGdpb]).GetFullTimeNs() < dWinBegErrors) {
          ++itErrorMessStart[uGdpb];
        }  // while( not at buffer end && (*itErrorMessStart[ uGdpb ]).GetFullTimeNs() < dWinBeg - 2 * gdpbv100::kdEpochInNs )
        while (itErrorMessStart[uGdpb] != fvvBufferMajorAsicErrors[uGdpb].end()
               && (*itErrorMessStart[uGdpb]).GetFullTimeNs() < dWinEnd) {
          /// Add errors to event only until the limit per gDPB
          if (uNbErrorsInEventGdpb < kuMaxNbErrorsPerGdpbPerEvent)
            starSubEvent.AddMsg((*itErrorMessStart[uGdpb]));

          ++itErrorMessStart[uGdpb];
          ++uNbErrorsInEventGdpb;
        }  // while( not at buffer end && (*itErrorMessStart[ uGdpb ]).GetFullTimeNs() < dWinEnd )

        /// Loop on data and select data fitting the trigger window
        /// Also advance start iterator to end of deadtime
        std::vector<gdpbv100::FullMessage>::iterator itFirstMessOutOfDeadtime =
          itMessStart[uGdpb];
        while (itMessStart[uGdpb] != fvvBufferMessages[uGdpb].end()
               && ((*itMessStart[uGdpb]).GetFullTimeNs() < dDeadEnd
                   || (*itMessStart[uGdpb]).GetFullTimeNs() < dWinEnd)) {
          Double_t dMessTime = (*itMessStart[uGdpb]).GetFullTimeNs();
          /*
               if( 5916 == fulCurrentTsIndex )
                  LOG(info) << Form( "gDPB %2u Match Mess %12f Start %12f Stop %12f dStart %6f dStop %6f",
                                    uGdpb, dMessTime,
                                    dWinBeg, dWinEnd,
                                    dMessTime - dWinBeg, dWinEnd - dMessTime )
                             << ( dWinBeg < dMessTime && dMessTime < dWinEnd ? " IN" : "" );
*/

          /// Fill Histo if monitor mode
          if (fbMonitorMode) {
            fvhHitsTimeToTriggerRaw[uGdpb]->Fill(
              dMessTime
              - gdpbv100::kdClockCycleSizeNs
                  * (*itTrigger[uGdpb]).GetFullGdpbTs());
            if (fbDebugMonitorMode
                && gdpbv100::MSG_HIT
                     == (*itMessStart[uGdpb]).getMessageType()) {
              UInt_t uTot        = (*itMessStart[uGdpb]).getGdpbHit32Tot();
              Double_t dTimeDiff = dMessTime
                                   - gdpbv100::kdClockCycleSizeNs
                                       * (*itTrigger[uGdpb]).GetFullGdpbTs();
              if (fuMinTotPulser < uTot && uTot < fuMaxTotPulser
                  && TMath::Abs(dTimeDiff) < 20e3)
                fvhHitsTimeToTriggerRawPulser[uGdpb]->Fill(dTimeDiff, uTot);
            }  // if( fbDebugMonitorMode && gdpbv100::MSG_HIT == (*itMessStart[ uGdpb ]).getMessageType() )
          }    // if( fbMonitorMode )


          /// If in trigger window, add to event
          if (dWinBeg < dMessTime && dMessTime < dWinEnd) {
            starSubEvent.AddMsg((*itMessStart[uGdpb]));
            /// Fill Histo if monitor mode
            if (fbMonitorMode) {
              fvhHitsTimeToTriggerSel[uGdpb]->Fill(
                dMessTime
                - gdpbv100::kdClockCycleSizeNs
                    * (*itTrigger[uGdpb]).GetFullGdpbTs());
              if (fbDebugMonitorMode) {
                fvhHitsTimeToTriggerSelVsDaq[uGdpb]->Fill(
                  dMessTime
                    - gdpbv100::kdClockCycleSizeNs
                        * (*itTrigger[uGdpb]).GetFullGdpbTs(),
                  usFirstStarDaqCmd);
                fvhHitsTimeToTriggerSelVsTrig[uGdpb]->Fill(
                  dMessTime
                    - gdpbv100::kdClockCycleSizeNs
                        * (*itTrigger[uGdpb]).GetFullGdpbTs(),
                  usFirstStarTrigCmd);
              }  // if( fbDebugMonitorMode )
            }    // if( fbMonitorMode )
          }      // if( dWinBeg < dMessTime && dMessTime < dWinEnd )

          /// Needed to catch the case where deadtime finishes before the trigger window
          if (dMessTime < dDeadEnd) ++itFirstMessOutOfDeadtime;

          ++itMessStart[uGdpb];
        }  // while( not at buffer end && (not out of deadtime || not out of trigg win ) )
        itMessStart[uGdpb] = itFirstMessOutOfDeadtime;

        /// Advance iterator on trigger
        ++itTrigger[uGdpb];
      }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

      /// Set Mean trigger in event header
      dMeanTriggerGdpbTs /= fuNrOfGdpbs;
      dMeanTriggerStarTs /= fuNrOfGdpbs;
      CbmTofStarTrigger2019 meanTrigger(
        static_cast<ULong64_t>(dMeanTriggerGdpbTs),
        static_cast<ULong64_t>(dMeanTriggerStarTs),
        uFirstStarToken,
        usFirstStarDaqCmd,
        usFirstStarTrigCmd);
      starSubEvent.SetTrigger(meanTrigger);

      /// Add event to event buffer
      fvEventsBuffer.push_back(starSubEvent);
    }  // if( kTRUE == bAllSectorsMatch )
    else {
      /// No = at least 1 missed a trigger
      /// Check which gDPB/sector has the trigger with the smallest gDPB TS
      std::vector<CbmTofStarTrigger2019>::iterator itEarliestTrigger;
      ULong64_t ulEarliestGdpbTs = 0xFFFFFFFFFFFFFFFFUL;
      for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
        if (itTrigger[uGdpb] != fvvBufferTriggers[uGdpb].end()
            && ((*itTrigger[uGdpb]).GetFullGdpbTs() < ulEarliestGdpbTs
                || 0xFFFFFFFFFFFFFFFFUL
                     == (*itTrigger[uGdpb]).GetFullGdpbTs())) {
          itEarliestTrigger = itTrigger[uGdpb];
          ulEarliestGdpbTs  = (*itTrigger[uGdpb]).GetFullGdpbTs();
        }  // if( not at end of buffer && (*itTrigger[ uGdpb ]).GetFullGdpbTs() < ulEarliestGdpbTs )
      }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
      UInt_t uEarliestStarToken      = (*itEarliestTrigger).GetStarToken();
      UShort_t usEarliestStarDaqCmd  = (*itEarliestTrigger).GetStarDaqCmd();
      UShort_t usEarliestStarTrigCmd = (*itEarliestTrigger).GetStarTrigCmd();
      ULong64_t ulEarliestFullGdpbTs = (*itEarliestTrigger).GetFullGdpbTs();

      /// Find all gDPB with a matching trigger
      UInt_t uNrOfMatchedGdpbs = 0;
      std::vector<Bool_t> vbMatchingTrigger(fuNrOfGdpbs, kFALSE);  /// [sector]
      Double_t dMeanTriggerGdpbTs = 0;
      Double_t dMeanTriggerStarTs = 0;
      for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
        if (itTrigger[uGdpb] != fvvBufferTriggers[uGdpb].end()
            && (*itTrigger[uGdpb]).GetStarToken() == uEarliestStarToken
            && (*itTrigger[uGdpb]).GetStarDaqCmd() == usEarliestStarDaqCmd
            && (*itTrigger[uGdpb]).GetStarTrigCmd() == usEarliestStarTrigCmd) {
          uNrOfMatchedGdpbs++;
          vbMatchingTrigger[uGdpb] = kTRUE;

          if (fbMonitorMode && fbDebugMonitorMode) {
            Long64_t ilTriggerDt =
              static_cast<Long64_t>((*itTrigger[uGdpb]).GetFullGdpbTs())
              - static_cast<Long64_t>(ulEarliestFullGdpbTs);
            fvhTriggerDt[uGdpb]->Fill(ilTriggerDt);
          }  // if( fbMonitorMode && fbDebugMonitorMode )

          dMeanTriggerGdpbTs += (*itTrigger[uGdpb]).GetFullGdpbTs();
          dMeanTriggerStarTs += (*itTrigger[uGdpb]).GetFullStarTs();
        }  // if matching trigger
      }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

      /// Compute the mean trigger from all matching triggers
      dMeanTriggerGdpbTs /= uNrOfMatchedGdpbs;
      dMeanTriggerStarTs /= uNrOfMatchedGdpbs;
      CbmTofStarTrigger2019 meanTrigger(
        static_cast<ULong64_t>(dMeanTriggerGdpbTs),
        static_cast<ULong64_t>(dMeanTriggerStarTs),
        (*itEarliestTrigger).GetStarToken(),
        (*itEarliestTrigger).GetStarDaqCmd(),
        (*itEarliestTrigger).GetStarTrigCmd());

      /// Create event & add to header the mean trigger, the "incomplete event" flag
      CbmTofStarSubevent2019
        starSubEvent;  /// Mean trigger is set latter, SourceId 0 for the full wheel!
      starSubEvent.SetTrigger(meanTrigger);
      starSubEvent.SetIncompleteEventFlag();

      /// Loop gDPB/sector
      for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
        /// Check if this gDPB/sector has a matching trigger
        /// Select trigger time => If matching trigger, use its TS, otherwise use the calulated mean trigger TS
        /// TODO: add option and code to use instead of the mean a majority trigger!!
        ULong64_t ulTriggerTime = static_cast<ULong64_t>(dMeanTriggerGdpbTs);
        if (kTRUE == vbMatchingTrigger[uGdpb]) {
          /// Select trigger time => If matching trigger, use its TS, otherwise use the calulated mean trigger TS
          ulTriggerTime = (*itTrigger[uGdpb]).GetFullGdpbTs();

          /// If matching trigger, convert STAR trigger to 4 full messages and insert in buffer
          std::vector<gdpbv100::FullMessage> vTrigMess =
            (*itTrigger[uGdpb]).GetGdpbMessages();
          for (std::vector<gdpbv100::FullMessage>::iterator itMess =
                 vTrigMess.begin();
               itMess != vTrigMess.end();
               ++itMess) {
            starSubEvent.AddMsg((*itMess));
          }  // for( std::vector< gdpbv100::FullMessage >::iterator itMess = vTrigMess.begin(); itMess != vTrigMess.end(); ++ itMess )
        }    // if( kTRUE == vbMatchingTrigger[ uGdpb ] )
        else if (fbMonitorMode && fbDebugMonitorMode) {
          /// Fill missing trigger per sector evolution plot only in debug monitor mode
          fhMissingTriggersEvolution->Fill(dMeanTriggerGdpbTs / 1e9 / 60.0,
                                           fUnpackPar->GetGdpbToSectorOffset()
                                             + uGdpb);
        }  // else if( fbMonitorMode && fbDebugMonitorMode )

        if (fbAddStatusToEvent) {
          /// Find Last status field update before the trigger, convert it to 7 status pattern messages and insert in buffer
          /// Iterator may reach the end if last update before trigger is also last update in general
          /// Used proper order of conditions to avoid segfault
          /// Loop until we find first update not matching or the end of the vector
          while (fvSectorStatusPattern[uGdpb].end() != itStatUpdtStart[uGdpb]
                 && ((*itStatUpdtStart[uGdpb]).first)
                        * gdpbv100::kuCoarseCounterSize
                      <= ulTriggerTime) {
            ++itStatUpdtStart[uGdpb];
          }  // while( not at the end of updates vector AND Update time under trigger time )
          /// Check that there was at least one update before this trigger
          if (fvSectorStatusPattern[uGdpb].begin() != itStatUpdtStart[uGdpb]) {
            /// Go back to last matching update
            --itStatUpdtStart[uGdpb];
            /// Generate and insert the messages
            uint16_t usGdpbId = fUnpackPar->GetGdpbId(uGdpb);
            for (uint32_t uIdx = 0; uIdx < kuNbMsgPerPattern; ++uIdx)
              starSubEvent.AddMsg(
                CreateStatusMessage(usGdpbId, uIdx, (*itStatUpdtStart[uGdpb])));
          }  // if( fvSectorStatusPattern[ uGdpb ].begin() != itStatUpdtStart[ uGdpb ] )
        }  // if( fbAddStatusToEvent )

        /// Prepare trigger window limits in ns for data selection
        Double_t dWinBeg = gdpbv100::kdClockCycleSizeNs * ulTriggerTime
                           + fdStarTriggerDelay[uGdpb];
        Double_t dWinEnd = gdpbv100::kdClockCycleSizeNs * ulTriggerTime
                           + fdStarTriggerDelay[uGdpb]
                           + fdStarTriggerWinSize[uGdpb];
        Double_t dDeadEnd = gdpbv100::kdClockCycleSizeNs * ulTriggerTime
                            + fdStarTriggerDeadtime[uGdpb];
        Double_t dWinBegErrors = dWinBeg - 2 * gdpbv100::kdEpochInNs;

        /// Loop on important errors buffer and select all from "last event" or 2 Epoch before the trigger window to "end of trigger window"
        UInt_t uNbErrorsInEventGdpb = 0;
        while (itErrorMessStart[uGdpb] != fvvBufferMajorAsicErrors[uGdpb].end()
               && (*itErrorMessStart[uGdpb]).GetFullTimeNs() < dWinBegErrors) {
          ++itErrorMessStart[uGdpb];
        }  // while( not at buffer end && (*itErrorMessStart[ uGdpb ]).GetFullTimeNs() < dWinBeg - 2 * gdpbv100::kdEpochInNs )
        while (itErrorMessStart[uGdpb] != fvvBufferMajorAsicErrors[uGdpb].end()
               && (*itErrorMessStart[uGdpb]).GetFullTimeNs() < dWinEnd) {
          /// Add errors to event only until the limit per gDPB
          if (uNbErrorsInEventGdpb < kuMaxNbErrorsPerGdpbPerEvent)
            starSubEvent.AddMsg((*itErrorMessStart[uGdpb]));

          ++itErrorMessStart[uGdpb];
          ++uNbErrorsInEventGdpb;
        }  // while( not at buffer end && (*itErrorMessStart[ uGdpb ]).GetFullTimeNs() < dWinEnd )

        /// Loop on data and select data fitting the trigger window
        /// Also advance start iterator to end of deadtime
        std::vector<gdpbv100::FullMessage>::iterator itFirstMessOutOfDeadtime =
          itMessStart[uGdpb];
        while (itMessStart[uGdpb] != fvvBufferMessages[uGdpb].end()
               && ((*itMessStart[uGdpb]).GetFullTimeNs() < dDeadEnd
                   || (*itMessStart[uGdpb]).GetFullTimeNs() < dWinEnd)) {
          Double_t dMessTime = (*itMessStart[uGdpb]).GetFullTimeNs();
          /*
               if( 5916 == fulCurrentTsIndex )
      LOG(info) << Form( "gDPB %2u Miss Mess %12f Start %12f Stop %12f",
                        uGdpb, dMessTime,
                        dWinBeg,
                        dWinEnd )
                 << ( dWinBeg < dMessTime && dMessTime < dWinEnd ? " IN" : "" );
*/
          /// Fill Histo if monitor mode
          if (fbMonitorMode) {
            fvhHitsTimeToTriggerRaw[uGdpb]->Fill(
              dMessTime - gdpbv100::kdClockCycleSizeNs * ulTriggerTime);
            if (fbDebugMonitorMode
                && gdpbv100::MSG_HIT
                     == (*itMessStart[uGdpb]).getMessageType()) {
              UInt_t uTot = (*itMessStart[uGdpb]).getGdpbHit32Tot();
              Double_t dTimeDiff =
                dMessTime - gdpbv100::kdClockCycleSizeNs * ulTriggerTime;
              if (fuMinTotPulser < uTot && uTot < fuMaxTotPulser
                  && TMath::Abs(dTimeDiff) < 20e3)
                fvhHitsTimeToTriggerRawPulser[uGdpb]->Fill(dTimeDiff, uTot);
            }  // if( fbDebugMonitorMode && gdpbv100::MSG_HIT == (*itMessStart[ uGdpb ]).getMessageType() )
          }    // if( fbMonitorMode )

          /// If in trigger window, add to event
          if (dWinBeg < dMessTime && dMessTime < dWinEnd) {
            starSubEvent.AddMsg((*itMessStart[uGdpb]));
            /// Fill Histo if monitor mode
            if (fbMonitorMode) {
              fvhHitsTimeToTriggerSel[uGdpb]->Fill(
                dMessTime - gdpbv100::kdClockCycleSizeNs * ulTriggerTime);
              if (fbDebugMonitorMode) {
                fvhHitsTimeToTriggerSelVsDaq[uGdpb]->Fill(
                  dMessTime - gdpbv100::kdClockCycleSizeNs * ulTriggerTime,
                  usEarliestStarDaqCmd);
                fvhHitsTimeToTriggerSelVsTrig[uGdpb]->Fill(
                  dMessTime - gdpbv100::kdClockCycleSizeNs * ulTriggerTime,
                  usEarliestStarTrigCmd);
              }  // if( fbDebugMonitorMode )
            }    // if( fbMonitorMode )
          }      // if( dWinBeg < dMessTime && dMessTime < dWinEnd )

          /// Needed to catch the case where deadtime finishes before the trigger window
          if (dMessTime < dDeadEnd) ++itFirstMessOutOfDeadtime;

          ++itMessStart[uGdpb];
        }  // while( not at buffer end && (not out of deadtime || not out of trigg win ) )
        itMessStart[uGdpb] = itFirstMessOutOfDeadtime;

        /// Advance iterator for gDPB/sector with matching trigger
        if (kTRUE == vbMatchingTrigger[uGdpb]) ++itTrigger[uGdpb];
      }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

      /// Add event to event buffer
      fvEventsBuffer.push_back(starSubEvent);
    }  // else of if( kTRUE == bAllSectorsMatch )

    /// Check if all gDPB/sectors are at end of their trigger buffer
    bAllSectAllTriggDone = kTRUE;
    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
      /// => Check can be stopped as soon as a gDPB/sector not at end is found!
      if (fvvBufferTriggers[uGdpb].end() != itTrigger[uGdpb]) {
        bAllSectAllTriggDone = kFALSE;
        break;
      }  // if( fvvBufferTriggers[ uGdpb ].end() != (*itTrigger[ uGdpb ]) )
    }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  }      // while( kFALSE == bAllSectAllTriggDone )

  return kTRUE;
}

// -------------------------------------------------------------------------

Bool_t CbmStar2019EventBuilderEtofAlgo::CreateHistograms() {
  /// Create sector related histograms
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    UInt_t uSector      = fUnpackPar->GetGdpbToSectorOffset() + uGdpb;
    std::string sFolder = Form("sector%2u", uSector);

    LOG(info) << "gDPB " << uGdpb << " is " << sFolder;

    fvhHitsTimeToTriggerRaw.push_back(
      new TH1D(Form("hHitsTimeToTriggerRawSect%2u", uSector),
               Form("Time to trigger for all neighboring hits in sector %2u; t "
                    "- Ttrigg [ns]; Hits []",
                    uSector),
               2000,
               -5000,
               5000));

    UInt_t uNbBinsDtSel = fdStarTriggerWinSize[uGdpb];
    Double_t dMaxDtSel =
      fdStarTriggerDelay[uGdpb] + fdStarTriggerWinSize[uGdpb];
    fvhHitsTimeToTriggerSel.push_back(
      new TH1D(Form("hHitsTimeToTriggerSelSect%2u", uSector),
               Form("Time to trigger for all selected hits in sector %2u; t - "
                    "Ttrigg [ns]; Hits []",
                    uSector),
               uNbBinsDtSel,
               fdStarTriggerDelay[uGdpb],
               dMaxDtSel));

    /// Add pointers to the vector with all histo for access by steering class
    AddHistoToVector(fvhHitsTimeToTriggerRaw[uGdpb], sFolder);
    AddHistoToVector(fvhHitsTimeToTriggerSel[uGdpb], sFolder);

    if (kTRUE == fbDebugMonitorMode) {
      fvhHitsTimeToTriggerRawPulser.push_back(new TH2D(
        Form("hHitsTimeToTriggerRawPulserSect%2u", uSector),
        Form("Time to trigger for all neighboring hits within pulser TOT range "
             "in sector %2u; t - Ttrigg [ns]; TOT [bins]; Hits []",
             uSector),
        2000,
        -5000,
        5000,
        256,
        0,
        256));

      fvhHitsTimeToTriggerSelVsDaq.push_back(
        new TH2D(Form("hHitsTimeToTriggerSelVsDaqSect%2u", uSector),
                 Form("Time to trigger for all selected hits vs DAQ CMD in "
                      "sector %2u; t - Ttrigg [ns]; DAQ CMD []; Hits []",
                      uSector),
                 uNbBinsDtSel,
                 fdStarTriggerDelay[uGdpb],
                 dMaxDtSel,
                 16,
                 0.,
                 16.));

      fvhHitsTimeToTriggerSelVsTrig.push_back(
        new TH2D(Form("hHitsTimeToTriggerSelVsTrigSect%2u", uSector),
                 Form("Time to trigger for all selected hits vs TRIG CMD in "
                      "sector %2u; t - Ttrigg [ns]; TRIG CMD []; Hits []",
                      uSector),
                 uNbBinsDtSel,
                 fdStarTriggerDelay[uGdpb],
                 dMaxDtSel,
                 16,
                 0.,
                 16.));

      fvhTriggerDt.push_back(new TH1I(
        Form("hTriggerDtSect%2u", uSector),
        Form("Trigger time difference between sector %2u and the first sector, "
             "full events only; Ttrigg%2u - TtriggRef [Clk]; events []",
             uSector,
             uSector),
        200,
        -100,
        100));

      /// FIXME: hardcoded nb of MS in TS (include overlap)
      /// as this number is known only later when 1st TS is received
      UInt_t uNbBinsInTs = fdMsSizeInNs * 111 / 1000. / 10.;
      UInt_t uNbBinsInMs = fdMsSizeInNs * 20 / 1000. / 10.;

      fvhTriggerDistributionInTs.push_back(
        new TH1I(Form("hTriggerDistInTsSect%2u", uSector),
                 Form("Trigger distribution inside TS in sector %2u; Time in "
                      "TS [us]; Trigger [];",
                      uSector),
                 uNbBinsInTs,
                 -0.5 - fdMsSizeInNs * 10 / 1000.,
                 fdMsSizeInNs * 101 / 1000. - 0.5));

      fvhTriggerDistributionInMs.push_back(
        new TH1I(Form("hTriggerDistInMsSect%2u", uSector),
                 Form("Trigger distribution inside MS in sector %2u; Time in "
                      "MS [us]; Trigger [];",
                      uSector),
                 uNbBinsInMs,
                 -0.5 - fdMsSizeInNs * 10 / 1000.,
                 fdMsSizeInNs * 10 / 1000. - 0.5));

      fvhMessDistributionInMs.push_back(
        new TH1I(Form("hMessDistInMsSect%2u", uSector),
                 Form("Messages distribution inside MS in sector %2u; Time in "
                      "MS [us]; Trigger [];",
                      uSector),
                 uNbBinsInMs,
                 -0.5 - fdMsSizeInNs * 10 / 1000.,
                 fdMsSizeInNs * 10 / 1000. - 0.5));

      /// Add pointers to the vector with all histo for access by steering class
      AddHistoToVector(fvhHitsTimeToTriggerRawPulser[uGdpb], sFolder);
      AddHistoToVector(fvhHitsTimeToTriggerSelVsDaq[uGdpb], sFolder);
      AddHistoToVector(fvhHitsTimeToTriggerSelVsTrig[uGdpb], sFolder);
      AddHistoToVector(fvhTriggerDt[uGdpb], sFolder);
      AddHistoToVector(fvhTriggerDistributionInTs[uGdpb], sFolder);
      AddHistoToVector(fvhTriggerDistributionInMs[uGdpb], sFolder);
      AddHistoToVector(fvhMessDistributionInMs[uGdpb], sFolder);
    }  // if( kTRUE == fbDebugMonitorMode )
  }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  /// Create event builder related histograms
  fhEventNbPerTs = new TH1I("hEventNbPerTs",
                            "Number of Events per TS; Events []; TS []",
                            1000,
                            0,
                            1000);

  fhEventSizeDistribution =
    new TH1I("hEventSizeDistribution",
             "Event size distribution; Event size [byte]; Events []",
             CbmTofStarSubevent2019::GetMaxOutputSize() / 8,
             0,
             CbmTofStarSubevent2019::GetMaxOutputSize());

  fhEventSizeEvolution = new TProfile(
    "hEventSizeEvolution",
    "Event size evolution; Time in run [min]; mean Event size [byte];",
    14400,
    0,
    14400);

  fhEventNbEvolution =
    new TH1I("hEventNbEvolution",
             "Event number evolution; Time in run [min]; Events [];",
             14400,
             0,
             14400);

  /// Add pointers to the vector with all histo for access by steering class
  AddHistoToVector(fhEventNbPerTs, "eventbuilder");
  AddHistoToVector(fhEventSizeDistribution, "eventbuilder");
  AddHistoToVector(fhEventSizeEvolution, "eventbuilder");
  AddHistoToVector(fhEventNbEvolution, "eventbuilder");

  if (kTRUE == fbDebugMonitorMode) {
    /// FIXME: hardcoded nb of MS in TS (include overlap)
    /// as this number is known only later when 1st TS is received
    UInt_t uNbBinsInTs = fdMsSizeInNs * 101 / 1000. / 10.;

    fhEventNbDistributionInTs = new TH1I(
      "hEventNbDistributionInTs",
      "Event number distribution inside TS; Time in TS [us]; Events [];",
      uNbBinsInTs,
      -0.5,
      fdMsSizeInNs * 101 / 1000. - 0.5);

    fhEventSizeDistributionInTs =
      new TProfile("hEventSizeDistributionInTs",
                   "Event size distribution inside TS; Time in TS [us]; mean "
                   "Event size [Byte];",
                   uNbBinsInTs,
                   -0.5,
                   fdMsSizeInNs * 101 / 1000. - 0.5);

    fhRawTriggersStats =
      new TH2I("hRawTriggersStats",
               "Raw triggers statistics per sector; ; Sector []; Messages []",
               5,
               0,
               5,
               12,
               13,
               25);
    fhRawTriggersStats->GetXaxis()->SetBinLabel(1, "A");
    fhRawTriggersStats->GetXaxis()->SetBinLabel(2, "B");
    fhRawTriggersStats->GetXaxis()->SetBinLabel(3, "C");
    fhRawTriggersStats->GetXaxis()->SetBinLabel(4, "D");
    fhRawTriggersStats->GetXaxis()->SetBinLabel(5, "F");

    fhRawTriggersStatsCore = new TH2I(
      "hRawTriggersStatsCore",
      "Raw triggers in Core MS statistics per sector; ; Sector []; Messages []",
      5,
      0,
      5,
      12,
      13,
      25);
    fhRawTriggersStatsCore->GetXaxis()->SetBinLabel(1, "A");
    fhRawTriggersStatsCore->GetXaxis()->SetBinLabel(2, "B");
    fhRawTriggersStatsCore->GetXaxis()->SetBinLabel(3, "C");
    fhRawTriggersStatsCore->GetXaxis()->SetBinLabel(4, "D");
    fhRawTriggersStatsCore->GetXaxis()->SetBinLabel(5, "F");

    fhRawTriggersStatsOver = new TH2I("hRawTriggersStatsOver",
                                      "Raw triggers in Overlap MS statistics "
                                      "per sector; ; Sector []; Messages []",
                                      5,
                                      0,
                                      5,
                                      12,
                                      13,
                                      25);
    fhRawTriggersStatsOver->GetXaxis()->SetBinLabel(1, "A");
    fhRawTriggersStatsOver->GetXaxis()->SetBinLabel(2, "B");
    fhRawTriggersStatsOver->GetXaxis()->SetBinLabel(3, "C");
    fhRawTriggersStatsOver->GetXaxis()->SetBinLabel(4, "D");
    fhRawTriggersStatsOver->GetXaxis()->SetBinLabel(5, "F");

    fhRawTriggersStatsSel = new TH2I(
      "hRawTriggersStatsSel",
      "Selected triggers statistics per sector; ; Sector []; Messages []",
      3,
      0,
      3,
      12,
      13,
      25);
    fhRawTriggersStatsSel->GetXaxis()->SetBinLabel(1, "All");
    fhRawTriggersStatsSel->GetXaxis()->SetBinLabel(2, "Core");
    fhRawTriggersStatsSel->GetXaxis()->SetBinLabel(3, "Over");

    fhMissingTriggersEvolution =
      new TH2I("hMissingTriggersEvolution",
               "Missing trigger counts per sector vs time in run; Time in run "
               "[min]; Sector []; Missing triggers []",
               14400,
               0,
               14400,
               12,
               13,
               25);

    /// Add pointers to the vector with all histo for access by steering class
    AddHistoToVector(fhEventNbDistributionInTs, "eventbuilder");
    AddHistoToVector(fhEventSizeDistributionInTs, "eventbuilder");
    AddHistoToVector(fhRawTriggersStats, "eventbuilder");
    AddHistoToVector(fhRawTriggersStatsCore, "eventbuilder");
    AddHistoToVector(fhRawTriggersStatsOver, "eventbuilder");
    AddHistoToVector(fhRawTriggersStatsSel, "eventbuilder");
    AddHistoToVector(fhMissingTriggersEvolution, "eventbuilder");
  }  // if( kTRUE == fbDebugMonitorMode )

  /// Canvases
  Double_t w = 10;
  Double_t h = 10;

  /// Raw Time to trig for all sectors
  fcTimeToTrigRaw =
    new TCanvas("cTimeToTrigRaw", "Raw Time to trig for all sectors", w, h);
  fcTimeToTrigRaw->Divide(2, fuNrOfGdpbs / 2);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fcTimeToTrigRaw->cd(1 + uGdpb);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();
    fvhHitsTimeToTriggerRaw[uGdpb]->Draw();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  AddCanvasToVector(fcTimeToTrigRaw, "canvases");

  /// Selected Time to trig for all sectors
  fcTimeToTrigSel = new TCanvas(
    "cTimeToTrigSel", "Selected Time to trig for all sectors", w, h);
  fcTimeToTrigSel->Divide(2, fuNrOfGdpbs / 2);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fcTimeToTrigSel->cd(1 + uGdpb);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();
    fvhHitsTimeToTriggerSel[uGdpb]->Draw();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  AddCanvasToVector(fcTimeToTrigSel, "canvases");

  if (kTRUE == fbDebugMonitorMode) {
    /// Trigger time to MS start for all sectors
    fcTrigDistMs = new TCanvas(
      "cTrigDistMs", "Trigger time to MS start for all sectors", w, h);
    fcTrigDistMs->Divide(2, fuNrOfGdpbs / 2);
    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
      fcTrigDistMs->cd(1 + uGdpb);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fvhTriggerDistributionInMs[uGdpb]->Draw();
    }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
    AddCanvasToVector(fcTrigDistMs, "canvases");

    /// Message time to MS start for all sectors
    fcMessDistMs = new TCanvas(
      "cMessDistMs", "Message time to MS start for all sectors", w, h);
    fcMessDistMs->Divide(2, fuNrOfGdpbs / 2);
    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
      fcMessDistMs->cd(1 + uGdpb);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fvhMessDistributionInMs[uGdpb]->Draw();
    }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
    AddCanvasToVector(fcMessDistMs, "canvases");

    fcTriggerStats =
      new TCanvas("cTriggerStats", "Trigger statistics per sector", w, h);
    fcTriggerStats->Divide(2, 2);

    fcTriggerStats->cd(1);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fhRawTriggersStats->Draw("colz text");

    fcTriggerStats->cd(2);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fhRawTriggersStatsCore->Draw("colz text");

    fcTriggerStats->cd(3);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fhRawTriggersStatsOver->Draw("colz text");

    fcTriggerStats->cd(4);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fhRawTriggersStatsSel->Draw("colz text");

    AddCanvasToVector(fcTriggerStats, "canvases");
  }  // if( kTRUE == fbDebugMonitorMode )

  /// Event building process summary and statistics
  fcEventBuildStats =
    new TCanvas("cEvtBuildStats", "Event building statistics", w, h);
  if (kTRUE == fbDebugMonitorMode)
    fcEventBuildStats->Divide(2, 3);
  else
    fcEventBuildStats->Divide(2, 2);

  fcEventBuildStats->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhEventNbPerTs->Draw();

  fcEventBuildStats->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhEventSizeDistribution->Draw();

  fcEventBuildStats->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhEventSizeEvolution->Draw();

  fcEventBuildStats->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhEventNbEvolution->Draw();

  if (kTRUE == fbDebugMonitorMode) {
    fcEventBuildStats->cd(5);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();
    fhEventNbDistributionInTs->Draw();

    fcEventBuildStats->cd(6);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();
    fhEventSizeDistributionInTs->Draw();
  }  // if( kTRUE == fbDebugMonitorMode )

  AddCanvasToVector(fcEventBuildStats, "canvases");

  return kTRUE;
}
Bool_t CbmStar2019EventBuilderEtofAlgo::FillHistograms() {
  UInt_t uNbEvents = fvEventsBuffer.size();
  fhEventNbPerTs->Fill(uNbEvents);

  for (UInt_t uEvent = 0; uEvent < uNbEvents; ++uEvent) {
    UInt_t uEventSize      = fvEventsBuffer[uEvent].GetEventSize();
    Double_t dEventTimeSec = fvEventsBuffer[uEvent].GetEventTimeSec();
    Double_t dEventTimeMin = dEventTimeSec / 60.0;

    fhEventSizeDistribution->Fill(uEventSize);
    if (fbDebugMonitorMode)
      fhEventSizeEvolution->Fill(dEventTimeSec, uEventSize);
    else
      fhEventSizeEvolution->Fill(dEventTimeMin, uEventSize);
    if (fbDebugMonitorMode)
      fhEventNbEvolution->Fill(dEventTimeSec);
    else
      fhEventNbEvolution->Fill(dEventTimeMin);

    if (kTRUE == fbDebugMonitorMode) {
      Double_t dEventTimeInTs =
        (fvEventsBuffer[uEvent].GetTrigger().GetFullGdpbTs()
           * gdpbv100::kdClockCycleSizeNs
         - fdTsStartTime)
        / 1000.0;

      fhEventNbDistributionInTs->Fill(dEventTimeInTs);
      fhEventSizeDistributionInTs->Fill(dEventTimeInTs, uEventSize);
    }  // if( kTRUE == fbDebugMonitorMode )
  }    // for( UInt_t uEvent = 0; uEvent < uNbEvents; ++uEvent )

  return kTRUE;
}
Bool_t CbmStar2019EventBuilderEtofAlgo::ResetHistograms() {
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fvhHitsTimeToTriggerRaw[uGdpb]->Reset();
    fvhHitsTimeToTriggerSel[uGdpb]->Reset();

    if (kTRUE == fbDebugMonitorMode) {
      fvhHitsTimeToTriggerRawPulser[uGdpb]->Reset();
      fvhHitsTimeToTriggerSelVsDaq[uGdpb]->Reset();
      fvhHitsTimeToTriggerSelVsTrig[uGdpb]->Reset();
      fvhTriggerDt[uGdpb]->Reset();
      fvhTriggerDistributionInTs[uGdpb]->Reset();
      fvhTriggerDistributionInMs[uGdpb]->Reset();
      fvhMessDistributionInMs[uGdpb]->Reset();
    }  // if( kTRUE == fbDebugMonitorMode )
  }    // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  /// Create event builder related histograms
  fhEventNbPerTs->Reset();
  fhEventSizeDistribution->Reset();
  fhEventSizeEvolution->Reset();
  fhEventNbEvolution->Reset();

  if (kTRUE == fbDebugMonitorMode) {
    fhEventNbDistributionInTs->Reset();
    fhEventSizeDistributionInTs->Reset();
    fhRawTriggersStats->Reset();
    fhRawTriggersStatsCore->Reset();
    fhRawTriggersStatsOver->Reset();
    fhRawTriggersStatsSel->Reset();
    fhMissingTriggersEvolution->Reset();
  }  // if( kTRUE == fbDebugMonitorMode )

  return kTRUE;
}
// -------------------------------------------------------------------------
