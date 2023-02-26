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
 * pointers to (y,z)-like objects are assigned to the corresponding grid bin
 * used by L1Tracker to speed-up the hit operations
 * grid axis are named Z,Y to be similar to TPC row coordinates.
 */
class L1Grid {
public:
  L1Grid() = default;

  L1Grid(const L1Grid& grid) : fN(grid.N()), fNy(grid.Ny()), fNz(grid.Nz()), fNt(grid.Nt()) {}

  ~L1Grid() = default;

  void StoreHits(L1Algo& Algo, char iS, L1HitIndex_t firstHitIndex, L1HitIndex_t nHits, const L1Hit hits[],
                 L1Hit gridHits[], L1HitIndex_t gridHitIds[]);

  void CreatePar0(float yMin, float yMax, float zMin, float zMax, float sy, float sz);
  void BuildBins(float yMin, float yMax, float zMin, float zMax, float tMin, float tMax, float sy, float sz, float st);

  void HitsSort(L1HitPoint* pointsBuf, L1Hit* hitsBuf, const L1Hit* hits, L1HitIndex_t* indices,
                L1HitIndex_t* indicesBuf, L1HitPoint* points, L1HitIndex_t n, L1HitIndex_t nhits, char iS,
                L1Algo& Algo);

  void Initial1(int NThreads);
  void AllocateMemory(int NThreads);
  void Create(float yMin, float yMax, float zMin, float zMax, float sy, float sz);

  void Fill(const L1HitPoint* points, L1HitIndex_t n);  // call after sort
  void FillPar(const L1HitPoint* points, L1HitIndex_t n);


  int GetBin(float Y, float Z) const;

  unsigned int GetBinBounded(const float& Y, const float& Z) const;
  void GetBinBounded(const float& Y, const float& Z, unsigned short& bY, unsigned short& bZ) const;


  int GetBin(float Y, float Z, float T) const;

  // static unsigned short GetBinBounded( const L1Grid *array, const unsigned short &indexes, const float &Y, const float &Z );
  // static void GetBinBounded( const L1Grid *array, const unsigned short &indexes, const float &Y, const float &Z, unsigned short *bY, unsigned short *bZ );
  // static unsigned short Ny( const L1Grid *array, const unsigned short &indexes ) { return unsigned short( array, &L1Grid::fNy, indexes ); }

  void GetBinBounds(unsigned int iBin, float& Ymin, float& Ymax, float& Zmin, float& Zmax) const;
  unsigned int GetBinBounded(const float& Y, const float& Z, const float& T) const;
  void GetBinBounded(const float& Y, const float& Z, const float& T, unsigned short& bY, unsigned short& bZ,
                     unsigned short& bT) const;
  void GetBinBounds(unsigned int iBin, float& Ymin, float& Ymax, float& Zmin, float& Zmax, float& Tmin,
                    float& Tmax) const;

  unsigned int N() const { return fN; }
  unsigned short Ny() const { return fNy; }
  unsigned short Nz() const { return fNz; }
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
  unsigned short fNy {0};     //* N bins in Y
  unsigned short fNz {0};     //* N bins in Z
  unsigned short fNt {0};     //* N bins in Z
  float fYMinOverStep {0.f};  //* minimal Y value * fStepYInv
  float fZMinOverStep {0.f};  //* minimal Z value * fStepZInv
  float fTMinOverStep {0.f};  //* minimal Z value * fStepZInv
  float fStepYInv {0.f};      //* inverse bin size in Y
  float fStepZInv {0.f};      //* inverse bin size in Z
  float fStepTInv {0.f};      //* inverse bin size in Z
  int fBinInGrid {0};
  unsigned short fNThreads {0};

  L1Vector<L1HitIndex_t> fFirstHitInBin {"L1Grid::fFirstHitInBin"};
  L1Vector<L1HitIndex_t> fHitsInBin {"L1Grid::fHitsInBin"};

  // vector <omp_lock_t> lock;
};

// inline unsigned short L1Grid::GetBinBounded( const L1Grid *array, const unsigned short &indexes, const float &Y, const float &Z )
// {

//   const float fZMinOverStep( array, &L1Grid::fZMinOverStep, indexes );
//   const float fStepZInv( array, &L1Grid::fStepZInv, indexes );
//   const unsigned short fNz( array, &L1Grid::fNz, indexes );
//   short_v zBin = ( Z * fStepZInv - fZMinOverStep ).staticCast<short_v>();
//   unsigned short zBin2 = CAMath::Max( short_v( Vc::Zero ), CAMath::Min( short_v( fNz - 1 ), zBin ) ).staticCast<unsigned short>();

//   const float fYMinOverStep( array, &L1Grid::fYMinOverStep, indexes );
//   const float fStepYInv( array, &L1Grid::fStepYInv, indexes );
//   const unsigned short fNy( array, &L1Grid::fNy, indexes );
//   short_v yBin = ( Y * fStepYInv - fYMinOverStep ).staticCast<short_v>();
//   unsigned short yBin2 = CAMath::Max( short_v( Vc::Zero ), CAMath::Min( short_v( fNy - 1 ), yBin ) ).staticCast<unsigned short>();
//   return zBin2 * fNy + yBin2;
// }

// inline void L1Grid::GetBinBounded( const L1Grid *array, const unsigned short &indexes, const float &Y, const float &Z, unsigned short *bY, unsigned short *bZ )
// {
//   const float fYMinOverStep( array, &L1Grid::fYMinOverStep, indexes );
//   const float fStepYInv( array, &L1Grid::fStepYInv, indexes );
//   const unsigned short fNy( array, &L1Grid::fNy, indexes );
//   const short fNy2 = fNy.staticCast<short_v>();
//   const short &yBin =  static_cast<short>( Y * fStepYInv - fYMinOverStep );
//   *bY = static_cast<unsigned short>(max( 0, min( fNy2 - 1, yBin ) ));

