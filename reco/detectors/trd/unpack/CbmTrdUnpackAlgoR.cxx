/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

#include "CbmTrdUnpackAlgoR.h"

#include "CbmTrdParManager.h"
#include "CbmTrdParSetAsic.h"
#include "CbmTrdRawMessageSpadic.h"
#include "CbmTrdSpadic.h"

#include <FairTask.h>
#include <Logger.h>

#include <RtypesCore.h>
#include <typeinfo>

#include <algorithm>
#include <bitset>
#include <cstdint>
#include <memory>

CbmTrdUnpackAlgoR::CbmTrdUnpackAlgoR(/* args */) : CbmTrdUnpackAlgoBaseR("CbmTrdUnpackAlgoR") {}

CbmTrdUnpackAlgoR::~CbmTrdUnpackAlgoR() {}


// ---- GetParContainerRequest ----
std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>*
CbmTrdUnpackAlgoR::GetParContainerRequest(std::string geoTag, std::uint32_t runId)
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


  return &fParContVec;
}

// ---- digestBufInfoFlags ----
Spadic::MsInfoType CbmTrdUnpackAlgoR::digestBufInfoFlags(const std::uint32_t frame, std::uint16_t criId,
                                                         std::uint8_t crobId, std::uint16_t elinkId)
{
  auto flag = (frame >> 15) & 0x3;
  Spadic::MsInfoType infotype;
  if (flag == 1) infotype = Spadic::MsInfoType::kChannelBuf;
  if (flag == 2) infotype = Spadic::MsInfoType::kOrdFifoBuf;
  if (flag == 3) infotype = Spadic::MsInfoType::kChannelBufM;

  if (fMonitor) {
    // ModuleId is the asic address after the first 3 digits
    auto moduleid = getAsicAddress(criId, crobId, elinkId) / 1000;
    fMonitor->FillHisto(infotype, moduleid);
  }
  return infotype;
}

// ---- digestInfoMsg ----
void CbmTrdUnpackAlgoR::digestInfoMsg(const std::uint32_t frame, std::uint16_t criId, std::uint8_t crobId,
                                      std::uint16_t elinkId)
{
  /// Save info message if needed.
  if (fOptOutBVec) { fOptOutBVec->emplace_back(std::make_pair(fLastFulltime, frame)); }
  fNrCreatedInfoMsgs++;
  Spadic::MsInfoType infotype = getInfoType(frame, criId, crobId, elinkId);
  // "Spadic_Info_Types";

  if (fMonitor) {
    // ModuleId is the asic address after the first 3 digits
    auto moduleid = getAsicAddress(criId, crobId, elinkId) / 1000;
    fMonitor->FillHisto(infotype, moduleid);
  }
}

// ---- digestInfoMsg ----
void CbmTrdUnpackAlgoR::digestMsFlags(const std::uint16_t flags, std::uint16_t criId, std::uint8_t crobId)
{
  // ModuleId is the asic address after the first 3 digits
  auto moduleid = getAsicAddress(criId, crobId, 0) / 1000;

  if (flags & static_cast<std::uint16_t>(fles::MicrosliceFlags::CrcValid)) {
    fNrCrcValidFlags++;
    if (fMonitor) { fMonitor->FillHisto(fles::MicrosliceFlags::CrcValid, moduleid); }
  }
  if (flags & static_cast<std::uint16_t>(fles::MicrosliceFlags::OverflowFlim)) {
    fNrOverflowFlimFlags++;
    if (fMonitor) { fMonitor->FillHisto(fles::MicrosliceFlags::OverflowFlim, moduleid); }
  }
  if (flags & static_cast<std::uint16_t>(fles::MicrosliceFlags::OverflowUser)) {
    fNrOverflowUserFlags++;
    if (fMonitor) { fMonitor->FillHisto(fles::MicrosliceFlags::OverflowUser, moduleid); }
  }
  if (flags & static_cast<std::uint16_t>(fles::MicrosliceFlags::DataError)) {
    fNrDataErrorFlags++;
    if (fMonitor) { fMonitor->FillHisto(fles::MicrosliceFlags::DataError, moduleid); }
  }
}

