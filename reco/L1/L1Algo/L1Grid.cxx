#include "L1Grid.h"

#include "CbmL1Def.h"
#include <algorithm>
#include <string.h>

#include <assert.h>
#include <cstdio>
#ifdef _OPENMP
#include "omp.h"
#endif
#include "L1Algo.h"


using namespace std;

  /// Copy to memory block [@dest, @dest+@num] num number of times the value of i of type @T with size @typesize.
  /// uses binary expansion of copied volume for speed up
template< typename T>

inline void memset( T *dest, T i, size_t num ) {
  const size_t tsize = sizeof(T);
  unsigned int lastBin = 0;
  dest[0] = i;
  while ( lastBin+1 < num ) {
    unsigned int l = lastBin + 1;
    l = lastBin + l + 1 > num ? num - lastBin - 1 : l;
    memcpy( dest + lastBin + 1, dest, l*tsize );
    lastBin += l;
  }
}


void L1Grid::UpdateIterGrid(unsigned int Nelements, L1StsHit* hits,  vector <THitI>* indicesBuf, THitI* indices, vector <L1StsHit>* hits2, vector <L1HitPoint>* pointsBuf, L1HitPoint* points, int &NHitsOnStation, char iS, L1Algo &Algo, const vector< unsigned char > *vSFlag, const vector< unsigned char > *vSFlagB)
{

    fFirstHitInBin.assign(fN+2, 0);

    fscal xs=0;
    fscal ys=0;

#ifdef _OPENMP    
#pragma omp parallel for schedule(dynamic, 250) firstprivate(xs, ys)
#endif    
    for(THitI x = 0; x < Nelements; x++ ) {
        
        const L1StsHit &hit = hits[x];
        
        if (!L1Algo::GetFUsed( (*vSFlag)[hit.f] | (*vSFlagB)[hit.b] ))
        {
            Algo.GetHitCoor(hit, xs, ys, iS);

            const THitI  &bin = GetBinBounded(xs, ys, hit.t_reco);
                
            fHitsInBin[x]=fFirstHitInBin[bin+1];
#ifdef _OPENMP            
            #pragma omp atomic
#endif             
            fFirstHitInBin[bin+1]++;
     
        } 
    }

            int kk = 2;
            /* Up-Sweep Phase */
            for (unsigned int k = 1; k < fN+2; k = kk) { kk = k << 1;
#ifdef _OPENMP              
#pragma omp parallel for //schedule(guided)
#endif               
                for (unsigned int i = kk - 1; i < fN+2; i += kk) { fFirstHitInBin[i] = fFirstHitInBin[i - k]+fFirstHitInBin[i];                   
                } }
            
            /* Down-Sweep Phase */
            for (int k = kk >> 1; k > 1; k = kk) { kk = k >> 1;
#ifdef _OPENMP               
#pragma omp parallel for //schedule(guided)
#endif               
                for (unsigned int i = k - 1; i < fN+2 - kk; i += k) {
                    fFirstHitInBin[i + kk] = fFirstHitInBin[i]+fFirstHitInBin[i + kk];   
                } }

#ifdef _OPENMP      
#pragma omp parallel for schedule(dynamic, 250) firstprivate(xs, ys)
#endif
    for(THitI x = 0; x < Nelements; x++ ) {
        
        const L1StsHit &hit = hits[x];
        if (!L1Algo::GetFUsed( (*vSFlag)[hit.f] | (*vSFlagB)[hit.b] ))
        {
        Algo.GetHitCoor(hit, xs, ys, iS);
        
        
        const THitI  &bin = GetBinBounded(xs, ys, hit.t_reco);
        
        {
   
            const THitI &index1 =fHitsInBin[x]+fFirstHitInBin[bin];
            
            (*hits2)[index1+NHitsOnStation] = hits[x];
            (*indicesBuf)[index1+NHitsOnStation]=indices[x];
            (*pointsBuf)[index1+NHitsOnStation]=points[x];
            
        }
        }
    
        
    }

    NHitsOnStation+=fFirstHitInBin[fN+1];

}


