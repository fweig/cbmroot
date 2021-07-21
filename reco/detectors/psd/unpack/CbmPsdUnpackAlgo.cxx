/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

#include "CbmPsdUnpackAlgo.h"

#include <FairParGenericSet.h>
#include <FairTask.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <numeric>

#include "PronyFitter.h"
#include "PsdGbtReader-v0.00.h"
#include "PsdGbtReader-v1.00.h"

CbmPsdUnpackAlgo::CbmPsdUnpackAlgo() : CbmRecoUnpackAlgo("CbmPsdUnpackAlgo") {}

// ----- Channel address
Int_t CbmPsdUnpackAlgo::getAddress(size_t channel)
{
  assert(channel < fChannelAddress.size());
  return fChannelAddress[channel];
}


// ----- Energy calibration
Double_t CbmPsdUnpackAlgo::getMipCalibration(uint8_t channel)
{
  assert(channel < fMipCalibration.size());
  return fMipCalibration[channel];
}


// ---- GetParContainerRequest ----
std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>*
  CbmPsdUnpackAlgo::GetParContainerRequest(std::string /*geoTag*/, std::uint32_t /*runId*/)
{
  // Basepath for default Trd parameter sets (those connected to a geoTag)
  std::string basepath = Form("%s", fParFilesBasePath.data());
  std::string temppath = "";

  // // Get parameter container
  temppath = basepath + "mPsdPar.par";
  fParContVec.emplace_back(std::make_pair(temppath, std::make_shared<CbmMcbm2018PsdPar>()));

  return &fParContVec;
}

// ---- init
Bool_t CbmPsdUnpackAlgo::init() { return kTRUE; }

// ---- initParSet(FairParGenericSet* parset) ----
Bool_t CbmPsdUnpackAlgo::initParSet(FairParGenericSet* parset)
{
  LOG(info) << fName << "::initParSet - for container " << parset->ClassName();
  if (parset->IsA() == CbmMcbm2018PsdPar::Class()) return initParSet(static_cast<CbmMcbm2018PsdPar*>(parset));

  // If we do not know the derived ParSet class we return false
  LOG(error)
    << fName << "::initParSet - for container " << parset->ClassName()
    << " failed, since CbmTrdUnpackAlgoBaseR::initParSet() does not know the derived ParSet and what to do with it!";
  return kFALSE;
}

// ---- initParSet(CbmMcbm2018PsdPar* parset) ----
Bool_t CbmPsdUnpackAlgo::initParSet(CbmMcbm2018PsdPar* parset)
{
  LOG(debug) << fName << "::initParSetAsic - ";

  fuRawDataVersion = parset->GetDataVersion();
  LOG(debug) << "Data Version: " << fuRawDataVersion;

  UInt_t uNrOfGdpbs = parset->GetNrOfGdpbs();
  LOG(debug) << "Nr. of Tof GDPBs: " << uNrOfGdpbs;

  UInt_t uNrOfFeePerGdpb = parset->GetNrOfFeesPerGdpb();
  LOG(debug) << "Nr. of FEEs per Psd GDPB: " << uNrOfFeePerGdpb;

  UInt_t uNrOfChannelsPerFee = parset->GetNrOfChannelsPerFee();
  LOG(debug) << "Nr. of channels per FEE: " << uNrOfChannelsPerFee;

  auto uNrOfChannelsPerGdpb = uNrOfChannelsPerFee * uNrOfFeePerGdpb;
  LOG(debug) << "Nr. of channels per GDPB: " << uNrOfChannelsPerGdpb;

  fGdpbIdIndexMap.clear();
  for (UInt_t i = 0; i < uNrOfGdpbs; ++i) {
    fGdpbIdIndexMap[parset->GetGdpbId(i)] = i;
    LOG(debug) << "GDPB Id of PSD  " << i << " : " << std::hex << parset->GetGdpbId(i) << std::dec;
  }  // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )

  fuNrOfGbtx = parset->GetNrOfGbtx();
  LOG(debug) << "Nr. of GBTx: " << fuNrOfGbtx;

  //Temporary until creation of full psd map
  UInt_t uNrOfModules  = parset->GetNrOfModules();
  UInt_t uNrOfSections = parset->GetNrOfSections();
  UInt_t uNrOfChannels = uNrOfModules * uNrOfSections;
  LOG(debug) << "Nr. of possible Psd channels: " << uNrOfChannels;
  fviPsdChUId.resize(uNrOfChannels);

  UInt_t iCh = 0;
  for (UInt_t iModule = 0; iModule < uNrOfModules; ++iModule) {
    for (UInt_t iSection = 0; iSection < uNrOfSections; ++iSection) {
      iCh              = iModule * uNrOfSections + iSection;
      fviPsdChUId[iCh] = CbmPsdAddress::GetAddress(iModule, iSection);
    }
  }

  for (size_t ichannel = 0; ichannel < fviPsdChUId.size(); ++ichannel) {
    fMipCalibration.emplace_back(parset->GetMipCalibration(ichannel));
  }
  return kTRUE;
}


