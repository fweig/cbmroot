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
    vector<CbmStsDigi> digiVec = {};
    UnpackStsMonitorData moni  = {};

    // --- Current Timeslice time and TS_MSB epoch cycle
    fCurrentTsTime = tTimeslice;
    auto msTime    = msDescr.idx;  // Unix time of MS in ns
    fCurrentCycle  = std::ldiv(msTime, fkCycleLength).quot;

    // --- Interpret MS content as sequence of SMX messages
    auto message = reinterpret_cast<const stsxyter::Message*>(msContent);

    // --- The first message in the MS is expected to be of type EPOCH and can be ignored
    if (message[0].GetMessType() != stsxyter::MessType::Epoch) {
      std::cout << "Error: UnpackSts: First message type is " << uint16_t(message[0].GetMessType()) << std::endl;
      moni.fNumErrInvalidFirstMessage++;
      return std::make_pair(digiVec, moni);
    }

    // --- The second message must be of type ts_msb
    if (message[1].GetMessType() != stsxyter::MessType::TsMsb) {
      std::cout << "Error: UnpackSts: Second message type is " << uint16_t(message[0].GetMessType()) << std::endl;
      moni.fNumErrInvalidFirstMessage++;
      return std::make_pair(digiVec, moni);
    }
    ProcessTsmsbMessage(message[1]);

    // --- Number of messages in microslice
    auto msSize = msDescr.size;
    if (msSize % sizeof(stsxyter::Message) != 0) {
      moni.fNumErrInvalidMsSize++;
      return std::make_pair(digiVec, moni);
    }
    const uint32_t numMessages = msSize / sizeof(stsxyter::Message);

    // --- Message loop
    for (uint32_t messageNr = 2; messageNr < numMessages; messageNr++) {

      // --- Action depending on message type
      switch (message[messageNr].GetMessType()) {

        case stsxyter::MessType::Hit: {
          ProcessHitMessage(message[messageNr], digiVec, moni);
          break;
        }
        case stsxyter::MessType::TsMsb: {
          ProcessTsmsbMessage(message[messageNr]);
          break;
        }
        default: {
          moni.fNumNonHitOrTsbMessage++;
          break;
        }

      }  //? Message type

    }  //# Messages

    return std::make_pair(digiVec, moni);
  }
  // --------------------------------------------------------------------------


  // -----   Process hit message   --------------------------------------------
  void UnpackSts::ProcessHitMessage(const stsxyter::Message& message, vector<CbmStsDigi>& digiVec,
                                    UnpackStsMonitorData& moni) const
  {

    // --- Check eLink and get parameters
    uint16_t elink = message.GetLinkIndexHitBinning();
    if (elink >= fParams.fElinkParams.size()) {
      moni.fNumErrElinkOutOfRange++;
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

    // --- Time stamp
    // --- Expand to full Unix time in clock cycles
    uint64_t timeCc = message.GetHitTimeBinning() + fCurrentEpochTime;
    if (timeCc >> 59 != 0) {  // avoid overflow in 64 bit // TODO: Hard-coded number!
      moni.fNumErrTimestampOverflow++;
      return;
    }
    // --- Convert time into ns
    uint64_t timeNs = (timeCc * fkClockCycleNom + fkClockCycleDen / 2) / fkClockCycleDen;
    // --- Correct ASIC-wise offsets
    timeNs -= elinkPar.fTimeOffset;
    // --- Calculate time relative to timeslice
    timeNs -= fCurrentTsTime;
    // --- TODO: Add walk correction (depends on ADC)

    // --- Charge
    double charge = elinkPar.fAdcOffset + (message.GetHitAdc() - 1) * elinkPar.fAdcGain;

    // --- Create output digi
    digiVec.emplace_back(address, channel, timeNs, charge);
  }
  // --------------------------------------------------------------------------


  // -----   Process an epoch (TS_MSB) message   ------------------------------
  void UnpackSts::ProcessTsmsbMessage(const stsxyter::Message& message)
  {

    auto epoch = message.GetTsMsbValBinning();

    // --- Cycle wrap
    if (epoch < fCurrentEpoch) fCurrentCycle++;

    // --- Update current epoch
    fCurrentEpoch     = epoch;
    fCurrentEpochTime = (fCurrentCycle * fkEpochsPerCycle + epoch) * fkEpochLength;
  }
  // --------------------------------------------------------------------------


} /* namespace cbm::algo */
