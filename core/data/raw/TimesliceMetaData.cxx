#include "TimesliceMetaData.h"

#include <TObject.h>  // for TObject

TimesliceMetaData::TimesliceMetaData( ULong64_t ulStart, ULong64_t ulDur ) :
   TObject(),
   fulStartTimeNs( ulStart ),
   fulDurationNs( ulDur )
{
   ;
}
