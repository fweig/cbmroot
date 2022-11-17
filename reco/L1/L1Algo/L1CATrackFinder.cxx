/* Copyright (C) 2009-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina, Ivan Kisel, Sergey Gorbunov [committer], Igor Kulakov, Maksym Zyzak */

/*
 *=====================================================
 *
 *  CBM Level 1 4D Reconstruction
 *
 *  Authors: V.Akishina, I.Kisel,  S.Gorbunov, I.Kulakov, M.Zyzak
 *  Documentation: V.Akishina
 *
 *  e-mail : v.akishina@gsi.de
 *
 *=====================================================
 *
 *  Finds tracks using the Cellular Automaton algorithm
 *
 */


#include "CbmL1.h"
#include "CbmL1MCTrack.h"

#include "L1Algo.h"
#include "L1Assert.h"
#include "L1Branch.h"
#include "L1Extrapolation.h"
#include "L1Filtration.h"
#include "L1Fit.h"
#include "L1Grid.h"
#include "L1HitArea.h"
#include "L1HitPoint.h"
#include "L1Portion.h"
#include "L1Track.h"
#include "L1TrackPar.h"

#ifdef _OPENMP
#include "omp.h"
#include "pthread.h"
#endif  // _OPENMP

#include "TRandom.h"
#include "TStopwatch.h"

#include "L1HitsSortHelper.h"
#include "L1Timer.h"
#ifdef DRAW
#include "utils/L1AlgoDraw.h"
#endif  // DRAW
#ifdef PULLS
#include "utils/L1AlgoPulls.h"
#endif  // PULLS
#ifdef TRIP_PERFORMANCE
#include "utils/L1AlgoEfficiencyPerformance.h"
#endif  // TRIP_PERFORMANCE
#ifdef DOUB_PERFORMANCE
#include "utils/L1AlgoEfficiencyPerformance.h"
#endif  // DOUB_PERFORMANCE

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>

#include <stdio.h>

#include "CaToolsDebugger.h"


// using namespace std;
using std::cout;
using std::endl;


bool L1Algo::checkTripletMatch(const L1Triplet& l, const L1Triplet& r, fscal& dchi2) const
{
  dchi2 = 1.e20;

  if (r.GetMHit() != l.GetRHit()) return false;
  if (r.GetLHit() != l.GetMHit()) return false;

  if (r.GetMSta() != l.GetRSta()) return false;
  if (r.GetLSta() != l.GetMSta()) return false;


  if (r.IsMomentumFitted()) {
    assert(l.IsMomentumFitted());

    fscal dqp = l.GetQp() - r.GetQp();
    fscal Cqp = l.GetCqp() + r.GetCqp();

    if (!std::isfinite(dqp)) return false;
    if (!std::isfinite(Cqp)) return false;

    if (dqp * dqp > fTripletLinkChi2 * Cqp) {
      return false;  // bad neighbour // CHECKME why do we need recheck it?? (it really change result)
    }
    dchi2 = dqp * dqp / Cqp;
  }
  else {

    fscal dtx = l.GetTx() - r.GetTx();
    fscal Ctx = l.GetCtx() + r.GetCtx();

    fscal dty = l.GetTy() - r.GetTy();
    fscal Cty = l.GetCty() + r.GetCty();

    // it shouldn't happen, but happens sometimes

    if (!std::isfinite(dtx)) return false;
    if (!std::isfinite(dty)) return false;
    if (!std::isfinite(Ctx)) return false;
    if (!std::isfinite(Cty)) return false;

    if (dty * dty > fTripletLinkChi2 * Cty) return false;
    if (dtx * dtx > fTripletLinkChi2 * Ctx) return false;

    dchi2 = 0.5f * (dtx * dtx / Ctx + dty * dty / Cty);
  }

  if (!std::isfinite(dchi2)) return false;

  return true;
}

inline void L1Algo::findSingletsStep0(  // input
  Tindex start_lh, Tindex n1_l, L1HitPoint* Hits_l,
  // output
  fvec* u_front_l, fvec* u_back_l, fvec* zPos_l, L1HitIndex_t* hitsl, fvec* t_l, fvec* dt2_l, fvec* du2_l, fvec* dv2_l)
{

  /// Prepare the portion of data of left hits of a triplet:
  /// all hits except the last and the second last station will be procesesed in the algorithm,
  /// the data is orginesed in order to be used by SIMD

  const Tindex end_lh = start_lh + n1_l;
  const int lastV     = (n1_l - 1) / fvec::size();
  if (lastV >= 0) {
    // set some positive errors to unfilled part of vectors in order to avoid nans
    L1HitPoint& hitl = Hits_l[0];
    du2_l[lastV]     = hitl.dU2();
    dv2_l[lastV]     = hitl.dV2();
    dt2_l[lastV]     = hitl.dT2();
    u_front_l[lastV] = hitl.U();
    u_back_l[lastV]  = hitl.V();
    t_l[lastV]       = hitl.T();
    zPos_l[lastV]    = hitl.Z();
  }

  for (Tindex ilh = start_lh, i1 = 0; ilh < end_lh; ++ilh, ++i1) {
    Tindex i1_V      = i1 / fvec::size();
    Tindex i1_4      = i1 % fvec::size();
    L1HitPoint& hitl = Hits_l[ilh];


    t_l[i1_V][i1_4]   = hitl.T();
    dt2_l[i1_V][i1_4] = hitl.dT2();

    hitsl[i1]             = ilh;
    u_front_l[i1_V][i1_4] = hitl.U();
    u_back_l[i1_V][i1_4]  = hitl.V();

    du2_l[i1_V][i1_4] = hitl.dU2();
    dv2_l[i1_V][i1_4] = hitl.dV2();

    zPos_l[i1_V][i1_4] = hitl.Z();
  }
}


inline void L1Algo::findSingletsStep1(  /// input 1st stage of singlet search
  int istal,
  int istam,    /// indexes of left and middle stations of a triplet
  Tindex n1_V,  ///
  fvec* u_front_l, fvec* u_back_l, fvec* zPos_l, fvec* t_l, fvec* dt2_l,
  // output
  //                 L1TrackPar *T_1,
  L1TrackPar* T_1, L1FieldRegion* fld_1, fvec* du2_l, fvec* dv2_l)
{

  /// Get the field approximation. Add the target to parameters estimation.
  /// Propagaete to the middle station of a triplet.

  const L1Station& stal = fParameters.GetStation(istal);
  const L1Station& stam = fParameters.GetStation(istam);

  // stations for approximating the field between the target and the left hit
  int fld0Ista1 = istal;
  if (fld0Ista1 >= fNfieldStations) { fld0Ista1 = fNfieldStations - 1; }
  if (fld0Ista1 < 1) { fld0Ista1 = 1; }
  int fld0Ista0 = fld0Ista1 / 2;  // station between istal and the target

  assert(0 <= fld0Ista0 && fld0Ista0 < fld0Ista1 && fld0Ista1 < fParameters.GetNstationsActive());

  // stations for approximating the field between the left and the right hit
  int fld1Ista0 = istal;
  int fld1Ista1 = istam;
  int fld1Ista2 = istam + 1;
  if (fld1Ista2 >= fParameters.GetNstationsActive()) {
    fld1Ista2 = istam;
    fld1Ista1 = istam - 1;
    if (fld1Ista0 >= fld1Ista1) { fld1Ista0 = fld1Ista1 - 1; }
  }

  assert(0 <= fld1Ista0 && fld1Ista0 < fld1Ista1 && fld1Ista1 < fld1Ista2
         && fld1Ista2 < fParameters.GetNstationsActive());

  const L1Station& fld0Sta0 = fParameters.GetStation(fld0Ista0);
  const L1Station& fld0Sta1 = fParameters.GetStation(fld0Ista1);
  const L1Station& fld1Sta0 = fParameters.GetStation(fld1Ista0);
  const L1Station& fld1Sta1 = fParameters.GetStation(fld1Ista1);
  const L1Station& fld1Sta2 = fParameters.GetStation(fld1Ista2);

  L1TrackParFit fit;
  fit.fQp0 = fvec(0.);

  if (fpCurrentIteration->GetElectronFlag()) { fit.SetParticleMass(L1Constants::phys::kElectronMass); }
  else {
    fit.SetParticleMass(fDefaultMass);
  }

  for (int i1_V = 0; i1_V < n1_V; i1_V++) {
    L1TrackPar& T = fit.fTr;

    // field made by  the left hit, the target and the station istac in-between.
    // is used for extrapolation to the target and to the middle hit
    L1FieldRegion fld0;

    // field, made by the left hit, the middle station and the right station
    // Will be used for extrapolation to the right hit
    L1FieldRegion& fld1 = fld_1[i1_V];

    L1FieldValue lB, mB, cB, rB _fvecalignment;
    L1FieldValue l_B_global, centB_global _fvecalignment;

    // get the magnetic field along the track.
    // (suppose track is straight line with origin in the target)

    fvec& u         = u_front_l[i1_V];
    fvec& v         = u_back_l[i1_V];
    auto [xl, yl]   = stal.ConvUVtoXY<fvec>(u, v);
    fvec zl         = zPos_l[i1_V];
    fvec& time      = t_l[i1_V];
    fvec& timeEr2   = dt2_l[i1_V];
    const fvec dzli = 1.f / (zl - fTargZ);

    const fvec tx = (xl - fTargX) * dzli;
    const fvec ty = (yl - fTargY) * dzli;

    L1FieldValue b00, b01, b10, b11, b12;
    fld0Sta0.fieldSlice.GetFieldValue(fTargX + tx * (fld0Sta0.fZ - fTargZ), fTargY + ty * (fld0Sta0.fZ - fTargZ), b00);
    fld0Sta1.fieldSlice.GetFieldValue(fTargX + tx * (fld0Sta1.fZ - fTargZ), fTargY + ty * (fld0Sta1.fZ - fTargZ), b01);
    fld1Sta0.fieldSlice.GetFieldValue(fTargX + tx * (fld1Sta0.fZ - fTargZ), fTargY + ty * (fld1Sta0.fZ - fTargZ), b10);
    fld1Sta1.fieldSlice.GetFieldValue(fTargX + tx * (fld1Sta1.fZ - fTargZ), fTargY + ty * (fld1Sta1.fZ - fTargZ), b11);
    fld1Sta2.fieldSlice.GetFieldValue(fTargX + tx * (fld1Sta2.fZ - fTargZ), fTargY + ty * (fld1Sta2.fZ - fTargZ), b12);

    fld0.Set(fTargB, fTargZ, b00, fld0Sta0.fZ, b01, fld0Sta1.fZ);
    fld1.Set(b10, fld1Sta0.fZ, b11, fld1Sta1.fZ, b12, fld1Sta2.fZ);

    T.chi2 = 0.;
    T.NDF  = (fpCurrentIteration->GetPrimaryFlag()) ? fvec(2.) : fvec(0.);

    // TODO: iteration parameter: "Starting NDF of track parameters"
    // NDF = number of track parameters (6: x, y, tx, ty, qp, time) - number of measured parameters (3: x, y, time) on station or (2: x, y) on target
    // Alternative: Iteration can find tracks starting from target or from station: -> use a FLAG

    T.tx = tx;
    T.ty = ty;
    T.t  = time;

    T.qp  = fvec(0.);
    T.C20 = T.C21 = fvec(0.);
    T.C30 = T.C31 = T.C32 = fvec(0.);
    T.C40 = T.C41 = T.C42 = T.C43 = fvec(0.);
    T.C50 = T.C51 = T.C52 = T.C53 = T.C54 = fvec(0.);
    T.C22 = T.C33 = fMaxSlopePV * fMaxSlopePV / fvec(9.);
    if (kGlobal == fTrackingMode || kMcbm == fTrackingMode) T.C22 = T.C33 = fvec(10.);
    // TODO: Why 9. and 10.?


    T.C44 = fMaxInvMom / fvec(3.) * fMaxInvMom / fvec(3.);
    T.C55 = timeEr2;

    {  // add the target constraint
      T.x = xl;
      T.y = yl;
      T.z = zl;

      std::tie(T.C00, T.C10, T.C11) = stal.FormXYCovarianceMatrix(du2_l[i1_V], dv2_l[i1_V]);

      fit.AddTargetToLine(fTargX, fTargY, fTargZ, TargetXYInfo, fld0);
    }

    fit.AddMsInMaterial(fParameters.GetMaterialThickness(istal, T.x, T.y), fMaxInvMom, fvec::One());

    //if ((istam >= fNstationsBeforePipe) && (istal <= fNstationsBeforePipe - 1)) {
    //fit.L1AddPipeMaterial(T, fMaxInvMom, fvec::One());
    //}

    fvec dz = stam.fZ - zl;
    L1ExtrapolateTime(T, dz, fvec::One());

    // extrapolate to the middle hit
    L1Extrapolate0(T, stam.fZ, fld0);
    T_1[i1_V] = T;
  }  // i1_V
}


