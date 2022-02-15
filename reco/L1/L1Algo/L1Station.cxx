/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#include "L1Station.h"

#include <FairLogger.h>

#include <iomanip>

// TODO: Improve log style (S.Zh.)
void L1Station::Print(int verbosity) const
{
  // TODO: Probably, it would be nice to use constexpr char indent (S.Zh.)
  LOG(info) << "\tL1Station object at " << this;
  LOG(info) << "\t\tStation type ID:             " << type;
  LOG(info) << "\t\tTime info ID:                " << timeInfo;
  LOG(info) << "\t\tz position:                  " << z[0];
  LOG(info) << "\t\tRmin:                        " << Rmin[0];
  LOG(info) << "\t\tRmax:                        " << Rmax[0];
  // TODO: Insert correct units (S.Zh.)
  LOG(info) << "\t\tThickness (X)    :           " << materialInfo.thick[0];
  LOG(info) << "\t\tRadiational length (X0):     " << materialInfo.RL[0];
  if (verbosity > 0) {
    LOG(info) << "\t\tX / X0:                      " << materialInfo.RadThick[0];
    LOG(info) << "\t\tlog(X / X0):                 " << materialInfo.logRadThick[0];
    LOG(info) << "\t\tField approximation coefficients:";
    LOG(info) << "      idx         CX         CY         CZ";
    for (int idx = 0; idx < L1Parameters::kMaxNFieldApproxCoefficients; ++idx) {
      LOG(info) << std::setw(9) << std::setfill(' ') << idx << ' ' << std::setw(10) << std::setfill(' ')
                << fieldSlice.cx[idx][0] << ' ' << std::setw(10) << std::setfill(' ') << fieldSlice.cy[idx][0] << ' '
                << std::setw(10) << std::setfill(' ') << fieldSlice.cz[idx][0];
    }
    LOG(info) << "\t\tStrips geometry:";
    LOG(info) << "\t\t\tFront:";
    LOG(info) << "\t\t\t\tcos(phi):            " << frontInfo.cos_phi[0];
    LOG(info) << "\t\t\t\tsin(phi):            " << frontInfo.sin_phi[0];
    LOG(info) << "\t\t\t\tsigma2:              " << frontInfo.sigma2[0];
    LOG(info) << "\t\t\tBack:";
    LOG(info) << "\t\t\t\tcos(phi):            " << backInfo.cos_phi[0];
    LOG(info) << "\t\t\t\tsin(phi):            " << backInfo.sin_phi[0];
    LOG(info) << "\t\t\t\tsigma2:              " << backInfo.sigma2[0];
    LOG(info) << "\t\t\tXY cov matrix:";
    LOG(info) << "\t\t\t\tC00:                 " << XYInfo.C00[0];
    LOG(info) << "\t\t\t\tC10:                 " << XYInfo.C10[0];
    LOG(info) << "\t\t\t\tC11:                 " << XYInfo.C11[0];
    LOG(info) << "\t\t\tX layer:";
    LOG(info) << "\t\t\t\tcos(phi):            " << xInfo.cos_phi[0];
    LOG(info) << "\t\t\t\tsin(phi):            " << xInfo.sin_phi[0];
    LOG(info) << "\t\t\t\tsigma2:              " << xInfo.sigma2[0];
    LOG(info) << "\t\t\tY layer:";
    LOG(info) << "\t\t\t\tcos(phi):            " << yInfo.cos_phi[0];
    LOG(info) << "\t\t\t\tsin(phi):            " << yInfo.sin_phi[0];
    LOG(info) << "\t\t\t\tsigma2:              " << yInfo.sigma2[0];
  }
}
