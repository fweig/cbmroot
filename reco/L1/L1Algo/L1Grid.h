/* Copyright (C) 2010 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer] */

// ****************************************************************
// This code is based on the code of the ALICE HLT Project        *
//*****************************************************************

#ifndef L1GRID_H
#define L1GRID_H

#include <algorithm>
#include <cstdio>

#include <assert.h>
#include <string.h>

#include "L1Def.h"
#ifdef _OPENMP
#include "omp.h"
#endif
#include "L1Hit.h"
#include "L1HitPoint.h"
#include "L1Vector.h"

class L1Algo;
//class L1HitPoint;


/**
 * @class L1Grid
 *
 * 2-dimensional grid of pointers.
 * pointers to (x,y)-like objects are assigned to the corresponding grid bin
 * used by L1Tracker to speed-up the hit operations
 * grid axis are named Y,x to be similar to TPC row coordinates.
 */
class L1Grid {
public:
  L1Grid() = default;

  L1Grid(const L1Grid& grid) : fN(grid.N()), fNx(grid.Nx()), fNy(grid.Ny()), fNt(grid.Nt()) {}

  ~L1Grid() = default;

  void StoreHits(L1Algo& Algo, int iS, L1HitIndex_t& nGridHitsFilled);

  void CreatePar0(float xMin, float xMax, float yMin, float yMax, float sx, float sy);
  void BuildBins(float xMin, float xMax, float yMin, float yMax, float tMin, float tMax, float sx, float sy, float st);

  void Initial1(int NThreads);
  void AllocateMemory(int NThreads);
  void Create(float xMin, float xMax, float yMin, float yMax, float sx, float sy);

  void Fill(const L1HitPoint* points, L1HitIndex_t n);  // call after sort
  void FillPar(const L1HitPoint* points, L1HitIndex_t n);


  int GetBin(float X, float Y) const;

  unsigned int GetBinBounded(const float& X, const float& Y) const;
  void GetBinBounded(const float& X, const float& Y, unsigned short& bX, unsigned short& bY) const;


  int GetBin(float X, float Y, float T) const;

  // static unsigned short GetBinBounded( const L1Grid *array, const unsigned short &indexes, const float &X, const float &Y );
  // static void GetBinBounded( const L1Grid *array, const unsigned short &indexes, const float &X, const float &Y, unsigned short *bX, unsigned short *bY );
  // static unsigned short Nx( const L1Grid *array, const unsigned short &indexes ) { return unsigned short( array, &L1Grid::fNx, indexes ); }

  void GetBinBounds(unsigned int iBin, float& Xmin, float& Xmax, float& Ymin, float& Ymax) const;
  unsigned int GetBinBounded(const float& X, const float& Y, const float& T) const;
  void GetBinBounded(const float& X, const float& Y, const float& T, unsigned short& bX, unsigned short& bY,
                     unsigned short& bT) const;
  void GetBinBounds(unsigned int iBin, float& Xmin, float& Xmax, float& Ymin, float& Ymax, float& Tmin,
                    float& Tmax) const;

  unsigned int N() const { return fN; }
  unsigned short Nx() const { return fNx; }
  unsigned short Ny() const { return fNy; }
  unsigned short Nt() const { return fNt; }

  L1HitIndex_t FirstHitInBin(unsigned int i) const
  {
    if (i < (fN + 1)) return fFirstHitInBin[i];
    else
      return fFirstHitInBin[fN + 1];
  }


  //     inline void UpdateGrid (fscal x, fscal y)
  //     {
  //       //  #pragma omp atomic
  //         fNumberHitsInBin[GetBinBounded(x, y)]++;
  //
  //     };


