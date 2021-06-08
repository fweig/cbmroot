/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Claudia Hoehne and Semen Lebedev, Semen Lebedev, Semen Lebedev, Denis Bertini [committer] */

/**
* \file CbmRichRingTrackAssignIdeal.cxx
*
* \author Claudia Hoehne and Semen Lebedev
* \date 2007
**/

#include "CbmRichRingTrackAssignIdeal.h"

#include "CbmGlobalTrack.h"
#include "CbmMCTrack.h"
#include "CbmRichRing.h"
#include "CbmTrackMatchNew.h"

#include "FairRootManager.h"
#include "FairTrackParam.h"
#include <Logger.h>

#include "TClonesArray.h"

#include <iostream>

CbmRichRingTrackAssignIdeal::CbmRichRingTrackAssignIdeal() {}

CbmRichRingTrackAssignIdeal::~CbmRichRingTrackAssignIdeal() {}

void CbmRichRingTrackAssignIdeal::Init()
{
  FairRootManager* manager = FairRootManager::Instance();
  if (nullptr == manager) LOG(fatal) << "CbmRichRingTrackAssignIdeal::Init(): FairRootManager is nullptr.";

  fGlobalTracks = (TClonesArray*) manager->GetObject("GlobalTrack");
  if (fGlobalTracks == nullptr) LOG(fatal) << "CbmRichRingTrackAssignIdeal::Init():No GlobalTrack.";

  fRingMatches = (TClonesArray*) manager->GetObject("RichRingMatch");
  if (fRingMatches == nullptr) LOG(fatal) << "CbmRichRingTrackAssignIdeal::Init():No RichRingMatch.";

  fStsTrackMatches = (TClonesArray*) manager->GetObject("StsTrackMatch");
  if (fStsTrackMatches == nullptr) LOG(fatal) << "CbmRichRingTrackAssignIdeal::Init():No StsTrackMatch.";
}

void CbmRichRingTrackAssignIdeal::DoAssign(CbmEvent* event, TClonesArray* rings, TClonesArray* richProj)
{

  if (event != nullptr) {
    LOG(fatal) << "CbmRichRingTrackAssignIdeal::DoAssign(): CbmEvent is not nullptr. "
                  "This class does not support time-based mode. Please switch to event-by-event mode.";
  }

  Int_t nofTracks = richProj->GetEntriesFast();
  Int_t nofRings  = rings->GetEntriesFast();

  for (Int_t iR = 0; iR < nofRings; iR++) {
    CbmRichRing* ring = static_cast<CbmRichRing*>(rings->At(iR));
    if (ring == nullptr) continue;
    if (ring->GetNofHits() < fMinNofHitsInRing) continue;

    CbmTrackMatchNew* ringMatch = static_cast<CbmTrackMatchNew*>(fRingMatches->At(iR));
    if (ringMatch == nullptr) continue;
    Int_t ringId = ringMatch->GetMatchedLink().GetIndex();

    for (Int_t iT = 0; iT < nofTracks; iT++) {
      FairTrackParam* proj = static_cast<FairTrackParam*>(richProj->At(iT));
      if (proj == nullptr) continue;

      // no projection to photodetector plane
      if (proj->GetX() == 0 && proj->GetY() == 0) continue;

      CbmGlobalTrack* gTrack = static_cast<CbmGlobalTrack*>(fGlobalTracks->At(iT));
      if (gTrack == nullptr) continue;
      if (gTrack->GetStsTrackIndex() == -1) continue;
      CbmTrackMatchNew* trackMatch = static_cast<CbmTrackMatchNew*>(fStsTrackMatches->At(gTrack->GetStsTrackIndex()));
      if (trackMatch == nullptr) continue;

      if (trackMatch->GetMatchedLink().GetIndex() == ringId) { gTrack->SetRichRingIndex(iR); }
    }
  }
}
