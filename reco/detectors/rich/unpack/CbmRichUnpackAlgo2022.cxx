/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

#include "CbmRichUnpackAlgo2022.h"

#include <FairParGenericSet.h>
#include <FairTask.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <cstdint>


CbmRichUnpackAlgo2022::CbmRichUnpackAlgo2022() : CbmRichUnpackAlgoBase("CbmRichUnpackAlgo2022") {}

CbmRichUnpackAlgo2022::~CbmRichUnpackAlgo2022() {}

// ---- unpack
bool CbmRichUnpackAlgo2022::unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice)
{

  const fles::MicrosliceView mv            = ts->get_microslice(icomp, imslice);
  const fles::MicrosliceDescriptor& msDesc = mv.desc();

  // Check the version number of the MS to identify wrong unpacker for data
  if (msDesc.sys_ver != 0x3) {
    LOG(fatal) << "CbmRichUnpackAlgo2022: System Version of the microslice is not compatible with this unpacker!";
    return false;
  }

  // Clear CbmTime of MS. Used to get time offset of subtriggers to MS start
  fCbmTimeMS = 0;

  CbmRichUnpackAlgoMicrosliceReader reader;
  reader.SetData(mv.content(), msDesc.size);

  auto mstime = msDesc.idx;
  fMsRefTime  = mstime - fTsStartTime;

  // There are a lot of MS  with 8 bytes size
  // Does one need these MS?
  if (reader.GetSize() <= 8) return true;

  while (true) {
    processTrbPacket(reader);
    // -4*2 for 2 last words which contain microslice index
    if (reader.GetOffset() >= reader.GetSize() - 8) break;
    // -4*3 for 0xffffffff padding and 2 last words which contain microslice index
    if (reader.IsNextPadding() && reader.GetOffset() >= reader.GetSize() - 12) break;
  }

  uint32_t msIndexWord1 = reader.NextWord();
  if (isLog()) LOG(DEBUG4) << getLogHeader(reader) << "Microslice Index 1:" << reader.GetWordAsHexString(msIndexWord1);

  uint32_t msIndexWord2 = reader.NextWord();
  if (isLog()) LOG(DEBUG4) << getLogHeader(reader) << "Microslice Index 2:" << reader.GetWordAsHexString(msIndexWord2);

  //Output debugging info
  if (fMonitor) {
    for (auto itHit = fOutputVec.begin(); itHit != fOutputVec.end(); ++itHit) {
      fMonitor->FillDigisTimeInRun(itHit->GetTime());
      fMonitor->FillDigisToT(itHit->GetToT());
    }
    fMonitor->FillVectorSize(ts->index(), fOutputVec.size());
  }

  return true;
}

void CbmRichUnpackAlgo2022::processHubBlock(CbmRichUnpackAlgoMicrosliceReader& reader)
{
  uint32_t word    = reader.NextWord();
  uint32_t hubId   = word & 0xffff;          // 16 bits
  uint32_t hubSize = (word >> 16) & 0xffff;  // 16 bits
  if (isLog())
    LOG(debug4) << getLogHeader(reader) << "hubId:0x" << std::hex << hubId << std::dec << " hubSize:" << hubSize;

  //if ((HubId == 0xc001) || (HubId == 0xc000)) //CTS subevent?
  //if (HubId == 0x5555)
  //if (((HubId >> 8) & 0xff) == 0x82) // TRB subevent? // TODO: check if it starts from 0x82

  // if true then it is CTS sub-sub-event
  bool isLast         = false;
  size_t counter      = 0;
  size_t totalSize    = 0;
  fCurrentSubSubEvent = 1;

  while (!isLast) {
    word                     = reader.NextWord();
    uint32_t subSubEventId   = word & 0xffff;                              // 16 bits
    uint32_t subSubEventSize = (word >> 16) & 0xffff;                      // 16 bits
    isLast                   = reader.IsLastSubSubEvent(subSubEventSize);  // if true then it is CTS sub-sub-event
    counter++;
    totalSize += (1 + subSubEventSize);

    if (isLog())
      LOG(debug4) << getLogHeader(reader) << counter << ((isLast) ? " CTS" : " DiRICH") << " subSubEventId:0x"
                  << std::hex << subSubEventId << std::dec << " subSubEventSize:" << subSubEventSize;

    if (!isLast) {  // DiRICH event
      // check correctness of subsub event, for safety reasons
      if (((subSubEventId >> 12) & 0xF) != 0x7) {
        LOG(error) << getLogHeader(reader) << "ERROR: subSubEventId has strange value:0x" << std::hex << subSubEventId
                   << std::dec;
      }
      processSubSubEvent(reader, subSubEventSize, subSubEventId);
    }
    else {  // CTS event
      fCurrentSubSubEvent = 0;
      processCtsSubSubEvent(reader, subSubEventSize, subSubEventId);
    }

    //  if (fbDebugMonitorMode) {
    //    //This address calculation is just for mCBM; will be a problem when using full CBM RICH acceptance
    //    uint16_t histAddr = ((subSubEventId >> 8) & 0xF) * 18 + ((subSubEventId >> 4) & 0xF) * 2 + (subSubEventId & 0xF);
    //    fhSubSubEventSize->Fill(histAddr, subSubEventSize);  // Words in a DiRICH
    //  }

    if ((totalSize == hubSize && !isLast) || (totalSize != hubSize && isLast)) {
      if (isLog()) LOG(error) << "ERROR: totalSize OR isLast is wrong";
    }

    if (totalSize >= hubSize || isLast) break;

    fCurrentSubSubEvent++;
  }

  // read last words
  int lastWordsCounter = 0;
  while (true) {
    lastWordsCounter++;
    word = reader.NextWord();
    if (isLog()) LOG(debug4) << getLogHeader(reader);
    if (word == 0x600dda7a) {
      if (reader.IsNextPadding()) word = reader.NextWord();
      break;
    }
    if (lastWordsCounter >= 7) {
      LOG(error) << getLogHeader(reader)
                 << "CbmMcbm2018UnpackerAlgoRich::ProcessHubBlock() ERROR: No word == 0x600dda7a";
    }
  }
}

