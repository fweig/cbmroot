/* Copyright (C) 2019-2020 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Egor Ovcharenko [committer], Pierre-Alain Loizeau */

/**
 * CbmMcbm2018UnpackerAlgoRich
 * E. Ovcharenko, Mar 2019
 * S. Lebedev, June 2021
 * based on other detectors' classes by P.-A. Loizeau
 */

#include "CbmMcbm2018UnpackerAlgoRich.h"

// ROOT
#include <Logger.h>

#include <TCanvas.h>
#include <TList.h>

// CbmRoot
#include "CbmMcbm2018RichPar.h"

#include <iostream>

CbmMcbm2018UnpackerAlgoRich::CbmMcbm2018UnpackerAlgoRich() : CbmStar2019Algo()
{
  this->Init();  //TODO why this is not called by the framework?
}

CbmMcbm2018UnpackerAlgoRich::~CbmMcbm2018UnpackerAlgoRich()
{
  if (nullptr != fParCList) delete fParCList;
  if (nullptr != fUnpackPar) delete fUnpackPar;
}

Bool_t CbmMcbm2018UnpackerAlgoRich::Init() { return kTRUE; }

void CbmMcbm2018UnpackerAlgoRich::Reset() {}

void CbmMcbm2018UnpackerAlgoRich::Finish() {}

Bool_t CbmMcbm2018UnpackerAlgoRich::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmMcbm2018UnpackerAlgoRich";
  Bool_t initOK = ReInitContainers();

  return initOK;
}

Bool_t CbmMcbm2018UnpackerAlgoRich::ReInitContainers()
{
  LOG(info) << "ReInit parameter containers for CbmMcbm2018UnpackerAlgoRich";

  fUnpackPar = (CbmMcbm2018RichPar*) fParCList->FindObject("CbmMcbm2018RichPar");
  if (fUnpackPar == nullptr) { return kFALSE; }

  Bool_t initOK = InitParameters();

  return initOK;
}

TList* CbmMcbm2018UnpackerAlgoRich::GetParList()
{
  if (fParCList == nullptr) { fParCList = new TList(); }
  fUnpackPar = new CbmMcbm2018RichPar("CbmMcbm2018RichPar");
  fParCList->Add(fUnpackPar);

  return fParCList;
}

Bool_t CbmMcbm2018UnpackerAlgoRich::InitParameters()
{
  InitStorage();
  return kTRUE;
}

void CbmMcbm2018UnpackerAlgoRich::InitStorage() {}

void CbmMcbm2018UnpackerAlgoRich::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx) {
    if (component == fvMsComponentsList[uCompIdx]) { return; }
  }

  /// Add to list
  fvMsComponentsList.push_back(component);

  if (fvMsComponentsList.size() == 1) { fRICHcompIdx = component; }
  else {
    LOG(WARN) << "fvMsComponentsList.size() > 1 for RICH. Unpacking may not work due to implementation limitations.";
  }

  LOG(info) << "CbmMcbm2018UnpackerAlgoRich::AddMsComponentToList => Component " << component << " with detector ID 0x"
            << std::hex << usDetectorId << std::dec << " added to list";
}

Bool_t CbmMcbm2018UnpackerAlgoRich::ProcessTs(const fles::Timeslice& /*ts*/)
{
  LOG(debug2) << "CbmMcbm2018UnpackerAlgoRich::ProcessTs(ts): this method do not have implementation.";
  return kTRUE;
}

