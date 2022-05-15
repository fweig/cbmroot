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
#include "L1Field.h"
#include "L1ObjectInitController.h"
#include "L1Parameters.h"
#include "L1Utils.h"

//#include <string>
#include <bitset>
#include <memory>  //unique_ptr
#include <set>

/// Forward declaration of the tracking detectors scoped enumeration. Concrete realization of this enumeration must be
/// determined in the concrete setup class (i.e. CbmL1/BmnL1)
enum class L1DetectorID;

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
///    initMan->SetStationsNumberCrosscheck(L1DetectorID::kMvd, NMvdStations)
///    initMan->SetStationsNumberCrosscheck(L1DetectorID::kMvd, NStsStations);
///
/// 4. Initialize each station using L1BaseStationInfo:
///
// TODO: Implement mechanism of reinitialization (S.Zharko)
class L1InitManager {
private:
  enum class InitKey
  {
    // NOTE: Please, keep the numbers of enum items in the existing order: it helps to debug the initialization with
    //       this->GetObjectInitController().ToString() method call (S.Zharko)
    keActiveDetectorIDs,             ///< 0) If the detector sequence is set
    keStationsNumberCrosscheck,      ///< 1) If the crosscheck station numbers were setup
    keFieldFunction,                 ///< 2) If magnetic field getter funciton is set
    keTargetPos,                     ///< 3) If target position was defined
    kePrimaryVertexField,            ///< 4) If magnetic field value and region defined at primary vertex
    keStationsInfo,                  ///< 5) If all the planned stations were added to the manager
    keCAIterationsNumberCrosscheck,  ///< 6) If the number of CA track finder is initialized
    keCAIterations,                  ///< 7) If the CA track finder iterations were initialized
    keTrackingLevel,                 ///< 8)
    keGhostSuppression,              ///< 9)
    keMomentumCutOff,                ///< 10)
    keEnd                            ///< 11) [technical] number of entries in the enum
  };

  using L1DetectorIDIntMap_t     = std::unordered_map<L1DetectorID, int, L1Utils::EnumClassHash>;
  using L1DetectorIDSet_t        = std::set<L1DetectorID>;
  using L1FieldFunction_t        = std::function<void(const double (&xyz)[3], double (&B)[3])>;
  using L1ObjectInitController_t = L1ObjectInitController<static_cast<int>(InitKey::keEnd), InitKey>;

public:
  //
  // CONSTRUCTORS AND DESTRUCTOR
  //
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

  //
  // BASIC METHODS
  //
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

  //
  // GETTERS
  //
  /// Gets a set of actie detectors for this analysis
  const L1DetectorIDSet_t& GetActiveDetectorIDs() const { return fActiveDetectorIDs; }
  /// Gets ghost suppression flag
  int GetGhostSuppression() const { return fGhostSuppression; }
  /// Gets momentum cutoff
  float GetMomentumCutOff() const { return fMomentumCutOff; }
  /// Gets a const reference to L1ObjectInitController
  const L1ObjectInitController_t& GetInitController() const { return fInitController; }
  /// Gets a pointer to L1Parameters instance with a posibility of its fields modification
  const L1Parameters* GetParameters() const { return fpParameters; }
  /// Gets a total number of stations (NOTE: this number includes both active and unactive stations!)
  int GetStationsNumber() const { return static_cast<int>(fStationsInfo.size()); }
  /// Gets a number of stations for a particualr detector ID
  int GetStationsNumber(L1DetectorID detectorID) const;
  // TODO: define enum of dimensions.... (S.Zh.)
  /// Gets a L1FieldRegion object at primary vertex
  const L1FieldRegion& GetTargetFieldRegion() const { return fTargetFieldRegion; }
  /// Gets a L1FieldValue object at primary vertex
  const L1FieldValue& GetTargetFieldValue() const { return fTargetFieldValue; }
  /// Gets a target position
  const std::array<double, 3>& GetTargetPosition() const { return fTargetPos; }
  /// Gets tracking level
  int GetTrackingLevel() const { return fTrackingLevel; }

  /// Calculates L1FieldValue and L1FieldReference values for a selected step in z coordinate from the target position
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

  //
  // SETTERS
  //
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
  /// Sets a number of stations for a particular tracking detector ID to provide initialization cross-check
  void SetStationsNumberCrosscheck(L1DetectorID detectorID, int nStations);
  /// Sets target poisition
  void SetTargetPosition(double x, double y, double z);

  /// Transfers an array of L1Stations formed inside a set of L1BaseStationInfo to a destination std::array
  void TransferL1StationArray(std::array<L1Station, L1Parameters::kMaxNstations>& destinationArray);


private:
  /// Checker for L1CAIteration container initialization (sets InitKey::keCAIterations)
  /// \return true If all L1CAIteration objects were initialized properly
  void CheckCAIterationsInit();
  /// Checker for L1BaseStationInfo set initialization (sets InitKey::keStationsInfo)
  /// \return true If all L1BaseStationInfo objects were initialized properly. Similar effect can be achieved by
  void CheckStationsInfoInit();

  /* Basic fields */

  L1ObjectInitController_t fInitController {};  ///< Initialization flags
  L1DetectorIDSet_t fActiveDetectorIDs {};      ///< Set of tracking detectors, active during this analysis session

  /* Target fields */

  std::array<double, /*nDimensions=*/3> fTargetPos {};  ///< Nominal target position coordinates

  /* Stations related fields */

  std::set<L1BaseStationInfo> fStationsInfo {};  ///< Set of L1BaseStationInfo objects

  /// Map of station numbers used for initialization crosscheck
  L1DetectorIDIntMap_t fStationsNumberCrosscheck {};
  /// A function which returns magnetic field vector B in a radius-vector xyz
  L1FieldFunction_t fFieldFunction {[](const double (&)[3], double (&)[3]) {}};
  // NOTE: Stations of daetectors which will not be assigned as active, will not be included in the tracking!!!!!!!
  // NOTE: fTotalNumberOfStations is excess field for logic, but it's imortant to track L1Algo initialization

  /* Vertex related fields */

  L1FieldValue fTargetFieldValue {};    ///< L1FieldValue object at target
  L1FieldRegion fTargetFieldRegion {};  ///< L1FieldRegion object at target

  /* CA track finder iterations related */

  //L1Vector<L1CAIteration> fCAIterationsContainer {};  ///< Container for CA track finder iterations
  int fCAIterationsNumberCrosscheck {-1};  ///< Number of iterations to be passed (must be used for cross-checks)

  /// Pointer to L1Parameters object
  // NOTE: Owner of the object is L1Algo instance
  L1Parameters* fpParameters {nullptr};

  int fTrackingLevel {0};     ///< tracking level
  int fGhostSuppression {0};  ///< flag: if true, ghost tracks are suppressed
  float fMomentumCutOff {0};  ///< minimum momentum of tracks TODO: ?
};

#endif
