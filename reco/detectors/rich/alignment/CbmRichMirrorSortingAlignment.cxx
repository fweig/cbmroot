/* Copyright (C) 2016-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Jordan Bendarouach [committer] */

#include "CbmRichMirrorSortingAlignment.h"

#include "FairRootManager.h"
#include <Logger.h>

// ----- PART 1 ----- //
#include "CbmGlobalTrack.h"
#include "CbmMCTrack.h"
#include "CbmRichConverter.h"
#include "CbmRichRing.h"
#include "CbmRichRingFitterCOP.h"
#include "CbmRichRingFitterEllipseTau.h"
#include "CbmUtils.h"

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TF1.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TVector3.h"

#include <vector>

// ----- PART 2 ----- //
#include "CbmRichGeoManager.h"
#include "CbmRichPoint.h"

#include "TGeoManager.h"
#include "TGeoNavigator.h"
class TGeoNode;
class TGeoMatrix;
#include "CbmRichNavigationUtil.h"
#include "CbmTrackMatchNew.h"

#include "TStyle.h"
#include "TSystem.h"

#include <iostream>

#include "string.h"

CbmRichMirrorSortingAlignment::CbmRichMirrorSortingAlignment()
  : FairTask("CbmRichMirrorSortingAlignment")
  , fEventNb(0)
  , fGlobalTracks(NULL)
  , fRichRings(NULL)
  , fMCTracks(NULL)
  , fCopFit(NULL)
  , fTauFit(NULL)
  , fOutputDir("")
  , fStudyName("")
  , fMirrorPoints(NULL)
  , fRefPlanePoints(NULL)
  , fPmtPoints(NULL)
  , fRichProjections(NULL)
  , fTrackParams(NULL)
  , fRichRingMatches(NULL)
  , fStsTrackMatches(NULL)
  , fMirrorMap()
  , fThreshold(0)
{
}

CbmRichMirrorSortingAlignment::~CbmRichMirrorSortingAlignment() {}

InitStatus CbmRichMirrorSortingAlignment::Init()
{
  FairRootManager* manager = FairRootManager::Instance();

  fGlobalTracks = (TClonesArray*) manager->GetObject("GlobalTrack");
  if (NULL == fGlobalTracks) { Fatal("CbmRichMirrorSortingAlignment::Init", "No GlobalTrack array!"); }

  fRichRings = (TClonesArray*) manager->GetObject("RichRing");
  if (NULL == fRichRings) { Fatal("CbmRichMirrorSortingAlignment::Init", "No RichRing array !"); }

  fMCTracks = (TClonesArray*) manager->GetObject("MCTrack");
  if (NULL == fMCTracks) { Fatal("CbmRichMirrorSortingAlignment::Init", "No MCTracks array !"); }

  fMirrorPoints = (TClonesArray*) manager->GetObject("RichMirrorPoint");
  if (NULL == fMirrorPoints) { Fatal("CbmRichMirrorSortingAlignment::Init", "No RichMirrorPoints array !"); }

  fRefPlanePoints = (TClonesArray*) manager->GetObject("RefPlanePoint");
  if (NULL == fRefPlanePoints) { Fatal("CbmRichMirrorSortingAlignment::Init", "No RichRefPlanePoint array !"); }

  fPmtPoints = (TClonesArray*) manager->GetObject("RichPoint");
  if (NULL == fPmtPoints) { Fatal("CbmRichMirrorSortingAlignment::Init", "No RichPoint array !"); }

  fRichProjections = (TClonesArray*) manager->GetObject("RichProjection");
  if (NULL == fRichProjections) { Fatal("CbmRichMirrorSortingAlignment::Init", "No RichProjection array !"); }

  fTrackParams = (TClonesArray*) manager->GetObject("RichTrackParamZ");
  if (NULL == fTrackParams) { Fatal("CbmRichMirrorSortingAlignment::Init", "No RichTrackParamZ array!"); }

  fRichRingMatches = (TClonesArray*) manager->GetObject("RichRingMatch");
  if (NULL == fRichRingMatches) { Fatal("CbmRichMirrorSortingAlignment::Init", "No RichRingMatch array!"); }

  fStsTrackMatches = (TClonesArray*) manager->GetObject("StsTrackMatch");
  if (NULL == fStsTrackMatches) { Fatal("CbmRichMirrorSortingAlignment::Init", "No StsTrackMatch array!"); }

  fCopFit = new CbmRichRingFitterCOP();
  fTauFit = new CbmRichRingFitterEllipseTau();
  CbmRichConverter::Init();

  return kSUCCESS;
}

