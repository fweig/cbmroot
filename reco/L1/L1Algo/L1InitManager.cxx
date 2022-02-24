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

//-----------------------------------------------------------------------------------------------------------------------
//
L1InitManager::L1InitManager(L1Parameters* pParameters) : fpParameters(pParameters) {}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::AddStation(const L1BaseStationInfo& inStation)
{
  // Check if other fields were defined already
  // Active detector IDs
  if (!fInitController.GetFlag(InitKey::keActiveDetectorIDs)) {
    LOG(error) << "L1InitManager::AddStation: station initialization called before the active detectors set had been "
                  "initialized";
    assert((fInitController.GetFlag(InitKey::keActiveDetectorIDs)));
  }

  // Number of stations check
  if (!fInitController.GetFlag(InitKey::keStationsNumberCrosscheck)) {
    LOG(error)
      << "L1InitManager::AddStation: station initialization called before the numbers of stations for each detector "
      << "had been initialized";
    assert((fInitController.GetFlag(InitKey::keStationsNumberCrosscheck)));
  }

  // Field function
  if (!fInitController.GetFlag(InitKey::keFieldFunction)) {
    LOG(error)
      << "L1InitManager::AddStation: station initialization called before the magnetic field function was intialized";
    assert((fInitController.GetFlag(InitKey::keFieldFunction)));
  }

  // Check activeness of this station type
  bool isDetectorActive = fActiveDetectorIDs.find(inStation.GetDetectorID()) != fActiveDetectorIDs.end();
  if (isDetectorActive) {
    // initialize magnetic field slice
    L1BaseStationInfo inStationCopy = L1BaseStationInfo(inStation);  // make a copy of station so it can be initialized
    inStationCopy.SetFieldSlice(fFieldFunction);
    bool isStationInitialized = inStationCopy.GetInitController().IsFinalized();
    if (!isStationInitialized) {
      LOG(debug) << "L1InitManager::AddStation:(original) L1BaseStationInfo "
                 << inStation.GetInitController().ToString();
      LOG(debug) << "L1InitManager::AddStation:(copy)     L1BaseStationInfo "
                 << inStation.GetInitController().ToString();
      LOG(error) << "L1InitManager::AddStation: attempt to add incompletely initialized object with detectorID = "
                 << static_cast<int>(inStationCopy.GetDetectorID())
                 << " and stationID = " << inStationCopy.GetStationID();
      assert((isStationInitialized));
    }
    // insert the station in a set
    auto insertionResult = fStationsInfo.insert(std::move(inStationCopy));
    if (!insertionResult.second) {
      LOG(error) << "L1InitManager::AddStation: attempt to insert a dublicating L1BaseStationInfo with StationID = "
                 << inStation.GetStationID() << " and DetectorID = " << static_cast<int>(inStation.GetDetectorID())
                 << ":";
      LOG(error) << ">>> Already inserted L1BaseStationInfo object:";
      insertionResult.first->Print();
      LOG(error) << ">>> A dublicating L1BaseStationInfo object:";
      inStation.Print();
      assert((insertionResult.second));
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
  if (!fInitController.GetFlag(InitKey::keFieldFunction)) {
    LOG(error) << "L1InitManager::InitTargetField: attempt to initialze the field value and field region near "
               << "target before initializing field function";
    assert((fInitController.GetFlag(InitKey::keFieldFunction)));
  }

  // Check for target defined
  if (!fInitController.GetFlag(InitKey::keTargetPos)) {
    LOG(error) << "L1InitManager::InitTargetField: attempt to initialize the field value and field region near "
               << "target before the target position initialization";
    assert((fInitController.GetFlag(InitKey::keTargetPos)));
  }

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
  // TODO: probably some checks must be inserted here
  if (!fInitController.GetFlag(InitKey::keCAIterationsNumberCrosscheck)) {
    LOG(error) << "L1InitManager::PushBackCAIteration: attempt to push back a CA track finder iteration before the "
               << "number of iterations was defined";
    assert((fInitController.GetFlag(InitKey::keCAIterationsNumberCrosscheck)));
  }
  //fCAIterationsContainer.push_back(iteration);
  L1Vector<L1CAIteration>& iterationsContainer = fpParameters->CAIterationsContainer();
  iterationsContainer.push_back(iteration);
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetActiveDetectorIDs(const std::set<L1DetectorID>& detectorIDs)
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
  if (!fInitController.IsFinalized()) {
    LOG(error) << "L1InitManager::TransferL1StationArray: attempt to pass L1Station array to L1Algo core before "
               << "all necessary fields initialization";
    LOG(error) << "L1InitManager " << fInitController.ToString();
    assert((fInitController.IsFinalized()));
  }

  //
  // 2) Check, if destinationArraySize is enough for the transfer
  //
  int nStationsTotal            = this->GetStationsNumber();
  bool ifDestinationArraySizeOk = nStationsTotal <= static_cast<int>(destinationArray.size());
  if (!ifDestinationArraySizeOk) {
    LOG(error) << "L1InitManager::TransferL1StationArray: destination array size (" << destinationArray.size()
               << ") is smaller then actual number of active tracking stations (" << nStationsTotal << ")";
    assert((ifDestinationArraySizeOk));
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

    if (!ifInitPassed) {
      LOG(error) << "L1InitManager::IsStationsInfo: initialization failed";
      assert((ifInitPassed));
    }

    //
    // 2) Check for maximum allowed number of stations
    //
    int nStationsTotal = GetStationsNumber();
    if (nStationsTotal > L1Parameters::kMaxNstations) {
      LOG(fatal) << "Actual total number of registered stations (" << nStationsTotal
                 << ") is larger then designed one (" << L1Parameters::kMaxNstations
                 << "). Please, select another set of active tracking detectors";
      // TODO: We have to provide an instruction of how to increase the kMaxNstations
      //       number keeping the code consistent (S.Zharko)
      ifInitPassed = false;
      assert((nStationsTotal <= L1Parameters::kMaxNstations));
    }
  }
  fInitController.SetFlag(InitKey::keStationsInfo, ifInitPassed);
}
