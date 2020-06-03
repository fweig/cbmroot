#ifndef SCALERS_EVENT_H
#define SCALERS_EVENT_H


/*
 * Put all definitions here and use them in processor,
 * since they are necessary for data structure of event
 */

#include "MbsCrateDefines.h"
#ifdef WITHGO4ROC
   #include "TGo4Version.h"
   #if __GO4BUILDVERSION__ > 40502
      #include "go4iostream.h"
   #endif
   #include "TTriglogEvent.h"
#else
   #include <vector>
   #include "Riostream.h"
#endif // WITHGO4ROC
#include "TObject.h"

class Scalers_Event : public TObject {
   public:
      Scalers_Event() : 
         TObject(),
         fDTimeSinceFirstEventSecondsTriglog(0.0),
         fDTimeSinceLastEventSecondsTriglog(0.0),
         fDTimeSinceFirstEventSecondsScalOrMu(0.0),
         fDTimeSinceLastEventSecondsScalOrMu(0.0)
         { Clear(); };
      ~Scalers_Event(){ Clear(); };

      /** Method called by the framework to clear the event element. */
      void Clear(Option_t* ="");

      // Oct 12
      Double_t fDTimeSinceFirstEventSecondsTriglog;
      Double_t fDTimeSinceLastEventSecondsTriglog;

      // Starting Nov 12
      Double_t fDTimeSinceFirstEventSecondsScalOrMu;
      Double_t fDTimeSinceLastEventSecondsScalOrMu;

      Double_t fDTriglogRate[NUM_SCALERS][N_SCALERS_CH];
      Double_t fDScalOrMuRate[SCALORMU_NB_SCAL];

      Double_t fDDetectorRate[SCALORMU_NB_SCAL];

   ClassDef(Scalers_Event,1)
};
#endif //SCALERS_EVENT_H



