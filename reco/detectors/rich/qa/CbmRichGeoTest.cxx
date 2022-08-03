/* Copyright (C) 2009-2021 UGiessen/JINR-LIT, Giessen/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev, Semen Lebedev [committer] */

/**
 * \file CbmRichGeoTest.cxx
 *
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 **/

#include "CbmRichGeoTest.h"

#include "CbmDigiManager.h"
#include "CbmDrawHist.h"
#include "CbmHistManager.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmReport.h"
#include "CbmRichConverter.h"
#include "CbmRichDetectorData.h"
#include "CbmRichDigi.h"
#include "CbmRichDigiMapManager.h"
#include "CbmRichDraw.h"
#include "CbmRichGeoManager.h"
#include "CbmRichHit.h"
#include "CbmRichPmt.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmRichRingFitterCOP.h"
#include "CbmRichRingFitterEllipseTau.h"
#include "CbmStudyReport.h"
#include "CbmTrackMatchNew.h"
#include "CbmUtils.h"

#include "FairGeoNode.h"
#include "FairGeoTransform.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TEllipse.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMCProcess.h"
#include "TMath.h"
#include "TPad.h"
#include "TStyle.h"
#include "TSystem.h"
#include <TFile.h>

#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace Cbm;

CbmRichGeoTest::CbmRichGeoTest() : FairTask("RichGeoTestQa") {}

CbmRichGeoTest::~CbmRichGeoTest() {}

InitStatus CbmRichGeoTest::Init()
{
  fMcTracks           = InitOrFatalMc("MCTrack", "CbmRichGeoTest::Init");
  fRichPoints         = InitOrFatalMc("RichPoint", "CbmRichGeoTest::Init");
  fRichRefPlanePoints = InitOrFatalMc("RefPlanePoint", "CbmRichGeoTest::Init");
  fRichHits           = GetOrFatal<TClonesArray>("RichHit", "CbmRichUrqmdTest::Init");
  fRichRings          = GetOrFatal<TClonesArray>("RichRing", "CbmRichUrqmdTest::Init");
  fRichRingMatches    = GetOrFatal<TClonesArray>("RichRingMatch", "CbmRichUrqmdTest::Init");
  fEventList          = GetOrFatal<CbmMCEventList>("MCEventList.", "CbmRichUrqmdTest::Init");

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();
  if (!fDigiMan->IsPresent(ECbmModuleId::kRich)) { LOG(fatal) << "CbmRichGeoTest::Init No RichDigi!"; }
  if (!fDigiMan->IsMatchPresent(ECbmModuleId::kRich)) { LOG(fatal) << "CbmRichGeoTest::Init No RichDigiMatch!"; }

  fCopFit = new CbmRichRingFitterCOP();
  fTauFit = new CbmRichRingFitterEllipseTau();

  InitHistograms();

  return kSUCCESS;
}

void CbmRichGeoTest::Exec(Option_t* /*option*/)
{
  fEventNum++;
  cout << "CbmRichGeoTest, event No. " << fEventNum << endl;

  ProcessMc();
  RingParameters();
  ProcessHits();
}

