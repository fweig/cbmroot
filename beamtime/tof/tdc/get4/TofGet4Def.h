// ------------------------------------------------------------------
// -----                     TofGet4Def                         -----
// -----              Created 22/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TOFGET4DEF_H_
#define TOFGET4DEF_H_

#include "Rtypes.h"

// "//<- XXXX" shows to which preproc definition in original GO4 unpacker the constant correspond
namespace get4tdc
{
   const UInt_t kuNbChan = 8; // <- NB_CHAN_GET4
   // Size of one clock cycle (=1 coarse bin)
   const Double_t kdClockCycleSize  = 6400.0; //[ps]
   // TODO:For now make 100ps default, maybe need later an option for it
   const Double_t kdTotBinSize      = 100.0; //ps
   
   // Maximal number of hits per channel per event stored
   // More than for VFTX as free streaming can induce maby extra data points
   const UInt_t kuNbMulti = 16;

   // Finetime need to be inverted after calibration as it count from next clock cycle
   const Bool_t   kbInvertFt = kFALSE;
   
   const Int_t kiFineTime    = 0x0000007F; // Fine Counter value <- NB_BIN_GET4_FTS
   const Int_t kiFtShift     =          0; // Fine Counter offset
   const Int_t kiCoarseTime  = 0x0007FF80; // Coarse Counter value
   const Int_t kiCtShift     =          7; // Coarse Counter offset
   
   const Int_t kiCoarseCounterSize  = ( (kiCoarseTime>>kiCtShift)+1 ); 
   const UInt_t kuCoarseOverflowTest = kiCoarseCounterSize / 2 ; // Limit for overflow check

   const Double_t kdBinSize     = kdClockCycleSize / Double_t(kiFineTime + 1); // <- GET4_BIN_SIZE_IN_PS
   const Int_t    kiEpochInBins = kiFineTime + kiCoarseTime + 1; // <- GET4_EPOCH_IN_BINS
   const Double_t kdEpochInPs   = kiEpochInBins*kdBinSize; // alternatively: (kiCoarseTime>>kiCtShift + 1)*kdClockCycleSize // <- GET4_EPOCH_IN_PS
}

namespace get4v10 {
   const UInt_t kuNbChan = get4tdc::kuNbChan/2; // <- NB_CHAN_GET4

   enum SysMessageTypes {
      SYSMSG_GET4V1_32BIT_0  = 240,     // Get4 V1.0, 32bit mode, Raw messages chip 0 link 1
      SYSMSG_GET4V1_32BIT_1  = 241,     // Get4 V1.0, 32bit mode, Raw messages chip 1 link 1
      SYSMSG_GET4V1_32BIT_2  = 242,     // Get4 V1.0, 32bit mode, Raw messages chip 2 link 1
      SYSMSG_GET4V1_32BIT_3  = 243,     // Get4 V1.0, 32bit mode, Raw messages chip 3 link 1
      SYSMSG_GET4V1_32BIT_4  = 244,     // Get4 V1.0, 32bit mode, Raw messages chip 4 link 1
      SYSMSG_GET4V1_32BIT_5  = 245,     // Get4 V1.0, 32bit mode, Raw messages chip 5 link 1
      SYSMSG_GET4V1_32BIT_6  = 246,     // Get4 V1.0, 32bit mode, Raw messages chip 6 link 1
      SYSMSG_GET4V1_32BIT_7  = 247,     // Get4 V1.0, 32bit mode, Raw messages chip 7 link 1
      SYSMSG_GET4V1_32BIT_8  = 248,     // Get4 V1.0, 32bit mode, Raw messages chip 0 link 2
      SYSMSG_GET4V1_32BIT_9  = 249,     // Get4 V1.0, 32bit mode, Raw messages chip 1 link 2
      SYSMSG_GET4V1_32BIT_10 = 250,     // Get4 V1.0, 32bit mode, Raw messages chip 2 link 2
      SYSMSG_GET4V1_32BIT_11 = 251,     // Get4 V1.0, 32bit mode, Raw messages chip 3 link 2
      SYSMSG_GET4V1_32BIT_12 = 252,     // Get4 V1.0, 32bit mode, Raw messages chip 4 link 2
      SYSMSG_GET4V1_32BIT_13 = 253,     // Get4 V1.0, 32bit mode, Raw messages chip 5 link 2
      SYSMSG_GET4V1_32BIT_14 = 254,     // Get4 V1.0, 32bit mode, Raw messages chip 6 link 2
      SYSMSG_GET4V1_32BIT_15 = 255,     // Get4 V1.0, 32bit mode, Raw messages chip 7 link 2
   };

   // Hardware max Number
   const UInt_t kuMaxRoc     = 15; // <- MAX_ROC
   const UInt_t kuMaxGet4    = 32; // <- MAX_GET4
   const UInt_t kuMaxGet4Roc = 16; // <- MAX_GET4_PER_ROC

   // Max nb of each external trigger signal type
   const UInt_t kuMaxAux     =  4; // <- MAX_AUX
   const UInt_t kuMaxSync    =  2; // <- MAX_SYNC

   // Nb of ROC message to be printed?
   const Int_t kiGet4MessPr =  6; // <- GET4_PRINT

   // Size of the epoch counters in ROC messages
   const ULong64_t kulMainEpochCycleSz = 0xFFFFFFFFuLL; // <- GET4_EPOCH_CYCLE_SIZE
   const ULong64_t kulGet4EpochCycleSz = 0xFFFFFFFFuLL; // <- MAIN_EPOCH_CYCLE_SIZE

   // Size of the SYNC cycles (aka nb of epoch between 2 epochs with SYNC flag)
   const Int_t kiSyncCycleSzMain =  40; // <- MAIN_CLK_SYNC_SIZE
   const Int_t kiSyncCycleSzGet4 =  25; // <- GET4_SYNC_CYCLE_SIZE

   // Self triggering
   const Int_t kiMaxNbDataTriggs = 24; // <- MAX_DATA_TRIGG
   const Int_t kiMaxNbSecTriggCh =  8; // <- MAX_SEC_TRIG
}

#endif // TOFGET4DEF_H_   
