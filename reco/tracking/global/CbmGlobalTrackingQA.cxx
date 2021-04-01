/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "CbmGlobalTrackingQA.h"

#include "CbmDigiManager.h"
#include "CbmGlobalTrack.h"
#include "CbmMCDataManager.h"
#include "CbmMCTrack.h"
#include "CbmStsCluster.h"
#include "CbmStsDigi.h"
#include "CbmStsHit.h"
#include "CbmStsPoint.h"
#include "CbmStsTrack.h"
#include "CbmTofPoint.h"

#include <Logger.h>

#include <map>

using std::cout;
using std::endl;
using std::list;
using std::map;
using std::set;
using std::vector;


CbmGlobalTrackingQA::CbmGlobalTrackingQA() {}

InitStatus CbmGlobalTrackingQA::Init() {
  // FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (nullptr == ioman) LOG(fatal) << "No FairRootManager";

  // MC data
  CbmMCDataManager* mcManager =
    static_cast<CbmMCDataManager*>(ioman->GetObject("MCDataManager"));
  if (nullptr == mcManager) LOG(fatal) << "No MC data manager";
  fMCTracks = mcManager->InitBranch("MCTrack");
  if (nullptr == fMCTracks) LOG(fatal) << "No MC tracks";
  fStsMCPoints = mcManager->InitBranch("StsPoint");
  if (nullptr == fStsMCPoints) LOG(fatal) << "No STS MC points";
  fTofMCPoints = mcManager->InitBranch("TofPoint");
  if (nullptr == fTofMCPoints) LOG(fatal) << "No ToF MC points";

  // Digi data
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();
  if (!fDigiMan->IsPresent(ECbmModuleId::kSts)) LOG(fatal) << "No STS digis";
  if (!fDigiMan->IsMatchPresent(ECbmModuleId::kSts))
    LOG(fatal) << "No STS digi matches";
  if (!fDigiMan->IsPresent(ECbmModuleId::kTof)) LOG(fatal) << "No ToF digis";

  // STS data
  fStsClusters = static_cast<TClonesArray*>(ioman->GetObject("StsCluster"));
  if (nullptr == fStsClusters) LOG(fatal) << "No STS clusters";
  fStsHits = static_cast<TClonesArray*>(ioman->GetObject("StsHit"));
  if (nullptr == fStsHits) LOG(fatal) << "No STS hits";
  fStsTracks = static_cast<TClonesArray*>(ioman->GetObject("StsTrack"));
  if (nullptr == fStsTracks) LOG(fatal) << "No STS tracks";

  // TOF data
  fTofHits = static_cast<TClonesArray*>(ioman->GetObject("TofHit"));
  if (nullptr == fTofHits) LOG(fatal) << "No ToF hits";
  fTofHitMatches = static_cast<TClonesArray*>(ioman->GetObject("TofHitMatch"));
  if (nullptr == fTofHitMatches) LOG(fatal) << "No ToF hit matches";

  // Global track
  fGlobalTracks = static_cast<TClonesArray*>(ioman->GetObject("GlobalTrack"));
  if (nullptr == fGlobalTracks) LOG(fatal) << "No global tracks";

  // Internal arrays
  fTracks.resize(fNofEvents);

  for (int i = 0; i < fNofEvents; ++i) {
    int nofTracks = fMCTracks->Size(0, i);

    if (nofTracks > 0) fTracks[i].resize(nofTracks);

    vector<TrackData>& evTracks = fTracks[i];

    for (int j = 0; j < nofTracks; ++j) {
      TrackData& track = evTracks[j];
      track.hasSts     = false;
      track.z          = -1000;
      track.use        = false;
      track.evN        = i;
      track.ind        = j;
      track.used       = false;
      const CbmMCTrack* mcTrack =
        static_cast<const CbmMCTrack*>(fMCTracks->Get(0, i, j));
      int motherId = mcTrack->GetMotherId();

      if (motherId < 0)
        track.parent = 0;
      else {
        track.parent = &evTracks[motherId];
        evTracks[motherId].offsprings.push_back(&track);
      }
    }
  }


  fStsPoints.resize(fNofEvents);

  for (int i = 0; i < fNofEvents; ++i) {
    int nofStsPoints = fStsMCPoints->Size(0, i);

    if (nofStsPoints > 0) fStsPoints[i].resize(nofStsPoints);

    vector<PointData>& evPoints = fStsPoints[i];

    for (int j = 0; j < nofStsPoints; ++j) {
      PointData& point = evPoints[j];
      point.evN        = i;
      point.ind        = j;
      const CbmStsPoint* stsPoint =
        static_cast<const CbmStsPoint*>(fStsMCPoints->Get(0, i, j));
      Int_t trackId              = stsPoint->GetTrackID();
      fTracks[i][trackId].hasSts = true;
      TrackData& track           = fTracks[i][trackId];
      point.track                = &track;
      point.x = (stsPoint->GetXIn() + stsPoint->GetXOut()) / 2;
      point.y = (stsPoint->GetYIn() + stsPoint->GetYOut()) / 2;
      point.z = (stsPoint->GetZIn() + stsPoint->GetZOut()) / 2;
      point.t = stsPoint->GetTime();
    }
  }


  fTofPoints.resize(fNofEvents);

  for (int i = 0; i < fNofEvents; ++i) {
    int nofTofPoints = fTofMCPoints->Size(0, i);

    if (nofTofPoints > 0) fTofPoints[i].resize(nofTofPoints);

    vector<PointData>& evPoints = fTofPoints[i];

    for (int j = 0; j < nofTofPoints; ++j) {
      PointData& point = evPoints[j];
      point.evN        = i;
      point.ind        = j;
      const CbmTofPoint* tofPoint =
        static_cast<const CbmTofPoint*>(fTofMCPoints->Get(0, i, j));
      Int_t trackId    = tofPoint->GetTrackID();
      TrackData& track = fTracks[i][trackId];
      point.track      = &track;
      point.x          = tofPoint->GetX();
      point.y          = tofPoint->GetY();
      point.z          = tofPoint->GetZ();
      point.t          = tofPoint->GetTime();
      track.tofPoints.push_back(&point);

      if (track.z > 0 && !track.use) {
        double deltaZ = point.z - track.z;
        track.tx      = (point.x - track.x) / deltaZ;
        track.ty      = (point.y - track.y) / deltaZ;
        track.use     = true;
      }
    }
  }

  return kSUCCESS;
}

