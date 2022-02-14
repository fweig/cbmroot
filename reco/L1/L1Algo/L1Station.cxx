/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#include <iomanip>
#include <FairLogger.h>
#include "L1Station.h"

// TODO: Improve log style (S.Zh.)
void L1Station::Print() const
{
  LOG(info) << "==== L1Station object at " << this;
  LOG(info) << "- L1Station fields:";
  LOG(info) << "--- Station type ID:             " << type;
  LOG(info) << "--- z position:                  " << z[0];
  LOG(info) << "--- Rmin:                        " << Rmin[0];
  LOG(info) << "--- Rmax:                        " << Rmax[0];
  LOG(info) << "--- Thickness (X), cm:           " << materialInfo.thick[0];
  LOG(info) << "--- Radiational length (X0), cm: " << materialInfo.RL[0];
  LOG(info) << "--- X / X0:                      " << materialInfo.RadThick[0];
  LOG(info) << "--- log(X / X0):                 " << materialInfo.logRadThick[0];
  LOG(info) << "--- Field approximation coefficients:";
  LOG(info) << "      idx         CX         CY         CZ";
  for (int idx = 0; idx < 21; ++idx) {
    LOG(info) << std::setw(9) << std::setfill(' ') << idx << ' ' << std::setw(10) << std::setfill(' ')
              << fieldSlice.cx[idx][0] << ' ' << std::setw(10) << std::setfill(' ') << fieldSlice.cy[idx][0] << ' '
              << std::setw(10) << std::setfill(' ') << fieldSlice.cz[idx][0];
  }
  LOG(info) << "--- Strips geometry:";
  LOG(info) << "----- Front:";
  LOG(info) << "------- cos(phi):            " << frontInfo.cos_phi[0];
  LOG(info) << "------- sin(phi):            " << frontInfo.sin_phi[0];
  LOG(info) << "------- sigma2:              " << frontInfo.sigma2[0];
  LOG(info) << "----- Back:";
  LOG(info) << "------- cos(phi):            " << backInfo.cos_phi[0];
  LOG(info) << "------- sin(phi):            " << backInfo.sin_phi[0];
  LOG(info) << "------- sigma2:              " << backInfo.sigma2[0];
  LOG(info) << "----- XY cov matrix:";
  LOG(info) << "------- C00:                 " << XYInfo.C00[0];
  LOG(info) << "------- C10:                 " << XYInfo.C10[0];
  LOG(info) << "------- C11:                 " << XYInfo.C11[0];
  LOG(info) << "----- X layer:";
  LOG(info) << "------- cos(phi):            " << xInfo.cos_phi[0];
  LOG(info) << "------- sin(phi):            " << xInfo.sin_phi[0];
  LOG(info) << "------- sigma2:              " << xInfo.sigma2[0];
  LOG(info) << "----- Y layer:";
  LOG(info) << "------- cos(phi):            " << yInfo.cos_phi[0];
  LOG(info) << "------- sin(phi):            " << yInfo.sin_phi[0];
  LOG(info) << "------- sigma2:              " << yInfo.sigma2[0];
}
