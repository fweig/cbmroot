// -----------------------------------------------------------------------------
// ----- TofTrbTdcDef header file                                          -----
// -----                                                                   -----
// ----- created by C. Simon on 2014-04-02                                 -----
// -----                                                                   -----
// ----- based on TofTrb3Def by P.-A. Loizeau                              -----
// ----- https://subversion.gsi.de/fairroot/cbmroot/development/ploizeau/  -----
// -----   main/unpack/tof/tdc/trb3/TofTrb3Def.h                           -----
// ----- revision 20754, 2013-07-17                                        -----
// -----------------------------------------------------------------------------

#ifndef TOFTRBTDCDEF_H_
#define TOFTRBTDCDEF_H_

namespace trbtdc
{
   enum TrbTdcMessage {
      message_Mask     = 0xe0000000,
      message_Reserved = 0x00000000,
      message_Header   = 0x20000000,
      message_Debug    = 0x40000000,
      message_Epoch    = 0x60000000,
      message_Time     = 0x80000000,
      message_Time1    = 0xa0000000,
      message_Time2    = 0xc0000000,
      message_Time3    = 0xe0000000
   };

   enum TrbTdcHeaderWord {
      header_ErrorBitMask        = 0x0000FFFF,
      header_ErrorBitShift       =          0,
      header_TriggerCodeBitMask  = 0x000000FF,
      header_TriggerCodeBitShift =         16,
      header_ReservedBitMask     = 0x0000001F,
      header_ReservedBitShift    =         24
   };

   enum TrbTdcTimeWord {
      time_CoarseTimeBitMask   = 0x000007FF,
      time_CoarseTimeBitShift  =          0,
      time_EdgeBitMask         = 0x00000001,
      time_EdgeBitShift        =         11,
      time_FineTimeBitMask     = 0x000003FF,
      time_FineTimeBitShift    =         12,
      time_ChannelBitMask      = 0x0000007F,
      time_ChannelBitShift     =         22,
      time_ReservedBitMask     = 0x00000003,
      time_ReservedBitShift    =         29
   };

   enum TrbTdcEpochWord {
      epoch_EpochBitMask        = 0x0FFFFFFF,
      epoch_EpochBitShift       =          0,
      epoch_ReservedBitMask     = 0x00000001,
      epoch_ReservedBitShift    =         28
   };

  /*
    enable debug mode: 0xc800 - bit 4
    default: not enabled (to save bandwidth for time messages)
             not read out (actually no debug read-out implemented yet)
  */
   enum TrbTdcDebugWord {
      debug_InfoBitMask  = 0x00FFFFFF,
      debug_InfoBitShift =          0,
      debug_ModeBitMask  = 0x0000001F,
      debug_ModeBitShift =         24
   };

  /*
    currently not read out
  */
   enum TrbTdcReservedWord {
      reserved_ReservedBitMask  = 0x1FFFFFFF,
      reserved_ReservedBitShift =          0
   };

   enum TrbTdcEdgeFlag {
      edge_Rising  = 0x1,
      edge_Falling = 0x0
   };

  /*
    did not find a straightforward way to extract the number of active bits
    from the bit masks of fine time, coarse time and epoch data fields
  */
   enum TrbTdcCounterSize {
      size_FineTimeCounter = 10,
      size_CoarseTimeCounter = 11,
      size_EpochCounter = 28
   };

   enum TrbTdcProcessStatus {
     process_Success = 0,
     process_FirstNoHeader = 1,
     process_SecondNoEpoch = 2,
     process_ThirdNoTime = 3,
     process_ThirdNoRefTime = 4,
     process_ThirdBadRefTime = 5,
     process_FourthNoEpoch = 6,
     process_BadChannelNumber = 7,
     process_ChannelButNoEpochChange = 8,
     process_TwoConsecutiveEpochs = 9,
     process_RingBufferOverwrite = 10,
     process_BadFineTime = 11,
     process_NoEpochIncrement = 12,
     process_TwoIdenticalWords = 13,
     process_StatusMessages
   };

   enum TrbTdcFineInterpolation {
     finetime_bincenter = 0,
     finetime_binedge = 1,
     finetime_linear = 2
   };

   // number of channels in FPGA-TDC modules
   const UInt_t kuNbChan = 64;

   // Maximal number of hits per channel per event stored
   const UInt_t kuNbMulti = 8;

   // Finetime counts back from the next rising clock edge
   const Bool_t kbInvertFt = kTRUE;

   const Int_t kiFineCounterSize  = time_FineTimeBitMask + 1;

   /*
     coarse counter size used in the calibration step
     attention: artificially fixed to 2^30 bits to keep signed 32-bit integer
                covering a coarse time range of 5 s bevor wrap around
                needs to be changed when running in 'triggerless' mode!!!
   */
   const Int_t kiCoarseCounterSize  = 0x3FFFFFFF + 1;
   const UInt_t kuCoarseOverflowTest = kiCoarseCounterSize / 2 ; // Limit for overflow check

   // Size of one clock cycle (=1 coarse bin)
   const Double_t kdClockCycleSize     = 5000.0; // [ps]
   const Double_t kdClockCycleSizeSec  = 5e-9;   // [s]

   // also seen: (20,500) and (31,421)
   const UInt_t kuSimpleFineTimeMinValue = 31;
   const UInt_t kuSimpleFineTimeMaxValue = 500;

   const UInt_t kuHighestReasonableFineTimeBin = 600;

   const UInt_t kuMinimumFineTimeRange = 400;

   const Long64_t kliFullCoarseSize = ((static_cast<Long64_t>(epoch_EpochBitMask)<<size_CoarseTimeCounter)|static_cast<Long64_t>(time_CoarseTimeBitMask))+1;
   const Long64_t kliEpochSize = static_cast<Long64_t>(time_CoarseTimeBitMask)+1;

}

#endif
