/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Claudia Hoehne and Semen Lebedev, Semen Lebedev, Semen Lebedev */

/**
* \file CbmRichRingTrackAssignClosestD.cxx
*
* \author Claudia Hoehne and Semen Lebedev
* \date 2007
**/

#include "CbmRichRingTrackAssignClosestD.h"

#include "CbmEvent.h"
#include "CbmGlobalTrack.h"
#include "CbmMCTrack.h"
#include "CbmRichRing.h"
#include "CbmTrdTrack.h"

#include "FairRootManager.h"
#include "FairTrackParam.h"
#include <Logger.h>

#include "TClonesArray.h"

#include <algorithm>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

CbmRichRingTrackAssignClosestD::CbmRichRingTrackAssignClosestD() {}

CbmRichRingTrackAssignClosestD::~CbmRichRingTrackAssignClosestD() {}

void CbmRichRingTrackAssignClosestD::Init()
{
  FairRootManager* manager = FairRootManager::Instance();
  if (nullptr == manager) LOG(fatal) << "CbmRichRingTrackAssignClosestD::Init(): FairRootManager is nullptr.";

  fGlobalTracks = (TClonesArray*) manager->GetObject("GlobalTrack");
  if (fGlobalTracks == nullptr) LOG(fatal) << "CbmRichRingTrackAssignClosestD::Init(): No GlobalTrack.";

  fTrdTracks = (TClonesArray*) manager->GetObject("TrdTrack");
  //if (NULL == fTrdTracks) {Fatal("CbmRichRingTrackAssignClosestD::Init", "No TrdTrack array!");}
}

void CbmRichRingTrackAssignClosestD::DoAssign(CbmEvent* event, TClonesArray* rings, TClonesArray* richProj)
{
  fEventNum++;
  if (fAlgorithmType == RingTrack) { DoAssignRingTrack(event, rings, richProj); }  // RingTrack algorithm

  else if (fAlgorithmType == TrackRing) {
    DoAssignTrackRing(event, rings, richProj);
  }  // TrackRing

  else if (fAlgorithmType == Combined) {
    DoAssignRingTrack(event, rings, richProj);
    DoAssignTrackRing(event, rings, richProj);
  }  // Combined

  Int_t nofTracks = event ? event->GetNofData(ECbmDataType::kRichTrackProjection) : richProj->GetEntriesFast();
  Int_t nofRings  = event ? event->GetNofData(ECbmDataType::kRichRing) : rings->GetEntriesFast();
  LOG(debug) << "CbmRichRingTrackAssignClosestD::DoAssign(): Event:" << fEventNum << " rings:" << nofRings
             << " ringsInTS:" << rings->GetEntriesFast() << " tracks:" << nofTracks
             << " tracksInTS:" << richProj->GetEntriesFast();
}

void CbmRichRingTrackAssignClosestD::DoAssignRingTrack(CbmEvent* event, TClonesArray* rings, TClonesArray* richProj)
{

  const Int_t nofTracks = event ? event->GetNofData(ECbmDataType::kRichTrackProjection) : richProj->GetEntriesFast();
  const Int_t nofRings  = event ? event->GetNofData(ECbmDataType::kRichRing) : rings->GetEntriesFast();
  if (nofTracks <= 0 || nofRings <= 0) return;

  vector<Int_t> trackIndex;
  vector<Double_t> trackDist;
  trackIndex.resize(nofRings);
  trackDist.resize(nofRings);
  for (UInt_t i = 0; i < trackIndex.size(); i++) {
    trackIndex[i] = -1;
    trackDist[i]  = 999.;
  }

  for (Int_t iIter = 0; iIter < 4; iIter++) {
    for (Int_t iR0 = 0; iR0 < nofRings; iR0++) {
      Int_t iR = event ? event->GetIndex(ECbmDataType::kRichRing, iR0) : iR0;

      if (trackIndex[iR] != -1) continue;
      CbmRichRing* ring = static_cast<CbmRichRing*>(rings->At(iR));
      if (ring == nullptr) continue;
      if (ring->GetNofHits() < fMinNofHitsInRing) continue;

      Double_t xRing  = ring->GetCenterX();
      Double_t yRing  = ring->GetCenterY();
      Double_t rMin   = 999.;
      Int_t iTrackMin = -1;

      for (Int_t iT0 = 0; iT0 < nofTracks; iT0++) {
        Int_t iT = event ? event->GetIndex(ECbmDataType::kRichTrackProjection, iT0) : iT0;

        vector<Int_t>::iterator it = find(trackIndex.begin(), trackIndex.end(), iT);
        if (it != trackIndex.end()) continue;

        FairTrackParam* track = static_cast<FairTrackParam*>(richProj->At(iT));
        Double_t xTrack       = track->GetX();
        Double_t yTrack       = track->GetY();
        // no projection onto the photodetector plane
        if (xTrack == 0 && yTrack == 0) continue;

        if (fUseTrd && fTrdTracks != nullptr && !IsTrdElectron(iT)) continue;

        Double_t dist = TMath::Sqrt((xRing - xTrack) * (xRing - xTrack) + (yRing - yTrack) * (yRing - yTrack));

        if (dist < rMin) {
          rMin      = dist;
          iTrackMin = iT;
        }
      }  // loop tracks
      trackIndex[iR] = iTrackMin;
      trackDist[iR]  = rMin;
    }  //loop rings

    for (UInt_t i1 = 0; i1 < trackIndex.size(); i1++) {
      for (UInt_t i2 = 0; i2 < trackIndex.size(); i2++) {
        if (i1 == i2) continue;
        if (trackIndex[i1] == trackIndex[i2] && trackIndex[i1] != -1) {
          if (trackDist[i1] >= trackDist[i2]) {
            trackDist[i1]  = 999.;
            trackIndex[i1] = -1;
          }
          else {
            trackDist[i2]  = 999.;
            trackIndex[i2] = -1;
          }
        }
      }
    }
  }  //iIter

  // fill global tracks
  for (UInt_t i = 0; i < trackIndex.size(); i++) {
    //		CbmRichRing* pRing = (CbmRichRing*)rings->At(i);
    // cout << "trackIndex[i]:" << trackIndex[i] << " trackDist[i]:" << trackDist[i] << " r:" << pRing->GetRadius() << " x:" << pRing->GetCenterX() << " y:" << pRing->GetCenterY()<< endl;
    if (trackIndex[i] == -1) continue;
    CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(trackIndex[i]);
    gTrack->SetRichRingIndex(i);
  }
}

