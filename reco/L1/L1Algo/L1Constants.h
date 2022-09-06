/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file   L1Constants.h
 * @brief  Compile-time constants definition for the L1 tracking algorithm
 * @since  02.06.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***********************************************************************************************************/

#ifndef L1Constants_h
#define L1Constants_h 1

#include "L1NaN.h"

/// Namespace contains compile-time constants definition for the L1 tracking algorithm
namespace L1Constants
{

  /// Array sizes
  namespace size
  {
    /// Amount of coefficients in field approximations
    constexpr int kMaxNFieldApproxCoefficients {21};  // TODO: May be it is better to use the polynomial
                                                      // order instead of this?
    /// Order of polynomial to approximate field in the vicinity of station plane
    constexpr int kMaxFieldApproxPolynomialOrder {5};

    /// Amount of bits to code a station, thread or triplet. This values determine the ma
    constexpr unsigned int kStationBits = 6u;                                ///< Amount of bits to code one station
    constexpr unsigned int kThreadBits  = 6u;                                ///< Amount of bits to code one thread
    constexpr unsigned int kTripletBits = 32u - kStationBits - kThreadBits;  ///< Amount of bits to code one triplet

    constexpr int kMaxNdetectors = 5;                   ///< Max number of tracking detectors
    constexpr int kMaxNstations  = 1u << kStationBits;  ///< Max number of stations, 2^6  = 64
    constexpr int kMaxNthreads   = 1u << kThreadBits;   ///< Max number of threads, 2^6  = 64
    constexpr int kMaxNtriplets  = 1u << kTripletBits;  ///< Max number of triplets, 2^20 = 1,048,576

    // TODO: Clarify the meaning of these coefficients
    constexpr int kCoeff               = 64 / 4;                        ///< TODO:
    constexpr int kPortionLeftHits     = 1024 / kCoeff;                 ///< portion of left hits
    constexpr int kPortionLeftHitsP    = 1024 / kCoeff / fvec::size();  ///< portion of left hits per one vector word
    constexpr int kMaxPortionDoublets  = 10000 / 5 * 64 / 2 / kCoeff;   ///< Max size of the doublets portion
    constexpr int kMaxPortionTriplets  = 10000 * 5 * 64 / 2 / kCoeff;   ///< Max size of the triplets portion
    constexpr int kMaxPortionTripletsP = kMaxPortionTriplets / fvec::size();  ///< Max size of the triplets portion
  }                                                                           // namespace size

  /// Control flags
  namespace control
  {
    /// Flag for the radiation length tables usage
    /// true  - material budget tables will be used,
    /// false - basic station material info is used
    constexpr bool kIfUseRadLengthTable {true};

    /// Flag for calling the CAMergeClones procedure ... TODO
    constexpr bool kIfMergeClones {true};

    /// Flag: hit errors
    /// true  - hit errors will be saved in the track extender algorithm
    constexpr bool kIfSaveHitErrorsInTrackExtender {false};

    /// Flag: input data QA level
    ///  - 0: no checks will be done
    ///  - 1: only number of hits and strips as well as validity of hits first and last indexes will be checked
    ///  - 2: hits sorting is checked
    ///  - 3: every hit is checked for consistency
    /// \note The larger Level corresponds to more precise checks, but is followed by larger time penalty
    constexpr int kInputDataQaLevel = 0;  // WARNING: other options does not work properly, more tests are needed!

  }  // namespace control

  /// Physics constants
  namespace phys
  {
    /* Particle masses used for track fit */
    constexpr float kMuonMass     = 0.10565800f;  ///< Muon mass     [GeV/c2]
    constexpr float kElectronMass = 0.000511f;    ///< Electron mass [GeV/c2]
  }                                               // namespace phys

  /// Miscellaneous constants
  namespace misc
  {
    constexpr int kAssertionLevel = 0;   ///< Assertion level
    constexpr int kAlignment      = 16;  ///< Default alignment of data (bytes)
  }                                      // namespace misc

  /// NoInit constants (aliases)
  namespace noin
  {
    constexpr float kF          = L1NaN::SetNaN<float>();
    constexpr double kD         = L1NaN::SetNaN<double>();
    constexpr int k32I          = L1NaN::SetNaN<int>();
    constexpr unsigned int k32U = L1NaN::SetNaN<unsigned int>();
  }  // namespace noin
}  // end namespace L1Constants


#endif  // L1Constants_h