bool CbmGlobalTrackingQA::CheckMatch(const TrackData* stsMCTrack,
                                     Int_t tofHitInd,
                                     bool deepSearch) const {
  set<TrackData*> tofMCTracks;

  const CbmMatch* tofHitMatch =
    static_cast<const CbmMatch*>(fTofHitMatches->At(tofHitInd));
  int nofTofPoints = tofHitMatch->GetNofLinks();

  for (Int_t j = 0; j < nofTofPoints; ++j) {
    const CbmLink& pointLnk = tofHitMatch->GetLink(j);
    Int_t evN               = pointLnk.GetEntry() - 1;
    Int_t pointInd          = pointLnk.GetIndex();
    tofMCTracks.insert(fTofPoints[evN][pointInd].track);
  }


  for (set<TrackData*>::const_iterator i = tofMCTracks.begin();
       i != tofMCTracks.end();
       ++i) {
    const TrackData* tofMCTrack = *i;

    if (deepSearch) {
      for (const TrackData* mcTrack = tofMCTrack; 0 != mcTrack;
           mcTrack                  = mcTrack->parent) {
        if (mcTrack == stsMCTrack) return true;
      }
    } else if (tofMCTrack == stsMCTrack)
      return true;
  }

  return false;
}

bool CbmGlobalTrackingQA::SemiTofTrack(const TrackData* mcTrack) const {
  if (!mcTrack->tofPoints.empty()) return true;

  for (list<TrackData*>::const_iterator i = mcTrack->offsprings.begin();
       i != mcTrack->offsprings.end();
       ++i) {
    if (SemiTofTrack(*i)) return true;
  }

  return false;
}

int nofReferenceTofTracks          = 0;
int nofCorMergedReferenceTofTracks = 0;
int nofWroMergedReferenceTofTracks = 0;
int nofNonMergedReferenceTofTracks = 0;

