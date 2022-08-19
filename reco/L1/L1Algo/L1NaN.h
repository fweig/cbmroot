/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   L1NaN.h
 * @brief  Definition of setters and checkers for NaN values
 * @since  21.06.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#ifndef L1NaN_h
#define L1NaN_h 1

#include <type_traits>

#include <limits>

#include <cmath>

#if defined(__CLING__) && defined(__arm64__)
#include "vectors/PSEUDO_F32vec4.h"
#else
#include "vectors/P4_F32vec4.h"
#endif

/// Namespace L1NaN defines functions to set variables to NaN and check wether they are NaN or not
///
namespace L1NaN
{
  /// Returns NaN value for a floating point variable
  template<typename T, typename std::enable_if<std::is_floating_point<T>::value, T>::type* = nullptr>
  constexpr auto SetNaN()
  {
    return std::numeric_limits<T>::signaling_NaN();
  }

  /// Returns MaN value for an intergral variable
  template<typename T,
           typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value, T>::type* = nullptr>
  constexpr auto SetNaN()
  {
    return T(-1);  // -1 for signed integers and MAX_INT for unsigned integers
  }

  /// Returns MaN value for fvec variable
  template<typename T, typename std::enable_if<std::is_same<T, fvec>::value, T>::type* = nullptr>
  constexpr auto SetNaN()
  {
    return fvec(SetNaN<fscal>());
  }

  /// Checks, if the floating point variable is NaN
  template<typename T, typename std::enable_if<std::is_floating_point<T>::value, T>::type* = nullptr>
  bool IsNaN(T value)
  {
    return std::isnan(value);
  }

  /// Checks, if the integral variable is NaN
  template<typename T,
           typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value, T>::type* = nullptr>
  bool IsNaN(T value)
  {
    return T(-1) == value;
  }

  /// Checks, if the fvec variable is NaN
  template<typename T, typename std::enable_if<std::is_same<T, fvec>::value, T>::type* = nullptr>
  bool IsNaN(T value)
  {
    return value.IsNanAny();  // NOTE: Here we consider fvec a NaN if at least one of its words is NaN
  }
};  // namespace L1NaN

#endif  // L1NaN_h
