/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer], Igor Kulakov, Sergei Zharko */

#ifndef L1Station_h
#define L1Station_h 1

#include <string>

#include "L1Field.h"
#include "L1MaterialInfo.h"
#include "L1NaN.h"
#include "L1UMeasurementInfo.h"
#include "L1Utils.h"
#include "L1XYMeasurementInfo.h"

/// Structure L1Station
/// Contains a set of geometry parameters for a particular station
///
class L1Station {
public:
  int type {L1NaN::SetNaN<decltype(type)>()};
  int timeInfo {L1NaN::SetNaN<decltype(timeInfo)>()};  ///< flag: if time information can be used
  int fieldStatus {L1NaN::SetNaN<decltype(
    fieldStatus)>()};  ///< flag: 1 - station is INSIDE the field, 0 - station is OUTSIDE the field (replace with enum)
  fvec z {L1NaN::SetNaN<decltype(z)>()};        ///< z position of station     [cm]
  fvec Rmin {L1NaN::SetNaN<decltype(Rmin)>()};  ///< min radius of the station [cm]
  fvec Rmax {L1NaN::SetNaN<decltype(Rmax)>()};  ///< max radius of the station [cm]
  fvec dt {L1NaN::SetNaN<decltype(dt)>()};      ///< time resolution [ns]
  /// structure containing station thickness(X), rad. length (X0), X/X0 and log(X/X0) values
  L1MaterialInfo materialInfo {};
  L1FieldSlice fieldSlice {};
  L1UMeasurementInfo frontInfo {};
  L1UMeasurementInfo backInfo {};
  L1UMeasurementInfo xInfo {};  ///< x axis in front,back coordinates
  L1UMeasurementInfo yInfo {};  ///< y axis in front,back coordinates
  L1XYMeasurementInfo XYInfo {};

  /// Prints object fields
  /// \param verbosity  Verbosity level of the output
  void Print(int verbosity = 0) const;

  /// String representation of class contents
  /// \param verbosityLevel  Verbosity level of the output
  /// \param indentLevel     Number of indent characters in the output
  std::string ToString(int verbosityLevel = 0, int indentLevel = 0) const;

  /// Verifies class invariant consistency
  /// \note Object is considered undefined in the creation time, so this function should be called after the object
  /// initialization
  void CheckConsistency() const;

  /// convert x,y to u,v
  std::pair<float, float> ConvXYtoUV(float x, float y) const
  {
    return std::make_pair(x * frontInfo.cos_phi[0] + y * frontInfo.sin_phi[0],
                          x * backInfo.cos_phi[0] + y * backInfo.sin_phi[0]);
  }

  /// convert u,v to x,y
  std::pair<float, float> ConvUVtoXY(float u, float v) const
  {
    return std::make_pair(u * xInfo.cos_phi[0] + v * xInfo.sin_phi[0], u * yInfo.cos_phi[0] + v * yInfo.sin_phi[0]);
  }

} _fvecalignment;

#endif  // L1Station_h
