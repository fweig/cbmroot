/* Copyright (C) 2010 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer] */

#ifndef L1vecPseudo_H
#define L1vecPseudo_H

#include <iomanip>
#include <iostream>

#include <cmath>

typedef float fscal;
const int fvecLen = 4;

/**********************************
 *
 *   Pseudo SIMD vector
 *
 **********************************/

fscal min(fscal x, fscal y);
fscal max(fscal x, fscal y);
fscal asgnb(fscal x, fscal y);
fscal sgn(fscal x);


class fmask {

public:
  static constexpr int Size {4};

  bool v[Size];

  bool& operator[](int i) { return v[i]; }
  bool operator[](int i) const { return v[i]; }

  fmask() : fmask(0.f) {}

  fmask(const fmask& a)
  {
    for (int i = 0; i < Size; i++) {
      v[i] = a.v[i];
    }
  }

  fmask(bool a)
  {
    for (int i = 0; i < Size; i++) {
      v[i] = a;
    }
  }

  static fmask One() { return fmask(true); }

  static fmask Zero() { return fmask(false); }

#define _op(A, B, F)                                                                                                   \
  fmask z;                                                                                                             \
  for (int i = 0; i < Size; i++) {                                                                                     \
    z[i] = (A[i] F B[i]);                                                                                              \
  }                                                                                                                    \
  return z;

  /* Logic */
  friend fmask operator&&(const fmask& a, const fmask& b) { _op(a, b, &&) }
  friend fmask operator||(const fmask& a, const fmask& b) { _op(a, b, ||) }
  friend fmask operator&(const fmask& a, const fmask& b) { _op(a, b, &) }
  friend fmask operator|(const fmask& a, const fmask& b) { _op(a, b, |) }
  friend fmask operator^(const fmask& a, const fmask& b) { _op(a, b, ^) }

#undef _op

  friend fmask operator!(const fmask& a)
  {
    fmask z;
    for (int i = 0; i < Size; i++) {
      z[i] = !a[i];
    }
    return z;
  }

  bool isEmpty()
  {
    bool ret = true;
    for (int i = 0; i < fvecLen; i++) {
      ret = ret && (!v[i]);
    }
    return ret;
  }

  bool isNotEmpty() { return !isEmpty(); }


  friend std::ostream& operator<<(std::ostream& strm, const fmask& a)
  {
    strm << '[';
    for (int i = 0; i < Size; i++) {
      strm << std::setw(12) << std::setfill(' ') << a[i] << ' ';
    }
    return strm;
  }

  friend std::istream& operator>>(std::istream& strm, fmask& a)
  {
    for (int i = 0; i < Size; i++) {
      strm >> a[i];
    }
    return strm;
  }
};


class fvec {

public:
  static constexpr int Size {fmask::Size};

  fscal v[Size];

  fvec() : fvec(0.f) {}

  fvec(const fvec& a)
  {
    for (int i = 0; i < Size; i++) {
      v[i] = a.v[i];
    }
  }

  fvec(fscal a)
  {
    for (int i = 0; i < Size; i++) {
      v[i] = a;
    }
  }

  static fvec One() { return fvec(1.); }

  static fvec Zero() { return fvec(0.); }

  fscal& operator[](int i) { return v[i]; }

  fscal operator[](int i) const { return v[i]; }

  void setZero(fmask m)
  {
    for (int i = 0; i < Size; i++) {
      if (m[i]) { v[i] = 0.; }
    }
  }

#define _f1(A, F)                                                                                                      \
  fvec z;                                                                                                              \
  for (int i = 0; i < Size; i++) {                                                                                     \
    z[i] = F(A[i]);                                                                                                    \
  }                                                                                                                    \
  return z;

#define _f2(A, B, F)                                                                                                   \
  fvec z;                                                                                                              \
  for (int i = 0; i < Size; i++) {                                                                                     \
    z[i] = F(A[i], B[i]);                                                                                              \
  }                                                                                                                    \
  return z;

#define _op(A, B, F)                                                                                                   \
  fvec z;                                                                                                              \
  for (int i = 0; i < Size; i++) {                                                                                     \
    z[i] = (A[i] F B[i]);                                                                                              \
  }                                                                                                                    \
  return z;

#define _opComp(A, B, F)                                                                                               \
  fmask z;                                                                                                             \
  for (int i = 0; i < Size; i++) {                                                                                     \
    z[i] = (A[i] F B[i]);                                                                                              \
  }                                                                                                                    \
  return z;


