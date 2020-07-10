#include "TimesliceMetaData.h"

#include <TObject.h>  // for TObject

TimesliceMetaData::TimesliceMetaData(ULong64_t ulStart,
                                     ULong64_t ulDur,
                                     ULong64_t ulOverDur,
                                     ULong64_t ulIndex)
  : TObject()
  , fulStartTimeNs(ulStart)
  , fulDurationNs(ulDur)
  , fulOverlapNs(ulOverDur)
  , fulIndex(ulIndex) {
  ;
}
