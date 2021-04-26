/**
 * \file CbmRichProjectionProducerAnalytical.cxx
 *
 * \author S.Lebedev (initial version by P.Stolpovsky (2005))
 * \date 2016
 **/

#include "CbmRichProjectionProducerAnalytical.h"

#include "CbmEvent.h"
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


CbmRichProjectionProducerAnalytical::CbmRichProjectionProducerAnalytical() {}

CbmRichProjectionProducerAnalytical::~CbmRichProjectionProducerAnalytical() {}


void CbmRichProjectionProducerAnalytical::Init()
{
  FairRootManager* manager = FairRootManager::Instance();
  if (nullptr == manager) LOG(fatal) << "CbmRichProjectionProducerAnalytical::Init(): FairRootManager is nullptr.";

  fTrackParams = (TClonesArray*) manager->GetObject("RichTrackParamZ");
  if (fTrackParams == nullptr) LOG(fatal) << "CbmRichProjectionProducerAnalytical::Init(): No RichTrackParamZ array.";
}

void CbmRichProjectionProducerAnalytical::DoProjection(CbmEvent* event, TClonesArray* richProj)
{

  if (richProj == nullptr) {
    LOG(error) << "CbmRichProjectionProducerAnalytical::DoExtrapolation(): richProj is nullptr.";
    return;
  }

  fEventNum++;

  CbmRichRecGeoPar* gp = CbmRichGeoManager::GetInstance().fGP;
  Double_t mirX        = gp->fMirrorX;
  Double_t mirY        = gp->fMirrorY;
  Double_t mirZ        = gp->fMirrorZ;
  Double_t mirR        = gp->fMirrorR;

  TMatrixFSym covMat(5);
  for (Int_t i = 0; i < 5; i++) {
    for (Int_t j = 0; j <= i; j++) {
      covMat(i, j) = 0;
    }
  }
  covMat(0, 0) = covMat(1, 1) = covMat(2, 2) = covMat(3, 3) = covMat(4, 4) = 1.e-4;

  Int_t nofTrackParams = event ? event->GetNofData(ECbmDataType::kRichTrackParamZ) : fTrackParams->GetEntriesFast();
  for (Int_t iT0 = 0; iT0 < nofTrackParams; iT0++) {
    Int_t iT              = event ? event->GetIndex(ECbmDataType::kRichTrackParamZ, iT0) : iT0;
    FairTrackParam* trPar = static_cast<FairTrackParam*>(fTrackParams->At(iT));
    new ((*richProj)[iT]) FairTrackParam(0., 0., 0., 0., 0., 0., covMat);
    if (event != nullptr) event->AddData(ECbmDataType::kRichTrackProjection, iT);

    // check if Array was filled
    if (trPar->GetX() == 0 && trPar->GetY() == 0 && trPar->GetZ() == 0 && trPar->GetTx() == 0 && trPar->GetTy() == 0)
      continue;
    if (trPar->GetQp() == 0) continue;

    Double_t rho1 = 0.;
    TVector3 startP, momP, crossP, centerP;


    Double_t p = 1. / TMath::Abs(trPar->GetQp());
    Double_t pz;
    Double_t pz2 = 1 + trPar->GetTx() * trPar->GetTx() + trPar->GetTy() * trPar->GetTy();
    if (pz2 > 0.) pz = p / TMath::Sqrt(pz2);
    else {
      LOG(error) << "CbmRichProjectionProducerAnalytical::DoProjection(): strange value for calculating pz: " << pz2;
      pz = 0.;
    }
    Double_t px = pz * trPar->GetTx();
    Double_t py = pz * trPar->GetTy();
    momP.SetXYZ(px, py, pz);
    trPar->Position(startP);
    if ((mirY * startP.y()) < 0) mirY = -mirY;  // check that mirror center and startP are in same hemisphere

    // calculation of intersection of track with selected mirror
    // corresponds to calculation of intersection between a straight line and a sphere:
    // vector: r = startP - mirrorCenter
    // RxP = r*momP
    // normP2 = momP^2
    // dist = r^2 - fR^2
    // -> rho1 = (-RxP+sqrt(RxP^2-normP2*dist))/normP2  extrapolation factor for:
    // intersection point crossP = startP + rho1 * momP
    Double_t RxP = (momP.x() * (startP.x() - mirX) + momP.y() * (startP.y() - mirY) + momP.z() * (startP.z() - mirZ));
    Double_t normP2 = (momP.x() * momP.x() + momP.y() * momP.y() + momP.z() * momP.z());
    Double_t dist =
      (startP.x() * startP.x() + mirX * mirX + startP.y() * startP.y() + mirY * mirY + startP.z() * startP.z()
       + mirZ * mirZ - 2 * startP.x() * mirX - 2 * startP.y() * mirY - 2 * startP.z() * mirZ - mirR * mirR);

    if ((RxP * RxP - normP2 * dist) > 0.) {
      if (normP2 != 0.) rho1 = (-RxP + TMath::Sqrt(RxP * RxP - normP2 * dist)) / normP2;
      if (normP2 == 0) LOG(error) << "CbmRichProjectionProducerAnalytical::DoProjection(): normP2 == 0";
    }
    else {
      //cout << " -E- RichProjectionProducer:  RxP*RxP-normP2*dist = " << RxP*RxP-normP2*dist << endl;
    }

    Double_t crossPx = startP.x() + rho1 * momP.x();
    Double_t crossPy = startP.y() + rho1 * momP.y();
    Double_t crossPz = startP.z() + rho1 * momP.z();
    crossP.SetXYZ(crossPx, crossPy, crossPz);

    // check if crosspoint with mirror and chosen mirrorcenter (y) are in same hemisphere
    // if not recalculate crossing point
    if ((mirY * crossP.y()) < 0) {
      mirY   = -mirY;
      RxP    = (momP.x() * (startP.x() - mirX) + momP.y() * (startP.y() - mirY) + momP.z() * (startP.z() - mirZ));
      normP2 = (momP.x() * momP.x() + momP.y() * momP.y() + momP.z() * momP.z());
      dist   = (startP.x() * startP.x() + mirX * mirX + startP.y() * startP.y() + mirY * mirY + startP.z() * startP.z()
              + mirZ * mirZ - 2 * startP.x() * mirX - 2 * startP.y() * mirY - 2 * startP.z() * mirZ - mirR * mirR);

      if ((RxP * RxP - normP2 * dist) > 0.) {
        if (normP2 != 0.) rho1 = (-RxP + TMath::Sqrt(RxP * RxP - normP2 * dist)) / normP2;
        if (normP2 == 0) LOG(error) << "CbmRichProjectionProducerAnalytical::DoProjection(): normP2 == 0";
      }
      else {
        //cout << " -E- RichProjectionProducer:  RxP*RxP-normP2*dist = " << RxP*RxP-normP2*dist << endl;
      }

      crossPx = startP.x() + rho1 * momP.x();
      crossPy = startP.y() + rho1 * momP.y();
      crossPz = startP.z() + rho1 * momP.z();
      crossP.SetXYZ(crossPx, crossPy, crossPz);
    }

    centerP.SetXYZ(mirX, mirY, mirZ);  // mirror center


    //   calculate normal on crosspoint with mirror
    TVector3 normP(crossP.x() - centerP.x(), crossP.y() - centerP.y(), crossP.z() - centerP.z());
    normP = normP.Unit();
    // check that normal has same z-direction as momentum
    if ((normP.z() * momP.z()) < 0.) normP = TVector3(-1. * normP.x(), -1. * normP.y(), -1. * normP.z());

    // reflect track
    Double_t np = normP.x() * momP.x() + normP.y() * momP.y() + normP.z() * momP.z();

    TVector3 ref;
    ref.SetXYZ(2 * np * normP.x() - momP.x(), 2 * np * normP.y() - momP.y(), 2 * np * normP.z() - momP.z());

    if (ref.z() != 0.) {

      TVector3 inPos(0., 0., 0.), outPos(0., 0., 0.);


      if (gp->fGeometryType == CbmRichGeometryTypeCylindrical) {
        GetPmtIntersectionPointCyl(&centerP, &crossP, &ref, &inPos);
        // Transform intersection point in same way as MCPoints were transformed in HitProducer before stored as Hit
        // For the cylindrical geometry we also pass the path to the strip block
        CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);

        //cout << "inPoint:" << inPos.X() << " " << inPos.Y() << " " << inPos.Z() << " outPoint:" << outPos.X() << " " << outPos.Y() << " " << outPos.Z()  << endl;
      }
      else if (gp->fGeometryType == CbmRichGeometryTypeTwoWings) {
        GetPmtIntersectionPointTwoWings(&centerP, &crossP, &ref, &inPos);
        // Transform intersection point in same way as MCPoints were transformed in HitProducer before stored as Hit
        CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);
      }
      else {
        LOG(fatal) << "CbmRichProjectionProducerAnalytical::DoProjection(): unknown geometry type";
      }

      Bool_t isInsidePmt = CbmRichGeoManager::GetInstance().IsPointInsidePmt(&outPos);

      if (isInsidePmt) {
        FairTrackParam richtrack(outPos.x(), outPos.y(), outPos.z(), 0., 0., 0., covMat);
        *(FairTrackParam*) (richProj->At(iT)) = richtrack;
      }
    }
  }
}

