/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file  L1InitManager.cxx
/// \brief Input parameters management class for L1Algo
/// \since 19.01.2022

#include "L1InitManager.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <algorithm>
#include <fstream>
#include <sstream>

#include "L1Algo.h"
#include "L1Assert.h"
// ----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::AddStation(const L1BaseStationInfo& inStation)
{
  // Check if other fields were defined already
  // Active detector IDs
  L1MASSERT(0, fInitController.GetFlag(EInitKey::kActiveDetectorIDs),
            "Attempt to add a station info before the active detectors set had been initialized");

  // Number of stations check
  L1MASSERT(0, fInitController.GetFlag(EInitKey::kStationsNumberCrosscheck),
            "Attempt to add a station info before the numbers of stations for each detector had been initialized");

  // Field function
  L1MASSERT(0, fInitController.GetFlag(EInitKey::kFieldFunction),
            "Attempt to add a station info before the magnetic field function had been initialized");

  // Check activeness of this station type
  bool isStationActive =
    inStation.GetTrackingStatus() && fActiveDetectorIDs.find(inStation.GetDetectorID()) != fActiveDetectorIDs.end();
  if (isStationActive) {
    // initialize magnetic field slice
    L1BaseStationInfo inStationCopy = L1BaseStationInfo(inStation);  // make a copy of station so it can be initialized
    inStationCopy.SetFieldFunction(fFieldFunction);

    // check, if material map is used
    if (!inStationCopy.GetInitController().GetFlag(L1BaseStationInfo::EInitKey::kThicknessMap)) {
      LOG(warn) << "Station material map was not set for detectorID = "
                << static_cast<int>(inStationCopy.GetDetectorID()) << ", stationID = " << inStationCopy.GetStationID()
                << ". Homogeneous material budget will be used: " << inStationCopy.GetRadThick()[0];
      L1Material material;
      material.SetBins(1, 100);
      material.SetRadThickBin(0, 0, inStationCopy.GetRadThick()[0]);
      inStationCopy.SetMaterialMap(std::move(material));
    }

    // Check station init
    LOG(debug) << "L1InitManager::AddStation:(original) L1BaseStationInfo " << inStation.GetInitController().ToString();
    LOG(debug) << "L1InitManager::AddStation:(copy)     L1BaseStationInfo " << inStation.GetInitController().ToString();
    if (!inStationCopy.GetInitController().IsFinalized()) {
      LOG(fatal) << "Attempt to add an incompletely initialized station info object (detectorID = "
                 << static_cast<int>(inStationCopy.GetDetectorID()) << ", stationID = " << inStationCopy.GetStationID()
                 << ")";
    }
    // insert the station in a set
    auto insertionResult = fStationsInfo.insert(std::move(inStationCopy));
    if (!insertionResult.second) {
      LOG(fatal) << "Attempt to add a duplicated station info object (detectorID = "
                 << static_cast<int>(inStationCopy.GetDetectorID()) << ", stationID = " << inStationCopy.GetStationID()
                 << ")";
    }

    int index = fStationsInfo.size() - 1 + fParameters.fNstationsGeometryTotal - fParameters.fNstationsActiveTotal;
    fParameters.fActiveStationGlobalIDs[index] = fStationsInfo.size() - 1;
  }
  else {
    int index = fStationsInfo.size() + fParameters.fNstationsGeometryTotal - fParameters.fNstationsActiveTotal;
    fParameters.fActiveStationGlobalIDs[index] = -1;
    fParameters.fNstationsActive[static_cast<L1DetectorID_t>(inStation.GetDetectorID())]--;
    fParameters.fNstationsActiveTotal--;
  }
  LOG(debug) << "L1InitManager: adding a station with stationID = " << inStation.GetStationID()
             << " and detectorID = " << static_cast<int>(inStation.GetDetectorID())
             << ". Is active: " << isStationActive;
}

