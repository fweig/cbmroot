/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina */

#ifndef L1Event_H
#define L1Event_H

#include "CbmL1Track.h"

#include <vector>

#include "KFPTrackVector.h"
#include "KFParticleTopoReconstructor.h"
#include "KFTopoPerformance.h"

using std::vector;

class L1Event {
public:
  L1Event() : fTopoReconstructor(), fKFPTrack(), fStsHit(0) {};

  void Clear();
  void TopoReconstructor();

  const KFPTrackVector& getTracks() const { return fKFPTrack; }
  KFPTrackVector& getTracks() { return fKFPTrack; }
  const vector<int>& getHits() const { return fStsHit; }
  vector<int>& getHits() { return fStsHit; }
  const KFParticleTopoReconstructor* getTopoReconstructor() const { return &fTopoReconstructor; }

  L1Event(const L1Event& event)
    : fTopoReconstructor(event.fTopoReconstructor)
    , fKFPTrack(event.fKFPTrack)
    , fStsHit(event.fStsHit)
  {
    fKFPTrack = event.fKFPTrack;
    fStsHit   = event.fStsHit;
  }

  L1Event& operator=(const L1Event& event)
  {
    fTopoReconstructor = event.fTopoReconstructor;
    fKFPTrack          = event.fKFPTrack;
    fStsHit            = event.fStsHit;

    return *this;
  }

private:
  KFParticleTopoReconstructor fTopoReconstructor;

  KFPTrackVector fKFPTrack;
  vector<int> fStsHit;
};

#endif
