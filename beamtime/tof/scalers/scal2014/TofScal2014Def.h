// ------------------------------------------------------------------
// -----                     TofScal2014Def                         -----
// -----              Created 19/04/2014 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TOFSCAL2014DEF_H_
#define TOFSCAL2014DEF_H_

#include "TString.h"

/******** VULOM3:ScalOrAnd Gsi April 2014 data unpacking definitions *********/

namespace scaler2014
{
   const UInt_t kuNbScalers = 1;
   const UInt_t kuNbChanIn  = 32;
   const UInt_t kuNbChanAnd = 16;
   const UInt_t kuNbChan    = kuNbChanIn + kuNbChanAnd;
   const Double_t kdRefClkFreq = 781250.0;
   const Double_t kdEvoRange = 3*3600; // s
   const Double_t kdEvoBin   =    1;  // s
}

#endif // TOFSCAL2014DEF_H_
