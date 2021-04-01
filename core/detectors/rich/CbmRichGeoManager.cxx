/*
 * CbmRichGeoManager.cxx
 *
 *  Created on: Dec 16, 2015
 *      Author: slebedev
 */
#include "CbmRichGeoManager.h"

#include "CbmRichRecGeoPar.h"  // for CbmRichRecGeoParPmt, CbmRichRecGeoPar

#include <Logger.h>  // for LOG

#include <TGeoBBox.h>     // for TGeoBBox
#include <TGeoManager.h>  // for TGeoManager, gGeoManager
#include <TGeoMatrix.h>   // for TGeoMatrix
#include <TGeoNode.h>     // for TGeoIterator, TGeoNode
#include <TGeoShape.h>    // for TGeoShape
#include <TGeoSphere.h>   // for TGeoSphere
#include <TGeoVolume.h>   // for TGeoVolume
#include <TMath.h>        // for ASin, Cos, ACos, Sin, DegToRad, Sqrt
#include <TMathBase.h>    // for Abs, Max, Min
#include <TString.h>      // for TString, operator==
#include <TVector3.h>     // for TVector3

#include <algorithm>  // for sort
#include <map>        // for map, __map_iterator, map<>::iterator
#include <string>     // for operator<, operator+
#include <utility>    // for pair
#include <vector>     // for vector

#include <stddef.h>  // for size_t

using namespace std;

CbmRichGeoManager::CbmRichGeoManager() : fGP(nullptr) { InitGeometry(); }

void CbmRichGeoManager::InitGeometry() {

  LOG(info) << "CbmRichGeoManager::InitGeometry";

  fGP = new CbmRichRecGeoPar();
  //TODO: get refractive index from material
  fGP->fNRefrac = 1.000446242;

  DetectGeometryType();

  if (fGP->fGeometryType == CbmRichGeometryTypeTwoWings) {
    LOG(info) << "CbmRichGeoManager: Init CbmRichGeometryTypeTwoWings";
    InitPmt();
  } else if (fGP->fGeometryType == CbmRichGeometryTypeCylindrical) {
    LOG(info) << "CbmRichGeoManager: Init CbmRichGeometryTypeCylindrical";
    InitPmtCyl();
  } else if (fGP->fGeometryType == CbmRichGeometryTypeNotDefined) {
    // Fatal("CbmRichGeoManager::InitGeometry()", " Geometry type is CbmRichGeometryTypeNotDefined. Geometry could not be defined automatically.");
    LOG(info);
    LOG(error) << "CbmRichGeoManager::InitGeometry(): Geometry type is "
                  "CbmRichGeometryTypeNotDefined. Geometry could not be "
                  "defined automatically.";
  }

  InitMirror();

  //fGP->Print();
}

void CbmRichGeoManager::DetectGeometryType() {
  TGeoIterator geoIterator(gGeoManager->GetTopNode()->GetVolume());
  geoIterator.SetTopName("/cave_1");
  TGeoNode* curNode;

  geoIterator.Reset();
  while ((curNode = geoIterator())) {
    TString nodeName(curNode->GetName());
    TString nodePath;
    // if (curNode->GetVolume()->GetName() == TString("camera_two_quarters")) {
    if (curNode->GetVolume()->GetName() == TString("camera_strip")) {
      fGP->fGeometryType = CbmRichGeometryTypeCylindrical;
      return;
    }
  }

  geoIterator.Reset();
  while ((curNode = geoIterator())) {
    TString nodeName(curNode->GetName());
    TString nodePath;
    if (curNode->GetVolume()->GetName() == TString("camera_quarter")) {
      fGP->fGeometryType = CbmRichGeometryTypeTwoWings;
      return;
    }
  }

  fGP->fGeometryType = CbmRichGeometryTypeNotDefined;
}

