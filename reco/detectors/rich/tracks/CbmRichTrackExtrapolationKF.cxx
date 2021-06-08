/* Copyright (C) 206 -2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Claudia Hoehne, Andrey Lebedev, Semen Lebedev, Denis Bertini [committer] */

/**
 * \file CbmRichTrackExtrapolationKF.cxx
 *
 * \author Claudia Hoehne
 * \date 206
 **/
#include "CbmRichTrackExtrapolationKF.h"

#include "CbmEvent.h"
#include "CbmGlobalTrack.h"
#include "CbmStsKFTrackFitter.h"
#include "CbmStsTrack.h"

#include "FairRootManager.h"
#include "FairTrackParam.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TMatrixFSym.h"

#include <iostream>


using std::cout;
using std::endl;

CbmRichTrackExtrapolationKF::CbmRichTrackExtrapolationKF() {}

CbmRichTrackExtrapolationKF::~CbmRichTrackExtrapolationKF() {}

void CbmRichTrackExtrapolationKF::Init()
{
  FairRootManager* manager = FairRootManager::Instance();
  if (manager == nullptr) LOG(fatal) << "CbmRichTrackExtrapolationKF::Init(): FairRootManager is nullptr.";

  fStsTracks = (TClonesArray*) manager->GetObject("StsTrack");
  if (fStsTracks == nullptr) LOG(fatal) << "CbmRichTrackExtrapolationKF::Init(): No StsTrack array.";
}

void CbmRichTrackExtrapolationKF::DoExtrapolation(CbmEvent* event, TClonesArray* globalTracks,
                                                  TClonesArray* extrapolatedTrackParams, double z)
{
  if (extrapolatedTrackParams == nullptr) {
    LOG(error) << "CbmRichTrackExtrapolationKF::DoExtrapolation(): extrapolatedTrackParams is nullptr.";
    return;
  }

  if (globalTracks == nullptr) {
    LOG(error) << "CbmRichTrackExtrapolationKF::DoExtrapolation(): globalTracks is nullptr.";
    return;
  }

  TMatrixFSym covMat(5);
  for (Int_t i = 0; i < 5; i++)
    for (Int_t j = 0; j <= i; j++)
      covMat(i, j) = 0;
  covMat(0, 0) = covMat(1, 1) = covMat(2, 2) = covMat(3, 3) = covMat(4, 4) = 1.e-4;

  TVector3 pos, mom;
  Int_t nofGlobalTracks = event ? event->GetNofData(ECbmDataType::kGlobalTrack) : globalTracks->GetEntriesFast();
  for (Int_t iT0 = 0; iT0 < nofGlobalTracks; iT0++) {
    Int_t iT               = event ? event->GetIndex(ECbmDataType::kGlobalTrack, iT0) : iT0;
    CbmGlobalTrack* gTrack = static_cast<CbmGlobalTrack*>(globalTracks->At(iT));
    new ((*extrapolatedTrackParams)[iT]) FairTrackParam(0., 0., 0., 0., 0., 0., covMat);
    if (event != nullptr) event->AddData(ECbmDataType::kRichTrackParamZ, iT);

    Int_t stsInd = gTrack->GetStsTrackIndex();
    if (stsInd < 0) continue;
    CbmStsTrack* stsTrack = static_cast<CbmStsTrack*>(fStsTracks->At(stsInd));
    if (stsTrack == nullptr) continue;
    CbmStsKFTrackFitter KF;
    FairTrackParam ExTrack;

    KF.Extrapolate(stsTrack, z, &ExTrack);

    *(FairTrackParam*) (extrapolatedTrackParams->At(iT)) = ExTrack;
  }
}
