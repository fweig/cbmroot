// ---------------------------------------------------------------------------------
// ----- HadaqTrbIterator header file                                          -----
// -----                                                                       -----
// ----- created by C. Simon on 2014-03-07                                     -----
// -----                                                                       -----
// ----- based on stream by S. Linev                                           -----
// ----- https://subversion.gsi.de/go4/app/stream/include/hadaq/TrbIterator.h  -----
// ----- revision 909, 2012-12-17                                              -----
// ---------------------------------------------------------------------------------

#ifndef HADAQ_TRBITERATOR_H
#define HADAQ_TRBITERATOR_H

#include "HadaqDefines.h"

#include "Rtypes.h"

namespace hadaq {

   class TrbIterator {
      protected:
         void* fData;
         UInt_t fDatalen;

         void* fEvCursor;
         UInt_t fEvLen;

         void* fSubCursor;
         UInt_t fSubLen;

         void* fSubsubCursor;
         UInt_t fSubsubLen;

      public:

         TrbIterator(void* data, UInt_t datalen);
         ~TrbIterator() {}

         RawEvent* nextEvent();

         RawEvent* currEvent() const { return (RawEvent*) fEvCursor; }

         RawSubevent* nextSubevent();

         RawSubevent* currSubevent() const { return (RawSubevent*) fSubCursor; }
         
   private:
      // no default Copy constructor and = OP as class not meant to be copied
      TrbIterator(const TrbIterator&);
      TrbIterator& operator=(const TrbIterator&);

   };


}


#endif
