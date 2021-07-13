/* Copyright (C) 2010-2021 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak, Igor Kulakov [committer], Sergey Gorbunov */

#ifndef L1Def_h
#define L1Def_h

// #define FAST_CODE // FAST_CODE = more unsafe

#include "TStopwatch.h"

#include <iostream>

#include <assert.h>

//#if defined(HAVE_SSE) || SSE_FOUND
#include "vectors/P4_F32vec4.h"
//#else
//#include "vectors/PSEUDO_F32vec4.h"
//#error NoSseFound
//#endif  // HAVE_SSE


template<typename T>
T finite(T x)
{
  return std::isfinite(x);
}

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
#else
#define L1_ASSERT(v, msg)                                                                                              \
  if (v) {}                                                                                                            \
  else {                                                                                                               \
    std::cerr << __FILE__ << ":" << __LINE__ << " assertion failed: " << #v << " = " << (v) << "\n"                    \
              << msg << std::endl;                                                                                     \
    abort();                                                                                                           \
  }
#define L1_assert(v) assert(v)
#endif


using namespace std;

typedef int index_type;

#endif
