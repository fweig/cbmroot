#ifndef RPC_EVENT_H
#define RPC_EVENT_H

#ifdef WITHGO4ROC
   #include "TGo4EventElement.h"
#else
   #include "Riostream.h"
   #include "TObject.h"
   #include <vector>
#endif // WITHGO4ROC

class Rpc_Hit: public TObject {
   public:
     Int_t    iStrip;
     Double_t dTimeLeft;  // in ps
     Double_t dTotLeft;   // in ps
     Double_t dTimeRight; // in ps
     Double_t dTotRight;  // in ps
     Bool_t   fbMultiEdge;

     Rpc_Hit() : 
         TObject(),
         iStrip(-1),
         dTimeLeft(0.0),
         dTotLeft(0.0),
         dTimeRight(0.0),
         dTotRight(0.0),
         fbMultiEdge(kFALSE)
      { Clear(); };

     void Clear(Option_t* ="");

     Double_t PositionCalculation( Double_t dTimeConv );
     Bool_t IsMultiEdge() const { return fbMultiEdge; };

     /*
     // copy constructor, used by vector to copy content for sorting
     Rpc_Hit(const Rpc_Hit& src) :
        iStrip(src.iStrip),
        dTimeLeft(src.dTimeLeft), dTotLeft(src.dTotLeft),
        dTimeRight(src.dTimeRight), dTotRight(src.dTotRight) {}
        */

     ClassDef(Rpc_Hit, 2)
};

class Rpc_Cluster: public TObject {
  public:
      std::vector<Rpc_Hit>  fHits;
      Double_t dX;
      Double_t dY;
      Double_t dMeanTime;
      Double_t dMeanStrip;

     Rpc_Cluster() : 
         TObject(),
         fHits(),
         dX(0.0),
         dY(0.0),
         dMeanTime(0.0),
         dMeanStrip(0.0)
      { Clear(); };

     void Clear(Option_t* ="");

     void PositionCalculation( Double_t dMiddleStrip, UInt_t uOrientation,
                                   Double_t dStripWidth, Double_t dTimeConv,
                                   UInt_t uUseGravityCenter = 0);


   ClassDef(Rpc_Cluster, 1)
};

class Rpc_Event: public TObject {
   public:

      // Maybe to be transformed into a TObjArray or TCloneArray
      std::vector<Rpc_Cluster>  fClusters;
      UInt_t                    fuEventNumber;
      Bool_t                    fbMultiEdgesPresent;

      Rpc_Event() : 
         TObject(),
         fClusters(),
         fuEventNumber(0),
         fbMultiEdgesPresent(kFALSE)
         { Clear(); };
      ~Rpc_Event(){ Clear(); };

      void Clear(Option_t* ="");
      Bool_t IsMultiEdge() const { return fbMultiEdgesPresent; };

    ClassDef(Rpc_Event, 2)
 };
#endif //RPC_EVENT_H
