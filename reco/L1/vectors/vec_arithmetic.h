/* Copyright (C) 2010-2014 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer] */

#ifndef _vec_arithmetic_H
#define _vec_arithmetic_H

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
