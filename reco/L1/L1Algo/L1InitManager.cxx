/* Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file L1InitManager.cxx
 * @bried Input data management class for L1Algo
 * @since 19.01.2022
 ***********************************************************************************************************/

#include "L1InitManager.h"

#include <algorithm>
#include <sstream>

#include "L1Assert.h"
//-----------------------------------------------------------------------------------------------------------------------
//
L1InitManager::L1InitManager(L1Parameters* pParameters) : fpParameters(pParameters) {}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::AddStation(const L1BaseStationInfo& inStation)
{
  // Check if other fields were defined already
  // Active detector IDs
  L1MASSERT(0, fInitController.GetFlag(EInitKey::kActiveDetectorIDs),
            "Attempt to add a station info before the active detetors set had been initialized");

  // Number of stations check
  L1MASSERT(0, fInitController.GetFlag(EInitKey::kStationsNumberCrosscheck),
            "Attempt to add a station info before the numbers of stations for each detector had been initialized");

  // Field function
  L1MASSERT(0, fInitController.GetFlag(EInitKey::kFieldFunction),
            "Attempt to add a station info before the magnetic field function had been intialized");

  // Check activeness of this station type
  bool isStationActive =
    inStation.GetTrackingStatus() && fActiveDetectorIDs.find(inStation.GetDetectorID()) != fActiveDetectorIDs.end();
  if (isStationActive) {
    // initialize magnetic field slice
    L1BaseStationInfo inStationCopy = L1BaseStationInfo(inStation);  // make a copy of station so it can be initialized
    inStationCopy.SetFieldSlice(fFieldFunction);
    // Check station init
    {
      LOG(debug) << "L1InitManager::AddStation:(original) L1BaseStationInfo "
                 << inStation.GetInitController().ToString();
      LOG(debug) << "L1InitManager::AddStation:(copy)     L1BaseStationInfo "
                 << inStation.GetInitController().ToString();
      std::stringstream aStream;
      aStream << "Attempt to add an incompletely initialized station info object (detectorID = "
              << static_cast<int>(inStationCopy.GetDetectorID()) << ", stationID = " << inStationCopy.GetStationID()
              << ")";
      L1MASSERT(0, inStationCopy.GetInitController().IsFinalized(), aStream.str().c_str());
    }
    // insert the station in a set
    auto insertionResult = fStationsInfo.insert(std::move(inStationCopy));
    // Check insertion
    {
      std::stringstream aStream;
      aStream << "Attempt to add a dublicating station info object (detectorID = "
              << static_cast<int>(inStationCopy.GetDetectorID()) << ", stationID = " << inStationCopy.GetStationID()
              << ")";
      L1MASSERT(0, insertionResult.second, aStream.str().c_str());
    }
    int index = fStationsInfo.size() - 1 + 
      (fNstationsGeometry[fNstationsGeometry.size() - 1] - fNstationsActive[fNstationsActive.size() - 1]);
    fActiveStationGlobalIDs[index] = fStationsInfo.size() - 1;
  }
  else {
    int index = fStationsInfo.size() + 
      (fNstationsGeometry[fNstationsGeometry.size() - 1] - fNstationsActive[fNstationsActive.size() - 1]);
    fActiveStationGlobalIDs[index] = -1;
    fNstationsActive[static_cast<L1DetectorID_t>(inStation.GetDetectorID())]--;
    fNstationsActive[fNstationsActive.size() - 1]--;
  }
  LOG(debug) << "L1InitManager: adding a station with stationID = " << inStation.GetStationID()
             << " and detectorID = " << static_cast<int>(inStation.GetDetectorID())
             << ". Is active: " << isStationActive;
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::CheckInit()
{
  this->CheckCAIterationsInit();
  this->CheckStationsInfoInit();
}

//-----------------------------------------------------------------------------------------------------------------------
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
            "Attempt to initialze the field value and field region near target before initializing field function");

  // Check for target defined
  L1MASSERT(
    0, fInitController.GetFlag(EInitKey::kTargetPos),
    "Attempt to initialize the field value and field region near target before the target position initialization");

  constexpr int nDimensions {3};
  constexpr int nPointsNodal {3};

  std::array<double, nPointsNodal> inputNodalZ {fTargetPos[2], fTargetPos[2] + zStep, fTargetPos[2] + 2. * zStep};
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
  fTargetFieldRegion.Set(B[0], z[0], B[1], z[1], B[2], z[2]);
  fTargetFieldValue = B[0];

  fInitController.SetFlag(EInitKey::kPrimaryVertexField);
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::PrintCAIterations(int verbosityLevel) const
{
  for (const auto& iteration : fpParameters->CAIterationsContainer()) {
    iteration.Print(verbosityLevel);
  }
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::PrintStations(int verbosityLevel) const
{
  for (const auto& station : fStationsInfo) {
    station.Print(verbosityLevel);
  }
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::PushBackCAIteration(const L1CAIteration& iteration)
{
  // TODO: probably some checks must be inserted here (S.Zharko)
  bool control = fInitController.GetFlag(EInitKey::kCAIterationsNumberCrosscheck);
  //std::cout << "L1InitManager::PushBackCAIteration " << control << '\n';
  L1MASSERT(0, control,  //fInitController.GetFlag(EInitKey::kCAIterationsNumberCrosscheck),
            "Attempt to push back a CA track finder iteration before the number of iterations was defined");

  L1Vector<L1CAIteration>& iterationsContainer = fpParameters->CAIterationsContainer();
  iterationsContainer.push_back(iteration);
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetActiveDetectorIDs(const L1DetectorIDSet_t& detectorIDs)
{
  // TODO: To think about redifinition possibilities: should it be allowed or not? (S.Zh.)
  fActiveDetectorIDs = detectorIDs;
  fInitController.SetFlag(EInitKey::kActiveDetectorIDs);
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetCAIterationsNumberCrosscheck(int nIterations)
{
  fCAIterationsNumberCrosscheck                = nIterations;
  L1Vector<L1CAIteration>& iterationsContainer = fpParameters->CAIterationsContainer();

  // NOTE: should be called to prevent multiple copyings of objects between the memory realocations
  iterationsContainer.reserve(nIterations);
  fInitController.SetFlag(EInitKey::kCAIterationsNumberCrosscheck);
}

//-----------------------------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetGhostSuppression(int ghostSuppression)
{
  if (fInitController.GetFlag(EInitKey::kGhostSuppression)) {
    LOG(warn) << "L1InitManager::SetGhostSuppression: attempt of reinitializating the ghost suppresion flag. Ignore";
    return;
  }
  fGhostSuppression = ghostSuppression;
  fInitController.SetFlag(EInitKey::kGhostSuppression);
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetMomentumCutOff(float momentumCutOff)
{
  if (fInitController.GetFlag(EInitKey::kMomentumCutOff)) {
    LOG(warn) << "L1InitManager::SetMomentumCutOff: attempt of reinitializating the momentum cutoff value. Ignore";
    return;
  }
  fMomentumCutOff = momentumCutOff;
  fInitController.SetFlag(EInitKey::kMomentumCutOff);
}

//-----------------------------------------------------------------------------------------------------------------------
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
      fNstationsGeometry[static_cast<L1DetectorID_t>(detectorID)] = nStations;
      fNstationsActive[static_cast<L1DetectorID_t>(detectorID)]   = nStations;
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
      if (fNstationsGeometry[static_cast<L1DetectorID_t>(item)] == 0) {
        ifInitialized = false;
        break;
      }
    }
    fInitController.SetFlag(EInitKey::kStationsNumberCrosscheck, ifInitialized);
  }
  if (fInitController.GetFlag(EInitKey::kStationsNumberCrosscheck)) {
    fNstationsGeometry[L1Parameters::kMaxNdetectors] = std::accumulate(fNstationsGeometry.begin(), fNstationsGeometry.end() - 1, 0);
    fNstationsActive[L1Parameters::kMaxNdetectors] = fNstationsGeometry[L1Parameters::kMaxNdetectors];
  }
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetTargetPosition(double x, double y, double z)
{
  if (fInitController.GetFlag(EInitKey::kTargetPos)) {
    LOG(warn) << "L1InitManager::SetTargetPosition: attempt to reinitialize the target position. Ignore";
    return;
  }

  fTargetPos[0] = x;
  fTargetPos[1] = y;
  fTargetPos[2] = z;
  fInitController.SetFlag(EInitKey::kTargetPos);
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetTrackingLevel(int trackingLevel)
{
  if (fInitController.GetFlag(EInitKey::kTrackingLevel)) {
    LOG(warn) << "L1InitManager::SetTrackingLevel: attempt of reinitialization the tracking level. Ignore";
    return;
  }
  fTrackingLevel = trackingLevel;
  fInitController.SetFlag(EInitKey::kTrackingLevel);
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::TransferL1StationArray(std::array<L1Station, L1Parameters::kMaxNstations>& destinationArray)
{
  //
  // 1) Check, if all fields of this were initialized
  //
  {
    std::stringstream aStream;
    aStream << "Attempt to pass L1Station array to L1Algo core before all necessary fields initialization\n"
            << "L1InitManager " << fInitController.ToString();
    L1MASSERT(0, fInitController.IsFinalized(), aStream.str().c_str());
  }
  //
  // 2) Check, if destinationArraySize is enough for the transfer
  //
  {
    int nStationsTotal = this->GetNstationsActive();
    std::stringstream aStream;
    aStream << "Destination array size (" << destinationArray.size()
            << ") is smaller then the actual number of active tracking stations (" << nStationsTotal << ")";
    L1MASSERT(0, nStationsTotal <= static_cast<int>(destinationArray.size()), aStream.str().c_str());
  }

  auto destinationArrayIterator = destinationArray.begin();
  for (const auto& item : fStationsInfo) {
    *destinationArrayIterator = std::move(item.GetL1Station());
    ++destinationArrayIterator;
  }
  LOG(info) << "L1InitManager: L1Station vector was successfully transfered to L1Algo core :)";
}

//
// INIT CHECKERS
//

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::CheckCAIterationsInit()
{
  //
  // 1) Check number of iterations
  //
  bool ifInitPassed = true;
  if (!fInitController.GetFlag(EInitKey::kCAIterations)) {
    int nIterationsActual   = fpParameters->CAIterationsContainer().size();
    int nIterationsExpected = fCAIterationsNumberCrosscheck;
    if (nIterationsActual != nIterationsExpected) {
      LOG(warn) << "L1InitManager::CheckCAIterations: incorrect number of iterations registered: " << nIterationsActual
                << " of " << nIterationsExpected << " expected";
      ifInitPassed = false;
    }
  }
  fInitController.SetFlag(EInitKey::kCAIterations, ifInitPassed);
}

//-----------------------------------------------------------------------------------------------------------------------
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
    int nStationsTotal = fNstationsGeometry[fNstationsGeometry.size() - 1];
    if (nStationsTotal > L1Parameters::kMaxNstations) {
      std::stringstream aStream;
      aStream << "Actual total number of registered stations in geometry (" << nStationsTotal << ") is larger then possible ("
              << L1Parameters::kMaxNstations << "). Please, select another set of active tracking detectors or recompile the code with enlarged"
              << " L1Parameters::kMaxNstations value";
      // TODO: We have to provide an instruction of how to increase the kMaxNstations
      //       number keeping the code consistent (S.Zharko)
      ifInitPassed = false;
      L1MASSERT(0, false, aStream.str().c_str());
    }
  }
  fInitController.SetFlag(EInitKey::kStationsInfo, ifInitPassed);
}
