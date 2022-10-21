/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   L1InputData.h
/// \brief  Structure for input data to the L1 tracking algorithm (declaration)
/// \since  08.08.2022
/// \author Sergei Zharko <s.zharko@gsi.de>

#ifndef L1InputData_h
#define L1InputData_h 1

#include <boost/serialization/access.hpp>
#include <boost/serialization/array.hpp>

#include <array>

#include "L1Constants.h"
#include "L1Hit.h"
#include "L1Vector.h"

/// Class L1InputData represents a block of the input data to the L1 tracking algorithm per event or time slice.
/// Filling of the L1InputData is carried out with L1IODataManager class
///
class alignas(L1Constants::misc::kAlignment) L1InputData {
public:
  // **************************
  // ** Friend classes list  **
  // **************************

  friend class L1IODataManager;  ///< Class which fills the L1InputData object for each event or time slice
  friend class boost::serialization::access;

  // ***************************
  // ** Member functions list **
  // ***************************

  // ** Constructors and destructor **

  /// Default constructor
  L1InputData();

  /// Destructor
  ~L1InputData() = default;

  /// Copy constructor
  L1InputData(const L1InputData& other);

  /// Move constructor
  L1InputData(L1InputData&&) noexcept;

  /// Copy assignment operator
  L1InputData& operator=(const L1InputData& other);

  /// Move assignment operator
  L1InputData& operator=(L1InputData&& other) noexcept;

  /// Gets hits sample size
  L1HitIndex_t GetSampleSize() const { return fvHits.size(); }


  // ** Accessors **

  /// Gets reference to hit by its index
  /// \param  index  Index of hit in the hits sample
  const L1Hit& GetHit(L1HitIndex_t index) const { return fvHits[index]; }

  /// Gets reference to hits vector
  const L1Vector<L1Hit>& GetHits() const { return fvHits; }

  /// Gets number of hits in the hits vector
  L1HitIndex_t GetNhits() const { return fvHits.size(); }

  /// Gets total number of stored keys
  int GetNhitKeys() const { return fNhitKeys; }

  /// Gets index of the first hit in the sorted hits vector
  /// \param iStation  Index of the tracking station in the active stations array
  L1HitIndex_t GetStartHitIndex(int iStation) const { return fvStartHitIndexes[iStation]; }

  /// Gets index of (the last + 1) hit in the sorted hits vector
  /// \param iStation  Index of the tracking station in the active stations array
  L1HitIndex_t GetStopHitIndex(int iStation) const { return fvStopHitIndexes[iStation]; }

  /// Gets n hits for the station
  /// \param iStation  Index of the tracking station in the active stations array
  L1HitIndex_t GetNhits(int iStation) const { return fvStopHitIndexes[iStation] - fvStartHitIndexes[iStation]; }


private:
  /// Swap method
  void Swap(L1InputData& other) noexcept;

  /// Data serialization method
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*versino*/)
  {
    ar& fvHits;
    ar& fvStartHitIndexes;
    ar& fvStopHitIndexes;
    ar& fNhitKeys;
  }

  // ***************************
  // ** Member variables list **
  // ***************************

  /// Sorted sample of input hits
  /// \note Hits in the vector are sorted as follows. Among two hits
  ///       the largest has the largest station index in the active
  ///       stations array. If both indexes were measured withing one
  ///       station, the largest hit has the largest y component of
  ///       the coordinates
  L1Vector<L1Hit> fvHits = {"L1InputData::fvHits"};

  /// Index of the first hit in the sorted hits vector for a given station
  std::array<L1HitIndex_t, L1Constants::size::kMaxNstations> fvStartHitIndexes = {0};

  /// Index of the last hit in the sorted hits vector for a given station
  std::array<L1HitIndex_t, L1Constants::size::kMaxNstations> fvStopHitIndexes = {0};

  /// Number of hit keys used for rejecting fake STS hits
  int fNhitKeys = -1;
};


// ********************************************
// ** Inline member functions initialization **
// *********************************************

// ---------------------------------------------------------------------------------------------------------------------
//
[[gnu::always_inline]] inline void L1InputData::Swap(L1InputData& other) noexcept
{
  std::swap(fvHits, other.fvHits);
  std::swap(fvStartHitIndexes, other.fvStartHitIndexes);
  std::swap(fvStopHitIndexes, other.fvStopHitIndexes);
  std::swap(fNhitKeys, other.fNhitKeys);
}

#endif  // L1InputData_h
