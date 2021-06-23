/* Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin, David Emschermann [committer], Pierre-Alain Loizeau */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmMcbm2018UnpackerAlgoPsd                    -----
// -----                 Created 09.10.2019 by N.Karpushkin                -----
// -----        based on CbmMcbm2018UnpackerAlgoTof by P.-A. Loizeau       -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018UnpackerAlgoPsd.h"

#include "CbmFormatMsHeaderPrintout.h"
#include "CbmMcbm2018PsdPar.h"
#include "CbmPsdAddress.h"

#include <Logger.h>

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
CbmMcbm2018UnpackerAlgoPsd::CbmMcbm2018UnpackerAlgoPsd()
  : CbmStar2019Algo()
  ,
  /// From the class itself
  fbMonitorMode(kFALSE)
  , fbDebugMonitorMode(kFALSE)
  , fvbMaskedComponents()
  , fUnpackPar(nullptr)
  , fuRawDataVersion(0)
  , fuNrOfGdpbs(0)
  , fGdpbIdIndexMap()
  , fuNrOfFeePerGdpb(0)
  , fuNrOfChannelsPerFee(0)
  , fuNrOfChannelsPerGdpb(0)
  , fuNrOfGbtx(0)
  , fuNrOfModules(0)
  , fdTimeOffsetNs(0.0)
  , fulCurrentTsIdx(0)
  , fulCurrentMsIdx(0)
  , fuCurrentMsSysId(0)
  , fdTsStartTime(-1.0)
  , fdTsStopTimeCore(-1.0)
  , fdMsTime(-1.0)
  , fuMsIndex(0)
  , fuCurrentEquipmentId(0)
  , fuCurrDpbId(0)
  , fuCurrDpbIdx(0)
  , fiRunStartDateTimeSec(-1)
  , fiBinSizeDatePlots(-1)
  , fvulCurrentEpoch()
  , fvulCurrentEpochCycle()
  , fvulCurrentEpochFull()
  , fdStartTime(-1.0)
  , fdStartTimeMsSz(0.0)
  , ftStartTimeUnix(std::chrono::steady_clock::now())
{
}
CbmMcbm2018UnpackerAlgoPsd::~CbmMcbm2018UnpackerAlgoPsd()
{
  /// Clear buffers
}

// -------------------------------------------------------------------------
Bool_t CbmMcbm2018UnpackerAlgoPsd::Init()
{
  LOG(info) << "Initializing mCBM PSD 2019 unpacker algo";

  return kTRUE;
}
void CbmMcbm2018UnpackerAlgoPsd::Reset() {}
void CbmMcbm2018UnpackerAlgoPsd::Finish() {}