void CbmRichGeoManager::InitPmtCyl() {
  TGeoIterator geoIterator(gGeoManager->GetTopNode()->GetVolume());
  geoIterator.SetTopName("/cave_1");
  TGeoNode* curNode;

  vector<Double_t> xCoord;
  geoIterator.Reset();
  while ((curNode = geoIterator())) {
    TString nodeName(curNode->GetName());
    TString nodePath;
    // if (curNode->GetVolume()->GetName() == TString("pmt_block_strip")) {
    if (curNode->GetVolume()->GetName() == TString("camera_strip")) {
      geoIterator.GetPath(nodePath);
      //cout << "nodePath:" << nodePath << endl;
      const TGeoMatrix* curMatrix = geoIterator.GetCurrentMatrix();
      const Double_t* curNodeTr   = curMatrix->GetTranslation();
      const Double_t* curNodeRot  = curMatrix->GetRotationMatrix();

      //curMatrix->Print();

      double pmtX = curNodeTr[0];
      double pmtY = curNodeTr[1];
      double pmtZ = curNodeTr[2];

      double rotY = TMath::ASin(-curNodeRot[2]);  // around Y
      //            double rotZ = TMath::ASin(curNodeRot[1]/TMath::Cos(TMath::ASin(-curNodeRot[2]))); // around Z
      //double rotX = TMath::ASin(curNodeRot[5]/TMath::Cos(TMath::ASin(-curNodeRot[2]))); // around X
      double rotX = TMath::ACos(
        curNodeRot[8] / TMath::Cos(TMath::ASin(-curNodeRot[2])));  // around X


      //double rotX = TMath::ATan2(curNodeRot[5], curNodeRot[8]);          // around Y
      //double c2 = TMath::Sqrt(curNodeRot[0]*curNodeRot[0] + curNodeRot[1]*curNodeRot[1]);
      //double rotY = TMath::ATan2(-curNodeRot[2], c2); // around X

      string nodePathStr = string(nodePath.Data()) + "/";

      fGP->fPmtMap[nodePathStr].fTheta = rotX;
      fGP->fPmtMap[nodePathStr].fPhi   = rotY;
      const TGeoBBox* shape =
        (const TGeoBBox*) (curNode->GetVolume()->GetShape());

      fGP->fPmtMap[nodePathStr].fWidth  = 2. * shape->GetDX();
      fGP->fPmtMap[nodePathStr].fHeight = 2. * shape->GetDY();
      fGP->fPmtMap[nodePathStr].fZ      = pmtZ;
      fGP->fPmtMap[nodePathStr].fX      = pmtX;
      fGP->fPmtMap[nodePathStr].fY      = pmtY;

      if (pmtX >= 0 && pmtY >= 0) { xCoord.push_back(pmtX); }
    }
  }
  std::sort(xCoord.begin(), xCoord.end());

  for (map<string, CbmRichRecGeoParPmt>::iterator it = fGP->fPmtMap.begin();
       it != fGP->fPmtMap.end();
       it++) {
    Double_t curX = TMath::Abs(it->second.fX);
    int pos       = -1;
    //int pos = find(xCoord.begin(), xCoord.end(), curX) - xCoord.begin();
    for (unsigned int i = 0; i < xCoord.size(); i++) {
      if (TMath::Abs(curX - xCoord[i]) < 0.1) {
        pos = i;
        break;
      }
    }
    it->second.fPmtPositionIndexX = pos;
  }

  // We also need to find pmt plane center
  map<string, CbmRichPmtPlaneMinMax> mapPmtPlaneMinMax;
  TString filterNamePixel("pmt_pixel");
  geoIterator.Reset();
  CbmRichPmtPlaneMinMax pmtPlaneMinMax;

  while ((curNode = geoIterator())) {
    TString nodeName(curNode->GetName());
    TString nodePath;
    if (curNode->GetVolume()->GetName() == filterNamePixel) {

      geoIterator.GetPath(nodePath);


      string nodePathStr = string(nodePath.Data());
      //size_t foundIndex1 = nodePathStr.find("pmt_block_strip");
      size_t foundIndex1 = nodePathStr.find("camera_strip");

      if (foundIndex1 == string::npos) continue;
      size_t foundIndex2 = nodePathStr.find("/", foundIndex1 + 1);
      if (foundIndex2 == string::npos) continue;

      string mapKey = nodePathStr.substr(0, foundIndex2) + "/";

      const TGeoMatrix* curMatrix = geoIterator.GetCurrentMatrix();
      const Double_t* curNodeTr   = curMatrix->GetTranslation();

      double pmtX = curNodeTr[0];
      double pmtY = curNodeTr[1];
      double pmtZ = curNodeTr[2];

      mapPmtPlaneMinMax[mapKey].AddPoint(pmtX, pmtY, pmtZ);
    }
  }

  for (map<string, CbmRichRecGeoParPmt>::iterator it = fGP->fPmtMap.begin();
       it != fGP->fPmtMap.end();
       it++) {
    it->second.fPlaneX = mapPmtPlaneMinMax[it->first].GetMeanX();
    it->second.fPlaneY = mapPmtPlaneMinMax[it->first].GetMeanY();
    it->second.fPlaneZ = mapPmtPlaneMinMax[it->first].GetMeanZ();

    //        cout << "name:" << it->first << " strip(x,y,z):" <<it->second.fX << "," << it->second.fY << "," << it->second.fZ <<
    //         " pmtPlane(x,y,z):" <<it->second.fPlaneX << "," << it->second.fPlaneY << "," << it->second.fPlaneZ << ", " <<
    //         "theta:" << it->second.fTheta << ", phi:" << it->second.fPhi << endl;
  }

  // Calculate gap between camera_strip
  geoIterator.Reset();
  double master1[3], master2[3];
  while ((curNode = geoIterator())) {
    TString nodeName(curNode->GetName());
    TString nodePath;
    if (curNode->GetVolume()->GetName() == TString("camera_strip")) {

      geoIterator.GetPath(nodePath);
      string nodePathStr          = string(nodePath.Data()) + "/";
      const TGeoMatrix* curMatrix = geoIterator.GetCurrentMatrix();
      const Double_t* curNodeTr   = curMatrix->GetTranslation();
      //            const Double_t* curNodeRot = curMatrix->GetRotationMatrix();

      double pmtX = curNodeTr[0];
      double pmtY = curNodeTr[1];
      //            double pmtZ = curNodeTr[2];

      if (pmtX < 0 || pmtY < 0) continue;
      const TGeoBBox* shape =
        (const TGeoBBox*) (curNode->GetVolume()->GetShape());


      double loc[3];
      if (fGP->fPmtMap[nodePathStr].fPmtPositionIndexX == 1) {
        loc[0] = shape->GetDX() + shape->GetOrigin()[0];
        loc[1] = shape->GetDY() + shape->GetOrigin()[1];
        loc[2] = shape->GetDZ() + shape->GetOrigin()[2];
        curMatrix->LocalToMaster(loc, master1);
      } else if (fGP->fPmtMap[nodePathStr].fPmtPositionIndexX == 2) {
        loc[0] = -shape->GetDX() + shape->GetOrigin()[0];
        loc[1] = shape->GetDY() + shape->GetOrigin()[1];
        loc[2] = shape->GetDZ() + shape->GetOrigin()[2];
        curMatrix->LocalToMaster(loc, master2);
      }
    }
  }
  //cout  << master1[0] << " "<< master1[1] << " "<< master1[2]<< endl;
  //cout  << master2[0] << " "<< master2[1] << " "<< master2[2]<< endl;
  double dist =
    TMath::Sqrt((master1[0] - master2[0]) * (master1[0] - master2[0])
                + (master1[1] - master2[1]) * (master1[1] - master2[1])
                + (master1[2] - master2[2]) * (master1[2] - master2[2]));
  //cout << "Gap:" << dist << endl;
  fGP->fPmtStripGap = dist;  // v17a = 0.3083394
}

