/* Copyright (C) 2019-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: David Emschermann [committer], Nikolay Karpushkin, Nikolay Karpushkin */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                  CbmMcbm2018MonitorAlgoPsd                        -----
// -----              Created 26.09.2019 by N.Karpushkin                   -----
// -----      based on CbmMcbm2018MonitorAlgoT0 by P.-A. Loizeau           -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018MonitorAlgoPsd.h"

#include "CbmFlesHistosTools.h"
#include "CbmFormatMsHeaderPrintout.h"
#include "CbmMcbm2018PsdPar.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

#include "TCanvas.h"
#include "TGraph.h"
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
CbmMcbm2018MonitorAlgoPsd::CbmMcbm2018MonitorAlgoPsd()
  : CbmStar2019Algo()
{
}

CbmMcbm2018MonitorAlgoPsd::~CbmMcbm2018MonitorAlgoPsd()
{
  /// Clear buffers
}

// -------------------------------------------------------------------------
Bool_t CbmMcbm2018MonitorAlgoPsd::Init()
{
  LOG(info) << "Initializing mCBM Psd 2019 monitor algo";

  return kTRUE;
}
void CbmMcbm2018MonitorAlgoPsd::Reset() {}
void CbmMcbm2018MonitorAlgoPsd::Finish()
{
  /// Printout Goodbye message and stats

  /// Write Output histos
}

// -------------------------------------------------------------------------
Bool_t CbmMcbm2018MonitorAlgoPsd::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmMcbm2018MonitorAlgoPsd";
  Bool_t initOK = ReInitContainers();

  return initOK;
}
Bool_t CbmMcbm2018MonitorAlgoPsd::ReInitContainers()
{
  LOG(info) << "**********************************************";
  LOG(info) << "ReInit parameter containers for CbmMcbm2018MonitorAlgoPsd";

  fUnpackPar = (CbmMcbm2018PsdPar*) fParCList->FindObject("CbmMcbm2018PsdPar");
  if (nullptr == fUnpackPar) return kFALSE;

  Bool_t initOK = InitParameters();

  return initOK;
}
TList* CbmMcbm2018MonitorAlgoPsd::GetParList()
{
  if (nullptr == fParCList) fParCList = new TList();
  fUnpackPar = new CbmMcbm2018PsdPar("CbmMcbm2018PsdPar");
  fParCList->Add(fUnpackPar);

  return fParCList;
}
Bool_t CbmMcbm2018MonitorAlgoPsd::InitParameters()
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


  return kTRUE;
}
// -------------------------------------------------------------------------

void CbmMcbm2018MonitorAlgoPsd::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Add to list
  fvMsComponentsList.push_back(component);

  LOG(info) << "CbmMcbm2018MonitorAlgoPsd::AddMsComponentToList => Component " << component << " with detector ID 0x"
            << std::hex << usDetectorId << std::dec << " added to list";
}
// -------------------------------------------------------------------------

Bool_t CbmMcbm2018MonitorAlgoPsd::ProcessTs(const fles::Timeslice& ts)
{
  fulCurrentTsIdx = ts.index();
  fdTsStartTime   = static_cast<Double_t>(ts.descriptor(0, 0).idx);

  /// Ignore First TS as first MS is typically corrupt
  if (0 == fulCurrentTsIdx) {
    LOG(info) << "Reseting Histos for a new run";
    ResetHistograms();
    return kTRUE;
  }

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

    /// Clear the buffer of hits

  }  // for( fuMsIndex = 0; fuMsIndex < uNbMsLoop; fuMsIndex ++ )

  /// Clear buffers to prepare for the next TS


  /// Fill plots if in monitor mode
  if (fbMonitorMode) {
    if (kFALSE == FillHistograms()) {
      LOG(error) << "Failed to fill histos in ts " << fulCurrentTsIdx;
      return kFALSE;
    }  // if( kFALSE == FillHistograms() )
  }    // if( fbMonitorMode )

  return kTRUE;
}

