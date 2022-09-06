/* Copyright (C) 2010-2014 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Maksym Zyzak */

#ifndef L1vecVc_H
#define L1vecVc_H

#include "Vc/Vc"

typedef Vc::float_v fvec;
typedef fvec::EntryType fscal;
typedef Vc::float_m fmask;

#define _fvecalignment __attribute__((aligned(Vc::VectorAlignment)))

inline fvec operator*(fscal a, const fvec& b) { return fvec(a) * b; }
inline fvec operator*(const fvec& a, fscal b) { return a * fvec(b); }

inline fvec operator/(fscal a, const fvec& b) { return fvec(a) / b; }
inline fvec operator/(const fvec& a, fscal b) { return a / fvec(b); }

inline fvec operator+(fscal a, const fvec& b) { return fvec(a) + b; }
inline fvec operator+(const fvec& a, fscal b) { return a + fvec(b); }

inline fvec operator-(fscal a, const fvec& b) { return fvec(a) - b; }
inline fvec operator-(const fvec& a, fscal b) { return a - fvec(b); }


/// Checks, if all bands are equal
/// NOTE: two values defined as signaling_NaN() are not equal, thus if there are all or one
/// of the words are kNaN, the function returns false
inline bool IsHorizontallyEqual(const fvec& v)
{
  fscal s  = v[0];
  bool ret = true;
  for (size_t i = 1; i < fvec::size(); i++) {
    ret = ret && (v[i] == s);
  }
  return ret;
}


#include "std_alloc.h"


#endif
