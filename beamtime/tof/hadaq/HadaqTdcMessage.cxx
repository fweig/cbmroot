// -----------------------------------------------------------------------------------
// ----- HadaqTdcMessage source file                                             -----
// -----                                                                         -----
// ----- created by C. Simon on 2014-04-02                                       -----
// -----                                                                         -----
// ----- based on stream by S. Linev                                             -----
// ----- https://subversion.gsi.de/go4/app/stream/framework/hadaq/TdcMessage.cxx -----
// ----- revision 1046, 2013-11-20                                               -----
// -----------------------------------------------------------------------------------

#include "HadaqTdcMessage.h"

#include "TString.h"

#include "FairLogger.h"

UInt_t hadaq::TdcMessage::gFineMinValue = trbtdc::kuSimpleFineTimeMinValue;
UInt_t hadaq::TdcMessage::gFineMaxValue = trbtdc::kuSimpleFineTimeMaxValue;

void hadaq::TdcMessage::print(Double_t tm)
{
   switch (getKind()) {
      case trbtdc::message_Reserved:
         LOG(debug)<<Form("     tdc reserv 0x%08x", (UInt_t) fData);
         break;

      case trbtdc::message_Header:
         LOG(debug)<<Form("     tdc head   0x%08x", (UInt_t) fData);
         break;
      case trbtdc::message_Debug:
         LOG(debug)<<Form("     tdc debug  0x%08x", (UInt_t) fData);
         break;
      case trbtdc::message_Epoch:
         LOG(debug)<<Form("     tdc epoch  0x%08x", (UInt_t) fData);
         if (tm>=0) LOG(debug)<<Form("  tm:%9.2f", tm*1e9);
         LOG(debug)<<Form("   epoch 0x%x", (UInt_t) getEpochValue());
         break;
      case trbtdc::message_Time:
      case trbtdc::message_Time1:
      case trbtdc::message_Time2:
      case trbtdc::message_Time3:
         LOG(debug)<<Form("     tdc hit    0x%08x", (UInt_t) fData);
         if (tm>=0) LOG(debug)<<Form("  tm:%9.2f", tm*1e9);
         LOG(debug)<<Form("   ch %3u isrising:%u tc 0x%03x tf 0x%03x",
                 (UInt_t) getTimeChannel(), (UInt_t)getTimeEdge(),
                 (UInt_t)getTimeTmCoarse(), (UInt_t)getTimeTmFine());
         break;
      default:
         LOG(debug)<<Form("     tdc unkn   0x%08x   kind %u", (UInt_t) fData, (UInt_t) getKind());
         break;
   }
}
