/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "UnpackBmon.h"

#include <cassert>
#include <sstream>
#include <utility>
#include <vector>

#include <cmath>

using std::unique_ptr;
using std::vector;

namespace cbm::algo
{

  // ----   Algorithm execution   ---------------------------------------------
  UnpackBmon::resultType UnpackBmon::operator()(const uint8_t* msContent, const fles::MicrosliceDescriptor& msDescr,
                                                const uint64_t tTimeslice)
  {

    // --- Output data
    resultType result = {};

    // --- Current Timeslice start time in epoch units. Note that it is always a multiple of epochs
    // --- and the epoch is a multiple of ns.
    fCurrentTsTime = static_cast<uint64_t>(tTimeslice / critof001::kuEpochInNs) % critof001::kulEpochCycleEp;

    // --- Number of messages in microslice
    auto msSize = msDescr.size;
    if (msSize % sizeof(critof001::Message) != 0) {
      result.second.fNumErrInvalidMsSize++;
      return result;
    }
    const uint32_t numMessages = msSize / sizeof(critof001::Message);
    if (numMessages < 2) {
      result.second.fNumErrInvalidMsSize++;
      return result;
    }

    // --- Interpret MS content as sequence of SMX messages
    auto message = reinterpret_cast<const critof001::Message*>(msContent);

    // --- The first message in the MS is expected to be of type EPOCH.
    if (message[0].getMessageType() != critof001::MSG_EPOCH) {
      result.second.fNumErrInvalidFirstMessage++;
      return result;
    }

    {  // --- Check that first epoch matches with the microslice index
      const uint64_t msStartEpoch =
        static_cast<uint64_t>(msDescr.idx / critof001::kuEpochInNs) % critof001::kulEpochCycleEp;
      if (message[0].getGdpbEpEpochNb() != msStartEpoch) {
        result.second.fNumErrInvalidStartEpoch++;
        return result;
      }
    }

    // --- The last message in the MS is expected to be EndOfMs.
    if (!message[numMessages - 1].isEndOfMs()) {
      result.second.fNumErrInvalidLastMessage++;
      return result;
    }
    //Check if last message is "EndOfMs"!! Maybe loop to messageNr < numMessages - 1

    // --- Message loop
    for (uint32_t messageNr = 0; messageNr < numMessages; messageNr++) {

      // --- Action depending on message type
      switch (message[messageNr].getMessageType()) {

        case critof001::MSG_HIT: {
          ProcessHitMessage(message[messageNr], result.first, result.second);
          break;
        }
        case critof001::MSG_EPOCH: {
          ProcessEpochMessage(message[messageNr]);
          break;
        }
        case critof001::MSG_SLOWC: {
          result.second.fNumNonHitOrTsbMessage++;
          break;
        }
        case critof001::MSG_SYST: {
          result.second.fNumNonHitOrTsbMessage++;
          break;
        }
        default: {
          result.second.fNumNonHitOrTsbMessage++;
          break;
        }
      }  //? Message type
    }    //# Messages

    return result;
  }
  // --------------------------------------------------------------------------


  // -----   Process hit message   --------------------------------------------
  inline void UnpackBmon::ProcessHitMessage(const critof001::Message& message, vector<CbmBmonDigi>& digiVec,
                                            UnpackBmonMonitorData& monitor) const
  {
    // IGNORES:
    // - Duplicate messages
    // - (0 == uChanUId)
    // - (fviRpcChUId.size() < uRemappedChannelNrInSys)
    // - successive digis with same time
    // (these are filtered in original version but not here)
    // also: does not apply new "remap digis" hack, and always includes timeslice overlap

    // --- Check eLink and get parameters
    const uint32_t elink = message.getGet4Idx();
    if (elink >= fParams.fElinkParams.size()) {
      monitor.fNumErrElinkOutOfRange++;
      return;
    }
    const UnpackBmonElinkPar& elinkPar = fParams.fElinkParams.at(elink);

    const uint32_t channel    = message.getGdpbHitChanId();
    const uint32_t channelUId = (elinkPar.fChannelUId)[channel];

    double messageTime  = message.getMsgFullTimeD(fCurrentEpochInTs) - elinkPar.fTimeOffset;
    const double charge = (double) message.getGdpbHit32Tot();  //cast from uint32_t

    // --- Create output digi
    digiVec.emplace_back(channelUId, messageTime, charge);
  }
  // --------------------------------------------------------------------------


  // -----   Process an epoch message   ---------------------------------------
  inline void UnpackBmon::ProcessEpochMessage(const critof001::Message& message)
  {
    const uint64_t epoch = message.getGdpbEpEpochNb();

    // --- Calculate epoch relative to timeslice start time; correct for epoch cycles
    if (fCurrentTsTime <= epoch) { fCurrentEpochInTs = epoch - fCurrentTsTime; }
    else {
      fCurrentEpochInTs = epoch + critof001::kulEpochCycleEp - fCurrentTsTime;
    }
    //Problem if MS spans multiple epoch cycles?
  }
  // --------------------------------------------------------------------------


} /* namespace cbm::algo */
