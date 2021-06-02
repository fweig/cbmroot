/** @file L1UMeasurementInfo.h
  * @copyright Copyright (C) 2007-2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Sergey Gorbunov [committer], Igor Kulakov **/

#ifndef L1UMeasurementInfo_h
#define L1UMeasurementInfo_h 1

#include "../CbmL1Def.h"


class L1UMeasurementInfo {

public:
  L1UMeasurementInfo() : cos_phi(0), sin_phi(0), sigma2(0) {}

  fvec cos_phi, sin_phi, sigma2;

} _fvecalignment;


#endif