void CbmRichGeoTest::InitHistograms()
{
  double xMin = -120.;
  double xMax = 120.;
  int nBinsX  = 240;
  double yMin = -210;
  double yMax = 210.;
  int nBinsY  = 420;

  fHM = new CbmHistManager();

  fHM->Create2<TH2D>("fhHitsXY", "fhHitsXY;X [cm];Y [cm];Counter", nBinsX, xMin, xMax, nBinsY, yMin, yMax);
  fHM->Create2<TH2D>("fhPointsXY", "fhPointsXY;X [cm];Y [cm];Counter", nBinsX, xMin, xMax, nBinsY, yMin, yMax);
  fHM->Create2<TH2D>("fhPointsXYNoRotation", "fhPointsXYNoRotation;X [cm];Y [cm];Counter", nBinsX, xMin, xMax, nBinsY,
                     yMin, yMax);
  fHM->Create1<TH1D>("fhHitsZ", "fhHitsZ;Z [cm];Yield", 150, 100, 250);
  fHM->Create1<TH1D>("fhPointsZ", "fhPointsZ;Z [cm];Yield", 150, 100, 250);
  fHM->Create1<TH1D>("fhMcVertexZEl", "fhMcVertexZEl;Z [cm];Counter", 250, -100., 150);
  fHM->Create2<TH2D>("fhMcVertexXYEl", "fhMcVertexXYEl;X [cm];Y [cm];Counter", 50, -5., 5., 50, -5., 5.);

  vector<string> hp = {"_hits", "_points"};
  for (size_t i = 0; i < hp.size(); i++) {
    string t = hp[i];
    if (i == 0) fHM->Create1<TH1D>("fhNofHits" + t, "fhNofHits" + t + ";# hits/ring;Yield", 50, -.5, 49.5);
    if (i == 1) fHM->Create1<TH1D>("fhNofHits" + t, "fhNofHits" + t + ";# points/ring;Yield", 400, -.5, 399.5);
    // ellipse fitting parameters
    fHM->Create2<TH2D>("fhBoAVsMom" + t, "fhBoAVsMom" + t + ";P [GeV/c];B/A;Yield", 40, 0., 10, 100, 0, 1);
    fHM->Create2<TH2D>("fhXcYcEllipse" + t, "fhXcYcEllipse" + t + ";X [cm];Y [cm];Yield", nBinsX, xMin, xMax, nBinsY,
                       yMin, yMax);
    fHM->Create2<TH2D>("fhBaxisVsMom" + t, "fhBaxisVsMom" + t + ";P [GeV/c];B axis [cm];Yield", 40, 0., 10, 200, 0.,
                       10.);
    fHM->Create2<TH2D>("fhAaxisVsMom" + t, "fhAaxisVsMom" + t + ";P [GeV/c];A axis [cm];Yield", 40, 0., 10, 200, 0.,
                       10.);
    fHM->Create2<TH2D>("fhChi2EllipseVsMom" + t, "fhChi2EllipseVsMom" + t + ";P [GeV/c];#Chi^{2};Yield", 40, 0., 10.,
                       50, 0., 0.5);
    // circle fitting parameters
    fHM->Create2<TH2D>("fhXcYcCircle" + t, "fhXcYcCircle" + t + ";X [cm];Y [cm];Yield", nBinsX, xMin, xMax, nBinsY,
                       yMin, yMax);
    fHM->Create2<TH2D>("fhRadiusVsMom" + t, "fhRadiusVsMom" + t + ";P [GeV/c];Radius [cm];Yield", 40, 0., 10, 200, 0.,
                       10.);
    fHM->Create2<TH2D>("fhChi2CircleVsMom" + t, "fhChi2CircleVsMom" + t + ";P [GeV/c];#Chi^{2};Yield", 40, 0., 10., 50,
                       0., .5);
    fHM->Create2<TH2D>("fhDRVsMom" + t, "fhDRVsMom" + t + ";P [GeV/c];dR [cm];Yield", 40, 0, 10, 200, -2., 2.);

    fHM->Create1<TH1D>("fhBaxisUpHalf" + t, "fhBaxisUpHalf" + t + ";B axis [cm];Yield", 200, 0., 10.);
    fHM->Create1<TH1D>("fhBaxisDownHalf" + t, "fhBaxisDownHalf" + t + ";B axis [cm];Yield", 200, 0., 10.);
  }

  fHM->Create1<TH1D>("fhNofPhotonsPerHit", "fhNofPhotonsPerHit;Number of photons per hit;Yield", 10, -0.5, 9.5);

  // Difference between Mc Points and Hits fitting.
  fHM->Create2<TH2D>("fhDiffAaxis", "fhDiffAaxis;# hits/ring;A_{point}-A_{hit} [cm];Yield", 25, 0., 50., 100, -1., 1.);
  fHM->Create2<TH2D>("fhDiffBaxis", "fhDiffBaxis;# hits/ring;B_{point}-B_{hit} [cm];Yield", 25, 0., 50., 100, -1., 1.);
  fHM->Create2<TH2D>("fhDiffXcEllipse", "fhDiffXcEllipse;# hits/ring;Xc_{point}-Xc_{hit} [cm];Yield", 25, 0., 50., 100,
                     -1., 1.);
  fHM->Create2<TH2D>("fhDiffYcEllipse", "fhDiffYcEllipse;# hits/ring;Yc_{point}-Yc_{hit} [cm];Yield", 25, 0., 50., 100,
                     -1., 1.);
  fHM->Create2<TH2D>("fhDiffXcCircle", "fhDiffXcCircle;# hits/ring;Xc_{point}-Xc_{hit} [cm];Yield", 25, 0., 50., 100,
                     -1., 1.);
  fHM->Create2<TH2D>("fhDiffYcCircle", "fhDiffYcCircle;# hits/ring;Yc_{point}-Yc_{hit} [cm];Yield", 25, 0., 50., 100,
                     -1., 1.);
  fHM->Create2<TH2D>("fhDiffRadius", "fhDiffRadius;# hits/ring;Radius_{point}-Radius_{hit} [cm];Yield", 25, 0., 50.,
                     100, -1., 1.);

  // R, A, B distribution for different number of hits from 0 to 40.
  fHM->Create2<TH2D>("fhRadiusVsNofHits", "fhRadiusVsNofHits;# hits/ring;Radius [cm];Yield", 40, 0., 40., 100, 0., 10.);
  fHM->Create2<TH2D>("fhAaxisVsNofHits", "fhAaxisVsNofHits;# hits/ring;A axis [cm];Yield", 40, 0., 40., 100, 0., 10.);
  fHM->Create2<TH2D>("fhBaxisVsNofHits", "fhBaxisVsNofHits;# hits/ring;B axis [cm];Yield", 40, 0., 40., 100, 0., 10.);
  fHM->Create2<TH2D>("fhDRVsNofHits", "fhDRVsNofHits;# hits/ring;dR [cm];Yield", 40, 0., 40., 200, -2., 2.);

  // Hits and points.
  fHM->Create1<TH1D>("fhDiffXhit", "fhDiffXhit;X_{point}-X_{hit} [cm];Yield", 200, -.5, .5);
  fHM->Create1<TH1D>("fhDiffYhit", "fhDiffYhit;Y_{point}-Y_{hit} [cm];Yield", 200, -.5, .5);

  // Fitting efficiency.
  fHM->Create1<TH1D>("fhNofHitsAll", "fhNofHitsAll;# hits/ring;Yield", 50, 0., 50.);
  fHM->Create1<TH1D>("fhNofHitsCircleFit", "fhNofHitsCircleFit;# hits/ring;Yield", 50, 0., 50.);
  fHM->Create1<TH1D>("fhNofHitsEllipseFit", "fhNofHitsEllipseFit;# hits/ring;Yield", 50, 0., 50.);
  fHM->Create1<TH1D>("fhNofHitsCircleFitEff", "fhNofHitsCircleFitEff;# hits/ring;Efficiency [%]", 50, 0., 50.);
  fHM->Create1<TH1D>("fhNofHitsEllipseFitEff", "fhNofHitsEllipseFitEff;# hits/ring;Efficiency [%]", 50, 0., 50.);

  // Detector acceptance efficiency 2D:Pt-Rapidity, 2D:P-Rapidity, 1D:P
  vector<string> mcAccTypes {"Mc", "Acc"};
  for (const string& t : mcAccTypes) {
    fHM->Create1<TH1D>("fhMomEl" + t, "fhMomEl" + t + ";p [GeV/c];Yield", 50, 0., 10.);
    fHM->Create2<TH2D>("fhPtYEl" + t, "fhPtYEl" + t + ";Rapidity;P_{t} [GeV/c];Yield", 25, 0., 4., 25, 0., 3.);
    fHM->Create2<TH2D>("fhPYEl" + t, "fhPYEl" + t + ";Rapidity;P [GeV/c];Yield", 25, 0., 4., 25, 0., 10.);
  }

  // Numbers in dependence on XY position onto the photodetector.
  fHM->Create3<TH3D>("fhNofHitsXYZ", "fhNofHitsXYZ;X [cm];Y [cm];# hits/ring", nBinsX, xMin, xMax, nBinsY, yMin, yMax,
                     50, 0., 50);
  fHM->Create3<TH3D>("fhNofPointsXYZ", "fhNofPointsXYZ;X [cm];Y [cm];# points/ring", nBinsX, xMin, xMax, nBinsY, yMin,
                     yMax, 50, 100., 300.);
  fHM->Create3<TH3D>("fhBoAXYZ", "fhBoAXYZ;X [cm];Y [cm];B/A", nBinsX, xMin, xMax, nBinsY, yMin, yMax, 100, 0., 1.);
  fHM->Create3<TH3D>("fhBaxisXYZ", "fhBaxisXYZ;X [cm];Y [cm];B axis [cm]", nBinsX, xMin, xMax, nBinsY, yMin, yMax, 80,
                     3., 7.);
  fHM->Create3<TH3D>("fhAaxisXYZ", "fhAaxisXYZ;X [cm];Y [cm];A axis [cm]", nBinsX, xMin, xMax, nBinsY, yMin, yMax, 80,
                     3., 7.);
  fHM->Create3<TH3D>("fhRadiusXYZ", "fhRadiusXYZ;X [cm];Y [cm];Radius [cm]", nBinsX, xMin, xMax, nBinsY, yMin, yMax, 80,
                     3., 7.);
  fHM->Create3<TH3D>("fhdRXYZ", "fhdRXYZ;X [cm];Y [cm];dR [cm]", nBinsX, xMin, xMax, nBinsY, yMin, yMax, 100, -1., 1.);

  int nBinsX1 = 60;
  int xMin1   = -120;
  int xMax1   = 120;
  int nBinsY1 = 25;
  int yMin1   = 100;
  int yMax1   = 200;
  // Numbers in dependence X or Y position onto the photodetector plane
  fHM->Create2<TH2D>("fhNofHitsVsX", "fhNofHitsVsX;X [cm];# hits/ring", nBinsX1, xMin1, xMax1, 50, 0., 50);
  fHM->Create2<TH2D>("fhNofHitsVsY", "fhNofHitsVsY;Abs(Y) [cm];# hits/ring", nBinsY1, yMin1, yMax1, 50, 0., 50);

  fHM->Create2<TH2D>("fhNofPointsVsX", "fhNofPointsVsX;X [cm];# points/ring", nBinsX1, xMin1, xMax1, 50, 100., 300.);
  fHM->Create2<TH2D>("fhNofPointsVsY", "fhNofPointsVsY;Abs(Y) [cm];# points/ring", nBinsY1, yMin1, yMax1, 50, 100.,
                     300.);

  fHM->Create2<TH2D>("fhBoAVsX", "fhBoAVsX;X [cm];B/A", nBinsX1, xMin1, xMax1, 100, 0., 1.);
  fHM->Create2<TH2D>("fhBoAVsY", "fhBoAVsY;Abs(Y) [cm];B/A", nBinsY1, yMin1, yMax1, 100, 0., 1.);

  fHM->Create2<TH2D>("fhBaxisVsX", "fhBaxisVsX;X [cm];B axis [cm]", nBinsX1, xMin1, xMax1, 80, 3., 7.);
  fHM->Create2<TH2D>("fhBaxisVsY", "fhBaxisVsY;Abs(Y) [cm];B axis [cm]", nBinsY1, yMin1, yMax1, 80, 3., 7.);

  fHM->Create2<TH2D>("fhAaxisVsX", "fhAaxisVsX;X [cm];A axis [cm]", nBinsX1, xMin1, xMax1, 80, 3., 7.);
  fHM->Create2<TH2D>("fhAaxisVsY", "fhAaxisVsY;Abs(Y) [cm];A axis [cm]", nBinsY1, yMin1, yMax1, 80, 3., 7.);

  fHM->Create2<TH2D>("fhRadiusVsX", "fhRadiusVsX;X [cm];Radius [cm]", nBinsX1, xMin1, xMax1, 80, 3., 7.);
  fHM->Create2<TH2D>("fhRadiusVsY", "fhRadiusVsY;Abs(Y) [cm];Radius [cm]", nBinsY1, yMin1, yMax1, 80, 3., 7.);

  fHM->Create2<TH2D>("fhdRVsX", "fhdRVsX;X [cm];dR [cm]", nBinsX1, xMin1, xMax1, 100, -1., 1.);
  fHM->Create2<TH2D>("fhdRVsY", "fhdRVsY;Abs(Y) [cm];dR [cm]", nBinsY1, yMin1, yMax1, 100, -1., 1.);

  //Photon energy and wevelength
  vector<string> photonECat = {"PlaneZ+", "PlaneZ-", "PmtPoint", "PmtHit"};
  for (const auto& t : photonECat) {
    fHM->Create1<TH1D>("fhPhotonE" + t, "fhPhotonE" + t + ";Photon energy [eV];Counter", 100, 0., 10.);
    fHM->Create1<TH1D>("fhLambda" + t, "fhLambda" + t + ";Photon wavelength [nm];Counter", 100, 0., 700.);
  }
}