inline void L1Algo::findDoubletsStep0(
  /// input
  Tindex n1, const L1Station& stal, L1HitPoint* vHits_l, const L1Station& stam, L1HitPoint* vHits_m, L1TrackPar* T_1,
  L1HitIndex_t* hitsl_1,
  /// output
  Tindex& n2, L1Vector<L1HitIndex_t>& i1_2,
#ifdef DOUB_PERFORMANCE
  L1Vector<L1HitIndex_t>& hitsl_2,
#endif  // DOUB_PERFORMANCE
  L1Vector<L1HitIndex_t>& hitsm_2)
{
  /// Find the doublets. Reformat data into portions of doublets.

  assert(i1_2.size() == 0);

  int iStaL = &stal - fParameters.GetStations().begin();
  int iStaM = &stam - fParameters.GetStations().begin();

  n2 = 0;  // number of doublets

  for (Tindex i1 = 0; i1 < n1; ++i1)  // for each singlet
  {
    unsigned int Ndoublets = 0;
    const Tindex i1_V      = i1 / fvec::size();
    const Tindex i1_4      = i1 % fvec::size();
    L1TrackPar& T1         = T_1[i1_V];

    // assert(T1.IsEntryConsistent(true, i1_4));
    // if (!T1.IsEntryConsistent(false, i1_4)) continue;

    const fvec Pick_m22 = (fDoubletChi2Cut - T1.chi2);
    // if make it bigger the found hits will be rejected later because of the chi2 cut.
    // Pick_m22 is not used, search for mean squared, 2nd version

    unsigned int indFirstDoublet = hitsm_2.size();

    // -- collect possible doublets --
    const fscal iz         = 1.f / (T1.z[i1_4] - fParameters.GetTargetPositionZ()[0]);
    const fscal timeError2 = T1.C55[i1_4];
    const fscal time       = T1.t[i1_4];

    L1HitAreaTime areaTime(vGridTime[iStaM], T1.x[i1_4] * iz, T1.y[i1_4] * iz,
                           (sqrt(Pick_m22 * T1.C00) + fMaxDx[iStaM] + fMaxDZ * abs(T1.tx))[i1_4] * iz,
                           (sqrt(Pick_m22 * T1.C11) + fMaxDy[iStaM] + fMaxDZ * abs(T1.ty))[i1_4] * iz, time,
                           sqrt(timeError2) * 5);

    for (L1HitIndex_t imh = -1; true;) {  // loop over the hits in the area
      if (fParameters.DevIsIgnoreHitSearchAreas()) {
        imh++;
        if ((L1HitIndex_t) imh >= (HitsUnusedStopIndex[iStaM] - HitsUnusedStartIndex[iStaM])) { break; }
      }
      else {
        if (!areaTime.GetNext(imh)) { break; }
      }

      L1HitPoint& hitm = vHits_m[imh];
      if (hitm.IsSuppressed()) continue;

      const L1HitPoint& hitl = vHits_l[hitsl_1[i1]];

      if (fParameters.DevIsMatchDoubletsViaMc()) {  // trd2d
        int indL = HitsUnusedStartIndex[iStaL] + hitsl_1[i1];
        int indM = HitsUnusedStartIndex[iStaM] + imh;
        int iMC  = GetMcTrackIdForUnusedHit(indL);
        if (iMC < 0 || iMC != GetMcTrackIdForUnusedHit(indM)) { continue; }
      }

      // check y-boundaries
      //TODO: move hardcoded cuts to parameters
      if ((stam.timeInfo) && (stal.timeInfo)) {
        if (fabs(time - hitm.T()) > sqrt(timeError2 + hitm.dT2()) * 5) continue;
        if (fabs(time - hitm.T()) > 30) continue;
      }

      // - check whether hit belong to the window ( track position +\- errors ) -
      const fscal zm = hitm.Z();
      //       L1TrackPar T1_new = T1;
      //       fvec dz           = fvec(zm) - T1.z;
      // L1ExtrapolateTime(T1, dz);


      //       if (fabs(T1_new.t[i1_4]-hitm.time)>sqrt(T1_new.C55[i1_4]+hitm.timeEr*hitm.timeEr)*4) continue;
      //       if (fabs(T1_new.t[i1_4]-hitm.time)>sqrt(2.9*2.9)*5) continue;
      //       const fscal dt_est2 = Pick_m22[i1_4] * fabs(T1_new.C55[i1_4] + hitm.timeEr*hitm.timeEr);
      //       const fscal dt = hitm.time - T1_new.t[i1_4];
      //       if ( dt*dt > dt_est2 && dt < 0  ) continue;

      fvec y, C11;
      L1ExtrapolateYC11Line(T1, zm, y, C11);

      /// Covariation matrix of the hit
      auto [dxxScalMhit, dxyScalMhit, dyyScalMhit] = stam.FormXYCovarianceMatrix(hitm.dU2(), hitm.dV2());

      fscal dy_est2 = Pick_m22[i1_4] * fabs(C11[i1_4] + dyyScalMhit);

      auto [xm, ym] = stam.ConvUVtoXY<fscal>(hitm.U(), hitm.V());

      const fscal dY = ym - y[i1_4];

      if (dY * dY > dy_est2) continue;

      // check x-boundaries
      fvec x, C00;
      L1ExtrapolateXC00Line(T1, zm, x, C00);

      fscal dx_est2 = Pick_m22[i1_4] * fabs(C00[i1_4] + dxxScalMhit);

      const fscal dX = xm - x[i1_4];

      if (dX * dX > dx_est2) continue;

      // check chi2
      fvec C10;
      L1ExtrapolateC10Line(T1, zm, C10);
      fvec chi2 = T1.chi2;

      L1FilterChi2XYC00C10C11(stam.frontInfo, x, y, C00, C10, C11, chi2, hitm.U(), hitm.dU2());

      if (!fpCurrentIteration->GetTrackFromTripletsFlag()) {
        if (chi2[i1_4] > fDoubletChi2Cut) continue;
      }

      L1FilterChi2(stam.backInfo, x, y, C00, C10, C11, chi2, hitm.V(), hitm.dV2());

      // FilterTime(T1, hitm.T(), hitm.dT2());

      if (!fpCurrentIteration->GetTrackFromTripletsFlag()) {
        if (chi2[i1_4] > fDoubletChi2Cut) continue;
      }

      // check if there is a second hit on the same station
      {
        bool isOtherHit = 0;
        fscal dz        = hitm.Z() - hitl.Z();
        fscal tu        = (hitm.U() - hitl.U()) / dz;
        fscal tv        = (hitm.V() - hitl.V()) / dz;
        fscal tt        = (hitm.T() - hitl.T()) / dz;

        for (unsigned int imh1 = indFirstDoublet; imh1 < hitsm_2.size(); imh1++) {
          const L1HitPoint& hitm1 = vHits_m[hitsm_2[imh1]];

          if ((stam.timeInfo) && (stal.timeInfo)) {
            fscal dt = hitm.T() + tt * (hitm1.Z() - hitm.Z()) - hitm1.T();
            if (dt * dt > 30. * (hitm.dT2() + hitm1.dT2())) { continue; }
          }

          fscal du = hitm.U() + tu * (hitm1.Z() - hitm.Z()) - hitm1.U();
          if (du * du > 20. * (hitm.dU2() + hitm1.dU2())) { continue; }

          fscal dv = hitm.V() + tv * (hitm1.Z() - hitm.Z()) - hitm1.V();
          if (dv * dv > 30. * (hitm.dV2() + hitm1.dV2())) { continue; }

          if (fParameters.DevIsSuppressOverlapHitsViaMc()) {
            int indL  = HitsUnusedStartIndex[iStaL] + hitsl_1[i1];
            int indM  = HitsUnusedStartIndex[iStaM] + imh;
            int indM1 = HitsUnusedStartIndex[iStaM] + hitsm_2[imh1];
            int iMC   = GetMcTrackIdForUnusedHit(indL);
            if ((iMC != GetMcTrackIdForUnusedHit(indM)) || (iMC != GetMcTrackIdForUnusedHit(indM1))) { continue; }
          }

          isOtherHit = true;
          break;
        }

        if (isOtherHit) {
          hitm.SetIsSuppresed(1);
          continue;
        }
      }

      i1_2.push_back(i1);
#ifdef DOUB_PERFORMANCE
      hitsl_2.push_back(hitsl_1[i1]);
#endif  // DOUB_PERFORMANCE
      hitsm_2.push_back(imh);

      Ndoublets++;
      n2++;

#ifndef FAST_CODE
//TODO: optimise triplet portion limit and put a warning
// assert(Ndoublets < fParameters.GetMaxDoubletsPerSinglet());
#endif  // FAST_CODE

      if (Ndoublets >= fParameters.GetMaxDoubletsPerSinglet()) {
        n2 = n2 - Ndoublets;
        i1_2.reduce(i1_2.size() - Ndoublets);
        hitsm_2.reduce(hitsm_2.size() - Ndoublets);
        break;
      }
    }  // loop over the hits in the area

  }  // for i1
}


/// Add the middle hits to parameters estimation. Propagate to right station.
/// Find the triplets(right hit). Reformat data in the portion of triplets.
inline void L1Algo::findTripletsStep0(  // input
  L1HitPoint* vHits_r, int iStaL, int iStaM, int iStaR, L1HitPoint* vHits_m, L1TrackPar* T_1, L1FieldRegion* fld_1,
  L1HitIndex_t* hitsl_1, Tindex n2, L1Vector<L1HitIndex_t>& hitsm_2, L1Vector<L1HitIndex_t>& i1_2,
  // output
  Tindex& n3, L1Vector<L1TrackPar>& T_3, L1Vector<L1HitIndex_t>& hitsl_3, L1Vector<L1HitIndex_t>& hitsm_3,
  L1Vector<L1HitIndex_t>& hitsr_3, L1Vector<fvec>& u_front_3, L1Vector<fvec>& u_back_3, L1Vector<fvec>& z_Pos_3,
  L1Vector<fvec>& du2_3, L1Vector<fvec>& dv2_3, L1Vector<fvec>& t_3, L1Vector<fvec>& dt2_3)
{
  const L1Station& stal = fParameters.GetStation(iStaL);
  const L1Station& stam = fParameters.GetStation(iStaM);
  const L1Station& star = fParameters.GetStation(iStaR);

  L1HitIndex_t hitsl_2[fvec::size()] {L1NaN::SetNaN<L1HitIndex_t>()};
  L1HitIndex_t hitsm_2_tmp[fvec::size()] {L1NaN::SetNaN<L1HitIndex_t>()};
  L1TrackPar L1TrackPar_0;
  // SG!! to avoid nans in unfilled part
  //TODO: SG: investigate, it changes the results !!
  /*
  L1TrackPar_0.C00 = 1.f;
  L1TrackPar_0.C11 = 1.f;
  L1TrackPar_0.C22 = 1.f;
  L1TrackPar_0.C33 = 1.f;
  L1TrackPar_0.C44 = 1.f;
  L1TrackPar_0.C55 = 1.f;
  */

  bool isMomentumFitted = (fIsTargetField || (stal.fieldStatus != 0) || (stam.fieldStatus != 0));
  //bool isTimeFitted     = ((stal.timeInfo != 0) || (stam.timeInfo != 0));

  L1TrackParFit fit;
  fit.SetParticleMass(fDefaultMass);
  fit.fQp0 = fvec(0.);

  n3          = 0;
  Tindex n3_V = 0, n3_4 = 0;

  T_3.reset(1, L1TrackPar_0);
  u_front_3.reset(1, fvec::Zero());
  u_back_3.reset(1, fvec::Zero());
  z_Pos_3.reset(1, fvec::Zero());
  du2_3.reset(1, fvec::One());
  dv2_3.reset(1, fvec::One());
  t_3.reset(1, fvec::Zero());
  dt2_3.reset(1, fvec::One());

  assert(iStaR < fParameters.GetNstationsActive());  //TODO SG!!! check if it is needed

  if (iStaR >= fParameters.GetNstationsActive()) return;

  // ---- Add the middle hits to parameters estimation. Propagate to right station. ----

  for (Tindex i2 = 0; i2 < n2;) {
    L1TrackPar& T2 = fit.fTr;
    T2             = L1TrackPar_0;
    L1FieldRegion f2;
    // pack the data
    fvec u_front_2 = 0.f;
    fvec u_back_2  = 0.f;
    fvec du2_2     = 1.f;
    fvec dv2_2     = 1.f;
    fvec zPos_2    = 0.f;
    fvec t_2       = 0.f;
    fvec dt2_2     = 1.f;

    size_t n2_4 = 0;
    for (; n2_4 < fvec::size() && i2 < n2; i2++, n2_4++) {

      const Tindex& i1  = i1_2[i2];
      const Tindex i1_V = i1 / fvec::size();
      const Tindex i1_4 = i1 % fvec::size();

      const L1TrackPar& T1 = T_1[i1_V];

      //assert(T1.IsEntryConsistent(true, i1_4));
      //if (!T1.IsEntryConsistent(false, i1_4)) continue;

      const L1FieldRegion& f1 = fld_1[i1_V];
      T2.SetOneEntry(n2_4, T1, i1_4);
      f2.SetOneEntry(n2_4, f1, i1_4);

      const Tindex& imh      = hitsm_2[i2];
      const L1HitPoint& hitm = vHits_m[imh];
      u_front_2[n2_4]        = hitm.U();
      u_back_2[n2_4]         = hitm.V();
      zPos_2[n2_4]           = hitm.Z();
      t_2[n2_4]              = hitm.T();
      dt2_2[n2_4]            = hitm.dT2();
      //  num[n2_4] = hitm.track;
      du2_2[n2_4] = hitm.dU2();
      dv2_2[n2_4] = hitm.dV2();

      hitsl_2[n2_4]     = hitsl_1[i1];
      hitsm_2_tmp[n2_4] = hitsm_2[i2];
    }  // n2_4

    // add the middle hit

    fit.ExtrapolateLine(zPos_2, fvec::One());

    fit.Filter(stam.frontInfo, u_front_2, du2_2, fvec::One());
    fit.Filter(stam.backInfo, u_back_2, dv2_2, fvec::One());
    //fit.FilterTime(t_2, dt2_2, fvec::One(), stam.timeInfo);
    FilterTime(T2, t_2, dt2_2, stam.timeInfo);

    fit.AddMsInMaterial(fParameters.GetMaterialThickness(iStaM, T2.x, T2.y), isMomentumFitted ? T2.qp : fMaxInvMom,
                        fvec::One());

    //if ((iStaR >= fNstationsBeforePipe) && (iStaM <= fNstationsBeforePipe - 1)) { fit.L1AddPipeMaterial(T2, T2.qp, 1); }

    // extrapolate to the right hit station
    //fit.Extrapolate(star.fZ, T2.qp, f2,fvec::One());

    fvec dz2 = star.fZ - T2.z;
    L1ExtrapolateTime(T2, dz2, fvec::One());
    L1Extrapolate(T2, star.fZ, T2.qp, f2);

    // assert(T2.IsConsistent(true, n2_4));

    // ---- Find the triplets(right hit). Reformat data in the portion of triplets. ----
    for (size_t i2_4 = 0; i2_4 < n2_4; ++i2_4) {

      //if (!T2.IsEntryConsistent(false, i2_4)) { continue; }
      if (kSts == fTrackingMode && (T2.C44[i2_4] < 0)) { continue; }
      if (T2.C00[i2_4] < 0 || T2.C11[i2_4] < 0 || T2.C22[i2_4] < 0 || T2.C33[i2_4] < 0 || T2.C55[i2_4] < 0) continue;

      if (fabs(T2.tx[i2_4]) > fMaxSlope) continue;
      if (fabs(T2.ty[i2_4]) > fMaxSlope) continue;

      const fvec Pick_r22    = fTripletChi2Cut - T2.chi2 + (!fpCurrentIteration->GetTrackFromTripletsFlag() ? 0 : 1);
      const fscal timeError2 = T2.C55[i2_4];
      const fscal time       = T2.t[i2_4];
      // find first possible hit


      const fscal iz = 1.f / (T2.z[i2_4] - fParameters.GetTargetPositionZ()[0]);
      L1HitAreaTime area(vGridTime[&star - fParameters.GetStations().begin()], T2.x[i2_4] * iz, T2.y[i2_4] * iz,
                         (sqrt(Pick_r22 * T2.C00) + fMaxDx[iStaM] + fMaxDZ * abs(T2.tx))[i2_4] * iz,
                         (sqrt(Pick_r22 * T2.C11) + fMaxDy[iStaM] + fMaxDZ * abs(T2.ty))[i2_4] * iz, time,
                         sqrt(timeError2) * 5);

      L1HitIndex_t irh              = 0;
      L1HitIndex_t doubletNtriplets = 0;
      int irh1                      = -1;
      while (true) {
        if (fParameters.DevIsIgnoreHitSearchAreas()) {
          irh1++;
          if ((L1HitIndex_t) irh1 >= (HitsUnusedStopIndex[iStaR] - HitsUnusedStartIndex[iStaR])) break;
          irh = irh1;
        }
        else {
          if (!area.GetNext(irh)) break;
        }

        // while (area.GetNext(irh)) {
        //for (int irh = 0; irh < ( HitsUnusedStopIndex[iStaR] - HitsUnusedStartIndex[iStaR] ); irh++){
        const L1HitPoint& hitr = vHits_r[irh];
        if (hitr.IsSuppressed()) continue;

        if (fParameters.DevIsMatchTripletsViaMc()) {
          int indL = HitsUnusedStartIndex[iStaL] + hitsl_2[i2_4];
          int indM = HitsUnusedStartIndex[iStaM] + hitsm_2_tmp[i2_4];
          int indR = HitsUnusedStartIndex[iStaR] + irh;
          int mcL  = GetMcTrackIdForUnusedHit(indL);
          if (mcL < 0 || mcL != GetMcTrackIdForUnusedHit(indM) || mcL != GetMcTrackIdForUnusedHit(indR)) { continue; }
        }

        const fscal zr = hitr.Z();
        //  const fscal yr = hitr.Y();

        auto [xr, yr] = star.ConvUVtoXY<fscal>(hitr.U(), hitr.V());

        L1TrackParFit fit3;
        fit3.SetParticleMass(fDefaultMass);
        fit3.fQp0 = T2.qp;

        L1TrackPar& T_cur = fit3.fTr;
        T_cur             = T2;

        fit3.ExtrapolateLine(zr, fvec::One());

        if ((star.timeInfo) && (stam.timeInfo))
          if (fabs(T_cur.t[i2_4] - hitr.T()) > sqrt(T_cur.C55[i2_4] + hitr.dT2()) * 5) continue;

        // TODO: SG: hardcoded cut of 30 ns
        if ((star.timeInfo) && (stam.timeInfo))
          if (fabs(T_cur.t[i2_4] - hitr.T()) > 30) continue;

        // - check whether hit belong to the window ( track position +\- errors ) -
        // check lower boundary
        fvec y, C11;
        L1ExtrapolateYC11Line(T2, zr, y, C11);

        /// Covariation matrix of the hit
        auto [dxxScalRhit, dxyScalRhit, dyyScalRhit] = star.FormXYCovarianceMatrix(hitr.dU2(), hitr.dV2());

        fscal dy_est2 = (Pick_r22[i2_4] * (fabs(C11[i2_4] + dyyScalRhit)));

        const fscal dY  = yr - y[i2_4];
        const fscal dY2 = dY * dY;

        if (dY2 > dy_est2) continue;  // if (yr > y_plus_new [i2_4] ) continue;
        // check x-boundaries
        fvec x, C00;

        L1ExtrapolateXC00Line(T2, zr, x, C00);

        fscal dx_est2 = (Pick_r22[i2_4] * (fabs(C00[i2_4] + dxxScalRhit)));

        const fscal dX = xr - x[i2_4];
        if (dX * dX > dx_est2) continue;
        // check chi2  // not effective
        fvec C10;
        L1ExtrapolateC10Line(T2, zr, C10);
        fvec chi2 = T2.chi2;

        L1FilterChi2XYC00C10C11(star.frontInfo, x, y, C00, C10, C11, chi2, hitr.U(), hitr.dU2());

        L1FilterChi2(star.backInfo, x, y, C00, C10, C11, chi2, hitr.V(), hitr.dV2());

        FilterTime(T_cur, hitr.T(), hitr.dT2(), star.timeInfo);


        if (!fpCurrentIteration->GetTrackFromTripletsFlag()) {
          if (chi2[i2_4] > fTripletChi2Cut || C00[i2_4] < 0 || C11[i2_4] < 0 || T_cur.C55[i2_4] < 0) {
            continue;  // chi2_triplet < CHI2_CUT
          }
        }

#ifndef FAST_CODE
//TODO: optimise triplet portion limit and put a warning
// assert(doubletNtriplets < fParameters.GetMaxTripletPerDoublets());
#endif  // FAST_CODE

        if (doubletNtriplets >= fParameters.GetMaxTripletPerDoublets()) {
          LOG(debug) << "L1: GetMaxTripletPerDoublets==" << fParameters.GetMaxTripletPerDoublets()
                     << " reached in findTripletsStep0()";
          // reject already created triplets for this doublet
          n3   = n3 - doubletNtriplets;
          n3_V = n3 / fvec::size();
          n3_4 = n3 % fvec::size();
          hitsl_3.reduce(n3);
          hitsm_3.reduce(n3);
          hitsr_3.reduce(n3);
          break;
        }

        // pack triplet

        doubletNtriplets++;

        L1TrackPar& T3 = T_3[n3_V];

        hitsl_3.push_back(hitsl_2[i2_4]);
        hitsm_3.push_back(hitsm_2_tmp[i2_4]);
        hitsr_3.push_back(irh);


        T3.SetOneEntry(n3_4, T2, i2_4);
        u_front_3[n3_V][n3_4] = hitr.U();
        u_back_3[n3_V][n3_4]  = hitr.V();
        du2_3[n3_V][n3_4]     = hitr.dU2();
        dv2_3[n3_V][n3_4]     = hitr.dV2();
        z_Pos_3[n3_V][n3_4]   = zr;
        t_3[n3_V][n3_4]       = hitr.T();
        dt2_3[n3_V][n3_4]     = hitr.dT2();

        n3++;
        n3_V = n3 / fvec::size();
        n3_4 = n3 % fvec::size();

        assert(n3 == (int) hitsl_3.size());
        assert(n3 == (int) hitsm_3.size());
        assert(n3 == (int) hitsr_3.size());

        if (0 == n3_4) {
          T_3.push_back(L1TrackPar_0);
          u_front_3.push_back(fvec::Zero());
          u_back_3.push_back(fvec::Zero());
          z_Pos_3.push_back(fvec::Zero());
          du2_3.push_back(fvec::One());
          dv2_3.push_back(fvec::One());
          t_3.push_back(fvec::Zero());
          dt2_3.push_back(fvec::One());
        }
      }  // search area

    }  // i2_4
  }    // i2_V
}

