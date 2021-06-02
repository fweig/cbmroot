/** @file CbmRichCorrection.cxx
  * @copyright Copyright (C) 2016-2021 Justus-Liebig-Universitaet Giessen, Giessen
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Jordan Bendarouach [committer] **/

// ---------- Original Headers ---------- //
#include "CbmRichCorrection.h"

#include "CbmDrawHist.h"
#include "CbmRichHit.h"

#include "FairRootManager.h"
#include <Logger.h>

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2D.h"

#include <iostream>
#include <vector>

// ---------- Included Headers ---------- //
#include "CbmMCTrack.h"
#include "CbmRichConverter.h"
#include "CbmRichPoint.h"
#include "CbmRichRingFitterCOP.h"
#include "CbmRichRingFitterEllipseTau.h"
#include "CbmRichRingLight.h"
#include "CbmTrackMatchNew.h"
#include "CbmUtils.h"

#include "FairTrackParam.h"
#include "FairVolume.h"

#include "TEllipse.h"
#include "TGeoManager.h"

#include <algorithm>
#include <fstream>
#include <iomanip>

#include <stdlib.h>
//#include <stdio.h>
#include "CbmGlobalTrack.h"
#include "CbmRichGeoManager.h"
#include "CbmRichHitProducer.h"

//#include "TLorentzVector.h"
#include "TGeoSphere.h"
#include "TVirtualMC.h"
class TGeoNode;
class TGeoVolume;
class TGeoShape;
class TGeoMatrix;

#include <boost/assign/list_of.hpp>
using boost::assign::list_of;
#include "TStyle.h"

#include <sstream>

CbmRichCorrection::CbmRichCorrection()
  : FairTask()
  , fRichHits(NULL)
  , fRichRings(NULL)
  , fRichProjections(NULL)
  , fRichMirrorPoints(NULL)
  , fRichMCPoints(NULL)
  , fMCTracks(NULL)
  , fRichRingMatches(NULL)
  , fRichRefPlanePoints(NULL)
  , fRichPoints(NULL)
  , fGlobalTracks(NULL)
  , fHM(NULL)
  ,
  //fGP(),
  fNumbAxis(0)
  , fTile(0)
  , fEventNum(0)
  , fOutputDir("")
  , fRunTitle("")
  , fAxisRotTitle("")
  , fDrawProjection(kTRUE)
  , fIsMeanCenter(kFALSE)
  , fIsReconstruction(kFALSE)
  , fCopFit(NULL)
  , fTauFit(NULL)
  , fPhi()
{
}

CbmRichCorrection::~CbmRichCorrection() {}

InitStatus CbmRichCorrection::Init()
{
  FairRootManager* manager = FairRootManager::Instance();

  fRichHits = (TClonesArray*) manager->GetObject("RichHit");
  if (NULL == fRichHits) { Fatal("CbmRichCorrection::Init", "No RichHit array !"); }

  fRichRings = (TClonesArray*) manager->GetObject("RichRing");
  if (NULL == fRichRings) { Fatal("CbmRichCorrection::Init", "No RichRing array !"); }

  fRichProjections = (TClonesArray*) manager->GetObject("RichProjection");
  if (NULL == fRichProjections) { Fatal("CbmRichCorrection::Init", "No RichProjection array !"); }

  fRichMirrorPoints = (TClonesArray*) manager->GetObject("RichMirrorPoint");
  if (NULL == fRichMirrorPoints) { Fatal("CbmRichCorrection::Init", "No RichMirrorPoints array !"); }

  fRichMCPoints = (TClonesArray*) manager->GetObject("RichPoint");
  if (NULL == fRichMCPoints) { Fatal("CbmRichCorrection::Init", "No RichMCPoints array !"); }

  fMCTracks = (TClonesArray*) manager->GetObject("MCTrack");
  if (NULL == fMCTracks) { Fatal("CbmRichCorrection::Init", "No MCTracks array !"); }

  fRichRingMatches = (TClonesArray*) manager->GetObject("RichRingMatch");
  if (NULL == fRichRingMatches) { Fatal("CbmRichCorrection::Init", "No RichRingMatches array !"); }

  fRichRefPlanePoints = (TClonesArray*) manager->GetObject("RefPlanePoint");
  if (NULL == fRichRefPlanePoints) { Fatal("CbmRichCorrection::Init", "No RichRefPlanePoint array !"); }

  fRichPoints = (TClonesArray*) manager->GetObject("RichPoint");
  if (NULL == fRichPoints) { Fatal("CbmRichCorrection::Init", "No RichPoint array !"); }

  fGlobalTracks = (TClonesArray*) manager->GetObject("GlobalTrack");
  if (NULL == fGlobalTracks) { Fatal("CbmRichCorrection::Init", "No GlobalTrack array!"); }

  fCopFit = new CbmRichRingFitterCOP();
  fTauFit = new CbmRichRingFitterEllipseTau();
  CbmRichConverter::Init();

  InitHistProjection();

  return kSUCCESS;
}

void CbmRichCorrection::InitHistProjection()
{
  fHM = new CbmHistManager();
  /*	for (std::map<string,string>::iterator it=fPathsMap.begin(); it!=fPathsMap.end(); ++it) {		// Initialize all the histograms, using map IDs as inputs.
		string name = "fHMCPoints_" + it->second;														// it->first gives the paths; it->second gives the ID.
		fHM->Create2<TH2D>(name, name + ";X_Axis [];Y_Axis [];Entries", 2001, -100., 100.,2001, 60., 210.);
	}
*/
  Double_t upperScaleLimit = 3.5, bin = 400.;
  // fhDistance => fhDistanceCenterToExtrapolatedTrack.
  fHM->Create1<TH1D>("fhDistanceCenterToExtrapolatedTrack",
                     "fhDistanceCenterToExtrapolatedTrack;Distance fitted "
                     "center to extrapolated track;Number of entries",
                     bin, 0., 2.);
  fHM->Create1<TH1D>("fhDistanceCorrected", "fhDistanceCorrected;Distance a [cm];A.U.", bin, 0., upperScaleLimit);
  fHM->Create1<TH1D>("fhDifferenceX", "fhDifferenceX;Difference in X (fitted center - extrapolated track);A.U.", bin,
                     0., upperScaleLimit);
  fHM->Create1<TH1D>("fhDifferenceY", "fhDifferenceY;Difference in Y (fitted center - extrapolated track);A.U.", bin,
                     0., upperScaleLimit);

  fHM->Create1<TH1D>("fhDistanceUncorrected", "fhDistanceUncorrected;Distance a [cm];A.U.", bin, 0., upperScaleLimit);
  fHM->Create1<TH1D>("fhDifferenceXUncorrected", "fhDifferenceXUncorrected;Difference in X uncorrected [cm];A.U.", bin,
                     0., upperScaleLimit);
  fHM->Create1<TH1D>("fhDifferenceYUncorrected", "fhDifferenceYUncorrected;Difference in Y uncorrected [cm];A.U.", bin,
                     0., upperScaleLimit);

  fHM->Create1<TH1D>("fhDistanceIdeal", "fhDistanceIdeal;Distance a [cm];A.U.", bin, 0., upperScaleLimit);
  fHM->Create1<TH1D>("fhDifferenceXIdeal", "fhDifferenceXIdeal;Difference in X ideal [cm];A.U.", bin, 0.,
                     upperScaleLimit);
  fHM->Create1<TH1D>("fhDifferenceYIdeal", "fhDifferenceYIdeal;Difference in Y ideal [cm];A.U.", bin, 0.,
                     upperScaleLimit);

  fHM->Create1<TH1D>("fHistoDiffX",
                     "fHistoDiffX;Histogram difference between corrected and "
                     "ideal X positions;A.U.",
                     bin, 0., upperScaleLimit);
  fHM->Create1<TH1D>("fHistoDiffY",
                     "fHistoDiffY;Histogram difference between corrected and "
                     "ideal Y positions;A.U.",
                     bin, 0., upperScaleLimit);

  fHM->Create1<TH1D>("fHistoBoA", "fHistoBoA;Histogram B axis over A axis;A.U.", bin, 0., upperScaleLimit);
}

void CbmRichCorrection::Exec(Option_t* /*option*/)
{
  cout << endl
       << "//"
          "--------------------------------------------------------------------"
          "------------------------------------------//"
       << endl;
  cout << "//---------------------------------------- CbmRichCorrection - EXEC "
          "Function ----------------------------------------//"
       << endl;
  cout << "//"
          "--------------------------------------------------------------------"
          "--------------------------------------------------//"
       << endl;
  fEventNum++;
  //LOG(debug2) << "CbmRichCorrection : Event #" << fEventNum;
  cout << "CbmRichCorrection : Event #" << fEventNum << endl;

  Int_t nofRingsInEvent = fRichRings->GetEntriesFast();
  Int_t nofMirrorPoints = fRichMirrorPoints->GetEntriesFast();
  Int_t nofHitsInEvent  = fRichHits->GetEntriesFast();
  Int_t NofMCPoints     = fRichMCPoints->GetEntriesFast();
  Int_t NofMCTracks     = fMCTracks->GetEntriesFast();
  cout << "Nb of rings in evt = " << nofRingsInEvent << ", nb of mirror points = " << nofMirrorPoints
       << ", nb of hits in evt = " << nofHitsInEvent << ", nb of Monte-Carlo points = " << NofMCPoints
       << " and nb of Monte-Carlo tracks = " << NofMCTracks << endl
       << endl;

  TClonesArray* projectedPoint;

  if (nofRingsInEvent == 0) { cout << "Error no rings registered in event." << endl << endl; }
  else {
    ProjectionProducer();
  }
}

