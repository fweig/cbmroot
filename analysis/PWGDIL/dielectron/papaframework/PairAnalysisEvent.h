#ifndef PAIRANALYSISEVENT_H
#define PAIRANALYSISEVENT_H

//#############################################################
//#                                                           #
//#      Class PairAnalysisEvent                                #
//#      Class for event structure and references to tracks   #
//#                                                           #
//#  Authors:                                                 #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch       #
//#                                                           #
//#############################################################
#include <TNamed.h>
#include <TClonesArray.h>
#include <TObjArray.h>

#include "CbmDefs.h"
#include "CbmVertex.h"

class FairRootManager;
class FairMCEventHeader;
class CbmStsTrack;
class PairAnalysisTrack;

class PairAnalysisEvent : public TNamed {
public:
  PairAnalysisEvent();
  PairAnalysisEvent(const char*name, const char* title);

  virtual ~PairAnalysisEvent();

  void SetInput(FairRootManager *man);
  void Init();


  // getters
  FairMCEventHeader *GetMCHeader()     const { return fMCHeader; }
  CbmVertex *GetPrimaryVertex()       const { return fPrimVertex; }
  Int_t      GetNumberOfTracks()      const { return (fTracks ? fTracks->GetEntriesFast() : -1); }
  Int_t      GetNumberOfMCTracks()    const { return (fMCTracks ? fMCTracks->GetEntriesFast() : -1); }
  Int_t      GetNumberOfVageMatches() const { return fMultiMatch; }
  Int_t      GetNumberOfMatches(   ECbmModuleId det) const;
  Int_t      GetNumberOfHits(      ECbmModuleId det) const;
  Int_t      GetNumberOfHitMatches(ECbmModuleId det) const;
  Int_t      GetNumberOfPoints(    ECbmModuleId det) const;

  TClonesArray *GetHits(      ECbmModuleId det) const;
  TClonesArray *GetHitMatches(ECbmModuleId det) const;
  TClonesArray *GetPoints(    ECbmModuleId det) const;

  TClonesArray *GetCluster(   ECbmModuleId det) const;

  TObjArray    *GetTracks()           const { return fTracks; }
  PairAnalysisTrack *GetTrack(UInt_t pos);

  virtual void Clear(Option_t *opt="C");


protected:
  Int_t GetMvdMatchingIndex(CbmStsTrack *track) const;


private:
 FairMCEventHeader *fMCHeader  = NULL; // mc header
 TClonesArray *fMCTracks       = NULL; // mc tracks
 TClonesArray *fStsMatches     = NULL; // STS matches
 TClonesArray *fMuchMatches    = NULL; // MUCH matches
 TClonesArray *fTrdMatches     = NULL; // TRD matches
 TClonesArray *fRichMatches    = NULL; // RICH matches

 TClonesArray *fMvdPoints      = NULL; // MVD points
 TClonesArray *fStsPoints      = NULL; // STS points
 TClonesArray *fMuchPoints     = NULL; // MUCH points
 TClonesArray *fRichPoints     = NULL; // RICH points
 TClonesArray *fTrdPoints      = NULL; // TRD points
 TClonesArray *fTofPoints      = NULL; // TOF points

 TClonesArray *fGlobalTracks   = NULL; // global tracks
 TClonesArray *fTrdTracks      = NULL; // TRD tracks
 TClonesArray *fStsTracks      = NULL; // STS tracks
 TClonesArray *fMuchTracks     = NULL; // MUCH tracks
 TClonesArray *fRichRings      = NULL; // RICH rings

 TClonesArray *fMvdHits        = NULL; // MVD hits
 TClonesArray *fStsHits        = NULL; // STS hits
 TClonesArray *fMuchHits       = NULL; // MUCH pixel hits
 TClonesArray *fMuchHitsStraw  = NULL; // MUCH straw hits
 TClonesArray *fTrdHits        = NULL; // TRD hits
 TClonesArray *fRichHits       = NULL; // RICH hits
 TClonesArray *fTofHits        = NULL; // TOF hits

 TClonesArray *fRichProjection = NULL; // RICH projection

 TClonesArray *fMvdHitMatches  = NULL; // hit matches
 TClonesArray *fStsHitMatches  = NULL; // hit matches
 TClonesArray *fRichHitMatches = NULL; // hit matches
 TClonesArray *fMuchHitMatches = NULL; // hit matches
 TClonesArray *fTrdHitMatches  = NULL; // hit matches
 TClonesArray *fTofHitMatches  = NULL; // hit matches

 TClonesArray *fTrdCluster     = NULL; // TRD cluster

 CbmVertex    *fPrimVertex     = NULL; // primary vertex
 TClonesArray *fFastTracks     = NULL; // fast(sim) tracks
 TObjArray    *fTracks;                // papa tracks

 Int_t         fMultiMatch     = 0;    // number of mutiple matched tracks

 PairAnalysisEvent(const PairAnalysisEvent& event);
 PairAnalysisEvent &operator=(const PairAnalysisEvent &c);

 ClassDef(PairAnalysisEvent,7)         // Event structure
};



#endif
