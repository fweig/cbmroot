/* Copyright (C) 2016-2021 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Semen Lebedev */

/**
 * \file CbmRichTrackExtrapolationLittrack
 *
 * \author Semen Lebedev
 * \date 2016
 **/
#include "CbmRichTrackExtrapolationLittrack.h"

#include "CbmEvent.h"
#include "CbmGlobalTrack.h"
#include "CbmLitConverterFairTrackParam.h"
#include "CbmLitToolFactory.h"
#include "CbmStsTrack.h"
#include "propagation/CbmLitTGeoTrackPropagator.h"

#include "FairRootManager.h"
#include "FairTrackParam.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TMatrixFSym.h"

#include <iostream>

using std::cout;
using std::endl;

CbmRichTrackExtrapolationLittrack::CbmRichTrackExtrapolationLittrack() {}

CbmRichTrackExtrapolationLittrack::~CbmRichTrackExtrapolationLittrack() {}

void CbmRichTrackExtrapolationLittrack::Init()
{
  FairRootManager* manager = FairRootManager::Instance();
  if (manager == nullptr) LOG(fatal) << "CbmRichTrackExtrapolationLittrack::Init(): FairRootManager is nullptr.";

  fStsTracks = (TClonesArray*) manager->GetObject("StsTrack");
  if (fStsTracks == nullptr) LOG(fatal) << "CbmRichTrackExtrapolationLittrack::Init(): No StsTrack array.";

  fLitPropagator = CbmLitToolFactory::CreateTrackPropagator("lit");
}

void CbmRichTrackExtrapolationLittrack::DoExtrapolation(CbmEvent* event, TClonesArray* globalTracks,
                                                        TClonesArray* extrapolatedTrackParams, double z)
{
  if (extrapolatedTrackParams == nullptr) {
    LOG(error) << "CbmRichTrackExtrapolationLittrack::DoExtrapolation(): extrapolatedTrackParams is nullptr.";
    return;
  }

  if (globalTracks == nullptr) {
    LOG(error) << "CbmRichTrackExtrapolationLittrack::DoExtrapolation(): globalTracks is nullptr.";
    return;
  }

  TMatrixFSym covMat(5);
  for (Int_t i = 0; i < 5; i++)
    for (Int_t j = 0; j <= i; j++)
      covMat(i, j) = 0;
  covMat(0, 0) = covMat(1, 1) = covMat(2, 2) = covMat(3, 3) = covMat(4, 4) = 1.e-4;

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

    CbmLitTrackParam litInParam, litOutParam;
    CbmLitConverterFairTrackParam::FairTrackParamToCbmLitTrackParam(stsTrack->GetParamLast(), &litInParam);
    std::vector<litfloat> F(36, 0.);
    litfloat length = 0;

    fLitPropagator->Propagate(&litInParam, &litOutParam, z, 11, &F, &length);

    FairTrackParam outParam(0., 0., 0., 0., 0., 0., covMat);
    CbmLitConverterFairTrackParam::CbmLitTrackParamToFairTrackParam(&litOutParam, &outParam);

    *(FairTrackParam*) (extrapolatedTrackParams->At(iT)) = outParam;
  }
}
