/* Copyright (C) 2010 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer] */

#ifndef L1Algo_PSEUDO_F32vec4_H
#define L1Algo_PSEUDO_F32vec4_H

#include <iomanip>
#include <iostream>

#include <cmath>

#include "vec_arithmetic.h"

/**********************************
 *
 *   Vector of four floats
 *
 **********************************/


float min(float x, float y);
float max(float x, float y);
float asgnb(float x, float y);
float rsqrt(float x);
float rcp(float x);
float sgn(float x);


union F32scalUnion {
  float f;
  int i;
  F32scalUnion() : f(0.f) {}
  F32scalUnion(int j) : i(j) {}
  F32scalUnion(float g) : f(g) {}
};

//const F32scalUnion __f32scal_abs_mask {static_cast<int>(0x7fffffff)};
//const F32scalUnion __f32scal_sgn_mask {static_cast<int>(0x80000000)};
const F32scalUnion __f32scal_true {static_cast<int>(0xFFFFFFFF)};
const F32scalUnion __f32scal_false {static_cast<int>(0x00000000)};


class F32vec4 {

public:
  F32scalUnion v[4];

  float& operator[](int i) { return v[i].f; }
  float operator[](int i) const { return v[i].f; }

  F32vec4() : F32vec4(0.f) {}

  F32vec4(const F32vec4& a)
  {
    v[0] = a.v[0];
    v[1] = a.v[1];
    v[2] = a.v[2];
    v[3] = a.v[3];
  }

  F32vec4(float a)
  {
    v[0].f = a;
    v[1].f = a;
    v[2].f = a;
    v[3].f = a;
  }

  F32vec4(float f0, float f1, float f2, float f3)
  {
    v[0].f = f0;
    v[1].f = f1;
    v[2].f = f2;
    v[3].f = f3;
  }

#define _f1(A, F)                                                                                                      \
  F32vec4 z;                                                                                                           \
  z[0] = F(A[0]);                                                                                                      \
  z[1] = F(A[1]);                                                                                                      \
  z[2] = F(A[2]);                                                                                                      \
  z[3] = F(A[3]);                                                                                                      \
  return z;

#define _f2(A, B, F)                                                                                                   \
  F32vec4 z;                                                                                                           \
  z[0] = F(A[0], B[0]);                                                                                                \
  z[1] = F(A[1], B[1]);                                                                                                \
  z[2] = F(A[2], B[2]);                                                                                                \
  z[3] = F(A[3], B[3]);                                                                                                \
  return z;

#define _op(A, B, F)                                                                                                   \
  F32vec4 z;                                                                                                           \
  z[0] = (A[0] F B[0]);                                                                                                \
  z[1] = (A[1] F B[1]);                                                                                                \
  z[2] = (A[2] F B[2]);                                                                                                \
  z[3] = (A[3] F B[3]);                                                                                                \
  return z;

#define _opBit(A, B, F)                                                                                                \
  F32vec4 z;                                                                                                           \
  z.v[0].i = (A.v[0].i F B.v[0].i);                                                                                    \
  z.v[1].i = (A.v[1].i F B.v[1].i);                                                                                    \
  z.v[2].i = (A.v[2].i F B.v[2].i);                                                                                    \
  z.v[3].i = (A.v[3].i F B.v[3].i);                                                                                    \
  return z;

#define _opComp(A, B, F)                                                                                               \
  F32vec4 z;                                                                                                           \
  z[0] = (A[0] F B[0]) ? __f32scal_true.f : __f32scal_false.f;                                                         \
  z[1] = (A[1] F B[1]) ? __f32scal_true.f : __f32scal_false.f;                                                         \
  z[2] = (A[2] F B[2]) ? __f32scal_true.f : __f32scal_false.f;                                                         \
  z[3] = (A[3] F B[3]) ? __f32scal_true.f : __f32scal_false.f;                                                         \
  return z;


  /* Arithmetic Operators */
  friend F32vec4 operator+(const F32vec4& a, const F32vec4& b) { _op(a, b, +) }
  friend F32vec4 operator-(const F32vec4& a, const F32vec4& b) { _op(a, b, -) }
  friend F32vec4 operator*(const F32vec4& a, const F32vec4& b) { _op(a, b, *) }
  friend F32vec4 operator/(const F32vec4& a, const F32vec4& b) { _op(a, b, /) }

  /* Comparison */
  friend F32vec4 operator<(const F32vec4& a, const F32vec4& b) { _opComp(a, b, <) }
  friend F32vec4 operator<=(const F32vec4& a, const F32vec4& b) { _opComp(a, b, <=) }
  friend F32vec4 operator>(const F32vec4& a, const F32vec4& b) { _opComp(a, b, >) }
  friend F32vec4 operator>=(const F32vec4& a, const F32vec4& b) { _opComp(a, b, >=) }
  friend F32vec4 operator==(const F32vec4& a, const F32vec4& b) { _opComp(a, b, ==) }

  /* Logic */
  friend F32vec4 operator&(const F32vec4& a, const F32vec4& b) { _opBit(a, b, &) }
  friend F32vec4 operator|(const F32vec4& a, const F32vec4& b) { _opBit(a, b, |) }
  friend F32vec4 operator^(const F32vec4& a, const F32vec4& b) { _opBit(a, b, ^) }

