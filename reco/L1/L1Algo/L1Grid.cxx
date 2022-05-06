/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak [committer], Valentina Akishina */

#include "L1Grid.h"

#include <algorithm>
#include <cstdio>

#include <assert.h>
#include <string.h>

#include "L1Def.h"
#ifdef _OPENMP
#include "omp.h"
#endif
#include "L1Algo.h"


using namespace std;

/// Copy to memory block [@dest, @dest+@num] num number of times the value of i of type @T with size @typesize.
/// uses binary expansion of copied volume for speed up
template<typename T>

inline void memset(T* dest, T i, size_t num)
{
  const size_t tsize   = sizeof(T);
  unsigned int lastBin = 0;
  dest[0]              = i;
  while (lastBin + 1 < num) {
    unsigned int l = lastBin + 1;
    l              = lastBin + l + 1 > num ? num - lastBin - 1 : l;
    memcpy(dest + lastBin + 1, dest, l * tsize);
    lastBin += l;
  }
}


void L1Grid::UpdateIterGrid(unsigned int Nelements, L1Hit* hits, L1Vector<L1HitIndex_t>* indicesBuf, L1HitIndex_t* indices,
                            L1Vector<L1Hit>* hits2, L1Vector<L1HitPoint>* pointsBuf, L1HitPoint* points,
                            int& NHitsOnStation, char iS, L1Algo& Algo, const L1Vector<unsigned char>* vSFlag)
{

  fFirstHitInBin.reset(fN + 2, 0);

  fscal xs = 0;
  fscal ys = 0;

#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic, 250) firstprivate(xs, ys)
#endif
  for (L1HitIndex_t x = 0; x < Nelements; x++) {

    const L1Hit& hit = hits[x];

    if (!L1Algo::GetFUsed((*vSFlag)[hit.f] | (*vSFlag)[hit.b])) {
      Algo.GetHitCoor(hit, xs, ys, iS);

      const L1HitIndex_t& bin = GetBinBounded(xs, ys, hit.t);

      fHitsInBin[x] = fFirstHitInBin[bin + 1];
#ifdef _OPENMP
#pragma omp atomic
#endif
      fFirstHitInBin[bin + 1]++;
    }
  }

  int kk = 2;
  /* Up-Sweep Phase */
  for (unsigned int k = 1; k < fN + 2; k = kk) {
    kk = k << 1;
#ifdef _OPENMP
#pragma omp parallel for  //schedule(guided)
#endif
    for (unsigned int i = kk - 1; i < fN + 2; i += kk) {
      fFirstHitInBin[i] = fFirstHitInBin[i - k] + fFirstHitInBin[i];
    }
  }

  /* Down-Sweep Phase */
  for (int k = kk >> 1; k > 1; k = kk) {
    kk = k >> 1;
#ifdef _OPENMP
#pragma omp parallel for  //schedule(guided)
#endif
    for (unsigned int i = k - 1; i < fN + 2 - kk; i += k) {
      fFirstHitInBin[i + kk] = fFirstHitInBin[i] + fFirstHitInBin[i + kk];
    }
  }

#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic, 250) firstprivate(xs, ys)
#endif
  for (L1HitIndex_t x = 0; x < Nelements; x++) {

    const L1Hit& hit = hits[x];
    if (!L1Algo::GetFUsed((*vSFlag)[hit.f] | (*vSFlag)[hit.b])) {
      Algo.GetHitCoor(hit, xs, ys, iS);


      const L1HitIndex_t& bin = GetBinBounded(xs, ys, hit.t);

      {

        const L1HitIndex_t& index1 = fHitsInBin[x] + fFirstHitInBin[bin];

        (*hits2)[index1 + NHitsOnStation]      = hits[x];
        (*indicesBuf)[index1 + NHitsOnStation] = indices[x];
        (*pointsBuf)[index1 + NHitsOnStation]  = points[x];
      }
    }
  }

  NHitsOnStation += fFirstHitInBin[fN + 1];
}


