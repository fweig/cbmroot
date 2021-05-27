/** @file L1XYMeasurementInfo.h
  * @copyright Copyright (C) 2007-2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Sergey Gorbunov [orginator] **/

#ifndef L1XYMeasurementInfo_h
#define L1XYMeasurementInfo_h 1

#include "../CbmL1Def.h"


class L1XYMeasurementInfo {

public:
  L1XYMeasurementInfo() : C00(0), C10(0), C11(0) {}
  fvec C00, C10, C11;

} _fvecalignment;


#endif
