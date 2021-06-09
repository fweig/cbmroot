/* Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin, David Emschermann [committer], Pierre-Alain Loizeau */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                       PsdGbtDataReader                            -----
// -----              Created 14.09.2019 by N.Karpushkin                   -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "PsdGbtReader.h"

#include <cstdint>  // for uint16_t, uint64_t, uint32_t

namespace PsdData
{

  PsdGbtReader::~PsdGbtReader()
  {
    EvHdrAb.clear();
    EvHdrAc.clear();
    HitHdr.clear();
    HitData.clear();
    VectHitHdr.clear();
    VectHitData.clear();
  }

  void PsdGbtReader::ReadEventHeaderAbFles()
  {
    EvHdrAb.clear();
    buffer_shift         = 0;
    EvHdrAb.ulMicroSlice = (buffer[gbt_word_index] >> buffer_shift) & 0xffffffffffffffff;
    gbt_word_index++;

    buffer_shift        = 0;
    EvHdrAb.uHitsNumber = (buffer[gbt_word_index] >> buffer_shift) & (((static_cast<uint32_t>(1)) << EvHdrAb.HNs) - 1);
    buffer_shift += EvHdrAb.HNs;
    EvHdrAb.uMagicWordAB = (buffer[gbt_word_index] >> buffer_shift) & (((static_cast<uint32_t>(1)) << EvHdrAb.MWs) - 1);
    gbt_word_index++;

    if (PrintOut) EvHdrAb.printout();
  }

  void PsdGbtReader::ReadEventHeaderAcFles()
  {
    EvHdrAc.clear();
    buffer_shift     = 0;
    EvHdrAc.uAdcTime = (buffer[gbt_word_index] >> buffer_shift) & (((static_cast<uint64_t>(1)) << EvHdrAc.TMs) - 1);
    gbt_word_index++;

    buffer_shift = 0;
    EvHdrAc.uPacketVersion =
      (buffer[gbt_word_index] >> buffer_shift) & (((static_cast<uint32_t>(1)) << EvHdrAc.PVs) - 1);
    buffer_shift += EvHdrAc.PVs;
    EvHdrAc.uMagicWordAC = (buffer[gbt_word_index] >> buffer_shift) & (((static_cast<uint32_t>(1)) << EvHdrAc.MWs) - 1);
    gbt_word_index++;

    if (PrintOut) EvHdrAc.printout();
  }

  void PsdGbtReader::ReadHitHeaderFles()
  {
    HitHdr.clear();
    buffer_shift      = 0;
    HitHdr.uZeroLevel = (buffer[gbt_word_index] >> buffer_shift) & (((static_cast<uint32_t>(1)) << HitHdr.ZLs) - 1);
    buffer_shift += HitHdr.ZLs;
    HitHdr.uSignalCharge = (buffer[gbt_word_index] >> buffer_shift) & (((static_cast<uint32_t>(1)) << HitHdr.SCs) - 1);
    gbt_word_index++;

    buffer_shift       = 0;
    HitHdr.uHitChannel = (buffer[gbt_word_index] >> buffer_shift) & (((static_cast<uint32_t>(1)) << HitHdr.HCs) - 1);
    buffer_shift += HitHdr.HCs;
    HitHdr.uWfmPoints = (buffer[gbt_word_index] >> buffer_shift) & (((static_cast<uint32_t>(1)) << HitHdr.WPSs) - 1);
    gbt_word_index++;

    if (PrintOut) HitHdr.printout();
  }

  void PsdGbtReader::ReadHitDataFles()
  {
    HitData.clear();
    buffer_shift = 64;
    for (int wfm_pt_iter = 0; wfm_pt_iter < HitHdr.uWfmPoints; wfm_pt_iter++) {
      buffer_shift -= HitData.WPs;
      uint16_t wfm_point = (buffer[gbt_word_index] >> buffer_shift) & (((static_cast<uint32_t>(1)) << HitData.WPs) - 1);
      HitData.uWfm.push_back(wfm_point);
      if (buffer_shift == 0) {
        gbt_word_index += 2;
        buffer_shift = 64;
      }
    }

    if (PrintOut) HitData.printout();
  }

  int PsdGbtReader::ReadEventFles()
  {
    bool IsAbHeaderInMessage = false;
    bool IsAcHeaderInMessage = false;

    ReadEventHeaderAbFles();
    ReadEventHeaderAcFles();
    IsAbHeaderInMessage = (EvHdrAb.uMagicWordAB == 171);
    IsAcHeaderInMessage = (EvHdrAc.uMagicWordAC == 172);

    if (IsAbHeaderInMessage && IsAcHeaderInMessage) {
      VectHitHdr.clear();
      VectHitData.clear();

      //hit loop
      for (int hit_iter = 0; hit_iter < EvHdrAb.uHitsNumber; hit_iter++) {
        ReadHitHeaderFles();
        VectHitHdr.push_back(HitHdr);
        ReadHitDataFles();
        VectHitData.push_back(HitData);

        if (VectHitHdr.at(hit_iter).uWfmPoints != 8) { return 2; }
      }  //hit loop

      if (EvHdrAb.uHitsNumber != VectHitHdr.size()) { return 3; }
    }
    else {
      return 1;
    }

    return 0;
  }
}  // namespace PsdData