void CbmRichUnpackAlgo2022::processCtsSubSubEvent(CbmRichUnpackAlgoMicrosliceReader& reader, uint32_t subSubEventSize,
                                                  uint32_t subSubEventId)
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
  if (isLog()) LOG(debug4) << getLogHeader(reader) << "CTS ctsState:" << ctsState << " ctsInfoSize:" << ctsInfoSize;
  for (uint32_t i = 0; i < ctsInfoSize; i++) {
    word = reader.NextWord();  // do nothing?
    if (isLog()) LOG(debug4) << getLogHeader(reader) << "CTS info words";
  }
  int nofTimeWords = subSubEventSize - ctsInfoSize - 1;
  processSubSubEvent(reader, nofTimeWords, subSubEventId);
}

void CbmRichUnpackAlgo2022::processSubSubEvent(CbmRichUnpackAlgoMicrosliceReader& reader, int nofTimeWords,
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

  // check if DiRICH (SubSubEvId) is masked
  bool DiRICH_masked = false;
  if (checkMaskedDiRICH(subSubEventId)) { DiRICH_masked = true; }

  for (int i = 0; i < nofTimeWords; i++) {
    uint32_t word = reader.NextWord();
    if (DiRICH_masked) continue;
    CbmRichUnpackAlgoTdcWordType type = CbmRichUnpackAlgoTdcWordReader::GetTdcWordType(word);

    if (type == CbmRichUnpackAlgoTdcWordType::TimeData) {
      if (!wasHeader || !wasEpoch || wasTrailer) {
        LOG(error) << getLogHeader(reader) << "DiRICH 0x" << std::hex << subSubEventId << std::dec
                   << ": illegal position of TDC Time (before header/epoch or after trailer)";
        errorInData = true;
        continue;
      }
      wasTime = true;
      processTimeDataWord(reader, i, epoch, word, subSubEventId, raisingTime);
    }
    else if (type == CbmRichUnpackAlgoTdcWordType::Epoch) {
      if (!wasHeader || wasTrailer) {
        LOG(error) << getLogHeader(reader) << "DiRICH 0x" << std::hex << subSubEventId << std::dec
                   << ": illegal position of TDC Epoch (before header or after trailer)";
        errorInData = true;
        continue;
      }
      wasEpoch = true;
      epoch    = CbmRichUnpackAlgoTdcWordReader::ProcessEpoch(word);
      if (isLog()) LOG(debug4) << getLogHeader(reader) << "SubSubEv[" << i << "] epoch:" << epoch;
    }
    else if (type == CbmRichUnpackAlgoTdcWordType::Header) {
      if (wasEpoch || wasTime || wasTrailer) {
        LOG(error) << getLogHeader(reader) << "DiRICH 0x" << std::hex << subSubEventId << std::dec
                   << ": illegal position of TDC Header (after time/epoch/trailer)";
        errorInData = true;
        continue;
      }
      wasHeader = true;
      // uint16_t errorBits = CbmRichUnpackAlgoTdcWordReader::ProcessHeader(word);
      // ErrorMsg(errorBits, CbmRichUnpackAlgoErrorType::tdcHeader, subSubEventId);
      if (isLog()) LOG(debug4) << getLogHeader(reader) << "SubSubEv[" << i << "] header";
    }
    else if (type == CbmRichUnpackAlgoTdcWordType::Trailer) {
      if (!wasEpoch || !wasTime || !wasHeader) {
        LOG(error) << getLogHeader(reader) << "DiRICH 0x" << std::hex << subSubEventId << std::dec
                   << ": illegal position of TDC Trailer (before time/epoch/header)";
        errorInData = true;
        continue;
      }
      wasTrailer = true;
      // uint16_t errorBits = CbmRichUnpackAlgoTdcWordReader::ProcessTrailer(word);
      // ErrorMsg(errorBits, CbmRichUnpackAlgoErrorType::tdcTrailer, subSubEventId);
      if (isLog()) LOG(debug4) << getLogHeader(reader) << "SubSubEv[" << i << "] trailer";
    }
    else if (type == CbmRichUnpackAlgoTdcWordType::Debug) {
      // for the moment do nothing
    }
    else if (type == CbmRichUnpackAlgoTdcWordType::Error) {
      LOG(error) << getLogHeader(reader) << "Wrong TDC word!!! marker:" << ((word >> 29) & 0x7);
      errorInData = true;
    }
  }

  if (errorInData) {
    //TODO:
  }
}