Bool_t CbmMcbm2018UnpackerAlgoRich::ProcessTs(const fles::Timeslice& ts, size_t component)
{
  /// Ignore First TS as first MS is typically corrupt
  if (0 == ts.index()) { return kTRUE; }

  LOG(debug2) << "CbmMcbm2018UnpackerAlgoRich::ProcessTs(ts, " << component << ")";

  if (1 != fvMsComponentsList.size()) {
    /// If no RICH component, do nothing!
    if (0 == fvMsComponentsList.size()) return kTRUE;

    /// If multiple RICH components, fail the run
    TString sCompList = "";
    for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx) {
      sCompList += Form(" %2lu ", fvMsComponentsList[uMsCompIdx]);
    }
    LOG(fatal)
      << "CbmMcbm2018UnpackerAlgoRich::ProcessTs => More than 1 component in list, unpacking impossible! List is "
      << sCompList;
  }
  component = fvMsComponentsList[0];

  LOG(debug) << "Components:  " << ts.num_components();
  LOG(debug) << "Microslices: " << ts.num_microslices(component);

  const uint64_t compSize = ts.size_component(component);
  LOG(debug) << "Component " << component << " has size " << compSize;

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

  for (size_t iMS = 0; iMS < fuNbMsLoop; ++iMS) {
    fMsInd = iMS;
    if (IsLog()) LOG(debug) << "=======================================================";
    const fles::MicrosliceView mv            = ts.get_microslice(component, iMS);
    const fles::MicrosliceDescriptor& msDesc = mv.desc();

    fCurMSidx = msDesc.idx;
    if (IsLog()) LOG(debug) << "msDesc.size=" << msDesc.size << " msDesc.idx=" << msDesc.idx;

    if (!fRawDataMode) ProcessMs(ts, component, iMS);
    if (fRawDataMode) DebugMs(ts, component, iMS);
  }

  FinalizeTs();

  if (0 == fTsCounter % 1000) { LOG(info) << "Processed " << fTsCounter << " TS"; }

  fTsCounter++;

  /// Sort the buffers of hits due to the time offsets applied
  std::sort(fDigiVect.begin(), fDigiVect.end(),
            [](const CbmRichDigi& a, const CbmRichDigi& b) -> bool { return a.GetTime() < b.GetTime(); });

  return kTRUE;
}


std::string CbmMcbm2018UnpackerAlgoRich::GetLogHeader(CbmMcbm2018RichMicrosliceReader& reader)
{
  std::stringstream stream;
  stream << "[" << fTsCounter << "-" << fMsInd << "-" << reader.GetWordCounter() << "/" << reader.GetSize() / 4 << " "
         << reader.GetWordAsHexString(reader.GetCurWord()) << "] ";
  return stream.str();
}

bool CbmMcbm2018UnpackerAlgoRich::IsLog()
{
  //if (fTsCounter == 25215) return true;
  return false;
}

Bool_t CbmMcbm2018UnpackerAlgoRich::ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx)
{
  const fles::MicrosliceView mv            = ts.get_microslice(uMsCompIdx, uMsIdx);
  const fles::MicrosliceDescriptor& msDesc = mv.desc();

  CbmMcbm2018RichMicrosliceReader reader;
  reader.SetData(mv.content(), msDesc.size);

  // There are a lot of MS  with 8 bytes size
  // Does one need these MS?
  if (reader.GetSize() <= 8) return true;

  while (true) {
    ProcessTrbPacket(reader);
    // -4*2 for 2 last words which contain microslice index
    if (reader.GetOffset() >= reader.GetSize() - 8) break;
    // -4*3 for 0xffffffff padding and 2 last words which contain microslice index
    if (reader.IsNextPadding() && reader.GetOffset() >= reader.GetSize() - 12) break;
  }

  uint32_t msIndexWord1 = reader.NextWord();
  if (IsLog()) LOG(DEBUG4) << GetLogHeader(reader) << "Microslice Index 1:" << reader.GetWordAsHexString(msIndexWord1);

  uint32_t msIndexWord2 = reader.NextWord();
  if (IsLog()) LOG(DEBUG4) << GetLogHeader(reader) << "Microslice Index 2:" << reader.GetWordAsHexString(msIndexWord2);

  return kTRUE;
}

void CbmMcbm2018UnpackerAlgoRich::ProcessTrbPacket(CbmMcbm2018RichMicrosliceReader& reader)
{
  ProcessMbs(reader, false);  // Current MBS
  ProcessMbs(reader, true);   // Previous MBS

  uint32_t trbNum = reader.NextWord();  // TRB trigger number
  if (IsLog()) LOG(debug4) << GetLogHeader(reader) << "TRB Num:" << reader.GetWordAsHexString(trbNum);

  ProcessHubBlock(reader);
}

