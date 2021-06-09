/* Copyright (C) 2010-2021 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Maksym Zyzak, Valentina Akishina */

#include <iostream>

#include "L1Algo.h"
#include "L1Branch.h"
#include "L1Extrapolation.h"
#include "L1Filtration.h"
#include "L1Fit.h"
#include "L1HitArea.h"
#include "L1HitPoint.h"
#include "L1Track.h"
#include "L1TrackPar.h"

// using namespace std;
using std::cout;
using std::endl;
using std::vector;


/// Fit track
/// t - track with hits
/// T - track params
/// dir - 0 - forward, 1 - backward
/// qp0 - momentum for extrapolation
/// initialize - should be params ititialized. 1 - yes.
void L1Algo::BranchFitterFast(const L1Branch& t, L1TrackPar& T, const bool dir, const fvec qp0, const bool initParams)
{
  L1_assert(t.NHits >= 3);

  L1Fit fit;
  fit.SetParticleMass(GetDefaultParticleMass());

  // get hits of current track
  const std::vector<THitI>& hits = t.StsHits;  // array of indeses of hits of current track
  const int nHits                = t.NHits;

  const signed short int step = -2 * static_cast<int>(dir) + 1;  // increment for station index
  const int iFirstHit         = (dir) ? nHits - 1 : 0;
  const int iLastHit          = (dir) ? 0 : nHits - 1;

  const L1StsHit& hit0 = (*vStsHits)[hits[iFirstHit]];
  const L1StsHit& hit1 = (*vStsHits)[hits[iFirstHit + step]];
  const L1StsHit& hit2 = (*vStsHits)[hits[iFirstHit + 2 * step]];

  int ista0 = GetFStation((*vSFlag)[hit0.f]);
  int ista1 = GetFStation((*vSFlag)[hit1.f]);
  int ista2 = GetFStation((*vSFlag)[hit2.f]);

  L1Station& sta0 = vStations[ista0];
  L1Station& sta1 = vStations[ista1];
  L1Station& sta2 = vStations[ista2];

  fvec u0 = hit0.u;
  fvec v0 = hit0.v;
  fvec x0, y0;
  StripsToCoor(u0, v0, x0, y0, sta0);
  fvec z0 = (*vStsZPos)[hit0.iz];

  fvec u1 = hit1.u;
  fvec v1 = hit1.v;
  fvec x1, y1;
  StripsToCoor(u1, v1, x1, y1, sta1);
  fvec z1 = (*vStsZPos)[hit1.iz];

  fvec u2 = hit2.u;
  fvec v2 = hit2.v;
  fvec x2, y2;
  StripsToCoor(u2, v2, x2, y2, sta2);
  //  fvec z2 = (*vStsZPos)[hit2.iz];

  fvec dzi = 1. / (z1 - z0);

  const fvec vINF = .1;
  T.x             = x0;
  T.y             = y0;
  if (initParams) {
    T.tx = (x1 - x0) * dzi;
    T.ty = (y1 - y0) * dzi;
    T.qp = qp0;
  }

  T.z = z0;
  T.t = hit0.t_reco;

  // T.t[0]=(hit0.t_reco+hit1.t_reco+hit2.t_reco)/3;
  T.chi2 = 0.;
  T.NDF  = 2.;
  T.C00  = sta0.XYInfo.C00;
  T.C10  = sta0.XYInfo.C10;
  T.C11  = sta0.XYInfo.C11;

#ifdef HitErrors
  T.C00 = hit0.dx * hit0.dx;
  T.C10 = hit0.dxy;
  T.C11 = hit0.dy * hit0.dy;
#endif

  T.C20 = T.C21 = 0;
  T.C30 = T.C31 = T.C32 = 0;
  T.C40 = T.C41 = T.C42 = T.C43 = 0;
  T.C50 = T.C51 = T.C52 = T.C53 = T.C54 = 0;
  T.C22 = T.C33 = vINF;
  T.C44         = 1.;
  T.C55         = hit0.t_er * hit0.t_er;

  L1FieldValue fB0, fB1, fB2 _fvecalignment;
  L1FieldRegion fld _fvecalignment;
  fvec fz0 = sta1.z;  // suppose field is smoth
  fvec fz1 = sta2.z;
  fvec fz2 = sta0.z;


  sta1.fieldSlice.GetFieldValue(x1, y1, fB0);
  sta2.fieldSlice.GetFieldValue(x2, y2, fB1);
  sta0.fieldSlice.GetFieldValue(x0, y0, fB2);

  fld.Set(fB2, fz2, fB1, fz1, fB0, fz0);

  int ista_prev = ista1;
  int ista      = ista2;

  for (int i = iFirstHit + step; step * i <= step * iLastHit; i += step) {
    const L1StsHit& hit = (*vStsHits)[hits[i]];
    ista_prev           = ista;
    ista                = GetFStation((*vSFlag)[hit.f]);

    L1Station& sta = vStations[ista];

    float z_sta = (*vStsZPos)[hit.iz];

    fvec dz = z_sta - T.z;

#if defined(mCBM) || defined(GLOBAL)
    L1ExtrapolateLine(T, (*vStsZPos)[hit.iz]);
#else
    L1Extrapolate(T, (*vStsZPos)[hit.iz], qp0, fld);
#endif
    L1ExtrapolateTime(T, dz);

    fit.L1AddMaterial(T, sta.materialInfo, qp0, 1);
    if ((step * ista <= step * (NMvdStations + (step + 1) / 2 - 1))
        && (step * ista_prev >= step * (NMvdStations + (step + 1) / 2 - 1 - step)))
      fit.L1AddPipeMaterial(T, qp0, 1);

    fvec u = hit.u;
    fvec v = hit.v;

    L1UMeasurementInfo info = sta.frontInfo;

#ifdef HitErrors
    info.sigma2 = hit.du * hit.du;
#endif
    L1Filter(T, info, u);

    info = sta.backInfo;

#ifdef HitErrors
    info.sigma2 = hit.dv * hit.dv;
#endif
    L1Filter(T, info, v);

    FilterTime(T, hit.t_reco, hit.t_er);

    fB0 = fB1;
    fB1 = fB2;
    fz0 = fz1;
    fz1 = fz2;
    fvec x, y;
    StripsToCoor(u, v, x, y, sta);
    sta.fieldSlice.GetFieldValue(x, y, fB2);

    fz2 = sta.z;
    fld.Set(fB2, fz2, fB1, fz1, fB0, fz0);
  }  // i

}  // void L1Algo::BranchFitterFast

