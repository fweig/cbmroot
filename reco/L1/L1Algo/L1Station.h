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
  L1Station()
    : type(0)
    , timeInfo(0)
    , z(0)
    , Rmin(0)
    , Rmax(0)
    , Sy(0)
    ,  /// z-position, min & max station radius, field integral
    materialInfo()
    , fieldSlice()
    , frontInfo()
    , backInfo()
    , xInfo()
    , yInfo()
    , XYInfo()
  {
  }

  // TODO: test this constructors
  //L1Station(const L1Station &) = default;
  //L1Station & operator=(const L1Station &) = default;
  //L1Station(L1Station &&) = default;
  //L1Station & operator=(L1Station &&) = default;

  int type;
  int timeInfo;
  fvec z;
  fvec Rmin;
  fvec Rmax;
  fvec Sy;
  L1MaterialInfo materialInfo;
  L1FieldSlice fieldSlice;
  L1UMeasurementInfo frontInfo;
  L1UMeasurementInfo backInfo;
  L1UMeasurementInfo xInfo;
  L1UMeasurementInfo yInfo;
  L1XYMeasurementInfo XYInfo;

} _fvecalignment;

#endif
