// ------------------------------------------------------------------
// -----                     TTofTriglogBoard                   -----
// -----              Created 21/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#include "TTofTriglogBoard.h"

// FAIR headers
#include "FairLogger.h"

// ROOT headers

// C/C++ headers
      
/************************** TTofTriglogBoard ****************************/
TTofTriglogBoard::TTofTriglogBoard():
   fbTriglogFoundInEvent(kFALSE),
   fMbsTimeSecs(0),
   fMbsTimeMillisec(0),
   fVulomSyncNumber(0),
   fVulomTriggerPattern(0),
   fTriglogInputPattern(0),
   fTriglogReferenceClock(0)
{
}
TTofTriglogBoard::TTofTriglogBoard( UInt_t uSec, UInt_t uMilSec, UInt_t uSyncNb, 
                        UInt_t uTrigPat, UInt_t uInpPat, UInt_t uRefClk ):
   fbTriglogFoundInEvent(kFALSE),
   fMbsTimeSecs(uSec),
   fMbsTimeMillisec(uMilSec),
   fVulomSyncNumber(uSyncNb),
   fVulomTriggerPattern(uTrigPat),
   fTriglogInputPattern(uInpPat),
   fTriglogReferenceClock(uRefClk)
{
}
TTofTriglogBoard::~TTofTriglogBoard()
{
}
      
void TTofTriglogBoard::Clear(Option_t *option)
{
   TObject::Clear( option );
   fbTriglogFoundInEvent  = kFALSE;
   fMbsTimeSecs           = 0;
   fMbsTimeMillisec       = 0;
   fVulomSyncNumber       = 0;
   fVulomTriggerPattern   = 0;
   fTriglogInputPattern   = 0;
   fTriglogReferenceClock = 0;
}
