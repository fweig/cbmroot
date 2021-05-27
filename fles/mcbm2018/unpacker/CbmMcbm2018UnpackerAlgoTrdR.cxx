/** @file CbmMcbm2018UnpackerAlgoTrdR.cxx
  * @copyright Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#include "CbmMcbm2018UnpackerAlgoTrdR.h"

#include "CbmTrdAddress.h"
#include "CbmTrdHardwareSetupR.h"
#include "CbmTrdParModDigi.h"
#include "CbmTrdParSpadic.h"

#include <Logger.h>

#include "TH2I.h"
#include "TObjString.h"
#include "TProfile.h"
#include "TSystem.h"
#include "TVector3.h"

#include <iostream>
#include <map>

CbmMcbm2018UnpackerAlgoTrdR::CbmMcbm2018UnpackerAlgoTrdR()
  : CbmStar2019Algo()
  //, fRawToDigi( new CbmTrdRawToDigiR )
  , fdMsSizeInCC(0)
  , fbMonitorMode(kFALSE)
  , fbDebugMonitorMode(kFALSE)
  , fbWriteOutput(kTRUE)
  , fbDebugWriteOutput(kFALSE)
  , fbBaselineAvg(kFALSE)
  , fTrdDigiVector(nullptr)
  , fTrdRawMessageVector(nullptr)
  , fSpadicInfoMsgVector(nullptr)
  //, fHistoMap()
  , fIsActiveHistoVec(ECbmTrdUnpackerHistograms::kEndDefinedHistos, false)
  , fHistoArray()
  , fLastDigiTimeVec()
  , fNbTimeslices(0)
  , fCurrTsIdx(0)
  , fMsIndex(0)
  , fTsStartTime(0.0)
  , fTsStopTimeCore(0.0)
  , fMsTime(0.0)
  , fSpadicEpoch(0)
  , fLastFulltime(0)
  , fNbSpadicRawMsg(0)
  , fNbWildRda(0)
  , fNbSpadicErrorMsg(0)
  , fNbUnkownWord(0)
  , fNbSpadicEpochMsg(0)
  , fParContList(nullptr)
  , fRefGeoTag("trd_v20a_mcbm")
  , fAsicPar(nullptr)
  , fDigiPar(nullptr)
  , fGasPar(nullptr)
  , fGainPar(nullptr)
  , fSpadicMap()
  , fAsicChannelMap()
  , fIsFirstChannelsElinkEven(false)
{
}

CbmMcbm2018UnpackerAlgoTrdR::~CbmMcbm2018UnpackerAlgoTrdR()
{
  if (fTrdRawMessageVector != nullptr) { delete fTrdRawMessageVector; }
  if (nullptr != fParCList) delete fParCList;
  if (nullptr != fParContList) delete fParContList;

  if (nullptr != fAsicPar) delete fAsicPar;
  if (nullptr != fDigiPar) delete fDigiPar;
  if (nullptr != fGasPar) delete fGasPar;
  if (nullptr != fGainPar) delete fGainPar;
}

Bool_t CbmMcbm2018UnpackerAlgoTrdR::Init()
{
  LOG(debug) << "Initializing CbmMcbm2018UnpackerAlgoTrdR";
  //fRawToDigi->Init();
  fbIgnoreOverlapMs = kTRUE;
  // fdMsSizeInNs = 1.28e6; //FIXME time should come from parameter file
  fdMsSizeInCC = fdMsSizeInNs / 62.5;

  return kTRUE;
}

void CbmMcbm2018UnpackerAlgoTrdR::Reset()
{
  if (fTrdDigiVector) fTrdDigiVector->clear();
  if (fTrdRawMessageVector) fTrdRawMessageVector->clear();
}

void CbmMcbm2018UnpackerAlgoTrdR::Finish()
{
  LOG(info) << "Finish of CbmMcbm2018UnpackerAlgoTrdR. Unpacked \n " << fNbTimeslices << " Timeslices with \n "
            << fNbSpadicRawMsg << " Spadic Raw Messages,\n " << fNbSpadicEpochMsg << " Spadic Epoch Messages,\n "
            << fNbSpadicErrorMsg << " Spadic Info Messages,\n " << fNbWildRda << " Unexpected RDA Words and\n "
            << fNbUnkownWord << " Unknown Words.";
}

// ---- InitContainers  ----------------------------------------------------
Bool_t CbmMcbm2018UnpackerAlgoTrdR::InitContainers()
{
  LOG(debug) << "Initializing Containers of CbmMcbm2018UnpackerAlgoTrdR";

  Bool_t initOK = ReInitContainers();

  return initOK;
}

// ---- ReInitContainers  ----------------------------------------------------
Bool_t CbmMcbm2018UnpackerAlgoTrdR::ReInitContainers()
{
  LOG(debug) << "(Re-)Initializing Containers of CbmMcbm2018UnpackerAlgoTrdR";

  Bool_t initOk = kTRUE;

  if (!(fAsicPar = (CbmTrdParSetAsic*) fParContList->FindObject("CbmTrdParSetAsic"))) {
    LOG(warning) << "CbmTrdParSetAsic not found";
    initOk = kFALSE;
  }
  if (!(fDigiPar = (CbmTrdParSetDigi*) fParContList->FindObject("CbmTrdParSetDigi"))) {
    LOG(warning) << "CbmTrdParSetDigi not found";
    initOk = kFALSE;
  }
  if (!(fGasPar = (CbmTrdParSetGas*) fParContList->FindObject("CbmTrdParSetGas"))) {
    LOG(warning) << "CbmTrdParSetGas not found";
    initOk = kFALSE;
  }
  if (!(fGainPar = (CbmTrdParSetGain*) fParContList->FindObject("CbmTrdParSetGain"))) {
    LOG(warning) << "CbmTrdParSetGain not found";
    initOk = kFALSE;
  }

  if (initOk) initOk = InitParameters();

  return initOk;
}

// ---- GetParList  ----------------------------------------------------
TList* CbmMcbm2018UnpackerAlgoTrdR::GetParList()
{
  if (fParContList) { return fParContList; }
  else  // create list with default parameters for trd
  {
    fParContList = new TList();

    fAsicPar = new CbmTrdParSetAsic();
    fDigiPar = new CbmTrdParSetDigi();
    fGasPar  = new CbmTrdParSetGas();
    fGainPar = new CbmTrdParSetGain();

    fParContList->Add(fAsicPar);
    fParContList->Add(fDigiPar);
    fParContList->Add(fGasPar);
    fParContList->Add(fGainPar);

    return fParContList;
  }
}

// ---- InitParameters ----------------------------------------------------
Bool_t CbmMcbm2018UnpackerAlgoTrdR::InitParameters()
{
  Bool_t initOk = kTRUE;

  // Initialize the spadic address map
  CbmTrdHardwareSetupR hwSetup;
  fSpadicMap = hwSetup.CreateHwToSwAsicAddressTranslatorMap(true);
  initOk &= !(fSpadicMap.empty());  // at least check that the loaded map is not empty
  fAsicChannelMap = hwSetup.CreateAsicChannelMap(true);
  initOk &= !(fAsicChannelMap.empty());  // at least check that the loaded map is not empty
  if (initOk)
    LOG(debug) << "CbmMcbm2018UnpackerAlgoTrdR - Successfully initialized "
                  "Spadic hardware address map";
  return initOk;
}

Bool_t CbmMcbm2018UnpackerAlgoTrdR::ProcessTs(const fles::Timeslice& ts)
{
  fCurrTsIdx         = ts.index();
  size_t itimeslice  = fCurrTsIdx / 10;
  auto timeshiftpair = fmapTimeshifts.find(itimeslice);
  if (timeshiftpair != fmapTimeshifts.end()) { fvecTimeshiftsPar = &timeshiftpair->second; }

  fTsStartTime = static_cast<Double_t>(ts.descriptor(0, 0).idx);

  /// On first TS, extract the TS parameters from header (by definition stable over time).
  if (0 == fNbTimeslices) {
    fuNbCoreMsPerTs  = ts.num_core_microslices();
    fuNbOverMsPerTs  = ts.num_microslices(0) - ts.num_core_microslices();
    fdTsCoreSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs);
    fdTsFullSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs + fuNbOverMsPerTs);
    LOG(info) << "CbmMcbm2018UnpackerAlgoTrdR::ProcessTs :";
    LOG(info) << "Timeslice parameters: each TS has " << fuNbCoreMsPerTs << " Core MS and " << fuNbOverMsPerTs
              << " Overlap MS, for a core duration of " << fdTsCoreSizeInNs << " ns and a full duration of "
              << fdTsFullSizeInNs << " ns";

    /// Ignore overlap ms if flag set by user
    fuNbMsLoop = fuNbCoreMsPerTs;
    if (kFALSE == fbIgnoreOverlapMs) fuNbMsLoop += fuNbOverMsPerTs;
    LOG(info) << "In each TS " << fuNbMsLoop << " MS will be looped over";
  }

  /// Loop over core microslices (and overlap ones if chosen)
  for (UInt_t MsIndex = 0; MsIndex < fuNbMsLoop; MsIndex++) {
    /// Loop over registered components
    for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx) {
      UInt_t uMsComp = fvMsComponentsList[uMsCompIdx];

      if (kFALSE == ProcessMs(ts, uMsComp, MsIndex)) {
        /// Sort the output vector according to the time
        /// => this assumes all digis before failure were OK. If not we should instead clear it!
        std::sort(fTrdDigiVector->begin(), fTrdDigiVector->end(),
                  [](const CbmTrdDigi& a, const CbmTrdDigi& b) -> bool { return a.GetTime() < b.GetTime(); });
        if (fbDebugWriteOutput && fbDebugSortOutput) {
          std::sort(fTrdRawMessageVector->begin(), fTrdRawMessageVector->end(),
                    [](const CbmTrdRawMessageSpadic& a, const CbmTrdRawMessageSpadic& b) -> bool {
                      return a.GetTime() < b.GetTime();
                    });
        }

        LOG(error) << "Failed to process ts " << fCurrTsIdx << " MS " << MsIndex << " for component " << uMsComp;
        return kFALSE;
      }
    }
  }

  /// Sort the output vector according to the time.
  std::sort(fTrdDigiVector->begin(), fTrdDigiVector->end(),
            [](const CbmTrdDigi& a, const CbmTrdDigi& b) -> bool { return a.GetTime() < b.GetTime(); });

  fNbTimeslices++;
  return kTRUE;
}

Bool_t CbmMcbm2018UnpackerAlgoTrdR::ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx)
{
  fles::MicrosliceDescriptor msDesc = ts.descriptor(uMsCompIdx, uMsIdx);
  // uint16_t msEquipmentID = msDesc.eq_id;   ///< Equipment identifier. Specifies the FLES input link. #FU 27.03.20 unused
  uint32_t msSize = msDesc.size;  ///< Content size. This is the size (in bytes) of the microslice data content.
  // uint64_t msTime = msDesc.idx;      ///< Start time of the microslice in ns since global time zero. #FU 27.03.20 unused
  uint32_t msNbWords =
    (msSize - (msSize % kBytesPerWord)) / kBytesPerWord;  ///< Number of complete Words in the input MS buffer.

  const uint8_t* msPointer  = reinterpret_cast<const uint8_t*>(ts.content(uMsCompIdx, uMsIdx));
  const uint64_t* msContent = reinterpret_cast<const uint64_t*>(msPointer);  ///< Spadic Messages are 64bit Words.

  /// Loop over all 64bit-Words in the current Microslice
  for (uint32_t iWord = 0; iWord < msNbWords; iWord++) {
    uint64_t curWord               = static_cast<uint64_t>(msContent[iWord]);
    Spadic::MsMessageType wordType = GetMessageType(curWord);

    if (wordType == Spadic::MsMessageType::kSOM) {
      //LOG(info) << "New Spadic Message!" ; //debug
      CbmTrdRawMessageSpadic raw = CreateRawMessage(curWord, msDesc);
      uint8_t nSamples           = raw.GetNrSamples();
      /// If the new Message has more than 3 Samples, we need to read in the next words that contain the remaining samples.
      if (nSamples > 3) {
        uint8_t nRda      = GetNumRda(nSamples);
        uint8_t curSample = 3;  // first 3 samples are in som
        /// Loop over the rda words
        for (uint8_t iRda = 0; iRda < nRda; iRda++) {
          ++iWord;
          curWord = static_cast<uint64_t>(msContent[(iWord)]);
          if (GetMessageType(curWord) != Spadic::MsMessageType::kRDA) {
            LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::ProcessMs]  Incomplete Spadic "
                          "Message! RDA Word missing, Microslice corrupted.";
            return kFALSE;
          }
          /// Loop over Samples. There are max 7 samples per word.
          for (uint8_t j = 0; curSample < nSamples && curSample < 32 && j < 7; curSample++, j++) {
            raw.SetSample(ExtractSample(curWord, curSample), curSample);
          }
        }
      }
      fNbSpadicRawMsg++;

      /// Message should now be complete. TODO: Generate Digi and save raw message if needed.
      std::shared_ptr<CbmTrdDigi> digi = MakeDigi(raw);
      if (digi) fTrdDigiVector->emplace_back(*digi);
      /// Save raw message:
      if (fbDebugWriteOutput && (fTrdRawMessageVector != nullptr)) { fTrdRawMessageVector->emplace_back(raw); }

      /// Fill histograms:
      if (fbMonitorMode || fbDebugMonitorMode) {
        FillHistograms();  // fill histograms not based on digi or rawMessage input
        if (digi && fbMonitorMode)
          if (!FillHistograms(*digi)) LOG(error) << "Failed to fill CbmTrdDigi histograms";  // fill digi histograms
        if (fbDebugMonitorMode)
          if (!FillHistograms(raw))
            LOG(error) << "Failed to fill CbmTrdRawMessageSpadic histograms";  // fill rawMessage histograms
      }
    }  // endif (wordType == kSOM )

    if (wordType == Spadic::MsMessageType::kRDA) {
      LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::ProcessMs]  Unexpected RDA "
                    "Word. Microslice corrupted.";
      fNbWildRda++;
      return kFALSE;
    }

    if (wordType == Spadic::MsMessageType::kINF) {
      /// Save info message if needed.
      if (fbDebugWriteOutput && (fSpadicInfoMsgVector != nullptr)) {
        fSpadicInfoMsgVector->emplace_back(std::make_pair(fLastFulltime, curWord));
      }
      fNbSpadicErrorMsg++;

      Spadic::MsInfoType infoType = GetInfoType(curWord);
      // "Spadic_Info_Types";
      if (fIsActiveHistoVec[kSpadic_Info_Types]) {
        ((TH2I*) fHistoArray.At(kSpadic_Info_Types))->Fill(fLastFulltime, (Int_t) infoType);
      }
    }

    if (wordType == Spadic::MsMessageType::kNUL) {
      if (iWord != (msNbWords - 1))  // last word in Microslice is 0.
      {
        LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::ProcessMs]  Null Word but "
                      "not at end of Microslice.";
      }
    }
    if (wordType == Spadic::MsMessageType::kUNK) {
      LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::ProcessMs]  Unknown Word. "
                    "Microslice corrupted.";
      fNbUnkownWord++;
      //return kFALSE ;
    }

    if (wordType == Spadic::MsMessageType::kEPO) {
      uint64_t mask     = 0x3FFFFFFF;
      mask              = mask << 32;
      uint64_t uTS_MSB  = (uint64_t)((curWord & mask) >> 32);
      Long64_t dt_epoch = uTS_MSB - fSpadicEpoch;
      if (dt_epoch != 1) LOG(debug4) << "[CbmMcbm2018UnpackerAlgoTrdR::ProcessMs]  dt_epoch = " << dt_epoch;

      //fLastFulltime = uTS_MSB;
      fNbSpadicEpochMsg++;
      fSpadicEpoch = uTS_MSB;
    }

  }  // end for (uint32_t iWord = 0; iWord < msNbWords; iWord++)

  return kTRUE;
}

void CbmMcbm2018UnpackerAlgoTrdR::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  /// Check for duplicates and ignore if it is the case.
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Add to list
  fvMsComponentsList.emplace_back(component);

  LOG(info) << "CbmMcbm2018UnpackerAlgoTrdR::AddMsComponentToList => Component " << component << " with detector ID 0x"
            << std::hex << usDetectorId << std::dec << " added to list";
}

void CbmMcbm2018UnpackerAlgoTrdR::SetNbMsInTs(size_t uCoreMsNb, size_t uOverlapMsNb)
{
  /// Set Number of Core Microslices per Timeslice
  fuNbCoreMsPerTs = uCoreMsNb;
  /// Set Number of Overlap Microslices per Timeslice
  fuNbOverMsPerTs = uOverlapMsNb;
}

Bool_t CbmMcbm2018UnpackerAlgoTrdR::CreateHistograms()
{
  if (!fbMonitorMode && !fbDebugMonitorMode) return kFALSE;

  fHistoArray.SetOwner(kTRUE);

  Bool_t createHistosOk = kTRUE;
  Int_t iHisto(ECbmTrdUnpackerHistograms::kBeginDefinedHistos);
  for (auto isActive : fIsActiveHistoVec) {
    if (isActive) { createHistosOk &= CreateHistogram((ECbmTrdUnpackerHistograms) iHisto); }
    iHisto++;
  }

  return createHistosOk;
}

// ----    CreateHistogram    ----
Bool_t CbmMcbm2018UnpackerAlgoTrdR::CreateHistogram(ECbmTrdUnpackerHistograms iHisto)
{
  Bool_t createHistoOk                            = kFALSE;
  TString histName                                = "";
  TH1* newHisto                                   = nullptr;
  std::map<Int_t, CbmTrdParMod*> parDigiModuleMap = (std::map<Int_t, CbmTrdParMod*>) fDigiPar->GetModuleMap();
  // Raw Message Histos still need to be separated into module wise histo
  for (auto mapIt : parDigiModuleMap) {
    CbmTrdParModDigi* parDigiModule = (CbmTrdParModDigi*) mapIt.second;
    Int_t moduleId                  = parDigiModule->GetModuleId();
    histName.Form("Module%d-", moduleId);
    switch (iHisto) {
      case kRawMessage_Signalshape_all:
        histName += "RawMessage_Signalshape_all";
        newHisto = new TH2I(histName.Data(), histName.Data(), 32, -0.5, 31.5, 512, -256.5, 255.5);
        newHisto->SetXTitle("time [cc]");
        newHisto->SetYTitle("Pulse height [ADC channels]");
        break;
      case kRawMessage_Signalshape_St:
        histName += "RawMessage_Signalshape_St";
        newHisto = new TH2I(histName.Data(), histName.Data(), 32, -0.5, 31.5, 512, -256.5, 255.5);
        newHisto->SetXTitle("time [cc]");
        newHisto->SetYTitle("Pulse height [ADC channels]");
        break;
      case kRawMessage_Signalshape_Nt:
        histName += "RawMessage_Signalshape_Nt";
        newHisto = new TH2I(histName.Data(), histName.Data(), 32, -0.5, 31.5, 512, -256.5, 255.5);
        newHisto->SetXTitle("time [cc]");
        newHisto->SetYTitle("Pulse height [ADC channels]");
        break;
      case kRawMessage_Signalshape_filtered:
        histName += "RawMessage_Signalshape_filtered";
        newHisto = new TH2I(histName.Data(), histName.Data(), 32, -0.5, 31.5, 512, -256.5, 255.5);
        break;
      case kRawDistributionMapModule5:
        histName += "RawDistributionMapModule5";
        newHisto = new TH2I(histName.Data(), histName.Data(), 42, -0.5, 41.5, 16, -0.5, 15.5);
        break;
      case kRawHitType:
        histName += "RawHitTypes";
        newHisto = new TH1I(histName.Data(), histName.Data(), ((Int_t) Spadic::TriggerType::kSandN + 1),
                            ((Int_t) Spadic::TriggerType::kGlobal - 0.5), ((Int_t) Spadic::TriggerType::kSandN) + 0.5);
        break;
      case kRawPulserDeltaT:
        histName += "RawPulserDeltaT";
        newHisto = new TH1I(histName.Data(), histName.Data(), 40000, 0, 4000000);
        newHisto->SetXTitle("#Delta t [cc]");
        newHisto->SetYTitle("Counts");
        break;
      case kSpadic_Info_Types:
        histName += "Spadic_Info_Types";
        newHisto = new TH2I(histName.Data(), histName.Data(), 500000, 0, 5e9, 5, -0.5, 4.5);
        ((TH2I*) newHisto)->SetXTitle("t /Clockcycles");
        ((TH2I*) newHisto)->SetYTitle("messagetype");
        ((TH2I*) newHisto)->GetYaxis()->SetBinLabel(1, "BOM");
        ((TH2I*) newHisto)->GetYaxis()->SetBinLabel(2, "MSB");
        ((TH2I*) newHisto)->GetYaxis()->SetBinLabel(3, "BUF");
        ((TH2I*) newHisto)->GetYaxis()->SetBinLabel(4, "UNU");
        ((TH2I*) newHisto)->GetYaxis()->SetBinLabel(5, "MIS");
        break;
      case kDigiPulserDeltaT:
        histName += "DigiPulserDeltaT";
        newHisto = new TH1I(histName.Data(), histName.Data(), 60000, 0, 6e8);
        newHisto->SetXTitle("#Delta t [ns]");
        newHisto->SetYTitle("Counts");
        break;
      case kDigiDeltaT:
        histName += "DigiDeltaT";
        newHisto = new TH1I(histName.Data(), histName.Data(), 6000, -10, ((6e7) - 10));
        // FIXME this should be more flexibel and made available for all modules of a given geometry
        fLastDigiTimeVec = std::vector<std::uint64_t>(((CbmTrdParModDigi*) fDigiPar->GetModulePar(5))->GetNofColumns()
                                                        * ((CbmTrdParModDigi*) fDigiPar->GetModulePar(5))->GetNofRows(),
                                                      0);
        newHisto->SetXTitle("#Delta t [ns]");
        newHisto->SetYTitle("Counts");
        break;
      case kDigiMeanHitFrequency:
        histName += "DigiMeanHitFrequency";
        newHisto =
          new TProfile(histName.Data(), histName.Data(), parDigiModule->GetNofColumns() * parDigiModule->GetNofRows(),
                       -0.5, parDigiModule->GetNofColumns() * parDigiModule->GetNofRows() - 0.5);
        // FIXME this should be more flexibel and made available for all modules of a given geometry
        fLastDigiTimeVec = std::vector<std::uint64_t>(((CbmTrdParModDigi*) fDigiPar->GetModulePar(5))->GetNofColumns()
                                                        * ((CbmTrdParModDigi*) fDigiPar->GetModulePar(5))->GetNofRows(),
                                                      0);
        newHisto->SetXTitle("Pad-Channel");
        newHisto->SetYTitle("Hit frequency [kHz]");
        break;
      case kDigiDistributionMap:
        histName += "DigiDistributionMap";
        newHisto = new TH2I(histName.Data(), histName.Data(), parDigiModule->GetNofColumns(), -0.5,
                            (parDigiModule->GetNofColumns() - 0.5), parDigiModule->GetNofRows(), -0.5,
                            (parDigiModule->GetNofRows() - 0.5));
        // newHisto = new TH2I(histName.Data(), histName.Data(), 128, -0.5, 127.5, 6, -0.5, 5.5);  // invert row value since they are count from top to bottom
        newHisto->SetXTitle("Pad column");
        newHisto->SetYTitle("Pad row");
        break;
      case kDigiDistributionMapSt:
        histName += "DigiDistributionMapSt";
        newHisto = new TH2I(histName.Data(), histName.Data(), parDigiModule->GetNofColumns(), -0.5,
                            (parDigiModule->GetNofColumns() - 0.5), parDigiModule->GetNofRows(), -0.5,
                            (parDigiModule->GetNofRows() - 0.5));
        // newHisto = new TH2I(histName.Data(), histName.Data(), 128, -0.5, 127.5, 6, -0.5, 5.5);  // invert row value since they are count from top to bottom
        newHisto->SetXTitle("Pad column");
        newHisto->SetYTitle("Pad row");
        break;
      case kDigiDistributionMapNt:
        histName += "DigiDistributionMapNt";
        newHisto = new TH2I(histName.Data(), histName.Data(), parDigiModule->GetNofColumns(), -0.5,
                            (parDigiModule->GetNofColumns() - 0.5), parDigiModule->GetNofRows(), -0.5,
                            (parDigiModule->GetNofRows() - 0.5));
        // newHisto = new TH2I(histName.Data(), histName.Data(), 128, -0.5, 127.5, 6, -0.5, 5.5);  // invert row value since they are count from top to bottom
        newHisto->SetXTitle("Pad column");
        newHisto->SetYTitle("Pad row");
        break;
      case kDigiChargeSpectrum:
        histName += "DigiChargeSpectrum";
        newHisto = new TH1I(histName.Data(), histName.Data(), 512, 0, 512);
        newHisto->SetYTitle("Counts");
        newHisto->SetXTitle("MaxAdc [ADC channels]");
        break;
      case kDigiChargeSpectrumSt:
        histName += "DigiChargeSpectrumSt";
        newHisto = new TH1I(histName.Data(), histName.Data(), 512, 0, 512);
        newHisto->SetYTitle("Counts");
        newHisto->SetXTitle("MaxAdc [ADC channels]");
        break;
      case kDigiChargeSpectrumNt:
        histName += "DigiChargeSpectrumNt";
        newHisto = new TH1I(histName.Data(), histName.Data(), 512, 0, 512);
        newHisto->SetYTitle("Counts");
        newHisto->SetXTitle("MaxAdc [ADC channels]");
        break;
      case kDigiRelativeTimeMicroslice:
        histName += "DigiRelativeTimeMicroslice";
        newHisto = new TH1D(histName.Data(), histName.Data(), fdMsSizeInNs, 0, fdMsSizeInNs);
        break;
      case kDigiTriggerType:
        histName += "DigiTriggerType";
        newHisto = new TH1I(histName.Data(), histName.Data(), CbmTrdDigi::kNTrg, -0.5, (CbmTrdDigi::kNTrg - 0.5));
        break;
      case kDigiHitFrequency:
        histName += "DigiHitFrequency";
        newHisto = new TProfile(histName.Data(), histName.Data(), 100000, 0, 100000);
        newHisto->SetXTitle("Timeslice");
        newHisto->SetYTitle("#langle hit frequency #rangle");
        break;
      default: return createHistoOk; break;
    }
    LOG(debug4) << Form("UnpackerTrdAlgo - Histo[%d]-%s - initialize", iHisto, histName.Data());
    if (newHisto) {
      TString moduleName(Form("%d", moduleId));
      if (iHisto < kBeginDigiHistos) { fHistoArray.AddAtAndExpand(newHisto, iHisto); }
      else {
        Int_t bitShift      = Int_t(std::log2(std::double_t(kEndDefinedHistos))) + 1;
        Int_t histoPosition = moduleId << bitShift;
        histoPosition += iHisto;
        if (iHisto >= kBeginDigiHistos) fHistoArray.AddAtAndExpand(newHisto, histoPosition);
      }
      // If new HistosTypes are added, they need to be added here!
      if (newHisto->IsA() == TProfile::Class()) AddHistoToVector((TProfile*) newHisto, moduleName.Data());

      if (newHisto->IsA() == TH2I::Class()) AddHistoToVector((TH2I*) newHisto, moduleName.Data());

      if (newHisto->IsA() == TH1I::Class()) AddHistoToVector((TH1I*) newHisto, moduleName.Data());

      if (newHisto->IsA() == TH1D::Class()) AddHistoToVector((TH1D*) newHisto, moduleName.Data());

      createHistoOk = kTRUE;
    }
  }
  return createHistoOk;
}

Bool_t CbmMcbm2018UnpackerAlgoTrdR::FillHistograms() { return kTRUE; }

// ----    FillHistograms(CbmTrdDigi)    ----
Bool_t CbmMcbm2018UnpackerAlgoTrdR::FillHistograms(CbmTrdDigi const& digi)
{
  Bool_t isOkFill      = kTRUE;
  Int_t channelAddress = digi.GetAddressChannel();
  Int_t histoBaseId    = digi.GetAddressModule() << (Int_t(std::log2(std::double_t(kEndDefinedHistos))) + 1);
  //Digi position monitoring
  if (fIsActiveHistoVec[kDigiDistributionMap] || fIsActiveHistoVec[kDigiDistributionMapSt]
      || fIsActiveHistoVec[kDigiDistributionMapNt]) {
    CbmTrdParModDigi* parModDigi = (CbmTrdParModDigi*) fDigiPar->GetModulePar(digi.GetAddressModule());
    Int_t rotatedAddress         = parModDigi->GetOrientation() == 2
                                     ? (channelAddress * (-1) + (parModDigi->GetNofRows() * parModDigi->GetNofColumns()) - 1)
                                     : channelAddress;
    Int_t row                    = parModDigi->GetPadRow(rotatedAddress);
    Int_t column                 = parModDigi->GetPadColumn(rotatedAddress);

    // "DigiDistributionModule5"
    if (fIsActiveHistoVec[kDigiDistributionMap]) {
      ((TH2I*) fHistoArray.At(histoBaseId + kDigiDistributionMap))->Fill(column, row);
    }
    if (fIsActiveHistoVec[kDigiDistributionMapSt]) {
      if (digi.GetAddressModule() == 5 && digi.GetTriggerType() == CbmTrdDigi::kSelf)
        ((TH2I*) fHistoArray.At(histoBaseId + kDigiDistributionMapSt))->Fill(column, row);
    }
    if (fIsActiveHistoVec[kDigiDistributionMapNt]) {
      if (digi.GetAddressModule() == 5 && digi.GetTriggerType() == CbmTrdDigi::kNeighbor)
        ((TH2I*) fHistoArray.At(histoBaseId + kDigiDistributionMapNt))->Fill(column, row);
    }
  }
  // "DigiRelativeTinTimeslice"
  if (fIsActiveHistoVec[kDigiRelativeTimeMicroslice]) {
    std::uint64_t digiTime = digi.GetTime();
    std::uint64_t deltaT   = digiTime - (fSpadicEpoch * fdMsSizeInCC * 62.5);  // in clockcycles
    ((TH1D*) fHistoArray.At(histoBaseId + kDigiRelativeTimeMicroslice))->Fill(deltaT);
  }
  // "kDigiChargeSpectrum"
  if (fIsActiveHistoVec[kDigiChargeSpectrum]) {
    ((TH1I*) fHistoArray.At(histoBaseId + kDigiChargeSpectrum))->Fill(digi.GetCharge());
  }
  // "kDigiChargeSpectrumSt"
  if (fIsActiveHistoVec[kDigiChargeSpectrumSt]) {
    if (digi.GetTriggerType() == CbmTrdDigi::kSelf)
      ((TH1I*) fHistoArray.At(histoBaseId + kDigiChargeSpectrumSt))->Fill(digi.GetCharge());
  }
  // "kDigiChargeSpectrumNt"
  if (fIsActiveHistoVec[kDigiChargeSpectrumNt]) {
    if (digi.GetTriggerType() == CbmTrdDigi::kNeighbor)
      ((TH1I*) fHistoArray.At(histoBaseId + kDigiChargeSpectrumNt))->Fill(digi.GetCharge());
  }
  // "kDigiChargeSpectrumNt"
  if (fIsActiveHistoVec[kDigiTriggerType]) {
    ((TH1I*) fHistoArray.At(histoBaseId + kDigiTriggerType))->Fill(digi.GetTriggerType());
  }


  // "kDigiDeltaT" // DigiPulserDeltaT // "kDigiMeanHitFrequency" // FIXME this works currently with only one module
  if (fIsActiveHistoVec[kDigiDeltaT] || fIsActiveHistoVec[kDigiMeanHitFrequency]
      || fIsActiveHistoVec[kDigiPulserDeltaT]) {
    std::uint64_t dt = ((std::uint64_t) digi.GetTime() - fLastDigiTimeVec.at(channelAddress));
    if (dt > 0) {
      // "kDigiDeltaT"
      if (fIsActiveHistoVec[kDigiDeltaT]) { ((TH1I*) fHistoArray.At(histoBaseId + kDigiDeltaT))->Fill(dt); }
      // "kDigiMeanHitFrequency"
      if (fIsActiveHistoVec[kDigiMeanHitFrequency]) {
        if (dt > 0 && dt < fdTsFullSizeInNs) {
          Double_t hitFreq = (Double_t) dt;
          hitFreq *= 1e-9;
          hitFreq = 1.0 / hitFreq;
          hitFreq /= 1000.0;
          ((TProfile*) fHistoArray.At(histoBaseId + kDigiMeanHitFrequency))->Fill(channelAddress, hitFreq);
        }
      }
      // DigiPulserDeltaT
      if (fIsActiveHistoVec[kDigiPulserDeltaT]) {
        Int_t pulserChannelAddress(663);  // status 03/27/2020
        Int_t pulserModule(5);            // status 03/27/2020
        if (channelAddress == pulserChannelAddress && digi.GetAddressModule() == pulserModule
            && digi.GetTriggerType() == CbmTrdDigi::kSelf) {
          ((TH1I*) fHistoArray.At(histoBaseId + kDigiPulserDeltaT))->Fill(dt);
        }
      }
      // "kDigiMeanHitFrequency"
      if (fIsActiveHistoVec[kDigiHitFrequency]) {
        if (dt > 0 && dt < fdTsFullSizeInNs) {
          Int_t tsCounter  = fNbTimeslices % ((TProfile*) fHistoArray.At(histoBaseId + kDigiHitFrequency))->GetNbinsX();
          Double_t hitFreq = (Double_t) dt;
          hitFreq *= 1e-9;
          hitFreq = 1.0 / hitFreq;
          hitFreq /= 1000.0;
          ((TProfile*) fHistoArray.At(histoBaseId + kDigiHitFrequency))->Fill(tsCounter, hitFreq);
          if (tsCounter == 0) ((TProfile*) fHistoArray.At(histoBaseId + kDigiHitFrequency))->Reset();
        }
      }
      fLastDigiTimeVec.at(channelAddress) = (std::uint64_t) digi.GetTime();
    }
  }
  return isOkFill;
}

// ----    FillHistograms(CbmTrdSpadicRawMessage)    ----
Bool_t CbmMcbm2018UnpackerAlgoTrdR::FillHistograms(CbmTrdRawMessageSpadic const& raw)
{
  Bool_t isOkFill = kTRUE;
  // "RawMessage_Signalshape_filtered"
  if (fIsActiveHistoVec[kRawMessage_Signalshape_filtered]) {
    if (raw.GetHitType() < 2 && !raw.GetMultiHit()) {
      for (unsigned int i = 0; i < raw.GetSamples().size(); i++) {
        ((TH2I*) fHistoArray.At(kRawMessage_Signalshape_filtered))->Fill(i, raw.GetSamples()[i]);
      }
    }
  }
  // "RawMessage_Signalshape_all"
  if (fIsActiveHistoVec[kRawMessage_Signalshape_all]) {
    for (unsigned int i = 0; i < raw.GetSamples().size(); i++) {
      //fHistoMap.at(HistName.Data())->Fill(i, raw.GetSamples()[i]);
      ((TH2I*) fHistoArray.At(kRawMessage_Signalshape_all))->Fill(i, raw.GetSamples()[i]);
    }
  }
  // "kRawMessage_Signalshape_St"
  if (fIsActiveHistoVec[kRawMessage_Signalshape_St]) {
    for (unsigned int i = 0; i < raw.GetSamples().size(); i++) {
      if (raw.GetHitType() == (Int_t) Spadic::TriggerType::kSelf
          || raw.GetHitType() == (Int_t) Spadic::TriggerType::kSandN)
        ((TH2I*) fHistoArray.At(kRawMessage_Signalshape_St))->Fill(i, raw.GetSamples()[i]);
    }
  }
  // "kRawMessage_Signalshape_Nt"
  if (fIsActiveHistoVec[kRawMessage_Signalshape_Nt]) {
    for (unsigned int i = 0; i < raw.GetSamples().size(); i++) {
      if (raw.GetHitType() == (Int_t) Spadic::TriggerType::kNeigh)
        ((TH2I*) fHistoArray.At(kRawMessage_Signalshape_Nt))->Fill(i, raw.GetSamples()[i]);
    }
  }
  // "RawDistributionMapModule5"
  if (fIsActiveHistoVec[kRawDistributionMapModule5]) {
    ((TH1I*) fHistoArray.At(kRawDistributionMapModule5))->Fill(raw.GetElinkId(), raw.GetChannelId());
  }
  // "kRawHitType"
  if (fIsActiveHistoVec[kRawHitType]) { ((TH1I*) fHistoArray.At(kRawHitType))->Fill(raw.GetHitType()); }
  // "RawPulserDeltaT"
  if (fIsActiveHistoVec[kRawPulserDeltaT]) {
    std::uint8_t pulserChannelId = 0;   // status 03/27/2020
    std::uint8_t pulserElinkId   = 29;  // status 03/27/2020
    if (raw.GetChannelId() == pulserChannelId && raw.GetElinkId() == pulserElinkId) {
      Long64_t dt = ((Long64_t) raw.GetFullTime() - (Long64_t) fLastFulltime);
      ((TH1I*) fHistoArray.At(kRawPulserDeltaT))->Fill(dt);
      fLastFulltime = raw.GetFullTime();
    }
  }
  return isOkFill;
}


Bool_t CbmMcbm2018UnpackerAlgoTrdR::ResetHistograms()
{
  /*
    for ( auto &it : fHistoMap )
    {
        it.second->Reset() ;
    }
    */

  for (auto it = fHistoArray.begin(); it != fHistoArray.end(); ++it) {
    ((TH1*) *it)->Reset();
  }
  return kTRUE;
}