void CbmRichCorrection::ProjectionProducer()
{
  cout << "//------------------------------ CbmRichCorrection: Projection "
          "Producer ------------------------------//"
       << endl
       << endl;

  Int_t NofMirrorPoints   = fRichMirrorPoints->GetEntriesFast();
  Int_t NofRingsInEvent   = fRichRings->GetEntriesFast();
  Int_t NofGTracks        = fGlobalTracks->GetEntriesFast();
  Int_t NofRefPlanePoints = fRichRefPlanePoints->GetEntriesFast();
  Int_t NofPMTPoints      = fRichPoints->GetEntriesFast();

  // Declaration of points coordinates.
  Double_t sphereRadius = 300., constantePMT = 0.;
  vector<Double_t> ptM(3), ptMNew(3), ptC(3), ptCNew(3), ptR1(3), momR1(3), normalPMT(3), ptR2Mirr(3), ptR2Center(3),
    ptPMirr(3), ptPR2(3), ptTileCenter(3);
  vector<Double_t> ptCIdeal(3), ptR2CenterUnCorr(3), ptR2CenterIdeal(3), ptR2MirrUnCorr(3), ptR2MirrIdeal(3),
    ptPMirrUnCorr(3), ptPMirrIdeal(3), ptPR2UnCorr(3), ptPR2Ideal(3);
  Double_t reflectedPtCooVectSphereUnity[] = {0., 0., 0.};
  TVector3 outPos, outPosUnCorr, outPosIdeal;
  // Declaration of ring parameters.
  Double_t ringCenter[] = {0., 0., 0.}, distToExtrapTrackHit = 0., distToExtrapTrackHitInPlane = 0.;
  //Declarations related to geometry.
  Int_t mirrTrackID = -1, pmtTrackID = -1, refPlaneTrackID = -1, motherID = -100, pmtMotherID = -100;
  CbmMCTrack* track = NULL;
  TGeoNavigator* navi;
  TGeoNode* mirrNode;
  TGeoMatrix *mirrMatrix, *pmtMatrix, *richMatrix;

  CbmRichRecGeoPar* gp = CbmRichGeoManager::GetInstance().fGP;
  Double_t pmtPlaneX   = gp->fPmt.fPlaneX;
  Double_t pmtPlaneY   = gp->fPmt.fPlaneY;
  Double_t pmtWidth    = gp->fPmt.fWidth;
  Double_t pmtHeight   = gp->fPmt.fHeight;

  GetPmtNormal(NofPMTPoints, normalPMT, constantePMT);
  cout << "Calculated normal vector to PMT plane = {" << normalPMT.at(0) << ", " << normalPMT.at(1) << ", "
       << normalPMT.at(2) << "} and constante d = " << constantePMT << endl
       << endl;

  for (Int_t iMirr = 0; iMirr < NofMirrorPoints; iMirr++) {
    //cout << "NofMirrorPoints = " << NofMirrorPoints << " and iMirr = " << iMirr << endl;
    CbmRichPoint* mirrPoint = (CbmRichPoint*) fRichMirrorPoints->At(iMirr);
    mirrTrackID             = mirrPoint->GetTrackID();
    //cout << "Mirror track ID = " << mirrTrackID << endl;
    if (mirrTrackID <= -1) {
      cout << "Mirror track ID <= 1 !!!" << endl;
      cout << "----------------------------------- End of loop N°" << iMirr + 1
           << " on the mirror points. -----------------------------------" << endl
           << endl;
      continue;
    }
    track    = (CbmMCTrack*) fMCTracks->At(mirrTrackID);
    motherID = track->GetMotherId();
    if (motherID == -1) {
      //cout << "Mirror motherID == -1 !!!" << endl << endl;
      ptM.at(0) = mirrPoint->GetX(), ptM.at(1) = mirrPoint->GetY(), ptM.at(2) = mirrPoint->GetZ();
      //cout << "Mirror Point coordinates; x = " << ptM.at(0) << ", y = " << ptM.at(1) << " and z = " << ptM.at(2) << endl;
      mirrNode = gGeoManager->FindNode(ptM.at(0), ptM.at(1), ptM.at(2));
      if (mirrNode) {
        //cout << "Mirror node found! Mirror node name = " << mirrNode->GetName() << endl;
        navi = gGeoManager->GetCurrentNavigator();
        cout << "Navigator path: " << navi->GetPath() << endl;
        cout << "Coordinates of sphere center: " << endl;
        navi->GetCurrentMatrix()->Print();
        if (fIsMeanCenter)
          GetMeanSphereCenter(navi,
                              ptC);  //IF NO INFORMATION ON MIRRORS ARE KNOWN (TO BE USED IN RECONSTRUCTION STEP) !!!
        else {
          ptCIdeal.at(0) = navi->GetCurrentMatrix()->GetTranslation()[0];
          ptCIdeal.at(1) = navi->GetCurrentMatrix()->GetTranslation()[1];
          ptCIdeal.at(2) = navi->GetCurrentMatrix()->GetTranslation()[2];
        }
        cout << "Coordinates of tile center: " << endl;
        navi->GetMotherMatrix()->Print();
        ptC.at(0) = 0., ptC.at(1) = 132.594000, ptC.at(2) = 54.267226;
        cout << "Sphere center coordinates of the aligned mirror tile, ideal = {" << ptCIdeal.at(0) << ", "
             << ptCIdeal.at(1) << ", " << ptCIdeal.at(2) << "}" << endl;
        cout << "Sphere center coordinates of the rotated mirror tile, w/ "
                "GeoManager, = {"
             << ptC.at(0) << ", " << ptC.at(1) << ", " << ptC.at(2) << "} and sphere inner radius = " << sphereRadius
             << endl
             << endl;
        //ptCNew = RotateSphereCenter(ptTileCenter, ptC, navi);

        for (Int_t iRefl = 0; iRefl < NofRefPlanePoints; iRefl++) {
          //new((*projectedPoint)[iRefl]) FairTrackParam(0., 0., 0., 0., 0., 0., covMat);
          CbmRichPoint* refPlanePoint = (CbmRichPoint*) fRichRefPlanePoints->At(iRefl);
          refPlaneTrackID             = refPlanePoint->GetTrackID();
          //cout << "Reflective plane track ID = " << refPlaneTrackID << endl;
          if (mirrTrackID == refPlaneTrackID) {
            //cout << "IDENTICAL TRACK ID FOUND !!!" << endl << endl;
            ptR1.at(0) = refPlanePoint->GetX(), ptR1.at(1) = refPlanePoint->GetY(), ptR1.at(2) = refPlanePoint->GetZ();
            momR1.at(0) = refPlanePoint->GetPx(), momR1.at(1) = refPlanePoint->GetPy(),
            momR1.at(2) = refPlanePoint->GetPz();
            cout << "Reflective Plane Point coordinates = {" << ptR1.at(0) << ", " << ptR1.at(1) << ", " << ptR1.at(2)
                 << "}" << endl;
            cout << "And reflective Plane Point momenta = {" << momR1.at(0) << ", " << momR1.at(1) << ", "
                 << momR1.at(2) << "}" << endl;
            cout << "Mirror Point coordinates = {" << ptM.at(0) << ", " << ptM.at(1) << ", " << ptM.at(2) << "}"
                 << endl;
            CalculateMirrorIntersection(ptM, ptCIdeal, ptMNew);

            if (fIsMeanCenter) {
              GetMirrorIntersection(ptM, ptR1, momR1, ptC, sphereRadius);
              // From ptM: how to retrieve tile ID ???
              // => Compare distance of ptM to tile centers
            }

            ComputeR2(ptR2CenterUnCorr, ptR2MirrUnCorr, ptM, ptC, ptR1, navi, "Uncorrected");
            ComputeR2(ptR2Center, ptR2Mirr, ptM, ptC, ptR1, navi, "Corrected");
            ComputeR2(ptR2CenterIdeal, ptR2MirrIdeal, ptM, ptCIdeal, ptR1, navi, "Uncorrected");

            ComputeP(ptPMirrUnCorr, ptPR2UnCorr, normalPMT, ptM, ptR2MirrUnCorr, constantePMT);
            ComputeP(ptPMirr, ptPR2, normalPMT, ptM, ptR2Mirr, constantePMT);
            ComputeP(ptPMirrIdeal, ptPR2Ideal, normalPMT, ptM, ptR2MirrIdeal, constantePMT);

            TVector3 inPosUnCorr(ptPMirrUnCorr.at(0), ptPMirrUnCorr.at(1), ptPMirrUnCorr.at(2));
            CbmRichGeoManager::GetInstance().RotatePoint(&inPosUnCorr, &outPosUnCorr);
            cout << endl
                 << "New mirror points coordinates = {" << outPosUnCorr.x() << ", " << outPosUnCorr.y() << ", "
                 << outPosUnCorr.z() << "}" << endl;
            TVector3 inPos(ptPMirr.at(0), ptPMirr.at(1), ptPMirr.at(2));
            CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);
            cout << "New mirror points coordinates = {" << outPos.x() << ", " << outPos.y() << ", " << outPos.z() << "}"
                 << endl;
            TVector3 inPosIdeal(ptPMirrIdeal.at(0), ptPMirrIdeal.at(1), ptPMirrIdeal.at(2));
            CbmRichGeoManager::GetInstance().RotatePoint(&inPosIdeal, &outPosIdeal);
            cout << "New mirror points coordinates = {" << outPosIdeal.x() << ", " << outPosIdeal.y() << ", "
                 << outPosIdeal.z() << "}" << endl
                 << endl;

            /*for (Int_t iPmt = 0; iPmt < NofPMTPoints; iPmt++) {
						CbmRichPoint *pmtPoint = (CbmRichPoint*) fRichPoints->At(iPmt);
						pmtTrackID = pmtPoint->GetTrackID();
						CbmMCTrack* track3 = (CbmMCTrack*) fMCTracks->At(pmtTrackID);
						pmtMotherID = track3->GetMotherId();
						//cout << "pmt mother ID = " << pmtMotherID << endl;
						if (pmtMotherID == mirrTrackID) {
								ptP[0] = pmtPoint->GetX(), ptP[1] = pmtPoint->GetY(), ptP[2] = pmtPoint->GetZ();
								//cout << "Identical mirror track ID and PMT mother ID !!!" << endl;
								//cout << "PMT Point coordinates; x = " << pmtPoint->GetX() << ", y = " << pmtPoint->GetY() << " and z = " << pmtPoint->GetZ() << endl;
							}
						}
						cout << "Looking for PMT hits: end." << endl << endl;*/
          }
          //else { cout << "No identical track ID between mirror point and reflective plane point found ..." << endl << endl; }
        }

        /*for (Int_t iPmt = 0; iPmt < NofPMTPoints; iPmt++) {
					CbmRichPoint *pmtPoint = (CbmRichPoint*) fRichPoints->At(iPmt);
					cout << "PMT points = {" << pmtPoint->GetX() << ", " << pmtPoint->GetY() << ", " << pmtPoint->GetZ() << "}" << endl;
					TVector3 inputPoint(pmtPoint->GetX(), pmtPoint->GetY(), pmtPoint->GetZ());
					TVector3 outputPoint;
					CbmRichHitProducer::TiltPoint(&inputPoint, &outputPoint, fGP.fPmtPhi, fGP.fPmtTheta, fGP.fPmtZOrig);
					cout << "New PMT points after rotation of the PMT plane: " << endl;
					cout << outputPoint.X() << "\t" << outputPoint.Y() << "\t" << outputPoint.Z() << endl;
				}*/

        FillHistProjection(outPosIdeal, outPosUnCorr, outPos, NofGTracks, normalPMT, constantePMT);
      }
      else {
        cout << "Not a mother particle ..." << endl;
      }
      cout << "----------------------------------- "
           << "End of loop N°" << iMirr + 1 << " on the mirror points."
           << " -----------------------------------" << endl
           << endl;
    }
  }
}