/// Add the right hits to parameters estimation.
inline void L1Algo::findTripletsStep1(  // input
  Tindex n3_V, const L1Station& star, L1Vector<fvec>& u_front_, L1Vector<fvec>& u_back_, L1Vector<fvec>& z_Pos,
  L1Vector<fvec>& du2_3, L1Vector<fvec>& dv2_3, L1Vector<fvec>& t_3, L1Vector<fvec>& dt2_3,
  // output
  //                L1TrackPar *T_3
  L1Vector<L1TrackPar>& T_3)
{
  L1TrackParFit fit;
  fit.SetParticleMass(fDefaultMass);

  for (Tindex i3_V = 0; i3_V < n3_V; ++i3_V) {

    L1TrackPar& T3 = fit.fTr;
    T3             = T_3[i3_V];
    fit.fQp0       = fit.fTr.qp;

    fit.ExtrapolateLine(z_Pos[i3_V], fvec::One());

    fit.Filter(star.frontInfo, u_front_[i3_V], du2_3[i3_V], fvec::One());
    fit.Filter(star.backInfo, u_back_[i3_V], dv2_3[i3_V], fvec::One());

    if (kMcbm != fTrackingMode) { fit.FilterTime(t_3[i3_V], dt2_3[i3_V], fvec::One(), star.timeInfo); }
    T_3[i3_V] = T3;
  }
}


inline void L1Algo::findTripletsStep2(Tindex n3, int istal, int istam, int istar, L1Vector<L1TrackPar>& T_3,
                                      L1Vector<L1HitIndex_t>& hitsl_3, L1Vector<L1HitIndex_t>& hitsm_3,
                                      L1Vector<L1HitIndex_t>& hitsr_3, int nIterations)
{

  nIterations = 2;

  /// Refit Triplets

  ca::tools::Debugger::Instance().AddNtuple("triplets", "ev:mc:sta:p:vx:vy:vz:chi2:ndf");

  L1TrackParFit fit;

  if (fpCurrentIteration->GetElectronFlag()) { fit.SetParticleMass(L1Constants::phys::kElectronMass); }
  else {
    fit.SetParticleMass(GetDefaultParticleMass());
  }
  const int NHits = 3;  // triplets

  // prepare data
  int ista[NHits] = {istal, istam, istar};

  L1Station sta[3];
  for (int is = 0; is < NHits; ++is) {
    sta[is] = fParameters.GetStation(ista[is]);
  };

  const L1Station& stal = fParameters.GetStation(istal);
  const L1Station& stam = fParameters.GetStation(istam);
  const L1Station& star = fParameters.GetStation(istar);

  bool isMomentumFitted = ((stal.fieldStatus != 0) || (stam.fieldStatus != 0) || (star.fieldStatus != 0));
  bool isTimeFitted     = ((stal.timeInfo != 0) || (stam.timeInfo != 0) || (star.timeInfo != 0));
  fvec ndf              = -4;  // straight line
  ndf += isMomentumFitted ? -1 : 0;
  ndf += isTimeFitted ? -1 : 0;

  for (int i3 = 0; i3 < n3; ++i3) {
    int i3_V = i3 / fvec::size();
    int i3_4 = i3 % fvec::size();

    L1TrackPar& T3 = T_3[i3_V];

    //if (!T3.IsEntryConsistent(false, i3_4)) continue;

    // prepare data
    L1HitIndex_t ihit[NHits] = {(*RealIHitP)[hitsl_3[i3] + HitsUnusedStartIndex[ista[0]]],
                                (*RealIHitP)[hitsm_3[i3] + HitsUnusedStartIndex[ista[1]]],
                                (*RealIHitP)[hitsr_3[i3] + HitsUnusedStartIndex[ista[2]]]};

    if (fParameters.DevIsMatchTripletsViaMc()) {
      int mc1 = GetMcTrackIdForUnusedHit(ihit[0]);
      int mc2 = GetMcTrackIdForUnusedHit(ihit[1]);
      int mc3 = GetMcTrackIdForUnusedHit(ihit[2]);
      if ((mc1 != mc2) || (mc1 != mc3)) { continue; }
    }

    fscal u[NHits], v[NHits], t[NHits], x[NHits], y[NHits], z[NHits];
    fscal du2[NHits], dv2[NHits], dt2[NHits];

    for (int ih = 0; ih < NHits; ++ih) {
      const L1Hit& hit       = fInputData.GetHit(ihit[ih]);
      u[ih]                  = hit.u;
      v[ih]                  = hit.v;
      t[ih]                  = hit.t;
      std::tie(x[ih], y[ih]) = sta[ih].ConvUVtoXY<fscal>(u[ih], v[ih]);
      z[ih]                  = hit.z;

      du2[ih] = hit.du2;
      dv2[ih] = hit.dv2;
      dt2[ih] = hit.dt2;
    };

    // find the field along the track

    L1FieldValue B[3] _fvecalignment;
    L1FieldRegion fld _fvecalignment;
    L1FieldRegion fldTarget _fvecalignment;

    fvec tx[3] = {(x[1] - x[0]) / (z[1] - z[0]), (x[2] - x[0]) / (z[2] - z[0]), (x[2] - x[1]) / (z[2] - z[1])};
    fvec ty[3] = {(y[1] - y[0]) / (z[1] - z[0]), (y[2] - y[0]) / (z[2] - z[0]), (y[2] - y[1]) / (z[2] - z[1])};
    for (int ih = 0; ih < NHits; ++ih) {
      fvec dz = (sta[ih].fZ - z[ih]);
      sta[ih].fieldSlice.GetFieldValue(x[ih] + tx[ih] * dz, y[ih] + ty[ih] * dz, B[ih]);
    };

    fld.Set(B[0], sta[0].fZ, B[1], sta[1].fZ, B[2], sta[2].fZ);
    fldTarget.Set(fTargB, fTargZ, B[0], sta[0].fZ, B[1], sta[1].fZ);

    L1TrackPar& T = fit.fTr;

    // initial parameters
    {
      fvec dz01 = 1. / (z[1] - z[0]);
      T.tx      = (x[1] - x[0]) * dz01;
      T.ty      = (y[1] - y[0]) * dz01;
      T.qp      = 0.;  //(fscal) T3.qp[i3_4];
    }

    // repeat several times in order to improve the precision
    for (int iiter = 0; iiter < nIterations; ++iiter) {
      // fit forward
      {
        fvec qp0 = T.qp;
        if (qp0[0] > GetMaxInvMom()) { qp0 = GetMaxInvMom(); }
        if (qp0[0] < -GetMaxInvMom()) { qp0 = -GetMaxInvMom(); }

        T.ResetErrors(200., 200., 1., 1., 100., 1.e4);
        //T.ResetErrors(200., 200., 10., 10., 100., 1.e4);
        T.NDF   = ndf;
        T.qp    = 0.;
        int ih0 = 0;
        T.x     = x[ih0];
        T.y     = y[ih0];
        T.z     = z[ih0];
        T.t     = t[ih0];

        //std::tie(T.C00, T.C10, T.C11) = sta[ih0].FormXYCovarianceMatrix(du2[ih0], dv2[ih0]);

        fit.Filter(sta[ih0].frontInfo, u[ih0], du2[ih0], fvec::One());
        fit.Filter(sta[ih0].backInfo, v[ih0], dv2[ih0], fvec::One());
        fit.FilterTime(t[ih0], dt2[ih0], fvec::One(), sta[ih0].timeInfo);

        //  add the target constraint
        fit.AddTargetToLine(fTargX, fTargY, fTargZ, TargetXYInfo, fldTarget);

        for (int ih = 1; ih < NHits; ++ih) {
          fit.Extrapolate(z[ih], qp0, fld, fvec::One());
          fit.AddMsInMaterial(fParameters.GetMaterialThickness(ista[ih], T.x, T.y), qp0, fvec::One());
          //if (ista[ih] == fNstationsBeforePipe) { fit.AddPipeMaterial(qp0, fvec::One()); }
          fit.Filter(sta[ih].frontInfo, u[ih], du2[ih], fvec::One());
          fit.Filter(sta[ih].backInfo, v[ih], dv2[ih], fvec::One());
          fit.FilterTime(t[ih], dt2[ih], fvec::One(), sta[ih].timeInfo);
        }
      }

      if (iiter == nIterations - 1) break;

      // fit backward
      {
        fvec qp0 = T.qp;
        if (qp0[0] > GetMaxInvMom()) { qp0 = GetMaxInvMom(); }
        if (qp0[0] < -GetMaxInvMom()) { qp0 = -GetMaxInvMom(); }

        T.ResetErrors(200., 200., 1., 1., 100., 1.e4);
        T.NDF   = ndf;
        T.qp    = 0.;
        int ih0 = NHits - 1;
        T.x     = x[ih0];
        T.y     = y[ih0];
        T.z     = z[ih0];
        T.t     = t[ih0];
        T.C55   = dt2[ih0];

        //std::tie(T.C00, T.C10, T.C11) = sta[ih0].FormXYCovarianceMatrix(du2[ih0], dv2[ih0]);
        fit.Filter(sta[ih0].frontInfo, u[ih0], du2[ih0], fvec::One());
        fit.Filter(sta[ih0].backInfo, v[ih0], dv2[ih0], fvec::One());
        //fit.FilterTime(t[ih0], dt[ih0], fvec::One(), sta[ih0].timeInfo);

        for (int ih = NHits - 2; ih >= 0; --ih) {
          fit.Extrapolate(z[ih], qp0, fld, fvec::One());
          fit.AddMsInMaterial(fParameters.GetMaterialThickness(ista[ih], T.x, T.y), qp0, fvec::One());
          //if (ista[ih] == fNstationsBeforePipe - 1) { fit.AddPipeMaterial(qp0, fvec::One()); }
          fit.Filter(sta[ih].frontInfo, u[ih], du2[ih], fvec::One());
          fit.Filter(sta[ih].backInfo, v[ih], dv2[ih], fvec::One());
          //fit.FilterTime(t[ih], dt[ih], fvec::One(), sta[ih].timeInfo);
        }
      }
    }  // for iiter

    //cout << " chi2 " << T3.chi2[i3_4] << " " << T.chi2[0] << endl;
    T.chi2 = (fscal) T3.chi2[i3_4];  //SG!!!
    T3.SetOneEntry(i3_4, T, i3_4);

    {
      int mc1 = GetMcTrackIdForUnusedHit(ihit[0]);
      int mc2 = GetMcTrackIdForUnusedHit(ihit[1]);
      int mc3 = GetMcTrackIdForUnusedHit(ihit[2]);
      if ((mc1 >= 0) && (mc1 == mc2) && (mc1 == mc3)) {
        const CbmL1MCTrack& mctr = CbmL1::Instance()->GetMcTracks()[mc1];
        float ev                 = 0;
        float chi2               = T.chi2[i3_4];
        float nd                 = T.NDF[i3_4];
        ca::tools::Debugger::Instance().FillNtuple("triplets", ev, mc1, istal, mctr.p, mctr.x, mctr.y, mctr.z, chi2,
                                                   nd);
      }
    }
  }  //i3

}  // findTripletsStep2


