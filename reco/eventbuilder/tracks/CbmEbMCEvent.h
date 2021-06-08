/* Copyright (C) 2017-2019 IKF-UFra, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina , Maksym Zyzak, Valentina Akishina [committer] */

/** @file CbmEbMCEvent.h
 ** @author Valentina Akishina <v.akishina@gsi.de>, Maksym Zyzak <m.zyzak@gsi.de>
 ** @date 14.03.2017
 **/

#ifndef CbmEbMCEvent_H
#define CbmEbMCEvent_H

#include <vector>

using std::vector;

class CbmEbMCEvent {
public:
  CbmEbMCEvent() : fMCEventId(-1), fRecoEvents(), fMCTrackId(0), fIsReconstructable(0), fRecoTrackId(0) {};

  int IsReconstructed()
  {
    if (fRecoEvents.size() > 0) return 1;
    if (fRecoEvents.size() == 0) return 0;
    return 0;
  }
  int NClones()
  {
    if (fRecoEvents.size() > 1) { return fRecoEvents.size() - 1; }
    else {
      return 0;
    }
  }
  void SetReconstructable(bool isReconstructable) { fIsReconstructable = isReconstructable; }
  void SetId(int mcEvId) { fMCEventId = mcEvId; }

  vector<int>& GetMCTrackIds() { return fMCTrackId; }
  int NMCTracks() const { return fMCTrackId.size(); }
  int& GetId() { return fMCEventId; }
  bool IsReconstructable() const { return fIsReconstructable; }
  vector<int>& GetRecoTrackIds() { return fRecoTrackId; }

  void AddRecoEvent(int iEvent) { fRecoEvents.push_back(iEvent); }

  const vector<int>& GetRecoEvents() const { return fRecoEvents; }

private:
  int fMCEventId;
  vector<int> fRecoEvents;
  vector<int> fMCTrackId;
  bool fIsReconstructable;
  vector<int> fRecoTrackId;
};

#endif
