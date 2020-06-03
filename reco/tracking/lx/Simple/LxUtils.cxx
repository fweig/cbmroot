#include "Lx.h"

using namespace std;

// Used for building of the background on an invariant mass.
void LxFinder::SaveEventTracks()
{
#ifdef MAKE_HISTOS
  for (list<LxTrack*>::iterator i = caSpace.tracks.begin(); i != caSpace.tracks.end(); ++i)
  {
    LxTrack* firstTrack = *i;

    if (0 == firstTrack->externalTrack)
      continue;

    CbmStsTrack t = *firstTrack->externalTrack->track;

    if (t.GetParamLast()->GetQp() > 0)
      extFitter.DoFit(&t, -13);
    else
      extFitter.DoFit(&t, 13);

//    scaltype chi2Prim = extFitter.GetChiToVertex(&t, fPrimVtx);
    extFitter.GetChiToVertex(&t, fPrimVtx);
    FairTrackParam params;
    extFitter.Extrapolate(&t, fPrimVtx->GetZ(), &params);

    scaltype p = 1 / params.GetQp();
    scaltype p2 = p * p;

    if (p2 < 9)
      continue;

    scaltype tx2 = params.GetTx() * params.GetTx();
    scaltype ty2 = params.GetTy() * params.GetTy();
    scaltype pt2 = p2 * (tx2 + ty2) / (1 + tx2 + ty2);

    if (pt2 < 1)
      continue;

    t.SetParamFirst(&params);
    *superEventData = t;
    superEventTracks->Fill();
  }
#endif//MAKE_HISTOS
}