void CbmRichGeoTest::ProcessMc()
{
  size_t nofEvents = fEventList->GetNofEvents();
  for (size_t iE = 0; iE < nofEvents; iE++) {
    int fileId  = fEventList->GetFileIdByIndex(iE);
    int eventId = fEventList->GetEventIdByIndex(iE);

    int nofMcTracks = fMcTracks->Size(fileId, eventId);
    for (int iT = 0; iT < nofMcTracks; iT++) {
      const CbmMCTrack* mcTrack = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, iT));
      if (mcTrack == nullptr) continue;
      int motherId = mcTrack->GetMotherId();
      int pdgAbs   = std::abs(mcTrack->GetPdgCode());
      bool isMcPrimaryElectron =
        (pdgAbs == 11 && motherId == -1) || (mcTrack->GetGeantProcessId() == kPPrimary && pdgAbs == 11);

      if (isMcPrimaryElectron) {
        fHM->H1("fhMomElMc")->Fill(mcTrack->GetP());
        fHM->H2("fhPtYElMc")->Fill(mcTrack->GetRapidity(), mcTrack->GetPt());
        fHM->H2("fhPYElMc")->Fill(mcTrack->GetRapidity(), mcTrack->GetP());
        TVector3 v;
        mcTrack->GetStartVertex(v);
        fHM->H1("fhMcVertexZEl")->Fill(v.Z());
        fHM->H1("fhMcVertexXYEl")->Fill(v.X(), v.Y());
      }

      if (pdgAbs == 211 && motherId == -1) {
        fHM->H1("fhMcMomPi")->Fill(mcTrack->GetP());
        fHM->H2("fhMcPtyPi")->Fill(mcTrack->GetRapidity(), mcTrack->GetPt());
      }
    }

    int nofPoints = fRichPoints->Size(fileId, eventId);
    for (int iP = 0; iP < nofPoints; iP++) {
      const CbmRichPoint* point = static_cast<CbmRichPoint*>(fRichPoints->Get(fileId, eventId, iP));
      if (point == nullptr) continue;
      TVector3 inPos(point->GetX(), point->GetY(), point->GetZ());
      TVector3 outPos;
      CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);
      fHM->H2("fhPointsXYNoRotation")->Fill(point->GetX(), point->GetY());
      fHM->H2("fhPointsXY")->Fill(outPos.X(), outPos.Y());
      fHM->H1("fhPointsZ")->Fill(point->GetZ());

      TVector3 mom;
      point->Momentum(mom);
      double momMag = mom.Mag();
      fHM->H1("fhPhotonEPmtPoint")->Fill(1.e9 * momMag);
      double lambda = CbmRichPmt::getLambda(momMag);
      fHM->H1("fhLambdaPmtPoint")->Fill(lambda);
    }

    int nofPlanePoints = fRichRefPlanePoints->Size(fileId, eventId);
    for (int iP = 0; iP < nofPlanePoints; iP++) {
      const CbmRichPoint* point = static_cast<CbmRichPoint*>(fRichRefPlanePoints->Get(fileId, eventId, iP));
      if (point == nullptr) continue;
      TVector3 mom;
      point->Momentum(mom);
      double momMag = mom.Mag();
      double lambda = CbmRichPmt::getLambda(momMag);
      if (point->GetPz() > 0) {
        fHM->H1("fhPhotonEPlaneZ+")->Fill(1.e9 * momMag);
        fHM->H1("fhLambdaPlaneZ+")->Fill(lambda);
      }
      else {
        fHM->H1("fhPhotonEPlaneZ-")->Fill(1.e9 * momMag);
        fHM->H1("fhLambdaPlaneZ-")->Fill(lambda);
      }
    }
  }
}

CbmRichRingLight CbmRichGeoTest::CreateRingLightWithPoints(int fileId, int mcEventId, int mcTrackId)
{
  CbmRichRingLight ringPoint;
  int nofRichPoints = fRichPoints->Size(fileId, mcEventId);
  for (int iPoint = 0; iPoint < nofRichPoints; iPoint++) {
    const CbmRichPoint* richPoint = static_cast<CbmRichPoint*>(fRichPoints->Get(fileId, mcEventId, iPoint));
    if (richPoint == nullptr) continue;
    int trackId = richPoint->GetTrackID();
    if (trackId < 0) continue;
    const CbmMCTrack* mcTrackRich = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, mcEventId, trackId));
    if (mcTrackRich == nullptr) continue;
    int motherIdRich = mcTrackRich->GetMotherId();
    if (motherIdRich == mcTrackId) {
      TVector3 posPoint;
      richPoint->Position(posPoint);
      TVector3 detPoint;
      CbmRichGeoManager::GetInstance().RotatePoint(&posPoint, &detPoint);
      CbmRichHitLight hit(detPoint.X(), detPoint.Y());
      ringPoint.AddHit(hit);
    }
  }
  return ringPoint;
}

