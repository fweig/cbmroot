/** @file CbmLitTrackSelectionCuts.cxx
  * @copyright Copyright (C) 2011-2013 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Andrey Lebedev [committer] **/

#include "selection/CbmLitTrackSelectionCuts.h"

#include "data/CbmLitTrack.h"

#include <limits>

#include <cmath>

CbmLitTrackSelectionCuts::CbmLitTrackSelectionCuts()
  : fMaxChiSq(std::numeric_limits<litfloat>::max())
  , fMinLastPlaneId(-1)
  , fMinNofHits(-1)
  , fMinMomentum(0.)
{
}

CbmLitTrackSelectionCuts::CbmLitTrackSelectionCuts(litfloat maxChiSq, int minLastPlaneId, int minNofHits,
                                                   litfloat minMomentum)
  : fMaxChiSq(maxChiSq)
  , fMinLastPlaneId(minLastPlaneId)
  , fMinNofHits(minNofHits)
  , fMinMomentum(minMomentum)
{
}

CbmLitTrackSelectionCuts::~CbmLitTrackSelectionCuts() {}

LitStatus CbmLitTrackSelectionCuts::DoSelect(TrackPtrIterator itBegin, TrackPtrIterator itEnd)
{
  if (itBegin == itEnd) { return kLITSUCCESS; }

  for (TrackPtrIterator iTrack = itBegin; iTrack != itEnd; iTrack++) {
    if ((*iTrack)->GetQuality() == kLITBAD) { continue; }

    if ((*iTrack)->GetChi2() / (*iTrack)->GetNDF() > fMaxChiSq || (*iTrack)->GetNofHits() < fMinNofHits
        || (*iTrack)->GetLastStationId() < fMinLastPlaneId) {
      (*iTrack)->SetQuality(kLITBAD);
    }

    // Momentum cut
    if (fMinMomentum != 0.) {
      if ((*iTrack)->GetParamLast()->GetQp() == 0.) {
        (*iTrack)->SetQuality(kLITBAD);
        continue;
      }
      litfloat mom = std::abs(1. / (*iTrack)->GetParamLast()->GetQp());
      if (mom < fMinMomentum) { (*iTrack)->SetQuality(kLITBAD); }
    }
  }

  return kLITSUCCESS;
}

LitStatus CbmLitTrackSelectionCuts::DoSelect(TrackPtrVector& tracks) { return DoSelect(tracks.begin(), tracks.end()); }
