/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "LxTBTrdTask.h"
#include "CbmCluster.h"
#include "CbmGlobalTrack.h"
#include "CbmMCDataManager.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmTrdHit.h"
#include "CbmTrdPoint.h"
#include "CbmTrdTrack.h"
#include "FairLogger.h"
#include "LxTBTask.h"
#include "TClonesArray.h"
#include "TGeoBBox.h"
#include "TGeoManager.h"
#include "TH1F.h"
#include "TMath.h"

#include <TFile.h>

using std::cout;
using std::endl;
using std::ifstream;
using std::ios_base;
using std::list;
using std::map;
using std::ofstream;
using std::pair;
using std::set;
using std::vector;

ClassImp(LxTBTrdFinder)

  LxTBTrdFinder::LxTBTrdFinder()
  : fFinder(nullptr)
  , fTrigDistance(200)
  , recoTracks()
  , nof_timebins(5)
  , last_timebin(nof_timebins - 1)
  , fTrdHits(nullptr)
  , fTrdClusters(nullptr)
  , fTrdDigiMatches(nullptr)
  , fTrdTracks(nullptr)
  , fGlobalTracks(nullptr)
#ifdef LXTB_QA
  , fTrdMCPoints(nullptr)
  , fMCTracks()
  , fTrdPoints()
  , fNEvents(1000)
#endif  //LXTB_QA
{
}

static void
FindGeoChild(TGeoNode* node, const char* name, list<TGeoNode*>& results) {
  Int_t nofChildren = node->GetNdaughters();

  for (Int_t i = 0; i < nofChildren; ++i) {
    TGeoNode* child = node->GetDaughter(i);
    TString childName(child->GetName());

    if (childName.Contains(name, TString::kIgnoreCase))
      results.push_back(child);
  }
}

void LxTBTrdFinder::HandleGeometry() {
  Double_t localCoords[3] = {0., 0., 0.};
  Double_t globalCoords[3];
  TGeoNavigator* pNavigator = gGeoManager->GetCurrentNavigator();
  gGeoManager->cd("/cave_1");
  list<TGeoNode*> detectors;
  FindGeoChild(gGeoManager->GetCurrentNode(), "trd", detectors);

  for (list<TGeoNode*>::iterator i = detectors.begin(); i != detectors.end();
       ++i) {
    TGeoNode* detector = *i;
    pNavigator->CdDown(detector);
    list<TGeoNode*> layers;
    FindGeoChild(detector, "layer", layers);
    int layerNumber = 0;

    for (list<TGeoNode*>::iterator j = layers.begin(); j != layers.end(); ++j) {
      TGeoNode* layer = *j;
      pNavigator->CdDown(layer);
      list<TGeoNode*> modules;
      FindGeoChild(layer, "module", modules);

      for (list<TGeoNode*>::iterator k = modules.begin(); k != modules.end();
           ++k) {
        TGeoNode* module = *k;
        pNavigator->CdDown(module);
        list<TGeoNode*> padCoppers;
        FindGeoChild(module, "padcopper", padCoppers);

        for (list<TGeoNode*>::iterator l = padCoppers.begin();
             l != padCoppers.end();
             ++l) {
          TGeoNode* padCopper = *l;
          pNavigator->CdDown(padCopper);
          TGeoBBox* pBox =
            static_cast<TGeoBBox*>(padCopper->GetVolume()->GetShape());
          pBox->ComputeBBox();
          gGeoManager->LocalToMaster(localCoords, globalCoords);
          fFinder->stations[layerNumber].z = globalCoords[2];

          if (fFinder->stations[layerNumber].minY
              > globalCoords[1] - pBox->GetDY())
            fFinder->stations[layerNumber].minY =
              globalCoords[1] - pBox->GetDY();

          if (fFinder->stations[layerNumber].maxY
              < globalCoords[1] + pBox->GetDY())
            fFinder->stations[layerNumber].maxY =
              globalCoords[1] + pBox->GetDY();

          if (fFinder->stations[layerNumber].minX
              > globalCoords[0] - pBox->GetDX())
            fFinder->stations[layerNumber].minX =
              globalCoords[0] - pBox->GetDX();

          if (fFinder->stations[layerNumber].maxX
              < globalCoords[0] + pBox->GetDX())
            fFinder->stations[layerNumber].maxX =
              globalCoords[0] + pBox->GetDX();

          pNavigator->CdUp();
        }

        pNavigator->CdUp();
      }

      ++layerNumber;
      pNavigator->CdUp();
    }

    pNavigator->CdUp();
  }
}

