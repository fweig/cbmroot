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
  const L1Vector<THitI>& hits = t.fStsHits;  // array of indeses of hits of current track
  const int nHits             = t.NHits;

  const signed short int step = -2 * static_cast<int>(dir) + 1;  // increment for station index
  const int iFirstHit         = (dir) ? nHits - 1 : 0;
  const int iLastHit          = (dir) ? 0 : nHits - 1;

  const L1Hit& hit0 = (*vStsHits)[hits[iFirstHit]];
  const L1Hit& hit1 = (*vStsHits)[hits[iFirstHit + step]];
  const L1Hit& hit2 = (*vStsHits)[hits[iFirstHit + 2 * step]];

  int ista0 = GetFStation((*fStripFlag)[hit0.f]);
  int ista1 = GetFStation((*fStripFlag)[hit1.f]);
  int ista2 = GetFStation((*fStripFlag)[hit2.f]);

  L1Station& sta0 = vStations[ista0];
  L1Station& sta1 = vStations[ista1];
  L1Station& sta2 = vStations[ista2];

  fvec u0 = hit0.u;
  fvec v0 = hit0.v;
  fvec x0, y0;
  StripsToCoor(u0, v0, x0, y0, sta0);
  fvec z0 = hit0.z;

  fvec u1 = hit1.u;
  fvec v1 = hit1.v;
  fvec x1, y1;
  StripsToCoor(u1, v1, x1, y1, sta1);
  fvec z1 = hit1.z;

  fvec u2 = hit2.u;
  fvec v2 = hit2.v;
  fvec x2, y2;
  StripsToCoor(u2, v2, x2, y2, sta2);
  //  fvec z2 = hit2.z;

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
  T.t = hit0.t;

  // T.t[0]=(hit0.t+hit1.t+hit2.t)/3;
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
  T.C55         = hit0.dt * hit0.dt;

  L1FieldValue fldB0, fldB1, fldB2 _fvecalignment;
  L1FieldRegion fld _fvecalignment;
  fvec fldZ0 = sta1.z;  // suppose field is smoth
  fvec fldZ1 = sta2.z;
  fvec fldZ2 = sta0.z;


  sta1.fieldSlice.GetFieldValue(x1, y1, fldB0);
  sta2.fieldSlice.GetFieldValue(x2, y2, fldB1);
  sta0.fieldSlice.GetFieldValue(x0, y0, fldB2);

  fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);

  int ista_prev = ista1;
  int ista      = ista2;

  for (int i = iFirstHit + step; step * i <= step * iLastHit; i += step) {
    const L1Hit& hit = (*vStsHits)[hits[i]];
    ista_prev        = ista;
    ista             = GetFStation((*fStripFlag)[hit.f]);

    L1Station& sta = vStations[ista];

    float z_sta = hit.z;

    fvec dz = z_sta - T.z;

#if defined(mCBM) || defined(GLOBAL)
    L1ExtrapolateLine(T, hit.z);
#else
    L1Extrapolate(T, hit.z, qp0, fld);
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

    FilterTime(T, hit.t, hit.dt);

    fldB0 = fldB1;
    fldB1 = fldB2;
    fldZ0 = fldZ1;
    fldZ1 = fldZ2;
    fvec x, y;
    StripsToCoor(u, v, x, y, sta);
    sta.fieldSlice.GetFieldValue(x, y, fldB2);

    fldZ2 = sta.z;
    fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);
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
  L1Vector<THitI> newHits {"L1TrackExtender::newHits"};
  newHits.reserve(NStations);
  L1Fit fit;
  fit.SetParticleMass(GetDefaultParticleMass());

  const signed short int step = -2 * static_cast<int>(dir) + 1;  // increment for station index
  const int iFirstHit         = (dir) ? 2 : t.NHits - 3;
  //  int ista = GetFStation((*fStripFlag)[(*vStsHits)[t.StsHits[iFirstHit]].f]) + 2*step; // current station. set to the end of track

  const L1Hit& hit0 = (*vStsHits)[t.fStsHits[iFirstHit]];  // optimize
  const L1Hit& hit1 = (*vStsHits)[t.fStsHits[iFirstHit + step]];
  const L1Hit& hit2 = (*vStsHits)[t.fStsHits[iFirstHit + 2 * step]];

  const int ista0 = GetFStation((*fStripFlag)[hit0.f]);
  const int ista1 = GetFStation((*fStripFlag)[hit1.f]);
  const int ista2 = GetFStation((*fStripFlag)[hit2.f]);

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

  L1FieldValue fldB0, fldB1, fldB2 _fvecalignment;
  L1FieldRegion fld _fvecalignment;
  fvec fldZ0 = sta1.z;
  fvec fldZ1 = sta2.z;
  fvec fldZ2 = sta0.z;

  sta1.fieldSlice.GetFieldValue(x1, y1, fldB0);
  sta2.fieldSlice.GetFieldValue(x2, y2, fldB1);
  sta0.fieldSlice.GetFieldValue(x0, y0, fldB2);

  fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);

  int ista = ista2 + 2 * step;  // skip one station. if there would be hit it has to be found on previous stap

  if (ista2 == FIRSTCASTATION) ista = ista2 + step;

  const fvec pickGather2 = fPickGather * fPickGather;

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

    const fscal iz = 1.f / (T.z[0] - fCbmTargetZ[0]);


    L1HitAreaTime area(vGridTime[ista], T.x[0] * iz, T.y[0] * iz,
                       (sqrt(fPickGather * (T.C00 + sta.XYInfo.C00)) + fMaxDZ * fabs(T.tx))[0] * iz,
                       (sqrt(fPickGather * (T.C11 + sta.XYInfo.C11)) + fMaxDZ * fabs(T.ty))[0] * iz, T.t[0],
                       sqrt(T.C55[0]));

    THitI ih = 0;
    while (area.GetNext(ih)) {

      ih += StsHitsUnusedStartIndex[ista];
      const L1Hit& hit = (*vStsHitsUnused)[ih];
      //TODO: bug, it should be hit.dt*hit.dt
      if (fabs(hit.t - T.t[0]) > sqrt(T.C55[0] + hit.dt * hit.dt) * 5) continue;

      if (GetFUsed((*fStripFlag)[hit.f] | (*fStripFlag)[hit.b])) continue;  // if used

      fscal xh, yh, zh;
      GetHitCoor(hit, xh, yh, zh, sta);  // faster

      fvec y, C11;
      L1ExtrapolateYC11Line(T, zh, y, C11);

      //   fscal dym_est = ( fPickGather * sqrt(fabs(C11[0]+sta.XYInfo.C11[0])) );
      //   fscal y_minus_new = y[0] - dym_est;
      // if (yh < y_minus_new) continue;  // CHECKME take into account overlaping?

      fvec x, C00;
      L1ExtrapolateXC00Line(T, zh, x, C00);

      fscal d_x = xh - x[0];
      fscal d_y = yh - y[0];
      fscal d2  = d_x * d_x + d_y * d_y;
      if (d2 > r2_best) continue;

      fscal dxm_est2 = (pickGather2 * (fabs(C00 + sta.XYInfo.C00)))[0];
      if (d_x * d_x > dxm_est2) continue;

      r2_best   = d2;
      iHit_best = ih;
    }
    if (iHit_best < 0) break;

    newHits.push_back((*RealIHitP)[iHit_best]);

    const L1Hit& hit = (*vStsHitsUnused)[iHit_best];
    fvec u           = hit.u;
    fvec v           = hit.v;
    fvec x, y, z;
    StripsToCoor(u, v, x, y, sta);
    z = hit.z;

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

    FilterTime(T, hit.t, hit.dt);

    fldB0 = fldB1;
    fldB1 = fldB2;
    fldZ0 = fldZ1;
    fldZ1 = fldZ2;
    sta.fieldSlice.GetFieldValue(x, y, fldB2);
    fldZ2 = sta.z;
    fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);
  }

  // save hits
  const unsigned int NOldHits = t.NHits;
  const unsigned int NNewHits = newHits.size();
  t.fStsHits.enlarge(NNewHits + NOldHits);
  t.NHits = (NNewHits + NOldHits);

  if (dir) {  // backward
    for (int i = NOldHits - 1; i >= 0; i--) {
      t.fStsHits[NNewHits + i] = t.fStsHits[i];
    }
    for (unsigned int i = 0, ii = NNewHits - 1; i < NNewHits; i++, ii--) {
      t.fStsHits[i] = newHits[ii];
    }
  }
  else {  // forward
    for (unsigned int i = 0; i < newHits.size(); i++) {
      t.fStsHits[NOldHits + i] = (newHits[i]);
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
