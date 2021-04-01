/**
 * \file CbmRichProjectionProducerAnalytical.cxx
 *
 * \author S.Lebedev (initial version by P.Stolpovsky (2005))
 * \date 2016
 **/

#include "CbmRichProjectionProducerAnalytical.h"

#include "CbmMCTrack.h"
#include "CbmRichGeoManager.h"

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


CbmRichProjectionProducerAnalytical::CbmRichProjectionProducerAnalytical()
  : fTrackParams(NULL), fNHits(0), fEventNum(0) {}

CbmRichProjectionProducerAnalytical::~CbmRichProjectionProducerAnalytical() {
  FairRootManager* fManager = FairRootManager::Instance();
  fManager->Write();
}


void CbmRichProjectionProducerAnalytical::Init() {
  LOG(info) << "CbmRichProjectionProducerAnalytical::Init()";
  FairRootManager* manager = FairRootManager::Instance();

  fTrackParams = (TClonesArray*) manager->GetObject("RichTrackParamZ");
  if (NULL == fTrackParams) { LOG(fatal) << "No RichTrackParamZ array!"; }
}

void CbmRichProjectionProducerAnalytical::DoProjection(TClonesArray* richProj) {
  fEventNum++;
  cout << "CbmRichProjectionProducerAnalytical:: event " << fEventNum << endl;

  CbmRichRecGeoPar* gp = CbmRichGeoManager::GetInstance().fGP;
  Double_t mirrorX     = gp->fMirrorX;
  Double_t mirrorY     = gp->fMirrorY;
  Double_t mirrorZ     = gp->fMirrorZ;
  Double_t mirrorR     = gp->fMirrorR;

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
    FairTrackParam* point = (FairTrackParam*) fTrackParams->At(j);
    new ((*richProj)[j]) FairTrackParam(0., 0., 0., 0., 0., 0., covMat);

    // check if Array was filled
    if (point->GetX() == 0 && point->GetY() == 0 && point->GetZ() == 0
        && point->GetTx() == 0 && point->GetTy() == 0)
      continue;
    if (point->GetQp() == 0) continue;

    Double_t rho1 = 0.;
    TVector3 startP, momP, crossP, centerP;


    Double_t p = 1. / TMath::Abs(point->GetQp());
    Double_t pz;
    if ((1 + point->GetTx() * point->GetTx() + point->GetTy() * point->GetTy())
        > 0.)
      pz = p
           / TMath::Sqrt(1 + point->GetTx() * point->GetTx()
                         + point->GetTy() * point->GetTy());
    else {
      cout << " -E- RichProjectionProducer: strange value for calculating pz: "
           << (1 + point->GetTx() * point->GetTx()
               + point->GetTy() * point->GetTy())
           << endl;
      pz = 0.;
    }
    Double_t px = pz * point->GetTx();
    Double_t py = pz * point->GetTy();
    momP.SetXYZ(px, py, pz);
    point->Position(startP);
    if ((mirrorY * startP.y()) < 0)
      mirrorY =
        -mirrorY;  // check that mirror center and startP are in same hemisphere

    // calculation of intersection of track with selected mirror
    // corresponds to calculation of intersection between a straight line and a sphere:
    // vector: r = startP - mirrorCenter
    // RxP = r*momP
    // normP2 = momP^2
    // dist = r^2 - fR^2
    // -> rho1 = (-RxP+sqrt(RxP^2-normP2*dist))/normP2  extrapolation factor for:
    // intersection point crossP = startP + rho1 * momP
    Double_t RxP =
      (momP.x() * (startP.x() - mirrorX) + momP.y() * (startP.y() - mirrorY)
       + momP.z() * (startP.z() - mirrorZ));
    Double_t normP2 =
      (momP.x() * momP.x() + momP.y() * momP.y() + momP.z() * momP.z());
    Double_t dist =
      (startP.x() * startP.x() + mirrorX * mirrorX + startP.y() * startP.y()
       + mirrorY * mirrorY + startP.z() * startP.z() + mirrorZ * mirrorZ
       - 2 * startP.x() * mirrorX - 2 * startP.y() * mirrorY
       - 2 * startP.z() * mirrorZ - mirrorR * mirrorR);

    if ((RxP * RxP - normP2 * dist) > 0.) {
      if (normP2 != 0.)
        rho1 = (-RxP + TMath::Sqrt(RxP * RxP - normP2 * dist)) / normP2;
      if (normP2 == 0)
        cout << " Error in track extrapolation: momentum = 0 " << endl;
    } else {
      //cout << " -E- RichProjectionProducer:  RxP*RxP-normP2*dist = " << RxP*RxP-normP2*dist << endl;
    }

    Double_t crossPx = startP.x() + rho1 * momP.x();
    Double_t crossPy = startP.y() + rho1 * momP.y();
    Double_t crossPz = startP.z() + rho1 * momP.z();
    crossP.SetXYZ(crossPx, crossPy, crossPz);

    // check if crosspoint with mirror and chosen mirrorcenter (y) are in same hemisphere
    // if not recalculate crossing point
    if ((mirrorY * crossP.y()) < 0) {
      mirrorY = -mirrorY;
      RxP =
        (momP.x() * (startP.x() - mirrorX) + momP.y() * (startP.y() - mirrorY)
         + momP.z() * (startP.z() - mirrorZ));
      normP2 =
        (momP.x() * momP.x() + momP.y() * momP.y() + momP.z() * momP.z());
      dist =
        (startP.x() * startP.x() + mirrorX * mirrorX + startP.y() * startP.y()
         + mirrorY * mirrorY + startP.z() * startP.z() + mirrorZ * mirrorZ
         - 2 * startP.x() * mirrorX - 2 * startP.y() * mirrorY
         - 2 * startP.z() * mirrorZ - mirrorR * mirrorR);

      if ((RxP * RxP - normP2 * dist) > 0.) {
        if (normP2 != 0.)
          rho1 = (-RxP + TMath::Sqrt(RxP * RxP - normP2 * dist)) / normP2;
        if (normP2 == 0)
          cout << " Error in track extrapolation: momentum = 0 " << endl;
      } else {
        //cout << " -E- RichProjectionProducer:  RxP*RxP-normP2*dist = " << RxP*RxP-normP2*dist << endl;
      }

      crossPx = startP.x() + rho1 * momP.x();
      crossPy = startP.y() + rho1 * momP.y();
      crossPz = startP.z() + rho1 * momP.z();
      crossP.SetXYZ(crossPx, crossPy, crossPz);
    }

    centerP.SetXYZ(mirrorX, mirrorY, mirrorZ);  // mirror center


    //   calculate normal on crosspoint with mirror
    TVector3 normP(crossP.x() - centerP.x(),
                   crossP.y() - centerP.y(),
                   crossP.z() - centerP.z());
    normP = normP.Unit();
    // check that normal has same z-direction as momentum
    if ((normP.z() * momP.z()) < 0.)
      normP = TVector3(-1. * normP.x(), -1. * normP.y(), -1. * normP.z());

    // reflect track
    Double_t np =
      normP.x() * momP.x() + normP.y() * momP.y() + normP.z() * momP.z();

    TVector3 ref;
    ref.SetXYZ(2 * np * normP.x() - momP.x(),
               2 * np * normP.y() - momP.y(),
               2 * np * normP.z() - momP.z());

    if (ref.z() != 0.) {

      TVector3 inPos(0., 0., 0.), outPos(0., 0., 0.);


      if (gp->fGeometryType == CbmRichGeometryTypeCylindrical) {
        GetPmtIntersectionPointCyl(&centerP, &crossP, &ref, &inPos);
        // Transform intersection point in same way as MCPoints were transformed in HitProducer before stored as Hit
        // For the cylindrical geometry we also pass the path to the strip block
        CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);

        //cout << "inPoint:" << inPos.X() << " " << inPos.Y() << " " << inPos.Z() << " outPoint:" << outPos.X() << " " << outPos.Y() << " " << outPos.Z()  << endl;


      } else if (gp->fGeometryType == CbmRichGeometryTypeTwoWings) {
        GetPmtIntersectionPointTwoWings(&centerP, &crossP, &ref, &inPos);
        // Transform intersection point in same way as MCPoints were transformed in HitProducer before stored as Hit
        CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);
      } else {
        Fatal("CbmRichProjectionProducerAnalytical ", "unnown geometry type");
      }

      Bool_t isInsidePmt =
        CbmRichGeoManager::GetInstance().IsPointInsidePmt(&outPos);

      if (isInsidePmt) {
        FairTrackParam richtrack(
          outPos.x(), outPos.y(), outPos.z(), 0., 0., 0., covMat);
        *(FairTrackParam*) (richProj->At(j)) = richtrack;
      }
    }  // if (refZ!=0.)
  }    // j
}

