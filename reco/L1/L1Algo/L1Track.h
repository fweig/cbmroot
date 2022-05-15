/* Copyright (C) 2010-2017 Frankfurt Institute for Advanced Studies, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ivan Kisel,  Sergey Gorbunov, Maksym Zyzak, Igor Kulakov [committer] */

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

#ifndef L1Track_H
#define L1Track_H

#include <limits>

/// Class representing a track in the L1 tracking algorithm
/// Track parameters vector: {x, y, tx, ty, q/p, z, t}
/// Covariation matrix: C[20] (C55) corresponds to the time variance
///
class L1Track {
public:
  static constexpr float kNaN {std::numeric_limits<float>::signaling_NaN()};

  unsigned char NHits;      ///> Number of hits in track
  unsigned char n;          ///> TODO: ??
  float Momentum {kNaN};    ///> TODO: ??
  float fTrackTime {kNaN};  ///> Track time
  fscal TFirst[7] {kNaN};   ///> Track parameters on the first station
  fscal CFirst[21] {kNaN};  ///> Track parameter covariation matrix elemenst on the first station
  fscal TLast[7] {kNaN};    ///> Track parameters on the last station
  fscal CLast[21] {kNaN};   ///> Track parameter covatiation matrix elements on the second station
  fscal Tpv[7] {kNaN};      ///> Track parameters in the primary vertex
  fscal Cpv[21] {kNaN};     ///> Track parameter covariation matrix elements in the primary vertex
  fscal chi2 {kNaN};        ///> Track fit chi-square value
  short int NDF;            ///> Track fit NDF value

  // TODO: shouldn't it be the L1HitIndex_t type instead of int? (S.Zharko)
  int FirstHitIndex;  ///> Track first hit index in the hits vector
  int LastHitIndex;   ///> Track last hit index in the hits vector
  int index;          ///> Index of the track
  int ista;           ///> TODO: ??

  /// Provides comparison of two L1Track objects
  /// If two tracks have different number of hits, the smallest track has the largest number of hits.
  /// If two tracks have the same numbers of hits and ...
  static bool compareCand(const L1Track& a, const L1Track& b)
  {
    if (a.NHits != b.NHits) { return (a.NHits > b.NHits); }
    if (a.ista != b.ista) { return (a.ista < b.ista); }
    else {
      return (a.chi2 < b.chi2);
    }
  }

  /// Provides comparison for two tracks by the time variance
  static bool compare(const L1Track& a, const L1Track& b) { return (a.Cpv[20] <= b.Cpv[20]); }
};

// #include "cmath"
//   bool operator==(const L1Track &other) const {
//     cout<<int(NHits)<<" NHits"<<endl;
//      if ((other.NHits==NHits)&&(fabs(other.Momentum-Momentum)<1.e-6)) return true;
//      else return false;
//   }

#endif  // L1Track_H
