/* Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file L1Parameters.h
 * @brief Parameter container for the L1Algo library
 * @since 19.12.2021
 *
 ***********************************************************************************************************/

#ifndef L1Parameters_h
#define L1Parameters_h 1

#include <FairLogger.h>
#include <string>
#include <unordered_map>
#include <iomanip>

#include "L1Vector.h"

//---------------------------------------------------------------------------------------------------------//
//    Track finder iterations definition
//---------------------------------------------------------------------------------------------------------//

// TODO: May be it is reasonable to create TrackFinderIterations class 

/// Different iterations for Track Finder running
enum class TrackFinderIter {
  kFastPrim,       ///< primary fast tracks
  kAllPrim,        ///< primary all tracks
  kAllPrimJump,    ///< primary all tracks with jumped triplets
  kAllSec,         ///< secondary all tracks
  kAllPrimE,       ///< primary all electron tracks
  kAllSecE,        ///< secondary all electron tracks 
  kFastPrimJump,   ///< primary fast tracks with jumped triplets
  kAllSecJump      ///< secondary all tracks with jumped triplets
};


const std::unordered_map<TrackFinderIter, std::string> kTrackFinderIterNames {
  { TrackFinderIter::kFastPrim,     "Primary tracks, fast" },
  { TrackFinderIter::kAllPrim,      "Primary tracks, all" },
  { TrackFinderIter::kAllPrimJump,  "Primary tracks with jumped triplets, all" },
  { TrackFinderIter::kAllSec,       "Secondary tracks, all" },
  { TrackFinderIter::kAllPrimE,     "Primary electron tracks, all" },
  { TrackFinderIter::kAllSecE,      "Secondary electron tracks, all" },
  { TrackFinderIter::kFastPrimJump, "Primary tracks with jumped triplets, fast" },
  { TrackFinderIter::kAllSecJump,   "Secondary tracks with jumped triplets, all" }
};

template <typename T>
using TrackFinderIterParameter_t = std::unordered_map<TrackFinderIter, T>;

class L1Parameters {
public:
  //-------------------------------------------------------------------------------------------------------//
  //    Compile time constants                                                                             //
  //-------------------------------------------------------------------------------------------------------//
  /// Amount of coefficients in field approximations
  static constexpr int kMaxNFieldApproxCoefficients {21};  // TODO: May be it is better to use the polynomial
                                                           // order instead of this?
  static constexpr int kMaxFieldApproxPolynomialOrder {5}; ///> Order of polynomial to approximate field in the vicinity of station plane

  static constexpr unsigned int kStationBits {6};  ///> Amount of bits to code one station
  static constexpr unsigned int kThreadBits {6};  ///> Amount of bits to code one thread
  static constexpr unsigned int kTripletBits {32 - kStationBits - kThreadBits};  ///> Amount of bits to code one triple

  static constexpr unsigned int kMaxNstations {1 << kStationBits};  ///> Max number of stations, 2^6  = 64
  static constexpr unsigned int kMaxNthreads {1 << kThreadBits};  ///> Max number of threads, 2^6  = 64
  static constexpr unsigned int kMaxNtriplets {1 << kTripletBits};  ///> Max number of triplets, 2^20 = 1,048,576

public:
  //-------------------------------------------------------------------------------------------------------//
  //    Basic methods                                                                                      //
  //-------------------------------------------------------------------------------------------------------//
  /// Default constructor
  L1Parameters() = default;
  /// Destructor
  ~L1Parameters() = default;
  /// Copy constructor
  L1Parameters(const L1Parameters& /*other*/) = default;
  /// Copy assignment operator
  L1Parameters& operator=(const L1Parameters& /*other*/) = default;
  /// Move constructor
  L1Parameters(L1Parameters&& /*other*/) = default;
  /// Move assignment operator
  L1Parameters& operator=(L1Parameters&& /*other*/) = default;
  /// Prints configuration
  void Print()
  {
    LOG(INFO) << "== L1Algo parameters ==============================================================";
    LOG(INFO) << "";
    LOG(INFO) << "  COMPILE TIME CONSTANTS";
    LOG(INFO) << "    Bits to code one station:          " << static_cast<unsigned int>(kStationBits);
    LOG(INFO) << "    Bits to code one thread:           " << static_cast<unsigned int>(kThreadBits);
    LOG(INFO) << "    Bits to code one triplet:          " << static_cast<unsigned int>(kTripletBits);
    LOG(INFO) << "    Max number of stations:            " << static_cast<unsigned int>(kMaxNstations);
    LOG(INFO) << "    Max number of threads:             " << static_cast<unsigned int>(kMaxNthreads);
    LOG(INFO) << "    Max number of triplets:            " << static_cast<unsigned int>(kMaxNtriplets);
    LOG(INFO) << "";
    LOG(INFO) << "  RUNTIME CONSTANTS (CUTS)";
    LOG(INFO) << "    Max number of doublets per singlet: " << fMaxDoubletsPerSinglet;
    LOG(INFO) << "    Max number of triplets per doublet: " << fMaxTripletPerDoublets;
    LOG(INFO) << "===================================================================================";
  }
  // TODO: change constant names with actual (human) names
  // TODO: create a map with parameters, their description etc.

public:
  //-------------------------------------------------------------------------------------------------------//
  //    Runtime constants                                                                                  //
  //-------------------------------------------------------------------------------------------------------//
  /// Sets upper-bound cut on max number of doublets per one singlet
  void SetMaxDoubletsPerSinglet(unsigned int value) { fMaxDoubletsPerSinglet = value; }
  /// Sets upper-bound cut on max number of triplets per one doublet
  void SetMaxTripletPerDoublets(unsigned int value) { fMaxTripletPerDoublets = value; }

