// ------------------------------------------------------------------
// -----                     TTofTriglogBoard                   -----
// -----              Created 21/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TTOFTRIGLOGBOARD_H_
#define TTOFTRIGLOGBOARD_H_

#include <vector>

#include "TObject.h"

class TTofTriglogBoard : public TObject
{
   public:
      TTofTriglogBoard();
      TTofTriglogBoard( UInt_t uSec, UInt_t uMilSec, UInt_t uSyncNb, 
                        UInt_t uTrigPat, UInt_t uInpPat = 0, UInt_t uRefClk = 0 );
      ~TTofTriglogBoard();
      
      virtual void Clear(Option_t *option = "");

      void SetPresentFlag( Bool_t bDataThere )  { fbTriglogFoundInEvent = bDataThere; };
      void SetMbsTimeSec( UInt_t uSec )         { fMbsTimeSecs           = uSec; };
      void SetMbsTimeMilliSec( UInt_t uMilSec ) { fMbsTimeMillisec       = uMilSec; };
      void SetSyncNb( UInt_t uSyncNb )          { fVulomSyncNumber       = uSyncNb; };
      void SetTriggPatt( UInt_t uTrigPat )      { fVulomTriggerPattern   = uTrigPat; };
      void SetInpPatt( UInt_t uInpPat )         { fTriglogInputPattern   = uInpPat; };
      void SetRefClk( UInt_t uRefClk )          { fTriglogReferenceClock = uRefClk; };

      Bool_t IsUpdated()          const { return fbTriglogFoundInEvent; };
      UInt_t GetMbsTimeSec()      const { return fMbsTimeSecs; };
      UInt_t GetMbsTimeMilliSec() const { return fMbsTimeMillisec; };
      UInt_t GetSyncNb()          const { return fVulomSyncNumber; };
      UInt_t GetTriggPatt()       const { return fVulomTriggerPattern; };
      UInt_t GetInpPatt()         const { return fTriglogInputPattern; };
      UInt_t GetRefClk()          const { return fTriglogReferenceClock; };
      
   protected:
      
   private:
      /* In GSI April 2014 beamtime some events mayb not contains any TRIGLOG data
       *  (TRB buffer flushing trigger)
       */
      Bool_t fbTriglogFoundInEvent;
      /* system time from mbs, seconds since 1970*/
      UInt_t fMbsTimeSecs;
      /* system time from mbs, microseconds since fMbsTimeSecs*/
      UInt_t fMbsTimeMillisec;
      /* vulom trigger sync number*/
      UInt_t fVulomSyncNumber;
      /* vulom trigger pattern*/
      UInt_t fVulomTriggerPattern;
      /* Triglog input pattern*/
      UInt_t fTriglogInputPattern;
      /* Internal 781250 Hz reference clock for rates calculation */
      UInt_t fTriglogReferenceClock;
      
   ClassDef(TTofTriglogBoard, 1)
};

#endif // TTOFTRIGLOGBOARD_H_
