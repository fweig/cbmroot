/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// @file    L1Assert.h
/// @author  Sergei Zharko
/// @date    22.02.2022
/// @brief   Level-defined assertion functionality
///
/// This file provides level-dependent assertion mechanism. Two defined macros L1ASSERT(LEVEL, COND)
/// and L1MASSERT(LEVEL, COND, MSG), which will do the assertion for LEVEL >= L1Assert::kAssertionLevel
/// and skip it otherwise. When L1ASSERT(LEVEL, COND) is called the COND expression is printed on the screen.
/// When L1MASSERT(LEVEL, COND, MSG) is called, the MSG will be printed instead of expression
///
#ifndef L1Assert_h
#define L1Assert_h 1

#include "FairLogger.h"

#if defined(NDEBUG) || defined(L1_NO_ASSERT)
#define L1ASSERT(LEVEL, COND)
#define L1MASSERT(LEVEL, COND, MSG)
#else
#define L1ASSERT(LEVEL, COND)                                                                                          \
  L1Assert::DoAssertion<(LEVEL) <= L1Assert::kAssertionLevel>((LEVEL), (COND), (#COND), __FILE__, __LINE__)
#define L1MASSERT(LEVEL, COND, MSG)                                                                                    \
  L1Assert::DoAssertion<(LEVEL) <= L1Assert::kAssertionLevel>((LEVEL), (COND), (MSG), __FILE__, __LINE__)
#endif  // defined(NDEBUG) || defined(L1_NO_ASSERT)

namespace L1Assert
{
  /// Assertion levels
  /// 0 -
  /// 1 -
  /// 2 -
  constexpr int kAssertionLevel {1};

  /// Basic template function. Usage: place "level >= L1Assert::kAssertionLevel"
  //template <bool IsAsserted>
  //int DoAssertion (int level, bool condition, const char* msg, const char* fileName, int lineNo);

  /// Specialization in case of IsAsserted = true, i.e. the assertion is made
  template<bool IsAsserted>
  int DoAssertion(int level, bool condition, const char* msg, const char* fileName, int lineNo)
  {
    if (!condition) {
      LOG(fatal) << "Level " << level << " assertion failed: " << msg << " (" << fileName << " : " << lineNo << ")\n";
      //std::abort(); // Do we need it with LOG(fatal)?
    }
    return 1;
  }

  /// Specialization in case of IsAsserted = false, i.e. the assertion is not made
  template<>
  constexpr int DoAssertion<false>(int /*level*/, bool /*condition*/, const char* /*msg*/, const char* /*fileName*/,
                                   int /*lineNo*/)
  {
    return 0;
  }
};  // namespace L1Assert

#endif  // L1Assert_h