inline void L1Algo::findTripletsStep3(  // input
  Tindex n3, int istal, int istam, int istar, L1Vector<L1TrackPar>& T_3, L1Vector<L1HitIndex_t>& hitsl_3,
  L1Vector<L1HitIndex_t>& hitsm_3, L1Vector<L1HitIndex_t>& hitsr_3)
{
  /// Selects good triplets and saves them into fTriplets.
  /// Finds neighbouring triplets at the next station.
  ///
  /// \param n3  Number of triplets to process
  /// \param istal  index of the left station
  /// \param istam  index of the middle station
  /// \param istar  index of the right station
  /// \param T_3  fitted trajectories of triplets
  /// \param hitsl_3  index of the left triplet hit in unused hits on the left station
  /// \param hitsm_3  index of the middle triplet hit in unused hits on the middle station
  /// \param hitsr_3  index of the right triplet hit in unused hits on the right station

#ifdef _OPENMP
  unsigned int Thread = omp_get_thread_num();
#else
  unsigned int Thread = 0;
#endif

  L1HitIndex_t ihitl_prev = 0;

  const L1Station& stal = fParameters.GetStation(istal);
  const L1Station& stam = fParameters.GetStation(istam);
  const L1Station& star = fParameters.GetStation(istar);

  bool isMomentumFitted = ((stal.fieldStatus != 0) || (stam.fieldStatus != 0) || (star.fieldStatus != 0));

  for (Tindex i3 = 0; i3 < n3; ++i3) {
    const Tindex i3_V = i3 / fvec::size();
    const Tindex i3_4 = i3 % fvec::size();

    L1TrackPar& T3 = T_3[i3_V];

    //if (!T3.IsEntryConsistent(false, i3_4)) continue;

    // select
    fscal chi2 = T3.chi2[i3_4];  // / T3.NDF[i3_4];

    const L1HitIndex_t ihitl = hitsl_3[i3] + HitsUnusedStartIndex[istal];
    const L1HitIndex_t ihitm = hitsm_3[i3] + HitsUnusedStartIndex[istam];
    const L1HitIndex_t ihitr = hitsr_3[i3] + HitsUnusedStartIndex[istar];
    L1_ASSERT(ihitl < HitsUnusedStopIndex[istal], ihitl << " < " << HitsUnusedStopIndex[istal]);
    L1_ASSERT(ihitm < HitsUnusedStopIndex[istam], ihitm << " < " << HitsUnusedStopIndex[istam]);
    L1_ASSERT(ihitr < HitsUnusedStopIndex[istar], ihitr << " < " << HitsUnusedStopIndex[istar]);

    unsigned int Location = PackTripletId(istal, Thread, fTriplets[istal][Thread].size());

    if (ihitl_prev != 1 + hitsl_3[i3]) {
      // TODO: SG: A bug! The code doesn't work for iterations with missing hits.
      // TODO: Triplets with the same left hit are not placed continiously in fTriplets vector.
      // TODO: Urgent!!!
      // assert(fHitNtriplets[ihitl] == 0);
      fHitFirstTriplet[ihitl] = Location;
      fHitNtriplets[ihitl]    = 0;
    }

    ihitl_prev = 1 + hitsl_3[i3];

    if (!fpCurrentIteration->GetTrackFromTripletsFlag()) {
      if (chi2 > fTripletFinalChi2Cut) { continue; }
    }

    // assert(std::isfinite(chi2) && chi2 > 0);

    if (!std::isfinite(chi2) || chi2 < 0) { continue; }
    //if (!T3.IsEntryConsistent(false, i3_4)) { continue; }

    fscal qp = T3.qp[i3_4];

    //TODO: why sqrt's? Wouldn't it be faster to skip sqrt() here and
    //TODO: compare the squared differences dqr*dqp later?

    fscal Cqp = T3.C44[i3_4];

    // TODO: SG: a magic correction that comes from the legacy code
    // removing it leads to a higher ghost ratio
    Cqp += 0.001;

    fTriplets[istal][Thread].emplace_back(ihitl, ihitm, ihitr, istal, istam, istar, 0, 0, 0, chi2, qp, Cqp, T3.tx[i3_4],
                                          T3.C22[i3_4], T3.ty[i3_4], T3.C33[i3_4]);

    L1Triplet& tr1 = fTriplets[istal][Thread].back();
    tr1.SetIsMomentumFitted(isMomentumFitted);
    tr1.SetLevel(0);
    tr1.SetFNeighbour(0);
    tr1.SetNNeighbours(0);

    fHitNtriplets[ihitl]++;

    if (istal > (fParameters.GetNstationsActive() - 4)) continue;

    unsigned int nNeighbours = fHitNtriplets[ihitm];

    unsigned int neighLocation = fHitFirstTriplet[ihitm];
    unsigned int neighStation  = TripletId2Station(neighLocation);
    unsigned int neighThread   = TripletId2Thread(neighLocation);
    unsigned int neighTriplet  = TripletId2Triplet(neighLocation);

    if (nNeighbours > 0) { assert((int) neighStation == istal + 1 || (int) neighStation == istal + 2); }

    unsigned char level = 0;

    for (unsigned int iN = 0; iN < nNeighbours; ++iN, ++neighTriplet, ++neighLocation) {

      L1Triplet& curNeighbour = fTriplets[neighStation][neighThread][neighTriplet];

      fscal dchi2 = 0.;
      if (!checkTripletMatch(tr1, curNeighbour, dchi2)) continue;

      if (tr1.GetFNeighbour() == 0) tr1.SetFNeighbour(neighLocation);

      tr1.SetNNeighbours(neighLocation - tr1.GetFNeighbour() + 1);

      const unsigned char jlevel = curNeighbour.GetLevel();

      if (level <= jlevel) level = jlevel + 1;
    }

    tr1.SetLevel(level);
  }
}

/// Find neighbours of triplets. Calculate level of triplets.
inline void L1Algo::f5(  // input
  // output

  int* nlevel)
{
  if (!fpCurrentIteration->GetTrackFromTripletsFlag()) {
    for (int istal = fParameters.GetNstationsActive() - 4; istal >= fFirstCAstation; istal--) {
      for (int tripType = 0; tripType < 3; tripType++)  // tT = 0 - 123triplet, tT = 1 - 124triplet, tT = 2 - 134triplet
      {
        // All iters - jump
        if ((!fpCurrentIteration->GetJumpedFlag() && tripType != 0)
            || (fpCurrentIteration->GetJumpedFlag() && tripType != 0
                && (fParameters.GetNstationsActive() - 4 == istal))) {
          continue;
        }

        int istam = istal + 1;
        int istar = istal + 2;
        switch (tripType) {
          case 1: istar++; break;
          case 2:
            istam++;
            istar++;
            break;
        }

        for (Tindex iThread = 0; iThread < fNThreads; ++iThread) {
          for (Tindex itrip = 0; itrip < (Tindex) fTriplets[istal][iThread].size(); ++itrip) {
            L1Triplet& trip = fTriplets[istal][iThread][itrip];
            if (istam != trip.GetMSta()) continue;
            if (istar != trip.GetRSta()) continue;

            unsigned char level = 0;
            //           fscal  chi2 = trip->GetChi2();

            L1HitIndex_t ihitl = trip.GetLHit();
            L1HitIndex_t ihitm = trip.GetMHit();
            L1HitIndex_t ihitr = trip.GetRHit();
            L1_ASSERT(ihitl < HitsUnusedStopIndex[istal], ihitl << " < " << HitsUnusedStopIndex[istal]);
            L1_ASSERT(ihitm < HitsUnusedStopIndex[istam], ihitm << " < " << HitsUnusedStopIndex[istam]);
            L1_ASSERT(ihitr < HitsUnusedStopIndex[istar], ihitr << " < " << HitsUnusedStopIndex[istar]);

            L1Vector<unsigned int> neighCands("L1CATrackFinder::neighCands");  // save neighbour candidates
            neighCands.reserve(8);                                             // ~average is 1-2 for central, up to 5

            unsigned int nNeighbours = fHitNtriplets[ihitm];

            unsigned int neighLocation = fHitFirstTriplet[ihitm];
            unsigned int neighStation  = TripletId2Station(neighLocation);
            unsigned int neighThread   = TripletId2Thread(neighLocation);
            unsigned int neighTriplet  = TripletId2Triplet(neighLocation);

            for (unsigned int iN = 0; iN < nNeighbours; ++iN, ++neighTriplet, ++neighLocation) {
              //    for (iN = first_triplet; iN <= last_triplet; ++iN){
              L1Triplet& neigh = fTriplets[neighStation][neighThread][neighTriplet];
              //      if (neigh.GetMSta() != istar) continue; // neighbours should have 2 common hits
              //      if (neigh.GetMHit() != ihitr) continue; //!!!

              fscal dchi2 = 0.;
              if (!checkTripletMatch(trip, neigh, dchi2)) continue;

              // calculate level
              unsigned char jlevel = neigh.GetLevel();
              if (level <= jlevel) level = jlevel + 1;
              if (level == jlevel + 1) neighCands.push_back(neighLocation);
            }

            // trip->neighbours.reset(0);
            // for (unsigned int in = 0; in < neighCands.size(); in++) {
            //   int ID           = neighCands[in];
            //   unsigned int Station     = TripletId2Station(ID);
            //   unsigned int Thread      = TripletId2Thread(ID);
            //   unsigned int Triplet     = TripletId2Triplet(ID);
            //   const int nLevel = fTriplets[Station][Thread][Triplet].GetLevel();
            //   if (level == nLevel + 1) trip->neighbours.push_back(Location);
            // }
            nlevel[level]++;
          }  // vTriplets
        }
      }  // tripType
    }    // istal
  }      // if !fpCurrentIteration->GetTrackFromTripletsFlag()
}

/// ------------------- doublets on station ----------------------

