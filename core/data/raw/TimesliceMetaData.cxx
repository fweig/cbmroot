/** @file TimesliceMetaData.cxx
  * @copyright Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

#include "TimesliceMetaData.h"

#include <TObject.h>  // for TObject

TimesliceMetaData::TimesliceMetaData(ULong64_t ulStart, ULong64_t ulDur, ULong64_t ulOverDur, ULong64_t ulIndex)
  : TObject()
  , fulStartTimeNs(ulStart)
  , fulDurationNs(ulDur)
  , fulOverlapNs(ulOverDur)
  , fulIndex(ulIndex)
{
  ;
}