void CbmRichGeoManager::InitPmt() {
  TGeoIterator geoIterator(gGeoManager->GetTopNode()->GetVolume());
  TGeoNode* curNode;

  // PMT plane position\rotation
  TString filterName_pixel("pmt_pixel");
  geoIterator.Reset();
  CbmRichPmtPlaneMinMax pmtPlaneMinMax;
  while ((curNode = geoIterator())) {
    TString nodeName(curNode->GetName());
    TString nodePath;
    if (curNode->GetVolume()->GetName() == filterName_pixel) {

      geoIterator.GetPath(nodePath);
      const TGeoMatrix* curMatrix = geoIterator.GetCurrentMatrix();
      const Double_t* curNodeTr   = curMatrix->GetTranslation();
      const Double_t* curNodeRot  = curMatrix->GetRotationMatrix();

      double pmtX = curNodeTr[0];
      double pmtY = curNodeTr[1];
      double pmtZ = curNodeTr[2];

      if (pmtX > 0. && pmtY > 0) {
        //printf ("%08f\t%08f\t%08f\t\n", curNodeTranslation[0], curNodeTranslation[1], curNodeTranslation[2]);
        double rotY = TMath::ASin(-curNodeRot[2]);  // around Y
        //                double rotZ = TMath::ASin(curNodeRot[1]/TMath::Cos(TMath::ASin(-curNodeRot[2]))); // around Z
        //double rotX = TMath::ASin(curNodeRot[5]/TMath::Cos(TMath::ASin(-curNodeRot[2]))); // around X
        double rotX = TMath::ACos(
          curNodeRot[8] / TMath::Cos(TMath::ASin(-curNodeRot[2])));  // around X

        fGP->fPmt.fTheta = rotX;
        fGP->fPmt.fPhi   = rotY;

        pmtPlaneMinMax.AddPoint(pmtX, pmtY, pmtZ);
      }
    }
  }

  fGP->fPmt.fPlaneX = pmtPlaneMinMax.GetMeanX();
  fGP->fPmt.fPlaneY = pmtPlaneMinMax.GetMeanY();
  fGP->fPmt.fPlaneZ = pmtPlaneMinMax.GetMeanZ();

  geoIterator.Reset();
  while ((curNode = geoIterator())) {
    TString nodeName(curNode->GetName());
    TString nodePath;
    if (curNode->GetVolume()->GetName() == TString("camera_quarter")) {

      geoIterator.GetPath(nodePath);
      const TGeoMatrix* curMatrix = geoIterator.GetCurrentMatrix();
      const Double_t* curNodeTr   = curMatrix->GetTranslation();
      //const Double_t* curNodeRot = curMatrix->GetRotationMatrix();


      double pmtX = curNodeTr[0];
      double pmtY = curNodeTr[1];
      double pmtZ = curNodeTr[2];

      if (pmtX > 0. && pmtY > 0) {
        const TGeoBBox* shape =
          (const TGeoBBox*) (curNode->GetVolume()->GetShape());
        fGP->fPmt.fWidth  = shape->GetDX();
        fGP->fPmt.fHeight = shape->GetDY();
        fGP->fPmt.fZ      = pmtZ;
        fGP->fPmt.fX      = pmtX;
        fGP->fPmt.fY      = pmtY;
      }
    }
  }
}

