/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CaToolsMCData.h
/// \brief  Data structure for internal tracking MC-information (header)
/// \since  23.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#ifndef CaToolsMCData_h
#define CaToolsMCData_h 1

#include <numeric>
#include <string>

#include "CaToolsLinkKey.h"
#include "CaToolsMCPoint.h"
#include "CaToolsMCTrack.h"
#include "L1Constants.h"
#include "L1EArray.h"
#include "L1Vector.h"

enum class L1DetectorID;

namespace ca::tools
{
  /// This class represents a package for tracking-related data
  class MCData {
  public:
    // *********************************
    // ** Constructors and destructor **
    // *********************************

    /// Default constructor
    MCData();

    /// Destructor
    ~MCData() = default;

    /// Copy constructor
    MCData(const MCData& other);

    /// Move constructor
    MCData(MCData&& other) noexcept;

    /// Copy assignment operator
    MCData& operator=(const MCData& other);

    /// Move assignment operator
    MCData& operator=(MCData&& other) noexcept;

    /// Swap method
    void Swap(MCData& other) noexcept;

    /// Adds an MC point to points container and a corresponding link key to the point index map
    /// \param  point  MC point object
    void AddPoint(const MCPoint& point);

    /// Adds an MC track to tracks container and a corresponding link key to the link index map
    /// \param  track  MC track object
    void AddTrack(const MCTrack& track);

    /// Clears contents
    void Clear();

    /// @brief Finds an index of MC point in internal point container
    /// @param  detID  Detector ID
    /// @param  index  Index of MC point in external point container
    /// @param  event  Index of MC event
    /// @param  file   Index of MC file
    /// @return        Index of MC point in internal point container within event/TS
    ///                If the point is not found the function returns -1
    int FindInternalPointIndex(L1DetectorID detID, int index, int event, int file) const
    {
      int indexGlob = GetPointGlobExtIndex(detID, index);
      auto it       = fmPointLinkMap.find(LinkKey(indexGlob, event, file));
      return (it != fmPointLinkMap.cend()) ? it->second : -1;
    }

    /// @brief Finds an index of MC track in internal track container
    /// @param  index  Index of MC track in external track container
    /// @param  event  Index of MC event
    /// @param  file   Index of MC file
    /// @return        Index of MC track in internal track container within event/TS
    ///                If the track is not found, the function returns -1
    int FindInternalTrackIndex(int index, int event, int file) const
    {
      auto it = fmTrackLinkMap.find(LinkKey(index, event, file));
      return (it != fmTrackLinkMap.cend()) ? it->second : -1;
    }

    /// @brief Gets the first point index for a given detector subsystem
    /// @param detID  Detector ID
    int GetFirstPointIndex(L1DetectorID detID) const
    {
      return std::accumulate(fvNofPointsUsed.cbegin(), fvNofPointsUsed.cbegin() + static_cast<int>(detID), 0);
    }

    /// @brief Gets the next index of point, which is expected being after the last one for a given detector
    /// @param detID  Detector ID
    int GetLastPointIndex(L1DetectorID detID) const
    {
      return std::accumulate(fvNofPointsUsed.cbegin(), fvNofPointsUsed.cbegin() + static_cast<int>(detID) + 1, 0);
    }

    /// @brief Calculates global index of MC point
    /// @param  iPointLocal  Local index of MC psinoint
    /// @param  detID        Detector ID
    ///
    /// The function calculates global external index of MC point as a sum of a given local index and total provided
    /// number of points in previous detector subsystem.
    int GetPointGlobExtIndex(L1DetectorID detID, int iPointLocal) const
    {
      return iPointLocal + std::accumulate(fvNofPointsOrig.cbegin(), fvNofPointsOrig.cbegin() + int(detID), 0);
    }

    /// Gets number of tracks in this event/TS
    int GetNofTracks() const { return fvTracks.size(); }

    /// Gets number of points in this event/TS
    int GetNofPoints() const { return fvPoints.size(); }

    /// @brief Gets original number of MC points in different detectors
    /// @param detID    Detector ID
    int GetNofPointsOrig(L1DetectorID detID) const { return fvNofPointsOrig[static_cast<int>(detID)]; }