// ---- extractSample ----
std::float_t CbmTrdUnpackAlgoR::extractAvgSample(size_t* adcbuffer, size_t* nadcbits)
{
  if (*nadcbits < 9)
    LOG(error) << fName << "::extractAvgSample can not extract samples from a buffer with less than 9 bits";

  *nadcbits -= 9;

  // The decoding of the average sample is kind of interesting:
  // We get 9 bits in total iiiiiiiff. The 7 "i" bits refer to std integer bits, hence,
  // covering a range of 0..128.
  // The 2 "f" bits refer to values after a fix point refering to [0,0.25,0.5,0.75].
  // The sign we have to assume to be negative (bit-9 = 1) and also bit 8 of our std
  // interger range we have to assume to be 0, such that the returned number is in
  // between -256..-128.

  // Activate the 7 "i" bits
  std::int16_t sample = 0x07f;

  // Write the content of the 7 "i" bits to temp
  sample &= (*adcbuffer >> (*nadcbits + 2));

  // Switch on the negative sign
  sample |= 0xff00;

  return sample;
}

// ---- extractSample ----
std::int16_t CbmTrdUnpackAlgoR::extractSample(size_t* adcbuffer, size_t* nadcbits)
{
  if (*nadcbits < 9)
    LOG(error) << fName << "::extractSample can not extract samples from a buffer with less than 9 bits";

  // We want to access the bits stored at the positions between nadcbits and nadcbits - 9, so we can already here reduce nadcbits by 9 and than shift the adcbuffer by this value to the right and compare it with temp which has the 9 lsbs set to 1
  *nadcbits -= 9;

  std::int16_t temp = 0x1ff;
  temp &= (*adcbuffer >> (*nadcbits));

  // Now we have our 9 bits stored in temp, but for a std::int16_t this does not match in terms of the sign handling.
  // So we check on bit 9 for the sign (temp & 0x0100) and if we have a negative value we manipulate bit 16-10 to 1 to get the correct negative number

  std::int16_t sample = (temp & 0x0100) ? (temp | 0xff00) : temp;

  return sample;
}

// ---- finishDerived ----
void CbmTrdUnpackAlgoR::finishDerived()
{
  LOG(info) << fName << " \n " << fNrWildRda << " unexpected RDA frames,\n " << fNrWildNul
            << " unexpected NUL frames, \n " << fNrWildEom << " unexpected EOM frames, \n " << fNrMissingEom
            << " missing EOM frames, \n " << fNrCorruptEom << " corrupt EOM frames, \n " << fNrElinkMis
            << " SOM to RDA/EOM eLink mismatches, \n " << fNrNonMajorTsMsb << " non-major ts_msbs and \n "
            << fNrUnknownWords << " unknown frames." << std::endl;
}

// ---- getInfoType ----
Spadic::MsInfoType CbmTrdUnpackAlgoR::getInfoType(const std::uint32_t frame, std::uint16_t criId, std::uint8_t crobId,
                                                  std::uint16_t elinkId)
{
  // Set first 20 bits to 1 for the mask
  size_t mask = 0x000FFFFF;

  // 000011.................. : BOM word
  // 0000010................. : MSB word
  // 0000011................. : BUF word
  // 0000100................. : UNU word
  // 0000101................. : MIS word

  if (((frame & mask) >> 18) == 3)  // BOM
  {
    return Spadic::MsInfoType::kBOM;
  }
  if (((frame & mask) >> 17) == 2)  // MSB
  {
    return Spadic::MsInfoType::kMSB;
  }
  if (((frame & mask) >> 17) == 3)  // BUF
  {
    digestBufInfoFlags(frame, criId, crobId, elinkId);
    return Spadic::MsInfoType::kBUF;
  }
  if (((frame & mask) >> 17) == 4)  // UNU
  {
    return Spadic::MsInfoType::kUNU;
  }
  if (((frame & mask) >> 17) == 5)  // MIS
  {
    return Spadic::MsInfoType::kMIS;
  }
  else {
    LOG(error) << fName << "::GetInfoType] unknown type!";
    return Spadic::MsInfoType::kMSB;
  }
}

// ---- getMessageType ----
Spadic::MsMessageType CbmTrdUnpackAlgoR::getMessageType(const std::uint32_t frame)
{
  std::uint32_t checkframe = frame;
  checkframe &= 0xffffff;
  if ((checkframe >> 21) == 1)  // SOM  001. ....
  {
    return Spadic::MsMessageType::kSOM;
  }
  else if ((checkframe >> 22) == 1)  // RDA  01.. ....
  {
    return Spadic::MsMessageType::kRDA;
  }
  else if ((checkframe >> 20) == 1)  // EOM  0001 ....
  {
    return Spadic::MsMessageType::kEOM;
  }
  else if ((checkframe >> 22) == 3)  // TS_MSB 11.. ....
  {
    return Spadic::MsMessageType::kEPO;
  }
  else if (0 < (checkframe >> 18) && (checkframe >> 18) <= 3) {
    return Spadic::MsMessageType::kINF;
  }
  else if (checkframe == 0)  // Last Word in a Microslice is 0
  {
    return Spadic::MsMessageType::kNUL;
  }
  else  // not a spadic message
  {
    return Spadic::MsMessageType::kUNK;
  }
}

