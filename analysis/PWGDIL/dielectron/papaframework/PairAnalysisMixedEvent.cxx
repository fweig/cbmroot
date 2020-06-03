///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                                                                       //
// Authors:
//   * Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
//   Julian Book   <Julian.Book@cern.ch>
/*

  Small event keeping holding track arrays and the event data.

*/
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include <TObjArray.h>
#include <TExMap.h>
#include <TProcessID.h>

#include "PairAnalysisTrack.h"

#include "PairAnalysisMixedEvent.h"

ClassImp(PairAnalysisMixedEvent)

PairAnalysisMixedEvent::PairAnalysisMixedEvent() :
  PairAnalysisMixedEvent("mixedevent","mixed event")
{
  //
  // Default Constructor
  //
}

//______________________________________________
PairAnalysisMixedEvent::PairAnalysisMixedEvent(const char* name, const char* title) :
  TNamed(name, title),
  fArrTrackP(),
  fArrTrackN()
{
  //
  // Named Constructor
  //
  for (Int_t i=0; i<PairAnalysisVarManager::kNMaxValuesMC;++i) fEventData[i]=0.;
}

//______________________________________________
PairAnalysisMixedEvent::~PairAnalysisMixedEvent()
{
  //
  // Default Destructor
  //
  fArrTrackP.Delete();
  fArrTrackN.Delete();
}

//______________________________________________
void PairAnalysisMixedEvent::SetTracks(const TObjArray &arrP, const TObjArray &arrN)
{
  //
  // Setup PairAnalysisPairs
  // assumes that the objects in arrP and arrN are
  //

  //Clear out old entries before filling new ones
  Clear(); // check if this can be improved, by calling clear instead of removeat

  // we keep the tracks buffered to minimise new / delete operations
  fNTracksN=0;
  fNTracksP=0;

  //check size of the arrays
  if (fArrTrackP.GetSize()<arrP.GetSize()) {
    fArrTrackP.Expand(arrP.GetSize());
  }
  if (fArrTrackN.GetSize()<arrN.GetSize()) {
    fArrTrackN.Expand(arrN.GetSize());
  }

  TExMap mapStoredVertices;
  fPIDIndex=TProcessID::GetPIDs()->IndexOf(fPID);
  // fill particles
  Int_t tracks=0;
  for (Int_t itrack=0; itrack<arrP.GetEntriesFast(); ++itrack){
    PairAnalysisTrack *track=dynamic_cast<PairAnalysisTrack*>(arrP.At(itrack));
    if (!track) continue;

    // buffer track
//    PairAnalysisTrack   *ctrack = new (fArrTrackP[tracks]) PairAnalysisTrack(*track);
    new (fArrTrackP[tracks]) PairAnalysisTrack(*track);

    ++tracks;
  }
  fNTracksP=tracks;

  tracks=0;
  for (Int_t itrack=0; itrack<arrN.GetEntriesFast(); ++itrack){
    PairAnalysisTrack *track=dynamic_cast<PairAnalysisTrack*>(arrN.At(itrack));
    if (!track) continue;

    //buffer track
//    PairAnalysisTrack   *ctrack = new (fArrTrackN[tracks]) PairAnalysisTrack(*track);
    new (fArrTrackN[tracks]) PairAnalysisTrack(*track);

    ++tracks;
  }
  fNTracksN=tracks;

}

//______________________________________________
void PairAnalysisMixedEvent::Clear(Option_t *opt)
{
  //
  // clear arrays
  //
  fArrTrackP.Clear(opt);
  fArrTrackN.Clear(opt);
}

//______________________________________________
void PairAnalysisMixedEvent::Set(Int_t size)
{
  //
  // set size of array
  //
  fArrTrackP.SetClass("PairAnalysisTrack",size);
  fArrTrackN.SetClass("PairAnalysisTrack",size);
}

//______________________________________________
void PairAnalysisMixedEvent::SetEventData(const Double_t data[PairAnalysisVarManager::kNMaxValuesMC])
{
  //
  // copy only evnet variables
  //
  for (Int_t i=PairAnalysisVarManager::kPairMax; i<PairAnalysisVarManager::kNMaxValuesMC;++i) fEventData[i]=data[i];
}

//______________________________________________
void PairAnalysisMixedEvent::AssignID(TObject *obj)
{
  //
  // Custom function to assign a uid to an object with an own process id
  // to avoid problems buffering the vertices
  //
  UInt_t uid=1;
  if (fPID->GetObjects()) uid=fPID->GetObjects()->GetEntriesFast();
  uid+=(fPIDIndex<<24);
  obj->SetBit(kIsReferenced);
  obj->SetUniqueID(uid);
  fPID->PutObjectWithID(obj);
}