void CbmRichProjectionProducerAnalytical::GetPmtIntersectionPointTwoWings(
  const TVector3* centerP,
  const TVector3* crossP,
  const TVector3* ref,
  TVector3* outPoint) {
  // crosspoint whith photodetector plane:
  // calculate intersection between straight line and (tilted) plane:
  // normal on plane tilted by theta around x-axis: (0,-sin(theta),cos(theta)) = n
  // normal on plane tilted by phi around y-axis: (-sin(phi),0,cos(phi)) = n
  // normal on plane tilted by theta around x-axis and phi around y-axis: (-sin(phi),-sin(theta)cos(phi),cos(theta)cos(phi)) = n
  // point on plane is (fDetX,fDetY,fDetZ) = p as photodetector is tiled around its center
  // equation of plane for r being point in plane: n(r-p) = 0
  // calculate intersection point of reflected track with plane: r=intersection point
  // intersection point = crossP + rho2 * refl_track
  // take care for all 4 cases:
  //        -> first calculate for case x>0, then check

  CbmRichRecGeoPar* gp = CbmRichGeoManager::GetInstance().fGP;

  if (gp->fGeometryType != CbmRichGeometryTypeTwoWings) {
    Fatal(
      "CbmRichProjectionProducerAnalytical::GetPmtIntersectionPointTwoWings ",
      "Wrong geometry, this method could be used only for "
      "CbmRichGeometryTypeTwoWings");
  }

  Double_t pmtPhi    = gp->fPmt.fPhi;
  Double_t pmtTheta  = gp->fPmt.fTheta;
  Double_t pmtPlaneX = gp->fPmt.fPlaneX;
  Double_t pmtPlaneY = gp->fPmt.fPlaneY;
  Double_t pmtPlaneZ = gp->fPmt.fPlaneZ;
  Double_t rho2      = 0.;

  if (centerP->y() > 0) {
    rho2 =
      (-TMath::Sin(pmtPhi) * (pmtPlaneX - crossP->x())
       - TMath::Sin(pmtTheta) * TMath::Cos(pmtPhi) * (pmtPlaneY - crossP->y())
       + TMath::Cos(pmtTheta) * TMath::Cos(pmtPhi) * (pmtPlaneZ - crossP->z()))
      / (-TMath::Sin(pmtPhi) * ref->x()
         - TMath::Sin(pmtTheta) * TMath::Cos(pmtPhi) * ref->y()
         + TMath::Cos(pmtTheta) * TMath::Cos(pmtPhi) * ref->z());
  }
  if (centerP->y() < 0) {
    rho2 =
      (-TMath::Sin(pmtPhi) * (pmtPlaneX - crossP->x())
       - TMath::Sin(-pmtTheta) * TMath::Cos(pmtPhi) * (-pmtPlaneY - crossP->y())
       + TMath::Cos(-pmtTheta) * TMath::Cos(pmtPhi) * (pmtPlaneZ - crossP->z()))
      / (-TMath::Sin(pmtPhi) * ref->x()
         - TMath::Sin(-pmtTheta) * TMath::Cos(pmtPhi) * ref->y()
         + TMath::Cos(-pmtTheta) * TMath::Cos(pmtPhi) * ref->z());
  }

  //rho2 = -1*(crossP.z() - fDetZ)/refZ;    // only for theta = 0, phi=0
  Double_t xX = crossP->x() + ref->x() * rho2;
  Double_t yY = crossP->y() + ref->y() * rho2;
  Double_t zZ = crossP->z() + ref->z() * rho2;

  if (xX < 0) {
    if (centerP->y() > 0) {
      rho2 = (-TMath::Sin(-pmtPhi) * (-pmtPlaneX - crossP->x())
              - TMath::Sin(pmtTheta) * TMath::Cos(-pmtPhi)
                  * (pmtPlaneY - crossP->y())
              + TMath::Cos(pmtTheta) * TMath::Cos(-pmtPhi)
                  * (pmtPlaneZ - crossP->z()))
             / (-TMath::Sin(-pmtPhi) * ref->x()
                - TMath::Sin(pmtTheta) * TMath::Cos(-pmtPhi) * ref->y()
                + TMath::Cos(pmtTheta) * TMath::Cos(-pmtPhi) * ref->z());
    }
    if (centerP->y() < 0) {
      rho2 = (-TMath::Sin(-pmtPhi) * (-pmtPlaneX - crossP->x())
              - TMath::Sin(-pmtTheta) * TMath::Cos(-pmtPhi)
                  * (-pmtPlaneY - crossP->y())
              + TMath::Cos(-pmtTheta) * TMath::Cos(-pmtPhi)
                  * (pmtPlaneZ - crossP->z()))
             / (-TMath::Sin(-pmtPhi) * ref->x()
                - TMath::Sin(-pmtTheta) * TMath::Cos(-pmtPhi) * ref->y()
                + TMath::Cos(-pmtTheta) * TMath::Cos(-pmtPhi) * ref->z());
    }

    xX = crossP->x() + ref->x() * rho2;
    yY = crossP->y() + ref->y() * rho2;
    zZ = crossP->z() + ref->z() * rho2;
  }

  outPoint->SetXYZ(xX, yY, zZ);
}


