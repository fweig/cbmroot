/* Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file L1InitManager.h
 * @bried Input data management class for L1Algo
 * @since 24.12.2021
 *
 ***********************************************************************************************************/
#ifndef L1InitManager_h
#define L1InitManager_h 1

#include "L1BaseStationInfo.h"
#include "L1Field.h"
#include "L1Parameters.h"
#include "utils/L1Utils.h"

//#include <string>
#include <bitset>
#include <memory>  //unique_ptr
#include <set>

/// Forward declaration of the tracking detectors scoped enumeration. Concrete realization of this enumeration must be
/// determined in the concrete setup class (i.e. CbmL1/BmnL1)
enum class L1DetectorID;

/// Initialization manager for L1Aglo
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
/// 3. Initialize each station using L1BaseStationInfo:
///
class L1InitManager {
private:
  enum
  {
    keActiveDetectorIDs,         ///< If the detector sequence is set
    keStationsNumberCrosscheck,  ///< If the crosscheck station numbers were setup
    keFieldFunction,             ///< If magnetic field getter funciton is set
    keTargetPos,                 ///< If target position was defined
    kePrimaryVertexField,        ///< If magnetic field value and region defined at primary vertex
    keIfStationNumbersChecked,   ///< If the station number was already checked
    keStationsInfo,              ///< If all the planned stations were added to the manager
    keL1StationTransfered,       ///< If the L1Station vector was already transfered to destination array
    keEnd
  };

public:
  //
  // CONSTRUCTORS AND DESTRUCTOR
  //

  /// Default constructor
  L1InitManager() = default;
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
  void AddStation(const L1BaseStationInfo& inStation);
  /// Adds another station of a given type using pointer to a L1BaseStationInfo object
  void AddStation(const L1BaseStationInfo* inStationPtr) { AddStation(*inStationPtr); }
  /// Adds another station of a given type using std::unique_ptr-wraped pointer to L1BaseStationInfo
  void AddStation(const std::unique_ptr<L1BaseStationInfo>& inStationUniquePtr) { AddStation(*inStationUniquePtr); }
  /// Initializes L1Algo: transfers all caputred data to the L1 tracking core
  void Init() const;
  /// Prints a list of stations
  void PrintStations(int verbosityLevel = 0) const;

  /// Transfer an array of L1Stations formed inside a set of L1BaseStationInfo to a destination std::array
  void TransferL1StationArray(std::array<L1Station, L1Parameters::kMaxNstations>& destinationArray);

  //
  // GETTERS
  //

  /// Gets a set of actie detectors for this analysis
  std::set<L1DetectorID> GetActiveDetectorIDs() const { return fActiveDetectorIDs; }
  /// Gets a total number of stations (NOTE: this number includes both active and unactive stations!)
  int GetStationsNumber() const { return static_cast<int>(fStationsInfo.size()); }
  /// Gets a number of stations for a particualr detector ID
  int GetStationsNumber(L1DetectorID detectorID) const;
  /// Gets a target position
  const std::array<double, 3>& GetTargetPosition() const { return fTargetPos; }
  // TODO: define enum of dimensions.... (S.Zh.)
  /// Gets a L1FieldRegion object at primary vertex
  const L1FieldRegion& GetTargetFieldRegion() const { return fTargetFieldRegion; }
  /// Gets a L1FieldValue object at primary vertex
  const L1FieldValue& GetTargetFieldValue() const { return fTargetFieldValue; }

  //
  // SETTERS
  //

  /// Sets a set of active tracking detector IDs
  void SetActiveDetectorIDs(const std::set<L1DetectorID>& detectorIDs);
  /// Sets a magnetic field function, which will be applied for all the stations
  void SetFieldFunction(const std::function<void(const double (&xyz)[3], double (&B)[3])>& fieldFcn);
  /// Sets a number of stations for a particular tracking detector ID to provide initialization cross-check
  void SetStationsNumberCrosscheck(L1DetectorID detectorID, int nStations);
  /// Sets target poisition
  void SetTargetPosition(double x, double y, double z);

  /// Calculates L1FieldValue and L1FieldReference values for a selected step in z coordinate from the target position
  /// \param zStep step between nodal points
  // TODO: Consider posibility for linear approximation (S.Zh.)
  void InitTargetField(double zStep);


private:
  /// Checker for L1BaseStationInfo set
  /// \return true if all L1BaseStationInfo objects were initialized properly. Similar effect can be achieved by
  /// calling the fInitFlags[L1InitManager::keStationsInfo] flag
  bool CheckStationsInfo();

  /* Basic fields */

  std::bitset<L1InitManager::keEnd> fInitFlags {};  ///< Initialization flags
  std::set<L1DetectorID> fActiveDetectorIDs {};     ///< Set of tracking detectors, active during this analysis session

  /* Target fields */

  std::array<double, 3> fTargetPos {};  ///< Nominal target position coordinates

  /* Stations related fields */

  std::set<L1BaseStationInfo> fStationsInfo {};  ///< Set of L1BaseStationInfo objects
  /// Map of station numbers used for initialization crosscheck
  std::unordered_map<L1DetectorID, int, L1Utils::EnumClassHash> fStationsNumberCrosscheck {};
  /// A function which returns magnetic field vector B in a radius-vector xyz
  std::function<void(const double (&xyz)[3], double (&B)[3])> fFieldFunction {
    [](const double (&)[3], double (&)[3]) {}};
  // NOTE: Stations of daetectors which will not be assigned as active, will not be included in the tracking!!!!!!!
  // NOTE: fTotalNumberOfStations is excess field for logic, but it's imortant to track L1Algo initialization

  /* Vertex related fields */
  L1FieldValue fTargetFieldValue {};    ///> L1FieldValue object at target
  L1FieldRegion fTargetFieldRegion {};  ///> L1FieldRegion object at target
};

#endif
