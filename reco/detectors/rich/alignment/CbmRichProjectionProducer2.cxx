/* Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

// ---------- Original Headers ---------- //
#include "CbmRichProjectionProducer2.h"

#include "CbmRichHit.h"

#include "FairRootManager.h"
#include <Logger.h>

#include "TClonesArray.h"

// ---------- Included Headers ---------- //
#include "CbmMCTrack.h"
#include "CbmRichGeoManager.h"
#include "CbmRichHitProducer.h"
#include "CbmRichNavigationUtil2.h"
#include "CbmRichPoint.h"
#include "CbmTrackMatchNew.h"
#include "CbmUtils.h"

#include "FairTrackParam.h"

#include "TGeoManager.h"
#include "TMatrixFSym.h"

class TGeoNode;
class TGeoVolume;

CbmRichProjectionProducer2::CbmRichProjectionProducer2()
  : fTrackParams(NULL)
  , fMCTracks(NULL)
  , fRichPoints(NULL)
  , fOutputDir("")
  , fNumbAxis("")
  , fTile("")
  , fEventNum(0)
{
}

CbmRichProjectionProducer2::~CbmRichProjectionProducer2()
{
  FairRootManager* fmanager = FairRootManager::Instance();
  fmanager->Write();
}

void CbmRichProjectionProducer2::Init()
{
  LOG(info) << "CbmRichProjectionProducerAnalytical::Init()";
  FairRootManager* manager = FairRootManager::Instance();

  fTrackParams = (TClonesArray*) manager->GetObject("RichTrackParamZ");
  if (NULL == fTrackParams) { Fatal("CbmRichProjectionProducerAnalytical::Init", "No RichTrackParamZ array !"); }

  fMCTracks = (TClonesArray*) manager->GetObject("MCTrack");
  if (NULL == fMCTracks) { Fatal("CbmRichCorrectionVector::Init", "No MCTracks array !"); }

  fRichPoints = (TClonesArray*) manager->GetObject("RichPoint");
  if (NULL == fRichPoints) { Fatal("CbmRichCorrectionVector::Init", "No RichPoint array !"); }
}

void CbmRichProjectionProducer2::DoProjection(TClonesArray* projectedPoint)
{
  cout << endl
       << "//"
          "--------------------------------------------------------------------"
          "------------------------------------------//"
       << endl;
  cout << "//-------------------------------- CbmRichProjectionProducer2: Do "
          "Projection ---------------------------------//"
       << endl
       << endl;
  cout << "//"
          "--------------------------------------------------------------------"
          "--------------------------------------------------//"
       << endl;

  fEventNum++;
  //LOG(debug2) << "CbmRichProjectionProducer2 : Event #" << fEventNum;
  cout << "CbmRichProjectionProducer2 : Event #" << fEventNum << endl;

  CbmRichRecGeoPar* gp = CbmRichGeoManager::GetInstance().fGP;
  Double_t pmtPlaneX   = gp->fPmt.fPlaneX;
  Double_t pmtPlaneY   = gp->fPmt.fPlaneY;
  Double_t pmtWidth    = gp->fPmt.fWidth;
  Double_t pmtHeight   = gp->fPmt.fHeight;

  projectedPoint->Delete();
  TMatrixFSym covMat(5);
  for (Int_t iMatrix = 0; iMatrix < 5; iMatrix++) {
    for (Int_t jMatrix = 0; jMatrix <= iMatrix; jMatrix++) {
      covMat(iMatrix, jMatrix) = 0;
    }
  }
  covMat(0, 0) = covMat(1, 1) = covMat(2, 2) = covMat(3, 3) = covMat(4, 4) = 1.e-4;

  for (Int_t j = 0; j < fTrackParams->GetEntriesFast(); j++) {
    FairTrackParam* point = (FairTrackParam*) fTrackParams->At(j);
    new ((*projectedPoint)[j]) FairTrackParam(0., 0., 0., 0., 0., 0., covMat);

    // check if Array was filled
    if (point->GetX() == 0 && point->GetY() == 0 && point->GetZ() == 0 && point->GetTx() == 0 && point->GetTy() == 0)
      continue;
    if (point->GetQp() == 0) continue;

    Double_t xDet = 0., yDet = 0., zDet = 0.;
    Double_t* pmtPt;
    pmtPt = ProjectionProducer(point);
    xDet  = pmtPt[0];
    yDet  = pmtPt[1];
    zDet  = pmtPt[2];

    //check that crosspoint inside the plane
    Double_t marginX = 2.;  // [cm]
    Double_t marginY = 2.;  // [cm]
    // upper pmt planes
    Double_t pmtYTop    = TMath::Abs(pmtPlaneY) + pmtHeight + marginY;
    Double_t pmtYBottom = TMath::Abs(pmtPlaneY) - pmtHeight - marginY;
    Double_t absYDet    = TMath::Abs(yDet);
    Bool_t isYOk        = (absYDet <= pmtYTop && absYDet >= pmtYBottom);

    Double_t pmtXMin = -TMath::Abs(pmtPlaneX) - pmtWidth - marginX;
    Double_t pmtXMax = TMath::Abs(pmtPlaneX) + pmtWidth + marginX;
    ///cout << pmtXMin << " " << pmtXMax << " " <<  pmtYBottom << "  " << pmtYTop <<  endl;
    Bool_t isXOk = (xDet >= pmtXMin && xDet <= pmtXMax);

    if (isYOk && isXOk) {
      FairTrackParam richtrack(xDet, yDet, zDet, 0., 0., 0., covMat);
      *(FairTrackParam*) (projectedPoint->At(j)) = richtrack;
    }
  }
}

Double_t* CbmRichProjectionProducer2::ProjectionProducer(FairTrackParam* trackParam)
{
  cout << "//------------------------------ CbmRichProjectionProducer2: "
          "Projection Producer ------------------------------//"
       << endl
       << endl;

  static Double_t pmtPt[3];

  Int_t NofPMTPoints = fRichPoints->GetEntriesFast();

  // Declaration of points coordinates.
  Double_t sphereRadius = 300., constantePMT = 0.;
  vector<Double_t> ptM(3), ptC(3), ptCNew(3), momR1(3), ptR1(3), normalPMT(3), ptR2Mirr(3), ptR2Center(3), ptPMirr(3),
    ptPR2(3), ptTileCenter(3);
  vector<Double_t> ptCIdeal(3), ptR2CenterUnCorr(3), ptR2CenterIdeal(3), ptR2MirrUnCorr(3), ptR2MirrIdeal(3),
    ptPMirrUnCorr(3), ptPMirrIdeal(3), ptPR2UnCorr(3), ptPR2Ideal(3);
  TVector3 outPos, outPosUnCorr, outPosIdeal;
  // Declaration of ring parameters.
  Double_t ringCenter[] = {0., 0., 0.}, distToExtrapTrackHit = 0., distToExtrapTrackHitInPlane = 0.;
  //Declarations related to geometry.
  Int_t pmtTrackID = -1, pmtMotherID = -100;
  TGeoNavigator* navi;

  ptR1.at(0)  = trackParam->GetX();
  ptR1.at(1)  = trackParam->GetY();
  ptR1.at(2)  = trackParam->GetZ();
  Double_t nx = 0., ny = 0., nz = 0.;
  CbmRichNavigationUtil2::GetDirCos(trackParam, nx, ny, nz);
  TVector3 dirCos;
  dirCos.SetXYZ(nx, ny, nz);
  GetPmtNormal(NofPMTPoints, normalPMT, constantePMT);
  cout << "Calculated normal vector to PMT plane = {" << normalPMT.at(0) << ", " << normalPMT.at(1) << ", "
       << normalPMT.at(2) << "} and constante d = " << constantePMT << endl
       << endl;

  TVector3 mirrorPoint;
  TString mirrorIntersection1 =
    CbmRichNavigationUtil2::FindIntersection(trackParam, mirrorPoint, "mirror_tile_type", navi);
  ptM.at(0) = mirrorPoint.x();
  ptM.at(1) = mirrorPoint.y();
  ptM.at(2) = mirrorPoint.z();
  cout << "mirrorIntersection1: " << mirrorIntersection1 << endl;
  cout << "Mirror point coordinates = {" << mirrorPoint.x() << ", " << mirrorPoint.y() << ", " << mirrorPoint.z() << "}"
       << endl;
  TString mirrorIntersection = "/cave_1/rich1_0/richContainer_333/rich_gas_329/mirror_full_half_321/"
                               "mirror_tile_2_0_148/mirror_tile_type2"
                               "_inter_108/mirror_tile_type2_94/"
                               + mirrorIntersection1;
  cout << "mirrorIntersection: " << mirrorIntersection << endl;

  if (mirrorIntersection1) {
    //navi->cd(mirrorIntersection1);
    //navi = gGeoManager->GetCurrentNavigator();
    navi->Dump();
    cout << "Navigator path: " << navi->GetPath() << endl;
    cout << "Coordinates of sphere center: " << endl;
    navi->GetCurrentMatrix()->Print();
    ptCIdeal.at(0) = navi->GetCurrentMatrix()->GetTranslation()[0];
    ptCIdeal.at(1) = navi->GetCurrentMatrix()->GetTranslation()[1];
    ptCIdeal.at(2) = navi->GetCurrentMatrix()->GetTranslation()[2];
    cout << "Coordinates of tile center: " << endl;
    navi->GetMotherMatrix()->Print();
    ptC.at(0) = 0., ptC.at(1) = 132.594000,
    ptC.at(2) = 54.267226;  // Theoretical coordinates of point C.
    cout << "Sphere center coordinates of the aligned mirror tile, ideal = {" << ptCIdeal.at(0) << ", "
         << ptCIdeal.at(1) << ", " << ptCIdeal.at(2) << "}" << endl;
    cout << "Sphere center coordinates of the rotated mirror tile, w/ "
            "GeoManager, = {"
         << ptC.at(0) << ", " << ptC.at(1) << ", " << ptC.at(2) << "} and sphere inner radius = " << sphereRadius
         << endl
         << endl;
    //ptCNew = RotateSphereCenter(ptTileCenter, ptC, navi);

    cout << "FairTrackParam Point coordinates = {" << ptR1.at(0) << ", " << ptR1.at(1) << ", " << ptR1.at(2) << "}"
         << endl;
    cout << "And FairTrackParam Point direction cosines = {" << ptR1.at(3) << ", " << ptR1.at(4) << ", " << ptR1.at(5)
         << "}" << endl;
    cout << "Mirror Point coordinates = {" << ptM.at(0) << ", " << ptM.at(1) << ", " << ptM.at(2) << "}" << endl;

    ComputeR2(ptR2CenterUnCorr, ptR2MirrUnCorr, ptM, ptC, ptR1, navi, "Uncorrected");
    ComputeR2(ptR2Center, ptR2Mirr, ptM, ptC, ptR1, navi, "Corrected");
    ComputeR2(ptR2CenterIdeal, ptR2MirrIdeal, ptM, ptCIdeal, ptR1, navi, "Uncorrected");

    ComputeP(ptPMirrUnCorr, ptPR2UnCorr, normalPMT, ptM, ptR2MirrUnCorr, constantePMT);
    ComputeP(ptPMirr, ptPR2, normalPMT, ptM, ptR2Mirr, constantePMT);
    ComputeP(ptPMirrIdeal, ptPR2Ideal, normalPMT, ptM, ptR2MirrIdeal, constantePMT);

    TVector3 inPosUnCorr(ptPMirrUnCorr.at(0), ptPMirrUnCorr.at(1), ptPMirrUnCorr.at(2));
    CbmRichGeoManager::GetInstance().RotatePoint(&inPosUnCorr, &outPosUnCorr);
    cout << "New mirror points coordinates = {" << outPosUnCorr.x() << ", " << outPosUnCorr.y() << ", "
         << outPosUnCorr.z() << "}" << endl;
    TVector3 inPos(ptPMirr.at(0), ptPMirr.at(1), ptPMirr.at(2));
    CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);
    cout << "New mirror points coordinates = {" << outPos.x() << ", " << outPos.y() << ", " << outPos.z() << "}"
         << endl;
    TVector3 inPosIdeal(ptPMirrIdeal.at(0), ptPMirrIdeal.at(1), ptPMirrIdeal.at(2));
    CbmRichGeoManager::GetInstance().RotatePoint(&inPosIdeal, &outPosIdeal);
    cout << endl
         << "New mirror points coordinates = {" << outPosIdeal.x() << ", " << outPosIdeal.y() << ", " << outPosIdeal.z()
         << "}" << endl
         << endl;
  }
  else {
    cout << "No mirror intersection found ..." << endl;
    outPos.SetXYZ(0, 0, 0);
  }

  pmtPt[0] = outPos.x();
  pmtPt[1] = outPos.y();
  pmtPt[2] = outPos.z();
  return pmtPt;
}

void CbmRichProjectionProducer2::GetPmtNormal(Int_t NofPMTPoints, vector<Double_t>& normalPMT, Double_t& normalCste)
{
  //cout << endl << "//------------------------------ CbmRichProjectionProducer2: Calculate PMT Normal ------------------------------//" << endl << endl;

  Int_t pmtTrackID, pmtMotherID;
  Double_t buffNormX = 0., buffNormY = 0., buffNormZ = 0., k = 0., scalarProd = 0.;
  Double_t pmtPt[] = {0., 0., 0.};
  Double_t a[] = {0., 0., 0.}, b[] = {0., 0., 0.}, c[] = {0., 0., 0.};
  CbmMCTrack* track;

  /*
 * Selection of three points (A, B, C), which form a plan and from which the calculation of the normal of the plan can be computed.
 * Formula used is: vect(AB) x vect(AC) = normal.
 * Normalize the normal vector obtained and check with three random points from the PMT plane, whether the scalar product is equal to zero.
 */
  for (Int_t iPmt = 0; iPmt < NofPMTPoints; iPmt++) {
    CbmRichPoint* pmtPoint = (CbmRichPoint*) fRichPoints->At(iPmt);
    pmtTrackID             = pmtPoint->GetTrackID();
    track                  = (CbmMCTrack*) fMCTracks->At(pmtTrackID);
    pmtMotherID            = track->GetMotherId();
    //a[0] = pmtPoint->GetX(), a[1] = pmtPoint->GetY(), a[2] = pmtPoint->GetZ();
    cout << "a[0] = " << a[0] << ", a[1] = " << a[1] << " et a[2] = " << a[2] << endl;
    break;
  }
  for (Int_t iPmt = 0; iPmt < NofPMTPoints; iPmt++) {
    CbmRichPoint* pmtPoint = (CbmRichPoint*) fRichPoints->At(iPmt);
    pmtTrackID             = pmtPoint->GetTrackID();
    track                  = (CbmMCTrack*) fMCTracks->At(pmtTrackID);
    pmtMotherID            = track->GetMotherId();
    //cout << "PMT Point coordinates; x = " << pmtPoint->GetX() << ", y = " << pmtPoint->GetY() << " and z = " << pmtPoint->GetZ() << endl;
    if (TMath::Sqrt(TMath::Power(a[0] - pmtPoint->GetX(), 2) + TMath::Power(a[1] - pmtPoint->GetY(), 2)
                    + TMath::Power(a[2] - pmtPoint->GetZ(), 2))
        > 7) {
      //b[0] = pmtPoint->GetX(), b[1] = pmtPoint->GetY(), b[2] = pmtPoint->GetZ();
      cout << "b[0] = " << b[0] << ", b[1] = " << b[1] << " et b[2] = " << b[2] << endl;
      break;
    }
  }
  for (Int_t iPmt = 0; iPmt < NofPMTPoints; iPmt++) {
    CbmRichPoint* pmtPoint = (CbmRichPoint*) fRichPoints->At(iPmt);
    pmtTrackID             = pmtPoint->GetTrackID();
    track                  = (CbmMCTrack*) fMCTracks->At(pmtTrackID);
    pmtMotherID            = track->GetMotherId();
    //cout << "PMT Point coordinates; x = " << pmtPoint->GetX() << ", y = " << pmtPoint->GetY() << " and z = " << pmtPoint->GetZ() << endl;
    if (TMath::Sqrt(TMath::Power(a[0] - pmtPoint->GetX(), 2) + TMath::Power(a[1] - pmtPoint->GetY(), 2)
                    + TMath::Power(a[2] - pmtPoint->GetZ(), 2))
          > 7
        && TMath::Sqrt(TMath::Power(b[0] - pmtPoint->GetX(), 2) + TMath::Power(b[1] - pmtPoint->GetY(), 2)
                       + TMath::Power(b[2] - pmtPoint->GetZ(), 2))
             > 7) {
      c[0] = pmtPoint->GetX(), c[1] = pmtPoint->GetY(), c[2] = pmtPoint->GetZ();
      //cout << "c[0] = " << c[0] << ", c[1] = " << c[1] << " et c[2] = " << c[2] << endl;
      break;
    }
  }

  k = (b[0] - a[0]) / (c[0] - a[0]);
  if ((b[1] - a[1]) - (k * (c[1] - a[1])) == 0 || (b[2] - a[2]) - (k * (c[2] - a[2])) == 0) {
    cout << "Error in normal calculation, vect_AB and vect_AC are collinear." << endl;
  }
  else {
    buffNormX = (b[1] - a[1]) * (c[2] - a[2]) - (b[2] - a[2]) * (c[1] - a[1]);
    buffNormY = (b[2] - a[2]) * (c[0] - a[0]) - (b[0] - a[0]) * (c[2] - a[2]);
    buffNormZ = (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);
    normalPMT.at(0) =
      buffNormX / TMath::Sqrt(TMath::Power(buffNormX, 2) + TMath::Power(buffNormY, 2) + TMath::Power(buffNormZ, 2));
    normalPMT.at(1) =
      buffNormY / TMath::Sqrt(TMath::Power(buffNormX, 2) + TMath::Power(buffNormY, 2) + TMath::Power(buffNormZ, 2));
    normalPMT.at(2) =
      buffNormZ / TMath::Sqrt(TMath::Power(buffNormX, 2) + TMath::Power(buffNormY, 2) + TMath::Power(buffNormZ, 2));
  }

  CbmRichPoint* pmtPoint1 = (CbmRichPoint*) fRichPoints->At(20);
  scalarProd              = normalPMT.at(0) * (pmtPoint1->GetX() - a[0]) + normalPMT.at(1) * (pmtPoint1->GetY() - a[1])
               + normalPMT.at(2) * (pmtPoint1->GetZ() - a[2]);
  //cout << "1st scalar product between vectAM and normale = " << scalarProd << endl;
  // To determine the constant term of the plane equation, inject the coordinates of a pmt point, which should solve it: a*x+b*y+c*z+d=0.
  normalCste =
    -1
    * (normalPMT.at(0) * pmtPoint1->GetX() + normalPMT.at(1) * pmtPoint1->GetY() + normalPMT.at(2) * pmtPoint1->GetZ());
  CbmRichPoint* pmtPoint2 = (CbmRichPoint*) fRichPoints->At(15);
  scalarProd              = normalPMT.at(0) * (pmtPoint2->GetX() - a[0]) + normalPMT.at(1) * (pmtPoint2->GetY() - a[1])
               + normalPMT.at(2) * (pmtPoint2->GetZ() - a[2]);
  //cout << "2nd scalar product between vectAM and normale = " << scalarProd << endl;
  CbmRichPoint* pmtPoint3 = (CbmRichPoint*) fRichPoints->At(25);
  scalarProd              = normalPMT.at(0) * (pmtPoint3->GetX() - a[0]) + normalPMT.at(1) * (pmtPoint3->GetY() - a[1])
               + normalPMT.at(2) * (pmtPoint3->GetZ() - a[2]);
  //cout << "3nd scalar product between vectAM and normale = " << scalarProd << endl;
}

