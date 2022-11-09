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
    constexpr int kCoeff                 = 64 / 4;                        ///< TODO:
    constexpr int kSingletPortionSize    = 1024 / kCoeff;                 ///< portion of left hits
    constexpr int kSingletPortionSizeVec = 1024 / kCoeff / fvec::size();  ///< portion of left hits per one vector word
    constexpr int kMaxPortionDoublets    = 10000 / 5 * 64 / 2 / kCoeff;   ///< Max size of the doublets portion
    constexpr int kMaxPortionTriplets    = 10000 * 5 * 64 / 2 / kCoeff;   ///< Max size of the triplets portion
    constexpr int kMaxPortionTripletsP   = kMaxPortionTriplets / fvec::size();  ///< Max size of the triplets portion
  }                                                                             // namespace size

  /// Control flags
  namespace control
  {
    /// Flag for calling the CAMergeClones procedure ... TODO
    constexpr bool kIfMergeClones {true};

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


  // Colors of terminal log
  namespace clrs
  {
    // NOTE: To be used first, because different users may have different console bg and fg colors
    constexpr char kCL[]   = "\e[0m";    ///< clear
    constexpr char kCLb[]  = "\e[1m";    ///< clear bold
    constexpr char kCLi[]  = "\e[3m";    ///< clear italic
    constexpr char kCLu[]  = "\e[4m";    ///< clear underline
    constexpr char kCLr[]  = "\e[7m";    ///< clear reverse
    constexpr char kCLbi[] = "\e[1;3m";  ///< clear bold-italic
    constexpr char kCLbu[] = "\e[1;4m";  ///< clear bold-underline
    constexpr char kCLbr[] = "\e[1;7m";  ///< clear bold-reverse

    // regular
    constexpr char kBK[] = "\e[30m";  ///< normal black
    constexpr char kRD[] = "\e[31m";  ///< normal red
    constexpr char kGN[] = "\e[32m";  ///< normal green
    constexpr char kYL[] = "\e[33m";  ///< normal yellow
    constexpr char kBL[] = "\e[34m";  ///< normal blue
    constexpr char kMG[] = "\e[35m";  ///< normal magenta
    constexpr char kCY[] = "\e[36m";  ///< normal cyan
    constexpr char kGY[] = "\e[37m";  ///< normal grey
    constexpr char kWT[] = "\e[38m";  ///< normal white

    // bold
    constexpr char kBKb[] = "\e[1;30m";  ///< bold black
    constexpr char kRDb[] = "\e[1;31m";  ///< bold red
    constexpr char kGNb[] = "\e[1;32m";  ///< bold green
    constexpr char kYLb[] = "\e[1;33m";  ///< bold yellow
    constexpr char kBLb[] = "\e[1;34m";  ///< bold blue
    constexpr char kMGb[] = "\e[1;35m";  ///< bold magenta
    constexpr char kCYb[] = "\e[1;36m";  ///< bold cyan
    constexpr char kGYb[] = "\e[1;37m";  ///< bold grey
    constexpr char kWTb[] = "\e[1;38m";  ///< bold white

    // italic
    constexpr char kBKi[] = "\e[3;30m";  ///< italic black
    constexpr char kRDi[] = "\e[3;31m";  ///< italic red
    constexpr char kGNi[] = "\e[3;32m";  ///< italic green
    constexpr char kYLi[] = "\e[3;33m";  ///< italic yellow
    constexpr char kBLi[] = "\e[3;34m";  ///< italic blue
    constexpr char kMGi[] = "\e[3;35m";  ///< italic magenta
    constexpr char kCYi[] = "\e[3;36m";  ///< italic cyan
    constexpr char kGYi[] = "\e[3;37m";  ///< italic grey
    constexpr char kWTi[] = "\e[3;38m";  ///< italic white

    // underline
    constexpr char kBKu[] = "\e[4;30m";  ///< underline black
    constexpr char kRDu[] = "\e[4;31m";  ///< underline red
    constexpr char kGNu[] = "\e[4;32m";  ///< underline green
    constexpr char kYLu[] = "\e[4;33m";  ///< underline yellow
    constexpr char kBLu[] = "\e[4;34m";  ///< underline blue
    constexpr char kMGu[] = "\e[4;35m";  ///< underline magenta
    constexpr char kCYu[] = "\e[4;36m";  ///< underline cyan
    constexpr char kGYu[] = "\e[4;37m";  ///< underline grey
    constexpr char kWTu[] = "\e[4;38m";  ///< underline white

    // reverse
    constexpr char kBKr[] = "\e[7;30m";  ///< reverse black
    constexpr char kRDr[] = "\e[7;31m";  ///< reverse red
    constexpr char kGNr[] = "\e[7;32m";  ///< reverse green
    constexpr char kYLr[] = "\e[7;33m";  ///< reverse yellow
    constexpr char kBLr[] = "\e[7;34m";  ///< reverse blue
    constexpr char kMGr[] = "\e[7;35m";  ///< reverse magenta
    constexpr char kCYr[] = "\e[7;36m";  ///< reverse cyan
    constexpr char kGYr[] = "\e[7;37m";  ///< reverse grey
    constexpr char kWTr[] = "\e[7;38m";  ///< reverse white

    // bold-underline
    constexpr char kBKbu[] = "\e[1;4;30m";  ///< bold-underline black
    constexpr char kRDbu[] = "\e[1;4;31m";  ///< bold-underline red
    constexpr char kGNbu[] = "\e[1;4;32m";  ///< bold-underline green
    constexpr char kYLbu[] = "\e[1;4;33m";  ///< bold-underline yellow
    constexpr char kBLbu[] = "\e[1;4;34m";  ///< bold-underline blue
    constexpr char kMGbu[] = "\e[1;4;35m";  ///< bold-underline magenta
    constexpr char kCYbu[] = "\e[1;4;36m";  ///< bold-underline cyan
    constexpr char kGYbu[] = "\e[1;4;37m";  ///< bold-underline grey
    constexpr char kWTbu[] = "\e[1;4;38m";  ///< bold-underline white

    // bold-italic
    constexpr char kBKbi[] = "\e[1;3;30m";  ///< bold-italic black
    constexpr char kRDbi[] = "\e[1;3;31m";  ///< bold-italic red
    constexpr char kGNbi[] = "\e[1;3;32m";  ///< bold-italic green
    constexpr char kYLbi[] = "\e[1;3;33m";  ///< bold-italic yellow
    constexpr char kBLbi[] = "\e[1;3;34m";  ///< bold-italic blue
    constexpr char kMGbi[] = "\e[1;3;35m";  ///< bold-italic magenta
    constexpr char kCYbi[] = "\e[1;3;36m";  ///< bold-italic cyan
    constexpr char kGYbi[] = "\e[1;3;37m";  ///< bold-italic grey
    constexpr char kWTbi[] = "\e[1;3;38m";  ///< bold-italic white

    // bold-reverse
    constexpr char kBKbr[] = "\e[1;7;30m";  ///< bold-reverse black
    constexpr char kRDbr[] = "\e[1;7;31m";  ///< bold-reverse red
    constexpr char kGNbr[] = "\e[1;7;32m";  ///< bold-reverse green
    constexpr char kYLbr[] = "\e[1;7;33m";  ///< bold-reverse yellow
    constexpr char kBLbr[] = "\e[1;7;34m";  ///< bold-reverse blue
    constexpr char kMGbr[] = "\e[1;7;35m";  ///< bold-reverse magenta
    constexpr char kCYbr[] = "\e[1;7;36m";  ///< bold-reverse cyan
    constexpr char kGYbr[] = "\e[1;7;37m";  ///< bold-reverse grey
    constexpr char kWTbr[] = "\e[1;7;38m";  ///< bold-reverse white
  }                                         // namespace clrs
}  // end namespace L1Constants


#endif  // L1Constants_h