void CbmRichUnpackAlgo2022::processTrbPacket(CbmRichUnpackAlgoMicrosliceReader& reader)
{
  processCBMtime(reader);
  uint32_t trigNum_this = reader.NextWord() & 0xFFFFFF;
  reader.NextWord();  // reserved

  for (auto l = 0; l < 2; ++l) {
    uint32_t wordEpoch = reader.NextWord();
    //uint32_t epoch     = CbmRichUnpackAlgoTdcWordReader::ProcessEpoch(wordEpoch);
    uint32_t wordTime = reader.NextWord();
    //CbmRichUnpackAlgoTdcTimeData td;
    //CbmRichUnpackAlgoTdcWordReader::ProcessTimeData(wordTime, td);

    //if (l == 0) fCtsCh0_cur = calculateTime(epoch, td.fCoarse, td.fFine);
    //if (l == 1) fCtsCh1_cur = calculateTime(epoch, td.fCoarse, td.fFine);
  }

  //prev CBM time (64bit)
  uint32_t cbmtime0           = reader.NextWord();  // CBM 63:32
  uint32_t cbmtime1           = reader.NextWord();  // CBM 31: 0
  uint64_t CbmTimePacket_prev = (uint64_t) cbmtime0 << 32 | cbmtime1;

  uint32_t trigNum_prevMes = reader.NextWord() & 0xFFFFFF;

  reader.NextWord();  // reserved

  for (auto l = 0; l < 14; ++l) {
    uint32_t wordEpoch = reader.NextWord();
    uint32_t epoch     = CbmRichUnpackAlgoTdcWordReader::ProcessEpoch(wordEpoch);
    uint32_t wordTime  = reader.NextWord();
    CbmRichUnpackAlgoTdcTimeData td;
    CbmRichUnpackAlgoTdcWordReader::ProcessTimeData(wordTime, td);

    double fullTime = calculateTime(epoch, td.fCoarse, td.fFine);

    if (l == 0) {
      fMbsPrevTimeCh0       = fullTime;
      fPrevLastCh0ReTime[0] = fullTime;
    }
    if (l == 1) fMbsPrevTimeCh1 = fullTime;
    if (l > 1) fPrevLastCh0ReTime[l - 1] = fullTime;
  }

  uint32_t trbNum = reader.NextWord();  // TRB trigger number
  if (isLog()) LOG(debug4) << getLogHeader(reader) << "TRB Num:" << reader.GetWordAsHexString(trbNum);

  processHubBlock(reader);
}

