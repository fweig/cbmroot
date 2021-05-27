// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmStar2019MonitorAlgo                           -----
// -----               Created 12.10.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmStar2019MonitorAlgo.h"

#include "CbmFormatMsHeaderPrintout.h"
#include "CbmStar2019TofPar.h"
#include "CbmTofAddress.h"
#include "CbmTofDetectorId_v14a.h"  // in cbmdata/tof

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TList.h>
#include <TPaveStats.h>
#include <TProfile.h>
#include <TROOT.h>
#include <TString.h>

#include <fstream>
#include <iomanip>
#include <iostream>

#include <stdint.h>

// -------------------------------------------------------------------------
CbmStar2019MonitorAlgo::CbmStar2019MonitorAlgo()
  : CbmStar2019Algo()
  ,
  /// From the class itself
  fbDebugMonitorMode(kFALSE)
  , fbIgnoreCriticalErrors(kFALSE)
  , fvbMaskedComponents()
  , fiSectorIndex(-1)
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
  , fuMinTotPulser(90)
  , fuMaxTotPulser(100)
  , fulCurrentTsIdx(0)
  , fulCurrentMsIdx(0)
  , fdTsStartTime(-1.0)
  , fdTsStopTimeCore(-1.0)
  , fdMsTime(-1.0)
  , fuMsIndex(0)
  , fmMsgCounter()
  , fuCurrentEquipmentId(0)
  , fuCurrDpbId(0)
  , fuCurrDpbIdx(0)
  , fuCurrSector(0)
  , fiRunStartDateTimeSec(-1)
  , fiBinSizeDatePlots(-1)
  , fuGet4Id(0)
  , fuGet4Nr(0)
  , fvulCurrentEpoch()
  , fvulCurrentEpochCycle()
  , fvulCurrentEpochFull()
  , fvmEpSupprBuffer()
  , fulStartTs(0)
  , fdStartTime(-1.0)
  , fdStartTimeMsSz(0.0)
  , ftStartTimeUnix(std::chrono::steady_clock::now())
  , fvulGdpbTsMsb()
  , fvulGdpbTsLsb()
  , fvulStarTsMsb()
  , fvulStarTsMid()
  , fvulGdpbTsFullLast()
  , fvulStarTsFullLast()
  , fvuStarTokenLast()
  , fvuStarDaqCmdLast()
  , fvuStarTrigCmdLast()
  , fbEpochSinceLastHit(kTRUE)
  , fuDuplicatesCount(0)
  , fmLastHit(0)
  , fuHistoryHistoSize(3600)
  , fhMessType(nullptr)
  , fhSysMessType(nullptr)
  , fhGet4MessType(nullptr)
  , fhGet4ChanScm(nullptr)
  , fhGet4ChanErrors(nullptr)
  , fhGet4EpochFlags(nullptr)
  , fhGdpbAsicSpiCounts(nullptr)
  , fhGdpbMessType(nullptr)
  , fhGdpbSysMessType(nullptr)
  , fhGdpbSysMessPattType(nullptr)
  , fhGdpbEpochFlags(nullptr)
  , fhGdpbEpochSyncEvo(nullptr)
  , fhGdpbEpochMissEvo(nullptr)
  , fhGdpbEndMsBufferNotEmpty(nullptr)
  , fhGdpbEndMsDataLost(nullptr)
  , fvhGdpbGet4MessType()
  , fvhGdpbGet4ChanScm()
  , fvhGdpbGet4ChanErrors()
  , fhMsgCntEvo(nullptr)
  , fhHitCntEvo(nullptr)
  , fhErrorCntEvo(nullptr)
  , fhLostEvtCntEvo(nullptr)
  , fhErrorFractEvo(nullptr)
  , fhLostEvtFractEvo(nullptr)
  , fhMsgCntPerMsEvo(nullptr)
  , fhHitCntPerMsEvo(nullptr)
  , fhErrorCntPerMsEvo(nullptr)
  , fhLostEvtCntPerMsEvo(nullptr)
  , fhErrorFractPerMsEvo(nullptr)
  , fhLostEvtFractPerMsEvo(nullptr)
  , fvhRawFt_gDPB()
  , fvhRawCt_gDPB()
  , fvhRemapTot_gDPB()
  , fvhRemapChCount_gDPB()
  , fvhRemapChRate_gDPB()
  , fuNbMissmatchPattern()
  , fhNbMissPatternPerMs(nullptr)
  , fhPatternMissmatch(nullptr)
  , fhPatternEnable(nullptr)
  , fhPatternResync(nullptr)
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
  , fvhTokenMsgType()
  , fvhTriggerRate()
  , fvhCmdDaqVsTrig()
  , fvhStarTokenEvo()
  , fvhStarTrigGdpbTsEvo()
  , fvhStarTrigStarTsEvo()
  , fcSummary(nullptr)
  , fcSummaryGdpb(nullptr)
  , fvcSumGdpbGet4()
  , fcStarTrigTokenType(nullptr)
  , fcStarTriggerRate(nullptr)
  , fcStarTrigCmdDaqVsTrig(nullptr)
  , fcStarTrigStarTokenEvo(nullptr)
  , fcStarTrigGdpbTsEvo(nullptr)
  , fcStarTrigStarTsEvo(nullptr)
{
}
CbmStar2019MonitorAlgo::~CbmStar2019MonitorAlgo()
{
  /// Clear buffers
  fvmEpSupprBuffer.clear();
}

// -------------------------------------------------------------------------
Bool_t CbmStar2019MonitorAlgo::Init()
{
  LOG(info) << "Initializing mCBM T0 2019 monitor algo";

  return kTRUE;
}
void CbmStar2019MonitorAlgo::Reset() {}
void CbmStar2019MonitorAlgo::Finish()
{
  /// Printout Goodbye message and stats

  /// Write Output histos
}

// -------------------------------------------------------------------------
Bool_t CbmStar2019MonitorAlgo::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmStar2019MonitorAlgo";
  Bool_t initOK = ReInitContainers();

  return initOK;
}
Bool_t CbmStar2019MonitorAlgo::ReInitContainers()
{
  LOG(info) << "**********************************************";
  LOG(info) << "ReInit parameter containers for CbmStar2019MonitorAlgo";

  fUnpackPar = (CbmStar2019TofPar*) fParCList->FindObject("CbmStar2019TofPar");
  if (nullptr == fUnpackPar) return kFALSE;

  Bool_t initOK = InitParameters();

  return initOK;
}
TList* CbmStar2019MonitorAlgo::GetParList()
{
  if (nullptr == fParCList) fParCList = new TList();
  fUnpackPar = new CbmStar2019TofPar("CbmStar2019TofPar");
  fParCList->Add(fUnpackPar);

  return fParCList;
}
Bool_t CbmStar2019MonitorAlgo::InitParameters()
{
  LOG(info) << "Debug Monitor mode: " << (fbDebugMonitorMode ? "ON" : "OFF");

  if (kTRUE == fbIgnoreCriticalErrors)
    LOG(warning) << "Monitor set to ignore critical GET4 errors!!! No printout "
                    "will be delivered for those!!!!";

  fuNrOfGdpbs = fUnpackPar->GetNrOfGdpbs();
  LOG(info) << "Nr. of Tof GDPBs: " << fuNrOfGdpbs;

  fuNrOfFeePerGdpb = fUnpackPar->GetNrOfFeePerGdpb();
  LOG(info) << "Nr. of FEES per Tof GDPB: " << fuNrOfFeePerGdpb;

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

  /// Parameters for FLES containers processing
  fdMsSizeInNs = fUnpackPar->GetSizeMsInNs();
  LOG(info) << "Timeslice parameters: each MS is " << fdMsSizeInNs << " ns";

  /// Check if user requested to monitor only a single selected sector
  if (-1 < fiSectorIndex) {
    fiSectorIndex -= fUnpackPar->GetGdpbToSectorOffset();
    if (fuNrOfGdpbs <= static_cast<UInt_t>(fiSectorIndex))
      LOG(fatal) << "Selected sector out of bounds relative to parameter file: " << fiSectorIndex << " VS "
                 << fuNrOfGdpbs;
    else
      LOG(info) << "Selected sector " << fiSectorIndex + fUnpackPar->GetGdpbToSectorOffset()
                << " for single sector analysis";

    fuNrOfGdpbs = 1;
    fGdpbIdIndexMap.clear();
    fGdpbIdIndexMap[fUnpackPar->GetGdpbId(fiSectorIndex)] = 0;
  }  // if( -1 < fiSectorIndex )

  /// Internal status initialization
  fvulCurrentEpoch.resize(fuNrOfGdpbs, 0);
  fvulCurrentEpochCycle.resize(fuNrOfGdpbs, 0);
  fvulCurrentEpochFull.resize(fuNrOfGdpbs, 0);

  /// Histogramming variables initialization
  /// Pattern detection
  fvvbGdpbLastMissmatchPattern.resize(fuNrOfGdpbs);
  fvvbGdpbLastEnablePattern.resize(fuNrOfGdpbs);
  fvvbGdpbLastResyncPattern.resize(fuNrOfGdpbs);
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
    /// Pattern detection
    fvvbGdpbLastMissmatchPattern[uGdpb].resize(fuNrOfGet4PerGdpb, kFALSE);
    fvvbGdpbLastEnablePattern[uGdpb].resize(fuNrOfGet4PerGdpb, kTRUE);
    fvvbGdpbLastResyncPattern[uGdpb].resize(fuNrOfGet4PerGdpb, kFALSE);
    /// STAR Trigger decoding and monitoring
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

  if (kTRUE == fbDebugMonitorMode) fuNbMissmatchPattern.resize(fuNrOfGdpbs, 0);

  return kTRUE;
}
// -------------------------------------------------------------------------

void CbmStar2019MonitorAlgo::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Add to list
  fvMsComponentsList.push_back(component);

  LOG(info) << "CbmStar2019MonitorAlgo::AddMsComponentToList => Component " << component << " with detector ID 0x"
            << std::hex << usDetectorId << std::dec << " added to list";
}
// -------------------------------------------------------------------------

Bool_t CbmStar2019MonitorAlgo::ProcessTs(const fles::Timeslice& ts)
{
  fulCurrentTsIdx = ts.index();
  fdTsStartTime   = static_cast<Double_t>(ts.descriptor(0, 0).idx);

  /// Ignore First TS as first MS is typically corrupt
  if (0 == fulCurrentTsIdx) return kTRUE;

  /// On first TS, extract the TS parameters from header (by definition stable over time)
  if (-1.0 == fdTsCoreSizeInNs) {
    fuNbCoreMsPerTs  = ts.num_core_microslices();
    fuNbOverMsPerTs  = ts.num_microslices(0) - ts.num_core_microslices();
    fdTsCoreSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs);
    fdTsFullSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs + fuNbOverMsPerTs);
    LOG(info) << "Timeslice parameters: each TS has " << fuNbCoreMsPerTs << " Core MS and " << fuNbOverMsPerTs
              << " Overlap MS, for a core duration of " << fdTsCoreSizeInNs << " ns and a full duration of "
              << fdTsFullSizeInNs << " ns";

    /// Ignore overlap ms if flag set by user
    fuNbMsLoop = fuNbCoreMsPerTs;
    if (kFALSE == fbIgnoreOverlapMs) fuNbMsLoop += fuNbOverMsPerTs;
    LOG(info) << "In each TS " << fuNbMsLoop << " MS will be looped over";

    /// Initialize start TS for evolution plots
    fulStartTs = fulCurrentTsIdx;
  }  // if( -1.0 == fdTsCoreSizeInNs )

  /// Compute time of TS core end
  fdTsStopTimeCore = fdTsStartTime + fdTsCoreSizeInNs;
  //      LOG(info) << Form( "TS %5d Start %12f Stop %12f", fulCurrentTsIdx, fdTsStartTime, fdTsStopTimeCore );

  /// Loop over core microslices (and overlap ones if chosen)
  for (fuMsIndex = 0; fuMsIndex < fuNbMsLoop; fuMsIndex++) {
    /// Loop over registered components
    for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx) {
      UInt_t uMsComp = fvMsComponentsList[uMsCompIdx];

      if (kFALSE == ProcessMs(ts, uMsComp, fuMsIndex)) {
        LOG(error) << "Failed to process ts " << fulCurrentTsIdx << " MS " << fuMsIndex << " for component " << uMsComp;
        return kFALSE;
      }  // if( kFALSE == ProcessMs( ts, uMsCompIdx, fuMsIndex ) )
    }    // for( UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx )

    if (kTRUE == fbDebugMonitorMode) {
      for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
        fhNbMissPatternPerMs->Fill(fuNbMissmatchPattern[uGdpb], uGdpb + fUnpackPar->GetGdpbToSectorOffset());
        fuNbMissmatchPattern[uGdpb] = 0;
      }  // for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb)
    }    // if( kTRUE == fbDebugMonitorMode )
  }      // for( fuMsIndex = 0; fuMsIndex < uNbMsLoop; fuMsIndex ++ )

  /// Fill plots if in monitor mode
  if (kFALSE == FillHistograms()) {
    LOG(error) << "Failed to fill histos in ts " << fulCurrentTsIdx;
    return kFALSE;
  }  // if( kFALSE == FillHistograms() )

  return kTRUE;
}