void CbmRichProjectionProducer2::ComputeR2(vector<Double_t>& ptR2Center, vector<Double_t>& ptR2Mirr,
                                           vector<Double_t> ptM, vector<Double_t> ptC, vector<Double_t> ptR1,
                                           TGeoNavigator* navi, TString s)
{
  cout << endl
       << "//------------------------------ CbmRichCorrection: ComputeR2 "
          "------------------------------//"
       << endl
       << endl;

  vector<Double_t> normalMirr(3), ptCNew(3), ptTileCenter(3);
  Double_t t1 = 0., t2 = 0., t3 = 0.;

  if (s == "Corrected") {
    // Use the correction information from text file, to the tile sphere center:
    // Reading misalignment information from correction_param.txt text file.
    vector<Double_t> outputFit(4);
    ifstream corr_file;
    TString str = fOutputDir + "correction_param_" + fNumbAxis + fTile + ".txt";
    corr_file.open(str);
    if (corr_file.is_open()) {
      for (Int_t i = 0; i < 4; i++) {
        corr_file >> outputFit.at(i);
      }
      corr_file.close();
    }
    else {
      cout << "Error in CbmRichCorrection: unable to open parameter file!" << endl << endl;
      sleep(5);
    }
    cout << "Misalignment parameters read from file = [" << outputFit.at(0) << " ; " << outputFit.at(1) << " ; "
         << outputFit.at(2) << " ; " << outputFit.at(3) << "]" << endl;

    ptCNew.at(0) = TMath::Abs(ptC.at(0) - TMath::Abs(outputFit.at(3)));
    ptCNew.at(1) = TMath::Abs(ptC.at(1) - TMath::Abs(outputFit.at(2)));
    //ptCNew.at(0) = ptC.at(0) - outputFit.at(3);
    //ptCNew.at(1) = ptC.at(1) - outputFit.at(2);
    ptCNew.at(2)       = ptC.at(2);
    ptTileCenter.at(0) = navi->GetMotherMatrix()->GetTranslation()[0];
    ptTileCenter.at(1) = navi->GetMotherMatrix()->GetTranslation()[1];
    ptTileCenter.at(2) = navi->GetMotherMatrix()->GetTranslation()[2];
    cout << "Mirror tile center coordinates = {" << ptTileCenter.at(0) << ", " << ptTileCenter.at(1) << ", "
         << ptTileCenter.at(2) << "}" << endl;
    Double_t x = 0., y = 0., z = 0., dist = 0., dist2 = 0., z2 = 0.;
    x    = TMath::Power(ptCNew.at(0) - ptTileCenter.at(0), 2);
    y    = TMath::Power(ptCNew.at(1) - ptTileCenter.at(1), 2);
    z    = TMath::Power(ptCNew.at(2) - ptTileCenter.at(2), 2);
    dist = TMath::Sqrt(x + y + z);
    z2   = ptTileCenter.at(2) - TMath::Sqrt(TMath::Power(300, 2) - x - y) - ptCNew.at(2);
    cout << "{x, y, z} = {" << x << ", " << y << ", " << z << "}, dist = " << dist << " and z2 = " << z2 << endl;
    dist2 = TMath::Sqrt(x + y + TMath::Power(z2 - ptTileCenter.at(2), 2));
    cout << "dist2 = " << dist2 << endl;
    ptCNew.at(2) += z2;
    cout << "Sphere center coordinates of the rotated mirror tile, after "
            "correction, = {"
         << ptCNew.at(0) << ", " << ptCNew.at(1) << ", " << ptCNew.at(2) << "}" << endl;
  }
  else if (s == "Uncorrected") {
    // Keep the same tile sphere center, with no correction information.
    ptCNew = ptC;
    cout << "Sphere center coordinates of the rotated mirror tile, without "
            "correction = {"
         << ptCNew.at(0) << ", " << ptCNew.at(1) << ", " << ptCNew.at(2) << "}" << endl;
  }
  else {
    cout << "No input given in function ComputeR2! Uncorrected parameters for "
            "the sphere center of the tile will be used!"
         << endl;
    ptCNew = ptC;
    cout << "Sphere center coordinates of the rotated mirror tile, without "
            "correction = {"
         << ptCNew.at(0) << ", " << ptCNew.at(1) << ", " << ptCNew.at(2) << "}" << endl;
  }

  normalMirr.at(0) = (ptCNew.at(0) - ptM.at(0))
                     / TMath::Sqrt(TMath::Power(ptCNew.at(0) - ptM.at(0), 2) + TMath::Power(ptCNew.at(1) - ptM.at(1), 2)
                                   + TMath::Power(ptCNew.at(2) - ptM.at(2), 2));
  normalMirr.at(1) = (ptCNew.at(1) - ptM.at(1))
                     / TMath::Sqrt(TMath::Power(ptCNew.at(0) - ptM.at(0), 2) + TMath::Power(ptCNew.at(1) - ptM.at(1), 2)
                                   + TMath::Power(ptCNew.at(2) - ptM.at(2), 2));
  normalMirr.at(2) = (ptCNew.at(2) - ptM.at(2))
                     / TMath::Sqrt(TMath::Power(ptCNew.at(0) - ptM.at(0), 2) + TMath::Power(ptCNew.at(1) - ptM.at(1), 2)
                                   + TMath::Power(ptCNew.at(2) - ptM.at(2), 2));
  //cout << "Calculated and normalized normal of mirror tile = {" << normalMirr.at(0) << ", " << normalMirr.at(1) << ", " << normalMirr.at(2) << "}" << endl;

  t1 = ((ptR1.at(0) - ptM.at(0)) * (ptCNew.at(0) - ptM.at(0)) + (ptR1.at(1) - ptM.at(1)) * (ptCNew.at(1) - ptM.at(1))
        + (ptR1.at(2) - ptM.at(2)) * (ptCNew.at(2) - ptM.at(2)))
       / (TMath::Power(ptCNew.at(0) - ptM.at(0), 2) + TMath::Power(ptCNew.at(1) - ptM.at(1), 2)
          + TMath::Power(ptCNew.at(2) - ptM.at(2), 2));
  ptR2Center.at(0) = 2 * (ptM.at(0) + t1 * (ptCNew.at(0) - ptM.at(0))) - ptR1.at(0);
  ptR2Center.at(1) = 2 * (ptM.at(1) + t1 * (ptCNew.at(1) - ptM.at(1))) - ptR1.at(1);
  ptR2Center.at(2) = 2 * (ptM.at(2) + t1 * (ptCNew.at(2) - ptM.at(2))) - ptR1.at(2);
  t2 =
    ((ptR1.at(0) - ptCNew.at(0)) * (ptCNew.at(0) - ptM.at(0)) + (ptR1.at(1) - ptCNew.at(1)) * (ptCNew.at(1) - ptM.at(1))
     + (ptR1.at(2) - ptCNew.at(2)) * (ptCNew.at(2) - ptM.at(2)))
    / (TMath::Power(ptCNew.at(0) - ptM.at(0), 2) + TMath::Power(ptCNew.at(1) - ptM.at(1), 2)
       + TMath::Power(ptCNew.at(2) - ptM.at(2), 2));
  ptR2Mirr.at(0) = 2 * (ptCNew.at(0) + t2 * (ptCNew.at(0) - ptM.at(0))) - ptR1.at(0);
  ptR2Mirr.at(1) = 2 * (ptCNew.at(1) + t2 * (ptCNew.at(1) - ptM.at(1))) - ptR1.at(1);
  ptR2Mirr.at(2) = 2 * (ptCNew.at(2) + t2 * (ptCNew.at(2) - ptM.at(2))) - ptR1.at(2);

  cout << "Coordinates of point R2 on reflective plane after reflection on the "
          "mirror tile:"
       << endl;
  //cout << "* using mirror point M to define \U00000394: {" << ptR2Center.at(0) << ", " << ptR2Center.at(1) << ", " << ptR2Center.at(2) << "}" << endl;
  cout << "* using sphere center C to define \U00000394: {" << ptR2Mirr.at(0) << ", " << ptR2Mirr.at(1) << ", "
       << ptR2Mirr.at(2) << "}" << endl;
}

