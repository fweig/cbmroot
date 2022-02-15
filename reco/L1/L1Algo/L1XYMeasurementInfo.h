/* Copyright (C) 2007-2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

#ifndef L1XYMeasurementInfo_h
#define L1XYMeasurementInfo_h 1

#include "L1Def.h"


class L1XYMeasurementInfo {

public:
  fvec C00 {0};
  fvec C10 {0};
  fvec C11 {0};
  /// String representation of class contents
  /// \param indentLevel      number of indent characters in the output
  std::string ToString(int indentLevel = 0) const;

} _fvecalignment;


#endif