// ---- makeDigi
void CbmPsdUnpackAlgo::makeDigi(CbmPsdDsp dsp)
{
  std::unique_ptr<CbmPsdDigi> digi(new CbmPsdDigi(dsp.GetAddress(), dsp.GetTime(), dsp.GetEdep()));

  if (digi) fOutputVec.emplace_back(*std::move(digi));

  if (fOptOutAVec) { fOptOutAVec->emplace_back(dsp); }
}


// ---- unpack
bool CbmPsdUnpackAlgo::unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice)
{
  auto msDescriptor = ts->descriptor(icomp, imslice);
  auto eqid         = msDescriptor.eq_id;

  const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts->content(icomp, imslice));

  uint32_t uSize = msDescriptor.size;
  auto mstime    = msDescriptor.idx;

  LOG(debug4) << "Microslice: " << mstime << " from EqId " << std::hex << eqid << std::dec << " has size: " << uSize;

  if (0 == fvbMaskedComponents.size()) fvbMaskedComponents.resize(ts->num_components(), kFALSE);

  auto dpbid = static_cast<uint16_t>(eqid & 0xFFFF);

  /// Check if this sDPB ID was declared in parameter file and stop there if not
  auto it = fGdpbIdIndexMap.find(dpbid);
  if (it == fGdpbIdIndexMap.end()) {
    if (kFALSE == fvbMaskedComponents[icomp]) {
      LOG(info) << "---------------------------------------------------------------";
      LOG(warning) << "Could not find the gDPB index for AFCK id 0x" << std::hex << dpbid << std::dec
                   << " in timeslice " << ts->index() << " in microslice " << imslice << " component " << icomp << "\n"
                   << "If valid this index has to be added in the PSD "
                      "parameter file in the DbpIdArray field";
      fvbMaskedComponents[icomp] = kTRUE;
    }  // if( kFALSE == fvbMaskedComponents[ uMsComp ] )
    else
      return kTRUE;

    /// Try to get it from the second message in buffer (first is epoch cycle without gDPB ID)
    /// TODO!!!!

    return kFALSE;

  }  // if( it == fGdpbIdIndexMap.end() )


  //   assert(dpbid == 0);  // mCBM201: only one GPDB

  // If not integer number of message in input buffer, print warning/error
  if (0 != (uSize % kuBytesPerMessage))
    LOG(error) << "The input microslice buffer does NOT "
               << "contain only complete nDPB messages!";

  /// Save start time of first valid MS
  /** @todo check if this is really needed */
  Double_t dMsRelativeTime = mstime - fTsStartTime;

  // Compute the number of complete messages in the input microslice buffer
  uint32_t uNbMessages = (uSize - (uSize % kuBytesPerMessage)) / kuBytesPerMessage;

  // Prepare variables for the loop on contents
  const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>(msContent);

  // every 80bit gbt word is decomposed into two 64bit words
  if (!(uNbMessages > 1)) return kTRUE;

  PsdDataV100::PsdGbtReader PsdReader(pInBuff);
  if (fair::Logger::Logging(fair::Severity::debug)) PsdReader.SetPrintOutMode(true);

  while (PsdReader.GetTotalGbtWordsRead() < uNbMessages) {
    int ReadResult = PsdReader.ReadMs();
    if (fair::Logger::Logging(fair::Severity::debug)) {
      // Cast requires to silence a warning on macos, there a uint64_t is a long long unsigned
      printf("\nMicroslice idx: %lu\n", static_cast<size_t>(mstime));
      PsdReader.PrintOut(); /*PsdReader.PrintSaveBuff();*/
    }
    if (ReadResult == 0) {

      double prev_hit_time =
        dMsRelativeTime + (double) PsdReader.VectPackHdr.at(0).uAdcTime * 12.5 - fdTimeOffsetNs;  //in ns

      //hit loop
      for (uint64_t hit_iter = 0; hit_iter < PsdReader.VectHitHdr.size(); hit_iter++) {
        if (PsdReader.VectPackHdr.size() != PsdReader.VectHitHdr.size()) {
          LOG(error) << "Different vector headers sizes!"
                     << " in VectPackHdr " << PsdReader.VectPackHdr.size() << " in VectHitHdr "
                     << PsdReader.VectHitHdr.size() << "\n";
          break;
        }

        uint8_t uHitChannel = PsdReader.VectHitHdr.at(hit_iter).uHitChannel;
        if (uHitChannel >= fviPsdChUId.size()) {
          LOG(error) << "hit channel number out of range! channel index: " << uHitChannel
                     << " max: " << fviPsdChUId.size();
          break;
        }
        UInt_t uChanUId = fviPsdChUId[uHitChannel];  //unique ID

        auto dTime = dMsRelativeTime + (double) PsdReader.VectPackHdr.at(hit_iter).uAdcTime * 12.5 - fdTimeOffsetNs;


        Double_t dEdep = (double) PsdReader.VectHitHdr.at(hit_iter).uSignalCharge / getMipCalibration(uHitChannel);
        /// Energy deposition from FPGA [MeV]

        UInt_t uZL        = PsdReader.VectHitHdr.at(hit_iter).uZeroLevel;  /// ZeroLevel from waveform [adc counts]
        Double_t dAccum   = (double) PsdReader.VectHitHdr.at(hit_iter).uFeeAccum;  /// FPGA FEE Accumulator
        Double_t dAdcTime = (double) PsdReader.VectPackHdr.at(hit_iter).uAdcTime;  /// Adc time of measurement

        Double_t dEdepWfm          = 0.;  /// Energy deposition from waveform [MeV]
        Double_t dAmpl             = 0.;  /// Amplitude from waveform [mV]
        UInt_t uMinimum            = 0;   /// Minimum of waveform [adc samples]
        UInt_t uTimeMax            = 0;   /// Time of maximum in waveform [adc samples]
        std::vector<uint16_t> uWfm = PsdReader.VectHitData.at(hit_iter).uWfm;

        Double_t dFitAmpl    = 0.;    /// Amplitude from fit of waveform [mV]
        Double_t dFitZL      = 0.;    /// ZeroLevel from fit of waveform [adc counts]
        Double_t dFitEdep    = 0.;    /// Energy deposition from fit of waveform [MeV]
        Double_t dFitR2      = 999.;  /// Quality of waveform fit [] -- good near 0
        Double_t dFitTimeMax = -1.;   /// Time of maximum in fit of waveform [adc samples]
        std::vector<uint16_t> uFitWfm;

        if (!uWfm.empty()) {

          int32_t iHitChargeWfm = std::accumulate(uWfm.begin(), uWfm.end(), -uZL * uWfm.size());
          auto const max_iter   = std::max_element(uWfm.begin(), uWfm.end());
          assert(max_iter != uWfm.end());
          if (max_iter == uWfm.end()) break;
          uint8_t uHitTimeMax   = std::distance(uWfm.begin(), max_iter);
          int32_t iHitAmlpitude = *max_iter - uZL;
          auto const min_iter   = std::min_element(uWfm.begin(), uWfm.end());
          uint32_t uHitMinimum  = *min_iter;

          dEdepWfm = (double) iHitChargeWfm / getMipCalibration(uHitChannel);  // ->now in MeV
          dAmpl    = (double) iHitAmlpitude / 16.5;                            // -> now in mV
          uTimeMax = uHitTimeMax;
          uMinimum = uHitMinimum;

          int gate_beg = 0;
          int gate_end = (int) uWfm.size() - 1;
          PsdSignalFitting::PronyFitter Pfitter(2, 2, gate_beg, gate_end);
          Pfitter.SetDebugMode(0);
          Pfitter.SetWaveform(uWfm, uZL);
          int SignalBeg = 4;
          //0.6, 0.2 // 0.72 0.38
          std::complex<float> first_fit_harmonic  = {0.72, 0.0};
          std::complex<float> second_fit_harmonic = {0.38, -0.0};
          Pfitter.SetExternalHarmonics(first_fit_harmonic, second_fit_harmonic);
          Int_t best_signal_begin = Pfitter.ChooseBestSignalBegin(SignalBeg - 1, SignalBeg + 1);
          Pfitter.SetSignalBegin(best_signal_begin);
          Pfitter.CalculateFitAmplitudes();

          dFitEdep    = Pfitter.GetIntegral(gate_beg, gate_end) / getMipCalibration(uHitChannel);  // ->now in MeV
          dFitAmpl    = (Pfitter.GetMaxAmplitude() - Pfitter.GetZeroLevel()) / 16.5;               // ->now in mV
          dFitR2      = Pfitter.GetRSquare(gate_beg, gate_end);
          dFitZL      = Pfitter.GetZeroLevel();
          dFitTimeMax = Pfitter.GetSignalMaxTime();
          uFitWfm     = Pfitter.GetFitWfm();
        }

        CbmPsdDsp dsp = CbmPsdDsp(uChanUId, dTime, fTsStartTime, dEdep, uZL, dAccum, dAdcTime,

                                  dEdepWfm, dAmpl, uMinimum, uTimeMax, uWfm,

                                  dFitAmpl, dFitZL, dFitEdep, dFitR2, dFitTimeMax, uFitWfm);

        // Create the actual digi and move it to the output vector
        makeDigi(dsp);

        if (dTime < prev_hit_time) printf("negative time btw hits! %f after %f \n", dTime, prev_hit_time);

        prev_hit_time = dTime;

      }  // for (uint64_t hit_iter = 0; hit_iter < PsdReader.VectHitHdr.size(); hit_iter++) {
    }
    else if (ReadResult == 1) {
      LOG(error) << "no pack headers in message!";
    }
    else if (ReadResult == 2) {
      LOG(error) << "wrong channel! In header: " << PsdReader.HitHdr.uHitChannel;
    }
    else if (ReadResult == 3) {
      LOG(error) << "check number of waveform points! In header: " << PsdReader.HitHdr.uWfmWords - 1;
    }
    else {
      LOG(error) << "PsdGbtReader.ReadEventFles() didn't return expected values";
    }


  }  // while(PsdReader.GetTotalGbtWordsRead()<uNbMessages)

  if (uNbMessages != PsdReader.GetTotalGbtWordsRead())
    LOG(error) << "Wrong amount of messages read!"
               << " in microslice " << uNbMessages << " by PsdReader " << PsdReader.GetTotalGbtWordsRead() << "\n";

  return kTRUE;
}

CbmPsdUnpackAlgo::~CbmPsdUnpackAlgo() {}


ClassImp(CbmPsdUnpackAlgo)
