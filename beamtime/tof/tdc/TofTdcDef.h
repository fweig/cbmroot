// ------------------------------------------------------------------
// -----                     TofTdcDef                          -----
// -----              Created 22/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TOFTDCDEF_H_
#define TOFTDCDEF_H_

#include "TString.h"

namespace toftdc
{
   enum TdcTypes {
      undef     = 0,  // unknown TDC type, default value
      caenV1290 = 1,  // CAEN v1290A and CAEN v1290N VME boards based on HPTDC chips
      vftx      = 2,  // FPGA TDC of the VFTX family (E. Bayer dev.)
      trb       = 3,  // FPGA TDC of the HADES TRB3 family
      get4      = 4,  // GET4 chips
      NbTdcTypes      // Nb of Tdc types, require indices to be consecutive (no gap)
   };
   
   // Default Maximal number of hits per channel per event stored
   const UInt_t kuDefNbMulti = 8;
   
   // Names for Histos
   // !! Has to be adapted to match in initializer size the number of entries in TdcTypes!!!
   const TString ksTdcHistName[NbTdcTypes] = { "default", "v1290", "vftx", "trb", "get4"};
   
   // Names for parameters
   // !! Has to be adapted to match in initializer size the number of entries in TdcTypes!!!
   const TString ksTdcParName[NbTdcTypes] = { "Def", "Caen", "Vftx", "Trb", "Get4"};
}

#endif // TOFTDCDEF_H_   