void CbmRichCorrection::GetPmtNormal(Int_t NofPMTPoints, vector<Double_t>& normalPMT, Double_t& normalCste)
{
  //cout << endl << "//------------------------------ CbmRichCorrection: Calculate PMT Normal ------------------------------//" << endl << endl;

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
    a[0] = pmtPoint->GetX(), a[1] = pmtPoint->GetY(), a[2] = pmtPoint->GetZ();
    //cout << "a[0] = " << a[0] << ", a[1] = " << a[1] << " et a[2] = " << a[2] << endl;
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
      b[0] = pmtPoint->GetX(), b[1] = pmtPoint->GetY(), b[2] = pmtPoint->GetZ();
      //cout << "b[0] = " << b[0] << ", b[1] = " << b[1] << " et b[2] = " << b[2] << endl;
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

void CbmRichCorrection::GetMeanSphereCenter(TGeoNavigator* navi, vector<Double_t>& ptC)
{
  const Char_t* topNodePath;
  topNodePath = gGeoManager->GetTopNode()->GetName();
  cout << "Top node path: " << topNodePath << endl;
  TGeoVolume* rootTop;
  rootTop = gGeoManager->GetTopVolume();
  rootTop->Print();

  TGeoIterator nextNode(rootTop);
  TGeoNode* curNode;
  const TGeoMatrix* curMatrix;
  const Double_t* curNodeTranslation;  // 3 components - pointers to some memory which is provided by ROOT
  const Double_t* curNodeRotationM;    // 9 components - pointers to some memory which is provided by ROOT
  TString filterName0("mirror_tile_type0");
  TString filterName1("mirror_tile_type1");
  TString filterName2("mirror_tile_type2");
  TString filterName3("mirror_tile_type3");
  TString filterName4("mirror_tile_type4");
  TString filterName5("mirror_tile_type5");
  Int_t counter       = 0;
  Double_t sphereXTot = 0., sphereYTot = 0., sphereZTot = 0.;

  while ((curNode = nextNode())) {
    TString nodeName(curNode->GetName());
    TString nodePath;

    // Filter using volume name, not node name
    // But you can do 'if (nodeName.Contains("filter"))'
    if (curNode->GetVolume()->GetName() == filterName0 || curNode->GetVolume()->GetName() == filterName1
        || curNode->GetVolume()->GetName() == filterName2 || curNode->GetVolume()->GetName() == filterName3
        || curNode->GetVolume()->GetName() == filterName4 || curNode->GetVolume()->GetName() == filterName5) {
      if (curNode->GetNdaughters() == 0) {
        // All deepest nodes of mirror tiles here (leaves)
        // Thus we get spherical surface centers
        nextNode.GetPath(nodePath);
        curMatrix          = nextNode.GetCurrentMatrix();
        curNodeTranslation = curMatrix->GetTranslation();
        curNodeRotationM   = curMatrix->GetRotationMatrix();
        printf("%s tr:\t", nodePath.Data());
        printf("%08f\t%08f\t%08f\t\n", curNodeTranslation[0], curNodeTranslation[1], curNodeTranslation[2]);
        if (curNodeTranslation[1] > 0) {  // CONDITION FOR UPPER MIRROR WALL STUDY
          sphereXTot += curNodeTranslation[0];
          sphereYTot += curNodeTranslation[1];
          sphereZTot += curNodeTranslation[2];
          counter++;
        }
      }
    }
  }
  ptC.at(0) = sphereXTot / counter;
  ptC.at(1) = sphereYTot / counter;
  ptC.at(2) = sphereZTot / counter;

  counter = 0;
  nextNode.Reset();
}

void CbmRichCorrection::GetMirrorIntersection(vector<Double_t>& ptM, vector<Double_t> ptR1, vector<Double_t> momR1,
                                              vector<Double_t> ptC, Double_t sphereRadius)
{
  Double_t a = 0., b = 0., c = 0., d = 0., k0 = 0., k1 = 0., k2 = 0.;

  a = TMath::Power(momR1.at(0), 2) + TMath::Power(momR1.at(1), 2) + TMath::Power(momR1.at(2), 2);
  b = 2
      * (momR1.at(0) * (ptR1.at(0) - ptC.at(0)) + momR1.at(1) * (ptR1.at(1) - ptC.at(1))
         + momR1.at(2) * (ptR1.at(2) - ptC.at(2)));
  c = TMath::Power(ptR1.at(0) - ptC.at(0), 2) + TMath::Power(ptR1.at(1) - ptC.at(1), 2)
      + TMath::Power(ptR1.at(2) - ptC.at(2), 2) - TMath::Power(sphereRadius, 2);
  d = b * b - 4 * a * c;
  cout << "d = " << d << endl;

  if (d < 0) {
    cout << "Error no solution to degree 2 equation found ; discriminant below 0." << endl;
    ptM.at(0) = 0., ptM.at(1) = 0., ptM.at(2) = 0.;
  }
  else if (d == 0) {
    cout << "One solution to degree 2 equation found." << endl;
    k0        = -b / (2 * a);
    ptM.at(0) = ptR1.at(0) + k0 * momR1.at(0);
    ptM.at(1) = ptR1.at(1) + k0 * momR1.at(1);
    ptM.at(2) = ptR1.at(2) + k0 * momR1.at(2);
  }
  else if (d > 0) {
    cout << "Two solutions to degree 2 equation found." << endl;
    k1 = ((-b - TMath::Sqrt(d)) / (2 * a));
    k2 = ((-b + TMath::Sqrt(d)) / (2 * a));

    if (ptR1.at(2) + k1 * momR1.at(2) > ptR1.at(2) + k2 * momR1.at(2)) {
      ptM.at(0) = ptR1.at(0) + k1 * momR1.at(0);
      ptM.at(1) = ptR1.at(1) + k1 * momR1.at(1);
      ptM.at(2) = ptR1.at(2) + k1 * momR1.at(2);
    }
    else if (ptR1.at(2) + k1 * momR1.at(2) < ptR1.at(2) + k2 * momR1.at(2)) {
      ptM.at(0) = ptR1.at(0) + k2 * momR1.at(0);
      ptM.at(1) = ptR1.at(1) + k2 * momR1.at(1);
      ptM.at(2) = ptR1.at(2) + k2 * momR1.at(2);
    }
  }
}

vector<Double_t> CbmRichCorrection::RotateSphereCenter(vector<Double_t> ptM, vector<Double_t> ptC, TGeoNavigator* navi)
{
  vector<Double_t> ptCNew(3), ptCNew2(3), ptCNew3(3);
  Double_t cosPhi = 0., sinPhi = 0., cosTheta = 0., sinTheta = 0., phi2 = 0., theta2 = 0.;
  Double_t diff[3], transfoMat[3][3], invMat[3][3], corrMat[3][3], buff1[3][3], buff2[3][3], buff3[3][3], buff4[3][3],
    buff5[3][3], RotX[3][3], RotY[3][3];
  Double_t corrMat2[3][3], RotX2[3][3], RotY2[3][3];
  InvertMatrix(transfoMat, invMat, navi);
  /*for (Int_t i=0; i<3; i++) {
		for (Int_t j=0; j<3; j++) {
			cout << invMat[i][j] << "\t";
		}
		cout << endl;
	}*/

  // Reading misalignment information from correction_param.txt text file.
  vector<Double_t> outputFit(4);
  ifstream corr_file;
  corr_file.open("correction_param.txt");
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

  // Initializing the matrices used for further calculations.
  for (Int_t i = 0; i < 3; i++) {
    for (Int_t j = 0; j < 3; j++) {
      corrMat[i][j]  = 0.;
      corrMat2[i][j] = 0.;
      buff1[i][j]    = 0.;
      buff2[i][j]    = 0.;
      buff3[i][j]    = 0.;
      buff4[i][j]    = 0.;
      buff5[i][j]    = 0.;
      RotX[i][j]     = 0.;
      RotX2[i][j]    = 0.;
      RotY[i][j]     = 0.;
      RotY2[i][j]    = 0.;
    }
    ptCNew.at(i)  = 0.;
    ptCNew2.at(i) = 0.;
    ptCNew3.at(i) = 0.;
  }

  //Initializing the cosine and sine functions, using inputs from text file. Phi (resp. Theta) angle corresponds to calculated rotation around X (resp. Y) axis.
  cosPhi   = TMath::Cos(outputFit.at(0));
  sinPhi   = TMath::Sin(outputFit.at(0));
  cosTheta = TMath::Cos(outputFit.at(1));
  sinTheta = TMath::Sin(outputFit.at(1));

  //Initializing the rotation matrices for rotations around X and Y axes.
  // Y towards Z is the rotation direction defined in the tile frame, which is the same definition as for the global
  // frame. But as the X axis direction is opposite in the two frames, to correct for misalignment, the rotation
  // direction remains unchanged in the global frame.
  // So define rotation around X axis, with Y towards Z, in the global frame:
  RotX[0][0] = 1;
  RotX[0][1] = 0;
  RotX[0][2] = 0;
  RotX[1][0] = 0;
  RotX[1][1] = cosPhi;
  RotX[1][2] = -1 * sinPhi;
  RotX[2][0] = 0;
  RotX[2][1] = sinPhi;
  RotX[2][2] = cosPhi;
  // X towards Z is the rotation direction defined in the tile frame, which is the same definition as for the global
  // frame. And as the Y axes direction in the tile and global frames are identical, the rotation direction for the
  // correction is the opposite in the global frame - in order to correct for the misalignment.
  // So define rotation around Y axis, with Z towards X, in the global frame:
  RotY[0][0] = cosTheta;
  RotY[0][1] = 0;
  RotY[0][2] = sinTheta;
  RotY[1][0] = 0;
  RotY[1][1] = 1;
  RotY[1][2] = 0;
  RotY[2][0] = -1 * sinTheta;
  RotY[2][1] = 0;
  RotY[2][2] = cosTheta;

  // Translation of the sphere center of the tile: S.
  for (Int_t i = 0; i < 3; i++) {
    diff[i] = ptC.at(i) - ptM.at(i);
  }

  // Calculation of the correction matrix, from rotation matrices.
  for (Int_t i = 0; i < 3; i++) {
    for (Int_t j = 0; j < 3; j++) {
      //corrMat[i][j] = RotZ[i][j];
      for (Int_t k = 0; k < 3; k++) {
        corrMat[i][j] = RotY[i][k] * RotX[k][j] + corrMat[i][j];
      }
    }
  }
  // Calculation of the new coordinates of the translated point S: newS = transfoMat*corrMat*invMat*diff.
  for (Int_t i = 0; i < 3; i++) {
    for (Int_t j = 0; j < 3; j++) {
      for (Int_t k = 0; k < 3; k++) {
        buff1[i][j] = corrMat[i][k] * invMat[k][j] + buff1[i][j];
      }
    }
  }
  for (Int_t i = 0; i < 3; i++) {
    for (Int_t j = 0; j < 3; j++) {
      for (Int_t k = 0; k < 3; k++) {
        buff2[i][j] = transfoMat[i][k] * buff1[k][j] + buff2[i][j];
      }
    }
  }
  for (Int_t i = 0; i < 3; i++) {
    for (Int_t j = 0; j < 3; j++) {
      ptCNew.at(i) = buff2[i][j] * diff[j] + ptCNew.at(i);
      //ptCNew.at(i) = invMat[i][j]*diff[j] + ptCNew.at(i);
    }
  }

  // Calculating the theoretical rotation angles to be applied to the translated point S, to get the point along the Z axis (should obtain a {0; 0; -300} coo).
  phi2   = TMath::ATan2(diff[1], -1 * diff[2]);
  theta2 = TMath::ATan2(diff[0], -1 * diff[2]);
  cout << "Calculated Phi (= arctan(y/-z)), in degrees: " << TMath::RadToDeg() * phi2
       << " and calculated Theta (= arctan(x/-z)), in degrees: " << TMath::RadToDeg() * theta2 << endl;
  // Defining the rotation matrices accordingly:
  // Rotation around X axis, with Z towards Y.
  RotX2[0][0] = 1;
  RotX2[0][1] = 0;
  RotX2[0][2] = 0;
  RotX2[1][0] = 0;
  RotX2[1][1] = TMath::Cos(phi2);
  RotX2[1][2] = TMath::Sin(phi2);
  RotX2[2][0] = 0;
  RotX2[2][1] = -1 * TMath::Sin(phi2);
  RotX2[2][2] = TMath::Cos(phi2);
  // Rotation around Y axis, with Z towards X.
  RotY2[0][0] = TMath::Cos(theta2);
  RotY2[0][1] = 0;
  RotY2[0][2] = TMath::Sin(theta2);
  RotY2[1][0] = 0;
  RotY2[1][1] = 1;
  RotY2[1][2] = 0;
  RotY2[2][0] = -1 * TMath::Sin(theta2);
  RotY2[2][1] = 0;
  RotY2[2][2] = TMath::Cos(theta2);

  // Calculation of the correction matrix, from new rotation matrices.
  for (Int_t i = 0; i < 3; i++) {
    for (Int_t j = 0; j < 3; j++) {
      for (Int_t k = 0; k < 3; k++) {
        corrMat2[i][j] = RotY2[i][k] * RotX2[k][j] + corrMat2[i][j];
      }
    }
  }
  // Calculation of the new coordinates of the translated point S: newS = transfoMat*corrMat2*invMat*diff.
  for (Int_t i = 0; i < 3; i++) {
    for (Int_t j = 0; j < 3; j++) {
      for (Int_t k = 0; k < 3; k++) {
        buff3[i][j] = corrMat2[i][k] * invMat[k][j] + buff3[i][j];
      }
    }
  }
  for (Int_t i = 0; i < 3; i++) {
    for (Int_t j = 0; j < 3; j++) {
      for (Int_t k = 0; k < 3; k++) {
        buff4[i][j] = transfoMat[i][k] * buff3[k][j] + buff4[i][j];
      }
    }
  }
  for (Int_t i = 0; i < 3; i++) {
    for (Int_t j = 0; j < 3; j++) {
      for (Int_t k = 0; k < 3; k++) {
        buff5[i][j] = RotX[i][k] * invMat[k][j] + buff5[i][j];
        //buff5[i][j] = RotY[i][k]*invMat[k][j] + buff5[i][j];
      }
    }
  }
  for (Int_t i = 0; i < 3; i++) {
    for (Int_t j = 0; j < 3; j++) {
      ptCNew2.at(i) = buff4[i][j] * diff[j] + ptCNew2.at(i);
      //ptCNew3.at(i) = buff3[i][j]*diff[j] + ptCNew3.at(i);
      ptCNew3.at(i) = buff5[i][j] * diff[j] + ptCNew3.at(i);
    }
  }

  for (Int_t i = 0; i < 3; i++) {
    ptCNew.at(i) += ptM.at(i);
    ptCNew2.at(i) += ptM.at(i);
    ptCNew3.at(i) += ptM.at(i);
  }
  cout << "diff = {" << diff[0] << ", " << diff[1] << ", " << diff[2] << "}" << endl;
  cout << "New coordinates of the rotated tile sphere center (using angles "
          "from text file) = {"
       << ptCNew.at(0) << ", " << ptCNew.at(1) << ", " << ptCNew.at(2) << "}" << endl;
  cout << "New coordinates of the rotated tile sphere center (using calculated "
          "angles) = {"
       << ptCNew2.at(0) << ", " << ptCNew2.at(1) << ", " << ptCNew2.at(2) << "}" << endl;
  cout << "Tile coordinates after translation, invMat and rotations around X "
          "and Y axes = {"
       << ptCNew3.at(0) << ", " << ptCNew3.at(1) << ", " << ptCNew3.at(2) << "}" << endl
       << endl;

  return ptCNew;
}

void CbmRichCorrection::InvertMatrix(Double_t mat[3][3], Double_t invMat[3][3], TGeoNavigator* navi)
{
  Double_t deter = 0., det11 = 0., det12 = 0., det13 = 0., det21 = 0., det22 = 0., det23 = 0., det31 = 0., det32 = 0.,
           det33 = 0., buff[3][3], prodMat[3][3];

  //Filling the transformation matrix of the tile.
  // STANDARD FILL:
  mat[0][0] = navi->GetMotherMatrix()->GetRotationMatrix()[0];
  mat[0][1] = navi->GetMotherMatrix()->GetRotationMatrix()[1];
  mat[0][2] = navi->GetMotherMatrix()->GetRotationMatrix()[2];
  mat[1][0] = navi->GetMotherMatrix()->GetRotationMatrix()[3];
  mat[1][1] = navi->GetMotherMatrix()->GetRotationMatrix()[4];
  mat[1][2] = navi->GetMotherMatrix()->GetRotationMatrix()[5];
  mat[2][0] = navi->GetMotherMatrix()->GetRotationMatrix()[6];
  mat[2][1] = navi->GetMotherMatrix()->GetRotationMatrix()[7];
  mat[2][2] = navi->GetMotherMatrix()->GetRotationMatrix()[8];

  /*	// TEST FILL:
	mat[0][0] = navi->GetMotherMatrix()->GetRotationMatrix()[2];
	mat[0][1] = navi->GetMotherMatrix()->GetRotationMatrix()[1];
	mat[0][2] = navi->GetMotherMatrix()->GetRotationMatrix()[0];
	mat[1][0] = navi->GetMotherMatrix()->GetRotationMatrix()[5];
	mat[1][1] = navi->GetMotherMatrix()->GetRotationMatrix()[4];
	mat[1][2] = navi->GetMotherMatrix()->GetRotationMatrix()[3];
	mat[2][0] = navi->GetMotherMatrix()->GetRotationMatrix()[8];
	mat[2][1] = navi->GetMotherMatrix()->GetRotationMatrix()[7];
	mat[2][2] = navi->GetMotherMatrix()->GetRotationMatrix()[6];
*/
  /*for (Int_t i=0; i<3; i++) {
		for (Int_t j=0; j<3; j++) {
			mat[i][j] = navi->GetMotherMatrix()->GetRotationMatrix()[i*3+j];
		}
	}
	cout << "Matrix index [2][0] = " << mat[2][0] << endl;*/

  //Computing the inverse of the matrix: inv = (1/det)*adjugate(mat) = (1/det)*transpose(cofactor_matrix(mat)).
  deter = mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1])
          - mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])
          + mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
  if (deter == 0) {
    cout << "Error in CbmRichCorrection::InvertMatrix; determinant of input "
            "matrix equals to zero !!!"
         << endl;
    sleep(5);
    for (Int_t i = 0; i < 3; i++) {
      for (Int_t j = 0; j < 3; j++) {
        invMat[i][j] = 0;
      }
    }
  }
  else {
    buff[0][0] = TMath::Power(-1, 2) * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]);
    buff[0][1] = TMath::Power(-1, 3) * (mat[0][1] * mat[2][2] - mat[0][2] * mat[2][1]);
    buff[0][2] = TMath::Power(-1, 4) * (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]);
    buff[1][0] = TMath::Power(-1, 3) * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]);
    buff[1][1] = TMath::Power(-1, 4) * (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]);
    buff[1][2] = TMath::Power(-1, 5) * (mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0]);
    buff[2][0] = TMath::Power(-1, 4) * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    buff[2][1] = TMath::Power(-1, 5) * (mat[0][0] * mat[2][1] - mat[0][1] * mat[2][0]);
    buff[2][2] = TMath::Power(-1, 6) * (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]);

    for (Int_t i = 0; i < 3; i++) {
      for (Int_t j = 0; j < 3; j++) {
        invMat[i][j]  = buff[i][j] / deter;
        prodMat[i][j] = 0;
      }
    }

    /*		for (Int_t i=0; i<3; i++) {
 			for (Int_t j=0; j<3; j++) {
 				for (Int_t k=0; k<3; k++) {
 					prodMat[i][j] = mat[i][k]*invMat[k][j] + prodMat[i][j];
 				}
 			}
 		}

		cout << "Matrix calculation to check whether inverse matrix is correct:" << endl;
 		for (Int_t i=0; i<3; i++) {
 			for (Int_t j=0; j<3; j++) {
 				cout << "Resulting matrix = [" << prodMat[i][j] << "] \t";
 			}
 			cout << endl;
 		}*/
  }
}

