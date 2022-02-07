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

//#include <iomanip>
//#include <sstream>
//#include <string>
//#include <unordered_map>

#include "L1Def.h"  // For EnumClassHash
#include "L1Vector.h"
//#include "utils/L1AlgoFunctions.h"

//#define L1ALGODEV

//---------------------------------------------------------------------------------------------------------//
//    Track finder iterations definition
//---------------------------------------------------------------------------------------------------------//

// TODO: Create independent class of L1CATrackFinderIterations ???


/////// > Will be moved to a separate class
/// All possible iteration species of Track Finder algorithm
// enum class L1CATrackFinderIter {
//   kFastPrim,       ///< primary fast tracks
//   kAllPrim,        ///< primary all tracks
//   kAllPrimJump,    ///< primary all tracks with jumped triplets
//   kAllSec,         ///< secondary all tracks
//   kAllPrimE,       ///< primary all electron tracks
//   kAllSecE,        ///< secondary all electron tracks
//   kFastPrimJump,   ///< primary fast tracks with jumped triplets
//   kAllSecJump      ///< secondary all tracks with jumped triplets
// };
//
// // Type for mapping L1CATrackFinder parameters, which are dependent from the current track finder iteration
// template <typename T>
// using L1CATrackFinderIterParameterMap_t = std::unordered_map<L1CATrackFinderIter, T, EnumClassHash>;
//
// ////// > Will be moved to separate class
// const L1CATrackFinderIterParameterMap_t<std::string> kTrackFinderIterNames = {
//   { L1CATrackFinderIter::kFastPrim,     "Primary tracks, fast" },
//   { L1CATrackFinderIter::kAllPrim,      "Primary tracks, all" },
//   { L1CATrackFinderIter::kAllPrimJump,  "Primary tracks with jumped triplets, all" },
//   { L1CATrackFinderIter::kAllSec,       "Secondary tracks, all" },
//   { L1CATrackFinderIter::kAllPrimE,     "Primary electron tracks, all" },
//   { L1CATrackFinderIter::kAllSecE,      "Secondary electron tracks, all" },
//   { L1CATrackFinderIter::kFastPrimJump, "Primary tracks with jumped triplets, fast" },
//   { L1CATrackFinderIter::kAllSecJump,   "Secondary tracks with jumped triplets, all" }
// };
//
// const L1CATrackFinderIterParameterMap_t<std::string> kTrackFinderIterNamesShort = {
//   { L1CATrackFinderIter::kFastPrim,     "FastPrim" },
//   { L1CATrackFinderIter::kAllPrim,      "AllPrim" },
//   { L1CATrackFinderIter::kAllPrimJump,  "AllPrimJump" },
//   { L1CATrackFinderIter::kAllSec,       "AllSec" },
//   { L1CATrackFinderIter::kAllPrimE,     "AllPrimE" },
//   { L1CATrackFinderIter::kAllSecE,      "AllSecE" },
//   { L1CATrackFinderIter::kFastPrimJump, "FastPrimJump" },
//   { L1CATrackFinderIter::kAllSecJump,   "AllSecJump" }
// };

class L1Parameters {
public:
  //-------------------------------------------------------------------------------------------------------//
  //    Compile time constants                                                                             //
  //-------------------------------------------------------------------------------------------------------//
  /// Amount of coefficients in field approximations
  static constexpr int kMaxNFieldApproxCoefficients {21};  // TODO: May be it is better to use the polynomial
                                                           // order instead of this?
  static constexpr int kMaxFieldApproxPolynomialOrder {
    5};  ///> Order of polynomial to approximate field in the vicinity of station plane

  static constexpr unsigned int kStationBits {6u};  ///> Amount of bits to code one station
  static constexpr unsigned int kThreadBits {6u};   ///> Amount of bits to code one thread
  static constexpr unsigned int kTripletBits {32u - kStationBits - kThreadBits};  ///> Amount of bits to code one triple

  static constexpr int kMaxNstations {1u << kStationBits};  ///> Max number of stations, 2^6  = 64
  static constexpr int kMaxNthreads {1u << kThreadBits};    ///> Max number of threads, 2^6  = 64
  static constexpr int kMaxNtriplets {1u << kTripletBits};  ///> Max number of triplets, 2^20 = 1,048,576

  static constexpr int kStandardIOWidth {15};  ///> Width of one output entry, passed to the std::setw()

