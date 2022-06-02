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
#include "L1Constants.h"
#include "L1Vector.h"

//#include "utils/L1AlgoFunctions.h"

/// Class L1Parameters represents a container for all external parameters of the L1 tracking algorithm,
/// including geometry parameters and physics cuts. The instance of the L1Parameters is constructed inside
/// L1InitManager class and then moved to the L1Algo instance.
class L1Parameters {
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