void CbmRichMirrorSortingAlignment::Exec(Option_t* Option)
{
  fEventNb++;
  cout << "CbmRichMirrorSortingAlignment: Event #" << fEventNb << endl;
  TVector3 momentum, outPos;
  Double_t constantePMT = 0., trackX = 0., trackY = 0.;
  vector<Double_t> vect(2, 0), ptM(3, 0), ptC(3, 0), ptCIdeal(3, 0), ptR1(3, 0), ptR2Center(3, 0), ptR2Mirr(3, 0),
    ptPR2(3, 0), ptPMirr(3, 0), normalPMT(3, 0);
  //ptC[0]=0.;
  ptC.at(0) = 0., ptC.at(1) = 132.594000, ptC.at(2) = 54.267226;
  TVector3 mirrorPoint, dirCos, pos;
  Double_t nx = 0., ny = 0., nz = 0.;
  TGeoNode* mirrNode;
  CbmRichPoint *mirrPoint, *refPlanePoint;

  if (fRichRings->GetEntriesFast() != 0) {
    cout << "Nb of rings in evt = " << fRichRings->GetEntriesFast() << endl << endl;
    GetPmtNormal(fPmtPoints->GetEntriesFast(), normalPMT, constantePMT);
    //cout << "Calculated normal vector to PMT plane = {" << normalPMT.at(0) << ", " << normalPMT.at(1) << ", " << normalPMT.at(2) << "} and constante d = " << constantePMT << endl;

    for (Int_t iGlobalTrack = 0; iGlobalTrack < fGlobalTracks->GetEntriesFast(); iGlobalTrack++) {
      CbmRichMirror* mirrorObject = new CbmRichMirror();  // Create CbmRichMirror object, to be later filled.
                                                          // ----- PART 1 ----- //
      // Ring-Track matching + Ring fit + Track momentum:
      CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(iGlobalTrack);
      Int_t richInd          = gTrack->GetRichRingIndex();
      Int_t stsInd           = gTrack->GetStsTrackIndex();
      //cout << "richInd: " << richInd << endl;
      if (richInd < 0) {
        cout << "Error richInd < 0" << endl;
        continue;
      }
      CbmRichRing* ring = (CbmRichRing*) fRichRings->At(richInd);
      if (ring == NULL) {
        cout << "Error ring == NULL!" << endl;
        continue;
      }
      //Int_t ringTrackID = ring->GetTrackID();				//Old code not working with changes for new matching method.
      //cout << "ringTrackID: " << ringTrackID << endl;
      CbmTrackMatchNew* cbmRichTrackMatch = (CbmTrackMatchNew*) fRichRingMatches->At(richInd);
      CbmTrackMatchNew* cbmStsTrackMatch  = (CbmTrackMatchNew*) fStsTrackMatches->At(stsInd);
      if (NULL == cbmRichTrackMatch) { continue; }
      cout << "Nof true hits = " << cbmRichTrackMatch->GetNofTrueHits() << endl;
      cout << "Nof wrong hits = " << cbmRichTrackMatch->GetNofWrongHits() << endl;
      Int_t mcRichTrackId = cbmRichTrackMatch->GetMatchedLink().GetIndex();
      Int_t mcStsTrackId  = cbmStsTrackMatch->GetMatchedLink().GetIndex();
      //cout << "mcTrackId: " << mcRichTrackId << endl;
      if (mcRichTrackId < 0) continue;
      //if (mcStsTrackId != ringTrackID) {					//Old code not working with changes for new matching method.
      if (mcStsTrackId != mcRichTrackId) {
        cout << "Error StsTrackIndex and TrackIndex from Ring do not match!" << endl;
        continue;
      }
      CbmMCTrack* mcTrack = (CbmMCTrack*) fMCTracks->At(mcRichTrackId);
      if (!mcTrack) continue;

      CbmRichRingLight ringL;
      CbmRichConverter::CopyHitsToRingLight(ring, &ringL);
      fCopFit->DoFit(&ringL);
      //fTauFit->DoFit(&ringL);
      mirrorObject->setRingLight(ringL);  // Fill Cbm Rich Ring Light inside mirrorObject.
      cout << "ring Center Coo: " << ringL.GetCenterX() << ", " << ringL.GetCenterY() << endl;
      mcTrack->GetMomentum(momentum);
      mirrorObject->setMomentum(momentum);  // Fill track momentum inside mirrorObject.
      //FairTrackParam* pr = (FairTrackParam*) fRichProjections->At(ringTrackID);	//Old code not working with changes for new matching method.
      FairTrackParam* pr    = (FairTrackParam*) fRichProjections->At(stsInd);
      FairTrackParam* point = (FairTrackParam*) fTrackParams->At(stsInd);
      if (pr == NULL) {
        cout << "CbmRichMirrorSortingAlignment::Exec : pr = NULL." << endl;
        continue;
      }
      trackX = pr->GetX(), trackY = pr->GetY();
      cout << "Track: " << trackX << ", " << trackY << endl;
      mirrorObject->setProjHit(trackX, trackY);

      // ----- PART 2 ----- //
      // Mirror ID via TGeoNavigator + Extrap hit:
      Int_t trackMotherId = mcTrack->GetMotherId();
      Int_t pdg           = TMath::Abs(mcTrack->GetPdgCode());
      if (trackMotherId == -1) {
        if (fMirrorPoints->GetEntriesFast() > 0) {
          //loop on mirrorPoint and compare w/ TrackID->GetTrackId to get correct one
          for (Int_t iMirrPt = 0; iMirrPt < fMirrorPoints->GetEntriesFast(); iMirrPt++) {
            mirrPoint = (CbmRichPoint*) fMirrorPoints->At(iMirrPt);
            if (mirrPoint == 0) { continue; }
            //cout << "Mirror point track ID: " << mirrPoint->GetTrackID() << endl;
            if (mirrPoint->GetTrackID() == mcRichTrackId) { break; }
          }
          ptM.at(0) = mirrPoint->GetX(), ptM.at(1) = mirrPoint->GetY(), ptM.at(2) = mirrPoint->GetZ();
          cout << "mirrPoint: {" << mirrPoint->GetX() << ", " << mirrPoint->GetY() << ", " << mirrPoint->GetZ() << "}"
               << endl;

          mirrNode = gGeoManager->FindNode(ptM.at(0), ptM.at(1), ptM.at(2));
          cout << "Mirror node name: " << mirrNode->GetName() << " and full path " << gGeoManager->GetPath() << endl;
          string str1 = gGeoManager->GetPath(), str2 = "mirror_tile_", str3 = "";
          cout << "str1 before CbmRichNavigationUtil::FindIntersection: " << str1 << endl;
          TVector3 crossP;
          str1 = CbmRichNavigationUtil::FindIntersection(point, crossP, "mirror_tile_");
          cout << "str1 after CbmRichNavigationUtil::FindIntersection: " << str1 << endl;

          std::size_t found = str1.find(str2);
          if (found != std::string::npos) {
            //cout << "first 'mirror_tile_type' found at: " << found << '\n';
            Int_t end = str2.length() + 3;
            str3      = str1.substr(found, end);
          }
          cout << "Mirror ID: " << str3 << endl;
          mirrorObject->setMirrorId(str3);

          if (mirrNode) {
            TGeoNavigator* navi = gGeoManager->GetCurrentNavigator();
            //cout << "Navigator path: " << navi->GetPath() << endl;
            ptCIdeal.at(0) = navi->GetCurrentMatrix()->GetTranslation()[0];
            ptCIdeal.at(1) = navi->GetCurrentMatrix()->GetTranslation()[1];
            ptCIdeal.at(2) = navi->GetCurrentMatrix()->GetTranslation()[2];
            cout << "Sphere center coordinates of the aligned mirror tile, "
                    "ideal = {"
                 << ptCIdeal.at(0) << ", " << ptCIdeal.at(1) << ", " << ptCIdeal.at(2) << "}" << endl;
            for (Int_t iRefPt = 0; iRefPt < fRefPlanePoints->GetEntriesFast(); iRefPt++) {
              refPlanePoint = (CbmRichPoint*) fRefPlanePoints->At(iRefPt);
              //cout << "Refl plane point track ID: " << refPlanePoint->GetTrackID() << endl;
              if (refPlanePoint->GetTrackID() == mcRichTrackId) { break; }
            }
            ptR1.at(0) = refPlanePoint->GetX(), ptR1.at(1) = refPlanePoint->GetY(), ptR1.at(2) = refPlanePoint->GetZ();
            cout << "Refl plane point coo = {" << ptR1[0] << ", " << ptR1[1] << ", " << ptR1[2] << "}" << endl;
            ComputeR2(ptR2Center, ptR2Mirr, ptM, ptC, ptR1, navi, "Uncorrected");
            ComputeP(ptPMirr, ptPR2, normalPMT, ptM, ptR2Mirr, constantePMT);
            TVector3 inPos(ptPMirr.at(0), ptPMirr.at(1), ptPMirr.at(2));
            cout << "inPos vector: " << inPos.x() << ", " << inPos.y() << ", " << inPos.z() << endl;
            CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);
            cout << "New PMT points coordinates = {" << outPos.x() << ", " << outPos.y() << ", " << outPos.z() << "}"
                 << endl;
            mirrorObject->setExtrapHit(outPos.x(), outPos.y());
          }
        }
        else {
          cout << "No mirror points registered." << endl;
        }
      }
      else {
        cout << "Not a mother particle." << endl;
      }
      //ComputeAngles();
      fMirrorMap[mirrorObject->getMirrorId()].push_back(mirrorObject);
      cout << "Key str: " << mirrorObject->getMirrorId() << endl
           << "Mirror map: " << fMirrorMap[mirrorObject->getMirrorId()].size() << endl
           << endl;
      //mirrNode->Clear();
      //gGeoManager->Clear();
    }
  }
  else {
    cout << "CbmRichMirrorSortingAlignment::Exec No rings in event were found." << endl;
  }
}