inline void L1Algo::CreatePortionOfDoublets(
  /// input:
  const int istal, const int istam, const Tindex iSingletPortion, const Tindex singletPortionSize,
  /// output:
  L1TrackPar* T_1, L1FieldRegion* fld_1, L1HitIndex_t* hitsl_1, Tindex& n_2, L1Vector<L1HitIndex_t>& i1_2,
  L1Vector<L1HitIndex_t>& hitsm_2
  ///
)
{
  /// creates doublets
  /// input:
  ///   @istal - start station number
  ///   @istam - last station number
  ///   @iSingletPortion - index of portion of left hits
  ///   @singletPortionSize - number of left hits in the portion
  /// output:
  ///   @*T_1 - singlet parameters
  ///   @*fld_1 - field aproximation for singlets
  ///   @*hitsl_1- left hits of future triplets
  ///   @&n_2 - number of doublets
  ///   @&i1_2 - index of 1st hit in portion indexed by doublet index
  ///   @&hitsm_2 - index of middle hit in hits array indexed by doublet index

  if (istam < fParameters.GetNstationsActive()) {
    const L1Station& stal = fParameters.GetStation(istal);
    const L1Station& stam = fParameters.GetStation(istam);

    // prepare data
    L1HitPoint* vHits_l = &((*vHitPointsUnused)[0]) + HitsUnusedStartIndex[istal];
    L1HitPoint* vHits_m = &((*vHitPointsUnused)[0]) + HitsUnusedStartIndex[istam];

    fvec u_front[L1Constants::size::kSingletPortionSizeVec];
    fvec u_back[L1Constants::size::kSingletPortionSizeVec];
    fvec dv2[L1Constants::size::kSingletPortionSizeVec];
    fvec du2[L1Constants::size::kSingletPortionSizeVec];
    fvec zPos[L1Constants::size::kSingletPortionSizeVec];
    fvec t[L1Constants::size::kSingletPortionSizeVec];
    fvec dt2[L1Constants::size::kSingletPortionSizeVec];

    /// prepare the portion of left hits data

    findSingletsStep0(  // input
      iSingletPortion * L1Constants::size::kSingletPortionSize, singletPortionSize, vHits_l,
      // output
      u_front, u_back, zPos, hitsl_1, t, dt2, du2, dv2);

    for (Tindex i = 0; i < singletPortionSize; ++i)
      L1_ASSERT(hitsl_1[i] < HitsUnusedStopIndex[istal] - HitsUnusedStartIndex[istal],
                hitsl_1[i] << " < " << HitsUnusedStopIndex[istal] - HitsUnusedStartIndex[istal]);

    Tindex portionSize_V = (singletPortionSize + fvec::size() - 1) / fvec::size();

    /// Get the field approximation. Add the target to parameters estimation. Propagaete to middle station.

    findSingletsStep1(istal, istam, portionSize_V, u_front, u_back, zPos, t, dt2,
                      // output
                      T_1, fld_1, du2, dv2);

    /// Find the doublets. Reformat data in the portion of doublets.

    // TODO: repalce with constexpr if (C++17) (S.Zharko)
#ifdef DOUB_PERFORMANCE
    L1Vector<L1HitIndex_t> hitsl_2("L1CATrackFinder::hitsl_2");
#endif  // DOUB_PERFORMANCE

    findDoubletsStep0(  // input
      singletPortionSize, stal, vHits_l, stam, vHits_m, T_1, hitsl_1,
      // output
      n_2, i1_2,
#ifdef DOUB_PERFORMANCE
      hitsl_2,
#endif  // DOUB_PERFORMANCE
      hitsm_2);

    for (Tindex i = 0; i < static_cast<Tindex>(hitsm_2.size()); ++i)
      L1_ASSERT(hitsm_2[i] < HitsUnusedStopIndex[istam] - HitsUnusedStartIndex[istam],
                hitsm_2[i] << " " << HitsUnusedStopIndex[istam] - HitsUnusedStartIndex[istam]);

#ifdef DOUB_PERFORMANCE
    L1HitIndex_t* RealIHitL = &((*RealIHitP)[HitsUnusedStartIndex[istal]]);
    L1HitIndex_t* RealIHitM = &((*RealIHitP)[HitsUnusedStartIndex[istam]]);
    for (Tindex i = 0; i < n2; ++i) {
      // int i_4 = i%4;
      // int i_V = i/4;
      L1HitIndex_t iHits[2] = {RealIHitL[hitsl_2[i]], RealIHitM[hitsm_2[i]]};
      fL1Eff_doublets->AddOne(iHits);
    }
#endif  // DOUB_PERFORMANCE
  }
}


/// ------------------- Triplets on station ----------------------

inline void L1Algo::CreatePortionOfTriplets(
  /// input
  int istal, int istam, int istar,

  /// input / output
  L1TrackPar* T_1, L1FieldRegion* fld_1, L1HitIndex_t* hitsl_1,

  Tindex& n_2, L1Vector<L1HitIndex_t>& i1_2, L1Vector<L1HitIndex_t>& hitsm_2)
{

  /// creates a portion of triplets:
  /// input:
  ///  @istal - left station number,
  ///  @istam - middle station number,
  ///  @istar - right station number,

  /// input / output:

  /// @*T_1 - track parameters for singlets,
  /// @*fld_1 - field approximation for singlets,
  /// @&n_2 - number of doublets in portion
  /// @&n_2 - number of doublets,
  /// @&i1_2 - index of 1st hit in portion indexed by doublet index,
  /// @&hitsm_2 - index of middle hit in hits array indexed by doublet index
  /// output:
  // @*vTriplets_part - array of triplets,
  // @*TripStartIndexH,
  // @*TripStopIndexH - start/stop index of a triplet in the array

  if (istar < fParameters.GetNstationsActive()) {
    // prepare data
    const L1Station& star = fParameters.GetStation(istar);

    L1HitPoint* vHits_m = &((*vHitPointsUnused)[0]) + HitsUnusedStartIndex[istam];

    L1HitPoint* vHits_r = 0;
    vHits_r             = &((*vHitPointsUnused)[0]) + HitsUnusedStartIndex[istar];

    Tindex n3 = 0;

    /// Add the middle hits to parameters estimation. Propagate to right station.


#ifdef _OPENMP
    int Thread = omp_get_thread_num();
#else
    int Thread = 0;
#endif

    L1Vector<L1TrackPar>& T_3       = fTripletPar[Thread];
    L1Vector<L1HitIndex_t>& hitsl_3 = fTripletHitsL[Thread];
    L1Vector<L1HitIndex_t>& hitsm_3 = fTripletHitsM[Thread];
    L1Vector<L1HitIndex_t>& hitsr_3 = fTripletHitsR[Thread];
    L1Vector<fvec>& u_front3        = fTripletHitR_Ufront[Thread];
    L1Vector<fvec>& u_back3         = fTripletHitR_Uback[Thread];
    L1Vector<fvec>& z_pos3          = fTripletHitR_Z[Thread];
    L1Vector<fvec>& t_3             = fTripletHitR_Time[Thread];
    L1Vector<fvec>& dt2_3           = fTripletHitR_TimeErr[Thread];
    L1Vector<fvec>& du2_3           = fTripletHitR_dUfront[Thread];
    L1Vector<fvec>& dv2_3           = fTripletHitR_dUback[Thread];

    T_3.clear();
    hitsl_3.clear();
    hitsm_3.clear();
    hitsr_3.clear();
    u_front3.clear();
    u_back3.clear();
    z_pos3.clear();
    du2_3.clear();
    dv2_3.clear();
    t_3.clear();
    dt2_3.clear();

    /// Find the triplets(right hit). Reformat data in the portion of triplets.

    findTripletsStep0(  // input
      vHits_r,

      istal, istam, istar, vHits_m, T_1, fld_1, hitsl_1,

      n_2, hitsm_2, i1_2,

      // output
      n3, T_3, hitsl_3, hitsm_3, hitsr_3, u_front3, u_back3, z_pos3, du2_3, dv2_3, t_3, dt2_3);


    for (Tindex i = 0; i < static_cast<Tindex>(hitsl_3.size()); ++i)
      L1_assert(hitsl_3[i] < HitsUnusedStopIndex[istal] - HitsUnusedStartIndex[istal]);
    for (Tindex i = 0; i < static_cast<Tindex>(hitsm_3.size()); ++i)
      L1_assert(hitsm_3[i] < HitsUnusedStopIndex[istam] - HitsUnusedStartIndex[istam]);
    for (Tindex i = 0; i < static_cast<Tindex>(hitsr_3.size()); ++i)
      L1_assert(hitsr_3[i] < HitsUnusedStopIndex[istar] - HitsUnusedStartIndex[istar]);

    /// Add the right hits to parameters estimation.


    Tindex n3_V = (n3 + fvec::size() - 1) / fvec::size();
    findTripletsStep1(  // input
      n3_V, star, u_front3, u_back3, z_pos3, du2_3, dv2_3, t_3, dt2_3,
      // output
      T_3);


    /// refit
    findTripletsStep2(n3, istal, istam, istar, T_3, hitsl_3, hitsm_3, hitsr_3, 1);

#ifdef TRIP_PERFORMANCE
    L1HitIndex_t* RealIHitL = &((*RealIHitP)[HitsUnusedStartIndex[istal]]);
    L1HitIndex_t* RealIHitM = &((*RealIHitP)[HitsUnusedStartIndex[istam]]);
    L1HitIndex_t* RealIHitR = &((*RealIHitP)[HitsUnusedStartIndex[istar]]);
    for (Tindex i = 0; i < n3; ++i) {
      Tindex i_4            = i % 4;
      Tindex i_V            = i / 4;
      L1HitIndex_t iHits[3] = {RealIHitL[hitsl_3[i]], RealIHitM[hitsm_3[i]], RealIHitR[hitsr_3[i]]};
#ifdef PULLS
      if (fL1Eff_triplets->AddOne(iHits)) fL1Pulls->AddOne(T_3[i_V], i_4, iHits[2]);
#else
      fL1Eff_triplets->AddOne(iHits);
#endif
      if (T_3[i_V].chi2[i_4] < fTripletFinalChi2Cut) { fL1Eff_triplets2->AddOne(iHits); }
    }
#endif  // TRIP_PERFORMANCE


    /// Fill Triplets.
    findTripletsStep3(  // input
      n3, istal, istam, istar, T_3, hitsl_3, hitsm_3, hitsr_3);
  }
}


// hitCheck::hitCheck()
// {
//          omp_init_lock(&Occupied);
//          trackCandidateIndex = -1;
//          UsedByTrack=0;
//          Chi2Track = 100000000;
//          Length = 0;
//          ista = 1000;
//
// }
// hitCheck::~hitCheck()
// {
// omp_destroy_lock(&Occupied);
// }


// **************************************************************************************************
// *                                                                                                *
// *                            ------ CATrackFinder procedure ------                               *
// *                                                                                                *
// **************************************************************************************************