void CbmRichGeoManager::InitMirror() {

  TGeoIterator geoIterator(gGeoManager->GetTopNode()->GetVolume());
  TGeoNode* curNode;
  geoIterator.Reset();

  //mirror position\rotation
  TString mirrorName0("mirror_tile_type0");
  TString mirrorName1("mirror_tile_type1");
  TString mirrorName2("mirror_tile_type2");
  TString mirrorName3("mirror_tile_type3");
  //TString mirrorName4("mirror_tile_type4");
  //TString mirrorName5("mirror_tile_type5");

  // these names are needed for misaligned geometry
  TString mirrorMisAlignName0("mirror_tile_0");
  TString mirrorMisAlignName1("mirror_tile_1");
  TString mirrorMisAlignName2("mirror_tile_2");
  TString mirrorMisAlignName3("mirror_tile_3");
  TString mirrorMisAlignName4("mirror_tile_4");
  TString mirrorMisAlignName5("mirror_tile_5");
  TString mirrorMisAlignName6("mirror_tile_6");
  TString mirrorMisAlignName7("mirror_tile_7");

  geoIterator.Reset();
  double maxTheta     = 0.;
  double minTheta     = 999999999.;
  double mirrorX      = 0.;
  double mirrorY      = 0.;
  double mirrorZ      = 0.;
  double mirrorRadius = 0.;
  while ((curNode = geoIterator())) {
    TString nodeName(curNode->GetName());
    TString nodePath;

    if (nodeName.Contains(mirrorName0) || nodeName.Contains(mirrorName1)
        || nodeName.Contains(mirrorName2) || nodeName.Contains(mirrorName3)
        || nodeName.Contains(mirrorMisAlignName0)
        || nodeName.Contains(mirrorMisAlignName1)
        || nodeName.Contains(mirrorMisAlignName2)
        || nodeName.Contains(mirrorMisAlignName3)
        || nodeName.Contains(mirrorMisAlignName4)
        || nodeName.Contains(mirrorMisAlignName5)
        || nodeName.Contains(mirrorMisAlignName6)
        || nodeName.Contains(mirrorMisAlignName7)) {
      geoIterator.GetPath(nodePath);
      const TGeoMatrix* curMatrix = geoIterator.GetCurrentMatrix();
      const Double_t* curNodeTr   = curMatrix->GetTranslation();
      mirrorX                     = TMath::Abs(curNodeTr[0]);
      mirrorY                     = TMath::Abs(curNodeTr[1]);
      mirrorZ                     = TMath::Abs(curNodeTr[2]);

      const TGeoSphere* shape =
        dynamic_cast<const TGeoSphere*>(curNode->GetVolume()->GetShape());
      //const TGeoSphere* shape = (const TGeoSphere*)(curNode->GetVolume()->GetShape());
      if (shape != nullptr) {
        mirrorRadius = shape->GetRmin();

        double theta1 = shape->GetTheta1();
        double theta2 = shape->GetTheta2();
        if (maxTheta < theta1 || maxTheta < theta2) {
          maxTheta = TMath::Max(theta1, theta2);
        }
        if (minTheta > theta1 || minTheta > theta2) {
          minTheta = TMath::Min(theta1, theta2);
        }
      }
    }
  }

  fGP->fMirrorTheta = -((maxTheta + minTheta) / 2. - 90.)
                      * TMath::DegToRad();  // rotation angle around x-axis
  fGP->fMirrorX = mirrorX;
  fGP->fMirrorY = mirrorY;
  fGP->fMirrorZ = mirrorZ;
  fGP->fMirrorR = mirrorRadius;
}