Bool_t CbmStar2019MonitorAlgo::ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx)
{
  auto msDescriptor        = ts.descriptor(uMsCompIdx, uMsIdx);
  fuCurrentEquipmentId     = msDescriptor.eq_id;
  const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(uMsCompIdx, uMsIdx));

  uint32_t uSize  = msDescriptor.size;
  fulCurrentMsIdx = msDescriptor.idx;
  fdMsTime        = (1e-9) * static_cast<double>(fulCurrentMsIdx);
  LOG(debug) << "Microslice: " << fulCurrentMsIdx << " from EqId " << std::hex << fuCurrentEquipmentId << std::dec
             << " has size: " << uSize;

  if (0 == fvbMaskedComponents.size()) fvbMaskedComponents.resize(ts.num_components(), kFALSE);

  fuCurrDpbId = static_cast<uint32_t>(fuCurrentEquipmentId & 0xFFFF);
  //   fuCurrDpbIdx = fDpbIdIndexMap[ fuCurrDpbId ];

  /// Check if this sDPB ID was declared in parameter file and stop there if not
  auto it = fGdpbIdIndexMap.find(fuCurrDpbId);
  if (it == fGdpbIdIndexMap.end()) {
    if (kFALSE == fvbMaskedComponents[uMsCompIdx]) {
      LOG(info) << "---------------------------------------------------------------";
      LOG(info) << FormatMsHeaderPrintout(msDescriptor);
      LOG(warning) << "Could not find the gDPB index for AFCK id 0x" << std::hex << fuCurrDpbId << std::dec
                   << " in timeslice " << fulCurrentTsIdx << " in microslice " << uMsIdx << " component " << uMsCompIdx
                   << "\n"
                   << "If valid this index has to be added in the TOF "
                      "parameter file in the DbpIdArray field";
      fvbMaskedComponents[uMsCompIdx] = kTRUE;
    }  // if( kFALSE == fvbMaskedComponents[ uMsComp ] )
    else
      return kTRUE;

    /// Try to get it from the second message in buffer (first is epoch cycle without gDPB ID)
    /// TODO!!!!

    return kFALSE;
  }  // if( it == fGdpbIdIndexMap.end() )
  else
    fuCurrDpbIdx = fGdpbIdIndexMap[fuCurrDpbId];

  fuCurrSector = fuCurrDpbIdx + fUnpackPar->GetGdpbToSectorOffset();

  /// Save start time of first valid MS )
  if (fdStartTime < 0) fdStartTime = fdMsTime;
  /// Reset the histograms if reached the end of the evolution histos range
  else if (fuHistoryHistoSize < fdMsTime - fdStartTime) {
    ResetEvolutionHistograms();
    fdStartTime = fdMsTime;
  }  // else if( fuHistoryHistoSize < fdMsTime - fdStartTime )
     /*
      /// In Run rate evolution
   if (fdStartTimeLong < 0)
      fdStartTimeLong = fdMsTime;
      /// Reset the evolution Histogram and the start time when we reach the end of the range
   if( fuHistoryHistoSizeLong < 1e-9 * (fdMsTime - fdStartTimeLong) / 60.0 )
   {
      ResetLongEvolutionHistograms();
      fdStartTimeLong = dHitTime;
   } // if( fuHistoryHistoSize < 1e-9 * (fdMsTime - fdStartTimeLong) / 60.0 )
*/

  // If not integer number of message in input buffer, print warning/error
  if (0 != (uSize % kuBytesPerMessage))
    LOG(error) << "The input microslice buffer does NOT "
               << "contain only complete nDPB messages!";

  // Compute the number of complete messages in the input microslice buffer
  uint32_t uNbMessages = (uSize - (uSize % kuBytesPerMessage)) / kuBytesPerMessage;

  // Prepare variables for the loop on contents
  Int_t messageType       = -111;
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
    /// Get message type
    messageType = mess.getMessageType();
    fhMessType->Fill(messageType);
    fhGdpbMessType->Fill(messageType, fuCurrSector);

    fuGet4Id = fUnpackPar->ElinkIdxToGet4Idx(mess.getGdpbGenChipId());
    fuGet4Nr = (fuCurrDpbIdx * fuNrOfGet4PerGdpb) + fuGet4Id;

    if (fuNrOfGet4PerGdpb <= fuGet4Id && !mess.isStarTrigger() && (gdpbv100::kuChipIdMergedEpoch != fuGet4Id))
      LOG(warning) << "Message with Get4 ID too high: " << fuGet4Id << " VS " << fuNrOfGet4PerGdpb
                   << " set in parameters.";

    switch (messageType) {
      case gdpbv100::MSG_HIT: {
        if (mess.getGdpbHitIs24b()) {
          /// Should never happen!!
          LOG(fatal) << "This monitor does not support 24b hit messages!!!.";
          continue;
        }  // if( getGdpbHitIs24b() )
        else {
          /// Histogramming
          fhGet4MessType->Fill(fuGet4Nr, 0);
          fvhGdpbGet4MessType[fuCurrDpbIdx]->Fill(fuGet4Id, 0);

          fhErrorFractEvo->Fill(fdMsTime - fdStartTime, 0.0);
          fhLostEvtFractEvo->Fill(fdMsTime - fdStartTime, 0.0);

          fhMsgCntEvo->Fill(fdMsTime - fdStartTime);
          fhHitCntEvo->Fill(fdMsTime - fdStartTime);
          /*
                  if( kFALSE == fbEpochSinceLastHit )
                  {
                     if( fmLastHit != mess )
                     {
                        if( 0 < fuDuplicatesCount )
                        {
                           LOG(warning) << "Detected duplicate hits in sector " << fuCurrSector
                                         << ": " << fuDuplicatesCount << " times "
                                        << Form( "0x%16lx", fmLastHit.getData() );
                        } // if( 0 < fuDuplicatesCount )
                        fmLastHit = mess;
                        fuDuplicatesCount = 0;
                     }
                        else fuDuplicatesCount++;
                  } // if( kFALSE == fbEpochSinceLastHit )
                     else
                     {
                        fmLastHit = mess;
                        fbEpochSinceLastHit = kFALSE;
                     } // else of if( kFALSE == fbEpochSinceLastHit )
*/
          fvmEpSupprBuffer.push_back(mess);
        }  // else of if( getGdpbHitIs24b() )
        break;
      }  // case gdpbv100::MSG_HIT:
      case gdpbv100::MSG_EPOCH: {
        if (gdpbv100::kuChipIdMergedEpoch == fuGet4Id) {
          ProcessEpoch(mess);
        }  // if this epoch message is a merged one valid for all chips
        else {
          /// Should never happen!!
          LOG(fatal) << "This event builder does not support unmerged epoch "
                        "messages!!!.";
          continue;
        }  // if single chip epoch message
        break;
      }  // case gdpbv100::MSG_EPOCH:
      case gdpbv100::MSG_SLOWC: {
        ProcessSlowCtrl(mess);
        break;
      }  // case gdpbv100::MSG_SLOWC:
      case gdpbv100::MSG_SYST: {
        ProcessSysMess(mess);
        break;
      }  // case gdpbv100::MSG_SYST:
      case gdpbv100::MSG_STAR_TRI_A:
      case gdpbv100::MSG_STAR_TRI_B:
      case gdpbv100::MSG_STAR_TRI_C:
      case gdpbv100::MSG_STAR_TRI_D: {
        ProcessStarTrig(mess);
        break;
      }  // case gdpbv100::MSG_STAR_TRI_A-D
      default:
        LOG(fatal) << "Message type " << std::hex << std::setw(2) << static_cast<uint16_t>(messageType)
                   << " not included in Get4 data format.";
    }  // switch( mess.getMessageType() )
  }    // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)

  /// Check if buffer is empty
  /// => If not, indication that last msg in MS was not an epoch
  if (0 < fvmEpSupprBuffer.size()) {
    fhGdpbEndMsBufferNotEmpty->Fill(fuCurrSector);
    fhGdpbEndMsDataLost->Fill(fuCurrSector, fvmEpSupprBuffer.size());
  }  // if( 0 < fvmEpSupprBuffer.size() )

  /// Clear the suppressed epoch buffer even if not empty
  fvmEpSupprBuffer.clear();

  /// Fill histograms
  FillHistograms();

  return kTRUE;
}

