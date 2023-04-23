/* Copyright (C) 2006-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ivan Kisel,  Sergey Gorbunov, Denis Bertini [committer], Igor Kulakov, Maksym Zyzak, Sergei Zharko */

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

#include "TMath.h"

#include <iterator>
#include <map>
#include <string>
#include <vector>
using std::map;
using std::vector;

class CbmL1MCTrack;

class CbmL1Track : public CbmL1TrackPar {
public:
  CbmL1Track() : Hits(), nStations(0), index(0), fTrackTime(0.), hitMap(), mcTracks(), maxPurity(-1) {}

  /// Adds pointer to MC track (TODO: remove this and related methods)
  void AddMCTrack(CbmL1MCTrack* mcTr) { mcTracks.push_back(mcTr); }

  /// Adds an index of MC track index
  void AddMCTrackIndex(int iMT) { fvMcTrackIndexes.push_back_no_warning(iMT); }

  /// Clears the contents of matched MC track indexes (and pointers)
  void ClearMatchedMCTracks()
  {
    mcTracks.clear();
    fvMcTrackIndexes.clear();
  }

  int GetNMCTracks() { return mcTracks.size(); }

  bool IsGhost() const { return (maxPurity < CbmL1Constants::MinPurity); }

  /// Gets charge
  int GetCharge() const { return (T[4] > 0) ? 1 : -1; }

  /// Gets Chi-square of track fit model
  double GetChiSq() const { return chi2; }

  /// @brief Gets pseudo-rapidity
  double GetEta() const { return -std::log(std::tan(GetTheta() * 0.5)); }

  /// @brief Gets first hit index
  int GetFirstHitIndex() const { return Hits.front(); }

  /// @brief Gets last hit index
  int GetLastHitIndex() const { return Hits.back(); }

  /// Gets hit indexes
  const auto& GetHitIndexes() const { return Hits; }

  /// Gets NDF of track fit model
  int GetNDF() const { return NDF; }

  /// Gets number of MC tracks
  // TODO: Remove this method
  int GetNMCTracks() const { return mcTracks.size(); }

  /// Gets a reference to MC track indexes
  const auto& GetMCTrackIndexes() const { return fvMcTrackIndexes; }

  /// Gets max purity
  double GetMaxPurity() const { return maxPurity; }

  /// Gets container of pointers to MC tracks
  // TODO: this function is to be replaced with GetMCTrackIndexes()
  vector<CbmL1MCTrack*>& GetMCTracks() { return mcTracks; }

  /// Gets pointer of the associated MC track
  CbmL1MCTrack* GetMCTrack() { return mcTracks[0]; }

  /// @brief Gets index of matched MC track
  /// @note  If two tracks are matched (should not happen, if purity > 50%), the first
  ///
  int GetMatchedMCTrackIndex() const { return fvMcTrackIndexes.size() ? fvMcTrackIndexes[0] : -1; }

  /// Gets number of hits of the track
  int GetNofHits() const { return Hits.size(); }

  /// Gets number of associated MC tracks
  int GetNofMCTracks() const { return fvMcTrackIndexes.size(); }

  /// Gets number of stations
  int GetNofStations() const { return nStations; }

  /// Gets absolute value of momentum divided by the charge (absolute value) of particle [GeV/ec]
  // NOTE: 1.e-10 precision is used in the old performance, but in FairTrackParam the 1.e-4 is used
  double GetP() const { return fabs(T[4]) > 1.e-10 ? 1. / fabs(T[4]) : 1.e-10; }

  /// Gets transverse momentum
  double GetPt() const { return sqrt(GetP() * GetP() - GetPz() * GetPz()); }

  /// Gets azimuthal angle of the track
  double GetPhi() const { return TMath::ATan2(-GetTy(), -GetTx()); }

  /// Gets probability of track fit model
  double GetProb() const { return TMath::Prob(chi2, NDF); }

  /// Gets x-component of momentum divided by the charge (absolute value) of particle [GeV/ec]
  double GetPx() const { return GetPz() * GetTx(); }

  /// Gets y-component of momentum divided by the charge (absolute value) of particle [GeV/ec]
  double GetPy() const { return GetPz() * GetTy(); }

  /// Gets z-component of momentum divided by the charge (absolute value) of particle [GeV/ec]
  double GetPz() const { return std::sqrt(GetP() * GetP() / (GetTx() * GetTx() + GetTy() * GetTy() + 1)); }

  /// Gets track polar angle
  double GetTheta() const { return std::acos(1. / std::sqrt(GetTx() * GetTx() + GetTy() * GetTy() + 1)); }

  /// Gets track slope along x-axis
  double GetTx() const { return T[2]; }

  /// Gets track slope along y-axis
  double GetTy() const { return T[3]; }

  /// Sets max purity
  /// NOTE: max purity is calculated as a ratio of max number of hits left by an actual track and the
  ///       total number of hits in the track
  void SetMaxPurity(double maxPurity_) { maxPurity = maxPurity_; }


  static bool compareChi2(const CbmL1Track& a, const CbmL1Track& b) { return (a.chi2 < b.chi2); }

  static bool comparePChi2(const CbmL1Track* a, const CbmL1Track* b) { return (a->chi2 < b->chi2); }

  /// @brief Provides a string representation of object
  /// @param verbose  Verbosity level
  /// @param header   If true, header will be printed
  std::string ToString(int verbose = 10, bool header = false) const;

  double Tpv[L1TrackPar::kNparTr];   ///< Track parameters at primary vertex
  double Cpv[L1TrackPar::kNparCov];  ///< Track covariance matrix at primary vertex

  double TLast[L1TrackPar::kNparTr];   ///< Track parameters in the end of the track
  double CLast[L1TrackPar::kNparCov];  ///< Track covariance matrix at the end of the track

  vector<int> Hits;  ///< Indexes of hits of this track
  int nStations;     ///< Number of stations with hits of this track
  int index;         ///< Index of this track (TODO: it seems to be not initialized)

  double fTrackTime;  ///< Time of the track [ns] ???

  map<int, int> hitMap;  // N hits (second) from each mcTrack (first is a MC track ID) belong to current recoTrack
  // FIXME: SZh 14.12.2022:  map => unordered_map


private:
  // next members filled and used in Performance
  vector<CbmL1MCTrack*> mcTracks;  // array of assosiated recoTracks. Should be only one.

  L1Vector<int> fvMcTrackIndexes = {"CbmL1Track::fvMcTrackIndexes"};  // global indexes of MC tracks
  // NOTE: mcTracks should be replaced with fvMcTrackIndexes

  double maxPurity;  ///< Maximum persent of hits, which belong to one mcTrack.
};

#endif