static bool GetHistoRMS(const char* name, Double_t& retVal) {
  char fileName[64];
  sprintf(fileName, "%s.root", name);
  bool result    = false;
  TFile* curFile = TFile::CurrentFile();
  TFile* f       = new TFile(fileName);

  if (!f->IsZombie()) {
    TH1F* h = static_cast<TH1F*>(f->Get(name));
    retVal  = h->GetRMS();
    result  = true;
  }

  delete f;
  TFile::CurrentFile() = curFile;
  return result;
}

static TH1F* signalDistanceHisto = 0;

InitStatus LxTBTrdFinder::Init() {
  signalDistanceHisto =
    new TH1F("signalDistanceHisto", "signalDistanceHisto", 200, 0., 800.);
  speedOfLight =
    100 * TMath::C();  // Multiply by 100 to express in centimeters.
  nof_timebins                  = 5;  // Corresponds to event by event mode.
  pair<int, int> stSpatLimits[] = {{20, 20}, {20, 20}, {20, 20}, {20, 20}};
  fFinder                       = new LxTbBinnedFinder(0,
                                 CUR_NOF_STATIONS,
                                 nof_timebins,
                                 stSpatLimits,
                                 20,
                                 20,
                                 CUR_TIMEBIN_LENGTH);
  fFinder->fHasTrd              = false;
  HandleGeometry();

  for (int i = 0; i < 4; ++i) {
    fFinder->stations[i].binSizeX =
      (fFinder->stations[i].maxX - fFinder->stations[i].minX)
      / fFinder->stations[i].nofXBins;
    fFinder->stations[i].binSizeY =
      (fFinder->stations[i].maxY - fFinder->stations[i].minY)
      / fFinder->stations[i].nofYBins;
  }

  for (int i = 1; i < 4; ++i) {
    char name[64];
    sprintf(name, "trdDeltaThetaX_%d", i);
    Double_t deltaThetaX = 0;

    if (!GetHistoRMS(name, deltaThetaX)) return kFATAL;

    fFinder->stations[i].deltaThetaX = deltaThetaX;
    sprintf(name, "trdDeltaThetaY_%d", i);
    Double_t deltaThetaY = 0;

    if (!GetHistoRMS(name, deltaThetaY)) return kFATAL;

    fFinder->stations[i].deltaThetaY = deltaThetaY;
    scaltype deltaZ = fFinder->stations[i].z - fFinder->stations[i - 1].z;
    fFinder->stations[i].dispX = deltaThetaX * deltaZ;
    fFinder->stations[i].dispY = deltaThetaY * deltaZ;
  }

  FairRootManager* ioman = FairRootManager::Instance();

  if (0 == ioman) LOG(fatal) << "No FairRootManager";

  Int_t nofEventsInFile = ioman->CheckMaxEventNo();

  if (nofEventsInFile < fNEvents) fNEvents = nofEventsInFile;

  fTrdHits = static_cast<TClonesArray*>(ioman->GetObject("TrdHit"));

  fTrdTracks = new TClonesArray("CbmTrdTrack", 100);
  LOG(info) << "IsOutputBranchPersistent(TrdTrack) is: "
            << (IsOutputBranchPersistent("TrdTrack") ? "true" : "false");
  ioman->Register(
    "TrdTrack", "Trd", fTrdTracks, IsOutputBranchPersistent("TrdTrack"));

  fGlobalTracks = new TClonesArray("CbmGlobalTrack", 100);
  LOG(info) << "IsOutputBranchPersistent(GlobalTrack) is: "
            << (IsOutputBranchPersistent("GlobalTrack") ? "true" : "false");
  ioman->Register("GlobalTrack",
                  "Global",
                  fGlobalTracks,
                  IsOutputBranchPersistent("GlobalTrack"));

#ifdef LXTB_QA
  CbmMCDataManager* mcManager =
    static_cast<CbmMCDataManager*>(ioman->GetObject("MCDataManager"));
  fTrdMCPoints = mcManager->InitBranch("TrdPoint");
  fTrdClusters = static_cast<TClonesArray*>(ioman->GetObject("TrdCluster"));
  fTrdDigiMatches =
    static_cast<TClonesArray*>(ioman->GetObject("TrdDigiMatch"));
  CbmMCDataArray* mcTracks = mcManager->InitBranch("MCTrack");

  for (int i = 0; i < fNEvents; ++i) {
    Int_t evSize = mcTracks->Size(0, i);
    fMCTracks.push_back(vector<TrackDataHolder>());

    if (0 >= evSize) continue;

    vector<TrackDataHolder>& evTracks = fMCTracks.back();

    for (int j = 0; j < evSize; ++j) {
      evTracks.push_back(TrackDataHolder());
      const CbmMCTrack* mcTrack =
        static_cast<const CbmMCTrack*>(mcTracks->Get(0, i, j));
      evTracks.back().pdg = mcTrack->GetPdgCode();
      evTracks.back().z   = mcTrack->GetStartZ();

      if (mcTrack->GetPdgCode() == 11 || mcTrack->GetPdgCode() == -11) {
        //Double_t m = mcTrack->GetMass(); (VF) Unused
        Int_t motherId = mcTrack->GetMotherId();

        if (static_cast<const CbmMCTrack*>(mcTracks->Get(0, i, motherId))
              ->GetPdgCode()
            == 443)
          evTracks.back().isSignal = true;
      }
    }  // for (int j = 0; j < evSize; ++j)
  }    // for (int i = 0; i < fNEvents; ++i)

  for (int i = 0; i < fNEvents; ++i) {
    Int_t evSize = fTrdMCPoints->Size(0, i);
    fTrdPoints.push_back(vector<PointDataHolder>());

    if (0 >= evSize) continue;

    vector<PointDataHolder>& evPoints = fTrdPoints.back();

    for (int j = 0; j < evSize; ++j) {
      const CbmTrdPoint* pTrdPt =
        static_cast<const CbmTrdPoint*>(fTrdMCPoints->Get(0, i, j));
      //Int_t trackId = pTrdPt->GetTrackID(); (VF) Unused
      Int_t layerNumber = CbmTrdAddress::GetLayerId(pTrdPt->GetModuleAddress());
      PointDataHolder trdPt;
      trdPt.x           = (pTrdPt->GetXIn() + pTrdPt->GetXOut()) / 2;
      trdPt.y           = (pTrdPt->GetYIn() + pTrdPt->GetYOut()) / 2;
      trdPt.z           = (pTrdPt->GetZIn() + pTrdPt->GetZOut()) / 2;
      trdPt.t           = 0;  //fEventTimes[i] + pTrdPt->GetTime();
      trdPt.eventId     = i;
      trdPt.trackId     = pTrdPt->GetTrackID();
      trdPt.pointId     = j;
      trdPt.layerNumber = layerNumber;
      evPoints.push_back(trdPt);
      fMCTracks[trdPt.eventId][trdPt.trackId].pointInds[trdPt.layerNumber] =
        trdPt.pointId;
    }
  }

  int eventId = 0;

  for (vector<vector<TrackDataHolder>>::iterator i = fMCTracks.begin();
       i != fMCTracks.end();
       ++i) {
    vector<TrackDataHolder>& evTracks = *i;
    list<TrackDataHolder*> eles;
    list<TrackDataHolder*> poss;

    for (vector<TrackDataHolder>::iterator j = evTracks.begin();
         j != evTracks.end();
         ++j) {
      TrackDataHolder& track = *j;

      if (11 == track.pdg && 15 > track.z) {
        bool use = true;

        for (int k = 0; k < CUR_NOF_STATIONS; ++k) {
          if (track.pointInds[k] < 0) {
            use = false;
            break;
          }
        }

        if (use) eles.push_back(&track);
      } else if (-11 == track.pdg && 15 > track.z) {
        bool use = true;

        for (int k = 0; k < CUR_NOF_STATIONS; ++k) {
          if (track.pointInds[k] < 0) {
            use = false;
            break;
          }
        }

        if (use) poss.push_back(&track);
      }

      if (!track.isSignal) continue;

      /*if (11 == track.pdg)
            negTrack = &track;
         else
            posTrack = &track;*/

      for (int k = 0; k < CUR_NOF_STATIONS; ++k) {
        if (track.pointInds[k] < 0) {
          track.isSignal = false;
          break;
        }
      }

      /*if (track.isSignal)
         {
            if (11 == track.pdg)
               negTrack = &track;
            else
               posTrack = &track;
         }*/
    }

    /*if (0 != posTrack && 0 != negTrack && posTrack->pointInds[0] >= 0 && negTrack->pointInds[0] >= 0)
      {
         const PointDataHolder& posPt = fTrdPoints[eventId][posTrack->pointInds[0]];
         const PointDataHolder& negPt = fTrdPoints[eventId][negTrack->pointInds[0]];
         signalDistanceHisto->Fill(sqrt((posPt.x - negPt.x) * (posPt.x - negPt.x) + (posPt.y - negPt.y) * (posPt.y - negPt.y)));
      }*/

    for (list<TrackDataHolder*>::const_iterator k = eles.begin();
         k != eles.end();
         ++k) {
      const TrackDataHolder* negTrack = *k;

      for (list<TrackDataHolder*>::const_iterator l = poss.begin();
           l != poss.end();
           ++l) {
        const TrackDataHolder* posTrack = *l;
        const PointDataHolder& posPt =
          fTrdPoints[eventId][posTrack->pointInds[0]];
        const PointDataHolder& negPt =
          fTrdPoints[eventId][negTrack->pointInds[0]];
        signalDistanceHisto->Fill(
          sqrt((posPt.x - negPt.x) * (posPt.x - negPt.x)
               + (posPt.y - negPt.y) * (posPt.y - negPt.y)));
      }
    }

    ++eventId;
  }
#endif  //LXTB_QA

  return kSUCCESS;  //, kERROR, kFATAL
}

