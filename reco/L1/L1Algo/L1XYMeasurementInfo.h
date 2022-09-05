/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer], Sergei Zharko */

#ifndef L1XYMeasurementInfo_h
#define L1XYMeasurementInfo_h 1

#include <string>

#include "L1Def.h"
#include "L1NaN.h"
#include "L1SimdSerializer.h"

class L1XYMeasurementInfo {
public:
  fvec C00 {L1NaN::SetNaN<decltype(C00)>()};
  fvec C10 {L1NaN::SetNaN<decltype(C10)>()};
  fvec C11 {L1NaN::SetNaN<decltype(C11)>()};

  /// Consistency checker
  void CheckConsistency() const;

  /// String representation of class contents
  /// \param indentLevel      number of indent characters in the output
  std::string ToString(int indentLevel = 0) const;

  /// Checks, if the fields are NaN
  bool IsNaN() const { return L1NaN::IsNaN(C00) || L1NaN::IsNaN(C10) || L1NaN::IsNaN(C11); }

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