// -------------------------------------------------------------------------
void CbmStar2019MonitorAlgo::ProcessEpochCycle(uint64_t ulCycleData)
{
  ULong64_t ulEpochCycleVal = ulCycleData & gdpbv100::kulEpochCycleFieldSz;

  if (!(ulEpochCycleVal == fvulCurrentEpochCycle[fuCurrDpbIdx]
        || ulEpochCycleVal == fvulCurrentEpochCycle[fuCurrDpbIdx] + 1)
      && 0 < fulCurrentMsIdx) {
    LOG(warning) << "CbmStar2019MonitorAlgo::ProcessEpochCycle => "
                 << " Missmatch in epoch cycles detected for Gdpb " << fuCurrDpbIdx
                 << ", probably fake cycles due to epoch index corruption! "
                 << Form(" Current cycle 0x%09llX New cycle 0x%09llX", fvulCurrentEpochCycle[fuCurrDpbIdx],
                         ulEpochCycleVal);
  }  // if epoch cycle did not stay constant or increase by exactly 1, except if first MS of the TS
  if (ulEpochCycleVal != fvulCurrentEpochCycle[fuCurrDpbIdx]) {
    LOG(info) << "CbmStar2019EventBuilderEtofAlgo::ProcessEpochCycle => "
              << " New epoch cycle for Gdpb " << fuCurrDpbIdx
              << Form(": Current cycle 0x%09llX New cycle 0x%09llX", fvulCurrentEpochCycle[fuCurrDpbIdx],
                      ulEpochCycleVal);
  }  // if( ulEpochCycleVal != fvulCurrentEpochCycle[fuCurrDpbIdx] )
  fvulCurrentEpochCycle[fuCurrDpbIdx] = ulEpochCycleVal;

  return;
}
void CbmStar2019MonitorAlgo::ProcessEpoch(gdpbv100::Message mess)
{
  ULong64_t ulEpochNr = mess.getGdpbEpEpochNb();
  Bool_t bSyncFlag    = (1 == mess.getGdpbEpSync());
  Bool_t bDataLoss    = (1 == mess.getGdpbEpDataLoss());
  Bool_t bEpochLoss   = (1 == mess.getGdpbEpEpochLoss());
  Bool_t bMissmMatch  = (1 == mess.getGdpbEpMissmatch());

  fvulCurrentEpoch[fuCurrDpbIdx] = ulEpochNr;
  fvulCurrentEpochFull[fuCurrDpbIdx] =
    ulEpochNr + (gdpbv100::kuEpochCounterSz + 1) * fvulCurrentEpochCycle[fuCurrDpbIdx];

  /// Histogramming
  if (bSyncFlag) {
    fhGdpbEpochFlags->Fill(fuCurrSector, 0);
    fhGdpbEpochSyncEvo->Fill(fdMsTime - fdStartTime, fuCurrSector);
  }  // if( bSyncFlag )

  if (bDataLoss) fhGdpbEpochFlags->Fill(fuCurrSector, 1);

  if (bEpochLoss) fhGdpbEpochFlags->Fill(fuCurrSector, 2);

  if (bMissmMatch) {
    fhGdpbEpochFlags->Fill(fuCurrSector, 3);
    fhGdpbEpochMissEvo->Fill(fdMsTime - fdStartTime, fuCurrSector);
  }  // if( bMissmMatch )

  for (uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGet4Index++) {
    fuGet4Id = uGet4Index;
    fuGet4Nr = (fuCurrDpbIdx * fuNrOfGet4PerGdpb) + fuGet4Id;

    fhGet4MessType->Fill(fuGet4Nr, 1);
    fvhGdpbGet4MessType[fuCurrDpbIdx]->Fill(fuGet4Id, 1);

    if (bSyncFlag) fhGet4EpochFlags->Fill(fuGet4Nr, 0);
    if (bDataLoss) fhGet4EpochFlags->Fill(fuGet4Nr, 1);
    if (bEpochLoss) fhGet4EpochFlags->Fill(fuGet4Nr, 2);
    if (bMissmMatch) fhGet4EpochFlags->Fill(fuGet4Nr, 3);
  }  // for( uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGet4Index ++ )

  if (0 < fuDuplicatesCount)
    LOG(warning) << "Detected duplicate hits: " << fuDuplicatesCount << " times "
                 << Form("0x%16lx", static_cast<unsigned long>(fmLastHit.getData()));
  fbEpochSinceLastHit = kTRUE;
  fuDuplicatesCount   = 0;

  /// Process the corresponding messages buffer for current gDPB
  ProcessEpSupprBuffer();
}
// -------------------------------------------------------------------------
void CbmStar2019MonitorAlgo::ProcessEpSupprBuffer()
{
  Int_t iBufferSize = fvmEpSupprBuffer.size();

  if (0 == iBufferSize) return;

  LOG(debug) << "Now processing stored messages for for gDPB " << fuCurrDpbIdx << " with epoch number "
             << (fvulCurrentEpoch[fuCurrDpbIdx] - 1);

  /// Data are sorted between epochs, not inside => Epoch level ordering
  /// Sorting at lower bin precision level
  std::stable_sort(fvmEpSupprBuffer.begin(), fvmEpSupprBuffer.end());

  /// Compute original epoch index before epoch suppression
  ULong64_t ulCurEpochGdpbGet4 = fvulCurrentEpochFull[fuCurrDpbIdx];

  /// Ignore the first epoch as it should never appear (start delay!!)
  if (0 == ulCurEpochGdpbGet4) return;

  /// In Ep. Suppr. Mode, receive following epoch instead of previous
  ulCurEpochGdpbGet4--;

  Int_t messageType = -111;
  for (Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++) {
    messageType = fvmEpSupprBuffer[iMsgIdx].getMessageType();

    fuGet4Id = fUnpackPar->ElinkIdxToGet4Idx(fvmEpSupprBuffer[iMsgIdx].getGdpbGenChipId());
    fuGet4Nr = (fuCurrDpbIdx * fuNrOfGet4PerGdpb) + fuGet4Id;

    /// Store the full message in the proper buffer
    gdpbv100::FullMessage fullMess(fvmEpSupprBuffer[iMsgIdx], ulCurEpochGdpbGet4);

    /// Do other actions on it if needed
    switch (messageType) {
      case gdpbv100::MSG_HIT: {
        ProcessHit(fullMess);
        break;
      }  // case gdpbv100::MSG_HIT:
      case gdpbv100::MSG_SLOWC:
        /// Should never appear there
        break;
      case gdpbv100::MSG_SYST: {
        /// Should be only error messages from GET4
        if (gdpbv100::SYS_GET4_ERROR == fullMess.getGdpbSysSubType()) ProcessError(fullMess);
        break;
      }  // case gdpbv100::MSG_SYST:
      case gdpbv100::MSG_EPOCH:
      case gdpbv100::MSG_STAR_TRI_A:
      case gdpbv100::MSG_STAR_TRI_B:
      case gdpbv100::MSG_STAR_TRI_C:
      case gdpbv100::MSG_STAR_TRI_D:
        /// Should never appear there
        break;
      default:
        LOG(error) << "Message type " << std::hex << std::setw(2) << static_cast<uint16_t>(messageType)
                   << " not included in Get4 unpacker.";
    }  // switch( mess.getMessageType() )
  }    // for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )

  fvmEpSupprBuffer.clear();
}
// -------------------------------------------------------------------------
void CbmStar2019MonitorAlgo::ProcessHit(gdpbv100::FullMessage mess)
{
  UInt_t uChannel = mess.getGdpbHitChanId();
  UInt_t uTot     = mess.getGdpbHit32Tot();

  /// In 32b mode the coarse counter is already computed back to 112 FTS bins
  /// => need to hide its contribution from the Finetime
  /// => FTS = Fullt TS modulo 112
  UInt_t uFts = mess.getGdpbHitFullTs() % 112;
  UInt_t uCts = mess.getGdpbHitFullTs() / 112;

  UInt_t uChannelNr      = fuGet4Id * fuNrOfChannelsPerGet4 + uChannel;
  UInt_t uChannelNrInFee = (fuGet4Id % fuNrOfGet4PerFee) * fuNrOfChannelsPerGet4 + uChannel;
  UInt_t uFeeNr          = (fuGet4Id / fuNrOfGet4PerFee);
  //   UInt_t uFeeNrInSys        = fuCurrDpbIdx * fuNrOfFeePerGdpb + uFeeNr;
  UInt_t uRemappedChannelNr = uFeeNr * fuNrOfChannelsPerFee + fUnpackPar->Get4ChanToPadiChan(uChannelNrInFee);
  //   UInt_t uGbtxNr            = (uFeeNr / fUnpackPar->GetNrOfFeePerGbtx());
  //   UInt_t uFeeInGbtx         = (uFeeNr % fUnpackPar->GetNrOfFeePerGbtx());
  //   UInt_t uGbtxNrInSys       = fuCurrDpbIdx * fUnpackPar->GetNrOfGbtxPerGdpb() + uGbtxNr;

  //   UInt_t uChanInSyst = fuCurrDpbIdx * fuNrOfChannelsPerGdpb + uChannelNr;
  //   UInt_t uRemappedChannelNrInSys = fuCurrDpbIdx * fuNrOfChannelsPerGdpb
  //                                   + uFeeNr * fuNrOfChannelsPerFee
  //                                   + fUnpackPar->Get4ChanToPadiChan( uChannelNrInFee );

  //   ULong_t  ulHitTime = mess.getMsgFullTime(  mess.getExtendedEpoch() );
  Double_t dHitTime = mess.GetFullTimeNs();
  //   Double_t dHitTot   = uTot;     // in bins

  /// Raw channel plots = GET4 related
  if (kTRUE == fbDebugMonitorMode) {
    fvhRawFt_gDPB[fuCurrDpbIdx]->Fill(uChannelNr, uFts);
    fvhRawCt_gDPB[fuCurrDpbIdx]->Fill(uChannelNr, uCts);
  }  // if( kTRUE == fbDebugMonitorMode )

  /// Try to catch the corrupt data problem reported by Florian Seck
  if (4096 <= uCts) {
    LOG(debug) << "CbmStar2019MonitorAlgo::ProcessHit => Coarse time above "
                  "4096 detected."
               << Form(" sector %02u GET4 %03u Channel %u, TS %8llu MS %3u (MS "
                       "time %12llu)",
                       fuCurrDpbIdx + fUnpackPar->GetGdpbToSectorOffset(), fuGet4Id, uChannel, fulCurrentTsIdx,
                       fuMsIndex, fulCurrentMsIdx);
  }  // if( 4096 <= uCts )

  /// Remapped channel plots = PADI/RPC related
  fvhRemapChCount_gDPB[fuCurrDpbIdx]->Fill(uRemappedChannelNr);
  fvhRemapTot_gDPB[fuCurrDpbIdx]->Fill(uRemappedChannelNr, uTot);

  /// Start time book-keeping
  /// In Run rate evolution
  if (0 <= fdStartTime) {
    fvhRemapChRate_gDPB[fuCurrDpbIdx]->Fill(1e-9 * dHitTime - fdStartTime, uRemappedChannelNr);
    //      fvhFeeRate_gDPB[(fuCurrDpbIdx * fuNrOfFeePerGdpb) + uFeeNr]->Fill( 1e-9 * (dHitTime - fdStartTime));
    //      fvhFeeErrorRatio_gDPB[(fuCurrDpbIdx * fuNrOfFeePerGdpb) + uFeeNr]->Fill( 1e-9 * (dHitTime - fdStartTime), 0, 1);
  }  // if (0 <= fdStartTime)
  /*
   if (0 <= fdStartTimeLong)
   {
      fvhFeeRateLong_gDPB[(fuCurrDpbIdx * fuNrOfFeePerGdpb) + uFeeNr]->Fill(
            1e-9 / 60.0 * (dHitTime - fdStartTimeLong), 1 / 60.0 );
      fvhFeeErrorRatioLong_gDPB[(fuCurrDpbIdx * fuNrOfFeePerGdpb) + uFeeNr]->Fill(
            1e-9 / 60.0 * (dHitTime - fdStartTimeLong), 0, 1 / 60.0 );
   } // if (0 <= fdStartTimeLong)
*/
}
// -------------------------------------------------------------------------
void CbmStar2019MonitorAlgo::ProcessSysMess(gdpbv100::FullMessage mess)
{
  UInt_t uSubType = mess.getGdpbSysSubType();
  fhSysMessType->Fill(uSubType);
  fhGdpbSysMessType->Fill(uSubType, fuCurrSector);

  switch (mess.getGdpbSysSubType()) {
    case gdpbv100::SYS_GET4_ERROR: {
      /// Histogramming
      fhGet4MessType->Fill(fuGet4Nr, 3);
      fvhGdpbGet4MessType[fuCurrDpbIdx]->Fill(fuGet4Id, 3);

      /// Buffering
      fvmEpSupprBuffer.push_back(mess);
      //         ProcessError( mess );
      break;
    }  // case gdpbv100::SYSMSG_GET4_EVENT
    case gdpbv100::SYS_GDPB_UNKWN: {
      LOG(debug) << "Unknown GET4 message, data: " << std::hex << std::setw(8) << mess.getGdpbSysUnkwData() << std::dec
                 << " Full message: " << std::hex << std::setw(16) << mess.getData() << std::dec;
      break;
    }  // case gdpbv100::SYS_GDPB_UNKWN:
    case gdpbv100::SYS_GET4_SYNC_MISS: {
      if (mess.getGdpbSysFwErrResync())
        LOG(info) << Form("GET4 Resynchronization: Get4:0x%04x ", mess.getGdpbGenChipId()) << fuCurrDpbIdx;
      else
        LOG(info) << "GET4 synchronization pulse missing in gDPB " << fuCurrDpbIdx;
      break;
    }  // case gdpbv100::SYS_GET4_SYNC_MISS:
    case gdpbv100::SYS_PATTERN: {
      ProcessPattern(mess);
      break;
    }  // case gdpbv100::SYS_PATTERN:
    default: {
      LOG(info) << "Crazy system message, subtype " << mess.getGdpbSysSubType();
      break;
    }  // default
  }    // switch( mess.getGdpbSysSubType() )
}
// -------------------------------------------------------------------------
void CbmStar2019MonitorAlgo::ProcessError(gdpbv100::FullMessage mess)
{
  uint32_t uErrorType = mess.getGdpbSysErrData();

  fhGet4MessType->Fill(fuGet4Nr, 3);
  fvhGdpbGet4MessType[fuCurrDpbIdx]->Fill(fuGet4Id, 3);

  //   UInt_t uFeeNr   = (fuGet4Id / fuNrOfGet4PerFee);

  /// General error rates
  fhErrorFractEvo->Fill(fdMsTime - fdStartTime, 0.0);
  fhLostEvtFractEvo->Fill(fdMsTime - fdStartTime, 0.0);

  fhMsgCntEvo->Fill(fdMsTime - fdStartTime);
  fhErrorCntEvo->Fill(fdMsTime - fdStartTime);
  fhErrorFractEvo->Fill(fdMsTime - fdStartTime, 1.0);

  if (gdpbv100::GET4_V2X_ERR_LOST_EVT == uErrorType) {
    fhLostEvtCntEvo->Fill(fdMsTime - fdStartTime);
    fhLostEvtFractEvo->Fill(fdMsTime - fdStartTime, 1.0);
  }  // if( gdpbv100::GET4_V2X_ERR_LOST_EVT == mess.getGdpbSysErrData() )
     /*
   /// Error rate per FEE
   if (0 <= fdStartTime)
   {
      fvhFeeErrorRate_gDPB[(fuCurrDpbIdx * fuNrOfFeePerGdpb) + uFeeNr]->Fill(
         1e-9 * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr]) - fdStartTime));
      fvhFeeErrorRatio_gDPB[(fuCurrDpbIdx * fuNrOfFeePerGdpb) + uFeeNr]->Fill(
         1e-9 * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr]) - fdStartTime), 1, 1 );
   } // if (0 <= fdStartTime)
   if (0 <= fdStartTimeLong)
   {
      fvhFeeErrorRateLong_gDPB[(fuCurrDpbIdx * fuNrOfFeePerGdpb) + uFeeNr]->Fill(
         1e-9 / 60.0 * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr]) - fdStartTimeLong), 1 / 60.0);
      fvhFeeErrorRatioLong_gDPB[(fuCurrDpbIdx * fuNrOfFeePerGdpb) + uFeeNr]->Fill(
         1e-9 / 60.0 * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr]) - fdStartTimeLong), 1, 1 / 60.0);
   } // if (0 <= fdStartTime)
*/
  Int_t dGdpbChId = fuGet4Id * fuNrOfChannelsPerGet4 + mess.getGdpbSysErrChanId();
  Int_t dFullChId = fuGet4Nr * fuNrOfChannelsPerGet4 + mess.getGdpbSysErrChanId();

  switch (uErrorType) {
    case gdpbv100::GET4_V2X_ERR_READ_INIT: {
      fhGet4ChanErrors->Fill(dFullChId, 0);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 0);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_READ_INIT:
    case gdpbv100::GET4_V2X_ERR_SYNC: {
      fhGet4ChanErrors->Fill(dFullChId, 1);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 1);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_SYNC:
    case gdpbv100::GET4_V2X_ERR_EP_CNT_SYNC: {
      fhGet4ChanErrors->Fill(dFullChId, 2);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 2);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_EP_CNT_SYNC:
    case gdpbv100::GET4_V2X_ERR_EP: {
      fhGet4ChanErrors->Fill(dFullChId, 3);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 3);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_EP:
    case gdpbv100::GET4_V2X_ERR_FIFO_WRITE: {
      fhGet4ChanErrors->Fill(dFullChId, 4);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 4);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_FIFO_WRITE:
    case gdpbv100::GET4_V2X_ERR_LOST_EVT: {
      fhGet4ChanErrors->Fill(dFullChId, 5);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 5);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_LOST_EVT:
    case gdpbv100::GET4_V2X_ERR_CHAN_STATE: {
      fhGet4ChanErrors->Fill(dFullChId, 6);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 6);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_CHAN_STATE:
    case gdpbv100::GET4_V2X_ERR_TOK_RING_ST: {
      fhGet4ChanErrors->Fill(dFullChId, 7);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 7);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_TOK_RING_ST:
    case gdpbv100::GET4_V2X_ERR_TOKEN: {
      fhGet4ChanErrors->Fill(dFullChId, 8);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 8);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_TOKEN:
    case gdpbv100::GET4_V2X_ERR_READOUT_ERR: {
      fhGet4ChanErrors->Fill(dFullChId, 9);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 9);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_READOUT_ERR:
    case gdpbv100::GET4_V2X_ERR_SPI: {
      fhGet4ChanErrors->Fill(dFullChId, 10);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 10);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_SPI:
    case gdpbv100::GET4_V2X_ERR_DLL_LOCK: {
      fhGet4ChanErrors->Fill(dFullChId, 11);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 11);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_DLL_LOCK:
    case gdpbv100::GET4_V2X_ERR_DLL_RESET: {
      fhGet4ChanErrors->Fill(dFullChId, 12);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 12);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_DLL_RESET:
    case gdpbv100::GET4_V2X_ERR_TOT_OVERWRT: {
      fhGet4ChanErrors->Fill(dFullChId, 13);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 13);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_TOT_OVERWRT:
    case gdpbv100::GET4_V2X_ERR_TOT_RANGE: {
      fhGet4ChanErrors->Fill(dFullChId, 14);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 14);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_TOT_RANGE:
    case gdpbv100::GET4_V2X_ERR_EVT_DISCARD: {
      fhGet4ChanErrors->Fill(dFullChId, 15);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 15);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_EVT_DISCARD:
    case gdpbv100::GET4_V2X_ERR_ADD_RIS_EDG: {
      fhGet4ChanErrors->Fill(dFullChId, 16);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 16);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_ADD_RIS_EDG:
    case gdpbv100::GET4_V2X_ERR_UNPAIR_FALL: {
      fhGet4ChanErrors->Fill(dFullChId, 17);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 17);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_UNPAIR_FALL:
    case gdpbv100::GET4_V2X_ERR_SEQUENCE_ER: {
      fhGet4ChanErrors->Fill(dFullChId, 18);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 18);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_SEQUENCE_ER:
    case gdpbv100::GET4_V2X_ERR_EPOCH_OVERF: {
      fhGet4ChanErrors->Fill(dFullChId, 19);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 19);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_EPOCH_OVERF:
    case gdpbv100::GET4_V2X_ERR_UNKNOWN: {
      fhGet4ChanErrors->Fill(dFullChId, 20);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 20);
      break;
    }  // case gdpbv100::GET4_V2X_ERR_UNKNOWN:
    default: // Corrupt error or not yet supported error
      {
      fhGet4ChanErrors->Fill(dFullChId, 21);
      fvhGdpbGet4ChanErrors[fuCurrDpbIdx]->Fill(dGdpbChId, 21);
      break;
    }  //
  }    // Switch( mess.getGdpbSysErrData() )

  switch (uErrorType) {
    case gdpbv100::GET4_V2X_ERR_READ_INIT:
    case gdpbv100::GET4_V2X_ERR_SYNC:
    case gdpbv100::GET4_V2X_ERR_EP_CNT_SYNC:
    case gdpbv100::GET4_V2X_ERR_EP:
    case gdpbv100::GET4_V2X_ERR_FIFO_WRITE:
    case gdpbv100::GET4_V2X_ERR_CHAN_STATE:
    case gdpbv100::GET4_V2X_ERR_TOK_RING_ST:
    case gdpbv100::GET4_V2X_ERR_TOKEN:
    case gdpbv100::GET4_V2X_ERR_READOUT_ERR:
    case gdpbv100::GET4_V2X_ERR_DLL_LOCK:
    case gdpbv100::GET4_V2X_ERR_DLL_RESET: {
      /// Critical errors
      if (kFALSE == fbIgnoreCriticalErrors)
        LOG(info) << " +++++++ > gDPB: " << std::hex << std::setw(4) << fuCurrDpbIdx << std::dec
                  << ", Chip = " << std::setw(2) << mess.getGdpbGenChipId() << ", Chan = " << std::setw(1)
                  << mess.getGdpbSysErrChanId() << ", Edge = " << std::setw(1) << mess.getGdpbSysErrEdge()
                  << ", Empt = " << std::setw(1) << mess.getGdpbSysErrUnused() << ", Data = " << std::hex
                  << std::setw(2) << uErrorType << std::dec << " -- GET4 V1 Error Event";
      break;
    }  // critical errors
    case gdpbv100::GET4_V2X_ERR_SPI:
      /// Error during SPI communication with slave (e.g. PADI)
      break;
    case gdpbv100::GET4_V2X_ERR_LOST_EVT:
    case gdpbv100::GET4_V2X_ERR_TOT_OVERWRT:
    case gdpbv100::GET4_V2X_ERR_TOT_RANGE:
    case gdpbv100::GET4_V2X_ERR_EVT_DISCARD:
    case gdpbv100::GET4_V2X_ERR_ADD_RIS_EDG:
    case gdpbv100::GET4_V2X_ERR_UNPAIR_FALL:
    case gdpbv100::GET4_V2X_ERR_SEQUENCE_ER: {
      /// Input channel related errors (TOT, shaky signals, etc...)
      LOG(debug) << " +++++++ >gDPB: " << std::hex << std::setw(4) << fuCurrDpbIdx << std::dec
                 << ", Chip = " << std::setw(2) << mess.getGdpbGenChipId() << ", Chan = " << std::setw(1)
                 << mess.getGdpbSysErrChanId() << ", Edge = " << std::setw(1) << mess.getGdpbSysErrEdge()
                 << ", Empt = " << std::setw(1) << mess.getGdpbSysErrUnused() << ", Data = " << std::hex << std::setw(2)
                 << uErrorType << std::dec << " -- GET4 V1 Error Event ";
      break;
    }  // Input channel related errors (TOT, shaky signals, etc...)
    case gdpbv100::GET4_V2X_ERR_EPOCH_OVERF: break;
    case gdpbv100::GET4_V2X_ERR_UNKNOWN:
      /// Unrecognised error code from GET4
      break;
    default:
      /// Corrupt error or not yet supported error
      break;
  }  // switch( uErrorType )

  return;
}
// -------------------------------------------------------------------------
void CbmStar2019MonitorAlgo::ProcessPattern(gdpbv100::Message mess)
{
  uint16_t usType   = mess.getGdpbSysPattType();
  uint16_t usIndex  = mess.getGdpbSysPattIndex();
  uint32_t uPattern = mess.getGdpbSysPattPattern();
  UInt_t uNbBits    = (7 == usIndex ? 16 : 32);
  fhGdpbSysMessPattType->Fill(usType, fuCurrSector);


  switch (usType) {
    case gdpbv100::PATT_MISSMATCH: {

      if (kTRUE == fbDebugMonitorMode && 0 == usIndex) {
        fuNbMissmatchPattern[fuCurrDpbIdx]++;

        LOG(debug) << Form("Missmatch pattern message => Type %d, Index %2d, "
                           "Pattern 0x%08X TS %12llu MS %3u Epoch %12llu",
                           usType, usIndex, uPattern, fulCurrentTsIdx, fuMsIndex, fvulCurrentEpoch[fuCurrDpbIdx]);
      }  // if( 0 == usIndex )

      LOG(debug) << Form("Missmatch pattern message => Type %d, Index %2d, Pattern 0x%08X", usType, usIndex, uPattern);
      if (kTRUE == fbDebugMonitorMode)
        for (UInt_t uBit = 0; uBit < uNbBits; ++uBit) {
          UInt_t uBadAsic = fUnpackPar->ElinkIdxToGet4Idx(32 * usIndex + uBit);

          if ((uPattern >> uBit) & 0x1) {
            fhPatternMissmatch->Fill(uBadAsic, fuCurrSector);
            fvhGdpbPatternMissmatchEvo[fuCurrDpbIdx]->Fill(fulCurrentTsIdx - fulStartTs, uBadAsic);
            fvvbGdpbLastMissmatchPattern[fuCurrDpbIdx][uBadAsic] = kTRUE;
          }  // if( ( uPattern >> uBit ) & 0x1 )
          else
            fvvbGdpbLastMissmatchPattern[fuCurrDpbIdx][uBadAsic] = kFALSE;

        }  // for( UInt_t uBit = 0; uBit < uNbBits; ++uBit )
      break;
    }  // case gdpbv100::PATT_MISSMATCH:
    case gdpbv100::PATT_ENABLE: {
      LOG(debug) << Form("ENABLE pattern message => Type %d, Index %2d, Pattern 0x%08X", usType, usIndex, uPattern);

      if (kTRUE == fbDebugMonitorMode)
        for (UInt_t uBit = 0; uBit < uNbBits; ++uBit) {
          UInt_t uAsic = fUnpackPar->ElinkIdxToGet4Idx(32 * usIndex + uBit);

          if ((uPattern >> uBit) & 0x1) {
            fhPatternEnable->Fill(uAsic, fuCurrSector);
            fvhGdpbPatternEnableEvo[fuCurrDpbIdx]->Fill(fulCurrentTsIdx, uAsic);
            fvvbGdpbLastEnablePattern[fuCurrDpbIdx][uAsic] = kFALSE;
          }  // if( ( uPattern >> uBit ) & 0x1 )
          else
            fvvbGdpbLastEnablePattern[fuCurrDpbIdx][uAsic] = kTRUE;

        }  // for( UInt_t uBit = 0; uBit < uNbBits; ++uBit )
      break;
    }  // case gdpbv100::PATT_ENABLE:
    case gdpbv100::PATT_RESYNC: {
      LOG(debug) << Form("RESYNC pattern message => Type %d, Index %2d, Pattern 0x%08X", usType, usIndex, uPattern);

      if (kTRUE == fbDebugMonitorMode)
        for (UInt_t uBit = 0; uBit < uNbBits; ++uBit) {
          UInt_t uBadAsic = fUnpackPar->ElinkIdxToGet4Idx(32 * usIndex + uBit);

          if ((uPattern >> uBit) & 0x1) {
            fhPatternResync->Fill(uBadAsic, fuCurrSector);
            fvhGdpbPatternResyncEvo[fuCurrDpbIdx]->Fill(fulCurrentTsIdx, uBadAsic);
            fvvbGdpbLastResyncPattern[fuCurrDpbIdx][uBadAsic] = kTRUE;
          }  // if( ( uPattern >> uBit ) & 0x1 )
          else
            fvvbGdpbLastResyncPattern[fuCurrDpbIdx][uBadAsic] = kFALSE;

        }  // for( UInt_t uBit = 0; uBit < uNbBits; ++uBit )
      break;
    }  // case gdpbv100::PATT_RESYNC:
    default: {
      LOG(debug) << "Crazy pattern message, subtype " << usType;
      break;
    }  // default
  }    // switch( usType )

  return;
}
// -------------------------------------------------------------------------
void CbmStar2019MonitorAlgo::ProcessSlowCtrl(gdpbv100::Message mess)
{
  UInt_t uChan = mess.getGdpbSlcChan();
  UInt_t uEdge = mess.getGdpbSlcEdge();
  UInt_t uData = mess.getGdpbSlcData();
  UInt_t uType = mess.getGdpbSlcType();

  Double_t dGdpbChId = fuGet4Id * fuNrOfChannelsPerGet4 + mess.getGdpbSlcChan() + 0.5 * mess.getGdpbSlcEdge();
  Double_t dFullChId = fuGet4Nr * fuNrOfChannelsPerGet4 + mess.getGdpbSlcChan() + 0.5 * mess.getGdpbSlcEdge();
  Double_t dMessTime = fulCurrentMsIdx * 1e-9;


  switch (uType) {
    case gdpbv100::GET4_32B_SLC_SCALER: {
      fhGet4ChanScm->Fill(dFullChId, uType);
      fvhGdpbGet4ChanScm[fuCurrDpbIdx]->Fill(dGdpbChId, uType);
      break;
    }  // case gdpbv100::GET4_32B_SLC_SCALER:
    case gdpbv100::GET4_32B_SLC_DEADT: {
      fhGet4ChanScm->Fill(dFullChId, uType);
      fvhGdpbGet4ChanScm[fuCurrDpbIdx]->Fill(dGdpbChId, uType);
      break;
    }  // case gdpbv100::GET4_32B_SLC_DEADT:
    case gdpbv100::GET4_32B_SLC_SPIREAD: {
      fhGet4ChanScm->Fill(dFullChId, uType);
      fvhGdpbGet4ChanScm[fuCurrDpbIdx]->Fill(dGdpbChId, uType);

      /// Printout if SPI message!
      fhGdpbAsicSpiCounts->Fill(fuGet4Id, fuCurrSector);
      LOG(info) << "GET4 Slow Control SPI message, time " << Form("%3.3f", dMessTime) << " s "
                << " for board ID " << std::hex << std::setw(4) << fuCurrDpbIdx << std::dec << "\n"
                << " +++++++ > Chip = " << std::setw(3) << fuGet4Id << ", Chan = " << std::setw(1) << uChan
                << ", Edge = " << std::setw(1) << uEdge << ", Type = " << std::setw(1) << mess.getGdpbSlcType() << ", "
                << Form("channel  %1u,", (uData >> 10) & 0xF) << Form("value 0x%03x ", uData & 0x3FF)
                << Form("level %4.1f ", fUnpackPar->GetPadiThresholdVal(uData & 0x3FF))
                << Form("(Data = 0x%06x) ", uData);
      break;
    }  // if( gdpbv100::GET4_32B_SLC_SPIREAD == uType )
    case gdpbv100::GET4_32B_SLC_START_SEU: {
      if (0 == mess.getGdpbSlcChan() && 0 == mess.getGdpbSlcEdge())  // START message
      {
        fhGet4ChanScm->Fill(dFullChId, uType + 1);
        fvhGdpbGet4ChanScm[fuCurrDpbIdx]->Fill(dGdpbChId, uType + 1);
      }  // if( 0 == mess.getGdpbSlcChan() && 0 == mess.getGdpbSlcEdge() )
      else if (0 == mess.getGdpbSlcChan() && 1 == mess.getGdpbSlcEdge())  // SEU counter message
      {
        fhGet4ChanScm->Fill(dFullChId, uType);
        fvhGdpbGet4ChanScm[fuCurrDpbIdx]->Fill(dGdpbChId, uType);
      }  // else if( 0 == mess.getGdpbSlcChan() && 1 == mess.getGdpbSlcEdge() )
      break;
    }  // case gdpbv100::GET4_32B_SLC_START_SEU:
    default: break;
  }  // switch( uType )

  /// Histogramming
  fhGet4MessType->Fill(fuGet4Nr, 2);
  fvhGdpbGet4MessType[fuCurrDpbIdx]->Fill(fuGet4Id, 2);
}
// -------------------------------------------------------------------------
void CbmStar2019MonitorAlgo::ProcessStarTrig(gdpbv100::Message mess)
{
  Int_t iMsgIndex = mess.getStarTrigMsgIndex();

  switch (iMsgIndex) {
    case 0:
      fvhTokenMsgType[fuCurrDpbIdx]->Fill(0);
      fvulGdpbTsMsb[fuCurrDpbIdx] = mess.getGdpbTsMsbStarA();
      break;
    case 1:
      fvhTokenMsgType[fuCurrDpbIdx]->Fill(1);
      fvulGdpbTsLsb[fuCurrDpbIdx] = mess.getGdpbTsLsbStarB();
      fvulStarTsMsb[fuCurrDpbIdx] = mess.getStarTsMsbStarB();
      break;
    case 2:
      fvhTokenMsgType[fuCurrDpbIdx]->Fill(2);
      fvulStarTsMid[fuCurrDpbIdx] = mess.getStarTsMidStarC();
      break;
    case 3: {
      fvhTokenMsgType[fuCurrDpbIdx]->Fill(3);

      ULong64_t ulNewGdpbTsFull = (fvulGdpbTsMsb[fuCurrDpbIdx] << 24) + (fvulGdpbTsLsb[fuCurrDpbIdx]);
      ULong64_t ulNewStarTsFull =
        (fvulStarTsMsb[fuCurrDpbIdx] << 48) + (fvulStarTsMid[fuCurrDpbIdx] << 8) + mess.getStarTsLsbStarD();
      UInt_t uNewToken   = mess.getStarTokenStarD();
      UInt_t uNewDaqCmd  = mess.getStarDaqCmdStarD();
      UInt_t uNewTrigCmd = mess.getStarTrigCmdStarD();

      if ((uNewToken == fvuStarTokenLast[fuCurrDpbIdx]) && (ulNewGdpbTsFull == fvulGdpbTsFullLast[fuCurrDpbIdx])
          && (ulNewStarTsFull == fvulStarTsFullLast[fuCurrDpbIdx]) && (uNewDaqCmd == fvuStarDaqCmdLast[fuCurrDpbIdx])
          && (uNewTrigCmd == fvuStarTrigCmdLast[fuCurrDpbIdx])) {
        UInt_t uTrigWord = ((fvuStarTrigCmdLast[fuCurrDpbIdx] & 0x00F) << 16)
                           + ((fvuStarDaqCmdLast[fuCurrDpbIdx] & 0x00F) << 12)
                           + ((fvuStarTokenLast[fuCurrDpbIdx] & 0xFFF));
        LOG(warning) << "Possible error: identical STAR tokens found twice in "
                        "a row => ignore 2nd! "
                     << " TS " << fulCurrentTsIdx << " gDBB #" << fuCurrDpbIdx << " "
                     << Form("token = %5u ", fvuStarTokenLast[fuCurrDpbIdx])
                     << Form("gDPB ts  = %12llu ", fvulGdpbTsFullLast[fuCurrDpbIdx])
                     << Form("STAR ts = %12llu ", fvulStarTsFullLast[fuCurrDpbIdx])
                     << Form("DAQ cmd = %2u ", fvuStarDaqCmdLast[fuCurrDpbIdx])
                     << Form("TRG cmd = %2u ", fvuStarTrigCmdLast[fuCurrDpbIdx]) << Form("TRG Wrd = %5x ", uTrigWord);
        return;
      }  // if exactly same message repeated

      // STAR TS counter reset detection
      if (ulNewStarTsFull < fvulStarTsFullLast[fuCurrDpbIdx])
        LOG(info) << "Probable reset of the STAR TS: old = " << Form("%16llu", fvulStarTsFullLast[fuCurrDpbIdx])
                  << " new = " << Form("%16llu", ulNewStarTsFull) << " Diff = -"
                  << Form("%8llu", fvulStarTsFullLast[fuCurrDpbIdx] - ulNewStarTsFull);

      //         ULong64_t ulGdpbTsDiff = ulNewGdpbTsFull - fvulGdpbTsFullLast[ fuCurrDpbIdx ];
      fvulGdpbTsFullLast[fuCurrDpbIdx] = ulNewGdpbTsFull;
      fvulStarTsFullLast[fuCurrDpbIdx] = ulNewStarTsFull;
      fvuStarTokenLast[fuCurrDpbIdx]   = uNewToken;
      fvuStarDaqCmdLast[fuCurrDpbIdx]  = uNewDaqCmd;
      fvuStarTrigCmdLast[fuCurrDpbIdx] = uNewTrigCmd;

      /// Histograms filling only in core MS
      if (fuMsIndex < fuNbCoreMsPerTs) {
        /// In Run rate evolution
        if (0 <= fdStartTime) {
          fvhTriggerRate[fuCurrDpbIdx]->Fill(fdMsTime - fdStartTime);
          fvhStarTokenEvo[fuCurrDpbIdx]->Fill(fdMsTime - fdStartTime, fvuStarTokenLast[fuCurrDpbIdx]);
          fvhStarTrigGdpbTsEvo[fuCurrDpbIdx]->Fill(fdMsTime - fdStartTime, fvulGdpbTsFullLast[fuCurrDpbIdx]);
          fvhStarTrigStarTsEvo[fuCurrDpbIdx]->Fill(fdMsTime - fdStartTime, fvulStarTsFullLast[fuCurrDpbIdx]);
        }  // if( 0 < fdStartTime )
        fvhCmdDaqVsTrig[fuCurrDpbIdx]->Fill(fvuStarDaqCmdLast[fuCurrDpbIdx], fvuStarTrigCmdLast[fuCurrDpbIdx]);
      }  // if( fuMsIndex < fuNbCoreMsPerTs  )

      break;
    }  // case 3
    default: LOG(error) << "Unknown Star Trigger messageindex: " << iMsgIndex;
  }  // switch( iMsgIndex )
}
// -------------------------------------------------------------------------

