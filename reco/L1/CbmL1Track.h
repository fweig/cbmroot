/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ivan Kisel,  Sergey Gorbunov, Denis Bertini [committer], Igor Kulakov, Maksym Zyzak */

/*
 *====================================================================
 *
 *  CBM Level 1 Reconstruction 
 *  
 *  Authors: I.Kisel,  S.Gorbunov
 *
 *  e-mail : ikisel@kip.uni-heidelberg.de 
 *
 *====================================================================
 *
 *  L1 track class
 *
 *====================================================================
 */


#ifndef CbmL1Track_H
#define CbmL1Track_H

#include "CbmL1Constants.h"
#include "CbmL1MCTrack.h"
#include "CbmL1TrackPar.h"

#include <map>
#include <vector>
using std::map;
using std::vector;

class CbmL1MCTrack;

class CbmL1Track : public CbmL1TrackPar {
public:
  CbmL1Track() : Hits(), nStations(0), index(0), fTrackTime(0.), hitMap(), mcTracks(), maxPurity(-1) {}

  int GetNOfHits() { return Hits.size(); }

  void AddMCTrack(CbmL1MCTrack* mcTr) { mcTracks.push_back(mcTr); }
  vector<CbmL1MCTrack*>& GetMCTracks() { return mcTracks; }
  CbmL1MCTrack* GetMCTrack() { return mcTracks[0]; }
  int GetNMCTracks() { return mcTracks.size(); }
  bool IsGhost() { return (maxPurity < CbmL1Constants::MinPurity); }

  void SetMaxPurity(double maxPurity_) { maxPurity = maxPurity_; }
  double GetMaxPurity() { return maxPurity; }

  static bool compareChi2(const CbmL1Track& a, const CbmL1Track& b) { return (a.chi2 < b.chi2); }
  static bool comparePChi2(const CbmL1Track* a, const CbmL1Track* b) { return (a->chi2 < b->chi2); }


  double Tpv[L1TrackPar::kNparTr], Cpv[L1TrackPar::kNparCov];


  double TLast[L1TrackPar::kNparTr], CLast[L1TrackPar::kNparCov];
  vector<int> Hits;
  int nStations;
  int index;

  double fTrackTime;

  map<int, int> hitMap;  // how many hits from each mcTrack belong to current recoTrack
private:
  // next members filled and used in Performance
  vector<CbmL1MCTrack*> mcTracks;  // array of assosiated mc Tracks. Should be only one.
  double maxPurity;                // maximum persent of hits, which belong to one mcTrack.
};

#endif
