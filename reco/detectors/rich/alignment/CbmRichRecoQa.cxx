
#include "CbmRichRecoQa.h"

#include "CbmDrawHist.h"
#include "CbmGlobalTrack.h"
#include "CbmHistManager.h"
#include "CbmMCTrack.h"
#include "CbmMatchRecoToMC.h"
#include "CbmRichDraw.h"
#include "CbmRichGeoManager.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmRichUtil.h"
#include "CbmTrackMatchNew.h"
#include "CbmUtils.h"
#include "elid/CbmLitGlobalElectronId.h"

#include "FairMCPoint.h"
#include "FairTrackParam.h"

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TEllipse.h"
#include "TF1.h"
#include "TH1.h"
#include "TH1D.h"
#include "TMCProcess.h"
#include "TMarker.h"
#include "TStyle.h"

#include <boost/assign/list_of.hpp>

#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using boost::assign::list_of;

CbmRichRecoQa::CbmRichRecoQa()
  : FairTask("CbmRichRecoQa")
  , fHM(nullptr)
  , fEventNum(0)
  , fOutputDir("")
  , fMCTracks(nullptr)
  , fRichPoints(nullptr)
  , fRichDigis(nullptr)
  , fRichHits(nullptr)
  , fRichRings(nullptr)
  , fRichRingMatches(nullptr)
  , fGlobalTracks(nullptr)
  , fStsTracks(nullptr)
  , fStsTrackMatches(nullptr)
  , fRichProjections(nullptr)
  , fNofHitsInRingMap()
  , fCanvas()
  , fCorrection("")
{
}

InitStatus CbmRichRecoQa::Init()
{
  cout << "CbmRichRecoQa::Init" << endl;
  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) { Fatal("CbmRichRecoQa::Init", "RootManager not instantised!"); }

  fMCTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (nullptr == fMCTracks) { Fatal("CbmRichRecoQa::Init", "No MC Tracks!"); }

  fRichPoints = (TClonesArray*) ioman->GetObject("RichPoint");
  if (nullptr == fRichPoints) { Fatal("CbmRichRecoQa::Init", "No Rich Points!"); }

  fRichDigis = (TClonesArray*) ioman->GetObject("RichDigi");
  if (nullptr == fRichDigis) { Fatal("CbmRichRecoQa::Init", "No Rich Digis!"); }

  fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
  if (nullptr == fRichHits) { Fatal("CbmRichRecoQa::Init", "No RichHits!"); }

  fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
  if (nullptr == fRichRings) { Fatal("CbmRichRecoQa::Init", "No RichRings!"); }

  fRichRingMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
  if (nullptr == fRichRingMatches) { Fatal("CbmRichRecoQa::Init", "No RichRingMatch array!"); }

  fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
  if (nullptr == fGlobalTracks) { Fatal("CbmRichRecoQa::Init", "No GlobalTrack array!"); }

  fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  if (nullptr == fStsTracks) { Fatal("CbmRichRecoQa::Init", ": No StsTrack array!"); }

  fStsTrackMatches = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  if (nullptr == fStsTrackMatches) { Fatal("CbmRichRecoQa::Init", ": No StsTrackMatch array!"); }

  fRichProjections = (TClonesArray*) ioman->GetObject("RichProjection");
  if (nullptr == fRichProjections) { Fatal("CbmRichUrqmdTest::Init", "No fRichProjections array!"); }

  InitHistograms();

  // CbmLitGlobalElectronId::GetInstance();

  return kSUCCESS;
}

