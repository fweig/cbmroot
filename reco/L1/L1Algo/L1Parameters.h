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

class L1Parameters {
public:
  //-------------------------------------------------------------------------------------------------------//
  //    Compile time constants                                                                             //
  //-------------------------------------------------------------------------------------------------------//
  /// Amount of coefficients in field approximations
  static constexpr int kMaxNFieldApproxCoefficients {21};  // TODO: May be it is better to use the polynomial
                                                           // order instead of this?
  /// Amount of bits to code one station
  static constexpr unsigned int kStationBits {6};
  /// Amount of bits to code one thread
  static constexpr unsigned int kThreadBits {6};
  /// Amount of bits to code one triple
  static constexpr unsigned int kTripletBits {32 - kStationBits - kThreadBits};

  /// Max number of stations
  static constexpr unsigned int kMaxNstations {1 << kStationBits};  // 2^6  = 64
  /// Max number of threads
  static constexpr unsigned int kMaxNthreads {1 << kThreadBits};  // 2^6  = 64
  /// Max number of triplets
  static constexpr unsigned int kMaxNtriplets {1 << kTripletBits};  // 2^20 = 1,048,576

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
  // TODO: create a map with parameters, their description and

public:
  //-------------------------------------------------------------------------------------------------------//
  //    Runtime constants                                                                                 //
  //-------------------------------------------------------------------------------------------------------//
  /// Sets upper-bound cut on max number of doublets per one singlet
  void SetMaxDoubletsPerSinglet(unsigned int value) { fMaxDoubletsPerSinglet = value; }
  /// Sets upper-bound cut on max number of triplets per one doublet
  void SetMaxTripletPerDoublets(unsigned int value) { fMaxTripletPerDoublets = value; }

  unsigned int GetMaxDoubletsPerSinglet() const { return fMaxDoubletsPerSinglet; }
  unsigned int GetMaxTripletPerDoublets() const { return fMaxTripletPerDoublets; }


private:
  ///
  unsigned int fMaxDoubletsPerSinglet {150};
  ///
  unsigned int fMaxTripletPerDoublets {15};
};

#endif  // L1Parameters_h