void L1Grid::AllocateMemory( int NThreads )
{
    
    fNThreads = NThreads*1;
    
   // fFirstHitInBinArray.resize(fNThreads, 0);
    
    
    int binsGrid = 600000;
    
    fFirstHitInBin.resize(binsGrid, 0);
    fHitsInBin.resize(binsGrid, 0);
    
  //  for( int i = 0; i < fNThreads; i++ )
  //  {
      //  delete[] fFirstHitInBinArray[i];
       // delete[] fFirstHitInBin[i];
     //   fFirstHitInBinArray[i] = new THitI[binsGrid];// TODO calculate safe number of bins
       // fFirstHitInBin[i] = new THitI[binsGrid];
  //  }
  //  fOffsets.resize(fNThreads +1, 0);
   // fNumberHitsInBin.resize(binsGrid, 0);
    
}




void L1Grid::BuildBins( float yMin, float yMax, float zMin, float zMax, float tMin, float tMax, float sy, float sz, float st)
{
    
    fStepYInv = 1.f / sy;
    fStepZInv = 1.f / sz;
    fStepTInv = 1.f / st;
    
    fYMinOverStep = yMin * fStepYInv;
    fZMinOverStep = zMin * fStepZInv;
    fTMinOverStep = tMin * fStepTInv;
    
    fNy = ( yMax * fStepYInv - fYMinOverStep + 1.f );
    fNz = ( zMax * fStepZInv - fZMinOverStep + 1.f );
    fNt = ( tMax * fStepTInv - fTMinOverStep + 1.f );
    // unsigned int Nold = fN;
   
    fN = fNy * fNz * fNt;

    fBinInGrid = (((fN)/fNThreads)+1);
}


void L1Grid::StoreHits(THitI nhits, const L1StsHit* hits, char iS, L1Algo &Algo, THitI n, L1StsHit* hitsBuf1, const L1StsHit* hits1, THitI* indices1)
{
    
    fscal xs=0;
    fscal ys=0;

    fFirstHitInBin.assign(fN+2, 0);
    
#ifdef _OPENMP    
#pragma omp parallel for firstprivate(xs, ys)
#endif     
    for( THitI x = 0; x < nhits; x++ ) {
        Algo.GetHitCoor((hits)[x], xs, ys, iS);

        
        fHitsInBin[x]=fFirstHitInBin[GetBinBounded(xs, ys, (hits)[x].t_reco )+1];
#ifdef _OPENMP        
#pragma omp atomic
#endif         
        fFirstHitInBin[GetBinBounded(xs, ys, (hits)[x].t_reco )+1]++;
      
    }

    
    int kk = 2;
    /* Up-Sweep Phase */
    for (unsigned int k = 1; k < fN+2; k = kk) { kk = k << 1;
#ifdef _OPENMP       
#pragma omp parallel for //schedule(guided)
#endif      
        for (unsigned int i = kk - 1; i < fN+2; i += kk) { fFirstHitInBin[i] = fFirstHitInBin[i - k]+fFirstHitInBin[i];
            
        } }
    
    /* Down-Sweep Phase */
    for (unsigned int k = kk >> 1; k > 1; k = kk) { kk = k >> 1;
#ifdef _OPENMP       
#pragma omp parallel for //schedule(guided)
#endif       
        for (unsigned int i = k - 1; i < fN+2 - kk; i += k) {
            fFirstHitInBin[i + kk] = fFirstHitInBin[i]+fFirstHitInBin[i + kk];      
        } }

    
#pragma omp parallel for firstprivate(xs, ys)
    for(THitI x = 0; x < nhits; x++ ) {
        
        Algo.GetHitCoor((hits1)[x], xs, ys, iS);
        
        
        const THitI  &bin = GetBinBounded(xs, ys, (hits1)[x].t_reco);
        
        {
            
            const THitI &index1 =fHitsInBin[x]+fFirstHitInBin[bin];
            
            (hitsBuf1)[index1]=hits[x];
            
            
            (indices1)[index1]=x+n;
            
        } 
    }  
}



