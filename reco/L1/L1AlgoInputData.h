/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak [committer], Valentina Akishina */

#ifndef _L1AlgoInputData_h
#define _L1AlgoInputData_h

#include <fstream>
#include <iostream>

#include "L1Def.h"
#include "L1Hit.h"
#include "L1Vector.h"

///
/// Class L1AlgoInputData implements a container for keeping a vector of L1Algo internal hit structures (the
/// L1Hit objects), used for track reconstruction procedure.
///
struct L1AlgoInputData {
  /*
   *  Funcitons
   */

  static constexpr int kMaxNStations {25};  ///> max number of stations to be passed to the L1AlgoInputData object
  // TODO: Why is it different to the value used in L1Algo (L1Constants::size::kMaxNstations)? (S.Zharko)

  /// Default constructor
  L1AlgoInputData()  = default;

  /// Default destructor
  ~L1AlgoInputData() = default;

  /// Resets the object
  /// For all the vectors the clear method is called, all other fields are set to zero
  void Clear();

  /// Gets number of the station strips
  int GetNstrips() const { return fNstrips; }
  /// Gives an access to the underlying vector of L1Hit objects
  L1Vector<L1Hit>& GetHits() { return vHits; }
  /// Gives an access to the vector of the strip flags
  L1Vector<unsigned char>& GetSFlag() { return fStripFlag; }

  /// Gets an access of the start indexes for different stations
  /// \return pointer to the first element of the array over the stations
  const L1HitIndex_t* GetHitsStartIndex() const { return HitsStartIndex; }
  /// Gets an access of the stop indexes for different stations
  /// \return pointer to the first element of the array over the stations
  const L1HitIndex_t* GetHitsStopIndex() const { return HitsStopIndex; }

  /// Reads a vector of L1Hit object from file
  /// The input text file must have the exactly "data_algo.txt" basename
  /// \param  work_dir  path to the file data_algo.txt file, containing the L1Hit objects for different events
  /// \param  maxNEvent max number of events to be read from the data_algo.txt file
  /// \param  iVerbose  verbosity level
  /// \return success flag:
  ///    true  - data was read and stored into this object
  ///    false - data was not read for some reason
  bool ReadHitsFromFile(const char work_dir[100], const int maxNEvent, const int iVerbose);

  //  void PrintHits();

  //
  // Redefinitions of the new\delete operators for using the alignment memmory
  //
  /// Placement new operator for single element
  void* operator new(size_t size, void* ptr) { return ::operator new(size, ptr); }

  /// Placement new operator for multiple elements
  void* operator new[](size_t size, void* ptr) { return ::operator new(size, ptr); }

  /// New operator for single element
  void* operator new(size_t size) { return _mm_malloc(size, 16); }

  /// New operator for multiple elements
  void* operator new[](size_t size) { return _mm_malloc(size, 16); }

  /// Delete operator for single element
  void operator delete(void* ptr, size_t) { _mm_free(ptr); }

  /// Delete operator for multiple elements
  void operator delete[](void* ptr, size_t) { _mm_free(ptr); }

  // TODO: Where are the definitions? (S.Zharko)

  /// Copy constructor
  L1AlgoInputData(const L1AlgoInputData& a);

  /// Copy assignment operator
  const L1AlgoInputData& operator=(const L1AlgoInputData& a);

  // private:
  //
  // functionality
  //
  /// Skips spaces in the input stream (TODO: is it used somewhere? (S.Zharko))
  static std::istream& eatwhite(std::istream& is);  // skip spaces
                                                    /// read data from data_algo.txt
  /*
   *  Data fields (public)
   */
  /// hits as a combination of front-, backstrips and z-position
  L1Vector<L1Hit> vHits {"L1AlgoInputData::vHits"};

  int fNstrips {0};  ///> Number of strips in the station
  /// information of hits station & used hits in tracks;
  L1Vector<unsigned char> fStripFlag {"L1AlgoInputData::fStripFlag"};

  /// Start indeces for a given station
  L1HitIndex_t HitsStartIndex[kMaxNStations + 1] {0};
  /// Stop indeces for a given station
  L1HitIndex_t HitsStopIndex[kMaxNStations + 1] {0};

} _fvecalignment;

#endif  // #ifndef _L1AlgoInputData_h
