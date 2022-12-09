/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer], Sergei Zharko */

#ifndef L1XYMeasurementInfo_h
#define L1XYMeasurementInfo_h 1

#include <string>

#include "L1Def.h"
#include "L1SimdSerializer.h"
#include "L1Undef.h"

class L1XYMeasurementInfo {
public:
  cbm::algo::ca::fvec C00 = undef::kFvc;
  cbm::algo::ca::fvec C10 = undef::kFvc;
  cbm::algo::ca::fvec C11 = undef::kFvc;

  /// Consistency checker
  void CheckConsistency() const;

  /// String representation of class contents
  /// \param indentLevel      number of indent characters in the output
  std::string ToString(int indentLevel = 0) const;

  /// Checks, if the fields are NaN
  bool IsNaN() const { return undef::IsUndefined(C00) || undef::IsUndefined(C10) || undef::IsUndefined(C11); }

  /// Serialization function
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int)
  {
    ar& C00;
    ar& C10;
    ar& C11;
  }
} _fvecalignment;


#endif
