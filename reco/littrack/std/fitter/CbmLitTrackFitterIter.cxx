/* Copyright (C) 2007-2012 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

/** CbmLitTrackFitterIter.cxx
 *@author A.Lebedev <alebedev@jinr.ru>
 *@since 2007
 **/

#include "fitter/CbmLitTrackFitterIter.h"

#include "data/CbmLitFitNode.h"
#include "data/CbmLitTrack.h"

#include <iostream>

CbmLitTrackFitterIter::CbmLitTrackFitterIter(TrackFitterPtr fitter, TrackFitterPtr smoother)
  : fFitter(fitter)
  , fSmoother(smoother)
  , fNofIterations(2)
  , fChiSqCut(15.)
  , fMinNofHits(3)
{
}

CbmLitTrackFitterIter::~CbmLitTrackFitterIter() {}

LitStatus CbmLitTrackFitterIter::Fit(CbmLitTrack* track, bool downstream)
{
  for (int iter = 0; iter < fNofIterations; iter++) {
    bool isRefit = false;

    if (fFitter->Fit(track) == kLITERROR) { return kLITERROR; }

    if (fSmoother->Fit(track) == kLITERROR) { return kLITERROR; }

    if (iter < fNofIterations - 1) {
      for (int i = 0; i < track->GetNofHits(); i++) {
        litfloat chiSq = track->GetFitNode(i)->GetChiSqSmoothed();
        if (chiSq > fChiSqCut) {
          track->RemoveHit(i);
          isRefit = true;
        }
      }
    }

    if (track->GetNofHits() < fMinNofHits) { return kLITERROR; }
    if (!isRefit) { return kLITSUCCESS; }
  }
  return kLITSUCCESS;
}
