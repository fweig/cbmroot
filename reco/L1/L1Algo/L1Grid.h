//-*- Mode: C++ -*-
// $Id: AliHLTTPCCAGrid.h,v 1.1.1.1 2010/07/26 20:55:38 ikulakov Exp $
// ************************************************************************
// This file is property of and copyright by the ALICE HLT Project        *
// ALICE Experiment at CERN, All rights reserved.                         *
// See cxx source for full Copyright notice                               *
//                                                                        *
//*************************************************************************

#ifndef L1GRID_H
#define L1GRID_H

#include "CbmL1Def.h"
#include <algorithm>
#include <string.h>

#include <assert.h>
#include <cstdio>
#ifdef _OPENMP
#include "omp.h"
#endif
#include "L1StsHit.h"
#include "L1HitPoint.h"

using namespace std;

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
 L1Grid():
 fN(0), fNy(0), fNz(0), fNt(0),
 fYMinOverStep(0.),
 fZMinOverStep(0.),
 fTMinOverStep(0.),
 fStepYInv(0.),
 fStepZInv(0.),
 fStepTInv(0.),
 fBinInGrid(0), 
 fFirstHitInBin(),
 fHitsInBin(),
 fNThreads(0)
 {
 }
              
 L1Grid(const L1Grid& grid):
 fN(grid.N()), fNy(grid.Ny()), fNz(grid.Nz()), fNt(grid.Nt()), 
 fYMinOverStep(0.),
 fZMinOverStep(0.),
 fTMinOverStep(0.),
 fStepYInv(0.),
 fStepZInv(0.),
 fStepTInv(0.),
 fBinInGrid(0),
 fFirstHitInBin(),
 fHitsInBin(),
 fNThreads(0)
 {
 }
//  ~L1Grid(){ //if ( fFirstHitInBin ) delete[] fFirstHitInBin;
//      
// 
//  
//    for( int i = 0; i < fNThreads; i++ ) {
//     // if  (fFirstHitInBinArray[i]) delete[] fFirstHitInBinArray[i];
//      if  (fFirstHitInBin[i]) delete[] fFirstHitInBin[i];
//    }
//  }
 
  void CreatePar( L1HitPoint* points, THitI nhits, L1Vector <L1HitPoint>* pointsBuf, L1Vector <L1StsHit>* hitsBuf, const L1StsHit* hits, L1Vector <THitI>* indices, L1Vector <THitI>* indicesBuf, char iS, L1Algo &Algo, THitI n);
  void StoreHits(THitI nhits, const L1StsHit* hits, char iS, L1Algo &Algo, THitI n, L1StsHit* hitsBuf1, const L1StsHit* hits1, THitI* indices1);
  
    void CreatePar0( float yMin, float yMax, float zMin, float zMax, float sy, float sz);
    void BuildBins( float yMin, float yMax, float zMin, float zMax, float tMin, float tMax, float sy, float sz, float st);
    
     void HitsSort(L1HitPoint* pointsBuf, L1StsHit* hitsBuf, const L1StsHit* hits, THitI* indices, THitI* indicesBuf, L1HitPoint* points, THitI n, THitI nhits, char iS, L1Algo &Algo);
  
   void Initial1( int NThreads );
    void AllocateMemory( int NThreads );
    void Create( float yMin, float yMax, float zMin, float zMax, float sy, float sz  );
    
  void Fill( const L1HitPoint* points, THitI n ); // call after sort
    void FillPar( const L1HitPoint* points, THitI n);
    
  
  int GetBin( float Y, float Z ) const;
  
  unsigned int GetBinBounded( const float &Y, const float &Z ) const;
  void GetBinBounded( const float &Y, const float &Z, unsigned short &bY, unsigned short &bZ ) const;
    
    
     int GetBin( float Y, float Z, float T ) const;

    // static unsigned short GetBinBounded( const L1Grid *array, const unsigned short &indexes, const float &Y, const float &Z );
    // static void GetBinBounded( const L1Grid *array, const unsigned short &indexes, const float &Y, const float &Z, unsigned short *bY, unsigned short *bZ );
    // static unsigned short Ny( const L1Grid *array, const unsigned short &indexes ) { return unsigned short( array, &L1Grid::fNy, indexes ); }

    void GetBinBounds( unsigned int iBin, float &Ymin, float &Ymax, float &Zmin, float &Zmax) const;
    unsigned int GetBinBounded( const float &Y, const float &Z, const float &T ) const;
    void GetBinBounded( const float &Y, const float &Z, const float &T, unsigned short &bY, unsigned short &bZ, unsigned short &bT ) const;
    void GetBinBounds( unsigned int iBin, float &Ymin, float &Ymax, float &Zmin, float &Zmax, float &Tmin, float &Tmax) const;
    
  unsigned int   N()        const { return fN;  }
  unsigned short Ny()       const { return fNy; }
  unsigned short Nz()       const { return fNz; }
  unsigned short Nt()       const { return fNt; }

  THitI FirstHitInBin(unsigned int i) const  {
    if (i<(fN+1)) return fFirstHitInBin[i];
    else return fFirstHitInBin[fN+1];
  }

    
