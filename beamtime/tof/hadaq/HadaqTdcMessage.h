// -------------------------------------------------------------------------------
// ----- HadaqTdcMessage header file                                         -----
// -----                                                                     -----
// ----- created by C. Simon on 2014-04-02                                   -----
// -----                                                                     -----
// ----- based on stream by S. Linev                                         -----
// ----- https://subversion.gsi.de/go4/app/stream/include/hadaq/TdcMessage.h -----
// ----- revision 1060, 2013-11-28                                           -----
// -------------------------------------------------------------------------------

#ifndef HADAQ_TDCMESSAGE_H
#define HADAQ_TDCMESSAGE_H

#include "Rtypes.h"

#include "TofTrbTdcDef.h"

namespace hadaq {

   /** TdcMessage is wrapper for data, produced by FPGA-TDC
    * struct is used to avoid any potential overhead */

   struct TdcMessage {
      protected:
         UInt_t   fData;

         static UInt_t gFineMinValue;
         static UInt_t gFineMaxValue;

      public:

         TdcMessage() : fData(0) {}

         TdcMessage(UInt_t d) : fData(d) {}

         void assign(UInt_t d) { fData=d; }

         // methods for message type determination

         /** Returns kind of the message (upper 3 bits of the TDC header word) */
         UInt_t getKind() const { return fData & trbtdc::message_Mask; }

         /** If used for the time message (MSb set), four different values can be returned */
         Bool_t isTimeMsg() const { return fData & trbtdc::message_Time; }

         Bool_t isEpochMsg() const { return getKind() == trbtdc::message_Epoch; }
         Bool_t isDebugMsg() const { return getKind() == trbtdc::message_Debug; }
         Bool_t isHeaderMsg() const { return getKind() == trbtdc::message_Header; }
         Bool_t isReservedMsg() const { return getKind() == trbtdc::message_Reserved; }

         // methods for HEADER word

         /** Return error bits of header message */
         UInt_t getHeaderErr() const { return (fData >> trbtdc::header_ErrorBitShift) & trbtdc::header_ErrorBitMask; }

         /** Return trigger code bits of header message */
         UInt_t getHeaderTrig() const { return (fData >> trbtdc::header_TriggerCodeBitShift) & trbtdc::header_TriggerCodeBitMask; }

         /** Return reserved bits of header message */
         UInt_t getHeaderRes() const { return (fData >> trbtdc::header_ReservedBitShift) & trbtdc::header_ReservedBitMask; }

         // methods for TIME word

         /** Returns time word coarse time counter */
         UInt_t getTimeTmCoarse() const { return (fData >> trbtdc::time_CoarseTimeBitShift) & trbtdc::time_CoarseTimeBitMask; }

         /** Returns time word edge */
         UInt_t getTimeEdge() const {  return (fData >> trbtdc::time_EdgeBitShift) & trbtdc::time_EdgeBitMask; }

         /** Returns time word fine time counter */
         UInt_t getTimeTmFine() const { return (fData >> trbtdc::time_FineTimeBitShift) & trbtdc::time_FineTimeBitMask; }

         /** Returns time word channel ID */
         UInt_t getTimeChannel() const { return (fData >> trbtdc::time_ChannelBitShift) & trbtdc::time_ChannelBitMask; }

         /** Returns time word reserved value */
         UInt_t getTimeReserved() const { return (fData >> trbtdc::time_ReservedBitShift) & trbtdc::time_ReservedBitMask; }

         /** Returns time stamp, which is simple combination coarse and fine counter */
         UInt_t getTimeTmStamp() const { return (getTimeTmCoarse() << trbtdc::size_FineTimeCounter) | getTimeTmFine(); }

         Bool_t isHitRisingEdge() const { return getTimeEdge() == trbtdc::edge_Rising; }
         Bool_t isHitFallingEdge() const { return getTimeEdge() == trbtdc::edge_Falling; }

         // methods for EPOCH word

         /** Return Epoch for epoch marker */
         UInt_t getEpochValue() const { return (fData >> trbtdc::epoch_EpochBitShift) & trbtdc::epoch_EpochBitMask; }
         /** Get reserved bit for epoch */
         UInt_t getEpochRes() const { return (fData >> trbtdc::epoch_ReservedBitShift) & trbtdc::epoch_ReservedBitMask; }

         void print(Double_t tm = -1.);

         static Double_t CoarseUnit() { return trbtdc::kdClockCycleSizeSec; }

         static Double_t SimpleFineCalibr(UInt_t fine)
         {
            if (fine < gFineMinValue) return 0.;
            if (fine > gFineMaxValue) return 1.;
            return (1.*(fine - (gFineMinValue - 1) - 0.5)/(gFineMaxValue - gFineMinValue + 1));
         }

         /** Method set static limits, which are used for simple interpolation of time for fine counter */
         static void SetFineLimits(UInt_t min, UInt_t max)
         {
            gFineMinValue = min;
            gFineMaxValue = max;
         }

         static void CoarseOverflow(Long64_t& liEdgeCoarseTime, const Long64_t& liRefCoarseTime)
         {
           // hit arrived prior to reference hit but the full coarse counter overflowed in between
           // time difference will yield a huge positive number
           if(liEdgeCoarseTime - liRefCoarseTime > trbtdc::kliFullCoarseSize/2)
           {
             liEdgeCoarseTime -= trbtdc::kliFullCoarseSize;
           }
           // hit arrived posterior to reference hit but the full coarse counter overflowed in between
           // time difference will yield a huge negative number
           else if(liEdgeCoarseTime - liRefCoarseTime < -trbtdc::kliFullCoarseSize/2)
           {
             liEdgeCoarseTime += trbtdc::kliFullCoarseSize;
           }
         }

      };
}

#endif
