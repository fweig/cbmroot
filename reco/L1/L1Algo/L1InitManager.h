/* Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file L1InitManager.h
 * @bried Input data management class for L1Algo
 * @since 24.12.2021
 ***********************************************************************************************************/
#ifndef L1InitManager_h
#define L1InitManager_h 1

#include "L1BaseStationInfo.h"
#include "L1CAIteration.h"
#include "L1Constants.h"
#include "L1Field.h"
#include "L1ObjectInitController.h"
#include "L1Parameters.h"
#include "L1Utils.h"
#include "L1Vector.h"

//#include <string>
#include <type_traits>

#include <array>
#include <bitset>
#include <memory>  //unique_ptr
#include <numeric>
#include <set>

/// Forward declaration of the tracking detectors scoped enumeration. Concrete realization of this enumeration must be
/// determined in the concrete setup class (i.e. CbmL1/BmnL1)
enum class L1DetectorID;

/// Underlying
using L1DetectorID_t = std::underlying_type_t<L1DetectorID>;

/// Initialization manager for L1Algo
///
/// ==== Expected initialization steps ==== (TODO: keep this instruction up-to-date)
///
/// 0. Implement your enum class L1DetectorID with detector systems used for tracking:
///    IMPORTANT!!! Detectors must be sorted in the beam direction
///    /// Concrete L1DetectorID implementation for CBM
///    enum class L1DetectorID {
///      kMvd,
///      kSts,
///      kMuch,
///      kTof,
///      kTrd
///    };
///
/// 1. Get a pointer to the :L1InitManager field of L1Algo:
///
///    L1InitManager * initMan = algo->GetL1InitManager();
///
/// 2. Initialize a set of L1DetectorID's for detectors active in tracking:
///
///    std::set<L1DetectorID> activeTrackingDetectors { L1Detector::kMvd, L1Detector::kSts };
///    initMan->SetActiveDetectorIDs(activeTrackingDetectors);
///
/// 3. Initialize number of stations for each detector:
///
///    initMan->SetNstationsCrosscheck(L1DetectorID::kMvd, NMvdStations)
///    initMan->SetNstationsCrosscheck(L1DetectorID::kSts, NStsStations);
///
/// 4. Initialize each station using L1BaseStationInfo:
///
// TODO: Implement mechanism of reinitialization (S.Zharko)
class L1InitManager {
private:
  enum class EInitKey
  {
    // NOTE: Please, keep the numbers of enum items in the existing order: it helps to debug the initialization with
    //       this->GetObjectInitController().ToString() method call (S.Zharko)
    kActiveDetectorIDs,             ///< 0) If the detector sequence is set
    kStationsNumberCrosscheck,      ///< 1) If the crosscheck station numbers were setup
    kFieldFunction,                 ///< 2) If magnetic field getter funciton is set
    kTargetPos,                     ///< 3) If target position was defined
    kPrimaryVertexField,            ///< 4) If magnetic field value and region defined at primary vertex
    kStationsInfo,                  ///< 5) If all the planned stations were added to the manager
    kCAIterationsNumberCrosscheck,  ///< 6) If the number of CA track finder is initialized
    kCAIterations,                  ///< 7) If the CA track finder iterations were initialized
    kTrackingLevel,                 ///< 8)
    kGhostSuppression,              ///< 9)
    kMomentumCutOff,                ///< 10)
    kEnd                            ///< 11) [technical] number of entries in the enum
  };

  using L1DetectorIDIntMap_t     = std::unordered_map<L1DetectorID, int, L1Utils::EnumClassHash>;
  using L1DetectorIDSet_t        = std::set<L1DetectorID>;
  using L1FieldFunction_t        = std::function<void(const double (&xyz)[3], double (&B)[3])>;
  using InitController_t         = L1ObjectInitController<static_cast<int>(EInitKey::kEnd), EInitKey>;

public:
  /// Default constructor
  L1InitManager() = delete;

