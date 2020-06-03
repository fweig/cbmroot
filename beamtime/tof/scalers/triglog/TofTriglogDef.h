// ------------------------------------------------------------------
// -----                     TofTriglogDef                      -----
// -----              Created 21/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TOFTRIGLOGDEF_H_
#define TOFTRIGLOGDEF_H_

#include "TString.h"

/******************** VULOM3:TRIGLOG data unpacking definitions *************/
namespace triglog
{
   const UInt_t kuNbScalers = 3;
   const UInt_t kuNbChan    = 16;
   const Double_t kdRefClkFreq = 781250.0;
   const Double_t kdEvoRange = 3*3600; // s
   const Double_t kdEvoBin   =    1;  // s
}

#endif // TOFTRIGLOGDEF_H_   
