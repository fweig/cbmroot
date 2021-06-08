/* Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer], Florian Uhlig */

#ifndef TIMESLICE_METADATA_H
#define TIMESLICE_METADATA_H

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for ULong64_t
#include <TObject.h>     // for TObject

#include <stdint.h>


class TimesliceMetaData : public TObject {
public:
  TimesliceMetaData(ULong64_t ulStart = 0, ULong64_t ulDur = 12800000, ULong64_t ulOverDur = 1280000,
                    ULong64_t ulIndex = 0);

  /// Copy construction
  TimesliceMetaData(const TimesliceMetaData&) = default;
  /// Move constuctor
  TimesliceMetaData(TimesliceMetaData&&) = default;
  /// Copy operator
  TimesliceMetaData& operator=(const TimesliceMetaData&) = default;

  void SetStartTime(ULong64_t ulStart) { fulStartTimeNs = ulStart; }
  void SetDuration(ULong64_t ulDur) { fulDurationNs = ulDur; }
  void SetOverlapDuration(ULong64_t ulDur) { fulOverlapNs = ulDur; }
  void SetIndex(ULong64_t ulIdx) { fulIndex = ulIdx; }

  ULong64_t GetStartTime() const { return fulStartTimeNs; }
  ULong64_t GetDuration() const { return fulDurationNs; }
  ULong64_t GetOverlapStartTime() const { return fulStartTimeNs + fulDurationNs; }
  ULong64_t GetOverlapDuration() const { return fulOverlapNs; }
  ULong64_t GetIndex() const { return fulIndex; }

private:
  ULong64_t fulStartTimeNs = 0;
  //      ULong64_t fulDurationNs   = 10240000; // 100 MS *  102400 ns (no TRD), default to update in source
  ULong64_t fulDurationNs = 12800000;  //  10 MS * 1280000 ns (with TRD), default to update in source
  ULong64_t fulOverlapNs  = 1280000;   //   1 MS * 1280000 ns (with TRD), default to update in source
  ULong64_t fulIndex      = 0;
  /*
      ULong64_t fulErrorsNbT0   = 0;
      ULong64_t fulErrorsNbSts  = 0;
      ULong64_t fulErrorsNbMuch = 0;
      ULong64_t fulErrorsNbTof  = 0;
      ULong64_t fulErrorsNbTrd  = 0;
      ULong64_t fulErrorsNbRich = 0;
      ULong64_t fulErrorsNbPsd  = 0;
*/

  ClassDef(TimesliceMetaData, 2);
};

#endif  // TIMESLICE_METADATA_H