void CbmMcbm2018UnpackerAlgoRich::ProcessMbs(CbmMcbm2018RichMicrosliceReader& reader, bool isPrev)
{
  uint32_t word     = reader.NextWord();
  uint32_t mbsNum   = word & 0xffffff;      //24 bits
  uint32_t nofCtsCh = (word >> 24) & 0xff;  // 8 bits
  if (IsLog())
    LOG(debug4) << GetLogHeader(reader) << "MBS mbsNum:0x" << std::hex << mbsNum << std::dec
                << " nofCtsCh:" << nofCtsCh;

  for (uint32_t i = 0; i < nofCtsCh; i++) {
    uint32_t wordEpoch = reader.NextWord();
    uint32_t epoch     = CbmMcbm2018RichTdcWordReader::ProcessEpoch(wordEpoch);
    if (IsLog()) LOG(debug4) << GetLogHeader(reader) << "MBS ch:" << i << " epoch:" << epoch;

    uint32_t wordTime = reader.NextWord();
    CbmMcbm2018RichTdcTimeData td;
    CbmMcbm2018RichTdcWordReader::ProcessTimeData(wordTime, td);
    if (IsLog()) LOG(debug4) << GetLogHeader(reader) << "MBS ch:" << i << " " << td.ToString();

    double fullTime = CalculateTime(epoch, td.fCoarse, td.fFine);

    if (isPrev && td.fChannel == 0) fMbsPrevTimeCh0 = fullTime;
    if (isPrev && td.fChannel == 1) fMbsPrevTimeCh1 = fullTime;
  }

  double mbsCorr = fMbsPrevTimeCh1 - fMbsPrevTimeCh0;
  if (IsLog())
    LOG(debug4) << GetLogHeader(reader) << "MBS Prev ch1:" << std::setprecision(15) << fMbsPrevTimeCh1
                << " ch0:" << fMbsPrevTimeCh0 << " corr:" << mbsCorr;
}

void CbmMcbm2018UnpackerAlgoRich::ProcessHubBlock(CbmMcbm2018RichMicrosliceReader& reader)
{
  uint32_t word    = reader.NextWord();
  uint32_t hubId   = word & 0xffff;          // 16 bits
  uint32_t hubSize = (word >> 16) & 0xffff;  // 16 bits
  if (IsLog())
    LOG(debug4) << GetLogHeader(reader) << "hubId:0x" << std::hex << hubId << std::dec << " hubSize:" << hubSize;

  //if ((HubId == 0xc001) || (HubId == 0xc000)) //CTS subevent?
  //if (HubId == 0x5555)
  //if (((HubId >> 8) & 0xff) == 0x82) // TRB subevent? // TODO: check if it starts from 0x82

  // if true then it is CTS sub-sub-event
  bool isLast      = false;
  size_t counter   = 0;
  size_t totalSize = 0;
  while (!isLast) {
    word                     = reader.NextWord();
    uint32_t subSubEventId   = word & 0xffff;                              // 16 bits
    uint32_t subSubEventSize = (word >> 16) & 0xffff;                      // 16 bits
    isLast                   = reader.IsLastSubSubEvent(subSubEventSize);  // if true then it is CTS sub-sub-event
    counter++;
    totalSize += (1 + subSubEventSize);

    if (IsLog())
      LOG(debug4) << GetLogHeader(reader) << counter << ((isLast) ? " CTS" : " DiRICH") << " subSubEventId:0x"
                  << std::hex << subSubEventId << std::dec << " subSubEventSize:" << subSubEventSize;

    if (!isLast) {  // DiRICH event
      // check correctness of subsub event, for safety reasons
      if (((subSubEventId >> 12) & 0xF) != 0x7) {
        LOG(error) << GetLogHeader(reader) << "ERROR: subSubEventId has strange value:0x" << std::hex << subSubEventId
                   << std::dec;
      }
      ProcessSubSubEvent(reader, subSubEventSize, subSubEventId);
    }
    else {  // CTS event
      ProcessCtsSubSubEvent(reader, subSubEventSize, subSubEventId);
    }

    if (fbDebugMonitorMode) {
      //This address calculation is just for mCBM; will be a problem when using full CBM RICH acceptance
      uint16_t histAddr = ((subSubEventId >> 8) & 0xF) * 18 + ((subSubEventId >> 4) & 0xF) * 2 + (subSubEventId & 0xF);
      fhSubSubEventSize->Fill(histAddr, subSubEventSize);  // Words in a DiRICH
    }

    if ((totalSize == hubSize && !isLast) || (totalSize != hubSize && isLast)) {
      if (IsLog()) LOG(error) << "ERROR: totalSize OR isLast is wrong";
    }

    if (totalSize >= hubSize || isLast) break;
  }

  // read last words
  int lastWordsCounter = 0;
  while (true) {
    lastWordsCounter++;
    word = reader.NextWord();
    if (IsLog()) LOG(debug4) << GetLogHeader(reader);
    if (word == 0x600dda7a) break;
    if (lastWordsCounter >= 7) {
      LOG(error) << GetLogHeader(reader)
                 << "CbmMcbm2018UnpackerAlgoRich::ProcessHubBlock() ERROR: No word == 0x600dda7a";
    }
  }
}