int nofSemiTofTracks          = 0;
int nofCorMergedSemiTofTracks = 0;
int nofWroMergedSemiTofTracks = 0;
int nofNonMergedSemiTofTracks = 0;

int nofNonTofTracks          = 0;
int nofMergedNonTofTracks    = 0;
int nofNonMergedNonTofTracks = 0;

void CbmGlobalTrackingQA::Exec(Option_t*) {
  Int_t nofHits = fTofHits->GetEntriesFast();

  for (int i = 0; i < nofHits; ++i) {
    const CbmTofHit* tofHit = static_cast<const CbmTofHit*>(fTofHits->At(i));
    const CbmMatch* hitMatch =
      static_cast<const CbmMatch*>(fTofHitMatches->At(i));
    int nofPoints = hitMatch->GetNofLinks();
    for (Int_t k = 0; k < nofPoints; ++k) {
      const CbmLink& pointLnk = hitMatch->GetLink(k);
      Int_t evN               = pointLnk.GetEntry() - 1;
      Int_t pointInd          = pointLnk.GetIndex();
      const PointData& point  = fTofPoints[evN][pointInd];
      point.track->tofHits.insert(tofHit);
    }
  }  //for (int i = 0; i < nofHits; ++i)

  int nofGlobalTracks = fGlobalTracks->GetEntriesFast();

  for (int i = 0; i < nofGlobalTracks; ++i) {
    const CbmGlobalTrack* globalTrack =
      static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(i));
    int stsTrackInd = globalTrack->GetStsTrackIndex();

    if (stsTrackInd < 0) continue;

    const CbmStsTrack* stsTrack =
      static_cast<const CbmStsTrack*>(fStsTracks->At(stsTrackInd));
    map<TrackData*, int> stsMCTracks;
    int nofStsHits    = stsTrack->GetNofHits();
    int nofStsMatches = 0;

    for (int j = 0; j < nofStsHits; ++j) {
      int stsHitInd = stsTrack->GetHitIndex(j);
      const CbmStsHit* stsHit =
        static_cast<const CbmStsHit*>(fStsHits->At(stsHitInd));
      int frontClusterInd = stsHit->GetFrontClusterId();
      int backClusterInd  = stsHit->GetBackClusterId();
      const CbmStsCluster* frontCluster =
        static_cast<const CbmStsCluster*>(fStsClusters->At(frontClusterInd));
      const CbmStsCluster* backCluster =
        static_cast<const CbmStsCluster*>(fStsClusters->At(backClusterInd));
      int nofFrontDigis = frontCluster->GetNofDigis();

      for (int k = 0; k < nofFrontDigis; ++k) {
        int stsDigiInd = frontCluster->GetDigi(k);
        const CbmMatch* stsDigiMatch =
          fDigiMan->GetMatch(ECbmModuleId::kSts, stsDigiInd);
        int nofLinks = stsDigiMatch->GetNofLinks();

        for (int l = 0; l < nofLinks; ++l) {
          const CbmLink& link = stsDigiMatch->GetLink(l);
          int eventId         = link.GetEntry();
          int mcPointId       = link.GetIndex();
          ++nofStsMatches;
          map<TrackData*, int>::iterator iter =
            stsMCTracks.find(fStsPoints[eventId][mcPointId].track);

          if (iter != stsMCTracks.end())
            ++iter->second;
          else
            stsMCTracks[fStsPoints[eventId][mcPointId].track] = 1;
        }
      }

      int nofBackDigis = backCluster->GetNofDigis();

      for (int k = 0; k < nofBackDigis; ++k) {
        int stsDigiInd = backCluster->GetDigi(k);
        const CbmMatch* stsDigiMatch =
          fDigiMan->GetMatch(ECbmModuleId::kSts, stsDigiInd);
        int nofLinks = stsDigiMatch->GetNofLinks();

        for (int l = 0; l < nofLinks; ++l) {
          const CbmLink& link = stsDigiMatch->GetLink(l);
          int eventId         = link.GetEntry();
          int mcPointId       = link.GetIndex();
          ++nofStsMatches;
          map<TrackData*, int>::iterator iter =
            stsMCTracks.find(fStsPoints[eventId][mcPointId].track);

          if (iter != stsMCTracks.end())
            ++iter->second;
          else
            stsMCTracks[fStsPoints[eventId][mcPointId].track] = 1;
        }
      }
    }  // for (int j = 0; j < nofStsHits; ++j)

    if (stsMCTracks.empty()) continue;

    map<TrackData*, int>::iterator lastStsTrackIter = stsMCTracks.end();
    --lastStsTrackIter;

    if (lastStsTrackIter->second < 0.7 * nofStsMatches) continue;

    int tofHitInd = globalTrack->GetTofHitIndex();

    if (lastStsTrackIter->first->tofPoints.empty()) {
      if (SemiTofTrack(lastStsTrackIter->first)) {
        ++nofSemiTofTracks;

        if (tofHitInd >= 0) {
          if (CheckMatch(lastStsTrackIter->first, tofHitInd, true))
            ++nofCorMergedSemiTofTracks;
          else
            ++nofWroMergedSemiTofTracks;
        } else
          ++nofNonMergedSemiTofTracks;
      } else {
        ++nofNonTofTracks;

        if (tofHitInd >= 0)
          ++nofMergedNonTofTracks;
        else
          ++nofNonMergedNonTofTracks;
      }
    }

    if (lastStsTrackIter->first->tofPoints.empty()
        || lastStsTrackIter->first->tofHits.empty())
      continue;

    ++nofReferenceTofTracks;

    if (tofHitInd < 0) {
      ++nofNonMergedReferenceTofTracks;
      continue;
    }

    if (CheckMatch(lastStsTrackIter->first, tofHitInd))
      ++nofCorMergedReferenceTofTracks;
    else
      ++nofWroMergedReferenceTofTracks;
  }  //for (int i = 0; i < nofGlobalTracks; ++i)
}

