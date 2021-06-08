/* Copyright (C) 2017-2020 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Timur Ablyazimov [committer] */

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

//#define CBM_BINNED_QA_FILL_HISTOS

#include "CbmBinnedTrackerQA.h"

#include "CbmMCDataManager.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmMuchCluster.h"
#include "CbmMuchDigi.h"
#include "CbmMuchGeoScheme.h"
#include "CbmMuchPixelHit.h"
#include "CbmMuchPoint.h"
#include "CbmMuchStation.h"
#include "CbmStsCluster.h"
#include "CbmStsDigi.h"
#include "CbmStsHit.h"
#include "CbmStsPoint.h"
#include "CbmStsSetup.h"
#include "CbmTofPoint.h"
#include "CbmTrdCluster.h"
#include "CbmTrdDigi.h"
#include "global/CbmGlobalTrack.h"
#include "trd/CbmTrdAddress.h"

#include "FairRun.h"
#include "FairRuntimeDb.h"

#include "TH1.h"
#include "TProfile.h"

#include <complex>

#include "GeoReader.h"

using namespace std;

//#define TRD_IDEAL

struct TrackDesc {
  static Int_t nofStsStations;
  static Int_t nofMuchStations;
  static Int_t nofTrdStations;
  static bool hasTof;

  static Int_t firstStsStationNo;
  static Int_t firstMuchStationNo;
  static Int_t firstTrdStationNo;
  static Int_t tofStationNo;

  // In the pairs below
  pair<set<Int_t>, set<Int_t>>* sts;
  pair<set<Int_t>, set<Int_t>>* much;
  pair<set<Int_t>, set<Int_t>>* trd;
  pair<set<Int_t>, set<Int_t>> tof;
  set<const CbmStsPoint*>* stsPoints;
  set<const CbmMuchPoint*>* muchPoints;
  set<const CbmTrdPoint*>* trdPoints;
  bool isPrimary;
  const CbmMCTrack* ptr;
  Double_t* nearestHitDistSts;
  Double_t* nearestHitDistMuch;
  Double_t* nearestHitDistTrd;
  const CbmStsPoint** nearestPointsSts;
  const CbmMuchPoint** nearestPointsMuch;
  const CbmTrdPoint** nearestPointsTrd;
  Double_t* pullXsts;
  Double_t* pullYsts;
  Double_t* pullXmuch;
  Double_t* pullYmuch;
  Double_t* pullXtrd;
  Double_t* pullYtrd;

  list<TrackDesc*> children;
  bool isReference;
  bool isReconstructed;

  TrackDesc()
    : sts(nofStsStations > 0 ? new pair<set<Int_t>, set<Int_t>>[nofStsStations] : 0)
    , much(nofMuchStations > 0 ? new pair<set<Int_t>, set<Int_t>>[nofMuchStations] : 0)
    , trd(nofTrdStations > 0 ? new pair<set<Int_t>, set<Int_t>>[nofTrdStations] : 0)
    , tof()
    , stsPoints(nofStsStations > 0 ? new set<const CbmStsPoint*>[nofStsStations] : 0)
    , muchPoints(nofMuchStations > 0 ? new set<const CbmMuchPoint*>[nofMuchStations] : 0)
    , trdPoints(nofTrdStations > 0 ? new set<const CbmTrdPoint*>[nofTrdStations] : 0)
    , isPrimary(false)
    , ptr(0)
    , nearestHitDistSts(nofStsStations > 0 ? new Double_t[nofStsStations] : 0)
    , nearestHitDistMuch(nofMuchStations > 0 ? new Double_t[nofMuchStations] : 0)
    , nearestHitDistTrd(nofTrdStations > 0 ? new Double_t[nofTrdStations] : 0)
    , nearestPointsSts(nofStsStations > 0 ? new const CbmStsPoint*[nofStsStations] : 0)
    , nearestPointsMuch(nofMuchStations > 0 ? new const CbmMuchPoint*[nofMuchStations] : 0)
    , nearestPointsTrd(nofTrdStations > 0 ? new const CbmTrdPoint*[nofTrdStations] : 0)
    , pullXsts(nofStsStations > 0 ? new Double_t[nofStsStations] : 0)
    , pullYsts(nofStsStations > 0 ? new Double_t[nofStsStations] : 0)
    , pullXmuch(nofMuchStations > 0 ? new Double_t[nofMuchStations] : 0)
    , pullYmuch(nofMuchStations > 0 ? new Double_t[nofMuchStations] : 0)
    , pullXtrd(nofTrdStations > 0 ? new Double_t[nofTrdStations] : 0)
    , pullYtrd(nofTrdStations > 0 ? new Double_t[nofTrdStations] : 0)
    , children()
    , isReference(false)
    , isReconstructed(false)
  {
    fill_n(nearestHitDistSts, nofStsStations, -1);
    fill_n(nearestHitDistMuch, nofMuchStations, -1);
    fill_n(nearestHitDistTrd, nofTrdStations, -1);
    fill_n(nearestPointsSts, nofStsStations, static_cast<const CbmStsPoint*>(0));
    fill_n(nearestPointsMuch, nofMuchStations, static_cast<const CbmMuchPoint*>(0));
    fill_n(nearestPointsTrd, nofTrdStations, static_cast<const CbmTrdPoint*>(0));
    fill_n(pullXsts, nofStsStations, 0);
    fill_n(pullYsts, nofStsStations, 0);
    fill_n(pullXmuch, nofMuchStations, 0);
    fill_n(pullYmuch, nofMuchStations, 0);
    fill_n(pullXtrd, nofTrdStations, 0);
    fill_n(pullYtrd, nofTrdStations, 0);
  }

  ~TrackDesc()
  {
    delete[] sts;
    delete[] much;
    delete[] trd;
    delete[] stsPoints;
    delete[] muchPoints;
    delete[] trdPoints;
    delete[] nearestHitDistSts;
    delete[] nearestHitDistMuch;
    delete[] nearestHitDistTrd;
    delete[] nearestPointsSts;
    delete[] nearestPointsMuch;
    delete[] nearestPointsTrd;
    delete[] pullXsts;
    delete[] pullYsts;
    delete[] pullXmuch;
    delete[] pullYmuch;
    delete[] pullXtrd;
    delete[] pullYtrd;
  }

  TrackDesc(const TrackDesc&) = default;
  TrackDesc& operator=(const TrackDesc&) = default;
};

Int_t TrackDesc::nofStsStations  = 0;
Int_t TrackDesc::nofMuchStations = 0;
Int_t TrackDesc::nofTrdStations  = 0;
bool TrackDesc::hasTof           = false;

Int_t TrackDesc::firstStsStationNo  = 0;
Int_t TrackDesc::firstMuchStationNo = 0;
Int_t TrackDesc::firstTrdStationNo  = 0;
Int_t TrackDesc::tofStationNo       = 0;

static vector<vector<TrackDesc>> gTracks;
static vector<vector<bool>> gStsPoints;
static vector<vector<bool>> gMuchPoints;
static vector<vector<bool>> gTrdPoints;
static vector<vector<char>> gTofPoints;

static TProfile* effByMom           = 0;
static TProfile* effByMomPrimary    = 0;
static TProfile* effByMomNonPrimary = 0;

static TProfile* effByPolarAngle           = 0;
static TProfile* effByPolarAnglePrimary    = 0;
static TProfile* effByPolarAngleNonPrimary = 0;

static TProfile* effByXAngle           = 0;
static TProfile* effByXAnglePrimary    = 0;
static TProfile* effByXAngleNonPrimary = 0;

static TProfile* effByYAngle           = 0;
static TProfile* effByYAnglePrimary    = 0;
static TProfile* effByYAngleNonPrimary = 0;

static TH1F* lambdaChildrenMoms         = 0;
static TProfile* lambdaChildrenEffByMom = 0;
static TH1F* clonesNofSameHits          = 0;

static TH1F* muchHitResidualX = 0;
static TH1F* muchHitResidualY = 0;
static TH1F* muchHitResidualT = 0;
static TH1F* muchHitPullX     = 0;
static TH1F* muchHitPullY     = 0;
static TH1F* muchHitPullT     = 0;

static TH1F* stsTrackResidualFirstX = 0;
static TH1F* stsTrackResidualFirstY = 0;
static TH1F* stsTrackPullFirstX     = 0;
static TH1F* stsTrackPullFirstY     = 0;
static TH1F* stsTrackResidualLastX  = 0;
static TH1F* stsTrackResidualLastY  = 0;
static TH1F* stsTrackPullLastX      = 0;
static TH1F* stsTrackPullLastY      = 0;

static TH1F* muchTrackResidualFirstX = 0;
static TH1F* muchTrackResidualFirstY = 0;
static TH1F* muchTrackPullFirstX     = 0;
static TH1F* muchTrackPullFirstY     = 0;
static TH1F* muchTrackResidualLastX  = 0;
static TH1F* muchTrackResidualLastY  = 0;
static TH1F* muchTrackPullLastX      = 0;
static TH1F* muchTrackPullLastY      = 0;

static TH1F* trdTrackResidualFirstX = 0;
static TH1F* trdTrackResidualFirstY = 0;
static TH1F* trdTrackPullFirstX     = 0;
static TH1F* trdTrackPullFirstY     = 0;
static TH1F* trdTrackResidualLastX  = 0;
static TH1F* trdTrackResidualLastY  = 0;
static TH1F* trdTrackPullLastX      = 0;
static TH1F* trdTrackPullLastY      = 0;

static TH1F* globalTrackResidualFirstX = 0;
static TH1F* globalTrackResidualFirstY = 0;
static TH1F* globalTrackPullFirstX     = 0;
static TH1F* globalTrackPullFirstY     = 0;
static TH1F* globalTrackResidualLastX  = 0;
static TH1F* globalTrackResidualLastY  = 0;
static TH1F* globalTrackPullLastX      = 0;
static TH1F* globalTrackPullLastY      = 0;

#ifdef CBM_BINNED_QA_FILL_HISTOS
static TH1F* stsXResHisto  = 0;
static TH1F* stsYResHisto  = 0;
static TH1F* stsTResHisto  = 0;
static TH1F* trdXResHisto  = 0;
static TH1F* trdYResHisto  = 0;
static TH1F* trdTResHisto  = 0;
static TH1F* muchXResHisto = 0;
static TH1F* muchYResHisto = 0;
static TH1F* muchTResHisto = 0;
static TH1F* tofXResHisto  = 0;
static TH1F* tofYResHisto  = 0;
static TH1F* tofTResHisto  = 0;

static TH1F* stsXPullHistos[] = {0, 0};
static TH1F* stsYPullHistos[] = {0, 0};

static TH1F* trdXPullHistos[] = {0, 0, 0, 0};
static TH1F* trdYPullHistos[] = {0, 0, 0, 0};

static TH1F* extrStsXHisto = 0;
static TH1F* extrStsYHisto = 0;

static TH1F* vtxXHisto = 0;
static TH1F* vtxYHisto = 0;
static TH1F* vtxZHisto = 0;

static TH1F* extrTrdXHistos[] = {0, 0, 0, 0};
static TH1F* extrTrdYHistos[] = {0, 0, 0, 0};

static TH1F* trdNearestHitDistHistos[] = {0, 0, 0, 0};
#endif  //CBM_BINNED_QA_FILL_HISTOS

//static int trdNofStrangerHits[] = { 0, 0, 0, 0 };

static list<TrackDesc*> lambdaList;

CbmBinnedTrackerQA::CbmBinnedTrackerQA()
  : fPrimaryParticleIds()
  , fIsOnlyPrimary(false)
  , fSettings(0)
  , fGlobalTracks(0)
  , fStsTracks(0)
  , fMuchTracks(0)
  , fTrdTracks(0)
  , fStsHits(0)
  , fMuchHits(0)
  , fTrdHits(0)
  , fTofHits(0)
  , fStsClusters(0)
  , fMuchClusters(0)
  , fTrdClusters(0)
  , fTrdDigiMatches(0)
  , fTofHitDigiMatches(0)
  , fTofDigiPointMatches(0)
  , fStsDigis(0)
  , fStsDigiMatches(0)
  , fMuchDigis(0)
  , fTrdDigis(0)
  , fTofDigis(0)
  , fMCTracks(0)
  , fStsPoints(0)
  , fMuchPoints(0)
  , fTrdPoints(0)
  , fTofPoints(0)
{
  fPrimaryParticleIds.push_back(ppiNone);
}