void CbmRichProjectionProducerAnalytical::GetPmtIntersectionPointTwoWings(const TVector3* centerP,
                                                                          const TVector3* crossP, const TVector3* ref,
                                                                          TVector3* outPoint)
{
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
    LOG(fatal) << "CbmRichProjectionProducerAnalytical::GetPmtIntersectionPointTwoWings(): Wrong geometry, this method "
                  "could be used only for CbmRichGeometryTypeTwoWings";
  }

  Double_t pmtPhi    = gp->fPmt.fPhi;
  Double_t pmtTheta  = gp->fPmt.fTheta;
  Double_t pmtPlaneX = gp->fPmt.fPlaneX;
  Double_t pmtPlaneY = gp->fPmt.fPlaneY;
  Double_t pmtPlaneZ = gp->fPmt.fPlaneZ;
  Double_t rho2      = 0.;

  if (centerP->y() > 0) {
    rho2 = (-TMath::Sin(pmtPhi) * (pmtPlaneX - crossP->x())
            - TMath::Sin(pmtTheta) * TMath::Cos(pmtPhi) * (pmtPlaneY - crossP->y())
            + TMath::Cos(pmtTheta) * TMath::Cos(pmtPhi) * (pmtPlaneZ - crossP->z()))
           / (-TMath::Sin(pmtPhi) * ref->x() - TMath::Sin(pmtTheta) * TMath::Cos(pmtPhi) * ref->y()
              + TMath::Cos(pmtTheta) * TMath::Cos(pmtPhi) * ref->z());
  }
  if (centerP->y() < 0) {
    rho2 = (-TMath::Sin(pmtPhi) * (pmtPlaneX - crossP->x())
            - TMath::Sin(-pmtTheta) * TMath::Cos(pmtPhi) * (-pmtPlaneY - crossP->y())
            + TMath::Cos(-pmtTheta) * TMath::Cos(pmtPhi) * (pmtPlaneZ - crossP->z()))
           / (-TMath::Sin(pmtPhi) * ref->x() - TMath::Sin(-pmtTheta) * TMath::Cos(pmtPhi) * ref->y()
              + TMath::Cos(-pmtTheta) * TMath::Cos(pmtPhi) * ref->z());
  }

  //rho2 = -1*(crossP.z() - fDetZ)/refZ;    // only for theta = 0, phi=0
  Double_t xX = crossP->x() + ref->x() * rho2;
  Double_t yY = crossP->y() + ref->y() * rho2;
  Double_t zZ = crossP->z() + ref->z() * rho2;

  if (xX < 0) {
    if (centerP->y() > 0) {
      rho2 = (-TMath::Sin(-pmtPhi) * (-pmtPlaneX - crossP->x())
              - TMath::Sin(pmtTheta) * TMath::Cos(-pmtPhi) * (pmtPlaneY - crossP->y())
              + TMath::Cos(pmtTheta) * TMath::Cos(-pmtPhi) * (pmtPlaneZ - crossP->z()))
             / (-TMath::Sin(-pmtPhi) * ref->x() - TMath::Sin(pmtTheta) * TMath::Cos(-pmtPhi) * ref->y()
                + TMath::Cos(pmtTheta) * TMath::Cos(-pmtPhi) * ref->z());
    }
    if (centerP->y() < 0) {
      rho2 = (-TMath::Sin(-pmtPhi) * (-pmtPlaneX - crossP->x())
              - TMath::Sin(-pmtTheta) * TMath::Cos(-pmtPhi) * (-pmtPlaneY - crossP->y())
              + TMath::Cos(-pmtTheta) * TMath::Cos(-pmtPhi) * (pmtPlaneZ - crossP->z()))
             / (-TMath::Sin(-pmtPhi) * ref->x() - TMath::Sin(-pmtTheta) * TMath::Cos(-pmtPhi) * ref->y()
                + TMath::Cos(-pmtTheta) * TMath::Cos(-pmtPhi) * ref->z());
    }

    xX = crossP->x() + ref->x() * rho2;
    yY = crossP->y() + ref->y() * rho2;
    zZ = crossP->z() + ref->z() * rho2;
  }

  outPoint->SetXYZ(xX, yY, zZ);
}


