/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file  L1Utils.h
 * @brief File contains some general purpose functions for L1Algo
 * @since 12.01.2022
 ***********************************************************************************************************/
#ifndef L1Utils_h
#define L1Utils_h 1

#include <Logger.h>

#include <chrono>
#include <iomanip>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>

#include <cmath>

#include "CaSimd.h"
#include "L1Def.h"

// TODO: SZh 16.05.2023:  Split this class into several headers and place them into L1Algo::utils


/// Class contains some utility functions for L1Algo
namespace L1Utils
{
  /// NaN value for float
  // TODO: SZh 16.05.2023: Replace this kNaN with L1Undef::kF
  constexpr float kNaN {std::numeric_limits<float>::signaling_NaN()};

  /// Comparison method for floats
  /// \param  lhs  Left floating point to compare
  /// \param  rhs  Right floating point to compare
  /// \return      Comparison result: true - equals within epsilon
  template<typename T, typename std::enable_if<std::is_floating_point<T>::value, T>::type* = nullptr>
  [[gnu::always_inline]] inline bool CmpFloats(T lhs, T rhs)
  {
    return fabs(lhs - rhs) < 2. * std::numeric_limits<T>::epsilon() * fabs(lhs + rhs)
           || fabs(lhs - rhs) < std::numeric_limits<T>::min();
  }

  [[gnu::always_inline]] inline void CheckSimdVectorEquality(cbm::algo::ca::fvec v, const char* name)
  {
    bool ok = true;
    for (size_t i = 1; i < cbm::algo::ca::fvec::size(); i++) {
      ok = ok && (v[i] == v[0]);
    }
    if (!ok) {
      std::stringstream msg;
      msg << name << " SIMD vector is inconsistent, not all of the words are equal each other: " << v;
      throw std::logic_error(msg.str());
    }
  }

  /// Hash for unordered_map with enum class keys
  struct EnumClassHash {
    template<typename T>
    int operator()(T t) const
    {
      return static_cast<int>(t);
    }
  };

  /// A time profiler for measuring performace of scopes
  class TimeProfiler {
  public:
    /// Constructor
    TimeProfiler(const char* scopeName) : fScopeName(scopeName), fStart(std::chrono::high_resolution_clock::now())
    {
      LOG(info) << "---------- Time measurement in scope \033[1;32m" << fScopeName << "\033[0m: Start";
    }

    /// Destructor
    ~TimeProfiler()
    {
      auto stop = std::chrono::high_resolution_clock::now();
      auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - fStart).count();
      LOG(info) << "---------- Time measurement in scope \033[1;32m" << fScopeName
                << "\033[0m: Finish. Result: " << time << " ns";
    }

  private:
    const char* fScopeName {};
    const std::chrono::high_resolution_clock::time_point fStart {};
  };
}  // namespace L1Utils

#endif  // L1Utils_h
