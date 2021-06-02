/** @file CbmMcbm2018MonitorAlgoTofPulser.cxx
  * @copyright Copyright (C) 2019-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmMcbm2018MonitorAlgoTofPulser                     -----
// -----               Created 12.10.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018MonitorAlgoTofPulser.h"

#include "CbmFormatMsHeaderPrintout.h"
#include "CbmMcbm2018TofPar.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TList.h"
#include "TPaveStats.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TString.h"

#include <fstream>
#include <iomanip>
#include <iostream>

#include <stdint.h>

// -------------------------------------------------------------------------
CbmMcbm2018MonitorAlgoTofPulser::CbmMcbm2018MonitorAlgoTofPulser()
  : CbmStar2019Algo()
  ,
  /// From the class itself
  fvbMaskedComponents()
  , fiGdpbIndex(-1)
  , fuUpdateFreqTs(100)
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
  , fuPulserMinTot(90)
  , fuPulserMaxTot(110)
  , fuPulserChannel(3)
  , fulCurrentTsIdx(0)
  , fulCurrentMsIdx(0)
  , fdTsStartTime(-1.0)
  , fdTsStopTimeCore(-1.0)
  , fdMsTime(-1.0)
  , fuMsIndex(0)
  , fuCurrentEquipmentId(0)
  , fuCurrDpbId(0)
  , fuCurrDpbIdx(0)
  , fuGet4Id(0)
  , fuGet4Nr(0)
  , fvulCurrentEpoch()
  , fvulCurrentEpochCycle()
  , fvulCurrentEpochFull()
  , fvmEpSupprBuffer()
  , fvvbFeeHitFound()
  , fvvdFeeHits()
  , dMinDt(0.0)
  , dMaxDt(0.0)
  , fdStartTime(-1.0)
  , fuHistoryHistoSize(1800)
  , fvvhFeePairPulserTimeDiff()
  , fhPulserTimeDiffMean(nullptr)
  , fhPulserTimeDiffRms(nullptr)
  , fhPulserTimeDiffRmsZoom(nullptr)
  , fhPulserRmsGdpbToRefEvo(nullptr)
  , fhPulserRmsGbtxToRefEvo(nullptr)
{
}
CbmMcbm2018MonitorAlgoTofPulser::~CbmMcbm2018MonitorAlgoTofPulser()
{
  /// Clear buffers
  fvmEpSupprBuffer.clear();
}

// -------------------------------------------------------------------------
Bool_t CbmMcbm2018MonitorAlgoTofPulser::Init()
{
  LOG(info) << "Initializing mCBM T0 2019 monitor algo";

  return kTRUE;
}
void CbmMcbm2018MonitorAlgoTofPulser::Reset() {}
void CbmMcbm2018MonitorAlgoTofPulser::Finish()
{
  /// Printout Goodbye message and stats

  /// Write Output histos
}

// -------------------------------------------------------------------------
Bool_t CbmMcbm2018MonitorAlgoTofPulser::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmMcbm2018MonitorAlgoTofPulser";
  Bool_t initOK = ReInitContainers();

  return initOK;
}
Bool_t CbmMcbm2018MonitorAlgoTofPulser::ReInitContainers()
{
  LOG(info) << "**********************************************";
  LOG(info) << "ReInit parameter containers for CbmMcbm2018MonitorAlgoTofPulser";

  fUnpackPar = (CbmMcbm2018TofPar*) fParCList->FindObject("CbmMcbm2018TofPar");
  if (nullptr == fUnpackPar) return kFALSE;

  Bool_t initOK = InitParameters();

  return initOK;
}
TList* CbmMcbm2018MonitorAlgoTofPulser::GetParList()
{
  if (nullptr == fParCList) fParCList = new TList();
  fUnpackPar = new CbmMcbm2018TofPar("CbmMcbm2018TofPar");
  fParCList->Add(fUnpackPar);

  return fParCList;
}
Bool_t CbmMcbm2018MonitorAlgoTofPulser::InitParameters()
{

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

  /// Check if user requested to monitor only a single selected gDPB
  if (-1 < fiGdpbIndex) {
    if (fuNrOfGdpbs <= static_cast<UInt_t>(fiGdpbIndex))
      LOG(fatal) << "Selected gDPB out of bounds relative to parameter file: " << fiGdpbIndex << " VS " << fuNrOfGdpbs;
    else
      LOG(info) << "Selected gDPB " << fiGdpbIndex << " for single gDPB analysis";
    fuNrOfGdpbs = 1;
    fGdpbIdIndexMap.clear();
    fGdpbIdIndexMap[fUnpackPar->GetGdpbId(fiGdpbIndex)] = 0;
  }  // if( -1 < fiGdpbIndex )

  /// Internal status initialization
  fvulCurrentEpoch.resize(fuNrOfGdpbs, 0);
  fvulCurrentEpochCycle.resize(fuNrOfGdpbs, 0);
  fvulCurrentEpochFull.resize(fuNrOfGdpbs, 0);
  fvvbFeeHitFound.resize(fuNrOfGdpbs);
  fvvdFeeHits.resize(fuNrOfGdpbs);
  for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb) {
    fvvbFeeHitFound[uGdpb].resize(fuNrOfFeePerGdpb, kFALSE);
    fvvdFeeHits[uGdpb].resize(fuNrOfFeePerGdpb, 0.0);
  }  // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )


  return kTRUE;
}
// -------------------------------------------------------------------------

void CbmMcbm2018MonitorAlgoTofPulser::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Add to list
  fvMsComponentsList.push_back(component);

  LOG(info) << "CbmMcbm2018MonitorAlgoTofPulser::AddMsComponentToList => Component " << component
            << " with detector ID 0x" << std::hex << usDetectorId << std::dec << " added to list";
}
// -------------------------------------------------------------------------

Bool_t CbmMcbm2018MonitorAlgoTofPulser::ProcessTs(const fles::Timeslice& ts)
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

    /// Fill histograms
    FillHistograms();
  }  // for( fuMsIndex = 0; fuMsIndex < uNbMsLoop; fuMsIndex ++ )

  return kTRUE;
}

Bool_t CbmMcbm2018MonitorAlgoTofPulser::ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx)
{
  auto msDescriptor        = ts.descriptor(uMsCompIdx, uMsIdx);
  fuCurrentEquipmentId     = msDescriptor.eq_id;
  const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(uMsCompIdx, uMsIdx));

  uint32_t uSize   = msDescriptor.size;
  fulCurrentMsIdx  = msDescriptor.idx;
  fuCurrentMsSysId = static_cast<uint32_t>(msDescriptor.sys_id);
  fdMsTime         = (1e-9) * static_cast<double>(fulCurrentMsIdx);
  LOG(debug) << "Microslice: " << fulCurrentMsIdx << " from EqId " << std::hex << fuCurrentEquipmentId << std::dec
             << " has size: " << uSize;

  if (-1.0 == fdStartTime) fdStartTime = fdMsTime;

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

    fuGet4Id = fUnpackPar->ElinkIdxToGet4Idx(mess.getGdpbGenChipId());
    /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
    if (0x90 == fuCurrentMsSysId) fuGet4Id = mess.getGdpbGenChipId();
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
      case gdpbv100::MSG_SLOWC:
      case gdpbv100::MSG_SYST:
      case gdpbv100::MSG_STAR_TRI_A:
      case gdpbv100::MSG_STAR_TRI_B:
      case gdpbv100::MSG_STAR_TRI_C:
      case gdpbv100::MSG_STAR_TRI_D: {
        /// Ignore all these
        break;
      }  // case not hit or epoch
      default:
        LOG(fatal) << "Message type " << std::hex << std::setw(2) << static_cast<uint16_t>(messageType)
                   << " not included in Get4 data format.";
    }  // switch( mess.getMessageType() )
  }    // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)

  return kTRUE;
}

// -------------------------------------------------------------------------
void CbmMcbm2018MonitorAlgoTofPulser::ProcessEpochCycle(uint64_t ulCycleData)
{
  ULong64_t ulEpochCycleVal = ulCycleData & gdpbv100::kulEpochCycleFieldSz;

  if (!(ulEpochCycleVal == fvulCurrentEpochCycle[fuCurrDpbIdx]
        || ulEpochCycleVal == fvulCurrentEpochCycle[fuCurrDpbIdx] + 1)
      && 0 < fulCurrentMsIdx) {
    LOG(warning) << "CbmMcbm2018MonitorAlgoTofPulser::ProcessEpochCycle => "
                 << " Missmatch in epoch cycles detected for Gdpb " << fuCurrDpbIdx
                 << ", probably fake cycles due to epoch index corruption! "
                 << Form(" Current cycle 0x%09llX New cycle 0x%09llX", fvulCurrentEpochCycle[fuCurrDpbIdx],
                         ulEpochCycleVal);
  }  // if epoch cycle did not stay constant or increase by exactly 1, except if first MS of the TS
  if (ulEpochCycleVal != fvulCurrentEpochCycle[fuCurrDpbIdx]) {
    LOG(info) << "CbmMcbm2018MonitorAlgoTofPulser::ProcessEpochCycle => "
              << " New epoch cycle for Gdpb " << fuCurrDpbIdx
              << Form(": Current cycle 0x%09llX New cycle 0x%09llX", fvulCurrentEpochCycle[fuCurrDpbIdx],
                      ulEpochCycleVal);
  }  // if( ulEpochCycleVal != fvulCurrentEpochCycle[fuCurrDpbIdx] )
  fvulCurrentEpochCycle[fuCurrDpbIdx] = ulEpochCycleVal;

  return;
}
void CbmMcbm2018MonitorAlgoTofPulser::ProcessEpoch(gdpbv100::Message mess)
{
  ULong64_t ulEpochNr = mess.getGdpbEpEpochNb();
  /*
   Bool_t bSyncFlag   = ( 1 == mess.getGdpbEpSync() );
   Bool_t bDataLoss   = ( 1 == mess.getGdpbEpDataLoss() );
   Bool_t bEpochLoss  = ( 1 == mess.getGdpbEpEpochLoss() );
   Bool_t bMissmMatch = ( 1 == mess.getGdpbEpMissmatch() );
*/
  fvulCurrentEpoch[fuCurrDpbIdx] = ulEpochNr;
  fvulCurrentEpochFull[fuCurrDpbIdx] =
    ulEpochNr + (gdpbv100::kuEpochCounterSz + 1) * fvulCurrentEpochCycle[fuCurrDpbIdx];

  /// Process the corresponding messages buffer for current gDPB
  ProcessEpSupprBuffer();
}
// -------------------------------------------------------------------------
void CbmMcbm2018MonitorAlgoTofPulser::ProcessEpSupprBuffer()
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
    /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
    if (0x90 == fuCurrentMsSysId) fuGet4Id = fvmEpSupprBuffer[iMsgIdx].getGdpbGenChipId();
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
      case gdpbv100::MSG_SYST:
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
void CbmMcbm2018MonitorAlgoTofPulser::ProcessHit(gdpbv100::FullMessage mess)
{
  UInt_t uChannel = mess.getGdpbHitChanId();
  UInt_t uTot     = mess.getGdpbHit32Tot();

  /// In 32b mode the coarse counter is already computed back to 112 FTS bins
  /// => need to hide its contribution from the Finetime
  /// => FTS = Fullt TS modulo 112
  //   UInt_t uFts     = mess.getGdpbHitFullTs() % 112;
  //   UInt_t uCts     = mess.getGdpbHitFullTs() / 112;

  //   UInt_t uChannelNr         = fuGet4Id * fuNrOfChannelsPerGet4 + uChannel;
  UInt_t uChannelNrInFee = (fuGet4Id % fuNrOfGet4PerFee) * fuNrOfChannelsPerGet4 + uChannel;
  UInt_t uFeeNr          = (fuGet4Id / fuNrOfGet4PerFee);
  //   UInt_t uFeeNrInSys        = fuCurrDpbIdx * fuNrOfFeePerGdpb + uFeeNr;
  //   UInt_t uRemappedChannelNr = uFeeNr * fuNrOfChannelsPerFee + fUnpackPar->Get4ChanToPadiChan( uChannelNrInFee );
  UInt_t uRemappedChanNrInFee = fUnpackPar->Get4ChanToPadiChan(uChannelNrInFee);
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

  /// Hardcode TOT limits for the T0
  if (0x90 == fuCurrentMsSysId) {
    if ((0 == fuGet4Id / 2) && (185 < uTot && uTot < 190)) {
      fvvbFeeHitFound[fuCurrDpbIdx][uFeeNr] = kTRUE;
      fvvdFeeHits[fuCurrDpbIdx][uFeeNr]     = dHitTime;
    }  // if( ( 0 == fuGet4Id / 2 ) && ( 185 < uTot && uTot < 190 ) )
  }    // if( 0x90 == fuCurrentMsSysId )
  else if (fuPulserChannel == uRemappedChanNrInFee && fuPulserMinTot < uTot && uTot < fuPulserMaxTot) {
    fvvbFeeHitFound[fuCurrDpbIdx][uFeeNr] = kTRUE;
    fvvdFeeHits[fuCurrDpbIdx][uFeeNr]     = dHitTime;
  }  // if( fuPulserChannel == uRemappedChanNrInFee && fuPulserMinTot < uTot && uTot < fuPulserMaxTot )
}
// -------------------------------------------------------------------------

