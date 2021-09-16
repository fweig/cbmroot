/* Copyright (C) 2017-2020 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Timur Ablyazimov [committer], Volker Friese */

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef CBMGLOBALTRACKINGQA_H
#define CBMGLOBALTRACKINGQA_H

#include "CbmGlobalTrackingDefs.h"
#include "CbmMCDataArray.h"
#include "CbmMatch.h"
#include "CbmTofDigi.h"
#include "CbmTofHit.h"

#include "FairTask.h"

#include "TClonesArray.h"

#include <list>
#include <set>
#include <vector>

class CbmDigiManager;


class CbmGlobalTrackingQa : public FairTask {
public:
  struct PointData;

  struct TrackData {
    bool hasSts;
    scaltype x;
    scaltype y;
    scaltype z;
    scaltype t;
    scaltype tx;
    scaltype ty;
    std::list<PointData*> tofPoints;
    bool use;
    int evN;
    int ind;
    std::set<const CbmTofHit*> tofHits;
    bool used;
    TrackData* parent;
    std::list<TrackData*> offsprings;

    TrackData()
      : hasSts(false)
      , x(0)
      , y(0)
      , z(0)
      , t(0)
      , tx(0)
      , ty(0)
      , tofPoints()
      , use(false)
      , evN(0)
      , ind(0)
      , tofHits()
      , used(false)
      , parent(nullptr)
      , offsprings()
    {
    }
    TrackData(const TrackData&) = default;
    TrackData& operator=(const TrackData&) = delete;
  };

  struct PointData {
    scaltype x;
    scaltype y;
    scaltype z;
    scaltype t;
    TrackData* track;
    int evN;
    int ind;
  };

public:
  CbmGlobalTrackingQa();

  CbmGlobalTrackingQa(const CbmGlobalTrackingQa&) = delete;
  CbmGlobalTrackingQa& operator=(const CbmGlobalTrackingQa&) = delete;

  InitStatus Init();         // Overridden from FairTask
  void Exec(Option_t* opt);  // Overridden from FairTask
  void Finish();             // Overridden from FairTask

  void SetNofEvents(Int_t v) { fNofEvents = v; }

private:
  bool CheckMatch(const TrackData* stsMCTrack, Int_t tofHitInd, bool deepSearch = false) const;
  bool SemiTofTrack(const TrackData* mcTrack) const;

private:
  CbmDigiManager* fDigiMan     = nullptr;
  TClonesArray* fTofHits       = nullptr;
  TClonesArray* fStsTracks     = nullptr;
  TClonesArray* fGlobalTracks  = nullptr;
  TClonesArray* fTofHitMatches = nullptr;
  TClonesArray* fStsHits       = nullptr;
  TClonesArray* fStsClusters   = nullptr;
  CbmMCDataArray* fMCTracks    = nullptr;
  CbmMCDataArray* fStsMCPoints = nullptr;
  CbmMCDataArray* fTrdMCPoints = nullptr;
  CbmMCDataArray* fTofMCPoints = nullptr;
  std::vector<std::vector<TrackData>> fTracks {};
  std::vector<std::vector<PointData>> fTofPoints {};
  std::vector<std::vector<PointData>> fStsPoints {};
  Int_t fNofEvents = 0;
  ClassDef(CbmGlobalTrackingQa, 1)
};

#endif /* CBMGLOBALTRACKINGQA_H */
