/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   L1Undef.h
/// \brief  Namespace L1Undef provides undefined values for different types
/// \since  18.11.2022
/// \author S.Zharko <s.zharko@gsi.de>

#ifndef L1Undef_h
#define L1Undef_h 1

#include <limits>

#include "CaSimd.h"

// NOTE: SZh 18.11.2022:
//
// A temporary solution. Previously, it was found that "using namespace cbm::algo::ca;" was introduced into a L1NaN.h
// header. This had an influence on every class in L1Algo directory (each of them has at least one fvec or fscal
// variable). In future classes inside this directory will be placed in the cbm::algo:;ca namespace, so the following
// lines will not be needed anymore.
using cbm::algo::ca::fmask;
using cbm::algo::ca::fscal;
using cbm::algo::ca::fvec;

namespace undef
{
  constexpr int kI32      = -1;
  constexpr unsigned kU32 = 4294967295;
  constexpr float kF      = std::numeric_limits<float>::quiet_NaN();
  constexpr double kD     = std::numeric_limits<double>::quiet_NaN();
  constexpr fscal kFvc    = std::numeric_limits<fscal>::quiet_NaN();

  /// Checks whether a variable of a particular type defined
  template<typename T>
  bool IsUndefined(T val);
}  // namespace undef

template<>
inline bool undef::IsUndefined<int>(int val)
{
  return val == undef::kI32;
}

template<>
inline bool undef::IsUndefined<unsigned>(unsigned val)
{
  return val == undef::kU32;
}

template<>
inline bool undef::IsUndefined<float>(float val)
{
  return std::isnan(val);
}

template<>
inline bool undef::IsUndefined<double>(double val)
{
  return std::isnan(val);
}

template<>
inline bool undef::IsUndefined<cbm::algo::ca::fvec>(cbm::algo::ca::fvec val)
{
  return isnan(val).isNotEmpty();
}

#endif  // L1Undef_h