void CbmRichRecoQa::InitHistograms()
{
  fHM = new CbmHistManager();

  // 2D histogramm XY
  double xMin = -120.;
  double xMax = 120.;
  int nBinsX  = 30;
  double yMin = -208;
  double yMax = 208.;
  int nBinsY  = 52;

  // 1D histogram X or Y
  int nBinsX1 = 60;
  int xMin1   = -120;
  int xMax1   = 120;
  int nBinsY1 = 25;
  int yMin1   = 100;
  int yMax1   = 200;

  for (Int_t i = 0; i < 4; i++) {
    string s;
    if (i == 0) s = "Primel";
    if (i == 1) s = "Pi";
    if (i == 2) s = "PrimelPlus";
    if (i == 3) s = "PrimelMinus";

    // Ring-Track VS momentum: (20, 0., 10., 100, 0., 5.)
    fHM->Create2<TH2D>("fhRingTrackDistVsMomTruematch" + s,
                       "fhRingTrackDistVsMomTruematch" + s + ";P [GeV/c];Ring-track distance [cm];Yield (a.u.)", 40, 0.,
                       10., 120, 0., 3.);
    fHM->Create2<TH2D>("fhRingTrackDistVsMomWrongmatch" + s,
                       "fhRingTrackDistVsMomWrongmatch" + s + ";P [GeV/c];Ring-track distance [cm];Yield (a.u.)", 20,
                       0., 10., 100, 0., 5.);

    // Ring-Track VS nb hits found in ring: (40, -.5, 39.5, 100, 0., 5.)
    fHM->Create2<TH2D>("fhRingTrackDistVsNofHitsTruematch" + s,
                       "fhRingTrackDistVsNofHitsTruematch" + s
                         + ";Nof hits in found ring;Ring-track distance [cm];Yield (a.u.)",
                       41, -.5, 40.5, 100, 0., 5.);
    // Ring-Track 3D: (nBinsX, xMin, xMax, nBinsY, yMin, yMax, 100, 0., 5.)
    fHM->Create3<TH3D>("fhRingTrackDistVsXYTruematch" + s,
                       "fhRingTrackDistVsXYTruematch" + s + ";X [cm];Y [cm];Ring-track distance [cm]", 240, xMin, xMax,
                       416, -200., 200., 400, 0., 5.);
    fHM->Create2<TH2D>("fhRingTrackDistVsXTruematch" + s,
                       "fhRingTrackDistVsXTruematch" + s + ";X [cm];Ring-track distance [cm]", nBinsX1, xMin1, xMax1,
                       100, 0., 5.);
    // Ring-Track VS Absolute Y distance: (nBinsY1, yMin1, yMax1, 100, 0., 5.)
    fHM->Create2<TH2D>("fhRingTrackDistVsYTruematch" + s,
                       "fhRingTrackDistVsYTruematch" + s + ";Abs(Y) [cm];Ring-track distance [cm]", 34, 110., yMax1,
                       100, 0., 5.);

    Double_t range = 3., nbBinsX = 120., nbBinsY = 208;
    fHM->Create3<TH3D>("fhRingTrackDistDiffXRingVsXYTruematchUp" + s,
                       "fhRingTrackDistDiffXRingVsXYTruematchUp" + s + ";X [cm];Y [cm];X Ring-track distance [cm]",
                       nbBinsX, xMin, xMax, nbBinsY, 110, 200, 200, -range, range);
    fHM->Create3<TH3D>("fhRingTrackDistDiffYRingVsXYTruematchUp" + s,
                       "fhRingTrackDistDiffYRingVsXYTruematchUp" + s + ";X [cm];Y [cm];Y Ring-track distance [cm]",
                       nbBinsX, xMin, xMax, nbBinsY, 110, 200, 200, -range, range);
    fHM->Create3<TH3D>("fhRingTrackDistDiffXTrackVsXYTruematchUp" + s,
                       "fhRingTrackDistDiffXTrackVsXYTruematchUp" + s + ";X [cm];Y [cm];X Track-ring distance [cm]",
                       nbBinsX, xMin, xMax, nbBinsY, 110, 200, 200, -range, range);
    fHM->Create3<TH3D>("fhRingTrackDistDiffYTrackVsXYTruematchUp" + s,
                       "fhRingTrackDistDiffYTrackVsXYTruematchUp" + s + ";X [cm];Y [cm];Y Track-ring distance [cm]",
                       nbBinsX, xMin, xMax, nbBinsY, 110, 200, 200, -range, range);

    fHM->Create3<TH3D>("fhRingTrackDistDiffXRingVsXYTruematchDown" + s,
                       "fhRingTrackDistDiffXRingVsXYTruematchDown" + s + ";X [cm];Y [cm];X Ring-track distance [cm]",
                       nbBinsX, xMin, xMax, nbBinsY, -200, -110, 200, -range, range);
    fHM->Create3<TH3D>("fhRingTrackDistDiffYRingVsXYTruematchDown" + s,
                       "fhRingTrackDistDiffYRingVsXYTruematchDown" + s + ";X [cm];Y [cm];Y Ring-track distance [cm]",
                       nbBinsX, xMin, xMax, nbBinsY, -200, -110, 200, -range, range);
    fHM->Create3<TH3D>("fhRingTrackDistDiffXTrackVsXYTruematchDown" + s,
                       "fhRingTrackDistDiffXTrackVsXYTruematchDown" + s + ";X [cm];Y [cm];X Track-ring distance [cm]",
                       nbBinsX, xMin, xMax, nbBinsY, -200, -110, 200, -range, range);
    fHM->Create3<TH3D>("fhRingTrackDistDiffYTrackVsXYTruematchDown" + s,
                       "fhRingTrackDistDiffYTrackVsXYTruematchDown" + s + ";X [cm];Y [cm];Y Track-ring distance [cm]",
                       nbBinsX, xMin, xMax, nbBinsY, -200, -110, 200, -range, range);

    fHM->Create3<TH3D>("fhRingTrackDistVsXYHalfUpTruematch" + s,
                       "fhRingTrackDistVsXYHalfUpTruematch" + s + ";X [cm];Y [cm];Ring-track distance [cm]", nbBinsX,
                       xMin, xMax, nbBinsY, 110., 200., 100, 0., range);
    fHM->Create3<TH3D>("fhRingTrackDistVsXYHalfDownTruematch" + s,
                       "fhRingTrackDistVsXYHalfDownTruematch" + s + ";X [cm];Y [cm];Ring-track distance [cm]", nbBinsX,
                       xMin, xMax, nbBinsY, -200., -110., 100, 0., range);
  }

  // after electron identification
  Double_t range_2 = 3.;
  //fHM->Create2<TH2D>("fhRingTrackDistVsMomTruematchElId", "fhRingTrackDistVsMomTruematchElId;P [GeV/c];Ring-track distance [cm];Yield (a.u.)", 20, 0., 10., 100, 0., 5.);
  fHM->Create2<TH2D>("fhRingTrackDistVsMomTruematchElId",
                     "fhRingTrackDistVsMomTruematchElId;P [GeV/c];Ring-track "
                     "distance [cm];Yield (a.u.)",
                     40, 0., 10., 120, 0., 3.);
  fHM->Create3<TH3D>("fhRingTrackDistDiffXAfterEl",
                     "fhRingTrackDistDiffXAfterEl;X [cm];Y [cm];X Ring-track distance [cm]", 60, xMin, xMax, 104, 110,
                     200, 200, -range_2, range_2);
  fHM->Create3<TH3D>("fhRingTrackDistDiffYAfterEl",
                     "fhRingTrackDistDiffYAfterEl;X [cm];Y [cm];Y Ring-track distance [cm]", 60, xMin, xMax, 104, 110,
                     200, 200, -range_2, range_2);

  fHM->Create1<TH1D>("fhMismatchSource", "fhMismatchSource;Global track category;% from MC", 13, -0.5, 12.5);

  fHM->Create1<TH1D>("fhMismatchSourceMomMc", "fhMismatchSourceMomMc;Momentum [GeV/c];Yield", 40, 0., 10.);
  fHM->Create1<TH1D>("fhMismatchSourceMomSts", "fhMismatchSourceMomSts;Momentum [GeV/c];Yield", 40, 0., 10.);
  fHM->Create1<TH1D>("fhMismatchSourceMomStsAccRich", "fhMismatchSourceMomStsAccRich;Momentum [GeV/c];Yield", 40, 0.,
                     10.);
  fHM->Create1<TH1D>("fhMismatchSourceMomStsRich", "fhMismatchSourceMomStsRich;Momentum [GeV/c];Yield", 40, 0., 10.);
  fHM->Create1<TH1D>("fhMismatchSourceMomStsRichTrue", "fhMismatchSourceMomStsRichTrue;Momentum [GeV/c];Yield", 40, 0.,
                     10.);
  fHM->Create1<TH1D>("fhMismatchSourceMomStsNoRich", "fhMismatchSourceMomStsNoRich;Momentum [GeV/c];Yield", 40, 0.,
                     10.);
  fHM->Create1<TH1D>("fhMismatchSourceMomStsNoRichRF", "fhMismatchSourceMomStsNoRichRF;Momentum [GeV/c];Yield", 40, 0.,
                     10.);
  fHM->Create1<TH1D>("fhMismatchSourceMomStsNoRichRM", "fhMismatchSourceMomStsNoRichRM;Momentum [GeV/c];Yield", 40, 0.,
                     10.);
  fHM->Create1<TH1D>("fhMismatchSourceMomStsNoRichNoRF", "fhMismatchSourceMomStsNoRichNoRF;Momentum [GeV/c];Yield", 40,
                     0., 10.);
  fHM->Create1<TH1D>("fhMismatchSourceMomStsNoRichNoProj", "fhMismatchSourceMomStsNoRichNoProj;Momentum [GeV/c];Yield",
                     40, 0., 10.);
  fHM->Create1<TH1D>("fhMismatchSourceMomStsRichWrong", "fhMismatchSourceMomStsRichWrong;Momentum [GeV/c];Yield", 40,
                     0., 10.);
  fHM->Create1<TH1D>("fhMismatchSourceMomStsRichWrongRF", "fhMismatchSourceMomStsRichWrongRF;Momentum [GeV/c];Yield",
                     40, 0., 10.);
  fHM->Create1<TH1D>("fhMismatchSourceMomStsRichWrongRM", "fhMismatchSourceMomStsRichWrongRM;Momentum [GeV/c];Yield",
                     40, 0., 10.);
}