Bool_t CbmMcbm2018UnpackerAlgoTrdR::SetDigiOutputPointer(std::vector<CbmTrdDigi>* const pVector)
{
  if (nullptr == fTrdDigiVector) {
    fTrdDigiVector = pVector;
    return kTRUE;
  }
  else {
    return kFALSE;
  }
}

Bool_t
CbmMcbm2018UnpackerAlgoTrdR::SetRawOutputPointer(std::vector<CbmTrdRawMessageSpadic>* const pVector,
                                                 std::vector<std::pair<std::uint64_t, std::uint64_t>>* const qVector)
{
  Bool_t ret = 1;
  if (nullptr == fTrdRawMessageVector) { fTrdRawMessageVector = pVector; }
  else {
    ret &= 0;
  }

  if (qVector != nullptr && fSpadicInfoMsgVector == nullptr) { fSpadicInfoMsgVector = qVector; }
  else {
    ret &= 0;
  }
  return ret;
}

// --------- private: -----------

std::shared_ptr<CbmTrdDigi> CbmMcbm2018UnpackerAlgoTrdR::MakeDigi(CbmTrdRawMessageSpadic raw)
{
  Int_t digiAddress = -1;
  Float_t digiCharge =
    (Float_t) raw.GetMaxAdc()
    + 256;  // REMARK raw.GetMaxADC returns a the value in the range of -256 til 255. However, the digiCharge is stored as unsigned.  // TODO make Settable

  // Int_t digiTriggerType = raw.GetHitType() ; // Spadic::TriggerType this does not work 03/27/2020 - PR digiTriggerType is not Spadic::TriggerType!
  Int_t digiTriggerType = raw.GetHitType();
  if (digiTriggerType == 1) digiTriggerType = 0;  // Shift self trigger to digi selftrigger
  if (digiTriggerType == 2) digiTriggerType = 1;  // Shift neighbour trigger to digi neighbour
  if (digiTriggerType == 3) digiTriggerType = 0;  // Hide spadic kSandN in Self

  Int_t digiErrClass = 0;

  std::uint64_t spadicHwAddress(0);
  spadicHwAddress = (raw.GetElinkId()) + (CbmTrdParAsic::kCriIdPosition * raw.GetCriId())
                    + (CbmTrdParAsic::kCrobIdPosition * raw.GetCrobId());
  Int_t asicAddress(0);
  auto mapIt = fSpadicMap.find(spadicHwAddress);  // check if asic exists
  if (mapIt == fSpadicMap.end()) {
    LOG(debug4) << "CbmMcbm2018UnpackerAlgoTrdR::MakeDigi - No asic address "
                   "found for Spadic hardware address %lu"
                << spadicHwAddress;
    return nullptr;
  }
  asicAddress          = mapIt->second;
  Int_t uniqueModuleId = asicAddress / 1000;
  // Int_t layerId(CbmTrdAddress::GetLayerId(uniqueModuleId));
  // Int_t moduleId(CbmTrdAddress::GetModuleId(uniqueModuleId));

  // GetChannelId per eLink add NSPADICCH / 2 to the second(first) eLink in the case we start with odd(even) eLinks, since, our mapping is based on odd eLinks
  auto asicChannelId =
    (raw.GetElinkId() % 2) == fIsFirstChannelsElinkEven ? raw.GetChannelId() : raw.GetChannelId() + (NSPADICCH / 2);

  digiAddress = (fAsicChannelMap.find(asicAddress))->second.at(asicChannelId);

  ULong64_t digiTime = raw.GetTime();
  if (fvecTimeshiftsPar) {
    digiTime = digiTime - fvecTimeshiftsPar->at(digiAddress);
    raw.SetTime(digiTime);
  }
  digiTime -= fdTimeOffsetNs;

  std::shared_ptr<CbmTrdDigi> digi = std::make_shared<CbmTrdDigi>(
    CbmTrdDigi(digiAddress, uniqueModuleId, digiCharge, digiTime, digiTriggerType, digiErrClass));

  return digi;
}