// ----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::CheckInit()
{
  this->CheckCAIterationsInit();
  this->CheckStationsInfoInit();
}

// ----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::ClearSetupInfo()
{
  // Clear stations set and a thickness map
  fStationsInfo.clear();
  std::fill(fParameters.fThickMap.begin(), fParameters.fThickMap.end(), L1Material());
  fInitController.SetFlag(EInitKey::kStationsInfo, false);

  // Set number of stations do default values
  std::fill(fParameters.fNstationsGeometry.begin(), fParameters.fNstationsGeometry.end(), 0);
  std::fill(fParameters.fNstationsActive.begin(), fParameters.fNstationsActive.end(), 0);
  std::fill(fParameters.fActiveStationGlobalIDs.begin(), fParameters.fActiveStationGlobalIDs.end(), 0);
  fParameters.fNstationsGeometryTotal = -1;
  fParameters.fNstationsActiveTotal   = -1;
  fInitController.SetFlag(EInitKey::kStationsNumberCrosscheck, false);

  // Clear active detectors
  fActiveDetectorIDs.clear();
  fInitController.SetFlag(EInitKey::kActiveDetectorIDs, false);

  // Clear field info
  fParameters.fVertexFieldRegion = L1FieldRegion();
  fParameters.fVertexFieldValue  = L1FieldValue();
  fInitController.SetFlag(EInitKey::kPrimaryVertexField, false);

  // Clear target position
  std::fill(fParameters.fTargetPos.begin(), fParameters.fTargetPos.end(), L1Utils::kNaN);
  fTargetZ = 0.;
  fInitController.SetFlag(EInitKey::kTargetPos, false);

  // Clear field function
  fFieldFunction = L1FieldFunction_t([](const double(&)[3], double(&)[3]) {});
  fInitController.SetFlag(EInitKey::kFieldFunction, false);

  // Clear other flags
  fParameters.fTrackingLevel    = 0;
  fParameters.fGhostSuppression = 0;
  fParameters.fMomentumCutOff   = 0;
  fInitController.SetFlag(EInitKey::kTrackingLevel, false);
  fInitController.SetFlag(EInitKey::kGhostSuppression, false);
  fInitController.SetFlag(EInitKey::kMomentumCutOff, false);

  fParameters.fDevIsIgnoreHitSearchAreas  = false;
  fParameters.fDevIsFitSingletsFromTarget = false;
  fParameters.fDevIsMatchDoubletsViaMc    = false;
  fParameters.fDevIsMatchTripletsViaMc    = false;
}

// ----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::ClearCAIterations()
{
  fParameters.fCAIterations.clear();
  fCAIterationsNumberCrosscheck = -1;
  fInitController.SetFlag(EInitKey::kCAIterations, false);
  fInitController.SetFlag(EInitKey::kCAIterationsNumberCrosscheck, false);
}

// ----------------------------------------------------------------------------------------------------------------------
// NOTE: this function should be called once in the SendParameters
bool L1InitManager::FormParametersContainer()
{
  // Read configuration file
  // NOTE: We consider parameters from the configuration file as ones with a higher priority, so all the defined
  //       variables there would be rewritten by the configuration
  if (fConfigInputName != "") { fConfigRW.ReadYaml(fConfigInputName); }

  // Check initialization
  this->CheckInit();

  if (!fInitController.IsFinalized()) {
    LOG(fatal) << "Attempt to form parameters container before all necessary fields were initialized"
               << fInitController.ToString();
  }

  // Form array of stations
  auto destinationArrayIterator = fParameters.fStations.begin();
  for (const auto& item : fStationsInfo) {
    *destinationArrayIterator = item.GetL1Station();
    ++destinationArrayIterator;
  }

  // Form array of material map
  auto thickMapIt = fParameters.fThickMap.begin();
  for (auto it = fStationsInfo.begin(); it != fStationsInfo.end(); ++it) {
    auto node   = fStationsInfo.extract(it);
    *thickMapIt = std::move(node.value().TakeMaterialMap());
    fStationsInfo.insert(std::move(node));
    ++thickMapIt;
  }

  // Check the consistency of the parameters object. If object inconsistent, it throws std::logic_error
  try {
    fParameters.CheckConsistency();
  }
  catch (const std::logic_error& err) {
    LOG(error) << "L1InitManager: parameters container consistency check failed. Reason: " << err.what();
    return false;
  }

  return true;
}

