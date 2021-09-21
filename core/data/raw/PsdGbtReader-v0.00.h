/* Copyright (C) 2019-2021 Institute for Nuclear Research, Moscow
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin [committer], David Emschermann, Pierre-Alain Loizeau*/

#ifndef PSD_GBT_READER_V000_H
#define PSD_GBT_READER_V000_H

#include <vector>  // for vector

#include <stdint.h>  // for uint64_t, uint32_t

#include "PsdGbtDataFormat-v0.00.h"  // for PsdHitData, PsdHitHeader, PsdEventHead...

namespace PsdDataV000
{
  class PsdGbtReader {

  public:
    PsdGbtReader() {};
    PsdGbtReader(const uint64_t* input)
    {
      buffer         = input;
      gbt_word_index = 0;
    }

    ~PsdGbtReader();

    PsdEventHeaderAB EvHdrAb;
    PsdEventHeaderAC EvHdrAc;
    PsdHitHeader HitHdr;
    PsdHitData HitData;

    std::vector<PsdHitHeader> VectHitHdr;
    std::vector<PsdHitData> VectHitData;

    void SetInput(const uint64_t* input)
    {
      buffer         = input;
      gbt_word_index = 0;
    }
    void SetPrintOutMode(bool mode) { PrintOut = mode; }
    void ReadEventHeaderAbFles();
    void ReadEventHeaderAcFles();
    void ReadHitHeaderFles();
    void ReadHitDataFles();
    int ReadEventFles();

    //Getters
    uint32_t GetTotalGbtWordsRead() { return gbt_word_index; }

  private:
    const uint64_t* buffer;

    bool PrintOut           = false;
    uint32_t gbt_word_index = 0;
    int buffer_shift        = 0;
  };
}  // namespace PsdDataV000

#endif  // PSD_GBT_READER_V000_H