void CbmMcbm2018UnpackerAlgoRich::ProcessCtsSubSubEvent(CbmMcbm2018RichMicrosliceReader& reader,
                                                        uint32_t subSubEventSize, uint32_t subSubEventId)
{
  uint32_t word         = reader.NextWord();
  uint32_t ctsState     = word & 0xffff;                                                                   // 16 bits
  uint32_t nofInputs    = (word >> 16) & 0xf;                                                              // 4 bits
  uint32_t nofTrigCh    = (word >> 20) & 0x1f;                                                             // 5 bits
  uint32_t inclLastIdle = (word >> 25) & 0x1;                                                              // 1 bit
  uint32_t inclTrigInfo = (word >> 26) & 0x1;                                                              // 1 bit
  uint32_t inclTime     = (word >> 27) & 0x1;                                                              // 1 bit
  uint32_t ETM          = (word >> 28) & 0x3;                                                              // 2 bits
  uint32_t ctsInfoSize  = 2 * nofInputs + 2 * nofTrigCh + 2 * inclLastIdle + 3 * inclTrigInfo + inclTime;  // in words
  switch (ETM) {
    case 0: break;
    case 1: ctsInfoSize += 1; break;
    case 2: ctsInfoSize += 4; break;
    case 3: break;
  }
  if (IsLog()) LOG(debug4) << GetLogHeader(reader) << "CTS ctsState:" << ctsState << " ctsInfoSize:" << ctsInfoSize;
  for (uint32_t i = 0; i < ctsInfoSize; i++) {
    word = reader.NextWord();  // do nothing?
    if (IsLog()) LOG(debug4) << GetLogHeader(reader) << "CTS info words";
  }
  int nofTimeWords = subSubEventSize - ctsInfoSize - 1;
  ProcessSubSubEvent(reader, nofTimeWords, subSubEventId);
}