InitStatus CbmBinnedTrackerQA::Init()
{
  CbmStsSetup* stsSetup = CbmStsSetup::Instance();

  if (!stsSetup->IsInit()) stsSetup->Init();

  fSettings                 = CbmBinnedSettings::Instance();
  fIsOnlyPrimary            = fSettings->IsOnlyPrimary();
  TrackDesc::nofStsStations = fSettings->Use(ECbmModuleId::kSts) ? fSettings->GetNofStsStations() : 0;
  LOG(info) << "The number of STS stations: " << TrackDesc::nofStsStations;
  TrackDesc::nofMuchStations = fSettings->Use(ECbmModuleId::kMuch) ? fSettings->GetNofMuchStations() : 0;
  LOG(info) << "The number of MuCh stations: " << TrackDesc::nofMuchStations;
  TrackDesc::nofTrdStations = fSettings->Use(ECbmModuleId::kTrd) ? fSettings->GetNofTrdStations() : 0;
  LOG(info) << "The number of TRD stations: " << TrackDesc::nofTrdStations;
  TrackDesc::hasTof = fSettings->Use(ECbmModuleId::kTof);
  LOG(info) << "Use ToF station: " << (TrackDesc::hasTof ? "true" : "false");

  TrackDesc::firstMuchStationNo = TrackDesc::nofStsStations;
  TrackDesc::firstTrdStationNo  = TrackDesc::firstMuchStationNo + TrackDesc::nofMuchStations;
  TrackDesc::tofStationNo       = TrackDesc::firstTrdStationNo + TrackDesc::nofTrdStations;

  effByMom = new TProfile("effByMom", "Track reconstruction efficiency by momentum distribution %", 400, 0., 10.);
  effByMomPrimary    = new TProfile("effByMomPrimary",
                                 "Track reconstruction efficiency by momentum "
                                 "distribution for primary tracks %",
                                 400, 0., 10.);
  effByMomNonPrimary = new TProfile("effByMomNonPrimary",
                                    "Track reconstruction efficiency by momentum distribution for "
                                    "non primary tracks %",
                                    200, 0., 10.);

  effByPolarAngle =
    new TProfile("effByPolarAngle", "Track reconstruction efficiency by polar angle distribution %", 300, 0., 30.);
  effByPolarAnglePrimary    = new TProfile("effByPolarAnglePrimary",
                                        "Track reconstruction efficiency by polar angle distribution "
                                        "for primary tracks %",
                                        300, 0., 30.);
  effByPolarAngleNonPrimary = new TProfile("effByPolarAngleNonPrimary",
                                           "Track reconstruction efficiency by polar angle distribution "
                                           "for non primary tracks %",
                                           300, 0., 30.);

  effByXAngle = new TProfile("effByXAngle", "Track reconstruction efficiency by XZ angle distribution %", 300, 0., 30.);
  effByXAnglePrimary    = new TProfile("effByXAnglePrimary",
                                    "Track reconstruction efficiency by XZ "
                                    "angle distribution for primary tracks %",
                                    300, 0., 30.);
  effByXAngleNonPrimary = new TProfile("effByXAngleNonPrimary",
                                       "Track reconstruction efficiency by XZ angle distribution for "
                                       "non primary tracks %",
                                       300, 0., 30.);

  effByYAngle = new TProfile("effByYAngle", "Track reconstruction efficiency by YZ angle distribution %", 300, 0., 30.);
  effByYAnglePrimary    = new TProfile("effByYAnglePrimary",
                                    "Track reconstruction efficiency by YZ "
                                    "angle distribution for primary tracks %",
                                    300, 0., 30.);
  effByYAngleNonPrimary = new TProfile("effByYAngleNonPrimary",
                                       "Track reconstruction efficiency by YZ angle distribution for "
                                       "non primary tracks %",
                                       300, 0., 30.);

  lambdaChildrenMoms     = new TH1F("lambdaChildrenMoms", "Lambda children momenta distribution", 100, 0., 10.);
  lambdaChildrenEffByMom = new TProfile("lambdaChildrenEffByMom",
                                        "Track reconstruction for Lambda children efficiency by "
                                        "momentum distribution %",
                                        200, 0., 10.);
  clonesNofSameHits =
    new TH1F("clonesNofSameHits", "The number of hits which are the same for a clone track", 10, 0., 10.);

  muchHitResidualX = new TH1F("muchHitResidualX", "muchHitResidualX", 100, -5.0, 5.0);
  muchHitResidualY = new TH1F("muchHitResidualY", "muchHitResidualY", 100, -5.0, 5.0);
  muchHitResidualT = new TH1F("muchHitResidualT", "muchHitResidualT", 100, -20.0, 20.0);
  muchHitPullX     = new TH1F("muchHitPullX", "muchHitPullX", 100, -5.0, 5.0);
  muchHitPullY     = new TH1F("muchHitPullY", "muchHitPullY", 100, -5.0, 5.0);
  muchHitPullT     = new TH1F("muchHitPullT", "muchHitPullT", 100, -5.0, 5.0);

  stsTrackResidualFirstX = new TH1F("stsTrackResidualFirstX", "stsTrackResidualFirstX", 100, -0.1, 0.1);
  stsTrackResidualFirstY = new TH1F("stsTrackResidualFirstY", "stsTrackResidualFirstY", 100, -0.1, 0.1);
  stsTrackPullFirstX     = new TH1F("stsTrackPullFirstX", "stsTrackPullFirstX", 100, -5.0, 5.0);
  stsTrackPullFirstY     = new TH1F("stsTrackPullFirstY", "stsTrackPullFirstY", 100, -5.0, 5.0);
  stsTrackResidualLastX  = new TH1F("stsTrackResidualLastX", "stsTrackResidualLastX", 100, -0.1, 0.1);
  stsTrackResidualLastY  = new TH1F("stsTrackResidualLastY", "stsTrackResidualLastY", 100, -0.1, 0.1);
  stsTrackPullLastX      = new TH1F("stsTrackPullLastX", "stsTrackPullLastX", 100, -5.0, 5.0);
  stsTrackPullLastY      = new TH1F("stsTrackPullLastY", "stsTrackPullLastY", 100, -5.0, 5.0);

  muchTrackResidualFirstX = new TH1F("muchTrackResidualFirstX", "muchTrackResidualFirstX", 100, -5.0, 5.0);
  muchTrackResidualFirstY = new TH1F("muchTrackResidualFirstY", "muchTrackResidualFirstY", 100, -5.0, 5.0);
  muchTrackPullFirstX     = new TH1F("muchTrackPullFirstX", "muchTrackPullFirstX", 100, -10.0, 10.0);
  muchTrackPullFirstY     = new TH1F("muchTrackPullFirstY", "muchTrackPullFirstY", 100, -10.0, 10.0);
  muchTrackResidualLastX  = new TH1F("muchTrackResidualLastX", "muchTrackResidualLastX", 100, -5.0, 5.0);
  muchTrackResidualLastY  = new TH1F("muchTrackResidualLastY", "muchTrackResidualLastY", 100, -5.0, 5.0);
  muchTrackPullLastX      = new TH1F("muchTrackPullLastX", "muchTrackPullLastX", 100, -10.0, 10.0);
  muchTrackPullLastY      = new TH1F("muchTrackPullLastY", "muchTrackPullLastY", 100, -10.0, 10.0);

  trdTrackResidualFirstX = new TH1F("trdTrackResidualFirstX", "trdTrackResidualFirstX", 100, -10.0, 10.0);
  trdTrackResidualFirstY = new TH1F("trdTrackResidualFirstY", "trdTrackResidualFirstY", 100, -10.0, 10.0);
  trdTrackPullFirstX     = new TH1F("trdTrackPullFirstX", "trdTrackPullFirstX", 100, -5.0, 5.0);
  trdTrackPullFirstY     = new TH1F("trdTrackPullFirstY", "trdTrackPullFirstY", 100, -5.0, 5.0);
  trdTrackResidualLastX  = new TH1F("trdTrackResidualLastX", "trdTrackResidualLastX", 100, -10.0, 10.0);
  trdTrackResidualLastY  = new TH1F("trdTrackResidualLastY", "trdTrackResidualLastY", 100, -10.0, 10.0);
  trdTrackPullLastX      = new TH1F("trdTrackPullLastX", "trdTrackPullLastX", 100, -5.0, 5.0);
  trdTrackPullLastY      = new TH1F("trdTrackPullLastY", "trdTrackPullLastY", 100, -5.0, 5.0);

  globalTrackResidualFirstX = new TH1F("globalTrackResidualFirstX", "globalTrackResidualFirstX", 100, -0.2, 0.2);
  globalTrackResidualFirstY = new TH1F("globalTrackResidualFirstY", "globalTrackResidualFirstY", 100, -0.2, 0.2);
  globalTrackPullFirstX     = new TH1F("globalTrackPullFirstX", "globalTrackPullFirstX", 100, -5.0, 5.0);
  globalTrackPullFirstY     = new TH1F("globalTrackPullFirstY", "globalTrackPullFirstY", 100, -5.0, 5.0);
  globalTrackResidualLastX  = new TH1F("globalTrackResidualLastX", "globalTrackResidualLastX", 100, -1.5, 1.5);
  globalTrackResidualLastY  = new TH1F("globalTrackResidualLastY", "globalTrackResidualLastY", 100, -1.5, 1.5);
  globalTrackPullLastX      = new TH1F("globalTrackPullLastX", "globalTrackPullLastX", 100, -5.0, 5.0);
  globalTrackPullLastY      = new TH1F("globalTrackPullLastY", "globalTrackPullLastY", 100, -5.0, 5.0);

#ifdef CBM_BINNED_QA_FILL_HISTOS
  stsXResHisto  = new TH1F("stsXResHisto", "stsXResHisto", 200, -0.1, 0.1);
  stsYResHisto  = new TH1F("stsYResHisto", "stsYResHisto", 200, -0.1, 0.1);
  stsTResHisto  = new TH1F("stsTResHisto", "stsTResHisto", 200, -10.0, 10.0);
  trdXResHisto  = new TH1F("trdXResHisto", "trdXResHisto", 600, -60.0, 60.0);
  trdYResHisto  = new TH1F("trdYResHisto", "trdYResHisto", 600, -60.0, 60.0);
  trdTResHisto  = new TH1F("trdTResHisto", "trdTResHisto", 200, -10.0, 10.0);
  muchXResHisto = new TH1F("muchXResHisto", "muchXResHisto", 200, -3.0, 3.0);
  muchYResHisto = new TH1F("muchYResHisto", "muchYResHisto", 200, -3.0, 3.0);
  muchTResHisto = new TH1F("muchTResHisto", "muchTResHisto", 200, -10.0, 100.0);
  tofXResHisto  = new TH1F("tofXResHisto", "tofXResHisto", 200, -3.0, 3.0);
  tofYResHisto  = new TH1F("tofYResHisto", "tofYResHisto", 200, -3.0, 3.0);
  tofTResHisto  = new TH1F("tofTResHisto", "tofTResHisto", 200, -10.0, 10.0);

  extrStsXHisto = new TH1F("extrStsXHisto", "extrStsXHisto", 200, -0.5, 0.5);
  extrStsYHisto = new TH1F("extrStsYHisto", "extrStsYHisto", 200, -0.5, 0.5);

  vtxXHisto = new TH1F("vtxXHisto", "vtxXHisto", 100, -0.5, 0.5);
  vtxYHisto = new TH1F("vtxYHisto", "vtxYHisto", 100, -0.5, 0.5);
  vtxZHisto = new TH1F("vtxZHisto", "vtxZHisto", 100, -0.5, 0.5);

  for (int i = 0; i < 2; ++i) {
    char name[256];
    sprintf(name, "stsXPullHistos_%d", i);
    Double_t range    = 5;
    stsXPullHistos[i] = new TH1F(name, name, 200, -range, range);
    sprintf(name, "stsYPullHistos_%d", i);
    stsYPullHistos[i] = new TH1F(name, name, 200, -range, range);
  }

  for (int i = 0; i < 4; ++i) {
    char name[256];
    sprintf(name, "extrTrdXHisto_%d", i);
    extrTrdXHistos[i] = new TH1F(name, name, 200, 0.5, 0.5);
    sprintf(name, "extrTrdYHisto_%d", i);
    extrTrdYHistos[i] = new TH1F(name, name, 200, 0.5, 0.5);

    sprintf(name, "trdNearestHitDistHistos_%d", i);
    trdNearestHitDistHistos[i] = new TH1F(name, name, 200, -100, 100);

    sprintf(name, "trdXPullHistos_%d", i);
    Double_t range    = i % 2 ? 5 : 25;
    trdXPullHistos[i] = new TH1F(name, name, 200, -range, range);
    sprintf(name, "trdYPullHistos_%d", i);
    range             = i % 2 ? 25 : 5;
    trdYPullHistos[i] = new TH1F(name, name, 200, -range, range);
  }
#endif  //CBM_BINNED_QA_FILL_HISTOS

  FairRootManager* ioman = FairRootManager::Instance();

  if (0 == ioman) LOG(fatal) << "No FairRootManager";

  CbmMCDataManager* mcManager = static_cast<CbmMCDataManager*>(ioman->GetObject("MCDataManager"));

  if (0 == mcManager) LOG(fatal) << "No MC data manager";

  fMCTracks = mcManager->InitBranch("MCTrack");

  if (0 == fMCTracks) LOG(fatal) << "No MC tracks in the input file";

  for (Int_t i = 0; fMCTracks->Size(0, i) >= 0; ++i) {
    gTracks.push_back(vector<TrackDesc>());
    gStsPoints.push_back(vector<bool>());
    gMuchPoints.push_back(vector<bool>());
    gTrdPoints.push_back(vector<bool>());
    gTofPoints.push_back(vector<char>());

    Int_t nofMcTracks              = fMCTracks->Size(0, i);
    vector<TrackDesc>& eventTracks = gTracks.back();
    eventTracks.resize(nofMcTracks);

    for (Int_t j = 0; j < nofMcTracks; ++j) {
      TrackDesc& track          = eventTracks[j];
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(fMCTracks->Get(0, i, j));
      track.ptr                 = mcTrack;

      bool isPrimary         = false;
      Int_t motherId         = mcTrack->GetMotherId();
      TrackDesc* motherTrack = 0 > motherId ? 0 : &eventTracks[motherId];

      for (EPrimaryParticleId ppi : fPrimaryParticleIds) {
        switch (ppi) {
          case ppiJpsi: {
            if (motherId >= 0) isPrimary = 443 == motherTrack->ptr->GetPdgCode();

            break;
          }

          default: isPrimary = motherId < 0;
        }

        if (isPrimary) break;
      }

      if (isPrimary) {
        track.isPrimary = true;
#ifdef CBM_BINNED_QA_FILL_HISTOS
        vtxXHisto->Fill(mcTrack->GetStartX());
        vtxYHisto->Fill(mcTrack->GetStartY());
        vtxZHisto->Fill(mcTrack->GetStartZ());
#endif  //CBM_BINNED_QA_FILL_HISTOS
      }
      else if (motherId >= 0) {
        if (motherTrack->ptr->GetPdgCode() == 3122)  // Mother particle is a Lambda baryon
          motherTrack->children.push_back(&track);
      }

      if (mcTrack->GetPdgCode() == 3122)  // Lambda baryon
        lambdaList.push_back(&track);
    }
  }

  LOG(info) << "The number of MC events in the input: " << gTracks.size();

  fGlobalTracks = static_cast<TClonesArray*>(ioman->GetObject("GlobalTrack"));

  if (0 == fGlobalTracks) LOG(fatal) << "No global tracks in the input file";

  if (TrackDesc::nofStsStations > 0) {
    fStsTracks = static_cast<TClonesArray*>(ioman->GetObject("StsTrack"));

    if (0 == fStsTracks) LOG(fatal) << "No sts tracks in the input file";

    fStsHits = static_cast<TClonesArray*>(ioman->GetObject("StsHit"));

    if (0 == fStsHits) LOG(fatal) << "No sts hits in the input file";

    fStsClusters = static_cast<TClonesArray*>(ioman->GetObject("StsCluster"));

    if (0 == fStsClusters) LOG(fatal) << "No sts clusters in the input file";

    fStsDigis = static_cast<TClonesArray*>(ioman->GetObject("StsDigi"));

    if (0 == fStsDigis) LOG(fatal) << "No sts digis in the input file";

    fStsDigiMatches = static_cast<TClonesArray*>(ioman->GetObject("StsDigiMatch"));

    if (0 == fStsDigiMatches) LOG(fatal) << "No sts digi matches in the input file";

    fStsPoints = mcManager->InitBranch("StsPoint");

    if (0 == fStsPoints) LOG(fatal) << "No sts MC points in the input file";

    for (Int_t i = 0; fStsPoints->Size(0, i) >= 0; ++i) {
      Int_t nofPoints = fStsPoints->Size(0, i);
      gStsPoints[i].resize(nofPoints, false);
      vector<TrackDesc>& tracks = gTracks[i];

      for (Int_t j = 0; j < nofPoints; ++j) {
        const CbmStsPoint* stsPoint = static_cast<const CbmStsPoint*>(fStsPoints->Get(0, i, j));
        Int_t trackId               = stsPoint->GetTrackID();
        Int_t stationNumber         = CbmStsSetup::Instance()->GetStationNumber(stsPoint->GetDetectorID());
        //Int_t stationNumber = CbmStsAddress::GetElementId(stsPoint->GetDetectorID(), kSts);
        //tracks[trackId].sts[stationNumber].first.insert(j);
        TrackDesc& trackDesk = tracks[trackId];
        trackDesk.stsPoints[stationNumber].insert(stsPoint);
      }
    }
  }

  if (TrackDesc::nofMuchStations > 0) {
    fMuchTracks = static_cast<TClonesArray*>(ioman->GetObject("MuchTrack"));

    if (0 == fMuchTracks) LOG(fatal) << "No much tracks in the input file";

    fMuchHits = static_cast<TClonesArray*>(ioman->GetObject("MuchPixelHit"));

    if (0 == fMuchHits) LOG(fatal) << "No much hits in the input file";

    fMuchClusters = static_cast<TClonesArray*>(ioman->GetObject("MuchCluster"));

    if (0 == fMuchClusters) LOG(fatal) << "No much clusters in the input file";

    fMuchDigis = static_cast<TClonesArray*>(ioman->GetObject("MuchDigi"));

    if (0 == fMuchDigis) LOG(fatal) << "No much digis in the input file";

    fMuchDigiMatches = static_cast<TClonesArray*>(ioman->GetObject("MuchDigiMatch"));

    if (0 == fMuchDigiMatches) LOG(fatal) << "No much digi matches in the input file";

    fMuchPoints = mcManager->InitBranch("MuchPoint");

    if (0 == fMuchPoints) LOG(fatal) << "No much MC points in the input file";

    for (Int_t i = 0; fMuchPoints->Size(0, i) >= 0; ++i) {
      Int_t nofPoints = fMuchPoints->Size(0, i);
      gMuchPoints[i].resize(nofPoints, false);
      vector<TrackDesc>& tracks = gTracks[i];

      for (Int_t j = 0; j < nofPoints; ++j) {
        const CbmMuchPoint* muchPoint = static_cast<const CbmMuchPoint*>(fMuchPoints->Get(0, i, j));
        Int_t trackId                 = muchPoint->GetTrackID();
        int muchStationNumber         = CbmMuchGeoScheme::GetStationIndex(muchPoint->GetDetectorID());
        int layerNumber               = CbmMuchGeoScheme::GetLayerIndex(muchPoint->GetDetectorID());
        int stationNumber             = muchStationNumber * 3 + layerNumber;
        //tracks[trackId].much[stationNumber].first.insert(j);
        TrackDesc& trackDesk = tracks[trackId];
        trackDesk.muchPoints[stationNumber].insert(muchPoint);
      }
    }
  }

  if (TrackDesc::nofTrdStations > 0) {
    fTrdTracks = static_cast<TClonesArray*>(ioman->GetObject("TrdTrack"));

    if (0 == fTrdTracks) LOG(fatal) << "No trd tracks in the input file";

    fTrdHits = static_cast<TClonesArray*>(ioman->GetObject("TrdHit"));

    if (0 == fTrdHits) LOG(fatal) << "No trd hits in the input file";

#ifdef TRD_IDEAL
    fTrdDigis = static_cast<TClonesArray*>(ioman->GetObject("TrdDigi"));

    if (0 == fTrdDigis) LOG(fatal) << "No trd digis in the input file";
#else  //TRD_IDEAL
    fTrdClusters = static_cast<TClonesArray*>(ioman->GetObject("TrdCluster"));

    if (0 == fTrdClusters) LOG(fatal) << "No global tracks in the input file";

    fTrdDigiMatches = static_cast<TClonesArray*>(ioman->GetObject("TrdDigiMatch"));

    if (0 == fTrdDigiMatches) LOG(fatal) << "No trd hit to digi matches in the input file";
#endif  //TRD_IDEAL

    fTrdPoints = mcManager->InitBranch("TrdPoint");

    if (0 == fTrdPoints) LOG(fatal) << "No trd MC points in the input file";

    for (Int_t i = 0; fTrdPoints->Size(0, i) >= 0; ++i) {
      Int_t nofPoints = fTrdPoints->Size(0, i);
      gTrdPoints[i].resize(nofPoints, false);
      vector<TrackDesc>& tracks = gTracks[i];

      for (Int_t j = 0; j < nofPoints; ++j) {
        const CbmTrdPoint* trdPoint = static_cast<const CbmTrdPoint*>(fTrdPoints->Get(0, i, j));
        Int_t trackId               = trdPoint->GetTrackID();
        int stationNumber           = CbmTrdAddress::GetLayerId(trdPoint->GetModuleAddress());
        //tracks[trackId].trd[stationNumber].first.insert(j);
        TrackDesc& trackDesk = tracks[trackId];
        trackDesk.trdPoints[stationNumber].insert(trdPoint);
      }
    }
  }

  if (TrackDesc::hasTof) {
    fTofHits = static_cast<TClonesArray*>(ioman->GetObject("TofHit"));

    if (0 == fTofHits) LOG(fatal) << "No tof hits in the input file";

    fTofHitDigiMatches = static_cast<TClonesArray*>(ioman->GetObject("TofDigiMatch"));

    if (0 == fTofHitDigiMatches) LOG(fatal) << "No tof hit to digi matches in the input file";

    fTofDigiPointMatches = static_cast<TClonesArray*>(ioman->GetObject("TofDigiMatchPoints"));

    if (0 == fTofDigiPointMatches) LOG(fatal) << "No tof digi to point matches in the input file";

    fTofDigis = static_cast<TClonesArray*>(ioman->GetObject("TofDigi"));

    if (0 == fTofDigis) LOG(fatal) << "No tof digis in the input file";

    fTofPoints = mcManager->InitBranch("TofPoint");

    if (0 == fTofPoints) LOG(fatal) << "No tof MC points in the input file";

    for (Int_t i = 0; fTofPoints->Size(0, i) >= 0; ++i) {
      Int_t nofPoints           = fTofPoints->Size(0, i);
      vector<TrackDesc>& tracks = gTracks[i];
      gTofPoints[i].resize(tracks.size(), 0);

      for (Int_t j = 0; j < nofPoints; ++j) {
        const CbmTofPoint* tofPoint = static_cast<const CbmTofPoint*>(fTofPoints->Get(0, i, j));
        Int_t trackId               = tofPoint->GetTrackID();
        gTofPoints[i][trackId]      = 1;
      }
    }
  }

  return kSUCCESS;
}

