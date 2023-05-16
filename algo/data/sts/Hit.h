/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */

#ifndef CBM_ALGO_DATA_STS_HIT_H
#define CBM_ALGO_DATA_STS_HIT_H

#include "Prelude.h"

namespace cbm::algo::sts
{

  struct Hit {
    real fX, fY;      ///< X, Y positions of hit [cm]
    real fZ;          ///< Z position of hit [cm]
    u32 fTime;        ///< Hit time [ns]
    real fDx, fDy;    ///< X, Y errors [cm]
    real fDz;         ///< Z position error [cm]
    real fDxy;        ///< XY correlation
    real fTimeError;  ///< Error of hit time [ns]
    real fDu;         ///< Error of coordinate across front-side strips [cm]
    real fDv;         ///< Error of coordinate across back-side strips [cm]
  };

}  // namespace cbm::algo::sts

#endif  // CBM_ALGO_DATA_STS_HIT_H
