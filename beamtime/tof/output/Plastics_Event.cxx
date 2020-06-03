
#include "Plastics_Event.h"

#ifdef WITHGO4ROC
   #include "TGo4Version.h"
   #if __GO4BUILDVERSION__ > 40502
      #include "go4iostream.h"
   #else
      #include "Riostream.h"
   #endif
#endif // WITHGO4ROC

void Plastics_Hit::Clear(Option_t * /*t*/)
{
   dTimeLeft  = -1 ;
   dTotLeft   = -1 ;
   dTimeRight = -1 ;
   dTotRight  = -1 ;
   fbMultiEdge = kFALSE;
}

void  Plastics_Event::Clear(Option_t * /*t*/)
{
  // all members should be cleared.
   for(Int_t iPlastic = 0; iPlastic<NB_PLASTICS_MAX; iPlastic++)
      fHits[iPlastic].clear();
   fbMultiEdgesPresent = kFALSE;
}
