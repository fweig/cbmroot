#ifndef _L1Strip_h_
#define _L1Strip_h_

#include "../CbmL1Def.h"

#ifdef _OPENMP
#include "omp.h"
#endif

struct L1Branch;

struct L1Strip {
  //  L1Strip():f(0),n(0){}
  //  L1Strip(fscal _f, unsigned short int _n = 0):f(_f),n(_n){}

  L1Strip() : f(0) {}
  L1Strip(fscal _f, unsigned short int /*_n*/ = 0) : f(_f) {};

  operator fscal() const { return f; }
  //   operator fvec() { return fscal(f);}

  // private:
  fscal f;

  // unsigned short int n; // number of event
};

typedef unsigned /*short*/ int TStripI;  // strip index type

#endif
