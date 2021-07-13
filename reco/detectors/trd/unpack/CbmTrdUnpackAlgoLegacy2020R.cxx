/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

#include "CbmTrdUnpackAlgoLegacy2020R.h"

#include "CbmTrdDigi.h"
#include "CbmTrdParManager.h"
#include "CbmTrdParSetAsic.h"
#include "CbmTrdRawMessageSpadic.h"

#include <FairTask.h>
#include <Logger.h>

#include <RtypesCore.h>
#include <typeinfo>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

CbmTrdUnpackAlgoLegacy2020R::CbmTrdUnpackAlgoLegacy2020R(/* args */)
  : CbmTrdUnpackAlgoBaseR("CbmTrdUnpackAlgoLegacy2020R")
{
}

CbmTrdUnpackAlgoLegacy2020R::~CbmTrdUnpackAlgoLegacy2020R() {}

// ---- GetParContainerRequest ----
std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>*
CbmTrdUnpackAlgoLegacy2020R::GetParContainerRequest(std::string geoTag, std::uint32_t runId)
{
  // Basepath for default Trd parameter sets (those connected to a geoTag)
  std::string basepath = Form("%s/trd_%s", fParFilesBasePath.data(), geoTag.data());
  std::string temppath = "";

  // Digest the runId information in case of runId = 0 we use the default fall back
  std::string runpath = "";
  if (runId != 0) { runpath = ".run" + std::to_string(runId); }

  // Get .asic parameter container
  temppath = basepath + runpath + ".asic" + ".par";
  fParContVec.emplace_back(std::make_pair(temppath, std::make_shared<CbmTrdParSetAsic>()));

  // Currently we need the ParSetDigi only for the monitor
  if (fMonitor) {
    temppath  = basepath + runpath + ".digi" + ".par";
    auto pair = std::make_pair(temppath, std::make_shared<CbmTrdParSetDigi>());
    fParContVec.emplace_back(pair);
  }


  // We do not want to require the timeshift pars, since the unpacker has to be run without them to extract them. But if it is available we want to use it.
  // Get timeshift correction parameters, currently only available for run 831
  if (runId == 831 && geoTag == "mcbm_beam_2020_03") {
    temppath = Form("%s/mcbm2020_special/CbmMcbm2020TrdTshiftPar_run%d.par", fParFilesBasePath.data(), runId);
    fParContVec.emplace_back(std::make_pair(temppath, std::make_shared<CbmMcbm2020TrdTshiftPar>()));
  }


  return &fParContVec;
}

