/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CaToolsMCData.cxx
/// \brief  Data structure for internal tracking MC-information (implementation)
/// \since  23.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#include "CaToolsMCData.h"

#include <iomanip>
#include <sstream>
#include <utility>  // for std::move

using namespace ca::tools;

// ---------------------------------------------------------------------------------------------------------------------
//
MCData::MCData() {}

// ---------------------------------------------------------------------------------------------------------------------
//
MCData::MCData(const MCData& other)
  : fvPoints(other.fvPoints)
  , fvTracks(other.fvTracks)
  , fmPointsLinksMap(other.fmPointsLinksMap)
  , fmTracksLinksMap(other.fmPointsLinksMap)
{
}

// ---------------------------------------------------------------------------------------------------------------------
//
MCData::MCData(MCData&& other) noexcept { this->Swap(other); }

// ---------------------------------------------------------------------------------------------------------------------
//
MCData& MCData::operator=(const MCData& other)
{
  if (this != &other) { MCData(other).Swap(*this); }
  return *this;
}

// ---------------------------------------------------------------------------------------------------------------------
//
MCData& MCData::operator=(MCData&& other) noexcept
{
  if (this != &other) {
    MCData tmp(std::move(other));
    this->Swap(tmp);
  }
  return *this;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void MCData::Swap(MCData& other) noexcept
{
  std::swap(fvPoints, other.fvPoints);
  std::swap(fvTracks, other.fvTracks);
  std::swap(fmPointsLinksMap, other.fmPointsLinksMap);
  std::swap(fmTracksLinksMap, other.fmTracksLinksMap);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void MCData::AddPoint(const MCPoint& point, int index, int event, int file)
{
  fmPointsLinksMap[LinkKey(index, event, file)] = static_cast<int>(fvPoints.size());
  fvPoints.push_back(point);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void MCData::AddTrack(const CbmL1MCTrack& track, int index, int event, int file)
{
  fmTracksLinksMap[LinkKey(index, event, file)] = static_cast<int>(fvTracks.size());
  fvTracks.push_back(track);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void MCData::Clear()
{
  fvPoints.clear();
  fvTracks.clear();
  fmPointsLinksMap.clear();
  fmTracksLinksMap.clear();
}

// ---------------------------------------------------------------------------------------------------------------------
//
std::string MCData::ToString(int verbose) const
{
  if (verbose < 1) { return std::string(); }
  std::stringstream msg;
  msg << "MCData: " << fvTracks.size() << " tracks, " << fvPoints.size() << " points";
  if (verbose > 1) {
    using std::setfill;
    using std::setw;
    const int kNofTracksToPrint = 5;
    msg << "\n Track sample (first 5 tracks):\n";
    msg << setw(10) << setfill(' ') << "ID" << ' ' << setw(10) << setfill(' ') << "motherID" << ' ' << setw(10)
        << setfill(' ') << "pdg" << ' ' << setw(10) << setfill(' ') << "zVTX [cm]" << ' ' << setw(10) << setfill(' ')
        << "px [GeV/c]" << ' ' << setw(10) << setfill(' ') << "py [GeV/c]" << ' ' << setw(10) << setfill(' ')
        << "pz [GeV/c]" << '\n';
    for (int i = 0; i < kNofTracksToPrint; ++i) {
      msg << setw(10) << setfill(' ') << fvTracks[i].ID << ' ' << setw(10) << setfill(' ') << fvTracks[i].mother_ID
          << ' ' << setw(10) << setfill(' ') << fvTracks[i].pdg << ' ' << setw(10) << setfill(' ') << fvTracks[i].z
          << ' ' << setw(10) << setfill(' ') << fvTracks[i].px << ' ' << setw(10) << setfill(' ') << fvTracks[i].py
          << ' ' << setw(10) << setfill(' ') << fvTracks[i].pz << '\n';
    }
  }
  return msg.str();
}