void CbmRichMirrorSortingAlignment::GetPmtNormal(Int_t NofPMTPoints, vector<Double_t>& normalPMT, Double_t& normalCste)
{
  //cout << endl << "//------------------------------ CbmRichMirrorSortingAlignment: Calculate PMT Normal ------------------------------//" << endl << endl;

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
    CbmRichPoint* pmtPoint = (CbmRichPoint*) fPmtPoints->At(iPmt);
    pmtTrackID             = pmtPoint->GetTrackID();
    track                  = (CbmMCTrack*) fMCTracks->At(pmtTrackID);
    pmtMotherID            = track->GetMotherId();
    a[0] = pmtPoint->GetX(), a[1] = pmtPoint->GetY(), a[2] = pmtPoint->GetZ();
    //cout << "a[0] = " << a[0] << ", a[1] = " << a[1] << " et a[2] = " << a[2] << endl;
    break;
  }
  for (Int_t iPmt = 0; iPmt < NofPMTPoints; iPmt++) {
    CbmRichPoint* pmtPoint = (CbmRichPoint*) fPmtPoints->At(iPmt);
    pmtTrackID             = pmtPoint->GetTrackID();
    track                  = (CbmMCTrack*) fMCTracks->At(pmtTrackID);
    pmtMotherID            = track->GetMotherId();
    //cout << "PMT Point coordinates; x = " << pmtPoint->GetX() << ", y = " << pmtPoint->GetY() << " and z = " << pmtPoint->GetZ() << endl;
    if (TMath::Sqrt(TMath::Power(a[0] - pmtPoint->GetX(), 2) + TMath::Power(a[1] - pmtPoint->GetY(), 2)
                    + TMath::Power(a[2] - pmtPoint->GetZ(), 2))
        > 7) {
      b[0] = pmtPoint->GetX(), b[1] = pmtPoint->GetY(), b[2] = pmtPoint->GetZ();
      //cout << "b[0] = " << b[0] << ", b[1] = " << b[1] << " et b[2] = " << b[2] << endl;
      break;
    }
  }
  for (Int_t iPmt = 0; iPmt < NofPMTPoints; iPmt++) {
    CbmRichPoint* pmtPoint = (CbmRichPoint*) fPmtPoints->At(iPmt);
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

  CbmRichPoint* pmtPoint1 = (CbmRichPoint*) fPmtPoints->At(20);
  scalarProd              = normalPMT.at(0) * (pmtPoint1->GetX() - a[0]) + normalPMT.at(1) * (pmtPoint1->GetY() - a[1])
               + normalPMT.at(2) * (pmtPoint1->GetZ() - a[2]);
  //cout << "1st scalar product between vectAM and normale = " << scalarProd << endl;
  // To determine the constant term of the plane equation, inject the coordinates of a pmt point, which should solve it: a*x+b*y+c*z+d=0.
  normalCste =
    -1
    * (normalPMT.at(0) * pmtPoint1->GetX() + normalPMT.at(1) * pmtPoint1->GetY() + normalPMT.at(2) * pmtPoint1->GetZ());
  CbmRichPoint* pmtPoint2 = (CbmRichPoint*) fPmtPoints->At(15);
  scalarProd              = normalPMT.at(0) * (pmtPoint2->GetX() - a[0]) + normalPMT.at(1) * (pmtPoint2->GetY() - a[1])
               + normalPMT.at(2) * (pmtPoint2->GetZ() - a[2]);
  //cout << "2nd scalar product between vectAM and normale = " << scalarProd << endl;
  CbmRichPoint* pmtPoint3 = (CbmRichPoint*) fPmtPoints->At(25);
  scalarProd              = normalPMT.at(0) * (pmtPoint3->GetX() - a[0]) + normalPMT.at(1) * (pmtPoint3->GetY() - a[1])
               + normalPMT.at(2) * (pmtPoint3->GetZ() - a[2]);
  //cout << "3nd scalar product between vectAM and normale = " << scalarProd << endl;
}

