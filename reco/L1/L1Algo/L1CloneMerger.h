/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer], Maksym Zyzak */

/// \file    L1CloneMerger.h
/// \authors S.Zharko <s.zharko@gsi.de> (interface), M.Zyzak (original algorithm)
/// \brief   A class wrapper over clones merger algorithm for the L1 track finder (declaration)
/// \since   22.07.2022

#ifndef L1CloneMerger_h
#define L1CloneMerger_h 1

#include "L1Def.h"
#include "L1Hit.h"  // For L1HitIndex_t
#include "L1Vector.h"

class L1Track;
class L1Algo;

/// Class implements a clones merger algorithm for the CA (L1) track finder
///
class L1CloneMerger {
public:
  /// Default constructor
  L1CloneMerger(const L1Algo& algo);

  /// Destructor
  ~L1CloneMerger();

  /// Copy constructor
  L1CloneMerger(const L1CloneMerger&) = delete;

  /// Move constructor
  L1CloneMerger(L1CloneMerger&&) = delete;

  /// Copy assignment operator
  L1CloneMerger& operator=(const L1CloneMerger&) = delete;

  /// Move assignment operator
  L1CloneMerger& operator=(L1CloneMerger&&) = delete;

  /// Registers

  /// Executes track clones merging algorithm and updates input containers
  /// \param  extTracks   Reference to the external container of reconstructed tracks
  /// \param  extRecoHits Reference to the external container of reconstructed hit indexes
  void Exec(L1Vector<L1Track>& extTracks, L1Vector<L1HitIndex_t>&);

private:
  // ***************
  // ** Functions **
  // ***************

  ///
  static void InvertCholesky(fvec a[15]);

  /// Multiplication of two symmetric matryces 5x5
  /// \param  C  Left matrix:
  ///      C[0]  C[1]  C[3]  C[6]  C[10]
  ///      C[1]  C[2]  C[4]  C[7]  C[11]
  /// C =  C[3]  C[4]  C[5]  C[8]  C[12]
  ///      C[6]  C[7]  C[8]  C[9]  C[13]
  ///      C[10] C[11] C[12] C[13] C[14]
  /// \param  V  Right matrix:
  ///      V[0]  V[1]  V[3]  V[6]  V[10]
  ///      V[1]  V[2]  V[4]  V[7]  V[11]
  /// V =  V[3]  V[4]  V[5]  V[8]  V[12]
  ///      V[6]  V[7]  V[8]  V[9]  V[13]
  ///      V[10] V[11] V[12] V[13] V[14]
  /// \param  K  Output: K = C * V
  static void MultiplySS(fvec const C[15], fvec const V[15], fvec K[5][5]);

  ///
  static void MultiplyMS(fvec const C[5][5], fvec const V[15], fvec K[15]);

  ///
  static void MultiplySR(fvec const C[15], fvec const r_in[5], fvec r_out[5]);

  ///
  static void FilterTracks(fvec const r[5], fvec const C[15], fvec const m[5], fvec const V[15], fvec R[5], fvec W[15],
                           fvec* chi2);


  // ***************
  // ** Variables **
  // ***************

  /// First station of a track
  L1Vector<unsigned short> fTrackFirstStation {"L1CloneMerger::fTrackFirstStation"};

  /// Last station of a track
  L1Vector<unsigned short> fTrackLastStation {"L1CloneMerger::fTrackLastStation"};

  /// Index of the first hit of a track
  L1Vector<L1HitIndex_t> fTrackFirstHit {"L1CloneMerger::fTrackFirstHit"};

  /// Index of the last hit of a track
  L1Vector<L1HitIndex_t> fTrackLastHit {"L1CloneMerger::fTrackLastHit"};

  /// Index (TODO:??) of a track that can be merge with the given track
  L1Vector<unsigned short> fTrackNeighbour {"L1CloneMerger::fTrackNeighbour"};

  /// Chi2 value of the track merging procedure
  L1Vector<float> fTrackChi2 {"L1CloneMerger::fTrackChi2"};

  /// Flag: is the given track already stored to the output
  L1Vector<char> fTrackIsStored {"L1CloneMerger::fTrackIsStored"};

  /// Flag: is the track a downstream neighbour of another track
  L1Vector<char> fTrackIsDownstreamNeighbour {"L1CloneMerger::fTrackIsDownstreamNeighbour"};

  L1Vector<L1Track> fTracksNew {"L1CAMergerClones::fTracksNew"};  ///< vector of tracks after the merge

  L1Vector<L1HitIndex_t> fRecoHitsNew {"L1CAMergerClones::fRecoHitsNew"};  ///< vector of track hits after the merge

  const L1Algo& frAlgo;  ///< Reference to the main track finder algorithm class
};

#endif  // L1CloneMerger_h