Spadic::MsMessageType CbmMcbm2018UnpackerAlgoTrdR::GetMessageType(const uint64_t msg)
{
  if ((msg >> 61) == 1)  // SOM  001. ....
  {
    return Spadic::MsMessageType::kSOM;
  }
  else if ((msg >> 63) == 1)  // RDA  1... ....
  {
    return Spadic::MsMessageType::kRDA;
  }
  else if ((msg >> 62) == 1)  // Epoch 01.. ....
  {
    return Spadic::MsMessageType::kEPO;
  }
  else if ((msg >> 60) == 1)  // Spadic Info Message 0001 ....
  {
    return Spadic::MsMessageType::kINF;
  }
  else if (msg == 0)  // Last Word in a Microslice is 0
  {
    return Spadic::MsMessageType::kNUL;
  }
  else  // not a spadic message
  {
    return Spadic::MsMessageType::kUNK;
  }
}

Spadic::MsInfoType CbmMcbm2018UnpackerAlgoTrdR::GetInfoType(const uint64_t msg)
{
  uint64_t mask = 0x000FFFFF;

  if (((msg & mask) >> 18) == 3)  // BOM
  {
    return Spadic::MsInfoType::kBOM;
  }
  if (((msg & mask) >> 17) == 2)  // MSB
  {
    return Spadic::MsInfoType::kMSB;
  }
  if (((msg & mask) >> 17) == 3)  // BUF
  {
    return Spadic::MsInfoType::kBUF;
  }
  if (((msg & mask) >> 17) == 4)  // UNU
  {
    return Spadic::MsInfoType::kUNU;
  }
  if (((msg & mask) >> 17) == 5)  // MIS
  {
    return Spadic::MsInfoType::kMIS;
  }
  else {
    LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::GetInfoType] unknown type!";
    return Spadic::MsInfoType::kMSB;
  }
}

