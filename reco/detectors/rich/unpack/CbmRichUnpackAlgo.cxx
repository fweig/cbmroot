/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

#include "CbmRichUnpackAlgo.h"

#include <FairParGenericSet.h>
#include <FairTask.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>


CbmRichUnpackAlgo::CbmRichUnpackAlgo() : CbmRecoUnpackAlgo(fgkFlesSubsystemIdTrdR, "CbmRichUnpackAlgo") {}

CbmRichUnpackAlgo::~CbmRichUnpackAlgo() {}

// ---- GetParContainerRequest ----
std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>*
  CbmRichUnpackAlgo::GetParContainerRequest(std::string /*geoTag*/, std::uint32_t /*runId*/)
{
  // Basepath for default Trd parameter sets (those connected to a geoTag)
  std::string basepath = Form("%s", fParFilesBasePath.data());
  std::string temppath = "";

  // // Get parameter container
  temppath = basepath + "mRichPar.par";

  fParContVec.emplace_back(std::make_pair(temppath, std::make_shared<CbmMcbm2018RichPar>()));

  return &fParContVec;
}

// ---- calculateTime
double CbmRichUnpackAlgo::calculateTime(uint32_t epoch, uint32_t coarse, uint32_t fine)
{
  return ((double) epoch) * 2048. * 5. + ((double) coarse) * 5. - ((double) fine) * 0.005;
}

// ---- getLogHeader
std::string CbmRichUnpackAlgo::getLogHeader(CbmRichUnpackAlgoMicrosliceReader& reader)
{
  std::stringstream stream;
  stream << "[" << fNrProcessedTs << "-" << reader.GetWordCounter() << "/" << reader.GetSize() / 4 << " "
         << reader.GetWordAsHexString(reader.GetCurWord()) << "] ";
  return stream.str();
}

// ---- init
Bool_t CbmRichUnpackAlgo::init() { return kTRUE; }

// ---- initParSet(FairParGenericSet* parset) ----
Bool_t CbmRichUnpackAlgo::initParSet(FairParGenericSet* parset)
{
  LOG(info) << fName << "::initParSet - for container " << parset->ClassName();
  if (parset->IsA() == CbmMcbm2018RichPar::Class()) return initParSet(static_cast<CbmMcbm2018RichPar*>(parset));

  // If we do not know the derived ParSet class we return false
  LOG(error) << fName << "::initParSet - for container " << parset->ClassName() << " failed, since" << fName
             << "::initParSet() does not know the derived ParSet and what to do with it!";
  return kFALSE;
}

// ---- initParSet(CbmTrdParSetAsic* parset) ----
Bool_t CbmRichUnpackAlgo::initParSet(CbmMcbm2018RichPar* parset)
{
  LOG(debug) << fName << "::initParSetAsic - ";
  fUnpackPar = *parset;

  LOG(info) << fName << "::initParSetRichMcbm2018 - Successfully initialized Spadic hardware address map";

  return kTRUE;
}

// ---- isLog ----
bool CbmRichUnpackAlgo::isLog()
{
  //if (fTsCounter == 25215) return true;
  return false;
}

// ---- unpack
bool CbmRichUnpackAlgo::unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice)
{

  const fles::MicrosliceView mv            = ts->get_microslice(icomp, imslice);
  const fles::MicrosliceDescriptor& msDesc = mv.desc();

  CbmRichUnpackAlgoMicrosliceReader reader;
  reader.SetData(mv.content(), msDesc.size);

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


  return true;
}

void CbmRichUnpackAlgo::processHubBlock(CbmRichUnpackAlgoMicrosliceReader& reader)
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
  }

  // read last words
  int lastWordsCounter = 0;
  while (true) {
    lastWordsCounter++;
    word = reader.NextWord();
    if (isLog()) LOG(debug4) << getLogHeader(reader);
    if (word == 0x600dda7a) break;
    if (lastWordsCounter >= 7) {
      LOG(error) << getLogHeader(reader)
                 << "CbmMcbm2018UnpackerAlgoRich::ProcessHubBlock() ERROR: No word == 0x600dda7a";
    }
  }
}