// -------------------------------------------------------------------------
Bool_t CbmMcbm2018UnpackerAlgoPsd::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmMcbm2018UnpackerAlgoPsd";
  Bool_t initOK = ReInitContainers();

  return initOK;
}
Bool_t CbmMcbm2018UnpackerAlgoPsd::ReInitContainers()
{
  LOG(info) << "**********************************************";
  LOG(info) << "ReInit parameter containers for CbmMcbm2018UnpackerAlgoPsd";

  fUnpackPar = (CbmMcbm2018PsdPar*) fParCList->FindObject("CbmMcbm2018PsdPar");
  if (nullptr == fUnpackPar) return kFALSE;

  Bool_t initOK = InitParameters();

  return initOK;
}
TList* CbmMcbm2018UnpackerAlgoPsd::GetParList()
{
  if (nullptr == fParCList) fParCList = new TList();
  fUnpackPar = new CbmMcbm2018PsdPar("CbmMcbm2018PsdPar");
  fParCList->Add(fUnpackPar);

  return fParCList;
}
Bool_t CbmMcbm2018UnpackerAlgoPsd::InitParameters()
{
  fuRawDataVersion = fUnpackPar->GetDataVersion();
  LOG(info) << "Data Version: " << fuRawDataVersion;

  fuNrOfGdpbs = fUnpackPar->GetNrOfGdpbs();
  LOG(info) << "Nr. of Tof GDPBs: " << fuNrOfGdpbs;

  fuNrOfFeePerGdpb = fUnpackPar->GetNrOfFeesPerGdpb();
  LOG(info) << "Nr. of FEEs per Psd GDPB: " << fuNrOfFeePerGdpb;

  fuNrOfChannelsPerFee = fUnpackPar->GetNrOfChannelsPerFee();
  LOG(info) << "Nr. of channels per FEE: " << fuNrOfChannelsPerFee;

  fuNrOfChannelsPerGdpb = fuNrOfChannelsPerFee * fuNrOfFeePerGdpb;
  LOG(info) << "Nr. of channels per GDPB: " << fuNrOfChannelsPerGdpb;

  fGdpbIdIndexMap.clear();
  for (UInt_t i = 0; i < fuNrOfGdpbs; ++i) {
    fGdpbIdIndexMap[fUnpackPar->GetGdpbId(i)] = i;
    LOG(info) << "GDPB Id of PSD  " << i << " : " << std::hex << fUnpackPar->GetGdpbId(i) << std::dec;
  }  // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )

  fuNrOfGbtx = fUnpackPar->GetNrOfGbtx();
  LOG(info) << "Nr. of GBTx: " << fuNrOfGbtx;

  //Temporary until creation of full psd map
  UInt_t uNrOfModules  = 1;
  UInt_t uNrOfSections = 32;
  UInt_t uNrOfChannels = uNrOfModules * uNrOfSections;
  LOG(info) << "Nr. of possible Psd channels: " << uNrOfChannels;
  fviPsdChUId.resize(uNrOfChannels);

  UInt_t iCh = 0;
  for (UInt_t iModule = 0; iModule < uNrOfModules; ++iModule) {
    for (UInt_t iSection = 0; iSection < uNrOfSections; ++iSection) {
      iCh              = iModule * uNrOfSections + iSection;
      fviPsdChUId[iCh] = CbmPsdAddress::GetAddress(iModule, iSection);
    }
  }

  /// Internal status initialization
  fvulCurrentEpoch.resize(fuNrOfGdpbs, 0);
  fvulCurrentEpochCycle.resize(fuNrOfGdpbs, 0);
  fvulCurrentEpochFull.resize(fuNrOfGdpbs, 0);

  return kTRUE;
}
// -------------------------------------------------------------------------

void CbmMcbm2018UnpackerAlgoPsd::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Add to list
  fvMsComponentsList.push_back(component);

  LOG(info) << "CbmMcbm2018UnpackerAlgoPsd::AddMsComponentToList => Component " << component << " with detector ID 0x"
            << std::hex << usDetectorId << std::dec << " added to list";
}
// -------------------------------------------------------------------------

Bool_t CbmMcbm2018UnpackerAlgoPsd::ProcessTs(const fles::Timeslice& ts)
{

  fulCurrentTsIdx = ts.index();
  fdTsStartTime   = static_cast<Double_t>(ts.descriptor(0, 0).idx);

  /// Ignore First TS as first MS is typically corrupt
  if (0 == fulCurrentTsIdx) { return kTRUE; }  // if( 0 == fulCurrentTsIdx )

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

  fDigiVect.clear();
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


for (auto &elem : fDigiVect)
{
	
          if ( elem.GetSectionID() <10 && elem.fuZL < 100) { printf("NOWW %s %u %f %f\n", elem.ToString().c_str(), elem.fuZL, elem.fdAmpl, elem.ffFitEdep ); /*PsdReader.PrintSaveBuff();*/ }


}


  /// Sort the buffers of hits due to the time offsets applied
  std::sort(fDigiVect.begin(), fDigiVect.end(),
            [](const CbmPsdDigi& a, const CbmPsdDigi& b) -> bool { return a.GetTime() < b.GetTime(); });

  return kTRUE;
}


