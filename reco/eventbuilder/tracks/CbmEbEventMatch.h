/* Copyright (C) 2017 IKF-UFra, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina , Maksym Zyzak, Valentina Akishina [committer] */

/** @file CbmEbEventMatch.h
 ** @author Valentina Akishina <v.akishina@gsi.de>, Maksym Zyzak <m.zyzak@gsi.de>
 ** @date 14.03.2017
 **/

#ifndef CbmEbEventMatch_H
#define CbmEbEventMatch_H

#include <map>
#include <vector>

using std::map;
using std::vector;

class CbmEbEventMatch {
public:
  CbmEbEventMatch() : fMCEvent(), fNTracks(0), fTracks() {};
  bool IsGhost() { return fMCEvent.size() == 0; }
  void Clear()
  {
    fMCEvent.clear();
    fNTracks = 0;
  }
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
