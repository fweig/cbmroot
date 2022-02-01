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

class L1Track {
public:
  static constexpr float kNaN {std::numeric_limits<float>::signaling_NaN()};

  unsigned char NHits;
  unsigned char n;
  float Momentum {kNaN}, fTrackTime {kNaN};
  fscal TFirst[7] {kNaN}, CFirst[21] {kNaN}, TLast[7] {kNaN}, CLast[21] {kNaN}, Tpv[7] {kNaN}, Cpv[21] {kNaN},
    chi2 {kNaN};
  short int NDF;

  int FirstHitIndex, LastHitIndex;
  int index;
  int ista;


  static bool compareCand(const L1Track& a, const L1Track& b)
  {

    if (a.NHits != b.NHits) return (a.NHits > b.NHits);

    if (a.ista != b.ista) return (a.ista < b.ista);

    else
      return (a.chi2 < b.chi2);
  }


  static bool compare(const L1Track& a, const L1Track& b) { return (a.Cpv[20] <= b.Cpv[20]); }
};

// #include "cmath"
//   bool operator==(const L1Track &other) const {
//     cout<<int(NHits)<<" NHits"<<endl;
//      if ((other.NHits==NHits)&&(fabs(other.Momentum-Momentum)<1.e-6)) return true;
//      else return false;
//   }

#endif