static Int_t currentEventN            = 0;
static unsigned long long tsStartTime = 0;

static void SpliceTriggerings(list<pair<timetype, timetype>>& out,
                              LxTbBinnedFinder::TriggerTimeArray& in) {
  for (int i = 0; i < in.nofTimebins; ++i)
    out.splice(out.end(), in.triggerTimeBins[i]);
}

#ifdef LXTB_QA
static list<pair<timetype, timetype>> triggerTimes_trd0_sign0_dist0;
static list<pair<timetype, timetype>> triggerTimes_trd0_sign0_dist1;
static list<pair<timetype, timetype>> triggerTimes_trd0_sign1_dist0;
static list<pair<timetype, timetype>> triggerTimes_trd0_sign1_dist1;
static list<pair<timetype, timetype>> triggerTimes_trd1_sign0_dist0;
static list<pair<timetype, timetype>> triggerTimes_trd1_sign0_dist1;
static list<pair<timetype, timetype>> triggerTimes_trd1_sign1_dist0;
static list<pair<timetype, timetype>> triggerTimes_trd1_sign1_dist1;
static scaltype gMaxDx = 0;
static scaltype gMaxDy = 0;
#endif  //LXTB_QA

void LxTBTrdFinder::Exec(Option_t*) {
  fFinder->Clear();
  fFinder->SetTSBegin(tsStartTime);

  for (int i = 0; i < fTrdHits->GetEntriesFast(); ++i) {
    const CbmTrdHit* hit = static_cast<const CbmTrdHit*>(fTrdHits->At(i));
    Int_t stationNumber  = hit->GetPlaneId();
    scaltype x           = hit->GetX();
    scaltype y           = hit->GetY();
    timetype t           = tsStartTime + 50;  //hit->GetTime();
    scaltype dx          = hit->GetDx();

    if (gMaxDx < dx) gMaxDx = dx;

    scaltype dy = hit->GetDy();

    if (gMaxDy < dy) gMaxDy = dy;

    timetype dt = 4;  //hit->GetTimeError();
    LxTbBinnedPoint point(
      x, dx, y, dy, t, dt, i, CUR_LAST_STATION == stationNumber);
#ifdef LXTB_QA
    point.isTrd         = true;
    point.stationNumber = stationNumber;
    Int_t clusterId     = hit->GetRefId();
    const CbmCluster* cluster =
      static_cast<const CbmCluster*>(fTrdClusters->At(clusterId));
    Int_t nDigis = cluster->GetNofDigis();

    for (Int_t j = 0; j < nDigis; ++j) {
      const CbmMatch* digiMatch =
        static_cast<const CbmMatch*>(fTrdDigiMatches->At(cluster->GetDigi(j)));
      Int_t nMCs = digiMatch->GetNofLinks();

      for (Int_t k = 0; k < nMCs; ++k) {
        const CbmLink& lnk       = digiMatch->GetLink(k);
        Int_t eventId            = currentEventN;  // : lnk.GetEntry();
        Int_t pointId            = lnk.GetIndex();
        const FairMCPoint* pMCPt = static_cast<const FairMCPoint*>(
          fTrdMCPoints->Get(0, eventId, pointId));
        Int_t trackId                     = pMCPt->GetTrackID();
        LxTbBinnedPoint::PointDesc ptDesc = {eventId, pointId, trackId};
        point.mcRefs.push_back(ptDesc);
      }
    }  // for (Int_t j = 0; j < nDigis; ++j)

    scaltype minY     = fFinder->stations[stationNumber].minY;
    scaltype binSizeY = fFinder->stations[stationNumber].binSizeY;
    int lastYBin      = fFinder->stations[stationNumber].lastYBin;
    scaltype minX     = fFinder->stations[stationNumber].minX;
    scaltype binSizeX = fFinder->stations[stationNumber].binSizeX;
    int lastXBin      = fFinder->stations[stationNumber].lastXBin;

    int tInd = (t - fFinder->minT) / CUR_TIMEBIN_LENGTH;

    if (tInd < 0)
      tInd = 0;
    else if (tInd > int(last_timebin))
      tInd = last_timebin;

    LxTbTYXBin& tyxBin = fFinder->stations[stationNumber].tyxBins[tInd];
    int yInd           = (y - minY) / binSizeY;

    if (yInd < 0)
      yInd = 0;
    else if (yInd > lastYBin)
      yInd = lastYBin;

    LxTbYXBin& yxBin = tyxBin.yxBins[yInd];
    int xInd         = (x - minX) / binSizeX;

    if (xInd < 0)
      xInd = 0;
    else if (xInd > lastXBin)
      xInd = lastXBin;

    LxTbXBin& xBin = yxBin.xBins[xInd];
    xBin.points.push_back(point);

    if (CUR_LAST_STATION == stationNumber) {
      xBin.use   = true;
      yxBin.use  = true;
      tyxBin.use = true;
    }
#endif  //LXTB_QA
  }

  fFinder->Reconstruct();

  //recoTracks.clear();
  fTrdTracks->Delete();
  fGlobalTracks->Clear();
  int trdTrackNo = 0;

  for (int i = 0; i < fFinder->nofTrackBins; ++i) {
    list<LxTbBinnedFinder::Chain*>& recoTracksBin = fFinder->recoTracks[i];

    for (list<LxTbBinnedFinder::Chain*>::const_iterator j =
           recoTracksBin.begin();
         j != recoTracksBin.end();
         ++j) {
      LxTbBinnedFinder::Chain* chain = *j;
      recoTracks.push_back(chain);

      CbmTrdTrack* track = new ((*fTrdTracks)[trdTrackNo]) CbmTrdTrack();

      for (int k = 0; k < chain->nofPoints; ++k)
        track->AddHit(chain->points[k]->refId, kTRDHIT);

      track->SetChiSq(chain->chi2);
      track->SetNDF(2 * chain->nofPoints - 5);

      CbmGlobalTrack* globalTrack =
        new ((*fGlobalTracks)[trdTrackNo]) CbmGlobalTrack();
      globalTrack->SetTrdTrackIndex(trdTrackNo);
      ++trdTrackNo;
    }
  }

  //cout << "In the event #: " << currentEventN << " " << recoTracks.size() << " reconstructed" << endl;

  SpliceTriggerings(triggerTimes_trd0_sign0_dist0,
                    fFinder->triggerTimes_trd0_sign0_dist0);
  SpliceTriggerings(triggerTimes_trd0_sign0_dist1,
                    fFinder->triggerTimes_trd0_sign0_dist1);
  SpliceTriggerings(triggerTimes_trd0_sign1_dist0,
                    fFinder->triggerTimes_trd0_sign1_dist0);
  SpliceTriggerings(triggerTimes_trd0_sign1_dist1,
                    fFinder->triggerTimes_trd0_sign1_dist1);
  SpliceTriggerings(triggerTimes_trd1_sign0_dist0,
                    fFinder->triggerTimes_trd1_sign0_dist0);
  SpliceTriggerings(triggerTimes_trd1_sign0_dist1,
                    fFinder->triggerTimes_trd1_sign0_dist1);
  SpliceTriggerings(triggerTimes_trd1_sign1_dist0,
                    fFinder->triggerTimes_trd1_sign1_dist0);
  SpliceTriggerings(triggerTimes_trd1_sign1_dist1,
                    fFinder->triggerTimes_trd1_sign1_dist1);

  ++currentEventN;
  tsStartTime += CUR_TIMEBIN_LENGTH;
}