// ----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::InitTargetField(double zStep)
{
  if (fInitController.GetFlag(EInitKey::kPrimaryVertexField)) {
    LOG(warn) << "L1InitManager::InitTargetField: attempt to reinitialize the field value and field region "
              << "near target. Ignore";
    return;
  }

  // Check for field function
  L1MASSERT(0, fInitController.GetFlag(EInitKey::kFieldFunction),
            "Attempt to initialize the field value and field region near target before initializing field function");

  // Check for target defined
  L1MASSERT(
    0, fInitController.GetFlag(EInitKey::kTargetPos),
    "Attempt to initialize the field value and field region near target before the target position initialization");

  constexpr int nDimensions {3};
  constexpr int nPointsNodal {3};

  std::array<double, nPointsNodal> inputNodalZ {fTargetZ, fTargetZ + zStep, fTargetZ + 2. * zStep};
  std::array<L1FieldValue, nPointsNodal> B {};
  std::array<fvec, nPointsNodal> z {};
  // loop over nodal points
  for (int idx = 0; idx < nPointsNodal; ++idx) {
    double point[nDimensions] {0., 0., inputNodalZ[idx]};
    double field[nDimensions] {};
    fFieldFunction(point, field);
    z[idx]   = inputNodalZ[idx];
    B[idx].x = field[0];
    B[idx].y = field[1];
    B[idx].z = field[2];
  }  // loop over nodal points: end
  fParameters.fVertexFieldRegion.Set(B[0], z[0], B[1], z[1], B[2], z[2]);
  fParameters.fVertexFieldValue = B[0];

  fInitController.SetFlag(EInitKey::kPrimaryVertexField);
}