void CbmRichGeoManager::RotatePoint(TVector3* inPos,
                                    TVector3* outPos,
                                    Bool_t noTilting) {
  if (fGP == nullptr) {
    LOG(error) << "CbmRichGeoManager::RotatePoint RICH geometry is not "
                  "initialized. fGP == nullptr";
  }

  if (fGP->fGeometryType == CbmRichGeometryTypeTwoWings) {
    RotatePointTwoWings(inPos, outPos, noTilting);
  } else if (fGP->fGeometryType == CbmRichGeometryTypeCylindrical) {
    RotatePointCyl(inPos, outPos, noTilting);
  } else {
    outPos->SetXYZ(inPos->X(), inPos->Y(), inPos->Z());
  }
}

void CbmRichGeoManager::RotatePointTwoWings(TVector3* inPos,
                                            TVector3* outPos,
                                            Bool_t noTilting) {
  if (noTilting == false) {
    RotatePointImpl(inPos,
                    outPos,
                    fGP->fPmt.fPhi,
                    fGP->fPmt.fTheta,
                    fGP->fPmt.fX,
                    fGP->fPmt.fY,
                    fGP->fPmt.fZ);
  } else {
    outPos->SetXYZ(inPos->X(), inPos->Y(), inPos->Z());
  }
}


