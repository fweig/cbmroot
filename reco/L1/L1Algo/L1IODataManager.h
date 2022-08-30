/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file L1IODataManager.h
/// \brief Input-output data manager for L1 tracking algorithm
/// \since 05.08.2022
/// \author S.Zharko <s.zharko@gsi.de>

#ifndef L1IODataManager_h
#define L1IODataManager_h 1

#include "L1Constants.h"
#include "L1InputData.h"

class L1Parameters;
class L1Algo;
//class L1OutputData;

/// Class L1IODataManager defines the interface for input and output data flow in the L1 tracking algorithm
///
class alignas(L1Constants::misc::kAlignment) L1IODataManager {
public:
  // ***************************
  // ** Member functions list **
  // ***************************

  // ** Constructors and destructor **

  /// Default constructor
  L1IODataManager() = delete;

  /// Constructor
  /// \param fpParameters  Pointer to the L1 tracking algorithm parameters instance
  L1IODataManager(const L1Parameters* pParameters);

  /// Destructor
  ~L1IODataManager() = default;

  /// Copy constructor
  L1IODataManager(const L1IODataManager& other) = delete;

  /// Move constructor
  L1IODataManager(L1IODataManager&& other) = delete;

  /// Copy assignment operator
  L1IODataManager& operator=(const L1IODataManager& other) = delete;

  /// Move assignment operator
  L1IODataManager& operator=(L1IODataManager&& other) = delete;

  /// Reads input data object from boost-serialized binary file
  /// \param  fileName  Name of input file
  void ReadInputData(const std::string& fileName);

  /// Reserve number of hits
  /// \param  nHits  Number of hits to be stored
  /// \note   If one does not call this method, the underlying vector of hits will be filled with the time penalty
  void ReserveNhits(L1HitIndex_t nHits) { fInputData.fvHits.reserve(nHits); }

  /// Resets the input data block
  void ResetInputData() noexcept;

  /// Pushes back a hit
  /// \param  hit  An L1Hit object
  void PushBackHit(const L1Hit& hit) { fInputData.fvHits.push_back(hit); }

  /// Sets the number of hit keys
  /// \param  nKeys  Number of hit keys
  void SetNhitKeys(int nKeys) { fInputData.fNhitKeys = nKeys; }

  /// Sends (moves) input data to the destination reference
  /// \param  pAlgo  Pointer to the L1 tracking algorithm main class
  /// \return Success flag
  bool SendInputData(L1Algo* pAlgo);

  /// Writes input data object to boost-serialized binary file
  /// \param  fileName  Name of input file
  void WriteInputData(const std::string& fileName) const;

private:
  /// Sets the start and stop indexes vs. station index
  void SetStartStopHitIndexes();

  /// Provides quick QA for input data
  /// \tparam  Level  The level of the checks. The values of the parameter:
  ///                 - 0: no checks will be done
  ///                 - 1: only number of hits and strips as well as validity of hits first and last indexes will be checked
  ///                 - 2: hits sorting is checked
  ///                 - 3: every hit is checked for consistency
  /// \note    The larger Level corresponds to more precise checks, but is followed by larger time penalty
  template<int Level>
  bool CheckInputData() const;


  // ***************************
  // ** Member variables list **
  // ***************************

  L1InputData fInputData {};  ///< Object of input data

  const L1Parameters* fpParameters = nullptr;  ///< Pointer to the tracking parameters object
};


// *************************************
// ** Inline functions implementation **
// *************************************

// ---------------------------------------------------------------------------------------------------------------------
//

// TODO: Complete this function
template<int Level>
inline bool L1IODataManager::CheckInputData() const
{
  if constexpr (Level == 0) { return true; }  // Level = 0 -> do nothing
  else if constexpr (Level > 0) {             // Level = 1 and higher
    // ----- Check if the hits container is not empty ------------------------------------------------------------------
    if (fInputData.fvHits.size() == 0) {
      LOG(warn) << "L1IODataManager [check input]: Sample contains empty hits, tracking will not be executed";
      return false;
    }

    // ----- Checks if the number of hit keys is valid -----------------------------------------------------------------
    if (fInputData.fNhitKeys < 1) {
      LOG(error) << "L1IODataManager [check input]: Incorrect number of keys passed (" << fInputData.fNhitKeys
                 << "), tracking will not be executed";
      return false;
    }

    // ----- Checks the indexes of first and last hits in stations
    // TODO: Add one of the two following checks for fvStartHitIn

    if constexpr (Level > 1) {  // Level = 2 and higher
      // ----- Checks for hits sorting ---------------------------------------------------------------------------------
      // TODO...
      if constexpr (Level > 2) {  // Level = 3 and higher
        // ----- Checks for consistency of the particular hit ----------------------------------------------------------
        // TODO...
      }
    }
    return true;
  }
  return true;
}


#endif  // L1IODataManager_h