void CbmRichRecoQa::Exec(Option_t* option)
{
  fEventNum++;
  cout << "CbmRichRecoQa, event No. " << fEventNum << endl;

  FillRichRingNofHits();
  FillRingTrackDistance();
  RingTrackMismatchSource();
}

void CbmRichRecoQa::FillRichRingNofHits()
{
  fNofHitsInRingMap.clear();
  Int_t nofRichHits = fRichHits->GetEntriesFast();
  for (Int_t iHit = 0; iHit < nofRichHits; iHit++) {
    CbmRichHit* hit = static_cast<CbmRichHit*>(fRichHits->At(iHit));
    if (nullptr == hit) continue;

    vector<Int_t> motherIds = CbmMatchRecoToMC::GetMcTrackMotherIdsForRichHit(hit, fRichDigis, fRichPoints, fMCTracks);
    for (UInt_t i = 0; i < motherIds.size(); i++) {
      fNofHitsInRingMap[motherIds[i]]++;
    }
  }
}

void CbmRichRecoQa::RingTrackMismatchSource()
{
  Int_t nofMcTracks = fMCTracks->GetEntriesFast();
  for (Int_t iTrack = 0; iTrack < nofMcTracks; iTrack++) {
    const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(fMCTracks->At(iTrack));
    if (mcTrack == nullptr) continue;
    bool isEl = IsMcPrimaryElectron(mcTrack);
    if (isEl) {
      //MC
      fHM->H1("fhMismatchSource")->Fill(0);
      fHM->H1("fhMismatchSourceMomMc")->Fill(mcTrack->GetP());
    }
  }


  Int_t nofGlobalTracks = fGlobalTracks->GetEntriesFast();
  for (Int_t iTrack = 0; iTrack < nofGlobalTracks; iTrack++) {
    const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(iTrack));

    Int_t stsId = globalTrack->GetStsTrackIndex();
    if (stsId < 0) continue;
    const CbmTrackMatchNew* stsTrackMatch = static_cast<const CbmTrackMatchNew*>(fStsTrackMatches->At(stsId));
    if (stsTrackMatch == nullptr) continue;
    int stsMcTrackId = stsTrackMatch->GetMatchedLink().GetIndex();

    CbmMCTrack* mctrack = static_cast<CbmMCTrack*>(fMCTracks->At(stsMcTrackId));
    if (mctrack == nullptr) continue;
    Double_t mom = mctrack->GetP();

    bool isEl = IsMcPrimaryElectron(mctrack);
    if (!isEl) continue;

    //STS
    fHM->H1("fhMismatchSource")->Fill(1);
    fHM->H1("fhMismatchSourceMomSts")->Fill(mom);

    if (fNofHitsInRingMap[stsMcTrackId] >= 7) {
      //Sts-AccRich
      fHM->H1("fhMismatchSource")->Fill(2);
      fHM->H1("fhMismatchSourceMomStsAccRich")->Fill(mctrack->GetP());
    }

    Int_t richId = globalTrack->GetRichRingIndex();

    // No RICH segment
    if (richId < 0) {
      //STS-noRICH
      fHM->H1("fhMismatchSource")->Fill(5);
      fHM->H1("fhMismatchSourceMomStsNoRich")->Fill(mom);
      bool ringFound   = WasRingFound(stsMcTrackId);
      bool ringMatched = WasRingMatched(stsMcTrackId);
      bool hasProj     = HasRichProjection(stsId);
      if (ringFound) {
        //STS-NoRich RF
        fHM->H1("fhMismatchSource")->Fill(6);
        fHM->H1("fhMismatchSourceMomStsNoRichRF")->Fill(mom);
      }
      else {
        //STS-NoRich NoRF
        fHM->H1("fhMismatchSource")->Fill(8);
        fHM->H1("fhMismatchSourceMomStsNoRichNoRF")->Fill(mom);
      }

      if (ringMatched) {
        //STS-NoRich RM
        fHM->H1("fhMismatchSource")->Fill(7);
        fHM->H1("fhMismatchSourceMomStsNoRichRM")->Fill(mom);
      }

      if (!hasProj) {
        //STS-NoRich NoProj
        fHM->H1("fhMismatchSource")->Fill(9);
        fHM->H1("fhMismatchSourceMomStsNoRichNoProj")->Fill(mom);
      }

      continue;
    }

    //STS-RICH
    fHM->H1("fhMismatchSource")->Fill(3);
    fHM->H1("fhMismatchSourceMomStsRich")->Fill(mom);

    const CbmTrackMatchNew* richRingMatch = static_cast<const CbmTrackMatchNew*>(fRichRingMatches->At(richId));
    if (richRingMatch == nullptr) continue;
    int richMcTrackId       = richRingMatch->GetMatchedLink().GetIndex();
    const CbmRichRing* ring = static_cast<const CbmRichRing*>(fRichRings->At(richId));
    if (nullptr == ring) continue;

    if (stsMcTrackId == richMcTrackId) {
      //STS-RICH true
      fHM->H1("fhMismatchSource")->Fill(4);
      fHM->H1("fhMismatchSourceMomStsRichTrue")->Fill(mom);
    }
    else {
      //STS-RICH wrong
      fHM->H1("fhMismatchSource")->Fill(10);
      fHM->H1("fhMismatchSourceMomStsRichWrong")->Fill(mom);
      if (WasRingFound(stsMcTrackId)) {
        //STS-RICH wrong RF
        fHM->H1("fhMismatchSource")->Fill(11);
        fHM->H1("fhMismatchSourceMomStsRichWrongRF")->Fill(mom);
      }

      if (WasRingFound(stsMcTrackId)) {
        //STS-RICH wrong RM
        fHM->H1("fhMismatchSource")->Fill(12);
        fHM->H1("fhMismatchSourceMomStsRichWrongRM")->Fill(mom);
      }
    }
  }
}