// ---- getTsMsb ----
std::uint8_t CbmTrdUnpackAlgoR::getTsMsb(const std::uint32_t frame)
{
  if ((frame & 0xf) > 0)
    return -2;  // if a 'error' ts_msb is received the tsmsb value is not correct. To not mess up the counting -2 is returned.
  // The epoch in form of the TS_MSBs is written 3 times into the frame to allow to check for bit flips and catch errors. It has the length of 6 bits and an offset of 4
  std::uint8_t tsmsb[3];
  for (uint iepoch = 0; iepoch < 3; ++iepoch) {
    tsmsb[iepoch] = static_cast<std::uint8_t>((frame >> (4 + 6 * iepoch) & 0x3f));
  }

  // Check if the epoch at position 0 is at least compatible with one of the others. Since, we only have 3 that value is automatically the majority value.
  if (tsmsb[0] == tsmsb[1] || tsmsb[0] == tsmsb[2]) return tsmsb[0];

  // If we arrive here the epoch at position 0 is not compatible with the other two. So let's check if they are compatible with each other. If so we have again a majority epoch
  if (tsmsb[1] == tsmsb[2]) return tsmsb[1];

  LOG(debug) << fName
             << "::checkMajorityTsMsb got a vector without a majority ts_msb, so please check what is going on here!";
  ++fNrNonMajorTsMsb;

  return tsmsb[0];
}

// ---- makeDigi ----
void CbmTrdUnpackAlgoR::makeDigi(CbmTrdRawMessageSpadic raw)
{

  // Extract the trigger type and translate it to the digi enum
  auto rawTriggerType = static_cast<Spadic::eTriggerType>(raw.GetHitType());
  auto triggerType    = CbmTrdRawToDigiBaseR::GetDigiTriggerType(rawTriggerType);

  // Get the digi error class (dummy for the time being)
  Int_t errClass = 0;

  // Get the address of the originating spadic
  auto asicAddress = getAsicAddress(raw.GetCriId(), raw.GetCrobId(), raw.GetElinkId());

  // Get the unique module id from the asic address
  Int_t uniqueModuleId = asicAddress / 1000;

  // Get the channel id on the module
  auto padChNr = getChannelId(asicAddress, raw.GetElinkId(), raw.GetChannelId());

  // Store the full time information to last full-time member for error message handling
  fLastFulltime = raw.GetFullTime();

  // Get the time information and apply the necessary correction
  ULong64_t time = raw.GetTime();

  time -= fSystemTimeOffset;

  auto digi = fRTDMethod->MakeDigi(raw.GetSamples(), padChNr, uniqueModuleId, time, triggerType, errClass);

  // If the raw message was flagged as multi hit, forward this info to the digi
  if (raw.GetMultiHit()) digi->SetTriggerType(CbmTrdDigi::eTriggerType::kMulti);

  // Digest the output, i.e. write to return vector and optional pass to the monitor
  digestOutput(std::move(digi), raw);
}

// ---- makeRaw ----
CbmTrdRawMessageSpadic CbmTrdUnpackAlgoR::makeRaw(const std::uint32_t frame, std::uint16_t criId, std::uint8_t crobId,
                                                  std::uint16_t elinkId, std::uint8_t istream)
{

  auto chId             = static_cast<std::uint8_t>(((frame >> 17) & 0xf));
  auto timestamp        = static_cast<std::uint8_t>((frame >> 9) & 0xff);
  bool multihit         = ((frame >> 8) & 0x1);
  auto hitType          = static_cast<std::uint8_t>((frame >> 6) & 0x3);
  std::uint8_t nsamples = 0;
  // We directly start with the largest possible samples vector to only init it once
  std::vector<std::int16_t> samples = std::vector<std::int16_t>(0);

  uint64_t fulltime = fMsStartTimeRelCC + (fNrTsMsbVec.at(istream) * fTsMsbLengthCC) + timestamp;


  // Create message
  CbmTrdRawMessageSpadic retval(chId, elinkId, crobId, criId, hitType, nsamples, multihit, fulltime, samples);
  return retval;
}