CbmTrdRawMessageSpadic CbmMcbm2018UnpackerAlgoTrdR::CreateRawMessage(const uint64_t word,
                                                                     fles::MicrosliceDescriptor msDesc)
{
  if (GetMessageType(word) != Spadic::MsMessageType::kSOM) {
    LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::CreateRawMessage]   Not a SOM word!";
    return CbmTrdRawMessageSpadic();
  }
  ///Extract Metadata
  uint8_t elinkId = 0, chId = 0, crobId = 0;
  uint16_t criId = msDesc.eq_id;
  // char crobId = msDesc.crob_id; // TODO this needs to be implemented into microslice! - PR 03.2020
  uint8_t hitType = 0, nSamples = 0;
  bool multihit      = false;
  uint16_t timestamp = 0;
  uint64_t mask      = 0x3F;
  mask               = mask << 55;
  elinkId            = (char) ((word & mask) >> 55);
  //extract chID
  mask = 0xF;
  mask = mask << 51;
  chId = (char) ((word & mask) >> 51);
  //extract timestamp
  mask      = 0xFFFF;
  mask      = mask << 35;
  timestamp = (uint16_t)((word & mask) >> 35);
  //extract hitType
  mask    = 0x3;
  mask    = mask << 33;
  hitType = (uint8_t)((word & mask) >> 33);
  //extract MultiHit
  mask     = 0x1;
  mask     = mask << 32;
  multihit = (bool) ((word & mask) >> 32);
  //extract nrSamples
  mask     = 0x1F;
  mask     = mask << 27;
  nSamples = (uint8_t)((word & mask) >> 27);
  nSamples += 1;  //spadic counts from 0 to 31

  // get the correct fulltime
  uint64_t fulltime = timestamp + (fSpadicEpoch * fdMsSizeInCC);  // this is in units of clock cycles
  //uint64_t epoch = fSpadicEpoch >> 3 ;


  // put the first 3 samples, contained in som, into the message.
  std::vector<int16_t> samples;
  for (int i = 0; i < nSamples && i < 3; i++) {
    samples.emplace_back(ExtractSample(word, i, multihit));
  }

  // Create message
  CbmTrdRawMessageSpadic retval(chId, elinkId, crobId, criId, hitType, nSamples, multihit, fulltime, samples);
  return retval;
}