Bool_t CbmMcbm2018UnpackerAlgoPsd::ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx)
{
  auto msDescriptor        = ts.descriptor(uMsCompIdx, uMsIdx);
  fuCurrentEquipmentId     = msDescriptor.eq_id;
  const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(uMsCompIdx, uMsIdx));

  uint32_t uSize  = msDescriptor.size;
  fulCurrentMsIdx = msDescriptor.idx;

  fdMsTime = (1e-9) * static_cast<double>(fulCurrentMsIdx);

  LOG(debug) << "Microslice: " << fulCurrentMsIdx << " from EqId " << std::hex << fuCurrentEquipmentId << std::dec
             << " has size: " << uSize;

  if (0 == fvbMaskedComponents.size()) fvbMaskedComponents.resize(ts.num_components(), kFALSE);

  fuCurrDpbId = static_cast<uint32_t>(fuCurrentEquipmentId & 0xFFFF);

  /// Check if this sDPB ID was declared in parameter file and stop there if not
  auto it = fGdpbIdIndexMap.find(fuCurrDpbId);
  if (it == fGdpbIdIndexMap.end()) {
    if (kFALSE == fvbMaskedComponents[uMsCompIdx]) {
      LOG(info) << "---------------------------------------------------------------";

      LOG(info) << FormatMsHeaderPrintout(msDescriptor);
      LOG(warning) << "Could not find the gDPB index for AFCK id 0x" << std::hex << fuCurrDpbId << std::dec
                   << " in timeslice " << fulCurrentTsIdx << " in microslice " << uMsIdx << " component " << uMsCompIdx
                   << "\n"
                   << "If valid this index has to be added in the PSD "
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

  fuCurrentMsSysId = static_cast<unsigned int>(msDescriptor.sys_id);

  // If not integer number of message in input buffer, print warning/error
  if (0 != (uSize % kuBytesPerMessage))
    LOG(error) << "The input microslice buffer does NOT "
               << "contain only complete nDPB messages!";

  /// Save start time of first valid MS )
  if (fdStartTime < 0) fdStartTime = fdMsTime;

  // If MS time is less than start time print error and return
  if (fdMsTime - fdStartTime < 0) {
    LOG(error) << "negative time! ";
    return kFALSE;
  }

  // Compute the number of complete messages in the input microslice buffer
  uint32_t uNbMessages = (uSize - (uSize % kuBytesPerMessage)) / kuBytesPerMessage;

  // Prepare variables for the loop on contents
  const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>(msContent);

/*
  if (uSize != 0) {
    printf("%u = %u 64bit messages\n", uSize, uNbMessages);
    for(uint32_t line_iter = 0; line_iter<uNbMessages; line_iter++){
      printf("%016lx\n", (pInBuff[line_iter])); 
    }
  }
*/

  if ((fair::Logger::Logging(fair::Severity::debug)) && (uNbMessages > 1)){
    printf("%u = %u 64bit messages\n", uSize, uNbMessages);
    for(uint32_t line_iter = 0; line_iter<uNbMessages-2; line_iter+=2){
      printf("%010lx", (pInBuff[line_iter]  &0xffffffffff)); 
      printf("%010lx", (pInBuff[line_iter+1]&0xffffffffff)); 
      printf("   %u - %u", line_iter+1, line_iter+2); printf("\n");   
    }
    printf("%020lx   %u\n", pInBuff[uNbMessages-1], uNbMessages);
  }

  // every 80bit gbt word is decomposed into two 64bit words
  if (uNbMessages > 1) {  //more than one 64 bit word

    switch (fuRawDataVersion) {
// --------------------------------------------------------------------------------------------------
      case 100: {

        PsdDataV100::PsdGbtReader PsdReader(pInBuff);
        //if (fair::Logger::Logging(fair::Severity::debug)) PsdReader.SetPrintOutMode(true);

        while (PsdReader.GetTotalGbtWordsRead() < uNbMessages) {
          int ReadResult = PsdReader.ReadMs();
          if (fair::Logger::Logging(fair::Severity::debug)) { printf("\n"); PsdReader.PrintOut(); /*PsdReader.PrintSaveBuff();*/ }
          if (ReadResult == 0) {

	    double prev_hit_time = (double) fulCurrentMsIdx + PsdReader.VectPackHdr.at(0).uAdcTime * 12.5;  //in ns
            //hit loop
            for (uint64_t hit_iter = 0; hit_iter < PsdReader.VectHitHdr.size(); hit_iter++) {
              if (PsdReader.VectPackHdr.size() != PsdReader.VectHitHdr.size()) {
                LOG(error) << "Different vector headers sizes!"
                           << " in VectPackHdr " << PsdReader.VectPackHdr.size() << " in VectHitHdr "
                           << PsdReader.VectHitHdr.size() << "\n";
                break;
              }

              uint8_t uHitChannel    = PsdReader.VectHitHdr.at(hit_iter).uHitChannel;
              //uint8_t uLinkIndex     = PsdReader.VectPackHdr.at(hit_iter).uLinkIndex;
              uint32_t uSignalCharge = PsdReader.VectHitHdr.at(hit_iter).uSignalCharge;
              uint16_t uZeroLevel    = PsdReader.VectHitHdr.at(hit_iter).uZeroLevel;
              double dHitTime = (double) fulCurrentMsIdx + PsdReader.VectPackHdr.at(hit_iter).uAdcTime * 12.5;  //in ns
              //double dHitTime = PsdReader.MsHdr.ulMicroSlice*1000. + PsdReader.VectPackHdr.at(hit_iter).uAdcTime*12.5; //in ns
              std::vector<uint16_t> uWfm = PsdReader.VectHitData.at(hit_iter).uWfm;

              int32_t iHitAmlpitude = 0;
              int32_t iHitChargeWfm = 0;
              uint8_t uHitTimeMax = 0; 
              if (!uWfm.empty()) {
                iHitChargeWfm = std::accumulate(uWfm.begin(), uWfm.end(), 0);
                iHitChargeWfm -= uZeroLevel * uWfm.size();

                auto const max_iter = std::max_element(uWfm.begin(), uWfm.end());
                assert(max_iter != uWfm.end());
                if (max_iter == uWfm.end()) break;

                uHitTimeMax = std::distance(uWfm.begin(), max_iter);
                iHitAmlpitude = *max_iter - uZeroLevel;
              }

              if (uHitChannel >= fviPsdChUId.size()) {
                LOG(error) << "hit channel number out of range! channel index: " << uHitChannel
                           << " max: " << fviPsdChUId.size();
                break;
              }

              UInt_t uChId    = uHitChannel;
              UInt_t uRpdChId = uChId;                  //Should be map(uChId) TODO
              UInt_t uChanUId = fviPsdChUId[uRpdChId];  //unique ID

              double dEdep    = (double) uSignalCharge / fUnpackPar->GetMipCalibration(uHitChannel);  // ->now in MeV
              double dEdepWfm = (double) iHitChargeWfm / fUnpackPar->GetMipCalibration(uHitChannel);  // ->now in MeV
              double dAmpl    = (double) iHitAmlpitude / 16.5; // -> now in mV

              CbmPsdDigi digi;
              digi.fuAddress = uChanUId;
              digi.fdTime = dHitTime;
              digi.fdEdep = dEdep;
              digi.fuZL = uZeroLevel;
              digi.fdAccum = (double) PsdReader.VectHitHdr.at(hit_iter).uFeeAccum;
              digi.fdAdcTime = (double) PsdReader.VectPackHdr.at(hit_iter).uAdcTime;
              digi.fdEdepWfm = dEdepWfm;
              digi.fdAmpl = dAmpl;
              digi.fuTimeMax = uHitTimeMax;
              digi.fdFitEdep = uWfm.back();

              fDigiVect.emplace_back(digi);


              //DEBUG
              if( dHitTime < prev_hit_time ) printf("negative time!\n");
              //DEBUG END
              prev_hit_time = dHitTime;
	      //DEBUG 

            }  // for(int hit_iter = 0; hit_iter < PsdReader.EvHdrAb.uHitsNumber; hit_iter++)
          }
          else if (ReadResult == 1) {
            LOG(error) << "no pack headers in message!";
            break;
          }
          else if (ReadResult == 2) {
            LOG(error) << "wrong channel! In header: " << PsdReader.HitHdr.uHitChannel;
            break;
          }
          else if (ReadResult == 3) {
            LOG(error) << "check number of waveform points! In header: " << PsdReader.HitHdr.uWfmWords - 1;
            break;
          }
          else {
            LOG(error) << "PsdGbtReader.ReadEventFles() didn't return expected values";
            break;
          }


        }  // while(PsdReader.GetTotalGbtWordsRead()<uNbMessages)

        if (uNbMessages != PsdReader.GetTotalGbtWordsRead())
          LOG(error) << "Wrong amount of messages read!"
                     << " in microslice " << uNbMessages << " by PsdReader " << PsdReader.GetTotalGbtWordsRead()
                     << "\n";

        break;
      } // case 100
// --------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------
      case 000: {

        PsdDataV000::PsdGbtReader PsdReader(pInBuff);
        //PsdReader.SetPrintOutMode(true);

        while (PsdReader.GetTotalGbtWordsRead() < uNbMessages) {
          int ReadResult = PsdReader.ReadEventFles();
          if (PsdReader.EvHdrAb.uHitsNumber > fviPsdChUId.size()) {
            LOG(error) << "too many triggered channels! In header: " << PsdReader.EvHdrAb.uHitsNumber
                       << " in PSD: " << fviPsdChUId.size();
            break;
          }

          if (ReadResult == 0) {
            //hit loop
            for (int hit_iter = 0; hit_iter < PsdReader.EvHdrAb.uHitsNumber; hit_iter++) {
              UInt_t uHitChannel         = PsdReader.VectHitHdr.at(hit_iter).uHitChannel;
              UInt_t uSignalCharge       = PsdReader.VectHitHdr.at(hit_iter).uSignalCharge;
              UInt_t uZeroLevel          = PsdReader.VectHitHdr.at(hit_iter).uZeroLevel;
              std::vector<uint16_t> uWfm = PsdReader.VectHitData.at(hit_iter).uWfm;

              if (uHitChannel >= fviPsdChUId.size()) {
                LOG(error) << "hit channel number out of range! channel index: " << uHitChannel
                           << " max: " << fviPsdChUId.size();
                break;
              }

              UInt_t uChId    = uHitChannel;
              UInt_t uRpdChId = uChId;                  //Should be map(uChId) TODO
              UInt_t uChanUId = fviPsdChUId[uRpdChId];  //unique ID

              UInt_t uHitAmlpitude = 0;
              UInt_t uHitChargeWfm = 0;
              for (UInt_t wfm_iter = 0; wfm_iter < uWfm.size(); wfm_iter++) {
                if (uWfm.at(wfm_iter) > uHitAmlpitude) uHitAmlpitude = uWfm.at(wfm_iter);
                uHitChargeWfm += uWfm.at(wfm_iter) - uZeroLevel;
              }
              uHitAmlpitude -= uZeroLevel;

              //printf("0x%08x %u %u %u %f %f\n", uChanUId, uChId, CbmPsdAddress::GetModuleId(uChanUId), CbmPsdAddress::GetSectionId(uChanUId), (double)PsdReader.VectHitHdr.at(hit_iter).uSignalCharge, (double)PsdReader.EvHdrAc.uAdcTime );

              Double_t dAdcTime =
                (double) PsdReader.EvHdrAb.ulMicroSlice + (double) PsdReader.EvHdrAc.uAdcTime * 12.5 - fdTimeOffsetNs;

              LOG(debug) << Form("Insert 0x%08x digi with charge ", uChanUId) << uSignalCharge
                         << Form(", at %u,", PsdReader.EvHdrAc.uAdcTime)
                         << " epoch: " << PsdReader.EvHdrAb.ulMicroSlice;

              CbmPsdDigi digi;
              digi.fuAddress = uChanUId;
              digi.fdTime = dAdcTime;
              digi.fdEdep = (double) uSignalCharge;
              digi.fuAmpl = uHitAmlpitude;
              digi.fdEdepWfm = (double) uHitChargeWfm;
              digi.fuZL = uZeroLevel;

              fDigiVect.emplace_back(digi);

            }  // for(int hit_iter = 0; hit_iter < PsdReader.EvHdrAb.uHitsNumber; hit_iter++)
          }
          else if (ReadResult == 1) {
            LOG(error) << "no event headers in message!";
            break;
          }
          else if (ReadResult == 2) {
            LOG(error) << "check number of waveform points! In header: " << PsdReader.HitHdr.uWfmPoints
                       << " should be: " << 8;
            break;
          }
          else if (ReadResult == 3) {
            LOG(error) << "wrong amount of hits read! In header: " << PsdReader.EvHdrAb.uHitsNumber
                       << " in hit vector: " << PsdReader.VectHitHdr.size();
            break;
          }
          else {
            LOG(error) << "PsdGbtReader.ReadEventFles() didn't return expected values";
            break;
          }

        }  // while(PsdReader.GetTotalGbtWordsRead()<uNbMessages)

        if (uNbMessages != PsdReader.GetTotalGbtWordsRead())
          LOG(error) << "Wrong amount of messages read!"
                     << " in microslice " << uNbMessages << " by PsdReader " << PsdReader.GetTotalGbtWordsRead()
                     << "\n";

        if (fulCurrentMsIdx != PsdReader.EvHdrAb.ulMicroSlice)
          LOG(error) << "Wrong MS index!"
                     << " in microslice " << fulCurrentMsIdx << " by PsdReader " << PsdReader.EvHdrAb.ulMicroSlice
                     << "\n";
        break;
      }//case 000
// --------------------------------------------------------------------------------------------------

    } // switch

  }  //if(uNbMessages > 1)



  return kTRUE;
}


Bool_t CbmMcbm2018UnpackerAlgoPsd::CreateHistograms() { return kTRUE; }
Bool_t CbmMcbm2018UnpackerAlgoPsd::FillHistograms() { return kTRUE; }
Bool_t CbmMcbm2018UnpackerAlgoPsd::ResetHistograms() { return kTRUE; }
// -------------------------------------------------------------------------
