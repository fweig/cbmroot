/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig, Alexandru Bercuci, Dominik Smith [committer] */

#include "UnpackTrd2d.h"

#include <Logger.h>

#include <algorithm>
#include <cassert>
#include <vector>

using std::unique_ptr;

namespace cbm::algo
{
  // ----   Fasp message constructor  ----------------------------------------
  CbmTrdFaspMessage::CbmTrdFaspMessage(uint8_t c, uint8_t typ, uint8_t t, uint16_t d, uint8_t rob, uint8_t asic)
    : ch(c)
    , type(typ)
    , tlab(t)
    , data(d)
    , crob(rob)
    , fasp(asic)
  {
  }

  // ----   Algorithm execution   ---------------------------------------------
  UnpackTrd2d::resultType UnpackTrd2d::operator()(const uint8_t* msContent, const fles::MicrosliceDescriptor& msDescr,
                                                  const uint64_t tTimeslice)
  {
    // --- Output data
    resultType result = {};

    // define time wrt start of time slice in TRD/FASP clks [80 MHz]. Contains:
    //  - relative offset of the MS wrt the TS
    //  - FASP epoch offset for current CROB
    //  - TRD2D system offset wrt to experiment time (e.g. T0)
    uint64_t time = uint64_t((msDescr.idx - tTimeslice - fSystemTimeOffset) / 12.5);

    // Get parameters for current eq id.
    const uint8_t crob_id = fParams.fCrobId;

    // Get the number of complete words in the input MS buffer.
    const uint32_t nwords = msDescr.size / 4;

    // We have 32 bit spadic frames in this readout version
    const uint32_t* wd = reinterpret_cast<const uint32_t*>(msContent);

    unsigned char lFaspOld(0xff);
    std::vector<CbmTrdFaspMessage> vMess;
    for (uint64_t j = 0; j < nwords; j++, wd++) {
      uint32_t w      = *wd;
      uint8_t ch_id   = w & 0xf;
      uint8_t isaux   = (w >> 4) & 0x1;
      uint8_t slice   = (w >> 5) & 0x7f;
      uint16_t data   = (w >> 12) & 0x3fff;
      uint8_t fasp_id = ((w >> 26) & 0x3f);

      if (isaux) {
        if (ch_id == 0) {
          // clear buffer
          if (vMess.size()) { pushDigis(vMess, time); }
          vMess.clear();

          lFaspOld = 0xff;
          time += FASP_EPOCH_LENGTH;
        }
        continue;
      }
      if (lFaspOld != fasp_id) {
        if (vMess.size()) { pushDigis(vMess, time); }
        vMess.clear();
        lFaspOld = fasp_id;
      }
      if (data & 0x1) {
        LOG(warn) << "UnpackTrd2d - Data corrupted : detect end bit set.";
        continue;
      }
      if (data & 0x2000) {
        LOG(debug) << "UnpackTrd2d - Self-triggered data.";
        data &= 0x1fff;
      }
      vMess.emplace_back(ch_id, kData, slice, data >> 1, crob_id, lFaspOld);
    }
    result.first = FinalizeComponent();

    return result;
  }

  //_________________________________________________________________________________
  bool UnpackTrd2d::pushDigis(std::vector<CbmTrdFaspMessage> messes, const uint64_t time)
  {
    const uint16_t mod_id             = fParams.fModId;
    const UnpackTrd2dAsicPar& asicPar = fParams.fAsicParams[messes[0].fasp];
    const uint64_t tdaqOffset         = asicPar.fChanParams[messes[0].ch].fDaqOffset;

    for (auto imess : messes) {
      const int32_t pad                   = asicPar.fChanParams[imess.ch].fPadAddress;
      const bool hasPairingR              = asicPar.fChanParams[imess.ch].fHasPairingR;
      const uint64_t lTime                = time + tdaqOffset + imess.tlab;
      const uint16_t lchR                 = hasPairingR ? imess.data : 0;
      const uint16_t lchT                 = hasPairingR ? 0 : imess.data;
      std::vector<CbmTrdDigi>& digiBuffer = fDigiBuffer[pad];

      if (digiBuffer.size() == 0) {  // init pad position in map and build digi for message
        digiBuffer.emplace_back(pad, lchT, lchR, lTime);
        digiBuffer.back().SetAddressModule(mod_id);
        continue;
      }

      // check if last digi has both R/T message components. Update if not and is within time window
      auto id = digiBuffer.rbegin();  // Should always be valid here.
                                      // No need to extra check
      double r, t;
      int32_t dt;
      const int32_t dtime = (*id).GetTimeDAQ() - lTime;
      bool use(false);

      if (abs(dtime) < 5) {  // test message part of (last) digi
        r = (*id).GetCharge(t, dt);
        if (lchR && r < 0.1) {  // set R charge on an empty slot
          (*id).SetCharge(t, lchR, -dtime);
          use = true;
        }
        else if (lchT && t < 0.1) {  // set T charge on an empty slot
          (*id).SetCharge(lchT, r, +dtime);
          (*id).SetTimeDAQ(uint64_t((*id).GetTimeDAQ() - dtime));
          use = true;
        }
      }

      // build digi for message when update failed
      if (!use) {
        digiBuffer.emplace_back(pad, lchT, lchR, lTime);
        digiBuffer.back().SetAddressModule(mod_id);
        id = digiBuffer.rbegin();
      }

      // update charge for previously allocated digis to account for FASPRO ADC buffering and read-out feature
      for (++id; id != digiBuffer.rend(); ++id) {
        r = (*id).GetCharge(t, dt);
        if (lchR && int(r)) {  // update R charge and mark on digi
          (*id).SetCharge(t, lchR, dt);
          (*id).SetFlag(1);
          break;
        }
        else if (lchT && int(t)) {  // update T charge and mark on digi
          (*id).SetCharge(lchT, r, dt);
          (*id).SetFlag(0);
          break;
        }
      }
    }
    messes.clear();

    return true;
  }

  std::vector<CbmTrdDigi> UnpackTrd2d::FinalizeComponent()
  {
    std::vector<CbmTrdDigi> outputDigis;

    for (uint16_t ipad(0); ipad < NFASPMOD * NFASPCH; ipad++) {
      if (!fDigiBuffer[ipad].size()) continue;
      uint nIncomplete(0);
      for (auto id = fDigiBuffer[ipad].begin(); id != fDigiBuffer[ipad].end(); id++) {
        double r, t;
        int32_t dt;
        r = (*id).GetCharge(t, dt);
        // check if digi has all signals CORRECTED
        if (((t > 0) != (*id).IsFlagged(0)) || ((r > 0) != (*id).IsFlagged(1))) {
          nIncomplete++;
          continue;
        }
        // reset flags as they were used only to mark the correctly setting of the charge/digi
        (*id).SetFlag(0, false);
        (*id).SetFlag(1, false);
        outputDigis.emplace_back(std::move((*id)));
      }
      // clear digi buffer wrt the digi which was forwarded to higher structures
      fDigiBuffer[ipad].clear();
      if (nIncomplete > 2) {
        LOG(warn) << "FinalizeComponent() skip " << nIncomplete << " incomplete digi at pad " << ipad << ".\n";
      }
    }
    return outputDigis;
  }
} /* namespace cbm::algo */