void CbmRichUnpackAlgo::processCtsSubSubEvent(CbmRichUnpackAlgoMicrosliceReader& reader, uint32_t subSubEventSize,
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

void CbmRichUnpackAlgo::processSubSubEvent(CbmRichUnpackAlgoMicrosliceReader& reader, int nofTimeWords,
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
    uint32_t word                     = reader.NextWord();
    CbmRichUnpackAlgoTdcWordType type = CbmRichUnpackAlgoTdcWordReader::GetTdcWordType(word);

    if (type == CbmRichUnpackAlgoTdcWordType::TimeData) {
      if (!wasHeader || !wasEpoch || wasTrailer) {
        LOG(error) << getLogHeader(reader) << "illegal position of TDC Time (before header/epoch or after trailer)";
        errorInData = true;
        continue;
      }
      wasTime = true;
      processTimeDataWord(reader, i, epoch, word, subSubEventId, raisingTime);
    }
    else if (type == CbmRichUnpackAlgoTdcWordType::Epoch) {
      if (!wasHeader || wasTrailer) {
        LOG(error) << getLogHeader(reader) << "illegal position of TDC Epoch (before header or after trailer)";
        errorInData = true;
        continue;
      }
      wasEpoch = true;
      epoch    = CbmRichUnpackAlgoTdcWordReader::ProcessEpoch(word);
      if (isLog()) LOG(debug4) << getLogHeader(reader) << "SubSubEv[" << i << "] epoch:" << epoch;
    }
    else if (type == CbmRichUnpackAlgoTdcWordType::Header) {
      if (wasEpoch || wasTime || wasTrailer) {
        LOG(error) << getLogHeader(reader) << "illegal position of TDC Header (after time/epoch/trailer)";
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
        LOG(error) << getLogHeader(reader) << "illegal position of TDC Trailer (before time/epoch/header)";
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

void CbmRichUnpackAlgo::processTrbPacket(CbmRichUnpackAlgoMicrosliceReader& reader)
{
  processMbs(reader, false);  // Current MBS
  processMbs(reader, true);   // Previous MBS

  uint32_t trbNum = reader.NextWord();  // TRB trigger number
  if (isLog()) LOG(debug4) << getLogHeader(reader) << "TRB Num:" << reader.GetWordAsHexString(trbNum);

  processHubBlock(reader);
}

void CbmRichUnpackAlgo::processMbs(CbmRichUnpackAlgoMicrosliceReader& reader, bool isPrev)
{
  uint32_t word     = reader.NextWord();
  uint32_t mbsNum   = word & 0xffffff;      //24 bits
  uint32_t nofCtsCh = (word >> 24) & 0xff;  // 8 bits
  if (isLog())
    LOG(debug4) << getLogHeader(reader) << "MBS mbsNum:0x" << std::hex << mbsNum << std::dec
                << " nofCtsCh:" << nofCtsCh;

  for (uint32_t i = 0; i < nofCtsCh; i++) {
    uint32_t wordEpoch = reader.NextWord();
    uint32_t epoch     = CbmRichUnpackAlgoTdcWordReader::ProcessEpoch(wordEpoch);
    if (isLog()) LOG(debug4) << getLogHeader(reader) << "MBS ch:" << i << " epoch:" << epoch;

    uint32_t wordTime = reader.NextWord();
    CbmRichUnpackAlgoTdcTimeData td;
    CbmRichUnpackAlgoTdcWordReader::ProcessTimeData(wordTime, td);
    if (isLog()) LOG(debug4) << getLogHeader(reader) << "MBS ch:" << i << " " << td.ToString();

    double fullTime = calculateTime(epoch, td.fCoarse, td.fFine);

    if (isPrev && td.fChannel == 0) fMbsPrevTimeCh0 = fullTime;
    if (isPrev && td.fChannel == 1) fMbsPrevTimeCh1 = fullTime;
  }

  double mbsCorr = fMbsPrevTimeCh1 - fMbsPrevTimeCh0;
  if (isLog())
    LOG(debug4) << getLogHeader(reader) << "MBS Prev ch1:" << std::setprecision(15) << fMbsPrevTimeCh1
                << " ch0:" << fMbsPrevTimeCh0 << " corr:" << mbsCorr;
}

// ---- processTimeDataWord ----
void CbmRichUnpackAlgo::processTimeDataWord(CbmRichUnpackAlgoMicrosliceReader& reader, int iTdc, uint32_t epoch,
                                            uint32_t tdcWord, uint32_t subSubEventId, std::vector<double>& raisingTime)
{
  CbmRichUnpackAlgoTdcTimeData td;
  CbmRichUnpackAlgoTdcWordReader::ProcessTimeData(tdcWord, td);
  double fullTime = calculateTime(epoch, td.fCoarse, td.fFine);


  if (td.fChannel == 0) {
    if (td.IsRisingEdge()) {
      fPrevLastCh0ReTime[subSubEventId] = fLastCh0ReTime[subSubEventId];
      fLastCh0ReTime[subSubEventId]     = fullTime;
      if (isLog())
        LOG(debug4) << getLogHeader(reader) << "SubSubEv[" << iTdc << "] " << td.ToString()
                    << " CH0 Last:" << std::setprecision(15) << fLastCh0ReTime[subSubEventId]
                    << " PrevLast:" << fPrevLastCh0ReTime[subSubEventId]
                    << " diff:" << fLastCh0ReTime[subSubEventId] - fPrevLastCh0ReTime[subSubEventId];
    }
  }
  else {
    double dT           = fullTime - fPrevLastCh0ReTime[subSubEventId];
    double mbsCorr      = fMbsPrevTimeCh1 - fMbsPrevTimeCh0;
    double fullTimeCorr = dT - mbsCorr;
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
          writeOutputDigi(subSubEventId, td.fChannel, raisingTime[td.fChannel], ToT);
        }
        // pair was created, set raising edge to -1.
        raisingTime[td.fChannel] = -1.;
      }
    }
  }
}

void CbmRichUnpackAlgo::writeOutputDigi(Int_t fpgaID, Int_t channel, Double_t time, Double_t tot)
{
  Double_t ToTcorr = fbDoToTCorr ? fUnpackPar.GetToTshift(fpgaID, channel) : 0.;
  Int_t pixelUID   = this->getPixelUID(fpgaID, channel);
  //check ordering
  //   Double_t finalTime = time + (Double_t) msRefTS - fSystemTimeoffset;
  Double_t finalTime = time - fSystemTimeoffset - fTsStartTime;

  Double_t lastTime = 0.;

  if (fOutputVec.size() < 1) { fOutputVec.emplace_back(pixelUID, finalTime, tot - ToTcorr); }
  else {
    lastTime = fOutputVec[fOutputVec.size() - 1].GetTime();
    if (fOutputVec[0].GetTime() > finalTime) {
      fOutputVec.emplace(fOutputVec.begin(), pixelUID, finalTime, tot - ToTcorr);
    }
    else if (lastTime > finalTime) {
      for (int i = fOutputVec.size() - 1; i >= 0; i--) {
        lastTime = fOutputVec[i].GetTime();
        if (lastTime <= finalTime) {
          fOutputVec.emplace(fOutputVec.begin() + i + 1, pixelUID, finalTime, tot - ToTcorr);
          break;
        }
      }
    }
    else {
      fOutputVec.emplace_back(pixelUID, finalTime, tot - ToTcorr);
    }
  }
}

ClassImp(CbmRichUnpackAlgo)
