/* Copyright (C) 2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak [committer] */

#ifndef L1EventMatch_H
#define L1EventMatch_H

#include <map>
#include <vector>

#include "L1Event.h"
#include "L1MCEvent.h"

using std::map;
using std::vector;

class L1EventMatch {
public:
  L1EventMatch() : fMCEvent(), fNTracks(0), fTracks() {};
  bool IsGhost() { return fMCEvent.size() == 0; }
  void Clear();
  void AddTrack(int mcEventId) { fMCEvent[mcEventId]++; }
  void SetNEventTracks(int ntracks) { fNTracks = ntracks; }
  void SetTracks(vector<int> tracks) { fTracks = tracks; }
  vector<int>& GetTracksId() { return fTracks; }

  int NRecoTracks() const { return fNTracks; }
  int NMCEvents() const { return fMCEvent.size(); }

  map<int, int>& GetMCEvents() { return fMCEvent; }

private:
  map<int, int> fMCEvent;
  int fNTracks;
  vector<int> fTracks;
};

#endif
