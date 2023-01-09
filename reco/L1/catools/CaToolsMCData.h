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

    /// Adds hit index to MC point
    /// \param  iPoint  Index of MC point
    /// \param

    /// Adds an MC point to points container and a corresponding link key to the point index map
    /// \param  point  MC point object
    void AddPoint(const MCPoint& point);

    /// Adds an MC track to tracks container and a corresponding link key to the link index map
    /// \param  track  MC track object
    void AddTrack(const MCTrack& track);

    /// Clears contents
    void Clear();

    /// Finds an index of MC point in internal point container
    /// \param  index  Index of MC point in external point container
    /// \param  event  Index of MC event
    /// \param  file   Index of MC file
    /// \return        Index of MC point in internal point container within event/TS
    ///                If the point is not found the function returns -1
    int FindInternalPointIndex(int index, int event, int file) const
    {
      auto it = fmPointLinkMap.find(LinkKey(index, event, file));
      return (it != fmPointLinkMap.cend()) ? it->second : -1;
    }

    /// Finds an index of MC track in internal track container
    /// \param  index  Index of MC track in external track container
    /// \param  event  Index of MC event
    /// \param  file   Index of MC file
    /// \return        Index of MC track in internal track container within event/TS
    ///                If the track is not found, the function returns -1
    int FindInternalTrackIndex(int index, int event, int file) const
    {
      auto it = fmTrackLinkMap.find(LinkKey(index, event, file));
      return (it != fmTrackLinkMap.cend()) ? it->second : -1;
    }

    /// Gets number of tracks in this event/TS
    int GetNofTracks() const { return fvTracks.size(); }

    /// Gets number of points in this event/TS
    int GetNofPoints() const { return fvPoints.size(); }

    /// Gets a reference to MC point by its index
    const auto& GetPoint(int idx) const { return fvPoints[idx]; }

    /// Gets mutual reference to MC point by its index
    // TODO: SZh 12.12.2022: Probably, the better solution is to write a specific accessor for
    //                       setting indexes to MC points
    auto& GetPoint(int idx) { return fvPoints[idx]; }

    /// Gets a reference to the vector of points
    const auto& GetPointContainer() const { return fvPoints; }

    /// Gets point index by global index of hit
    /// \param  iHit  Index of hit
    auto GetPointIndexOfHit(int iHit) const { return fvPointIndexOfHit[iHit]; }

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

    /// Registers index of point for a given index of hit
    /// \param  iHit    Index of hit
    /// \param  iPoint  Index of point
    void RegisterPointIndexForHit(int iHit, int iPoint)
    {
      assert(int(fvPointIndexOfHit.size()) > iHit);
      fvPointIndexOfHit[iHit] = iPoint;
    }

    /// Reserves memory for tracks to avoid extra allocations
    void ReserveNofTracks(int nTracks) { fvTracks.reserve(nTracks); }

    /// Reserves memory for points to avoid extra allocations
    void ReserveNofPoints(int nPoints) { fvPoints.reserve(nPoints); }

    /// Reserves total number of used hits in the event
    void ReserveNofHits(int nHits) { fvPointIndexOfHit.reserve(nHits); }

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

    /// Correspondence of MC point index to the global hit index
    L1Vector<int> fvPointIndexOfHit = {"ca::tools::MCData::fvPointIndexOfHit"};

    std::unordered_map<LinkKey, int> fmPointLinkMap = {};  ///< MC point internal index vs. link
    std::unordered_map<LinkKey, int> fmTrackLinkMap = {};  ///< MC track internal index vs. link
  };
}  // namespace ca::tools

#endif  // CaToolsMCData_h
