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
  L1HitIndex_t GetSampleSize() const { return fHits.size(); }


  // ** Accessors **

  /// Gets number of data streams
  int GetNdataStreams() const { return fStreamStartIndices.size(); }

  /// Gets reference to hit by its index
  /// \param  index  Index of hit in the hits sample
  const L1Hit& GetHit(L1HitIndex_t index) const { return fHits[index]; }

  /// Gets reference to hits vector
  const L1Vector<L1Hit>& GetHits() const { return fHits; }

  /// Gets number of hits in the hits vector
  L1HitIndex_t GetNhits() const { return fHits.size(); }

  /// Gets total number of stored keys
  int GetNhitKeys() const { return fNhitKeys; }

  /// Gets index of the first hit in the sorted hits vector
  /// \param iStream  Index of the data stream
  L1HitIndex_t GetStreamStartIndex(int iStream) const { return fStreamStartIndices[iStream]; }

  /// Gets index of (the last + 1) hit in the sorted hits vector
  /// \param iStream  Index of the data stream
  L1HitIndex_t GetStreamStopIndex(int iStream) const { return fStreamStopIndices[iStream]; }

  /// Gets n hits for the data stream
  /// \param iStream  Index of the data stream
  L1HitIndex_t GetStreamNhits(int iStream) const { return fStreamStopIndices[iStream] - fStreamStartIndices[iStream]; }


private:
  /// Swap method
  void Swap(L1InputData& other) noexcept;

  /// Data serialization method
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*versino*/)
  {
    ar& fHits;
    ar& fStreamStartIndices;
    ar& fStreamStopIndices;
    ar& fNhitKeys;
  }

  // ***************************
  // ** Member variables list **
  // ***************************

  /// @brief Sample of input hits
  L1Vector<L1Hit> fHits {"L1InputData::fHits"};

  /// @brief Index of the first hit in the sorted hits vector for a given data stream
  L1Vector<L1HitIndex_t> fStreamStartIndices {"L1InputData::fStreamStartIndices"};
  L1Vector<L1HitIndex_t> fStreamStopIndices {"L1InputData::fStreamStopIndices"};

  /// @brief Number of hit keys used for rejecting fake STS hits
  int fNhitKeys = -1;
};


// ********************************************
// ** Inline member functions initialization **
// *********************************************

// ---------------------------------------------------------------------------------------------------------------------
//
[[gnu::always_inline]] inline void L1InputData::Swap(L1InputData& other) noexcept
{
  std::swap(fHits, other.fHits);
  std::swap(fStreamStartIndices, other.fStreamStartIndices);
  std::swap(fStreamStopIndices, other.fStreamStopIndices);
  std::swap(fNhitKeys, other.fNhitKeys);
}

#endif  // L1InputData_h
