/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Claudia Hoehne, Andrey Lebedev, Semen Lebedev */

/**
 * \file CbmRichTrackExtrapolationMirrorIdeal.cxx
 *
 * \author Claudia Hoehne
 * \date 2006
 **/

#include "CbmRichTrackExtrapolationMirrorIdeal.h"

#include "CbmEvent.h"
#include "CbmGlobalTrack.h"
#include "CbmMCTrack.h"
#include "CbmRichPoint.h"
#include "CbmStsTrack.h"
#include "CbmTrackMatchNew.h"

#include "FairRootManager.h"
#include "FairTrackParam.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TMatrixFSym.h"

#include <iostream>

using std::cout;
using std::endl;

CbmRichTrackExtrapolationMirrorIdeal::CbmRichTrackExtrapolationMirrorIdeal() {}

CbmRichTrackExtrapolationMirrorIdeal::~CbmRichTrackExtrapolationMirrorIdeal() {}

void CbmRichTrackExtrapolationMirrorIdeal::Init()
{
  FairRootManager* manager = FairRootManager::Instance();
  if (manager == nullptr) LOG(fatal) << "CbmRichTrackExtrapolationMirrorIdeal::Init(): FairRootManager is nullptr.";

  fMcTracks = (TClonesArray*) manager->GetObject("MCTrack");
  if (fMcTracks == nullptr) LOG(fatal) << "CbmRichTrackExtrapolationMirrorIdeal::Init(): No MCTrack array.";

  fRichMirrorPoints = (TClonesArray*) manager->GetObject("RichMirrorPoint");
  if (fRichMirrorPoints == nullptr)
    LOG(fatal) << "CbmRichTrackExtrapolationMirrorIdeal::Init(): No RichMirrorPoint array.";

  fStsTracks = (TClonesArray*) manager->GetObject("StsTrack");
  if (fStsTracks == nullptr) LOG(fatal) << "CbmRichTrackExtrapolationMirrorIdeal::Init(): No StsTrack array.";

  fStsTrackMatches = (TClonesArray*) manager->GetObject("StsTrackMatch");
  if (fStsTrackMatches == nullptr)
    LOG(fatal) << "CbmRichTrackExtrapolationMirrorIdeal::Init(): No StsTrackMatch array.";
}

void CbmRichTrackExtrapolationMirrorIdeal::DoExtrapolation(CbmEvent* event, TClonesArray* globalTracks,
                                                           TClonesArray* extrapolatedTrackParams, double /*z*/)
{

  if (event != nullptr) {
    LOG(fatal) << "CbmRichTrackExtrapolationMirrorIdeal::DoExtrapolation(): CbmEvent is not nullptr. "
                  "This class does not support time-based mode. Please switch to event-by-event mode.";
  }

  if (extrapolatedTrackParams == nullptr) {
    LOG(error) << "CbmRichTrackExtrapolationMirrorIdeal::DoExtrapolation(): extrapolatedTrackParams missing!";
    return;
  }

  if (globalTracks == nullptr) {
    LOG(error) << "CbmRichTrackExtrapolationMirrorIdeal::DoExtrapolation(): globalTracks missing!";
    return;
  }

  Double_t charge = 1.;
  TMatrixFSym covMat(5);
  for (Int_t i = 0; i < 5; i++)
    for (Int_t j = 0; j <= i; j++)
      covMat(i, j) = 0;
  covMat(0, 0) = covMat(1, 1) = covMat(2, 2) = covMat(3, 3) = covMat(4, 4) = 1.e-4;

  TVector3 pos, mom;
  Int_t nofGlobalTracks = globalTracks->GetEntriesFast();
  for (Int_t iT = 0; iT < nofGlobalTracks; iT++) {
    CbmGlobalTrack* gTrack = static_cast<CbmGlobalTrack*>(globalTracks->At(iT));
    new ((*extrapolatedTrackParams)[iT]) FairTrackParam(0., 0., 0., 0., 0., 0., covMat);

    Int_t stsInd = gTrack->GetStsTrackIndex();
    if (stsInd < 0) continue;
    CbmStsTrack* stsTrack = static_cast<CbmStsTrack*>(fStsTracks->At(stsInd));
    if (stsTrack == nullptr) continue;
    CbmTrackMatchNew* stsTrackMatch = static_cast<CbmTrackMatchNew*>(fStsTrackMatches->At(stsInd));
    if (stsTrackMatch == nullptr) continue;
    Int_t stsMcInd = stsTrackMatch->GetMatchedLink().GetIndex();
    for (Int_t iM = 0; iM < fRichMirrorPoints->GetEntriesFast(); iM++) {
      CbmRichPoint* pMirror = static_cast<CbmRichPoint*>(fRichMirrorPoints->At(iM));
      if (pMirror->GetTrackID() == stsMcInd) {
        pMirror->Momentum(mom);
        pMirror->Position(pos);
        Double_t tx = mom.Px() / mom.Pz();
        Double_t ty = mom.Py() / mom.Pz();
        Double_t qp = charge / mom.Mag();
        FairTrackParam richtrack(pos.X(), pos.Y(), pos.Z(), tx, ty, qp, covMat);
        *(FairTrackParam*) (extrapolatedTrackParams->At(iT)) = richtrack;
      }
    }
  }
}