void CbmGlobalTrackingQA::Finish() {
  double eff = 100 * nofCorMergedReferenceTofTracks;
  eff /= nofReferenceTofTracks;
  cout << "The correctly merged reference tracks: " << eff << " % ["
       << nofCorMergedReferenceTofTracks << " / " << nofReferenceTofTracks
       << " ]" << endl;
  eff = 100 * nofWroMergedReferenceTofTracks;
  eff /= nofReferenceTofTracks;
  cout << "The incorrectly merged reference tracks: " << eff << " % ["
       << nofWroMergedReferenceTofTracks << " / " << nofReferenceTofTracks
       << " ]" << endl;
  eff = 100 * nofNonMergedReferenceTofTracks;
  eff /= nofReferenceTofTracks;
  cout << "The non merged reference tracks: " << eff << " % ["
       << nofNonMergedReferenceTofTracks << " / " << nofReferenceTofTracks
       << " ]" << endl;

  eff = 100 * nofCorMergedSemiTofTracks;
  eff /= nofSemiTofTracks;
  cout << "The correctly merged semi ToF tracks: " << eff << " % ["
       << nofCorMergedSemiTofTracks << " / " << nofSemiTofTracks << " ]"
       << endl;
  eff = 100 * nofWroMergedSemiTofTracks;
  eff /= nofSemiTofTracks;
  cout << "The incorrectly merged semi ToF tracks: " << eff << " % ["
       << nofWroMergedSemiTofTracks << " / " << nofSemiTofTracks << " ]"
       << endl;
  eff = 100 * nofNonMergedReferenceTofTracks;
  eff /= nofSemiTofTracks;
  cout << "The non merged semi ToF tracks: " << eff << " % ["
       << nofNonMergedReferenceTofTracks << " / " << nofSemiTofTracks << " ]"
       << endl;

  eff = 100 * nofMergedNonTofTracks;
  eff /= nofNonTofTracks;
  cout << "The merged non ToF tracks: " << eff << " % ["
       << nofMergedNonTofTracks << " / " << nofNonTofTracks << " ]" << endl;
  eff = 100 * nofNonMergedNonTofTracks;
  eff /= nofNonTofTracks;
  cout << "The non merged non ToF tracks: " << eff << " % ["
       << nofNonMergedNonTofTracks << " / " << nofNonTofTracks << " ]" << endl;
}

ClassImp(CbmGlobalTrackingQA)
