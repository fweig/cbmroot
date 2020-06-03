// ------------------------------------------------------------------
// -----                     TofVftxDef                         -----
// -----              Created 12/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TOFVFTXDEF_H_
#define TOFVFTXDEF_H_

//#define OLD_VERSION_VULOM // <- Pre-COSY nov11 HD vulom FPGA code
//#define VERSION_VFTX    // <- June   2012 version based on VFTX board (32 chan, 16 true chan+tot)
#define VERSION_VFTX_28 // <- August 2012 version based on VFTX board (56 chan, 28 true chan+tot)

/******************** FPGA TDC data unpacking definitions *************/
namespace vftxtdc
{
   // Maximal number of hits per channel per event stored
   const UInt_t kuNbMulti = 8;
   // number of channels in FPGA TDC modules
#ifdef VERSION_VFTX
   const UInt_t kuNbChan = 32;
#elif defined VERSION_VFTX_28
   const UInt_t kuNbChan = 56;
#else
   const UInt_t kuNbChan = 8;
#endif

   // Size of one clock cycle (=1 coarse bin)
   const Double_t kdClockCycleSize  = 5000.0; //[ps]
   
   // Finetime need to be inverted after calibration as it count from next clock cycle
   const Bool_t   kbInvertFt = kTRUE;

   // Header data word (inserted by MBS program)
   const Int_t kiHeaderKeyMask  = 0xFFFF0000;
   const Int_t kiHeaderKeyShift =         16;
   const Int_t kiHeaderKeyword  =     0xABCD;
   const Int_t kiHeaderModMask  = 0x0000FF00;
   const Int_t kiHeaderModShift =          8;
   const Int_t kiHeaderNbMask   = 0x000000FF;
   
   const UInt_t kuFifoMessageType = 0x80000000; // Message Type: 1 = header, 0 = data
   const Int_t kiFifoMessageTypeShift =    31;

   // FIFO header data word (TDC generated, not modified by MBS)
#ifdef OLD_VERSION_VULOM
   const Int_t kiFifoHeadDataCnt  = 0x000000FF; // Data count
   const Int_t kiFifoHeadDataCntShift  =     0;
   const Int_t kiFifoHeadTrigTime = 0x01FFFC00; // Coarse counter value
   const Int_t kiFifoHeadTrigTimeShift =    10;
   const Int_t kiFifoHeadTrigType = 0x60000000; // Trigger Type
   const Int_t kiFifoHeadTrigTypeShift =    29;
#elif defined VERSION_VFTX
   const Int_t kiFifoHeadDataCnt  = 0x000000FF; // Data count
   const Int_t kiFifoHeadDataCntShift  =     0;
   const Int_t kiFifoHeadTrigTime = 0x01FFF800; // Coarse counter value
   const Int_t kiFifoHeadTrigTimeShift =    11;
   const Int_t kiFifoHeadTrigType = 0x60000000; // Trigger Type
   const Int_t kiFifoHeadTrigTypeShift =    29;
#elif defined VERSION_VFTX_28
   const Int_t kiFifoHeadDataCnt  = 0x000000FF; // Data count
   const Int_t kiFifoHeadDataCntShift  =     0;
   const Int_t kiFifoHeadTrigTime = 0x00FFF800; // Coarse counter value
   const Int_t kiFifoHeadTrigTimeShift =    11;
   const Int_t kiFifoHeadTrigType = 0x60000000; // Trigger Type
   const Int_t kiFifoHeadTrigTypeShift =    29;
#else
   const Int_t kiFifoHeadDataCnt  = 0x000000FF; // Data count
   const Int_t kiFifoHeadDataCntShift  =     0;
   const Int_t kiFifoHeadTrigTime = 0x03FFF800; // Coarse counter value
   const Int_t kiFifoHeadTrigTimeShift =    11;
   const Int_t kiFifoHeadTrigType = 0x60000000; // Trigger Type
   const Int_t kiFifoHeadTrigTypeShift =    29;
#endif

   // FIFO data word (TDC generated, not modified by MBS)
#ifdef OLD_VERSION_VULOM
   const Int_t kiFifoFineTime    = 0x000003FF; // Fine Counter value
   const Int_t kiFifoFtShift     =          0;
   const Int_t kiFifoCoarseTime  = 0x01FFFC00; // Coarse counter value
   const Int_t kiFifoCtShift     =         10;
   const Int_t kiFifoFutureBit   = 0x02000000; // Flag for hits after trigger
   const Int_t kiFiFoFutBitShift =         25;
   const Int_t kiChannel         = 0x7C000000; // Data channel
   const Int_t kiChannelShift    =         26;
#elif defined VERSION_VFTX
   const Int_t kiFifoFineTime    = 0x000007FF; // Fine Counter value
   const Int_t kiFifoFtShift     =          0;
   const Int_t kiFifoCoarseTime  = 0x01FFF800; // Coarse counter value
   const Int_t kiFifoCtShift     =         11;
   const Int_t kiFifoFutureBit   = 0x02000000; // Flag for hits after trigger
   const Int_t kiFiFoFutBitShift =         25;
   const Int_t kiChannel         = 0x7C000000; // Data channel
   const Int_t kiChannelShift    =         26;
#elif defined VERSION_VFTX_28
   const Int_t kiFifoFineTime    = 0x000007FF; // Fine Counter value
   const Int_t kiFifoFtShift     =          0;
   const Int_t kiFifoCoarseTime  = 0x00FFF800; // Coarse counter value
   const Int_t kiFifoCtShift     =         11;
   const Int_t kiFifoFutureBit   = 0x01000000; // Flag for hits after trigger
   const Int_t kiFiFoFutBitShift =         24;
   const Int_t kiChannel         = 0x7E000000; // Data channel
   const Int_t kiChannelShift    =         25;
#else
   const Int_t kiFifoFineTime    = 0x000007FF; // Fine Counter value
   const Int_t kiFifoFtShift     =          0;
   const Int_t kiFifoCoarseTime  = 0x03FFF800; // Coarse counter value
   const Int_t kiFifoCtShift     =         11;
   const Int_t kiFifoFutureBit   = 0x04000000; // Flag for hits after trigger
   const Int_t kiFiFoFutBitShift =         26;
   const Int_t kiChannel         = 0x78000000; // Data channel
   const Int_t kiChannelShift    =         27;
#endif

   const Int_t kiCoarseCounterSize  = ( (kiFifoCoarseTime>>kiFifoCtShift)+1 ); 
   const UInt_t kuCoarseOverflowTest = kiCoarseCounterSize / 2 ; // Limit for overflow check
}
/**************** End FPGA TDC data unpacking definitions *************/

#endif // TOFVFTXDEF_H_   