struct RecoTrackData {
  Int_t eventId;
  Int_t trackId;

  RecoTrackData(Int_t eId, Int_t tId) : eventId(eId), trackId(tId) {}
};

struct RTDLess {
  bool operator()(const RecoTrackData& x, const RecoTrackData& y) const {
    if (x.eventId < y.eventId) return true;

    return x.trackId < y.trackId;
  }
};

static void SaveHisto(TH1* histo) {
  TFile* curFile    = TFile::CurrentFile();
  TString histoName = histo->GetName();
  histoName += ".root";
  TFile fh(histoName.Data(), "RECREATE");
  histo->Write();
  fh.Close();
  delete histo;
  TFile::CurrentFile() = curFile;
}

void LxTBTrdFinder::Finish() {
  int nofRecoTracks = recoTracks.size();
  cout << "LxTbBinnedFinder::Reconstruct() the number of found tracks: "
       << nofRecoTracks << endl;
#ifdef LXTB_QA
  static int nofSignalTracks     = 0;
  static int nofRecoSignalTracks = 0;
  int eventN                     = 0;

  for (vector<vector<TrackDataHolder>>::iterator i = fMCTracks.begin();
       i != fMCTracks.end();
       ++i) {
    vector<TrackDataHolder>& evTracks = *i;

    for (vector<TrackDataHolder>::iterator j = evTracks.begin();
         j != evTracks.end();
         ++j) {
      TrackDataHolder& track = *j;

      if (!track.isSignal) continue;

      ++nofSignalTracks;

      int nofMatchPoints = 0;

      for (list<LxTbBinnedFinder::Chain*>::const_iterator k =
             recoTracks.begin();
           k != recoTracks.end();
           ++k) {
        const LxTbBinnedFinder::Chain* chain = *k;

        for (int l = 0; l < CUR_NOF_STATIONS; ++l) {
          bool pointsMatched = false;

          for (list<LxTbBinnedPoint::PointDesc>::const_iterator m =
                 chain->points[l]->mcRefs.begin();
               m != chain->points[l]->mcRefs.end();
               ++m) {
            if (m->eventId == eventN && m->pointId == track.pointInds[l]) {
              pointsMatched = true;
              break;
            }
          }

          if (pointsMatched) ++nofMatchPoints;
        }
      }

      if (nofMatchPoints >= CUR_NOF_STATIONS - 1) {
        ++nofRecoSignalTracks;
        continue;
      }
    }

    ++eventN;
  }

  double eff =
    0 == nofSignalTracks ? 100 : 100.0 * nofRecoSignalTracks / nofSignalTracks;
  cout << "Reconstruction efficiency is: " << eff << "% [ "
       << nofRecoSignalTracks << " / " << nofSignalTracks << " ]" << endl;

  int nofRightRecoTracks = 0;
  map<Int_t, pair<list<LxTbBinnedPoint*>, list<LxTbBinnedPoint*>>>
    elecPositrons;

  for (list<LxTbBinnedFinder::Chain*>::const_iterator i = recoTracks.begin();
       i != recoTracks.end();
       ++i) {
    const LxTbBinnedFinder::Chain* chain = *i;
    map<RecoTrackData, int, RTDLess> nofTracks;

    for (int j = 0; j < CUR_NOF_STATIONS; ++j) {
      int stMask = 1 << j;

      for (list<LxTbBinnedPoint::PointDesc>::const_iterator k =
             chain->points[j]->mcRefs.begin();
           k != chain->points[j]->mcRefs.end();
           ++k) {
        RecoTrackData st(k->eventId, k->trackId);
        map<RecoTrackData, int, RTDLess>::iterator nofTIter =
          nofTracks.find(st);

        if (nofTIter != nofTracks.end())
          nofTIter->second |= stMask;
        else
          nofTracks[st] = stMask;
      }
    }

    int nofPoints                    = 0;
    const RecoTrackData* bestMCTrack = 0;

    for (map<RecoTrackData, int, RTDLess>::const_iterator j = nofTracks.begin();
         j != nofTracks.end();
         ++j) {
      int nofp = 0;

      for (int k = 0; k < CUR_NOF_STATIONS; ++k) {
        if (j->second & (1 << k)) ++nofp;
      }

      if (nofp > nofPoints) {
        nofPoints   = nofp;
        bestMCTrack = &j->first;
      }
    }

    if (nofPoints >= CUR_NOF_STATIONS - 1) {
      ++nofRightRecoTracks;

      //if (35 > fMCTracks[bestMCTrack->eventId][bestMCTrack->trackId].z)
      {
        if (11 == fMCTracks[bestMCTrack->eventId][bestMCTrack->trackId].pdg)
          elecPositrons[bestMCTrack->eventId].first.push_back(chain->points[0]);
        else if (-11
                 == fMCTracks[bestMCTrack->eventId][bestMCTrack->trackId].pdg)
          elecPositrons[bestMCTrack->eventId].second.push_back(
            chain->points[0]);
      }
    }
  }

  eff =
    0 == recoTracks.size() ? 100 : 100.0 * nofRightRecoTracks / nofRecoTracks;
  cout << "Non ghosts are: " << eff << "% [ " << nofRightRecoTracks << " / "
       << nofRecoTracks << " ]" << endl;

  int nofTriggPairs = 0;

  for (map<Int_t,
           pair<list<LxTbBinnedPoint*>, list<LxTbBinnedPoint*>>>::iterator i =
         elecPositrons.begin();
       i != elecPositrons.end();
       ++i) {
    list<LxTbBinnedPoint*>& evEls = i->second.first;
    list<LxTbBinnedPoint*>& evPos = i->second.second;
    bool trigPair                 = false;

    for (list<LxTbBinnedPoint*>::const_iterator j = evEls.begin();
         j != evEls.end();
         ++j) {
      const LxTbBinnedPoint* elp = *j;
      scaltype negX              = elp->x;
      scaltype negY              = elp->y;

      for (list<LxTbBinnedPoint*>::const_iterator k = evPos.begin();
           k != evPos.end();
           ++k) {
        const LxTbBinnedPoint* pop = *k;
        scaltype posX              = pop->x;
        scaltype posY              = pop->y;

        if (sqrt((posX - negX) * (posX - negX) + (posY - negY) * (posY - negY))
            > fTrigDistance)
          trigPair = true;
      }
    }

    if (trigPair) ++nofTriggPairs;
  }

  cout << "NOF triggering events: " << nofTriggPairs << endl;

  SaveHisto(signalDistanceHisto);
#endif  //LXTB_QA
}
