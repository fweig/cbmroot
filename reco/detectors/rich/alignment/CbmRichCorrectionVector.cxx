// ---------- Original Headers ---------- //
#include "CbmRichCorrectionVector.h"
#include "FairLogger.h"
#include "FairRootManager.h"

#include "TClonesArray.h"

#include "CbmDrawHist.h"
#include "CbmRichHit.h"
#include "TCanvas.h"
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

CbmRichCorrectionVector::CbmRichCorrectionVector()
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
  , fHM2(NULL)
  ,
  //fGP(),
  fEventNum(0)
  , fOutputDir("")
  , fRunTitle("")
  , fAxisRotTitle("")
  , fDrawAlignment(kTRUE)
  , fDrawMapping(kFALSE)
  , fDrawProjection(kTRUE)
  , fIsMeanCenter(kFALSE)
  , fIsReconstruction(kFALSE)
  , fCopFit(NULL)
  , fTauFit(NULL)
  , fPathsMap()
  , fPathsMapEllipse()
  , fPhi() {
  fMirrCounter = 0.;
  for (int i = 0; i < 3; i++) {
    fArray[i] = 0.;
  }
}

CbmRichCorrectionVector::~CbmRichCorrectionVector() {}

InitStatus CbmRichCorrectionVector::Init() {
  FairRootManager* manager = FairRootManager::Instance();

  fRichHits = (TClonesArray*) manager->GetObject("RichHit");
  if (NULL == fRichHits) {
    Fatal("CbmRichCorrectionVector::Init", "No RichHit array !");
  }

  fRichRings = (TClonesArray*) manager->GetObject("RichRing");
  if (NULL == fRichRings) {
    Fatal("CbmRichCorrectionVector::Init", "No RichRing array !");
  }

  fRichProjections = (TClonesArray*) manager->GetObject("RichProjection");
  if (NULL == fRichProjections) {
    Fatal("CbmRichCorrectionVector::Init", "No RichProjection array !");
  }

  fRichMirrorPoints = (TClonesArray*) manager->GetObject("RichMirrorPoint");
  if (NULL == fRichMirrorPoints) {
    Fatal("CbmRichCorrectionVector::Init", "No RichMirrorPoints array !");
  }

  fRichMCPoints = (TClonesArray*) manager->GetObject("RichPoint");
  if (NULL == fRichMCPoints) {
    Fatal("CbmRichCorrectionVector::Init", "No RichMCPoints array !");
  }

  fMCTracks = (TClonesArray*) manager->GetObject("MCTrack");
  if (NULL == fMCTracks) {
    Fatal("CbmRichCorrectionVector::Init", "No MCTracks array !");
  }

  fRichRingMatches = (TClonesArray*) manager->GetObject("RichRingMatch");
  if (NULL == fRichRingMatches) {
    Fatal("CbmRichCorrectionVector::Init", "No RichRingMatches array !");
  }

  fRichRefPlanePoints = (TClonesArray*) manager->GetObject("RefPlanePoint");
  if (NULL == fRichRefPlanePoints) {
    Fatal("CbmRichCorrectionVector::Init", "No RichRefPlanePoint array !");
  }

  fRichPoints = (TClonesArray*) manager->GetObject("RichPoint");
  if (NULL == fRichPoints) {
    Fatal("CbmRichCorrectionVector::Init", "No RichPoint array !");
  }

  fGlobalTracks = (TClonesArray*) manager->GetObject("GlobalTrack");
  if (NULL == fGlobalTracks) {
    Fatal("CbmAnaDielectronTask::Init", "No GlobalTrack array!");
  }

  fPathsMap["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
            "RICH_mirror_and_support_belt_strip3_126/RICH_mirror_2_53"] =
    "2_53";
  fPathsMap["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
            "RICH_mirror_and_support_belt_strip3_126/RICH_mirror_2_52"] =
    "2_52";
  fPathsMap["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
            "RICH_mirror_and_support_belt_strip3_126/RICH_mirror_1_51"] =
    "1_51";
  fPathsMap["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
            "RICH_mirror_and_support_belt_strip5_128/RICH_mirror_2_77"] =
    "2_77";
  fPathsMap["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
            "RICH_mirror_and_support_belt_strip5_128/RICH_mirror_2_76"] =
    "2_76";
  fPathsMap["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
            "RICH_mirror_and_support_belt_strip5_128/RICH_mirror_1_75"] =
    "1_75";
  fPathsMap["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
            "RICH_mirror_and_support_belt_strip1_124/RICH_mirror_2_29"] =
    "2_29";
  fPathsMap["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
            "RICH_mirror_and_support_belt_strip1_124/RICH_mirror_2_28"] =
    "2_28";
  fPathsMap["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
            "RICH_mirror_and_support_belt_strip1_124/RICH_mirror_1_27"] =
    "1_27";
  fPathsMap["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
            "RICH_mirror_and_support_belt_strip_cut_123/RICH_mirror_3_15"] =
    "3_15";
  fPathsMap["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
            "RICH_mirror_and_support_belt_strip_cut_123/RICH_mirror_2_16"] =
    "2_16";
  fPathsMap["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
            "RICH_mirror_and_support_belt_strip_cut_123/RICH_mirror_2_17"] =
    "2_17";

  fPathsMapEllipse["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
                   "RICH_mirror_and_support_belt_strip3_126/RICH_mirror_2_53"] =
    "2_53";
  fPathsMapEllipse["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
                   "RICH_mirror_and_support_belt_strip3_126/RICH_mirror_2_52"] =
    "2_52";
  fPathsMapEllipse["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
                   "RICH_mirror_and_support_belt_strip3_126/RICH_mirror_1_51"] =
    "1_51";
  fPathsMapEllipse["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
                   "RICH_mirror_and_support_belt_strip5_128/RICH_mirror_2_77"] =
    "2_77";
  fPathsMapEllipse["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
                   "RICH_mirror_and_support_belt_strip5_128/RICH_mirror_2_76"] =
    "2_76";
  fPathsMapEllipse["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
                   "RICH_mirror_and_support_belt_strip5_128/RICH_mirror_1_75"] =
    "1_75";
  fPathsMapEllipse["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
                   "RICH_mirror_and_support_belt_strip1_124/RICH_mirror_2_29"] =
    "2_29";
  fPathsMapEllipse["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
                   "RICH_mirror_and_support_belt_strip1_124/RICH_mirror_2_28"] =
    "2_28";
  fPathsMapEllipse["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
                   "RICH_mirror_and_support_belt_strip1_124/RICH_mirror_1_27"] =
    "1_27";
  fPathsMapEllipse
    ["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
     "RICH_mirror_and_support_belt_strip_cut_123/RICH_mirror_3_15"] = "3_15";
  fPathsMapEllipse
    ["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
     "RICH_mirror_and_support_belt_strip_cut_123/RICH_mirror_2_16"] = "2_16";
  fPathsMapEllipse
    ["/cave_1/rich1_0/RICH_gas_221/RICH_mirror_half_total_208/"
     "RICH_mirror_and_support_belt_strip_cut_123/RICH_mirror_2_17"] = "2_17";

  fCopFit = new CbmRichRingFitterCOP();
  fTauFit = new CbmRichRingFitterEllipseTau();
  CbmRichConverter::Init();

  InitHistProjection();
  InitHistAlignment();

  return kSUCCESS;
}

void CbmRichCorrectionVector::InitHistProjection() {
  fHM = new CbmHistManager();
  for (std::map<string, string>::iterator it = fPathsMap.begin();
       it != fPathsMap.end();
       ++it) {  // Initialize all the histograms, using map IDs as inputs.
    string name =
      "fHMCPoints_"
      + it->second;  // it->first gives the paths; it->second gives the ID.
    fHM->Create2<TH2D>(name,
                       name + ";X_Axis [];Y_Axis [];Entries",
                       2001,
                       -100.,
                       100.,
                       2001,
                       60.,
                       210.);
  }

  for (std::map<string, string>::iterator it = fPathsMapEllipse.begin();
       it != fPathsMapEllipse.end();
       ++it) {
    string name = "fHPoints_Ellipse_" + it->second;
    fHM->Create2<TH2D>(name,
                       name + ";X_Axis [];Y_Axis [];Entries",
                       2001,
                       -100.,
                       100.,
                       2001,
                       60.,
                       210.);
  }

  fHM->Create1<TH1D>("fhDistanceCenterToExtrapolatedTrack",
                     "fhDistanceCenterToExtrapolatedTrack;Distance fitted "
                     "center to extrapolated track;Number of entries",
                     400,
                     0.,
                     2.);
  fHM->Create1<TH1D>(
    "fhDistanceCenterToExtrapolatedTrackInPlane",
    "fhDistanceCenterToExtrapolatedTrackInPlane;Distance fitted center to "
    "extrapolated track in plane;Number of entries",
    400,
    0.,
    2.);
  fHM->Create1<TH1D>("fhDifferenceX",
                     "fhDifferenceX;Difference in X (fitted center - "
                     "extrapolated track);Number of entries",
                     400,
                     0.,
                     2.);
  fHM->Create1<TH1D>("fhDifferenceY",
                     "fhDifferenceY;Difference in Y (fitted center - "
                     "extrapolated track);Number of entries",
                     400,
                     0.,
                     2.);

  fHM->Create1<TH1D>(
    "fhDistanceCenterToExtrapolatedTrackInPlaneUncorrected",
    "fhDistanceCenterToExtrapolatedTrackInPlaneUncorrected;Distance fitted "
    "center to extrapolated track in plane;Number of entries",
    300,
    0.,
    3.);
  fHM->Create1<TH1D>("fhDifferenceXUncorrected",
                     "fhDifferenceXUncorrected;Difference in X (fitted center "
                     "- extrapolated track);Number of entries",
                     300,
                     0.,
                     3.);
  fHM->Create1<TH1D>("fhDifferenceYUncorrected",
                     "fhDifferenceYUncorrected;Difference in Y (fitted center "
                     "- extrapolated track);Number of entries",
                     300,
                     0.,
                     3.);
}

