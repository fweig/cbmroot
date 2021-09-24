/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Claudia Hoehne [committer], Andrey Lebedev, Semen Lebedev */

/**
 * \file CbmRichTrackExtrapolationIdeal.cxx
 *
 * \author Claudia Hoehne
 * \date 2006
 **/

#include "CbmRichTrackExtrapolationIdeal.h"

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

CbmRichTrackExtrapolationIdeal::CbmRichTrackExtrapolationIdeal() {}

CbmRichTrackExtrapolationIdeal::~CbmRichTrackExtrapolationIdeal() {}

void CbmRichTrackExtrapolationIdeal::Init()
{
  FairRootManager* manager = FairRootManager::Instance();
  if (manager == nullptr) LOG(fatal) << "CbmRichTrackExtrapolationIdeal::Init(): FairRootManager is nullptr.";

  fRefPlanePoints = (TClonesArray*) manager->GetObject("RefPlanePoint");
  if (fRefPlanePoints == nullptr) LOG(fatal) << "CbmRichTrackExtrapolationIdeal::Init(): No RefPlanePoint array.";

  fStsTracks = (TClonesArray*) manager->GetObject("StsTrack");
  if (fStsTracks == nullptr) LOG(fatal) << "CbmRichTrackExtrapolationIdeal::Init(): No StsTrack array.";

  fStsTrackMatches = (TClonesArray*) manager->GetObject("StsTrackMatch");
  if (fStsTrackMatches == nullptr) LOG(fatal) << "CbmRichTrackExtrapolationIdeal::Init(): No StsTrackMatch array.";
}

void CbmRichTrackExtrapolationIdeal::DoExtrapolation(CbmEvent* event, TClonesArray* globalTracks,
                                                     TClonesArray* extrapolatedTrackParams, double /*z*/)
{

  if (event != nullptr) {
    LOG(fatal) << "CbmRichTrackExtrapolationIdeal::DoExtrapolation(): CbmEvent is not nullptr. "
                  "This class does not support time-based mode. Please switch to event-by-event mode.";
  }

  if (extrapolatedTrackParams == nullptr) {
    LOG(error) << "CbmRichTrackExtrapolationIdeal::DoExtrapolation(): extrapolatedTrackParams missing!";
    return;
  }

  if (globalTracks == nullptr) {
    LOG(error) << "CbmRichTrackExtrapolationIdeal::DoExtrapolation(): globalTracks missing!";
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

    for (Int_t iR = 0; iR < fRefPlanePoints->GetEntriesFast(); iR++) {
      CbmRichPoint* refPlanePoint = static_cast<CbmRichPoint*>(fRefPlanePoints->At(iR));
      if (refPlanePoint->GetTrackID() == stsMcInd) {
        refPlanePoint->Momentum(mom);
        refPlanePoint->Position(pos);
        Double_t tx = mom.Px() / mom.Pz();
        Double_t ty = mom.Py() / mom.Pz();
        Double_t qp = charge / mom.Mag();
        FairTrackParam richtrack(pos.X(), pos.Y(), pos.Z(), tx, ty, qp, covMat);
        *(FairTrackParam*) (extrapolatedTrackParams->At(iT)) = richtrack;
      }
    }
  }
}
