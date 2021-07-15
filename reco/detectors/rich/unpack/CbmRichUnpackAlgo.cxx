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
  LOG(info) << "HelloWorld " << temppath;
  fParContVec.emplace_back(std::make_pair(temppath, std::make_shared<CbmMcbm2018RichPar>()));

  return &fParContVec;
}

// ---- calculateTime
double CbmRichUnpackAlgo::calculateTime(uint32_t epoch, uint32_t coarse, uint32_t fine)
{
  return ((double) epoch) * 2048. * 5. + ((double) coarse) * 5. - ((double) fine) * 0.005;
}

// ---- getLogHeader
std::string CbmRichUnpackAlgo::getLogHeader(CbmMcbm2018RichMicrosliceReader& reader)
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

  CbmMcbm2018RichMicrosliceReader reader;
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

void CbmRichUnpackAlgo::processTrbPacket(CbmMcbm2018RichMicrosliceReader& reader)
{
  processMbs(reader, false);  // Current MBS
  processMbs(reader, true);   // Previous MBS

  uint32_t trbNum = reader.NextWord();  // TRB trigger number
  if (isLog()) LOG(debug4) << getLogHeader(reader) << "TRB Num:" << reader.GetWordAsHexString(trbNum);

  processHubBlock(reader);
}

void CbmRichUnpackAlgo::processMbs(CbmMcbm2018RichMicrosliceReader& reader, bool isPrev)
{
  uint32_t word     = reader.NextWord();
  uint32_t mbsNum   = word & 0xffffff;      //24 bits
  uint32_t nofCtsCh = (word >> 24) & 0xff;  // 8 bits
  if (isLog())
    LOG(debug4) << getLogHeader(reader) << "MBS mbsNum:0x" << std::hex << mbsNum << std::dec
                << " nofCtsCh:" << nofCtsCh;

  for (uint32_t i = 0; i < nofCtsCh; i++) {
    uint32_t wordEpoch = reader.NextWord();
    uint32_t epoch     = CbmMcbm2018RichTdcWordReader::ProcessEpoch(wordEpoch);
    if (isLog()) LOG(debug4) << getLogHeader(reader) << "MBS ch:" << i << " epoch:" << epoch;

    uint32_t wordTime = reader.NextWord();
    CbmMcbm2018RichTdcTimeData td;
    CbmMcbm2018RichTdcWordReader::ProcessTimeData(wordTime, td);
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
void CbmRichUnpackAlgo::processTimeDataWord(CbmMcbm2018RichMicrosliceReader& reader, int iTdc, uint32_t epoch,
                                            uint32_t tdcWord, uint32_t subSubEventId, std::vector<double>& raisingTime)
{
  CbmMcbm2018RichTdcTimeData td;
  CbmMcbm2018RichTdcWordReader::ProcessTimeData(tdcWord, td);
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