void CbmRichCorrectionVector::InitHistAlignment() {
  fHM2 = new CbmHistManager();

  fHM2->Create1<TH1D>("fHCenterDistance",
                      "fHCenterDistance;Distance C-C';Nb of entries",
                      100,
                      -0.1,
                      5.);
  fHM2->Create1<TH1D>(
    "fHPhi", "fHPhi;Phi_Ch [rad];Nb of entries", 200, -3.4, 3.4);
  fHM2->Create1<TH1D>(
    "fHThetaDiff", "fHThetaDiff;Th_Ch-Th_0 [cm];Nb of entries", 252, -5., 5.);
  fHM2->Create2<TH2D>(
    "fHCherenkovHitsDistribTheta0",
    "fHCherenkovHitsDistribTheta0;Phi_0 [rad];Theta_0 [cm];Entries",
    200,
    -2.,
    2.,
    600,
    2.,
    8.);
  fHM2->Create2<TH2D>(
    "fHCherenkovHitsDistribThetaCh",
    "fHCherenkovHitsDistribThetaCh;Phi_Ch [rad];Theta_Ch [cm];Entries",
    200,
    -3.4,
    3.4,
    600,
    0.,
    20);
  fHM2->Create2<TH2D>(
    "fHCherenkovHitsDistribReduced",
    "fHCherenkovHitsDistribReduced;Phi_Ch [rad];Th_Ch-Th_0 [cm];Entries",
    200,
    -3.4,
    3.4,
    500,
    -5.,
    5.);
}

void CbmRichCorrectionVector::Exec(Option_t* /*option*/) {
  cout << endl
       << "//"
          "--------------------------------------------------------------------"
          "------------------------------------------//"
       << endl;
  cout << "//---------------------------------------- EXEC Function - Defining "
          "the entries ----------------------------------------//"
       << endl;
  cout << "//"
          "--------------------------------------------------------------------"
          "--------------------------------------------------//"
       << endl;
  fEventNum++;
  //LOG(debug2) << "CbmRichCorrectionVector : Event #" << fEventNum;
  cout << "CbmRichCorrectionVector : Event #" << fEventNum << endl;

  Int_t nofRingsInEvent = fRichRings->GetEntries();
  Int_t nofMirrorPoints = fRichMirrorPoints->GetEntries();
  Int_t nofHitsInEvent  = fRichHits->GetEntries();
  Int_t NofMCPoints     = fRichMCPoints->GetEntriesFast();
  Int_t NofMCTracks     = fMCTracks->GetEntriesFast();
  cout << "Nb of rings in evt = " << nofRingsInEvent
       << ", nb of mirror points = " << nofMirrorPoints
       << ", nb of hits in evt = " << nofHitsInEvent
       << ", nb of Monte-Carlo points = " << NofMCPoints
       << " and nb of Monte-Carlo tracks = " << NofMCTracks << endl
       << endl;

  cout << "//"
          "--------------------------------------------------------------------"
          "--------------------------------------//"
       << endl;
  cout << "//-------------------------------- EXEC Function - Correction "
          "Vector class ------------------------------------------//"
       << endl;
  cout << "//"
          "--------------------------------------------------------------------"
          "--------------------------------------//"
       << endl
       << endl;

  TClonesArray* projectedPoint;

  if (nofRingsInEvent == 0) {
    cout << "Error no rings registered in event." << endl << endl;
  } else {
    CalculateAnglesAndDrawDistrib();
    //MatchFinder(); // PMT Mapping
    //fGP = CbmRichHitProducer::InitGeometry();
    //fGP.Print();
    //ProjectionProducer(projectedPoint);
  }
}

void CbmRichCorrectionVector::CalculateAnglesAndDrawDistrib() {
  Double_t trackX = 0., trackY = 0.;
  GetTrackPosition(trackX, trackY);

  Int_t nofRingsInEvent = fRichRings->GetEntries();
  Float_t DistCenters, Theta_Ch, Theta_0, Angles_0;
  Float_t Pi    = 3.14159265;
  Float_t TwoPi = 2. * 3.14159265;
  fPhi.resize(kMAX_NOF_HITS);

  // ------------------------- Loop to get ring center coordinates and photon hit coordinates per ring and per event ------------------------- //
  if (nofRingsInEvent >= 1) {
    cout << "Number of Rings in event: " << nofRingsInEvent << endl;
    //sleep(2);
    for (Int_t iR = 0; iR < nofRingsInEvent; iR++) {
      // ----- Convert Ring to Ring Light ----- //
      CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(iR));
      CbmRichRingLight ringL;
      CbmRichConverter::CopyHitsToRingLight(ring, &ringL);
      fCopFit->DoFit(&ringL);
      // ----- Distance between mean center and fitted center calculation ----- //
      DistCenters = sqrt(TMath::Power(ringL.GetCenterX() - trackX, 2)
                         + TMath::Power(ringL.GetCenterY() - trackY, 2));
      fHM2->H1("fHCenterDistance")->Fill(DistCenters);
      // ----- Declaration of new variables ----- //
      Int_t NofHits = ringL.GetNofHits();
      Float_t xRing = ringL.GetCenterX();
      Float_t yRing = ringL.GetCenterY();

      for (Int_t iH = 0; iH < NofHits; iH++) {
        // ----- Phi angle calculation ----- //
        Int_t HitIndex  = ring->GetHit(iH);
        CbmRichHit* hit = static_cast<CbmRichHit*>(fRichHits->At(HitIndex));
        Float_t xHit    = hit->GetX();
        Float_t yHit    = hit->GetY();
        Angles_0        = TMath::ATan2(
          (hit->GetX() - ringL.GetCenterX()),
          (ringL.GetCenterY() - hit->GetY()));  //* TMath::RadToDeg();
        //cout << "Angles_0 = " << Angles_0[iH] << endl;

        if (xRing - xHit == 0 || yRing - yHit == 0) continue;
        fPhi[iH] = TMath::ATan2(yHit - yRing, xHit - xRing);
        fHM2->H1("fHPhi")->Fill(fPhi[iH]);

        // ----- Theta_Ch and Theta_0 calculations ----- //
        Theta_Ch = sqrt(TMath::Power(trackX - hit->GetX(), 2)
                        + TMath::Power(trackY - hit->GetY(), 2));
        Theta_0  = sqrt(TMath::Power(ringL.GetCenterX() - hit->GetX(), 2)
                       + TMath::Power(ringL.GetCenterY() - hit->GetY(), 2));
        //cout << "Theta_0 = " << Theta_0 << endl;
        fHM2->H1("fHThetaDiff")->Fill(Theta_Ch - Theta_0);

        // ----- Filling of final histograms ----- //
        fHM2->H2("fHCherenkovHitsDistribTheta0")->Fill(Angles_0, Theta_0);
        fHM2->H2("fHCherenkovHitsDistribThetaCh")->Fill(fPhi[iH], Theta_Ch);
        fHM2->H2("fHCherenkovHitsDistribReduced")
          ->Fill(fPhi[iH], (Theta_Ch - Theta_0));
      }
      //cout << endl;
    }
  }
}

void CbmRichCorrectionVector::GetTrackPosition(Double_t& x, Double_t& y) {
  Int_t NofProjections = fRichProjections->GetEntries();
  //cout << "!!! NB PROJECTIONS !!! " << NofProjections << endl;
  for (Int_t iP = 0; iP < NofProjections; iP++) {
    FairTrackParam* pr = (FairTrackParam*) fRichProjections->At(iP);
    if (NULL == pr) {
      x = 0.;
      y = 0.;
      cout << "Error: CbmRichCorrectionVector::GetTrackPosition. No fair track "
              "param found."
           << endl;
    }
    x = pr->GetX();
    y = pr->GetY();
    //cout << "Center X: " << *x << " and Center y: " << *y << endl;
  }
}

void CbmRichCorrectionVector::MatchFinder() {
  cout << "//---------------------------------------- MATCH_FINDER Function "
          "----------------------------------------//"
       << endl;
  Int_t NofMirrorPoints = fRichMirrorPoints->GetEntries();
  Int_t NofRingsInEvent = fRichRings->GetEntries();
  Int_t NofMCPoints     = fRichMCPoints->GetEntriesFast();
  Int_t NofMCTracks     = fMCTracks->GetEntriesFast();
  //Int_t NofProjections = fRichProjections->GetEntries();

  Double_t x_Mirr = 0, y_Mirr = 0, z_Mirr = 0, x_PMT = 0, y_PMT = 0, z_PMT = 0;
  Double_t CenterX = 0, CenterY = 0;
  TGeoNode* mirr_node;
  const Char_t* mirr_path;
  UShort_t pHit;

  for (Int_t iMirr = 0; iMirr < NofMirrorPoints; iMirr++) {
    CbmRichPoint* MirrPoint = (CbmRichPoint*) fRichMirrorPoints->At(iMirr);
    Int_t trackID           = MirrPoint->GetTrackID();
    //cout << "Track ID = " << trackID << endl;

    for (Int_t iMCPoint = 0; iMCPoint < NofMCPoints; iMCPoint++) {
      CbmRichPoint* pPoint = (CbmRichPoint*) fRichMCPoints->At(iMCPoint);
      if (NULL == pPoint) continue;
      CbmMCTrack* pTrack = (CbmMCTrack*) fMCTracks->At(pPoint->GetTrackID());
      if (NULL == pTrack) continue;
      Int_t gcode    = pTrack->GetPdgCode();
      Int_t motherID = pTrack->GetMotherId();
      if (motherID == -1) continue;

      if (trackID == motherID) {
        //cout << "MATCH BETWEEN TRACK ID AND MOTHER ID FOUND !" << endl;
        //sleep(2);
        //cout << "TrackID from mirror point = " << trackID << " and mother ID from MC point = " << motherID << endl;
        x_Mirr = MirrPoint->GetX();
        y_Mirr = MirrPoint->GetY();
        z_Mirr = MirrPoint->GetZ();
        //cout << "x_Mirr: " << x_Mirr << ", y_Mirr: " << y_Mirr << " and z_Mirr: " << z_Mirr << endl;
        mirr_node = gGeoManager->FindNode(x_Mirr, y_Mirr, z_Mirr);
        mirr_path = gGeoManager->GetPath();
        FillPMTMap(mirr_path, pPoint);
        //break;
      }
    }

    // Loop filling the PMT map with ellipse points
    for (Int_t iRing = 0; iRing < NofRingsInEvent; iRing++) {
      CbmRichRing* ring = (CbmRichRing*) fRichRings->At(iRing);
      if (NULL == ring) continue;
      CbmRichRingLight ringL;
      CbmRichConverter::CopyHitsToRingLight(ring, &ringL);
      //fCopFit->DoFit(&ringL);
      fTauFit->DoFit(&ringL);
      CenterX = ringL.GetCenterX();
      CenterY = ringL.GetCenterY();
      CbmTrackMatchNew* richRingMatch =
        static_cast<CbmTrackMatchNew*>(fRichRingMatches->At(iRing));
      Int_t richMCID = richRingMatch->GetMatchedLink().GetIndex();
      //Int_t trackID2 = ring->GetTrackID();
      //cout << "Track ID from ring = " << richMCID << endl;
      if (richMCID == -1) continue;

      if (trackID == richMCID) {

        cout << "MATCH BETWEEN TRACK_ID AND RICH_MC_ID FOUND !" << endl;
        cout << "Center X = " << CenterX << " and center Y = " << CenterY
             << endl;
        x_Mirr = MirrPoint->GetX();
        y_Mirr = MirrPoint->GetY();
        z_Mirr = MirrPoint->GetZ();
        cout << "x_Mirr: " << x_Mirr << ", y_Mirr: " << y_Mirr
             << " and z_Mirr: " << z_Mirr << endl;
        mirr_node = gGeoManager->FindNode(x_Mirr, y_Mirr, z_Mirr);
        mirr_path = gGeoManager->GetPath();
        cout << "Mirror path: " << mirr_path << endl;

        FillPMTMapEllipse(mirr_path, ringL.GetCenterX(), ringL.GetCenterY());
        //break;
      }
    }
  }
}

