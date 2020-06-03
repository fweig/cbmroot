// ---------------------------------------------------------------------------------
// ----- HadaqDefines source file                                              -----
// -----                                                                       -----
// ----- created by C. Simon on 2014-03-06                                     -----
// -----                                                                       -----
// ----- based on stream by S. Linev                                           -----
// ----- https://subversion.gsi.de/go4/app/stream/framework/hadaq/defines.cxx  -----
// ----- revision 1156, 2014-02-14                                             -----
// ---------------------------------------------------------------------------------

#include "HadaqDefines.h"

#include "TString.h"

#include "FairLogger.h"
#include <sys/time.h>     // for function gettimeofday, and struct timeval (!!Unix!!)
#include <time.h>         // for type time_t, functions time and gmtime, and struct tm

void hadaq::RawEvent::Dump()
{
   LOG(info)<<Form("*** Event #0x%06x fullid=0x%04x size %u ***",
                   (UInt_t) GetSeqNr(),
                   (UInt_t) GetId(),
                   (UInt_t) GetSize()
                  );
}

void hadaq::RawEvent::InitHeader(UInt_t id)
{
   tuDecoding = EvtDecoding_64bitAligned;
   SetId(id);
   SetSize(sizeof(hadaq::RawEvent));
   // timestamp at creation of structure:
   time_t tempo = time(NULL);
   struct tm* gmTime = gmtime(&tempo);
   UInt_t date = 0, clock = 0;
   date |= gmTime->tm_year << 16;
   date |= gmTime->tm_mon << 8;
   date |= gmTime->tm_mday;
   SetDate(date);
   clock |= gmTime->tm_hour << 16;
   clock |= gmTime->tm_min << 8;
   clock |= gmTime->tm_sec;
   SetTime(clock);
}

UInt_t hadaq::RawEvent::CreateRunId()
{
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return tv.tv_sec - hadaq::HADAQ_TIMEOFFSET;
}

// ===========================================================

void hadaq::RawSubevent::Dump(Bool_t print_raw_data)
{
   LOG(info)<<Form("   *** Subevent size %u decoding 0x%06x id 0x%04x trig 0x%08x %s align %u ***",
                   (UInt_t) GetSize(),
                   (UInt_t) GetDecoding(),
                   (UInt_t) GetId(),
                   (UInt_t) GetTrigNr(),
                   IsSwapped() ? "swapped" : "not swapped",
                   (UInt_t) Alignment()
                  );

   if (!print_raw_data) return;

   Bool_t newline = true;

   UInt_t size = ((GetSize() - sizeof(RawSubevent)) / Alignment());
   UInt_t width = 2;
   if (size>=100) width = 3;
   if (size>=1000) width = 4;

   for (UInt_t ix=0; ix < size; ix++)
   {
      if (ix % 8 == 0) LOG(info)<<Form("  "); newline = false;

      LOG(info)<<Form("  [%*u] %08x", width, ix, (UInt_t) Data(ix));

      if (((ix + 1) % 8) == 0)  { LOG(info); newline = true; }
   }

   if (!newline) LOG(info);
}
