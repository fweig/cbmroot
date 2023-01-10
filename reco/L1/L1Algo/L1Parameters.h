/* Copyright (C) 2021-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// @file L1Parameters.h
/// @brief Parameter container for the L1Algo library
/// @since 19.12.2021
/// @author S.Zharko <s.zharko@gsi.de>

#ifndef L1Parameters_h
#define L1Parameters_h 1

#include <boost/serialization/array.hpp>

#include <numeric>
#include <type_traits>

#include "L1CAIteration.h"
#include "L1Constants.h"
#include "L1Material.h"
#include "L1SearchWindow.h"
#include "L1Station.h"
#include "L1Utils.h"
#include "L1Vector.h"

class L1InitManager;
enum class L1DetectorID;

/// Type definitions for used containers
using L1IterationsContainer_t = L1Vector<L1CAIteration>;
using L1StationsContainer_t   = std::array<L1Station, L1Constants::size::kMaxNstations>;
using L1MaterialContainer_t   = std::array<L1Material, L1Constants::size::kMaxNstations>;


/// Class L1Parameters represents a container for all external parameters of the L1 tracking algorithm,
/// including geometry parameters and physics cuts. The instance of the L1Parameters is constructed inside
/// L1InitManager class and then moved to the L1Algo instance.
///
class alignas(L1Constants::misc::kAlignment) L1Parameters {

  // ********************************
  // ** Friend classes declaration **
  // ********************************

  friend class L1InitManager;

  using L1DetectorID_t = std::underlying_type_t<L1DetectorID>;

public:
  /// Default constructor
  L1Parameters();

  /// Destructor
  ~L1Parameters() noexcept = default;

  /// Copy constructor
  L1Parameters(const L1Parameters& other) = default;

  /// Copy assignment operator
  L1Parameters& operator=(const L1Parameters& other) = default;

  /// Move constructor
  L1Parameters(L1Parameters&& other) noexcept = default;

  /// Move assignment operator
  L1Parameters& operator=(L1Parameters&& other) = default;

  /// Prints configuration
  void Print(int verbosityLevel = 0) const;

  /// String representation of the class contents
  std::string ToString(int verbosity = 0, int indentLevel = 0) const;

  /// Sets upper-bound cut on max number of doublets per one singlet
  void SetMaxDoubletsPerSinglet(unsigned int value) { fMaxDoubletsPerSinglet = value; }

  /// Sets upper-bound cut on max number of triplets per one doublet
  void SetMaxTripletPerDoublets(unsigned int value) { fMaxTripletPerDoublets = value; }

  /// Gets upper-bound cut on max number of doublets per one singlet
  unsigned int GetMaxDoubletsPerSinglet() const { return fMaxDoubletsPerSinglet; }

  /// Gets upper-bound cut on max number of triplets per one doublet
  unsigned int GetMaxTripletPerDoublets() const { return fMaxTripletPerDoublets; }

  /// Gets total number of active stations
  int GetNstationsActive() const { return fNstationsActiveTotal; }

  /// Gets number of active stations for given detector ID
  int GetNstationsActive(L1DetectorID detectorID) const
  {
    return fNstationsActive[static_cast<L1DetectorID_t>(detectorID)];
  }

  /// Gets total number of stations, provided by setup geometry
  int GetNstationsGeometry() const { return fNstationsGeometryTotal; }

  /// Gets number of stations, provided by setup geometry for given detector ID
  int GetNstationsGeometry(L1DetectorID detectorID) const
  {
    return fNstationsGeometry[static_cast<L1DetectorID_t>(detectorID)];
  }

  /// Calculates global index of station among geometry (accounts for inactive stations)
  /// \param localIndex  index of the detector subsystem module/station/layer provided by detector subsystem experts
  /// \param detectorID  ID of the detector subsystem
  [[gnu::always_inline]] int GetStationIndexGeometry(int localIndex, L1DetectorID detectorID) const
  {
    return localIndex
           + std::accumulate(fNstationsGeometry.cbegin(), fNstationsGeometry.cbegin() + static_cast<int>(detectorID),
                             0);
  }

  /// Calculates global index of station used by track finder
  /// \param localIndex  index of the detector subsystem module/station/layer provided by detector subsystem experts
  /// \param detectorID  ID of the detector subsystem
  [[gnu::always_inline]] int GetStationIndexActive(int localIndex, L1DetectorID detectorID) const
  {
    return fActiveStationGlobalIDs[GetStationIndexGeometry(localIndex, detectorID)];
  }

  /// Provides access to L1CAIteration vector (const)
  const L1IterationsContainer_t& GetCAIterations() const { return fCAIterations; }

  /// Provides number of iterations
  int GetNcaIterations() const { return fCAIterations.size(); }

  /// Provides access to L1Stations container (const)
  const L1StationsContainer_t& GetStations() const { return fStations; }

  /// Gets reference to the particular station
  /// \param iStation  Index of station in the active stations container
  const L1Station& GetStation(int iStation) const { return fStations[iStation]; }

  /// Gets a search window for a selected station and track group
  /// \note For a particular track finder iteration one can select a track group, which is defined by the minimal
  ///       momentum of tracks (fast tracks, all tracks), their vertex (primary or secondary tracks), or by particle
  ///       (electrons, muons, hadrons, etc.)
  /// \param  iStation  Global index of active station
  /// \param  iTrackGr  Index of a track group
  const L1SearchWindow& GetSearchWindow(int iStation, int iTrackGr) const
  {
    assert(iStation < fNstationsActiveTotal && iStation > 0);
    assert(iTrackGr < int(fCAIterations.size()));
    return fSearchWindows[iTrackGr * L1Constants::size::kMaxNstations + iStation];
  }

  /// Gets reference to the array of station thickness map
  const L1MaterialContainer_t& GetThicknessMaps() const { return fThickMap; }

  /// Gets material thickness in units of radiation length in a point on the XY plane for a selected station
  /// \param iStActive  Global index of an active station
  /// \param xPos       Position of the point in X dimension [cm]
  /// \param yPos       Position of the point in Y dimension [cm]
  float GetMaterialThicknessScal(int iStActive, float xPos, float yPos) const
  {
    return fThickMap[iStActive].GetRadThickScal(xPos, yPos);
  }

  /// Gets material thickness in units of radiation length in a point on the XY plane for a selected station
  /// \param iStActive  Global index of an active station
  /// \param xPos       Position of the point in X dimension [cm] (SIMDized vector)
  /// \param yPos       Position of the point in Y dimension [cm] (SIMDized vector)
  fvec GetMaterialThickness(int iStActive, fvec xPos, fvec yPos) const
  {
    return fThickMap[iStActive].GetRadThickVec(xPos, yPos);
  }

  /// Gets X component of target position
  fvec GetTargetPositionX() const { return fTargetPos[0]; }

  /// Gets Y component of target position
  fvec GetTargetPositionY() const { return fTargetPos[1]; }

  /// Gets Z component of target position
  fvec GetTargetPositionZ() const { return fTargetPos[2]; }

  /// Gets L1FieldRegion object at primary vertex
  const L1FieldRegion& GetVertexFieldRegion() const { return fVertexFieldRegion; }

  /// Gets L1FieldValue object at primary vertex
  const L1FieldValue& GetVertexFieldValue() const { return fVertexFieldValue; }

  /// Gets ghost suppression flag
  int GetGhostSuppression() const { return fGhostSuppression; }

  /// Gets tracking level
  int GetTrackingLevel() const { return fTrackingLevel; }

  /// Gets momentum cutoff
  float GetMomentumCutOff() const { return fMomentumCutOff; }

  /// Class invariant checker
  void CheckConsistency() const;

  // ***************************
  // ** Flags for development **
  // ***************************

  /// Are the hit search areas ignored
  bool DevIsIgnoreHitSearchAreas() const { return fDevIsIgnoreHitSearchAreas; }

  /// Is original field must be used instead of the approximated one
  bool DevIsUseOfOriginalField() const { return fDevIsUseOfOriginalField; }

  /// Flag to match doublets using MC information
  bool DevIsMatchDoubletsViaMc() const { return fDevIsMatchDoubletsViaMc; }

  /// Flag to match triplets using Mc information
  bool DevIsMatchTripletsViaMc() const { return fDevIsMatchTripletsViaMc; }

  /// Flag to extend tracks using Mc information
  bool DevIsExtendTracksViaMc() const { return fDevIsExtendTracksViaMc; }

  /// Flag to match hits in overlaps using Mc information
  bool DevIsSuppressOverlapHitsViaMc() const { return fDevIsSuppressOverlapHitsViaMc; }

private:
  unsigned int fMaxDoubletsPerSinglet {150};  ///< Upper-bound cut on max number of doublets per one singlet
  unsigned int fMaxTripletPerDoublets {15};   ///< Upper-bound cut on max number of triplets per one doublet

  alignas(L1Constants::misc::kAlignment) L1IterationsContainer_t fCAIterations {};  ///< L1 tracking iterations vector

  /*************************
   ** Geometry parameters **
   *************************/
  /// Target position
  alignas(L1Constants::misc::kAlignment) std::array<fvec, /*nDimensions*/ 3> fTargetPos {L1Utils::kNaN, L1Utils::kNaN,
                                                                                         L1Utils::kNaN};

  /// Field value object at primary vertex (between target and the first station)
  alignas(L1Constants::misc::kAlignment) L1FieldValue fVertexFieldValue {};

  /// Field region object at primary vertex (between target and the first station)
  alignas(L1Constants::misc::kAlignment) L1FieldRegion fVertexFieldRegion {};

  /// Array of stations
  alignas(L1Constants::misc::kAlignment) L1StationsContainer_t fStations {};

  /// Array of station thickness map
  alignas(L1Constants::misc::kAlignment) L1MaterialContainer_t fThickMap {};

  int fNstationsGeometryTotal = -1;  ///< total number of stations, provided by geometry
  int fNstationsActiveTotal   = -1;  ///< total number of active tracking stations

  /// Actual numbers of stations, provided by geometry. Index of an array element (except the last one) corresponds to a given
  /// L1DetectorID of the detector subsystem. The last array element corresponds to the total number of stations.
  alignas(L1Constants::misc::kAlignment) std::array<int, L1Constants::size::kMaxNdetectors> fNstationsGeometry = {};

  /// Numbers of stations, which are active in tracking. Index of an array element (except the last one) corresponds to a given
  /// L1DetectorID of the detector subsystem. The last array element corresponds to the total number of stations.
  alignas(L1Constants::misc::kAlignment) std::array<int, L1Constants::size::kMaxNdetectors> fNstationsActive = {};

  /// Map of the actual detector indexes to the active detector indexes
  /// The vector maps actual station index (which is defined by ) to the index of station in tracking. If the station is inactive,
  /// its index is equal to -1. Example: let stations 1 and 4 be inactive. Then:
  ///   actual index:  0  1  2  3  4  5  6  7  8  9  0  0  0  0
  ///   active index:  0 -1  1  2 -1  3  4  5  6  7  0  0  0  0
  alignas(L1Constants::misc::kAlignment) std::array<int, L1Constants::size::kMaxNstations> fActiveStationGlobalIDs {};

  /// Map of search windows vs. active station global index and tracks group
  /// The tracks group can be defined by minimum momentum (fast/all tracks), origin (primary/secondary) and particle type
  /// (electron, muon, all). Other options also can be added
  alignas(L1Constants::misc::kAlignment)
    std::array<L1SearchWindow, L1Constants::size::kMaxNstations* L1Constants::size::kMaxNtrackGroups> fSearchWindows =
      {};

  int fTrackingLevel    = 0;  ///< tracking level
  int fGhostSuppression = 0;  ///< flag: if true, ghost tracks are suppressed
  float fMomentumCutOff = 0;  ///< minimum momentum of tracks

  // ***************************
  // ** Flags for development **
  // ***************************

  bool fDevIsIgnoreHitSearchAreas {false};      ///< Process all hits on the station ignoring hit search area
  bool fDevIsUseOfOriginalField {false};        ///< Force use of original field
  bool fDevIsMatchDoubletsViaMc {false};        ///< Flag to match doublets using MC information
  bool fDevIsMatchTripletsViaMc {false};        ///< Flag to match triplets using Mc information
  bool fDevIsExtendTracksViaMc {false};         ///< Flag to extend tracks using Mc information
  bool fDevIsSuppressOverlapHitsViaMc {false};  ///<  Flag to match hits in overlaps using Mc information

  bool fDevIsParSearchWUsed       = false;  ///< Flag: when true, the parametrized search windows are used in track
                                            ///< finder; when false, the Kalman filter is used instead

  /// Serialization function
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int)
  {
    ar& fMaxDoubletsPerSinglet;
    ar& fMaxTripletPerDoublets;

    ar& fCAIterations;
    ar& fTargetPos;

    ar& fVertexFieldValue;
    ar& fVertexFieldRegion;

    ar& fStations;
    ar& fThickMap;

    ar& fNstationsGeometryTotal;
    ar& fNstationsActiveTotal;
    ar& fNstationsGeometry;
    ar& fNstationsActive;
    ar& fActiveStationGlobalIDs;
    ar& fSearchWindows;

    ar& fTrackingLevel;
    ar& fGhostSuppression;
    ar& fMomentumCutOff;

    ar& fDevIsIgnoreHitSearchAreas;
    ar& fDevIsUseOfOriginalField;
    ar& fDevIsMatchDoubletsViaMc;
    ar& fDevIsMatchTripletsViaMc;
    ar& fDevIsExtendTracksViaMc;
    ar& fDevIsSuppressOverlapHitsViaMc;
  }
};

#endif  // L1Parameters_h
