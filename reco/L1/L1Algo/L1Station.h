/* Copyright (C) 2007-2018 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer], Igor Kulakov */

#ifndef L1Station_h
#define L1Station_h 1

#include "L1Field.h"
#include "L1MaterialInfo.h"
#include "L1UMeasurementInfo.h"
#include "L1XYMeasurementInfo.h"

class L1Station {
public:
  // TODO: test this constructors
  //L1Station(const L1Station &) = default;
  //L1Station & operator=(const L1Station &) = default;
  //L1Station(L1Station &&) = default;
  //L1Station & operator=(L1Station &&) = default;

  int type {0};
  int timeInfo {0};
  fvec z {0};
  fvec Rmin {0};
  fvec Rmax {0};
  fvec Sy {0};
  fvec dt {0};
  L1MaterialInfo materialInfo {};
  L1FieldSlice fieldSlice {};
  L1UMeasurementInfo frontInfo {};
  L1UMeasurementInfo backInfo {};
  L1UMeasurementInfo xInfo {};
  L1UMeasurementInfo yInfo {};
  L1XYMeasurementInfo XYInfo {};

  /// Prints object fields
  void Print() const;

} _fvecalignment;

#endif  // L1Station_h
