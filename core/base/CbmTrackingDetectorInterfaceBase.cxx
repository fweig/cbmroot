/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CbmTrackingDetectorInterfaceBase.h
/// \brief  Base abstract class for tracking detector interface to L1 (implementation of Checker)
/// \since  31.05.2022
/// \author S.Zharko (s.zharko@gsi.de)

#include "CbmTrackingDetectorInterfaceBase.h"

#include <Logger.h>

#include <limits>
#include <set>
#include <vector>

bool CbmTrackingDetectorInterfaceBase::Check() const
{
  bool res           = true;
  std::string prefix = std::string("Detector interface for ") + this->GetDetectorName() + ": ";

  // Number of stations
  if (this->GetNtrackingStations() < 1) {
    LOG(error) << prefix << "Number of stations is less then 1(" << this->GetNtrackingStations() << ")";
    res = false && res;
  }
  else {
    // Position along beam axis
    std::vector<double> zPositions(this->GetNtrackingStations());
    for (int iSt = 0; iSt < this->GetNtrackingStations(); ++iSt) {
      zPositions[iSt] = this->GetZ(iSt);
    }
    std::set<double> zPositionSet(zPositions.begin(), zPositions.end());
    if (zPositions.size() != zPositionSet.size()) {
      LOG(error) << prefix << "Some of stations have the same z position component:";
      for (size_t iSt = 0; iSt < zPositions.size(); ++iSt) {
        LOG(error) << "\tstation " << iSt << ", z = " << zPositions[iSt] << " cm";
      }
      res = false && res;
    }

    // Station sizes check
    for (int iSt = 0; iSt < this->GetNtrackingStations(); ++iSt) {
      // Size along X-axis
      if (this->GetXmax(iSt) < std::numeric_limits<double>::epsilon()) {
        LOG(error) << prefix << "Station " << iSt << " has zero or negative X-size (" << this->GetXmax(iSt) << " cm)";
        res = false && res;
      }

      // Size along Y-axis
      if (this->GetYmax(iSt) < std::numeric_limits<double>::epsilon()) {
        LOG(error) << prefix << "Station " << iSt << " has zero or negative Y-size (" << this->GetYmax(iSt) << " cm)";
        res = false && res;
      }

      // Max station radius
      if (this->GetRmax(iSt) < std::numeric_limits<double>::epsilon()) {
        LOG(error) << prefix << "Station " << iSt << " has zero or negative outer radius (" << this->GetRmax(iSt)
                   << " cm)";
        res = false && res;
      }

      // Min station radius
      if (this->GetRmin(iSt) < 0) {
        LOG(error) << prefix << "Station " << iSt << " has negative inner radius (" << this->GetRmin(iSt) << " cm)";
        res = false && res;
      }

      // Front strips spatial resolution
      if (this->GetStripsSpatialRmsFront(iSt) < std::numeric_limits<double>::epsilon()) {
        LOG(error) << prefix << "Station " << iSt << " has zero or negative front strips spatial resolution ("
                   << this->GetStripsSpatialRmsFront(iSt) << " cm)";
        res = false && res;
      }

      // Back strips spatial resolution
      if (this->GetStripsSpatialRmsBack(iSt) < std::numeric_limits<double>::epsilon()) {
        LOG(error) << prefix << "Station " << iSt << " has zero or negative back strips spatial resolution ("
                   << this->GetStripsSpatialRmsBack(iSt) << " cm)";
        res = false && res;
      }
    }
  }
  if (!res) {
    LOG(error) << "\033[4mErrors above mean that CA tracking cannot be used for the current version of "
               << this->GetDetectorName() << " setup. Please, check if the " << this->GetDetectorName()
               << " setup parameters and the corresponding tracking detector interface are initialized properly\033[0m";
  }
  return res;
}