  /* Arithmetic Operators */
  friend fvec operator+(const fvec& a, const fvec& b) { _op(a, b, +) }
  friend fvec operator-(const fvec& a, const fvec& b) { _op(a, b, -) }
  friend fvec operator*(const fvec& a, const fvec& b) { _op(a, b, *) }
  friend fvec operator/(const fvec& a, const fvec& b) { _op(a, b, /) }

  /* Comparison */
  friend fmask operator<(const fvec& a, const fvec& b) { _opComp(a, b, <) }
  friend fmask operator<=(const fvec& a, const fvec& b) { _opComp(a, b, <=) }
  friend fmask operator>(const fvec& a, const fvec& b) { _opComp(a, b, >) }
  friend fmask operator>=(const fvec& a, const fvec& b) { _opComp(a, b, >=) }
  friend fmask operator==(const fvec& a, const fvec& b) { _opComp(a, b, ==) }

  friend fvec iif(fmask a, fvec b, fvec c)
  {
    fvec z;
    for (int i = 0; i < Size; i++) {
      z[i] = a[i] ? b[i] : c[i];
    }
    return z;
  }

  /* Functions */
  friend fscal min(fscal x, fscal y) { return x < y ? x : y; }
  friend fscal max(fscal x, fscal y) { return x < y ? y : x; }
  friend fscal asgnb(fscal x, fscal y) { return y >= 0.f ? fabs(x) : -fabs(x); }
  friend fscal sgn(fscal x) { return x >= 0.f ? 1.f : -1.f; }

  friend fvec min(const fvec& a, const fvec& b) { _f2(a, b, min) }
  friend fvec max(const fvec& a, const fvec& b) { _f2(a, b, max) }
  friend fvec asgnb(const fvec& a, const fvec& b) { _f2(a, b, asgnb) }
  friend fvec sqrt(const fvec& a) { _f1(a, sqrt) }
  friend fvec abs(const fvec& a) { _f1(a, fabs) }
  friend fvec sgn(const fvec& a) { _f1(a, sgn) }
  friend fvec exp(const fvec& a) { _f1(a, exp) }
  friend fvec log(const fvec& a) { _f1(a, log) }
  friend fvec sin(const fvec& a) { _f1(a, sin) }
  friend fvec cos(const fvec& a) { _f1(a, cos) }
#undef _f1
#undef _f2
#undef _op
#undef _opComp

  /* Define all operators for consistensy */

  friend fvec operator-(const fvec& a) { return fvec(0) - a; }
  friend fvec operator+(const fvec& a) { return a; }
  friend fvec operator+(const fvec& a, const fscal& b) { return a + fvec(b); }
  friend fvec operator-(const fvec& a, const fscal& b) { return a - fvec(b); }
  friend fvec operator*(const fvec& a, const fscal& b) { return a * fvec(b); }
  friend fvec operator/(const fvec& a, const fscal& b) { return a / fvec(b); }
  friend fvec operator+(const fscal& a, const fvec& b) { return fvec(a) + b; }
  friend fvec operator-(const fscal& a, const fvec& b) { return fvec(a) - b; }
  friend fvec operator*(const fscal& a, const fvec& b) { return fvec(a) * b; }
  friend fvec operator/(const fscal& a, const fvec& b) { return fvec(a) / b; }
  friend void operator+=(fvec& a, const fvec& b) { a = a + b; }
  friend void operator-=(fvec& a, const fvec& b) { a = a - b; }
  friend void operator*=(fvec& a, const fvec& b) { a = a * b; }
  friend void operator/=(fvec& a, const fvec& b) { a = a / b; }
  friend void operator+=(fvec& a, const fscal& b) { a = a + b; }
  friend void operator-=(fvec& a, const fscal& b) { a = a - b; }
  friend void operator*=(fvec& a, const fscal& b) { a = a * b; }
  friend void operator/=(fvec& a, const fscal& b) { a = a / b; }

  friend std::ostream& operator<<(std::ostream& strm, const fvec& a)
  {
    //strm << "[" << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << "]";
    strm << '[';
    for (int i = 0; i < Size; i++) {
      strm << std::setw(12) << std::setfill(' ') << a[i] << ' ';
    }
    return strm;
  }

  friend std::istream& operator>>(std::istream& strm, fvec& a)
  {
    for (int i = 0; i < Size; i++) {
      strm >> a[i];
    }
    return strm;
  }

} __attribute__((aligned(16)));


#define _fvecalignment __attribute__((aligned(fvec::Size * sizeof(fscal))))


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


#include "std_alloc.h"

#endif