void CbmRichCorrection::CalculateMirrorIntersection(vector<Double_t> ptM, vector<Double_t> ptCIdeal,
                                                    vector<Double_t>& ptMNew)
{
  Double_t t = 0., diffX = 0., diffY = 0., diffZ = 0.;
  diffX = ptM.at(0) - ptCIdeal.at(0);
  diffY = ptM.at(1) - ptCIdeal.at(1);
  diffZ = ptM.at(2) - ptCIdeal.at(2);
  t     = TMath::Sqrt(300 * 300 / (diffX * diffX + diffY * diffY + diffZ * diffZ));

  ptMNew.at(0) = t * diffX + ptCIdeal.at(0);
  ptMNew.at(1) = t * diffY + ptCIdeal.at(1);
  ptMNew.at(2) = t * diffZ + ptCIdeal.at(2);
  cout << "New coordinates of point M = {" << ptMNew.at(0) << ", " << ptMNew.at(1) << ", " << ptMNew.at(2) << "}"
       << endl;
}

void CbmRichCorrection::ComputeR2(vector<Double_t>& ptR2Center, vector<Double_t>& ptR2Mirr, vector<Double_t> ptM,
                                  vector<Double_t> ptC, vector<Double_t> ptR1, TGeoNavigator* navi, TString s)
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
    TString str = fOutputDir + "correction_param_array___" + fNumbAxis + fTile + ".txt";
    corr_file.open(str);
    if (corr_file.is_open()) {
      for (Int_t i = 0; i < 4; i++) {
        corr_file >> outputFit.at(i);
      }
      //for (Int_t i=0; i<2; i++) {corr_file >> outputFit.at(i);}
      corr_file.close();
    }
    else {
      cout << "Error in CbmRichCorrection: unable to open parameter file!" << endl;
      cout << "Parameter file path = " << str << endl << endl;
      sleep(5);
    }
    cout << "Misalignment parameters read from file = [" << outputFit.at(0) << " ; " << outputFit.at(1) << " ; "
         << outputFit.at(2) << " ; " << outputFit.at(3) << "]" << endl;

    //ptCNew.at(0) = TMath::Abs(ptC.at(0) - TMath::Abs(outputFit.at(3)));
    //ptCNew.at(1) = TMath::Abs(ptC.at(1) - TMath::Abs(outputFit.at(2)));
    ptCNew.at(0)       = ptC.at(0) + outputFit.at(3);
    ptCNew.at(1)       = ptC.at(1) + outputFit.at(2);
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
  /*//SAME AS calculation of t2 above
	t3 = ((ptR1.at(0)-ptCNew.at(0))*(ptCNew.at(0)-ptM.at(0)) + (ptR1.at(1)-ptCNew.at(1))*(ptCNew.at(1)-ptM.at(1)) + (ptR1.at(2)-ptCNew.at(2))*(ptCNew.at(2)-ptM.at(2)))/TMath::Sqrt(TMath::Power(ptCNew.at(0) - ptM.at(0),2)+TMath::Power(ptCNew.at(1) - ptM.at(1),2)+TMath::Power(ptCNew.at(2) - ptM.at(2),2));
	reflectedPtCooVectSphereUnity[0] = 2*(ptCNew.at(0)+t3*(normalMirr.at(0)))-ptR1.at(0);
	reflectedPtCooVectSphereUnity[1] = 2*(ptCNew.at(1)+t3*(normalMirr.at(1)))-ptR1.at(1);
	reflectedPtCooVectSphereUnity[2] = 2*(ptCNew.at(2)+t3*(normalMirr.at(2)))-ptR1.at(2);*/
  cout << "Coordinates of point R2 on reflective plane after reflection on the "
          "mirror tile:"
       << endl;
  //cout << "* using mirror point M to define \U00000394: {" << ptR2Center.at(0) << ", " << ptR2Center.at(1) << ", " << ptR2Center.at(2) << "}" << endl;
  cout << "* using sphere center C to define \U00000394: {" << ptR2Mirr.at(0) << ", " << ptR2Mirr.at(1) << ", "
       << ptR2Mirr.at(2) << "}" << endl;
  //cout << "Ref Pt Coo using unity Mirror-Sphere vector & sphere pt = {" << reflectedPtCooVectSphereUnity[0] << ", " << reflectedPtCooVectSphereUnity[1] << ", " << reflectedPtCooVectSphereUnity[2] << "}" << endl << endl;
  //cout << "NofPMTPoints = " << NofPMTPoints << endl;
}

