/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Volker Friese [committer] */

#include "UnpackMuch.h"

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
  UnpackMuch::resultType UnpackMuch::operator()(const uint8_t* msContent, const fles::MicrosliceDescriptor& msDescr,
                                                const uint64_t tTimeslice)
  {

    // --- Output data
    resultType result = {};

    // --- Current Timeslice start time in epoch units. Note that it is always a multiple of epochs
    // --- and the epoch is a multiple of ns.
    const uint64_t epochLengthInNs = fkEpochLength * fkClockCycleNom / fkClockCycleDen;
    fCurrentTsTime                 = tTimeslice / epochLengthInNs;

    // --- Current TS_MSB epoch cycle
    auto const msTime = msDescr.idx;  // Unix time of MS in ns
    fCurrentCycle     = std::ldiv(msTime, fkCycleLength).quot;

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

    // --- The first message in the MS is expected to be of type EPOCH and can be ignored.
    if (message[0].GetMessType() != stsxyter::MessType::Epoch) {
      result.second.fNumErrInvalidFirstMessage++;
      return result;
    }

    // --- The second message must be of type ts_msb.
    if (message[1].GetMessType() != stsxyter::MessType::TsMsb) {
      result.second.fNumErrInvalidFirstMessage++;
      return result;
    }
    ProcessTsmsbMessage(message[1]);

    // --- Message loop
    for (uint32_t messageNr = 2; messageNr < numMessages; messageNr++) {

      // --- Action depending on message type
      switch (message[messageNr].GetMessType()) {

        case stsxyter::MessType::Hit: {
          ProcessHitMessage(message[messageNr], result.first, result.second);
          break;
        }
        case stsxyter::MessType::TsMsb: {
          ProcessTsmsbMessage(message[messageNr]);
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
  inline void UnpackMuch::ProcessHitMessage(const stsxyter::Message& message, vector<CbmMuchDigi>& digiVec,
                                            UnpackMuchMonitorData& monitor) const
  {
    /* STS version
    // --- Check eLink and get parameters
    uint16_t elink = message.GetLinkIndexHitBinning();
    if (elink >= fParams.fElinkParams.size()) {
      monitor.fNumErrElinkOutOfRange++;
      return;
    }
    const UnpackMuchElinkPar& elinkPar = fParams.fElinkParams.at(elink);
    uint32_t asicNr                    = elinkPar.fAsicNr;

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

    // --- Expand time stamp to time within timeslice (in clock cycle)
    uint64_t messageTime = message.GetHitTimeBinning() + fCurrentEpochTime;

    // --- Convert time stamp from clock cycles to ns. Round to nearest full ns.
    messageTime = (messageTime * fkClockCycleNom + fkClockCycleDen / 2) / fkClockCycleDen;

    // --- Correct ASIC-wise offsets
    messageTime -= elinkPar.fTimeOffset;
    // --- TODO: Add walk correction (depends on ADC)

    // --- Charge
    double charge = elinkPar.fAdcOffset + (message.GetHitAdc() - 1) * elinkPar.fAdcGain;

    // --- Create output digi
    digiVec.emplace_back(address, channel, messageTime, charge);
*/
  }
  // --------------------------------------------------------------------------


  // -----   Process an epoch (TS_MSB) message   ------------------------------
  inline void UnpackMuch::ProcessTsmsbMessage(const stsxyter::Message& message)
  {
    // The compression of time is based on the hierarchy epoch cycle - epoch - message time.
    // Cycles are counted from the start of Unix time and are multiples of an epoch (ts_msb).
    // The epoch number is counted within each cycle. The time in the hit message is expressed
    // in units of the readout clock cycle relative to the current epoch.
    // The ts_msb message indicates the start of a new epoch. Its basic information is the epoch
    // number within the current cycle. A cycle wrap resets the epoch number to zero, so it is
    // indicated by the epoch number being smaller than the previous one (epoch messages are
    // seemingly not consecutively in the data stream, but only if there are hit messages in between).
    auto epoch = message.GetTsMsbValBinning();

    // --- Cycle wrap
    if (epoch < fCurrentEpoch) fCurrentCycle++;

    // --- Update current epoch counter
    fCurrentEpoch = epoch;

    // --- Calculate epoch time in clocks cycles relative to timeslice start time
    fCurrentEpochTime = (fCurrentCycle * fkEpochsPerCycle + epoch - fCurrentTsTime) * fkEpochLength;
  }
  // --------------------------------------------------------------------------


} /* namespace cbm::algo */
