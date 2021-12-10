/* Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer], Florian Uhlig */

#ifndef TIMESLICE_METADATA_H
#define TIMESLICE_METADATA_H

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <TObject.h>     // for TObject

#include <cstdint>


class TimesliceMetaData : public TObject {
public:
  TimesliceMetaData(uint64_t ulStart = 0, uint64_t ulDur = 12800000, uint64_t ulOverDur = 1280000,
                    uint64_t ulIndex = 0);

  /// Copy construction
  TimesliceMetaData(const TimesliceMetaData&) = default;
  /// Move constuctor
  TimesliceMetaData(TimesliceMetaData&&) = default;
  /// Copy operator
  TimesliceMetaData& operator=(const TimesliceMetaData&) = default;

  void SetStartTime(uint64_t ulStart) { fulStartTimeNs = ulStart; }
  void SetDuration(uint64_t ulDur) { fulDurationNs = ulDur; }
  void SetOverlapDuration(uint64_t ulDur) { fulOverlapNs = ulDur; }
  void SetIndex(uint64_t ulIdx) { fulIndex = ulIdx; }

  uint64_t GetStartTime() const { return fulStartTimeNs; }
  uint64_t GetDuration() const { return fulDurationNs; }
  uint64_t GetOverlapStartTime() const { return fulStartTimeNs + fulDurationNs; }
  uint64_t GetOverlapDuration() const { return fulOverlapNs; }
  uint64_t GetIndex() const { return fulIndex; }

private:
  uint64_t fulStartTimeNs = 0;
  //      uint64_t fulDurationNs   = 10240000; // 100 MS *  102400 ns (no TRD), default to update in source
  uint64_t fulDurationNs = 12800000;  //  10 MS * 1280000 ns (with TRD), default to update in source
  uint64_t fulOverlapNs  = 1280000;   //   1 MS * 1280000 ns (with TRD), default to update in source
  uint64_t fulIndex      = 0;
  /*
      uint64_t fulErrorsNbT0   = 0;
      uint64_t fulErrorsNbSts  = 0;
      uint64_t fulErrorsNbMuch = 0;
      uint64_t fulErrorsNbTof  = 0;
      uint64_t fulErrorsNbTrd  = 0;
      uint64_t fulErrorsNbRich = 0;
      uint64_t fulErrorsNbPsd  = 0;
*/

  ClassDef(TimesliceMetaData, 2);
};

#endif  // TIMESLICE_METADATA_H
