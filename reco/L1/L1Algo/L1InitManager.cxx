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

  L1MASSERT(0, fInitController.GetFlag(InitKey::keActiveDetectorIDs),
            "Attempt to add a station info before the active detetors set had been initialized");

  // Number of stations check
  L1MASSERT(0, fInitController.GetFlag(InitKey::keStationsNumberCrosscheck),
            "Attempt to add a station info before the numbers of stations for each detector had been initialized");

  // Field function
  L1MASSERT(0, fInitController.GetFlag(InitKey::keFieldFunction),
            "Attempt to add a station info before the magnetic field function had been intialized");

  // Check activeness of this station type
  bool isDetectorActive = fActiveDetectorIDs.find(inStation.GetDetectorID()) != fActiveDetectorIDs.end();
  if (isDetectorActive) {
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
  }
  LOG(debug) << "L1InitManager: adding a station with stationID = " << inStation.GetStationID()
             << " and detectorID = " << static_cast<int>(inStation.GetDetectorID())
             << ". Is active: " << isDetectorActive;
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
int L1InitManager::GetStationsNumber(L1DetectorID detectorID) const
{
  auto ifDetectorIdDesired = [&detectorID](const L1BaseStationInfo& station) {
    return station.GetDetectorID() == detectorID;
  };
  return std::count_if(fStationsInfo.begin(), fStationsInfo.end(), ifDetectorIdDesired);
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::InitTargetField(double zStep)
{
  if (fInitController.GetFlag(InitKey::kePrimaryVertexField)) {
    LOG(warn) << "L1InitManager::InitTargetField: attempt to reinitialize the field value and field region "
              << "near target. Ignore";
    return;
  }

  // Check for field function
  L1MASSERT(0, fInitController.GetFlag(InitKey::keFieldFunction),
            "Attempt to initialze the field value and field region near target before initializing field function");

  // Check for target defined
  L1MASSERT(
    0, fInitController.GetFlag(InitKey::keTargetPos),
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

  fInitController.SetFlag(InitKey::kePrimaryVertexField);
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
  bool control = fInitController.GetFlag(InitKey::keCAIterationsNumberCrosscheck);
  //std::cout << "L1InitManager::PushBackCAIteration " << control << '\n';
  L1MASSERT(0, control, //fInitController.GetFlag(InitKey::keCAIterationsNumberCrosscheck),
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
  fInitController.SetFlag(InitKey::keActiveDetectorIDs);
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetCAIterationsNumberCrosscheck(int nIterations)
{
  fCAIterationsNumberCrosscheck                = nIterations;
  L1Vector<L1CAIteration>& iterationsContainer = fpParameters->CAIterationsContainer();

  // NOTE: should be called to prevent multiple copyings of objects between the memory realocations
  iterationsContainer.reserve(nIterations);
  fInitController.SetFlag(InitKey::keCAIterationsNumberCrosscheck);
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetFieldFunction(const L1FieldFunction_t& fieldFunction)
{
  if (!fInitController.GetFlag(InitKey::keFieldFunction)) {
    fFieldFunction = fieldFunction;
    fInitController.SetFlag(InitKey::keFieldFunction);
  }
  else {
    LOG(warn) << "L1InitManager::SetFieldFunction: attempt to reinitialize the field function. Ignored";
  }
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetStationsNumberCrosscheck(L1DetectorID detectorID, int nStations)
{
  // NOTE: We add and check only those detectors which will be active (?)
  // For INACTIVE detectors the initialization code for it inside CbmL1/BmnL1 can (and must) be still in,
  // but it will be ignored inside L1InitManager.
  if (fActiveDetectorIDs.find(detectorID) != fActiveDetectorIDs.end()) {
    fStationsNumberCrosscheck[detectorID] = nStations;
  }

  // Check if all the station numbers for active detectors are initialized now:
  LOG(debug) << "SetStationsNumberCrosscheck called for detectorID = " << static_cast<int>(detectorID);
  if (!fInitController.GetFlag(InitKey::keStationsNumberCrosscheck)) {
    bool ifInitialized = true;
    for (auto item : fActiveDetectorIDs) {
      if (fStationsNumberCrosscheck.find(item) == fStationsNumberCrosscheck.end()) {
        ifInitialized = false;
        break;
      }
    }
    fInitController.SetFlag(InitKey::keStationsNumberCrosscheck, ifInitialized);
  }
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetTargetPosition(double x, double y, double z)
{
  if (fInitController.GetFlag(InitKey::keTargetPos)) {
    LOG(warn) << "L1InitManager::SetTargetPosition: attempt to reinitialize the target position. Ignore";
    return;
  }

  fTargetPos[0] = x;
  fTargetPos[1] = y;
  fTargetPos[2] = z;
  fInitController.SetFlag(InitKey::keTargetPos);
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
    int nStationsTotal = this->GetStationsNumber();
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
  if (!fInitController.GetFlag(InitKey::keCAIterations)) {
    int nIterationsActual   = fpParameters->CAIterationsContainer().size();
    int nIterationsExpected = fCAIterationsNumberCrosscheck;
    if (nIterationsActual != nIterationsExpected) {
      LOG(warn) << "L1InitManager::CheckCAIterations: incorrect number of iterations registered: " << nIterationsActual
                << " of " << nIterationsExpected << " expected";
      ifInitPassed = false;
    }
  }
  fInitController.SetFlag(InitKey::keCAIterations, ifInitPassed);
}

//-----------------------------------------------------------------------------------------------------------------------
// TODO: REWRITE! and add const qualifier (S.Zharko)
void L1InitManager::CheckStationsInfoInit()
{
  bool ifInitPassed = true;
  if (!fInitController.GetFlag(InitKey::keStationsInfo)) {
    //
    // 1) Check numbers of stations passed
    //
    // loop over active detectors
    for (const auto& itemDetector : fActiveDetectorIDs) {
      int nStationsActual   = GetStationsNumber(itemDetector);
      int nStationsExpected = fStationsNumberCrosscheck.at(itemDetector);
      if (nStationsActual != nStationsExpected) {
        LOG(error) << "L1InitManager::IsStationsInfoInitialized: Incorrect number of L1BaseStationInfo objects passed"
                   << " to the L1Manager for L1DetectorID = " << static_cast<int>(itemDetector) << ": "
                   << nStationsActual << " of " << nStationsExpected << " expected";
        ifInitPassed = false;
      }
    }  // loop over active detectors: end
    L1MASSERT(0, ifInitPassed, "Station info initialization failed");

    //
    // 2) Check for maximum allowed number of stations
    //
    int nStationsTotal = GetStationsNumber();
    if (nStationsTotal > L1Parameters::kMaxNstations) {
      std::stringstream aStream;
      aStream << "Actual total number of registered stations (" << nStationsTotal << ") is larger then designed one ("
              << L1Parameters::kMaxNstations << "). Please, select another set of active tracking detectors";
      // TODO: We have to provide an instruction of how to increase the kMaxNstations
      //       number keeping the code consistent (S.Zharko)
      ifInitPassed = false;
      L1MASSERT(0, false, aStream.str().c_str());
    }
  }
  fInitController.SetFlag(InitKey::keStationsInfo, ifInitPassed);
}
