/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#include "L1SearchWindow.h"

#include <cassert>
#include <iomanip>
#include <sstream>

// ---------------------------------------------------------------------------------------------------------------------
//
L1SearchWindow::L1SearchWindow(int stationID, int trackGrID) : fStationID(stationID), fTrackGroupID(trackGrID)
{
  assert(stationID > -1);
  assert(trackGrID > -1);

  // Case for constant windows (TEMPORARY: we should add selection of different windows):
  static_assert(kNpars == 1);
}

// TODO: SZh 08.11.2022: Probably, we should have the assertions in the InitManager and remove them from here, since
//                       this class is supposed to be used inside the algorithm core
// ---------------------------------------------------------------------------------------------------------------------
//
void L1SearchWindow::SetParamDxMaxVsX0(int id, float val)
{
  assert(id > -1 && id < kNpars);
  fvParams[kDxMaxVsX0 * kNpars + id] = val;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1SearchWindow::SetParamDxMinVsX0(int id, float val)
{
  assert(id > -1 && id < kNpars);
  fvParams[kDxMinVsX0 * kNpars + id] = val;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1SearchWindow::SetParamDxMaxVsY0(int id, float val)
{
  assert(id > -1 && id < kNpars);
  fvParams[kDxMaxVsY0 * kNpars + id] = val;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1SearchWindow::SetParamDxMinVsY0(int id, float val)
{
  assert(id > -1 && id < kNpars);
  fvParams[kDxMinVsY0 * kNpars + id] = val;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1SearchWindow::SetParamDyMaxVsX0(int id, float val)
{
  assert(id > -1 && id < kNpars);
  fvParams[kDyMaxVsX0 * kNpars + id] = val;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1SearchWindow::SetParamDyMinVsX0(int id, float val)
{
  assert(id > -1 && id < kNpars);
  fvParams[kDyMinVsX0 * kNpars + id] = val;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1SearchWindow::SetParamDyMaxVsY0(int id, float val)
{
  assert(id > -1 && id < kNpars);
  fvParams[kDyMaxVsY0 * kNpars + id] = val;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1SearchWindow::SetParamDyMinVsY0(int id, float val)
{
  assert(id > -1 && id < kNpars);
  fvParams[kDyMinVsY0 * kNpars + id] = val;
}

// ---------------------------------------------------------------------------------------------------------------------
//
std::string L1SearchWindow::ToString() const
{
  std::stringstream msg;
  msg << "----- CA hits search window: \n";
  msg << "\tstation ID:      " << fStationID << '\n';
  msg << "\ttracks group ID: " << fTrackGroupID << '\n';
  msg << "\tparameters:\n";
  msg << "\t\t" << std::setw(6) << std::setfill(' ') << "No." << ' ';
  msg << std::setw(12) << std::setfill(' ') << "dx_max(x0)" << ' ';
  msg << std::setw(12) << std::setfill(' ') << "dx_min(x0)" << ' ';
  msg << std::setw(12) << std::setfill(' ') << "dx_max(y0)" << ' ';
  msg << std::setw(12) << std::setfill(' ') << "dx_min(y0)" << ' ';
  msg << std::setw(12) << std::setfill(' ') << "dy_max(x0)" << ' ';
  msg << std::setw(12) << std::setfill(' ') << "dy_min(x0)" << ' ';
  msg << std::setw(12) << std::setfill(' ') << "dy_max(y0)" << ' ';
  msg << std::setw(12) << std::setfill(' ') << "dy_min(y0)" << '\n';
  for (int iPar = 0; iPar < kNpars; ++iPar) {
    msg << "\t\t" << std::setw(6) << std::setfill(' ') << iPar << ' ';
    for (int iDep = 0; iDep < kNdeps; ++iDep) {
      msg << std::setw(12) << std::setfill(' ') << fvParams[iDep * kNpars + iPar] << ' ';
    }
  }
  return msg.str();
}