void CbmRichRingTrackAssignClosestD::DoAssignTrackRing(CbmEvent* event, TClonesArray* rings, TClonesArray* richProj)
{
  const Int_t nofTracks = event ? event->GetNofData(ECbmDataType::kRichTrackProjection) : richProj->GetEntriesFast();
  const Int_t nofRings  = event ? event->GetNofData(ECbmDataType::kRichRing) : rings->GetEntriesFast();
  if (nofTracks <= 0 || nofRings <= 0) return;
  for (Int_t iT0 = 0; iT0 < nofTracks; iT0++) {
    Int_t iT               = event ? event->GetIndex(ECbmDataType::kRichTrackProjection, iT0) : iT0;
    CbmGlobalTrack* gTrack = static_cast<CbmGlobalTrack*>(fGlobalTracks->At(iT));
    // track already has rich segment
    if (gTrack == nullptr || gTrack->GetRichRingIndex() >= 0) continue;

    FairTrackParam* track = static_cast<FairTrackParam*>(richProj->At(iT));
    Double_t xTrack       = track->GetX();
    Double_t yTrack       = track->GetY();
    if (xTrack == 0 && yTrack == 0) continue;
    if (fUseTrd && fTrdTracks != nullptr && !IsTrdElectron(iT)) continue;
    Double_t rMin  = 999.;
    Int_t iRingMin = -1;
    for (Int_t iR0 = 0; iR0 < nofRings; iR0++) {
      Int_t iR          = event ? event->GetIndex(ECbmDataType::kRichRing, iR0) : iR0;
      CbmRichRing* ring = static_cast<CbmRichRing*>(rings->At(iR));
      if (ring == nullptr) continue;
      if (ring->GetNofHits() < fMinNofHitsInRing) continue;

      Double_t xRing = ring->GetCenterX();
      Double_t yRing = ring->GetCenterY();
      Double_t dist  = TMath::Sqrt((xRing - xTrack) * (xRing - xTrack) + (yRing - yTrack) * (yRing - yTrack));
      if (dist < rMin) {
        rMin     = dist;
        iRingMin = iR;
      }
    }  // loop rings
    if (iRingMin < 0) continue;
    //		CbmRichRing* pRing = (CbmRichRing*)rings->At(iRingMin);
    gTrack->SetRichRingIndex(iRingMin);
  }  //loop tracks
}

Bool_t CbmRichRingTrackAssignClosestD::IsTrdElectron(Int_t iTrack)
{
  CbmGlobalTrack* gTrack = static_cast<CbmGlobalTrack*>(fGlobalTracks->At(iTrack));
  Int_t trdIndex         = gTrack->GetTrdTrackIndex();
  if (trdIndex == -1) return false;
  CbmTrdTrack* trdTrack = static_cast<CbmTrdTrack*>(fTrdTracks->At(trdIndex));
  if (NULL == trdTrack) return false;

  if (trdTrack->GetPidANN() > fTrdAnnCut) { return true; }

  return false;
}