Bool_t CbmMcbm2018MonitorAlgoTofPulser::CreateHistograms()
{
  std::string sFolder = "mTofMoni";

  LOG(info) << "create Histos for mTof monitoring ";

  /*******************************************************************/
  UInt_t uNbBinsDt = kuNbBinsDt + 1;  // To account for extra bin due to shift by 1/2 bin of both ranges
  dMinDt           = -1. * (kuNbBinsDt * gdpbv100::kdBinSize / 2.) - gdpbv100::kdBinSize / 2.;
  dMaxDt           = 1. * (kuNbBinsDt * gdpbv100::kdBinSize / 2.) + gdpbv100::kdBinSize / 2.;

  std::cout << " Bin size " << gdpbv100::kdBinSize << std::endl;
  std::cout << " Epo bins " << gdpbv100::kuEpochInBins << std::endl;
  std::cout << " Epo size " << gdpbv100::kdEpochInPs << std::endl;
  std::cout << " Epo size " << gdpbv100::kdEpochInNs << std::endl;
  /*******************************************************************/
  /// FIXME: Disable clang formatting to keep readable histo creation
  /* clang-format off */
  /// Internal plot per FEE pair
  fvvhFeePairPulserTimeDiff.resize(fuNrOfFeePerGdpb * fuNrOfGdpbs);
  for (UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; ++uFeeA) {
    UInt_t uGdpbA = uFeeA / (fuNrOfFeePerGdpb);
    if (-1 != fiGdpbIndex) uGdpbA = fiGdpbIndex;
    /// Standard
    UInt_t uFeeIndexA = uFeeA;
    //      UInt_t uFeeIdA  = uFeeA - ( fuNrOfFeePerGdpb * uGdpbA );
    UInt_t uFeeIdA = uFeeIndexA - (3 * 6 * uGdpbA);

    fvvhFeePairPulserTimeDiff[uFeeA].resize(fuNrOfFeePerGdpb * fuNrOfGdpbs);
    for (UInt_t uFeeB = 0; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; ++uFeeB) {

      if (uFeeA < uFeeB) {
        /// Standard
        UInt_t uFeeIndexB = uFeeB;

        UInt_t uGdpbB = uFeeB / (fuNrOfFeePerGdpb);
        if (-1 != fiGdpbIndex) uGdpbB = fiGdpbIndex;
        //            UInt_t uFeeIdB  = uFeeB - ( fuNrOfFeePerGdpb * uGdpbB );
        UInt_t uFeeIdB = uFeeIndexB - (3 * 6 * uGdpbB);
            fvvhFeePairPulserTimeDiff[ uFeeA ][ uFeeB ] = new TH1I(
               Form("hFeePairPulserTimeDiff_s%02u_f%1u_s%02u_f%1u", uGdpbA, uFeeIdA, uGdpbB, uFeeIdB),
               Form("Time difference for pulser on gDPB %02u FEE %1u and gDPB %02u FEE %1u; DeltaT [ps]; Counts",
                     uGdpbA, uFeeIdA, uGdpbB, uFeeIdB ),
                     uNbBinsDt, dMinDt, dMaxDt);

        AddHistoToVector(fvvhFeePairPulserTimeDiff[uFeeA][uFeeB],
                         Form("TofDt/s%03u", uFeeIndexA));
      }  // if( uFeeA < uFeeB )
      else
        fvvhFeePairPulserTimeDiff[uFeeA][uFeeB] = NULL;
    }  // for( UInt_t uFeeB = 0; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; ++uFeeB )
  }  // for( UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; ++uFeeA )

  /// Preparing histo ranges
  UInt_t uTotalNbFee = fuNrOfFeePerGdpb * fuNrOfGdpbs;  /// Standard
  //   Double_t dGdpbMin = -0.5;
  //   Double_t dGdpbMax = fuNrOfGdpbs;
   fhPulserTimeDiffMean = new TH2D( "hPulserTimeDiffMean",
    "Time difference Mean for each FEE pairs; FEE A; FEE B ; Mean [ps]",
         uTotalNbFee - 1, -0.5, uTotalNbFee - 1.5,
         uTotalNbFee - 1,  0.5, uTotalNbFee - 0.5 );

   fhPulserTimeDiffRms = new TH2D( "hPulserTimeDiffRms",
             "Time difference RMS for each FEE pairs; FEE A; FEE B ; RMS [ps]",
         uTotalNbFee - 1, -0.5, uTotalNbFee - 1.5,
         uTotalNbFee - 1,  0.5, uTotalNbFee - 0.5 );

   fhPulserTimeDiffRmsZoom = new TH2D( "hPulserTimeDiffRmsZoom",
         "Time difference RMS for each FEE pairs after zoom on peak; FEE A; FEE B ; RMS [ps]",
         uTotalNbFee - 1, -0.5, uTotalNbFee - 1.5,
         uTotalNbFee - 1,  0.5, uTotalNbFee - 0.5 );


   fhPulserRmsGdpbToRefEvo = new TH2D( "hPulserRmsGdpbToRefEvo",
         "Evo. of Time difference RMS for selected FEE of each gDPb to the 1st; Time in run [s] A; gDPB ; RMS [ps]",
         fuHistoryHistoSize, 0, fuHistoryHistoSize,
         fuNrOfGdpbs - 1,  0.5, fuNrOfGdpbs - 0.5 );

   fhPulserRmsGbtxToRefEvo = new TH2D( "hPulserTimeDiffRmsZoom",
         "Evo. of Time difference RMS for selected FEE pairs of each GBTx to the 1st in same gDPB; Time in run [s] A; FEE ; RMS [ps]",
         fuHistoryHistoSize, 0, fuHistoryHistoSize,
         uTotalNbFee - 1,  0.5, uTotalNbFee - 0.5 );
  /// FIXME: Re-enable clang formatting after histo creation
  /* clang-format on */

  /// Add pointers to the vector with all histo for access by steering class
  AddHistoToVector(fhPulserTimeDiffMean, "Pulser");
  AddHistoToVector(fhPulserTimeDiffRms, "Pulser");
  AddHistoToVector(fhPulserTimeDiffRmsZoom, "Pulser");
  /*******************************************************************/

  /*******************************************************************/
  /// Canvases
  /// Create summary Canvas
  fcSummary = new TCanvas("cSummary", "Pulser Monitoring Summary");
  fcSummary->Divide(3);

  fcSummary->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  fhPulserTimeDiffMean->Draw("colz");

  fcSummary->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  fhPulserTimeDiffRms->Draw("colz");

  fcSummary->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  fhPulserTimeDiffRmsZoom->Draw("colz");

  AddCanvasToVector(fcSummary, "canvases");
  /// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ///
  /*******************************************************************/

  return kTRUE;
}
Bool_t CbmMcbm2018MonitorAlgoTofPulser::FillHistograms()
{
  for (UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeA++) {
    UInt_t uGdpbA  = uFeeA / (fuNrOfFeePerGdpb);
    UInt_t uFeeIdA = uFeeA - (fuNrOfFeePerGdpb * uGdpbA);

    /// If no hits in this FEE in this MS, just go to the next one
    if (kFALSE == fvvbFeeHitFound[uGdpbA][uFeeIdA]) continue;

    for (UInt_t uFeeB = uFeeA + 1; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++) {
      UInt_t uGdpbB  = uFeeB / (fuNrOfFeePerGdpb);
      UInt_t uFeeIdB = uFeeB - (fuNrOfFeePerGdpb * uGdpbB);

      /// If no hits in this FEE in this MS, just go to the next one
      if (kFALSE == fvvbFeeHitFound[uGdpbB][uFeeIdB]) continue;

      Double_t dTimeDiff = 1e3 * (fvvdFeeHits[uGdpbB][uFeeIdB] - fvvdFeeHits[uGdpbA][uFeeIdA]);
      if (TMath::Abs(dTimeDiff) < kdMaxDtPulserPs) {
        fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->Fill(dTimeDiff);
      }  // if( TMath::Abs( dTimeDiff ) < kdMaxDtPulserPs )
    }    // for( UInt_t uFeeB = uFeeA + 1; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++)

    /// Reset the flag for hit found in MS
    fvvbFeeHitFound[uGdpbA][uFeeIdA] = kFALSE;
  }  // for( UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeA++)

  /// Update the Mean and RMS plots only every N TS in last MS
  if (1 == fulCurrentTsIdx % fuUpdateFreqTs && fuNbCoreMsPerTs - 1 == fuMsIndex) {
    /// => Need to be cleared before loop on pairs as we fill directly the value
    LOG(info) << Form("Reseting stats histos for update on TS %5llu MS %3u", fulCurrentTsIdx, fuMsIndex);
    fhPulserTimeDiffMean->Reset();
    fhPulserTimeDiffRms->Reset();
    fhPulserTimeDiffRmsZoom->Reset();

    for (UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeA++) {
      for (UInt_t uFeeB = uFeeA + 1; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++) {
        if (0 == fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetEntries()) continue;

        /// Standard
        UInt_t uFeeIndexA = uFeeA;
        UInt_t uFeeIndexB = uFeeB;
        fhPulserTimeDiffMean->Fill(uFeeIndexA, uFeeIndexB, fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetMean());
        fhPulserTimeDiffRms->Fill(uFeeIndexA, uFeeIndexB, fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetRMS());

        /// Read the peak position (bin with max counts) + total nb of entries
        Int_t iBinWithMax  = fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetMaximumBin();
        Double_t dNbCounts = fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->Integral();

        /// Zoom the X axis to +/- ZoomWidth around the peak position
        Double_t dPeakPos = fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetXaxis()->GetBinCenter(iBinWithMax);
        fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetXaxis()->SetRangeUser(dPeakPos - kdFitZoomWidthPs,
                                                                          dPeakPos + kdFitZoomWidthPs);

        /// Read integral and check how much we lost due to the zoom (% loss allowed)
        Double_t dZoomCounts = fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->Integral();

        /// Fill new RMS after zoom into summary histo
        if ((dZoomCounts / dNbCounts) < 0.8) {
          fhPulserTimeDiffRmsZoom->Fill(uFeeIndexA, uFeeIndexB, 0.0);
          //               LOG(warning) << "CbmMcbm2018MonitorAlgoTofPulser::FillHistograms => Zoom too strong, "
          //                            << "more than 20% loss for FEE pair " << uFeeA << " and " << uFeeB << " !!! ";
        }  // if( ( dZoomCounts / dNbCounts ) < 0.8 )
        else
          fhPulserTimeDiffRmsZoom->Fill(uFeeIndexA, uFeeIndexB, fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetRMS());

        /// Restore original axis state
        fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetXaxis()->UnZoom();
        /*
            LOG(info) << "Stats FEE A " << std::setw(3) << uFeeA
                      << " FEE B " << std::setw(3) << uFeeB
                      << Form( " %5.0f %f", fvvhFeePairPulserTimeDiff[ uFeeA ][ uFeeB ]->GetMean(),
                                            fvvhFeePairPulserTimeDiff[ uFeeA ][ uFeeB ]->GetRMS() );
*/
      }  // for( UInt_t uFeeB = uFeeA + 1; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++)
    }    // for( UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeA++)
  }      // if( 1 == fulCurrentTsIdx % fuUpdateFreqTs && fuNbCoreMsPerTs - 1 == fuMsIndex )

  return kTRUE;
}
Bool_t CbmMcbm2018MonitorAlgoTofPulser::UpdateStats()
{
  LOG(info) << Form("Reseting stats histos for final update on TS %5llu MS %3u", fulCurrentTsIdx, fuMsIndex);
  fhPulserTimeDiffMean->Reset();
  fhPulserTimeDiffRms->Reset();
  fhPulserTimeDiffRmsZoom->Reset();

  for (UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeA++) {
    for (UInt_t uFeeB = uFeeA + 1; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++) {
      if (nullptr == fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]) continue;

      if (0 == fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetEntries()) continue;

      /// Standard
      UInt_t uFeeIndexA = uFeeA;
      UInt_t uFeeIndexB = uFeeB;
      fhPulserTimeDiffMean->Fill(uFeeIndexA, uFeeIndexB, fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetMean());
      fhPulserTimeDiffRms->Fill(uFeeIndexA, uFeeIndexB, fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetRMS());

      /// Read the peak position (bin with max counts) + total nb of entries
      Int_t iBinWithMax  = fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetMaximumBin();
      Double_t dNbCounts = fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->Integral();

      /// Zoom the X axis to +/- ZoomWidth around the peak position
      Double_t dPeakPos = fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetXaxis()->GetBinCenter(iBinWithMax);
      fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetXaxis()->SetRangeUser(dPeakPos - kdFitZoomWidthPs,
                                                                        dPeakPos + kdFitZoomWidthPs);

      /// Read integral and check how much we lost due to the zoom (% loss allowed)
      Double_t dZoomCounts = fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->Integral();

      /// Fill new RMS after zoom into summary histo
      if ((dZoomCounts / dNbCounts) < 0.8) {
        fhPulserTimeDiffRmsZoom->Fill(uFeeIndexA, uFeeIndexB, 0.0);
        LOG(warning) << "CbmMcbm2018MonitorAlgoTofPulser::FillHistograms => "
                        "Zoom too strong, "
                     << "more than 20% loss for FEE pair " << uFeeA << " and " << uFeeB << " !!! ";
        continue;
      }  // if( ( dZoomCounts / dNbCounts ) < 0.8 )
      else
        fhPulserTimeDiffRmsZoom->Fill(uFeeIndexA, uFeeIndexB, fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetRMS());


      /// Restore original axis state?
      fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetXaxis()->UnZoom();

      LOG(info) << "Stats FEE A " << std::setw(3) << uFeeIndexA << " FEE B " << std::setw(3) << uFeeIndexB
                << Form(" %5.0f %f", fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetMean(),
                        fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->GetRMS());

    }  // for( UInt_t uFeeB = uFeeA + 1; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++)
  }    // for( UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeA++)

  return kTRUE;
}
Bool_t CbmMcbm2018MonitorAlgoTofPulser::ResetHistograms()
{
  LOG(info) << Form("Reseting stats histos on TS %5llu MS %3u", fulCurrentTsIdx, fuMsIndex);
  for (UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeA++) {
    for (UInt_t uFeeB = uFeeA + 1; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++) {
      fvvhFeePairPulserTimeDiff[uFeeA][uFeeB]->Reset();
    }  // for( UInt_t uFeeB = uFeeA + 1; uFeeB < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeB++)
  }    // for( UInt_t uFeeA = 0; uFeeA < fuNrOfFeePerGdpb * fuNrOfGdpbs; uFeeA++)
  fhPulserTimeDiffMean->Reset();
  fhPulserTimeDiffRms->Reset();
  fhPulserTimeDiffRmsZoom->Reset();

  fdStartTime = -1.0;

  return kTRUE;
}
// -------------------------------------------------------------------------