void CbmRichUnpackAlgo2022::processCBMtime(CbmRichUnpackAlgoMicrosliceReader& reader)
{
  uint32_t word_MSB = reader.NextWord();  // CBM 63:32
  uint32_t word_LSB = reader.NextWord();  // CBM 31: 0

  fCbmTimePacket = 0;
  fCbmTimePacket = (uint64_t) word_MSB << 32 | word_LSB;
  if (fCbmTimeMS == 0) fCbmTimeMS = fCbmTimePacket;
}

// ---- processTimeDataWord ----
void CbmRichUnpackAlgo2022::processTimeDataWord(CbmRichUnpackAlgoMicrosliceReader& reader, int iTdc, uint32_t epoch,
                                                uint32_t tdcWord, uint32_t subSubEventId,
                                                std::vector<double>& raisingTime)
{
  CbmRichUnpackAlgoTdcTimeData td;
  CbmRichUnpackAlgoTdcWordReader::ProcessTimeData(tdcWord, td);
  double fullTime = calculateTime(epoch, td.fCoarse, td.fFine);


  if (td.fChannel == 0) {
    if (td.IsRisingEdge()) {
      if (isLog())
        LOG(debug4) << getLogHeader(reader) << "SubSubEv[" << iTdc << "] " << td.ToString()
                    << " CH0 Last:" << std::setprecision(15) << fullTime
                    << " PrevLast:" << fPrevLastCh0ReTime[fCurrentSubSubEvent]
                    << " diff:" << fullTime - fPrevLastCh0ReTime[fCurrentSubSubEvent];
    }
  }
  else {
    double dT            = fullTime - fPrevLastCh0ReTime[fCurrentSubSubEvent];
    double mbsCorr       = fMbsPrevTimeCh1 - fMbsPrevTimeCh0;
    double subtrigOffset = (fCbmTimePacket - fCbmTimeMS) * 25.0;  // offset of SubTrigger to MS start in ns
    double fullTimeCorr  = dT - mbsCorr + subtrigOffset;

    // if ((subSubEventId >> 12 ) == 0x7)
    //   std::cout<< dT <<"  "
    //          << fullTimeCorr <<"  "<< std::setprecision(15)
    //          << fullTime <<"  "
    //          << fPrevLastCh0ReTime[fCurrentSubSubEvent] <<"  "
    //          << mbsCorr <<"  "
    //          << subtrigOffset <<"  "
    //          << fCurrentSubSubEvent <<"  " <<std::hex
    //          << subSubEventId << std::dec <<std::endl;

    if (isLog())
      LOG(debug4) << getLogHeader(reader) << "SubSubEv[" << iTdc << "] " << td.ToString()
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
        if (isLog())
          LOG(debug4) << getLogHeader(reader) << "SubSubEv[" << iTdc << "] "
                      << "No raising channel was found before. Skip this falling edge time.";
      }
      else {
        // Matching was found, calculate ToT, if tot is in a good range -> create digi
        double ToT = fullTimeCorr - raisingTime[td.fChannel];
        if (isLog())
          LOG(debug4) << getLogHeader(reader) << "SubSubEv[" << iTdc << "] "
                      << "ToT:" << ToT;
        if (ToT >= fToTMin && ToT <= fToTMax) {
          //  if (fbMonitorMode) {
          //    TH1D* h = GetTotH1(subSubEventId, td.fChannel);
          //    if (h != nullptr) h->Fill(ToT);

          //    TH2D* h2 = GetTotH2(subSubEventId);
          //    if (h2 != nullptr) h2->Fill(td.fChannel, ToT);
          //  }
          if (fullTimeCorr >= 0.0) { writeOutputDigi(subSubEventId, td.fChannel, raisingTime[td.fChannel], ToT); }
        }
        // pair was created, set raising edge to -1.
        raisingTime[td.fChannel] = -1.;
      }
    }
  }
}

void CbmRichUnpackAlgo2022::writeOutputDigi(Int_t fpgaID, Int_t channel, Double_t time, Double_t tot)
{
  Double_t ToTcorr = fbDoToTCorr ? fUnpackPar.GetToTshift(fpgaID, channel) : 0.;
  Int_t pixelUID   = this->getPixelUID(fpgaID, channel);
  //check ordering
  Double_t finalTime = time + (Double_t) fMsRefTime - fSystemTimeOffset;

  // Do not accept digis, where the MS und TS differs by more than 6 sec (mainly TS0)
  if (6e9 < finalTime) return;

  fOutputVec.emplace_back(pixelUID, finalTime, tot - ToTcorr);
}

ClassImp(CbmRichUnpackAlgo2022)