void CbmBinnedTrackerQA::IterateTrdHits(std::function<void(const CbmTrdHit*, const CbmTrdPoint*)> handleData)
{
  Int_t nofTrdHits = fTrdHits->GetEntriesFast();

  for (Int_t i = 0; i < nofTrdHits; ++i) {
    const CbmTrdHit* trdHit = static_cast<const CbmTrdHit*>(fTrdHits->At(i));
    //Int_t stationNumber = trdHit->GetPlaneId();   (VF) unused
    Int_t clusterId              = trdHit->GetRefId();
    const CbmTrdCluster* cluster = static_cast<const CbmTrdCluster*>(fTrdClusters->At(clusterId));
    Int_t nofDigis               = cluster->GetNofDigis();

    for (Int_t j = 0; j < nofDigis; ++j) {
      Int_t digiId          = cluster->GetDigi(j);
      const CbmMatch* match = static_cast<const CbmMatch*>(fTrdDigiMatches->At(digiId));
      Int_t nofLinks        = match->GetNofLinks();

      for (Int_t k = 0; k < nofLinks; ++k) {
        const CbmLink& link         = match->GetLink(k);
        Int_t eventId               = link.GetEntry();
        Int_t mcPointId             = link.GetIndex();
        const CbmTrdPoint* trdPoint = static_cast<const CbmTrdPoint*>(fTrdPoints->Get(0, eventId, mcPointId));
        handleData(trdHit, trdPoint);
      }
    }
  }  // TRD hits
}

static Int_t gEventNumber   = 0;
static Int_t gNofRecoTracks = 0;
static Int_t gNofNonGhosts  = 0;
static Int_t gNofClones     = 0;
static vector<bool> gIsRecoClone;

