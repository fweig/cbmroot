#ifndef PLASTICS_EVENT_H
#define PLASTICS_EVENT_H

#ifdef WITHGO4ROC
   #include "TGo4EventElement.h"
#else
   #include "Riostream.h"
   #include "TObject.h"
   #include <vector>
#endif // WITHGO4ROC

#define NB_PLASTICS_MAX         4 // Maximal Nb of plastics which can be defined

class Plastics_Hit: public TObject {
   public:
     Double_t dTimeLeft;  // in ps
     Double_t dTotLeft;   // in ps
     Double_t dTimeRight; // in ps
     Double_t dTotRight;  // in ps
     Bool_t   fbMultiEdge;

     Plastics_Hit() : 
         TObject(),
         dTimeLeft(0.0),
         dTotLeft(0.0),
         dTimeRight(0.0),
         dTotRight(0.0),
         fbMultiEdge(kFALSE)
         { Clear(); };
     virtual ~Plastics_Hit(){ Clear(); };

     virtual void Clear(Option_t *t="");

     Bool_t IsMultiEdge() const { return fbMultiEdge; };

     /*
     // copy constructor, used by vector to copy content for sorting
     Plastics_Hit(const Plastics_Hit& src) :
        iStrip(src.iStrip),
        dTimeLeft(src.dTimeLeft), dTotLeft(src.dTotLeft),
        dTimeRight(src.dTimeRight), dTotRight(src.dTotRight) {}
        */

     ClassDef(Plastics_Hit, 2)
};

class Plastics_Event : public TObject {
   public:


      Plastics_Event(): 
         TObject(),
         fuEventNumber(0),
         fbMultiEdgesPresent(kFALSE)
         { Clear(); };
      virtual ~Plastics_Event(){ Clear(); };

      /** Method called by the framework to clear the event element. */
      virtual void Clear(Option_t *t="");
      Bool_t IsMultiEdge() const { return fbMultiEdgesPresent; };

      // Maybe to be transformed into a TObjArray or TCloneArray
      std::vector<Plastics_Hit>  fHits[NB_PLASTICS_MAX];
      UInt_t                     fuEventNumber;
      Bool_t                     fbMultiEdgesPresent;

   ClassDef(Plastics_Event,2)
};

#endif //PLASTICS_EVENT_H
