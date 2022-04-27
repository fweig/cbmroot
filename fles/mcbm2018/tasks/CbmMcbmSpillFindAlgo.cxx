/* Copyright (C) 2019-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmMcbmSpillFindAlgo                         -----
// -----               Created 10.02.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbmSpillFindAlgo.h"

#include "CbmFlesHistosTools.h"
#include "CbmFormatMsHeaderPrintout.h"
#include "CbmMcbm2018TofPar.h"
#include "CbmTofAddress.h"
#include "CbmTofDetectorId_v14a.h"  // in cbmdata/tof

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

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
CbmMcbmSpillFindAlgo::CbmMcbmSpillFindAlgo() : CbmStar2019Algo() {}
CbmMcbmSpillFindAlgo::~CbmMcbmSpillFindAlgo() {}

// -------------------------------------------------------------------------
Bool_t CbmMcbmSpillFindAlgo::Init()
{
  LOG(info) << "Initializing mCBM T0 2019 monitor algo";

  return kTRUE;
}
void CbmMcbmSpillFindAlgo::Reset() {}
void CbmMcbmSpillFindAlgo::Finish()
{
  /// If Spill is On, add a fake spill break to have the last spill
  /// If Spill is Off, add a fake spill break end so that all modes include last spill
  if (fbSpillOn) {
    fvuSpillBreakBegTs.push_back(fulCurrentTsIdx + 1);
    fvuSpillBreakEndTs.push_back(fulCurrentTsIdx + 1);
  }  // if (fbSpillOn)
  else
    fvuSpillBreakEndTs.push_back(fulCurrentTsIdx + 1);

  /// Fill the vector of spill break middle points
  std::vector<ULong64_t>::iterator itBreakBeg = fvuSpillBreakBegTs.begin();
  std::vector<ULong64_t>::iterator itBreakEnd = fvuSpillBreakEndTs.begin();

  if (itBreakBeg != fvuSpillBreakBegTs.end() && itBreakEnd != fvuSpillBreakEndTs.end() && *itBreakEnd < *itBreakBeg) {
    fvuSpillBreakMidTs.push_back((*itBreakEnd + fulFirstTsIdx) / 2);
    ++itBreakEnd;
  }  // if( itBreakBeg != fvuSpillBreakBegTs.end() && itBreakEnd != fvuSpillBreakEndTs.end() && *itBreakEnd < *itBreakBeg )

  while (itBreakBeg != fvuSpillBreakBegTs.end() && itBreakEnd != fvuSpillBreakEndTs.end()) {
    fvuSpillBreakMidTs.push_back((*itBreakBeg + *itBreakEnd) / 2);
    ++itBreakBeg;
    ++itBreakEnd;
  }  // while( itBreakBeg != fvuSpillBreakBegTs.end() && itBreakEnd != fvuSpillBreakEndTs.end() )

  if (itBreakBeg != fvuSpillBreakBegTs.end()) {
    fvuSpillBreakMidTs.push_back((*itBreakBeg + fulCurrentTsIdx) / 2);
    ++itBreakBeg;
  }  // if( itBreakBeg != fvuSpillBreakBegTs.end() )

  if (itBreakBeg != fvuSpillBreakBegTs.end() || itBreakEnd != fvuSpillBreakEndTs.end()) {
    LOG(warning) << "Size of spill breaks beginning or end did not match: " << fvuSpillBreakBegTs.size() << " VS "
                 << fvuSpillBreakEndTs.size();
  }  // if( itBreakBeg != fvuSpillBreakBegTs.end() || itBreakEnd != fvuSpillBreakEndTs.end() )

  LOG(info) << "**********************************************";
  LOG(info) << "TS index for beginning of spill breaks:";
  for (ULong64_t uBeg : fvuSpillBreakBegTs) {
    LOG(info) << Form("%9llu", uBeg);
  }  // for (ULong64_t uBeg : fvuSpillBreakBegTs)
  LOG(info) << "**********************************************";
  LOG(info) << "TS index for ending of spill breaks:";
  for (ULong64_t uEnd : fvuSpillBreakEndTs) {
    LOG(info) << Form("%9llu", uEnd);
  }  // for (ULong64_t uBeg : fvuSpillBreakBegTs)
  LOG(info) << "**********************************************";
  LOG(info) << "TS index for middle of spill breaks:";
  for (ULong64_t uMid : fvuSpillBreakMidTs) {
    LOG(info) << Form("%9llu", uMid);
  }  // for (ULong64_t uBeg : fvuSpillBreakBegTs)
  LOG(info) << "**********************************************";
}

// -------------------------------------------------------------------------
Bool_t CbmMcbmSpillFindAlgo::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmMcbmSpillFindAlgo";
  Bool_t initOK = ReInitContainers();

  return initOK;
}
Bool_t CbmMcbmSpillFindAlgo::ReInitContainers()
{
  LOG(info) << "**********************************************";
  LOG(info) << "ReInit parameter containers for CbmMcbmSpillFindAlgo";

  fUnpackPar = (CbmMcbm2018TofPar*) fParCList->FindObject("CbmMcbm2018TofPar");
  if (nullptr == fUnpackPar) return kFALSE;

  Bool_t initOK = InitParameters();

  return initOK;
}
TList* CbmMcbmSpillFindAlgo::GetParList()
{
  if (nullptr == fParCList) fParCList = new TList();
  fUnpackPar = new CbmMcbm2018TofPar("CbmMcbm2018TofPar");
  fParCList->Add(fUnpackPar);

  return fParCList;
}
Bool_t CbmMcbmSpillFindAlgo::InitParameters()
{

  fuNrOfGdpbs = fUnpackPar->GetNrOfGdpbs();
  LOG(info) << "Nr. of Tof GDPBs: " << fuNrOfGdpbs;

  fuNrOfFeePerGdpb = fUnpackPar->GetNrOfFeesPerGdpb();
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

  return kTRUE;
}
// -------------------------------------------------------------------------

void CbmMcbmSpillFindAlgo::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Add to list
  fvMsComponentsList.push_back(component);

  LOG(info) << "CbmMcbmSpillFindAlgo::AddMsComponentToList => Component " << component << " with detector ID 0x"
            << std::hex << usDetectorId << std::dec << " added to list";
}
// -------------------------------------------------------------------------

Bool_t CbmMcbmSpillFindAlgo::ProcessTs(const fles::Timeslice& ts)
{
  fulCurrentTsIdx = ts.index();
  fdTsStartTime   = static_cast<Double_t>(ts.descriptor(0, 0).idx);
  if (fulCurrentTsIdx < fulFirstTsIdx) fulFirstTsIdx = fulCurrentTsIdx;

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
  }      // for( fuMsIndex = 0; fuMsIndex < uNbMsLoop; fuMsIndex ++ )

  /// Fill plots if in monitor mode
  if (fbMonitorMode) {
    if (kFALSE == FillHistograms()) {
      LOG(error) << "Failed to fill histos in ts " << fulCurrentTsIdx;
      return kFALSE;
    }  // if( kFALSE == FillHistograms() )
  }    // if( fbMonitorMode )

  return kTRUE;
}

Bool_t CbmMcbmSpillFindAlgo::ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx)
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

  /*
 * Should be only for first detected TS
 */
  if (fulCurrentTsIdx < 10 && 0 == uMsIdx) {
    LOG(info) << "---------------------------------------------------------------";
    LOG(info) << "Component " << uMsCompIdx << " TS Idx " << fulCurrentTsIdx;
    LOG(info) << "hi hv eqid flag si sv idx/start        crc      size     offset";
    LOG(info) << Form("%02x %02x %04x %04x %02x %02x %016lx %08x %08x %016lx",
                      static_cast<unsigned int>(msDescriptor.hdr_id), static_cast<unsigned int>(msDescriptor.hdr_ver),
                      msDescriptor.eq_id, msDescriptor.flags, static_cast<unsigned int>(msDescriptor.sys_id),
                      static_cast<unsigned int>(msDescriptor.sys_ver), static_cast<unsigned long>(msDescriptor.idx),
                      msDescriptor.crc, msDescriptor.size, static_cast<unsigned long>(msDescriptor.offset));
  }  // if( fulCurrentTsIdx < 10 && 0 == uMsIdx )
     /*
*/

  /// Check if this sDPB ID was declared in parameter file and stop there if not
  auto it = fGdpbIdIndexMap.find(fuCurrDpbId);
  if (it == fGdpbIdIndexMap.end()) {
    if (kFALSE == fvbMaskedComponents[uMsCompIdx]) {
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

  /// Spill Detection
  if (0 == fuCurrDpbIdx) {
    /// Check only every user defined interval (0.5s per default)
    if (fdSpillCheckInterval < fdMsTime - fdLastSecondTime) {
      /// Spill Off detection
      if (fbSpillOn && fuCountsLastInterval < fuOffSpillCountLimit) {
        fbSpillOn = kFALSE;
        fuCurrentSpillIdx++;
        fdStartTimeSpill = fdMsTime;
        if (0 < fvuSpillBreakBegTs.size()) {
          fhSpillDuration->Fill(fulCurrentTsIdx - fvuSpillBreakBegTs.back());
        }  // if( 0 < fvuSpillBreakBegTs.size() )
        fvuSpillBreakBegTs.push_back(fulCurrentTsIdx);
      }  // if( fbSpillOn && fuCountsLastInterval < fuOffSpillCountLimit )
      else if (!fbSpillOn && fuOffSpillCountLimit < fuCountsLastInterval) {
        fbSpillOn = kTRUE;
        if (0 < fvuSpillBreakBegTs.size()) {
          fhSpillBreakDuration->Fill(fuCurrentSpillIdx, fulCurrentTsIdx - fvuSpillBreakBegTs.back());
        }  // if( 0 < fvuSpillBreakBegTs.size() )
        if (0 < fvuSpillBreakEndTs.size()) {
          fhHitsPerSpill->Fill(fuCurrentSpillIdx, fuCountsLastSpill);
          fhSpillDuration->Fill(fuCurrentSpillIdx, fulCurrentTsIdx - fvuSpillBreakEndTs.back());
        }  // if( 0 < fvuSpillBreakEndTs.size() )
        fvuSpillBreakEndTs.push_back(fulCurrentTsIdx);
        fuCountsLastSpill = 0;
      }  // else if (fuOffSpillCountLimit < fuCountsLastInterval)

      fuCountsLastInterval = 0;
      fdLastSecondTime     = fdMsTime;
    }  // if( fdSpillCheckInterval < fdMsTime - fdLastSecondTime )
  }    // if( 0 == fuCurrDpbIdx )

  /// Save start time of first valid MS )
  if (fdStartTime < 0) fdStartTime = fdMsTime;
  /// Reset the histograms if reached the end of the evolution histos range
  else if (fuHistoryHistoSize < fdMsTime - fdStartTime) {
    ResetHistograms();
    fdStartTime = fdMsTime;
  }  // else if( fuHistoryHistoSize < fdMsTime - fdStartTime )

  // If not integer number of message in input buffer, print warning/error
  if (0 != (uSize % kuBytesPerMessage))
    LOG(error) << "The input microslice buffer does NOT "
               << "contain only complete nDPB messages!";

  // Compute the number of complete messages in the input microslice buffer
  uint32_t uNbMessages = (uSize - (uSize % kuBytesPerMessage)) / kuBytesPerMessage;

  // Prepare variables for the loop on contents
  Int_t messageType       = -111;
  ULong64_t ulNbHitsTs    = 0;
  const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>(msContent);
  for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++) {
    // Fill message
    uint64_t ulData = static_cast<uint64_t>(pInBuff[uIdx]);

    /// Catch the Epoch cycle block which is always the first 64b of the MS
    if (0 == uIdx) { continue; }  // if( 0 == uIdx )

    gdpbv100::Message mess(ulData);
    /// Get message type
    messageType = mess.getMessageType();

    fuGet4Id = mess.getGdpbGenChipId();
    fuGet4Nr = fuGet4Id / 2;
    // UInt_t uChannelT0 = ( fuGet4Id < 32 ) ? ( fuGet4Id / 8 ) : (fuGet4Id / 8 - 1); /// December 2018 mapping
    // UInt_t uChannelT0 = fuGet4Id / 2 + 4 * fuCurrDpbIdx;  /// 2019 mapping with 320/640 Mb/s FW

    if (fuNrOfGet4PerGdpb <= fuGet4Id && !mess.isStarTrigger() && (gdpbv100::kuChipIdMergedEpoch != fuGet4Id))
      LOG(warning) << "Message with Get4 ID too high: " << fuGet4Id << " VS " << fuNrOfGet4PerGdpb
                   << " set in parameters.";

    switch (messageType) {
      case gdpbv100::MSG_HIT: {
        if (mess.getGdpbHitIs24b()) {
          LOG(error) << "This event builder does not support 24b hit message!!!.";
          continue;
        }  // if( getGdpbHitIs24b() )
        else {
          /// Spill detection

          /// Do not fill the pulser hits to keep counts low for channel 0
          /// => Check channel != first GET4 per board + TOT
          UInt_t uTot = mess.getGdpbHit32Tot();
          if (0 != fuGet4Nr || uTot < fuMinTotPulser || fuMaxTotPulser < uTot) {
            fuCountsLastInterval++;
            fuCountsLastSpill++;
            ulNbHitsTs++;
          }  // if (0 != fuGet4Nr || uTot < fuMinTotPulser || fuMaxTotPulser < uTot) {
        }    // else of if( getGdpbHitIs24b() )
        break;
      }  // case gdpbv100::MSG_HIT:
      case gdpbv100::MSG_EPOCH: {
        break;
      }  // case gdpbv100::MSG_EPOCH:
      case gdpbv100::MSG_SLOWC: {
        break;
      }  // case gdpbv100::MSG_SLOWC:
      case gdpbv100::MSG_SYST: {
        break;
      }  // case gdpbv100::MSG_SYST:
      case gdpbv100::MSG_STAR_TRI_A:
      case gdpbv100::MSG_STAR_TRI_B:
      case gdpbv100::MSG_STAR_TRI_C:
      case gdpbv100::MSG_STAR_TRI_D: {
        break;
      }  // case gdpbv100::MSG_STAR_TRI_A-D
      default:
        LOG(error) << "Message type " << std::hex << std::setw(2) << static_cast<uint16_t>(messageType)
                   << " not included in Get4 unpacker.";
    }  // switch( mess.getMessageType() )
  }    // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
  fhHitsEvo->Fill(fdTsStartTime * 1e-9, ulNbHitsTs);

  /// Fill histograms
  FillHistograms();

  return kTRUE;
}
// -------------------------------------------------------------------------
Bool_t CbmMcbmSpillFindAlgo::CreateHistograms()
{
  std::string sFolder = "SpillFinder";

  LOG(info) << "create Histos for T0 monitoring ";

  /// Logarithmic bining
  uint32_t iNbBinsLog = 0;
  /// Parameters are NbDecadesLog, NbStepsDecade, NbSubStepsInStep
  std::vector<double> dBinsLogVector = GenerateLogBinArray(4, 9, 1, iNbBinsLog);
  //double* dBinsLog                   = dBinsLogVector.data();
  //   double * dBinsLog = GenerateLogBinArray( 4, 9, 1, iNbBinsLog );

  /*******************************************************************/
  fhHitsEvo =
    new TH1I("hHitsEvo", "Hit count evo; Time [s]; Hits Count []", fuHistoryHistoSize * 50, 0., fuHistoryHistoSize);
  fhHitsPerSpill       = new TH1I("hHitsPerSpill", "Hit count per spill; Spill; Hits Count []", 2000, 0., 2000);
  fhSpillBreakDuration = new TH1I("hSpillBreakDuration", "Spill break duration; Spill; Duration [TS]", 2000, 0., 2000);
  fhSpillDuration      = new TH1I("hSpillDuration", "Spill duration; Spill; Duration [TS]", 2000, 0., 2000);

  /// Add pointers to the vector with all histo for access by steering class
  AddHistoToVector(fhHitsEvo, sFolder);
  AddHistoToVector(fhHitsPerSpill, sFolder);
  AddHistoToVector(fhSpillBreakDuration, sFolder);
  AddHistoToVector(fhSpillDuration, sFolder);

  /*******************************************************************/
  /// Cleanup array of log bins
  //   delete dBinsLog;

  /*******************************************************************/

  /// Canvases
  //  Double_t w = 10;
  //  Double_t h = 10;

  /*******************************************************************/
  /*
  /// Map of hits over T0 detector and same vs time in run
  fcHitMaps = new TCanvas("cHitMaps", "Hit maps", w, h);
  fcHitMaps->Divide(2);

  fcHitMaps->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhChannelMap->Draw();

  fcHitMaps->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHitMapEvo->Draw("colz");

  AddCanvasToVector(fcHitMaps, "canvases");
*/
  /*******************************************************************/

  /*******************************************************************/

  return kTRUE;
}
Bool_t CbmMcbmSpillFindAlgo::FillHistograms() { return kTRUE; }
Bool_t CbmMcbmSpillFindAlgo::ResetHistograms(Bool_t bResetTime)
{
  fuCurrentSpillIdx = 0;
  fhHitsEvo->Reset();
  fhHitsPerSpill->Reset();
  fhSpillBreakDuration->Reset();
  fhSpillDuration->Reset();

  if (kTRUE == bResetTime) {
    /// Also reset the Start time for the evolution plots!
    fdStartTime = -1.0;
  }  // if( kTRUE == bResetTime )

  return kTRUE;
}
// -------------------------------------------------------------------------