void CbmBinnedTrackerQA::Exec(Option_t*)
{
  gIsRecoClone.clear();

  if (TrackDesc::nofStsStations > 0) {
    Int_t nofStsHits = fStsHits->GetEntriesFast();

    for (Int_t i = 0; i < nofStsHits; ++i) {
      const CbmStsHit* stsHit = static_cast<const CbmStsHit*>(fStsHits->At(i));
      Int_t stationNumber     = CbmStsSetup::Instance()->GetStationNumber(stsHit->GetAddress());
      //Int_t stationNumber = CbmStsAddress::GetElementId(stsHit->GetAddress(), kSts);
      Int_t frontClusterInd             = stsHit->GetFrontClusterId();
      Int_t backClusterInd              = stsHit->GetBackClusterId();
      const CbmStsCluster* frontCluster = static_cast<const CbmStsCluster*>(fStsClusters->At(frontClusterInd));
      Int_t nofFrontDigis               = frontCluster->GetNofDigis();

      for (Int_t j = 0; j < nofFrontDigis; ++j) {
        Int_t stsDigiInd = frontCluster->GetDigi(j);
        //const CbmStsDigi* stsDigi = static_cast<const CbmStsDigi*> (fStsDigis->At(stsDigiInd));
        const CbmMatch* stsDigiMatch = static_cast<const CbmMatch*>(fStsDigiMatches->At(stsDigiInd));
        Int_t nofLinks               = stsDigiMatch->GetNofLinks();

        for (Int_t k = 0; k < nofLinks; ++k) {
          const CbmLink& link            = stsDigiMatch->GetLink(k);
          Int_t eventId                  = link.GetEntry();
          Int_t mcPointId                = link.GetIndex();
          gStsPoints[eventId][mcPointId] = true;
          const CbmStsPoint* stsPoint    = static_cast<const CbmStsPoint*>(fStsPoints->Get(0, eventId, mcPointId));
#ifdef CBM_BINNED_QA_FILL_HISTOS
          stsXResHisto->Fill(stsHit->GetX() - (stsPoint->GetXIn() + stsPoint->GetXOut()) / 2);
          stsYResHisto->Fill(stsHit->GetY() - (stsPoint->GetYIn() + stsPoint->GetYOut()) / 2);
          stsTResHisto->Fill(stsHit->GetTime() - stsPoint->GetTime());
#endif  //CBM_BINNED_QA_FILL_HISTOS
          Int_t trackId        = stsPoint->GetTrackID();
          TrackDesc& trackDesk = gTracks[eventId][trackId];
          trackDesk.sts[stationNumber].first.insert(i);
          Double_t mcX = (stsPoint->GetXIn() + stsPoint->GetXOut()) / 2;
          Double_t mcY = (stsPoint->GetYIn() + stsPoint->GetYOut()) / 2;

          if (trackDesk.isPrimary && !trackDesk.stsPoints[0].empty() && !trackDesk.stsPoints[1].empty()) {
            Double_t dist = std::sqrt((stsHit->GetX() - mcX) * (stsHit->GetX() - mcX)
                                      + (stsHit->GetY() - mcY) * (stsHit->GetY() - mcY));

            if (trackDesk.nearestHitDistSts[stationNumber] < 0 || dist < trackDesk.nearestHitDistSts[stationNumber]) {
              trackDesk.nearestHitDistSts[stationNumber] = dist;
              trackDesk.nearestPointsSts[stationNumber]  = stsPoint;
              trackDesk.pullXsts[stationNumber]          = (stsHit->GetX() - mcX) / stsHit->GetDx();
              trackDesk.pullYsts[stationNumber]          = (stsHit->GetY() - mcY) / stsHit->GetDy();
            }
          }
        }
      }

      const CbmStsCluster* backCluster = static_cast<const CbmStsCluster*>(fStsClusters->At(backClusterInd));
      Int_t nofBackDigis               = backCluster->GetNofDigis();

      for (Int_t j = 0; j < nofBackDigis; ++j) {
        Int_t stsDigiInd = backCluster->GetDigi(j);
        //const CbmStsDigi* stsDigi = static_cast<const CbmStsDigi*> (fStsDigis->At(stsDigiInd));
        const CbmMatch* stsDigiMatch = static_cast<const CbmMatch*>(fStsDigiMatches->At(stsDigiInd));
        Int_t nofLinks               = stsDigiMatch->GetNofLinks();

        for (Int_t k = 0; k < nofLinks; ++k) {
          const CbmLink& link            = stsDigiMatch->GetLink(k);
          Int_t eventId                  = link.GetEntry();
          Int_t mcPointId                = link.GetIndex();
          gStsPoints[eventId][mcPointId] = true;
          const CbmStsPoint* stsPoint    = static_cast<const CbmStsPoint*>(fStsPoints->Get(0, eventId, mcPointId));
#ifdef CBM_BINNED_QA_FILL_HISTOS
          stsXResHisto->Fill(stsHit->GetX() - (stsPoint->GetXIn() + stsPoint->GetXOut()) / 2);
          stsYResHisto->Fill(stsHit->GetY() - (stsPoint->GetYIn() + stsPoint->GetYOut()) / 2);
          stsTResHisto->Fill(stsHit->GetTime() - stsPoint->GetTime());
#endif  //CBM_BINNED_QA_FILL_HISTOS
          Int_t trackId        = stsPoint->GetTrackID();
          TrackDesc& trackDesk = gTracks[eventId][trackId];
          trackDesk.sts[stationNumber].first.insert(i);
          Double_t mcX = (stsPoint->GetXIn() + stsPoint->GetXOut()) / 2;
          Double_t mcY = (stsPoint->GetYIn() + stsPoint->GetYOut()) / 2;

          if (trackDesk.isPrimary && !trackDesk.stsPoints[0].empty() && !trackDesk.stsPoints[1].empty()) {
            Double_t dist = std::sqrt((stsHit->GetX() - mcX) * (stsHit->GetX() - mcX)
                                      + (stsHit->GetY() - mcY) * (stsHit->GetY() - mcY));

            if (trackDesk.nearestHitDistSts[stationNumber] < 0 || dist < trackDesk.nearestHitDistSts[stationNumber]) {
              trackDesk.nearestHitDistSts[stationNumber] = dist;
              trackDesk.nearestPointsSts[stationNumber]  = stsPoint;
              trackDesk.pullXsts[stationNumber]          = (stsHit->GetX() - mcX) / stsHit->GetDx();
              trackDesk.pullYsts[stationNumber]          = (stsHit->GetY() - mcY) / stsHit->GetDy();
            }
          }
        }
      }
    }  // STS hits
  }

  if (TrackDesc::nofMuchStations > 0) {
    Int_t nofMuchHits = fMuchHits->GetEntriesFast();

    for (Int_t i = 0; i < nofMuchHits; ++i) {
      const CbmMuchPixelHit* muchHit = static_cast<const CbmMuchPixelHit*>(fMuchHits->At(i));
      Int_t muchStationNumber        = CbmMuchGeoScheme::GetStationIndex(muchHit->GetAddress());
      Int_t layerNumber              = CbmMuchGeoScheme::GetLayerIndex(muchHit->GetAddress());
      Int_t stationNumber            = muchStationNumber * 3 + layerNumber;
      Int_t clusterId                = muchHit->GetRefId();
      const CbmMuchCluster* cluster  = static_cast<const CbmMuchCluster*>(fMuchClusters->At(clusterId));
      Int_t nofDigis                 = cluster->GetNofDigis();

      for (Int_t j = 0; j < nofDigis; ++j) {
        Int_t digiId = cluster->GetDigi(j);
        //const CbmMuchDigi* digi = static_cast<const CbmMuchDigi*> (fMuchDigis->At(digiId));
        const CbmMatch* muchDigiMatch = static_cast<const CbmMatch*>(fMuchDigiMatches->At(digiId));
        Int_t nofLinks                = muchDigiMatch->GetNofLinks();

        for (Int_t k = 0; k < nofLinks; ++k) {
          const CbmLink& link             = muchDigiMatch->GetLink(k);
          Int_t eventId                   = link.GetEntry();
          Int_t mcPointId                 = link.GetIndex();
          gMuchPoints[eventId][mcPointId] = true;
          const CbmMuchPoint* muchPoint   = static_cast<const CbmMuchPoint*>(fMuchPoints->Get(0, eventId, mcPointId));
          Double_t mcX                    = (muchPoint->GetXIn() + muchPoint->GetXOut()) / 2;
          Double_t mcY                    = (muchPoint->GetYIn() + muchPoint->GetYOut()) / 2;
          Double_t mcT                    = muchPoint->GetTime();
          Double_t xRes                   = muchHit->GetX() - mcX;
          Double_t yRes                   = muchHit->GetY() - mcY;
          Double_t tRes                   = muchHit->GetTime() - mcT;
          Double_t xPull                  = xRes / muchHit->GetDx();
          Double_t yPull                  = yRes / muchHit->GetDy();
          Double_t tPull                  = tRes / muchHit->GetTimeError();
          muchHitResidualX->Fill(xRes);
          muchHitResidualY->Fill(yRes);
          muchHitResidualT->Fill(tRes);
          muchHitPullX->Fill(xPull);
          muchHitPullY->Fill(yPull);
          muchHitPullT->Fill(tPull);
#ifdef CBM_BINNED_QA_FILL_HISTOS
          muchXResHisto->Fill(muchHit->GetX() - (muchPoint->GetXIn() + muchPoint->GetXOut()) / 2);
          muchYResHisto->Fill(muchHit->GetY() - (muchPoint->GetYIn() + muchPoint->GetYOut()) / 2);
          muchTResHisto->Fill(muchHit->GetTime() - muchPoint->GetTime());
#endif  //CBM_BINNED_QA_FILL_HISTOS
          Int_t trackId        = muchPoint->GetTrackID();
          TrackDesc& trackDesk = gTracks[eventId][trackId];
          trackDesk.much[stationNumber].first.insert(i);
        }
      }
    }  // MUCH hits*/
  }

  if (TrackDesc::nofTrdStations > 0) {
    Int_t nofTrdHits = fTrdHits->GetEntriesFast();

    for (Int_t i = 0; i < nofTrdHits; ++i) {
      const CbmTrdHit* trdHit = static_cast<const CbmTrdHit*>(fTrdHits->At(i));
      Int_t stationNumber     = trdHit->GetPlaneId();
      Int_t clusterId         = trdHit->GetRefId();
#ifndef TRD_IDEAL
      const CbmTrdCluster* cluster = static_cast<const CbmTrdCluster*>(fTrdClusters->At(clusterId));
      Int_t nofDigis               = cluster->GetNofDigis();

      for (Int_t j = 0; j < nofDigis; ++j) {
        Int_t digiId          = cluster->GetDigi(j);
        const CbmMatch* match = static_cast<const CbmMatch*>(fTrdDigiMatches->At(digiId));
        Int_t nofLinks        = match->GetNofLinks();

        for (Int_t k = 0; k < nofLinks; ++k) {
          const CbmLink& link            = match->GetLink(k);
          Int_t eventId                  = link.GetEntry();
          Int_t mcPointId                = link.GetIndex();
          gTrdPoints[eventId][mcPointId] = true;
          const CbmTrdPoint* trdPoint    = static_cast<const CbmTrdPoint*>(fTrdPoints->Get(0, eventId, mcPointId));
#ifdef CBM_BINNED_QA_FILL_HISTOS
          //Double_t mcX = (trdPoint->GetXIn() + trdPoint->GetXOut()) / 2; (VF) unused
          //Double_t mcY = (trdPoint->GetYIn() + trdPoint->GetYOut()) / 2; (VF) unused
          trdXResHisto->Fill(trdHit->GetX() - mcX);
          trdYResHisto->Fill(trdHit->GetY() - mcY);
          trdTResHisto->Fill(trdHit->GetTime() - trdPoint->GetTime());
#endif  //CBM_BINNED_QA_FILL_HISTOS
          Int_t trackId        = trdPoint->GetTrackID();
          TrackDesc& trackDesk = gTracks[eventId][trackId];
          trackDesk.trd[stationNumber].first.insert(i);

#ifdef CBM_BINNED_QA_FILL_HISTOS
          if (trackDesk.isPrimary && !trackDesk.trdPoints[0].empty() && !trackDesk.trdPoints[1].empty()
              && !trackDesk.trdPoints[2].empty() && !trackDesk.trdPoints[3].empty()) {
            Double_t dist = std::sqrt((trdHit->GetX() - mcX) * (trdHit->GetX() - mcX)
                                      + (trdHit->GetY() - mcY) * (trdHit->GetY() - mcY));

            if (trackDesk.nearestHitDistTrd[stationNumber] < 0 || dist < trackDesk.nearestHitDistTrd[stationNumber]) {
              trackDesk.nearestHitDistTrd[stationNumber] = dist;
              trackDesk.nearestPointsTrd[stationNumber]  = trdPoint;
              trackDesk.pullXtrd[stationNumber]          = (trdHit->GetX() - mcX) / trdHit->GetDx();
              trackDesk.pullYtrd[stationNumber]          = (trdHit->GetY() - mcY) / trdHit->GetDy();
            }
          }
#endif  //CBM_BINNED_QA_FILL_HISTOS
        }
      }
#else  //TRD_IDEAL
      const CbmTrdPoint* trdPoint = static_cast<const CbmTrdPoint*>(fTrdPoints->Get(0, gEventNumber, clusterId));
      Double_t mcX                = (trdPoint->GetXIn() + trdPoint->GetXOut()) / 2;
      Double_t mcY                = (trdPoint->GetYIn() + trdPoint->GetYOut()) / 2;
      trdXResHisto->Fill(trdHit->GetX() - mcX);
      trdYResHisto->Fill(trdHit->GetY() - mcY);
      trdTResHisto->Fill(trdHit->GetTime() - trdPoint->GetTime());
      Int_t trackId        = trdPoint->GetTrackID();
      TrackDesc& trackDesk = gTracks[gEventNumber][trackId];
      trackDesk.trd[stationNumber].first.insert(i);

      if (trackDesk.isPrimary && !trackDesk.trdPoints[0].empty() && !trackDesk.trdPoints[1].empty()
          && !trackDesk.trdPoints[2].empty() && !trackDesk.trdPoints[3].empty()) {
        Double_t dist =
          std::sqrt((trdHit->GetX() - mcX) * (trdHit->GetX() - mcX) + (trdHit->GetY() - mcY) * (trdHit->GetY() - mcY));

        if (trackDesk.nearestHitDist[stationNumber] < 0 || dist < trackDesk.nearestHitDist[stationNumber]) {
          trackDesk.nearestHitDist[stationNumber] = dist;
          trackDesk.nearestPoints[stationNumber]  = trdPoint;
          trackDesk.pullX[stationNumber]          = (trdHit->GetX() - mcX) / trdHit->GetDx();
          trackDesk.pullY[stationNumber]          = (trdHit->GetY() - mcY) / trdHit->GetDy();
        }
      }
#endif  //TRD_IDEAL
    }  // TRD hits
  }

  if (TrackDesc::hasTof) {
    Int_t nofTofHits = fTofHits->GetEntriesFast();

    for (Int_t i = 0; i < nofTofHits; ++i) {
      //const CbmTofHit* tofHit = static_cast<const CbmTofHit*> (fTofHits->At(i));
      const CbmMatch* tofHitMatch = static_cast<const CbmMatch*>(fTofHitDigiMatches->At(i));
      Int_t nofTofDigis           = tofHitMatch->GetNofLinks();

      for (Int_t j = 0; j < nofTofDigis; ++j) {
        const CbmLink& digiLink    = tofHitMatch->GetLink(j);
        Int_t digiInd              = digiLink.GetIndex();
        const CbmMatch* pointMatch = static_cast<const CbmMatch*>(fTofDigiPointMatches->At(digiInd));
        Int_t nofPoints            = pointMatch->GetNofLinks();

        for (Int_t k = 0; k < nofPoints; ++k) {
          const CbmLink& pointLink    = pointMatch->GetLink(k);
          Int_t eventId               = pointLink.GetEntry();
          Int_t mcPointId             = pointLink.GetIndex();
          const CbmTofPoint* tofPoint = static_cast<const CbmTofPoint*>(fTofPoints->Get(0, eventId, mcPointId));
#ifdef CBM_BINNED_QA_FILL_HISTOS
          tofXResHisto->Fill(tofHit->GetX() - tofPoint->GetX());
          tofYResHisto->Fill(tofHit->GetY() - tofPoint->GetY());
          tofTResHisto->Fill(tofHit->GetTime() - tofPoint->GetTime());
#endif  //CBM_BINNED_QA_FILL_HISTOS
          Int_t trackId        = tofPoint->GetTrackID();
          TrackDesc& trackDesk = gTracks[eventId][trackId];
          trackDesk.tof.first.insert(i);
          gTofPoints[eventId][trackId] = 2;
        }
      }
    }  // TOF hits
  }

  Int_t nofStations             = fSettings->GetNofStations();
  Int_t nofGlobalTracks         = fGlobalTracks->GetEntriesFast();
  set<Int_t>* globalTrackMCRefs = new set<Int_t>[nofGlobalTracks * nofStations];
  Int_t* globalTracksHitInds    = new Int_t[nofGlobalTracks * nofStations];
  gIsRecoClone.resize(nofGlobalTracks);
  fill(gIsRecoClone.begin(), gIsRecoClone.end(), false);

  for (Int_t i = 0; i < nofGlobalTracks; ++i) {
    ++gNofRecoTracks;
    const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(i));
    Int_t stsIndex                    = globalTrack->GetStsTrackIndex();
    Int_t muchIndex                   = globalTrack->GetMuchTrackIndex();
    Int_t trdIndex                    = globalTrack->GetTrdTrackIndex();
    Int_t tofIndex                    = globalTrack->GetTofHitIndex();

    if (TrackDesc::nofStsStations > 0 && stsIndex < 0) continue;

    if (TrackDesc::nofMuchStations > 0 && muchIndex < 0) continue;

    if (TrackDesc::nofTrdStations > 0 && trdIndex < 0) continue;

    if (TrackDesc::hasTof && tofIndex < 0) continue;

    map<Int_t, set<Int_t>> mcTrackIds;

    if (TrackDesc::nofStsStations > 0) HandleSts(stsIndex, mcTrackIds, globalTrackMCRefs, globalTracksHitInds);

    if (TrackDesc::nofMuchStations > 0) HandleMuch(muchIndex, mcTrackIds, globalTrackMCRefs, globalTracksHitInds);

    if (TrackDesc::nofTrdStations > 0) HandleTrd(trdIndex, mcTrackIds, globalTrackMCRefs, globalTracksHitInds);

    if (TrackDesc::hasTof) HandleTof(i, tofIndex, mcTrackIds, globalTrackMCRefs, globalTracksHitInds);

    map<Int_t, set<Int_t>>::const_iterator maxIter = max_element(
      mcTrackIds.begin(), mcTrackIds.end(), [](const pair<Int_t, set<Int_t>>& p1, const pair<Int_t, set<Int_t>>& p2) {
        return p1.second.size() < p2.second.size();
      });

    if (maxIter->second.size() < size_t(0.7 * nofStations)) continue;

    ++gNofNonGhosts;
  }

  map<Int_t, set<Int_t>>* mcToGlobalRefs = new map<Int_t, set<Int_t>>[nofStations];

  for (Int_t i = 0; i < nofGlobalTracks; ++i) {
    //map<Int_t, Int_t> matches;// The map from global track IDs, different from the current (i-th) global track ID, to the number of stations, where their hits originate from the MC tracks.

    for (Int_t j = 0; j < nofStations; ++j) {
      //set<Int_t> globals;// Global track IDs, which reference the same Monte Carlo track IDs on this station as the i-th (global) track.
      const set<Int_t>& mcs = globalTrackMCRefs[i * nofStations + j];

      for (set<Int_t>::const_iterator k = mcs.begin(); k != mcs.end(); ++k) {
        Int_t mc = *k;
        /*map<Int_t, set<Int_t> >::const_iterator mctogIter = mcToGlobalRefs[j].find(mc);
            
            if (mctogIter == mcToGlobalRefs[j].end())
               continue;
            
            globals.insert(mctogIter->second.begin(), mctogIter->second.end());*/
        mcToGlobalRefs[j][mc].insert(i);
      }

      /*for (set<Int_t>::const_iterator k = globals.begin(); k != globals.end(); ++k)
         {
            Int_t gl = *k;
            map<Int_t, Int_t>::iterator mIter = matches.find(gl);
            
            if (mIter == matches.end())
               matches[gl] = 1;
            else
               ++mIter->second;
         }*/
    }

    //map<Int_t, Int_t>::const_iterator maxIter = max_element(matches.begin(), matches.end(),
    // [](const pair<Int_t, Int_t>& p1, const pair<Int_t, Int_t>& p2)
    //{
    //return p1.second < p2.second;
    //});

    /*for (map<Int_t, Int_t>::const_iterator maxIter = matches.begin(); maxIter != matches.end(); ++maxIter)
      {
         if (maxIter->second >= int(0.7 * nofStations))
         {
            ++gNofClones;
            Int_t origTrackInd = maxIter->first;
            int nofSameHits = 0;
         
            for (Int_t j = 0; j < nofStations; ++j)
            {
               Int_t hitInd = globalTracksHitInds[i * nofStations + j];
               Int_t origHitInd = globalTracksHitInds[origTrackInd * nofStations + j];
            
               if (hitInd == origHitInd)
                  ++nofSameHits;
            }
         
            clonesNofSameHits->Fill(nofSameHits);
            continue;
         }
      }
         
      for (Int_t j = 0; j < nofStations; ++j)
      {
         const set<Int_t>& mcs = globalTrackMCRefs[i * nofStations + j];
         
         for (set<Int_t>::const_iterator k = mcs.begin(); k != mcs.end(); ++k)
         {
            Int_t mc = *k;
            mcToGlobalRefs[j][mc].insert(i);
         }
      }*/
  }  // Filling global tracks to mc maps for furhter clones searches

  for (Int_t i = 0; i < nofGlobalTracks; ++i) {
    const CbmGlobalTrack* thisTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(i));

    //if (gIsRecoClone[i])
    //continue;

    map<Int_t, Int_t>
      matches;  // The map from global track IDs, different from the current (i-th) global track ID, to the number of stations, where their hits originate from the MC tracks.

    for (Int_t j = 0; j < nofStations; ++j) {
      set<Int_t>
        globals;  // Global track IDs, which reference the same Monte Carlo track IDs on this station as the i-th (global) track.
      const set<Int_t>& mcs = globalTrackMCRefs[i * nofStations + j];

      for (set<Int_t>::const_iterator k = mcs.begin(); k != mcs.end(); ++k) {
        Int_t mc                                         = *k;
        map<Int_t, set<Int_t>>::const_iterator mctogIter = mcToGlobalRefs[j].find(mc);

        if (mctogIter == mcToGlobalRefs[j].end()) continue;

        globals.insert(mctogIter->second.begin(), mctogIter->second.end());
      }

      for (set<Int_t>::const_iterator k = globals.begin(); k != globals.end(); ++k) {
        Int_t gl = *k;

        if (gl == i) continue;

        map<Int_t, Int_t>::iterator mIter = matches.find(gl);

        if (mIter == matches.end()) matches[gl] = 1;
        else
          ++mIter->second;
      }
    }

    for (map<Int_t, Int_t>::const_iterator j = matches.begin(); j != matches.end(); ++j) {
      if (j->second >= int(0.7 * nofStations)) {
        const CbmGlobalTrack* anotherTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(j->first));

        //if (gIsRecoClone[j->first])
        //continue;

        if (thisTrack->GetChi2() < anotherTrack->GetChi2()) gIsRecoClone[j->first] = true;
        else
          gIsRecoClone[i] = true;
      }
    }
  }

  for (Int_t i = 0; i < nofGlobalTracks; ++i) {
    if (gIsRecoClone[i]) ++gNofClones;
  }

  delete[] mcToGlobalRefs;
  delete[] globalTrackMCRefs;
  delete[] globalTracksHitInds;

  ++gEventNumber;
}