void CbmRichCorrectionVector::FillPMTMap(const Char_t* mirr_path,
                                         CbmRichPoint* pPoint) {
  //cout << "//---------------------------------------- FILL_PMT_MAP Function ----------------------------------------//" << endl;
  string name = string(mirr_path);
  for (std::map<string, string>::iterator it = fPathsMap.begin();
       it != fPathsMap.end();
       ++it) {
    if (name.compare(it->first) == 0) {
      //cout << "IDENTICAL PATHS FOUND !" << endl;
      //cout << "Mirror ID: " << it->second << endl;
      Double_t x_PMT = pPoint->GetX();
      Double_t y_PMT = pPoint->GetY();
      Double_t z_PMT = pPoint->GetZ();
      //cout << "x_PMT: " << x_PMT << ", y_PMT: " << y_PMT << " and z_PMT: " << z_PMT << endl << endl;
      //sleep(1);
      fHM->H2("fHMCPoints_" + it->second)->Fill(-x_PMT, y_PMT);
      fMirrCounter++;
    }
  }
}

void CbmRichCorrectionVector::FillPMTMapEllipse(const Char_t* mirr_path,
                                                Float_t CenterX,
                                                Float_t CenterY) {
  cout << "//---------------------------------------- FILL_PMT_MAP_ELLIPSE "
          "Function ----------------------------------------//"
       << endl;
  string name = string(mirr_path);
  for (std::map<string, string>::iterator it = fPathsMap.begin();
       it != fPathsMap.end();
       ++it) {
    if (name.compare(it->first) == 0) {
      cout << "IDENTICAL PATHS FOUND !" << endl;
      //sleep(2);
      cout << "Mirror ID: " << it->second << endl;
      //cout << "Center X: " << CenterX << " and Center Y: " << CenterY << endl << endl;
      fHM->H2("fHPoints_Ellipse_" + it->second)->Fill(-CenterX, CenterY);
    }
  }
  cout << endl;
}