void CbmRichMirrorSortingAlignment::ComputeR2(vector<Double_t>& ptR2Center, vector<Double_t>& ptR2Mirr,
                                              vector<Double_t> ptM, vector<Double_t> ptC, vector<Double_t> ptR1,
                                              TGeoNavigator* navi, TString s)
{
  //cout << endl << "//------------------------------ CbmRichCorrection: ComputeR2 ------------------------------//" << endl << endl;

  vector<Double_t> normalMirr(3), ptCNew(3), ptTileCenter(3);
  Double_t t1 = 0., t2 = 0., t3 = 0.;

  if (s == "Corrected") {
    // Use the correction information from text file, to the tile sphere center:
    // Reading misalignment information from correction_param.txt text file.
    vector<Double_t> outputFit(4);
    ifstream corrFile;
    //TString str = fOutputDir + "correction_param_" + fNumbAxis + fTile + ".txt";
    TString str = fOutputDir + "/correction_table/correction_param.txt";
    corrFile.open(str);
    if (corrFile.is_open()) {
      for (Int_t i = 0; i < 4; i++) {
        corrFile >> outputFit.at(i);
      }
      corrFile.close();
    }
    else {
      cout << "Error in CbmRichCorrection: unable to open parameter file!" << endl;
      cout << "Parameter file path = " << str << endl << endl;
      sleep(5);
    }
    //cout << "Misalignment parameters read from file = [" << outputFit.at(0) << " ; " << outputFit.at(1) << " ; " << outputFit.at(2) << " ; " << outputFit.at(3) << "]" << endl;

    //ptCNew.at(0) = TMath::Abs(ptC.at(0) - TMath::Abs(outputFit.at(3)));
    //ptCNew.at(1) = TMath::Abs(ptC.at(1) - TMath::Abs(outputFit.at(2)));
    ptCNew.at(0)       = ptC.at(0) + outputFit.at(3);
    ptCNew.at(1)       = ptC.at(1) + outputFit.at(2);
    ptCNew.at(2)       = ptC.at(2);
    ptTileCenter.at(0) = navi->GetMotherMatrix()->GetTranslation()[0];
    ptTileCenter.at(1) = navi->GetMotherMatrix()->GetTranslation()[1];
    ptTileCenter.at(2) = navi->GetMotherMatrix()->GetTranslation()[2];
    //cout << "Mirror tile center coordinates = {" << ptTileCenter.at(0) << ", " << ptTileCenter.at(1) << ", " << ptTileCenter.at(2) << "}" << endl;
    Double_t x = 0., y = 0., z = 0., dist = 0., dist2 = 0., z2 = 0.;
    x    = TMath::Power(ptCNew.at(0) - ptTileCenter.at(0), 2);
    y    = TMath::Power(ptCNew.at(1) - ptTileCenter.at(1), 2);
    z    = TMath::Power(ptCNew.at(2) - ptTileCenter.at(2), 2);
    dist = TMath::Sqrt(x + y + z);
    z2   = ptTileCenter.at(2) - TMath::Sqrt(TMath::Power(300, 2) - x - y) - ptCNew.at(2);
    //cout << "{x, y, z} = {" << x << ", " << y << ", " << z << "}, dist = " << dist << " and z2 = " << z2 << endl;
    dist2 = TMath::Sqrt(x + y + TMath::Power(z2 - ptTileCenter.at(2), 2));
    //cout << "dist2 = " << dist2 << endl;
    ptCNew.at(2) += z2;
    //cout << "Sphere center coordinates of the rotated mirror tile, after correction, = {" << ptCNew.at(0) << ", " << ptCNew.at(1) << ", " << ptCNew.at(2) << "}" << endl;
  }
  else if (s == "Uncorrected") {
    // Keep the same tile sphere center, with no correction information.
    ptCNew = ptC;
    //cout << "Sphere center coordinates of the rotated mirror tile, without correction = {" << ptCNew.at(0) << ", " << ptCNew.at(1) << ", " << ptCNew.at(2) << "}" << endl;
  }
  else {
    //cout << "No input given in function ComputeR2! Uncorrected parameters for the sphere center of the tile will be used!" << endl;
    ptCNew = ptC;
    //cout << "Sphere center coordinates of the rotated mirror tile, without correction = {" << ptCNew.at(0) << ", " << ptCNew.at(1) << ", " << ptCNew.at(2) << "}" << endl;
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
  /*//SAME AS calculation of t2 above
	t3 = ((ptR1.at(0)-ptCNew.at(0))*(ptCNew.at(0)-ptM.at(0)) + (ptR1.at(1)-ptCNew.at(1))*(ptCNew.at(1)-ptM.at(1)) + (ptR1.at(2)-ptCNew.at(2))*(ptCNew.at(2)-ptM.at(2)))/TMath::Sqrt(TMath::Power(ptCNew.at(0) - ptM.at(0),2)+TMath::Power(ptCNew.at(1) - ptM.at(1),2)+TMath::Power(ptCNew.at(2) - ptM.at(2),2));
	reflectedPtCooVectSphereUnity[0] = 2*(ptCNew.at(0)+t3*(normalMirr.at(0)))-ptR1.at(0);
	reflectedPtCooVectSphereUnity[1] = 2*(ptCNew.at(1)+t3*(normalMirr.at(1)))-ptR1.at(1);
	reflectedPtCooVectSphereUnity[2] = 2*(ptCNew.at(2)+t3*(normalMirr.at(2)))-ptR1.at(2);*/
  //cout << "* using mirror point M to define \U00000394: {" << ptR2Center.at(0) << ", " << ptR2Center.at(1) << ", " << ptR2Center.at(2) << "}" << endl;
  //cout << "Ref Pt Coo using unity Mirror-Sphere vector & sphere pt = {" << reflectedPtCooVectSphereUnity[0] << ", " << reflectedPtCooVectSphereUnity[1] << ", " << reflectedPtCooVectSphereUnity[2] << "}" << endl << endl;
  //cout << "NofPMTPoints = " << NofPMTPoints << endl;

  //cout << "Coordinates of point R2 on reflective plane after reflection on the mirror tile:" << endl;
  //cout << "* using sphere center C to define \U00000394: {" << ptR2Mirr.at(0) << ", " << ptR2Mirr.at(1) << ", " << ptR2Mirr.at(2) << "}" << endl;
}

void CbmRichMirrorSortingAlignment::ComputeP(vector<Double_t>& ptPMirr, vector<Double_t>& ptPR2,
                                             vector<Double_t> normalPMT, vector<Double_t> ptM,
                                             vector<Double_t> ptR2Mirr, Double_t constantePMT)
{
  //cout << endl << "//------------------------------ CbmRichCorrection: ComputeP ------------------------------//" << endl << endl;

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
  //cout << "Coordinates of point P on PMT plane, after reflection on the mirror tile and extrapolation to the PMT plane:" << endl;
  //cout << "* using mirror point M to define \U0001D49F ': {" << ptPMirr.at(0) << ", " << ptPMirr.at(1) << ", " << ptPMirr.at(2) << "}" << endl;
  //cout << "* using reflected point R2 to define \U0001D49F ': {" << ptPR2.at(0) << ", " << ptPR2.at(1) << ", " << ptPR2.at(2) << "}" << endl;
  checkCalc1 =
    ptPMirr.at(0) * normalPMT.at(0) + ptPMirr.at(1) * normalPMT.at(1) + ptPMirr.at(2) * normalPMT.at(2) + constantePMT;
  //cout << "Check whether extrapolated track point on PMT plane verifies its equation (value should be 0.):" << endl;
  //cout << "* using mirror point M, checkCalc = " << checkCalc1 << endl;
  checkCalc2 =
    ptPR2.at(0) * normalPMT.at(0) + ptPR2.at(1) * normalPMT.at(1) + ptPR2.at(2) * normalPMT.at(2) + constantePMT;
  //cout << "* using reflected point R2, checkCalc = " << checkCalc2 << endl;
}

void CbmRichMirrorSortingAlignment::CreateHistoMap(std::map<string, vector<CbmRichMirror*>> mirrorMap,
                                                   std::map<string, TH2D*>& histoMap)
{
  Int_t NofHits = 0;
  Double_t phi = 0., theta0 = 0., thetaCh = 0.;

  for (std::map<string, vector<CbmRichMirror*>>::iterator it = mirrorMap.begin(); it != mirrorMap.end(); ++it) {
    // to get mirror Id:
    string curMirrorId = it->first;
    cout << "curMirrorId: '" << curMirrorId << "' and vector size: " << it->second.size() << endl;
    vector<CbmRichMirror*> mirror = it->second;
    if (curMirrorId != "" && it->second.size() > fThreshold) {
      histoMap[it->first] = new TH2D(string("CherenkovHitsDistribReduced_" + it->first).c_str(),
                                     "CherenkovHitsDistribReduced;#Phi_{Ch} "
                                     "[rad];#theta_{Ch}-#theta_{0} [cm];Entries",
                                     200, -3.4, 3.4, 500, -6., 6.);
      histoMap[it->first]->GetXaxis()->SetTitleSize(0.05);
      histoMap[it->first]->GetXaxis()->SetTitleOffset(0.75);
      histoMap[it->first]->GetYaxis()->SetTitleSize(0.04);
      histoMap[it->first]->GetYaxis()->SetTitleOffset(1.2);
      histoMap[it->first]->GetZaxis()->SetTitleSize(0.03);
      histoMap[it->first]->GetZaxis()->SetTitleOffset(0.6);
      for (int i = 0; i < it->second.size(); i++) {
        CbmRichMirror* mirr        = mirror.at(i);
        string str                 = mirr->getMirrorId();
        TVector3 mom               = mirr->getMomentum();
        vector<Double_t> projHit   = mirr->getProjHit();
        vector<Double_t> extrapHit = mirr->getExtrapHit();
        CbmRichRingLight ringL     = mirr->getRingLight();
        //				cout << "mirror: " << str << endl;
        //				cout << "momentum: {" << mom.X() << ", " << mom.Y() << ", " << mom.Z() << "}" << endl;
        //				cout << "Proj hit coo: {" << projHit[0] << ", " << projHit[1] << "}" << endl;
        //				cout << "Extrap hit coo: {" << extrapHit[0] << ", " << extrapHit[1] << "}" << endl;

        for (Int_t iH = 0; iH < ringL.GetNofHits(); iH++) {
          // ----- Phi angle calculation ----- //
          CbmRichHitLight hit = ringL.GetHit(iH);
          //CbmRichHit* hit = static_cast<CbmRichHit*>(fPmtPoints->At(HitIndex));
          phi = TMath::ATan2(hit.fY - ringL.GetCenterY(), hit.fX - ringL.GetCenterX());

          // ----- Theta_Ch and Theta_0 calculations ----- //
          thetaCh = sqrt(TMath::Power(projHit[0] - hit.fX, 2) + TMath::Power(projHit[1] - hit.fY, 2));
          theta0  = sqrt(TMath::Power(ringL.GetCenterX() - hit.fX, 2) + TMath::Power(ringL.GetCenterY() - hit.fY, 2));
          //cout << "Theta_0 = " << Theta_0 << endl;

          histoMap[it->first]->Fill(phi, thetaCh - theta0);
        }
      }
    }
  }
  cout << endl;
  // For loop to draw all histograms collected for all misaligned mirror tiles
  //	for (std::map<string, TH2D*>::iterator it=histoMap.begin(); it!=histoMap.end(); ++it) {
  //		cout << "Key str: " << it->first << " and nb of entries: " << it->second->GetEntries() << endl << endl;
  //		TCanvas* can = new TCanvas();
  //		it->second->Draw("colz");
  //	}
}

void CbmRichMirrorSortingAlignment::DrawFitAndExtractAngles(std::map<string, vector<Double_t>>& anglesMap,
                                                            std::map<string, TH2D*> histoMap)
{
  Int_t thresh = 3;
  Double_t p1 = 0, p2 = 0, p3 = 0, chi2 = 0, focalLength = 150., q = 0., A = 0., alpha = 0., mis_x = 0., mis_y = 0.;
  // mis_x && mis_y corresponds respect. to rotation angles around the Y and X axes.
  // !!! BEWARE: AXES INDEXES ARE SWITCHED !!!

  for (std::map<string, TH2D*>::iterator it = histoMap.begin(); it != histoMap.end(); ++it) {
    if (it->first != "") {
      TCanvas* can = new TCanvas("can");
      //		can->Divide(3,1);
      can->Divide(2, 1);
      can->SetGrid();
      gStyle->SetOptStat(0);
      can->cd(1);
      TH2D* histo = it->second;
      for (Int_t y_bin = 1; y_bin <= 500; y_bin++) {
        for (Int_t x_bin = 1; x_bin <= 200; x_bin++) {
          if (histo->GetBinContent(x_bin, y_bin) < thresh) { histo->SetBinContent(x_bin, y_bin, 0); }
        }
      }
      histo->Draw("colz");
      histo->FitSlicesY(0, 0, -1, 1);
      histo->Write();

      can->cd(2);
      string histoName = "CherenkovHitsDistribReduced_" + it->first + "_1";
      //cout << "HistoName: " << histoName << endl;
      TH1D* histo_1 = gDirectory->Get<TH1D>((histoName).c_str());
      histo_1->GetXaxis()->SetTitle("#Phi_{Ch} [rad]");
      histo_1->GetYaxis()->SetTitle("#theta_{Ch}-#theta_{0} [cm]");
      histo_1->GetXaxis()->SetTitleSize(0.05);
      histo_1->GetXaxis()->SetTitleOffset(0.75);
      histo_1->GetYaxis()->SetTitleSize(0.04);
      histo_1->GetYaxis()->SetTitleOffset(1.2);
      histo_1->Draw("HIST");
      histo_1->Write();
      //
      //		if ( histo_1->GetSumOfWeights() == 0 || histo_1->Integral() == 0 ) {
      //			cout << "For mirror tile: " << it->first << ":" << endl;
      //			cout << "Sum of weights: " << histo_1->GetSumOfWeights() << endl;
      //			cout << "Integral: " << histo_1->Integral() << endl;
      //			continue;
      //		}
      //
      //		can->cd(3);
      TF1* f1 = new TF1("f1", "[2]+[0]*cos(x)+[1]*sin(x)", -3.5, 3.5);
      f1->SetParameters(0, 0, 0);
      f1->SetParNames("Delta_phi", "Delta_lambda", "Offset");
      histo_1->Fit("f1", "", "");
      TF1* fit = histo_1->GetFunction("f1");
      p1 = fit->GetParameter("Delta_phi"), p2 = fit->GetParameter("Delta_lambda"), p3 = fit->GetParameter("Offset"),
      chi2 = fit->GetChisquare();
      f1->SetParameters(fit->GetParameter(0), fit->GetParameter(1));
      char leg[128];
      f1->SetLineColor(2);
      f1->Draw("same");
      f1->Write();
      // ------------------------------ CALCULATION OF MISALIGNMENT ANGLE ------------------------------ //
      cout << setprecision(6) << endl;
      q = TMath::ATan(fit->GetParameter(0) / fit->GetParameter(1));
      //cout << endl << "fit_1 = " << fit->GetParameter(0) << " and fit_2 = " << fit->GetParameter(1) << endl;
      //cout << "q = " << q << endl;
      A = fit->GetParameter(1) / TMath::Cos(q);
      //cout << "Parameter a = " << A << endl;
      alpha =
        TMath::ATan(A / 1.5) * 0.5
        * TMath::Power(
          10,
          3);  // *0.5, because a mirror rotation of alpha implies a rotation in the particle trajectory of 2*alpha ; 1.5 meters = Focal length = Radius_of_curvature/2
      //cout << setprecision(6) << "Total angle of misalignment alpha = " << alpha << endl;
      mis_x = TMath::ATan(fit->GetParameter(1) / focalLength) * 0.5 * TMath::Power(10, 3);
      mis_y = TMath::ATan(fit->GetParameter(0) / focalLength) * 0.5 * TMath::Power(10, 3);
      cout << "Horizontal displacement = " << mis_x << " [mrad] and vertical displacement = " << mis_y << " [mrad]."
           << endl
           << endl;
      TLegend* LEG = new TLegend(0.15, 0.25, 0.84, 0.4);
      LEG->SetBorderSize(1);
      LEG->SetFillColor(0);
      LEG->SetMargin(0.2);
      LEG->SetTextSize(0.04);
      sprintf(leg, "Fitted sinusoid");
      LEG->AddEntry(f1, leg, "l");
      sprintf(leg, "Rotation angle around X = %.3f", -1 * mis_x);
      LEG->AddEntry("", leg, "l");
      sprintf(leg, "Rotation angle around Y = %.3f", mis_y);
      LEG->AddEntry("", leg, "l");
      sprintf(leg, "Offset = %.3f", fit->GetParameter(2));
      LEG->AddEntry("", leg, "l");
      LEG->Draw();
      Cbm::SaveCanvasAsImage(can, string(fOutputDir.Data() + fStudyName), "png");

      anglesMap[it->first].push_back(fit->GetParameter(1));
      anglesMap[it->first].push_back(fit->GetParameter(0));
      anglesMap[it->first].push_back(mis_x);
      anglesMap[it->first].push_back(mis_y);
    }
  }
}

void CbmRichMirrorSortingAlignment::Finish()
{
  std::map<string, TH2D*> histoMap;
  histoMap.clear();
  std::map<string, vector<Double_t>> anglesMap;
  anglesMap.clear();

  // Filling the reduced thetaCh VS phi histogram and writing the resulting histogram in histoMap:
  CreateHistoMap(fMirrorMap, histoMap);

  // Drawing the obtained thetaCh VS phi histogram ; fitting with sinusoid ; write calculated misalignment angles in anglesMap:
  DrawFitAndExtractAngles(anglesMap, histoMap);

  //TString str_correction = fOutputDir + "/corr_params/correction_param_array_" + fStudyName + ".txt";
  TString str_correction = fOutputDir + "/correction_table/correction_param_array.txt";

  // Check if the directory exists, otherwise creates it.
  TString pathToCorrectionTable = fOutputDir + "/correction_table/";
  gSystem->mkdir(pathToCorrectionTable, true);

  ofstream corrFile;
  corrFile.open(str_correction, std::ofstream::trunc);
  if (corrFile.is_open()) { corrFile.close(); }
  else {
    cout << "Error in CbmRichMirrorSortingAlignment::Finish ; unable to open "
            "parameter file!"
         << endl;
  }

  // Write correction values in output file:
  for (std::map<string, vector<Double_t>>::iterator it = anglesMap.begin(); it != anglesMap.end(); ++it) {
    string mirrorId = it->first;
    cout << "curMirrorId: " << mirrorId << endl;
    vector<Double_t> misAngles = it->second;
    cout << "mirror correction parameters infos: {" << misAngles[0] << ", " << misAngles[1] << "}" << endl;
    corrFile.open(str_correction, std::ofstream::app);
    if (corrFile.is_open()) {
      corrFile << mirrorId << "\n";
      corrFile << setprecision(7) << misAngles[0] << "\n";
      corrFile << setprecision(7) << misAngles[1] << "\n";
      corrFile.close();
      cout << "Wrote correction parameters to: " << str_correction << endl;
    }
    else {
      cout << "Error in CbmRichMirrorSortingAlignment::Finish ; unable to open "
              "parameter file!"
           << endl;
    }
  }

  //TString str_angles = fOutputDir + "/corr_params/reconstructed_angles_array_" + fStudyName + ".txt";
  TString str_angles = fOutputDir + "/correction_table/reconstructed_angles_array.txt";
  ofstream anglesFile;
  anglesFile.open(str_angles, std::ofstream::trunc);
  if (anglesFile.is_open()) { anglesFile.close(); }
  else {
    cout << "Error in CbmRichMirrorSortingAlignment::Finish ; unable to open "
            "parameter file!"
         << endl;
  }

  // Write misalignment angles in output file:
  for (std::map<string, vector<Double_t>>::iterator it = anglesMap.begin(); it != anglesMap.end(); ++it) {
    string mirrorId = it->first;
    cout << "curMirrorId: " << mirrorId << endl;
    vector<Double_t> misAngles = it->second;
    cout << "mirror reconstructed angles infos: {" << misAngles[2] << ", " << misAngles[3] << "}" << endl;
    anglesFile.open(str_angles, std::ofstream::app);
    if (anglesFile.is_open()) {
      anglesFile << mirrorId << "\n";
      anglesFile << setprecision(7) << misAngles[2] << "\n";
      anglesFile << setprecision(7) << misAngles[3] << "\n";
      anglesFile.close();
      cout << "Wrote reconstructed angles to: " << str_angles << endl;
    }
    else {
      cout << "Error in CbmRichMirrorSortingAlignment::Finish ; unable to open "
              "parameter file!"
           << endl;
    }
  }
}
ClassImp(CbmRichMirrorSortingAlignment)
