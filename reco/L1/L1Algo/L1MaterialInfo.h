/* Copyright (C) 2007-2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov, Sergey Gorbunov [committer], Andrey Lebedev */

#ifndef L1MaterialInfo_h
#define L1MaterialInfo_h

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "L1Def.h"

class L1MaterialInfo {
public:
  fvec thick {0};
  fvec RL {0};
  fvec RadThick {0};
  fvec logRadThick {0};

  /// String representation of class contents
  /// \param indentLevel    number of indent characters in the output
  std::string ToString(int indentLevel = 0) const;
} _fvecalignment;


// TODO: Probably one should refactor this class:
//       1) Make MBins and RMax private
//       2) Replace std::vector<std::vector<float>> with std::vector<float> (using recalculation of (i,j) into one idx)
//       (S.Zharko)
class L1Material {
public:
  L1Material() : table(0), NBins(0), RMax(0.), iD(0.) {};

  std::vector<std::vector<float>> table;

  // static const int NBins = 100; // TODO file?
  // static const float RMax = 60.f;
  // static const float iD = 0.5*NBins/60.f;//RMax!;

  void SetBins(int n, float r)
  {
    NBins = n;
    RMax  = r;
    iD    = 0.5 * NBins / RMax;
  }

  float GetRadThick(float x, float y)
  {
    x     = (x < RMax && x >= -RMax) ? x : 0;
    y     = (y < RMax && y >= -RMax) ? y : 0;
    int i = static_cast<int>((x + RMax) * iD);
    int j = static_cast<int>((y + RMax) * iD);
    i     = (i < NBins && i >= 0) ? i : NBins / 2;
    j     = (j < NBins && j >= 0) ? j : NBins / 2;
    return table[i][j];
  }


  fvec GetRadThick(fvec x, fvec y)
  {
    fvec r;
    for (int i = 0; i < fvecLen; i++)
      r[i] = GetRadThick(x[i], y[i]);
    return r;
  }

  int NBins;
  float RMax;
  float iD;

} _fvecalignment;

#endif