  void UpdateIterGrid(unsigned int Nelements, L1Hit* hits, L1Vector<L1HitIndex_t>& indicesBuf, L1HitIndex_t* indices,
                      L1Vector<L1Hit>& hitsBuf, L1Vector<L1HitPoint>& pointsBuf, L1HitPoint* points,
                      int& NHitsOnStation, char iS, L1Algo& Algo, const L1Vector<unsigned char>& vSFlag);


private:
  unsigned int fN {0};        //* total N bins
  unsigned short fNx {0};     //* N bins in X
  unsigned short fNy {0};     //* N bins in Y
  unsigned short fNt {0};     //* N bins in T
  float fXMinOverStep {0.f};  //* minimal X value * fStepXInv
  float fYMinOverStep {0.f};  //* minimal Y value * fStepYInv
  float fTMinOverStep {0.f};  //* minimal T value * fStepTInv
  float fStepXInv {0.f};      //* inverse bin size in X
  float fStepYInv {0.f};      //* inverse bin size in Y
  float fStepTInv {0.f};      //* inverse bin size in T
  int fBinInGrid {0};
  unsigned short fNThreads {0};

  L1Vector<L1HitIndex_t> fFirstHitInBin {"L1Grid::fFirstHitInBin"};
  L1Vector<L1HitIndex_t> fHitsInBin {"L1Grid::fHitsInBin"};

  // vector <omp_lock_t> lock;
};

// inline unsigned short L1Grid::GetBinBounded( const L1Grid *array, const unsigned short &indexes, const float &X, const float &Y )
// {

//   const float fYMinOverStep( array, &L1Grid::fYMinOverStep, indexes );
//   const float fStepYInv( array, &L1Grid::fStepYInv, indexes );
//   const unsigned short fNy( array, &L1Grid::fNy, indexes );
//   short_v yBin = ( Y * fStepYInv - fYMinOverStep ).staticCast<short_v>();
//   unsigned short yBin2 = CAMath::Max( short_v( Vc::Zero ), CAMath::Min( short_v( fNy - 1 ), yBin ) ).staticCast<unsigned short>();

//   const float fXMinOverStep( array, &L1Grid::fXMinOverStep, indexes );
//   const float fStepXInv( array, &L1Grid::fStepXInv, indexes );
//   const unsigned short fNx( array, &L1Grid::fNx, indexes );
//   short_v xBin = ( X * fStepXInv - fXMinOverStep ).staticCast<short_v>();
//   unsigned short xBin2 = CAMath::Max( short_v( Vc::Zero ), CAMath::Min( short_v( fNx - 1 ), xBin ) ).staticCast<unsigned short>();
//   return yBin2 * fNx + xBin2;
// }

// inline void L1Grid::GetBinBounded( const L1Grid *array, const unsigned short &indexes, const float &X, const float &Y, unsigned short *bX, unsigned short *bY )
// {
//   const float fXMinOverStep( array, &L1Grid::fXMinOverStep, indexes );
//   const float fStepXInv( array, &L1Grid::fStepXInv, indexes );
//   const unsigned short fNx( array, &L1Grid::fNx, indexes );
//   const short fNx2 = fNx.staticCast<short_v>();
//   const short &xBin =  static_cast<short>( X * fStepXInv - fXMinOverStep );
//   *bX = static_cast<unsigned short>(max( 0, min( fNx2 - 1, xBin ) ));

//   const float fYMinOverStep( array, &L1Grid::fYMinOverStep, indexes );
//   const float fStepYInv( array, &L1Grid::fStepYInv, indexes );
//   const unsigned short fNy( array, &L1Grid::fNy, indexes );
//   const short_v fNy2 = fNy.staticCast<short_v>();
//   const short_v &yBin = ( Y * fStepYInv - fYMinOverStep ).staticCast<short_v>();
//   *bY = CAMath::Max( short_v( Vc::Zero ), CAMath::Min( fNy2 - 1, yBin ) ).staticCast<unsigned short>();


// }
inline int L1Grid::GetBin(float X, float Y) const
{
  //* get the bin pointer
  const int& xBin = static_cast<int>(X * fStepXInv - fXMinOverStep);
  const int& yBin = static_cast<int>(Y * fStepYInv - fYMinOverStep);
  assert(xBin >= 0);
  assert(yBin >= 0);
  assert(xBin < static_cast<int>(fNx));
  assert(yBin < static_cast<int>(fNy));
  const int& bin = yBin * fNx + xBin;
  return bin;
}