  friend F32vec4 operator!(const F32vec4& a)
  {
    F32vec4 z;
    z.v[0].i = ~a.v[0].i;
    z.v[1].i = ~a.v[1].i;
    z.v[2].i = ~a.v[2].i;
    z.v[3].i = ~a.v[3].i;
    return z;
  }

  friend F32vec4 if3(F32vec4 a, F32vec4 b, F32vec4 c) { return ((a) & (b)) | ((!(a)) & (c)); }  // analog (a) ? b : c

  /*
  friend F32vec4 if3(const F32vec4& a, const F32vec4& b, const F32vec4& c)
  {
    F32vec4 z;
    z[0] = (a.v[0].i) ? b[0] : c[0];
    z[1] = (a.v[1].i) ? b[1] : c[1];
    z[2] = (a.v[2].i) ? b[2] : c[2];
    z[3] = (a.v[3].i) ? b[3] : c[3];
    return z;
  }*/

  friend bool NotEmptyFvec(F32vec4 a) { return bool(a.v[0].i) || bool(a.v[1].i) || bool(a.v[2].i) || bool(a.v[3].i); }
  friend bool EmptyFvec(F32vec4 a) { return !NotEmptyFvec(a); }

  // bool NotEmpty(const F32vec4 &a) { return a[0]||a[1]||a[2]||a[3]; }
  // bool    Empty(const F32vec4 &a) { return !(a[0]||a[1]||a[2]||a[3]); } // optimize

  friend F32vec4 bool2int(const F32vec4& a)
  {  // mask returned
    return if3(a, 1, 0);
  }


  /* Functions */
  friend float min(float x, float y) { return x < y ? x : y; }
  friend float max(float x, float y) { return x < y ? y : x; }
  friend float asgnb(float x, float y) { return y >= 0.f ? fabs(x) : -fabs(x); }
  friend float rsqrt(float x) { return 1. / sqrt(x); }
  friend float rcp(float x) { return 1. / x; }
  friend float sgn(float x) { return x >= 0.f ? 1.f : -1.f; }

  friend F32vec4 min(const F32vec4& a, const F32vec4& b) { _f2(a, b, min) }
  friend F32vec4 max(const F32vec4& a, const F32vec4& b) { _f2(a, b, max) }
  friend F32vec4 asgnb(const F32vec4& a, const F32vec4& b) { _f2(a, b, asgnb) }
  friend F32vec4 sqrt(const F32vec4& a) { _f1(a, sqrt) }
  friend F32vec4 rsqrt(const F32vec4& a) { _f1(a, rsqrt) }
  friend F32vec4 rcp(const F32vec4& a) { _f1(a, rcp) }
  friend F32vec4 fabs(const F32vec4& a) { _f1(a, fabs) }
  friend F32vec4 sgn(const F32vec4& a) { _f1(a, sgn) }
  friend F32vec4 exp(const F32vec4& a) { _f1(a, exp) }
  friend F32vec4 log(const F32vec4& a) { _f1(a, log) }
  friend F32vec4 sin(const F32vec4& a) { _f1(a, sin) }
  friend F32vec4 cos(const F32vec4& a) { _f1(a, cos) }
#undef _f1
#undef _f2
#undef _op
#undef _opBit
#undef _opComp

  /* Define all operators for consistensy */

  vec_arithmetic(F32vec4, float);

  friend std::ostream& operator<<(std::ostream& strm, const F32vec4& a)
  {
    //strm << "[" << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << "]";
    strm << '[';
    strm << std::setw(12) << std::setfill(' ') << a[0] << ' ';
    strm << std::setw(12) << std::setfill(' ') << a[1] << ' ';
    strm << std::setw(12) << std::setfill(' ') << a[2] << ' ';
    strm << std::setw(12) << std::setfill(' ') << a[3] << ']';
    return strm;
  }

  friend std::istream& operator>>(std::istream& strm, F32vec4& a)
  {
    float tmp;
    strm >> tmp;
    a = tmp;
    return strm;
  }

  /// Checks, if all bands are equal
  /// NOTE: two values defined as signaling_NaN() are not equal, thus if there are all or one
  /// of the words are kNaN, the function returns false
  bool IsHorizontallyEqual() const { return (v[0].f == v[1].f) && (v[1].f == v[2].f) && (v[2].f == v[3].f); }

  /// Checks, if any of the bands is NaN
  bool IsNanAny() const { return std::isnan(v[0].f) || std::isnan(v[1].f) || std::isnan(v[2].f) || std::isnan(v[3].f); }


} __attribute__((aligned(16)));


typedef F32vec4 fvec;
typedef float fscal;
const int fvecLen = 4;

const F32vec4 fvec_zero(0.f);
const F32vec4 fvec_one(1.f);
const F32vec4 fvec_true(__f32scal_true.f);
const F32vec4 fvec_false(__f32scal_false.f);

#define _fvecalignment


namespace nsL1
{
  template<typename T>
  struct vector {
    typedef std::vector<T> TStd;
    typedef std::vector<T> TSimd;
  };

  typedef nsL1::vector<fvec>::TSimd vector_fvec;
};  // namespace nsL1

template<typename T>
struct nsL1vector :
  public nsL1::vector<T>  // just for use std::vector simultaniosly
{
};

#endif