void CbmRichGeoManager::RotatePointCyl(TVector3* inPos,
                                       TVector3* outPos,
                                       Bool_t noTilting,
                                       Bool_t noShift) {
  if (noTilting == false) {
    //        TGeoNode* node = gGeoManager->FindNode(inPos->X(), inPos->Y(), inPos->Z());
    gGeoManager->FindNode(inPos->X(), inPos->Y(), inPos->Z());
    string path(gGeoManager->GetPath());


    CbmRichRecGeoParPmt pmtPar =
      fGP->GetGeoRecPmtByBlockPathOrClosest(path, inPos);

    RotatePointImpl(inPos,
                    outPos,
                    -TMath::Abs(pmtPar.fPhi),
                    TMath::Abs(pmtPar.fTheta),
                    TMath::Abs(pmtPar.fX),
                    TMath::Abs(pmtPar.fY),
                    TMath::Abs(pmtPar.fZ));

    // After rotation wee need to shift point
    if (!noShift) {
      // All pmt blocks centers will be move to this Y position
      // TODO: We can also take smallest Y from all pmt blocks
      Double_t baseLineY = (outPos->Y() >= 0) ? 160. : -160.;  //cm
      Double_t dY        = pmtPar.fY - baseLineY;
      outPos->SetY(outPos->Y() - dY);

      // Calculate pmt block center after rotation
      TVector3 inPosPmt(pmtPar.fX, pmtPar.fY, pmtPar.fZ);
      TVector3 outPosPmt;
      RotatePointImpl(&inPosPmt,
                      &outPosPmt,
                      -TMath::Abs(pmtPar.fPhi),
                      TMath::Abs(pmtPar.fTheta),
                      TMath::Abs(pmtPar.fX),
                      TMath::Abs(pmtPar.fY),
                      TMath::Abs(pmtPar.fZ));
      //RotatePointCyl(&inPosPmt, &outPosPmt, false, true);

      // calculate ideal position assuming the same width for all pmt blocks
      //TODO:Actually we need to implement general solution if pmt-block widths are not the same
      Double_t gap     = fGP->fPmtStripGap;
      Double_t padding = gap / 2.;
      Double_t idealX  = padding + 0.5 * pmtPar.fWidth
                        + pmtPar.fPmtPositionIndexX * (pmtPar.fWidth + gap);
      if (outPos->X() < 0) idealX = -idealX;
      Double_t dX = idealX - outPosPmt.X();

      outPos->SetX(outPos->X() + dX);
    }
  } else {
    outPos->SetXYZ(inPos->X(), inPos->Y(), inPos->Z());
  }
}


