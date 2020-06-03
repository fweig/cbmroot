//-----------------------------------------------------------
//-----------------------------------------------------------

#ifndef CbmKFParticleInterface_HH
#define CbmKFParticleInterface_HH

#include "TObject.h"

class CbmStsTrack;
class KFParticle;
class FairTrackParam;
class CbmVertex;

class CbmKFParticleInterface : public TObject {
 public:

  // Constructors/Destructors ---------
  CbmKFParticleInterface() {};
  ~CbmKFParticleInterface() {};
  
  static void SetKFParticleFromStsTrack(CbmStsTrack *track, KFParticle* particle, Int_t pdg = 211, Bool_t firstPoint = kTRUE);
  static void ExtrapolateTrackToPV(const CbmStsTrack *track, CbmVertex* pv, FairTrackParam* paramAtPV, float& chiPrim);
  
 private:  
   
  const CbmKFParticleInterface& operator = (const CbmKFParticleInterface&);
  CbmKFParticleInterface(const CbmKFParticleInterface&);
  
  ClassDef(CbmKFParticleInterface,1);
};

#endif
