/* Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "TimesliceMetaData.h"

#include <TObject.h>  // for TObject

TimesliceMetaData::TimesliceMetaData(uint64_t ulStart, uint64_t ulDur, uint64_t ulOverDur, uint64_t ulIndex)
  : TObject()
  , fulStartTimeNs(ulStart)
  , fulDurationNs(ulDur)
  , fulOverlapNs(ulOverDur)
  , fulIndex(ulIndex)
{
  ;
}
