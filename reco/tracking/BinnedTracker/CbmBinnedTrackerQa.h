/* Copyright (C) 2017-2019 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Timur Ablyazimov [committer] */

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef CBMBINNEDTRACKERQA_H
#define CBMBINNEDTRACKERQA_H

#include "CbmMCDataArray.h"
#include "CbmMuchTrack.h"
#include "CbmStsTrack.h"
#include "CbmTofHit.h"
#include "CbmTrdHit.h"
#include "CbmTrdPoint.h"
#include "CbmTrdTrack.h"

#include "FairTask.h"

#include "TClonesArray.h"

#include <functional>

#include "Settings.h"

class CbmBinnedTrackerQa : public FairTask {
public:
  CbmBinnedTrackerQa();
  CbmBinnedTrackerQa(const CbmBinnedTrackerQa&) = delete;
  CbmBinnedTrackerQa& operator=(const CbmBinnedTrackerQa&) = delete;
  InitStatus Init();         // Overridden from FairTask
  void Exec(Option_t* opt);  // Overridden from FairTask
  void Finish();             // Overridden from FairTask
  void SetParContainers();

  void SetPrimaryParticle(EPrimaryParticleId v)
  {
    fPrimaryParticleIds.clear();
    fPrimaryParticleIds.push_back(v);
  }

  void AddPrimaryParticle(EPrimaryParticleId v) { fPrimaryParticleIds.push_back(v); }

private:
  void HandleSts(Int_t stsTrackIndex, std::map<Int_t, std::set<Int_t>>& mcTrackIds, std::set<Int_t>* globalTrackMCRefs,
                 Int_t* globalTracksHitInds);
  void HandleMuch(Int_t muchTrackIndex, std::map<Int_t, std::set<Int_t>>& mcTrackIds,
                  std::set<Int_t>* globalTrackMCRefs, Int_t* globalTracksHitInds);
  void HandleTrd(Int_t trdTrackIndex, std::map<Int_t, std::set<Int_t>>& mcTrackIds, std::set<Int_t>* globalTrackMCRefs,
                 Int_t* globalTracksHitInds);
  void HandleTof(Int_t globalTrackIndex, Int_t tofHitIndex, std::map<Int_t, std::set<Int_t>>& mcTrackIds,
                 std::set<Int_t>* globalTrackMCRefs, Int_t* globalTracksHitInds);
  void IterateTrdHits(std::function<void(const CbmTrdHit*, const CbmTrdPoint*)> handleData);

private:
  std::list<EPrimaryParticleId> fPrimaryParticleIds;
  bool fIsOnlyPrimary;
  CbmBinnedSettings* fSettings;
  TClonesArray* fGlobalTracks;
  TClonesArray* fStsTracks;
  TClonesArray* fMuchTracks;
  TClonesArray* fTrdTracks;
  TClonesArray* fStsHits;
  TClonesArray* fMuchHits;
  TClonesArray* fTrdHits;
  TClonesArray* fTofHits;
  TClonesArray* fStsClusters;
  TClonesArray* fMuchClusters;
  TClonesArray* fTrdClusters;

  TClonesArray* fTrdDigiMatches;
  TClonesArray* fTofHitDigiMatches;
  TClonesArray* fTofDigiPointMatches;

  TClonesArray* fStsDigis;
  TClonesArray* fStsDigiMatches;
  TClonesArray* fMuchDigis;
  TClonesArray* fMuchDigiMatches;
  TClonesArray* fTrdDigis;
  TClonesArray* fTofDigis;

  CbmMCDataArray* fMCTracks;
  CbmMCDataArray* fStsPoints;
  CbmMCDataArray* fMuchPoints;
  CbmMCDataArray* fTrdPoints;
  CbmMCDataArray* fTofPoints;
  ClassDef(CbmBinnedTrackerQa, 1)
};

#endif /* CBMBINNEDTRACKERQA_H */