inline int L1Grid::GetBin(float X, float Y, float T) const
{
  //* get the bin pointer
  const int& xBin = static_cast<int>(X * fStepXInv - fXMinOverStep);
  const int& yBin = static_cast<int>(Y * fStepYInv - fYMinOverStep);
  const int& tBin = static_cast<int>(T * fStepTInv - fTMinOverStep);
  assert(xBin >= 0);
  assert(yBin >= 0);
  assert(tBin >= 0);
  assert(xBin < static_cast<int>(fNx));
  assert(yBin < static_cast<int>(fNy));
  assert(tBin < static_cast<int>(fNt));
  const int& bin = yBin * fNx + xBin + tBin * (fNx * fNy);
  return bin;
}


inline void L1Grid::GetBinBounds(unsigned int iBin, float& Xmin, float& Xmax, float& Ymin, float& Ymax) const
{
  int yBin = iBin / fNx;
  int xBin = iBin % fNx;
  Xmin     = (fXMinOverStep + xBin) / fStepXInv;
  Ymin     = (fYMinOverStep + yBin) / fStepYInv;
  Xmax     = Xmin + 1. / fStepXInv;
  Ymax     = Ymin + 1. / fStepYInv;
}

inline void L1Grid::GetBinBounds(unsigned int iBin, float& Xmin, float& Xmax, float& Ymin, float& Ymax, float& Tmin,
                                 float& Tmax) const
{
  int yBin = (iBin % (fNx * fNy)) / fNx;
  int xBin = (iBin % (fNx * fNy)) % fNx;
  int tBin = (iBin / (fNx * fNy));
  Xmin     = (fXMinOverStep + xBin) / fStepXInv;
  Ymin     = (fYMinOverStep + yBin) / fStepYInv;
  Tmin     = (fTMinOverStep + tBin) / fStepTInv;
  Xmax     = Xmin + 1. / fStepXInv;
  Ymax     = Ymin + 1. / fStepYInv;
  Tmax     = Tmin + 1. / fStepTInv;
}


inline unsigned int L1Grid::GetBinBounded(const float& X, const float& Y) const
{
  //* get the bin pointer at

  unsigned short xBin, yBin;
  GetBinBounded(X, Y, xBin, yBin);
  return (unsigned int) yBin * (unsigned int) fNx + (unsigned int) xBin;
}

inline unsigned int L1Grid::GetBinBounded(const float& X, const float& Y, const float& T) const
{
  //* get the bin pointer at

  unsigned short xBin, yBin, tBin;
  GetBinBounded(X, Y, T, xBin, yBin, tBin);
  return (unsigned int) yBin * (unsigned int) fNx + (unsigned int) xBin
         + tBin * ((unsigned int) fNx * (unsigned int) fNy);
}

inline void L1Grid::GetBinBounded(const float& X, const float& Y, unsigned short& bX, unsigned short& bY) const
{
  const short& xBin = (X * fStepXInv - fXMinOverStep);
  const short& yBin = (Y * fStepYInv - fYMinOverStep);

  bX = std::max(short(0), std::min(short(fNx - 1), xBin));
  bY = std::max(short(0), std::min(short(fNy - 1), yBin));
}

inline void L1Grid::GetBinBounded(const float& X, const float& Y, const float& T, unsigned short& bX,
                                  unsigned short& bY, unsigned short& bT) const
{
  const short& xBin = (X * fStepXInv - fXMinOverStep);
  const short& yBin = (Y * fStepYInv - fYMinOverStep);
  const short& tBin = (T * fStepTInv - fTMinOverStep);


  // cout<<fStepTInv<<" fStepTInv "<<fTMinOverStep<<" fTMinOverStep "<<T<<" T "<<endl;

  bX = std::max(short(0), std::min(short(fNx - 1), xBin));
  bY = std::max(short(0), std::min(short(fNy - 1), yBin));
  bT = std::max(short(0), std::min(short(fNt - 1), tBin));

  // cout<<(fNt - 1)<<" (fNt - 1) "<<tBin<<" tBin "<<bT<<" bT "<<endl;
}


#endif
