/* Copyright (C) 2010-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko, Maksym Zyzak [committer]*/

/// \file    L1CloneMerger.h
/// \authors S.Zharko <s.zharko@gsi.de> (interface), M.Zyzak (original algorithm)
/// \brief   A class wrapper over clones merger algorithm for the L1 track finder (implementation)
/// \since   22.07.2022 (second version)

#include "L1CloneMerger.h"

#include <iostream>

#include "L1Algo.h"
#include "L1Extrapolation.h"
#include "L1Parameters.h"
#include "L1Track.h"

// ---------------------------------------------------------------------------------------------------------------------
//
L1CloneMerger::L1CloneMerger(const L1Algo& algo) : frAlgo(algo) {}

// ---------------------------------------------------------------------------------------------------------------------
//
L1CloneMerger::~L1CloneMerger() {}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1CloneMerger::Exec(L1Vector<L1Track>& extTracks, L1Vector<L1HitIndex_t>& extRecoHits)
{
  L1Vector<unsigned short>& firstStation = fTrackFirstStation;
  L1Vector<unsigned short>& lastStation  = fTrackLastStation;
  L1Vector<L1HitIndex_t>& firstHit       = fTrackFirstHit;
  L1Vector<L1HitIndex_t>& lastHit        = fTrackLastHit;
  L1Vector<unsigned short>& neighbour    = fTrackNeighbour;
  L1Vector<float>& trackChi2             = fTrackChi2;
  L1Vector<char>& isStored               = fTrackIsStored;
  L1Vector<char>& isDownstreamNeighbour  = fTrackIsDownstreamNeighbour;

  int nTracks = extTracks.size();

  assert(nTracks < std::numeric_limits<unsigned short>::max());

  constexpr unsigned short kNoNeighbour = std::numeric_limits<unsigned short>::max();

  fTracksNew.clear();
  fTracksNew.reserve(nTracks);
  fRecoHitsNew.clear();
  fRecoHitsNew.reserve(extRecoHits.size());

  firstStation.reset(nTracks);
  lastStation.reset(nTracks);
  firstHit.reset(nTracks);
  lastHit.reset(nTracks);
  isStored.reset(nTracks);
  trackChi2.reset(nTracks);
  neighbour.reset(nTracks);
  isDownstreamNeighbour.reset(nTracks);

  L1HitIndex_t start_hit = 0;

#ifdef OMP
#pragma omp parallel for
#endif
  for (int iTr = 0; iTr < nTracks; iTr++) {
    firstHit[iTr]     = start_hit;
    firstStation[iTr] = frAlgo.GetInputData()->GetHit(extRecoHits[start_hit]).iSt;
    start_hit += extTracks[iTr].NHits - 1;
    lastHit[iTr]     = start_hit;
    lastStation[iTr] = frAlgo.GetInputData()->GetHit(extRecoHits[start_hit]).iSt;
    start_hit++;

    isStored[iTr]              = false;
    neighbour[iTr]             = kNoNeighbour;
    trackChi2[iTr]             = 100000.;
    isDownstreamNeighbour[iTr] = false;
  }

  L1TrackPar Tb;
  L1TrackPar Tf;
  L1FieldValue fBm, fBb, fBf _fvecalignment;
  L1FieldRegion fld _fvecalignment;

  // Max length for merging
  unsigned char maxLengthForMerge = static_cast<unsigned char>(frAlgo.GetParameters()->GetNstationsActive() - 3);

#ifdef OMP
#pragma omp parallel for
#endif
  for (int iTr = 0; iTr < nTracks; iTr++) {
    if (extTracks[iTr].NHits > maxLengthForMerge) continue;
    for (int jTr = 0; jTr < nTracks; jTr++) {
      if (extTracks[jTr].NHits > maxLengthForMerge) continue;
      if (iTr == jTr) continue;
      if (firstStation[iTr] <= lastStation[jTr]) continue;

      unsigned short stab = firstStation[iTr];

      Tb.x   = extTracks[iTr].TFirst[0];
      Tb.y   = extTracks[iTr].TFirst[1];
      Tb.tx  = extTracks[iTr].TFirst[2];
      Tb.ty  = extTracks[iTr].TFirst[3];
      Tb.qp  = extTracks[iTr].TFirst[4];
      Tb.z   = extTracks[iTr].TFirst[5];
      Tb.t   = extTracks[iTr].TFirst[6];
      Tb.C00 = extTracks[iTr].CFirst[0];
      Tb.C10 = extTracks[iTr].CFirst[1];
      Tb.C11 = extTracks[iTr].CFirst[2];
      Tb.C20 = extTracks[iTr].CFirst[3];
      Tb.C21 = extTracks[iTr].CFirst[4];
      Tb.C22 = extTracks[iTr].CFirst[5];
      Tb.C30 = extTracks[iTr].CFirst[6];
      Tb.C31 = extTracks[iTr].CFirst[7];
      Tb.C32 = extTracks[iTr].CFirst[8];
      Tb.C33 = extTracks[iTr].CFirst[9];
      Tb.C40 = extTracks[iTr].CFirst[10];
      Tb.C41 = extTracks[iTr].CFirst[11];
      Tb.C42 = extTracks[iTr].CFirst[12];
      Tb.C43 = extTracks[iTr].CFirst[13];
      Tb.C44 = extTracks[iTr].CFirst[14];
      Tb.C50 = extTracks[iTr].CFirst[15];
      Tb.C51 = extTracks[iTr].CFirst[16];
      Tb.C52 = extTracks[iTr].CFirst[17];
      Tb.C53 = extTracks[iTr].CFirst[18];
      Tb.C54 = extTracks[iTr].CFirst[19];
      Tb.C55 = extTracks[iTr].CFirst[20];

      unsigned short staf = lastStation[jTr];

      Tf.x   = extTracks[jTr].TLast[0];
      Tf.y   = extTracks[jTr].TLast[1];
      Tf.tx  = extTracks[jTr].TLast[2];
      Tf.ty  = extTracks[jTr].TLast[3];
      Tf.qp  = extTracks[jTr].TLast[4];
      Tf.z   = extTracks[jTr].TLast[5];
      Tf.t   = extTracks[jTr].TLast[6];
      Tf.C00 = extTracks[jTr].CLast[0];
      Tf.C10 = extTracks[jTr].CLast[1];
      Tf.C11 = extTracks[jTr].CLast[2];
      Tf.C20 = extTracks[jTr].CLast[3];
      Tf.C21 = extTracks[jTr].CLast[4];
      Tf.C22 = extTracks[jTr].CLast[5];
      Tf.C30 = extTracks[jTr].CLast[6];
      Tf.C31 = extTracks[jTr].CLast[7];
      Tf.C32 = extTracks[jTr].CLast[8];
      Tf.C33 = extTracks[jTr].CLast[9];
      Tf.C40 = extTracks[jTr].CLast[10];
      Tf.C41 = extTracks[jTr].CLast[11];
      Tf.C42 = extTracks[jTr].CLast[12];
      Tf.C43 = extTracks[jTr].CLast[13];
      Tf.C44 = extTracks[jTr].CLast[14];
      Tf.C50 = extTracks[jTr].CLast[15];
      Tf.C51 = extTracks[jTr].CLast[16];
      Tf.C52 = extTracks[jTr].CLast[17];
      Tf.C53 = extTracks[jTr].CLast[18];
      Tf.C54 = extTracks[jTr].CLast[19];
      Tf.C55 = extTracks[jTr].CLast[20];


      if (fabs(Tf.t[0] - Tb.t[0]) > 3 * sqrt(Tf.C55[0] + Tb.C55[0])) continue;
      unsigned short stam;

      frAlgo.GetParameters()->GetStation(staf).fieldSlice.GetFieldValue(Tf.x, Tf.y, fBf);
      frAlgo.GetParameters()->GetStation(stab).fieldSlice.GetFieldValue(Tb.x, Tb.y, fBb);

      unsigned short dist = firstStation[iTr] - lastStation[jTr];

      if (dist > 1) stam = staf + 1;
      else
        stam = staf - 1;

      fvec zm = frAlgo.GetParameters()->GetStation(stam).fZ;
      fvec xm = fvec(0.5) * (Tf.x + Tf.tx * (zm - Tf.z) + Tb.x + Tb.tx * (zm - Tb.z));
      fvec ym = fvec(0.5) * (Tf.y + Tf.ty * (zm - Tf.z) + Tb.y + Tb.ty * (zm - Tb.z));
      frAlgo.GetParameters()->GetStation(stam).fieldSlice.GetFieldValue(xm, ym, fBm);
      fld.Set(fBb, Tb.z, fBm, zm, fBf, Tf.z);

      fvec zMiddle = fvec(0.5) * (Tb.z + Tf.z);

      L1Extrapolate(Tf, zMiddle, Tf.qp, fld);
      L1Extrapolate(Tb, zMiddle, Tb.qp, fld);

      fvec Chi2Tracks(0.);
      FilterTracks(&(Tf.x), &(Tf.C00), &(Tb.x), &(Tb.C00), 0, 0, &Chi2Tracks);
      if (Chi2Tracks[0] > 50) continue;

      if (Chi2Tracks[0] < trackChi2[iTr] || Chi2Tracks[0] < trackChi2[jTr]) {
        if (neighbour[iTr] < kNoNeighbour) {
          neighbour[neighbour[iTr]]             = kNoNeighbour;
          trackChi2[neighbour[iTr]]             = 100000.;
          isDownstreamNeighbour[neighbour[iTr]] = false;
        }
        if (neighbour[jTr] < kNoNeighbour) {
          neighbour[neighbour[jTr]]             = kNoNeighbour;
          trackChi2[neighbour[jTr]]             = 100000.;
          isDownstreamNeighbour[neighbour[jTr]] = false;
        }
        neighbour[iTr]             = jTr;
        neighbour[jTr]             = iTr;
        trackChi2[iTr]             = Chi2Tracks[0];
        trackChi2[jTr]             = Chi2Tracks[0];
        isDownstreamNeighbour[iTr] = true;
        isDownstreamNeighbour[jTr] = false;
      }
    }
  }

  for (int iTr = 0; iTr < nTracks; iTr++) {
    if (isStored[iTr]) continue;

    fTracksNew.push_back(extTracks[iTr]);
    if (!isDownstreamNeighbour[iTr]) {
      for (L1HitIndex_t HI = firstHit[iTr]; HI <= lastHit[iTr]; HI++) {
        fRecoHitsNew.push_back(extRecoHits[HI]);
      }
    }

    if (neighbour[iTr] < kNoNeighbour) {
      isStored[neighbour[iTr]] = true;
      fTracksNew.back().NHits += extTracks[neighbour[iTr]].NHits;
      for (L1HitIndex_t HI = firstHit[neighbour[iTr]]; HI <= lastHit[neighbour[iTr]]; HI++)
        fRecoHitsNew.push_back(extRecoHits[HI]);
    }

    if (isDownstreamNeighbour[iTr]) {
      for (L1HitIndex_t HI = firstHit[iTr]; HI <= lastHit[iTr]; HI++) {
        fRecoHitsNew.push_back(extRecoHits[HI]);
      }
    }
  }

  // Save the merging results into external vectors
  extTracks   = std::move(fTracksNew);
  extRecoHits = std::move(fRecoHitsNew);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1CloneMerger::FilterTracks(fvec const r[5], fvec const C[15], fvec const m[5], fvec const V[15], fvec R[5],
                                 fvec W[15], fvec* chi2)
{
  fvec S[15];
  for (int i = 0; i < 15; i++) {
    if (W) W[i] = C[i];
    S[i] = C[i] + V[i];
  }

  InvertCholesky(S);

  fvec dzeta[5];

  for (int i = 0; i < 5; i++)
    dzeta[i] = m[i] - r[i];

  if (W && R) {
    for (int i = 0; i < 5; i++)
      R[i] = r[i];

    fvec K[5][5];
    MultiplySS(C, S, K);

    fvec KC[15];
    MultiplyMS(K, C, KC);
    for (int i = 0; i < 15; i++)
      W[i] -= KC[i];

    fvec kd;
    for (int i = 0; i < 5; i++) {
      kd = 0.f;
      for (int j = 0; j < 5; j++)
        kd += K[i][j] * dzeta[j];
      R[i] += kd;
    }
  }
  if (chi2) {
    fvec S_dzeta[5];
    MultiplySR(S, dzeta, S_dzeta);
    *chi2 = dzeta[0] * S_dzeta[0] + dzeta[1] * S_dzeta[1] + dzeta[2] * S_dzeta[2] + dzeta[3] * S_dzeta[3]
            + dzeta[4] * S_dzeta[4];
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1CloneMerger::InvertCholesky(fvec a[15])
{
  fvec d[5], uud, u[5][5];
  for (int i = 0; i < 5; i++) {
    d[i] = 0.f;
    for (int j = 0; j < 5; j++)
      u[i][j] = 0.f;
  }

  for (int i = 0; i < 5; i++) {
    uud = 0.f;
    for (int j = 0; j < i; j++)
      uud += u[j][i] * u[j][i] * d[j];
    uud = a[i * (i + 3) / 2] - uud;

    fvec smallval(1.e-12);
    uud = iif(uud >= smallval, uud, smallval);

    d[i]    = uud / abs(uud);
    u[i][i] = sqrt(abs(uud));

    for (int j = i + 1; j < 5; j++) {
      uud = 0.f;
      for (int k = 0; k < i; k++)
        uud += u[k][i] * u[k][j] * d[k];
      uud     = a[j * (j + 1) / 2 + i] /*a[i][j]*/ - uud;
      u[i][j] = d[i] / u[i][i] * uud;
    }
  }

  fvec u1[5];

  for (int i = 0; i < 5; i++) {
    u1[i]   = u[i][i];
    u[i][i] = 1.f / u[i][i];
  }
  for (int i = 0; i < 4; i++) {
    u[i][i + 1] = -u[i][i + 1] * u[i][i] * u[i + 1][i + 1];
  }
  for (int i = 0; i < 3; i++) {
    u[i][i + 2] = u[i][i + 1] * u1[i + 1] * u[i + 1][i + 2] - u[i][i + 2] * u[i][i] * u[i + 2][i + 2];
  }
  for (int i = 0; i < 2; i++) {
    u[i][i + 3] = u[i][i + 2] * u1[i + 2] * u[i + 2][i + 3] - u[i][i + 3] * u[i][i] * u[i + 3][i + 3];
    u[i][i + 3] -= u[i][i + 1] * u1[i + 1] * (u[i + 1][i + 2] * u1[i + 2] * u[i + 2][i + 3] - u[i + 1][i + 3]);
  }
  u[0][4] = u[0][2] * u1[2] * u[2][4] - u[0][4] * u[0][0] * u[4][4];
  u[0][4] += u[0][1] * u1[1] * (u[1][4] - u[1][3] * u1[3] * u[3][4] - u[1][2] * u1[2] * u[2][4]);
  u[0][4] += u[3][4] * u1[3] * (u[0][3] - u1[2] * u[2][3] * (u[0][2] - u[0][1] * u1[1] * u[1][2]));

  for (int i = 0; i < 5; i++)
    a[i + 10] = u[i][4] * d[4] * u[4][4];
  for (int i = 0; i < 4; i++)
    a[i + 6] = u[i][3] * u[3][3] * d[3] + u[i][4] * u[3][4] * d[4];
  for (int i = 0; i < 3; i++)
    a[i + 3] = u[i][2] * u[2][2] * d[2] + u[i][3] * u[2][3] * d[3] + u[i][4] * u[2][4] * d[4];
  for (int i = 0; i < 2; i++)
    a[i + 1] =
      u[i][1] * u[1][1] * d[1] + u[i][2] * u[1][2] * d[2] + u[i][3] * u[1][3] * d[3] + u[i][4] * u[1][4] * d[4];
  a[0] = u[0][0] * u[0][0] * d[0] + u[0][1] * u[0][1] * d[1] + u[0][2] * u[0][2] * d[2] + u[0][3] * u[0][3] * d[3]
         + u[0][4] * u[0][4] * d[4];
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1CloneMerger::MultiplyMS(fvec const C[5][5], fvec const V[15], fvec K[15])
{
  K[0] = C[0][0] * V[0] + C[0][1] * V[1] + C[0][2] * V[3] + C[0][3] * V[6] + C[0][4] * V[10];

  K[1] = C[1][0] * V[0] + C[1][1] * V[1] + C[1][2] * V[3] + C[1][3] * V[6] + C[1][4] * V[10];
  K[2] = C[1][0] * V[1] + C[1][1] * V[2] + C[1][2] * V[4] + C[1][3] * V[7] + C[1][4] * V[11];

  K[3] = C[2][0] * V[0] + C[2][1] * V[1] + C[2][2] * V[3] + C[2][3] * V[6] + C[2][4] * V[10];
  K[4] = C[2][0] * V[1] + C[2][1] * V[2] + C[2][2] * V[4] + C[2][3] * V[7] + C[2][4] * V[11];
  K[5] = C[2][0] * V[3] + C[2][1] * V[4] + C[2][2] * V[5] + C[2][3] * V[8] + C[2][4] * V[12];

  K[6] = C[3][0] * V[0] + C[3][1] * V[1] + C[3][2] * V[3] + C[3][3] * V[6] + C[3][4] * V[10];
  K[7] = C[3][0] * V[1] + C[3][1] * V[2] + C[3][2] * V[4] + C[3][3] * V[7] + C[3][4] * V[11];
  K[8] = C[3][0] * V[3] + C[3][1] * V[4] + C[3][2] * V[5] + C[3][3] * V[8] + C[3][4] * V[12];
  K[9] = C[3][0] * V[6] + C[3][1] * V[7] + C[3][2] * V[8] + C[3][3] * V[9] + C[3][4] * V[13];

  K[10] = C[4][0] * V[0] + C[4][1] * V[1] + C[4][2] * V[3] + C[4][3] * V[6] + C[4][4] * V[10];
  K[11] = C[4][0] * V[1] + C[4][1] * V[2] + C[4][2] * V[4] + C[4][3] * V[7] + C[4][4] * V[11];
  K[12] = C[4][0] * V[3] + C[4][1] * V[4] + C[4][2] * V[5] + C[4][3] * V[8] + C[4][4] * V[12];
  K[13] = C[4][0] * V[6] + C[4][1] * V[7] + C[4][2] * V[8] + C[4][3] * V[9] + C[4][4] * V[13];
  K[14] = C[4][0] * V[10] + C[4][1] * V[11] + C[4][2] * V[12] + C[4][3] * V[13] + C[4][4] * V[14];
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1CloneMerger::MultiplySR(fvec const C[15], fvec const r_in[5], fvec r_out[5])
{
  r_out[0] = r_in[0] * C[0] + r_in[1] * C[1] + r_in[2] * C[3] + r_in[3] * C[6] + r_in[4] * C[10];
  r_out[1] = r_in[0] * C[1] + r_in[1] * C[2] + r_in[2] * C[4] + r_in[3] * C[7] + r_in[4] * C[11];
  r_out[2] = r_in[0] * C[3] + r_in[1] * C[4] + r_in[2] * C[5] + r_in[3] * C[8] + r_in[4] * C[12];
  r_out[3] = r_in[0] * C[6] + r_in[1] * C[7] + r_in[2] * C[8] + r_in[3] * C[9] + r_in[4] * C[13];
  r_out[4] = r_in[0] * C[10] + r_in[1] * C[11] + r_in[2] * C[12] + r_in[3] * C[13] + r_in[4] * C[14];
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1CloneMerger::MultiplySS(fvec const C[15], fvec const V[15], fvec K[5][5])
{
  K[0][0] = C[0] * V[0] + C[1] * V[1] + C[3] * V[3] + C[6] * V[6] + C[10] * V[10];
  K[0][1] = C[0] * V[1] + C[1] * V[2] + C[3] * V[4] + C[6] * V[7] + C[10] * V[11];
  K[0][2] = C[0] * V[3] + C[1] * V[4] + C[3] * V[5] + C[6] * V[8] + C[10] * V[12];
  K[0][3] = C[0] * V[6] + C[1] * V[7] + C[3] * V[8] + C[6] * V[9] + C[10] * V[13];
  K[0][4] = C[0] * V[10] + C[1] * V[11] + C[3] * V[12] + C[6] * V[13] + C[10] * V[14];

  K[1][0] = C[1] * V[0] + C[2] * V[1] + C[4] * V[3] + C[7] * V[6] + C[11] * V[10];
  K[1][1] = C[1] * V[1] + C[2] * V[2] + C[4] * V[4] + C[7] * V[7] + C[11] * V[11];
  K[1][2] = C[1] * V[3] + C[2] * V[4] + C[4] * V[5] + C[7] * V[8] + C[11] * V[12];
  K[1][3] = C[1] * V[6] + C[2] * V[7] + C[4] * V[8] + C[7] * V[9] + C[11] * V[13];
  K[1][4] = C[1] * V[10] + C[2] * V[11] + C[4] * V[12] + C[7] * V[13] + C[11] * V[14];

  K[2][0] = C[3] * V[0] + C[4] * V[1] + C[5] * V[3] + C[8] * V[6] + C[12] * V[10];
  K[2][1] = C[3] * V[1] + C[4] * V[2] + C[5] * V[4] + C[8] * V[7] + C[12] * V[11];
  K[2][2] = C[3] * V[3] + C[4] * V[4] + C[5] * V[5] + C[8] * V[8] + C[12] * V[12];
  K[2][3] = C[3] * V[6] + C[4] * V[7] + C[5] * V[8] + C[8] * V[9] + C[12] * V[13];
  K[2][4] = C[3] * V[10] + C[4] * V[11] + C[5] * V[12] + C[8] * V[13] + C[12] * V[14];

  K[3][0] = C[6] * V[0] + C[7] * V[1] + C[8] * V[3] + C[9] * V[6] + C[13] * V[10];
  K[3][1] = C[6] * V[1] + C[7] * V[2] + C[8] * V[4] + C[9] * V[7] + C[13] * V[11];
  K[3][2] = C[6] * V[3] + C[7] * V[4] + C[8] * V[5] + C[9] * V[8] + C[13] * V[12];
  K[3][3] = C[6] * V[6] + C[7] * V[7] + C[8] * V[8] + C[9] * V[9] + C[13] * V[13];
  K[3][4] = C[6] * V[10] + C[7] * V[11] + C[8] * V[12] + C[9] * V[13] + C[13] * V[14];

  K[4][0] = C[10] * V[0] + C[11] * V[1] + C[12] * V[3] + C[13] * V[6] + C[14] * V[10];
  K[4][1] = C[10] * V[1] + C[11] * V[2] + C[12] * V[4] + C[13] * V[7] + C[14] * V[11];
  K[4][2] = C[10] * V[3] + C[11] * V[4] + C[12] * V[5] + C[13] * V[8] + C[14] * V[12];
  K[4][3] = C[10] * V[6] + C[11] * V[7] + C[12] * V[8] + C[13] * V[9] + C[14] * V[13];
  K[4][4] = C[10] * V[10] + C[11] * V[11] + C[12] * V[12] + C[13] * V[13] + C[14] * V[14];
}