  /// Constructor from ptr to L1Paramters object
  L1InitManager(L1Parameters* pParameters);

  /// Destructor
  ~L1InitManager() = default;

  /// Copy constructor is forbidden
  L1InitManager(const L1InitManager& /*other*/) = delete;

  /// Move constructor is forbidden
  L1InitManager(L1InitManager&& /*other*/) = delete;

  /// Copy assignment operator is forbidden
  L1InitManager& operator=(const L1InitManager& /*other*/) = delete;

  /// Move assignment operator is forbidden
  L1InitManager& operator=(L1InitManager&& /*other*/) = delete;


  /// Adds another station of a given type using reference to a L1BaseStationInfo object
  void AddStation(const L1BaseStationInfo& station);

  /// Adds another station of a given type using pointer to a L1BaseStationInfo object
  void AddStation(const L1BaseStationInfo* pStation) { AddStation(*pStation); }

  /// Adds another station of a given type using std::unique_ptr-wraped pointer to L1BaseStationInfo
  void AddStation(const std::unique_ptr<L1BaseStationInfo>& puStation) { AddStation(*puStation); }

  /// Provides final checks of large fields initialization calling Check"Object"Init() privat methods,
  /// must be called in the begining of L1Algo::Init()
  void CheckInit();
  // NOTE: This method calls checkers of large fields initializations like a station or an iteration. The method must be
  //       called in the L1Algo class. (S.Zharko)

  /// Gets ghost suppression flag
  int GetGhostSuppression() const { return fGhostSuppression; }

  /// Gets momentum cutoff
  float GetMomentumCutOff() const { return fMomentumCutOff; }

  /// Gets a const reference to L1ObjectInitController
  const InitController_t& GetInitController() const { return fInitController; }

  /// Gets a pointer to L1Parameters instance with a posibility of its fields modification
  const L1Parameters* GetParameters() const { return fpParameters; }

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

  /// Gets L1FieldRegion object at primary vertex
  const L1FieldRegion& GetTargetFieldRegion() const { return fTargetFieldRegion; }

  /// Gets L1FieldValue object at primary vertex
  const L1FieldValue& GetTargetFieldValue() const { return fTargetFieldValue; }

  /// Gets a target position
  const std::array<double, 3>& GetTargetPosition() const { return fTargetPos; }

  /// Gets tracking level
  int GetTrackingLevel() const { return fTrackingLevel; }

  /// Calculates L1FieldValue and L1FieldReference values for a selected step in z-axis from the target position
  /// \param zStep step between nodal points
  // TODO: Consider posibility for linear approximation (S.Zh.)
  void InitTargetField(double zStep);

  /// Prints a list of CA track finder iterations
  void PrintCAIterations(int verbosityLevel = 0) const;

  /// Prints a list of stations
  void PrintStations(int verbosityLevel = 0) const;

  /// Pushes an CA track finder iteration into a sequence of iteration using reference
  void PushBackCAIteration(const L1CAIteration& iteration);

  /// Pushes an CA track finder iteration into a sequence of iteration using raw pointer
  void PushBackCAIteration(const L1CAIteration* pIteration) { PushBackCAIteration(*pIteration); }

  /// Pushes an CA track finder iteration into a sequence of iteration using std::unique_ptr
  void PushBackCAIteration(const std::unique_ptr<L1CAIteration>& puIteration) { PushBackCAIteration(*puIteration); }

  /// Sets a set of active tracking detector IDs
  void SetActiveDetectorIDs(const L1DetectorIDSet_t& detectorIDs);

  /// Sets a number of CA track finder iterations to provide initialization cross-check
  void SetCAIterationsNumberCrosscheck(int nIterations);

  /// Sets a magnetic field function, which will be applied for all the stations
  void SetFieldFunction(const L1FieldFunction_t& fieldFcn);

  ///
  void SetGhostSuppression(int ghostSuppression);

  ///
  void SetMomentumCutOff(float momentumCutOff);