void CbmMcbm2018UnpackerAlgoRich::ProcessSubSubEvent(CbmMcbm2018RichMicrosliceReader& reader, int nofTimeWords,
                                                     uint32_t subSubEventId)
{
  // Store if a certain TDC word type was analysed,
  // later one can check if the order is correct
  bool wasHeader   = false;
  bool wasEpoch    = false;
  bool wasTime     = false;
  bool wasTrailer  = false;
  uint32_t epoch   = 0;  // store last epoch obtained in sub-sub-event
  bool errorInData = false;

  // Store last raising edge time for each channel or -1. if no time
  // this array is used to match raising and falling edges
  std::vector<double> raisingTime(33, -1.);

  for (int i = 0; i < nofTimeWords; i++) {
    uint32_t word                   = reader.NextWord();
    CbmMcbm2018RichTdcWordType type = CbmMcbm2018RichTdcWordReader::GetTdcWordType(word);

    if (type == CbmMcbm2018RichTdcWordType::TimeData) {
      if (!wasHeader || !wasEpoch || wasTrailer) {
        LOG(error) << GetLogHeader(reader) << "illegal position of TDC Time (before header/epoch or after trailer)";
        errorInData = true;
        continue;
      }
      wasTime = true;
      ProcessTimeDataWord(reader, i, epoch, word, subSubEventId, raisingTime);
    }
    else if (type == CbmMcbm2018RichTdcWordType::Epoch) {
      if (!wasHeader || wasTrailer) {
        LOG(error) << GetLogHeader(reader) << "illegal position of TDC Epoch (before header or after trailer)";
        errorInData = true;
        continue;
      }
      wasEpoch = true;
      epoch    = CbmMcbm2018RichTdcWordReader::ProcessEpoch(word);
      if (IsLog()) LOG(debug4) << GetLogHeader(reader) << "SubSubEv[" << i << "] epoch:" << epoch;
    }
    else if (type == CbmMcbm2018RichTdcWordType::Header) {
      if (wasEpoch || wasTime || wasTrailer) {
        LOG(error) << GetLogHeader(reader) << "illegal position of TDC Header (after time/epoch/trailer)";
        errorInData = true;
        continue;
      }
      wasHeader          = true;
      uint16_t errorBits = CbmMcbm2018RichTdcWordReader::ProcessHeader(word);
      ErrorMsg(errorBits, CbmMcbm2018RichErrorType::tdcHeader, subSubEventId);
      if (IsLog()) LOG(debug4) << GetLogHeader(reader) << "SubSubEv[" << i << "] header";
    }
    else if (type == CbmMcbm2018RichTdcWordType::Trailer) {
      if (!wasEpoch || !wasTime || !wasHeader) {
        LOG(error) << GetLogHeader(reader) << "illegal position of TDC Trailer (before time/epoch/header)";
        errorInData = true;
        continue;
      }
      wasTrailer         = true;
      uint16_t errorBits = CbmMcbm2018RichTdcWordReader::ProcessTrailer(word);
      ErrorMsg(errorBits, CbmMcbm2018RichErrorType::tdcTrailer, subSubEventId);
      if (IsLog()) LOG(debug4) << GetLogHeader(reader) << "SubSubEv[" << i << "] trailer";
    }
    else if (type == CbmMcbm2018RichTdcWordType::Debug) {
      // for the moment do nothing
    }
    else if (type == CbmMcbm2018RichTdcWordType::Error) {
      LOG(error) << GetLogHeader(reader) << "Wrong TDC word!!! marker:" << ((word >> 29) & 0x7);
      errorInData = true;
    }
  }

  if (errorInData) {
    //TODO:
  }
}

double CbmMcbm2018UnpackerAlgoRich::CalculateTime(uint32_t epoch, uint32_t coarse, uint32_t fine)
{
  return ((double) epoch) * 2048. * 5. + ((double) coarse) * 5. - ((double) fine) * 0.005;
}