void CbmRichGeoTest::RingParameters()
{
  int fileId   = 0;
  int nofRings = fRichRings->GetEntriesFast();
  for (int iR = 0; iR < nofRings; iR++) {
    const CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(iR));
    if (ring == nullptr) continue;
    const CbmTrackMatchNew* ringMatch = static_cast<CbmTrackMatchNew*>(fRichRingMatches->At(iR));
    if (ringMatch == nullptr) continue;
    int mcEventId             = ringMatch->GetMatchedLink().GetEntry();
    int mcTrackId             = ringMatch->GetMatchedLink().GetIndex();
    const CbmMCTrack* mcTrack = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, mcEventId, mcTrackId));
    if (mcTrack == nullptr) continue;
    int motherId    = mcTrack->GetMotherId();
    int pdgAbs      = std::abs(mcTrack->GetPdgCode());
    double mom      = mcTrack->GetP();
    double pt       = mcTrack->GetPt();
    double rapidity = mcTrack->GetRapidity();
    bool isMcPrimaryElectron =
      (pdgAbs == 11 && motherId == -1) || (mcTrack->GetGeantProcessId() == kPPrimary && pdgAbs == 11);

    if (ring->GetNofHits() >= fMinNofHits) {
      if (isMcPrimaryElectron) {
        fHM->H1("fhMomElAcc")->Fill(mom);
        fHM->H2("fhPtYElAcc")->Fill(rapidity, pt);
        fHM->H2("fhPYElAcc")->Fill(rapidity, mom);
      }
    }

    if (!isMcPrimaryElectron) continue;  // only primary electrons

    CbmRichRingLight ringPoint = CreateRingLightWithPoints(fileId, mcEventId, mcTrackId);
    fHM->H1("fhNofHitsAll")->Fill(ring->GetNofHits());

    CbmRichRingLight ringHit;
    CbmRichConverter::CopyHitsToRingLight(ring, &ringHit);

    FitAndFillHistCircle(0, &ringHit, mom);    //hits
    FitAndFillHistCircle(1, &ringPoint, mom);  // points
    FillMcVsHitFitCircle(&ringHit, &ringPoint);

    double r  = ringHit.GetRadius();
    double xc = ringHit.GetCenterX();
    double yc = ringHit.GetCenterY();

    if (ringHit.GetRadius() > fMinRadius && ringHit.GetRadius() < fMaxRadius) {
      fHM->H1("fhNofHitsCircleFit")->Fill(ringHit.GetNofHits());
    }
    if (fDrawEventDisplay && fNofDrawnRings < 10) { DrawRing(&ringHit, &ringPoint); }

    FitAndFillHistEllipse(0, &ringHit, mom);    // hits
    FitAndFillHistEllipse(1, &ringPoint, mom);  // points
    FillMcVsHitFitEllipse(&ringHit, &ringPoint);

    if (ringHit.GetAaxis() > fMinAaxis && ringHit.GetAaxis() < fMaxAaxis && ringHit.GetBaxis() > fMinBaxis
        && ringHit.GetAaxis() < fMaxBaxis) {
      fHM->H1("fhNofHitsEllipseFit")->Fill(ringHit.GetNofHits());

      double np = ringPoint.GetNofHits();
      double a  = ringHit.GetAaxis();
      double b  = ringHit.GetBaxis();
      double nh = ring->GetNofHits();

      fHM->H3("fhNofHitsXYZ")->Fill(xc, yc, nh);
      fHM->H3("fhNofPointsXYZ")->Fill(xc, yc, np);
      fHM->H3("fhBoAXYZ")->Fill(xc, yc, b / a);
      fHM->H3("fhBaxisXYZ")->Fill(xc, yc, b);
      fHM->H3("fhAaxisXYZ")->Fill(xc, yc, a);
      fHM->H3("fhRadiusXYZ")->Fill(xc, yc, r);

      fHM->H2("fhNofHitsVsX")->Fill(xc, nh);
      fHM->H2("fhNofPointsVsX")->Fill(xc, np);
      fHM->H2("fhBoAVsX")->Fill(xc, b / a);
      fHM->H2("fhBaxisVsX")->Fill(xc, b);
      fHM->H2("fhAaxisVsX")->Fill(xc, a);
      fHM->H2("fhRadiusVsX")->Fill(xc, r);

      fHM->H2("fhNofHitsVsY")->Fill(abs(yc), nh);
      fHM->H2("fhNofPointsVsY")->Fill(abs(yc), np);
      fHM->H2("fhBoAVsY")->Fill(abs(yc), b / a);
      fHM->H2("fhBaxisVsY")->Fill(abs(yc), b);
      fHM->H2("fhAaxisVsY")->Fill(abs(yc), a);
      fHM->H2("fhRadiusVsY")->Fill(abs(yc), r);

      for (int iH = 0; iH < ringHit.GetNofHits(); iH++) {
        double xh = ringHit.GetHit(iH).fX;
        double yh = ringHit.GetHit(iH).fY;
        double dr = r - sqrt((xc - xh) * (xc - xh) + (yc - yh) * (yc - yh));
        fHM->H3("fhdRXYZ")->Fill(xc, yc, dr);
        fHM->H2("fhdRVsX")->Fill(xc, dr);
        fHM->H2("fhdRVsY")->Fill(abs(yc), dr);
      }
    }
  }  // iR
}

void CbmRichGeoTest::FitAndFillHistEllipse(int histIndex, CbmRichRingLight* ring, double momentum)
{
  fTauFit->DoFit(ring);
  double axisA     = ring->GetAaxis();
  double axisB     = ring->GetBaxis();
  double xcEllipse = ring->GetCenterX();
  double ycEllipse = ring->GetCenterY();
  int nofHitsRing  = ring->GetNofHits();
  string t         = (histIndex == 0) ? "_hits" : "_points";

  if (axisA > fMinAaxis && axisA < fMaxAaxis && axisB > fMinBaxis && axisB < fMaxBaxis) {
    fHM->H1("fhBoAVsMom" + t)->Fill(momentum, axisB / axisA);
    fHM->H2("fhXcYcEllipse" + t)->Fill(xcEllipse, ycEllipse);
  }
  fHM->H1("fhNofHits" + t)->Fill(nofHitsRing);

  if (ycEllipse > 149 || ycEllipse < -149) { fHM->H1("fhBaxisUpHalf" + t)->Fill(axisB); }
  else {
    fHM->H1("fhBaxisDownHalf" + t)->Fill(axisB);
  }

  fHM->H2("fhBaxisVsMom" + t)->Fill(momentum, axisB);
  fHM->H2("fhAaxisVsMom" + t)->Fill(momentum, axisA);
  fHM->H2("fhChi2EllipseVsMom" + t)->Fill(momentum, ring->GetChi2() / ring->GetNofHits());
  if (histIndex == 0) {  // only hit fit
    fHM->H2("fhAaxisVsNofHits")->Fill(nofHitsRing, axisA);
    fHM->H2("fhBaxisVsNofHits")->Fill(nofHitsRing, axisB);
  }
}

void CbmRichGeoTest::FitAndFillHistCircle(int histIndex, CbmRichRingLight* ring, double momentum)
{
  fCopFit->DoFit(ring);
  double radius   = ring->GetRadius();
  double xcCircle = ring->GetCenterX();
  double ycCircle = ring->GetCenterY();
  int nofHitsRing = ring->GetNofHits();
  string t        = (histIndex == 0) ? "_hits" : "_points";

  fHM->H1("fhXcYcCircle" + t)->Fill(xcCircle, ycCircle);
  fHM->H1("fhRadiusVsMom" + t)->Fill(momentum, radius);
  fHM->H1("fhChi2CircleVsMom" + t)->Fill(momentum, ring->GetChi2() / ring->GetNofHits());

  for (int iH = 0; iH < nofHitsRing; iH++) {
    double xh = ring->GetHit(iH).fX;
    double yh = ring->GetHit(iH).fY;
    double dr = radius - sqrt((xcCircle - xh) * (xcCircle - xh) + (ycCircle - yh) * (ycCircle - yh));
    fHM->H1("fhDRVsMom" + t)->Fill(momentum, dr);

    if (histIndex == 0) {  // only hit fit
      fHM->H2("fhDRVsNofHits")->Fill(nofHitsRing, dr);
    }
  }

  if (histIndex == 0) {  // only hit fit
    fHM->H2("fhRadiusVsNofHits")->Fill(nofHitsRing, radius);
  }
}

void CbmRichGeoTest::FillMcVsHitFitEllipse(CbmRichRingLight* ring, CbmRichRingLight* ringMc)
{
  fHM->H2("fhDiffAaxis")->Fill(ring->GetNofHits(), ringMc->GetAaxis() - ring->GetAaxis());
  fHM->H2("fhDiffBaxis")->Fill(ring->GetNofHits(), ringMc->GetBaxis() - ring->GetBaxis());
  fHM->H2("fhDiffXcEllipse")->Fill(ring->GetNofHits(), ringMc->GetCenterX() - ring->GetCenterX());
  fHM->H2("fhDiffYcEllipse")->Fill(ring->GetNofHits(), ringMc->GetCenterY() - ring->GetCenterY());
}

void CbmRichGeoTest::FillMcVsHitFitCircle(CbmRichRingLight* ring, CbmRichRingLight* ringMc)
{
  fHM->H2("fhDiffXcCircle")->Fill(ring->GetNofHits(), ringMc->GetCenterX() - ring->GetCenterX());
  fHM->H2("fhDiffYcCircle")->Fill(ring->GetNofHits(), ringMc->GetCenterY() - ring->GetCenterY());
  fHM->H2("fhDiffRadius")->Fill(ring->GetNofHits(), ringMc->GetRadius() - ring->GetRadius());
}

