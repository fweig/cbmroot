/* Copyright (C) 2007-2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer], Igor Kulakov */

#ifndef L1UMeasurementInfo_h
#define L1UMeasurementInfo_h 1

#include <string>

#include "L1Def.h"
#include "L1Utils.h"

class L1UMeasurementInfo {

public:
  fvec cos_phi {0.f};
  fvec sin_phi {0.f};
  fvec sigma2 {0.f};

  /// String representation of class contents
  /// \param indentLevel      number of indent characters in the output
  std::string ToString(int indentLevel = 0) const;

  /// Check consistency
  void CheckConsistency() const;

} _fvecalignment;


#endif
