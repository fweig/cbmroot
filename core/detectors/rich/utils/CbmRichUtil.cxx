/* Copyright (C) 2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer]*/

#include "CbmRichUtil.h"

#include "CbmDigiManager.h"
#include "CbmGlobalTrack.h"
#include "CbmMCDataArray.h"
#include "CbmMatchRecoToMC.h"
#include "CbmRichDetectorData.h"
#include "CbmRichDigi.h"
#include "CbmRichDigiMapManager.h"
#include "CbmRichGeoManager.h"
#include "CbmRichHit.h"
#include "CbmRichRing.h"

#include "TClonesArray.h"

using namespace std;

map<pair<int, int>, int> CbmRichUtil::CreateNofHitsInRingMap(TClonesArray* richHits, CbmMCDataArray* richPoints,
                                                             CbmMCDataArray* mcTracks, CbmDigiManager* digiMan)
{
  map<pair<int, int>, int> nofHitsInRingMap;
  int nofRichHits = richHits->GetEntriesFast();
  for (int iHit = 0; iHit < nofRichHits; iHit++) {
    const CbmRichHit* hit = static_cast<CbmRichHit*>(richHits->At(iHit));
    if (hit == nullptr || hit->GetRefId() < 0) continue;
    const CbmRichDigi* digi = digiMan->Get<CbmRichDigi>(hit->GetRefId());
    if (digi == nullptr) continue;
    const CbmMatch* digiMatch = digiMan->GetMatch(ECbmModuleId::kRich, hit->GetRefId());
    if (digiMatch == nullptr) continue;
    int eventId = digiMatch->GetMatchedLink().GetEntry();

    vector<pair<int, int>> motherIds =
      CbmMatchRecoToMC::GetMcTrackMotherIdsForRichHit(digiMan, hit, richPoints, mcTracks, eventId);
    for (size_t i = 0; i < motherIds.size(); i++) {
      nofHitsInRingMap[motherIds[i]]++;
    }
  }
  return nofHitsInRingMap;
}

double CbmRichUtil::GetRingTrackDistance(Int_t globalTrackId) { return GetRingTrackDistanceImpl(globalTrackId)[0]; }

double CbmRichUtil::GetRingTrackDistanceX(Int_t globalTrackId) { return GetRingTrackDistanceImpl(globalTrackId)[1]; }

double CbmRichUtil::GetRingTrackDistanceY(Int_t globalTrackId) { return GetRingTrackDistanceImpl(globalTrackId)[2]; }

vector<Double_t> CbmRichUtil::GetRingTrackDistanceImpl(Int_t globalTrackId)
{
  vector<Double_t> errorVec = {999., 999., 999.};
  FairRootManager* ioman    = FairRootManager::Instance();
  if (ioman == NULL) return errorVec;
  // Do we really need static here, depends on ioman->GetObject() method
  static TClonesArray* globalTracks    = NULL;
  static TClonesArray* richRings       = NULL;
  static TClonesArray* richProjections = NULL;

  if (globalTracks == NULL || richRings == NULL || richProjections == NULL) {
    //cout << "globalTracks == NULL || richRings == NULL || richProjections == NULL" << endl;
    globalTracks    = (TClonesArray*) ioman->GetObject("GlobalTrack");
    richRings       = (TClonesArray*) ioman->GetObject("RichRing");
    richProjections = (TClonesArray*) ioman->GetObject("RichProjection");
  }
  else {
    //cout << "globalTracks, richRings, richProjections NOT NULL" << endl;
  }

  if (globalTracks == NULL || richRings == NULL || richProjections == NULL) {
    LOG(error) << "CbmRichUtil::GetRingTrackDistance globalTracks, "
                  "richRings, richProjections NOT INITIALIZED"
               << endl;
    return errorVec;
  }

  const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(globalTracks->At(globalTrackId));
  if (globalTrack == NULL) return errorVec;

  Int_t stsId = globalTrack->GetStsTrackIndex();
  if (stsId < 0) return errorVec;

  const FairTrackParam* pTrack = static_cast<const FairTrackParam*>(richProjections->At(stsId));
  if (pTrack == NULL) return errorVec;

  if (pTrack->GetX() == 0 && pTrack->GetY() == 0) return errorVec;

  Int_t richId = globalTrack->GetRichRingIndex();
  if (richId < 0) return errorVec;

  const CbmRichRing* richRing = static_cast<const CbmRichRing*>(richRings->At(richId));
  if (richRing == NULL) return errorVec;

  //		Double_t xRing = richRing->GetCenterX();
  //		Double_t yRing = richRing->GetCenterY();
  Double_t dx = richRing->GetCenterX() - pTrack->GetX();
  Double_t dy = richRing->GetCenterY() - pTrack->GetY();

  Double_t dist = TMath::Sqrt(dx * dx + dy * dy);

  vector<Double_t> v = {dist, dx, dy};
  return v;
}

vector<double> CbmRichUtil::GetPmtHistXbins() { return GetPmtHistBins(true); }

vector<double> CbmRichUtil::GetPmtHistYbins() { return GetPmtHistBins(false); }

vector<double> CbmRichUtil::GetPmtHistBins(bool isX)
{
  vector<double> initVec;
  vector<int> pmts = CbmRichDigiMapManager::GetInstance().GetPmtIds();
  for (int pmtId : pmts) {
    CbmRichPmtData* pmtData = CbmRichDigiMapManager::GetInstance().GetPmtDataById(pmtId);
    TVector3 inPos(pmtData->fX, pmtData->fY, pmtData->fZ);
    TVector3 outPos;
    CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);
    if (isX) {
      initVec.push_back(outPos.X() - 0.5 * pmtData->fWidth);
      initVec.push_back(outPos.X() + 0.5 * pmtData->fWidth);
    }
    else {
      initVec.push_back(outPos.Y() - 0.5 * pmtData->fHeight);
      initVec.push_back(outPos.Y() + 0.5 * pmtData->fHeight);
    }
  }
  sort(initVec.begin(), initVec.end());

  vector<double> uniVec;
  for (size_t i = 0; i < initVec.size(); i++) {
    if (i == 0) uniVec.push_back(initVec[i]);
    if (initVec[i] - uniVec[uniVec.size() - 1] > 0.000001) uniVec.push_back(initVec[i]);
  }
  return uniVec;
}