  /// Sets track finder iteration sequence
  void SetTrackFinderIterSequence(const L1Vector<TrackFinderIter>& iterations) {
    fTrackFinderIterSequence = iterations;
  }
  

  unsigned int GetMaxDoubletsPerSinglet() const { return fMaxDoubletsPerSinglet; }
  unsigned int GetMaxTripletPerDoublets() const { return fMaxTripletPerDoublets; }

  //-------------------------------------------------------------------------------------------------------//
  //    Auxilary methods                                                                                   //
  //-------------------------------------------------------------------------------------------------------//
  
  void PrintTrackFinderIterationsSequence() const
  {  
    LOG(INFO) << "== L1Algo track finder iterations =================================================";
    LOG(INFO) << "";
    int idx = 0;
    for (auto& iteration: fTrackFinderIterSequence) {
      LOG(INFO) << "  " << std::setw(2) << std::setfill(' ') << idx << ' ' << kTrackFinderIterNames.at(iteration);
      ++idx;
    }
    LOG(INFO) << "";
    LOG(INFO) << "===================================================================================";

  }

private:
  unsigned int fMaxDoubletsPerSinglet {150};  ///<
  unsigned int fMaxTripletPerDoublets {15};  ///<

  L1Vector<TrackFinderIter> fTrackFinderIterSequence {}; ///< Sequence of iterations in the track finder algorithm 

  // TODO: Develope naming system for these constants

  std::unordered_map<TrackFinderIter, float> fTrackChi2Cut {
    { TrackFinderIter::kFastPrim,     10.f },
    { TrackFinderIter::kAllPrim,      10.f },
    { TrackFinderIter::kAllPrimJump,  10.f },
    { TrackFinderIter::kAllSec,       10.f },
    { TrackFinderIter::kAllPrimE,     10.f },
    { TrackFinderIter::kAllSecE,      10.f },
    { TrackFinderIter::kFastPrimJump, 10.f },
    { TrackFinderIter::kAllSecJump,   10.f }
  };

  std::unordered_map<TrackFinderIter, float> fTripletChi2Cut {
    { TrackFinderIter::kFastPrim,     5.f },
    { TrackFinderIter::kAllPrim,      5.f },
    { TrackFinderIter::kAllPrimJump,  5.f },
    { TrackFinderIter::kAllSec,       5.f },
    { TrackFinderIter::kAllPrimE,     5.f },
    { TrackFinderIter::kAllSecE,      5.f },
    { TrackFinderIter::kFastPrimJump, 5.f },
    { TrackFinderIter::kAllSecJump,   5.f }
  };

  std::unordered_map<TrackFinderIter, float> fDoubletChi2Cut {
    { TrackFinderIter::kFastPrim,     5.f },
    { TrackFinderIter::kAllPrim,      5.f },
    { TrackFinderIter::kAllPrimJump,  5.f },
    { TrackFinderIter::kAllSec,       5.f },
    { TrackFinderIter::kAllPrimE,     5.f },
    { TrackFinderIter::kAllSecE,      5.f },
    { TrackFinderIter::kFastPrimJump, 5.f },
    { TrackFinderIter::kAllSecJump,   5.f }
  };

  

  //float fTripletChi2Cut {5.f};
  //float fTripletChi2Cut {5.f};

};

#endif  // L1Parameters_h
