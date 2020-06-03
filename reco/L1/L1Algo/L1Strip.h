#ifndef _L1Strip_h_
#define _L1Strip_h_

#include "../CbmL1Def.h"

#ifdef _OPENMP
#include "omp.h"
#endif

struct L1Branch;

struct L1Strip{
//  L1Strip():f(0),n(0){}
//  L1Strip(fscal _f, unsigned short int _n = 0):f(_f),n(_n){}
  
   L1Strip():f(0){
   }
  L1Strip(fscal _f, unsigned short int /*_n*/ = 0):f(_f){ };

  operator fscal() const { return f;}
//   operator fvec() { return fscal(f);}

// private:
  fscal f;
 
 // unsigned short int n; // number of event
};

struct L1StripSelectTracks{
  
  L1StripSelectTracks() : Cand(-1)
#ifdef _OPENMP
  , Occupied()
#endif
  {   
#ifdef _OPENMP
    omp_init_lock(&Occupied);
#endif
  //  Candidates.resize(70);
   // Candidates2.resize(70);
  //  Cand = -1;
  }
  
  ~L1StripSelectTracks()
  {
#ifdef _OPENMP
    omp_destroy_lock(&Occupied);
#endif
  }
  
//  L1Vector <L1Branch*> Candidates;
//  L1Vector <int> Candidates2;
  int Cand;
#ifdef _OPENMP
  omp_lock_t Occupied;
#endif
};

typedef unsigned /*short*/ int TStripI; // strip index type

#endif
