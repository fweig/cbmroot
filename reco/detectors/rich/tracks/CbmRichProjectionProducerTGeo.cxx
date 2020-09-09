/**
 * \file CbmRichProjectionProducer.cxx
 *
 * \author P.Stolpovsky
 * \date 2005
 **/

#include "CbmRichProjectionProducerTGeo.h"

#include "CbmMCTrack.h"
#include "FairGeoNode.h"
#include "FairGeoTransform.h"
#include "FairGeoVector.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairTrackParam.h"
#include "TGeoManager.h"

#include "CbmRichGeoManager.h"
#include "TClonesArray.h"
#include "TMatrixFSym.h"
#include "TVector3.h"
#include "utils/CbmRichNavigationUtil.h"

#include <Logger.h>

#include <cmath>
#include <iostream>

using std::cout;
using std::endl;


CbmRichProjectionProducerTGeo::CbmRichProjectionProducerTGeo()
  : fTrackParams(NULL), fNHits(0), fEventNum(0) {}

CbmRichProjectionProducerTGeo::~CbmRichProjectionProducerTGeo() {
  FairRootManager* fManager = FairRootManager::Instance();
  fManager->Write();
}


void CbmRichProjectionProducerTGeo::Init() {
  LOG(info) << "CbmRichProjectionProducerTGeo::Init()";
  FairRootManager* manager = FairRootManager::Instance();

  fTrackParams = (TClonesArray*) manager->GetObject("RichTrackParamZ");
  if (NULL == fTrackParams) { LOG(fatal) << "No RichTrackParamZ array!"; }
}

void CbmRichProjectionProducerTGeo::DoProjection(TClonesArray* richProj) {
  fEventNum++;
  LOG(info) << "CbmRichProjectionProducer: event " << fEventNum;

  CbmRichRecGeoPar* gp = CbmRichGeoManager::GetInstance().fGP;
  double mirrorX       = gp->fMirrorX;
  double mirrorY       = gp->fMirrorY;
  double mirrorZ       = gp->fMirrorZ;

  richProj->Delete();
  TMatrixFSym covMat(5);
  for (Int_t i = 0; i < 5; i++) {
    for (Int_t j = 0; j <= i; j++) {
      covMat(i, j) = 0;
    }
  }
  covMat(0, 0) = covMat(1, 1) = covMat(2, 2) = covMat(3, 3) = covMat(4, 4) =
    1.e-4;

  for (Int_t j = 0; j < fTrackParams->GetEntriesFast(); j++) {
    FairTrackParam* trackParam = (FairTrackParam*) fTrackParams->At(j);
    new ((*richProj)[j]) FairTrackParam(0., 0., 0., 0., 0., 0., covMat);

    if (trackParam->GetX() == 0 && trackParam->GetY() == 0
        && trackParam->GetZ() == 0 && trackParam->GetTx() == 0
        && trackParam->GetTy() == 0)
      continue;
    if (trackParam->GetQp() == 0) continue;

    TVector3 startP, crossP, centerP;
    TVector3 dirCos;
    Double_t nx, ny, nz;
    CbmRichNavigationUtil::GetDirCos(trackParam, nx, ny, nz);
    dirCos.SetXYZ(nx, ny, nz);

    string volumeName = CbmRichNavigationUtil::FindIntersection(
      trackParam, crossP, "mirror_tile_type");
    Bool_t mirrorIntersectionFound = (volumeName != string(""));
    if (!mirrorIntersectionFound) continue;

    // mirror center
    if (crossP.Y() > 0) {
      centerP.SetXYZ(mirrorX, mirrorY, mirrorZ);
    } else {
      centerP.SetXYZ(mirrorX, -mirrorY, mirrorZ);
    }

    //   calculate normal on crosspoint with mirror
    TVector3 normP(crossP.x() - centerP.x(),
                   crossP.y() - centerP.y(),
                   crossP.z() - centerP.z());
    normP = normP.Unit();
    // check that normal has same z-direction as momentum
    if ((normP.z() * dirCos.z()) < 0.)
      normP = TVector3(-1. * normP.x(), -1. * normP.y(), -1. * normP.z());

    // reflect track
    Double_t np =
      normP.x() * dirCos.x() + normP.y() * dirCos.y() + normP.z() * dirCos.z();
    Double_t refX = 2 * np * normP.x() - dirCos.x();
    Double_t refY = 2 * np * normP.y() - dirCos.y();
    Double_t refZ = 2 * np * normP.z() - dirCos.z();
    TVector3 refl;
    refl.SetXYZ(-refX, -refY, -refZ);
    refl.Unit();

    TVector3 pmtIntersectionPoint;
    volumeName = CbmRichNavigationUtil::FindIntersection(
      refl, crossP, pmtIntersectionPoint, "pmt");
    Bool_t pmtIntersectionFound = (volumeName != string(""));
    if (!pmtIntersectionFound) continue;


    // Transform intersection point in same way as MCPoints were
    // transformed in HitProducer before stored as Hit:
    TVector3 outPos;
    CbmRichGeoManager::GetInstance().RotatePoint(&pmtIntersectionPoint,
                                                 &outPos);
    Double_t xDet = outPos.X();
    Double_t yDet = outPos.Y();
    Double_t zDet = outPos.Z();

    //check that crosspoint inside the plane
    //         if( xDet > (-fGP.fPmtXOrig-fGP.fPmtWidthX) && xDet < (fGP.fPmtXOrig+fGP.fPmtWidthX)){
    //            if(TMath::Abs(yDet) > (fGP.fPmtY-fGP.fPmtWidthY) && TMath::Abs(yDet) < (fGP.fPmtY+fGP.fPmtWidthY)){
    FairTrackParam richtrack(xDet, yDet, zDet, 0., 0., 0., covMat);
    *(FairTrackParam*) (richProj->At(j)) = richtrack;
    //            }
    //         }
  }  // j

  cout << "nofRichProjections:" << richProj->GetEntriesFast() << endl;
}
