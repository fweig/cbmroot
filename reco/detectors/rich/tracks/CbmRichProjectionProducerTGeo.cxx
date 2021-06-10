/* Copyright (C) 2005-2021 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Petr Stolpovsky, Andrey Lebedev [committer], Semen Lebedev */

/**
 * \file CbmRichProjectionProducer.cxx
 *
 * \author P.Stolpovsky
 * \date 2005
 **/

#include "CbmRichProjectionProducerTGeo.h"

#include "CbmEvent.h"
#include "CbmMCTrack.h"
#include "CbmRichGeoManager.h"
#include "utils/CbmRichNavigationUtil.h"

#include "FairGeoNode.h"
#include "FairGeoTransform.h"
#include "FairGeoVector.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairTrackParam.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TMatrixFSym.h"
#include "TVector3.h"

#include <iostream>

#include <cmath>

using std::cout;
using std::endl;


CbmRichProjectionProducerTGeo::CbmRichProjectionProducerTGeo() {}

CbmRichProjectionProducerTGeo::~CbmRichProjectionProducerTGeo() {}


void CbmRichProjectionProducerTGeo::Init()
{
  FairRootManager* manager = FairRootManager::Instance();
  if (nullptr == manager) LOG(fatal) << "CbmRichProjectionProducerTGeo::Init(): FairRootManager is nullptr.";

  fTrackParams = (TClonesArray*) manager->GetObject("RichTrackParamZ");
  if (fTrackParams == nullptr) LOG(fatal) << "CbmRichProjectionProducerTGeo::Init(): No RichTrackParamZ array.";
}

void CbmRichProjectionProducerTGeo::DoProjection(CbmEvent* event, TClonesArray* richProj)
{

  if (richProj == nullptr) {
    LOG(error) << "CbmRichProjectionProducerTGeo::DoExtrapolation(): richProj is nullptr.";
    return;
  }

  fEventNum++;

  CbmRichRecGeoPar* gp = CbmRichGeoManager::GetInstance().fGP;
  double mirrorX       = gp->fMirrorX;
  double mirrorY       = gp->fMirrorY;
  double mirrorZ       = gp->fMirrorZ;

  TMatrixFSym covMat(5);
  for (Int_t i = 0; i < 5; i++) {
    for (Int_t j = 0; j <= i; j++) {
      covMat(i, j) = 0;
    }
  }
  covMat(0, 0) = covMat(1, 1) = covMat(2, 2) = covMat(3, 3) = covMat(4, 4) = 1.e-4;

  Int_t nofTrackParams = event ? event->GetNofData(ECbmDataType::kRichTrackParamZ) : fTrackParams->GetEntriesFast();
  for (Int_t iT0 = 0; iT0 < nofTrackParams; iT0++) {
    Int_t iT                   = event ? event->GetIndex(ECbmDataType::kRichTrackParamZ, iT0) : iT0;
    FairTrackParam* trackParam = static_cast<FairTrackParam*>(fTrackParams->At(iT));
    new ((*richProj)[iT]) FairTrackParam(0., 0., 0., 0., 0., 0., covMat);
    if (event != nullptr) event->AddData(ECbmDataType::kRichTrackProjection, iT);

    if (trackParam->GetX() == 0 && trackParam->GetY() == 0 && trackParam->GetZ() == 0 && trackParam->GetTx() == 0
        && trackParam->GetTy() == 0)
      continue;
    if (trackParam->GetQp() == 0) continue;

    TVector3 startP, crossP, centerP;
    TVector3 dirCos;
    Double_t nx, ny, nz;
    CbmRichNavigationUtil::GetDirCos(trackParam, nx, ny, nz);
    dirCos.SetXYZ(nx, ny, nz);

    string volumeName              = CbmRichNavigationUtil::FindIntersection(trackParam, crossP, "mirror_tile_type");
    Bool_t mirrorIntersectionFound = (volumeName != string(""));
    if (!mirrorIntersectionFound) continue;

    // mirror center
    if (crossP.Y() > 0) { centerP.SetXYZ(mirrorX, mirrorY, mirrorZ); }
    else {
      centerP.SetXYZ(mirrorX, -mirrorY, mirrorZ);
    }

    //   calculate normal on crosspoint with mirror
    TVector3 normP(crossP.x() - centerP.x(), crossP.y() - centerP.y(), crossP.z() - centerP.z());
    normP = normP.Unit();
    // check that normal has same z-direction as momentum
    if ((normP.z() * dirCos.z()) < 0.) normP = TVector3(-1. * normP.x(), -1. * normP.y(), -1. * normP.z());

    // reflect track
    Double_t np   = normP.x() * dirCos.x() + normP.y() * dirCos.y() + normP.z() * dirCos.z();
    Double_t refX = 2 * np * normP.x() - dirCos.x();
    Double_t refY = 2 * np * normP.y() - dirCos.y();
    Double_t refZ = 2 * np * normP.z() - dirCos.z();
    TVector3 refl;
    refl.SetXYZ(-refX, -refY, -refZ);
    refl.Unit();

    TVector3 pmtIntersectionPoint;
    volumeName                  = CbmRichNavigationUtil::FindIntersection(refl, crossP, pmtIntersectionPoint, "pmt");
    Bool_t pmtIntersectionFound = (volumeName != string(""));
    if (!pmtIntersectionFound) continue;


    // Transform intersection point in same way as MCPoints were
    // transformed in HitProducer before stored as Hit:
    TVector3 outPos;
    CbmRichGeoManager::GetInstance().RotatePoint(&pmtIntersectionPoint, &outPos);
    Double_t xDet = outPos.X();
    Double_t yDet = outPos.Y();
    Double_t zDet = outPos.Z();

    //check that crosspoint inside the plane
    // if( xDet > (-fGP.fPmtXOrig-fGP.fPmtWidthX) && xDet < (fGP.fPmtXOrig+fGP.fPmtWidthX)){
    // if(TMath::Abs(yDet) > (fGP.fPmtY-fGP.fPmtWidthY) && TMath::Abs(yDet) < (fGP.fPmtY+fGP.fPmtWidthY)){
    FairTrackParam richtrack(xDet, yDet, zDet, 0., 0., 0., covMat);
    *(FairTrackParam*) (richProj->At(iT)) = richtrack;
    //            }
    //         }
  }  // j
}