// ----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::PushBackCAIteration(const L1CAIteration& iteration)
{
  // TODO: probably some checks must be inserted here (S.Zharko)
  bool control = fInitController.GetFlag(EInitKey::kCAIterationsNumberCrosscheck);
  //std::cout << "L1InitManager::PushBackCAIteration " << control << '\n';
  L1MASSERT(0, control,  //fInitController.GetFlag(EInitKey::kCAIterationsNumberCrosscheck),
            "Attempt to push back a CA track finder iteration before the number of iterations was defined");

  fParameters.fCAIterations.push_back(iteration);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::ReadParametersObject(const std::string& fileName)
{
  // Open input binary file
  std::ifstream ifs(fileName, std::ios::binary);
  if (!ifs) { LOG(fatal) << "L1InitManager: parameters data file \"" << fileName << "\" was not found"; }

  // Get L1InputData object
  try {
    boost::archive::binary_iarchive ia(ifs);
    ia >> fParameters;
  }
  catch (const std::exception&) {
    LOG(fatal) << "L1InitManager: parameters file \"" << fileName << "\" has incorrect data format or was corrupted";
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
bool L1InitManager::SendParameters(L1Algo* pAlgo)
{
  assert(pAlgo);
  pAlgo->ReceiveParameters(std::move(fParameters));
  return true;
}

// ----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetActiveDetectorIDs(const L1DetectorIDSet_t& detectorIDs)
{
  // TODO: To think about redefinition possibilities: should it be allowed or not? (S.Zh.)
  fActiveDetectorIDs = detectorIDs;
  fInitController.SetFlag(EInitKey::kActiveDetectorIDs);
}

// ----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetCAIterationsNumberCrosscheck(int nIterations)
{
  fCAIterationsNumberCrosscheck                = nIterations;
  L1Vector<L1CAIteration>& iterationsContainer = fParameters.fCAIterations;

  // NOTE: should be called to prevent multiple copies of objects between the memory reallocations
  iterationsContainer.reserve(nIterations);
  fInitController.SetFlag(EInitKey::kCAIterationsNumberCrosscheck);
}

// ----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetFieldFunction(const L1FieldFunction_t& fieldFunction)
{
  if (!fInitController.GetFlag(EInitKey::kFieldFunction)) {
    fFieldFunction = fieldFunction;
    fInitController.SetFlag(EInitKey::kFieldFunction);
  }
  else {
    LOG(warn) << "L1InitManager::SetFieldFunction: attempt to reinitialize the field function. Ignored";
  }
}

// ----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetGhostSuppression(int ghostSuppression)
{
  if (fInitController.GetFlag(EInitKey::kGhostSuppression)) {
    LOG(warn) << "L1InitManager::SetGhostSuppression: attempt of reinitializating the ghost suppresion flag. Ignore";
    return;
  }
  fParameters.fGhostSuppression = ghostSuppression;
  fInitController.SetFlag(EInitKey::kGhostSuppression);
}

// ----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetMomentumCutOff(float momentumCutOff)
{
  if (fInitController.GetFlag(EInitKey::kMomentumCutOff)) {
    LOG(warn) << "L1InitManager::SetMomentumCutOff: attempt of reinitializating the momentum cutoff value. Ignore";
    return;
  }
  fParameters.fMomentumCutOff = momentumCutOff;
  fInitController.SetFlag(EInitKey::kMomentumCutOff);
}

// ----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetNstations(L1DetectorID detectorID, int nStations)
{
  L1MASSERT(0, fInitController.GetFlag(EInitKey::kActiveDetectorIDs),
            "Attempt to set crosscheck number of stations before the active detetors set had been initialized");

  // NOTE: We add and check only those detectors which will be active (?)
  // For INACTIVE detectors the initialization code for it inside CbmL1/BmnL1 can (and must) be still in,
  // but it will be ignored inside L1InitManager.
  if (fActiveDetectorIDs.find(detectorID) != fActiveDetectorIDs.end()) {
    if (nStations) {
      fParameters.fNstationsGeometry[static_cast<L1DetectorID_t>(detectorID)] = nStations;
      fParameters.fNstationsActive[static_cast<L1DetectorID_t>(detectorID)]   = nStations;
    }
    else {
      // TODO: Probably it is better to replace fatal with warn and remove the detectorID from active detectors (S.Zharko)
      LOG(fatal) << "L1InitManager::SetNstations: attempt to initialize zero stations for active detector: "
                 << static_cast<L1DetectorID_t>(detectorID);
    }
  }

  // Check if all the station numbers for active detectors are initialized now:
  LOG(debug) << "L1InitManager::SetNstations called for detectorID = " << static_cast<int>(detectorID);
  if (!fInitController.GetFlag(EInitKey::kStationsNumberCrosscheck)) {
    bool ifInitialized = true;
    for (auto item : fActiveDetectorIDs) {
      if (fParameters.fNstationsGeometry[static_cast<L1DetectorID_t>(item)] == 0) {
        ifInitialized = false;
        break;
      }
    }
    fInitController.SetFlag(EInitKey::kStationsNumberCrosscheck, ifInitialized);
  }
  if (fInitController.GetFlag(EInitKey::kStationsNumberCrosscheck)) {
    fParameters.fNstationsGeometryTotal =
      std::accumulate(fParameters.fNstationsGeometry.begin(), fParameters.fNstationsGeometry.end(), 0);
    fParameters.fNstationsActiveTotal = fParameters.fNstationsGeometryTotal;
  }
}

// ----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetTargetPosition(double x, double y, double z)
{
  if (fInitController.GetFlag(EInitKey::kTargetPos)) {
    LOG(warn) << "L1InitManager::SetTargetPosition: attempt to reinitialize the target position. Ignore";
    return;
  }

  /// Fill fvec target fields
  fParameters.fTargetPos[0] = x;
  fParameters.fTargetPos[1] = y;
  fParameters.fTargetPos[2] = z;
  /// Set additional field for z component in double precision
  fTargetZ = z;
  fInitController.SetFlag(EInitKey::kTargetPos);
}

// ----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetTrackingLevel(int trackingLevel)
{
  if (fInitController.GetFlag(EInitKey::kTrackingLevel)) {
    LOG(warn) << "L1InitManager::SetTrackingLevel: attempt of reinitialization the tracking level. Ignore";
    return;
  }
  fParameters.fTrackingLevel = trackingLevel;
  fInitController.SetFlag(EInitKey::kTrackingLevel);
}

// ----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::WriteParametersObject(const std::string& fileName) const
{
  // Open output binary file
  std::ofstream ofs(fileName, std::ios::binary);
  if (!ofs) {
    LOG(error) << "L1InitManager: failed opening file \"" << fileName << " for writing parameters object\"";
    return;
  }

  // Serialize L1Parameters object and write
  boost::archive::binary_oarchive oa(ofs);
  oa << fParameters;
}

//
// INIT CHECKERS
//

// ----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::CheckCAIterationsInit()
{
  //
  // 1) Check number of iterations
  //
  bool ifInitPassed = true;
  if (!fInitController.GetFlag(EInitKey::kCAIterations)) {
    int nIterationsActual   = fParameters.fCAIterations.size();
    int nIterationsExpected = fCAIterationsNumberCrosscheck;
    if (nIterationsActual != nIterationsExpected) {
      LOG(warn) << "L1InitManager::CheckCAIterations: incorrect number of iterations registered: " << nIterationsActual
                << " of " << nIterationsExpected << " expected";
      ifInitPassed = false;
    }
  }
  fInitController.SetFlag(EInitKey::kCAIterations, ifInitPassed);
}

// ----------------------------------------------------------------------------------------------------------------------
// TODO: REWRITE! and add const qualifier (S.Zharko)
void L1InitManager::CheckStationsInfoInit()
{
  bool ifInitPassed = true;
  if (!fInitController.GetFlag(EInitKey::kStationsInfo)) {
    //
    // 1) Check numbers of stations passed
    //
    // loop over active detectors
    for (auto itemDetector : fActiveDetectorIDs) {
      auto selectDetector = [&itemDetector](const L1BaseStationInfo& station) {
        return station.GetDetectorID() == itemDetector;
      };
      int nStationsExpected = GetNstationsActive(itemDetector);
      int nStations         = std::count_if(fStationsInfo.begin(), fStationsInfo.end(), selectDetector);
      if (nStations != nStationsExpected) {
        LOG(error) << "L1InitManager::CheckStationsInfoInit: Incorrect number of L1BaseStationInfo objects passed"
                   << " to the L1Manager for L1DetectorID = " << static_cast<int>(itemDetector) << ": " << nStations
                   << " of " << nStationsExpected << " expected";
        ifInitPassed = false;
      }
    }  // loop over active detectors: end
    L1MASSERT(0, ifInitPassed, "Station info initialization failed");

    //
    // 2) Check for maximum allowed number of stations
    //
    int nStationsTotal = fParameters.fNstationsGeometryTotal;
    if (nStationsTotal > L1Constants::size::kMaxNstations) {
      LOG(fatal) << "Actual total number of registered stations in geometry (" << nStationsTotal
                 << ") is larger then possible (" << L1Constants::size::kMaxNstations
                 << "). Please, select another set of active tracking detectors or recompile the code with enlarged"
                 << " L1Constants::size::kMaxNstations value";
    }
  }

  fInitController.SetFlag(EInitKey::kStationsInfo, ifInitPassed);
}