void L1Algo::CATrackFinder()
{
  fNFindIterations = fParameters.GetNcaIterations();


#ifdef _OPENMP
  omp_set_num_threads(fNThreads);
#endif

  /*******************************/ /**
   * Performance monitors setting
   **********************************/

#ifdef PULLS
  static L1AlgoPulls* l1Pulls_ = new L1AlgoPulls();
  fL1Pulls                     = l1Pulls_;
  fL1Pulls->Init();
#endif
#ifdef TRIP_PERFORMANCE
  static L1AlgoEfficiencyPerformance<3>* l1Eff_triplets_ = new L1AlgoEfficiencyPerformance<3>();
  fL1Eff_triplets                                        = l1Eff_triplets_;
  fL1Eff_triplets->Init();
  static L1AlgoEfficiencyPerformance<3>* l1Eff_triplets2_ = new L1AlgoEfficiencyPerformance<3>();
  fL1Eff_triplets2                                        = l1Eff_triplets2_;
  fL1Eff_triplets2->Init();
#endif
#ifdef DOUB_PERFORMANCE
  static L1AlgoEfficiencyPerformance<2>* l1Eff_doublets_ = new L1AlgoEfficiencyPerformance<2>();
  fL1Eff_doublets                                        = l1Eff_doublets_;
  fL1Eff_doublets->Init();
#endif

#ifdef DRAW
  if (!draw) draw = new L1AlgoDraw;
  draw->InitL1Draw(this);
#endif

  TStopwatch c_time;  // for performance time
#if defined(XXX) || defined(COUNTERS)
  static unsigned int stat_N = 0;  // number of events
  stat_N++;
#endif

#ifdef XXX
  TStopwatch c_timerG;  // global
  TStopwatch c_timerI;  // for iterations

  L1CATFIterTimerInfo gti;  // global
  gti.Add("init  ");
  gti.Add("iterts");
  gti.Add("merge ");

  L1CATFTimerInfo ti;
  ti.SetNIter(fNFindIterations);  // for iterations
  ti.Add("init  ");
  // ti.Add("dblte1");
  // ti.Add("dblte2");
  ti.Add("tripl1");


  ti.Add("tracks");
  ti.Add("table");
  ti.Add("save");
  ti.Add("delete");
  ti.Add("copy");
  ti.Add("finish");

  static L1CATFIterTimerInfo stat_gti = gti;
  static L1CATFTimerInfo stat_ti      = ti;
#endif  // XXX

#ifdef COUNTERS
  static Tindex stat_nStartHits              = 0;
  static Tindex stat_nHits[fNFindIterations] = {0};

  static Tindex stat_nSinglets[fNFindIterations] = {0};
  //  static Tindex stat_nDoublets[fNFindIterations] = {0};
  static Tindex stat_nTriplets[fNFindIterations] = {0};

  static Tindex stat_nLevels[L1Constants::size::kMaxNstations - 2][fNFindIterations] = {{0}};
  static Tindex stat_nCalls[fNFindIterations]                                        = {0};  // n calls of CAFindTrack
  static Tindex stat_nTrCandidates[fNFindIterations]                                 = {0};
#endif

  /********************************/ /**
   * CATrackFinder routine setting
   ***********************************/

  RealIHitP                        = &RealIHit_v;
  RealIHitPBuf                     = &RealIHit_v_buf;
  vHitsUnused                      = &vNotUsedHits_B;  /// array of hits used on current iteration
  L1Vector<L1Hit>* vHitsUnused_buf = &vNotUsedHits_A;  /// buffer for copy

  vHitPointsUnused                           = &vNotUsedHitsxy_B;  /// array of info for hits used on current iteration
  L1Vector<L1HitPoint>* vHitPointsUnused_buf = &vNotUsedHitsxy_A;

  NHitsIsecAll = 0;
  fTracks.clear();
  fRecoHits.clear();

  fRecoHits.reserve(2 * fInputData.GetNhits());
  fTracks.reserve(2 * fInputData.GetNhits() / fParameters.GetNstationsActive());

  int nNotUsedHits = 0;

  // #pragma omp parallel for  reduction(+:nNotUsedHits)
  for (int ista = 0; ista < fParameters.GetNstationsActive(); ++ista) {
    nNotUsedHits += (fInputData.GetStopHitIndex(ista) - fInputData.GetStartHitIndex(ista));
    HitsUnusedStartIndex[ista] = fInputData.GetStartHitIndex(ista);
    HitsUnusedStopIndex[ista]  = fInputData.GetStopHitIndex(ista);
  }

#ifdef XXX
  c_time.Start();
  c_timerG.Start();
#endif  // XXX

  fscal yStep = 1.5 / sqrt(nNotUsedHits);  // empirics. 0.01*sqrt(2374) ~= 0.5


  //  fscal yStep = 0.5 / sqrt(nNotUsedHits);  // empirics. 0.01*sqrt(2374) ~= 0.5
  if (yStep > 0.3) yStep = 0.3;
  fscal xStep = yStep * 3;
  // fscal xStep = yStep * 3;

  //  yStep = 0.0078;
  //  const fscal hitDensity = sqrt( nNotUsedHits );

  //     fscal yStep = 0.7*4/hitDensity; // empirics. 0.01*sqrt(2374) ~= 0.5
  //     if (yStep > 0.3)
  //      yStep = 1.25;
  //      xStep = 2.05;

  vHitsUnused = &vNotUsedHits_Buf;

  for (int iS = 0; iS < fParameters.GetNstationsActive(); ++iS) {
    const L1Station& st    = fParameters.GetStation(iS);
    fMaxDx[iS]             = 0.;
    fMaxDy[iS]             = 0.;
    fMaxDt[iS]             = 0.;
    bool timeUninitialised = 1;
    fscal lasttime         = 0;
    fscal starttime        = 0;
    for (L1HitIndex_t ih = fInputData.GetStartHitIndex(iS); ih < fInputData.GetStopHitIndex(iS); ++ih) {
      const L1Hit& h       = fInputData.GetHit(ih);
      auto [dxx, dxy, dyy] = st.FormXYCovarianceMatrix(h.du2, h.dv2);
      fscal dx             = sqrt(dxx);
      fscal dy             = sqrt(dyy);
      fscal dt             = sqrt(h.dt2);

      if (fMaxDx[iS] < dx) { fMaxDx[iS] = dx; }
      if (fMaxDy[iS] < dy) { fMaxDy[iS] = dy; }
      if (fMaxDt[iS] < dt) { fMaxDt[iS] = dt; }

      const fscal time = h.t;
      assert(std::isfinite(time));
      if (timeUninitialised || lasttime < time) { lasttime = time; }
      if (timeUninitialised || starttime > time) { starttime = time; }
      timeUninitialised = false;
    }

    vGridTime[iS].BuildBins(-1, 1, -0.6, 0.6, starttime, lasttime, xStep, yStep, (lasttime - starttime + 1));
    int start = HitsUnusedStartIndex[iS];
    int nhits = HitsUnusedStopIndex[iS] - start;

    if (nhits > 0) {
      vGridTime[iS].StoreHits(nhits, &(fInputData.GetHit(start)), iS, *this, start, &(vNotUsedHits_Buf[start]),
                              &(fInputData.GetHit(start)), &(RealIHit_v[start]));
    }
    else {  // to avoid out-of-range crash in array[start]
      vGridTime[iS].StoreHits(nhits, nullptr, iS, *this, start, nullptr, nullptr, nullptr);
    }
  }


  for (int ist = 0; ist < fParameters.GetNstationsActive(); ++ist)
    for (L1HitIndex_t ih = fInputData.GetStartHitIndex(ist); ih < fInputData.GetStopHitIndex(ist); ++ih) {
      const L1Hit& h = fInputData.GetHit(ih);
      //SetFUnUsed((*fStripFlag)[h.f]);
      //SetFUnUsed((*fStripFlag)[h.b]);
      //L1_SHOW(fInputData.GetNhitKeys());
      //L1_SHOW(fvHitKeyFlags.size());
      //L1_SHOW(h.f);
      //L1_SHOW(h.b);
      fvHitKeyFlags[h.f] = 0;
      fvHitKeyFlags[h.b] = 0;
    }

  for (int ista = 0; ista < fParameters.GetNstationsActive(); ++ista) {
#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic, 5)
#endif
    for (L1HitIndex_t ih = fInputData.GetStartHitIndex(ista); ih < fInputData.GetStopHitIndex(ista); ++ih) {
      CreateHitPoint(vNotUsedHits_Buf[ih], vNotUsedHitsxy_B[ih]);
    }
  }

#ifdef COUNTERS
  stat_nStartHits += nNotUsedHits;
#endif

#ifdef XXX
  c_timerG.Stop();
  gti["init  "] = c_timerG;
  c_timerG.Start();
#endif

  TStopwatch c_time1;
  c_time1.Start();

  /********************************/ /**
   *  Loop over tracking iterations
   ***********************************/


  // ---- Loop over Track Finder iterations ----------------------------------------------------------------//


  L1ASSERT(0, fNFindIterations == (int) fParameters.GetCAIterations().size());
  isec = 0;  // TODO: temporary! (S.Zharko)

  for (const auto& caIteration : fParameters.GetCAIterations())  // all finder
  {
    fpCurrentIteration = &caIteration;  // select current iteration

    for (int n = 0; n < fNThreads; n++) {
      for (int j = 0; j < fParameters.GetNstationsActive(); j++) {
        fTriplets[j][n].clear();
      }
    }
    /// isec - number of current iterations, fNFindIterations - number of all iterations
#ifdef COUNTERS
    Tindex nSinglets = 0;
#endif

    if (isec != 0) {
      L1Vector<L1HitIndex_t>* RealIHitPTmp = RealIHitP;
      RealIHitP                            = RealIHitPBuf;
      RealIHitPBuf                         = RealIHitPTmp;

      L1Vector<L1Hit>* vHitsUnused_temp = vHitsUnused;
      vHitsUnused                       = vHitsUnused_buf;
      vHitsUnused_buf                   = vHitsUnused_temp;

      L1Vector<L1HitPoint>* vHitsUnused_temp2 = vHitPointsUnused;
      vHitPointsUnused                        = vHitPointsUnused_buf;
      vHitPointsUnused_buf                    = vHitsUnused_temp2;
    }
    // TODO: Replace NStations with fInitManager.GetNstationsGeom() (S.Zharko)
    for (int ist = 0; ist < fParameters.GetNstationsActive(); ++ist) {
      for (L1HitIndex_t ih = HitsUnusedStartIndex[ist]; ih < HitsUnusedStopIndex[ist]; ++ih) {
        //SG!!
        fHitFirstTriplet[ih] = 0;
        fHitNtriplets[ih]    = 0;
      }
    }
    /*
   fHitFirstTriplet.assign(HitsUnusedStopIndex[fParameters.GetNstationsActive() - 1],0);
   fHitNtriplets.assign(HitsUnusedStopIndex[fParameters.GetNstationsActive() - 1],0);
*/
    {
      // #pragma omp  task
      {
        // --- SET PARAMETERS FOR THE ITERATION ---

        fFirstCAstation      = caIteration.GetFirstStationIndex();
        fTrackChi2Cut        = caIteration.GetTrackChi2Cut();
        fDoubletChi2Cut      = caIteration.GetDoubletChi2Cut();  //11.3449 * 2.f / 3.f;  // prob = 0.1
        fTripletChi2Cut      = caIteration.GetTripletChi2Cut();  //21.1075;  // prob = 0.01%
        fTripletFinalChi2Cut = caIteration.GetTripletFinalChi2Cut();
        fPickGather          = caIteration.GetPickGather();       //3.0;
        fTripletLinkChi2     = caIteration.GetTripletLinkChi2();  //5.0;
        fMaxInvMom           = caIteration.GetMaxQp();            //1.0 / 0.5;  // max considered q/p
        fMaxSlopePV          = caIteration.GetMaxSlopePV();       //1.1;
        fMaxSlope            = caIteration.GetMaxSlope();         //2.748;  // corresponds to 70 grad

        // define the target
        fTargX = fParameters.GetTargetPositionX();
        fTargY = fParameters.GetTargetPositionY();
        fTargZ = fParameters.GetTargetPositionZ();

        fscal SigmaTargetX = caIteration.GetTargetPosSigmaX();
        fscal SigmaTargetY = caIteration.GetTargetPosSigmaY();  // target constraint [cm]

        // Select magnetic field. For primary tracks - fVtxFieldValue, for secondary tracks - st.fieldSlice
        if (caIteration.GetPrimaryFlag()) { fTargB = fParameters.GetVertexFieldValue(); }
        else {
          fParameters.GetStation(0).fieldSlice.GetFieldValue(0, 0, fTargB);
        }  // NOTE: calculates field fTargB in the center of 0th station

        fIsTargetField = (fabs(fTargB.y[0]) > 0.001);

        TargetXYInfo.C00 = SigmaTargetX * SigmaTargetX;
        TargetXYInfo.C10 = 0;
        TargetXYInfo.C11 = SigmaTargetY * SigmaTargetY;

        /// Set correction in order to take into account overlaping and iff z.
        /// The reason is that low momentum tracks are too curved and goes not from target direction. That's why sort by hit_y/hit_z is not work idealy
        /// If sort by y then it is max diff between same station's modules (~0.4cm)
        fMaxDZ = caIteration.GetMaxDZ();  //0;
      }

#ifndef L1_NO_ASSERT
      for (int istal = fParameters.GetNstationsActive() - 1; istal >= 0; istal--) {
        L1_ASSERT(HitsUnusedStopIndex[istal] >= HitsUnusedStartIndex[istal],
                  HitsUnusedStopIndex[istal] << " >= " << HitsUnusedStartIndex[istal]);
        L1_ASSERT(HitsUnusedStopIndex[istal] <= (*vHitsUnused).size(),
                  HitsUnusedStopIndex[istal] << " <= " << (*vHitsUnused).size());
      }
#endif  // L1_NO_ASSERT
    }


    {
      /// possible left hits of triplets are splited in portions of 16 (4 SIMDs) to use memory faster

      for (int istal = fParameters.GetNstationsActive() - 2; istal >= fFirstCAstation;
           istal--) {  //start downstream chambers
        fSingletPortionSize[istal].clear();
        int NHits_l   = HitsUnusedStopIndex[istal] - HitsUnusedStartIndex[istal];
        int nPortions = NHits_l / L1Constants::size::kSingletPortionSize;
        int rest      = NHits_l - nPortions * L1Constants::size::kSingletPortionSize;
        for (int ipp = 0; ipp < nPortions; ipp++) {
          fSingletPortionSize[istal].push_back(L1Constants::size::kSingletPortionSize);
        }  // start_lh - portion of left hits
        if (rest > 0) fSingletPortionSize[istal].push_back(rest);
      }  // lstations


#ifdef COUNTERS
      stat_nSinglets[isec] += nSinglets;
#endif
    }

    /*    {
         /// possible left hits of triplets are splited in portions of 16 (4 SIMDs) to use memory faster
         fDoubletPortionStopIndex[fParameters.GetNstationsActive() - 1] = 0;
         unsigned int ip = 0;  //index of curent portion

         for (int istal = fParameters.GetNstationsActive() - 2; istal >= fFirstCAstation; istal--)  //start downstream chambers
         {
         int nHits = HitsUnusedStopIndex[istal] - HitsUnusedStartIndex[istal];

         int NHits_P = nHits/L1Constants::size::kSingletPortionSize;

         for( int ipp = 0; ipp < NHits_P; ipp++ )
         {
         n_g1[ip] = L1Constants::size::kSingletPortionSize;
         ip++;
         } // start_lh - portion of left hits

         n_g1[ip] = nHits - NHits_P * L1Constants::size::kSingletPortionSize;

         ip++;
         fDoubletPortionStopIndex[istal] = ip;
         }// lstations
         //       nPortions = ip;
         } */

    ///   stage for triplets creation

#ifdef XXX
    TStopwatch c_timer;
    c_timer.Start();
#endif


    L1TrackPar T_1[L1Constants::size::kSingletPortionSizeVec];
    L1FieldRegion fld_1[L1Constants::size::kSingletPortionSizeVec];
    L1HitIndex_t hitsl_1[L1Constants::size::kSingletPortionSize];
    L1TrackPar TG_1[L1Constants::size::kSingletPortionSizeVec];
    L1FieldRegion fldG_1[L1Constants::size::kSingletPortionSizeVec];
    L1HitIndex_t hitslG_1[L1Constants::size::kSingletPortionSize];

    /// middle hits indexed by number of doublets in portion(i2)
    L1Vector<L1HitIndex_t> hitsm_2("L1CATrackFinder::hitsm_2");

    /// index in portion of singlets(i1) indexed by index in portion of doublets(i2)
    L1Vector<L1HitIndex_t> i1_2("L1CATrackFinder::i1_2");

    /// middle hits indexed by number of doublets in portion(i2)
    L1Vector<L1HitIndex_t> hitsmG_2("L1CATrackFinder::hitsmG_2");

    /// index in portion of singlets(i1) indexed by index in portion of doublets(i2)
    L1Vector<L1HitIndex_t> i1G_2("L1CATrackFinder::i1G_2");

    hitsm_2.reserve(9000);  // TODO: make reasonable cuts on n combinations, put them to the header
    i1_2.reserve(9000);  // TODO: why that large numbers are needed even for mbias??? something goes wrong sometimes..
    hitsmG_2.reserve(9000);
    i1G_2.reserve(9000);

    for (int istal = fParameters.GetNstationsActive() - 2; istal >= fFirstCAstation;
         istal--)  // start downstream chambers
    {

#ifdef _OPENMP
#pragma omp parallel for firstprivate(T_1, fld_1, hitsl_1, hitsm_2, i1_2, TG_1, fldG_1, hitslG_1, hitsmG_2,            \
                                      i1G_2)  //schedule(dynamic, 2)
#endif
      for (Tindex ip = 0; ip < (Tindex) fSingletPortionSize[istal].size(); ++ip) {
        Tindex n_2 = 0;  /// number of doublets in portion

        hitsm_2.clear();
        i1_2.clear();

        CreatePortionOfDoublets(istal, istal + 1, ip, fSingletPortionSize[istal][ip],
                                // output
                                T_1, fld_1, hitsl_1, n_2, i1_2, hitsm_2);


        CreatePortionOfTriplets(  // input
          istal, istal + 1, istal + 2, T_1, fld_1, hitsl_1, n_2, i1_2, hitsm_2);

        if (fpCurrentIteration->GetJumpedFlag() || (fMissingHits)) {
          // All iterations are "jump"!
          Tindex nG_2;
          hitsmG_2.clear();
          i1G_2.clear();
          if ((fMissingHits && ((istal == 0) || (istal == 1))) || !fMissingHits) {
            CreatePortionOfDoublets(  // input
              istal, istal + 2, ip, fSingletPortionSize[istal][ip],
              // output
              TG_1, fldG_1, hitslG_1, nG_2, i1G_2, hitsmG_2);
          }

          if ((fMissingHits && (istal == 0)) || !fMissingHits) {
            CreatePortionOfTriplets(  // input
              istal, istal + 1, istal + 3, T_1, fld_1, hitsl_1,

              n_2, i1_2, hitsm_2);
          }

          if ((fMissingHits && (istal == 1)) || !fMissingHits) {
            CreatePortionOfTriplets(  // input
              istal, istal + 2, istal + 3, TG_1, fldG_1, hitslG_1, nG_2, i1G_2, hitsmG_2);
          }
        }
      }  //
    }

    //     int nlevels[L1Constants::size::kMaxNstations];  // number of triplets with some number of neighbours.
    //     for (int il = 0; il < fParameters.GetNstationsActive(); ++il) nlevels[il] = 0;
    //
    //      f5(   // input
    //           // output
    //         0,
    //         nlevels
    //       );

#ifdef XXX
    c_timer.Stop();
    ti[isec]["tripl1"] = c_timer;
    c_timer.Start();
#endif

    ///====================================================================
    ///=                                                                  =
    ///=        Collect track candidates. CREATE TRACKS                   =
    ///=                                                                  =
    ///====================================================================

    // min level to start triplet. So min track length = min_level+3.
    int min_level = std::min(caIteration.GetMinNhits(), caIteration.GetMinNhitsStation0()) - 3;

    // TODO: SZh 04.10.2022: Why this fatal error is called?
    // NOTE: This line was wrapped into TRACKS_FROM_TRIPLETS ifdef
    //LOG(FATAL) << "L1CATrackFinder: min_level undefined in " << __FILE__ << " : " << __LINE__;
    if (fpCurrentIteration->GetTrackFromTripletsFlag()) { min_level = 0; }

    L1Branch curr_tr;
    L1Branch new_tr[L1Constants::size::kMaxNstations];
    L1Branch best_tr;
    fscal curr_chi2 = 0;

    fscal best_chi2      = 0;
    unsigned char best_L = 0;

    unsigned char curr_L = 1;
    int ndf              = 1;


    // collect consequtive: the longest tracks, shorter, more shorter and so on
    for (int firstTripletLevel = fParameters.GetNstationsActive() - 3; firstTripletLevel >= min_level;
         firstTripletLevel--) {
      // choose length in triplets number - firstTripletLevel - the maximum possible triplet level among all triplets in the searched candidate
      TStopwatch Time;

      //  how many levels to check
      int nlevel = (fParameters.GetNstationsActive() - 2) - firstTripletLevel + 1;

      const unsigned char min_best_l =
        (firstTripletLevel > min_level) ? firstTripletLevel + 2 : min_level + 3;  // loose maximum


      for (int i = 0; i < fNThreads; ++i) {
        fTrackCandidates[i].clear();
      }

      fStripToTrack.reset(fInputData.GetNhitKeys(), -1);

      //== Loop over triplets with the required level, find and store track candidates

      for (int istaF = fFirstCAstation; istaF <= fParameters.GetNstationsActive() - 3 - firstTripletLevel; ++istaF) {

        if (--nlevel == 0) break;  //TODO: SG: this is not needed

#ifdef _OPENMP
#pragma omp parallel for firstprivate(curr_tr, new_tr, best_tr, curr_chi2, best_chi2, best_L, curr_L,                  \
                                      ndf)  // schedule(dynamic, 10)
#endif
        for (Tindex iThread = 0; iThread < fNThreads; ++iThread) {
          for (Tindex itrip = 0; itrip < (Tindex) fTriplets[istaF][iThread].size(); ++itrip) {

#ifdef _OPENMP
            int thread_num = omp_get_thread_num();
#else
            int thread_num = 0;
#endif
            L1Triplet& first_trip = (fTriplets[istaF][iThread][itrip]);
            if (fvHitKeyFlags[(*vHitsUnused)[first_trip.GetLHit()].f]
                || fvHitKeyFlags[(*vHitsUnused)[first_trip.GetLHit()].b]) {
              continue;
            }
            //               ghost suppression !!!
            //

            if (!fpCurrentIteration->GetTrackFromTripletsFlag()) {  // ghost suppression !!!
              int nHits = 3 + first_trip.GetLevel();
              if ((*vHitsUnused)[first_trip.GetLHit()].iSt == 0) {
                if (nHits < fpCurrentIteration->GetMinNhitsStation0()) { continue; }
              }
              else {
                if (nHits < fpCurrentIteration->GetMinNhits()) { continue; }
              }
            }

            // Collect triplets, which can start a track with length equal to firstTipletLevel + 3. This cut suppresses
            // ghost tracks, but does not affect the efficiency
            if (first_trip.GetLevel() < firstTripletLevel) { continue; }

            //  curr_tr.Momentum = 0.f;
            curr_tr.chi2 = 0.f;
            //   curr_tr.Lengtha  = 0;
            curr_tr.ista = 0;
            curr_tr.fHits.clear();
            curr_tr.fHits.push_back((*RealIHitP)[first_trip.GetLHit()]);
            curr_tr.NHits = 1;

            curr_L    = 1;
            curr_chi2 = first_trip.GetChi2();

            best_tr   = (curr_tr);
            best_chi2 = curr_chi2;
            best_L    = curr_L;

            CAFindTrack(istaF, best_tr, best_L, best_chi2, &first_trip, (curr_tr), curr_L, curr_chi2, min_best_l,
                        new_tr);  /// reqursive func to build a tree of possible track-candidates and choose the best
            //              if ( best_L < min_best_l ) continue;
            if (best_L < firstTripletLevel + 2) continue;  // lose maximum one hit

            if (best_L < min_level + 3) continue;  // should find all hits for min_level

            ndf = best_L * 2 - 5;

            if (kGlobal == fTrackingMode || kMcbm == fTrackingMode) { ndf = best_L * 2 - 4; }

            best_chi2 = best_chi2 / ndf;  //normalize

            if (fGhostSuppression) {
              if (3 == best_L) {
                if (!fpCurrentIteration->GetPrimaryFlag() && (istaF != 0)) continue;  // too /*short*/ non-MAPS track
                if (fpCurrentIteration->GetPrimaryFlag() && (best_chi2 > 5.0)) continue;
              }
            }
            fTrackCandidates[thread_num].push_back(best_tr);
            L1Branch& tr = fTrackCandidates[thread_num].back();
            tr.Set(istaF, best_L, best_chi2, first_trip.GetQp(),
                   PackTrackId(thread_num, fTrackCandidates[thread_num].size() - 1));
            if (0) {  // debug
              cout << "track " << fTrackCandidates[thread_num].size() - 1 << " found, L = " << (int) best_L
                   << " chi2= " << best_chi2 << endl;
              cout << " hits: ";
              for (unsigned int i = 0; i < tr.fHits.size(); i++) {
                cout << GetMcTrackIdForHit(tr.fHits[i]) << " ";
              }
              cout << endl;
            }
          }  // itrip
        }    // iThread
      }      // istaF

      for (Tindex iThread = 0; iThread < fNThreads; ++iThread) {
        for (Tindex iTrack = 0; iTrack < (Tindex) fTrackCandidates[iThread].size(); ++iTrack) {
          L1Branch& tr = fTrackCandidates[iThread][iTrack];
          tr.fIsAlive  = false;
        }
      }

      bool repeatCompetition = true;
      int iComp              = 0;
      for (iComp = 0; (iComp < 100) && repeatCompetition; ++iComp) {

        // == Loop over track candidates and mark their strips
        // TODO: OpenMP

        repeatCompetition = false;

        for (Tindex iThread = 0; iThread < fNThreads; ++iThread) {
          for (Tindex iTrack = 0; iTrack < (Tindex) fTrackCandidates[iThread].size(); ++iTrack) {
            L1Branch& tr = fTrackCandidates[iThread][iTrack];
            if (tr.fIsAlive) continue;

            for (int iHit = 0; iHit < (int) tr.fHits.size(); ++iHit) {
              const L1Hit& h = fInputData.GetHit(tr.fHits[iHit]);
              bool isAlive   = true;
              {  // front  strip
#ifdef _OPENMP
                omp_set_lock(&fStripToTrackLock[h.f]);
#endif
                auto& stripF = (fStripToTrack)[h.f];
                if ((stripF >= 0) && (stripF != tr.fID)) {  // strip is used by other candidate
                  const auto& other = fTrackCandidates[TrackId2Thread(stripF)][TrackId2Track(stripF)];
                  if (!other.fIsAlive && L1Branch::compareCand(tr, other)) { stripF = tr.fID; }
                  else {
                    isAlive = false;
                  }
                }
                else {
                  stripF = tr.fID;
                }
#ifdef _OPENMP
                omp_unset_lock(&fStripToTrackLock[h.f]);
#endif
                if (!isAlive) { break; }
              }

              {  // back strip
#ifdef _OPENMP
                omp_set_lock(&fStripToTrackLock[h.b]);
#endif
                auto& stripB = (fStripToTrack)[h.b];
                if ((stripB >= 0) && (stripB != tr.fID)) {  // strip is used by other candidate
                  const auto& other = fTrackCandidates[TrackId2Thread(stripB)][TrackId2Track(stripB)];
                  if (!other.fIsAlive && L1Branch::compareCand(tr, other)) { stripB = tr.fID; }
                  else {
                    isAlive = false;
                  }
                }
                else {
                  stripB = tr.fID;
                }
#ifdef _OPENMP
                omp_unset_lock(&fStripToTrackLock[h.b]);
#endif
                if (!isAlive) { break; }
              }
            }  // loop over hits
          }    // itrack
        }      // iThread

        // == Loop over track candidates and select those that are alive
        // TODO: OpenMP
        for (Tindex iThread = 0; iThread < fNThreads; ++iThread) {
          for (Tindex iTrack = 0; iTrack < (Tindex) fTrackCandidates[iThread].size(); ++iTrack) {
            L1Branch& tr = fTrackCandidates[iThread][iTrack];
            if (tr.fIsAlive) { continue; }

            tr.fIsAlive = true;
            for (int iHit = 0; tr.fIsAlive && (iHit < (int) tr.fHits.size()); ++iHit) {
              const L1Hit& h = fInputData.GetHit(tr.fHits[iHit]);
              tr.fIsAlive    = tr.fIsAlive && ((fStripToTrack)[h.f] == tr.fID) && ((fStripToTrack)[h.b] == tr.fID);
            }

            if (!tr.fIsAlive) {  // release strips
              for (int iHit = 0; (iHit < (int) tr.fHits.size()); ++iHit) {
                const L1Hit& h = fInputData.GetHit(tr.fHits[iHit]);
                if (fStripToTrack[h.f] == tr.fID) { fStripToTrack[h.f] = -1; }
                if (fStripToTrack[h.b] == tr.fID) { fStripToTrack[h.b] = -1; }
              }
            }
            else {
              repeatCompetition = true;
            }
          }  // itrack
        }    // iThread
      }      // competitions

      // cout << " N Competitions: " << iComp << endl;

      // ==

      for (int i = 0; i < fNThreads; ++i) {
        fTracks_local[i].clear();
        fRecoHits_local[i].clear();
      }

      for (int i = 0; i < fNThreads; ++i) {
        L1Track t;

#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic, 10) firstprivate(t)
#endif
        for (Tindex iCandidate = 0; iCandidate < (Tindex) fTrackCandidates[i].size(); ++iCandidate) {
          L1Branch& tr = fTrackCandidates[i][iCandidate];
          if (!tr.fIsAlive) continue;
          if (kMcbm == fTrackingMode) {
            if (tr.NHits <= 3) { continue; }
          }
          else if (kGlobal == fTrackingMode) {
            if (tr.NHits < 3) { continue; }
          }
          else {
            if (tr.NHits < 3) { continue; }
          }
          if (fpCurrentIteration->GetExtendTracksFlag()) {
            if (tr.NHits != fParameters.GetNstationsActive()) BranchExtender(tr);
          }
          fscal sumTime = 0;

#ifdef _OPENMP
          int num_thread = omp_get_thread_num();
#else
          int num_thread = 0;
#endif
          for (L1Vector<L1HitIndex_t>::iterator phIt = tr.fHits.begin();  /// used strips are marked
               phIt != tr.fHits.end(); ++phIt) {
            const L1Hit& hit     = fInputData.GetHit(*phIt);
            fvHitKeyFlags[hit.f] = 1;
            fvHitKeyFlags[hit.b] = 1;
            fRecoHits_local[num_thread].push_back(*phIt);
            L1HitPoint tempPoint = CreateHitPoint(hit);  //TODO take number of station from hit

            const L1Station& stah = fParameters.GetStation(hit.iSt);
            auto [xcoor, ycoor]   = stah.ConvUVtoXY<fscal>(tempPoint.U(), tempPoint.V());

            fscal zcoor = tempPoint.Z() - fParameters.GetTargetPositionZ()[0];

            fscal timeFlight = sqrt(xcoor * xcoor + ycoor * ycoor + zcoor * zcoor) / 30.f;  // c = 30[cm/ns]
            sumTime += (hit.t - timeFlight);
          }

          t.NHits = tr.NHits;
          // t.Momentum   = tr.Momentum;
          t.fTrackTime = sumTime / t.NHits;
          fTracks_local[num_thread].push_back(t);
          if (0) {  // SG debug
            cout << "store track " << iCandidate << " chi2= " << tr.chi2 << endl;
            cout << " hits: ";
            for (unsigned int ih = 0; ih < tr.fHits.size(); ih++) {
              cout << GetMcTrackIdForHit(tr.fHits[ih]) << " ";
            }
            cout << '\n';
          }

        }  // tracks
      }    // threads

#ifdef XXX
      Time.Stop();
      ti[isec]["table"] += Time;

      Time.Start();

#endif
      int nTracks = fTracks.size();
      L1Vector<int> offset_tracks("L1CATrackFinder::offset_tracks", fNThreads, nTracks);
      L1Vector<int> offset_hits("L1CATrackFinder::offset_hits", fNThreads, NHitsIsecAll);

      assert(NHitsIsecAll == fRecoHits.size());  //SG!!

      nTracks += fTracks_local[0].size();
      NHitsIsecAll += fRecoHits_local[0].size();

      for (int i = 1; i < fNThreads; ++i) {
        offset_tracks[i] = offset_tracks[i - 1] + fTracks_local[i - 1].size();
        offset_hits[i]   = offset_hits[i - 1] + fRecoHits_local[i - 1].size();
        nTracks += fTracks_local[i].size();
        NHitsIsecAll += fRecoHits_local[i].size();
      }
      fTracks.enlarge(nTracks);
      fRecoHits.enlarge(NHitsIsecAll);
#ifdef _OPENMP
#pragma omp parallel for
#endif
      for (int i = 0; i < fNThreads; ++i) {
        for (Tindex iC = 0; iC < (Tindex) fTracks_local[i].size(); ++iC) {
          fTracks[offset_tracks[i] + iC] = fTracks_local[i][iC];
        }
        for (Tindex iH = 0; iH < (Tindex) fRecoHits_local[i].size(); ++iH) {
          fRecoHits[offset_hits[i] + iH] = fRecoHits_local[i][iH];
        }
      }
    }  // firstTripletLevel

    // suppress strips of suppressed hits
    for (int ista = 0; ista < fParameters.GetNstationsActive(); ++ista) {
      for (L1HitIndex_t ih = HitsUnusedStartIndex[ista]; ih < HitsUnusedStopIndex[ista]; ih++) {
        const L1HitPoint& hp = (*vHitPointsUnused)[ih];
        if (hp.IsSuppressed()) {
          int hitId            = (*RealIHitP)[ih];
          const L1Hit& hit     = fInputData.GetHit(hitId);
          fvHitKeyFlags[hit.f] = 1;
          fvHitKeyFlags[hit.b] = 1;
        }
      }
    }

