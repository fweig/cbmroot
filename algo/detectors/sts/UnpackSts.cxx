/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Volker Friese [committer] */

#include "UnpackSts.h"

#include <cassert>
#include <utility>
#include <vector>

#include <cmath>

#include "StsXyterMessage.h"

using std::unique_ptr;
using std::vector;

namespace cbm::algo
{

  // ----   Algorithm execution   ---------------------------------------------
  UnpackSts::resultType UnpackSts::operator()(const uint8_t* msContent, const fles::MicrosliceDescriptor& msDescr,
                                              const uint64_t tTimeslice)
  {

    // --- Output data
    resultType result = {};

    // --- Current Timeslice time and TS_MSB epoch cycle
    fCurrentTsTime = tTimeslice;
    auto msTime    = msDescr.idx;  // Unix time of MS in ns
    fCurrentCycle  = std::ldiv(msTime, fkCycleLength).quot;

    // --- Number of messages in microslice
    auto msSize = msDescr.size;
    if (msSize % sizeof(stsxyter::Message) != 0) {
      result.second.fNumErrInvalidMsSize++;
      return result;
    }
    const uint32_t numMessages = msSize / sizeof(stsxyter::Message);
    if (numMessages < 2) {
      result.second.fNumErrInvalidMsSize++;
      return result;
    }

    // --- Interpret MS content as sequence of SMX messages
    auto message = reinterpret_cast<const stsxyter::Message*>(msContent);

    // --- The first message in the MS is expected to be of type EPOCH and can be ignored
    if (message[0].GetMessType() != stsxyter::MessType::Epoch) {
      result.second.fNumErrInvalidFirstMessage++;
      return result;
    }

    // --- The second message must be of type ts_msb
    if (message[1].GetMessType() != stsxyter::MessType::TsMsb) {
      result.second.fNumErrInvalidFirstMessage++;
      return result;
    }
    ProcessTsmsbMessage(message[1], result.second);

    // --- Message loop
    for (uint32_t messageNr = 2; messageNr < numMessages; messageNr++) {

      // --- Action depending on message type
      switch (message[messageNr].GetMessType()) {

        case stsxyter::MessType::Hit: {
          ProcessHitMessage(message[messageNr], result.first, result.second);
          break;
        }
        case stsxyter::MessType::TsMsb: {
          ProcessTsmsbMessage(message[messageNr], result.second);
          break;
        }
        default: {
          result.second.fNumNonHitOrTsbMessage++;
          break;
        }

      }  //? Message type

    }  //# Messages

    return result;
  }
  // --------------------------------------------------------------------------


  // -----   Process hit message   --------------------------------------------
  inline void UnpackSts::ProcessHitMessage(const stsxyter::Message& message, vector<CbmStsDigi>& digiVec,
                                           UnpackStsMonitorData& monitor) const
  {

    // --- Check eLink and get parameters
    uint16_t elink = message.GetLinkIndexHitBinning();
    if (elink >= fParams.fElinkParams.size()) {
      monitor.fNumErrElinkOutOfRange++;
      return;
    }
    const UnpackStsElinkPar& elinkPar = fParams.fElinkParams.at(elink);
    uint32_t asicNr                   = elinkPar.fAsicNr;

    // --- Hardware-to-software address
    uint32_t numChansPerModule = fParams.fNumAsicsPerModule * fParams.fNumChansPerAsic;
    uint32_t address           = elinkPar.fAddress;
    uint32_t channel           = 0;
    if (asicNr < fParams.fNumAsicsPerModule / 2) {  // front side (n side)
      channel = message.GetHitChannel() + fParams.fNumChansPerAsic * asicNr;
    }
    else {  // back side (p side)
      channel = numChansPerModule - message.GetHitChannel() + 1;
    }

    // --- Convert time stamp from clock cycles to ns
    uint64_t messageTime = (message.GetHitTimeBinning() * fkClockCycleNom + fkClockCycleDen / 2) / fkClockCycleDen;
    // --- Expand to time within timeslice
    messageTime += fCurrentEpochTime;
    // --- Correct ASIC-wise offsets
    messageTime -= elinkPar.fTimeOffset;
    // --- TODO: Add walk correction (depends on ADC)

    // --- Charge
    double charge = elinkPar.fAdcOffset + (message.GetHitAdc() - 1) * elinkPar.fAdcGain;

    // --- Create output digi
    digiVec.emplace_back(address, channel, messageTime, charge);
  }
  // --------------------------------------------------------------------------


  // -----   Process an epoch (TS_MSB) message   ------------------------------
  inline void UnpackSts::ProcessTsmsbMessage(const stsxyter::Message& message, UnpackStsMonitorData& monitor)
  {

    auto epoch = message.GetTsMsbValBinning();

    // --- Cycle wrap
    if (epoch < fCurrentEpoch) fCurrentCycle++;

    // --- Update current epoch
    fCurrentEpoch          = epoch;
    uint64_t epochTimeInCc = (fCurrentCycle * fkEpochsPerCycle + epoch) * fkEpochLength;
    if (epochTimeInCc >> 59 != 0) {  // avoid overflow in 64 bit // TODO: Hard-coded number!
      monitor.fNumErrTimestampOverflow++;
      return;
    }

    // --- Calculate epoch time in ns relative to timeslice start time
    fCurrentEpochTime = (epochTimeInCc * fkClockCycleNom + fkClockCycleDen / 2) / fkClockCycleDen;
    fCurrentEpochTime -= fCurrentTsTime;
  }
  // --------------------------------------------------------------------------


} /* namespace cbm::algo */