void CbmRichCorrectionVector::ProjectionProducer(TClonesArray* projectedPoint) {
  cout << "//------------------------------ CbmRichCorrectionVector: "
          "Projection Producer ------------------------------//"
       << endl
       << endl;

  Int_t NofMirrorPoints   = fRichMirrorPoints->GetEntriesFast();
  Int_t NofRingsInEvent   = fRichRings->GetEntries();
  Int_t NofGTracks        = fGlobalTracks->GetEntriesFast();
  Int_t NofRefPlanePoints = fRichRefPlanePoints->GetEntriesFast();
  Int_t NofPMTPoints      = fRichPoints->GetEntriesFast();

  if (fIsReconstruction) { projectedPoint->Delete(); }
  TMatrixFSym covMat(5);
  for (Int_t iMatrix = 0; iMatrix < 5; iMatrix++) {
    for (Int_t jMatrix = 0; jMatrix <= iMatrix; jMatrix++) {
      covMat(iMatrix, jMatrix) = 0;
    }
  }
  covMat(0, 0) = covMat(1, 1) = covMat(2, 2) = covMat(3, 3) = covMat(4, 4) =
    1.e-4;

  // Declaration of points coordinates.
  Double_t sphereRadius = 300., constantePMT = 0.;
  vector<Double_t> ptM(3), ptC(3), ptR1(3), momR1(3), normalPMT(3), ptR2Mirr(3),
    ptR2Center(3), ptPMirr(3), ptPR2(3);
  vector<Double_t> ptMUnCorr(3), ptCUnCorr(3), ptR2CenterUnCorr(3),
    ptR2MirrUnCorr(3), ptPMirrUnCorr(3), ptPR2UnCorr(3);
  Double_t reflectedPtCooVectSphereUnity[] = {0., 0., 0.};
  TVector3 outPos, outPosUnCorr;
  // Declaration of ring parameters.
  Double_t ringCenter[] = {0., 0., 0.}, distToExtrapTrackHit = 0.,
           distToExtrapTrackHitInPlane = 0.;
  //Declarations related to geometry.
  Int_t mirrTrackID = -1, pmtTrackID = -1, refPlaneTrackID = -1,
        motherID = -100, pmtMotherID = -100;
  CbmMCTrack *track = NULL, *track2 = NULL;
  TGeoNavigator* navi;
  TGeoNode* mirrNode;
  TGeoMatrix *mirrMatrix, *pmtMatrix, *richMatrix;

  CbmRichRecGeoPar* gp = CbmRichGeoManager::GetInstance().fGP;
  double mirrorX       = gp->fMirrorX;
  Double_t pmtPlaneX   = gp->fPmt.fPlaneX;
  double pmtPlaneY     = gp->fPmt.fPlaneY;
  double pmtWidth      = gp->fPmt.fWidth;
  double pmtHeight     = gp->fPmt.fHeight;

  GetPmtNormal(NofPMTPoints, normalPMT, constantePMT);
  cout << "Calculated normal vector to PMT plane = {" << normalPMT.at(0) << ", "
       << normalPMT.at(1) << ", " << normalPMT.at(2)
       << "} and constante d = " << constantePMT << endl
       << endl;

  for (Int_t iMirr = 0; iMirr < NofMirrorPoints; iMirr++) {
    //cout << "NofMirrorPoints = " << NofMirrorPoints << " and iMirr = " << iMirr << endl;
    CbmRichPoint* mirrPoint = (CbmRichPoint*) fRichMirrorPoints->At(iMirr);
    mirrTrackID             = mirrPoint->GetTrackID();
    //cout << "Mirror track ID = " << mirrTrackID << endl;
    if (mirrTrackID <= -1) {
      cout << "Mirror track ID <= 1 !!!" << endl;
      cout << "----------------------------------- End of loop N°" << iMirr + 1
           << " on the mirror points. -----------------------------------"
           << endl
           << endl;
      continue;
    }
    track    = (CbmMCTrack*) fMCTracks->At(mirrTrackID);
    motherID = track->GetMotherId();
    if (motherID == -1) {
      //cout << "Mirror motherID == -1 !!!" << endl << endl;
      ptM.at(0) = mirrPoint->GetX(), ptM.at(1) = mirrPoint->GetY(),
      ptM.at(2)       = mirrPoint->GetZ();
      ptMUnCorr.at(0) = -70.6622238, ptMUnCorr.at(1) = 55.1816025,
      ptMUnCorr.at(2) = 335.3621216;
      //cout << "Mirror Point coordinates; x = " << ptM.at(0) << ", y = " << ptM.at(1) << " and z = " << ptM.at(2) << endl;
      mirrNode = gGeoManager->FindNode(ptM.at(0), ptM.at(1), ptM.at(2));
      if (mirrNode) {
        cout << "Mirror node found! Mirror node name = " << mirrNode->GetName()
             << endl;
        navi = gGeoManager->GetCurrentNavigator();
        cout << "Navigator path: " << navi->GetPath() << endl;
        cout << "Coordinates of sphere center: " << endl;
        navi->GetCurrentMatrix()->Print();
        if (fIsMeanCenter)
          GetMeanSphereCenter(
            navi,
            ptC);  //IF NO INFORMATION ON MIRRORS ARE KNOWN (TO BE USED IN RECONSTRUCTION STEP) !!!
        else {
          ptC.at(0) = navi->GetCurrentMatrix()->GetTranslation()[0];
          ptC.at(1) = navi->GetCurrentMatrix()->GetTranslation()[1];
          ptC.at(2) = navi->GetCurrentMatrix()->GetTranslation()[2];
        }
        ptCUnCorr.at(0) = 0., ptCUnCorr.at(1) = 132.594000,
        ptCUnCorr.at(2) = 54.267226;
        cout << "Coordinates of tile center: " << endl;
        navi->GetMotherMatrix()->Print();
        cout << endl
             << "Sphere center coordinates of the rotated mirror tile = {"
             << ptC.at(0) << ", " << ptC.at(1) << ", " << ptC.at(2)
             << "} and sphere inner radius = " << sphereRadius << endl;

        for (Int_t iRefl = 0; iRefl < NofRefPlanePoints; iRefl++) {
          //new((*projectedPoint)[iRefl]) FairTrackParam(0., 0., 0., 0., 0., 0., covMat);
          CbmRichPoint* refPlanePoint =
            (CbmRichPoint*) fRichRefPlanePoints->At(iRefl);
          refPlaneTrackID = refPlanePoint->GetTrackID();
          //cout << "Reflective plane track ID = " << refPlaneTrackID << endl;
          if (mirrTrackID == refPlaneTrackID) {
            //cout << "IDENTICAL TRACK ID FOUND !!!" << endl << endl;
            ptR1.at(0)  = refPlanePoint->GetX(),
            ptR1.at(1)  = refPlanePoint->GetY(),
            ptR1.at(2)  = refPlanePoint->GetZ();
            momR1.at(0) = refPlanePoint->GetPx(),
            momR1.at(1) = refPlanePoint->GetPy(),
            momR1.at(2) = refPlanePoint->GetPz();
            cout << "Reflective Plane Point coordinates = {" << ptR1.at(0)
                 << ", " << ptR1.at(1) << ", " << ptR1.at(2) << "}" << endl;
            cout << "And reflective Plane Point momenta = {" << momR1.at(0)
                 << ", " << momR1.at(1) << ", " << momR1.at(2) << "}" << endl;
            cout << "Mirror Point coordinates = {" << ptM.at(0) << ", "
                 << ptM.at(1) << ", " << ptM.at(2) << "}" << endl
                 << endl;
            cout << "Mirror Point coordinates (Uncorrected) = {"
                 << ptMUnCorr.at(0) << ", " << ptMUnCorr.at(1) << ", "
                 << ptMUnCorr.at(2) << "}" << endl
                 << endl;

            if (fIsMeanCenter) {
              GetMirrorIntersection(ptM, ptR1, momR1, ptC, sphereRadius);
              // From ptM: how to retrieve tile ID ???
              // => Compare distance of ptM to tile centers
            }

            ComputeR2(ptR2Center, ptR2Mirr, ptC, ptM, ptR1);
            ComputeR2(ptR2CenterUnCorr, ptR2MirrUnCorr, ptCUnCorr, ptM, ptR1);

            ComputeP(ptPMirr, ptPR2, normalPMT, ptM, ptR2Mirr, constantePMT);
            ComputeP(ptPMirrUnCorr,
                     ptPR2UnCorr,
                     normalPMT,
                     ptM,
                     ptR2MirrUnCorr,
                     constantePMT);

            TVector3 inPos(ptPMirr.at(0), ptPMirr.at(1), ptPMirr.at(2));
            CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);
            cout << "New mirror points coordinates = {" << outPos.x() << ", "
                 << outPos.y() << ", " << outPos.z() << "}" << endl;

            TVector3 inPosUnCorr(
              ptPMirrUnCorr.at(0), ptPMirrUnCorr.at(1), ptPMirrUnCorr.at(2));
            CbmRichGeoManager::GetInstance().RotatePoint(&inPosUnCorr,
                                                         &outPosUnCorr);
            cout << "New mirror points coordinates = {" << outPosUnCorr.x()
                 << ", " << outPosUnCorr.y() << ", " << outPosUnCorr.z() << "}"
                 << endl
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

        FillHistProjection(
          outPos, outPosUnCorr, NofGTracks, normalPMT, constantePMT);
      } else {
        cout << "Not a mother particle ..." << endl;
      }
      cout << "----------------------------------- "
           << "End of loop N°" << iMirr + 1 << " on the mirror points."
           << " -----------------------------------" << endl
           << endl;
    }
  }
}

void CbmRichCorrectionVector::GetPmtNormal(Int_t NofPMTPoints,
                                           vector<Double_t>& normalPMT,
                                           Double_t& normalCste) {
  //cout << endl << "//------------------------------ CbmRichCorrectionVector: Calculate PMT Normal ------------------------------//" << endl << endl;

  Int_t pmtTrackID, pmtMotherID;
  Double_t buffNormX = 0., buffNormY = 0., buffNormZ = 0., k = 0.,
           scalarProd = 0.;
  Double_t pmtPt[]    = {0., 0., 0.};
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
    if (TMath::Sqrt(TMath::Power(a[0] - pmtPoint->GetX(), 2)
                    + TMath::Power(a[1] - pmtPoint->GetY(), 2)
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
    if (TMath::Sqrt(TMath::Power(a[0] - pmtPoint->GetX(), 2)
                    + TMath::Power(a[1] - pmtPoint->GetY(), 2)
                    + TMath::Power(a[2] - pmtPoint->GetZ(), 2))
          > 7
        && TMath::Sqrt(TMath::Power(b[0] - pmtPoint->GetX(), 2)
                       + TMath::Power(b[1] - pmtPoint->GetY(), 2)
                       + TMath::Power(b[2] - pmtPoint->GetZ(), 2))
             > 7) {
      c[0] = pmtPoint->GetX(), c[1] = pmtPoint->GetY(), c[2] = pmtPoint->GetZ();
      //cout << "c[0] = " << c[0] << ", c[1] = " << c[1] << " et c[2] = " << c[2] << endl;
      break;
    }
  }

  k = (b[0] - a[0]) / (c[0] - a[0]);
  if ((b[1] - a[1]) - (k * (c[1] - a[1])) == 0
      || (b[2] - a[2]) - (k * (c[2] - a[2])) == 0) {
    cout << "Error in normal calculation, vect_AB and vect_AC are collinear."
         << endl;
  } else {
    buffNormX = (b[1] - a[1]) * (c[2] - a[2]) - (b[2] - a[2]) * (c[1] - a[1]);
    buffNormY = (b[2] - a[2]) * (c[0] - a[0]) - (b[0] - a[0]) * (c[2] - a[2]);
    buffNormZ = (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);
    normalPMT.at(0) =
      buffNormX
      / TMath::Sqrt(TMath::Power(buffNormX, 2) + TMath::Power(buffNormY, 2)
                    + TMath::Power(buffNormZ, 2));
    normalPMT.at(1) =
      buffNormY
      / TMath::Sqrt(TMath::Power(buffNormX, 2) + TMath::Power(buffNormY, 2)
                    + TMath::Power(buffNormZ, 2));
    normalPMT.at(2) =
      buffNormZ
      / TMath::Sqrt(TMath::Power(buffNormX, 2) + TMath::Power(buffNormY, 2)
                    + TMath::Power(buffNormZ, 2));
  }

  CbmRichPoint* pmtPoint1 = (CbmRichPoint*) fRichPoints->At(20);
  scalarProd              = normalPMT.at(0) * (pmtPoint1->GetX() - a[0])
               + normalPMT.at(1) * (pmtPoint1->GetY() - a[1])
               + normalPMT.at(2) * (pmtPoint1->GetZ() - a[2]);
  //cout << "1st scalar product between vectAM and normale = " << scalarProd << endl;
  // To determine the constant term of the plane equation, inject the coordinates of a pmt point, which should solve it: a*x+b*y+c*z+d=0.
  normalCste =
    -1
    * (normalPMT.at(0) * pmtPoint1->GetX() + normalPMT.at(1) * pmtPoint1->GetY()
       + normalPMT.at(2) * pmtPoint1->GetZ());
  CbmRichPoint* pmtPoint2 = (CbmRichPoint*) fRichPoints->At(15);
  scalarProd              = normalPMT.at(0) * (pmtPoint2->GetX() - a[0])
               + normalPMT.at(1) * (pmtPoint2->GetY() - a[1])
               + normalPMT.at(2) * (pmtPoint2->GetZ() - a[2]);
  //cout << "2nd scalar product between vectAM and normale = " << scalarProd << endl;
  CbmRichPoint* pmtPoint3 = (CbmRichPoint*) fRichPoints->At(25);
  scalarProd              = normalPMT.at(0) * (pmtPoint3->GetX() - a[0])
               + normalPMT.at(1) * (pmtPoint3->GetY() - a[1])
               + normalPMT.at(2) * (pmtPoint3->GetZ() - a[2]);
  //cout << "3nd scalar product between vectAM and normale = " << scalarProd << endl;
}

void CbmRichCorrectionVector::GetMeanSphereCenter(TGeoNavigator* navi,
                                                  vector<Double_t>& ptC) {
  const Char_t* topNodePath;
  topNodePath = gGeoManager->GetTopNode()->GetName();
  cout << "Top node path: " << topNodePath << endl;
  TGeoVolume* rootTop;
  rootTop = gGeoManager->GetTopVolume();
  rootTop->Print();

  TGeoIterator nextNode(rootTop);
  TGeoNode* curNode;
  const TGeoMatrix* curMatrix;
  const Double_t*
    curNodeTranslation;  // 3 components - pointers to some memory which is provided by ROOT
  const Double_t*
    curNodeRotationM;  // 9 components - pointers to some memory which is provided by ROOT
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
    if (curNode->GetVolume()->GetName() == filterName0
        || curNode->GetVolume()->GetName() == filterName1
        || curNode->GetVolume()->GetName() == filterName2
        || curNode->GetVolume()->GetName() == filterName3
        || curNode->GetVolume()->GetName() == filterName4
        || curNode->GetVolume()->GetName() == filterName5) {
      if (curNode->GetNdaughters() == 0) {
        // All deepest nodes of mirror tiles here (leaves)
        // Thus we get spherical surface centers
        nextNode.GetPath(nodePath);
        curMatrix          = nextNode.GetCurrentMatrix();
        curNodeTranslation = curMatrix->GetTranslation();
        curNodeRotationM   = curMatrix->GetRotationMatrix();
        printf("%s tr:\t", nodePath.Data());
        printf("%08f\t%08f\t%08f\t\n",
               curNodeTranslation[0],
               curNodeTranslation[1],
               curNodeTranslation[2]);
        if (curNodeTranslation[1]
            > 0) {  // CONDITION FOR UPPER MIRROR WALL STUDY
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

void CbmRichCorrectionVector::GetMirrorIntersection(vector<Double_t>& ptM,
                                                    vector<Double_t> ptR1,
                                                    vector<Double_t> momR1,
                                                    vector<Double_t> ptC,
                                                    Double_t sphereRadius) {
  Double_t a = 0., b = 0., c = 0., d = 0., k0 = 0., k1 = 0., k2 = 0.;

  a = TMath::Power(momR1.at(0), 2) + TMath::Power(momR1.at(1), 2)
      + TMath::Power(momR1.at(2), 2);
  b = 2
      * (momR1.at(0) * (ptR1.at(0) - ptC.at(0))
         + momR1.at(1) * (ptR1.at(1) - ptC.at(1))
         + momR1.at(2) * (ptR1.at(2) - ptC.at(2)));
  c = TMath::Power(ptR1.at(0) - ptC.at(0), 2)
      + TMath::Power(ptR1.at(1) - ptC.at(1), 2)
      + TMath::Power(ptR1.at(2) - ptC.at(2), 2) - TMath::Power(sphereRadius, 2);
  d = b * b - 4 * a * c;
  cout << "d = " << d << endl;

  if (d < 0) {
    cout
      << "Error no solution to degree 2 equation found ; discriminant below 0."
      << endl;
    ptM.at(0) = 0., ptM.at(1) = 0., ptM.at(2) = 0.;
  } else if (d == 0) {
    cout << "One solution to degree 2 equation found." << endl;
    k0        = -b / (2 * a);
    ptM.at(0) = ptR1.at(0) + k0 * momR1.at(0);
    ptM.at(1) = ptR1.at(1) + k0 * momR1.at(1);
    ptM.at(2) = ptR1.at(2) + k0 * momR1.at(2);
  } else if (d > 0) {
    cout << "Two solutions to degree 2 equation found." << endl;
    k1 = ((-b - TMath::Sqrt(d)) / (2 * a));
    k2 = ((-b + TMath::Sqrt(d)) / (2 * a));

    if (ptR1.at(2) + k1 * momR1.at(2) > ptR1.at(2) + k2 * momR1.at(2)) {
      ptM.at(0) = ptR1.at(0) + k1 * momR1.at(0);
      ptM.at(1) = ptR1.at(1) + k1 * momR1.at(1);
      ptM.at(2) = ptR1.at(2) + k1 * momR1.at(2);
    } else if (ptR1.at(2) + k1 * momR1.at(2) < ptR1.at(2) + k2 * momR1.at(2)) {
      ptM.at(0) = ptR1.at(0) + k2 * momR1.at(0);
      ptM.at(1) = ptR1.at(1) + k2 * momR1.at(1);
      ptM.at(2) = ptR1.at(2) + k2 * momR1.at(2);
    }
  }
}

void CbmRichCorrectionVector::ComputeR2(vector<Double_t>& ptR2Center,
                                        vector<Double_t>& ptR2Mirr,
                                        vector<Double_t> ptM,
                                        vector<Double_t> ptC,
                                        vector<Double_t> ptR1) {
  vector<Double_t> normalMirr(3);
  Double_t t1 = 0., t2 = 0., t3 = 0.;

  normalMirr.at(0) = (ptC.at(0) - ptM.at(0))
                     / TMath::Sqrt(TMath::Power(ptC.at(0) - ptM.at(0), 2)
                                   + TMath::Power(ptC.at(1) - ptM.at(1), 2)
                                   + TMath::Power(ptC.at(2) - ptM.at(2), 2));
  normalMirr.at(1) = (ptC.at(1) - ptM.at(1))
                     / TMath::Sqrt(TMath::Power(ptC.at(0) - ptM.at(0), 2)
                                   + TMath::Power(ptC.at(1) - ptM.at(1), 2)
                                   + TMath::Power(ptC.at(2) - ptM.at(2), 2));
  normalMirr.at(2) = (ptC.at(2) - ptM.at(2))
                     / TMath::Sqrt(TMath::Power(ptC.at(0) - ptM.at(0), 2)
                                   + TMath::Power(ptC.at(1) - ptM.at(1), 2)
                                   + TMath::Power(ptC.at(2) - ptM.at(2), 2));
  //cout << "Calculated and normalized normal of mirror tile = {" << normalMirr.at(0) << ", " << normalMirr.at(1) << ", " << normalMirr.at(2) << "}" << endl;

  t1 = ((ptR1.at(0) - ptM.at(0)) * (ptC.at(0) - ptM.at(0))
        + (ptR1.at(1) - ptM.at(1)) * (ptC.at(1) - ptM.at(1))
        + (ptR1.at(2) - ptM.at(2)) * (ptC.at(2) - ptM.at(2)))
       / (TMath::Power(ptC.at(0) - ptM.at(0), 2)
          + TMath::Power(ptC.at(1) - ptM.at(1), 2)
          + TMath::Power(ptC.at(2) - ptM.at(2), 2));
  ptR2Center.at(0) =
    2 * (ptM.at(0) + t1 * (ptC.at(0) - ptM.at(0))) - ptR1.at(0);
  ptR2Center.at(1) =
    2 * (ptM.at(1) + t1 * (ptC.at(1) - ptM.at(1))) - ptR1.at(1);
  ptR2Center.at(2) =
    2 * (ptM.at(2) + t1 * (ptC.at(2) - ptM.at(2))) - ptR1.at(2);
  t2 = ((ptR1.at(0) - ptC.at(0)) * (ptC.at(0) - ptM.at(0))
        + (ptR1.at(1) - ptC.at(1)) * (ptC.at(1) - ptM.at(1))
        + (ptR1.at(2) - ptC.at(2)) * (ptC.at(2) - ptM.at(2)))
       / (TMath::Power(ptC.at(0) - ptM.at(0), 2)
          + TMath::Power(ptC.at(1) - ptM.at(1), 2)
          + TMath::Power(ptC.at(2) - ptM.at(2), 2));
  ptR2Mirr.at(0) = 2 * (ptC.at(0) + t2 * (ptC.at(0) - ptM.at(0))) - ptR1.at(0);
  ptR2Mirr.at(1) = 2 * (ptC.at(1) + t2 * (ptC.at(1) - ptM.at(1))) - ptR1.at(1);
  ptR2Mirr.at(2) = 2 * (ptC.at(2) + t2 * (ptC.at(2) - ptM.at(2))) - ptR1.at(2);
  /*//SAME AS calculation of t2 above
	t3 = ((ptR1.at(0)-ptC.at(0))*(ptC.at(0)-ptM.at(0)) + (ptR1.at(1)-ptC.at(1))*(ptC.at(1)-ptM.at(1)) + (ptR1.at(2)-ptC.at(2))*(ptC.at(2)-ptM.at(2)))/TMath::Sqrt(TMath::Power(ptC.at(0) - ptM.at(0),2)+TMath::Power(ptC.at(1) - ptM.at(1),2)+TMath::Power(ptC.at(2) - ptM.at(2),2));
	reflectedPtCooVectSphereUnity[0] = 2*(ptC.at(0)+t3*(normalMirr.at(0)))-ptR1.at(0);
	reflectedPtCooVectSphereUnity[1] = 2*(ptC.at(1)+t3*(normalMirr.at(1)))-ptR1.at(1);
	reflectedPtCooVectSphereUnity[2] = 2*(ptC.at(2)+t3*(normalMirr.at(2)))-ptR1.at(2);*/
  cout << "Coordinates of point R2 on reflective plane after reflection on the "
          "mirror tile:"
       << endl;
  //cout << "* using mirror point M to define \U00000394: {" << ptR2Center.at(0) << ", " << ptR2Center.at(1) << ", " << ptR2Center.at(2) << "}" << endl;
  cout << "* using sphere center C to define \U00000394: {" << ptR2Mirr.at(0)
       << ", " << ptR2Mirr.at(1) << ", " << ptR2Mirr.at(2) << "}" << endl
       << endl;
  //cout << "Ref Pt Coo using unity Mirror-Sphere vector & sphere pt = {" << reflectedPtCooVectSphereUnity[0] << ", " << reflectedPtCooVectSphereUnity[1] << ", " << reflectedPtCooVectSphereUnity[2] << "}" << endl << endl;
  //cout << "NofPMTPoints = " << NofPMTPoints << endl;
}

void CbmRichCorrectionVector::ComputeP(vector<Double_t>& ptPMirr,
                                       vector<Double_t>& ptPR2,
                                       vector<Double_t> normalPMT,
                                       vector<Double_t> ptM,
                                       vector<Double_t> ptR2Mirr,
                                       Double_t constantePMT) {
  Double_t k1 = 0., k2 = 0., checkCalc1 = 0., checkCalc2 = 0.;

  k1 = -1
       * ((normalPMT.at(0) * ptM.at(0) + normalPMT.at(1) * ptM.at(1)
           + normalPMT.at(2) * ptM.at(2) + constantePMT)
          / (normalPMT.at(0) * (ptR2Mirr.at(0) - ptM.at(0))
             + normalPMT.at(1) * (ptR2Mirr.at(1) - ptM.at(1))
             + normalPMT.at(2) * (ptR2Mirr.at(2) - ptM.at(2))));
  ptPMirr.at(0) = ptM.at(0) + k1 * (ptR2Mirr.at(0) - ptM.at(0));
  ptPMirr.at(1) = ptM.at(1) + k1 * (ptR2Mirr.at(1) - ptM.at(1));
  ptPMirr.at(2) = ptM.at(2) + k1 * (ptR2Mirr.at(2) - ptM.at(2));
  k2            = -1
       * ((normalPMT.at(0) * ptR2Mirr.at(0) + normalPMT.at(1) * ptR2Mirr.at(1)
           + normalPMT.at(2) * ptR2Mirr.at(2) + constantePMT)
          / (normalPMT.at(0) * (ptR2Mirr.at(0) - ptM.at(0))
             + normalPMT.at(1) * (ptR2Mirr.at(1) - ptM.at(1))
             + normalPMT.at(2) * (ptR2Mirr.at(2) - ptM.at(2))));
  ptPR2.at(0) = ptR2Mirr.at(0) + k2 * (ptR2Mirr.at(0) - ptM.at(0));
  ptPR2.at(1) = ptR2Mirr.at(1) + k2 * (ptR2Mirr.at(1) - ptM.at(1));
  ptPR2.at(2) = ptR2Mirr.at(2) + k2 * (ptR2Mirr.at(2) - ptM.at(2));
  cout << "Coordinates of point P on PMT plane, after reflection on the mirror "
          "tile and extrapolation to the PMT plane:"
       << endl;
  cout << "* using mirror point M to define \U0001D49F ': {" << ptPMirr.at(0)
       << ", " << ptPMirr.at(1) << ", " << ptPMirr.at(2) << "}" << endl;
  //cout << "* using reflected point R2 to define \U0001D49F ': {" << ptPR2.at(0) << ", " << ptPR2.at(1) << ", " << ptPR2.at(2) << "}" << endl;
  checkCalc1 = ptPMirr.at(0) * normalPMT.at(0) + ptPMirr.at(1) * normalPMT.at(1)
               + ptPMirr.at(2) * normalPMT.at(2) + constantePMT;
  cout << "Check whether extrapolated track point on PMT plane verifies its "
          "equation (value should be 0.):"
       << endl;
  cout << "* using mirror point M, checkCalc = " << checkCalc1 << endl;
  checkCalc2 = ptPR2.at(0) * normalPMT.at(0) + ptPR2.at(1) * normalPMT.at(1)
               + ptPR2.at(2) * normalPMT.at(2) + constantePMT;
  //cout << "* using reflected point R2, checkCalc = " << checkCalc2 << endl << endl;
}

void CbmRichCorrectionVector::FillHistProjection(TVector3 outPos,
                                                 TVector3 outPosUnCorr,
                                                 Int_t NofGTracks,
                                                 vector<Double_t> normalPMT,
                                                 Double_t constantePMT) {
  CbmMCTrack* track2    = NULL;
  Double_t ringCenter[] = {0, 0, 0}, distToExtrapTrackHit = 0,
           distToExtrapTrackHitInPlane       = 0;
  Double_t distToExtrapTrackHitInPlaneUnCorr = 0;

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
    //RotateAndCopyHitsToRingLight(ring, &ringL);
    fCopFit->DoFit(&ringL);
    //fTauFit->DoFit(&ringL);
    ringCenter[0] = ringL.GetCenterX();
    ringCenter[1] = ringL.GetCenterY();
    ringCenter[2] = -1
                    * ((normalPMT.at(0) * ringCenter[0]
                        + normalPMT.at(1) * ringCenter[1] + constantePMT)
                       / normalPMT.at(2));

    vector<Double_t> r(3),
      p(3);  // Absolute coordinates of fitted ring Center r and PMT extrapolated point p
    r.at(0) = TMath::Abs(ringCenter[0]), r.at(1) = TMath::Abs(ringCenter[1]),
    r.at(2) = TMath::Abs(ringCenter[2]);
    p.at(0) = TMath::Abs(outPos.x()), p.at(1) = TMath::Abs(outPos.y()),
    p.at(2) = TMath::Abs(outPos.z());
    cout << "Ring center coordinates = {" << ringCenter[0] << ", "
         << ringCenter[1] << ", " << ringCenter[2] << "}" << endl;
    cout << "Difference in X = " << TMath::Abs(r.at(0) - p.at(0)) << "\t ; \t"
         << "Difference in Y = " << TMath::Abs(r.at(1) - p.at(1)) << "\t ; \t"
         << "Difference in Z = " << TMath::Abs(r.at(2) - p.at(2)) << endl;
    fHM->H1("fhDifferenceX")->Fill(TMath::Abs(r.at(0) - p.at(0)));
    fHM->H1("fhDifferenceY")->Fill(TMath::Abs(r.at(1) - p.at(1)));
    distToExtrapTrackHit        = TMath::Sqrt(TMath::Power(r.at(0) - p.at(0), 2)
                                       + TMath::Power(r.at(1) - p.at(1), 2)
                                       + TMath::Power(r.at(2) - p.at(2), 2));
    distToExtrapTrackHitInPlane = TMath::Sqrt(
      TMath::Power(r.at(0) - p.at(0), 2) + TMath::Power(r.at(1) - p.at(1), 2));
    fHM->H1("fhDistanceCenterToExtrapolatedTrack")->Fill(distToExtrapTrackHit);
    fHM->H1("fhDistanceCenterToExtrapolatedTrackInPlane")
      ->Fill(distToExtrapTrackHitInPlane);
    cout << "Distance between fitted ring center and extrapolated track hit = "
         << distToExtrapTrackHit << endl;
    cout << "Distance between fitted ring center and extrapolated track hit in "
            "plane = "
         << distToExtrapTrackHitInPlane << endl;

    vector<Double_t> pUnCorr(
      3);  // Absolute coordinates of fitted ring Center r and PMT extrapolated point p
    pUnCorr.at(0) = TMath::Abs(outPosUnCorr.x()),
    pUnCorr.at(1) = TMath::Abs(outPosUnCorr.y()),
    pUnCorr.at(2) = TMath::Abs(outPosUnCorr.z());
    //cout << "Ring center coordinates = {" << ringCenter[0] << ", " << ringCenter[1] << ", " << ringCenter[2] << "}" << endl;
    cout << "Difference in X = " << TMath::Abs(r.at(0) - pUnCorr.at(0))
         << "\t ; \t"
         << "Difference in Y = " << TMath::Abs(r.at(1) - pUnCorr.at(1))
         << "\t ; \t"
         << "Difference in Z = " << TMath::Abs(r.at(2) - pUnCorr.at(2)) << endl;
    fHM->H1("fhDifferenceXUncorrected")
      ->Fill(TMath::Abs(r.at(0) - pUnCorr.at(0)));
    fHM->H1("fhDifferenceYUncorrected")
      ->Fill(TMath::Abs(r.at(1) - pUnCorr.at(1)));
    distToExtrapTrackHitInPlaneUnCorr =
      TMath::Sqrt(TMath::Power(r.at(0) - pUnCorr.at(0), 2)
                  + TMath::Power(r.at(1) - pUnCorr.at(1), 2));
    fHM->H1("fhDistanceCenterToExtrapolatedTrackInPlaneUncorrected")
      ->Fill(distToExtrapTrackHitInPlaneUnCorr);
    cout << "Distance between fitted ring center and extrapolated track hit in "
            "plane = "
         << distToExtrapTrackHitInPlaneUnCorr << endl
         << endl;
    //}
    //else { cout << "No identical ring mother ID and mirror track ID ..." << endl;}
  }
  cout << "End of loop on global tracks;" << endl;
}

void CbmRichCorrectionVector::RotateAndCopyHitsToRingLight(
  const CbmRichRing* ring1,
  CbmRichRingLight* ring2) {
  Int_t nofHits = ring1->GetNofHits();

  for (Int_t i = 0; i < nofHits; i++) {
    Int_t hitInd    = ring1->GetHit(i);
    CbmRichHit* hit = (CbmRichHit*) fRichHits->At(hitInd);
    if (NULL == hit) continue;
    TVector3 inputHit(hit->GetX(), hit->GetY(), hit->GetZ());
    TVector3 outputHit(0, 0, 0);
    //CbmRichHitProducer::TiltPoint(&inputHit, &outputHit, fGP.fPmtPhi, fGP.fPmtTheta, fGP.fPmtZOrig);
    CbmRichHitLight hl(outputHit.X(), outputHit.Y());
    ring2->AddHit(hl);
  }
}

void CbmRichCorrectionVector::DrawHistAlignment() {
  TCanvas* c1 = new TCanvas(fRunTitle + "_Main_Analysis_" + fAxisRotTitle,
                            fRunTitle + "_Main_Analysis_" + fAxisRotTitle,
                            1500,
                            600);
  c1->Divide(3, 2);
  c1->cd(1);
  /*c1->SetGridx(1);
	c1->SetGridy(1);
	fHM2->H1("fHCenterDistance")->GetXaxis()->SetLabelSize(0.03);
	fHM2->H1("fHCenterDistance")->GetXaxis()->SetTitleSize(0.03);
	fHM2->H1("fHCenterDistance")->GetXaxis()->CenterTitle();
	fHM2->H1("fHCenterDistance")->GetXaxis()->SetNdivisions(612,kTRUE);
	fHM2->H1("fHCenterDistance")->GetYaxis()->CenterTitle();
	fHM2->H1("fHCenterDistance")->GetYaxis()->SetTitleSize(0.03);
	fHM2->H1("fHCenterDistance")->GetYaxis()->SetTitleOffset(1.);
	fHM2->H1("fHCenterDistance")->Draw();*/
  DrawH1(fHM2->H1("fHCenterDistance"));
  c1->cd(2);
  DrawH1(fHM2->H1("fHThetaDiff"));
  c1->cd(3);
  DrawH2(fHM2->H2("fHCherenkovHitsDistribTheta0"));
  c1->cd(4);
  DrawH1(fHM2->H1("fHPhi"));
  c1->cd(5);
  DrawH2(fHM2->H2("fHCherenkovHitsDistribThetaCh"));
  c1->cd(6);
  DrawH2(fHM2->H2("fHCherenkovHitsDistribReduced"));
  Cbm::SaveCanvasAsImage(c1, string(fOutputDir.Data()), "png");
}

void CbmRichCorrectionVector::DrawFit(vector<Double_t>& outputFit,
                                      Int_t thresh) {
  //vector<Double_t> paramVect;
  //paramVect.reserve(5);

  TCanvas* c3 = new TCanvas(fRunTitle + "_Fit_Slices_" + fAxisRotTitle,
                            fRunTitle + "_Fit_Slices_" + fAxisRotTitle,
                            1100,
                            600);
  c3->SetFillColor(42);
  c3->Divide(4, 2);
  gPad->SetTopMargin(0.1);
  gPad->SetFillColor(33);
  c3->cd(1);
  //	TH2D* CloneArr = (TH2D*)fHM2->H2("fHCherenkovHitsDistribThetaCh")->Clone();
  TH2D* CloneArr = (TH2D*) fHM2->H2("fHCherenkovHitsDistribReduced")->Clone();
  CloneArr->GetXaxis()->SetLabelSize(0.03);
  CloneArr->GetXaxis()->SetTitleSize(0.03);
  CloneArr->GetXaxis()->CenterTitle();
  CloneArr->GetXaxis()->SetNdivisions(612, kTRUE);
  CloneArr->GetYaxis()->SetLabelSize(0.03);
  CloneArr->GetYaxis()->SetTitleSize(0.03);
  CloneArr->GetYaxis()->SetNdivisions(612, kTRUE);
  CloneArr->GetYaxis()->CenterTitle();
  //CloneArr->GetYaxis()->SetRangeUser(-2.5,2.5);
  CloneArr->GetZaxis()->SetLabelSize(0.03);
  CloneArr->GetZaxis()->SetTitleSize(0.03);
  CloneArr->GetYaxis()->SetTitleOffset(1.0);
  CloneArr->Draw("colz");
  //Double_t ymax = CloneArr->GetYaxis()->GetXmax();
  //Double_t ymin = CloneArr->GetYaxis()->GetXmin();
  //TF1 *fgauss = TF1 fgauss("gauss", "[0]*exp(-0.5*((x-[1])/[2])**2)", 0, 100);

  // ------------------------------ APPLY THRESHOLD TO 2D-HISTO ------------------------------ //
  //	TH2D* CloneArr_2 = (TH2D*)fHM2->H2("fHCherenkovHitsDistribThetaCh")->Clone();
  TH2D* CloneArr_2 = (TH2D*) fHM2->H2("fHCherenkovHitsDistribReduced")->Clone();
  for (Int_t y_bin = 1; y_bin <= 500; y_bin++) {
    for (Int_t x_bin = 1; x_bin <= 200; x_bin++) {
      /*if (CloneArr_2->GetBinContent(x_bin, y_bin)!=0) {
			cout << "Bin Content: " << CloneArr_2->GetBinContent(x_bin, y_bin) << endl;
			sleep(1);
			}
			else;*/
      if (CloneArr_2->GetBinContent(x_bin, y_bin) < thresh) {
        CloneArr_2->SetBinContent(x_bin, y_bin, 0);
      }
    }
  }
  c3->cd(2);
  CloneArr_2->GetXaxis()->SetLabelSize(0.03);
  CloneArr_2->GetXaxis()->SetTitleSize(0.03);
  CloneArr_2->GetXaxis()->CenterTitle();
  CloneArr_2->GetXaxis()->SetNdivisions(612, kTRUE);
  CloneArr_2->GetYaxis()->SetLabelSize(0.03);
  CloneArr_2->GetYaxis()->SetTitleSize(0.03);
  CloneArr_2->GetYaxis()->SetNdivisions(612, kTRUE);
  CloneArr_2->GetYaxis()->CenterTitle();
  //CloneArr_2->GetYaxis()->SetRangeUser(-2.5,2.5);
  CloneArr_2->GetZaxis()->SetLabelSize(0.03);
  CloneArr_2->GetZaxis()->SetTitleSize(0.03);
  CloneArr_2->GetYaxis()->SetTitleOffset(1.0);
  CloneArr_2->Draw("colz");
  CloneArr_2->Write();

  // -------------------- FIT SLICES AND FIT THE MEAN OF THE RESULT TO A SIN FUNCTION -------------------- //
  CloneArr_2->FitSlicesY(0, 0, -1, 1);
  c3->cd(3);
  TH1D* histo_0 = (TH1D*) gDirectory->Get("fHCherenkovHitsDistribReduced_0");
  histo_0->Draw();
  c3->cd(4);
  TH1D* histo_1 = (TH1D*) gDirectory->Get("fHCherenkovHitsDistribReduced_1");
  //histo_1->GetYaxis()->SetRangeUser(-2.5, 2.5);
  histo_1->Draw();
  c3->cd(5);
  TH1D* histo_2 = (TH1D*) gDirectory->Get("fHCherenkovHitsDistribReduced_2");
  histo_2->Draw();
  c3->cd(6);
  TH1D* histo_chi2 =
    (TH1D*) gDirectory->Get("fHCherenkovHitsDistribReduced_chi2");
  histo_chi2->Draw();

  c3->cd(7);
  TF1* f1 = new TF1("f1", "[2]+[0]*cos(x)+[1]*sin(x)", -3.5, 3.5);
  f1->SetParameters(0, 0, 0);
  f1->SetParNames("Delta_phi", "Delta_lambda", "Offset");
  histo_1->Fit("f1", "", "");
  TF1* fit      = histo_1->GetFunction("f1");
  Double_t p1   = fit->GetParameter("Delta_phi");
  Double_t p2   = fit->GetParameter("Delta_lambda");
  Double_t p3   = fit->GetParameter("Offset");
  Double_t chi2 = fit->GetChisquare();
  //cout << setprecision(6) << "Delta_phi = " << fit->GetParameter(0) << " and delta_lambda = " << fit->GetParameter(1) << endl;
  //cout << "Delta_phi error = " << fit->GetParError(0) << " and delta_lambda error = " << fit->GetParError(1) << endl;
  //cout << endl << "Chi2: " << chi2 << endl;

  /*	paramVect.push_back(fit->GetParameter("Delta_phi"));
	paramVect.push_back(fit->GetParameter("Delta_lambda"));
	paramVect.push_back(fit->GetParameter("Offset"));
	paramVect.push_back(fit->GetChisquare());
	//cout << "Vectors: Delta_phi = " << paramVect[0] << ", Delta_lambda = " << paramVect[1] << ", Offset = " << paramVect[2] << endl;
*/
  f1->SetParameters(fit->GetParameter(0), fit->GetParameter(1));
  char leg[128];
  f1->SetLineColor(2);
  f1->Draw();
  f1->Write();

  // ------------------------------ CALCULATION OF MISALIGNMENT ANGLE ------------------------------ //
  Double_t Focal_length = 150., q = 0., A = 0., Alpha = 0., mis_x = 0.,
           mis_y = 0.;
  q              = TMath::ATan(fit->GetParameter(0) / fit->GetParameter(1));
  cout << endl
       << "fit_1 = " << fit->GetParameter(0)
       << " and fit_2 = " << fit->GetParameter(1) << endl;
  //cout << "q = " << q << endl;
  A = fit->GetParameter(1) / TMath::Cos(q);
  //cout << "Parameter a = " << A << endl;
  Alpha =
    TMath::ATan(A / 1.5) * 0.5
    * TMath::Power(
      10,
      3);  // *0.5, because a mirror rotation of alpha implies a rotation in the particle trajectory of 2*alpha ; 1.5 meters = Focal length = Radius_of_curvature/2
  //cout << setprecision(6) << "Total angle of misalignment alpha = " << Alpha << endl;       // setprecision(#) gives the number of digits in the cout.
  mis_x = TMath::ATan(fit->GetParameter(0) / Focal_length) * 0.5
          * TMath::Power(10, 3);
  mis_y = TMath::ATan(fit->GetParameter(1) / Focal_length) * 0.5
          * TMath::Power(10, 3);
  //cout << "Horizontal displacement = " << outputFit[0] << " [mrad] and vertical displacement = " << outputFit[1] << " [mrad]." << endl;

  TLegend* LEG = new TLegend(0.30, 0.7, 0.72, 0.85);  // Set legend position
  LEG->SetBorderSize(1);
  LEG->SetFillColor(0);
  LEG->SetMargin(0.2);
  LEG->SetTextSize(0.03);
  sprintf(leg, "Fitted sinusoid");
  LEG->AddEntry(f1, leg, "l");
  sprintf(leg, "Misalign in X = %f", mis_x);
  LEG->AddEntry("", leg, "l");
  sprintf(leg, "Misalign in Y = %f", mis_y);
  LEG->AddEntry("", leg, "l");
  sprintf(leg, "Offset = %f", fit->GetParameter(2));
  LEG->AddEntry("", leg, "l");
  LEG->Draw();
  Cbm::SaveCanvasAsImage(c3, string(fOutputDir.Data()), "png");

  TCanvas* c4 =
    new TCanvas(fRunTitle + fAxisRotTitle, fRunTitle + fAxisRotTitle, 400, 400);
  CloneArr_2->Draw("colz");
  f1->Draw("same");
  TLegend* LEG1 = new TLegend(0.30, 0.7, 0.72, 0.85);  // Set legend position
  LEG1->SetBorderSize(1);
  LEG1->SetFillColor(0);
  LEG1->SetMargin(0.2);
  LEG1->SetTextSize(0.03);
  sprintf(leg, "Fitted sinusoid");
  LEG1->AddEntry(f1, leg, "l");
  sprintf(leg, "Misalign in X = %f", mis_x);
  LEG1->AddEntry("", leg, "l");
  sprintf(leg, "Misalign in Y = %f", mis_y);
  LEG1->AddEntry("", leg, "l");
  sprintf(leg, "Offset = %f", fit->GetParameter(2));
  LEG1->AddEntry("", leg, "l");
  LEG1->Draw();

  // ------------------------------ APPLY SECOND FIT USING LOG-LIKELIHOOD METHOD ------------------------------ //
  /*	TCanvas* c4 = new TCanvas(fRunTitle + "_Second_Fit_" + fAxisRotTitle, fRunTitle + "_Second_Fit_" + fAxisRotTitle, 600, 600);
	c4->SetFillColor(42);
	gPad->SetTopMargin(0.1);
	gPad->SetFillColor(33);
	f1->SetParameters(fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2));
	histo_1->Fit("f1","L","");
	TF1 *fit2 = histo_1->GetFunction("f1");
	f1->SetParameters(fit2->GetParameter(0), fit2->GetParameter(1), fit2->GetParameter(2));
	f1->Draw();

	Double_t q_2 = TMath::ATan(fit2->GetParameter(0)/fit2->GetParameter(1));
	//cout << endl << "fit2_1 = " << fit2->GetParameter(0) << " and fit2_2 = " << fit2->GetParameter(1) << endl;
	//cout << "q_2 = " << q_2 << endl;
	Double_t A_2 = fit2->GetParameter(1)/TMath::Cos(q_2);
	//cout << "Parameter a_2 = " << A_2 << endl;
	Double_t Alpha_2 = TMath::ATan(A_2/1.5)*0.5*TMath::Power(10,3);
	//cout << setprecision(6) << "Total angle of misalignment alpha_2 = " << Alpha_2 << endl;
	Double_t mis_x_2 = TMath::ATan(fit2->GetParameter(0)/Focal_length)*0.5*TMath::Power(10,3);
	Double_t mis_y_2 = TMath::ATan(fit2->GetParameter(1)/Focal_length)*0.5*TMath::Power(10,3);

	TLegend* LEG2= new TLegend(0.31,0.7,0.72,0.85); // Set legend position
	LEG2->SetBorderSize(1);
	LEG2->SetFillColor(0);
	LEG2->SetMargin(0.2);
	LEG2->SetTextSize(0.03);
	sprintf(leg, "Fitted sinusoid");
	LEG2->AddEntry(f1, leg, "l");
	sprintf(leg, "Misalign in X = %f", mis_x_2);
	LEG2->AddEntry("", leg, "l");
	sprintf(leg, "Misalign in Y = %f", mis_y_2);
	LEG2->AddEntry("", leg, "l");
	sprintf(leg, "Offset = %f", fit2->GetParameter(2));
	LEG2->AddEntry("", leg, "l");
	LEG2->Draw();
	Cbm::SaveCanvasAsImage(c4, string(fOutputDir.Data()), "png");*/

  outputFit.at(0) = mis_x;
  outputFit.at(1) = mis_y;
}

void CbmRichCorrectionVector::DrawHistMapping() {
  TCanvas* can = new TCanvas(
    fRunTitle + "_Separated_Hits", fRunTitle + "_Separated_Hits", 1500, 900);
  can->SetGridx(1);
  can->SetGridy(1);
  can->Divide(4, 3);
  can->cd(9);
  DrawH2(fHM->H2("fHMCPoints_3_15"));
  can->cd(5);
  DrawH2(fHM->H2("fHMCPoints_2_16"));
  can->cd(1);
  DrawH2(fHM->H2("fHMCPoints_2_17"));
  can->cd(2);
  DrawH2(fHM->H2("fHMCPoints_2_29"));
  can->cd(3);
  DrawH2(fHM->H2("fHMCPoints_2_53"));
  can->cd(4);
  DrawH2(fHM->H2("fHMCPoints_2_77"));
  can->cd(6);
  DrawH2(fHM->H2("fHMCPoints_2_28"));
  can->cd(7);
  DrawH2(fHM->H2("fHMCPoints_2_52"));
  can->cd(8);
  DrawH2(fHM->H2("fHMCPoints_2_76"));
  can->cd(10);
  DrawH2(fHM->H2("fHMCPoints_1_27"));
  can->cd(11);
  DrawH2(fHM->H2("fHMCPoints_1_51"));
  can->cd(12);
  DrawH2(fHM->H2("fHMCPoints_1_75"));
  Cbm::SaveCanvasAsImage(can, string(fOutputDir.Data()), "png");

  TCanvas* can2 = new TCanvas(fRunTitle + "_Separated_Ellipse",
                              fRunTitle + "_Separated_Ellipse",
                              1500,
                              900);
  can2->SetGridx(1);
  can2->SetGridy(1);
  can2->Divide(4, 3);
  can2->cd(9);
  DrawH2(fHM->H2("fHPoints_Ellipse_3_15"));
  can2->cd(5);
  DrawH2(fHM->H2("fHPoints_Ellipse_2_16"));
  can2->cd(1);
  DrawH2(fHM->H2("fHPoints_Ellipse_2_17"));
  can2->cd(2);
  DrawH2(fHM->H2("fHPoints_Ellipse_2_29"));
  can2->cd(3);
  DrawH2(fHM->H2("fHPoints_Ellipse_2_53"));
  can2->cd(4);
  DrawH2(fHM->H2("fHPoints_Ellipse_2_77"));
  can2->cd(6);
  DrawH2(fHM->H2("fHPoints_Ellipse_2_28"));
  can2->cd(7);
  DrawH2(fHM->H2("fHPoints_Ellipse_2_52"));
  can2->cd(8);
  DrawH2(fHM->H2("fHPoints_Ellipse_2_76"));
  can2->cd(10);
  DrawH2(fHM->H2("fHPoints_Ellipse_1_27"));
  can2->cd(11);
  DrawH2(fHM->H2("fHPoints_Ellipse_1_51"));
  can2->cd(12);
  DrawH2(fHM->H2("fHPoints_Ellipse_1_75"));
  Cbm::SaveCanvasAsImage(can2, string(fOutputDir.Data()), "png");
}

void CbmRichCorrectionVector::DrawHistProjection() {
  TCanvas* can3 =
    new TCanvas(fRunTitle + "_Projected_Points_w/Corr_" + fAxisRotTitle,
                fRunTitle + "_Projected_Points_w/Corr_" + fAxisRotTitle,
                1500,
                900);
  can3->Divide(2, 2);
  can3->cd(1);
  DrawH1(fHM->H1("fhDifferenceX"));
  can3->cd(2);
  DrawH1(fHM->H1("fhDifferenceY"));
  can3->cd(3);
  /*	DrawH1(fHM->H1("fhDistanceCenterToExtrapolatedTrack"));
	can3->cd(4);*/
  DrawH1(fHM->H1("fhDistanceCenterToExtrapolatedTrackInPlane"));
  Cbm::SaveCanvasAsImage(can3, string(fOutputDir.Data()), "png");

  TCanvas* can4 =
    new TCanvas(fRunTitle + "_Projected_Points_w/oCorr_" + fAxisRotTitle,
                fRunTitle + "_Projected_Points_w/oCorr_" + fAxisRotTitle,
                1500,
                900);
  can4->Divide(2, 2);
  can4->cd(1);
  DrawH1(fHM->H1("fhDifferenceXUncorrected"));
  can4->cd(2);
  DrawH1(fHM->H1("fhDifferenceYUncorrected"));
  can4->cd(3);
  DrawH1(fHM->H1("fhDistanceCenterToExtrapolatedTrackInPlaneUncorrected"));
  can4->SaveAs("Uncorrected_Histograms_" + fAxisRotTitle + ".png");
}

void CbmRichCorrectionVector::DrawHistFromFile(TString fileName) {
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  fHM         = new CbmHistManager();
  TFile* file = new TFile(fileName, "READ");
  fHM->ReadFromFile(file);
  DrawHistMapping();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

void CbmRichCorrectionVector::Finish() {
  // ---------------------------------------------------------------------------------------------------------------------------------------- //
  // -------------------------------------------------- Mapping for mirror - PMT relations -------------------------------------------------- //
  // ---------------------------------------------------------------------------------------------------------------------------------------- //

  if (fDrawAlignment) {
    DrawHistAlignment();
    Int_t thresh = 5;
    vector<Double_t> outputFit(2);
    DrawFit(outputFit, thresh);
    cout << setprecision(6) << endl;
    cout << "Horizontal displacement = " << outputFit[0]
         << " [mrad] and vertical displacement = " << outputFit[1] << " [mrad]."
         << endl;
    /*Double_t Focal_length = 150;
		Double_t q = TMath::ATan(outputFit[0]/outputFit[1]);
		//cout << endl << "fit_1 = " << fit->GetParameter(0) << " and fit_2 = " << fit->GetParameter(1) << endl;
		//cout << "q = " << q << endl;
		Double_t A = outputFit[1]/TMath::Cos(q);
		//cout << "Parameter a = " << A << endl;
		Double_t Alpha = TMath::ATan(A/Focal_length)*0.5*TMath::Power(10,3);                                // *0.5, because a mirror rotation of alpha implies a rotation in the particle trajectory of 2*alpha ; 1.5 meters = Focal length = Radius_of_curvature/2
		cout << endl << setprecision(6) << "Angle of misalignment alpha [mrad] = " << Alpha << endl;       // setprecision(#) gives the number of digits in the cout.
		Double_t mis_x = TMath::ATan(outputFit[0]/Focal_length)*0.5*TMath::Power(10,3);
		Double_t mis_y = TMath::ATan(outputFit[1]/Focal_length)*0.5*TMath::Power(10,3);
		cout << "Misalignment in X [mrad] = " << mis_x << " and misalignment in Y [mrad] = " << mis_y << endl;*/

    fHM2->Create2<TH2D>(
      "fHCherenkovHitsDistribReduced",
      "fHCherenkovHitsDistribReduced;Phi_Ch [rad];Th_Ch-Th_0 [cm];Entries",
      200,
      -3.4,
      3.4,
      500,
      -5.,
      5.);
  }

  if (fDrawMapping) { DrawHistMapping(); }

  if (fDrawProjection) {
    DrawHistProjection();
    fHM->H1("fhDifferenceX")->Write();
    fHM->H1("fhDifferenceY")->Write();
    fHM->H1("fhDistanceCenterToExtrapolatedTrack")->Write();
    fHM->H1("fhDistanceCenterToExtrapolatedTrackInPlane")->Write();
    fHM->H1("fhDifferenceXUncorrected")->Write();
    fHM->H1("fhDifferenceYUncorrected")->Write();
    fHM->H1("fhDistanceCenterToExtrapolatedTrackInPlaneUncorrected")->Write();
  }

  //cout << endl << "Mirror counter = " << fMirrCounter << endl;
  //cout << setprecision(6) << endl;
}
ClassImp(CbmRichCorrectionVector)
