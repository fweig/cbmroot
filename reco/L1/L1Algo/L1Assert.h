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
/// Assertion levels:
/// - 0: non-critical code (any kind of algorithm initialization checks, code, which is called once)
/// - 1: semi-critical code (may be called inside the L1 core, but a small number of times)
/// - 2: critical code (possibly called in large loops inside the algorithm)

#ifndef L1Assert_h
#define L1Assert_h 1

#include "FairLogger.h"

#if defined(NDEBUG) || defined(L1_NO_ASSERT)  // TODO: Do we need to add FAST_CODE here? (S.Zharko)
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
  constexpr int kAssertionLevel {3};

  /// Basic template function. Usage: place "level <= L1Assert::kAssertionLevel" as a template parameter
  template<bool IsAsserted>
  int DoAssertion(int level, bool condition, const char* msg, const char* fileName, int lineNo)
  {
    if (!condition) {
      LOG(fatal) << '\n'
                 << " ***** Level " << level << " assertion failed: " << '\n'
                 << " *****   message/condition : " << msg << '\n'
                 << " *****   file:               " << fileName << '\n'
                 << " *****   line:               " << lineNo;
      std::abort();  // keep it here, because sometimes LOG(fatal) does not work (for example, in your unit testes)
    }
    return 1;
  }

  /// Specialization in case of IsAsserted = false, i.e. the assertion is not made
  template<>
  inline __attribute__((always_inline)) int DoAssertion<false>(int /*level*/, bool /*condition*/, const char* /*msg*/,
                                                               const char* /*fileName*/, int /*lineNo*/)
  {
    return 0;
  }
};  // namespace L1Assert

#endif  // L1Assert_h
