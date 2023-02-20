/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaCheckerTypedefs.h
/// @brief  Common definitions for QA-Checker framework
/// @author S. Zharko <s.zharko@gsi.de>
/// @since  08.02.2023

#ifndef CbmQaCheckerTypedefs_h
#define CbmQaCheckerTypedefs_h 1

#include <boost/range/iterator_range.hpp>

#include <bitset>
#include <string>
#include <unordered_map>
#include <vector>

namespace cbm::qa::checker
{
  /// @brief Enumerations for QA-Checker execution control
  ///
  enum class EFlagBit
  {
    kSKIP_LOST_OBJECTS,  //< Skips objects, which are defined in config, but not presented in file
    kEND
  };

  // ----- Aliases
  using MapStrToStr_t     = std::unordered_map<std::string, std::string>;
  using MapStrToStrVect_t = std::unordered_map<std::string, std::vector<std::string>>;
  using FlagBitSet_t      = std::bitset<static_cast<int>(EFlagBit::kEND)>;
  using CmpResult_t       = uint16_t;  ///< Bitset to keep the comparison result

  template<class T>
  using VectRange_t = boost::iterator_range<typename std::vector<T>::iterator>;

  // ----- Constants
  constexpr double kLegendSize[2] = {.3, .05};  // width and height in % of the pad size

}  // namespace cbm::qa::checker

#endif  // CbmQaCheckerTypedefs_h