/// like BranchFitterFast but more precise
void L1Algo::BranchFitter(const L1Branch& t, L1TrackPar& T, const bool dir, const fvec qp0, const bool initParams)
{
  BranchFitterFast(t, T, dir, qp0, initParams);
  for (int i = 0; i < 1; i++) {
    BranchFitterFast(t, T, !dir, T.qp, false);
    BranchFitterFast(t, T, dir, T.qp, false);
  }
}  // void L1Algo::BranchFitter

/// Find additional hits for existing track
/// t - track with hits
/// T - track params
/// dir - 0 - forward, 1 - backward
/// qp0 - momentum for extrapolation
/// initialize - should be params ititialized. 1 - yes.
void L1Algo::FindMoreHits(L1Branch& t, L1TrackPar& T, const bool dir,
                          const fvec qp0)  // TODO take into account pipe
{
  std::vector<THitI> newHits;
  newHits.clear();
  newHits.reserve(5);
  L1Fit fit;
  fit.SetParticleMass(GetDefaultParticleMass());

  const signed short int step = -2 * static_cast<int>(dir) + 1;  // increment for station index
  const int iFirstHit         = (dir) ? 2 : t.NHits - 3;
  //  int ista = GetFStation((*vSFlag)[(*vStsHits)[t.StsHits[iFirstHit]].f]) + 2*step; // current station. set to the end of track

  const L1StsHit& hit0 = (*vStsHits)[t.StsHits[iFirstHit]];  // optimize
  const L1StsHit& hit1 = (*vStsHits)[t.StsHits[iFirstHit + step]];
  const L1StsHit& hit2 = (*vStsHits)[t.StsHits[iFirstHit + 2 * step]];

  const int ista0 = GetFStation((*vSFlag)[hit0.f]);
  const int ista1 = GetFStation((*vSFlag)[hit1.f]);
  const int ista2 = GetFStation((*vSFlag)[hit2.f]);

  const L1Station& sta0 = vStations[ista0];
  const L1Station& sta1 = vStations[ista1];
  const L1Station& sta2 = vStations[ista2];

  fvec u0 = hit0.u;
  fvec v0 = hit0.v;
  fvec x0, y0;
  StripsToCoor(u0, v0, x0, y0, sta0);

  fvec u1 = hit1.u;
  fvec v1 = hit1.v;
  fvec x1, y1;
  StripsToCoor(u1, v1, x1, y1, sta1);

  fvec u2 = hit2.u;
  fvec v2 = hit2.v;
  fvec x2, y2;
  StripsToCoor(u2, v2, x2, y2, sta2);

  L1FieldValue fB0, fB1, fB2 _fvecalignment;
  L1FieldRegion fld _fvecalignment;
  fvec fz0 = sta1.z;
  fvec fz1 = sta2.z;
  fvec fz2 = sta0.z;

  sta1.fieldSlice.GetFieldValue(x1, y1, fB0);
  sta2.fieldSlice.GetFieldValue(x2, y2, fB1);
  sta0.fieldSlice.GetFieldValue(x0, y0, fB2);

  fld.Set(fB2, fz2, fB1, fz1, fB0, fz0);

  int ista = ista2 + 2 * step;  // skip one station. if there would be hit it has to be found on previous stap

  if (ista2 == FIRSTCASTATION) ista = ista2 + step;

  const fvec Pick_gather2 = Pick_gather * Pick_gather;

  for (; (ista < NStations) && (ista >= 0); ista += step) {  // CHECKME why ista2?

    L1Station& sta = vStations[ista];

    fvec dz = sta.z - T.z;

    L1ExtrapolateTime(T, dz);

#if defined(mCBM) || defined(GLOBAL)
    L1ExtrapolateLine(T, sta.z);
#else
    L1Extrapolate(T, sta.z, qp0, fld);
#endif

    fscal r2_best = 1e8;  // best distance to hit
    int iHit_best = -1;   // index of the best hit

    const fscal iz = 1 / T.z[0];


    L1HitAreaTime area(vGridTime[ista], T.x[0] * iz, T.y[0] * iz,
                       (sqrt(Pick_gather * (T.C00 + sta.XYInfo.C00)) + MaxDZ * fabs(T.tx))[0] * iz,
                       (sqrt(Pick_gather * (T.C11 + sta.XYInfo.C11)) + MaxDZ * fabs(T.ty))[0] * iz, T.t[0],
                       sqrt(T.C55[0]));

    THitI ih = 0;
    while (area.GetNext(ih)) {

      ih += StsHitsUnusedStartIndex[ista];
      const L1StsHit& hit = (*vStsHitsUnused)[ih];
      if (fabs(hit.t_reco - T.t[0]) > sqrt(T.C55[0] + hit.t_er) * 5) continue;

      if (GetFUsed((*vSFlag)[hit.f] | (*vSFlag)[hit.b])) continue;  // if used

      fscal xh, yh, zh;
      GetHitCoor(hit, xh, yh, zh, sta);  // faster

      fvec y, C11;
      L1ExtrapolateYC11Line(T, zh, y, C11);

      //   fscal dym_est = ( Pick_gather*sqrt(fabs(C11[0]+sta.XYInfo.C11[0])) );
      //   fscal y_minus_new = y[0] - dym_est;
      // if (yh < y_minus_new) continue;  // CHECKME take into account overlaping?

      fvec x, C00;
      L1ExtrapolateXC00Line(T, zh, x, C00);

      fscal d_x = xh - x[0];
      fscal d_y = yh - y[0];
      fscal d2  = d_x * d_x + d_y * d_y;
      if (d2 > r2_best) continue;

      fscal dxm_est2 = (Pick_gather2 * (fabs(C00 + sta.XYInfo.C00)))[0];
      if (d_x * d_x > dxm_est2) continue;

      r2_best   = d2;
      iHit_best = ih;
    }
    if (iHit_best < 0) break;

    newHits.push_back((*RealIHitP)[iHit_best]);

    const L1StsHit& hit = (*vStsHitsUnused)[iHit_best];
    fvec u              = hit.u;
    fvec v              = hit.v;
    fvec x, y, z;
    StripsToCoor(u, v, x, y, sta);
    z = (*vStsZPos)[hit.iz];

    fvec dz1 = z - T.z;

    L1ExtrapolateTime(T, dz1);

    L1ExtrapolateLine(T, z);
    fit.L1AddMaterial(T, sta.materialInfo, qp0, 1);

    L1UMeasurementInfo info = sta.frontInfo;

#ifdef HitErrors
    info.sigma2 = hit.du * hit.du;
#endif
    L1Filter(T, info, u);

    info = sta.backInfo;

#ifdef HitErrors
    info.sigma2 = hit.dv * hit.dv;
#endif
    L1Filter(T, info, v);

    FilterTime(T, hit.t_reco, hit.t_er);

    fB0 = fB1;
    fB1 = fB2;
    fz0 = fz1;
    fz1 = fz2;
    sta.fieldSlice.GetFieldValue(x, y, fB2);
    fz2 = sta.z;
    fld.Set(fB2, fz2, fB1, fz1, fB0, fz0);
  }

  // save hits
  if (dir) {  // backward
    const unsigned int NOldHits = t.NHits;
    const unsigned int NNewHits = newHits.size();
    //  t.StsHits.resize(NNewHits + NOldHits);
    t.NHits = (NNewHits + NOldHits);
    for (int i = NOldHits - 1; i >= 0; i--) {
      t.StsHits[NNewHits + i] = t.StsHits[i];
    }
    for (unsigned int i = 0, ii = NNewHits - 1; i < NNewHits; i++, ii--) {
      t.StsHits[i] = newHits[ii];
    }
  }
  else {  // forward
    const unsigned int NOldHits = t.NHits;
    t.NHits                     = (newHits.size() + NOldHits);
    for (unsigned int i = 0; i < newHits.size(); i++) {
      t.StsHits[NOldHits + i] = (newHits[i]);
    }
  }

}  // void L1Algo::FindMoreHits

/// Try to extrapolate and find additional hits on other stations
fscal L1Algo::BranchExtender(L1Branch& t)  // TODO Simdize
{
  //   const unsigned int minNHits = 3;

  L1TrackPar T;

  // forward
  bool dir = 0;

  BranchFitter(t, T, dir);
  // BranchFitterFast (t, T, dir, 0, true);

  // if (t.NHits < minNHits) return T.chi2[0];
  FindMoreHits(t, T, dir, T.qp);

  // backward
  dir = 1;
  BranchFitterFast(t, T, dir, T.qp, false);  // 577


  FindMoreHits(t, T, dir, T.qp);

  return T.chi2[0];
}