void CbmMcbm2018UnpackerAlgoRich::ProcessTimeDataWord(CbmMcbm2018RichMicrosliceReader& reader, int iTdc, uint32_t epoch,
                                                      uint32_t tdcWord, uint32_t subSubEventId,
                                                      std::vector<double>& raisingTime)
{
  CbmMcbm2018RichTdcTimeData td;
  CbmMcbm2018RichTdcWordReader::ProcessTimeData(tdcWord, td);
  double fullTime = CalculateTime(epoch, td.fCoarse, td.fFine);
  // TODO: I do not use unpacker parameters, I use std::map to store full time
  // int idx = fUnpackPar->GetAddressIdx(subSubEventId);
  // if (idx == -1) {
  //   LOG(error) << "ERROR: No AddressIdx found for subSubEventId:0x" << std::hex << subSubEventId << std::dec;
  //   return;
  // }

  if (td.fChannel == 0) {
    if (td.IsRisingEdge()) {
      fPrevLastCh0ReTime[subSubEventId] = fLastCh0ReTime[subSubEventId];
      fLastCh0ReTime[subSubEventId]     = fullTime;
      if (IsLog())
        LOG(debug4) << GetLogHeader(reader) << "SubSubEv[" << iTdc << "] " << td.ToString()
                    << " CH0 Last:" << std::setprecision(15) << fLastCh0ReTime[subSubEventId]
                    << " PrevLast:" << fPrevLastCh0ReTime[subSubEventId]
                    << " diff:" << fLastCh0ReTime[subSubEventId] - fPrevLastCh0ReTime[subSubEventId];
    }
  }
  else {
    double dT           = fullTime - fPrevLastCh0ReTime[subSubEventId];
    double mbsCorr      = fMbsPrevTimeCh1 - fMbsPrevTimeCh0;
    double fullTimeCorr = dT - mbsCorr;
    if (IsLog())
      LOG(debug4) << GetLogHeader(reader) << "SubSubEv[" << iTdc << "] " << td.ToString()
                  << " time:" << std::setprecision(15) << fullTime << " fullTimeCorr:" << fullTimeCorr;

    if (td.fChannel < 1 || td.fChannel >= raisingTime.size()) {
      LOG(error) << "ERROR: channel number is out of limit. Channel:" << td.fChannel;
    }

    if (td.IsRisingEdge()) {
      // always store the latest raising edge. It means that in case RRFF situation only middle RF will be matched.
      raisingTime[td.fChannel] = fullTimeCorr;
    }
    else {
      if (raisingTime[td.fChannel] == -1.) {
        //No raising channel was found before. Skip this falling edge time.
        if (IsLog())
          LOG(debug4) << GetLogHeader(reader) << "SubSubEv[" << iTdc << "] "
                      << "No raising channel was found before. Skip this falling edge time.";
      }
      else {
        // Matching was found, calculate ToT, if tot is in a good range -> create digi
        double ToT = fullTimeCorr - raisingTime[td.fChannel];
        if (IsLog())
          LOG(debug4) << GetLogHeader(reader) << "SubSubEv[" << iTdc << "] "
                      << "ToT:" << ToT;
        if (ToT >= fToTMin && ToT <= fToTMax) {
          if (fbMonitorMode) {
            TH1D* h = GetTotH1(subSubEventId, td.fChannel);
            if (h != nullptr) h->Fill(ToT);

            TH2D* h2 = GetTotH2(subSubEventId);
            if (h2 != nullptr) h2->Fill(td.fChannel, ToT);
          }
          WriteOutputDigi(subSubEventId, td.fChannel, raisingTime[td.fChannel], ToT, fCurMSidx);
        }
        // pair was created, set raising edge to -1.
        raisingTime[td.fChannel] = -1.;
      }
    }
  }
}

void CbmMcbm2018UnpackerAlgoRich::WriteOutputDigi(Int_t fpgaID, Int_t channel, Double_t time, Double_t tot,
                                                  uint64_t MSidx)
{
  Double_t ToTcorr = fbDoToTCorr ? fUnpackPar->GetToTshift(fpgaID, channel) : 0.;
  Int_t pixelUID   = this->GetPixelUID(fpgaID, channel);
  //check ordering
  Double_t finalTime = time + (Double_t) MSidx - fdTimeOffsetNs;

  Double_t lastTime = 0.;

  if (fDigiVect.size() < 1) { fDigiVect.emplace_back(pixelUID, finalTime, tot - ToTcorr); }
  else {
    lastTime = fDigiVect[fDigiVect.size() - 1].GetTime();
    if (lastTime > finalTime) {
      for (int i = fDigiVect.size() - 1; i >= 0; i--) {
        lastTime = fDigiVect[i].GetTime();
        if (lastTime <= finalTime) {
          fDigiVect.emplace(fDigiVect.begin() + i + 1, pixelUID, finalTime, tot - ToTcorr);
          break;
        }
      }
    }
    else {
      fDigiVect.emplace_back(pixelUID, finalTime, tot - ToTcorr);
    }
  }
  // LOG(debug4) << "CbmMcbm2018UnpackerAlgoRich::WriteOutputDigi fDigiVect.size=" << fDigiVect.size();
}

