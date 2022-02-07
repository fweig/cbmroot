/* Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file L1InitManager.cxx
 * @bried Input data management class for L1Algo
 * @since 19.01.2022
 *
 ***********************************************************************************************************/

#include "L1InitManager.h"

#include <algorithm>


//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::AddStation(const L1BaseStationInfo& inStation)
{
  // Check if other fields were defined already
  // Active detector IDs
  if (!fInitFlags[L1InitManager::kEactiveDetectorIDs]) {
    LOG(error) << "L1InitManager::AddStation: station initialization called before the active detectors set had been "
                  "initialized";
    assert((fInitFlags[L1InitManager::kEactiveDetectorIDs]));
  }

  // Number of stations check
  if (!fInitFlags[L1InitManager::kEstationsNumberCrosscheck]) {
    LOG(error)
      << "L1InitManager::AddStation: station initialization called before the numbers of stations for each detector "
      << "had been initialized";
    assert((fInitFlags[L1InitManager::kEstationsNumberCrosscheck]));
  }

  // Field function
  if (!fInitFlags[L1InitManager::kEfieldFunction]) {
    LOG(error)
      << "L1InitManager::AddStation: station initialization called before the magnetic field function was intialized";
    assert((fInitFlags[L1InitManager::kEfieldFunction]));
  }

  // Check activeness of this station type
  bool isDetectorActive = fActiveDetectorIDs.find(inStation.GetDetectorID()) != fActiveDetectorIDs.end();
  if (isDetectorActive) {
    // initialize magnetic field slice
    L1BaseStationInfo inStationCopy = L1BaseStationInfo(inStation);  // make a copy of station so it can be initialized
    inStationCopy.SetFieldSlice(fFieldFunction);
    bool isStationInitialized = inStationCopy.IsInitialized();
    if (!isStationInitialized) {
      LOG(debug) << "L1InitManager::AddStation: station init flags (original)" << inStation.GetInitFlags();
      LOG(debug) << "L1InitManager::AddStation: station init flags (copy)    " << inStation.GetInitFlags();
      LOG(error) << "L1InitManager::AddStation: Trying to add incompletely initialized object with detectorID = "
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
      assert((insertionResult.second));  // TODO: rewrite the assertion
    }
  }
  LOG(debug) << "L1InitManager: adding a station with stationID = " << inStation.GetStationID()
             << " and detectorID = " << static_cast<int>(inStation.GetDetectorID())
             << ". Is active: " << isDetectorActive;
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::Init() const
{  // To be implemented
  // Plans:
  //  1. Must make a final check of the inititalization and turn on a corresponding trigger in L1Algo class to accept
  //     the incoming data
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::PrintStations(int verbosityLevel) const
{
  if (verbosityLevel < 1) {
    for (auto& station : fStationsInfo) {
      LOG(info) << "----------- station: ";
      LOG(info) << "\ttype = " << station.GetStationType();  // TMP
      LOG(info) << "\tz = " << station.GetZdouble();
    }
  }
  else {
    for (auto& station : fStationsInfo) {
      station.Print();
    }
  }
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::TransferL1StationArray(std::array<L1Station, L1Parameters::kMaxNstations>& destinationArray)
{
  /// First of all, we must check if L1Station was properly initialized
  // TODO: actually, false condition will never reached (must thing about it, may be remove assertions from
  // CheckStationInfo and leave only warnings and flag)
  bool ifStationsInitialized = CheckStationsInfo();
  if (!ifStationsInitialized) {
    LOG(error) << "L1InitManager::TransferL1StationArray: attempt to pass unitialized L1Station array to L1Algo core";
    assert((ifStationsInitialized));
  }

  /// Check if destinationArraySize is enough for the transfer
  int totalStationsNumber       = this->GetStationsNumber();
  bool ifDestinationArraySizeOk = totalStationsNumber <= static_cast<int>(destinationArray.size());
  if (!ifDestinationArraySizeOk) {
    LOG(error) << "L1InitManager::TransferL1StationArray: destination array size (" << destinationArray.size()
               << ") is smaller then actual number of active tracking stations (" << totalStationsNumber << ")";
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
// GETTERS
//

//-----------------------------------------------------------------------------------------------------------------------
//
int L1InitManager::GetStationsNumber(L1DetectorID detectorID) const
{
  auto ifDetectorIdDesired = [&detectorID](const L1BaseStationInfo& station) {
    return station.GetDetectorID() == detectorID;
  };
  return std::count_if(fStationsInfo.begin(), fStationsInfo.end(), ifDetectorIdDesired);
}

//
// SETTERS
//

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetActiveDetectorIDs(const std::set<L1DetectorID>& detectorIDs)
{
  // TODO: To think about redifinition possibilities: should it be allowed or not?
  fActiveDetectorIDs                             = detectorIDs;
  fInitFlags[L1InitManager::kEactiveDetectorIDs] = true;
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetFieldFunction(const std::function<void(const double (&xyz)[3], double (&B)[3])>& fieldFunction)
{
  if (!fInitFlags[L1InitManager::kEfieldFunction]) {
    fFieldFunction                             = fieldFunction;
    fInitFlags[L1InitManager::kEfieldFunction] = true;
  }
  else {
    LOG(warn) << "L1InitManager::SetFieldFunction: attemt to reinitialize the field function. Ignored";
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
  if (!fInitFlags[L1InitManager::kEstationsNumberCrosscheck]) {
    bool ifInitialized = true;
    for (auto item : fActiveDetectorIDs) {
      if (fStationsNumberCrosscheck.find(item) == fStationsNumberCrosscheck.end()) {
        LOG(warn) << "L1InitManager::SetStationsNumberCrosscheck: uninitialized number of stations for detectorID = "
                  << static_cast<int>(item);
        ifInitialized = false;
        break;
      }
    }
    fInitFlags[L1InitManager::kEstationsNumberCrosscheck] = ifInitialized;
  }
  LOG(debug) << "InitResult: " << fInitFlags[L1InitManager::kEstationsNumberCrosscheck];
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::SetTargetPosition(double x, double y, double z)
{ 
  if (fInitFlags[L1InitManager::kEtargetPos]) {
    LOG(warn) << "L1InitManager::SetTargetPosition: attempt to reinitialize the target position. Ignore";
    return;
  }

  fTargetPos[0] = x;
  fTargetPos[1] = y;
  fTargetPos[2] = z;
  fInitFlags[L1InitManager::kEtargetPos] = true;
}

//-----------------------------------------------------------------------------------------------------------------------
//
void L1InitManager::InitTargetField(double zStep)
{
  if (fInitFlags[L1InitManager::kEprimaryVertexField]) {
    LOG(warn) << "L1InitManager::InitTargetField: attempt to reinitialize the field value and field region "
              << "near target. Ignore";
    return;
  }

  // Check for field function
  if (!fInitFlags[L1InitManager::kEfieldFunction]) {
    LOG(error) << "L1InitManager::InitTargetField: attempt to initialze the field value and field region near "
               << "target before initializing field function";
    assert((fInitFlags[L1InitManager::kEfieldFunction]));
  }

  // Check for target defined
  if (!fInitFlags[L1InitManager::kEtargetPos]) {
    LOG(error) << "L1InitManager::InitTargetField: attempt to initialize the field value and field region near "
               << "target before the target position initialization";
    assert((fInitFlags[L1InitManager::kEtargetPos]));
  }

  constexpr int numberOfDimensions {3};
  constexpr int numberOfReferencePoints {3};

  std::array<double, numberOfReferencePoints> inputNodalZ { 
    fTargetPos[2], fTargetPos[2] + zStep, fTargetPos[2] + 2. * zStep };  
  std::array<L1FieldValue, numberOfReferencePoints> B = {};
  std::array<fvec, numberOfReferencePoints> z         = {};
  for (int idx = 0; idx < numberOfReferencePoints; ++idx) {
    double point[numberOfDimensions] = {0., 0., inputNodalZ[idx]};
    double field[numberOfDimensions] = {};
    fFieldFunction(point, field);
    z[idx] = inputNodalZ[idx];
    B[idx].x = field[0];
    B[idx].y = field[1];
    B[idx].z = field[2];
  }
  fTargetFieldRegion.Set(B[0], z[0], B[1], z[1], B[2], z[2]);
  fTargetFieldValue = B[0];

  fInitFlags[L1InitManager::kEprimaryVertexField] = true;
}


//-----------------------------------------------------------------------------------------------------------------------
//
bool L1InitManager::CheckStationsInfo()
{
  if (!fInitFlags[L1InitManager::kEstationsInfo]) {
    bool ifInitPassed = true;

    if (!fInitFlags[L1InitManager::kEifStationNumbersChecked]) {
      for (const auto& itemDetector : fActiveDetectorIDs) {
        int actualStationsNumber   = GetStationsNumber(itemDetector);
        int expectedStationsNumber = fStationsNumberCrosscheck.at(itemDetector);
        if (actualStationsNumber != expectedStationsNumber) {
          LOG(error) << "L1InitManager::CheckStationsInfo: Incorrect number of L1BaseStationInfo objects passed to the "
                        "L1Manager "
                     << "for L1DetectorID = " << static_cast<int>(itemDetector) << ": " << actualStationsNumber
                     << " of " << expectedStationsNumber << " expected";
          ifInitPassed = false;
        }
      }
      fInitFlags[L1InitManager::kEifStationNumbersChecked] = ifInitPassed;
    }
    if (!ifInitPassed) {
      LOG(error) << "L1InitManager::CheckStationsInfo: initialization failed";
      assert((ifInitPassed));
    }

    // Check for maximum allowed number of stations
    int totalStationsNumber = GetStationsNumber();
    if (totalStationsNumber > L1Parameters::kMaxNstations) {
      LOG(fatal) << "Actual total number of registered stations (" << totalStationsNumber
                 << ") is larger then designed one (" << L1Parameters::kMaxNstations
                 << "). Please, select another set of active tracking detectors";
      // TODO: We have to provide an instruction of how to increase the kMaxNstations number keeping the code consistent
      assert((totalStationsNumber <= L1Parameters::kMaxNstations));
    }

    fInitFlags[L1InitManager::kEstationsInfo] = true;
  }
  else {
    LOG(warn) << "L1InitManager: L1BaseStationInfo set has already been initialized";
  }
  // NOTE: we return a flag here to reduce a number of calls outside the funcition. In other hands we keep this flag
  // to be consistent with other class fields initialization rules
  return fInitFlags[L1InitManager::kEstationsInfo];
}