Bool_t CbmMcbm2018MonitorAlgoPsd::ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx)
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

  /// Spill Detection
  if (0 == fuCurrDpbIdx) {
    /// Check only every second
    if (1.0 < fdMsTime - fdLastSecondTime) {
      /// Spill Off detection
      if (fbSpillOn && fuCountsLastSecond < kuOffSpillCountLimit) {
        fbSpillOn = kFALSE;
        fuCurrentSpillIdx++;
        fdStartTimeSpill = fdMsTime;
      }  // if( fbSpillOn && fuCountsLastSecond < kuOffSpillCountLimit )
      else if (kuOffSpillCountLimit < fuCountsLastSecond)
        fbSpillOn = kTRUE;

      fuCountsLastSecond = 0;
      fdLastSecondTime   = fdMsTime;
    }  // if( 1 < fdMsTime - fdLastSecondTime )
  }    // if( 0 == fuCurrDpbIdx )

  /// Save start time of first valid MS )
  if (fdStartTime < 0) fdStartTime = fdMsTime;

  /// Reset the histograms if reached the end of the evolution histos range
  else if (fuHistoryHistoSize < fdMsTime - fdStartTime) {
    ResetHistograms();
    fdStartTime = fdMsTime;
  }  // else if( fuHistoryHistoSize < fdMsTime - fdStartTime )

  // If MS time is less than start time print error and return
  if (fdMsTime - fdStartTime < 0) {
    LOG(error) << "negative time! ";
    ResetHistograms();
    return kFALSE;
  }

  // If not integer number of message in input buffer, print warning/error
  if (0 != (uSize % kuBytesPerMessage))
    LOG(error) << "The input microslice buffer does NOT "
               << "contain only complete nDPB messages!";

  // Compute the number of complete messages in the input microslice buffer
  uint32_t uNbMessages = (uSize - (uSize % kuBytesPerMessage)) / kuBytesPerMessage;

  // Prepare variables for the loop on contents
  const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>(msContent);

  if (fair::Logger::Logging(fair::Severity::debug)) {
    if (uNbMessages != 0) printf("\n\n");
    for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++) {
      // Fill message
      uint64_t ulData = static_cast<uint64_t>(pInBuff[uIdx]);
      printf("%016llx\n", (long long int) ulData);
    }
  }

  switch (fuRawDataVersion) {
      // --------------------------------------------------------------------------------------------------
    case 100: {

      PsdDataV100::PsdGbtReader PsdReader(pInBuff);
      if (fair::Logger::Logging(fair::Severity::debug)) PsdReader.SetPrintOutMode(true);
      // every 80bit gbt word is decomposed into two 64bit words
      if (uNbMessages > 1) {
        while (PsdReader.GetTotalGbtWordsRead() < uNbMessages) {
          int ReadResult = PsdReader.ReadMs();
          if (ReadResult == 0) {
            fuCountsLastSecond++;
            fuReadEvtCntInMs++;

            //hit loop
            for (uint64_t hit_iter = 0; hit_iter < PsdReader.VectHitHdr.size(); hit_iter++) {
              if (PsdReader.VectPackHdr.size() != PsdReader.VectHitHdr.size()) {
                LOG(error) << "Different vector headers sizes!"
                           << " in VectPackHdr " << PsdReader.VectPackHdr.size() << " in VectHitHdr "
                           << PsdReader.VectHitHdr.size() << "\n";
                break;
              }

              uint16_t uHitChannel = PsdReader.VectHitHdr.at(hit_iter).uHitChannel;
              //uint8_t uLinkIndex     = PsdReader.VectPackHdr.at(hit_iter).uLinkIndex;
              uint32_t uSignalCharge = PsdReader.VectHitHdr.at(hit_iter).uSignalCharge;
              uint16_t uZeroLevel    = PsdReader.VectHitHdr.at(hit_iter).uZeroLevel;
              uint32_t uAccum        = PsdReader.VectHitHdr.at(hit_iter).uFeeAccum;
              //double dHitTime = (double) fulCurrentMsIdx + PsdReader.VectPackHdr.at(hit_iter).uAdcTime * 12.5;  //in ns
              //double dHitTime = PsdReader.MsHdr.ulMicroSlice*1000. + PsdReader.VectPackHdr.at(hit_iter).uAdcTime*12.5; //in ns
              std::vector<uint16_t> uWfm = PsdReader.VectHitData.at(hit_iter).uWfm;
              uSignalCharge /= (int) kfAdc_to_mV;  // ->now in mV

              fhAdcTime->Fill(PsdReader.VectPackHdr.at(hit_iter).uAdcTime);

              //uHitChannel numerated from 0
              if (uHitChannel >= kuNbChanPsd) {
                LOG(error) << "hit channel number out of range! channel index: " << uHitChannel
                           << " max: " << GetNbChanPsd();
                break;
              }

              //Pack header
              fhHitChargeMap->Fill(uHitChannel, uSignalCharge);
              fhChanHitMapEvo->Fill(uHitChannel, fdMsTime - fdStartTime);  //should be placed map(channel)
              if (fbMonitorChanMode) {
                fvhHitChargeChan[uHitChannel]->Fill(uSignalCharge);
                fvhHitZeroLevelChan[uHitChannel]->Fill(uZeroLevel);
                fvhHitZLChanEvo[uHitChannel]->Fill(fdMsTime - fdStartTime, uZeroLevel);
                fvhHitFAChanEvo[uHitChannel]->Fill(fdMsTime - fdStartTime, uAccum);

                //Hit data
                double dHitAmlpitude = 0;
                double dHitChargeWfm = 0;
                if (fbMonitorWfmMode) fvhHitWfmChan[uHitChannel]->Reset();
                if (fbMonitorFitMode) fvhHitFitWfmChan[uHitChannel]->Reset();

                if (!uWfm.empty()) {
                  dHitChargeWfm = std::accumulate(uWfm.begin(), uWfm.end(), 0);
                  dHitChargeWfm -= uZeroLevel * uWfm.size();
                  auto const max_iter = std::max_element(uWfm.begin(), uWfm.end());
                  assert(max_iter != uWfm.end());
                  if (max_iter == uWfm.end()) break;
                  //uint8_t hit_time_max = std::distance(uWfm.begin(), max_iter);
                  dHitAmlpitude = *max_iter - uZeroLevel;
                  dHitAmlpitude /= kfAdc_to_mV;
                  dHitChargeWfm /= kfAdc_to_mV;
                  fvhHitAmplChan[uHitChannel]->Fill(dHitAmlpitude);
                  fvhHitChargeByWfmChan[uHitChannel]->Fill(dHitChargeWfm);

                  if (fbMonitorWfmMode) {
                    fvhHitLPChanEvo[uHitChannel]->Fill(fdMsTime - fdStartTime, uWfm.back());
                    for (UInt_t wfm_iter = 0; wfm_iter < uWfm.size(); wfm_iter++)
                      fvhHitWfmChan[uHitChannel]->Fill(wfm_iter, uWfm.at(wfm_iter));
                    fvhHitWfmChan[uHitChannel]->SetTitle(
                      Form("Waveform channel %03u charge %0u zero level %0u; Time [adc "
                           "counts]; Amplitude [adc counts]",
                           uHitChannel, uSignalCharge, uZeroLevel));
                    for (uint8_t i = 0; i < kuNbWfmRanges; ++i) {
                      if (uSignalCharge > kvuWfmRanges.at(i) && uSignalCharge < kvuWfmRanges.at(i + 1)) {
                        UInt_t uFlatIndexOfChange = i * kuNbChanPsd + uHitChannel;

                        UInt_t uWfmExampleIter = kvuWfmInRangeToChangeChan.at(uFlatIndexOfChange);
                        UInt_t uFlatIndexHisto =
                          uWfmExampleIter * kuNbWfmRanges * kuNbChanPsd + i * kuNbChanPsd + uHitChannel;
                        fv3hHitWfmFlattenedChan[uFlatIndexHisto]->Reset();

                        for (UInt_t wfm_iter = 0; wfm_iter < uWfm.size(); wfm_iter++)
                          fv3hHitWfmFlattenedChan[uFlatIndexHisto]->Fill(wfm_iter, uWfm.at(wfm_iter));
                        fv3hHitWfmFlattenedChan[uFlatIndexHisto]->SetTitle(
                          Form("Waveform channel %03u charge %0u zero level %0u; Time "
                               "[adc counts]; Amplitude [adc counts]",
                               uHitChannel, uSignalCharge, uZeroLevel));

                        kvuWfmInRangeToChangeChan.at(uFlatIndexOfChange)++;
                        if (kvuWfmInRangeToChangeChan.at(uFlatIndexOfChange) == kuNbWfmExamples)
                          kvuWfmInRangeToChangeChan.at(uFlatIndexOfChange) = 0;

                      }  // if( uSignalCharge > kvuWfmRanges.at(i) && uSignalCharge < kvuWfmRanges.at(i+1) )
                    }    // for (uint8_t i=0; i<kuNbWfmRanges; ++i)
                  }      //if (fbMonitorWfmMode)
                }        //if (!uWfm.empty())


                if (fbMonitorFitMode && !uWfm.empty()) {
                  int gate_beg = 0;
                  int gate_end = 10;  //uWfm.size() - 1;
                  PsdSignalFitting::PronyFitter Pfitter(2, 2, gate_beg, gate_end);

                  Pfitter.SetDebugMode(0);
                  Pfitter.SetWaveform(uWfm, uZeroLevel);
                  int SignalBeg                           = 4;
                  std::complex<float> first_fit_harmonic  = {0.72, 0.0};
                  std::complex<float> second_fit_harmonic = {0.38, -0.0};
                  Pfitter.SetExternalHarmonics(first_fit_harmonic, second_fit_harmonic);
                  Int_t best_signal_begin = Pfitter.ChooseBestSignalBegin(SignalBeg - 1, SignalBeg + 1);
                  Pfitter.SetSignalBegin(best_signal_begin);
                  Pfitter.CalculateFitAmplitudes();

                  Pfitter.SetSignalBegin(best_signal_begin);
                  Pfitter.CalculateFitHarmonics();
                  Pfitter.CalculateFitAmplitudes();

                  Float_t fit_integral = Pfitter.GetIntegral(gate_beg, uWfm.size() - 1) / kfAdc_to_mV;
                  Float_t fit_R2       = Pfitter.GetRSquare(gate_beg, uWfm.size() - 1);

                  std::complex<float>* harmonics = Pfitter.GetHarmonics();
                  std::vector<uint16_t> uFitWfm  = Pfitter.GetFitWfm();
                  for (UInt_t wfm_iter = 0; wfm_iter < uFitWfm.size(); wfm_iter++)
                    fvhHitFitWfmChan[uHitChannel]->Fill(wfm_iter, uFitWfm.at(wfm_iter));
                  fvhHitWfmChan[uHitChannel]->SetTitle(Form("Waveform channel %03u charge %0u zero level %0u R2 %.5f; "
                                                            "Time [adc counts]; Amplitude [adc counts]",
                                                            uHitChannel, uSignalCharge, uZeroLevel, fit_R2));

                  fvhFitQaChan[uHitChannel]->Fill(fit_integral, fit_R2);

                  if (fit_R2 > 0.02) continue;
                  fvhFitHarmonic1Chan[uHitChannel]->Fill(std::real(harmonics[1]), std::imag(harmonics[1]));
                  fvhFitHarmonic2Chan[uHitChannel]->Fill(std::real(harmonics[2]), std::imag(harmonics[2]));
                }  //if (fbMonitorFitMode && !uWfm.empty())
              }    //if (fbMonitorChanMode)
            }      // for(int hit_iter = 0; hit_iter < PsdReader.EvHdrAb.uHitsNumber; hit_iter++)
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


        if (uNbMessages != PsdReader.GetTotalGbtWordsRead()) {
          fbPsdMissedData = kTRUE;
          LOG(error) << "Wrong amount of messages read!"
                     << " in microslice " << uNbMessages << " by PsdReader " << PsdReader.GetTotalGbtWordsRead();

          if (fbFirstPackageError) {
            std::ofstream error_log(Form("%llu_errorlog.txt", fulCurrentMsIdx), std::ofstream::out);
            for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++) {
              uint64_t ulData = static_cast<uint64_t>(pInBuff[uIdx]);
              error_log << Form("%016llx\n", (long long int) ulData);
            }
            error_log.close();
            fbFirstPackageError = kFALSE;
            printf("Written file %llu_errorlog.txt\n", fulCurrentMsIdx);
          }
        }

        fuMsgsCntInMs += uNbMessages;
        fuReadMsgsCntInMs += PsdReader.GetTotalGbtWordsRead();
        fuLostMsgsCntInMs += uNbMessages - PsdReader.GetTotalGbtWordsRead();
      }  //if (uNbMessages > 1)

      if (fdPrevMsTime < 0.) fdPrevMsTime = fdMsTime;
      else {
        fhMsLengthEvo->Fill(fdMsTime - fdStartTime, 1e9 * (fdMsTime - fdPrevMsTime));
        fdPrevMsTime = fdMsTime;
      }

      /// Fill histograms
      FillHistograms();

      break;

    }  // case 1
       // --------------------------------------------------------------------------------------------------
       // --------------------------------------------------------------------------------------------------
    case 000: {

      PsdDataV000::PsdGbtReader PsdReader(pInBuff);
      if (fair::Logger::Logging(fair::Severity::debug)) PsdReader.SetPrintOutMode(true);
      if (uNbMessages > 1) {
        while (PsdReader.GetTotalGbtWordsRead() < uNbMessages) {
          int ReadResult = PsdReader.ReadEventFles();
          if (PsdReader.EvHdrAb.uHitsNumber > kuNbChanPsd) {
            LOG(error) << "too many triggered channels! In header: " << PsdReader.EvHdrAb.uHitsNumber
                       << " in PSD: " << GetNbChanPsd();
            break;
          }

          if (ReadResult == 0) {
            fuCountsLastSecond++;
            fhAdcTime->Fill(PsdReader.EvHdrAc.uAdcTime);
            fuReadEvtCntInMs++;

            //hit loop
            for (int hit_iter = 0; hit_iter < PsdReader.EvHdrAb.uHitsNumber; hit_iter++) {
              UInt_t uHitChannel         = PsdReader.VectHitHdr.at(hit_iter).uHitChannel;
              UInt_t uSignalCharge       = PsdReader.VectHitHdr.at(hit_iter).uSignalCharge;
              UInt_t uZeroLevel          = PsdReader.VectHitHdr.at(hit_iter).uZeroLevel;
              std::vector<uint16_t> uWfm = PsdReader.VectHitData.at(hit_iter).uWfm;
              uSignalCharge /= (int) kfAdc_to_mV;  // ->now in mV

              if (uHitChannel >= kuNbChanPsd)  //uHitChannel numerated from 0
              {
                LOG(error) << "hit channel number out of range! channel index: " << uHitChannel
                           << " max: " << GetNbChanPsd();
                break;
              }
              //Hit header
              fhHitChargeMap->Fill(uHitChannel, uSignalCharge);
              fhHitMapEvo->Fill(uHitChannel, fdMsTime - fdStartTime);
              fhChanHitMapEvo->Fill(uHitChannel, fdMsTime - fdStartTime);  //should be placed map(ch)

              if (fbMonitorChanMode) {

                fvhHitChargeChan[uHitChannel]->Fill(uSignalCharge);
                fvhHitZeroLevelChan[uHitChannel]->Fill(uZeroLevel);

                //Hit data
                double dHitAmlpitude = 0;
                double dHitChargeWfm = 0;
                if (fbMonitorWfmMode) fvhHitWfmChan[uHitChannel]->Reset();
                if (fbMonitorFitMode) fvhHitFitWfmChan[uHitChannel]->Reset();

                dHitChargeWfm = std::accumulate(uWfm.begin(), uWfm.end(), 0);
                dHitChargeWfm -= uZeroLevel * uWfm.size();
                auto const max_iter = std::max_element(uWfm.begin(), uWfm.end());
                assert(max_iter != uWfm.end());
                if (max_iter == uWfm.end()) break;
                //uint8_t hit_time_max = std::distance(uWfm.begin(), max_iter);
                dHitAmlpitude = *max_iter - uZeroLevel;
                dHitAmlpitude /= kfAdc_to_mV;
                dHitChargeWfm /= kfAdc_to_mV;
                fvhHitAmplChan[uHitChannel]->Fill(dHitAmlpitude);
                fvhHitChargeByWfmChan[uHitChannel]->Fill(dHitChargeWfm);

                if (fbMonitorWfmMode) {
                  for (UInt_t wfm_iter = 0; wfm_iter < uWfm.size(); wfm_iter++)
                    fvhHitWfmChan[uHitChannel]->Fill(wfm_iter, uWfm.at(wfm_iter));
                  fvhHitWfmChan[uHitChannel]->SetTitle(
                    Form("Waveform channel %03u charge %0u zero level %0u; Time [adc "
                         "counts]; Amplitude [adc counts]",
                         uHitChannel, uSignalCharge, uZeroLevel));
                  for (uint8_t i = 0; i < kuNbWfmRanges; ++i) {
                    if (uSignalCharge > kvuWfmRanges.at(i) && uSignalCharge < kvuWfmRanges.at(i + 1)) {
                      UInt_t uFlatIndexOfChange = i * kuNbChanPsd + uHitChannel;

                      UInt_t uWfmExampleIter = kvuWfmInRangeToChangeChan.at(uFlatIndexOfChange);
                      UInt_t uFlatIndexHisto =
                        uWfmExampleIter * kuNbWfmRanges * kuNbChanPsd + i * kuNbChanPsd + uHitChannel;
                      fv3hHitWfmFlattenedChan[uFlatIndexHisto]->Reset();

                      for (UInt_t wfm_iter = 0; wfm_iter < uWfm.size(); wfm_iter++)
                        fv3hHitWfmFlattenedChan[uFlatIndexHisto]->Fill(wfm_iter, uWfm.at(wfm_iter));
                      fv3hHitWfmFlattenedChan[uFlatIndexHisto]->SetTitle(
                        Form("Waveform channel %03u charge %0u zero level %0u; Time "
                             "[adc counts]; Amplitude [adc counts]",
                             uHitChannel, uSignalCharge, uZeroLevel));

                      kvuWfmInRangeToChangeChan.at(uFlatIndexOfChange)++;
                      if (kvuWfmInRangeToChangeChan.at(uFlatIndexOfChange) == kuNbWfmExamples)
                        kvuWfmInRangeToChangeChan.at(uFlatIndexOfChange) = 0;

                    }  // if( uSignalCharge > kvuWfmRanges.at(i) && uSignalCharge < kvuWfmRanges.at(i+1) )
                  }    // for (uint8_t i=0; i<kuNbWfmRanges; ++i)
                }      //if (fbMonitorWfmMode)

                if (fbMonitorFitMode) {
                  int gate_beg = 0;
                  int gate_end = uWfm.size() - 1;
                  PsdSignalFitting::PronyFitter Pfitter(2, 2, gate_beg, gate_end);

                  Pfitter.SetDebugMode(0);
                  Pfitter.SetWaveform(uWfm, uZeroLevel);
                  int SignalBeg           = 2;
                  Int_t best_signal_begin = Pfitter.ChooseBestSignalBeginHarmonics(SignalBeg - 1, SignalBeg + 1);

                  Pfitter.SetSignalBegin(best_signal_begin);
                  Pfitter.CalculateFitHarmonics();
                  Pfitter.CalculateFitAmplitudes();

                  Float_t fit_integral = Pfitter.GetIntegral(gate_beg, gate_end) / kfAdc_to_mV;
                  Float_t fit_R2       = Pfitter.GetRSquare(gate_beg, gate_end);

                  std::complex<float>* harmonics = Pfitter.GetHarmonics();
                  std::vector<uint16_t> uFitWfm  = Pfitter.GetFitWfm();
                  for (UInt_t wfm_iter = 0; wfm_iter < uFitWfm.size(); wfm_iter++)
                    fvhHitFitWfmChan[uHitChannel]->Fill(wfm_iter, uFitWfm.at(wfm_iter));
                  fvhHitWfmChan[uHitChannel]->SetTitle(Form("Waveform channel %03u charge %0u zero level %0u R2 %.5f; "
                                                            "Time [adc counts]; Amplitude [adc counts]",
                                                            uHitChannel, uSignalCharge, uZeroLevel, fit_R2));

                  fvhFitQaChan[uHitChannel]->Fill(fit_integral, fit_R2);

                  if (fit_R2 > 0.02) continue;
                  fvhFitHarmonic1Chan[uHitChannel]->Fill(std::real(harmonics[1]), std::imag(harmonics[1]));
                  fvhFitHarmonic2Chan[uHitChannel]->Fill(std::real(harmonics[2]), std::imag(harmonics[2]));
                }  //if (fbMonitorFitMode)
              }    //if (fbMonitorChanMode)

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

        if (uNbMessages != PsdReader.GetTotalGbtWordsRead()) {
          fbPsdMissedData = kTRUE;
          LOG(error) << "Wrong amount of messages read!"
                     << " in microslice " << uNbMessages << " by PsdReader " << PsdReader.GetTotalGbtWordsRead();

          if (fbFirstPackageError) {
            std::ofstream error_log(Form("%llu_errorlog.txt", fulCurrentMsIdx), std::ofstream::out);
            for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++) {
              uint64_t ulData = static_cast<uint64_t>(pInBuff[uIdx]);
              error_log << Form("%016llx\n", (long long int) ulData);
            }
            error_log.close();
            fbFirstPackageError = kFALSE;
            printf("Written file %llu_errorlog.txt\n", fulCurrentMsIdx);
          }
        }

        if (fulCurrentMsIdx != PsdReader.EvHdrAb.ulMicroSlice)
          LOG(error) << "Wrong MS index!"
                     << " in microslice " << fulCurrentMsIdx << " by PsdReader " << PsdReader.EvHdrAb.ulMicroSlice
                     << "\n";

        fuMsgsCntInMs += uNbMessages;
        fuReadMsgsCntInMs += PsdReader.GetTotalGbtWordsRead();
        fuLostMsgsCntInMs += uNbMessages - PsdReader.GetTotalGbtWordsRead();

      }  //if(uNbMessages > 1)

      if (fdPrevMsTime < 0.) fdPrevMsTime = fdMsTime;
      else {
        fhMsLengthEvo->Fill(fdMsTime - fdStartTime, 1e9 * (fdMsTime - fdPrevMsTime));
        fdPrevMsTime = fdMsTime;
      }

      /// Fill histograms
      FillHistograms();

      break;
    }  // case 0
       // --------------------------------------------------------------------------------------------------

  }    // switch

  return kTRUE;
}

Bool_t CbmMcbm2018MonitorAlgoPsd::CreateHistograms()
{
  std::string sFolder    = "MoniPsd";
  std::string sFitFolder = "PronyFit";
  LOG(info) << "create Histos for PSD monitoring ";

  /// Logarithmic bining
  uint32_t iNbBinsLog = 0;
  /// Parameters are NbDecadesLog, NbStepsDecade, NbSubStepsInStep
  std::vector<double> dBinsLogVector = GenerateLogBinArray(2, 3, 1, iNbBinsLog);
  double* dBinsLog                   = dBinsLogVector.data();

  fhHitChargeMap  = new TH2I("hHitChargeMap", "Map of hits charges in PSD detector; Chan; Charge [mV]", kuNbChanPsd, 0.,
                            kuNbChanPsd, fviHistoChargeArgs.at(0), fviHistoChargeArgs.at(1), fviHistoChargeArgs.at(2));
  fhHitMapEvo     = new TH2I("hHitMapEvo",
                         "Map of hits in PSD detector electronics vs time in "
                         "run; Chan; Time in run [s]; Hits Count []",
                         kuNbChanPsd, 0., kuNbChanPsd, fuHistoryHistoSize, 0, fuHistoryHistoSize);
  fhChanHitMapEvo = new TH2I("hChanHitMapEvo",
                             "Map of hits in PSD detector vs time in run; "
                             "Chan; Time in run [s]; Hits Count []",
                             kuNbChanPsd, 0., kuNbChanPsd, fuHistoryHistoSize, 0, fuHistoryHistoSize);


  fhMissedData = new TH1I("hMissedData", "PSD Missed data", 2, 0, 2);

  fhAdcTime = new TH1I("hAdcTime", "ADC time; Adc time []", 100, 0, 160000);

  fhMsLengthEvo = new TH2I("hMsLengthEvo", "Evolution of MS length vs time in run; Time in run [s]; MS length [ns]",
                           fuHistoryHistoSize, 0, fuHistoryHistoSize, (Int_t) 1e2, 0, 1e8);

  fhMsgsCntPerMsEvo     = new TH2I("hMsgsCntPerMsEvo",
                               "Evolution of TotalMsgs counts, per MS vs time in run; Time in "
                               "run [s]; TotalMsgs Count/MS []; MS",
                               fuHistoryHistoSize, 0, fuHistoryHistoSize, iNbBinsLog, dBinsLog);
  fhReadMsgsCntPerMsEvo = new TH2I("ReadMsgsCntPerMsEvo",
                                   "Evolution of ReadMsgs counts, per MS vs time in run; Time in run "
                                   "[s]; ReadMsgs Count/MS []; MS",
                                   fuHistoryHistoSize, 0, fuHistoryHistoSize, iNbBinsLog, dBinsLog);
  fhLostMsgsCntPerMsEvo = new TH2I("hLostMsgsCntPerMsEvo",
                                   "Evolution of LostMsgs counts, per MS vs time in run; Time in run "
                                   "[s]; LostMsgs Count/MS []; MS",
                                   fuHistoryHistoSize, 0, fuHistoryHistoSize, iNbBinsLog, dBinsLog);
  fhReadEvtsCntPerMsEvo = new TH2I("hReadEvtCntPerMsEvo",
                                   "Evolution of ReadEvents counts, per MS vs time in run; Time in "
                                   "run [s]; ReadEvents Count/MS []; MS",
                                   fuHistoryHistoSize, 0, fuHistoryHistoSize, iNbBinsLog, dBinsLog);

  /// Add pointers to the vector with all histo for access by steering class
  AddHistoToVector(fhHitChargeMap, sFolder);
  AddHistoToVector(fhHitMapEvo, sFolder);
  AddHistoToVector(fhChanHitMapEvo, sFolder);

  AddHistoToVector(fhMissedData, sFolder);
  AddHistoToVector(fhAdcTime, sFolder);
  AddHistoToVector(fhMsLengthEvo, sFolder);

  AddHistoToVector(fhMsgsCntPerMsEvo, sFolder);
  AddHistoToVector(fhReadMsgsCntPerMsEvo, sFolder);
  AddHistoToVector(fhLostMsgsCntPerMsEvo, sFolder);
  AddHistoToVector(fhReadEvtsCntPerMsEvo, sFolder);

  /*******************************************************************/
  if (fbMonitorChanMode) {

    for (UInt_t uChan = 0; uChan < kuNbChanPsd; ++uChan) {

      fvhHitZLChanEvo[uChan] = new TH2I(
        Form("hHitZLChanEvo%03u", uChan),
        Form("Hits ZeroLevel evolution for channel %03u; Time in run [s]; ZeroLevel [adc counts]", uChan),
        fuHistoryHistoSize, 0, fuHistoryHistoSize, fviHistoZLArgs.at(0), fviHistoZLArgs.at(1), fviHistoZLArgs.at(2));
      fvhHitZLChanEvo[uChan]->SetMarkerColor(kRed);
      fvhHitLPChanEvo[uChan] = new TH2I(
        Form("hHitLPChanEvo%03u", uChan),
        Form("Hits LastPoint evolution for channel %03u; Time in run [s]; ZeroLevel [adc counts]", uChan),
        fuHistoryHistoSize, 0, fuHistoryHistoSize, fviHistoZLArgs.at(0), fviHistoZLArgs.at(1), fviHistoZLArgs.at(2));
      fvhHitLPChanEvo[uChan]->SetMarkerColor(kBlue);
      fvhHitFAChanEvo[uChan] = new TH2I(
        Form("hHitFAChanEvo%03u", uChan),
        Form("Hits FeeAccumulator evolution for channel %03u; Time in run [s]; ZeroLevel [adc counts]", uChan),
        fuHistoryHistoSize, 0, fuHistoryHistoSize, fviHistoZLArgs.at(0), fviHistoZLArgs.at(1), fviHistoZLArgs.at(2));
      fvhHitFAChanEvo[uChan]->SetMarkerColor(kOrange);


      fvhHitChargeChan[uChan] = new TH1I(Form("hHitChargeChan%03u", uChan),
                                         Form("Hits charge distribution for channel %03u; Charge [mV]", uChan),
                                         fviHistoChargeArgs.at(0), fviHistoChargeArgs.at(1), fviHistoChargeArgs.at(2));

      fvhHitZeroLevelChan[uChan] =
        new TH1I(Form("hHitZeroLevelChan%03u", uChan),
                 Form("Hits zero level distribution for channel %03u; ZeroLevel [adc counts]", uChan),
                 fviHistoZLArgs.at(0), fviHistoZLArgs.at(1), fviHistoZLArgs.at(2));

      fvhHitAmplChan[uChan] = new TH1F(Form("hHitAmplChan%03u", uChan),
                                       Form("Hits amplitude distribution for channel %03u; Amplitude [mV]", uChan),
                                       fviHistoAmplArgs.at(0), fviHistoAmplArgs.at(1), fviHistoAmplArgs.at(2));

      fvhHitChargeByWfmChan[uChan] =
        new TH1I(Form("hHitChargeByWfmChan%03u", uChan),
                 Form("Hits charge by waveform distribution for channel %03u; "
                      "Charge [mV]",
                      uChan),
                 fviHistoChargeArgs.at(0), fviHistoChargeArgs.at(1), fviHistoChargeArgs.at(2));


      AddHistoToVector(fvhHitZLChanEvo[uChan], sFolder);
      AddHistoToVector(fvhHitLPChanEvo[uChan], sFolder);
      AddHistoToVector(fvhHitFAChanEvo[uChan], sFolder);
      AddHistoToVector(fvhHitChargeChan[uChan], sFolder);
      AddHistoToVector(fvhHitZeroLevelChan[uChan], sFolder);
      AddHistoToVector(fvhHitAmplChan[uChan], sFolder);
      AddHistoToVector(fvhHitChargeByWfmChan[uChan], sFolder);

      if (fbMonitorWfmMode) {
        fvhHitWfmChan[uChan] = new TH1I(Form("hHitWfmChan%03u", uChan), Form("HitWfmChan%03u", uChan), 32, 0, 32);
        fvhHitWfmChan[uChan]->SetMarkerStyle(31);
        fvhHitWfmChan[uChan]->SetMarkerSize(0.5);

        for (UInt_t uWfmRangeIter = 0; uWfmRangeIter < kuNbWfmRanges; uWfmRangeIter++) {
          for (UInt_t uWfmExampleIter = 0; uWfmExampleIter < kuNbWfmExamples; uWfmExampleIter++) {
            UInt_t uFlatIndex = uWfmExampleIter * kuNbWfmRanges * kuNbChanPsd + uWfmRangeIter * kuNbChanPsd + uChan;
            fv3hHitWfmFlattenedChan[uFlatIndex] =
              new TH1I(Form("hHitWfmChan%03uRange%02uExample%02u", uChan, uWfmRangeIter, uWfmExampleIter),
                       Form("HitWfmChan%03uRange%02uExample%02u", uChan, uWfmRangeIter, uWfmExampleIter), 32, 0, 32);

          }  // for( UInt_t uWfmRangeIter = 0; uWfmRangeIter < kuNbWfmRanges; uWfmRangeIter ++)
        }    // for( UInt_t uWfmExampleIter = 0; uWfmExampleIter < kuNbWfmExamples; uWfmExampleIter ++)
      }      // if(fbMonitorWfmMode)

      if (fbMonitorFitMode) {

        fvhHitFitWfmChan[uChan] =
          new TH1I(Form("hHitFitWfmChan%03u", uChan), Form("HitFitWfmChan%03u", uChan), 32, 0, 32);
        fvhHitFitWfmChan[uChan]->SetLineColor(kRed);
        fvhHitFitWfmChan[uChan]->SetLineWidth(2);

        fvhFitHarmonic1Chan[uChan] = new TH2I(
          Form("hFitHarmonic1Chan%03u", uChan),
          Form("Waveform fit harmonic 1 for channel %03u; Real part []; Imag part []", uChan), 400, -2, 2, 200, -1, 1);
        fvhFitHarmonic1Chan[uChan]->SetMarkerColor(kRed);

        fvhFitHarmonic2Chan[uChan] = new TH2I(
          Form("hFitHarmonic2Chan%03u", uChan),
          Form("Waveform fit harmonic 2 for channel %03u; Real part []; Imag part []", uChan), 400, -2, 2, 200, -1, 1);
        fvhFitHarmonic2Chan[uChan]->SetMarkerColor(kBlue);

        fvhFitQaChan[uChan] = new TH2I(
          Form("hFitQaChan%03u", uChan), Form("Waveform fit QA for channel %03u;  Integral [adc counts]; R2 []", uChan),
          fviHistoChargeArgs.at(0), fviHistoChargeArgs.at(1), fviHistoChargeArgs.at(2), 500, 0, 1);

        AddHistoToVector(fvhFitHarmonic1Chan[uChan], sFitFolder);
        AddHistoToVector(fvhFitHarmonic2Chan[uChan], sFitFolder);
        AddHistoToVector(fvhFitQaChan[uChan], sFitFolder);

      }  // if(fbMonitorFitMode)
    }    // for( UInt_t uChan = 0; uChan < kuNbChanPsd; ++uChan )

  }  // if (fbMonitorChanMode)
  /*******************************************************************/

  /// Canvases
  Double_t w = 10;
  Double_t h = 10;

  /*******************************************************************/
  /// Map of hits over PSD detector and same vs time in run
  fcHitMaps = new TCanvas("cHitMaps", "Hit maps", w, h);
  fcHitMaps->Divide(2);

  fcHitMaps->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhChanHitMapEvo->Draw("colz");

  fcHitMaps->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHitChargeMap->Draw("colz");

  AddCanvasToVector(fcHitMaps, "canvases");
  /*******************************************************************/

  /*******************************************************************/
  /// General summary: Hit maps, Hit rate vs time in run, error fraction vs time un run
  fcSummary = new TCanvas("cSummary", "Hit maps, Hit rate, Error fraction", w, h);
  fcSummary->Divide(2, 2);

  fcSummary->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhHitChargeMap->Draw("colz");

  fcSummary->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhChanHitMapEvo->Draw("colz");

  fcSummary->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhMissedData->Draw();

  fcSummary->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  gPad->SetLogz();
  fhMsLengthEvo->Draw("colz");

  AddCanvasToVector(fcSummary, "canvases");
  /*******************************************************************/

  /*******************************************************************/
  /// General summary: Hit maps, Hit rate vs time in run, error fraction vs time un run
  fcGenCntsPerMs = new TCanvas("cGenCntsPerMs", "Messages and hit cnt per MS, Error and Evt Loss Fract. per MS ", w, h);
  fcGenCntsPerMs->Divide(2, 2);

  fcGenCntsPerMs->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  gPad->SetLogz();
  fhMsgsCntPerMsEvo->Draw("colz");

  fcGenCntsPerMs->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  gPad->SetLogz();
  fhReadMsgsCntPerMsEvo->Draw("colz");

  fcGenCntsPerMs->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  gPad->SetLogz();
  fhReadEvtsCntPerMsEvo->Draw("colz");

  fcGenCntsPerMs->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  gPad->SetLogz();
  fhLostMsgsCntPerMsEvo->Draw("colz");

  AddCanvasToVector(fcGenCntsPerMs, "canvases");
  /*******************************************************************/

  if (fbMonitorChanMode) {

    /*******************************************************************/
    /// ZL evo all channels
    fcZLevo = new TCanvas("cZLevo", "ZeroLevel evolaution in all channels", w, h);
    fcZLevo->DivideSquare(kuNbChanPsd);

    for (UInt_t uChan = 0; uChan < kuNbChanPsd; uChan++) {
      fcZLevo->cd(1 + uChan);
      fvhHitZLChanEvo[uChan]->Draw();
      fvhHitLPChanEvo[uChan]->Draw("same");
      fvhHitFAChanEvo[uChan]->Draw("same");
    }  // for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)

    AddCanvasToVector(fcZLevo, "canvases");
    /*******************************************************************/

    /*******************************************************************/
    /// Charge from FPGA all channels
    fcChargesFPGA = new TCanvas("cChargesFPGA", "Charges spectra in all channels calculated by FPGA", w, h);
    fcChargesFPGA->DivideSquare(kuNbChanPsd);

    for (UInt_t uChan = 0; uChan < kuNbChanPsd; uChan++) {
      fcChargesFPGA->cd(1 + uChan)->SetLogy();
      fvhHitChargeChan[uChan]->Draw();
    }  // for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)

    AddCanvasToVector(fcChargesFPGA, "canvases");
    /*******************************************************************/

    /*******************************************************************/
    /// Charge from Waveform all channels
    fcChargesWfm = new TCanvas("cChargesWfm", "Charges spectra in all channels calculated over waveform", w, h);
    fcChargesWfm->DivideSquare(kuNbChanPsd);

    for (UInt_t uChan = 0; uChan < kuNbChanPsd; uChan++) {
      fcChargesWfm->cd(1 + uChan)->SetLogy();
      fvhHitChargeByWfmChan[uChan]->Draw();
    }  // for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)

    AddCanvasToVector(fcChargesWfm, "canvases");
    /*******************************************************************/

    /*******************************************************************/
    /// Amplitudes all channels
    fcAmplitudes = new TCanvas("cAmplitudes", "Amplitude spectra in all channels", w, h);
    fcAmplitudes->DivideSquare(kuNbChanPsd);

    for (UInt_t uChan = 0; uChan < kuNbChanPsd; uChan++) {
      fcAmplitudes->cd(1 + uChan)->SetLogy();
      fvhHitAmplChan[uChan]->Draw();
    }  // for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)

    AddCanvasToVector(fcAmplitudes, "canvases");
    /*******************************************************************/

    /*******************************************************************/
    /// ZeroLevels all channels
    fcZeroLevels = new TCanvas("cZeroLevels", "Zero Level spectra in all channels", w, h);
    fcZeroLevels->DivideSquare(kuNbChanPsd);

    for (UInt_t uChan = 0; uChan < kuNbChanPsd; uChan++) {
      fcZeroLevels->cd(1 + uChan);
      fvhHitZeroLevelChan[uChan]->Draw();
    }  // for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)

    AddCanvasToVector(fcZeroLevels, "canvases");
    /*******************************************************************/

    if (fbMonitorWfmMode) {

      /*******************************************************************/
      /// General summary: Hit maps, Hit rate vs time in run, error fraction vs time un run
      fcWfmsAllChannels = new TCanvas("cWfmsAllChannels", "Last waveforms in PSD fired channels", w, h);
      fcWfmsAllChannels->DivideSquare(kuNbChanPsd);

      for (UInt_t uChan = 0; uChan < kuNbChanPsd; uChan++) {
        fcWfmsAllChannels->cd(1 + uChan);
        if (!fbMonitorFitMode) fvhHitWfmChan[uChan]->Draw("HIST LP");
        if (fbMonitorFitMode) {
          fvhHitWfmChan[uChan]->Draw("HIST P");
          fvhHitFitWfmChan[uChan]->Draw("L SAME");
        }
      }  // for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)

      AddCanvasToVector(fcWfmsAllChannels, "waveforms");
      /*******************************************************************/

      /*******************************************************************/
      /// General summary: Hit maps, Hit rate vs time in run, error fraction vs time un run
      for (UInt_t uChan = 0; uChan < kuNbChanPsd; uChan++) {
        fvcWfmsChan[uChan] =
          new TCanvas(Form("cWfmsChan%03u", uChan), Form("Canvas with last waveforms in channel %03u", uChan), w, h);
        fvcWfmsChan[uChan]->Divide(kuNbWfmExamples, kuNbWfmRanges);
        UInt_t uHisto = 0;

        for (UInt_t uWfmRangeIter = 0; uWfmRangeIter < kuNbWfmRanges; uWfmRangeIter++) {
          for (UInt_t uWfmExampleIter = 0; uWfmExampleIter < kuNbWfmExamples; uWfmExampleIter++) {
            fvcWfmsChan[uChan]->cd(1 + uHisto);
            UInt_t uFlatIndex = uWfmExampleIter * kuNbWfmRanges * kuNbChanPsd + uWfmRangeIter * kuNbChanPsd + uChan;
            fv3hHitWfmFlattenedChan[uFlatIndex]->Draw("HIST LP");
            uHisto++;
          }  // for( UInt_t uWfmRangeIter = 0; uWfmRangeIter < kuNbWfmRanges; uWfmRangeIter ++)
        }    // for( UInt_t uWfmExampleIter = 0; uWfmExampleIter < kuNbWfmExamples; uWfmExampleIter ++)

        AddCanvasToVector(fvcWfmsChan[uChan], "waveforms");
      }  // for( UInt_t uChan = 0; uChan < kuNbChanPsd; uChan ++)

      /*******************************************************************/

    }  // if (fbMonitorWfmMode)

    if (fbMonitorFitMode) {

      fcPronyFit = new TCanvas("cPronyFit", "Prony wfm fitting", w, h);
      fcPronyFit->Divide(2);

      fcPronyFit->cd(1);
      for (UInt_t uChan = 0; uChan < kuNbChanPsd; uChan++) {
        fvhFitHarmonic1Chan[uChan]->Draw("same");
        fvhFitHarmonic2Chan[uChan]->Draw("same");
      }

      fcPronyFit->cd(2);
      for (UInt_t uChan = 0; uChan < kuNbChanPsd; uChan++) {
        fvhFitQaChan[uChan]->Draw("same");
      }

      AddCanvasToVector(fcPronyFit, "PronyFit");

      /*******************************************************************/

    }  // if (fbMonitorFitMode)

  }  // if (fbMonitorChanMode)

  return kTRUE;
}

Bool_t CbmMcbm2018MonitorAlgoPsd::FillHistograms()
{
  fhMissedData->Fill(fbPsdMissedData);
  fhMsgsCntPerMsEvo->Fill(fdMsTime - fdStartTime, fuMsgsCntInMs);
  fhReadMsgsCntPerMsEvo->Fill(fdMsTime - fdStartTime, fuReadMsgsCntInMs);
  fhLostMsgsCntPerMsEvo->Fill(fdMsTime - fdStartTime, fuLostMsgsCntInMs);
  fhReadEvtsCntPerMsEvo->Fill(fdMsTime - fdStartTime, fuReadEvtCntInMs);
  fuMsgsCntInMs     = 0;
  fuReadMsgsCntInMs = 0;
  fuLostMsgsCntInMs = 0;
  fuReadEvtCntInMs  = 0;

  return kTRUE;
}

Bool_t CbmMcbm2018MonitorAlgoPsd::ResetHistograms(Bool_t bResetTime)
{

  if (fbMonitorChanMode) {
    for (UInt_t uChan = 0; uChan < kuNbChanPsd; ++uChan) {
      fvhHitZLChanEvo[uChan]->Reset();
      fvhHitLPChanEvo[uChan]->Reset();
      fvhHitFAChanEvo[uChan]->Reset();
      fvhHitChargeChan[uChan]->Reset();
      fvhHitZeroLevelChan[uChan]->Reset();
      fvhHitAmplChan[uChan]->Reset();
      fvhHitChargeByWfmChan[uChan]->Reset();
      if (fbMonitorWfmMode) fvhHitWfmChan[uChan]->Reset();

      if (fbMonitorFitMode) {
        fvhHitFitWfmChan[uChan]->Reset();
        fvhFitHarmonic1Chan[uChan]->Reset();
        fvhFitHarmonic2Chan[uChan]->Reset();
        fvhFitQaChan[uChan]->Reset();
      }  // if (fbMonitorFitMode)
    }    // for( UInt_t uChan = 0; uChan < kuNbChanPsd; ++uChan )
  }      // if(fbMonitorChanMode)

  if (fbMonitorWfmMode) {
    for (UInt_t uFlatIndex = 0; uFlatIndex < kuNbChanPsd * kuNbWfmRanges * kuNbWfmExamples; ++uFlatIndex)
      fv3hHitWfmFlattenedChan[uFlatIndex]->Reset();
    for (UInt_t uWfmIndex = 0; uWfmIndex < kuNbChanPsd * kuNbWfmRanges; ++uWfmIndex)
      kvuWfmInRangeToChangeChan[uWfmIndex] = 0;
  }  // if(fbMonitorWfmMode)

  fuCurrentSpillIdx = 0;
  fhHitChargeMap->Reset();
  fhHitMapEvo->Reset();
  fhChanHitMapEvo->Reset();

  fhMissedData->Reset();
  fhAdcTime->Reset();
  fhMsLengthEvo->Reset();

  fhMsgsCntPerMsEvo->Reset();
  fhReadMsgsCntPerMsEvo->Reset();
  fhLostMsgsCntPerMsEvo->Reset();
  fhReadEvtsCntPerMsEvo->Reset();

  if (kTRUE == bResetTime) {
    /// Also reset the Start time for the evolution plots!
    fdStartTime = -1.0;
  }  // if( kTRUE == bResetTime )


  return kTRUE;
}

// -------------------------------------------------------------------------
