// ------------------------------------------------------------------
// -----                     TofScomDef                         -----
// -----              Created 21/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TOFSCOMDEF_H_
#define TOFSCOMDEF_H_

#include "TString.h"

/******************** VULOM3:ScalOrMu data unpacking definitions *************/
namespace scalormu
{
   const UInt_t kuNbScalers = 1;
   const UInt_t kuNbChan    = 24;
   const Double_t kdRefClkFreq = 781250.0;
   const Double_t kdEvoRange = 3600; // s
   const Double_t kdEvoBin   =    0.1;  // s
}
namespace scalormuBig
{
   const UInt_t kuNbScalers = 1;
   const UInt_t kuNbChan    = 32;
   const Double_t kdRefClkFreq = 781250.0;
   const Double_t kdEvoRange = 3600; // s
   const Double_t kdEvoBin   =    0.1;  // s
}

#endif // TOFSCOMDEF_H_   
