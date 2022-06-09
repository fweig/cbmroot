/* Copyright (C) 2007-2018 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer], Igor Kulakov */

#ifndef L1Station_h
#define L1Station_h 1

#include <string>

#include "L1Field.h"
#include "L1MaterialInfo.h"
#include "L1UMeasurementInfo.h"
#include "L1XYMeasurementInfo.h"
#include "L1Utils.h"

/// Structure L1Station
/// Contains a set of geometry parameters for a particular station
///
struct L1Station {
  int type {-1};
  int timeInfo {-1};     ///< flag: if time information can be used
  int fieldStatus {-1};  ///< flag: 1 - station is INSIDE the field, 0 - station is OUTSIDE the field
  fvec z {L1Utils::kNaN};           ///< z position of station     [cm]
  fvec Rmin {L1Utils::kNaN};        ///< min radius of the station [cm]
  fvec Rmax {L1Utils::kNaN};        ///< max radius of the station [cm]
  fvec dt   {L1Utils::kNaN};        ///< time resolution [ns]
  L1MaterialInfo materialInfo {};   ///< structure containing station thickness(X), rad. length (X0), X/X0 and log(X/X0) values
  L1FieldSlice fieldSlice {};      
  L1UMeasurementInfo frontInfo {};
  L1UMeasurementInfo backInfo {};
  L1UMeasurementInfo xInfo {};
  L1UMeasurementInfo yInfo {};
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

} _fvecalignment;

#endif  // L1Station_h
