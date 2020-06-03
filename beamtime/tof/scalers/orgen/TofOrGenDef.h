// ------------------------------------------------------------------
// -----                     TofOrGenDef                        -----
// -----              Created 22/11/2015 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TOFORGENDEF_H_
#define TOFORGENDEF_H_

#include "TString.h"

/******** VULOM3/4:OrGen CERN November 2015 data unpacking definitions *********/

namespace orgen
{
   const UInt_t kuNbScalers  = 1;
   const UInt_t kuNbChanIn   = 32;
   const UInt_t kuNbChanEclo = 16;
   const UInt_t kuNbChanLemo = 1;
   const UInt_t kuNbChan     = kuNbChanIn + kuNbChanEclo + kuNbChanLemo;
   const Double_t kdRefClkFreq = 781250.0;
   const Double_t kdEvoRange = 3*3600; // s
   const Double_t kdEvoBin   =    1;  // s
}

#endif // TOFORGENDEFDEF_H_