  static constexpr int kAssertionLevel {0};  ///> Assertion level

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
    LOG(INFO) << "    Bits to code one station:          " << kStationBits;
    LOG(INFO) << "    Bits to code one thread:           " << kThreadBits;
    LOG(INFO) << "    Bits to code one triplet:          " << kTripletBits;
    LOG(INFO) << " #endif // L1ALGODEV   Max number of stations:            " << kMaxNstations;
    LOG(INFO) << "    Max number of threads:             " << kMaxNthreads;
    LOG(INFO) << "    Max number of triplets:            " << kMaxNtriplets;
    LOG(INFO) << "";
    LOG(INFO) << "  RUNTIME CONSTANTS (CUTS)";
    LOG(INFO) << "    Max number of doublets per singlet: " << fMaxDoubletsPerSinglet;
    LOG(INFO) << "    Max number of triplets per doublet: " << fMaxTripletPerDoublets;
    LOG(INFO) << "";
    LOG(INFO) << "  TRACK FINDER ITERATION DEPENDENT CONSTANTS";


    LOG(INFO) << "===================================================================================";
  }

  // TODO: change constant names with actual (human) names

public:
  //-------------------------------------------------------------------------------------------------------//
  //    Runtime constants                                                                                  //
  //-------------------------------------------------------------------------------------------------------//

  // ***** Setters ***** //
  /// Sets upper-bound cut on max number of doublets per one singlet
  void SetMaxDoubletsPerSinglet(unsigned int value) { fMaxDoubletsPerSinglet = value; }
  /// Sets upper-bound cut on max number of triplets per one doublet
  void SetMaxTripletPerDoublets(unsigned int value) { fMaxTripletPerDoublets = value; }

  // /// Sets track finder iteration sequence
  // void SetTrackFinderIterSequence(const L1Vector<L1CATrackFinderIter>& iterations) {
  //   fTrackFinderIterSequence = iterations;
  // }
  //
  // /// Sets track Chi2 upper cut
  // /// \param tfIteration Track Finder iteration of the L1 algorithm run
  // /// \param chi2Cut     Upper cut on track chi2 during selected iteration
  // void SetTrackChi2Cut(L1CATrackFinderIter tfIteration, float chi2Cut) { fTrackChi2Cut[tfIteration] = chi2Cut; }
  // void SetTrackChi2Cut(float chi2Cut) { SetSameValueToMap(chi2Cut, fTrackChi2Cut); }
  // void SetTrackChi2Cut(const L1CATrackFinderIterParameterMap_t<float>& newValues)
  // {
  //   SetMappedValuesToMap(newValues, fTrackChi2Cut);
  // }
  // /// Sets triplet Chi2 upper cut
  // /// \param tfIteration Track Finder iteration of the L1 algorithm run
  // /// \param chi2Cut     Upper cut on triplet chi2 during selected iteration
  // void SetTripletChi2Cut(L1CATrackFinderIter tfIteration, float chi2Cut) { fTripletChi2Cut[tfIteration] = chi2Cut; }
  // void SetTripletChi2Cut(float chi2Cut) { SetSameValueToMap(chi2Cut, fTripletChi2Cut); }
  // void SetTripletChi2Cut(const L1CATrackFinderIterParameterMap_t<float>& newValues)
  // {
  //   SetMappedValuesToMap(newValues, fTripletChi2Cut);
  // }
  // /// Sets triplet Chi2 upper cut
  // /// \param tfIteration Track Finder iteration of the L1 algorithm run
  // /// \param chi2Cut     Upper cut on triplet chi2 during selected iteration
  // void SetDoubletChi2Cut(L1CATrackFinderIter tfIteration, float chi2Cut) { fDoubletChi2Cut[tfIteration] = chi2Cut; }
  // void SetDoubletChi2Cut(float chi2Cut) { SetSameValueToMap(chi2Cut, fDoubletChi2Cut); }
  // void SetDoubletChi2Cut(const L1CATrackFinderIterParameterMap_t<float>& newValues)
  // {
  //   SetMappedValuesToMap(newValues, fDoubletChi2Cut);
  // }
  //

  // ***** Getters ***** //
  unsigned int GetMaxDoubletsPerSinglet() const { return fMaxDoubletsPerSinglet; }
  unsigned int GetMaxTripletPerDoublets() const { return fMaxTripletPerDoublets; }

  // /// Gets track Chi2 upper cut
  // /// \param tfIteration Track Finder iteration of the L1 algorithm run
  // /// \return            Upper cut on track chi2 during selected iteration
  // float GetTrackChi2Cut(L1CATrackFinderIter tfIteration)   const { return fTrackChi2Cut.at(tfIteration); }
  // /// Gets triplet Chi2 upper cut
  // /// \param tfIteration Track Finder iteration of the L1 algorithm run
  // /// \return            Upper cut on triplet chi2 during selected iteration
  // float GetTripletChi2Cut(L1CATrackFinderIter tfIteration) const { return fTripletChi2Cut.at(tfIteration); }
  // /// Gets triplet Chi2 upper cut
  // /// \param tfIteration Track Finder iteration of the L1 algorithm run
  // /// \return            Upper cut on triplet chi2 during selected iteration
  // float GetDoubletChi2Cut(L1CATrackFinderIter tfIteration) const { return fDoubletChi2Cut.at(tfIteration); }


  //-------------------------------------------------------------------------------------------------------//
  //    Auxilary methods                                                                                   //
  //-------------------------------------------------------------------------------------------------------//
