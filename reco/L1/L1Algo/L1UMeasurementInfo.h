/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer], Igor Kulakov, Sergei Zharko */

#ifndef L1UMeasurementInfo_h
#define L1UMeasurementInfo_h 1

#include <string>

#include "L1Def.h"
#include "L1NaN.h"
#include "L1SimdSerializer.h"
#include "L1Undef.h"
#include "L1Utils.h"

class L1UMeasurementInfo {

public:
  fvec cos_phi = undef::kFvc;
  fvec sin_phi = undef::kFvc;

  /// String representation of class contents
  /// \param indentLevel      number of indent characters in the output
  std::string ToString(int indentLevel = 0) const;

  /// Check consistency
  void CheckConsistency() const;

  /// Checks, if the fields are NaN
  bool IsNaN() const { return isnan(cos_phi).isNotEmpty() || isnan(sin_phi).isNotEmpty(); }

  /// Serialization function
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int)
  {
    ar& cos_phi;
    ar& sin_phi;
  }
} _fvecalignment;


#endif
