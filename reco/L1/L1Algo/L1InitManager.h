/* Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file L1InitManager.h
 * @bried Input data management class for L1Algo
 * @since 24.12.2021
 *
 ***********************************************************************************************************/

#include "L1BaseStationInfo.h"
#include "L1Parameters.h"
#include "L1Vector.h"

//#include <string>
#include <unordered_map>

/// Initialization manager for L1Aglo
///
/// The L1InitManager class provides interface for L1Algo initialization

class L1InitManager {
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
  void AddStation(const L1BaseStationInfo& inStation)
  {
    fvStationsInfo.push_back(inStation);
    ++fmStationsNumber[inStation.GetStationType()];
    // TODO: Think, how we would like to initialize station ID
  }
  /// Adds another station of a given type using pointer to a L1BaseStationInfo object
  void AddStation(const L1BaseStationInfo* inStationPtr)
  {
    fvStationsInfo.push_back(*inStationPtr);  // Copy is occured
    ++fmStationsNumber[inStationPtr->GetStationType()];
  }

  /// Prints out a list of stations
  void PrintStations(int verbosityLevel = 0)
  {
    if (verbosityLevel < 1) {
      for (auto& station : fvStationsInfo) {
        LOG(INFO) << "----------- station: ";
        LOG(INFO) << "\ttype = " << station.GetStationType();  // TMP
        LOG(INFO) << "\tz = " << station.GetZdouble(); 
      }
    }
    else {
      for (auto& station : fvStationsInfo) {
        station.Print();
      }
    }
  }

private:
  // TODO: TEST IT!!!!!!!!!!!!!!!!!!!!!
  L1Vector<L1BaseStationInfo> fvStationsInfo {"L1InitManager::fvStationsInfo"};  ///< Vector containing all the stations
  // TODO: Max stations is probably not optimal for vector initialization. Must think, what to do here. 
  std::unordered_map<int, int> fmStationsNumber;  ///< Number of each station type <station type, counter>
};