void CbmRichGeoTest::ProcessHits()
{
  int fileId  = 0;
  int nofHits = fRichHits->GetEntriesFast();
  for (int iH = 0; iH < nofHits; iH++) {
    const CbmRichHit* hit = static_cast<CbmRichHit*>(fRichHits->At(iH));
    if (hit == nullptr) continue;
    int digiIndex = hit->GetRefId();
    if (digiIndex < 0) continue;
    const CbmRichDigi* digi = fDigiMan->Get<CbmRichDigi>(digiIndex);
    if (digi == nullptr) continue;
    const CbmMatch* digiMatch = fDigiMan->GetMatch(ECbmModuleId::kRich, digiIndex);
    if (digiMatch == nullptr) continue;

    vector<CbmLink> links = digiMatch->GetLinks();
    for (size_t i = 0; i < links.size(); i++) {
      int pointId = links[i].GetIndex();
      int eventId = links[i].GetEntry();
      if (pointId < 0) continue;  // noise hit

      const CbmRichPoint* point = static_cast<CbmRichPoint*>(fRichPoints->Get(fileId, eventId, pointId));
      if (point == nullptr) continue;

      TVector3 inPos(point->GetX(), point->GetY(), point->GetZ());
      TVector3 outPos;
      CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);
      fHM->H1("fhDiffXhit")->Fill(hit->GetX() - outPos.X());
      fHM->H1("fhDiffYhit")->Fill(hit->GetY() - outPos.Y());

      TVector3 mom;
      point->Momentum(mom);
      double momMag = mom.Mag();
      fHM->H1("fhPhotonEPmtHit")->Fill(1.e9 * momMag);
      double lambda = CbmRichPmt::getLambda(momMag);
      fHM->H1("fhLambdaPmtHit")->Fill(lambda);
    }

    //fHM->H1("fhNofPhotonsPerHit")->Fill(hit->GetNPhotons());
    fHM->H2("fhHitsXY")->Fill(hit->GetX(), hit->GetY());
    fHM->H1("fhHitsZ")->Fill(hit->GetZ());
  }
}

void CbmRichGeoTest::DrawRing(CbmRichRingLight* ringHit, CbmRichRingLight* ringPoint)
{
  if (ringHit->GetNofHits() < 1 || ringPoint->GetNofHits() < 1) return;
  stringstream ss;
  ss << "event_display/richgeo_event_display_" << fNofDrawnRings;
  fNofDrawnRings++;
  TCanvas* c = fHM->CreateCanvas(ss.str().c_str(), ss.str().c_str(), 800, 800);
  c->SetGrid(true, true);
  TH2D* pad = new TH2D(ss.str().c_str(), (ss.str() + ";X [cm];Y [cm]").c_str(), 1, -15., 15., 1, -15., 15);
  pad->SetStats(false);
  pad->Draw();

  // find min and max x and y positions of the hits in order to shift drawing
  double xmin = ringHit->GetHit(0).fX;
  double xmax = ringHit->GetHit(0).fX;
  double ymin = ringHit->GetHit(0).fY;
  double ymax = ringHit->GetHit(0).fY;
  for (int i = 0; i < ringHit->GetNofHits(); i++) {
    double hitX = ringHit->GetHit(i).fX;
    double hitY = ringHit->GetHit(i).fY;
    if (xmin > hitX) xmin = hitX;
    if (xmax < hitX) xmax = hitX;
    if (ymin > hitY) ymin = hitY;
    if (ymax < hitY) ymax = hitY;
  }
  double xCur = (xmin + xmax) / 2.;
  double yCur = (ymin + ymax) / 2.;

  //Draw circle and center
  TEllipse* circle = new TEllipse(ringHit->GetCenterX() - xCur, ringHit->GetCenterY() - yCur, ringHit->GetRadius());
  circle->SetFillStyle(0);
  circle->SetLineWidth(3);
  circle->Draw();
  TEllipse* center = new TEllipse(ringHit->GetCenterX() - xCur, ringHit->GetCenterY() - yCur, .5);
  center->Draw();

  // Draw hits
  for (int i = 0; i < ringHit->GetNofHits(); i++) {
    TEllipse* hitDr = new TEllipse(ringHit->GetHit(i).fX - xCur, ringHit->GetHit(i).fY - yCur, .5);
    hitDr->SetFillColor(kRed);
    hitDr->Draw();
  }

  // Draw MC Points
  for (int i = 0; i < ringPoint->GetNofHits(); i++) {
    TEllipse* pointDr = new TEllipse(ringPoint->GetHit(i).fX - xCur, ringPoint->GetHit(i).fY - yCur, 0.15);
    pointDr->SetFillColor(kBlue);
    pointDr->Draw();
  }

  //Draw information
  stringstream ss2;
  ss2 << "(r, n)=(" << setprecision(3) << ringHit->GetRadius() << ", " << ringHit->GetNofHits() << ")";
  TLatex* latex = new TLatex(-8., 8., ss2.str().c_str());
  latex->Draw();
}

TH1D* CbmRichGeoTest::CreateAccVsMinNofHitsHist()
{
  int nofMc  = fHM->H1("fhMomElMc")->GetEntries();
  TH1D* hist = static_cast<TH1D*>(fHM->H1("fhNofHits_hits")->Clone("fhAccVsMinNofHitsHist"));
  hist->GetXaxis()->SetTitle("Required min nof hits in ring");
  hist->GetYaxis()->SetTitle("Detector acceptance [%]");
  double sum = 0.;
  for (int i = hist->GetNbinsX(); i > 1; i--) {
    sum += fHM->H1("fhNofHits_hits")->GetBinContent(i);
    hist->SetBinContent(i, 100. * sum / nofMc);
  }
  return hist;
}

void CbmRichGeoTest::DrawH2MeanRms(TH2* hist, const string& canvasName)
{
  TCanvas* c = fHM->CreateCanvas(canvasName.c_str(), canvasName.c_str(), 1200, 600);
  c->Divide(2, 1);
  c->cd(1);
  DrawH2WithProfile(hist);
  c->cd(2);
  TH1D* py = (TH1D*) hist->ProjectionY((string(hist->GetName()) + "_py").c_str())->Clone();
  //DrawH1andFitGauss(py);
  DrawH1(py, kLinear, kLinear, "hist");
  py->SetStats(true);
  py->Scale(1. / py->Integral());
  py->GetYaxis()->SetTitle("Yield");
}