Bool_t CbmStar2019MonitorAlgo::CreateHistograms()
{
  std::string sFolder = "eTofMoni";

  LOG(info) << "create Histos for eTOF monitoring ";

  /// Logarithmic bining
  // Number of log bins =
  //      9 for the sub-unit decade
  //    + 9 for each unit of each decade * 10 for the subdecade range
  //    + 1 for the closing bin top edge
  const Int_t iNbDecadesLog     = 4;
  const Int_t iNbStepsDecade    = 9;
  const Int_t iNbSubStepsInStep = 1;
  const Int_t iNbBinsLog        = iNbStepsDecade + iNbStepsDecade * iNbSubStepsInStep * iNbDecadesLog + 1;
  Double_t dBinsLog[iNbBinsLog];
  // First fill sub-unit decade
  for (Int_t iSubU = 0; iSubU < iNbStepsDecade; iSubU++)
    dBinsLog[iSubU] = 0.1 * (1 + iSubU);
  std::cout << std::endl;
  // Then fill the main decades
  Double_t dSubstepSize = 1.0 / iNbSubStepsInStep;
  for (Int_t iDecade = 0; iDecade < iNbDecadesLog; iDecade++) {
    Double_t dBase   = std::pow(10, iDecade);
    Int_t iDecadeIdx = iNbStepsDecade + iDecade * iNbStepsDecade * iNbSubStepsInStep;
    for (Int_t iStep = 0; iStep < iNbStepsDecade; iStep++) {
      Int_t iStepIdx = iDecadeIdx + iStep * iNbSubStepsInStep;
      for (Int_t iSubStep = 0; iSubStep < iNbSubStepsInStep; iSubStep++) {
        dBinsLog[iStepIdx + iSubStep] = dBase * (1 + iStep) + dBase * dSubstepSize * iSubStep;
      }  // for( Int_t iSubStep = 0; iSubStep < iNbSubStepsInStep; iSubStep++ )
    }    // for( Int_t iStep = 0; iStep < iNbStepsDecade; iStep++ )
  }      // for( Int_t iDecade = 0; iDecade < iNbDecadesLog; iDecade ++)
  dBinsLog[iNbBinsLog - 1] = std::pow(10, iNbDecadesLog);

  /*******************************************************************/
  fhMessType = new TH1I("hMessageType", "Nb of message for each type; Type", 1 + gdpbv100::MSG_STAR_TRI_D, 0.,
                        1 + gdpbv100::MSG_STAR_TRI_D);
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_HIT, "HIT");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_EPOCH, "EPOCH");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_SLOWC, "SLOWC");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_SYST, "SYST");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_A, "TRI_A");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_B, "TRI_B");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_C, "TRI_C");
  fhMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_D, "TRI_D");

  fhSysMessType = new TH1I("hSysMessType", "Nb of system message for each type; System Type", 1 + gdpbv100::SYS_PATTERN,
                           0., 1 + gdpbv100::SYS_PATTERN);
  fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GET4_ERROR, "GET4 ERROR");
  fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GDPB_UNKWN, "UNKW GET4 MSG");
  fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GET4_SYNC_MISS, "SYS_GET4_SYNC_MISS");
  fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_PATTERN, "SYS_PATTERN");

  fhGet4MessType = new TH2I("hGet4MessType", "Nb of message for each type per GET4; GET4 chip # ; Type", fuNrOfGet4, 0.,
                            fuNrOfGet4, 4, 0., 4.);
  fhGet4MessType->GetYaxis()->SetBinLabel(1, "DATA 32b");
  fhGet4MessType->GetYaxis()->SetBinLabel(2, "EPOCH");
  fhGet4MessType->GetYaxis()->SetBinLabel(3, "S.C. M");
  fhGet4MessType->GetYaxis()->SetBinLabel(4, "ERROR");
  //   fhGet4MessType->GetYaxis()->SetBinLabel( 5, "DATA 24b");
  //   fhGet4MessType->GetYaxis()->SetBinLabel( 6, "STAR Trigger");

  fhGet4ChanScm = new TH2I("hGet4ChanScm", "SC messages per GET4 channel; GET4 channel # ; SC type",
                           2 * fuNrOfGet4 * fuNrOfChannelsPerGet4, 0., fuNrOfGet4 * fuNrOfChannelsPerGet4, 5, 0., 5.);
  fhGet4ChanScm->GetYaxis()->SetBinLabel(1, "Hit Scal");
  fhGet4ChanScm->GetYaxis()->SetBinLabel(2, "Deadtime");
  fhGet4ChanScm->GetYaxis()->SetBinLabel(3, "SPI");
  fhGet4ChanScm->GetYaxis()->SetBinLabel(4, "SEU Scal");
  fhGet4ChanScm->GetYaxis()->SetBinLabel(5, "START");

  fhGet4ChanErrors = new TH2I("hGet4ChanErrors", "Error messages per GET4 channel; GET4 channel # ; Error",
                              fuNrOfGet4 * fuNrOfChannelsPerGet4, 0., fuNrOfGet4 * fuNrOfChannelsPerGet4, 21, 0., 21.);
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
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(12, "0x0b: DLL Lock error  ");  // <- From GET4 v1.2
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(13, "0x0c: DLL Reset invoc.");  // <- From GET4 v1.2
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(14, "0x11: Overwrite       ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(15, "0x12: ToT out of range");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(16, "0x13: Event Discarded ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(17, "0x14: Add. Rising edge");  // <- From GET4 v1.3
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(18, "0x15: Unpaired Falling");  // <- From GET4 v1.3
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(19, "0x16: Sequence error  ");  // <- From GET4 v1.3
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(20, "0x7f: Unknown         ");
  fhGet4ChanErrors->GetYaxis()->SetBinLabel(21, "Corrupt/unsuprtd error");

  fhGet4EpochFlags =
    new TH2I("hGet4EpochFlags", "Epoch flags per GET4; GET4 chip # ; Type", fuNrOfGet4, 0., fuNrOfGet4, 4, 0., 4.);
  fhGet4EpochFlags->GetYaxis()->SetBinLabel(1, "SYNC");
  fhGet4EpochFlags->GetYaxis()->SetBinLabel(2, "Ep LOSS");
  fhGet4EpochFlags->GetYaxis()->SetBinLabel(3, "Da LOSS");
  fhGet4EpochFlags->GetYaxis()->SetBinLabel(4, "MISSMAT");

  Double_t dSectorMin = -0.5 + fUnpackPar->GetGdpbToSectorOffset();
  Double_t dSectorMax = fuNrOfGdpbs + dSectorMin;
  fhGdpbAsicSpiCounts =
    new TH2I("hGdpbAsicSpiCounts", "SPI messages count per Sector and ASIC; ASIC Idx []; Sector []; SPI msg[]",
             fuNrOfGet4PerGdpb, -0.5, fuNrOfGet4PerGdpb - 0.5, fuNrOfGdpbs, dSectorMin, dSectorMax);

  fhGdpbMessType =
    new TH2I("hGdpbMessageType", "Nb of message for each type per Sector; Type; Sector []",
             1 + gdpbv100::MSG_STAR_TRI_D, 0., 1 + gdpbv100::MSG_STAR_TRI_D, fuNrOfGdpbs, dSectorMin, dSectorMax);
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_HIT, "HIT");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_EPOCH, "EPOCH");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_SLOWC, "SLOWC");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_SYST, "SYST");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_A, "TRI_A");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_B, "TRI_B");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_C, "TRI_C");
  fhGdpbMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_D, "TRI_D");

  fhGdpbSysMessType =
    new TH2I("hGdpbSysMessType", "Nb of system message for each type per Sector; System Type; Sector []",
             1 + gdpbv100::SYS_PATTERN, 0., 1 + gdpbv100::SYS_PATTERN, fuNrOfGdpbs, dSectorMin, dSectorMax);
  fhGdpbSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GET4_ERROR, "GET4 ERROR");
  fhGdpbSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GDPB_UNKWN, "UNKW GET4 MSG");
  fhGdpbSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_GET4_SYNC_MISS, "SYS_GET4_SYNC_MISS");
  fhGdpbSysMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::SYS_PATTERN, "SYS_PATTERN");

  fhGdpbSysMessPattType =
    new TH2I("hGdpbSysMessPattType", "Nb of pattern message for each type per Sector; Pattern Type; Sector []",
             1 + gdpbv100::PATT_RESYNC, 0., 1 + gdpbv100::PATT_RESYNC, fuNrOfGdpbs, dSectorMin, dSectorMax);
  fhGdpbSysMessPattType->GetXaxis()->SetBinLabel(1 + gdpbv100::PATT_MISSMATCH, "PATT_MISSMATCH");
  fhGdpbSysMessPattType->GetXaxis()->SetBinLabel(1 + gdpbv100::PATT_ENABLE, "PATT_ENABLE");
  fhGdpbSysMessPattType->GetXaxis()->SetBinLabel(1 + gdpbv100::PATT_RESYNC, "PATT_RESYNC");

  fhGdpbEpochFlags = new TH2I("hGdpbEpochFlags", "Epoch flags per Sector; Sector # ; Type", fuNrOfGdpbs, dSectorMin,
                              dSectorMax, 4, 0., 4.);
  fhGdpbEpochFlags->GetYaxis()->SetBinLabel(1, "SYNC");
  fhGdpbEpochFlags->GetYaxis()->SetBinLabel(2, "Ep LOSS");
  fhGdpbEpochFlags->GetYaxis()->SetBinLabel(3, "Da LOSS");
  fhGdpbEpochFlags->GetYaxis()->SetBinLabel(4, "MISSMAT");

  fhGdpbEpochSyncEvo = new TH2D("hGdpbEpochSyncEvo", "Epoch SYNC per second and Sector; Time[s];  Sector #; SYNC Nb",
                                fuHistoryHistoSize, 0, fuHistoryHistoSize, fuNrOfGdpbs, dSectorMin, dSectorMax);

  fhGdpbEpochMissEvo =
    new TH2D("hGdpbEpochMissEvo", "Epoch Missmatch per second and Sector; Time[s];  Sector #; Missmatch Nb",
             fuHistoryHistoSize, 0, fuHistoryHistoSize, fuNrOfGdpbs, dSectorMin, dSectorMax);

  fhGdpbEndMsBufferNotEmpty =
    new TH1D("hGdpbEndMsBufferNotEmpty", "MS where buffer is not empty at end, per Sector; Sector #; Bad MS",
             fuNrOfGdpbs, dSectorMin, dSectorMax);

  fhGdpbEndMsDataLost = new TH2D("hGdpbEndMsDataLost",
                                 "Amount of lost data when buffer not empty at end, per MS and "
                                 "Sector; Sector #; Lost Data per bad MS []; Bad MS",
                                 fuNrOfGdpbs, dSectorMin, dSectorMax, iNbBinsLog - 1, dBinsLog);

  if (kTRUE == fbDebugMonitorMode) {
    fhNbMissPatternPerMs = new TH2I("hNbMissPatternPerMs",
                                    "Nb of missmatch pattern per MS for each sector; Number of "
                                    "pattern messages []; sector []; MS",
                                    1000, -0.5, 999.5, fuNrOfGdpbs, dSectorMin, dSectorMax);

    fhPatternMissmatch =
      new TH2I("hPatternMissmatch", "Missmatch pattern integral per Sector; ASIC Pattern []; Sector []",
               fuNrOfGet4PerGdpb, 0., fuNrOfGet4PerGdpb, fuNrOfGdpbs, dSectorMin, dSectorMax);

    fhPatternEnable = new TH2I("hPatternEnable", "Enable pattern integral per Sector; ASIC Pattern []; Sector []",
                               fuNrOfGet4PerGdpb, 0., fuNrOfGet4PerGdpb, fuNrOfGdpbs, dSectorMin, dSectorMax);

    fhPatternResync = new TH2I("hPatternResync", "Resync pattern integral per Sector; ASIC Pattern []; Sector []",
                               fuNrOfGet4PerGdpb, 0., fuNrOfGet4PerGdpb, fuNrOfGdpbs, dSectorMin, dSectorMax);
  }  // if( kTRUE == fbDebugMonitorMode )

  /// Add pointers to the vector with all histo for access by steering class
  AddHistoToVector(fhMessType, sFolder);
  AddHistoToVector(fhSysMessType, sFolder);
  /// Per GET4 in system
  AddHistoToVector(fhGet4MessType, sFolder);
  AddHistoToVector(fhGet4ChanScm, sFolder);
  AddHistoToVector(fhGet4ChanErrors, sFolder);
  AddHistoToVector(fhGet4EpochFlags, sFolder);
  AddHistoToVector(fhGdpbAsicSpiCounts, sFolder);
  /// Per Gdpb
  AddHistoToVector(fhGdpbMessType, sFolder);
  AddHistoToVector(fhGdpbSysMessType, sFolder);
  AddHistoToVector(fhGdpbSysMessPattType, sFolder);
  AddHistoToVector(fhGdpbEpochFlags, sFolder);
  AddHistoToVector(fhGdpbEpochSyncEvo, sFolder);
  AddHistoToVector(fhGdpbEpochMissEvo, sFolder);
  AddHistoToVector(fhGdpbEndMsBufferNotEmpty, sFolder);
  AddHistoToVector(fhGdpbEndMsDataLost, sFolder);
  /// Pattern Messages
  /// Pattern messages per gDPB
  if (kTRUE == fbDebugMonitorMode) {
    AddHistoToVector(fhNbMissPatternPerMs, sFolder);
    AddHistoToVector(fhPatternMissmatch, sFolder);
    AddHistoToVector(fhPatternEnable, sFolder);
    AddHistoToVector(fhPatternResync, sFolder);
  }  // if( kTRUE == fbDebugMonitorMode )

  /*******************************************************************/
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    UInt_t uSectorIndex = uGdpb + fUnpackPar->GetGdpbToSectorOffset();
    if (-1 < fiSectorIndex) uSectorIndex += fiSectorIndex;

    std::string sFolderSector     = Form("sector%02u", uSectorIndex);
    std::string sFolderSectorPatt = Form("sector%02u/Pattern", uSectorIndex);
    std::string sFolderSectorTrig = Form("sector%02u/Trigger", uSectorIndex);

    fvhGdpbGet4MessType.push_back(new TH2I(Form("hGdpbGet4MessType_%02u", uGdpb),
                                           Form("Nb of message for each type per GET4 in Sector %02u; GET4 "
                                                "chip # ; Type",
                                                uSectorIndex),
                                           fuNrOfGet4PerGdpb, 0., fuNrOfGet4PerGdpb, 4, 0., 4.));
    fvhGdpbGet4MessType[uGdpb]->GetYaxis()->SetBinLabel(1, "DATA 32b");
    fvhGdpbGet4MessType[uGdpb]->GetYaxis()->SetBinLabel(2, "EPOCH");
    fvhGdpbGet4MessType[uGdpb]->GetYaxis()->SetBinLabel(3, "S.C. M");
    fvhGdpbGet4MessType[uGdpb]->GetYaxis()->SetBinLabel(4, "ERROR");

    fvhGdpbGet4ChanScm.push_back(
      new TH2I(Form("hGdpbGet4ChanScm_%02u", uGdpb),
               Form("SC messages per GET4 channel in Sector %02u; GET4 channel # ; SC type", uSectorIndex),
               2 * fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb, 5, 0., 5.));
    fvhGdpbGet4ChanScm[uGdpb]->GetYaxis()->SetBinLabel(1, "Hit Scal");
    fvhGdpbGet4ChanScm[uGdpb]->GetYaxis()->SetBinLabel(2, "Deadtime");
    fvhGdpbGet4ChanScm[uGdpb]->GetYaxis()->SetBinLabel(3, "SPI");
    fvhGdpbGet4ChanScm[uGdpb]->GetYaxis()->SetBinLabel(4, "SEU Scal");
    fvhGdpbGet4ChanScm[uGdpb]->GetYaxis()->SetBinLabel(5, "START");

    fvhGdpbGet4ChanErrors.push_back(new TH2I(Form("hGdpbGet4ChanErrors_%02u", uGdpb),
                                             Form("Error messages per GET4 channel in Sector %02u; GET4 "
                                                  "channel # ; Error",
                                                  uSectorIndex),
                                             fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb, 22, 0., 22.));
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(1, "0x00: Readout Init    ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(2, "0x01: Sync            ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(3, "0x02: Epoch count sync");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(4, "0x03: Epoch           ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(5, "0x04: FIFO Write      ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(6, "0x05: Lost event      ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(7, "0x06: Channel state   ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(8, "0x07: Token Ring state");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(9, "0x08: Token           ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(10, "0x09: Error Readout   ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(11, "0x0a: SPI             ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(12, "0x0b: DLL Lock error  ");  // <- From GET4 v1.2
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(13, "0x0c: DLL Reset invoc.");  // <- From GET4 v1.2
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(14, "0x11: Overwrite       ");  // <- From GET4 v1.0 to 1.3
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(15, "0x12: ToT out of range");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(16, "0x13: Event Discarded ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(17, "0x14: Add. Rising edge");  // <- From GET4 v1.3
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(18, "0x15: Unpaired Falling");  // <- From GET4 v1.3
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(19, "0x16: Sequence error  ");  // <- From GET4 v1.3
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(20, "0x17: Epoch Overflow  ");  // <- From GET4 v2.0
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(21, "0x7f: Unknown         ");
    fvhGdpbGet4ChanErrors[uGdpb]->GetYaxis()->SetBinLabel(22, "Corrupt/unsuprtd error");

    /// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ///
    if (kTRUE == fbDebugMonitorMode) {
      fvhRawFt_gDPB.push_back(new TH2I(Form("RawFt_gDPB_%02u", uSectorIndex),
                                       Form("Raw FineTime Sector %02u Plot 0; channel; FineTime [bin]", uSectorIndex),
                                       fuNrOfChannelsPerGdpb, 0, fuNrOfChannelsPerGdpb, 128, 0, 128));

      fvhRawCt_gDPB.push_back(
        new TH2I(Form("RawCt_gDPB_%02u", uSectorIndex),
                 Form("Raw CoarseTime Sector %02u Plot 0; channel; CoarseTime [bin]", uSectorIndex),
                 fuNrOfChannelsPerGdpb, 0, fuNrOfChannelsPerGdpb, 4096, 0, 4096));
    }  // if( kTRUE == fbDebugMonitorMode )

    fvhRemapTot_gDPB.push_back(new TH2I(Form("RemapTot_gDPB_%02u", uSectorIndex),
                                        Form("Raw TOT Sector %02u remapped; PADI channel; TOT [bin]", uSectorIndex),
                                        fuNrOfChannelsPerGdpb, 0, fuNrOfChannelsPerGdpb, 256, 0, 256));

    fvhRemapChCount_gDPB.push_back(
      new TH1I(Form("RemapChCount_gDPB_%02u", uSectorIndex),
               Form("Channel counts Sector %02u remapped; PADI channel; Hits", uSectorIndex),
               fuNrOfFeePerGdpb * fuNrOfChannelsPerFee, 0, fuNrOfFeePerGdpb * fuNrOfChannelsPerFee));

    fvhRemapChRate_gDPB.push_back(new TH2D(Form("RemapChRate_gDPB_%02u", uSectorIndex),
                                           Form("PADI channel rate Sector %02u; Time in run [s]; PADI "
                                                "channel; Rate [1/s]",
                                                uSectorIndex),
                                           fuHistoryHistoSize, 0, fuHistoryHistoSize,
                                           fuNrOfFeePerGdpb * fuNrOfChannelsPerFee, 0,
                                           fuNrOfFeePerGdpb * fuNrOfChannelsPerFee));

    /// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ///
    if (kTRUE == fbDebugMonitorMode) {
      fvhGdpbPatternMissmatchEvo.push_back(new TH2I(Form("hGdpbPatternMissmatchEvo_%02u", uSectorIndex),
                                                    Form("Missmatch pattern vs TS index in Sector %02u; TS # ; "
                                                         "ASIC Pattern []",
                                                         uSectorIndex),
                                                    10000, 0., 100000, fuNrOfGet4PerGdpb, 0., fuNrOfGet4PerGdpb));

      fvhGdpbPatternEnableEvo.push_back(
        new TH2I(Form("hGdpbPatternEnableEvo_%02u", uSectorIndex),
                 Form("Enable pattern vs TS index in Sector %02u; TS # ; ASIC Pattern []", uSectorIndex), 10000, 0.,
                 100000, fuNrOfGet4PerGdpb, 0., fuNrOfGet4PerGdpb));

      fvhGdpbPatternResyncEvo.push_back(
        new TH2I(Form("hGdpbPatternResyncEvo%02u", uSectorIndex),
                 Form("Resync pattern vs TS index in Sector %02u; TS # ; ASIC Pattern []", uSectorIndex), 10000, 0.,
                 100000, fuNrOfGet4PerGdpb, 0., fuNrOfGet4PerGdpb));

      fvhGdpbMissmatchEvoPerTs.push_back(
        new TH2I(Form("hGdpbMissmatchEvoPerTs%02u", uSectorIndex),
                 Form("Missmatch vs TS index in Sector %02u; TS # ; Asic []; Missmatch? []", uSectorIndex), 10000, 0.,
                 100000, fuNrOfGet4PerGdpb, 0., fuNrOfGet4PerGdpb));

      fvhGdpbMissmatchEnaEvoPerTs.push_back(new TH2I(Form("hGdpbMissmatchEnaEvoPerTs%02u", uSectorIndex),
                                                     Form("Enable+Missmatch vs TS index in Sector %02u; TS # ; "
                                                          "Asic []; Enabled & Missmatch? []",
                                                          uSectorIndex),
                                                     10000, 0., 100000, fuNrOfGet4PerGdpb, 0., fuNrOfGet4PerGdpb));

      fvhGdpbEnableEvoPerTs.push_back(
        new TH2I(Form("hGdpbEnableEvoPerTs%02u", uSectorIndex),
                 Form("Enable vs TS index in Sector %02u; TS # ; Asic []; Enabled? []", uSectorIndex), 100000, 0.,
                 100000, fuNrOfGet4PerGdpb, 0., fuNrOfGet4PerGdpb));

      fvhGdpbResyncEvoPerTs.push_back(
        new TH2I(Form("hGdpbResyncEvoPerTs%02u", uSectorIndex),
                 Form("Resync vs TS index in Sector %02u; TS # ; Asic []; Resync? []", uSectorIndex), 10000, 0., 100000,
                 fuNrOfGet4PerGdpb, 0., fuNrOfGet4PerGdpb));

      fvhGdpbResyncEnaEvoPerTs.push_back(new TH2I(Form("hGdpbResyncEnaEvoPerTs%02u", uSectorIndex),
                                                  Form("Enable+Resync vs TS index in Sector %02u; TS # ; Asic "
                                                       "[]; Enabled & Resync? []",
                                                       uSectorIndex),
                                                  10000, 0., 100000, fuNrOfGet4PerGdpb, 0., fuNrOfGet4PerGdpb));

      fvhGdpbStateEvoPerTs.push_back(new TH2I(Form("hGdpbStateEvoPerTs%02u", uSectorIndex),
                                              Form("ASIC State vs TS index in Sector %02u; TS # ; Asic []; 0 = Off, "
                                                   "1 = OK, 2 = Miss, 3 = Resync, 4 = Miss + Resync []",
                                                   uSectorIndex),
                                              100000, 0., 100000, fuNrOfGet4PerGdpb, 0., fuNrOfGet4PerGdpb));
    }  // if( kTRUE == fbDebugMonitorMode )

    /// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ///
    /// STAR Trigger decoding and monitoring
    fvhTokenMsgType.push_back(new TH1F(Form("hTokenMsgType_gDPB_%02u", uSectorIndex),
                                       Form("STAR trigger Messages type Sector %02u; Type ; Counts", uSectorIndex), 4,
                                       0, 4));
    fvhTokenMsgType[uGdpb]->GetXaxis()->SetBinLabel(1, "A");  // gDPB TS high
    fvhTokenMsgType[uGdpb]->GetXaxis()->SetBinLabel(2, "B");  // gDPB TS low, STAR TS high
    fvhTokenMsgType[uGdpb]->GetXaxis()->SetBinLabel(3, "C");  // STAR TS mid
    fvhTokenMsgType[uGdpb]->GetXaxis()->SetBinLabel(4, "D");  // STAR TS low, token, CMDs

    fvhTriggerRate.push_back(
      new TH1F(Form("hTriggerRate_gDPB_%02u", uSectorIndex),
               Form("STAR trigger signals per second Sector %02u; Time[s] ; Counts", uSectorIndex), fuHistoryHistoSize,
               0, fuHistoryHistoSize));

    fvhCmdDaqVsTrig.push_back(new TH2I(
      Form("hCmdDaqVsTrig_gDPB_%02u", uSectorIndex),
      Form("STAR daq command VS STAR trigger command Sector %02u; DAQ ; TRIGGER", uSectorIndex), 16, 0, 16, 16, 0, 16));
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(1, "0x0: no-trig ");  // idle link
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(
      2, "0x1: clear   ");  // clears redundancy counters on the readout boards
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(3, "0x2: mast-rst");  // general reset of the whole front-end logic
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(4, "0x3: spare   ");  // reserved
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
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(14, "0xD: abort   ");  // aborts and clears an active event
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(15, "0xE: L1accept");  //
    fvhCmdDaqVsTrig[uGdpb]->GetXaxis()->SetBinLabel(16, "0xF: L2accept");  //
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(1, "0x0:  0");         // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(2, "0x1:  1");         // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(3, "0x2:  2");         // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(4, "0x3:  3");         // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(5, "0x4:  4");         // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(6, "0x5:  5");         // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(7, "0x6:  6");         // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(8, "0x7:  7");         // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(9, "0x8:  8");         // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(10, "0x9:  9");        // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(11, "0xA: 10");        // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(12, "0xB: 11");        // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(13, "0xC: 12");        // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(14, "0xD: 13");        // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(15, "0xE: 14");        // To be filled at STAR
    fvhCmdDaqVsTrig[uGdpb]->GetYaxis()->SetBinLabel(16, "0xF: 15");        // To be filled at STAR

    fvhStarTokenEvo.push_back(new TH2I(Form("hStarTokenEvo_gDPB_%02u", uSectorIndex),
                                       Form("STAR token value VS time Sector %02u; Time in Run [s] ; "
                                            "STAR Token; Counts",
                                            uSectorIndex),
                                       fuHistoryHistoSize, 0, fuHistoryHistoSize, 410, 0, 4100));

    fvhStarTrigGdpbTsEvo.push_back(new TProfile(Form("hStarTrigGdpbTsEvo_gDPB_%02u", uSectorIndex),
                                                Form("gDPB TS in STAR triger tokens for Sector %02u; Time "
                                                     "in Run [s] ; gDPB TS;",
                                                     uSectorIndex),
                                                fuHistoryHistoSize, 0, fuHistoryHistoSize));

    fvhStarTrigStarTsEvo.push_back(new TProfile(Form("hStarTrigStarTsEvo_gDPB_%02u", uSectorIndex),
                                                Form("STAR TS in STAR triger tokens for Sector %02u; Time "
                                                     "in Run [s] ; STAR TS;",
                                                     uSectorIndex),
                                                fuHistoryHistoSize, 0, fuHistoryHistoSize));


    /// Add pointers to the vector with all histo for access by steering class
    /// Per GET4 in gDPB
    AddHistoToVector(fvhGdpbGet4MessType[uGdpb], sFolderSector);
    AddHistoToVector(fvhGdpbGet4ChanScm[uGdpb], sFolderSector);
    AddHistoToVector(fvhGdpbGet4ChanErrors[uGdpb], sFolderSector);
    /// Raw data per channel
    if (kTRUE == fbDebugMonitorMode) {
      AddHistoToVector(fvhRawFt_gDPB[uGdpb], sFolderSector);
      AddHistoToVector(fvhRawCt_gDPB[uGdpb], sFolderSector);
    }  // if( kTRUE == fbDebugMonitorMode )
    AddHistoToVector(fvhRemapTot_gDPB[uGdpb], sFolderSector);
    AddHistoToVector(fvhRemapChCount_gDPB[uGdpb], sFolderSector);
    AddHistoToVector(fvhRemapChRate_gDPB[uGdpb], sFolderSector);
    if (kTRUE == fbDebugMonitorMode) {
      /// Per MS in gDPB
      AddHistoToVector(fvhGdpbPatternMissmatchEvo[uGdpb], sFolderSectorPatt);
      AddHistoToVector(fvhGdpbPatternEnableEvo[uGdpb], sFolderSectorPatt);
      AddHistoToVector(fvhGdpbPatternResyncEvo[uGdpb], sFolderSectorPatt);
      /// State Per TS
      AddHistoToVector(fvhGdpbMissmatchEvoPerTs[uGdpb], sFolderSectorPatt);
      AddHistoToVector(fvhGdpbMissmatchEnaEvoPerTs[uGdpb], sFolderSectorPatt);
      AddHistoToVector(fvhGdpbEnableEvoPerTs[uGdpb], sFolderSectorPatt);
      AddHistoToVector(fvhGdpbResyncEvoPerTs[uGdpb], sFolderSectorPatt);
      AddHistoToVector(fvhGdpbResyncEnaEvoPerTs[uGdpb], sFolderSectorPatt);
      AddHistoToVector(fvhGdpbStateEvoPerTs[uGdpb], sFolderSectorPatt);
    }  // if( kTRUE == fbDebugMonitorMode )
       /// STAR TRIGGER detection
    AddHistoToVector(fvhTokenMsgType[uGdpb], sFolderSectorTrig);
    AddHistoToVector(fvhTriggerRate[uGdpb], sFolderSectorTrig);
    AddHistoToVector(fvhCmdDaqVsTrig[uGdpb], sFolderSectorTrig);
    AddHistoToVector(fvhStarTokenEvo[uGdpb], sFolderSectorTrig);
    AddHistoToVector(fvhStarTrigGdpbTsEvo[uGdpb], sFolderSectorTrig);
    AddHistoToVector(fvhStarTrigStarTsEvo[uGdpb], sFolderSectorTrig);
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  /*******************************************************************/
  fhMsgCntEvo   = new TH1I("hMsgCntEvo",
                         "Evolution of Hit & error msgs counts vs time in run; "
                         "Time in run [s]; Msgs Count []",
                         fuHistoryHistoSize, 0, fuHistoryHistoSize);
  fhHitCntEvo   = new TH1I("hHitCntEvo", "Evolution of Hit counts vs time in run; Time in run [s]; Hits Count []",
                         fuHistoryHistoSize, 0, fuHistoryHistoSize);
  fhErrorCntEvo = new TH1I("hErrorCntEvo", "Evolution of Error counts vs time in run; Time in run [s]; Error Count []",
                           fuHistoryHistoSize, 0, fuHistoryHistoSize);
  fhLostEvtCntEvo = new TH1I("hLostEvtCntEvo",
                             "Evolution of LostEvent counts vs time in run; "
                             "Time in run [s]; LostEvent Count []",
                             fuHistoryHistoSize, 0, fuHistoryHistoSize);

  fhErrorFractEvo   = new TProfile("hErrorFractEvo",
                                 "Evolution of Error Fraction vs time in run; "
                                 "Time in run [s]; Error Fract []",
                                 fuHistoryHistoSize, 0, fuHistoryHistoSize);
  fhLostEvtFractEvo = new TProfile("hLostEvtFractEvo",
                                   "Evolution of LostEvent Fraction vs time in "
                                   "run; Time in run [s]; LostEvent Fract []",
                                   fuHistoryHistoSize, 0, fuHistoryHistoSize);

  fhMsgCntPerMsEvo     = new TH2I("hMsgCntPerMsEvo",
                              "Evolution of Hit & error msgs counts, per MS vs time in run; "
                              "Time in run [s]; Hits Count/MS []; MS",
                              fuHistoryHistoSize, 0, fuHistoryHistoSize, iNbBinsLog - 1, dBinsLog);
  fhHitCntPerMsEvo     = new TH2I("hHitCntPerMsEvo",
                              "Evolution of Hit counts, per MS vs time in run; "
                              "Time in run [s]; Hits Count/MS []; MS",
                              fuHistoryHistoSize, 0, fuHistoryHistoSize, iNbBinsLog - 1, dBinsLog);
  fhErrorCntPerMsEvo   = new TH2I("hErrorCntPerMsEvo",
                                "Evolution of Error counts, per MS vs time in "
                                "run; Time in run [s]; Error Count/MS []; MS",
                                fuHistoryHistoSize, 0, fuHistoryHistoSize, iNbBinsLog - 1, dBinsLog);
  fhLostEvtCntPerMsEvo = new TH2I("hLostEvtCntPerMsEvo",
                                  "Evolution of LostEvent, per MS counts vs time in run; Time in "
                                  "run [s]; LostEvent Count/MS []; MS",
                                  fuHistoryHistoSize, 0, fuHistoryHistoSize, iNbBinsLog - 1, dBinsLog);

  fhErrorFractPerMsEvo   = new TH2I("hErrorFractPerMsEvo",
                                  "Evolution of Error Fraction, per MS vs time in run; Time in run "
                                  "[s]; Error Fract/MS []; MS",
                                  fuHistoryHistoSize, 0, fuHistoryHistoSize, 1000, 0, 1);
  fhLostEvtFractPerMsEvo = new TH2I("hLostEvtFractPerMsEvo",
                                    "Evolution of LostEvent Fraction, per MS vs time in run; Time in "
                                    "run [s]; LostEvent Fract/MS []; MS",
                                    fuHistoryHistoSize, 0, fuHistoryHistoSize, 1000, 0, 1);

  /// Add pointers to the vector with all histo for access by steering class
  AddHistoToVector(fhMsgCntEvo, sFolder);
  AddHistoToVector(fhHitCntEvo, sFolder);
  AddHistoToVector(fhErrorCntEvo, sFolder);
  AddHistoToVector(fhLostEvtCntEvo, sFolder);

  AddHistoToVector(fhErrorFractEvo, sFolder);
  AddHistoToVector(fhLostEvtFractEvo, sFolder);

  AddHistoToVector(fhHitCntPerMsEvo, sFolder);
  AddHistoToVector(fhErrorCntPerMsEvo, sFolder);
  AddHistoToVector(fhLostEvtCntPerMsEvo, sFolder);
  AddHistoToVector(fhErrorFractPerMsEvo, sFolder);
  AddHistoToVector(fhLostEvtFractPerMsEvo, sFolder);

  /*******************************************************************/

  /*******************************************************************/
  /// Canvases
  /// General summary: Hit maps, Hit rate vs time in run, error fraction vs time un run
  fcSummary = new TCanvas("cSummary", "gDPB Monitoring Summary");
  fcSummary->Divide(2, 3);

  // 1st Column: Messages types
  fcSummary->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhMessType->Draw();

  fcSummary->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhSysMessType->Draw();

  fcSummary->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhGet4MessType->Draw("colz");

  // 2nd Column: GET4 Errors + Epoch flags + SCm
  fcSummary->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhGet4ChanErrors->Draw("colz");

  fcSummary->cd(5);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhGet4EpochFlags->Draw("colz");

  fcSummary->cd(6);
  gPad->SetGridx();
  gPad->SetGridy();
  fhGet4ChanScm->Draw("colz");

  AddCanvasToVector(fcSummary, "canvases");
  /// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ///

  /// Create summary Canvases with plots VS Gdpb
  fcSummaryGdpb = new TCanvas("cSummaryGdpb", "gDPB Monitoring Summary");
  fcSummaryGdpb->Divide(2, 3);

  fcSummaryGdpb->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhGdpbMessType->Draw("colz");

  fcSummaryGdpb->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhGdpbSysMessPattType->Draw("text colz");

  fcSummaryGdpb->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhGdpbSysMessType->Draw("colz");

  fcSummaryGdpb->cd(5);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhGdpbEpochFlags->Draw("text colz");

  fcSummaryGdpb->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  fhGdpbEpochSyncEvo->Draw("colz");

  fcSummaryGdpb->cd(6);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhGdpbEpochMissEvo->Draw("colz");

  AddCanvasToVector(fcSummaryGdpb, "canvases");
  /// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ///
  fcStarTrigTokenType    = new TCanvas("cStarTrigTokenType", "STAR trigger token message type per sector");
  fcStarTriggerRate      = new TCanvas("cStarTriggerRate", "STAR trigger rate per sector");
  fcStarTrigCmdDaqVsTrig = new TCanvas("cStarTrigCmdDaqVsTrig", "STAR trigger command types per sector");
  fcStarTrigStarTokenEvo = new TCanvas("cStarTrigStarTokenEvo", "STAR trigger token evolution per sector");
  fcStarTrigGdpbTsEvo    = new TCanvas("cStarTrigGdpbTsEvo", "STAR trigger gDPB TS evo per sector");
  fcStarTrigStarTsEvo    = new TCanvas("cStarTrigStarTsEvo", "STAR trigger STAR TS evo per sector");

  fcStarTrigTokenType->Divide(4, 3);
  fcStarTriggerRate->Divide(4, 3);
  fcStarTrigCmdDaqVsTrig->Divide(4, 3);
  fcStarTrigStarTokenEvo->Divide(4, 3);
  fcStarTrigGdpbTsEvo->Divide(4, 3);
  fcStarTrigStarTsEvo->Divide(4, 3);

  AddCanvasToVector(fcStarTrigTokenType, "canvases");
  AddCanvasToVector(fcStarTriggerRate, "canvases");
  AddCanvasToVector(fcStarTrigCmdDaqVsTrig, "canvases");
  AddCanvasToVector(fcStarTrigStarTokenEvo, "canvases");
  AddCanvasToVector(fcStarTrigGdpbTsEvo, "canvases");
  AddCanvasToVector(fcStarTrigStarTsEvo, "canvases");

  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    UInt_t uSectorIndex = uGdpb + fUnpackPar->GetGdpbToSectorOffset();
    if (-1 < fiSectorIndex) uSectorIndex += fiSectorIndex;

    fvcSumGdpbGet4.push_back(new TCanvas(Form("cSumSector%02u", uSectorIndex),
                                         Form("Summary per GET4 or channel for sector %02u", uSectorIndex)));
    fvcSumGdpbGet4[uGdpb]->Divide(2, 2);

    fvcSumGdpbGet4[uGdpb]->cd(1);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhGdpbGet4MessType[uGdpb]->Draw("colz");

    fvcSumGdpbGet4[uGdpb]->cd(2);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhGdpbGet4ChanScm[uGdpb]->Draw("colz");

    fvcSumGdpbGet4[uGdpb]->cd(3);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhGdpbGet4ChanErrors[uGdpb]->Draw("colz");

    fvcSumGdpbGet4[uGdpb]->cd(4);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhRemapTot_gDPB[uGdpb]->Draw("colz");


    AddCanvasToVector(fvcSumGdpbGet4[uGdpb], "canvases");

    fcStarTrigTokenType->cd(1 + uGdpb);
    gPad->SetGridx();
    gPad->SetGridy();
    fvhTokenMsgType[uGdpb]->Draw("hist");

    fcStarTriggerRate->cd(1 + uGdpb);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();
    fvhTriggerRate[uGdpb]->Draw("hist");

    fcStarTrigCmdDaqVsTrig->cd(1 + uGdpb);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhCmdDaqVsTrig[uGdpb]->Draw("colz");

    fcStarTrigStarTokenEvo->cd(1 + uGdpb);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhStarTokenEvo[uGdpb]->Draw("colz");

    fcStarTrigGdpbTsEvo->cd(1 + uGdpb);
    gPad->SetGridx();
    gPad->SetGridy();
    fvhStarTrigGdpbTsEvo[uGdpb]->Draw("hist");

    fcStarTrigStarTsEvo->cd(1 + uGdpb);
    gPad->SetGridx();
    gPad->SetGridy();
    fvhStarTrigStarTsEvo[uGdpb]->Draw("hist");
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  /*******************************************************************/

  return kTRUE;
}
Bool_t CbmStar2019MonitorAlgo::FillHistograms()
{
  if (kTRUE == fbDebugMonitorMode) {
    /// Fill plot of Enable flag state per TS
    for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb)
      for (UInt_t uAsic = 0; uAsic < fuNrOfGet4PerGdpb; ++uAsic) {
        if (fvvbGdpbLastMissmatchPattern[uGdpb][uAsic]) {
          fvhGdpbMissmatchEvoPerTs[uGdpb]->Fill(fulCurrentTsIdx, uAsic);
        }  // if( fvvbGdpbLastMissmatchPattern[ uGdpb ][ uAsic ] )

        if (fvvbGdpbLastEnablePattern[uGdpb][uAsic]) {
          fvhGdpbEnableEvoPerTs[uGdpb]->Fill(fulCurrentTsIdx, uAsic);

          if (fvvbGdpbLastMissmatchPattern[uGdpb][uAsic]) {
            fvhGdpbMissmatchEnaEvoPerTs[uGdpb]->Fill(fulCurrentTsIdx, uAsic);
          }  // if( fvvbGdpbLastMissmatchPattern[ uGdpb ][ uAsic ] )

          if (fvvbGdpbLastResyncPattern[uGdpb][uAsic]) {
            fvhGdpbResyncEnaEvoPerTs[uGdpb]->Fill(fulCurrentTsIdx, uAsic);
          }  // if( fvvbGdpbLastResyncPattern[ uGdpb ][ uAsic ] )

          /// State evolution
          if (fvvbGdpbLastMissmatchPattern[uGdpb][uAsic]) {
            if (fvvbGdpbLastResyncPattern[uGdpb][uAsic]) {
              fvhGdpbStateEvoPerTs[uGdpb]->Fill(fulCurrentTsIdx, uAsic, 4);
            }  // if( fvvbGdpbLastResyncPattern[ uGdpb ][ uAsic ] )
            else
              fvhGdpbStateEvoPerTs[uGdpb]->Fill(fulCurrentTsIdx, uAsic, 2);
          }  // if( fvvbGdpbLastMissmatchPattern[ uGdpb ][ uAsic ] )
          else if (fvvbGdpbLastResyncPattern[uGdpb][uAsic]) {
            fvhGdpbStateEvoPerTs[uGdpb]->Fill(fulCurrentTsIdx, uAsic, 3);
          }  // else if( fvvbGdpbLastResyncPattern[ uGdpb ][ uAsic ] )
          else
            fvhGdpbStateEvoPerTs[uGdpb]->Fill(fulCurrentTsIdx, uAsic, 1);
        }  // if( fvvbGdpbLastEnablePattern[ uGdpb ][ uAsic ] )
        else
          fvhGdpbStateEvoPerTs[uGdpb]->Fill(fulCurrentTsIdx, uAsic, 0);

        if (fvvbGdpbLastResyncPattern[uGdpb][uAsic]) {
          fvhGdpbResyncEvoPerTs[uGdpb]->Fill(fulCurrentTsIdx, uAsic);
        }  // if( fvvbGdpbLastResyncPattern[ uGdpb ][ uAsic ] )
      }    // Loop on gDPB and ASICs
  }        // if( kTRUE == fbDebugMonitorMode )
  /*
   UInt_t   uCountHitsInMs = 0;
   UInt_t   uCountErrorsInMs = 0;
   UInt_t   uCountLostEvtInMs = 0;

   Double_t dFractErrorsInMs  = uCountErrorsInMs;
   Double_t dFractLostEvtInMs = uCountLostEvtInMs;
   dFractErrorsInMs  /= ( uCountHitsInMs + uCountErrorsInMs );
   dFractLostEvtInMs /= ( uCountHitsInMs + uCountErrorsInMs );

   fhMsgCntPerMsEvo->Fill(       fdMsTime - fdStartTime, uCountHitsInMs + uCountErrorsInMs );
   fhHitCntPerMsEvo->Fill(       fdMsTime - fdStartTime, uCountHitsInMs );
   fhErrorCntPerMsEvo->Fill(     fdMsTime - fdStartTime, uCountErrorsInMs );
   fhLostEvtCntPerMsEvo->Fill(   fdMsTime - fdStartTime, uCountLostEvtInMs );
   fhErrorFractPerMsEvo->Fill(   fdMsTime - fdStartTime, dFractErrorsInMs );
   fhLostEvtFractPerMsEvo->Fill( fdMsTime - fdStartTime, dFractLostEvtInMs );
*/
  return kTRUE;
}
Bool_t CbmStar2019MonitorAlgo::ResetHistograms()
{
  fhMessType->Reset();
  fhSysMessType->Reset();
  fhGet4MessType->Reset();
  fhGet4ChanScm->Reset();
  fhGet4ChanErrors->Reset();
  fhGet4EpochFlags->Reset();
  fhGdpbAsicSpiCounts->Reset();
  fhGdpbMessType->Reset();
  fhGdpbSysMessType->Reset();
  fhGdpbSysMessPattType->Reset();
  fhGdpbEpochFlags->Reset();
  fhGdpbEpochSyncEvo->Reset();
  fhGdpbEpochMissEvo->Reset();
  fhGdpbEndMsBufferNotEmpty->Reset();
  fhGdpbEndMsDataLost->Reset();
  if (kTRUE == fbDebugMonitorMode) {
    fhNbMissPatternPerMs->Reset();
    fhPatternMissmatch->Reset();
    fhPatternEnable->Reset();
    fhPatternResync->Reset();
  }  // if( kTRUE == fbDebugMonitorMode )

  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fvhGdpbGet4MessType[uGdpb]->Reset();
    fvhGdpbGet4ChanScm[uGdpb]->Reset();
    fvhGdpbGet4ChanErrors[uGdpb]->Reset();
    if (kTRUE == fbDebugMonitorMode) {
      fvhRawFt_gDPB[uGdpb]->Reset();
      fvhRawCt_gDPB[uGdpb]->Reset();
    }  // if( kTRUE == fbDebugMonitorMode )
    fvhRemapTot_gDPB[uGdpb]->Reset();
    fvhRemapChCount_gDPB[uGdpb]->Reset();
    fvhRemapChRate_gDPB[uGdpb]->Reset();
    if (kTRUE == fbDebugMonitorMode) {
      fvhGdpbPatternMissmatchEvo[uGdpb]->Reset();
      fvhGdpbPatternEnableEvo[uGdpb]->Reset();
      fvhGdpbPatternResyncEvo[uGdpb]->Reset();
      fvhGdpbMissmatchEvoPerTs[uGdpb]->Reset();
      fvhGdpbMissmatchEnaEvoPerTs[uGdpb]->Reset();
      fvhGdpbEnableEvoPerTs[uGdpb]->Reset();
      fvhGdpbResyncEvoPerTs[uGdpb]->Reset();
      fvhGdpbResyncEnaEvoPerTs[uGdpb]->Reset();
      fvhGdpbStateEvoPerTs[uGdpb]->Reset();
    }  // if( kTRUE == fbDebugMonitorMode )
    fvhTokenMsgType[uGdpb]->Reset();
    fvhTriggerRate[uGdpb]->Reset();
    fvhCmdDaqVsTrig[uGdpb]->Reset();
    fvhStarTokenEvo[uGdpb]->Reset();
    fvhStarTrigGdpbTsEvo[uGdpb]->Reset();
    fvhStarTrigStarTsEvo[uGdpb]->Reset();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  fhMsgCntEvo->Reset();
  fhHitCntEvo->Reset();
  fhErrorCntEvo->Reset();
  fhLostEvtCntEvo->Reset();
  fhErrorFractEvo->Reset();
  fhLostEvtFractEvo->Reset();
  fhHitCntPerMsEvo->Reset();
  fhErrorCntPerMsEvo->Reset();
  fhLostEvtCntPerMsEvo->Reset();
  fhErrorFractPerMsEvo->Reset();
  fhLostEvtFractPerMsEvo->Reset();


  /// Also reset the Start time for the evolution plots!
  fulStartTs  = fulCurrentTsIdx;
  fdStartTime = -1.0;

  return kTRUE;
}
void CbmStar2019MonitorAlgo::ResetEvolutionHistograms()
{
  fhGdpbEpochSyncEvo->Reset();
  fhGdpbEpochMissEvo->Reset();

  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fvhRemapChRate_gDPB[uGdpb]->Reset();
    if (kTRUE == fbDebugMonitorMode) {
      //         fvhGdpbPatternMissmatchEvo[ uGdpb ] ->Reset();
      //         fvhGdpbPatternEnableEvo[ uGdpb ]    ->Reset();
      //         fvhGdpbPatternResyncEvo[ uGdpb ]    ->Reset();
      fvhGdpbMissmatchEvoPerTs[uGdpb]->Reset();
      fvhGdpbMissmatchEnaEvoPerTs[uGdpb]->Reset();
      fvhGdpbEnableEvoPerTs[uGdpb]->Reset();
      fvhGdpbResyncEvoPerTs[uGdpb]->Reset();
      fvhGdpbResyncEnaEvoPerTs[uGdpb]->Reset();
      fvhGdpbStateEvoPerTs[uGdpb]->Reset();
    }  // if( kTRUE == fbDebugMonitorMode )
    fvhTriggerRate[uGdpb]->Reset();
    fvhStarTokenEvo[uGdpb]->Reset();
    fvhStarTrigGdpbTsEvo[uGdpb]->Reset();
    fvhStarTrigStarTsEvo[uGdpb]->Reset();
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  fhMsgCntEvo->Reset();
  fhHitCntEvo->Reset();
  fhErrorCntEvo->Reset();
  fhLostEvtCntEvo->Reset();
  fhErrorFractEvo->Reset();
  fhLostEvtFractEvo->Reset();
  fhHitCntPerMsEvo->Reset();
  fhErrorCntPerMsEvo->Reset();
  fhLostEvtCntPerMsEvo->Reset();
  fhErrorFractPerMsEvo->Reset();
  fhLostEvtFractPerMsEvo->Reset();

  fdStartTime = -1;
}
// -------------------------------------------------------------------------
Bool_t CbmStar2019MonitorAlgo::SaveLatencyHistograms(TString fsHistoFileName)
{
  /// (Re-)Create ROOT file to store the histos
  TDirectory* oldDir = NULL;
  TFile* histoFile   = NULL;
  // Store current directory position to allow restore later
  oldDir = gDirectory;
  // open separate histo file in recreate mode
  histoFile = new TFile(fsHistoFileName, "RECREATE");

  if (nullptr == histoFile) return kFALSE;

  /// Make sure we end up in chosen folder
  /// => for now write everything in the root of the file!!
  /*
   TString sFolder = "";
   if( nullptr == gDirectory->Get( sFolder ) )
      gDirectory->mkdir( sFolder );
   gDirectory->cd( sFolder );
*/
  fhGdpbEpochFlags->Write();
  if (kTRUE == fbDebugMonitorMode) {
    fhPatternMissmatch->Write();
    fhPatternEnable->Write();
  }  // if( kTRUE == fbDebugMonitorMode )

  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    /// Make sure we end up in chosen folder
    /// => for now write everything in the root of the file!!
    /*
      TString sFolder = "";
      if( nullptr == gDirectory->Get( sFolder ) )
         gDirectory->mkdir( sFolder );
      gDirectory->cd( sFolder );
*/
    fvhGdpbGet4MessType[uGdpb]->Write();
    /*
      if( kTRUE == fbDebugMonitorMode )
      {
         fvhGdpbPatternMissmatchEvo[ uGdpb ]->Write();
      } // if( kTRUE == fbDebugMonitorMode )
*/
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

  /// Restore original directory position
  oldDir->cd();
  histoFile->Close();

  return kTRUE;
}
// -------------------------------------------------------------------------