    /// @brief Gets used number of MC points in different detectors
    /// @param detID    Detector ID
    int GetNofPointsUsed(L1DetectorID detID) const { return fvNofPointsUsed[static_cast<int>(detID)]; }

    /// Gets a reference to MC point by its index
    const auto& GetPoint(int idx) const { return fvPoints[idx]; }

    /// Gets mutual reference to MC point by its index
    // TODO: SZh 12.12.2022: Probably, the better solution is to write a specific accessor for
    //                       setting indexes to MC points
    auto& GetPoint(int idx) { return fvPoints[idx]; }

    /// Gets a reference to the vector of points
    const auto& GetPointContainer() const { return fvPoints; }

    /// Gets a reference to MC track by its internal index
    const auto& GetTrack(int idx) const { return fvTracks[idx]; }

    /// Gets a mutual reference to MC track by its internal index
    auto& GetTrack(int idx) { return fvTracks[idx]; }

    /// Gets a reference to the vector of tracks
    const auto& GetTrackContainer() const { return fvTracks; }

    /// Gets a mutual reference to the vector of tracks
    auto& GetTrackContainer() { return fvTracks; }

    /// \brief  Initialize information about points and hits association with MC track
    /// \param  vHits  Vector of hit objects
    /// Initialize tracks: defines indexes of hits and points related to the track, calculates max number of points and
    /// hits on a station, number of consecutive stations containing a hit or point and number of stations and points
    /// with hits.
    void InitTrackInfo(const L1Vector<CbmL1HitDebugInfo>& vHits);

    /// Reserves memory for tracks to avoid extra allocations
    void ReserveNofTracks(int nTracks) { fvTracks.reserve(nTracks); }

    /// Reserves memory for points to avoid extra allocations
    void ReserveNofPoints(int nPoints) { fvPoints.reserve(nPoints); }

    /// @brief Sets original number of MC points in different detectors
    /// @param detID    Detector ID
    /// @param nPoints  Number of points
    void SetNofPointsOrig(L1DetectorID detID, int nPoints) { fvNofPointsOrig[static_cast<int>(detID)] = nPoints; }

    /// Prints an example of tracks and points
    /// \param verbose  Verbose level:
    ///                 - #0: Nothing is printed
    ///                 - #1: Only numbers of tracks and points are printed
    ///                 - #2: First five tracks and points are printed (partially)
    std::string ToString(int verbose = 1) const;

  private:
    // ******************************
    // **     Member variables     **
    // ******************************

    L1Vector<MCPoint> fvPoints = {"ca::tools::MCData::fvPoints"};  ///< Container of points
    L1Vector<MCTrack> fvTracks = {"ca::tools::MCData::fvTracks"};  ///< Container of tracks

    std::array<int, L1Constants::size::kMaxNdetectors> fvNofPointsOrig = {0};  ///< Total number of points by detector
    std::array<int, L1Constants::size::kMaxNdetectors> fvNofPointsUsed = {0};  ///< Number of points used vs. detector

    std::unordered_map<LinkKey, int> fmPointLinkMap = {};  ///< MC point internal index vs. link
    std::unordered_map<LinkKey, int> fmTrackLinkMap = {};  ///< MC track internal index vs. link
  };
}  // namespace ca::tools


// *********************************************************
// **     Template and inline function implementation     **
// *********************************************************

// ---------------------------------------------------------------------------------------------------------------------
//
inline void ca::tools::MCData::AddPoint(const MCPoint& point)
{
  fmPointLinkMap[point.GetLinkKey()] = point.GetId();
  fvPoints.push_back(point);
  ++fvNofPointsUsed[static_cast<int>(point.GetDetectorId())];
}

// ---------------------------------------------------------------------------------------------------------------------
//
inline void ca::tools::MCData::AddTrack(const MCTrack& track)
{
  fmTrackLinkMap[track.GetLinkKey()] = track.GetId();
  fvTracks.push_back(track);
}

#endif  // CaToolsMCData_h