void CbmRichGeoTest::DrawHist()
{
  SetDefaultDrawStyle();

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_vertex_el", "richgeo_vertex_el", 2000, 1000);
    c->Divide(2, 1);
    c->cd(1);
    DrawH1(fHM->H1("fhMcVertexZEl"), kLinear, kLinear, "hist");
    c->cd(2);
    DrawH2(fHM->H2("fhMcVertexXYEl"));
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_hits_xy", "richgeo_hits_xy", 1200, 1200);
    CbmRichDraw::DrawPmtH2(fHM->H2("fhHitsXY"), c);
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_points_xy", "richgeo_points_xy", 1200, 1200);
    CbmRichDraw::DrawPmtH2(fHM->H2("fhPointsXY"), c);
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_points_xy_no_rotation", "richgeo_points_xy_no_rotation", 1200, 1200);
    CbmRichDraw::DrawPmtH2(fHM->H2("fhPointsXYNoRotation"), c);
  }

  {
    fHM->CreateCanvas("richgeo_hits_z", "richgeo_hits_z", 1200, 1200);
    fHM->NormalizeToIntegral("fhHitsZ");
    DrawH1(fHM->H1("fhHitsZ"), kLinear, kLinear, "hist");
  }

  {
    fHM->CreateCanvas("richgeo_points_z", "richgeo_points_z", 1200, 1200);
    fHM->NormalizeToIntegral("fhPointsZ");
    DrawH1(fHM->H1("fhPointsZ"), kLinear, kLinear, "hist");
  }

  vector<string> ph = {"_hits", "_points"};
  for (const string& t : ph) {

    DrawH2MeanRms(fHM->H2("fhBoAVsMom" + t), "richgeo" + t + "_ellipse_boa_vs_mom");
    fHM->H2("fhBoAVsMom" + t)->GetYaxis()->SetRangeUser(0.8, 1.0);

    {
      string name = "richgeo" + t + "_ellipse_xc_yc";
      fHM->CreateCanvas(name.c_str(), name.c_str(), 1200, 1200);
      DrawH2(fHM->H2("fhXcYcEllipse" + t));
    }

    DrawH2MeanRms(fHM->H2("fhChi2EllipseVsMom" + t), "richgeo" + t + "_chi2_ellipse_vs_mom");
    DrawH2MeanRms(fHM->H2("fhAaxisVsMom" + t), "richgeo" + t + "_a_vs_mom");
    DrawH2MeanRms(fHM->H2("fhBaxisVsMom" + t), "richgeo" + t + "_b_vs_mom");

    {
      string name = "richgeo" + t + "_b_up_down_halves";
      TCanvas* c  = fHM->CreateCanvas(name.c_str(), name.c_str(), 2000, 1000);
      c->Divide(2, 1);
      c->cd(1);
      TH1* hUp = fHM->H1Clone("fhBaxisUpHalf" + t);
      DrawH1(hUp, kLinear, kLinear, "hist");
      hUp->SetStats(true);
      c->cd(2);
      TH1* hDown = fHM->H1Clone("fhBaxisDownHalf" + t);
      DrawH1(hDown, kLinear, kLinear, "hist");
      hDown->SetStats(true);
    }

    {
      string name = "richgeo" + t + "_circle";
      TCanvas* c  = fHM->CreateCanvas(name.c_str(), name.c_str(), 2000, 1000);
      c->Divide(2, 1);
      c->cd(1);
      DrawH1andFitGauss(fHM->H1Clone("fhNofHits" + t));
      LOG(info) << "Number of hits/points = " << fHM->H1("fhNofHits" + t)->GetMean();
      //gPad->SetLogy(true);
      c->cd(2);
      DrawH2(fHM->H2("fhXcYcCircle" + t));
    }

    DrawH2MeanRms(fHM->H2("fhChi2CircleVsMom" + t), "richgeo" + t + "_chi2_circle_vs_mom");
    DrawH2MeanRms(fHM->H2("fhRadiusVsMom" + t), "richgeo" + t + "_r_vs_mom");
    DrawH2MeanRms(fHM->H2("fhDRVsMom" + t), "richgeo" + t + "_dr_vs_mom");
    fHM->H2("fhDRVsMom" + t)->GetYaxis()->SetRangeUser(-1.05, 1.05);
  }  // _hits, _points

  {
    fHM->CreateCanvas("richgeo_nof_photons_per_hit", "richgeo_nof_photons_per_hit", 1200, 1200);
    fHM->NormalizeToIntegral("fhNofPhotonsPerHit");
    DrawH1(fHM->H1("fhNofPhotonsPerHit"), kLinear, kLinear, "hist");
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_diff_ellipse", "richgeo_diff_ellipse", 2000, 1000);
    c->Divide(4, 2);
    c->cd(1);
    DrawH2WithProfile(fHM->H2("fhDiffAaxis"));
    c->cd(2);
    DrawH2WithProfile(fHM->H2("fhDiffBaxis"));
    c->cd(3);
    DrawH2WithProfile(fHM->H2("fhDiffXcEllipse"));
    c->cd(4);
    DrawH2WithProfile(fHM->H2("fhDiffYcEllipse"));
    c->cd(5);
    DrawH1(fHM->H2("fhDiffAaxis")->ProjectionY(), kLinear, kLog, "hist");
    c->cd(6);
    DrawH1(fHM->H2("fhDiffBaxis")->ProjectionY(), kLinear, kLog, "hist");
    c->cd(7);
    DrawH1(fHM->H2("fhDiffXcEllipse")->ProjectionY(), kLinear, kLog, "hist");
    c->cd(8);
    DrawH1(fHM->H2("fhDiffYcEllipse")->ProjectionY(), kLinear, kLog, "hist");
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_diff_circle", "richgeo_diff_circle", 1500, 1000);
    c->Divide(3, 2);
    c->cd(1);
    DrawH2WithProfile(fHM->H2("fhDiffXcCircle"));
    c->cd(2);
    DrawH2WithProfile(fHM->H2("fhDiffYcCircle"));
    c->cd(3);
    DrawH2WithProfile(fHM->H2("fhDiffRadius"));
    c->cd(4);
    DrawH1(fHM->H2("fhDiffXcCircle")->ProjectionY(), kLinear, kLog, "hist");
    c->cd(5);
    DrawH1(fHM->H2("fhDiffYcCircle")->ProjectionY(), kLinear, kLog, "hist");
    c->cd(6);
    DrawH1(fHM->H2("fhDiffRadius")->ProjectionY(), kLinear, kLog, "hist");
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_hits_residual", "richgeo_hits", 2000, 1000);
    c->Divide(2, 1);
    c->cd(1);
    fHM->NormalizeToIntegral("fhDiffXhit");
    DrawH1(fHM->H1("fhDiffXhit"), kLinear, kLinear, "hist");
    c->cd(2);
    fHM->NormalizeToIntegral("fhDiffYhit");
    DrawH1(fHM->H1("fhDiffYhit"), kLinear, kLinear, "hist");
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_fit_eff", "richgeo_fit_eff", 1800, 600);
    c->Divide(3, 1);
    c->cd(1);
    DrawH1({fHM->H1Clone("fhNofHitsAll"), fHM->H1Clone("fhNofHitsCircleFit"), fHM->H1Clone("fhNofHitsEllipseFit")},
           {"All", "Circle fit", "Ellipse fit"}, kLinear, kLog, true, 0.7, 0.7, 0.99, 0.99, "hist");
    TH1D* fhNofHitsCircleFitEff  = Cbm::DivideH1(fHM->H1("fhNofHitsCircleFit"), fHM->H1("fhNofHitsAll"));
    TH1D* fhNofHitsEllipseFitEff = Cbm::DivideH1(fHM->H1("fhNofHitsEllipseFit"), fHM->H1("fhNofHitsAll"));
    c->cd(2);
    DrawH1(fhNofHitsCircleFitEff);
    TLatex* circleFitEffTxt =
      new TLatex(15, 0.5, CalcEfficiency(fHM->H1("fhNofHitsCircleFit"), fHM->H1("fhNofHitsAll")).c_str());
    cout << "Circle fit efficiency:" << circleFitEffTxt << "%" << endl;
    circleFitEffTxt->Draw();
    c->cd(3);
    DrawH1(fhNofHitsEllipseFitEff);
    TLatex* ellipseFitEffTxt =
      new TLatex(15, 0.5, CalcEfficiency(fHM->H1("fhNofHitsEllipseFit"), fHM->H1("fhNofHitsAll")).c_str());
    cout << "Ellipse fit efficiency:" << ellipseFitEffTxt << "%" << endl;
    ellipseFitEffTxt->Draw();
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_acc_el", "richgeo_acc_el", 1800, 1200);
    c->Divide(3, 2);
    c->cd(1);
    DrawH1(fHM->H1Vector({string("fhMomElMc"), "fhMomElAcc"}), {"MC", "ACC"}, kLinear, kLog, true, 0.8, 0.8, 0.99, 0.99,
           "hist");
    c->cd(2);
    DrawH2(fHM->H2("fhPtYElMc"));
    fHM->H2("fhPtYElMc")->SetMinimum(0.);
    c->cd(3);
    DrawH2(fHM->H2("fhPtYElAcc"));
    fHM->H2("fhPtYElAcc")->SetMinimum(0.);
    c->cd(4);
    DrawH2(fHM->H2("fhPYElMc"));
    fHM->H2("fhPYElMc")->SetMinimum(0.);
    c->cd(5);
    DrawH2(fHM->H2("fhPYElAcc"));
    fHM->H2("fhPYElAcc")->SetMinimum(0.);
  }


  TH1D* pxEff = Cbm::DivideH1(fHM->H1Clone("fhMomElAcc"), (TH1D*) fHM->H1Clone("fhMomElMc"), "", 100.,
                              "Geometrical acceptance [%]");
  {
    fHM->CreateCanvas("richgeo_acc_eff_el_mom", "richgeo_acc_eff_el_mom", 800, 800);
    string effEl = CalcEfficiency(fHM->H1Clone("fhMomElAcc"), fHM->H1Clone("fhMomElMc"));
    cout << "Geometrical acceptance electrons:" << effEl << "%" << endl;
    DrawH1({pxEff}, {"e^{#pm} (" + effEl + "%)"}, kLinear, kLinear, true, 0.6, 0.55, 0.88, 0.65);
  }

  {
    TH2D* pyzEff =
      Cbm::DivideH2(fHM->H2Clone("fhPtYElAcc"), fHM->H2Clone("fhPtYElMc"), "", 100., "Geometrical acceptance [%]");
    fHM->CreateCanvas("richgeo_acc_eff_el_pty", "richgeo_acc_eff_el_pty", 800, 800);
    DrawH2(pyzEff);
  }

  {
    TH2D* pyzEff =
      Cbm::DivideH2(fHM->H2Clone("fhPYElAcc"), fHM->H2Clone("fhPYElMc"), "", 100., "Geometrical acceptance [%]");
    fHM->CreateCanvas("richgeo_acc_eff_el_py", "richgeo_acc_eff_el_py", 800, 800);
    DrawH2(pyzEff);
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_acc_eff_el_zoom", "richgeo_acc_eff_el_zoom", 1000, 500);
    c->Divide(2, 1);
    c->cd(1);
    TH1* fhMcMomEl  = fHM->H1Clone("fhMomElMc");
    TH1* fhAccMomEl = fHM->H1Clone("fhMomElAcc");
    fhMcMomEl->GetXaxis()->SetRangeUser(0., 3.);
    fhAccMomEl->GetXaxis()->SetRangeUser(0., 3.);
    fhMcMomEl->SetMinimum(0.);
    DrawH1({fhMcMomEl, fhAccMomEl}, {"MC", "ACC"}, kLinear, kLog, true, 0.8, 0.8, 0.99, 0.99);
    gPad->SetLogy(false);
    c->cd(2);
    TH1D* pxEffClone = static_cast<TH1D*>(pxEff->Clone());
    pxEffClone->GetXaxis()->SetRangeUser(0., 3.);
    pxEffClone->SetMinimum(0.);
    DrawH1(pxEffClone);
  }

  // Draw number vs position onto the photodetector plane
  {
    TCanvas* c = fHM->CreateCanvas("richgeo_numbers_vs_xy_hits", "richgeo_numbers_vs_xy_hits", 1800, 600);
    c->Divide(3, 1);
    c->cd(1);
    DrawH3Profile(fHM->H3("fhNofHitsXYZ"), true, false, 10, 30);
    c->cd(2);
    DrawH2WithProfile(fHM->H2("fhNofHitsVsX"), false, true);
    c->cd(3);
    DrawH2WithProfile(fHM->H2("fhNofHitsVsY"), false, true);
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_numbers_vs_xy_points", "richgeo_numbers_vs_xy_points", 1800, 600);
    c->Divide(3, 1);
    c->cd(1);
    DrawH3Profile(fHM->H3("fhNofPointsXYZ"), true, false, 100., 300.);
    c->cd(2);
    DrawH2WithProfile(fHM->H2("fhNofPointsVsX"), false, true);
    c->cd(3);
    DrawH2WithProfile(fHM->H2("fhNofPointsVsY"), false, true);
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_numbers_vs_xy_boa", "richgeo_numbers_vs_xy_boa", 1800, 600);
    c->Divide(3, 1);
    c->cd(1);
    DrawH3Profile(fHM->H3("fhBoAXYZ"), true, false, 0.75, 1.0);
    c->cd(2);
    DrawH2WithProfile(fHM->H2("fhBoAVsX"), false, true);
    fHM->H2("fhBoAVsX")->GetYaxis()->SetRangeUser(0.75, 1.0);
    c->cd(3);
    DrawH2WithProfile(fHM->H2("fhBoAVsY"), false, true);
    fHM->H2("fhBoAVsY")->GetYaxis()->SetRangeUser(0.75, 1.0);
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_numbers_vs_xy_b", "richgeo_numbers_vs_xy_b", 1800, 600);
    c->Divide(3, 1);
    c->cd(1);
    DrawH3Profile(fHM->H3("fhBaxisXYZ"), true, false, 4., 5.);
    c->cd(2);
    DrawH2WithProfile(fHM->H2("fhBaxisVsX"), false, true);
    c->cd(3);
    DrawH2WithProfile(fHM->H2("fhBaxisVsY"), false, true);
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_numbers_vs_xy_a", "richgeo_numbers_vs_xy_a", 1800, 600);
    c->Divide(3, 1);
    c->cd(1);
    DrawH3Profile(fHM->H3("fhAaxisXYZ"), true, false, 4.4, 5.7);
    c->cd(2);
    DrawH2WithProfile(fHM->H2("fhAaxisVsX"), false, true);
    c->cd(3);
    DrawH2WithProfile(fHM->H2("fhAaxisVsY"), false, true);
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_numbers_vs_xy_r", "richgeo_numbers_vs_xy_r", 1800, 600);
    c->Divide(3, 1);
    c->cd(1);
    DrawH3Profile(fHM->H3("fhRadiusXYZ"), true, false, 4.2, 5.2);
    c->cd(2);
    DrawH2WithProfile(fHM->H2("fhRadiusVsX"), false, true);
    c->cd(3);
    DrawH2WithProfile(fHM->H2("fhRadiusVsY"), false, true);
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_numbers_vs_xy_dr", "richgeo_numbers_vs_xy_dr", 1800, 600);
    c->Divide(3, 1);
    c->cd(1);
    DrawH3Profile(fHM->H3("fhdRXYZ"), false, false, 0., .5);
    c->cd(2);
    DrawH2WithProfile(fHM->H2("fhdRVsX"), false, false);
    c->cd(3);
    DrawH2WithProfile(fHM->H2("fhdRVsY"), false, false);
  }

  {
    fHM->CreateCanvas("richgeo_acc_vs_min_nof_hits", "richgeo_acc_vs_min_nof_hits", 1000, 1000);
    TH1D* h = CreateAccVsMinNofHitsHist();
    h->GetXaxis()->SetRangeUser(0., 40.0);
    DrawH1(h);
  }

  DrawH2MeanRms(fHM->H2("fhRadiusVsNofHits"), "richgeo_hits_r_vs_nof_hits");
  DrawH2MeanRms(fHM->H2("fhAaxisVsNofHits"), "richgeo_hits_a_vs_nof_hits");
  DrawH2MeanRms(fHM->H2("fhBaxisVsNofHits"), "richgeo_hits_b_vs_nof_hits");
  DrawH2MeanRms(fHM->H2("fhDRVsNofHits"), "richgeo_hits_dr_vs_nof_hits");
  fHM->H2("fhDRVsNofHits")->GetYaxis()->SetRangeUser(-1.05, 1.05);

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_hits_rab", "richgeo_hits_rab", 1500, 600);
    c->Divide(3, 1);
    c->cd(1);
    TH1D* pyR =
      fHM->H2("fhRadiusVsNofHits")->ProjectionY((string(fHM->H2("fhRadiusVsNofHits")->GetName()) + "_py").c_str());
    DrawH1(pyR, kLinear, kLinear, "hist");
    pyR->SetStats(true);
    c->cd(2);
    TH1D* pyA =
      fHM->H2("fhAaxisVsNofHits")->ProjectionY((string(fHM->H2("fhAaxisVsNofHits")->GetName()) + "_py").c_str());
    DrawH1(pyA, kLinear, kLinear, "hist");
    pyA->SetStats(true);
    c->cd(3);
    TH1D* pyB =
      fHM->H2("fhBaxisVsNofHits")->ProjectionY((string(fHM->H2("fhBaxisVsNofHits")->GetName()) + "_py").c_str());
    DrawH1(pyB, kLinear, kLinear, "hist");
    pyB->SetStats(true);
  }


  {
    TCanvas* c            = fHM->CreateCanvas("richgeo_photon_energy", "richgeo_photon_energy", 2000, 1000);
    vector<string> labels = {"Sens plane Z+", "Sens plane Z-", "PMT Point", "PMT hit"};
    c->Divide(2, 1);
    c->cd(1);
    DrawH1(fHM->H1Vector({"fhPhotonEPlaneZ+", "fhPhotonEPlaneZ-", "fhPhotonEPmtPoint", "fhPhotonEPmtHit"}), labels);
    c->cd(2);
    DrawH1(fHM->H1Vector({"fhLambdaPlaneZ+", "fhLambdaPlaneZ-", "fhLambdaPmtPoint", "fhLambdaPmtHit"}), labels);
  }
}


