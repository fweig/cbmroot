/** @file vec_arithmetic.h
  * @copyright Copyright (C) 2010-2014 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Andrey Lebedev [orginator] **/

#ifndef LIT_VEC_ARITHMETIC_H
#define LIT_VEC_ARITHMETIC_H

/* Define all operators after definition of  basic operators */

#define vec_arithmetic(V, S)                                                                                           \
  friend V operator-(const V& a) { return V(0) - a; }                                                                  \
  friend V operator+(const V& a) { return a; }                                                                         \
  friend V operator+(const V& a, const S& b) { return a + V(b); }                                                      \
  friend V operator-(const V& a, const S& b) { return a - V(b); }                                                      \
  friend V operator*(const V& a, const S& b) { return a * V(b); }                                                      \
  friend V operator/(const V& a, const S& b) { return a / V(b); }                                                      \
  friend V operator+(const S& a, const V& b) { return V(a) + b; }                                                      \
  friend V operator-(const S& a, const V& b) { return V(a) - b; }                                                      \
  friend V operator*(const S& a, const V& b) { return V(a) * b; }                                                      \
  friend V operator/(const S& a, const V& b) { return V(a) / b; }                                                      \
  friend void operator+=(V& a, const V& b) { a = a + b; }                                                              \
  friend void operator-=(V& a, const V& b) { a = a - b; }                                                              \
  friend void operator*=(V& a, const V& b) { a = a * b; }                                                              \
  friend void operator/=(V& a, const V& b) { a = a / b; }                                                              \
  friend void operator+=(V& a, const S& b) { a = a + b; }                                                              \
  friend void operator-=(V& a, const S& b) { a = a - b; }                                                              \
  friend void operator*=(V& a, const S& b) { a = a * b; }                                                              \
  friend void operator/=(V& a, const S& b) { a = a / b; }

#endif
