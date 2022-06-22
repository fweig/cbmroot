/* Copyright (C) 2021-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file L1Parameters.h
 * @brief Parameter container for the L1Algo library
 * @since 19.12.2021
 * @author S.Zharko <s.zharko@gsi.de>
 ***********************************************************************************************************/

#ifndef L1Parameters_h
#define L1Parameters_h 1

#include <type_traits>

#include <numeric>

#include "L1CAIteration.h"
#include "L1Constants.h"
#include "L1MaterialInfo.h"
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
  using L1DetectorID_t = std::underlying_type_t<L1DetectorID>;

public:
  /// Default constructor
  L1Parameters();

  /// Destructor
  ~L1Parameters() noexcept;

  /// Copy constructor
  L1Parameters(const L1Parameters& other) noexcept;

  /// Copy assignment operator
  L1Parameters& operator=(const L1Parameters& other) noexcept;

  /// Move constructor
  L1Parameters(L1Parameters&& other) noexcept;

  /// Move assignment operator
  L1Parameters& operator=(L1Parameters&& other) noexcept;

  /// Swap method
  void Swap(L1Parameters& other) noexcept;

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
  int GetNstationsActive() const { return fNstationsActive[fNstationsActive.size() - 1]; }

  /// Gets number of active stations for given detector ID
  int GetNstationsActive(L1DetectorID detectorID) const
  {
    return fNstationsActive[static_cast<L1DetectorID_t>(detectorID)];
  }

  /// Gets total number of stations, provided by setup geometry
  int GetNstationsGeometry() const { return fNstationsGeometry[fNstationsGeometry.size() - 1]; }

  /// Gets number of stations, provided by setup geometry for given detector ID
  int GetNstationsGeometry(L1DetectorID detectorID) const
  {
    return fNstationsGeometry[static_cast<L1DetectorID_t>(detectorID)];
  }

  /// Calculates global index of station among geometry (accounts for inactive stations)
  /// \param localIndex  index of the detector subsystem module/station/layer provided by detector subsystem experts
  /// \param detectorID  ID of the detector subsystem
  __attribute__((always_inline)) int GetStationIndexGeometry(int localIndex, L1DetectorID detectorID) const
  {
    return localIndex
           + std::accumulate(fNstationsGeometry.cbegin(), fNstationsGeometry.cbegin() + static_cast<int>(detectorID),
                             0);
  }

  /// Calculates global index of station used by track finder
  /// \param localIndex  index of the detector subsystem module/station/layer provided by detector subsystem experts
  /// \param detectorID  ID of the detector subsystem
  __attribute__((always_inline)) int GetStationIndexActive(int localIndex, L1DetectorID detectorID) const
  {
    return fActiveStationGlobalIDs[GetStationIndexGeometry(localIndex, detectorID)];
  }

  /// Provides access to L1CAIteration vector (const)
  const L1IterationsContainer_t& GetCAIterations() const { return fCAIterations; }

  /// Provides access to L1Stations container (const)
  const L1StationsContainer_t& GetStations() const { return fStations; }

  /// Gets reference to the particular station
  /// \param iStation  Index of station in the active stations container
  const L1Station& GetStation(int iStation) const { return fStations[iStation]; }

  /// Gets reference to the array of station thickness map
  const L1MaterialContainer_t& GetThicknessMaps() const { return fThickMap; }

  /// Gets material thickness in units of radiation length in a point on the XY plane for a selected station
  /// \param iStActive  Global index of an active station
  /// \param xPos       Position of the point in X dimension [cm]
  /// \param yPos       Position of the point in Y dimension [cm]
  float GetMaterialThickness(int iStActive, float xPos, float yPos) const
  {
    return fThickMap[iStActive].GetRadThick(xPos, yPos);
  }

  /// Gets material thickness in units of radiation length in a point on the XY plane for a selected station
  /// \param iStActive  Global index of an active station
  /// \param xPos       Position of the point in X dimension [cm] (SIMDized vector)
  /// \param yPos       Position of the point in Y dimension [cm] (SIMDized vector)
  fvec GetMaterialThickness(int iStActive, fvec xPos, fvec yPos) const
  {
    return fThickMap[iStActive].GetRadThick(xPos, yPos);
  }

  /// Gets target position
  /// \param iDimension  Component of target position: 0 - x, 1 - y, 2 - z
  fvec GetTargetPositionX() const { return fTargetPos[0]; }
  fvec GetTargetPositionY() const { return fTargetPos[1]; }
  fvec GetTargetPositionZ() const { return fTargetPos[2]; }

  /// Gets L1FieldRegion object at primary vertex
  const L1FieldRegion& GetVertexFieldRegion() const { return fVertexFieldRegion; }

  /// Gets L1FieldValue object at primary vertex
  const L1FieldValue& GetVertexFieldValue() const { return fVertexFieldValue; }

  /// Class invariant checker
  void CheckConsistency() const;

  /// ***************************
  /// ** Flags for development **
  /// ***************************

  /// Are the hit search areas ignored
  bool DevIsIgnoreHitSearchAreas() const { return fDevIsIgnoreHitSearchAreas; }

  /// Is singlets fit starts at the target
  bool DevIsFitSingletsFromTarget() const { return fDevIsFitSingletsFromTarget; }

  /// Flag to match doublets using Mc information
  bool DevIsMatchDoubletsViaMc() const { return fDevIsMatchDoubletsViaMc; }

private:
  unsigned int fMaxDoubletsPerSinglet {150};  ///< Upper-bound cut on max number of doublets per one singlet
  unsigned int fMaxTripletPerDoublets {15};   ///< Upper-bound cut on max number of triplets per one doublet

  alignas(L1Constants::misc::kAlignment)
    L1IterationsContainer_t fCAIterations {};  ///< L1 Track finder iterations vector

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

  /// Numbers of stations, which are active in tracking. Index of an array element (except the last one) corresponds to a given
  /// L1DetectorID of the detector subystem. The last array element corresponds to the total number of stations.
  alignas(L1Constants::misc::kAlignment) std::array<int, (L1Constants::size::kMaxNdetectors + 1)> fNstationsActive {};

  /// Actual numbers of stations, provided by geometry. Index of an array element (except the last one) corresponds to a given
  /// L1DetectorID of the detector subystem. The last array element corresponds to the total number of stations.
  alignas(L1Constants::misc::kAlignment) std::array<int, (L1Constants::size::kMaxNdetectors + 1)> fNstationsGeometry {};

  /// Map of the actual detector indeces to the active detector indeces
  /// The vector maps actual station index (which is defined by ) to the index of station in tracking. If the station is inactive,
  /// its index is equal to -1. Example: let stations 1 and 4 be inactive. Then:
  ///   actual index:  0  1  2  3  4  5  6  7  8  9  0  0  0  0
  ///   active index:  0 -1  1  2 -1  3  4  5  6  7  0  0  0  0
  alignas(L1Constants::misc::kAlignment) std::array<int, L1Constants::size::kMaxNstations> fActiveStationGlobalIDs {};

  /// ***************************
  /// ** Flags for development **
  /// ***************************

  bool fDevIsIgnoreHitSearchAreas {false};  ///< Process all hits on the station ignoring hit search area

  bool fDevIsFitSingletsFromTarget {false};  ///< Fit singlet starting from the target with the KF

  bool fDevIsMatchDoubletsViaMc {false};  ///< Flag to match doublets using Mc information

  /********************************
   ** Friend classes declaration **
   ********************************/

  /// Note provide friend access to L1InitManager (TODO: L1Algo auch?)
  friend class L1InitManager;
};

#endif  // L1Parameters_h
