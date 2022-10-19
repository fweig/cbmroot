/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CbmTrackingDetectorInterfaceBase.h
/// \brief  Base abstract class for tracking detector interface to L1 (implementation of Checker)
/// \since  31.05.2022
/// \author S.Zharko (s.zharko@gsi.de)

#include "CbmTrackingDetectorInterfaceBase.h"

#include <Logger.h>

#include <iomanip>
#include <limits>
#include <set>
#include <vector>

// ---------------------------------------------------------------------------------------------------------------------
//
bool CbmTrackingDetectorInterfaceBase::Check() const
{
  bool res = true;
  LOG(info) << ToString();
  std::stringstream msg;
  msg << "Errors in the detector interface initialization for " << this->GetDetectorName() << ":\n";

  // Number of stations
  if (this->GetNtrackingStations() < 1) {
    msg << "\t- Number of stations is less then 1(" << this->GetNtrackingStations() << ")";
    res = false && res;
  }
  else {
    // Station individual parameters check
    for (int iSt = 0; iSt < this->GetNtrackingStations(); ++iSt) {
      std::string prefix = std::string("\t- Station ") + std::to_string(iSt) + " has ";
      // Position along Z-axis
      if (std::isnan(this->GetZ(iSt))) {
        msg << prefix << " NaN component along Z-axis (" << this->GetZ(iSt) << " cm)\n";
        res = false && res;
      }

      // Size along X-axis
      auto xMax = this->GetXmax(iSt);
      if (xMax < std::numeric_limits<double>::epsilon() || std::isnan(xMax)) {
        msg << prefix << " zero, negative or NaN X-size (" << xMax << " cm)\n";
        res = false && res;
      }

      // Size along Y-axis
      auto yMax = this->GetYmax(iSt);
      if (yMax < std::numeric_limits<double>::epsilon() || std::isnan(yMax)) {
        msg << prefix << " zero, negative or NaN Y-size (" << yMax << " cm)\n";
        res = false && res;
      }

      // Max station radius
      auto rMax = this->GetRmax(iSt);
      if (rMax < std::numeric_limits<double>::epsilon() || std::isnan(rMax)) {
        msg << prefix << " zero, negative or NaN outer radius (" << rMax << " cm)\n";
        res = false && res;
      }

      // Min station radius
      auto rMin = this->GetRmin(iSt);
      if (rMin < 0 || std::isnan(rMin)) {
        msg << prefix << " negative or NaN inner radius (" << rMin << " cm)\n";
        res = false && res;
      }

      // Front strips stereo angle
      auto angleF = this->GetStripsStereoAngleFront(iSt);
      if (std::isnan(angleF)) {
        msg << prefix << " NaN front strips stereo angle (" << angleF << " rad)\n";
        res = false && res;
      }

      // Back strips stereo angle
      auto angleB = this->GetStripsStereoAngleBack(iSt);
      if (std::isnan(angleF)) {
        msg << prefix << " NaN back strips stereo angle (" << angleB << " rad)\n";
        res = false && res;
      }

      // Front strips spatial resolution
      auto rmsF = this->GetStripsSpatialRmsFront(iSt);
      if (rmsF < std::numeric_limits<double>::epsilon() || std::isnan(rmsF)) {
        msg << prefix << " zero, negative or NaN front strips spatial resolution (" << rmsF << " cm)\n";
        res = false && res;
      }

      // Back strips spatial resolution
      auto rmsB = this->GetStripsSpatialRmsBack(iSt);
      if (rmsB < std::numeric_limits<double>::epsilon() || std::isnan(rmsB)) {
        msg << prefix << " zero, negative or NaN back strips spatial resolution (" << rmsB << " cm)\n";
        res = false && res;
      }

      // Time resolution
      auto timeRes = this->GetTimeResolution(iSt);
      if (timeRes < std::numeric_limits<double>::epsilon() || std::isnan(timeRes)) {
        msg << prefix << " zero, negative or NaN time resolution (" << timeRes << " cm)\n";
        res = false && res;
      }
    }

    // Position along beam axis
    std::vector<double> zPositions(this->GetNtrackingStations());
    for (int iSt = 0; iSt < this->GetNtrackingStations(); ++iSt) {
      zPositions[iSt] = this->GetZ(iSt);
    }
    std::set<double> zPositionSet(zPositions.begin(), zPositions.end());
    if (zPositions.size() != zPositionSet.size()) {
      msg << "\t- Some of stations have the same z position component:\n";
      for (size_t iSt = 0; iSt < zPositions.size(); ++iSt) {
        msg << "\t\tstation " << iSt << ", z = " << zPositions[iSt] << " cm\n";
      }
      res = false && res;
    }
  }

  if (!res) {
    LOG(error) << msg.str() << "\033[4mErrors above mean that CA tracking cannot be used for the current version of "
               << this->GetDetectorName() << " setup. Please, check if the " << this->GetDetectorName()
               << " setup parameters and the corresponding tracking detector interface are initialized properly\033[0m";
  }

  return res;
}

// ---------------------------------------------------------------------------------------------------------------------
//
std::string CbmTrackingDetectorInterfaceBase::ToString() const
{
  using std::setfill;
  using std::setw;
  std::stringstream table;
  table << "\nTracking detector interface: " << setw(5) << setfill(' ') << GetDetectorName() << '\n';
  table << setw(5) << setfill(' ') << "st.No" << ' ';
  table << setw(10) << setfill(' ') << "z[cm]" << ' ';
  table << setw(10) << setfill(' ') << "R_min[cm]" << ' ';
  table << setw(10) << setfill(' ') << "R_max[cm]" << ' ';
  table << setw(10) << setfill(' ') << "x_max[cm]" << ' ';
  table << setw(10) << setfill(' ') << "y_max[cm]" << ' ';
  table << setw(12) << setfill(' ') << "res.time[ns]" << ' ';
  table << setw(11) << setfill(' ') << "angleF[rad]" << ' ';
  table << setw(11) << setfill(' ') << "angleB[rad]" << ' ';
  table << setw(10) << setfill(' ') << "res.F [cm]" << ' ';
  table << setw(10) << setfill(' ') << "res.B [cm]" << ' ';
  table << setw(10) << setfill(' ') << "dz [cm]" << ' ';
  table << setw(10) << setfill(' ') << "RL [cm]" << '\n';
  for (int iSt = 0; iSt < GetNtrackingStations(); ++iSt) {
    table << setw(5) << setfill(' ') << iSt << ' ';
    table << setw(10) << setfill(' ') << GetZ(iSt) << ' ';
    table << setw(10) << setfill(' ') << GetRmin(iSt) << ' ';
    table << setw(10) << setfill(' ') << GetRmax(iSt) << ' ';
    table << setw(10) << setfill(' ') << GetXmax(iSt) << ' ';
    table << setw(10) << setfill(' ') << GetYmax(iSt) << ' ';
    table << setw(12) << setfill(' ') << GetTimeResolution(iSt) << ' ';
    table << setw(11) << setfill(' ') << GetStripsStereoAngleFront(iSt) << ' ';
    table << setw(11) << setfill(' ') << GetStripsStereoAngleBack(iSt) << ' ';
    table << setw(10) << setfill(' ') << GetStripsSpatialRmsFront(iSt) << ' ';
    table << setw(10) << setfill(' ') << GetStripsSpatialRmsBack(iSt) << ' ';
    table << setw(10) << setfill(' ') << GetThickness(iSt) << ' ';
    table << setw(10) << setfill(' ') << GetRadLength(iSt) << '\n';
  }
  return table.str();
}
