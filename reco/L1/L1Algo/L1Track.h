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

  unsigned char NHits {0};  ///< Number of hits in track
  fscal TFirst[8] {kNaN};   ///< Track parameters on the first station
  fscal CFirst[28] {kNaN};  ///< Track parameter covariation matrix elements on the first station
  fscal TLast[8] {kNaN};    ///< Track parameters on the last station
  fscal CLast[28] {kNaN};   ///< Track parameter covariation matrix elements on the second station
  fscal Tpv[8] {kNaN};      ///< Track parameters in the primary vertex
  fscal Cpv[28] {kNaN};     ///< Track parameter covariation matrix elements in the primary vertex
  fscal chi2 {kNaN};        ///< Track fit chi-square value
  short int NDF {0};        ///< Track fit NDF value
};


#endif  // L1Track_H