void CbmRichProjectionProducerAnalytical::GetPmtIntersectionPointCyl(
  const TVector3* centerP,
  const TVector3* crossP,
  const TVector3* ref,
  TVector3* outPoint) {
  CbmRichRecGeoPar* gp = CbmRichGeoManager::GetInstance().fGP;
  if (gp->fGeometryType != CbmRichGeometryTypeCylindrical) {
    Fatal("CbmRichProjectionProducerAnalytical::GetPmtIntersectionPointCyl ",
          "Wrong geometry, this method could be used only for "
          "CbmRichGeometryTypeCylindrical");
  }
  Double_t xX      = 0.;
  Double_t yY      = 0.;
  Double_t zZ      = 0.;
  Double_t maxDist = 0.;

  for (map<string, CbmRichRecGeoParPmt>::iterator it = gp->fPmtMap.begin();
       it != gp->fPmtMap.end();
       it++) {
    Double_t pmtPlaneX = it->second.fPlaneX;
    Double_t pmtPlaneY = it->second.fPlaneY;
    Double_t pmtPlaneZ = it->second.fPlaneZ;
    Double_t pmtPhi    = it->second.fPhi;
    Double_t pmtTheta  = it->second.fTheta;

    if (!(pmtPlaneX >= 0 && pmtPlaneY >= 0)) continue;

    double rho2 = 0.;

    if (centerP->y() > 0) {
      rho2 =
        (-TMath::Sin(pmtPhi) * (pmtPlaneX - crossP->x())
         - TMath::Sin(pmtTheta) * TMath::Cos(pmtPhi) * (pmtPlaneY - crossP->y())
         + TMath::Cos(pmtTheta) * TMath::Cos(pmtPhi)
             * (pmtPlaneZ - crossP->z()))
        / (-TMath::Sin(pmtPhi) * ref->x()
           - TMath::Sin(pmtTheta) * TMath::Cos(pmtPhi) * ref->y()
           + TMath::Cos(pmtTheta) * TMath::Cos(pmtPhi) * ref->z());
    }
    if (centerP->y() < 0) {
      rho2 = (-TMath::Sin(pmtPhi) * (pmtPlaneX - crossP->x())
              - TMath::Sin(-pmtTheta) * TMath::Cos(pmtPhi)
                  * (-pmtPlaneY - crossP->y())
              + TMath::Cos(-pmtTheta) * TMath::Cos(pmtPhi)
                  * (pmtPlaneZ - crossP->z()))
             / (-TMath::Sin(pmtPhi) * ref->x()
                - TMath::Sin(-pmtTheta) * TMath::Cos(pmtPhi) * ref->y()
                + TMath::Cos(-pmtTheta) * TMath::Cos(pmtPhi) * ref->z());
    }

    Double_t cxX = crossP->x() + ref->x() * rho2;
    Double_t cyY = crossP->y() + ref->y() * rho2;
    Double_t czZ = crossP->z() + ref->z() * rho2;

    if (cxX < 0) {
      if (centerP->y() > 0) {
        rho2 = (-TMath::Sin(-pmtPhi) * (-pmtPlaneX - crossP->x())
                - TMath::Sin(pmtTheta) * TMath::Cos(-pmtPhi)
                    * (pmtPlaneY - crossP->y())
                + TMath::Cos(pmtTheta) * TMath::Cos(-pmtPhi)
                    * (pmtPlaneZ - crossP->z()))
               / (-TMath::Sin(-pmtPhi) * ref->x()
                  - TMath::Sin(pmtTheta) * TMath::Cos(-pmtPhi) * ref->y()
                  + TMath::Cos(pmtTheta) * TMath::Cos(-pmtPhi) * ref->z());
      }
      if (centerP->y() < 0) {
        rho2 = (-TMath::Sin(-pmtPhi) * (-pmtPlaneX - crossP->x())
                - TMath::Sin(-pmtTheta) * TMath::Cos(-pmtPhi)
                    * (-pmtPlaneY - crossP->y())
                + TMath::Cos(-pmtTheta) * TMath::Cos(-pmtPhi)
                    * (pmtPlaneZ - crossP->z()))
               / (-TMath::Sin(-pmtPhi) * ref->x()
                  - TMath::Sin(-pmtTheta) * TMath::Cos(-pmtPhi) * ref->y()
                  + TMath::Cos(-pmtTheta) * TMath::Cos(-pmtPhi) * ref->z());
      }

      cxX = crossP->x() + ref->x() * rho2;
      cyY = crossP->y() + ref->y() * rho2;
      czZ = crossP->z() + ref->z() * rho2;
    }

    Double_t dP = TMath::Sqrt((crossP->x() - cxX) * (crossP->X() - cxX)
                              + (crossP->y() - cyY) * (crossP->y() - cyY)
                              + (crossP->z() - czZ) * (crossP->Z() - czZ));

    if (dP >= maxDist) {
      maxDist = dP;
      xX      = cxX;
      yY      = cyY;
      zZ      = czZ;
    }
  }

  outPoint->SetXYZ(xX, yY, zZ);
}
