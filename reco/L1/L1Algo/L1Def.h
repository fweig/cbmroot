/* Copyright (C) 2010-2021 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak, Igor Kulakov [committer], Sergey Gorbunov */

#ifndef L1Def_h
#define L1Def_h

// #define FAST_CODE // FAST_CODE = more unsafe

#include <iostream>

#include <assert.h>

#ifdef FAST_CODE

#define L1_NO_ASSERT  // use with asserts, etc.
#define L1_DYNAMIC_CAST static_cast

#else  // FAST_CODE

#define L1_DYNAMIC_CAST dynamic_cast

#endif  // FAST_CODE

#if 1 && defined(__GNUC__) && __GNUC__ - 0 >= 3  // for speed up conditions
#define ISLIKELY(x) __builtin_expect(!!(x), 1)
#define ISUNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define ISLIKELY(x) (x)
#define ISUNLIKELY(x) (x)
#endif

#if defined(NDEBUG) || defined(L1_NO_ASSERT)
#define L1_ASSERT(v, msg)
#define L1_assert(v)

// Prints expression value to the std::cout
#define L1_SHOW(expr)

// Prints file and line information to the std::cout
#define L1_SHOWF(msg)


#else
#define L1_ASSERT(v, msg)                                                                                              \
  if (v) {}                                                                                                            \
  else {                                                                                                               \
    std::cerr << __FILE__ << ":" << __LINE__ << " assertion failed: " << #v << " = " << (v) << "\n"                    \
              << msg << std::endl;                                                                                     \
    abort();                                                                                                           \
  }
#define L1_assert(v) assert(v)
#define L1_SHOW(expr)                                                                                                  \
  std::cout << __FILE__ << ":" << __LINE__ << ": \033[01;38;5;208m" << (#expr) << "\033[0m = " << (expr) << "\n"
#define L1_SHOWF(msg)                                                                                                  \
  std::cout << "(!) " << __FILE__ << ":" << __LINE__ << ": \033[01;38;5;208m" << (#msg) << "\033[0m\n"
#endif

// Prints function call
#if defined(__GNUC__)
#define L1_SHOWFN std::cout << "\033[1;32mCALL \033[1;33m" << __PRETTY_FUNCTION__ << "\033[0m\n"
#else
#define L1_SHOWFN std::cout << "\033[1;32mCALL \033[1;33m" << __func__ << "\033[0m\n"
#endif

typedef int index_type;

#endif
