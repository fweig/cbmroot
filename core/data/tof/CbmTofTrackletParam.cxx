/* Copyright (C) 2005-2020 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Norbert Herrmann [committer], Florian Uhlig */

// -------------------------------------------------------------------------
// -----                     FairTrackParam source file                 -----
// -----                  Created 27/01/05  by V. Friese               -----
// -------------------------------------------------------------------------

#include "CbmTofTrackletParam.h"

#include <Logger.h>  // for Logger, LOG

double CbmTofTrackletParam::GetZr(double R) const
{
  double P   = (fTx + fTy);
  double Q   = 0.5 * (fX * fX + fY * fY - R * R);
  double Arg = P * P * 0.25 - Q;
  LOG(info) << " GetZr " << R << ", P " << P << ", Q " << Q << ", Arg " << Arg;

  if (Arg > 0.) {
    double z = -P * 0.5 + std::sqrt(Arg);
    LOG(info) << " GetZr " << R << ", P " << P << ", Q " << Q << ", Arg " << Arg << ", z " << z;
    return z;
  }
  return 0.;
}


ClassImp(CbmTofTrackletParam)