static inline void IncrementForId(map<Int_t, set<Int_t>>& ids, Int_t id, Int_t stId) { ids[id].insert(stId); }

void CbmBinnedTrackerQA::HandleSts(Int_t stsTrackIndex, map<Int_t, set<Int_t>>& mcTrackIds,
                                   set<Int_t>* globalTrackMCRefs, Int_t* globalTracksHitInds)
{
  int nofStations             = fSettings->GetNofStations();
  const CbmStsTrack* stsTrack = static_cast<const CbmStsTrack*>(fStsTracks->At(stsTrackIndex));
  Int_t nofStsHits            = stsTrack->GetNofHits();

  for (Int_t i = 0; i < nofStsHits; ++i) {
    Int_t stsHitInd         = stsTrack->GetStsHitIndex(i);
    const CbmStsHit* stsHit = static_cast<const CbmStsHit*>(fStsHits->At(stsHitInd));
    Int_t stationNumber     = CbmStsSetup::Instance()->GetStationNumber(stsHit->GetAddress());
    globalTracksHitInds[stsTrackIndex * nofStations + TrackDesc::firstStsStationNo + stationNumber] = stsHitInd;
    Double_t mcX                                                                                    = 0;
    Double_t mcY                                                                                    = 0;
    Double_t mcCnt                                                                                  = 0;
    //Int_t stationNumber = CbmStsAddress::GetElementId(stsHit->GetAddress(), kSts);
    Int_t frontClusterInd             = stsHit->GetFrontClusterId();
    Int_t backClusterInd              = stsHit->GetBackClusterId();
    const CbmStsCluster* frontCluster = static_cast<const CbmStsCluster*>(fStsClusters->At(frontClusterInd));
    Int_t nofFrontDigis               = frontCluster->GetNofDigis();

    for (Int_t j = 0; j < nofFrontDigis; ++j) {
      Int_t stsDigiInd = frontCluster->GetDigi(j);
      //const CbmStsDigi* stsDigi = static_cast<const CbmStsDigi*> (fStsDigis->At(stsDigiInd));
      const CbmMatch* stsDigiMatch = static_cast<const CbmMatch*>(fStsDigiMatches->At(stsDigiInd));
      Int_t nofLinks               = stsDigiMatch->GetNofLinks();

      for (Int_t k = 0; k < nofLinks; ++k) {
        const CbmLink& link         = stsDigiMatch->GetLink(k);
        Int_t eventId               = link.GetEntry();
        Int_t mcPointId             = link.GetIndex();
        const CbmStsPoint* stsPoint = static_cast<const CbmStsPoint*>(fStsPoints->Get(0, eventId, mcPointId));
        mcX += (stsPoint->GetXIn() + stsPoint->GetXOut()) / 2;
        mcY += (stsPoint->GetYIn() + stsPoint->GetYOut()) / 2;
        ++mcCnt;
        Int_t trackId = stsPoint->GetTrackID();
        IncrementForId(mcTrackIds, trackId, stationNumber);
        globalTrackMCRefs[stsTrackIndex * nofStations + stationNumber].insert(trackId);
        TrackDesc& trackDesk = gTracks[eventId][trackId];

        if (trackDesk.sts[stationNumber].first.find(stsHitInd) != trackDesk.sts[stationNumber].first.end())
          trackDesk.sts[stationNumber].second.insert(stsTrackIndex);
      }
    }

    const CbmStsCluster* backCluster = static_cast<const CbmStsCluster*>(fStsClusters->At(backClusterInd));
    Int_t nofBackDigis               = backCluster->GetNofDigis();

    for (Int_t j = 0; j < nofBackDigis; ++j) {
      Int_t stsDigiInd = backCluster->GetDigi(j);
      //const CbmStsDigi* stsDigi = static_cast<const CbmStsDigi*> (fStsDigis->At(stsDigiInd));
      const CbmMatch* stsDigiMatch = static_cast<const CbmMatch*>(fStsDigiMatches->At(stsDigiInd));
      Int_t nofLinks               = stsDigiMatch->GetNofLinks();

      for (Int_t k = 0; k < nofLinks; ++k) {
        const CbmLink& link         = stsDigiMatch->GetLink(k);
        Int_t eventId               = link.GetEntry();
        Int_t mcPointId             = link.GetIndex();
        const CbmStsPoint* stsPoint = static_cast<const CbmStsPoint*>(fStsPoints->Get(0, eventId, mcPointId));
        mcX += (stsPoint->GetXIn() + stsPoint->GetXOut()) / 2;
        mcY += (stsPoint->GetYIn() + stsPoint->GetYOut()) / 2;
        ++mcCnt;
        Int_t trackId = stsPoint->GetTrackID();
        IncrementForId(mcTrackIds, trackId, stationNumber);
        globalTrackMCRefs[stsTrackIndex * nofStations + stationNumber].insert(trackId);
        TrackDesc& trackDesk = gTracks[eventId][trackId];

        if (trackDesk.sts[stationNumber].first.find(stsHitInd) != trackDesk.sts[stationNumber].first.end())
          trackDesk.sts[stationNumber].second.insert(stsTrackIndex);
      }
    }

    if (0 == mcCnt) continue;

    mcX /= mcCnt;
    mcY /= mcCnt;

    if (0 == stationNumber) {
      const FairTrackParam* param = stsTrack->GetParamFirst();
      Double_t xRes               = param->GetX() - mcX;
      Double_t yRes               = param->GetY() - mcY;
      Double_t xPull              = xRes / TMath::Sqrt(param->GetCovariance(0, 0));
      Double_t yPull              = yRes / TMath::Sqrt(param->GetCovariance(1, 1));
      stsTrackResidualFirstX->Fill(xRes);
      stsTrackResidualFirstY->Fill(yRes);
      stsTrackPullFirstX->Fill(xPull);
      stsTrackPullFirstY->Fill(yPull);
    }
    else if (TrackDesc::nofStsStations - 1 == stationNumber) {
      const FairTrackParam* param = stsTrack->GetParamLast();
      Double_t xRes               = param->GetX() - mcX;
      Double_t yRes               = param->GetY() - mcY;
      Double_t xPull              = xRes / TMath::Sqrt(param->GetCovariance(0, 0));
      Double_t yPull              = yRes / TMath::Sqrt(param->GetCovariance(1, 1));
      stsTrackResidualLastX->Fill(xRes);
      stsTrackResidualLastY->Fill(yRes);
      stsTrackPullLastX->Fill(xPull);
      stsTrackPullLastY->Fill(yPull);
    }
  }
}