void CbmMcbm2018UnpackerAlgoRich::FinalizeTs() {}

Bool_t CbmMcbm2018UnpackerAlgoRich::CreateHistograms()
{
  int nofTdc = fUnpackPar->GetNaddresses();

  std::vector<std::string> tdcErrorLabels = {"RingBuffOverw.", "noRefTime",      "refTimePrecedes",
                                             "trigW/oRefTime", "markMisRefTime", "multiRefTime",
                                             "refTime<40ns",   "noValidation",   "trigger!=0x1"};
  fhTdcErrors = new TH2D("fhTdcErrors", "Errors in TDC msgs;;", nofTdc, -0.5, nofTdc - 0.5, tdcErrorLabels.size(), -0.5,
                         (double) tdcErrorLabels.size() - 0.5);
  for (size_t i = 0; i < tdcErrorLabels.size(); i++) {
    fhTdcErrors->GetYaxis()->SetBinLabel(i + 1, tdcErrorLabels[i].c_str());
  }
  fhTdcErrors->GetXaxis()->LabelsOption("v");
  fhTdcErrors->GetYaxis()->SetTickSize(0.0);
  fhTdcErrors->GetXaxis()->SetTickSize(0.0);

  for (Int_t iTdc = 0; iTdc < nofTdc; iTdc++) {
    Int_t tdcId    = fUnpackPar->GetAddress(iTdc);
    fMapFEE[tdcId] = iTdc;
    fhTdcErrors->GetXaxis()->SetBinLabel(iTdc + 1, Form("0x%4x", tdcId));

    // init ToT histogramms
    for (Int_t iCh = 0; iCh <= 32; iCh++) {
      GetTotH1(tdcId, iCh);
    }
    GetTotH2(tdcId);

    {
      std::stringstream cName, cTitle;
      int tdc = fUnpackPar->GetAddress(iTdc);
      cName << "cToT2d_TDC_0x" << std::hex << tdc;
      cTitle << "ToTs of TDC 0x" << std::hex << tdc;
      TCanvas* c = new TCanvas(cName.str().c_str(), cTitle.str().c_str(), 10, 10);
      TH2D* h2   = GetTotH2(tdc);
      h2->Draw("colz");
      fcTot2d.push_back(c);
      AddCanvasToVector(c, "ToT_Canvases");
    }
  }

  AddHistoToVector(fhTdcErrors, "");

  if (fbDebugMonitorMode) {
    fhEventSize = new TH1I("fhEventSize", "Size of the Event from TrbNet; Size [bytes]", 350, 0., 70000.);
    AddHistoToVector(fhEventSize, "");

    fhSubEventSize =
      new TH2I("fhSubEventSize", "fhSubEventSize; HubId ; Size [bytes]; Entries", 6, 0, 6, 10000, 0., 10000.);
    AddHistoToVector(fhSubEventSize, "");

    fhSubSubEventSize =
      new TH2I("fhSubSubEventSize", "fhSubSubEventSize; DiRICH ; Size [words]; Entries", 72, 0, 72, 510, 0., 510.);
    AddHistoToVector(fhSubSubEventSize, "");

    fhChnlSize = new TH2I("fhChnlSize", "fhChnlSize; channel; Size [words]; Entries", 33, 0, 33, 25, 0, 25.);
    AddHistoToVector(fhChnlSize, "");
  }

  return kTRUE;
}