void CbmRichProjectionProducer2::ComputeP(vector<Double_t>& ptPMirr, vector<Double_t>& ptPR2,
                                          vector<Double_t> normalPMT, vector<Double_t> ptM, vector<Double_t> ptR2Mirr,
                                          Double_t constantePMT)
{
  cout << endl
       << "//------------------------------ CbmRichCorrection: ComputeP "
          "------------------------------//"
       << endl
       << endl;

  Double_t k1 = 0., k2 = 0., checkCalc1 = 0., checkCalc2 = 0.;

  k1 = -1
       * ((normalPMT.at(0) * ptM.at(0) + normalPMT.at(1) * ptM.at(1) + normalPMT.at(2) * ptM.at(2) + constantePMT)
          / (normalPMT.at(0) * (ptR2Mirr.at(0) - ptM.at(0)) + normalPMT.at(1) * (ptR2Mirr.at(1) - ptM.at(1))
             + normalPMT.at(2) * (ptR2Mirr.at(2) - ptM.at(2))));
  ptPMirr.at(0) = ptM.at(0) + k1 * (ptR2Mirr.at(0) - ptM.at(0));
  ptPMirr.at(1) = ptM.at(1) + k1 * (ptR2Mirr.at(1) - ptM.at(1));
  ptPMirr.at(2) = ptM.at(2) + k1 * (ptR2Mirr.at(2) - ptM.at(2));
  k2            = -1
       * ((normalPMT.at(0) * ptR2Mirr.at(0) + normalPMT.at(1) * ptR2Mirr.at(1) + normalPMT.at(2) * ptR2Mirr.at(2)
           + constantePMT)
          / (normalPMT.at(0) * (ptR2Mirr.at(0) - ptM.at(0)) + normalPMT.at(1) * (ptR2Mirr.at(1) - ptM.at(1))
             + normalPMT.at(2) * (ptR2Mirr.at(2) - ptM.at(2))));
  ptPR2.at(0) = ptR2Mirr.at(0) + k2 * (ptR2Mirr.at(0) - ptM.at(0));
  ptPR2.at(1) = ptR2Mirr.at(1) + k2 * (ptR2Mirr.at(1) - ptM.at(1));
  ptPR2.at(2) = ptR2Mirr.at(2) + k2 * (ptR2Mirr.at(2) - ptM.at(2));
  cout << "Coordinates of point P on PMT plane, after reflection on the mirror "
          "tile and extrapolation to the PMT plane:"
       << endl;
  cout << "* using mirror point M to define \U0001D49F ': {" << ptPMirr.at(0) << ", " << ptPMirr.at(1) << ", "
       << ptPMirr.at(2) << "}" << endl;
  //cout << "* using reflected point R2 to define \U0001D49F ': {" << ptPR2.at(0) << ", " << ptPR2.at(1) << ", " << ptPR2.at(2) << "}" << endl;
  checkCalc1 =
    ptPMirr.at(0) * normalPMT.at(0) + ptPMirr.at(1) * normalPMT.at(1) + ptPMirr.at(2) * normalPMT.at(2) + constantePMT;
  cout << "Check whether extrapolated track point on PMT plane verifies its "
          "equation (value should be 0.):"
       << endl;
  cout << "* using mirror point M, checkCalc = " << checkCalc1 << endl;
  checkCalc2 =
    ptPR2.at(0) * normalPMT.at(0) + ptPR2.at(1) * normalPMT.at(1) + ptPR2.at(2) * normalPMT.at(2) + constantePMT;
  //cout << "* using reflected point R2, checkCalc = " << checkCalc2 << endl;
}

ClassImp(CbmRichProjectionProducer2)