void CbmRichGeoManager::RotatePointImpl(TVector3* inPos,
                                        TVector3* outPos,
                                        Double_t phi,
                                        Double_t theta,
                                        Double_t pmtX,
                                        Double_t pmtY,
                                        Double_t pmtZ) {
  Double_t xDet = 0., yDet = 0., zDet = 0.;
  Double_t x = inPos->X();
  Double_t y = inPos->Y();
  Double_t z = inPos->Z();

  Double_t sinTheta = TMath::Sin(theta);
  Double_t cosTheta = TMath::Cos(theta);
  Double_t sinPhi   = TMath::Sin(phi);
  Double_t cosPhi   = TMath::Cos(phi);

  if (x > 0 && y > 0) {
    y -= pmtY;
    x -= pmtX;
    z -= pmtZ;
    //xDet = x*cosPhi + z*sinPhi;// - detZOrig*sinPhi;
    //yDet = -x*sinTheta*sinPhi + y*cosTheta + z*sinTheta*cosPhi;
    //zDet = -x*cosTheta*sinPhi - y*sinTheta + z*cosTheta*cosPhi;

    xDet = x * cosPhi - y * sinPhi * sinTheta + z * cosTheta * sinPhi;
    yDet = y * cosTheta + z * sinTheta;
    zDet = -x * sinPhi - y * sinTheta * cosPhi + z * cosTheta * cosPhi;

    yDet += pmtY;
    xDet += pmtX;
    zDet += pmtZ;

  } else if (x > 0 && y < 0) {
    y += pmtY;
    x -= pmtX;
    z -= pmtZ;
    // xDet = x*cosPhi + z*sinPhi;// - detZOrig*sinPhi;
    //yDet = x*sinTheta*sinPhi + y*cosTheta - z*sinTheta*cosPhi;
    //zDet = -x*cosTheta*sinPhi + y*sinTheta + z*cosTheta*cosPhi;

    xDet = x * cosPhi + y * sinPhi * sinTheta + z * cosTheta * sinPhi;
    yDet = y * cosTheta - z * sinTheta;
    zDet = -x * sinPhi + y * sinTheta * cosPhi + z * cosTheta * cosPhi;

    yDet -= pmtY;
    xDet += pmtX;
    zDet += pmtZ;
  } else if (x < 0 && y < 0) {
    y += pmtY;
    x += pmtX;
    z -= pmtZ;
    // xDet = x*cosPhi - z*sinPhi;// + detZOrig*sinPhi;
    //yDet = -x*sinTheta*sinPhi + y*cosTheta - z*sinTheta*cosPhi;
    //zDet = x*cosTheta*sinPhi + y*sinTheta + z*cosTheta*cosPhi;

    xDet = x * cosPhi - y * sinPhi * sinTheta - z * cosTheta * sinPhi;
    yDet = y * cosTheta - z * sinTheta;
    zDet = x * sinPhi + y * sinTheta * cosPhi + z * cosTheta * cosPhi;

    yDet -= pmtY;
    xDet -= pmtX;
    zDet += pmtZ;
  } else if (x < 0 && y > 0) {
    y -= pmtY;
    x += pmtX;
    z -= pmtZ;
    //xDet = x*cosPhi - z*sinPhi;// + detZOrig*sinPhi;
    //yDet = x*sinTheta*sinPhi + y*cosTheta + z*sinTheta*cosPhi;
    //zDet = x*cosTheta*sinPhi - y*sinTheta + z*cosTheta*cosPhi;

    xDet = x * cosPhi + y * sinPhi * sinTheta - z * cosTheta * sinPhi;
    yDet = y * cosTheta + z * sinTheta;
    zDet = x * sinPhi - y * sinTheta * cosPhi + z * cosTheta * cosPhi;


    yDet += pmtY;
    xDet -= pmtX;
    zDet += pmtZ;
  } else {
    outPos->SetXYZ(x, y, z);
  }
  outPos->SetXYZ(xDet, yDet, zDet);
}