void CbmRichProjectionProducerAnalytical::GetPmtIntersectionPointCyl(const TVector3* centerP, const TVector3* crossP,
                                                                     const TVector3* ref, TVector3* outPoint)
{
  CbmRichRecGeoPar* gp = CbmRichGeoManager::GetInstance().fGP;
  if (gp->fGeometryType != CbmRichGeometryTypeCylindrical) {
    LOG(fatal) << "CbmRichProjectionProducerAnalytical::GetPmtIntersectionPointCyl(): Wrong geometry, this method "
                  "could be used only for CbmRichGeometryTypeCylindrical";
  }
  Double_t xX      = 0.;
  Double_t yY      = 0.;
  Double_t zZ      = 0.;
  Double_t maxDist = 0.;

  for (map<string, CbmRichRecGeoParPmt>::iterator it = gp->fPmtMap.begin(); it != gp->fPmtMap.end(); it++) {
    Double_t pmtPlaneX = it->second.fPlaneX;
    Double_t pmtPlaneY = it->second.fPlaneY;
    Double_t pmtPlaneZ = it->second.fPlaneZ;
    Double_t pmtPhi    = it->second.fPhi;
    Double_t pmtTheta  = it->second.fTheta;

    if (!(pmtPlaneX >= 0 && pmtPlaneY >= 0)) continue;

    double rho2 = 0.;

    if (centerP->y() > 0) {
      rho2 = (-TMath::Sin(pmtPhi) * (pmtPlaneX - crossP->x())
              - TMath::Sin(pmtTheta) * TMath::Cos(pmtPhi) * (pmtPlaneY - crossP->y())
              + TMath::Cos(pmtTheta) * TMath::Cos(pmtPhi) * (pmtPlaneZ - crossP->z()))
             / (-TMath::Sin(pmtPhi) * ref->x() - TMath::Sin(pmtTheta) * TMath::Cos(pmtPhi) * ref->y()
                + TMath::Cos(pmtTheta) * TMath::Cos(pmtPhi) * ref->z());
    }
    if (centerP->y() < 0) {
      rho2 = (-TMath::Sin(pmtPhi) * (pmtPlaneX - crossP->x())
              - TMath::Sin(-pmtTheta) * TMath::Cos(pmtPhi) * (-pmtPlaneY - crossP->y())
              + TMath::Cos(-pmtTheta) * TMath::Cos(pmtPhi) * (pmtPlaneZ - crossP->z()))
             / (-TMath::Sin(pmtPhi) * ref->x() - TMath::Sin(-pmtTheta) * TMath::Cos(pmtPhi) * ref->y()
                + TMath::Cos(-pmtTheta) * TMath::Cos(pmtPhi) * ref->z());
    }

    Double_t cxX = crossP->x() + ref->x() * rho2;
    Double_t cyY = crossP->y() + ref->y() * rho2;
    Double_t czZ = crossP->z() + ref->z() * rho2;

    if (cxX < 0) {
      if (centerP->y() > 0) {
        rho2 = (-TMath::Sin(-pmtPhi) * (-pmtPlaneX - crossP->x())
                - TMath::Sin(pmtTheta) * TMath::Cos(-pmtPhi) * (pmtPlaneY - crossP->y())
                + TMath::Cos(pmtTheta) * TMath::Cos(-pmtPhi) * (pmtPlaneZ - crossP->z()))
               / (-TMath::Sin(-pmtPhi) * ref->x() - TMath::Sin(pmtTheta) * TMath::Cos(-pmtPhi) * ref->y()
                  + TMath::Cos(pmtTheta) * TMath::Cos(-pmtPhi) * ref->z());
      }
      if (centerP->y() < 0) {
        rho2 = (-TMath::Sin(-pmtPhi) * (-pmtPlaneX - crossP->x())
                - TMath::Sin(-pmtTheta) * TMath::Cos(-pmtPhi) * (-pmtPlaneY - crossP->y())
                + TMath::Cos(-pmtTheta) * TMath::Cos(-pmtPhi) * (pmtPlaneZ - crossP->z()))
               / (-TMath::Sin(-pmtPhi) * ref->x() - TMath::Sin(-pmtTheta) * TMath::Cos(-pmtPhi) * ref->y()
                  + TMath::Cos(-pmtTheta) * TMath::Cos(-pmtPhi) * ref->z());
      }

      cxX = crossP->x() + ref->x() * rho2;
      cyY = crossP->y() + ref->y() * rho2;
      czZ = crossP->z() + ref->z() * rho2;
    }

    Double_t dP = TMath::Sqrt((crossP->x() - cxX) * (crossP->X() - cxX) + (crossP->y() - cyY) * (crossP->y() - cyY)
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