void CbmBinnedTrackerQA::HandleMuch(Int_t muchTrackIndex, map<Int_t, set<Int_t>>& mcTrackIds,
                                    set<Int_t>* globalTrackMCRefs, Int_t* globalTracksHitInds)
{
  int nofStations               = fSettings->GetNofStations();
  const CbmMuchTrack* muchTrack = static_cast<const CbmMuchTrack*>(fMuchTracks->At(muchTrackIndex));
  Int_t nofMuchHits             = muchTrack->GetNofHits();

  for (Int_t i = 0; i < nofMuchHits; ++i) {
    Int_t muchHitInd               = muchTrack->GetHitIndex(i);
    const CbmMuchPixelHit* muchHit = static_cast<const CbmMuchPixelHit*>(fMuchHits->At(muchHitInd));
    Int_t muchStationNumber        = CbmMuchGeoScheme::GetStationIndex(muchHit->GetAddress());
    Int_t layerNumber              = CbmMuchGeoScheme::GetLayerIndex(muchHit->GetAddress());
    Int_t stationNumber            = muchStationNumber * 3 + layerNumber;
    Double_t mcX                   = 0;
    Double_t mcY                   = 0;
    //Double_t mcCnt = 0; (VF) unused
    globalTracksHitInds[muchTrackIndex * nofStations + TrackDesc::firstMuchStationNo + stationNumber] = muchHitInd;
    Int_t clusterId               = muchHit->GetRefId();
    const CbmMuchCluster* cluster = static_cast<const CbmMuchCluster*>(fMuchClusters->At(clusterId));
    Int_t nofDigis                = cluster->GetNofDigis();

    for (Int_t j = 0; j < nofDigis; ++j) {
      Int_t digiId = cluster->GetDigi(j);
      //const CbmMuchDigi* digi = static_cast<const CbmMuchDigi*> (fMuchDigis->At(digiId));
      const CbmMatch* digiMatch = static_cast<const CbmMatch*>(fMuchDigiMatches->At(digiId));
      Int_t nofLinks            = digiMatch->GetNofLinks();

      for (Int_t k = 0; k < nofLinks; ++k) {
        const CbmLink& link           = digiMatch->GetLink(k);
        Int_t eventId                 = link.GetEntry();
        Int_t mcPointId               = link.GetIndex();
        const CbmMuchPoint* muchPoint = static_cast<const CbmMuchPoint*>(fMuchPoints->Get(0, eventId, mcPointId));
        //mcX += (muchPoint->GetXIn() + muchPoint->GetXOut()) / 2;
        //mcY += (muchPoint->GetYIn() + muchPoint->GetYOut()) / 2;
        //++mcCnt;
        mcX = (muchPoint->GetXIn() + muchPoint->GetXOut()) / 2;
        mcY = (muchPoint->GetYIn() + muchPoint->GetYOut()) / 2;

        if (0 == stationNumber) {
          const FairTrackParam* param = muchTrack->GetParamFirst();
          Double_t xRes               = param->GetX() - mcX;
          Double_t yRes               = param->GetY() - mcY;
          Double_t xPull              = xRes / TMath::Sqrt(param->GetCovariance(0, 0));
          Double_t yPull              = yRes / TMath::Sqrt(param->GetCovariance(1, 1));
          muchTrackResidualFirstX->Fill(xRes);
          muchTrackResidualFirstY->Fill(yRes);
          muchTrackPullFirstX->Fill(xPull);
          muchTrackPullFirstY->Fill(yPull);
        }
        else if (TrackDesc::nofMuchStations - 1 == stationNumber) {
          const FairTrackParam* param = muchTrack->GetParamLast();
          Double_t xRes               = param->GetX() - mcX;
          Double_t yRes               = param->GetY() - mcY;
          Double_t xPull              = xRes / TMath::Sqrt(param->GetCovariance(0, 0));
          Double_t yPull              = yRes / TMath::Sqrt(param->GetCovariance(1, 1));
          muchTrackResidualLastX->Fill(xRes);
          muchTrackResidualLastY->Fill(yRes);
          muchTrackPullLastX->Fill(xPull);
          muchTrackPullLastY->Fill(yPull);
        }

        Int_t trackId = muchPoint->GetTrackID();
        IncrementForId(mcTrackIds, trackId, 100 + stationNumber);
        globalTrackMCRefs[muchTrackIndex * nofStations + TrackDesc::firstMuchStationNo + stationNumber].insert(trackId);
        TrackDesc& trackDesk = gTracks[eventId][trackId];

        if (trackDesk.much[stationNumber].first.find(muchHitInd) != trackDesk.much[stationNumber].first.end())
          trackDesk.much[stationNumber].second.insert(muchTrackIndex);
      }
    }

    /*if (0 == mcCnt)
         continue;
      
      mcX /= mcCnt;
      mcY /= mcCnt;
      
      if (0 == stationNumber)
      {
         const FairTrackParam* param = muchTrack->GetParamFirst();
         Double_t xRes = param->GetX() - mcX;
         Double_t yRes = param->GetY() - mcY;
         Double_t xPull = xRes / TMath::Sqrt(param->GetCovariance(0, 0));
         Double_t yPull = yRes / TMath::Sqrt(param->GetCovariance(1, 1));
         muchTrackResidualFirstX->Fill(xRes);
         muchTrackResidualFirstY->Fill(yRes);
         muchTrackPullFirstX->Fill(xPull);
         muchTrackPullFirstY->Fill(yPull);
      }
      else if (TrackDesc::nofMuchStations - 1 == stationNumber)
      {
         const FairTrackParam* param = muchTrack->GetParamLast();
         Double_t xRes = param->GetX() - mcX;
         Double_t yRes = param->GetY() - mcY;
         Double_t xPull = xRes / TMath::Sqrt(param->GetCovariance(0, 0));
         Double_t yPull = yRes / TMath::Sqrt(param->GetCovariance(1, 1));
         muchTrackResidualLastX->Fill(xRes);
         muchTrackResidualLastY->Fill(yRes);
         muchTrackPullLastX->Fill(xPull);
         muchTrackPullLastY->Fill(yPull);
      }*/
  }
}

void CbmBinnedTrackerQA::HandleTrd(Int_t trdTrackIndex, map<Int_t, set<Int_t>>& mcTrackIds,
                                   set<Int_t>* globalTrackMCRefs, Int_t* globalTracksHitInds)
{
  int nofStations             = fSettings->GetNofStations();
  const CbmTrdTrack* trdTrack = static_cast<const CbmTrdTrack*>(fTrdTracks->At(trdTrackIndex));
  Int_t nofTrdHits            = trdTrack->GetNofHits();

  for (Int_t i = 0; i < nofTrdHits; ++i) {
    Int_t trdHitInd         = trdTrack->GetHitIndex(i);
    const CbmTrdHit* trdHit = static_cast<const CbmTrdHit*>(fTrdHits->At(trdHitInd));
    Int_t stationNumber     = trdHit->GetPlaneId();
    Double_t mcX            = 0;
    Double_t mcY            = 0;
    Double_t mcCnt          = 0;
    globalTracksHitInds[trdTrackIndex * nofStations + TrackDesc::firstTrdStationNo + stationNumber] = trdHitInd;
    Int_t clusterId = trdHit->GetRefId();
#ifndef TRD_IDEAL
    const CbmTrdCluster* cluster = static_cast<const CbmTrdCluster*>(fTrdClusters->At(clusterId));
    Int_t nofDigis               = cluster->GetNofDigis();

    for (Int_t j = 0; j < nofDigis; ++j) {
      Int_t digiId          = cluster->GetDigi(j);
      const CbmMatch* match = static_cast<const CbmMatch*>(fTrdDigiMatches->At(digiId));
      Int_t nofLinks        = match->GetNofLinks();

      for (Int_t k = 0; k < nofLinks; ++k) {
        const CbmLink& link         = match->GetLink(k);
        Int_t eventId               = link.GetEntry();
        Int_t mcPointId             = link.GetIndex();
        const CbmTrdPoint* trdPoint = static_cast<const CbmTrdPoint*>(fTrdPoints->Get(0, eventId, mcPointId));
        mcX += (trdPoint->GetXIn() + trdPoint->GetXOut()) / 2;
        mcY += (trdPoint->GetYIn() + trdPoint->GetYOut()) / 2;
        ++mcCnt;
        Int_t trackId = trdPoint->GetTrackID();
        IncrementForId(mcTrackIds, trackId, 200 + stationNumber);
        globalTrackMCRefs[trdTrackIndex * nofStations + TrackDesc::firstTrdStationNo + stationNumber].insert(trackId);
        TrackDesc& trackDesk = gTracks[eventId][trackId];

        if (trackDesk.trd[stationNumber].first.find(trdHitInd) != trackDesk.trd[stationNumber].first.end())
          trackDesk.trd[stationNumber].second.insert(trdTrackIndex);
      }
    }
#else  //TRD_IDEAL
    const CbmTrdPoint* trdPoint = static_cast<const CbmTrdPoint*>(fTrdPoints->Get(0, gEventNumber, clusterId));
    Int_t trackId               = trdPoint->GetTrackID();
    TrackDesc& trackDesk        = gTracks[gEventNumber][trackId];

    if (trackDesk.trd[stationNumber].first.find(trdHitInd) != trackDesk.trd[stationNumber].first.end())
      trackDesk.trd[stationNumber].second.insert(trdTrackIndex);
#endif  //TRD_IDEAL
    if (0 == mcCnt) continue;

    mcX /= mcCnt;
    mcY /= mcCnt;

    if (0 == stationNumber) {
      const FairTrackParam* param = trdTrack->GetParamFirst();
      Double_t xRes               = param->GetX() - mcX;
      Double_t yRes               = param->GetY() - mcY;
      Double_t xPull              = xRes / TMath::Sqrt(param->GetCovariance(0, 0));
      Double_t yPull              = yRes / TMath::Sqrt(param->GetCovariance(1, 1));
      trdTrackResidualFirstX->Fill(xRes);
      trdTrackResidualFirstY->Fill(yRes);
      trdTrackPullFirstX->Fill(xPull);
      trdTrackPullFirstY->Fill(yPull);
    }
    else if (TrackDesc::nofTrdStations - 1 == stationNumber) {
      const FairTrackParam* param = trdTrack->GetParamLast();
      Double_t xRes               = param->GetX() - mcX;
      Double_t yRes               = param->GetY() - mcY;
      Double_t xPull              = xRes / TMath::Sqrt(param->GetCovariance(0, 0));
      Double_t yPull              = yRes / TMath::Sqrt(param->GetCovariance(1, 1));
      trdTrackResidualLastX->Fill(xRes);
      trdTrackResidualLastY->Fill(yRes);
      trdTrackPullLastX->Fill(xPull);
      trdTrackPullLastY->Fill(yPull);
    }
  }
}

void CbmBinnedTrackerQA::HandleTof(Int_t globalTrackIndex, Int_t tofHitIndex, map<Int_t, set<Int_t>>& mcTrackIds,
                                   set<Int_t>* globalTrackMCRefs, Int_t* globalTracksHitInds)
{
  const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(globalTrackIndex));
  Double_t mcX                      = 0;
  Double_t mcY                      = 0;
  Double_t mcCnt                    = 0;
  int nofStations                   = fSettings->GetNofStations();
  globalTracksHitInds[globalTrackIndex * nofStations + TrackDesc::tofStationNo] = tofHitIndex;
  const CbmMatch* tofHitMatch = static_cast<const CbmMatch*>(fTofHitDigiMatches->At(tofHitIndex));
  Int_t nofTofDigis           = tofHitMatch->GetNofLinks();

  for (Int_t i = 0; i < nofTofDigis; ++i) {
    const CbmLink& digiLink    = tofHitMatch->GetLink(i);
    Int_t digiInd              = digiLink.GetIndex();
    const CbmMatch* pointMatch = static_cast<const CbmMatch*>(fTofDigiPointMatches->At(digiInd));
    Int_t nofPoints            = pointMatch->GetNofLinks();

    for (Int_t j = 0; j < nofPoints; ++j) {
      const CbmLink& pointLink    = pointMatch->GetLink(j);
      Int_t eventId               = pointLink.GetEntry();
      Int_t pointId               = pointLink.GetIndex();
      const CbmTofPoint* tofPoint = static_cast<const CbmTofPoint*>(fTofPoints->Get(0, eventId, pointId));
      mcX += tofPoint->GetX();
      mcY += tofPoint->GetY();
      ++mcCnt;
      Int_t trackId = tofPoint->GetTrackID();
      IncrementForId(mcTrackIds, trackId, 300);
      globalTrackMCRefs[globalTrackIndex * nofStations + TrackDesc::tofStationNo].insert(trackId);
      TrackDesc& trackDesk = gTracks[eventId][trackId];

      if (trackDesk.tof.first.find(tofHitIndex) != trackDesk.tof.first.end()) trackDesk.tof.second.insert(tofHitIndex);
    }
  }

  if (0 == mcCnt) return;

  mcX /= mcCnt;
  mcY /= mcCnt;
  const FairTrackParam* param = globalTrack->GetParamLast();
  Double_t xRes               = param->GetX() - mcX;
  Double_t yRes               = param->GetY() - mcY;
  Double_t xPull              = xRes / TMath::Sqrt(param->GetCovariance(0, 0));
  Double_t yPull              = yRes / TMath::Sqrt(param->GetCovariance(1, 1));
  globalTrackResidualLastX->Fill(xRes);
  globalTrackResidualLastY->Fill(yRes);
  globalTrackPullLastX->Fill(xPull);
  globalTrackPullLastY->Fill(yPull);
}

static void SaveHisto(TH1* histo)
{
  TFile* curFile    = TFile::CurrentFile();
  TString histoName = histo->GetName();
  histoName += ".root";
  TFile fh(histoName.Data(), "RECREATE");
  histo->Write();
  fh.Close();
  delete histo;
  TFile::CurrentFile() = curFile;
}

template<typename T>
void NumberToFile(const char* name, T number)
{
  char buf[256];
  sprintf(buf, "%s.txt", name);
  ofstream fileStream(buf, ios_base::out | ios_base::trunc);
  fileStream << number;
}

static void effOfMCPoints(const char* name, const vector<vector<bool>>& points)
{
  int nofAllMCPoints  = 0;
  int nofRecoMCPoints = 0;

  for (const auto& i : points) {
    for (auto j : i) {
      ++nofAllMCPoints;

      if (j) ++nofRecoMCPoints;
    }
  }

  double eff = 100 * nofRecoMCPoints;

  if (0 == nofAllMCPoints) eff = 100;
  else
    eff /= nofAllMCPoints;

  cout << "Reconstructed of the " << name << " points: " << eff << "% " << nofRecoMCPoints << "/" << nofAllMCPoints
       << endl;
}