#ifdef XXX
    c_timer.Stop();
    ti[isec]["tracks"] = c_timer;
    c_timer.Start();
#endif


    if (isec < (fNFindIterations - 1)) {
      int NHitsOnStation = 0;

      for (int ista = 0; ista < fParameters.GetNstationsActive(); ++ista) {
        int start                   = HitsUnusedStartIndex[ista];
        int Nelements               = HitsUnusedStopIndex[ista] - start;
        L1Hit* staHits              = nullptr;  // to avoid out-of-range error in ..[start]
        L1HitIndex_t* staHitIndices = nullptr;
        L1HitPoint* staHitPoints    = nullptr;
        if (Nelements > 0) {
          staHits       = &((*vHitsUnused)[start]);
          staHitIndices = &((*RealIHitP)[start]);
          staHitPoints  = &((*vHitPointsUnused)[start]);
        }

        int NHitsOnStationTmp = NHitsOnStation;

        vGridTime[ista].UpdateIterGrid(Nelements, staHits, RealIHitPBuf, staHitIndices, vHitsUnused_buf,
                                       vHitPointsUnused_buf, staHitPoints, NHitsOnStation, ista, *this, fvHitKeyFlags);

        HitsUnusedStartIndex[ista] = NHitsOnStationTmp;
        HitsUnusedStopIndex[ista]  = NHitsOnStation;
      }

#ifdef XXX
      c_timer.Stop();
      ti[isec]["finish"] = c_timer;
#endif

#ifdef XXX
      //     if( stat_max_n_trip<stat_n_trip ) stat_max_n_trip = vTriplets.size();
      //     Tindex tsize = vTripletsP.size()*sizeof(L1Triplet);
      //     if( stat_max_trip_size < tsize ) stat_max_trip_size = tsize;
#endif
      // #ifdef DRAW
      //     draw->ClearVeiw();
      // //   draw->DrawInfo();
      //     draw->DrawRestHits(HitsUnusedStartIndex, HitsUnusedStopIndex, RealIHit);
      //     draw->DrawRecoTracks();
      //     draw->SaveCanvas("Reco_"+isec+"_");
      //     draw->DrawAsk();
      // #endif

      // #ifdef PULLS
      //       fL1Pulls->Build(1);
      // #endif
#ifdef COUNTERS
      //  stat_nHits[isec] += (vHitsUnused*)->Size();

      cout << "iter = " << isec << endl;
      cout << " NSinglets = " << stat_nSinglets[isec] / stat_N << endl;
      //    cout << " NDoublets = " << stat_nDoublets[isec]/stat_N << endl;
      cout << " NTriplets = " << stat_nTriplets[isec] / stat_N << endl;
      cout << " NHitsUnused = " << stat_nHits[isec] / stat_N << endl;

#endif  // COUNTERS
    }
    ++isec;  // TODO: temporary, to be removed! ()
  }          // for (int isec
  // ---- Loop over Track Finder iterations: END -----------------------------------------------------------//