//     inline void UpdateGrid (fscal x, fscal y)
//     {
//       //  #pragma omp atomic
//         fNumberHitsInBin[GetBinBounded(x, y)]++;
// 
//     };
    

    
void UpdateIterGrid(unsigned int Nelements, L1StsHit* hits,  vector <THitI>* indicesBuf, THitI* indices, vector <L1StsHit>* hits2, vector <L1HitPoint>* pointsBuf, L1HitPoint* points, int &NHitsOnStation, char iS, L1Algo &Algo, const vector< unsigned char > *vSFlag, const vector< unsigned char > *vSFlagB);


  
 
 private:

  unsigned int   fN;       //* total N bins
  unsigned short fNy;      //* N bins in Y
  unsigned short fNz;      //* N bins in Z
  unsigned short fNt;      //* N bins in Z
  float fYMinOverStep;     //* minimal Y value * fStepYInv
  float fZMinOverStep;     //* minimal Z value * fStepZInv
  float fTMinOverStep;     //* minimal Z value * fStepZInv
  float fStepYInv; //* inverse bin size in Y
  float fStepZInv; //* inverse bin size in Z
  float fStepTInv; //* inverse bin size in Z
  int fBinInGrid;
  
    vector <THitI> fFirstHitInBin;
   vector <THitI> fHitsInBin;
  // vector <THitI*> fFirstHitInBinArray;
//   vector <THitI> fOffsets;
  // vector <THitI> fNumberHitsInBin;

    
   unsigned short fNThreads;
   
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
 inline int L1Grid::GetBin( float Y, float Z ) const
{
  //* get the bin pointer
  const int& yBin = static_cast<int>( Y * fStepYInv - fYMinOverStep );
  const int& zBin = static_cast<int>( Z * fStepZInv - fZMinOverStep );
  assert( yBin >= 0 );
  assert( zBin >= 0 );
  assert( yBin < static_cast<int>( fNy ) );
  assert( zBin < static_cast<int>( fNz ) );
  const int& bin = zBin * fNy + yBin;
  return bin;
}

 inline int L1Grid::GetBin( float Y, float Z, float T ) const
{
    //* get the bin pointer
    const int& yBin = static_cast<int>( Y * fStepYInv - fYMinOverStep );
    const int& zBin = static_cast<int>( Z * fStepZInv - fZMinOverStep );
    const int& tBin = static_cast<int>( T * fStepTInv - fTMinOverStep );
    assert( yBin >= 0 );
    assert( zBin >= 0 );
    assert( tBin >= 0 );
    assert( yBin < static_cast<int>( fNy ) );
    assert( zBin < static_cast<int>( fNz ) );
    assert( tBin < static_cast<int>( fNt ) );
    const int& bin = zBin * fNy + yBin + tBin * (fNy*fNz);
    return bin;
}


 inline void L1Grid::GetBinBounds( unsigned int iBin, float &Ymin, float &Ymax, float &Zmin, float &Zmax) const
{
  int zBin = iBin / fNy;
  int yBin = iBin % fNy;
  Ymin = (fYMinOverStep + yBin)/fStepYInv;
  Zmin = (fZMinOverStep + zBin)/fStepZInv;
  Ymax = Ymin + 1. / fStepYInv;
  Zmax = Zmin + 1. / fStepZInv;
}

 inline void L1Grid::GetBinBounds( unsigned int iBin, float &Ymin, float &Ymax, float &Zmin, float &Zmax, float &Tmin, float &Tmax) const
{
    int zBin = (iBin % (fNy*fNz)) / fNy;
    int yBin = (iBin % (fNy*fNz)) % fNy;
    int tBin = (iBin / (fNy*fNz));
    Ymin = (fYMinOverStep + yBin)/fStepYInv;
    Zmin = (fZMinOverStep + zBin)/fStepZInv;
    Tmin = (fTMinOverStep + tBin)/fStepTInv;
    Ymax = Ymin + 1. / fStepYInv;
    Zmax = Zmin + 1. / fStepZInv;
    Tmax = Tmin + 1. / fStepTInv;
}


inline unsigned int L1Grid::GetBinBounded( const float &Y, const float &Z ) const
{
  //* get the bin pointer at 

  unsigned short yBin, zBin;
  GetBinBounded( Y, Z, yBin, zBin );
  return (unsigned int)zBin * (unsigned int)fNy + (unsigned int)yBin;
}

inline unsigned int L1Grid::GetBinBounded( const float &Y, const float &Z, const float &T ) const
{
    //* get the bin pointer at
    
    unsigned short yBin, zBin, tBin;
    GetBinBounded( Y, Z, T, yBin, zBin, tBin);
    return (unsigned int)zBin * (unsigned int)fNy + (unsigned int)yBin + tBin * ((unsigned int)fNy*(unsigned int)fNz);
}

inline void L1Grid::GetBinBounded( const float &Y, const float &Z, unsigned short &bY, unsigned short &bZ ) const
{
  const short &yBin = ( Y * fStepYInv - fYMinOverStep );
  const short &zBin = ( Z * fStepZInv - fZMinOverStep );

  bY = max( short(0), min( short( fNy - 1 ), yBin ) );
  bZ = max( short(0), min( short( fNz - 1 ), zBin ) );
}

inline void L1Grid::GetBinBounded( const float &Y, const float &Z, const float &T, unsigned short &bY, unsigned short &bZ, unsigned short &bT ) const
{
    const short &yBin = ( Y * fStepYInv - fYMinOverStep );
    const short &zBin = ( Z * fStepZInv - fZMinOverStep );
    const short &tBin = ( T * fStepTInv - fTMinOverStep );
    
   
    
   // cout<<fStepTInv<<" fStepTInv "<<fTMinOverStep<<" fTMinOverStep "<<T<<" T "<<endl;
    
    bY = max( short(0), min( short( fNy - 1 ), yBin ) );
    bZ = max( short(0), min( short( fNz - 1 ), zBin ) );
    bT = max( short(0), min( short( fNt - 1 ), tBin ) );
    
    // cout<<(fNt - 1)<<" (fNt - 1) "<<tBin<<" tBin "<<bT<<" bT "<<endl;
}


#endif