  ///
  void SetTrackingLevel(int trackingLevel);

  /// Sets a number of actual stations for a particular tracking detector ID to provide initialization cross-check
  void SetNstations(L1DetectorID detectorID, int nStations);

  /// Sets target poisition
  void SetTargetPosition(double x, double y, double z);

  /// Transfers an array of L1Stations formed inside a set of L1BaseStationInfo to a destination std::array
  /// \param destinationArray  Reference to the destination array of L1Station objects in the L1Algo core
  void TransferL1StationArray(std::array<L1Station, L1Constants::size::kMaxNstations>& destinationArray);

  /// Transfers an array of L1Material tables formed inside a set of L1BaseStationInfo to a destination std::array
  /// \param destinationArray  Reference to the destination array of L1Material objects in the L1Algo core
  void TransferL1MaterialArray(std::array<L1Material, L1Constants::size::kMaxNstations>& destinationArray);


private:
  /// Checker for L1CAIteration container initialization (sets EInitKey::kCAIterations)
  /// \return true If all L1CAIteration objects were initialized properly
  void CheckCAIterationsInit();

  /// Checker for L1BaseStationInfo set initialization (sets EInitKey::kStationsInfo)
  /// \return true If all L1BaseStationInfo objects were initialized properly. Similar effect can be achieved by
  void CheckStationsInfoInit();

  /* Basic fields */

  InitController_t fInitController {};          ///< Initialization flags
  L1DetectorIDSet_t fActiveDetectorIDs {};      ///< Set of tracking detectors, active during this analysis session

  /* Target fields */

  std::array<double, /*nDimensions=*/3> fTargetPos {};  ///< Nominal target position coordinates [cm]

  /* Stations related fields */

  std::set<L1BaseStationInfo> fStationsInfo {};  ///< Set of L1BaseStationInfo objects

  /// Numbers of stations, which are active in tracking. Index of an array element (except the last one) corresponds to a given
  /// L1DetectorID of the detector subystem. The last array element corresponds to the total number of stations.
  std::array<int, L1Constants::size::kMaxNdetectors + 1> fNstationsActive {};

  /// Actual numbers of stations, provided by geometry. Index of an array element (except the last one) corresponds to a given
  /// L1DetectorID of the detector subystem. The last array element corresponds to the total number of stations.
  std::array<int, L1Constants::size::kMaxNdetectors + 1> fNstationsGeometry {};

  /// Map of the actual detector indeces to the active detector indeces
  /// The vector maps actual station index (which is defined by ) to the index of station in tracking. If the station is inactive,
  /// its index is equal to -1. Example: let stations 1 and 4 be inactive. Then:
  ///   actual index:  0  1  2  3  4  5  6  7  8  9  0  0  0  0
  ///   active index:  0 -1  1  2 -1  3  4  5  6  7  0  0  0  0
  std::array<int, L1Constants::size::kMaxNstations> fActiveStationGlobalIDs {};

  /// A function which returns magnetic field vector B in a radius-vector xyz
  L1FieldFunction_t fFieldFunction {[](const double (&)[3], double (&)[3]) {}};
  // NOTE: Stations of daetectors which will not be assigned as active, will not be included in the tracking!

  /* Vertex related fields */

  L1FieldValue fTargetFieldValue {};    ///< L1FieldValue object at target
  L1FieldRegion fTargetFieldRegion {};  ///< L1FieldRegion object at target

  /* CA track finder iterations related */

  int fCAIterationsNumberCrosscheck {-1};  ///< Number of iterations to be passed (must be used for cross-checks)


  L1Parameters* fpParameters {
    nullptr};  ///< Pointer to L1Parameters object. NOTE: Owner of the object is L1Algo instance

  int fTrackingLevel {0};     ///< tracking level
  int fGhostSuppression {0};  ///< flag: if true, ghost tracks are suppressed
  float fMomentumCutOff {0};  ///< minimum momentum of tracks TODO: ?
};

#endif
