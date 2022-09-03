/* Copyright (C) 2010-2014 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Maksym Zyzak */

#ifndef L1vecVc_H
#define L1vecVc_H

#include "Vc/Vc"

typedef Vc::float_v fvec;
typedef fvec::EntryType fscal;
typedef Vc::float_m fmask;
const int fvecLen = fvec::Size;

#define _fvecalignment __attribute__((aligned(Vc::VectorAlignment)))

inline fvec operator*(fscal a, const fvec& b) { return fvec(a) * b; }
inline fvec operator*(const fvec& a, fscal b) { return a * fvec(b); }

inline fvec operator/(fscal a, const fvec& b) { return fvec(a) / b; }
inline fvec operator/(const fvec& a, fscal b) { return a / fvec(b); }

inline fvec operator+(fscal a, const fvec& b) { return fvec(a) + b; }
inline fvec operator+(const fvec& a, fscal b) { return a + fvec(b); }

inline fvec operator-(fscal a, const fvec& b) { return fvec(a) - b; }
inline fvec operator-(const fvec& a, fscal b) { return a - fvec(b); }

inline fvec if3(const fmask& a, const fvec& b, const fvec& c)
{
  // return (a ?b :c);
  fvec ret = c;
  ret(a)   = b;
  return ret;
}

inline fmask MaskOne() { return fmask::One(); }
inline fmask MaskZero() { return fmask::One(); }

inline fvec fabs(const fvec& a) { return abs(a); }

inline fvec masked(const fvec& a, const fmask& mask) { return if3(mask, a, fvec::Zero()); }

inline fvec mask2int(const fmask& mask)
{  // mask returned
  return if3(mask, fvec::One(), fvec::Zero());
}

/// Checks, if all bands are equal
/// NOTE: two values defined as signaling_NaN() are not equal, thus if there are all or one
/// of the words are kNaN, the function returns false
inline bool IsHorizontallyEqual(const fvec& v)
{
  fscal s  = v[0];
  bool ret = true;
  for (int i = 1; i < fvecLen; i++) {
    ret = ret && (v[i] == s);
  }
  return ret;
}

/// Checks, if any of the bands is NaN
inline bool IsNanAny(const fvec& v)
{
  bool ret = false;
  for (int i = 0; i < fvecLen; i++) {
    ret = ret || std::isnan(v[i]);
  }
  return ret;
}

inline bool EmptyFmask(const fmask& a)
{
  bool ret = true;
  for (int i = 0; i < fvecLen; i++) {
    ret = ret && (!bool(a[i]));
  }
  return ret;
}

inline bool NotEmptyFmask(const fmask& a) { return !EmptyFmask(a); }

#include "std_alloc.h"


#endif