void CbmRichGeoTest::DrawPmts()
{
  fHM->Create3<TH3D>("fhPointsXYZ", "fhPointsXYZ;X [cm];Y [cm];Z [cm];Yield", 100, -50, 50, 100, -300, 300, 100, 100,
                     300);
  fHM->Create3<TH3D>("fhHitsXYZ", "fhHitsXYZ;X [cm];Y [cm];Z [cm];Yield", 100, -50, 50, 100, -300, 300, 100, 100, 300);
  vector<int> pixels = CbmRichDigiMapManager::GetInstance().GetPixelAddresses();
  vector<int> pmts   = CbmRichDigiMapManager::GetInstance().GetPmtIds();

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_pixels_xy", "richgeo_pixels_xy", 1500, 1500);
    c->SetGrid(true, true);
    TH2D* pad = new TH2D("richgeo_pixels_xy", ";X [cm];Y [cm]", 1, -120, 120, 1, -210, 210);
    //TH2D* pad = new TH2D("richgeo_pixels_xy", ";X [cm];Y [cm]", 1, -20, 20, 1, 140, 180);
    pad->SetStats(false);
    pad->Draw();
    DrawPmtPoint("xy", pixels, true);
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_pixels_xz", "richgeo_pixels_xz", 1500, 1500);
    c->SetGrid(true, true);
    TH2D* pad = new TH2D("richgeo_pixels_xz", ";Z [cm];X [cm]", 1, 200, 250, 1, -120., 120.);
    pad->SetStats(false);
    pad->Draw();
    DrawPmtPoint("zx", pixels, true);
  }

  {
    fHM->CreateCanvas("richgeo_pixels_yz", "richgeo_pixels_yz", 1500, 1500);
    TH2D* pad = new TH2D("richgeo_pixels_yz", ";Z [cm];Y [cm]", 1, 200, 250, 1, -220, 220);
    pad->SetStats(false);
    pad->Draw();
    DrawPmtPoint("zy", pixels, true);
  }

  {
    TCanvas* c = fHM->CreateCanvas("richgeo_pmts_xy", "richgeo_pmts_xy", 1500, 1500);
    c->SetGrid(true, true);
    TH2D* pad = new TH2D("richgeo_pmts_xy", ";X [cm];Y [cm]", 1, -120, 120, 1, -210, 210);
    pad->SetStats(false);
    pad->Draw();
    DrawPmtPoint("xy", pmts, false);
  }


  for (unsigned int i = 0; i < pixels.size(); i++) {
    CbmRichPixelData* pixelData = CbmRichDigiMapManager::GetInstance().GetPixelDataByAddress(pixels[i]);
    TVector3 inPos(pixelData->fX, pixelData->fY, pixelData->fZ);
    TVector3 outPos;
    CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);

    fHM->H3("fhPointsXYZ")->Fill(inPos.X(), inPos.Y(), inPos.Z());
    fHM->H3("fhHitsXYZ")->Fill(outPos.X(), outPos.Y(), outPos.Z());
  }

  {
    fHM->CreateCanvas("richgeo_pixels_points_xyz", "richgeo_pixels_points_xyz", 1500, 1500);
    fHM->H3("fhPointsXYZ")->Draw();
  }

  {
    fHM->CreateCanvas("richgeo_pixels_hits_xyz", "richgeo_pixels_hits_xyz", 1500, 1500);
    fHM->H3("fhHitsXYZ")->Draw();
  }
}