void CbmRichCorrection::ComputeP(vector<Double_t>& ptPMirr, vector<Double_t>& ptPR2, vector<Double_t> normalPMT,
                                 vector<Double_t> ptM, vector<Double_t> ptR2Mirr, Double_t constantePMT)
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

void CbmRichCorrection::FillHistProjection(TVector3 outPosIdeal, TVector3 outPosUnCorr, TVector3 outPos,
                                           Int_t NofGTracks, vector<Double_t> normalPMT, Double_t constantePMT)
{
  CbmMCTrack* track2    = NULL;
  Double_t ringCenter[] = {0, 0, 0}, distToExtrapTrackHit = 0, distToExtrapTrackHitInPlane = 0,
           distToExtrapTrackHitInPlaneUnCorr = 0, distToExtrapTrackHitInPlaneIdeal = 0;

  for (Int_t iGlobalTrack = 0; iGlobalTrack < NofGTracks; iGlobalTrack++) {
    //cout << "Nb of global tracks = " << NofGTracks << " and iGlobalTrack = " << iGlobalTrack << endl;
    CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(iGlobalTrack);
    if (NULL == gTrack) continue;
    Int_t richInd = gTrack->GetRichRingIndex();
    //cout << "Rich index = " << richInd << endl;
    if (richInd < 0) { continue; }
    CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(richInd));
    if (NULL == ring) { continue; }
    Int_t ringTrackID  = ring->GetTrackID();
    track2             = (CbmMCTrack*) fMCTracks->At(ringTrackID);
    Int_t ringMotherID = track2->GetMotherId();
    //cout << "Ring mother ID = " << ringMotherID << ", ring track ID = " << ringTrackID << " and track2 pdg = " << track2->GetPdgCode() << endl;
    CbmRichRingLight ringL;
    CbmRichConverter::CopyHitsToRingLight(ring, &ringL);
    fCopFit->DoFit(&ringL);
    //fTauFit->DoFit(&ringL);
    ringCenter[0] = ringL.GetCenterX();
    ringCenter[1] = ringL.GetCenterY();
    ringCenter[2] =
      -1 * ((normalPMT.at(0) * ringCenter[0] + normalPMT.at(1) * ringCenter[1] + constantePMT) / normalPMT.at(2));

    vector<Double_t> r(3),
      p(3);  // Absolute coordinates of fitted ring Center r and PMT extrapolated point p
    r.at(0) = TMath::Abs(ringCenter[0]), r.at(1) = TMath::Abs(ringCenter[1]), r.at(2) = TMath::Abs(ringCenter[2]);
    p.at(0) = TMath::Abs(outPos.x()), p.at(1) = TMath::Abs(outPos.y()), p.at(2) = TMath::Abs(outPos.z());
    cout << "Ring center coordinates = {" << ringCenter[0] << ", " << ringCenter[1] << ", " << ringCenter[2] << "}"
         << endl;
    cout << "Difference in X = " << TMath::Abs(r.at(0) - p.at(0)) << "; \t"
         << "Difference in Y = " << TMath::Abs(r.at(1) - p.at(1)) << "; \t"
         << "Difference in Z = " << TMath::Abs(r.at(2) - p.at(2)) << endl;
    fHM->H1("fhDifferenceX")->Fill(TMath::Abs(r.at(0) - p.at(0)));
    fHM->H1("fhDifferenceY")->Fill(TMath::Abs(r.at(1) - p.at(1)));
    distToExtrapTrackHit        = TMath::Sqrt(TMath::Power(r.at(0) - p.at(0), 2) + TMath::Power(r.at(1) - p.at(1), 2)
                                       + TMath::Power(r.at(2) - p.at(2), 2));
    distToExtrapTrackHitInPlane = TMath::Sqrt(TMath::Power(r.at(0) - p.at(0), 2) + TMath::Power(r.at(1) - p.at(1), 2));
    fHM->H1("fhDistanceCenterToExtrapolatedTrack")->Fill(distToExtrapTrackHit);
    fHM->H1("fhDistanceCorrected")->Fill(distToExtrapTrackHitInPlane);
    //cout << "Distance between fitted ring center and extrapolated track hit = " << distToExtrapTrackHit << endl;
    cout << "Distance between fitted ring center and extrapolated track hit in "
            "plane = "
         << distToExtrapTrackHitInPlane << endl;

    vector<Double_t> pUnCorr(3);  // Absolute coordinates of fitted ring Center r and PMT extrapolated point p
    pUnCorr.at(0) = TMath::Abs(outPosUnCorr.x()), pUnCorr.at(1) = TMath::Abs(outPosUnCorr.y()),
    pUnCorr.at(2) = TMath::Abs(outPosUnCorr.z());
    //cout << "Ring center coordinates = {" << ringCenter[0] << ", " << ringCenter[1] << ", " << ringCenter[2] << "}" << endl;
    cout << "Difference in X = " << TMath::Abs(r.at(0) - pUnCorr.at(0)) << "; \t"
         << "Difference in Y = " << TMath::Abs(r.at(1) - pUnCorr.at(1)) << "; \t"
         << "Difference in Z = " << TMath::Abs(r.at(2) - pUnCorr.at(2)) << endl;
    fHM->H1("fhDifferenceXUncorrected")->Fill(TMath::Abs(r.at(0) - pUnCorr.at(0)));
    fHM->H1("fhDifferenceYUncorrected")->Fill(TMath::Abs(r.at(1) - pUnCorr.at(1)));
    distToExtrapTrackHitInPlaneUnCorr =
      TMath::Sqrt(TMath::Power(r.at(0) - pUnCorr.at(0), 2) + TMath::Power(r.at(1) - pUnCorr.at(1), 2));
    fHM->H1("fhDistanceUncorrected")->Fill(distToExtrapTrackHitInPlaneUnCorr);
    cout << "Distance between fitted ring center and extrapolated track hit in "
            "plane = "
         << distToExtrapTrackHitInPlaneUnCorr << endl;

    vector<Double_t> pIdeal(3);  // Absolute coordinates of fitted ring Center r and PMT extrapolated point p
    pIdeal.at(0) = TMath::Abs(outPosIdeal.x()), pIdeal.at(1) = TMath::Abs(outPosIdeal.y()),
    pIdeal.at(2) = TMath::Abs(outPosIdeal.z());
    //cout << "Ring center coordinates = {" << ringCenter[0] << ", " << ringCenter[1] << ", " << ringCenter[2] << "}" << endl;
    cout << "Difference in X = " << TMath::Abs(r.at(0) - pIdeal.at(0)) << "; \t"
         << "Difference in Y = " << TMath::Abs(r.at(1) - pIdeal.at(1)) << "; \t"
         << "Difference in Z = " << TMath::Abs(r.at(2) - pIdeal.at(2)) << endl;
    fHM->H1("fhDifferenceXIdeal")->Fill(TMath::Abs(r.at(0) - pIdeal.at(0)));
    fHM->H1("fhDifferenceYIdeal")->Fill(TMath::Abs(r.at(1) - pIdeal.at(1)));
    distToExtrapTrackHitInPlaneIdeal =
      TMath::Sqrt(TMath::Power(r.at(0) - pIdeal.at(0), 2) + TMath::Power(r.at(1) - pIdeal.at(1), 2));
    fHM->H1("fhDistanceIdeal")->Fill(distToExtrapTrackHitInPlaneIdeal);
    cout << "Distance between fitted ring center and extrapolated track hit in "
            "plane = "
         << distToExtrapTrackHitInPlaneIdeal << endl
         << endl;
    //}
    //else { cout << "No identical ring mother ID and mirror track ID ..." << endl;}
  }
  cout << "End of loop on global tracks;" << endl;
}

