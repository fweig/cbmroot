/* Copyright (C) 2021-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file  L1InitManager.h
/// \brief Input data management class for L1Algo
/// \since 24.12.2021
#ifndef L1InitManager_h
#define L1InitManager_h 1

#include "L1BaseStationInfo.h"
#include "L1CAIteration.h"
#include "L1ConfigRW.h"
#include "L1Constants.h"
#include "L1Field.h"
#include "L1ObjectInitController.h"
#include "L1Parameters.h"
#include "L1Utils.h"
#include "L1Vector.h"

//#include <string>
#include <array>
#include <bitset>
#include <memory>  //unique_ptr
#include <numeric>
#include <set>
#include <type_traits>

class L1ConfigRW;
class L1Algo;

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
///    initMan->SetNstationsCrosscheck(L1DetectorID::kMvd, fNMvdStations)
///    initMan->SetNstationsCrosscheck(L1DetectorID::kSts, fNStsStations);
///
/// 4. Initialize each station using L1BaseStationInfo:
///
// TODO: Implement mechanism of reinitialization (S.Zharko)
class L1InitManager {
private:
  enum class EInitKey
  {
    // NOTE: Please, keep the numbers of the enumeration items in the existing order: it helps to debug the
    //       initialization with this->GetObjectInitController().ToString() method call (S.Zharko)
    kActiveDetectorIDs,             ///< 0) If the detector sequence is set
    kStationsNumberCrosscheck,      ///< 1) If the crosscheck station numbers were setup
    kFieldFunction,                 ///< 2) If magnetic field getter function is set
    kTargetPos,                     ///< 3) If target position was defined
    kPrimaryVertexField,            ///< 4) If magnetic field value and region defined at primary vertex
    kStationsInfo,                  ///< 5) If all the planned stations were added to the manager
    kCAIterationsNumberCrosscheck,  ///< 6) If the number of CA track finder is initialized
    kCAIterations,                  ///< 7) If the CA track finder iterations were initialized
    kSearchWindows,                 ///< 8) If the hit search windows were initialized
    kTrackingLevel,                 ///< 9)
    kGhostSuppression,              ///< 10)
    kMomentumCutOff,                ///< 11)
    kEnd                            ///< 12) [technical] number of entries in the enumeration
  };

  using L1DetectorIDIntMap_t = std::unordered_map<L1DetectorID, int, L1Utils::EnumClassHash>;
  using L1DetectorIDSet_t    = std::set<L1DetectorID>;
  using L1FieldFunction_t    = std::function<void(const double (&xyz)[3], double (&B)[3])>;
  using InitController_t     = L1ObjectInitController<static_cast<int>(EInitKey::kEnd), EInitKey>;

public:
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


  /// Adds another station of a given type using reference to a L1BaseStationInfo object
  void AddStation(const L1BaseStationInfo& station);

  /// Adds another station of a given type using pointer to a L1BaseStationInfo object
  void AddStation(const L1BaseStationInfo* pStation) { AddStation(*pStation); }

  /// Adds another station of a given type using std::unique_ptr-wraped pointer to L1BaseStationInfo
  void AddStation(const std::unique_ptr<L1BaseStationInfo>& puStation) { AddStation(*puStation); }

  /// Provides final checks of large fields initialization calling Check"Object"Init() private methods,
  /// must be called in the beginning of L1Algo::Init()
  void CheckInit();
  // NOTE: This method calls checkers of large fields initializations like a station or an iteration. The method must be
  //       called in the L1Algo class. (S.Zharko)

  /// Clears vector of CA track finder iterations
  void ClearCAIterations();

  /// Clears vector of base setup
  void ClearSetupInfo();

  /// Forms parameters container
  /// \return  Success flag
  bool FormParametersContainer();

  /// Gets ghost suppression flag
  int GetGhostSuppression() const { return fParameters.fGhostSuppression; }

  /// Gets a name of the input configuration file
  const std::string& GetInputConfigName() const { return fConfigInputName; }

  /// Gets momentum cutoff
  float GetMomentumCutOff() const { return fParameters.fMomentumCutOff; }

  /// Gets a const reference to L1ObjectInitController
  const InitController_t& GetInitController() const { return fInitController; }

  /// Gets total number of active stations
  int GetNstationsActive() const { return fParameters.fNstationsActiveTotal; }

  /// Gets number of active stations for given detector ID
  int GetNstationsActive(L1DetectorID detectorID) const
  {
    return fParameters.fNstationsActive[static_cast<L1DetectorID_t>(detectorID)];
  }

  /// Gets total number of stations, provided by setup geometry
  int GetNstationsGeometry() const { return fParameters.fNstationsGeometryTotal; }

  /// Gets number of stations, provided by setup geometry for given detector ID
  int GetNstationsGeometry(L1DetectorID detectorID) const
  {
    return fParameters.fNstationsGeometry[static_cast<L1DetectorID_t>(detectorID)];
  }

  /// Gets a name of the output configuration file
  const std::string& GetOutputConfigName() const { return fConfigOutputName; }

  /// Gets tracking level
  int GetTrackingLevel() const { return fParameters.fTrackingLevel; }

  /// Calculates L1FieldValue and L1FieldReference values for a selected step in z-axis from the target position
  /// \param zStep step between nodal points
  // TODO: Consider possibility for linear approximation (S.Zh.)
  void InitTargetField(double zStep);

  /// Pushes an CA track finder iteration into a sequence of iteration using reference
  void PushBackCAIteration(const L1CAIteration& iteration);

  /// Pushes an CA track finder iteration into a sequence of iteration using raw pointer
  void PushBackCAIteration(const L1CAIteration* pIteration) { PushBackCAIteration(*pIteration); }

  /// Pushes an CA track finder iteration into a sequence of iteration using std::unique_ptr
  void PushBackCAIteration(const std::unique_ptr<L1CAIteration>& puIteration) { PushBackCAIteration(*puIteration); }

  /// Reads parameters object from boost-serialized binary file
  /// \param  fileName  Name of input file
  void ReadParametersObject(const std::string& fileName);

  /// Reads search windows from file
  /// \param  fileName  Name of input file
  void ReadSearchWindows(const std::string& fileName);

  /// Sets a set of active tracking detector IDs
  void SetActiveDetectorIDs(const L1DetectorIDSet_t& detectorIDs);

  /// Sets a number of CA track finder iterations to provide initialization cross-check
  void SetCAIterationsNumberCrosscheck(int nIterations);

  /// Sets a magnetic field function, which will be applied for all the stations
  void SetFieldFunction(const L1FieldFunction_t& fieldFcn);

  /// FIXME: ...
  void SetGhostSuppression(int ghostSuppression);

  /// Sets a name of the input configuration file. If the file is undefined, default settings will be used. If the file is
  /// defined, the default settings will be overwritten with
  /// \param  filename  Name of the input L1 parameters configuration
  void SetInputConfigName(const std::string& filename) { fConfigInputName = filename; }

  ///
  void SetMomentumCutOff(float momentumCutOff);

  /// Sets a number of actual stations for a particular tracking detector ID to provide initialization cross-check
  void SetNstations(L1DetectorID detectorID, int nStations);

  /// Sets a name of the output configuration file. The output file is created from the fields, saved in the resulted
  /// L1Parameters object
  /// \param  filename  Name of the output L1 parameters configuration
  void SetOutputConfigName(const std::string& filename) { fConfigOutputName = filename; }

  /// @brief Sets pseudo-random numbers generator seed
  /// @param seed  Seed value
  /// @note  The default seed is 0
  void SetRandomSeed(unsigned int seed) { fParameters.fRandomSeed = seed; }

  /// Sets target position
  /// \param  x  Position X component [cm]
  /// \param  y  Position Y component [cm]
  /// \param  z  Position Z component [cm]
  void SetTargetPosition(double x, double y, double z);

  ///
  void SetTrackingLevel(int trackingLevel);
  /// Sets upper-bound cut on max number of doublets per one singlet
  void SetMaxDoubletsPerSinglet(unsigned int value) { fParameters.fMaxDoubletsPerSinglet = value; }

  /// Sets upper-bound cut on max number of triplets per one doublet
  void SetMaxTripletPerDoublets(unsigned int value) { fParameters.fMaxTripletPerDoublets = value; }

  /// Sends (moves) parameters object to the destination reference
  /// \param  pAlgo  Pointer to the L1 tracking algorithm main class
  /// \return Success flag
  bool SendParameters(L1Algo* pAlgo);

  /// @brief Sends (moves) parameters object to an external destination
  /// @param destination  Reference to the parameters object of destination
  /// @return  Success flag
  bool SendParameters(L1Parameters& destination);

  /// Writes parameters object from boost-serialized binary file
  /// \param  fileName  Name of input file
  void WriteParametersObject(const std::string& fileName) const;


  // ***************************
  // ** Flags for development **
  // ***************************

  /// Ignore hit search areas
  void DevSetIgnoreHitSearchAreas(bool value = true) { fParameters.fDevIsIgnoreHitSearchAreas = value; }

  /// Force use of the original field (not approximated)
  void DevSetUseOfOriginalField(bool value = true) { fParameters.fDevIsUseOfOriginalField = value; }

  /// Flag to match doublets using MC information
  void DevSetIsMatchDoubletsViaMc(bool value = true) { fParameters.fDevIsMatchDoubletsViaMc = value; }

  /// Flag to match triplets using Mc information
  void DevSetIsMatchTripletsViaMc(bool value = true) { fParameters.fDevIsMatchTripletsViaMc = value; }

  /// Flag to match triplets using Mc information
  void DevSetIsExtendTracksViaMc(bool value = true) { fParameters.fDevIsExtendTracksViaMc = value; }

  /// Flag to match triplets using Mc information
  void DevSetIsSuppressOverlapHitsViaMc(bool value = true) { fParameters.fDevIsSuppressOverlapHitsViaMc = value; }

  /// Flag to use estimated hit search windows
  /// true:  estimated search windows will be used in track finder
  /// false: Kalman filter will be used in track finder
  void DevSetIsParSearchWUsed(bool value = true) { fParameters.fDevIsParSearchWUsed = value; }

private:
  // *********************
  // ** Private methods **
  // *********************

  /// Checker for L1CAIteration container initialization (sets EInitKey::kCAIterations)
  /// \return true If all L1CAIteration objects were initialized properly
  void CheckCAIterationsInit();

  /// Checker for L1BaseStationInfo set initialization (sets EInitKey::kStationsInfo)
  /// \return true If all L1BaseStationInfo objects were initialized properly. Similar effect can be achieved by
  void CheckStationsInfoInit();


  // *****************
  // ** Data fields **
  // *****************

  // * Basic fields *

  InitController_t fInitController {};      ///< Initialization flags
  L1DetectorIDSet_t fActiveDetectorIDs {};  ///< Set of tracking detectors, active during this analysis session

  // * Target fields *

  double fTargetZ {0.};  ///< Target position z component in double precision

  // * Stations related fields *

  std::set<L1BaseStationInfo> fStationsInfo {};  ///< Set of L1BaseStationInfo objects

  /// A function which returns magnetic field vector B in a radius-vector xyz
  L1FieldFunction_t fFieldFunction {[](const double (&)[3], double (&)[3]) {}};
  // NOTE: Stations of detectors which will not be assigned as active, will not be included in the tracking!

  // * CA track finder iterations related *

  int fCAIterationsNumberCrosscheck {-1};  ///< Number of iterations to be passed (must be used for cross-checks)

  L1Parameters fParameters {};  ///< L1Algo parameters object

  // * Configuration related *
  std::string fConfigInputName {""};              ///< name for the input configuration file
  std::string fConfigOutputName {""};             ///< name for the output configuration file
  L1ConfigRW fConfigRW {this, /*verbose = */ 4};  ///< configuration file reader and writer
};

#endif