// ---- extractSample ----
std::int16_t CbmTrdUnpackAlgoLegacy2020R::extractSample(const size_t word, const Spadic::MsMessageType msgType,
                                                        std::uint32_t isample, bool multihit)
{

  switch (msgType) {
    case Spadic::MsMessageType::kSOM: {
      if (isample > 2) {
        LOG(error) << fName << "::ExtractSample] SOM MSG - Idx " << isample << " Wrong sample index!";
        return -256;
      }
      break;
    }
    case Spadic::MsMessageType::kRDA: {
      if (isample < 3 || isample > 31) {
        LOG(error) << fName << "::ExtractSample] RDA MSG - Idx " << isample << " Wrong sample index!";
        return -256;
      }
      break;
    }
    default:
      LOG(error) << fName << "::ExtractSample]  Wrong Message Type!";
      return -256;
      break;
  }

  size_t mask = 0x1FF;
  auto index  = fExtractSampleIndicesVec.at(isample);

  mask        = mask << (9 * (6 - index));
  size_t temp = word & mask;
  temp        = temp >> (6 - index) * 9;
  if (fSpadic->GetUseBaselineAvg() && (isample == 0) && !(multihit)) {
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

// ---- extractEpoch ----
size_t CbmTrdUnpackAlgoLegacy2020R::extractEpoch(const size_t word)
{
  size_t mask = 0x3FFFFFFF;
  mask        = mask << 32;
  return ((word & mask) >> 32);
}

// ---- finishDerived ----
void CbmTrdUnpackAlgoLegacy2020R::finishDerived()
{
  LOG(info) << fName << fNrWildRda << " unexpected RDA words and\n " << fNrUnknownWords << " unknown words.";
}


// ---- getInfoType ----
Spadic::MsInfoType CbmTrdUnpackAlgoLegacy2020R::getInfoType(const size_t msg)
{
  size_t mask = 0x000FFFFF;

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
    LOG(error) << fName << "::GetInfoType] unknown type!";
    return Spadic::MsInfoType::kMSB;
  }
}


// ---- getMessageType ----
Spadic::MsMessageType CbmTrdUnpackAlgoLegacy2020R::getMessageType(const size_t msg)
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

// ---- getNrOfRdaWords ----
std::uint8_t CbmTrdUnpackAlgoLegacy2020R::getNrOfRdaWords(std::uint8_t nsamples)
{
  auto nwords = (nsamples + 3) / 7;
  return nwords > 5 ? 5 : nwords;
}

// ---- makeRaw ----
CbmTrdRawMessageSpadic CbmTrdUnpackAlgoLegacy2020R::makeRaw(const size_t word, fles::MicrosliceDescriptor msDesc)
{

  ///Extract Metadata
  uint8_t elinkId = 0, chId = 0, crobId = 0;
  uint16_t criId = msDesc.eq_id;
  // char crobId = msDesc.crob_id; // TODO this needs to be implemented into microslice! - PR 03.2020
  uint8_t hitType = 0, nSamples = 0;
  bool multihit      = false;
  uint16_t timestamp = 0;
  size_t mask        = 0x3F;
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
  size_t fulltime = timestamp + (fEpoch * fMsLengthCC);  // this is in units of clock cycles


  // put the first 3 samples, contained in som, into the message.
  std::vector<std::int16_t> samples = {};
  for (std::uint32_t isample = 0; isample < nSamples && isample < 3; isample++) {
    auto adcvalue = extractSample(word, Spadic::MsMessageType::kSOM, isample, multihit);
    samples.emplace_back(adcvalue);
  }

  // Create message
  CbmTrdRawMessageSpadic retval(chId, elinkId, crobId, criId, hitType, nSamples, multihit, fulltime, samples);
  return retval;
}

void CbmTrdUnpackAlgoLegacy2020R::makeDigi(CbmTrdRawMessageSpadic raw)
{

  // Extract the trigger type and translate it to the digi enum
  auto rawTriggerType = static_cast<Spadic::eTriggerType>(raw.GetHitType());
  auto triggerType    = CbmTrdRawToDigiBaseR::GetDigiTriggerType(rawTriggerType);

  // Get the digi error class (dummy for the time being)
  Int_t errClass = 0;

  // Get the address of the originating spadic
  size_t spadicHwAddress(0);
  spadicHwAddress = (raw.GetElinkId()) + (CbmTrdParAsic::kCriIdPosition * raw.GetCriId())
                    + (CbmTrdParAsic::kCrobIdPosition * raw.GetCrobId());
  Int_t asicAddress(0);
  auto mapIt = fSpadicAddressMap.find(spadicHwAddress);  // check if asic exists
  if (mapIt == fSpadicAddressMap.end()) {
    LOG(debug4) << fName
                << "::makeDigi - No asic address "
                   "found for Spadic hardware address %lu"
                << spadicHwAddress;
    return;
  }
  asicAddress = mapIt->second;

  // Get the unique module id from the asic address
  Int_t uniqueModuleId = asicAddress / 1000;

  // GetChannelId per eLink add NSPADICCH / 2 to the second(first) eLink in the case we start with odd(even) eLinks, since, our mapping is based on odd eLinks
  auto asicChannelId = (raw.GetElinkId() % 2) == fIsFirstChannelsElinkEven
                         ? raw.GetChannelId()
                         : raw.GetChannelId() + (CbmTrdSpadic::GetNrChannels() / 2);


  auto padChNr = (fAsicChannelMap.find(asicAddress))->second.at(asicChannelId);

  // Store the full time information to last full-time member for error message handling
  fLastFulltime = raw.GetFullTime();

  // Get the time information and apply the necessary correction
  ULong64_t time = raw.GetTime();
  if (fTimeshiftsParVec) {
    time = time - fTimeshiftsParVec->at(padChNr);
    raw.SetTime(time);
  }
  time -= fSystemTimeoffset;

  auto digi = fRTDMethod->MakeDigi(raw.GetSamples(), padChNr, uniqueModuleId, time, triggerType, errClass);

  // Digest the output, i.e. write to return vector and optional pass to the monitor
  digestOutput(std::move(digi), raw);
}

// ---- setDerivedTsParameters ----
bool CbmTrdUnpackAlgoLegacy2020R::setDerivedTsParameters(size_t itimeslice)
{
  auto timeshiftpair = fTimeshiftsMap.find(itimeslice);
  if (timeshiftpair != fTimeshiftsMap.end()) { fTimeshiftsParVec = &timeshiftpair->second; }
  else
    return false;
  return true;
}

// ---- unpack ----
bool CbmTrdUnpackAlgoLegacy2020R::unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice)
{


  bool unpackOk = true;

  auto msdesc = ts->descriptor(icomp, imslice);

  // Get the µslice size in bytes to calculate the number of completed words
  auto mssize = msdesc.size;

  // Get the number of complete words in the input MS buffer.
  std::uint32_t msNrWords = mssize / fBytesPerWord;

  const auto mspointer = ts->content(icomp, imslice);
  const auto mscontent = reinterpret_cast<const size_t*>(mspointer);

  // Loop over all 64bit-Spadic-Words in the current µslice
  for (std::uint32_t iword = 0; iword < msNrWords; iword++) {
    // Access the actual word from the pointer
    auto word = static_cast<size_t>(mscontent[iword]);

    // Get the type of the word
    auto kWordtype = getMessageType(word);

    switch (kWordtype) {
      case Spadic::MsMessageType::kSOM: {
        CbmTrdRawMessageSpadic raw = makeRaw(word, msdesc);
        auto nsamples              = raw.GetNrSamples();
        /// If the new Message has more than 3 Samples, we need to read in the next words that contain the remaining samples.
        if (nsamples > 3) {
          std::uint32_t isample = 3;  // first 3 samples are already handled in som
          /// Loop over the rda words
          for (uint8_t irda = 0; irda < getNrOfRdaWords(nsamples); irda++) {
            ++iword;
            word              = static_cast<size_t>(mscontent[iword]);
            auto kAddWordtype = getMessageType(word);
            if (kAddWordtype != Spadic::MsMessageType::kRDA) {
              LOG(error) << fName
                         << "::unpack()  Incomplete Spadic "
                            "Message! RDA Word missing, Microslice corrupted.";
              return kFALSE;
            }
            /// Loop over Samples. There are max 7 samples per word.
            for (std::uint32_t iwordsample = 0; isample < nsamples && iwordsample < 7; isample++, iwordsample++) {
              auto adcvalue = extractSample(word, kAddWordtype, isample);
              raw.SetSample(adcvalue, isample);
            }
          }
        }
        ++fNrCreatedRawMsgs;

        // Message is completed here. Now lets Generate the digi and save raw message if needed.
        makeDigi(raw);

        break;
      }
      case Spadic::MsMessageType::kRDA: {
        // All rda words should be handled inside the kSOM case. Hence, this case should not appear and we have to count it as an error appearance.
        LOG(error) << fName
                   << "::unpack()  Unexpected RDA "
                      "Word. Microslice corrupted.";
        fNrWildRda++;
        return false;
        break;
      }
      case Spadic::MsMessageType::kINF: {
        /// Save info message if needed.
        if (fOptOutBVec) { fOptOutBVec->emplace_back(std::make_pair(fLastFulltime, word)); }
        fNrCreatedInfoMsgs++;
        // TODO here we also want to call the monitoring class
        // Spadic::MsInfoType infoType = getInfoType(word);
        // "Spadic_Info_Types";
        // if (fIsActiveHistoVec[kSpadic_Info_Types]) {
        //   ((TH2I*) fHistoArray.At(kSpadic_Info_Types))->Fill(fLastFulltime, (Int_t) infoType);
        // }
        break;
      }
      case Spadic::MsMessageType::kNUL: {
        if (iword != (msNrWords - 1))  // last word in Microslice is 0.
        {
          LOG(error) << fName
                     << "::unpack()  Null Word but "
                        "not at end of Microslice.";
        }
        break;
      }
      case Spadic::MsMessageType::kUNK: {
        LOG(error) << fName
                   << "::unpack()  Unknown Word. "
                      "Microslice corrupted.";
        ++fNrUnknownWords;
        return false;
        break;
      }
      case Spadic::MsMessageType::kEPO: {
        fEpoch = extractEpoch(word);
        fNrEpochMsgs++;
        break;
      }
      default:
        // We have varying msg types for different versions of the message format. Hence, to not produce compiler warnings we have a "default break;" here.
        break;
    }
  }
  return unpackOk;
}

ClassImp(CbmTrdUnpackAlgoLegacy2020R)
