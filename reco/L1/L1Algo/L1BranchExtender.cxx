/* Copyright (C) 2010-2021 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Maksym Zyzak, Valentina Akishina */

#include <iostream>

#include "L1Algo.h"
#include "L1Branch.h"
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
void L1Algo::BranchFitterFast(const L1Branch& t, L1TrackPar& Tout, const bool dir, const fvec qp0,
                              const bool initParams)
{
  L1_assert(t.NHits >= 3);

  L1Fit fit;
  fit.SetParticleMass(GetDefaultParticleMass());
  fit.SetMask(fmask::One());
  fit.SetTrack(Tout);
  L1TrackPar& T = fit.Tr();

  // get hits of current track
  const L1Vector<L1HitIndex_t>& hits = t.fHits;  // array of indeses of hits of current track
  const int nHits                    = t.NHits;

  const signed short int step = -2 * static_cast<int>(dir) + 1;  // increment for station index
  const int iFirstHit         = (dir) ? nHits - 1 : 0;
  const int iLastHit          = (dir) ? 0 : nHits - 1;

  const L1Hit& hit0 = fInputData.GetHit(hits[iFirstHit]);
  const L1Hit& hit1 = fInputData.GetHit(hits[iFirstHit + step]);
  const L1Hit& hit2 = fInputData.GetHit(hits[iFirstHit + 2 * step]);

  int ista0 = hit0.iSt;
  int ista1 = hit1.iSt;
  int ista2 = hit2.iSt;

  const L1Station& sta0 = fParameters.GetStation(ista0);
  const L1Station& sta1 = fParameters.GetStation(ista1);
  const L1Station& sta2 = fParameters.GetStation(ista2);

  fvec u0       = hit0.u;
  fvec v0       = hit0.v;
  auto [x0, y0] = sta0.ConvUVtoXY<fvec>(u0, v0);
  fvec z0       = hit0.z;

  fvec u1       = hit1.u;
  fvec v1       = hit1.v;
  auto [x1, y1] = sta1.ConvUVtoXY<fvec>(u1, v1);
  fvec z1       = hit1.z;

  fvec u2       = hit2.u;
  fvec v2       = hit2.v;
  auto [x2, y2] = sta2.ConvUVtoXY<fvec>(u2, v2);
  //  fvec z2 = hit2.z;

  fvec dzi = fvec(1.) / (z1 - z0);

  T.x = x0;
  T.y = y0;
  if (initParams) {
    T.tx = (x1 - x0) * dzi;
    T.ty = (y1 - y0) * dzi;
    T.qp = qp0;
  }
  fit.SetQp0(qp0);

  T.z  = z0;
  T.t  = hit0.t;
  T.vi = 0.;

  T.ResetErrors(1., 1., .1, .1, 1., (sta0.timeInfo ? hit0.dt2 : 1.e6), 1.e6);
  T.NDF = fvec(2.);

  std::tie(T.C00, T.C10, T.C11) = sta0.FormXYCovarianceMatrix(hit0.du2, hit0.dv2);

  L1FieldValue fldB0, fldB1, fldB2 _fvecalignment;
  L1FieldRegion fld _fvecalignment;
  fvec fldZ0 = sta1.fZ;  // suppose field is smoth
  fvec fldZ1 = sta2.fZ;
  fvec fldZ2 = sta0.fZ;


  sta1.fieldSlice.GetFieldValue(x1, y1, fldB0);
  sta2.fieldSlice.GetFieldValue(x2, y2, fldB1);
  sta0.fieldSlice.GetFieldValue(x0, y0, fldB2);

  fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);

  //int ista_prev = ista1;
  int ista = ista2;

  for (int i = iFirstHit + step; step * i <= step * iLastHit; i += step) {
    const L1Hit& hit = fInputData.GetHit(hits[i]);
    //ista_prev        = ista;
    ista = hit.iSt;

    const L1Station& sta = fParameters.GetStation(ista);

    fit.Extrapolate(hit.z, fld);
    fit.Filter(sta.frontInfo, hit.u, hit.du2);
    fit.Filter(sta.backInfo, hit.v, hit.dv2);
    fit.FilterTime(hit.t, hit.dt2, sta.timeInfo);
    auto radThick = fParameters.GetMaterialThickness(ista, T.x, T.y);
    fit.AddMsInMaterial(radThick);
    fit.EnergyLossCorrection(radThick, fvec(-1.f));

    fldB0       = fldB1;
    fldB1       = fldB2;
    fldZ0       = fldZ1;
    fldZ1       = fldZ2;
    auto [x, y] = sta.ConvUVtoXY<fvec>(hit.u, hit.v);
    sta.fieldSlice.GetFieldValue(x, y, fldB2);

    fldZ2 = sta.fZ;
    fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);
  }  // i

  Tout = T;
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
void L1Algo::FindMoreHits(L1Branch& t, L1TrackPar& Tout, const bool dir,
                          const fvec qp0)  // TODO take into account pipe
{
  L1Vector<L1HitIndex_t> newHits {"L1TrackExtender::newHits"};
  newHits.reserve(fParameters.GetNstationsActive());

  L1Fit fit;
  fit.SetParticleMass(GetDefaultParticleMass());
  fit.SetMask(fmask::One());
  fit.SetTrack(Tout);
  fit.SetQp0(qp0);

  const signed short int step = -2 * static_cast<int>(dir) + 1;  // increment for station index
  const int iFirstHit         = (dir) ? 2 : t.NHits - 3;
  //  int ista = fInputData.GetHit(t.Hits[iFirstHit]).iSt + 2 * step; // current station. set to the end of track

  const L1Hit& hit0 = fInputData.GetHit(t.fHits[iFirstHit]);  // optimize
  const L1Hit& hit1 = fInputData.GetHit(t.fHits[iFirstHit + step]);
  const L1Hit& hit2 = fInputData.GetHit(t.fHits[iFirstHit + 2 * step]);

  const int ista0 = hit0.iSt;
  const int ista1 = hit1.iSt;
  const int ista2 = hit2.iSt;

  const L1Station& sta0 = fParameters.GetStation(ista0);
  const L1Station& sta1 = fParameters.GetStation(ista1);
  const L1Station& sta2 = fParameters.GetStation(ista2);

  fvec u0       = hit0.u;
  fvec v0       = hit0.v;
  auto [x0, y0] = sta0.ConvUVtoXY<fvec>(u0, v0);

  fvec u1       = hit1.u;
  fvec v1       = hit1.v;
  auto [x1, y1] = sta1.ConvUVtoXY<fvec>(u1, v1);

  fvec u2       = hit2.u;
  fvec v2       = hit2.v;
  auto [x2, y2] = sta2.ConvUVtoXY<fvec>(u2, v2);

  L1FieldValue fldB0, fldB1, fldB2 _fvecalignment;
  L1FieldRegion fld _fvecalignment;
  fvec fldZ0 = sta1.fZ;
  fvec fldZ1 = sta2.fZ;
  fvec fldZ2 = sta0.fZ;

  sta1.fieldSlice.GetFieldValue(x1, y1, fldB0);
  sta2.fieldSlice.GetFieldValue(x2, y2, fldB1);
  sta0.fieldSlice.GetFieldValue(x0, y0, fldB2);

  fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);

  int ista = ista2 + 2 * step;  // skip one station. if there would be hit it has to be found on previous stap

  if (ista2 == fFirstCAstation) ista = ista2 + step;

  const fvec pickGather2 = fPickGather * fPickGather;

  for (; (ista < fParameters.GetNstationsActive()) && (ista >= 0); ista += step) {  // CHECKME why ista2?

    const L1Station& sta = fParameters.GetStation(ista);

    fit.Extrapolate(sta.fZ, fld);

    fscal r2_best = 1e8;  // best distance to hit
    int iHit_best = -1;   // index of the best hit

    L1TrackPar& tr = fit.Tr();

    const fscal iz = 1.f / (tr.z[0] - fParameters.GetTargetPositionZ()[0]);

    L1HitAreaTime area(vGridTime[ista], tr.x[0] * iz, tr.y[0] * iz,
                       (sqrt(fPickGather * tr.C00) + fMaxDx[ista] + fMaxDZ * abs(tr.tx))[0] * iz,
                       (sqrt(fPickGather * tr.C11) + fMaxDy[ista] + fMaxDZ * abs(tr.ty))[0] * iz, tr.t[0],
                       sqrt(tr.C55[0]));

    for (L1HitIndex_t ih = -1; true;) {  // loop over the hits in the area

      if (fParameters.DevIsIgnoreHitSearchAreas()) {
        ih++;
        if ((L1HitIndex_t) ih >= (fGridHitStopIndex[ista] - fGridHitStartIndex[ista])) { break; }
      }
      else {
        if (!area.GetNext(ih)) { break; }
      }

      L1HitIndex_t globalInd = fGridHitStartIndex[ista] + ih;

      const L1HitPoint& hitPoint = fGridPoints[globalInd];
      if (hitPoint.IsSuppressed()) { continue; }

      const L1Hit& hit = fGridHits[globalInd];

      if (sta.timeInfo && tr.nTimeMeasurements[0] > 0) {
        fscal dt = hit.t - tr.t[0];
        if (dt * dt > (tr.C55[0] + hit.dt2) * 25) continue;
      }

      //if (GetFUsed((*fStripFlag)[hit.f] | (*fStripFlag)[hit.b])) continue;  // if used
      //L1_SHOW(fvHitKeyFlags.size());
      //L1_SHOW(hit.f);
      //L1_SHOW(hit.b);
      if (fvHitKeyFlags[hit.f] || fvHitKeyFlags[hit.b]) { continue; }

      fscal xh, yh, zh;
      GetHitCoor(hit, xh, yh, zh, sta);  // faster

      fvec y, C11;
      fit.ExtrapolateYC11Line(zh, y, C11);

      //   fscal dym_est = ( fPickGather * sqrt(fabs(C11[0]+sta.XYInfo.C11[0])) );
      //   fscal y_minus_new = y[0] - dym_est;
      // if (yh < y_minus_new) continue;  // CHECKME take into account overlaping?

      fvec x, C00;
      fit.ExtrapolateXC00Line(zh, x, C00);

      fscal d_x = xh - x[0];
      fscal d_y = yh - y[0];
      fscal d2  = d_x * d_x + d_y * d_y;
      if (d2 > r2_best) continue;

      fscal dxm_est2 = (pickGather2 * (abs(C00 + fMaxDx[ista] * fMaxDx[ista])))[0];
      if (d_x * d_x > dxm_est2) continue;

      r2_best   = d2;
      iHit_best = globalInd;
    }
    if (iHit_best < 0) break;

    newHits.push_back(fGridHitIds[iHit_best]);

    const L1Hit& hit = fGridHits[iHit_best];

    auto [x, y] = sta.ConvUVtoXY<fvec>(hit.u, hit.v);

    fit.Extrapolate(hit.z, fld);
    fit.Filter(sta.frontInfo, hit.u, hit.du2);
    fit.Filter(sta.backInfo, hit.v, hit.dv2);
    fit.FilterTime(hit.t, hit.dt2, sta.timeInfo);
    auto radThick = fParameters.GetMaterialThickness(ista, tr.x, tr.y);
    fit.AddMsInMaterial(radThick);
    fit.EnergyLossCorrection(radThick, dir ? fvec(1.f) : fvec(-1.f));

    fldB0 = fldB1;
    fldB1 = fldB2;
    fldZ0 = fldZ1;
    fldZ1 = fldZ2;
    sta.fieldSlice.GetFieldValue(x, y, fldB2);
    fldZ2 = sta.fZ;
    fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);
  }

  // save hits
  const unsigned int NOldHits = t.NHits;
  const unsigned int NNewHits = newHits.size();
  t.fHits.enlarge(NNewHits + NOldHits);
  t.NHits = (NNewHits + NOldHits);

  if (dir) {  // backward
    for (int i = NOldHits - 1; i >= 0; i--) {
      t.fHits[NNewHits + i] = t.fHits[i];
    }
    for (unsigned int i = 0, ii = NNewHits - 1; i < NNewHits; i++, ii--) {
      t.fHits[i] = newHits[ii];
    }
  }
  else {  // forward
    for (unsigned int i = 0; i < newHits.size(); i++) {
      t.fHits[NOldHits + i] = (newHits[i]);
    }
  }

  Tout = fit.Tr();

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
