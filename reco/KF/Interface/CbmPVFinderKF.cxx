#include "CbmPVFinderKF.h"

#include "CbmKFPrimaryVertexFinder.h"
#include "CbmKFTrack.h"
#include "CbmKFVertex.h"

#include "CbmEvent.h"
#include "CbmStsTrack.h"

#include "TClonesArray.h"

#include "math.h"

ClassImp(CbmPVFinderKF)

  Int_t CbmPVFinderKF::FindPrimaryVertex(TClonesArray* tracks,
                                         CbmVertex* vertex) {

  Int_t NTracks = tracks->GetEntries();

  CbmKFPrimaryVertexFinder Finder;
  CbmKFTrack* CloneArray = new CbmKFTrack[NTracks];
  for (Int_t i = 0; i < NTracks; i++) {
    CbmStsTrack* st = (CbmStsTrack*) tracks->At(i);
    Int_t NHits     = st->GetNofHits();
    if (NHits < 4) continue;
    if (st->GetFlag()) continue;
    if (st->GetChiSq() < 0. || st->GetChiSq() > 3.5 * 3.5 * st->GetNDF())
      continue;
    CbmKFTrack& T = CloneArray[i];
    T.SetStsTrack(*st);
    if (!finite(T.GetTrack()[0]) || !finite(T.GetCovMatrix()[0])) continue;
    Finder.AddTrack(&T);
  }
  CbmKFVertex v;
  Finder.Fit(v);
  v.GetVertex(*vertex);
  delete[] CloneArray;
  return 0;
}


// ----   Find vertex for one event   ---------------------------------------
Int_t CbmPVFinderKF::FindEventVertex(CbmEvent* event, TClonesArray* tracks) {

  assert(event);
  CbmKFPrimaryVertexFinder vFinder;

  // Get vertex object
  CbmVertex* vertex = event->GetVertex();

  // Copy input tracks to KF tracks
  Int_t nTracks          = event->GetNofData(ECbmDataType::kStsTrack);
  CbmKFTrack* trackArray = new CbmKFTrack[nTracks];
  for (Int_t iTrack = 0; iTrack < nTracks; iTrack++) {
    Int_t trackIndex   = event->GetIndex(ECbmDataType::kStsTrack, iTrack);
    CbmStsTrack* track = (CbmStsTrack*) tracks->At(trackIndex);
    Int_t nHits        = track->GetNofHits();
    if (nHits < 4) continue;         // use only tracks with at least 4 hits
    if (track->GetFlag()) continue;  // do not use suspicious tracks
    if (track->GetChiSq() < 0.       // use only good-quality tracks
        || track->GetChiSq() > 12.25 * track->GetNDF())
      continue;
    CbmKFTrack& kTrack = trackArray[iTrack];
    kTrack.SetStsTrack(*track);
    if (!finite(kTrack.GetTrack()[0]) || !finite(kTrack.GetCovMatrix()[0]))
      continue;
    vFinder.AddTrack(&kTrack);
  }

  // Do the vertex finding
  CbmKFVertex v;
  vFinder.Fit(v);

  // Copy KFVertex into CbmVertex
  v.GetVertex(*vertex);

  delete[] trackArray;
  return 0;
}
// --------------------------------------------------------------------------
