/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Volker Friese [committer], Semen Lebedev */

/** @file CbmDefs.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 30.05.2017
 **
 ** Header for definition of CBM enumerators and constants
 **/

#ifndef CBMDEFS_H
#define CBMDEFS_H 1

#include <type_traits>  // for underlying_type

#include <iostream>  // for ostream
#include <string>

// Convert an element of enum class to its underlying intergral type
// since with C++11 the return type can't be deduced automatically it has
// to be put explicitely
// constexpr should result in a compile time evaluation of the function
// call where possible
// E.g. ToIntegralType(ECbmModuleId::KSts) should be evaluated at compile
// time and should not affect the run time performance at all
template<typename T>
constexpr auto ToIntegralType(T enumerator) -> typename std::underlying_type<T>::type
{
  return static_cast<typename std::underlying_type<T>::type>(enumerator);
}

/** Enumerator for module Identifiers. Modules can be active (detector systems)
 ** or passive (magnet, beam pipe, target etc.)
 ** In order to loop over all detectors, loop until kNofSystems.
 **/
enum class ECbmModuleId
{
  kRef        = 0,   ///< Reference plane
  kMvd        = 1,   ///< Micro-Vertex Detector
  kSts        = 2,   ///< Silicon Tracking System
  kRich       = 3,   ///< Ring-Imaging Cherenkov Detector
  kMuch       = 4,   ///< Muon detection system
  kTrd        = 5,   ///< Transition Radiation Detector
  kTof        = 6,   ///< Time-of-flight Detector
  kEcal       = 7,   ///< EM-Calorimeter
  kPsd        = 8,   ///< Projectile spectator detector
  kHodo       = 9,   ///< Hodoscope (for test beam times)
  kDummyDet   = 10,  ///< Dummy for tutorials or tests
  kT0         = 11,  ///< ToF start Detector (FIXME)
  kTrd2d      = 12,  ///< TRD-FASP Detector  (FIXME)
  kNofSystems = 13,  ///< For loops over active systems
  kMagnet     = 17,  ///< Magnet
  kTarget     = 18,  ///< Target
  kPipe       = 19,  ///< Beam pipe
  kShield     = 20,  ///< Beam pipe shielding in MUCH section
  kPlatform   = 21,  ///< RICH rail platform
  kCave       = 22,  ///< Cave
  kLastModule = 23,  ///< For loops over all modules
  kNotExist   = -1   ///< If not found
};

// operator ++ for ECbmModuleId for convenient usage in loops
// This operator is tuned for ECbmModuleID. It takes into account non
// continuous values for the enum. Since the detectorID which is stored
// in the generated output has only 4 bit the maximum number of detectors
// can be 16 (0-15). To avoid that the enum class has to be changed again
// the values 11-15 are reserved for future detectors.
// The ids of the passive modules are only relevant at run time so they can
// be shifted easily
// The opeartor takes care about the non continuous values for the enum
// When it reaches the last detector it automatically continuous with the
// first passive module
ECbmModuleId& operator++(ECbmModuleId&);

// operator << for convenient output to std::ostream.
// Converts the enum value to a string which is put in the stream
std::ostream& operator<<(std::ostream&, const ECbmModuleId&);

// conversion functions between ECbmModuleId and std::string
std::string ToString(ECbmModuleId modId);
ECbmModuleId ToCbmModuleId(std::string modIdStr);
ECbmModuleId ToCbmModuleIdCaseInsensitive(std::string modIdStr);

/** Enumerator for CBM data types **/
enum class ECbmDataType
{
  kUnknown  = -1,
  kMCTrack  = 0,
  kMvdPoint = ToIntegralType(ECbmModuleId::kMvd) * 100,
  kMvdDigi,
  kMvdCluster,
  kMvdHit,  // MVD
  kStsPoint = ToIntegralType(ECbmModuleId::kSts) * 100,
  kStsDigi,
  kStsCluster,
  kStsHit,
  kStsTrack,  // STS
  kRichPoint = ToIntegralType(ECbmModuleId::kRich) * 100,
  kRichDigi,
  kRichHit,
  kRichRing,
  kRichTrackParamZ,
  kRichTrackProjection,  // RICH
  kMuchPoint = ToIntegralType(ECbmModuleId::kMuch) * 100,
  kMuchDigi,
  kMuchCluster,
  kMuchPixelHit,
  kMuchStrawHit,
  kMuchTrack,  // MUCH
  kTrdPoint = ToIntegralType(ECbmModuleId::kTrd) * 100,
  kTrdDigi,
  kTrdCluster,
  kTrdHit,
  kTrdTrack,  // TRD
  kTofPoint = ToIntegralType(ECbmModuleId::kTof) * 100,
  kTofDigi,
  kTofCalDigi,
  kTofHit,
  kTofTrack,  // TOF
  kPsdPoint = ToIntegralType(ECbmModuleId::kPsd) * 100,
  kPsdDigi,
  kPsdHit,  // PSD
  kT0Point = ToIntegralType(ECbmModuleId::kT0) * 100,
  kT0Digi,
  kT0CalDigi,
  kT0Hit,              // T0
  kGlobalTrack = 2000  // Global
};

// operator << for convenient output to std::ostream.
// Converts the enum value to a string which is put in the stream
std::ostream& operator<<(std::ostream&, const ECbmDataType&);

/** @enumerator ETreeAccess
 ** @brief Mode to read entries from a ROOT TTree
 **
 ** kRegular: Incremental; start with first entry; stop with last entry
 ** kRepeat:  Incremental; start with first entry; after last entry jump
 **           to first entry
 ** kRandom:  Random choice of entries between first and last one.
 **/
enum class ECbmTreeAccess
{
  kRegular,
  kRepeat,
  kRandom
};


/** Global functions for particle masses **/
inline double CbmProtonMass() { return 0.938272046; }
inline double CbmNeutronMass() { return 0.939565379; }
inline double CbmElectronMass() { return 0.000510998928; }

#endif
