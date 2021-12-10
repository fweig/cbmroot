/* Copyright (C) 2018-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/************************************************************
 *
 ************************************************************/

#ifndef STSXYTERFINALHIT_H
#define STSXYTERFINALHIT_H

// C/C++ headers
#include <cstdint>
#include <iostream>

namespace stsxyter
{

  class FinalHit {
  private:
  protected:
    uint64_t fulLongTs;   // (Software-)Extended TS, in clock cycles
    uint16_t fusRawAdc;   // Raw ADC value
    uint16_t fusAsicIdx;  // ASIC Index
    uint16_t fusChanIdx;  // channel Index
    uint16_t fusDpbIdx;   // Optional DPB  Index
    uint16_t fusCrobIdx;  // Optional CROB Index

  public:
    FinalHit(const FinalHit& src)
      : fulLongTs(src.fulLongTs)
      , fusRawAdc(src.fusRawAdc)
      , fusAsicIdx(src.fusAsicIdx)
      , fusChanIdx(src.fusChanIdx)
      , fusDpbIdx(src.fusDpbIdx)
      , fusCrobIdx(src.fusCrobIdx)
    {
    }

    FinalHit(uint64_t ulLongTsIn, uint16_t usRawAdcIn, uint16_t usAsicIdxIn, uint16_t usChanIdxIn,
             uint16_t usDpbIdx = 0, uint16_t usCrobIdx = 0)
      : fulLongTs(ulLongTsIn)
      , fusRawAdc(usRawAdcIn)
      , fusAsicIdx(usAsicIdxIn)
      , fusChanIdx(usChanIdxIn)
      , fusDpbIdx(usDpbIdx)
      , fusCrobIdx(usCrobIdx)
    {
    }

    FinalHit() : FinalHit(0, 0, 0, 0) {}

    ~FinalHit() {};

    void assign(const FinalHit& src)
    {
      fulLongTs  = src.fulLongTs;
      fusRawAdc  = src.fusRawAdc;
      fusAsicIdx = src.fusAsicIdx;
      fusChanIdx = src.fusChanIdx;
      fusDpbIdx  = src.fusDpbIdx;
      fusCrobIdx = src.fusCrobIdx;
    }

    FinalHit& operator=(const FinalHit& src)
    {
      assign(src);
      return *this;
    }
    bool operator<(const FinalHit& other) const { return fulLongTs < other.fulLongTs; }

    inline void reset()
    {
      fulLongTs  = 0;
      fusRawAdc  = 0;
      fusAsicIdx = 0;
      fusChanIdx = 0;
      fusDpbIdx  = 0;
      fusCrobIdx = 0;
    }

    inline uint64_t GetTs() const { return fulLongTs; }
    inline uint16_t GetAdc() const { return fusRawAdc; }
    inline uint16_t GetAsic() const { return fusAsicIdx; }
    inline uint16_t GetChan() const { return fusChanIdx; }
    inline uint16_t GetDpb() const { return fusDpbIdx; }
    inline uint16_t GetCrob() const { return fusCrobIdx; }
  };
}  // namespace stsxyter
#endif  // STSXYTERFINALHIT_H
