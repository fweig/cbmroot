// ------------------------------------------------------------------
// -----                     TofCaenDef                       -----
// -----              Created 22/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TOFCAENDEF_H_
#define TOFCAENDEF_H_

namespace caentdc
{
   const UInt_t kuNbChan = 32;
   // Size of one clock cycle (=1 coarse bin)
   const Double_t kdClockCycleSize  = 25000.0; //[ps]
   
   // Finetime need to be inverted after calibration as it count from next clock cycle
   const Bool_t   kbInvertFt = kFALSE;
   
   const Int_t kiFineTime    = 0x00000000; // Fine Counter value
   const Int_t kiFtShift     =          0; // Fine Counter offset
   const Int_t kiCoarseTime  = 0x00000000; // Coarse Counter value
   const Int_t kiCtShift     =          0; // Coarse Counter offset
   
   const Int_t kiCoarseCounterSize  = ( (kiCoarseTime>>kiCtShift)+1 ); 
   const UInt_t kuCoarseOverflowTest = kiCoarseCounterSize / 2 ; // Limit for overflow check
}

#endif // TOFCAENDEF_H_   
