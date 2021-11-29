/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Volker Friese [committer] */

#include "UnpackSts.h"

#include <cassert>
#include <vector>

#include <cmath>

#include "StsXyterMessage.h"

using std::vector;

namespace cbm::algo
{

  // ----   Algorithm execution   ---------------------------------------------
  vector<CbmStsDigi> UnpackSts::operator()(const uint8_t* msContent, const fles::MicrosliceDescriptor& msDescr,
                                           const uint64_t tTimeslice)
  {

    // --- Assert that parameters are set
    assert(fParams);
    fCurrentTsTime = tTimeslice;

    // --- Output vector
    vector<CbmStsDigi> digiVec;

    // --- Current TS_MSB epoch cycle
    auto msTime   = msDescr.idx;  // Unix time of MS in ns
    fCurrentCycle = std::ldiv(msTime, fCycleLength).quot;

    // --- Get first TS_MSB (first message in microslice must be of type ts_msb)
    const stsxyter::Message firstMessage(msContent[0]);
    assert(firstMessage.GetMessType() == stsxyter::MessType::TsMsb);
    ProcessTsmsbMessage(firstMessage);

    // --- Number of messages in microslice
    auto msSize = msDescr.size;
    assert(msSize % sizeof(stsxyter::Message) == 0);
    const uint32_t numMessages = msSize / sizeof(stsxyter::Message);

    // --- Message loop
    for (uint32_t messageNr = 1; messageNr < numMessages; messageNr++) {

      // --- Cast MS content to STSXYTER message
      const stsxyter::Message message(msContent[messageNr]);
      const stsxyter::MessType type = message.GetMessType();

      // --- Action depending on message type
      switch (type) {

        case stsxyter::MessType::Hit: {
          ProcessHitMessage(message, digiVec);
          break;
        }
        case stsxyter::MessType::TsMsb: {
          ProcessTsmsbMessage(message);
          break;
        }
        default: {
          break;
        }

      }  //? Message type

    }  //# Messages

    return digiVec;
  }
  // --------------------------------------------------------------------------


  // -----   Process hit message   --------------------------------------------
  void UnpackSts::ProcessHitMessage(const stsxyter::Message& message, vector<CbmStsDigi>& digiVec) const
  {

    uint16_t elink                    = message.GetLinkIndexHitBinning();
    const UnpackStsElinkPar& elinkPar = fParams->GetElinkPar(elink);
    uint32_t asicNr                   = elinkPar.fAsicNr;
    uint32_t numChansPerModule        = fParams->fNumAsicsPerModule * fParams->fNumChansPerAsic;

    // --- Hardware-to-software address
    uint32_t address = elinkPar.fAddress;
    uint32_t channel = 0;
    if (asicNr < fParams->fNumAsicsPerModule / 2) {  // front side (n side)
      channel = message.GetHitChannel() + fParams->fNumChansPerAsic * asicNr;
    }
    else {  // back side (p side)
      channel = numChansPerModule - message.GetHitChannel() + 1;
    }

    // --- Time stamp
    // --- Expand to full Unix time in clock cycles
    uint64_t timeCC = message.GetHitTimeBinning() + fCurrentEpochTime;
    // --- Convert time into ns
    assert(timeCC >> 59 == 0);  // avoid overflow in 64 bit
    uint64_t timeNs = (timeCC * fParams->fClockCycleNom) / fParams->fClockCycleDen;
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
    fCurrentEpochTime = (fCurrentCycle * fParams->fEpochsPerCycle + epoch) * fParams->fEpochLength;
  }
  // --------------------------------------------------------------------------


} /* namespace cbm::algo */