// ---- unpack ----
bool CbmTrdUnpackAlgoR::unpack(const fles::Timeslice* ts, std::uint16_t icomp, UInt_t imslice)
{
  bool unpackOk = true;

  if (fMonitor) fMonitor->SetCurrentTimesliceStartTime(fTsStartTime);

  auto msdesc = ts->descriptor(icomp, imslice);

  // Get the µSlice starttime relative to the timeslice starttime.
  // The UTC is already to large for storing it CbmTrdRawMessageSpadic due to a cast, caused by the multiplication with a double used in the raw message
  fMsStartTimeRelCC = (msdesc.idx - fTsStartTime) / fSpadic->GetClockCycle();

  // We only want to count on TS_MSB per Stream per TS_MSB package (each eLink sends its own TS_MSB frame) so we store the current TS_MSB and compare it to the incoming.
  std::int8_t currTsMsb = 0;

  // Reset the TS_MSB counter for the new µSlice we unpack
  fNrTsMsbVec.clear();
  fNrTsMsbVec.resize(fStreamsPerWord);


  // Get the µslice size in bytes to calculate the number of completed words
  auto mssize = msdesc.size;


  // Get the hardware ids from which the current µSlice is coming
  std::uint8_t crobId = 0;
  auto criId          = msdesc.eq_id;

  // Digest the flags from the µSlice
  digestMsFlags(msdesc.flags, criId, crobId);

  // Get the number of complete words in the input MS buffer.
  std::uint32_t nwords = mssize / fBytesPerWord;

  const auto mspointer = ts->content(icomp, imslice);

  // We have 32 bit spadic frames in this readout version
  const auto mscontent = reinterpret_cast<const size_t*>(mspointer);

  // Loop over all 64bit-Spadic-Words in the current µslice
  for (std::uint32_t istream = 0; istream < fStreamsPerWord; istream++) {
    currTsMsb = -1;
    for (std::uint32_t iword = 0; iword < nwords; ++iword) {
      // Access the actual word from the pointer
      size_t word = static_cast<size_t>(mscontent[iword]);

      // Access the actual frame[iframe] from the word. (see fStreamsPerWord)
      std::uint32_t frame = (word >> (32 * istream)) & 0xffffffff;

      // Get the type of the frame
      auto kWordtype = getMessageType(frame);

      // In case we saw any other word than an EPO(TS_MSB) reset the flag, such that we again increase by one if an EPO frame arrives
      auto elinkId = (frame >> 24) & 0x3f;

      switch (kWordtype) {
        case Spadic::MsMessageType::kEPO: {
          auto tsmsb = getTsMsb(frame);
          if (((tsmsb - currTsMsb) & 0x3f) == 1 || currTsMsb == -1) fNrTsMsbVec.at(istream)++;
          currTsMsb = tsmsb;
          fNrEpochMsgs++;
          break;
          // FIXME in the kEPO msg we also have further flags that should be extracted
        }
        case Spadic::MsMessageType::kSOM: {
          // Create the raw message and fill it with all information we can get from the SOM msg
          CbmTrdRawMessageSpadic raw = makeRaw(frame, criId, crobId, elinkId, istream);

          // FIXME since we can not deduce the sample position from the messages we need in future some parameter handling here to place the samples at the correct position
          // 6 adc bits are stored in the som message
          size_t nadcbits      = 6;
          size_t nadcbitstotal = 6;
          // Get the first bits from the adc signal
          size_t adcbuffer = frame & 0x3f;
          size_t isample   = 0;
          size_t irda      = 0;

          // Now lets check if we have rda words following our som
          iword++;
          word  = static_cast<size_t>(mscontent[iword]);
          frame = (word >> (32 * istream)) & 0xffffffff;

          // The maximum amount of samples (32) equals to 12 RDA messages
          while (getMessageType(frame) == Spadic::MsMessageType::kRDA && irda < 12) {
            // We have to count the number of rda frames for sample reconstruction in eom
            irda++;

            // Ensure that we are on the correct eLink
            elinkId = (frame >> 24) & 0x3f;
            if (elinkId != raw.GetElinkId()) {
              fNrElinkMis++;
              LOG(debug) << fName << "::unpack() SOM eLinkId(" << static_cast<std::uint16_t>(raw.GetElinkId())
                         << ") does not match the RDA eLinkId(" << elinkId << ")";
            }

            // We have 22 adc bits per RDA word lets add them to the buffer...
            adcbuffer <<= 22;
            adcbuffer |= static_cast<size_t>((frame & 0x3fffff));
            // and increase the adcbit counter by 22 bits
            nadcbits += 22;
            nadcbitstotal += 22;
            // If we have 9 or more samples stored we can extract n samples
            while (nadcbits >= 9) {
              raw.IncNrSamples();
              // In case the avg baseline feature was used we need to take special care of sample 0
              if (isample == 0 && fSpadic->GetUseBaselineAvg())
                raw.SetSample(extractAvgSample(&adcbuffer, &nadcbits), isample);
              else
                raw.SetSample(extractSample(&adcbuffer, &nadcbits), isample);
              isample++;
            }
            iword++;
            word  = static_cast<size_t>(mscontent[iword]);
            frame = (word >> (32 * istream)) & 0xffffffff;
          }

          if (getMessageType(frame) == Spadic::MsMessageType::kEOM) {
            // Ensure that we are on the correct eLink
            elinkId = (frame >> 24) & 0x3f;
            if (elinkId != raw.GetElinkId()) {
              fNrElinkMis++;
              LOG(debug) << fName << "::unpack() SOM eLinkId(" << static_cast<std::uint16_t>(raw.GetElinkId())
                         << ") does not match the RDA eLinkId(" << elinkId << ")";
            }

            // Number of samples indicator = nsamples % 4
            std::uint8_t nsamplesindicator = (frame >> 18) & 0x3;
            // Number of required samples as indicated
            std::uint64_t nreqsamples = (nadcbitstotal + 18) / 9;
            std::uint8_t nn           = nreqsamples % 4;
            for (std::uint8_t itest = 0; itest < 3; itest++) {
              if (nn == nsamplesindicator || nreqsamples == 0) break;
              nreqsamples--;
              nn = nreqsamples % 4;
            }

            // There is a chance that the nsamplesindicator bits are corrupted, here we check that we do not try to extract more adcbits than actually are streamed
            if (nreqsamples >= isample) {
              // Now extract from the above values the number of required adc bits from the eom
              std::int8_t nrequiredbits = (nreqsamples - isample) * 9 - nadcbits;
              adcbuffer <<= nrequiredbits;

              // The eom carries at maximum 18 adcbits
              adcbuffer |= static_cast<size_t>((frame & 0x3ffff) >> (18 - nrequiredbits));
              nadcbits += nrequiredbits;

              while (nadcbits >= 9) {
                raw.IncNrSamples();
                raw.SetSample(extractSample(&adcbuffer, &nadcbits), isample);
                isample++;
              }
            }
            else {
              ++fNrCorruptEom;
            }
            ++fNrCreatedRawMsgs;

            // the message is done and the raw message container should contain everything we need. So now we can call makeDigi(). Nevertheless there is a chance for a corrupted message, which ends up with 0 samples so we have to check for it.
            if (isample > 0) makeDigi(raw);
          }
          else {
            // We move the word counter backwards by one, such that the unexpected message can correctly be digested
            iword--;
            ++fNrMissingEom;
            LOG(debug)
              << fName
              << "::unpack() We had a SOM message and hence, desparately need an EOM message but none came. This "
                 "is quite mysterious and a problem!";
          }
          break;
        }
        case Spadic::MsMessageType::kRDA: {
          LOG(debug) << fName << "::unpack() Unexpected wild RDA word";
          fNrWildRda++;
          break;
        }
        case Spadic::MsMessageType::kEOM: {
          LOG(debug) << fName << "::unpack() Unexpected wild EOM word";
          fNrWildEom++;
          break;
        }
        case Spadic::MsMessageType::kINF: {
          digestInfoMsg(frame, criId, crobId, elinkId);
          break;
        }
        case Spadic::MsMessageType::kNUL: {
          if (iword != (nwords - 1) || (istream != (fStreamsPerWord - 1)))
          // last word in Microslice is 0.
          {
            LOG(debug) << fName
                       << "::unpack()  Null Word but "
                          "not at end of Microslice.";
            fNrWildNul++;
          }
          break;
        }
        case Spadic::MsMessageType::kUNK: {
          LOG(debug) << fName
                     << "::unpack()  Unknown Word. "
                        "Microslice corrupted.";
          ++fNrUnknownWords;
          return false;
          break;
        }
        default:
          // We have varying msg types for different versions of the message format. Hence, to not produce compiler warnings we have a "default break;" here.
          break;
      }
    }
  }
  return unpackOk;
}

ClassImp(CbmTrdUnpackAlgoR)