public:
  //void PrintTrackFinderIterSequence() const
  //{
  //  LOG(INFO) << "== L1Algo track finder iterations sequence ==========";
  //  LOG(INFO) << "";
  //  int idx = 0;
  //  for (auto& iteration: fTrackFinderIterSequence) {
  //    LOG(INFO) << "  " << std::setw(2) << std::setfill(' ') << idx << ") " << kTrackFinderIterNames.at(iteration);
  //    ++idx;
  //  }
  //  LOG(INFO) << "";
  //  LOG(INFO) << "=====================================================";
  //}

private:
  /// Aux method for printing mapped Track Finder parameters
  ///template <typename T> // NOTE: moved to L1AlgoFunctions.h
  ///void PrintTrackFinderIterParameters(const std::string& parName, const L1CATrackFinderIterParameterMap_t<T>& parMap)
  ///{
  ///  std::stringstream token; // Is used because LOG() automatically sets '\n' to the end of the string
  ///  token << "\t\t";
  ///  token << std::setw(35) << std::setfill(' ') << parName << ' ';
  ///  for ()
  ///}

private:
  unsigned int fMaxDoubletsPerSinglet {150};  ///<
  unsigned int fMaxTripletPerDoublets {15};   ///<

  // L1Vector<L1CATrackFinderIter> fTrackFinderIterSequence {}; ///< Sequence of iterations in the track finder algorithm

  // // TODO: Develope naming system for these constants
  // L1CATrackFinderIterParameterMap_t<float> fTrackChi2Cut {
  //   { L1CATrackFinderIter::kFastPrim,     10.f },
  //   { L1CATrackFinderIter::kAllPrim,      10.f },
  //   { L1CATrackFinderIter::kAllPrimJump,  10.f },
  //   { L1CATrackFinderIter::kAllSec,       10.f },
  //   { L1CATrackFinderIter::kAllPrimE,     10.f },
  //   { L1CATrackFinderIter::kAllSecE,      10.f },
  //   { L1CATrackFinderIter::kFastPrimJump, 10.f },
  //   { L1CATrackFinderIter::kAllSecJump,   10.f }
  // };
  // // TODO: What is the meaning of the following numbers?
  // //   23.4450 <- 7.815 * 3
  // //   18.7560 <- 6.252 * 3
  // //   11.3449 * 2. / 3.
  // //
  // L1CATrackFinderIterParameterMap_t<float> fTripletChi2Cut {
  //   { L1CATrackFinderIter::kFastPrim,     23.4450f }, // kFastPrimIter
  //   { L1CATrackFinderIter::kAllPrim,      23.4450f },
  //   { L1CATrackFinderIter::kAllPrimE,     23.4450f },
  //   { L1CATrackFinderIter::kAllPrimJump,  18.7560f },
  //   { L1CATrackFinderIter::kAllSec,       18.7560f },
  //   { L1CATrackFinderIter::kAllSecE,      18.7560f },
  //   { L1CATrackFinderIter::kFastPrimJump, 21.1075f },
  //   { L1CATrackFinderIter::kAllSecJump,   21.1075f }
  // };

  // L1CATrackFinderIterParameterMap_t<float> fDoubletChi2Cut {
  //   { L1CATrackFinderIter::kFastPrim,     11.3449 * 2.f / 3.f },
  //   { L1CATrackFinderIter::kAllPrim,      11.3449 * 2.f / 3.f },
  //   { L1CATrackFinderIter::kAllPrimJump,  11.3449 * 2.f / 3.f },
  //   { L1CATrackFinderIter::kAllSec,       11.3449 * 2.f / 3.f },
  //   { L1CATrackFinderIter::kAllPrimE,     11.3449 * 2.f / 3.f },
  //   { L1CATrackFinderIter::kAllSecE,      11.3449 * 2.f / 3.f },
  //   { L1CATrackFinderIter::kFastPrimJump, 11.3449 * 2.f / 3.f },
  //   { L1CATrackFinderIter::kAllSecJump,   11.3449 * 2.f / 3.f }
  // };
};

#endif  // L1Parameters_h
