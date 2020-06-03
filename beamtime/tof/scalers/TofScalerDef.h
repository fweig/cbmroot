// ------------------------------------------------------------------
// -----                     TofScalerDef                          -----
// -----              Created 21/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TOFSCALERDEF_H_
#define TOFSCALERDEF_H_

#include "TString.h"

namespace tofscaler
{
   enum ScalerTypes {
      undef       = 0,  // unknown TDC type, default value
      triglog     = 1,  // TRIGger LOGic on boards of the VULOM family (J. Fruehauf dev.)
      scalormu    = 2,  // Scaler Or Multiplicty on boards of the VULOM family (J. Fruehauf dev.)
      scalormubig = 3,  // Scaler Or Multiplicty on boards of the VULOM family (J. Fruehauf dev.) with more channels
      scaler2014  = 4,  // Scalers on boards of the VULOM family (J. Fruehauf dev.) with 32 input channels and 16 And scaler
      triglogscal = 5,  // TRIGLOG used as Scalers on boards of the VULOM family (J. Fruehauf dev.) with 16 IN, 16 LMU and 16 OUT scalers
      orgen       = 6,  // VULOM Or Generation board (nov 2015) with 32 input channels, 16 ECl output scalers and 1-2 LEM output scalers
      NbScalerTypes   // Nb of Scaler types, require indices to be consecutive (no gap)
   };
   
   // Names for Histos
   // !! Has to be adapted to match in initializer size the number of entries in TdcTypes!!!
   const TString ksTdcHistName[NbScalerTypes] = { "default", "triglog", "scalormu", "scalormubig", "scaler2014", "trigscal", "orgen"};
   
   // Names for parameters
   // !! Has to be adapted to match in initializer size the number of entries in TdcTypes!!!
   const TString ksTdcParName[NbScalerTypes] = { "Def", "Triglog", "Scalormu", "ScalormuBig", "Scaler2014", "TrigScal", "OrGen"};
}

#endif // TOFSCALERDEF_H_   