void L1Grid::AllocateMemory(int NThreads)
{

  fNThreads = NThreads * 1;

  // fFirstHitInBinArray.resize(fNThreads, 0);


  int binsGrid = 600000;

  fFirstHitInBin.reset(binsGrid, 0);
  fHitsInBin.reset(binsGrid, 0);

  //  for( int i = 0; i < fNThreads; i++ )
  //  {
  //  delete[] fFirstHitInBinArray[i];
  // delete[] fFirstHitInBin[i];
  //   fFirstHitInBinArray[i] = new L1HitIndex_t[binsGrid];// TODO calculate safe number of bins
  // fFirstHitInBin[i] = new L1HitIndex_t[binsGrid];
  //  }
  //  fOffsets.resize(fNThreads +1, 0);
  // fNumberHitsInBin.resize(binsGrid, 0);
}


void L1Grid::BuildBins(float yMin, float yMax, float zMin, float zMax, float tMin, float tMax, float sy, float sz,
                       float st)
{

  fStepYInv = 1.f / sy;
  fStepZInv = 1.f / sz;
  fStepTInv = 1.f / st;

  fYMinOverStep = yMin * fStepYInv;
  fZMinOverStep = zMin * fStepZInv;
  fTMinOverStep = tMin * fStepTInv;

  fNy = (yMax * fStepYInv - fYMinOverStep + 1.f);
  fNz = (zMax * fStepZInv - fZMinOverStep + 1.f);
  fNt = (tMax * fStepTInv - fTMinOverStep + 1.f);
  // unsigned int Nold = fN;

  fN = fNy * fNz * fNt;

  fBinInGrid = (((fN) / fNThreads) + 1);
}


void L1Grid::StoreHits(L1HitIndex_t nhits, const L1Hit* hits, char iS, L1Algo& Algo, L1HitIndex_t n, L1Hit* hitsBuf1,
                       const L1Hit* hits1, L1HitIndex_t* indices1)
{

  fscal xs = 0;
  fscal ys = 0;

  fFirstHitInBin.reset(fN + 2, 0);

#ifdef _OPENMP
#pragma omp parallel for firstprivate(xs, ys)
#endif
  for (L1HitIndex_t x = 0; x < nhits; x++) {
    Algo.GetHitCoor((hits)[x], xs, ys, iS);


    fHitsInBin[x] = fFirstHitInBin[GetBinBounded(xs, ys, (hits)[x].t) + 1];
#ifdef _OPENMP
#pragma omp atomic
#endif
    fFirstHitInBin[GetBinBounded(xs, ys, (hits)[x].t) + 1]++;
  }


  int kk = 2;
  /* Up-Sweep Phase */
  for (unsigned int k = 1; k < fN + 2; k = kk) {
    kk = k << 1;
#ifdef _OPENMP
#pragma omp parallel for  //schedule(guided)
#endif
    for (unsigned int i = kk - 1; i < fN + 2; i += kk) {
      fFirstHitInBin[i] = fFirstHitInBin[i - k] + fFirstHitInBin[i];
    }
  }

  /* Down-Sweep Phase */
  for (unsigned int k = kk >> 1; k > 1; k = kk) {
    kk = k >> 1;
#ifdef _OPENMP
#pragma omp parallel for  //schedule(guided)
#endif
    for (unsigned int i = k - 1; i < fN + 2 - kk; i += k) {
      fFirstHitInBin[i + kk] = fFirstHitInBin[i] + fFirstHitInBin[i + kk];
    }
  }


#pragma omp parallel for firstprivate(xs, ys)
  for (L1HitIndex_t x = 0; x < nhits; x++) {

    Algo.GetHitCoor((hits1)[x], xs, ys, iS);


    const L1HitIndex_t& bin = GetBinBounded(xs, ys, (hits1)[x].t);

    {

      const L1HitIndex_t& index1 = fHitsInBin[x] + fFirstHitInBin[bin];

      (hitsBuf1)[index1] = hits[x];


      (indices1)[index1] = x + n;
    }
  }
}