TH1D* CbmMcbm2018UnpackerAlgoRich::GetTotH1(uint32_t tdc, uint32_t channel)
{
  TH1D* h = fhTotMap[tdc][channel];
  if (h == nullptr) {
    std::stringstream name, subFolder;
    name << "ToT_tdc0x" << std::hex << tdc << std::dec << "_ch" << channel;
    std::string title = name.str() + ";ToT [ns];Entries";
    subFolder << "ToT/tdc0x" << std::hex << tdc;
    h = new TH1D(name.str().c_str(), title.c_str(), 100, -1., 49.);
    AddHistoToVector(h, subFolder.str());
    fhTotMap[tdc][channel] = h;
  }
  return h;
}

TH2D* CbmMcbm2018UnpackerAlgoRich::GetTotH2(uint32_t tdc)
{
  TH2D* h = fhTot2dMap[tdc];
  if (h == nullptr) {
    std::stringstream name;
    name << "ToT_2d_tdc0x" << std::hex << tdc;
    std::string title     = name.str() + ";channels;ToT [ns]";
    std::string subFolder = "ToT2d";
    h                     = new TH2D(name.str().c_str(), title.c_str(), 33, 0, 32, 200, -1., 49.);
    AddHistoToVector(h, subFolder);
    fhTot2dMap[tdc] = h;
  }
  return h;
}

Bool_t CbmMcbm2018UnpackerAlgoRich::DebugMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx)
{
  const fles::MicrosliceView mv            = ts.get_microslice(uMsCompIdx, uMsIdx);
  const fles::MicrosliceDescriptor& msDesc = mv.desc();
  const uint8_t* ptr                       = mv.content();
  const size_t size                        = msDesc.size;

  if (size == 0) return kTRUE;
  Debug(ptr, size);

  return kTRUE;
}

Int_t CbmMcbm2018UnpackerAlgoRich::Debug(const uint8_t* ptr, const size_t size) { return 0; }


void CbmMcbm2018UnpackerAlgoRich::ErrorMsg(uint16_t errbits, CbmMcbm2018RichErrorType type, uint16_t tdcId)
{
  if (!fbMonitorMode) return;

  if (type == CbmMcbm2018RichErrorType::tdcHeader) {
    // Errors description in TRB manual section 11.3.2. TDC HEADER
    // Bit 0: min. 1 rinǵ buffer overwritten
    int nofHeaderErrorBits       = 1;
    int histBinOffsetHeaderError = 0;
    for (int i = 0; i < nofHeaderErrorBits; i++) {
      if (((errbits >> i) & 0x1) == 1) fhTdcErrors->Fill(fMapFEE[tdcId], i + histBinOffsetHeaderError);
    }
  }
  else if (type == CbmMcbm2018RichErrorType::tdcTrailer) {
    // Errors description in TRB manual section 11.3.5. TDC TRAILER
    // Bit 0: no reference time in trigger handler in TDC
    // Bit 1: reference time precedes a non-timing trigger
    // Bit 2: timing trigger is delivered without a reference time
    // Bit 3: Set with the bit 2 to mark the missing reference time
    // Bit 4: there are more than one detected reference time
    // Bit 5: reference time was too short (<40 ns)
    // Bit 6: no trigger validation arrives from the endpoint after a valid  reference time
    // Bit 7: any timing trigger type except 0x1 is send
    int nofTrailerErrorBits       = 8;
    int histBinOffsetTrailerError = 1;
    for (int i = 0; i < nofTrailerErrorBits; i++) {
      if (((errbits >> i) & 0x1) == 1) fhTdcErrors->Fill(fMapFEE[tdcId], i + histBinOffsetTrailerError);
    }
  }
  else if (type == CbmMcbm2018RichErrorType::ctsHeader) {
    // To be implemented
  }
  else if (type == CbmMcbm2018RichErrorType::ctsTrailer) {
    // To be implemented
  }
  else {
  }
}

Bool_t CbmMcbm2018UnpackerAlgoRich::ResetHistograms() { return kTRUE; }

ClassImp(CbmMcbm2018UnpackerAlgoRich)
