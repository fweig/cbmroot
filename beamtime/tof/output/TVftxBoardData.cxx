
#include "TVftxBoardData.h"

#ifndef WITHGO4ROC
   #define NB_BIN_FTS      1024
   #define CLOCK_TIME      5000 //ps
#endif // WITHGO4ROC

void TVftxBoardData::Clear(Option_t*)
{
   iTriggerTime = 0;
   for( Int_t iChan = 0; iChan < FPGA_TDC_NBCHAN; iChan++)
   {
      iMultiplicity[iChan] = 0;
      for( Int_t iHit = 0; iHit < MaxMult; iHit++)
      {
         iFineTime[iChan][iHit]     = -1;
         iCoarseTime[iChan][iHit]   = -1;
         uFutureBit[iChan][iHit]    =  0;
         dTimeCorr[iChan][iHit]     = -1;
         dFineTimeCorr[iChan][iHit] = -1;
      }
   }
}
Double_t TVftxBoardData::GetCalibratedTime( UInt_t uChan, UInt_t uMult)
{
   // Return either the calibrated time
   // when possible, otherwise 0
   if( uChan < FPGA_TDC_NBCHAN && uMult < MaxMult )
      return dTimeCorr[uChan][uMult];
      else return 0.0;
}
Double_t TVftxBoardData::GetCoarseCorrectedTime( UInt_t uChan, UInt_t uMult)
{
   // Return the calibrated time with a correction for coarse counter overflow relative to trigger
   // when possible, otherwise 0
   if( uChan < FPGA_TDC_NBCHAN && uMult < MaxMult )
   {
      Double_t dCoarseCorrectedTime = dTimeCorr[uChan][uMult];

      if( VFTX_COARSE_OVERFLW_CHK < iCoarseTime[ uChan ][ uMult ] - iTriggerTime )
         dCoarseCorrectedTime -= ((TDC_FIFO_COARSE_CT>>TDC_FIFO_COARSE_CT_SHIFT)+1)*(Double_t)CLOCK_TIME;
      if( iCoarseTime[ uChan ][ uMult ] - iTriggerTime  < -1*VFTX_COARSE_OVERFLW_CHK )
         dCoarseCorrectedTime += ((TDC_FIFO_COARSE_CT>>TDC_FIFO_COARSE_CT_SHIFT)+1)*(Double_t)CLOCK_TIME;

      return dCoarseCorrectedTime;
   }
      else return 0.0;
}
