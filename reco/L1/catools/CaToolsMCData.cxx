/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CaToolsMCData.cxx
/// \brief  Data structure for internal tracking MC-information (implementation)
/// \since  23.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#include "CaToolsMCData.h"

#include "CbmL1Hit.h"

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
  , fvPointIndexOfHit(other.fvPointIndexOfHit)
  , fmPointLinkMap(other.fmPointLinkMap)
  , fmTrackLinkMap(other.fmTrackLinkMap)
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
  std::swap(fvPointIndexOfHit, other.fvPointIndexOfHit);
  std::swap(fmPointLinkMap, other.fmPointLinkMap);
  std::swap(fmTrackLinkMap, other.fmTrackLinkMap);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void MCData::AddPoint(const MCPoint& point)
{
  fmPointLinkMap[point.GetLinkKey()] = point.GetId();
  fvPoints.push_back(point);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void MCData::AddTrack(const MCTrack& track)
{
  fmTrackLinkMap[track.GetLinkKey()] = track.GetId();
  fvTracks.push_back(track);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void MCData::Clear()
{
  fvPoints.clear();
  fvTracks.clear();
  fvPointIndexOfHit.clear();
  fmPointLinkMap.clear();
  fmTrackLinkMap.clear();
}

// ---------------------------------------------------------------------------------------------------------------------
//
void MCData::InitTrackInfo(const L1Vector<CbmL1Hit>& vHits)
{
  for (auto& aTrk : fvTracks) {
    // Assign hits to tracks
    aTrk.ClearHitIndexes();
    auto& vHitIds = aTrk.GetHitIndexes();
    for (int iP : aTrk.GetPointIndexes()) {
      const auto& point = fvPoints[iP];
      for (int iH : point.GetHitIndexes()) {
        if (std::find(vHitIds.begin(), vHitIds.end(), iH) == vHitIds.end()) { aTrk.AddHitIndex(iH); }
      }
    }
    // Initialize arrangements of points and hits within stations
    aTrk.InitPointsInfo(fvPoints);
    aTrk.InitHitsInfo(vHits);
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
std::string MCData::ToString(int verbose) const
{
  if (verbose < 1) { return std::string(); }
  std::stringstream msg;
  msg << "MCData: " << fvTracks.size() << " tracks, " << fvPoints.size() << " points, ";
  msg << fmTrackLinkMap.size() << " track links, " << fmPointLinkMap.size() << " point links";
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
      msg << setw(10) << setfill(' ') << fvTracks[i].GetId() << ' ';
      msg << setw(10) << setfill(' ') << fvTracks[i].GetMotherId() << ' ';
      msg << setw(10) << setfill(' ') << fvTracks[i].GetPdgCode() << ' ';
      msg << setw(10) << setfill(' ') << fvTracks[i].GetStartZ() << ' ';
      msg << setw(10) << setfill(' ') << fvTracks[i].GetPx() << ' ';
      msg << setw(10) << setfill(' ') << fvTracks[i].GetPy() << ' ';
      msg << setw(10) << setfill(' ') << fvTracks[i].GetPz() << '\n';
    }
  }
  return msg.str();
}