static void effOfMCPoints(const char* name, const vector<vector<char>>& points)
{
  int nofAllMCPoints  = 0;
  int nofRecoMCPoints = 0;

  for (const auto& i : points) {
    for (auto j : i) {
      if (j > 0) ++nofAllMCPoints;

      if (j > 1) ++nofRecoMCPoints;
    }
  }

  double eff = 100 * nofRecoMCPoints;

  if (0 == nofAllMCPoints) eff = 100;
  else
    eff /= nofAllMCPoints;

  cout << "Reconstructed of the " << name << " points: " << eff << "% " << nofRecoMCPoints << "/" << nofAllMCPoints
       << endl;
}

void CbmBinnedTrackerQA::Finish()
{
  int nofAllTracks               = 0;
  int nofRefTracks               = 0;
  int nofMatchedRefTracks        = 0;
  int nofRefPrimTracks           = 0;
  int nofMatchedRefPrimTracks    = 0;
  int nofRefNonPrimTracks        = 0;
  int nofMatchedRefNonPrimTracks = 0;
  //int nofSts[2] = { 0, 0 };
  //int nofTrd[4] = { 0, 0, 0, 0 };
  //int nofMuch[3] = { 0, 0, 0 };
  //int nofTof = 0;

  for (vector<vector<TrackDesc>>::iterator i = gTracks.begin(); i != gTracks.end(); ++i) {
    vector<TrackDesc>& evTracks = *i;

    for (vector<TrackDesc>::iterator j = evTracks.begin(); j != evTracks.end(); ++j) {
      ++nofAllTracks;
      TrackDesc& trackDesc = *j;

      if (fIsOnlyPrimary && !trackDesc.isPrimary) continue;

      bool isRef = true;

      for (int k = 0; k < TrackDesc::nofStsStations; ++k) {
        if (trackDesc.sts[k].first.empty()) {
          isRef = false;
          break;
        }
      }

      if (!isRef) continue;

#ifdef CBM_BINNED_QA_FILL_HISTOS
      for (set<const CbmStsPoint*>::const_iterator k = trackDesc.stsPoints[0].begin();
           k != trackDesc.stsPoints[0].end(); ++k) {
        const CbmStsPoint* p0 = *k;
        Double_t x0           = (p0->GetXIn() + p0->GetXOut()) / 2;
        Double_t y0           = (p0->GetYIn() + p0->GetYOut()) / 2;
        Double_t z0           = (p0->GetZIn() + p0->GetZOut()) / 2;
        Double_t tx           = (x0 - trackDesc.ptr->GetStartX()) / (z0 - trackDesc.ptr->GetStartZ());
        Double_t ty           = (y0 - trackDesc.ptr->GetStartY()) / (z0 - trackDesc.ptr->GetStartZ());

        for (set<const CbmStsPoint*>::const_iterator l = trackDesc.stsPoints[1].begin();
             l != trackDesc.stsPoints[1].end(); ++l) {
          const CbmStsPoint* p1 = *l;
          Double_t x1           = (p1->GetXIn() + p1->GetXOut()) / 2;
          Double_t y1           = (p1->GetYIn() + p1->GetYOut()) / 2;
          Double_t z1           = (p1->GetZIn() + p1->GetZOut()) / 2;

          extrStsXHisto->Fill(x1 - tx * z1);
          extrStsYHisto->Fill(y1 - ty * z1);
        }
      }
#endif  //CBM_BINNED_QA_FILL_HISTOS

      for (int k = 0; k < TrackDesc::nofMuchStations; ++k) {
        if (trackDesc.much[k].first.empty()) {
          isRef = false;
          break;
        }
      }

      if (!isRef) continue;

      for (int k = 0; k < TrackDesc::nofTrdStations; ++k) {
        if (trackDesc.trd[k].first.empty()) {
          isRef = false;
          break;
        }
      }

      if (!isRef) continue;

#ifdef CBM_BINNED_QA_FILL_HISTOS
      for (set<const CbmStsPoint*>::const_iterator k = trackDesc.stsPoints[0].begin();
           k != trackDesc.stsPoints[0].end(); ++k) {
        const CbmStsPoint* p1 = *k;
        Double_t x1           = (p1->GetXIn() + p1->GetXOut()) / 2;
        Double_t y1           = (p1->GetYIn() + p1->GetYOut()) / 2;
        Double_t z1           = (p1->GetZIn() + p1->GetZOut()) / 2;

        for (set<const CbmStsPoint*>::const_iterator l = trackDesc.stsPoints[1].begin();
             l != trackDesc.stsPoints[1].end(); ++l) {
          const CbmStsPoint* p2 = *l;
          Double_t x2           = (p2->GetXIn() + p2->GetXOut()) / 2;
          Double_t y2           = (p2->GetYIn() + p2->GetYOut()) / 2;
          Double_t z2           = (p2->GetZIn() + p2->GetZOut()) / 2;
          Double_t tx           = (x2 - x1) / (z2 - z1);
          Double_t ty           = (y2 - y1) / (z2 - z1);

          for (set<const CbmTrdPoint*>::const_iterator m = trackDesc.trdPoints[0].begin();
               m != trackDesc.trdPoints[0].end(); ++m) {
            const CbmTrdPoint* p = *m;
            Double_t x           = (p->GetXIn() + p->GetXOut()) / 2;
            Double_t y           = (p->GetYIn() + p->GetYOut()) / 2;
            Double_t z           = (p->GetZIn() + p->GetZOut()) / 2;
            Double_t deltaZ      = z - z2;
            extrTrdXHistos[0]->Fill(x - x2 - tx * deltaZ);
            extrTrdYHistos[0]->Fill(y - y2 - ty * deltaZ);
          }
        }
      }

      for (set<const CbmStsPoint*>::const_iterator k = trackDesc.stsPoints[1].begin();
           k != trackDesc.stsPoints[1].end(); ++k) {
        const CbmStsPoint* p1 = *k;
        Double_t x1           = (p1->GetXIn() + p1->GetXOut()) / 2;
        Double_t y1           = (p1->GetYIn() + p1->GetYOut()) / 2;
        Double_t z1           = (p1->GetZIn() + p1->GetZOut()) / 2;

        for (set<const CbmTrdPoint*>::const_iterator l = trackDesc.trdPoints[0].begin();
             l != trackDesc.trdPoints[0].end(); ++l) {
          const CbmTrdPoint* p2 = *l;
          Double_t x2           = (p2->GetXIn() + p2->GetXOut()) / 2;
          Double_t y2           = (p2->GetYIn() + p2->GetYOut()) / 2;
          Double_t z2           = (p2->GetZIn() + p2->GetZOut()) / 2;
          Double_t tx           = (x2 - x1) / (z2 - z1);
          Double_t ty           = (y2 - y1) / (z2 - z1);

          for (set<const CbmTrdPoint*>::const_iterator m = trackDesc.trdPoints[1].begin();
               m != trackDesc.trdPoints[1].end(); ++m) {
            const CbmTrdPoint* p = *m;
            Double_t x           = (p->GetXIn() + p->GetXOut()) / 2;
            Double_t y           = (p->GetYIn() + p->GetYOut()) / 2;
            Double_t z           = (p->GetZIn() + p->GetZOut()) / 2;
            Double_t deltaZ      = z - z2;
            extrTrdXHistos[1]->Fill(x - x2 - tx * deltaZ);
            extrTrdYHistos[1]->Fill(y - y2 - ty * deltaZ);
          }
        }
      }

      for (set<const CbmTrdPoint*>::const_iterator k = trackDesc.trdPoints[0].begin();
           k != trackDesc.trdPoints[0].end(); ++k) {
        const CbmTrdPoint* p1 = *k;
        Double_t x1           = (p1->GetXIn() + p1->GetXOut()) / 2;
        Double_t y1           = (p1->GetYIn() + p1->GetYOut()) / 2;
        Double_t z1           = (p1->GetZIn() + p1->GetZOut()) / 2;

        for (set<const CbmTrdPoint*>::const_iterator l = trackDesc.trdPoints[1].begin();
             l != trackDesc.trdPoints[1].end(); ++l) {
          const CbmTrdPoint* p2 = *l;
          Double_t x2           = (p2->GetXIn() + p2->GetXOut()) / 2;
          Double_t y2           = (p2->GetYIn() + p2->GetYOut()) / 2;
          Double_t z2           = (p2->GetZIn() + p2->GetZOut()) / 2;
          Double_t tx           = (x2 - x1) / (z2 - z1);
          Double_t ty           = (y2 - y1) / (z2 - z1);

          for (set<const CbmTrdPoint*>::const_iterator m = trackDesc.trdPoints[2].begin();
               m != trackDesc.trdPoints[2].end(); ++m) {
            const CbmTrdPoint* p = *m;
            Double_t x           = (p->GetXIn() + p->GetXOut()) / 2;
            Double_t y           = (p->GetYIn() + p->GetYOut()) / 2;
            Double_t z           = (p->GetZIn() + p->GetZOut()) / 2;
            Double_t deltaZ      = z - z2;
            extrTrdXHistos[2]->Fill(x - x2 - tx * deltaZ);
            extrTrdYHistos[2]->Fill(y - y2 - ty * deltaZ);
          }
        }
      }

      for (set<const CbmTrdPoint*>::const_iterator k = trackDesc.trdPoints[1].begin();
           k != trackDesc.trdPoints[1].end(); ++k) {
        const CbmTrdPoint* p1 = *k;
        Double_t x1           = (p1->GetXIn() + p1->GetXOut()) / 2;
        Double_t y1           = (p1->GetYIn() + p1->GetYOut()) / 2;
        Double_t z1           = (p1->GetZIn() + p1->GetZOut()) / 2;

        for (set<const CbmTrdPoint*>::const_iterator l = trackDesc.trdPoints[2].begin();
             l != trackDesc.trdPoints[2].end(); ++l) {
          const CbmTrdPoint* p2 = *l;
          Double_t x2           = (p2->GetXIn() + p2->GetXOut()) / 2;
          Double_t y2           = (p2->GetYIn() + p2->GetYOut()) / 2;
          Double_t z2           = (p2->GetZIn() + p2->GetZOut()) / 2;
          Double_t tx           = (x2 - x1) / (z2 - z1);
          Double_t ty           = (y2 - y1) / (z2 - z1);

          for (set<const CbmTrdPoint*>::const_iterator m = trackDesc.trdPoints[3].begin();
               m != trackDesc.trdPoints[3].end(); ++m) {
            const CbmTrdPoint* p = *m;
            Double_t x           = (p->GetXIn() + p->GetXOut()) / 2;
            Double_t y           = (p->GetYIn() + p->GetYOut()) / 2;
            Double_t z           = (p->GetZIn() + p->GetZOut()) / 2;
            Double_t deltaZ      = z - z2;
            extrTrdXHistos[3]->Fill(x - x2 - tx * deltaZ);
            extrTrdYHistos[3]->Fill(y - y2 - ty * deltaZ);
          }
        }
      }
#endif  //CBM_BINNED_QA_FILL_HISTOS

      /*for (int k = 0; k < NOF_MUCH_STATIONS * NOF_MUCH_LAYERS; ++k)
         {
            if (trackDesc.much[k].first.empty())
            {
               isRef = false;
               break;
            }
         }
         
         if (!isRef)
            continue;*/

      if (TrackDesc::hasTof && trackDesc.tof.first.empty()) continue;

      trackDesc.isReference = true;
      ++nofRefTracks;

      if (trackDesc.isPrimary) ++nofRefPrimTracks;
      else
        ++nofRefNonPrimTracks;

      map<Int_t, Int_t> matchedReco;

      for (int k = 0; k < TrackDesc::nofStsStations; ++k) {
        for (set<Int_t>::const_iterator l = trackDesc.sts[k].second.begin(); l != trackDesc.sts[k].second.end(); ++l) {
          map<Int_t, Int_t>::iterator mrIter = matchedReco.find(*l);

          if (mrIter != matchedReco.end()) ++mrIter->second;
          else
            matchedReco[*l] = 1;
        }
      }

      for (int k = 0; k < TrackDesc::nofMuchStations; ++k) {
        for (set<Int_t>::const_iterator l = trackDesc.much[k].second.begin(); l != trackDesc.much[k].second.end();
             ++l) {
          map<Int_t, Int_t>::iterator mrIter = matchedReco.find(*l);

          if (mrIter != matchedReco.end()) ++mrIter->second;
          else
            matchedReco[*l] = 1;
        }
      }

      for (int k = 0; k < TrackDesc::nofTrdStations; ++k) {
        for (set<Int_t>::const_iterator l = trackDesc.trd[k].second.begin(); l != trackDesc.trd[k].second.end(); ++l) {
          map<Int_t, Int_t>::iterator mrIter = matchedReco.find(*l);

          if (mrIter != matchedReco.end()) ++mrIter->second;
          else
            matchedReco[*l] = 1;
        }
      }

#ifdef CBM_BINNED_QA_FILL_HISTOS
      for (int k = 0; k < NOF_STS_STATIONS; ++k) {
        if (trackDesc.nearestHitDistSts[k] >= 0) {
          stsXPullHistos[k]->Fill(trackDesc.pullXsts[k]);
          stsYPullHistos[k]->Fill(trackDesc.pullYsts[k]);
        }
      }

      for (int k = 0; k < NOF_TRD_LAYERS; ++k) {
        if (trackDesc.nearestHitDistTrd[k] >= 0) {
          trdNearestHitDistHistos[k]->Fill(trackDesc.nearestHitDistTrd[k]);
          trdXPullHistos[k]->Fill(trackDesc.pullXtrd[k]);
          trdYPullHistos[k]->Fill(trackDesc.pullYtrd[k]);
        }
      }
#endif  //CBM_BINNED_QA_FILL_HISTOS

      /*for (int k = 0; k < NOF_MUCH_STATIONS * NOF_MUCH_LAYERS; ++k)
         {
            for (set<Int_t>::const_iterator l = trackDesc.much[k].second.begin(); l != trackDesc.much[k].second.end(); ++l)
            {
               map<Int_t, Int_t>::iterator mrIter = matchedReco.find(*l);
               
               if (mrIter != matchedReco.end())
                  ++mrIter->second;
               else
                  matchedReco[*l] = 1;
            }
            
            if (!trackDesc.much[k].second.empty())
               ++nofMuch[k];
         }*/

      /*for (set<Int_t>::const_iterator k = trackDesc.tof.second.begin(); k != trackDesc.tof.second.end(); ++k)
         {
            map<Int_t, Int_t>::iterator mrIter = matchedReco.find(*k);
               
            if (mrIter != matchedReco.end())
               ++mrIter->second;
            else
               matchedReco[*k] = 1;
         }
         
         if (!trackDesc.tof.second.empty())
            ++nofTof;*/

      TVector3 mom;
      trackDesc.ptr->GetMomentum(mom);
      Double_t momV     = trackDesc.ptr->GetP();
      Double_t momX     = mom.X();
      Double_t momY     = mom.Y();
      Double_t momZ     = mom.Z();
      Double_t polarAng = 180 * TMath::ACos(momZ / momV) / TMath::Pi();
      Double_t xAng     = 180 * TMath::ASin(momX / momV) / TMath::Pi();
      Double_t yAng     = 180 * TMath::ASin(momY / momV) / TMath::Pi();

      if (matchedReco.empty()) {
        if (trackDesc.isPrimary) {
          effByMomPrimary->Fill(momV, 0.);
          effByPolarAnglePrimary->Fill(polarAng, 0.);
          effByXAnglePrimary->Fill(xAng, 0.);
          effByYAnglePrimary->Fill(yAng, 0.);
        }
        else {
          effByMomNonPrimary->Fill(momV, 0.);
          effByPolarAngleNonPrimary->Fill(polarAng, 0.);
          effByXAngleNonPrimary->Fill(xAng, 0.);
          effByYAngleNonPrimary->Fill(yAng, 0.);
        }

        effByMom->Fill(momV, 0.);
        effByPolarAngle->Fill(polarAng, 0.);
        effByXAngle->Fill(xAng, 0.);
        effByYAngle->Fill(yAng, 0.);
        continue;
      }

      map<Int_t, Int_t>::const_iterator maxIter =
        max_element(matchedReco.begin(), matchedReco.end(),
                    [](const pair<Int_t, Int_t>& p1, const pair<Int_t, Int_t>& p2) { return p1.second < p2.second; });

      if (maxIter->second < int(0.7 * fSettings->GetNofStations())) {
        if (trackDesc.isPrimary) {
          effByMomPrimary->Fill(momV, 0.);
          effByPolarAnglePrimary->Fill(polarAng, 0.);
          effByXAnglePrimary->Fill(xAng, 0.);
          effByYAnglePrimary->Fill(yAng, 0.);
        }
        else {
          effByMomNonPrimary->Fill(momV, 0.);
          effByPolarAngleNonPrimary->Fill(polarAng, 0.);
          effByXAngleNonPrimary->Fill(xAng, 0.);
          effByYAngleNonPrimary->Fill(yAng, 0.);
        }

        effByMom->Fill(momV, 0.);
        effByPolarAngle->Fill(polarAng, 0.);
        effByXAngle->Fill(xAng, 0.);
        effByYAngle->Fill(yAng, 0.);
        continue;
      }

      effByMom->Fill(momV, 100.);
      effByPolarAngle->Fill(polarAng, 100.);
      effByXAngle->Fill(xAng, 100.);
      effByYAngle->Fill(yAng, 100.);
      trackDesc.isReconstructed = true;
      ++nofMatchedRefTracks;

      if (trackDesc.isPrimary) {
        effByMomPrimary->Fill(momV, 100.);
        effByPolarAnglePrimary->Fill(polarAng, 100.);
        effByXAnglePrimary->Fill(xAng, 100.);
        effByYAnglePrimary->Fill(yAng, 100.);
        ++nofMatchedRefPrimTracks;
      }
      else {
        effByMomNonPrimary->Fill(momV, 100.);
        effByPolarAngleNonPrimary->Fill(polarAng, 100.);
        effByXAngleNonPrimary->Fill(xAng, 100.);
        effByYAngleNonPrimary->Fill(yAng, 100.);
        ++nofMatchedRefNonPrimTracks;
      }
    }
  }

  cout << "Total tracks: " << nofAllTracks << endl;

  double eff = 100 * nofMatchedRefTracks;

  if (0 == nofRefTracks) eff = 100;
  else
    eff /= nofRefTracks;

  cout << "The track reconstruction efficiency: " << eff << "%: " << nofMatchedRefTracks << "/" << nofRefTracks << endl;
  NumberToFile("nofRefTracks", nofRefTracks);
  NumberToFile("nofMatchedRefTracks", nofMatchedRefTracks);

  eff = 100 * nofMatchedRefPrimTracks;

  if (0 == nofRefPrimTracks) eff = 100;
  else
    eff /= nofRefPrimTracks;

  cout << "The track reconstruction efficiency for primary tracks: " << eff << "%: " << nofMatchedRefPrimTracks << "/"
       << nofRefPrimTracks << endl;
  NumberToFile("nofMatchedRefPrimTracks", nofMatchedRefPrimTracks);

  eff = 100 * nofMatchedRefNonPrimTracks;

  if (0 == nofRefNonPrimTracks) eff = 100;
  else
    eff /= nofRefNonPrimTracks;

  cout << "The track reconstruction efficiency for non primary tracks: " << eff << "%: " << nofMatchedRefNonPrimTracks
       << "/" << nofRefNonPrimTracks << endl;
  NumberToFile("nofMatchedRefNonPrimTracks", nofMatchedRefNonPrimTracks);

  eff = 100 * (gNofRecoTracks - gNofNonGhosts);

  if (0 == gNofRecoTracks) eff = 100;
  else
    eff /= gNofRecoTracks;

  cout << "The number of ghosts: " << eff << "%: " << gNofRecoTracks - gNofNonGhosts << "/" << gNofRecoTracks << endl;
  NumberToFile("nofRecoTracks", gNofRecoTracks);
  NumberToFile("nofGhosts", gNofRecoTracks - gNofNonGhosts);

  eff = 100 * gNofClones;

  if (0 == gNofRecoTracks) eff = 100;
  else
    eff /= gNofRecoTracks;

  cout << "The number of clones: " << eff << "%: " << gNofClones << "/" << gNofRecoTracks << endl;
  NumberToFile("nofClones", gNofClones);
  //cout << "Nof STS[0]: " << nofSts[0] << endl;
  //cout << "Nof STS[1]: " << nofSts[1] << endl;
  //cout << "Nof TRD[0]: " << nofTrd[0] << endl;
  //cout << "Nof TRD[1]: " << nofTrd[1] << endl;
  //cout << "Nof TRD[2]: " << nofTrd[2] << endl;
  //cout << "Nof TRD[3]: " << nofTrd[3] << endl;
  //cout << "Nof MUCH[0]: " << nofMuch[0] << endl;
  //cout << "Nof MUCH[1]: " << nofMuch[1] << endl;
  //cout << "Nof MUCH[2]: " << nofMuch[2] << endl;
  //cout << "Nof TOF: " << nofTof << endl;

  //cout << "Nof stranger hits on TRD stations: ";

  //for (int i = 0; i < 4; ++i)
  //cout << "[" << trdNofStrangerHits[i] << "]";

  //cout << endl;

  int nofLambdas      = lambdaList.size();
  int nofLambdasInAcc = 0;
  int nofRecoLambdas  = 0;
  cout << "The number of Lambda baryons: " << nofLambdas << endl;

  for (list<TrackDesc*>::const_iterator i = lambdaList.begin(); i != lambdaList.end(); ++i) {
    const TrackDesc* lambdaTrack = *i;
    bool isInAcc                 = !lambdaTrack->children.empty();
    bool isReco                  = isInAcc;

    for (list<TrackDesc*>::const_iterator j = lambdaTrack->children.begin(); j != lambdaTrack->children.end(); ++j) {
      const TrackDesc* childTrack = *j;
      Double_t p                  = childTrack->ptr->GetP();
      lambdaChildrenMoms->Fill(p);

      if (!childTrack->isReference) isInAcc = false;
      else {
        if (childTrack->isReconstructed) lambdaChildrenEffByMom->Fill(p, 100.);
        else
          lambdaChildrenEffByMom->Fill(p, 0.);
      }

      if (!childTrack->isReconstructed) isReco = false;
    }

    if (isInAcc) {
      ++nofLambdasInAcc;

      if (isReco) ++nofRecoLambdas;
    }
  }

  eff = 100 * nofLambdasInAcc;

  if (0 == nofLambdas) eff = 100;
  else
    eff /= nofLambdas;

  cout << "Lambda baryons in the acceptance: " << eff << "% " << nofLambdasInAcc << "/" << nofLambdas << endl;

  eff = 100 * nofRecoLambdas;

  if (0 == nofLambdas) eff = 100;
  else
    eff /= nofLambdas;

  cout << "Reconstructed of all the Lambda baryons: " << eff << "% " << nofRecoLambdas << "/" << nofLambdas << endl;

  eff = 100 * nofRecoLambdas;

  if (0 == nofLambdasInAcc) eff = 100;
  else
    eff /= nofLambdasInAcc;

  cout << "Reconstructed of the Lambda baryons in the acceptance: " << eff << "% " << nofRecoLambdas << "/"
       << nofLambdasInAcc << endl;

  effOfMCPoints("STS", gStsPoints);
  effOfMCPoints("MuCh", gMuchPoints);
  effOfMCPoints("TRD", gTrdPoints);
  effOfMCPoints("ToF", gTofPoints);

  SaveHisto(effByMom);
  SaveHisto(effByMomPrimary);
  SaveHisto(effByMomNonPrimary);

  SaveHisto(effByPolarAngle);
  SaveHisto(effByPolarAnglePrimary);
  SaveHisto(effByPolarAngleNonPrimary);

  SaveHisto(effByXAngle);
  SaveHisto(effByXAnglePrimary);
  SaveHisto(effByXAngleNonPrimary);

  SaveHisto(effByYAngle);
  SaveHisto(effByYAnglePrimary);
  SaveHisto(effByYAngleNonPrimary);

  SaveHisto(lambdaChildrenMoms);
  SaveHisto(lambdaChildrenEffByMom);
  SaveHisto(clonesNofSameHits);

  SaveHisto(muchHitResidualX);
  SaveHisto(muchHitResidualY);
  SaveHisto(muchHitResidualT);
  SaveHisto(muchHitPullX);
  SaveHisto(muchHitPullY);
  SaveHisto(muchHitPullT);

  SaveHisto(stsTrackResidualFirstX);
  SaveHisto(stsTrackResidualFirstY);
  SaveHisto(stsTrackPullFirstX);
  SaveHisto(stsTrackPullFirstY);
  SaveHisto(stsTrackResidualLastX);
  SaveHisto(stsTrackResidualLastY);
  SaveHisto(stsTrackPullLastX);
  SaveHisto(stsTrackPullLastY);

  SaveHisto(muchTrackResidualFirstX);
  SaveHisto(muchTrackResidualFirstY);
  SaveHisto(muchTrackPullFirstX);
  SaveHisto(muchTrackPullFirstY);
  SaveHisto(muchTrackResidualLastX);
  SaveHisto(muchTrackResidualLastY);
  SaveHisto(muchTrackPullLastX);
  SaveHisto(muchTrackPullLastY);

  SaveHisto(trdTrackResidualFirstX);
  SaveHisto(trdTrackResidualFirstY);
  SaveHisto(trdTrackPullFirstX);
  SaveHisto(trdTrackPullFirstY);
  SaveHisto(trdTrackResidualLastX);
  SaveHisto(trdTrackResidualLastY);
  SaveHisto(trdTrackPullLastX);
  SaveHisto(trdTrackPullLastY);

  SaveHisto(globalTrackResidualFirstX);
  SaveHisto(globalTrackResidualFirstY);
  SaveHisto(globalTrackPullFirstX);
  SaveHisto(globalTrackPullFirstY);
  SaveHisto(globalTrackResidualLastX);
  SaveHisto(globalTrackResidualLastY);
  SaveHisto(globalTrackPullLastX);
  SaveHisto(globalTrackPullLastY);

#ifdef CBM_BINNED_QA_FILL_HISTOS
  SaveHisto(stsXResHisto);
  SaveHisto(stsYResHisto);
  SaveHisto(stsTResHisto);
  SaveHisto(trdXResHisto);
  SaveHisto(trdYResHisto);
  SaveHisto(trdTResHisto);
  SaveHisto(muchXResHisto);
  SaveHisto(muchYResHisto);
  SaveHisto(muchTResHisto);
  SaveHisto(tofXResHisto);
  SaveHisto(tofYResHisto);
  SaveHisto(tofTResHisto);

  SaveHisto(extrStsXHisto);
  SaveHisto(extrStsYHisto);

  SaveHisto(vtxXHisto);
  SaveHisto(vtxYHisto);
  SaveHisto(vtxZHisto);

  for (int i = 0; i < 2; ++i) {
    SaveHisto(stsXPullHistos[i]);
    SaveHisto(stsYPullHistos[i]);
  }

  for (int i = 0; i < 4; ++i) {
    SaveHisto(extrTrdXHistos[i]);
    SaveHisto(extrTrdYHistos[i]);

    SaveHisto(trdNearestHitDistHistos[i]);

    SaveHisto(trdXPullHistos[i]);
    SaveHisto(trdYPullHistos[i]);
  }
#endif  //CBM_BINNED_QA_FILL_HISTOS
}

void CbmBinnedTrackerQA::SetParContainers()
{
  fSettings = static_cast<CbmBinnedSettings*>(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmBinnedSettings"));
}

ClassImp(CbmBinnedTrackerQA)