Bool_t CbmRichGeoManager::IsPointInsidePmt(const TVector3* rotatedPoint) {
  if (fGP->fGeometryType == CbmRichGeometryTypeTwoWings) {
    CbmRichRecGeoPar* gp = CbmRichGeoManager::GetInstance().fGP;
    Double_t pmtPlaneX   = gp->fPmt.fPlaneX;
    Double_t pmtPlaneY   = gp->fPmt.fPlaneY;
    Double_t pmtWidth    = gp->fPmt.fWidth;   // half width
    Double_t pmtHeight   = gp->fPmt.fHeight;  // half height

    Double_t marginX = 2.;  // [cm]
    Double_t marginY = 2.;  // [cm]
    // upper pmt planes
    Double_t pmtYTop    = TMath::Abs(pmtPlaneY) + pmtHeight + marginY;
    Double_t pmtYBottom = TMath::Abs(pmtPlaneY) - pmtHeight - marginY;
    Double_t absYDet    = TMath::Abs(rotatedPoint->y());
    Bool_t isYOk        = (absYDet <= pmtYTop && absYDet >= pmtYBottom);

    Double_t pmtXMin = -TMath::Abs(pmtPlaneX) - pmtWidth - marginX;
    Double_t pmtXMax = TMath::Abs(pmtPlaneX) + pmtWidth + marginX;
    Bool_t isXOk =
      (rotatedPoint->x() >= pmtXMin && rotatedPoint->x() <= pmtXMax);

    return (isXOk && isYOk);
  } else if (fGP->fGeometryType == CbmRichGeometryTypeCylindrical) {
    CbmRichRecGeoPar* gp = CbmRichGeoManager::GetInstance().fGP;
    map<string, CbmRichRecGeoParPmt> pmtMap = gp->fPmtMap;

    int maxIndex  = -1;
    string maxKey = "";
    for (auto const& entPmt : pmtMap) {
      if (maxIndex < entPmt.second.fPmtPositionIndexX && entPmt.second.fX > 0
          && entPmt.second.fY > 0) {
        maxKey   = entPmt.first;
        maxIndex = entPmt.second.fPmtPositionIndexX;
      }
    }
    CbmRichRecGeoParPmt pmtPar = pmtMap[maxKey];
    Double_t pmtPlaneX         = pmtPar.fPlaneX;
    Double_t pmtPlaneY         = pmtPar.fPlaneY;
    Double_t pmtPlaneZ         = pmtPar.fPlaneZ;
    Double_t pmtWidth          = pmtPar.fWidth;   // full width of the strip
    Double_t pmtHeight         = pmtPar.fHeight;  // full height

    //cout << "IsPointInsidePmt" << endl;
    //cout << "maxIndex:" << maxIndex << " maxKey:" << maxKey << endl;
    //cout << "pmtPlaneX:"<< pmtPlaneX << " pmtPlaneY:" << pmtPlaneY << " pmtPlaneZ:" << pmtPlaneZ<< " pmtWidth:" << pmtWidth << " pmtHeight:" <<pmtHeight << endl;
    TVector3 inPmt(pmtPlaneX, pmtPlaneY, pmtPlaneZ), outPmt(0., 0., 0.);
    CbmRichGeoManager::GetInstance().RotatePoint(&inPmt, &outPmt);
    //cout << "Rotated pmtPlaneX:"<< outPmt.X() << " pmtPlaneY:" << outPmt.Y() << " pmtPlaneZ:" << outPmt.Z()<< endl;

    Double_t marginX = 2.;  // [cm]
    Double_t marginY = 2.;  // [cm]
    // upper pmt planes
    Double_t pmtYTop    = TMath::Abs(outPmt.Y()) + 0.5 * pmtHeight + marginY;
    Double_t pmtYBottom = TMath::Abs(outPmt.Y()) - 0.5 * pmtHeight - marginY;
    //cout << "pmtYTop:"<< pmtYTop << " pmtYBottom:" << pmtYBottom << endl;
    Double_t absYDet = TMath::Abs(rotatedPoint->y());
    Bool_t isYOk     = (absYDet <= pmtYTop && absYDet >= pmtYBottom);

    Double_t pmtXMin = -TMath::Abs(outPmt.X()) - 0.5 * pmtWidth - marginX;
    Double_t pmtXMax = TMath::Abs(outPmt.X()) + 0.5 * pmtWidth + marginX;
    //cout << "pmtXMin:"<< pmtXMin << " pmtXMax:" << pmtXMax << endl;
    Bool_t isXOk =
      (rotatedPoint->x() >= pmtXMin && rotatedPoint->x() <= pmtXMax);

    return (isXOk && isYOk);
    // return true;
  } else {
    return false;
  }
}