Int_t CbmMcbm2018UnpackerAlgoTrdR::GetNumRda(Int_t nsamples)
{
  if (nsamples < 4) return 0;
  else if (nsamples < 11)
    return 1;
  else if (nsamples < 18)
    return 2;
  else if (nsamples < 25)
    return 3;
  else if (nsamples < 32)
    return 4;
  else
    return 5;
}

int16_t CbmMcbm2018UnpackerAlgoTrdR::ExtractSample(const uint64_t word, uint8_t sample, Bool_t multihit)
{
  const std::array<uint16_t, 32> indices {
    {4, 5, 6, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6, 0}};
  uint64_t mask = 0x1FF;
  if (!((GetMessageType(word) == Spadic::MsMessageType::kSOM)
        || (GetMessageType(word) == Spadic::MsMessageType::kRDA))) {
    LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::ExtractSample]  Wrong Message Type!";
    return -256;
  }

  if ((GetMessageType(word) == Spadic::MsMessageType::kSOM) && (sample > 2)) {
    LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::ExtractSample]  Wrong sample index!";
    return -256;
  }
  if ((GetMessageType(word) == Spadic::MsMessageType::kRDA) && (sample < 3 || sample > 31)) {
    LOG(error) << "[CbmMcbm2018UnpackerAlgoTrdR::ExtractSample]  Wrong sample index!";
    return -256;
  }
  uint16_t index = indices[sample];

  mask          = mask << (9 * (6 - index));
  uint64_t temp = word & mask;
  temp          = temp >> (6 - index) * 9;
  if (fbBaselineAvg && (sample == 0) && !(multihit)) {
    /** When the average baseline feature of the spadic22 is activated,
        *   the value of samples[0] is always lower than -128, so we know it is 10-------
        *   Because of this it is possible to increase the precision by two bits,
        *   by cutting the two MSBs off and shifting everything two bits to the left.
        *   Here we need to undo this operation by shifting two bits righ
        *   and setting the MSB to 1 (negative sign) and the second msb to 0 (value < -128 ).
        **/
    temp = temp >> 2;
    temp ^= (-0 ^ temp) & (1 << 7);
    temp ^= (-1 ^ temp) & (1 << 8);
  }
  struct {
    signed int x : 9;
  } s;
  int16_t result = s.x = temp;
  return result;
}

ClassImp(CbmMcbm2018UnpackerAlgoTrdR)