bool CbmRichRecoQa::WasRingFound(Int_t mcTrackId)
{
  Int_t nofRings = fRichRings->GetEntriesFast();
  for (Int_t iR = 0; iR < nofRings; iR++) {
    const CbmRichRing* ring = static_cast<const CbmRichRing*>(fRichRings->At(iR));
    if (ring == nullptr) continue;
    const CbmTrackMatchNew* richRingMatch = static_cast<const CbmTrackMatchNew*>(fRichRingMatches->At(iR));
    if (richRingMatch == nullptr) continue;
    int richMcTrackId = richRingMatch->GetMatchedLink().GetIndex();
    if (richMcTrackId == mcTrackId) return true;
  }
  return false;
}

bool CbmRichRecoQa::WasRingMatched(Int_t mcTrackId)
{
  Int_t nofGlobalTracks = fGlobalTracks->GetEntriesFast();
  for (Int_t iTrack = 0; iTrack < nofGlobalTracks; iTrack++) {
    const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(iTrack));

    Int_t richId = globalTrack->GetRichRingIndex();
    if (richId < 0) continue;
    const CbmTrackMatchNew* richRingMatch = static_cast<const CbmTrackMatchNew*>(fRichRingMatches->At(richId));
    if (richRingMatch == nullptr) continue;
    int richMcTrackId = richRingMatch->GetMatchedLink().GetIndex();
    if (richMcTrackId == mcTrackId) { return true; }
  }

  return false;
}

bool CbmRichRecoQa::HasRichProjection(Int_t stsTrackId)
{
  if (stsTrackId < 0) { return false; }
  FairTrackParam* pTrack = (FairTrackParam*) fRichProjections->At(stsTrackId);
  if (pTrack == nullptr) { return false; }

  if (pTrack->GetX() == 0. && pTrack->GetY() == 0.) { return false; }
  else {
    return true;
  }
}