#ifdef XXX
  c_timerG.Stop();
  gti["iterts"] = c_timerG;
  c_timerG.Start();
#endif

  if constexpr (L1Constants::control::kIfMergeClones) {
    //CAMergeClones();
    // Fit tracks
    this->L1KFTrackFitter();

    // Merge clones
    fCloneMerger.Exec(fTracks, fRecoHits);
  }

#ifdef XXX
  c_timerG.Stop();
  gti["merge "] = c_timerG;
#endif

  //==================================

  c_time.Stop();

  //   cout << "End TrackFinder" << endl;
  //  CATime = (double(c_time.CpuTime()));
  fCATime = (double(c_time.RealTime()));

#ifdef XXX


  cout << endl << " --- Timers, ms --- " << endl;
  ti.Calc();
  stat_ti += ti;
  L1CATFTimerInfo tmp_ti = stat_ti / 0.001 / stat_N;  // ms

  tmp_ti.PrintReal();
  stat_gti += gti;
  L1CATFIterTimerInfo tmp_gti = stat_gti / 0.001 / stat_N;  // ms
  tmp_gti.PrintReal(1);
  fstream filestr;
  filestr.open("speedUp.log", fstream::out | fstream::app);
  fscal tripl_speed = 1000. / (tmp_ti.GetTimerAll()["tripl1"].Real());
  filestr << tripl_speed << " ";
  filestr.close();


#if 0
        static long int NTimes =0, NHits=0, HitSize =0, NStrips=0, StripSize =0, NStripsB=0, StripSizeB =0,
        NDup=0, DupSize=0, NTrip=0, TripSize=0, NBranches=0, BranchSize=0, NTracks=0, TrackSize=0 ;

        NTimes++;
        NHits += vHitsUnused->size();
        HitSize += vHitsUnused->size()*sizeof(L1Hit);
        NStrips+= vStrips.size();
        StripSize += vStrips.size()*sizeof(fscal) + (*fStripFlag).size()*sizeof(unsigned char);
        NStripsB+= (*fStripFlagB).size();
        StripSizeB += vStripsB.size()*sizeof(fscal) + (*fStripFlagB).size()*sizeof(unsigned char);
        NDup += stat_max_n_dup;
        DupSize += stat_max_n_dup*sizeof(/*short*/ int);
        NTrip += stat_max_n_trip;
        TripSize += stat_max_trip_size;

        NBranches += stat_max_n_branches;
        BranchSize += stat_max_BranchSize;
        NTracks += fTracks.size();
        TrackSize += sizeof(L1Track)*fTracks.size() + sizeof(L1HitIndex_t)*fRecoHits.size();
        int k = 1024*NTimes;

        cout<<"L1 Event size: \n"
        <<HitSize/k<<"kB for "<<NHits/NTimes<<" hits, \n"
        <<StripSize/k<<"kB for "<<NStrips/NTimes<<" strips, \n"
        <<StripSizeB/k<<"kB for "<<NStripsB/NTimes<<" stripsB, \n"
        <<DupSize/k<<"kB for "<<NDup/NTimes<<" doublets, \n"
        <<TripSize/k<<"kB for "<<NTrip/NTimes<<" triplets\n"
        <<BranchSize/k<<"kB for "<<NBranches/NTimes<<" branches, \n"
        <<TrackSize/k<<"kB for "<<NTracks/NTimes<<" tracks. "<<endl;
        cout<<" L1 total event size = "
        <<( HitSize + StripSize +  StripSizeB + DupSize + TripSize + BranchSize + TrackSize )/k
        <<" Kb"<<endl;
#endif  // 0
#endif

#ifdef DRAW
  draw->ClearVeiw();
  //   draw->DrawInputHits();
  //   draw->DrawInfo();
  draw->DrawRecoTracks();

  draw->SaveCanvas("Reco_");
  draw->DrawAsk();
#endif
#ifdef PULLS
  static int iEvee = 0;
  iEvee++;
  if (iEvee % 1 == 0) fL1Pulls->Build(1);
#endif
#ifdef DOUB_PERFORMANCE
  fL1Eff_doublets->CalculateEff();
  fL1Eff_doublets->Print("Doublets performance.", 1);
#endif
#ifdef TRIP_PERFORMANCE
  fL1Eff_triplets->CalculateEff();
  fL1Eff_triplets->Print("Triplet performance", 1);
  //   fL1Eff_triplets2->CalculateEff();
  //   fL1Eff_triplets2->Print("Triplet performance. After chi2 cut");
#endif
}


/** *************************************************************
     *                                                              *
     *     The routine performs recursive search for tracks         *
     *                                                              *
     *     I. Kisel                                    06.03.05     *
     *     I.Kulakov                                    2012        *
     *                                                              *
     ****************************************************************/

inline void L1Algo::CAFindTrack(int ista, L1Branch& best_tr, unsigned char& best_L, fscal& best_chi2,
                                const L1Triplet* curr_trip, L1Branch& curr_tr, unsigned char& curr_L, fscal& curr_chi2,
                                unsigned char min_best_l, L1Branch* new_tr)
/// recursive search for tracks
/// input: @ista - station index, @&best_tr - best track for the privious call, @&best_L -
/// output: @&NCalls - number of function calls
{
  if (curr_trip->GetLevel() == 0)  // the end of the track -> check and store
  {
    // -- finish with current track
    // add rest of hits
    const L1HitIndex_t& ihitm = curr_trip->GetMHit();
    const L1HitIndex_t& ihitr = curr_trip->GetRHit();


    //if (!GetFUsed((*fStripFlag)[(*vHitsUnused)[ihitm].f] | (*fStripFlag)[(*vHitsUnused)[ihitm].b])) {
    //L1_SHOW(fInputData.GetNhitKeys());
    //L1_SHOW(fvHitKeyFlags.size());
    //L1_SHOW((*vHitsUnused)[ihitm].f);
    //L1_SHOW((*vHitsUnused)[ihitm].b);
    if (!(fvHitKeyFlags[(*vHitsUnused)[ihitm].f] || fvHitKeyFlags[(*vHitsUnused)[ihitm].b])) {

      //        curr_tr.Hits.push_back((*RealIHitP)[ihitm]);

      curr_tr.fHits.push_back((*RealIHitP)[ihitm]);

      curr_tr.NHits++;

      curr_L++;
    }

    //if (!GetFUsed((*fStripFlag)[(*vHitsUnused)[ihitr].f] | (*fStripFlag)[(*vHitsUnused)[ihitr].b])) {
    //L1_SHOW(fInputData.GetNhitKeys());
    //L1_SHOW(fvHitKeyFlags.size());
    //L1_SHOW((*vHitsUnused)[ihitr].f);
    //L1_SHOW((*vHitsUnused)[ihitr].b);
    if (!(fvHitKeyFlags[(*vHitsUnused)[ihitr].f] || fvHitKeyFlags[(*vHitsUnused)[ihitr].b])) {

      //curr_tr.Hits.push_back((*RealIHitP)[ihitr]);
      curr_tr.fHits.push_back((*RealIHitP)[ihitr]);

      curr_tr.NHits++;

      curr_L++;
    }

    //if( curr_L < min_best_l - 1 ) return; // suppouse that only one hit can be added by extender
    int ndf = curr_L * 2 - 5;

    if (kGlobal == fTrackingMode || kMcbm == fTrackingMode) { ndf = curr_L * 2 - 4; }

    if (curr_chi2 > fTrackChi2Cut * ndf) return;

    // TODO: SZh 04.10.2022: Does one should delete this code?
    //       // try to find more hits
    // if (fpCurrentIteration->GetExtendTracksFlag()) {
    //     // if( curr_L < min_best_l )
    //     if (!fpCurrentIteration->GetJumpedFlag() && curr_L >= 3){
    //       //curr_chi2 = BranchExtender(curr_tr);
    //       BranchExtender(curr_tr);
    //       curr_L = curr_tr.Hits.size();
    //         //      if( 2*curr_chi2 > (2*(curr_L*2-5) + 1) * 4*4 ) return;
    //     }
    // }

    // -- select the best
    if ((curr_L > best_L) || ((curr_L == best_L) && (curr_chi2 < best_chi2))) {

      best_tr   = curr_tr;
      best_chi2 = curr_chi2;
      best_L    = curr_L;
    }

    return;
  }
  else  //MEANS level ! = 0
  {
    int N_neighbour = (curr_trip->GetNNeighbours());

    for (Tindex in = 0; in < N_neighbour; in++) {

      unsigned int ID = curr_trip->GetFNeighbour() + in;

      unsigned int Station = TripletId2Station(ID);
      unsigned int Thread  = TripletId2Thread(ID);
      unsigned int Triplet = TripletId2Triplet(ID);

      const L1Triplet& new_trip = fTriplets[Station][Thread][Triplet];

      fscal dchi2 = 0.;
      if (!checkTripletMatch(*curr_trip, new_trip, dchi2)) continue;

      if (fvHitKeyFlags[(*vHitsUnused)[new_trip.GetLHit()].f]
          || fvHitKeyFlags[(*vHitsUnused)[new_trip.GetLHit()].b]) {  //hits are used
        //  no used hits allowed -> compare and store track
        if ((curr_L > best_L) || ((curr_L == best_L) && (curr_chi2 < best_chi2))) {
          best_tr = curr_tr;

          best_chi2 = curr_chi2;
          best_L    = curr_L;
        }
      }
      else {  // hit is not used: add the left hit from the new triplet to the current track

        unsigned char new_L = curr_L + 1;
        fscal new_chi2      = curr_chi2 + dchi2;


        if (0) {  //SGtrd2d debug!!
          int mc01 = GetMcTrackIdForUnusedHit(curr_trip->GetLHit());
          int mc02 = GetMcTrackIdForUnusedHit(curr_trip->GetMHit());
          int mc03 = GetMcTrackIdForUnusedHit(curr_trip->GetRHit());
          int mc11 = GetMcTrackIdForUnusedHit(new_trip.GetLHit());
          int mc12 = GetMcTrackIdForUnusedHit(new_trip.GetMHit());
          int mc13 = GetMcTrackIdForUnusedHit(new_trip.GetRHit());
          if ((mc01 == mc02) && (mc02 == mc03)) {
            cout << " sta " << ista << " mc0 " << mc01 << " " << mc02 << " " << mc03 << " mc1 " << mc11 << " " << mc12
                 << " " << mc13 << " chi2 " << curr_chi2 / (2 * (curr_L + 2) - 4) << " new "
                 << new_chi2 / (2 * (new_L + 2) - 4) << endl;
            cout << "   hits " << curr_trip->GetLHit() << " " << curr_trip->GetMHit() << " " << curr_trip->GetRHit()
                 << " " << new_trip.GetLHit() << endl;
          }
        }

        if (kGlobal == fTrackingMode) {  //SGtrd2d!!!
          // nHits in chi2 calculation == new_L+2, ndf == 4
          if (new_chi2 > fTrackChi2Cut * (2 * (new_L + 2) - 4)) continue;
        }
        else {
          if (new_chi2 > fTrackChi2Cut * new_L) continue;  // TODO: SG: it must be  ( 2 * new_L )
        }

        // add new hit
        new_tr[ista] = curr_tr;
        new_tr[ista].fHits.push_back((*RealIHitP)[new_trip.GetLHit()]);
        new_tr[ista].NHits++;

        const int new_ista = ista + new_trip.GetMSta() - new_trip.GetLSta();

        CAFindTrack(new_ista, best_tr, best_L, best_chi2, &new_trip, new_tr[ista], new_L, new_chi2, min_best_l, new_tr);
      }  // add triplet to track
    }    // for neighbours
  }      // level = 0
}

#ifdef DRAW
void L1Algo::DrawRecoTracksTime(const L1Vector<CbmL1Track>& tracks)
{
  // TODO: find where the DrawRecoTracksTime is. It is missing in the git repository.
  //draw->DrawRecoTracksTime(tracks);
  draw->SaveCanvas(" ");
}
#endif