//   const float fZMinOverStep( array, &L1Grid::fZMinOverStep, indexes );
//   const float fStepZInv( array, &L1Grid::fStepZInv, indexes );
//   const unsigned short fNz( array, &L1Grid::fNz, indexes );
//   const short_v fNz2 = fNz.staticCast<short_v>();
//   const short_v &zBin = ( Z * fStepZInv - fZMinOverStep ).staticCast<short_v>();
//   *bZ = CAMath::Max( short_v( Vc::Zero ), CAMath::Min( fNz2 - 1, zBin ) ).staticCast<unsigned short>();


// }
inline int L1Grid::GetBin(float Y, float Z) const
{
  //* get the bin pointer
  const int& yBin = static_cast<int>(Y * fStepYInv - fYMinOverStep);
  const int& zBin = static_cast<int>(Z * fStepZInv - fZMinOverStep);
  assert(yBin >= 0);
  assert(zBin >= 0);
  assert(yBin < static_cast<int>(fNy));
  assert(zBin < static_cast<int>(fNz));
  const int& bin = zBin * fNy + yBin;
  return bin;
}

inline int L1Grid::GetBin(float Y, float Z, float T) const
{
  //* get the bin pointer
  const int& yBin = static_cast<int>(Y * fStepYInv - fYMinOverStep);
  const int& zBin = static_cast<int>(Z * fStepZInv - fZMinOverStep);
  const int& tBin = static_cast<int>(T * fStepTInv - fTMinOverStep);
  assert(yBin >= 0);
  assert(zBin >= 0);
  assert(tBin >= 0);
  assert(yBin < static_cast<int>(fNy));
  assert(zBin < static_cast<int>(fNz));
  assert(tBin < static_cast<int>(fNt));
  const int& bin = zBin * fNy + yBin + tBin * (fNy * fNz);
  return bin;
}


inline void L1Grid::GetBinBounds(unsigned int iBin, float& Ymin, float& Ymax, float& Zmin, float& Zmax) const
{
  int zBin = iBin / fNy;
  int yBin = iBin % fNy;
  Ymin     = (fYMinOverStep + yBin) / fStepYInv;
  Zmin     = (fZMinOverStep + zBin) / fStepZInv;
  Ymax     = Ymin + 1. / fStepYInv;
  Zmax     = Zmin + 1. / fStepZInv;
}

inline void L1Grid::GetBinBounds(unsigned int iBin, float& Ymin, float& Ymax, float& Zmin, float& Zmax, float& Tmin,
                                 float& Tmax) const
{
  int zBin = (iBin % (fNy * fNz)) / fNy;
  int yBin = (iBin % (fNy * fNz)) % fNy;
  int tBin = (iBin / (fNy * fNz));
  Ymin     = (fYMinOverStep + yBin) / fStepYInv;
  Zmin     = (fZMinOverStep + zBin) / fStepZInv;
  Tmin     = (fTMinOverStep + tBin) / fStepTInv;
  Ymax     = Ymin + 1. / fStepYInv;
  Zmax     = Zmin + 1. / fStepZInv;
  Tmax     = Tmin + 1. / fStepTInv;
}


inline unsigned int L1Grid::GetBinBounded(const float& Y, const float& Z) const
{
  //* get the bin pointer at

  unsigned short yBin, zBin;
  GetBinBounded(Y, Z, yBin, zBin);
  return (unsigned int) zBin * (unsigned int) fNy + (unsigned int) yBin;
}

inline unsigned int L1Grid::GetBinBounded(const float& Y, const float& Z, const float& T) const
{
  //* get the bin pointer at

  unsigned short yBin, zBin, tBin;
  GetBinBounded(Y, Z, T, yBin, zBin, tBin);
  return (unsigned int) zBin * (unsigned int) fNy + (unsigned int) yBin
         + tBin * ((unsigned int) fNy * (unsigned int) fNz);
}

inline void L1Grid::GetBinBounded(const float& Y, const float& Z, unsigned short& bY, unsigned short& bZ) const
{
  const short& yBin = (Y * fStepYInv - fYMinOverStep);
  const short& zBin = (Z * fStepZInv - fZMinOverStep);

  bY = std::max(short(0), std::min(short(fNy - 1), yBin));
  bZ = std::max(short(0), std::min(short(fNz - 1), zBin));
}

inline void L1Grid::GetBinBounded(const float& Y, const float& Z, const float& T, unsigned short& bY,
                                  unsigned short& bZ, unsigned short& bT) const
{
  const short& yBin = (Y * fStepYInv - fYMinOverStep);
  const short& zBin = (Z * fStepZInv - fZMinOverStep);
  const short& tBin = (T * fStepTInv - fTMinOverStep);


  // cout<<fStepTInv<<" fStepTInv "<<fTMinOverStep<<" fTMinOverStep "<<T<<" T "<<endl;

  bY = std::max(short(0), std::min(short(fNy - 1), yBin));
  bZ = std::max(short(0), std::min(short(fNz - 1), zBin));
  bT = std::max(short(0), std::min(short(fNt - 1), tBin));

  // cout<<(fNt - 1)<<" (fNt - 1) "<<tBin<<" tBin "<<bT<<" bT "<<endl;
}


#endif