void CbmRichGeoTest::DrawPmtPoint(const string& coordOpt, const vector<int>& ids, bool isDrawPixel)
{
  for (size_t i = 0; i < ids.size(); i++) {
    TVector3 inPos;
    double halfSize = 0.0;
    if (isDrawPixel) {
      CbmRichPixelData* pixelData = CbmRichDigiMapManager::GetInstance().GetPixelDataByAddress(ids[i]);
      inPos.SetXYZ(pixelData->fX, pixelData->fY, pixelData->fZ);
      halfSize = 0.15;
    }
    else {
      CbmRichPmtData* pmtData = CbmRichDigiMapManager::GetInstance().GetPmtDataById(ids[i]);
      inPos.SetXYZ(pmtData->fX, pmtData->fY, pmtData->fZ);
      halfSize = 0.5 * pmtData->fHeight;
    }

    TVector3 outPos;
    CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);
    TBox* boxOut = nullptr;
    TBox* boxIn  = nullptr;
    if (coordOpt == "xy") {
      boxOut = new TBox(outPos.X() - halfSize, outPos.Y() - halfSize, outPos.X() + halfSize, outPos.Y() + halfSize);
      boxIn  = new TBox(inPos.X() - halfSize, inPos.Y() - halfSize, inPos.X() + halfSize, inPos.Y() + halfSize);
    }
    else if (coordOpt == "zx") {
      boxOut = new TBox(outPos.Z() - halfSize, outPos.X() - halfSize, outPos.Z() + halfSize, outPos.X() + halfSize);
      boxIn  = new TBox(inPos.Z() - halfSize, inPos.X() - halfSize, inPos.Z() + halfSize, inPos.X() + halfSize);
    }
    else if (coordOpt == "zy") {
      boxOut = new TBox(outPos.Z() - halfSize, outPos.Y() - halfSize, outPos.Z() + halfSize, outPos.Y() + halfSize);
      boxIn  = new TBox(inPos.Z() - halfSize, inPos.Y() - halfSize, inPos.Z() + halfSize, inPos.Y() + halfSize);
    }

    if (boxOut != nullptr && boxIn != nullptr) {
      if (isDrawPixel) {
        boxOut->SetFillColor(kBlue);
        boxIn->SetFillColor(kRed);
      }
      else {
        boxOut->SetFillStyle(0);
        boxOut->SetLineColor(kBlue);
        boxIn->SetFillStyle(0);
        boxIn->SetLineColor(kRed);
      }
      boxOut->Draw();
      boxIn->Draw();
    }
  }
}


void CbmRichGeoTest::Finish()
{
  DrawHist();
  if (fDrawPmts) { DrawPmts(); }

  TDirectory* oldir = gDirectory;
  TFile* outFile    = FairRootManager::Instance()->GetOutFile();
  if (outFile != nullptr) {
    outFile->cd();
    fHM->WriteToFile();
  }
  gDirectory->cd(oldir->GetPath());
  fHM->SaveCanvasToImage(fOutputDir, "png");
}

string CbmRichGeoTest::CalcEfficiency(TH1* histRec, TH1* histAcc)
{
  if (histAcc->GetEntries() == 0) { return "0"; }
  else {
    double eff = 100. * double(histRec->GetEntries()) / double(histAcc->GetEntries());
    return Cbm::NumberToString(eff, 2);
  }
}

void CbmRichGeoTest::DrawFromFile(const string& fileName, const string& outputDir)
{
  fOutputDir = outputDir;

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  if (fHM != nullptr) delete fHM;

  fHM         = new CbmHistManager();
  TFile* file = new TFile(fileName.c_str());
  fHM->ReadFromFile(file);

  DrawHist();

  fHM->SaveCanvasToImage(fOutputDir, "png,eps");

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

ClassImp(CbmRichGeoTest)