void CbmRichCorrection::DrawHistProjection()
{
  char leg[128];
  int colorInd = 1;

  TCanvas* can3 = new TCanvas(fRunTitle + "_Distance_Histos_" + fAxisRotTitle,
                              fRunTitle + "_Distance_Histos_" + fAxisRotTitle, 1500, 400);
  can3->SetGrid(1, 1);
  can3->Divide(2, 1);
  can3->cd(1)->SetGrid(1, 1);
  can3->cd(2)->SetGrid(1, 1);
  can3->cd(1);
  TH1D* Clone1 = (TH1D*) fHM->H1("fhDifferenceXIdeal")->Clone();
  Clone1->GetXaxis()->SetTitleSize(0.04);
  Clone1->GetYaxis()->SetTitleSize(0.04);
  Clone1->GetXaxis()->SetLabelSize(0.03);
  Clone1->GetYaxis()->SetLabelSize(0.03);
  Clone1->GetXaxis()->CenterTitle();
  Clone1->GetYaxis()->CenterTitle();
  Clone1->SetTitle("Difference in X");
  Clone1->SetLineColor(kBlue);
  Clone1->SetLineWidth(2);
  Clone1->Rebin(2);
  Clone1->Draw();
  TH1D* Clone2 = (TH1D*) fHM->H1("fhDifferenceX")->Clone();
  Clone2->SetTitleSize(0.04);
  Clone2->SetLabelSize(0.03);
  Clone2->SetLineColor(kGreen);
  Clone2->SetLineWidth(2);
  Clone2->Rebin(2);
  Clone2->Draw("same");
  TH1D* Clone3 = (TH1D*) fHM->H1("fhDifferenceXUncorrected")->Clone();
  Clone3->SetTitleSize(0.04);
  Clone3->SetLabelSize(0.03);
  Clone3->SetLineColor(kRed);
  Clone3->SetLineWidth(2);
  Clone3->Rebin(2);
  Clone3->Draw("same");
  gStyle->Clear();

  TLegend* LEG = new TLegend(0.3, 0.78, 0.5, 0.88);  // Set legend position
  LEG->SetBorderSize(1);
  LEG->SetFillColor(0);
  LEG->SetMargin(0.2);
  LEG->SetTextSize(0.02);
  sprintf(leg, "X diff uncorr");
  LEG->AddEntry(Clone3, leg, "l");
  sprintf(leg, "X diff corr");
  LEG->AddEntry(Clone2, leg, "l");
  sprintf(leg, "X diff ideal");
  LEG->AddEntry(Clone1, leg, "l");
  LEG->Draw();

  can3->cd(2);
  can3->SetGrid(1, 1);
  TH1D* Clone4 = (TH1D*) fHM->H1("fhDifferenceYIdeal")->Clone();
  Clone4->GetXaxis()->SetTitleSize(0.04);
  Clone4->GetYaxis()->SetTitleSize(0.04);
  Clone4->GetXaxis()->SetLabelSize(0.03);
  Clone4->GetYaxis()->SetLabelSize(0.03);
  Clone4->GetXaxis()->CenterTitle();
  Clone4->GetYaxis()->CenterTitle();
  Clone4->SetTitle("Difference in Y");
  Clone4->SetLineColor(kBlue);
  Clone4->SetLineWidth(2);
  Clone4->Rebin(2);
  Clone4->Draw();
  TH1D* Clone5 = (TH1D*) fHM->H1("fhDifferenceY")->Clone();
  Clone5->SetTitleSize(0.04);
  Clone5->SetLabelSize(0.03);
  Clone5->SetLineColor(kGreen);
  Clone5->SetLineWidth(2);
  Clone5->Rebin(2);
  Clone5->Draw("same");
  TH1D* Clone6 = (TH1D*) fHM->H1("fhDifferenceYUncorrected")->Clone();
  Clone6->SetTitleSize(0.04);
  Clone6->SetLabelSize(0.03);
  Clone6->SetLineColor(kRed);
  Clone6->SetLineWidth(2);
  Clone6->Rebin(2);
  Clone6->Draw("same");

  TLegend* LEG1 = new TLegend(0.3, 0.78, 0.5, 0.88);  // Set legend position
  LEG1->SetBorderSize(1);
  LEG1->SetFillColor(0);
  LEG1->SetMargin(0.2);
  LEG1->SetTextSize(0.02);
  sprintf(leg, "Y diff uncorr");
  LEG1->AddEntry(Clone6, leg, "l");
  sprintf(leg, "Y diff corr");
  LEG1->AddEntry(Clone5, leg, "l");
  sprintf(leg, "Y diff ideal");
  LEG1->AddEntry(Clone4, leg, "l");
  LEG1->Draw();

  /*can3->cd(3);
	//DrawH1(list_of(fHM->H1("fhDistanceCorrected"))(fHM->H1("fhDistanceUncorrected"))(fHM->H1("fhDistanceIdeal")), list_of("a corrected")("a uncorrected")("a ideal"), kLinear, kLog, true, 0.7, 0.7, 0.99, 0.99);
	TH1D* CloneDist1 = (TH1D*)fHM->H1("fhDistanceIdeal")->Clone();
	CloneDist1->GetXaxis()->SetTitleSize(0.04);
	CloneDist1->GetYaxis()->SetTitleSize(0.04);
	CloneDist1->GetXaxis()->SetLabelSize(0.03);
	CloneDist1->GetYaxis()->SetLabelSize(0.03);
	CloneDist1->GetXaxis()->CenterTitle();
	CloneDist1->GetYaxis()->CenterTitle();
	CloneDist1->SetTitle("Distance between extrapolated track center and fitted ring center");
	CloneDist1->SetLineColor(kBlue);
	CloneDist1->SetLineWidth(2);
	CloneDist1->Rebin(2);
	CloneDist1->Draw();
	TH1D* CloneDist2 = (TH1D*)fHM->H1("fhDistanceUncorrected")->Clone();
	CloneDist2->SetTitleSize(0.04);
	CloneDist2->SetTitleSize(0.04);
	CloneDist2->SetLineColor(kRed);
	CloneDist2->SetLineWidth(2);
	CloneDist2->Rebin(2);
	CloneDist2->Draw("same");
	TH1D* CloneDist3 = (TH1D*)fHM->H1("fhDistanceCorrected")->Clone();
	CloneDist3->SetTitleSize(0.04);
	CloneDist3->SetTitleSize(0.04);
	CloneDist3->SetLineColor(kGreen);
	CloneDist3->SetLineWidth(2);
	CloneDist3->Rebin(2);
	CloneDist3->Draw("same");

	TLegend* LEG2 = new TLegend(0.3,0.78,0.5,0.88); // Set legend position
	LEG2->SetBorderSize(1);
	LEG2->SetFillColor(0);
	LEG2->SetMargin(0.2);
	LEG2->SetTextSize(0.02);
	sprintf(leg, "Distance uncorr");
	LEG2->AddEntry(CloneDist2, leg, "l");
	sprintf(leg, "Distance corr");
	LEG2->AddEntry(CloneDist3, leg, "l");
	sprintf(leg, "Distance ideal");
	LEG2->AddEntry(CloneDist1, leg, "l");
	LEG2->Draw();*/
  gStyle->SetOptStat(000000);

  Cbm::SaveCanvasAsImage(can3, string(fOutputDir.Data()), "png");

  /*TCanvas* can4 = new TCanvas(fRunTitle + "_Difference_Fits_" + fAxisRotTitle, fRunTitle + "_Difference_Fits_" + fAxisRotTitle, 800, 800);
	int colorInd3 = 1;
	can4->Divide(2,2);
	can4->cd(1);
	//fHM->H1("fHistoDiffX")->Add(fHM->H1("fhDifferenceX"), fHM->H1("fhDifferenceXIdeal"), -1, 1);
	Clone2->GetXaxis()->SetTitleSize(0.04);
	Clone2->GetYaxis()->SetTitleSize(0.04);
	Clone2->GetXaxis()->SetLabelSize(0.03);
	Clone2->GetYaxis()->SetLabelSize(0.03);
	Clone2->GetXaxis()->CenterTitle();
	Clone2->GetYaxis()->CenterTitle();
	Clone2->SetTitle("Corrected difference in X");
	//Clone2->SetAxisRange(0., 0.6);
	Clone2->SetLineColor(kGreen);
	//Clone2->SetLineColor(colorInd3);
	//colorInd3++;
    Clone2->Draw();
    Clone2->Fit("gaus", "0", "", 0., 1.5);
    gStyle->SetOptFit(1111);
    TF1 *fit1 = Clone2->GetFunction("gaus");
    can4->cd(2);
	//Clone3->SetLineColor(colorInd3);
	//colorInd3++;
	//Clone1->SetAxisRange(0., 0.6);
    Clone1->SetTitle("Difference in X ideal");
	Clone1->Draw();
	Clone1->Fit("gaus", "0", "", 0., 1.5);
	gStyle->SetOptFit(1111);
	TF1 *fit2 = Clone1->GetFunction("gaus");

	can4->cd(3);
	Clone5->GetXaxis()->SetTitleSize(0.04);
	Clone5->GetYaxis()->SetTitleSize(0.04);
	Clone5->GetXaxis()->SetLabelSize(0.03);
	Clone5->GetYaxis()->SetLabelSize(0.03);
	Clone5->GetXaxis()->CenterTitle();
	Clone5->GetYaxis()->CenterTitle();
	Clone5->SetTitle("Corrected difference in Y");
	//Clone5->SetAxisRange(0., 0.6);
	//Clone5->SetLineColor(colorInd3);
	//colorInd3++;
    Clone5->Draw();
    Clone5->Fit("gaus", "0", "", 0., 1.5);
    gStyle->SetOptFit(1111);
    TF1 *fit3 = Clone5->GetFunction("gaus");
    can4->cd(4);
	//Clone4->SetAxisRange(0., 0.6);
	//Clone6->SetLineColor(colorInd3);
	//colorInd3++;
    Clone4->SetTitle("Difference in Y ideal");
	Clone4->Draw();
	Clone4->Fit("gaus", "0", "", 0., 1.5);
	gStyle->SetOptFit(1111);
	TF1 *fit4 = Clone4->GetFunction("gaus");

	//Cbm::SaveCanvasAsImage(can4, string(fOutputDir.Data()), "png");

    Double_t meanX_corr = fit1->GetParameter(1);
	Double_t meanX_ideal = fit2->GetParameter(1);
	cout << endl;
	cout << "Mean X corrected = " << meanX_corr << " and mean X ideal = " << meanX_ideal << endl;
	cout << "Fitted parameters of differenceX histo = " << fit1->GetParameter(0) << ", " << fit1->GetParameter(1) << " and " << fit1->GetParameter(2) << endl;
	cout << "Fitted parameters of differenceXIdeal histo = " << fit2->GetParameter(0) << ", " << fit2->GetParameter(1) << " and " << fit2->GetParameter(2) << endl;
	Double_t meanY_corr = fit3->GetParameter(1);
	Double_t meanY_ideal = fit4->GetParameter(1);
	cout << "Fitted parameters of differenceY histo = " << fit3->GetParameter(0) << ", " << fit3->GetParameter(1) << " and " << fit3->GetParameter(2) << endl;
	cout << "Fitted parameters of differenceYIdeal histo = " << fit4->GetParameter(0) << ", " << fit4->GetParameter(1) << " and " << fit4->GetParameter(2) << endl;
	cout << "Mean Y corrected = " << meanY_corr << " and mean Y ideal = " << meanY_ideal << endl;*/
}

