#ifndef TIMESLICE_METADATA_H
#define TIMESLICE_METADATA_H

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for ULong64_t
#include <TObject.h>     // for TObject

#include <stdint.h>


class TimesliceMetaData : public TObject
{
   public:
      TimesliceMetaData( ULong64_t ulStart = 0, ULong64_t ulDur = 10240000 );

      void SetStartTime( ULong64_t ulStart ) { fulStartTimeNs = ulStart; }
      void SetDuration( ULong64_t ulDur ) { fulDurationNs = ulDur; }

      ULong64_t GetStartTime() { return fulStartTimeNs; }
      ULong64_t GetDuration() { return fulDurationNs; }

   private:
      ULong64_t fulStartTimeNs  = 0;
      ULong64_t fulDurationNs   = 10240000; // 100 MS * 102400 ns
/*
      ULong64_t fulErrorsNbT0   = 0;
      ULong64_t fulErrorsNbSts  = 0;
      ULong64_t fulErrorsNbMuch = 0;
      ULong64_t fulErrorsNbTof  = 0;
      ULong64_t fulErrorsNbTrd  = 0;
      ULong64_t fulErrorsNbRich = 0;
      ULong64_t fulErrorsNbPsd  = 0;
*/

   ClassDef(TimesliceMetaData,1);
};

#endif // TIMESLICE_METADATA_H
