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


#include "L1CAIteration.h"
#include "L1Vector.h"
//#include "utils/L1AlgoFunctions.h"


class L1Parameters {
public:
  /**********************/ /**
   * COMPILE TIME CONSTANTS *
   **************************/

  //
  // Array sizes
  //

  /// Amount of coefficients in field approximations
  static constexpr int kMaxNFieldApproxCoefficients {21};  // TODO: May be it is better to use the polynomial
                                                           // order instead of this?
  /// Order of polynomial to approximate field in the vicinity of station plane
  static constexpr int kMaxFieldApproxPolynomialOrder {5};

  static constexpr unsigned int kStationBits {6u};  ///< Amount of bits to code one station
  static constexpr unsigned int kThreadBits {6u};   ///< Amount of bits to code one thread
  static constexpr unsigned int kTripletBits {32u - kStationBits - kThreadBits};  ///< Amount of bits to code one triple

  static constexpr int kMaxNdetectors {5};                  ///< Max number of tracking detectors
  static constexpr int kMaxNstations {1u << kStationBits};  ///< Max number of stations, 2^6  = 64
  static constexpr int kMaxNthreads {1u << kThreadBits};    ///< Max number of threads, 2^6  = 64
  static constexpr int kMaxNtriplets {1u << kTripletBits};  ///< Max number of triplets, 2^20 = 1,048,576

  static constexpr int kStandardIOWidth {15};  ///< Width of one output entry, passed to the std::setw()

  static constexpr int kAssertionLevel {0};  ///< Assertion level

  //
  // Compile control flags
  //

  /// Flag for the radiation length tables usage
  /// true - Radiational tables will be used,
  /// false - basic station material info is used
  static constexpr bool kIfUseRadLengthTable {true};

  /// Flag for calling the CAMergeClones procedure ... TODO
  static constexpr bool kIfMergeClones {true};

  /// Flag: debug mode for analyzing the doublets pergormance efficiencies
  static constexpr bool kIfDebugDoubletsPerformance {false};
  /// Flag: debug mode for analyzing the tiplets pergormance efficiencies
  static constexpr bool kIfDebugTripletsPerformance {false};
  /// Flag: debug mode for creating pools for triplets.
  /// NOTE: this feature will work only if the L1Parameters::kIfDebugTipletsPerformace is true!
  static constexpr bool kIfCreateTipletPulls {false};


public:
  /// Default constructor
  L1Parameters() = default;
  /// Destructor
  ~L1Parameters() = default;
  /// Copy constructor
  L1Parameters(const L1Parameters& other) = default;
  /// Copy assignment operator
  L1Parameters& operator=(const L1Parameters& other) = default;
  /// Move constructor
  L1Parameters(L1Parameters&& other) = default;
  /// Move assignment operator
  L1Parameters& operator=(L1Parameters&& other) = default;

  //
  // BASIC METHODS
  //

  /// Prints configuration
  void Print(int verbosityLevel = 0) const;
  /// String representation of the class contents
  std::string ToString(int indentLevel = 0) const;

  // TODO: change constant names with actual (human) names

  //-------------------------------------------------------------------------------------------------------//
  //    Runtime constants                                                                                  //
  //-------------------------------------------------------------------------------------------------------//

  //
  // SETTERS
  //
  /// Sets upper-bound cut on max number of doublets per one singlet
  void SetMaxDoubletsPerSinglet(unsigned int value) { fMaxDoubletsPerSinglet = value; }
  /// Sets upper-bound cut on max number of triplets per one doublet
  void SetMaxTripletPerDoublets(unsigned int value) { fMaxTripletPerDoublets = value; }

  //
  // GETTERS
  //
  /// Gets upper-bound cut on max number of doublets per one singlet
  unsigned int GetMaxDoubletsPerSinglet() const { return fMaxDoubletsPerSinglet; }
  /// Gets upper-bound cut on max number of triplets per one doublet
  unsigned int GetMaxTripletPerDoublets() const { return fMaxTripletPerDoublets; }

  /// Provides access to L1CAIteration vector (modifiable)
  L1Vector<L1CAIteration>& CAIterationsContainer() { return fCAIterationsContainer; }
  /// Provides access to L1CAIteration vector (const)
  const L1Vector<L1CAIteration>& CAIterationsContainer() const { return fCAIterationsContainer; }


private:
  unsigned int fMaxDoubletsPerSinglet {150};  ///< Upper-bound cut on max number of doublets per one singlet
  unsigned int fMaxTripletPerDoublets {15};   ///< Upper-bound cut on max number of triplets per one doublet

  L1Vector<L1CAIteration> fCAIterationsContainer {};  ///< L1 Track finder iterations vector
};

#endif  // L1Parameters_h