void CbmRichRecoQa::FillRingTrackDistance()
{
  Int_t nofGlobalTracks = fGlobalTracks->GetEntriesFast();
  for (Int_t iTrack = 0; iTrack < nofGlobalTracks; iTrack++) {
    const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(iTrack));

    Int_t stsId  = globalTrack->GetStsTrackIndex();
    Int_t richId = globalTrack->GetRichRingIndex();
    if (stsId < 0 || richId < 0) continue;

    const CbmTrackMatchNew* stsTrackMatch = static_cast<const CbmTrackMatchNew*>(fStsTrackMatches->At(stsId));
    if (stsTrackMatch == nullptr) continue;
    int stsMcTrackId = stsTrackMatch->GetMatchedLink().GetIndex();

    const CbmTrackMatchNew* richRingMatch = static_cast<const CbmTrackMatchNew*>(fRichRingMatches->At(richId));
    if (richRingMatch == nullptr) continue;
    int richMcTrackId       = richRingMatch->GetMatchedLink().GetIndex();
    const CbmRichRing* ring = static_cast<const CbmRichRing*>(fRichRings->At(richId));
    if (nullptr == ring) continue;

    double rtDistance  = CbmRichUtil::GetRingTrackDistance(iTrack);
    double rtDistanceX = CbmRichUtil::GetRingTrackDistanceX(iTrack);
    double rtDistanceY = CbmRichUtil::GetRingTrackDistanceY(iTrack);
    double xc          = ring->GetCenterX();
    double yc          = ring->GetCenterY();
    int nofHits        = ring->GetNofHits();

    CbmMCTrack* mctrack = static_cast<CbmMCTrack*>(fMCTracks->At(stsMcTrackId));
    if (mctrack == nullptr) continue;
    double mom = mctrack->GetP();
    int charge = mctrack->GetCharge();

    bool isEl = IsMcPrimaryElectron(mctrack);
    bool isPi = IsMcPion(mctrack);

    if (!isEl && !isPi) continue;

    string s = "";

    for (int i = 0; i < 2; i++) {
      if (i == 0) {
        if (isEl) { s = "Primel"; }
        else if (isPi) {
          s = "Pi";
        }
        else {
          continue;
        }
      }
      else if (i == 1) {
        if (isEl) {
          if (charge > 0) { s = "PrimelPlus"; }
          else if (charge < 0) {
            s = "PrimelMinus";
          }
          else {
            continue;
          }
        }
        else {
          continue;
        }
      }

      if (stsMcTrackId == richMcTrackId) {
        fHM->H2("fhRingTrackDistVsMomTruematch" + s)->Fill(mom, rtDistance);
        fHM->H3("fhRingTrackDistVsXYTruematch" + s)->Fill(xc, yc, rtDistance);
        fHM->H2("fhRingTrackDistVsXTruematch" + s)->Fill(xc, rtDistance);
        fHM->H2("fhRingTrackDistVsYTruematch" + s)->Fill(abs(yc), rtDistance);
        fHM->H2("fhRingTrackDistVsNofHitsTruematch" + s)->Fill(nofHits, rtDistance);

        Double_t rtX = -1 * rtDistanceX;
        Double_t rtY = -1 * rtDistanceY;

        if (yc > 0) {
          fHM->H3("fhRingTrackDistDiffXRingVsXYTruematchUp" + s)->Fill(xc, yc, rtDistanceX);
          fHM->H3("fhRingTrackDistDiffYRingVsXYTruematchUp" + s)->Fill(xc, yc, rtDistanceY);
          fHM->H3("fhRingTrackDistDiffXTrackVsXYTruematchUp" + s)->Fill(xc, yc, rtX);
          fHM->H3("fhRingTrackDistDiffYTrackVsXYTruematchUp" + s)->Fill(xc, yc, rtY);
          fHM->H3("fhRingTrackDistVsXYHalfUpTruematch" + s)->Fill(xc, yc, rtDistance);
        }

        else if (yc < 0) {
          fHM->H3("fhRingTrackDistDiffXRingVsXYTruematchDown" + s)->Fill(xc, yc, rtDistanceX);
          fHM->H3("fhRingTrackDistDiffYRingVsXYTruematchDown" + s)->Fill(xc, yc, rtDistanceY);
          fHM->H3("fhRingTrackDistDiffXTrackVsXYTruematchDown" + s)->Fill(xc, yc, rtX);
          fHM->H3("fhRingTrackDistDiffYTrackVsXYTruematchDown" + s)->Fill(xc, yc, rtY);
          fHM->H3("fhRingTrackDistVsXYHalfDownTruematch" + s)->Fill(xc, yc, rtDistance);
        }

        //std::cout << "CbmRichRecoQa::FillRingTrackDistance:" << std::endl << "rtDistanceX & rtDistanceY = " << rtDistanceX << ", " << rtDistanceY << std::endl;

        if (i == 0 && isEl) {
          //if (CbmLitGlobalElectronId::GetInstance().IsRichElectron(iTrack, mom)){
          fHM->H2("fhRingTrackDistVsMomTruematchElId")->Fill(mom, rtDistance);
          fHM->H3("fhRingTrackDistDiffXAfterEl")->Fill(xc, yc, rtDistanceX);
          fHM->H3("fhRingTrackDistDiffYAfterEl")->Fill(xc, yc, rtDistanceY);
          //}
        }
      }
      else {
        fHM->H2("fhRingTrackDistVsMomWrongmatch" + s)->Fill(mom, rtDistance);
      }
    }
  }
}

