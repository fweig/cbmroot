/* Copyright (C) 2010-2017 Frankfurt Institute for Advanced Studies, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ivan Kisel,  Sergey Gorbunov, Igor Kulakov [committer], Maksym Zyzak */

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
 *  L1 Monte Carlo information
 *
 *====================================================================
 */

#ifndef CbmL1MCTrack_H
#define CbmL1MCTrack_H

#include "CbmL1MCPoint.h"

#include "TLorentzVector.h"
#include "TVector3.h"

#include <iostream>
#include <string>

#include "L1Vector.h"

class CbmL1Track;

class CbmL1MCTrack {
public:
  CbmL1MCTrack() = default;

  CbmL1MCTrack(int _ID) : ID(_ID) {};

  CbmL1MCTrack(double mass, double q, TVector3 vr, TLorentzVector vp, int ID, int mother_ID, int pdg,
               unsigned int procID);
  //   CbmL1MCTrack(TmpMCPoints &mcPoint, TVector3 vr, TLorentzVector vp, int ID, int mother_ID);

  bool IsPrimary() const { return mother_ID < 0; }
  bool IsReconstructable() const { return isReconstructable; }
  bool IsAdditional() const { return isAdditional; }
  int NStations() const { return nStations; }
  int NHitContStations() const { return nHitContStations; }
  int NMCStations() const { return nMCStations; }
  int NMCContStations() const { return nMCContStations; }

  void Init();

  void AddRecoTrack(CbmL1Track* rTr) { rTracks.push_back_no_warning(rTr); }
  void AddRecoTrackIndex(int iT) { rTrackIndexes.push_back_no_warning(iT); }
  L1Vector<CbmL1Track*>& GetRecoTracks() { return rTracks; }
  int GetNClones() const { return rTracks.size() - 1; }
  bool IsReconstructed() const { return rTracks.size(); }

  void AddTouchTrack(CbmL1Track* tTr) { tTracks.push_back_no_warning(tTr); }
  void AddTouchTrackIndex(int iT) { tTrackIndexes.push_back_no_warning(iT); }
  bool IsDisturbed() const { return tTracks.size(); }

  void SetIsReconstructable(bool v) { isReconstructable = v; }
  const auto& GetRecoTrackIndexes() const { return rTrackIndexes; }
  const auto& GetTouchTrackIndexes() const { return tTrackIndexes; }

  friend class CbmL1;

  double pt() { return sqrt(px * px + py * py); }

  /// @brief String representation of the contents
  std::string ToString(int verbose = 10, bool header = false) const;

private:
  void CalculateMCCont();
  void CountHitStations();
  void CalculateMaxNStaMC();
  void CalculateIsReconstructable();

public:
  double mass             = 0.;
  double q                = 0.;
  double p                = 0.;
  double x                = 0.;
  double y                = 0.;
  double z                = 0.;
  double px               = 0.;
  double py               = 0.;
  double pz               = 0.;
  double time             = 0.;
  int ID                  = -1;
  int iFile               = -1;
  int iEvent              = -1;
  int mother_ID           = -1;
  int chainID             = -1;  // ID of the first particle in the decay chain
  int pdg                 = -1;
  unsigned int process_ID = (unsigned int) -1;
  bool isSignal {0};
  L1Vector<int> Points {"CbmL1MCTrack::Points"};  // indices of pints in CbmL1::fvMCPoints
  L1Vector<int> Hits {"CbmL1MCTrack::Hits"};      // indices of hits in algo->vHits or L1::vHits

private:
  int nMCContStations  = 0;  // number of consecutive stations with mcPoints
  int nHitContStations = 0;  // number of consecutive stations with hits
  int maxNStaMC        = 0;  // max number of mcPoints on station
  int maxNSensorMC     = 0;  // max number of mcPoints with same z
  int maxNStaHits      = 0;  // max number of hits on station

  int nStations   = 0;  // number of stations with hits
  int nMCStations = 0;  // number of stations with MCPoints

  bool isReconstructable = false;
  bool isAdditional      = false;  // is not reconstructable, but stil interesting

  // next members filled and used in Performance
  L1Vector<CbmL1Track*> rTracks {"CbmL1MCTrack::rTracks"};  // array of associated recoTracks
  L1Vector<CbmL1Track*> tTracks {"CbmL1MCTrack::tTracks"};  // array of recoTracks
                                                            // which aren't associated with this mcTrack,
                                                            // but use some hits from it.

  // NOTE: SZh 14.12.2022: on the replacement from rTracks and tTracks
  L1Vector<int> rTrackIndexes = {"CbmL1MCTrack::rTrackIndexes"};  // array of associated recoTrack indexes
  L1Vector<int> tTrackIndexes = {"CbmL1MCTrack::tTrackIndexes"};  // .....
};


#endif