void CbmRichCorrection::DrawHistFromFile(TString fileName)
{
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  fHM         = new CbmHistManager();
  TFile* file = new TFile(fileName, "READ");
  fHM->ReadFromFile(file);
  DrawHistProjection();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

void CbmRichCorrection::Finish()
{
  // ---------------------------------------------------------------------------------------------------------------------------------------- //
  // -------------------------------------------------- Mapping for mirror - PMT relations -------------------------------------------------- //
  // ---------------------------------------------------------------------------------------------------------------------------------------- //

  if (fDrawProjection) {
    DrawHistProjection();
    fHM->H1("fhDifferenceXUncorrected")->Write();
    fHM->H1("fhDifferenceYUncorrected")->Write();
    fHM->H1("fhDistanceUncorrected")->Write();
    fHM->H1("fhDifferenceX")->Write();
    fHM->H1("fhDifferenceY")->Write();
    fHM->H1("fhDistanceCorrected")->Write();
    fHM->H1("fhDifferenceXIdeal")->Write();
    fHM->H1("fhDifferenceYIdeal")->Write();
    fHM->H1("fhDistanceIdeal")->Write();
  }

  //cout << setprecision(6) << endl;
}
ClassImp(CbmRichCorrection)

  /*
vector<Double_t> CbmRichCorrection::RotateSphereCenter(vector<Double_t> ptM, vector<Double_t> ptC, TGeoNavigator* navi)
{
	vector<Double_t> ptCNew(3);
	Double_t cosPhi=0., sinPhi=0., cosTheta=0., sinTheta=0., diff[3], mat[3][3], invMat[3][3], corrMat[3][3], buff1[3][3], buff2[3][3];

	InvertMatrix(mat, invMat, navi);
	/*for (Int_t i=0; i<3; i++) {
		for (Int_t j=0; j<3; j++) {
			cout << invMat[i][j] << "\t";
		}
		cout << endl;
	}*/
  /*
	// Reading misalignment information from correction_param.txt text file.
	vector<Float_t> outputFit(4);
	ifstream corr_file;
	corr_file.open("correction_param.txt");
	if (corr_file.is_open())
	{
		for (Int_t i=0; i<4; i++) {corr_file >> outputFit.at(i);}
		corr_file.close();
	}
	else {
		cout << "Error in CbmRichCorrection: unable to open parameter file!" << endl << endl;
		sleep(5);
	}
	cout << "Misalignment parameters read from file = [" << outputFit.at(0) << " ; " << outputFit.at(1) << " ; " << outputFit.at(2) << " ; " << outputFit.at(3) << "]" << endl;

	// Calculating the new coordinates of sphere center C, according to the correction parameters.
	for (Int_t i=0; i<3; i++) {
		for (Int_t j=0; j<3; j++) {
			corrMat[i][j] = 0.;
			buff1[i][j] = 0.;
			buff2[i][j] = 0.;
		}
	}

	// Correction Matrix = Id(3).
	//corrMat[0][0] = corrMat[1][1] = corrMat[2][2] = 1;

	cosPhi = TMath::Cos(outputFit.at(0));
	sinPhi = TMath::Sin(outputFit.at(0));		// BEWARE !!! the horizontal rotation axis of the tile is opposite to the global horizontal rotation axis !!!
	cosTheta = TMath::Cos(outputFit.at(1));
	sinTheta = -1*TMath::Sin(outputFit.at(1));

	Double_t RotX[3][3], RotY[3][3], RotZ[3][3];
	// Define rotation around X axis, with y towards z.
	// y towards z is the rotation direction defined in the tile frame, which is the same definition as for the global
	// frame. But as the X axis direction is opposite in the two frames, to correct for misalignment, the rotation
	// direction remains unchanged in the global frame.
	RotX[0][0] = 1;
	RotX[0][1] = 0;
	RotX[0][2] = 0;
	RotX[1][0] = 0;
	RotX[1][1] = cosPhi;
	RotX[1][2] = -1*sinPhi;
	RotX[2][0] = 0;
	RotX[2][1] = sinPhi;
	RotX[2][2] = cosPhi;
	// Define rotation around Y axis, with z towards x.
	// x towards z is the rotation direction defined in the tile frame, which is the same definition as for the global
	// frame. And as the tile and global frames are identical, the rotation direction for the correction is the opposite
	// in the global frame - in order to correct for the mibuff3[i][j] = 0.;salignment.
	RotY[0][0] = cosTheta;
	RotY[0][1] = 0;
	RotY[0][2] = sinTheta;
	RotY[1][0] = 0;
	RotY[1][1] = 1;
	RotY[1][2] = 0;
	RotY[2][0] = -1*sinTheta;
	RotY[2][1] = 0;
	RotY[2][2] = cosTheta;
	// Define rotation around Z axis.
	RotZ[0][0] = cosTheta;
	RotZ[0][1] = -1*sinTheta;
	RotZ[0][2] = 0;
	RotZ[1][0] = sinTheta;
	RotZ[1][1] = cosTheta;
	RotZ[1][2] = 0;
	RotZ[2][0] = 0;
	RotZ[2][1] = 0;
	RotZ[2][2] = 1;
	cout << "RotY[0][0] = " << RotY[0][0] << " and RotY[0][2] = " << RotY[0][2] << endl;

	for (Int_t i=0; i<3; i++) {
		for (Int_t j=0; j<3; j++) {
			//corrMat[i][j] = RotZ[i][j];
			for (Int_t k=0; k<3; k++) {
				corrMat[i][j] = RotY[i][k]*RotX[k][j] + corrMat[i][j];
			}
		}
	}

/*	corrMat[0][0] = cosTheta;
	corrMat[0][1] = -1*sinTheta*sinPhi;
	corrMat[0][2] = -1*sinTheta*cosPhi;
	corrMat[1][0] = 0;
	corrMat[1][1] = cosPhi;
	corrMat[1][2] = -1*sinPhi;
	corrMat[2][0] = sinTheta;
	corrMat[2][1] = cosPhi;
	corrMat[2][2] = cosTheta*cosPhi;
	corrMat[0][0] = 1;
	corrMat[0][1] = 0;
	corrMat[0][2] = 0;
	corrMat[1][0] = 0;
	corrMat[1][1] = cosPhi;
	corrMat[1][2] = -1*sinPhi;
	corrMat[2][0] = 0;
	corrMat[2][1] = sinPhi;
	corrMat[2][2] = cosPhi;
	corrMat[0][0] = cosTheta;
	corrMat[0][1] = 0;
	corrMat[0][2] = -1*sinTheta;
	corrMat[1][0] = -1*sinTheta*sinPhi;
	corrMat[1][1] = cosPhi;
	corrMat[1][2] = -1*sinPhi*cosTheta;
	corrMat[2][0] = cosPhi*sinTheta;
	corrMat[2][1] = sinPhi;
	corrMat[2][2] = cosTheta*cosPhi;

	for (Int_t i=0; i<3; i++) {
		for (Int_t j=0; j<3; j++) {
			for (Int_t k=0; k<3; k++) {
				buff1[i][j] = corrMat[i][k]*invMat[k][j] + buff1[i][j];
			}
		}
	}
	for (Int_t i=0; i<3; i++) {
		for (Int_t j=0; j<3; j++) {
			for (Int_t k=0; k<3; k++) {
				buff2[i][j] = mat[i][k]*buff1[k][j] + buff2[i][j];
			}
		}
	}

	diff[0] = ptC.at(0) - ptM.at(0);
	diff[1] = ptC.at(1) - ptM.at(1);
	diff[2] = ptC.at(2) - ptM.at(2);
	cout << "diff = {" << diff[0] << ", " << diff[1] << ", " << diff[2] << "}" << endl;
	Double_t phi2 =TMath::ATan2(diff[1], -1*diff[2]), theta2 =TMath::ATan2(diff[0], -1*diff[2]);
	cout << "Calculated phi (= arctan(y/-z)): " << TMath::RadToDeg()*phi2 << " and calculated theta (= arctan(x/-z)): " << TMath::RadToDeg()*theta2 << endl;
	cout << "cosPhi = " << TMath::Cos(phi2) << ", sinPhi = " << TMath::Sin(phi2) << " and cosTheta = " << TMath::Cos(theta2) << ", sinTheta = " << TMath::Sin(theta2) << endl;
	Double_t RotX2[3][3], RotY2[3][3], corrMat2[3][3];
	RotX2[0][0] = 1;
	RotX2[0][1] = 0;
	RotX2[0][2] = 0;
	RotX2[1][0] = 0;
	RotX2[1][1] = TMath::Cos(phi2);
	RotX2[1][2] = TMath::Sin(phi2);
	RotX2[2][0] = 0;
	RotX2[2][1] = -1*TMath::Sin(phi2);
	RotX2[2][2] = TMath::Cos(phi2);
	RotY2[0][0] = TMath::Cos(theta2);
	RotY2[0][1] = 0;
	RotY2[0][2] = TMath::Sin(theta2);
	RotY2[1][0] = 0;
	RotY2[1][1] = 1;
	RotY2[1][2] = 0;
	RotY2[2][0] = -1*TMath::Sin(theta2);
	RotY2[2][1] = 0;
	RotY2[2][2] = TMath::Cos(theta2);

	for (Int_t i=0; i<3; i++) {
			for (Int_t j=0; j<3; j++) {
				for (Int_t k=0; k<3; k++) {
					corrMat2[i][j] = RotX2[i][k]*RotY2[k][j] + corrMat2[i][j];
				}
			}
	}

	vector<Double_t> ptCNew2(3), ptCNew3(3);
	for (Int_t i=0; i<3; i++) {
		for (Int_t j=0; j<3; j++) {
			//ptCNew.at(i) = buff2[i][j]*diff[j] + ptCNew.at(i);
			//ptCNew.at(i) = invMat[i][j]*diff[j] + ptCNew.at(i);
			ptCNew.at(i) = corrMat2[i][j]*diff[j] + ptCNew.at(i);
			ptCNew2.at(i) = RotY[i][j]*diff[j] + ptCNew2.at(i);
		}
	}

	phi2 = TMath::ATan2(ptCNew2.at(1), -ptCNew2.at(2));
	cout << "New phi2 = " << TMath::RadToDeg()*phi2 << endl;
	RotX2[0][0] = 1;
	RotX2[0][1] = 0;
	RotX2[0][2] = 0;
	RotX2[1][0] = 0;
	RotX2[1][1] = TMath::Cos(phi2);
	RotX2[1][2] = TMath::Sin(phi2);
	RotX2[2][0] = 0;
	RotX2[2][1] = -1*TMath::Sin(phi2);
	RotX2[2][2] = TMath::Cos(phi2);
	for (Int_t i=0; i<3; i++) {
		for (Int_t j=0; j<3; j++) {
			ptCNew3.at(i) = RotX[i][j]*diff[j] + ptCNew3.at(i);
		}
	}

	//ptCNew.at(0) += ptM.at(0);
	//ptCNew.at(1) += ptM.at(1);
	//ptCNew.at(2) += ptM.at(2);

	/*ptCNew.at(0) = ptM.at(0) + diffX*cosTheta - diffY*sinTheta*sinPhi - diffZ*sinTheta*cosPhi;
	ptCNew.at(1) = ptM.at(1) + diffY*cosPhi - diffZ*sinPhi;
	ptCNew.at(2) = ptM.at(2) + diffX*sinTheta + diffY*cosPhi + diffZ*cosTheta*cosPhi;*/
  /*	cout << "Sphere center coordinates of the rotated mirror tile, w/ calculation, = {" << ptCNew.at(0) << ", " << ptCNew.at(1) << ", " << ptCNew.at(2) << "}" << endl;
	cout << "Sphere center coordinates of the rotated mirror tile (after rotation around Y), w/ calculation, = {" << ptCNew2.at(0) << ", " << ptCNew2.at(1) << ", " << ptCNew2.at(2) << "}" << endl;
	cout << "Sphere center coordinates of the rotated mirror tile (around unrotated axes), w/ calculation, = {" << ptCNew3.at(0) << ", " << ptCNew3.at(1) << ", " << ptCNew3.at(2) << "}" << endl << endl;

	return ptCNew;
}
*/