void CbmRichRecoQa::DrawHist()
{
  cout.precision(4);


  SetDefaultDrawStyle();
  //fHM->ScaleByPattern("fh_.*", 1./fEventNum);

  {
    TCanvas* c     = fHM->CreateCanvas("fh_ring_track_distance_truematch_comparison_primel",
                                   "fh_ring_track_distance_truematch_comparison_primel", 800, 800);
    TH1D* py_minus = (TH1D*) (fHM->H2("fhRingTrackDistVsMomTruematchPrimelMinus")
                                ->ProjectionY("fhRingTrackDistVsMomTruematchPrimelMinus_py")
                                ->Clone());
    py_minus->Scale(1. / py_minus->Integral());
    TH1D* py_plus = (TH1D*) (fHM->H2("fhRingTrackDistVsMomTruematchPrimelPlus")
                               ->ProjectionY("fhRingTrackDistVsMomTruematchPrimelPlus_py")
                               ->Clone());
    py_plus->Scale(1. / py_plus->Integral());
    DrawH1({py_minus, py_plus},
           {"e_{prim}^{-} (" + this->GetMeanRmsOverflowString(py_minus, false) + ")",
            "e_{prim}^{+} (" + this->GetMeanRmsOverflowString(py_plus, false) + ")"},
           kLinear, kLog, true, 0.5, 0.85, 0.99, 0.99);
  }

  {
    TCanvas* c  = fHM->CreateCanvas("fh_ring_track_distance_truematch_comparison_elpi",
                                   "fh_ring_track_distance_truematch_comparison_elpi", 800, 800);
    TH1D* py_el = (TH1D*) (fHM->H2("fhRingTrackDistVsMomTruematchPrimel")
                             ->ProjectionY("fhRingTrackDistVsMomTruematchPrimel_py")
                             ->Clone());
    py_el->Scale(1. / py_el->Integral());
    TH1D* py_pi =
      (TH1D*) (fHM->H2("fhRingTrackDistVsMomTruematchPi")->ProjectionY("fhRingTrackDistVsMomTruematchPi_py")->Clone());
    py_pi->Scale(1. / py_pi->Integral());
    DrawH1({py_el, py_pi},
           {"e_{prim}^{#pm} (" + this->GetMeanRmsOverflowString(py_el, false) + ")",
            "#pi^{#pm} (" + this->GetMeanRmsOverflowString(py_pi, false) + ")"},
           kLinear, kLog, true, 0.5, 0.85, 0.99, 0.99);
  }

  {
    gStyle->SetPaintTextFormat("4.1f");
    TCanvas* c       = fHM->CreateCanvas("fh_mismatch_source", "fh_mismatch_source", 1000, 800);
    Double_t nofMcEl = fHM->H1("fhMismatchSource")->GetBinContent(1);
    cout << "nofMcEl = " << nofMcEl << endl;
    fHM->H1("fhMismatchSource")->Scale(100. / nofMcEl);
    DrawH1(fHM->H1("fhMismatchSource"), kLinear, kLog, "hist text");
    fHM->H1("fhMismatchSource")->SetMarkerSize(1.9);

    vector<string> labels = {"MC",
                             "STS",
                             "STS-Acc RICH",
                             "STS-RICH",
                             "STS-RICH true",
                             "STS-NoRICH",
                             "STS-NoRICH RF",
                             "STS-NoRICH RM",
                             "STS-NoRICH NoRF",
                             "STS-NoRICH NoPrj",
                             "STS-RICH wrong",
                             "STS-RICH wrong RF",
                             "STS-RICH wrong RM"};
    for (int i = 0; i < labels.size(); i++) {
      fHM->H1("fhMismatchSource")->GetXaxis()->SetBinLabel(i + 1, labels[i].c_str());
    }
    fHM->H1("fhMismatchSource")->GetXaxis()->SetLabelSize(0.03);
  }

  {
    TCanvas* c            = fHM->CreateCanvas("fh_mismatch_source_mom", "fh_mismatch_source_mom", 1000, 800);
    vector<string> labels = {"MC",
                             "STS",
                             "STS-Acc RICH",
                             "STS-RICH",
                             "STS-RICH true",
                             "STS-NoRICH",
                             "STS-NoRICH RF",
                             "STS-NoRICH RM",
                             "STS-NoRICH NoRF",
                             "STS-NoRICH NoPrj",
                             "STS-RICH wrong",
                             "STS-RICH wrong RF",
                             "STS-RICH wrong RM"};
    vector<TH1*> hists    = {fHM->H1("fhMismatchSourceMomMc"),
                          fHM->H1("fhMismatchSourceMomSts"),
                          fHM->H1("fhMismatchSourceMomStsAccRich"),
                          fHM->H1("fhMismatchSourceMomStsRich"),
                          fHM->H1("fhMismatchSourceMomStsRichTrue"),
                          fHM->H1("fhMismatchSourceMomStsNoRich"),
                          fHM->H1("fhMismatchSourceMomStsNoRichRF"),
                          fHM->H1("fhMismatchSourceMomStsNoRichRM"),
                          fHM->H1("fhMismatchSourceMomStsNoRichNoRF"),
                          fHM->H1("fhMismatchSourceMomStsNoRichNoProj"),
                          fHM->H1("fhMismatchSourceMomStsRichWrong"),
                          fHM->H1("fhMismatchSourceMomStsRichWrongRF"),
                          fHM->H1("fhMismatchSourceMomStsRichWrongRM")};

    DrawH1(hists, labels, kLinear, kLog, true, 0.8, 0.35, 0.99, 0.99);
    fHM->H1("fhMismatchSourceMomMc")->SetMinimum(0.9);
  }

  DrawRingTrackDistHistWithSuffix("Primel");
  DrawRingTrackDistHistWithSuffix("PrimelPlus");
  DrawRingTrackDistHistWithSuffix("PrimelMinus");
  //DrawRingTrackDistHistWithSuffix("Pi");

  DrawRingTrackDistHistWithSuffixPmtHalf("Primel");
  //DrawRingTrackDistHistWithSuffixPmtHalf("PrimelPlus");
  //DrawRingTrackDistHistWithSuffixPmtHalf("PrimelMinus");
  //DrawRingTrackDistHistWithSuffixPmtHalf("Pi");

  // before and after electron identification
  {
    TCanvas* c =
      fHM->CreateCanvas("fh_ring_track_distance_truematch_elid", "fh_ring_track_distance_truematch_elid", 1400, 1400);
    c->Divide(2, 2);
    c->cd(1);
    DrawH2WithProfile(fHM->H2("fhRingTrackDistVsMomTruematchPrimel"), false, true);
    fHM->H2("fhRingTrackDistVsMomTruematchPrimel")->GetYaxis()->SetRangeUser(0., 2.);
    c->cd(2);
    DrawH2WithProfile(fHM->H2("fhRingTrackDistVsMomTruematchElId"), false, true);
    fHM->H2("fhRingTrackDistVsMomTruematchElId")->GetYaxis()->SetRangeUser(0., 2.);
    c->cd(3);
    TH1D* py      = (TH1D*) (fHM->H2("fhRingTrackDistVsMomTruematchPrimel")
                          ->ProjectionY(string("fhRingTrackDistVsMomTruematchPrimel_py2").c_str())
                          ->Clone());
    TH1D* pyElId  = (TH1D*) (fHM->H2("fhRingTrackDistVsMomTruematchElId")
                              ->ProjectionY(string("fhRingTrackDistVsMomTruematchElId_py").c_str())
                              ->Clone());
    TH1D* pyElId2 = (TH1D*) (fHM->H2("fhRingTrackDistVsMomTruematchElId")->ProjectionY("test_py", 0, 40));
    //		DrawH1({py, pyElId}, {string("before ElId (" + this->GetMeanRmsOverflowString(py) + ")"), string("after ElId (" + this->GetMeanRmsOverflowString(pyElId) + ")")},
    //						kLinear, kLog, true, 0.45, 0.2, 0.9, 0.35);
    DrawH1({py}, {string("before ElId (" + this->GetMeanRmsOverflowString(py) + ")")}, kLinear, kLog, false, 0.45, 0.2,
           0.9, 0.35);
    c->cd(4);
    DrawH1({pyElId}, {string("after ElId (" + this->GetMeanRmsOverflowString(pyElId) + ")")}, kLinear, kLog, false,
           0.45, 0.2, 0.9, 0.35);
  }

  Double_t range = 2.5;
  {
    TCanvas* c = fHM->CreateCanvas("fh_track_ring_distance_diff_x_y_track_vs_xy_truematch_after_el",
                                   "fh_track_ring_distance_diff_x_y_track_vs_xy_truematch_after_el", 1800, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH3Profile(fHM->H3("fhRingTrackDistDiffXAfterEl"), true, false, -range, range);
    c->cd(2);
    DrawH3Profile(fHM->H3("fhRingTrackDistDiffYAfterEl"), true, false, -range, range);
  }
}

string CbmRichRecoQa::GetMeanRmsOverflowString(TH1* h, Bool_t withOverflow)
{
  if (withOverflow) {
    double overflow = h->GetBinContent(h->GetNbinsX() + 1);
    return Cbm::NumberToString<Double_t>(h->GetMean(), 2) + " / " + Cbm::NumberToString<Double_t>(h->GetRMS(), 2)
           + " / " + Cbm::NumberToString<Double_t>(100. * overflow / h->Integral(0, h->GetNbinsX() + 1), 2) + "%";
  }
  else {
    return Cbm::NumberToString<Double_t>(h->GetMean(), 2) + " / " + Cbm::NumberToString<Double_t>(h->GetRMS(), 2);
  }
}

void CbmRichRecoQa::DrawRingTrackDistHistWithSuffix(const string& s)
{
  {
    TCanvas* c =
      fHM->CreateCanvas("fh_ring_track_distance_truematch_" + s, "fh_ring_track_distance_truematch_" + s, 1800, 600);
    c->Divide(3, 1);
    c->cd(1);
    DrawH2WithProfile(fHM->H2("fhRingTrackDistVsMomTruematch" + s), false, true);
    c->cd(2);
    DrawH2WithProfile(fHM->H2("fhRingTrackDistVsNofHitsTruematch" + s), false, true);
    c->cd(3);
    TH1D* py = (TH1D*) (fHM->H2("fhRingTrackDistVsMomTruematch" + s)
                          ->ProjectionY(("fhRingTrackDistVsMomTruematch_py" + s).c_str())
                          ->Clone());
    DrawH1(py);
    DrawTextOnPad(this->GetMeanRmsOverflowString(py), 0.2, 0.9, 0.8, 0.99);
    gPad->SetLogy(true);

    fHM->H2("fhRingTrackDistVsMomTruematch" + s)->GetYaxis()->SetRangeUser(0., 2.);
    fHM->H2("fhRingTrackDistVsNofHitsTruematch" + s)->GetYaxis()->SetRangeUser(0., 2.);
  }

  {
    TCanvas* c =
      fHM->CreateCanvas("fh_ring_track_distance_wrongmatch" + s, "fh_ring_track_distance_wrongmatch" + s, 600, 600);
    TH1D* py = (TH1D*) (fHM->H2("fhRingTrackDistVsMomWrongmatch" + s)
                          ->ProjectionY(("fhRingTrackDistVsMomWrongmatch_py" + s).c_str())
                          ->Clone());
    DrawH1(py);
    DrawTextOnPad(this->GetMeanRmsOverflowString(py), 0.2, 0.9, 0.8, 0.99);
    gPad->SetLogy(true);
  }

  {
    TCanvas* c = fHM->CreateCanvas("fh_ring_track_distance_vs_xy_truematch" + s,
                                   "fh_ring_track_distance_vs_xy_truematch" + s, 1800, 600);
    c->Divide(3, 1);
    c->cd(1);
    DrawH3Profile(fHM->H3("fhRingTrackDistVsXYTruematch" + s), true, false, 0., 2.);
    c->cd(2);
    DrawH2WithProfile(fHM->H2("fhRingTrackDistVsXTruematch" + s), false, true);
    fHM->H2("fhRingTrackDistVsXTruematch" + s)->GetYaxis()->SetRangeUser(0., 1.5);
    c->cd(3);
    DrawH2WithProfile(fHM->H2("fhRingTrackDistVsYTruematch" + s), false, true);
    fHM->H2("fhRingTrackDistVsYTruematch" + s)->GetYaxis()->SetRangeUser(0., 1.5);
  }
}

void CbmRichRecoQa::DrawRingTrackDistHistWithSuffixPmtHalf(const string& s)
{
  {
    TCanvas* c = fHM->CreateCanvas("fh_ring_track_distance_vs_xy_truematch_half_" + s,
                                   "fh_ring_track_distance_vs_xy_truematch_half_" + s, 1800, 600);
    c->Divide(2, 1);
    TH3D* h1 = (TH3D*) fHM->H3("fhRingTrackDistVsXYTruematch" + s)->Clone();
    TH3D* h2 = (TH3D*) fHM->H3("fhRingTrackDistVsXYTruematch" + s)->Clone();
    c->cd(1);
    h1->GetYaxis()->SetRangeUser(110., 200.);
    DrawH3Profile(h1, true, false, 0., 2.);
    c->cd(2);
    h2->GetYaxis()->SetRangeUser(-200., -110.);
    DrawH3Profile(h2, true, false, 0., 2.);
  }

  {
    TCanvas* c = fHM->CreateCanvas("fh_track_ring_distance_diff_x_y_track_vs_xy_truematch_projection_" + s,
                                   "fh_track_ring_distance_diff_x_y_track_vs_xy_truematch_projection_" + s, 1800, 600);
    c->Divide(2, 1);
    TH3D* h1 = (TH3D*) fHM->H3("fhRingTrackDistVsXYTruematch" + s)->Clone();
    c->cd(1);
    TH2D* h2 = (TH2D*) h1->Project3D("yx")->Clone();
    h2->GetYaxis()->SetRangeUser(110., 200.);
    h2->Scale(1. / .15);
    h2->Draw("colz");
    c->cd(2);
    TH2D* h3 = (TH2D*) h1->Project3D("yx")->Clone();
    h3->GetYaxis()->SetRangeUser(-200., -110.);
    h3->Scale(1. / .15);
    h3->Draw("colz");
  }

  Double_t range = 1.;
  {
    TCanvas* c = fHM->CreateCanvas("fh_ring_track_distance_diff_x_y_ring_vs_xy_truematch_up_" + s,
                                   "fh_ring_track_distance_diff_x_y_ring_vs_xy_truematch_up_" + s, 1800, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH3Profile(fHM->H3("fhRingTrackDistDiffXRingVsXYTruematchUp" + s), true, false, -range, range);
    c->cd(2);
    DrawH3Profile(fHM->H3("fhRingTrackDistDiffYRingVsXYTruematchUp" + s), true, false, -range, range);
  }

  {
    TCanvas* c = fHM->CreateCanvas("fh_ring_track_distance_diff_x_y_ring_vs_xy_truematch_down_" + s,
                                   "fh_ring_track_distance_diff_x_y_ring_vs_xy_truematch_down_" + s, 1800, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH3Profile(fHM->H3("fhRingTrackDistDiffXRingVsXYTruematchDown" + s), true, false, -range, range);
    c->cd(2);
    DrawH3Profile(fHM->H3("fhRingTrackDistDiffYRingVsXYTruematchDown" + s), true, false, -range, range);
  }

  /*    {
    	TCanvas* c = fHM->CreateCanvas("fh_track_ring_distance_diff_x_y_ring_single_vs_xy_truematch"+s, "fh_track_ring_distance_diff_x_y_ring_single_vs_xy_truematch"+s, 1800, 600);
    	c->Divide(2, 2);
    	c->cd(1);
    	DrawH3Profile(fHM->H3("fhRingTrackDistDiffXRingVsXYTruematch"+s), true, false, 0, 2*range);
    	c->cd(2);
    	DrawH3Profile(fHM->H3("fhRingTrackDistDiffXRingVsXYTruematch"+s), true, false, -2*range, 0);
		c->cd(3);
    	DrawH3Profile(fHM->H3("fhRingTrackDistDiffYRingVsXYTruematch"+s), true, false, 0, 2*range);
		c->cd(4);
    	DrawH3Profile(fHM->H3("fhRingTrackDistDiffYRingVsXYTruematch"+s), true, false, -2*range, 0);
    }
*/
  {
    TCanvas* c = fHM->CreateCanvas("fh_track_ring_distance_diff_x_y_track_vs_xy_truematch_up_" + s,
                                   "fh_track_ring_distance_diff_x_y_track_vs_xy_truematch_up_" + s, 1800, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH3Profile(fHM->H3("fhRingTrackDistDiffXTrackVsXYTruematchUp" + s), true, false, -range, range);
    c->cd(2);
    DrawH3Profile(fHM->H3("fhRingTrackDistDiffYTrackVsXYTruematchUp" + s), true, false, -range, range);
  }

  {
    TCanvas* c = fHM->CreateCanvas("fh_track_ring_distance_diff_x_y_track_vs_xy_truematch_down_" + s,
                                   "fh_track_ring_distance_diff_x_y_track_vs_xy_truematch_down_" + s, 1800, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH3Profile(fHM->H3("fhRingTrackDistDiffXTrackVsXYTruematchDown" + s), true, false, -range, range);
    c->cd(2);
    DrawH3Profile(fHM->H3("fhRingTrackDistDiffYTrackVsXYTruematchDown" + s), true, false, -range, range);
  }

  {
    TCanvas* c = fHM->CreateCanvas("fh_track_ring_distance_vs_xy_truematch_halves_" + s,
                                   "fh_track_ring_distance_vs_xy_truematch_halves_" + s, 1800, 600);
    c->Divide(2, 1);
    c->cd(1);
    DrawH3Profile(fHM->H3("fhRingTrackDistVsXYHalfUpTruematch" + s), true, false, 0., range);
    c->cd(2);
    DrawH3Profile(fHM->H3("fhRingTrackDistVsXYHalfDownTruematch" + s), true, false, 0., range);
  }
}

bool CbmRichRecoQa::IsMcPrimaryElectron(const CbmMCTrack* mctrack)
{
  if (mctrack == nullptr) return false;
  int pdg = TMath::Abs(mctrack->GetPdgCode());
  if (mctrack->GetGeantProcessId() == kPPrimary && pdg == 11) return true;
  return false;
}

bool CbmRichRecoQa::IsMcPion(const CbmMCTrack* mctrack)
{
  if (mctrack == nullptr) return false;
  int pdg = TMath::Abs(mctrack->GetPdgCode());
  if (pdg == 211) return true;
  return false;
}

void CbmRichRecoQa::Finish()
{
  DrawHist();

  TDirectory* oldir = gDirectory;
  TFile* outFile    = FairRootManager::Instance()->GetOutFile();
  if (outFile != NULL) {
    fHM->WriteToFile();
    //        fHM->WriteToFileNew(fOutputDir + "/histos.root");
  }
  gDirectory->cd(oldir->GetPath());

  fHM->SaveCanvasToImage(fOutputDir + "/" + fCorrection);
  //fHM->H3("fhRingTrackDistVsXYTruematchPrimel")->Write();
}

void CbmRichRecoQa::DrawFromFile(const string& fileName, const string& outputDir)
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

  fHM->SaveCanvasToImage(fOutputDir);

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

ClassImp(CbmRichRecoQa)
