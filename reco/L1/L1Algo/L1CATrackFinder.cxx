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


#include "L1Algo.h"
#include "L1Assert.h"
#include "L1Extrapolation.h"
#include "L1Filtration.h"
#include "L1Fit.h"
#include "L1HitPoint.h"
#include "L1Track.h"
#include "L1TrackPar.h"
//#include "TDHelper.h"
#include "L1Branch.h"
#include "L1Grid.h"
#include "L1HitArea.h"
#include "L1Portion.h"

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

#ifdef TBB
#include "L1AlgoTBB.h"
#endif  // TBB

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>

#include <stdio.h>


// using namespace std;
using std::cout;
using std::endl;


inline void L1Algo::findSingletsStep0(  // input
  Tindex start_lh, Tindex n1_l, L1HitPoint* Hits_l,
  // output
  fvec* u_front_l, fvec* u_back_l, fvec* zPos_l, L1HitIndex_t* hitsl, fvec* HitTime_l, fvec* HitTimeEr,
  // comment unused parameters, FU, 18.01.21
  fvec* /*Event_l*/, fvec* d_u, fvec* d_v)
{

  /// Prepare the portion of data of left hits of a triplet:
  /// all hits except the last and the second last station will be procesesed in the algorithm,
  /// the data is orginesed in order to be used by SIMD

  const Tindex end_lh = start_lh + n1_l;
  const int lastV     = (n1_l - 1) / fvec::size();
  if (lastV >= 0) {
    // set some positive errors to unfilled part of vectors in order to avoid nans
    L1HitPoint& hitl = Hits_l[0];
    d_u[lastV]       = hitl.dU();
    d_v[lastV]       = hitl.dV();
    HitTimeEr[lastV] = hitl.timeEr;
    u_front_l[lastV] = hitl.U();
    u_back_l[lastV]  = hitl.V();
    HitTime_l[lastV] = hitl.time;
    zPos_l[lastV]    = hitl.Z();
  }

  for (Tindex ilh = start_lh, i1 = 0; ilh < end_lh; ++ilh, ++i1) {
    Tindex i1_V      = i1 / fvec::size();
    Tindex i1_4      = i1 % fvec::size();
    L1HitPoint& hitl = Hits_l[ilh];


#ifdef USE_EVENT_NUMBER
    Event_l[i1_V][i1_4] = hitl.track;
#endif  // USE_EVENT_NUMBER

    HitTime_l[i1_V][i1_4] = hitl.time;
    HitTimeEr[i1_V][i1_4] = hitl.timeEr;

    hitsl[i1]             = ilh;
    u_front_l[i1_V][i1_4] = hitl.U();
    u_back_l[i1_V][i1_4]  = hitl.V();

    if (fUseHitErrors) {
      d_u[i1_V][i1_4] = hitl.dU();
      d_v[i1_V][i1_4] = hitl.dV();
    }

    zPos_l[i1_V][i1_4] = hitl.Z();
  }
}


inline void L1Algo::findSingletsStep1(  /// input 1st stage of singlet search
  int istal,
  int istam,    /// indexes of left and middle stations of a triplet
  Tindex n1_V,  ///
  fvec* u_front_l, fvec* u_back_l, fvec* zPos_l, fvec* HitTime_l, fvec* HitTimeEr,
  // output
  //                 L1TrackPar *T_1,
  L1TrackPar* T_1, L1FieldRegion* fld_1, fvec* d_u, fvec* d_v)
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

  L1Fit fit;

  if (fpCurrentIteration->GetElectronFlag()) { fit.SetParticleMass(L1Constants::phys::kElectronMass); }
  else {
    fit.SetParticleMass(fDefaultMass);
  }

  for (int i1_V = 0; i1_V < n1_V; i1_V++) {
    L1TrackPar& T = T_1[i1_V];

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
    fvec& time      = HitTime_l[i1_V];
    fvec& timeEr    = HitTimeEr[i1_V];
    const fvec dzli = 1.f / (zl - fTargZ);

    const fvec tx = (xl - fTargX) * dzli;
    const fvec ty = (yl - fTargY) * dzli;

    L1FieldValue b00, b01, b10, b11, b12;
    fld0Sta0.fieldSlice.GetFieldValue(fTargX + tx * (fld0Sta0.z - fTargZ), fTargY + ty * (fld0Sta0.z - fTargZ), b00);
    fld0Sta1.fieldSlice.GetFieldValue(fTargX + tx * (fld0Sta1.z - fTargZ), fTargY + ty * (fld0Sta1.z - fTargZ), b01);
    fld1Sta0.fieldSlice.GetFieldValue(fTargX + tx * (fld1Sta0.z - fTargZ), fTargY + ty * (fld1Sta0.z - fTargZ), b10);
    fld1Sta1.fieldSlice.GetFieldValue(fTargX + tx * (fld1Sta1.z - fTargZ), fTargY + ty * (fld1Sta1.z - fTargZ), b11);
    fld1Sta2.fieldSlice.GetFieldValue(fTargX + tx * (fld1Sta2.z - fTargZ), fTargY + ty * (fld1Sta2.z - fTargZ), b12);

    fld0.Set(fTargB, fTargZ, b00, fld0Sta0.z, b01, fld0Sta1.z);
    fld1.Set(b10, fld1Sta0.z, b11, fld1Sta1.z, b12, fld1Sta2.z);

    T.chi2 = 0.;
    T.NDF  = 2.;  /// Iterations -> Number of parameters - number of measurements,
    if ((isec == kAllSecIter) || (isec == kAllSecEIter) || (isec == kAllSecJumpIter)) T.NDF = fvec(0.);
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
    T.C44 = fMaxInvMom / fvec(3.) * fMaxInvMom / fvec(3.);
    T.C55 = timeEr * timeEr;

    if (fParameters.DevIsFitSingletsFromTarget()) {  // TODO: doesn't work. Why?

      T.x = fTargX;
      T.y = fTargY;

      T.z   = fTargZ;
      T.C00 = TargetXYInfo.C00;
      T.C10 = TargetXYInfo.C10;
      T.C11 = TargetXYInfo.C11;
      // extrapolate to left hit

      if (istal <= fNfieldStations) { L1Extrapolate0(T, zl, fld0); }
      else {
        L1ExtrapolateLine(T, zl);
      }

      for (int ista = 0; ista <= istal - 1; ista++) {
        if constexpr (L1Constants::control::kIfUseRadLengthTable) {
          fit.L1AddMaterial(T, fParameters.GetMaterialThickness(ista, T.x, T.y), fMaxInvMom, fvec::One());
        }
        else {
          fit.L1AddMaterial(T, fParameters.GetStation(ista).materialInfo, fMaxInvMom, fvec::One());
        }
        if (ista == fNstationsBeforePipe - 1) fit.L1AddPipeMaterial(T, fMaxInvMom, fvec::One());
      }

      // add left hit
      L1UMeasurementInfo info = stal.frontInfo;

      if (fUseHitErrors) info.sigma2 = d_u[i1_V] * d_u[i1_V];

      if (istal < fNfieldStations) L1Filter(T, info, u);
      else {
        L1FilterNoField(T, info, u);
      }

      info = stal.backInfo;

      if (fUseHitErrors) { info.sigma2 = d_v[i1_V] * d_v[i1_V]; }

      if (istal < fNfieldStations) { L1Filter(T, info, v); }
      else {
        L1FilterNoField(T, info, v);
      }
    }
    else {  // not BEGIN_FROM_TARGET  -- the best for now
      T.x   = xl;
      T.y   = yl;
      T.z   = zl;
      T.C00 = stal.XYInfo.C00;
      T.C10 = stal.XYInfo.C10;
      T.C11 = stal.XYInfo.C11;

      if (fUseHitErrors) { std::tie(T.C00, T.C10, T.C11) = stal.FormXYCovarianceMatrix(d_u[i1_V], d_v[i1_V]); }

      //assert(T.IsConsistent(true, -1));

      //  add the target
      if (istal < fNfieldStations) {
        fvec eX, eY, J04, J14;
        fvec dz;
        dz = fTargZ - zl;
        L1ExtrapolateJXY0(T.tx, T.ty, dz, fld0, eX, eY, J04, J14);
        fvec J[6];
        J[0] = dz;
        J[1] = 0;
        J[2] = J04;
        J[3] = 0;
        J[4] = dz;
        J[5] = J14;
        L1FilterVtx(T, fTargX, fTargY, TargetXYInfo, eX, eY, J);
      }
      else {  //TODO: SG: take the field into account properly
        fvec eX, eY, J04, J14;
        fvec dz;
        dz  = fTargZ - zl;
        eX  = T.tx * dz;
        eY  = T.ty * dz;
        J04 = 0.f;
        J14 = 0.f;
        L1ExtrapolateJXY0(T.tx, T.ty, dz, fld0, eX, eY, J04, J14);
        fvec J[6];
        J[0] = dz;
        J[1] = 0;
        J[2] = J04;
        J[3] = 0;
        J[4] = dz;
        J[5] = J14;
        L1FilterVtx(T, fTargX, fTargY, TargetXYInfo, eX, eY, J);
        // old code
        //L1ExtrapolateLine(T, fTargZ);
        //assert(T.IsConsistent(true, -1));
        //L1FilterXY(T, fTargX, fTargY, TargetXYInfo);
        //assert(T.IsConsistent(true, -1));
      }
    }

    //assert(T.IsConsistent(true, -1));

    if constexpr (L1Constants::control::kIfUseRadLengthTable) {
      if (kMcbm == fTrackingMode) {
        fit.L1AddThickMaterial(T, fParameters.GetMaterialThickness(istal, T.x, T.y), fMaxInvMom, fvec::One(),
                               stal.materialInfo.thick, 1);
      }
      else if (kGlobal == fTrackingMode) {
        fit.L1AddMaterial(T, fParameters.GetMaterialThickness(istal, T.x, T.y), fMaxInvMom, fvec::One());
      }
      else {
        fit.L1AddMaterial(T, fParameters.GetMaterialThickness(istal, T.x, T.y), fMaxInvMom, fvec::One());
      }
    }
    else {
      fit.L1AddMaterial(T, stal.materialInfo, fMaxInvMom, fvec::One());
    }
    if ((istam >= fNstationsBeforePipe) && (istal <= fNstationsBeforePipe - 1)) {
      fit.L1AddPipeMaterial(T, fMaxInvMom, fvec::One());
    }

    //assert(T.IsConsistent(true, -1));

    fvec dz = stam.z - zl;
    L1ExtrapolateTime(T, dz, stam.timeInfo);

    // extrapolate to the middle hit
    if (istam < fNfieldStations) { L1Extrapolate0(T, stam.z, fld0); }
    else {
      L1ExtrapolateLine(T, stam.z);  // TODO: fld1 doesn't work!
    }

    // assert(T.IsConsistent(true, -1));

  }  // i1_V
}


inline void L1Algo::findDoubletsStep0(
  /// input
  Tindex n1, const L1Station& stal, const L1Station& stam, L1HitPoint* vHits_m, L1TrackPar* T_1, L1HitIndex_t* hitsl_1,
  /// output
  Tindex& n2, L1Vector<L1HitIndex_t>& i1_2,
#ifdef DOUB_PERFORMANCE
  L1Vector<L1HitIndex_t>& hitsl_2,
#endif  // DOUB_PERFORMANCE
  L1Vector<L1HitIndex_t>& hitsm_2, fvec* /*Event*/, L1Vector<char>& lmDuplets)
{
  /// Find the doublets. Reformat data in the portion of doublets.

  int iStaL = &stal - fParameters.GetStations().begin();
  int iStaM = &stam - fParameters.GetStations().begin();

  n2 = 0;                             // number of doublet
  for (Tindex i1 = 0; i1 < n1; ++i1)  // for each singlet
  {
    unsigned int Ndoublets = 0;
    const Tindex i1_V      = i1 / fvec::size();
    const Tindex i1_4      = i1 % fvec::size();
    L1TrackPar& T1         = T_1[i1_V];

    // assert(T1.IsEntryConsistent(true, i1_4));
    // if (!T1.IsEntryConsistent(false, i1_4)) continue;

    const int n2Saved = n2;

    const fvec Pick_m22 = (fDoubletChi2Cut - T1.chi2);
    // if make it bigger the found hits will be rejected later because of the chi2 cut.
    // Pick_m22 is not used, search for mean squared, 2nd version

    // -- collect possible doublets --
    const fscal iz        = 1.f / (T1.z[i1_4] - fParameters.GetTargetPositionZ()[0]);
    const fscal timeError = T1.C55[i1_4];
    const fscal time      = T1.t[i1_4];

    L1HitAreaTime areaTime(vGridTime[iStaM], T1.x[i1_4] * iz, T1.y[i1_4] * iz,
                           (sqrt(Pick_m22 * (T1.C00 + stam.XYInfo.C00)) + fMaxDZ * abs(T1.tx))[i1_4] * iz,
                           (sqrt(Pick_m22 * (T1.C11 + stam.XYInfo.C11)) + fMaxDZ * abs(T1.ty))[i1_4] * iz, time,
                           sqrt(timeError) * 5);


    L1HitIndex_t imh = 0;
    int irm1         = -1;
    while (true) {  // loop over the hits in the area
      if (fParameters.DevIsIgnoreHitSearchAreas()) {
        irm1++;
        if ((L1HitIndex_t) irm1 >= (HitsUnusedStopIndex[iStaM] - HitsUnusedStartIndex[iStaM])) break;
        imh = irm1;
      }
      else {
        if (!areaTime.GetNext(imh)) break;
      }

      const L1HitPoint& hitm = vHits_m[imh];

      if (fParameters.DevIsMatchDoubletsViaMc()) {  // trd2d
        int indL = HitsUnusedStartIndex[iStaL] + hitsl_1[i1];
        int indM = HitsUnusedStartIndex[iStaM] + imh;
        if (GetMcTrackIdForUnusedHit(indL) != GetMcTrackIdForUnusedHit(indM)) { continue; }
      }

      // check y-boundaries
      //TODO: move hardcoded cuts to parameters
      if ((stam.timeInfo) && (stal.timeInfo)) {
        if (fabs(time - hitm.time) > sqrt(timeError + hitm.timeEr * hitm.timeEr) * 5) continue;
        if (fabs(time - hitm.time) > 30) continue;
      }

#ifdef USE_EVENT_NUMBER
      if ((Event[i1_V][i1_4] != hitm.n)) continue;
#endif  // USE_EVENT_NUMBER
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

      fscal dy_est2 = Pick_m22[i1_4] * fabs(C11[i1_4] + stam.XYInfo.C11[i1_4]);

      /// Covariation matrix of the hit
      auto [dxxScalMhit, dxyScalMhit, dyyScalMhit] = stam.FormXYCovarianceMatrix(hitm.dU(), hitm.dV());

      if (fUseHitErrors) { dy_est2 = Pick_m22[i1_4] * fabs(C11[i1_4] + dyyScalMhit); }

      auto [xm, ym] = stam.ConvUVtoXY<fscal>(hitm.U(), hitm.V());

      const fscal dY = ym - y[i1_4];

      if (dY * dY > dy_est2) continue;

      // check x-boundaries
      fvec x, C00;
      L1ExtrapolateXC00Line(T1, zm, x, C00);

      fscal dx_est2 = Pick_m22[i1_4] * fabs(C00[i1_4] + stam.XYInfo.C00[i1_4]);

      if (fUseHitErrors) { dx_est2 = Pick_m22[i1_4] * fabs(C00[i1_4] + dxxScalMhit); }

      const fscal dX = xm - x[i1_4];

      if (dX * dX > dx_est2) continue;

      // check chi2
      fvec C10;
      L1ExtrapolateC10Line(T1, zm, C10);
      fvec chi2 = T1.chi2;

      L1UMeasurementInfo info = stam.frontInfo;

      if (fUseHitErrors) info.sigma2 = hitm.dU() * hitm.dU();

      L1FilterChi2XYC00C10C11(info, x, y, C00, C10, C11, chi2, hitm.U());
#ifdef DO_NOT_SELECT_TRIPLETS
      if (isec != TRACKS_FROM_TRIPLETS_ITERATION)
#endif  // DO_NOT_SELECT_TRIPLETS
        if (chi2[i1_4] > fDoubletChi2Cut) continue;
          //       T1.t[i1_4] = hitm.time;

#ifdef USE_EVENT_NUMBER
      T1.n[i1_4] = hitm.n;
#endif  // USE_EVENT_NUMBER

      info = stam.backInfo;

      if (fUseHitErrors) info.sigma2 = hitm.dV() * hitm.dV();

      L1FilterChi2(info, x, y, C00, C10, C11, chi2, hitm.V());

      // FilterTime(T1, hitm.time, hitm.timeEr);

#ifdef DO_NOT_SELECT_TRIPLETS
      if (isec != TRACKS_FROM_TRIPLETS_ITERATION)
#endif  // DO_NOT_SELECT_TRIPLETS
        if (chi2[i1_4] > fDoubletChi2Cut) continue;

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

    lmDuplets[hitsl_1[i1]] = (n2Saved < n2);

  }  // for i1
}


/// Add the middle hits to parameters estimation. Propagate to right station.
/// Find the triplets(right hit). Reformat data in the portion of triplets.
inline void L1Algo::findTripletsStep0(  // input
  L1HitPoint* vHits_r, const L1Station& stam, const L1Station& star, int istam, int istar, L1HitPoint* vHits_m,
  L1TrackPar* T_1, L1FieldRegion* fld_1, L1HitIndex_t* hitsl_1, Tindex n2, L1Vector<L1HitIndex_t>& hitsm_2,
  L1Vector<L1HitIndex_t>& i1_2, const L1Vector<char>& /*mrDuplets*/,
  // output
  Tindex& n3, L1Vector<L1TrackPar>& T_3, L1Vector<L1HitIndex_t>& hitsl_3, L1Vector<L1HitIndex_t>& hitsm_3,
  L1Vector<L1HitIndex_t>& hitsr_3, L1Vector<fvec>& u_front_3, L1Vector<fvec>& u_back_3, L1Vector<fvec>& z_Pos_3,
  L1Vector<fvec>& dv_, L1Vector<fvec>& du_, L1Vector<fvec>& timeR, L1Vector<fvec>& timeER)
{
  int iStaM = &stam - fParameters.GetStations().begin();
  int iStaR = &star - fParameters.GetStations().begin();

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

  L1Fit fit;
  fit.SetParticleMass(fDefaultMass);

  n3          = 0;
  Tindex n3_V = 0, n3_4 = 0;

  T_3.reset(1, L1TrackPar_0);
  u_front_3.reset(1, fvec::Zero());
  u_back_3.reset(1, fvec::Zero());
  z_Pos_3.reset(1, fvec::Zero());
  du_.reset(1, fvec::One());
  dv_.reset(1, fvec::One());
  timeR.reset(1, fvec::Zero());
  timeER.reset(1, fvec::One());

  assert(istar < fParameters.GetNstationsActive());  //TODO SG!!! check if it is needed

  if (istar >= fParameters.GetNstationsActive()) return;

  // ---- Add the middle hits to parameters estimation. Propagate to right station. ----

  for (Tindex i2 = 0; i2 < n2;) {
    L1TrackPar T2 = L1TrackPar_0;
    L1FieldRegion f2;
    // pack the data
    fvec u_front_2 = 0.f;
    fvec u_back_2  = 0.f;
    fvec du2       = 1.f;
    fvec dv2       = 1.f;
    fvec zPos_2    = 0.f;
    fvec timeM     = 0.f;
    fvec timeMEr   = 1.f;

    size_t n2_4 = 0;
    for (; n2_4 < fvec::size() && i2 < n2; i2++, n2_4++) {
      //         if (!mrDuplets[hitsm_2[i2]]) {
      //           n2_4--;
      //           continue;
      //         }
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
      timeM[n2_4]            = hitm.time;
      timeMEr[n2_4]          = hitm.timeEr;
      //  num[n2_4] = hitm.track;
      du2[n2_4] = hitm.dU();
      dv2[n2_4] = hitm.dV();

      hitsl_2[n2_4]     = hitsl_1[i1];
      hitsm_2_tmp[n2_4] = hitsm_2[i2];
    }  // n2_4

    fvec dz = zPos_2 - T2.z;

    L1ExtrapolateTime(T2, dz, stam.timeInfo);

    // assert(T2.IsConsistent(true, n2_4));

    // add middle hit
    L1ExtrapolateLine(T2, zPos_2);

    // assert(T2.IsConsistent(true, n2_4));

    // L1TrackPar tStore1 = T2;

    L1UMeasurementInfo info = stam.frontInfo;

    if (fUseHitErrors) info.sigma2 = du2 * du2;

    // TODO: SG: L1FilterNoField is wrong.
    // TODO: If the field was present before,
    // TODO: the momentum is correlated with the position and corresponding
    // TODO: matrix elements must be up[dated

    if (istam < fNfieldStations) { L1Filter(T2, info, u_front_2); }
    else {
      L1FilterNoField(T2, info, u_front_2);
    }

    // L1TrackPar tStore2 = T2;

    //assert(T2.IsConsistent(true, n2_4));

    // a good place to debug the fit

    /*    
    if (tStore1.IsConsistent(false, n2_4) && !tStore2.IsConsistent(false, n2_4)) {
      cout << " i2 " << i2 << " dx2 " << dx2 << endl;
      cout << "\n\n before filtration: \n\n" << endl;
      tStore1.Print();
      tStore1.IsConsistent(true, n2_4);      
      cout << "\n\n after filtration: \n\n" << endl;
      tStore2.Print();
      tStore2.IsConsistent(true, n2_4); 
      cout << "\n\n " << n2_4 << " vector elements are filled." << endl;
      cout << "measurement: cos " << info.cos_phi[0] << " sin " << info.sin_phi[0] << " u " << u_front_2 << " s2 "
           << sqrt(info.sigma2) << endl;
      cout << "\n\n" << endl;
      exit(0);
    }
    */

    info = stam.backInfo;
    if (fUseHitErrors) info.sigma2 = dv2 * dv2;

    if (istam < fNfieldStations) { L1Filter(T2, info, u_back_2); }
    else {
      L1FilterNoField(T2, info, u_back_2);
    }

    // assert(T2.IsConsistent(true, n2_4));

    FilterTime(T2, timeM, timeMEr, stam.timeInfo);

    // assert(T2.IsConsistent(true, n2_4));

    if constexpr (L1Constants::control::kIfUseRadLengthTable) {
      if (kMcbm == fTrackingMode) {
        fit.L1AddThickMaterial(T2, fParameters.GetMaterialThickness(istam, T2.x, T2.y), fMaxInvMom, fvec::One(),
                               stam.materialInfo.thick, 1);
      }
      else if (kGlobal == fTrackingMode) {
        fit.L1AddMaterial(T2, fParameters.GetMaterialThickness(istam, T2.x, T2.y), fMaxInvMom, fvec::One());
      }
      else {
        fit.L1AddMaterial(T2, fParameters.GetMaterialThickness(istam, T2.x, T2.y), T2.qp, fvec::One());
      }
    }
    else {
      fit.L1AddMaterial(T2, stam.materialInfo, T2.qp, fvec::One());
    }
    if ((istar >= fNstationsBeforePipe) && (istam <= fNstationsBeforePipe - 1)) { fit.L1AddPipeMaterial(T2, T2.qp, 1); }

    // assert(T2.IsConsistent(true, n2_4));

    fvec dz2 = star.z - T2.z;
    L1ExtrapolateTime(T2, dz2, stam.timeInfo);

    // assert(T2.IsConsistent(true, n2_4));

    // extrapolate to the right hit station

    if (istar <= fNfieldStations) {
      L1Extrapolate(T2, star.z, T2.qp, f2);  // Full extrapolation in the magnetic field
    }
    else {
      L1ExtrapolateLine(T2, star.z);  // Extrapolation with line ()
    }

    // assert(T2.IsConsistent(true, n2_4));

    // ---- Find the triplets(right hit). Reformat data in the portion of triplets. ----
    for (size_t i2_4 = 0; i2_4 < n2_4; ++i2_4) {

      //if (!T2.IsEntryConsistent(false, i2_4)) { continue; }
      if (kSts == fTrackingMode && (T2.C44[i2_4] < 0)) { continue; }
      if (T2.C00[i2_4] < 0 || T2.C11[i2_4] < 0 || T2.C22[i2_4] < 0 || T2.C33[i2_4] < 0 || T2.C55[i2_4] < 0) continue;

      if (fabs(T2.tx[i2_4]) > fMaxSlope) continue;
      if (fabs(T2.ty[i2_4]) > fMaxSlope) continue;

      const fvec Pick_r22   = (fTripletChi2Cut - T2.chi2);
      const fscal timeError = T2.C55[i2_4];
      const fscal time      = T2.t[i2_4];
      // find first possible hit

#ifdef DO_NOT_SELECT_TRIPLETS
      if (isec == TRACKS_FROM_TRIPLETS_ITERATION) { Pick_r22 = Pick_r2 + 1; }
#endif  // DO_NOT_SELECT_TRIPLETS
      const fscal iz = 1.f / (T2.z[i2_4] - fParameters.GetTargetPositionZ()[0]);
      L1HitAreaTime area(vGridTime[&star - fParameters.GetStations().begin()], T2.x[i2_4] * iz, T2.y[i2_4] * iz,
                         (sqrt(Pick_r22 * (T2.C00 + stam.XYInfo.C00)) + fMaxDZ * abs(T2.tx))[i2_4] * iz,
                         (sqrt(Pick_r22 * (T2.C11 + stam.XYInfo.C11)) + fMaxDZ * abs(T2.ty))[i2_4] * iz, time,
                         sqrt(timeError) * 5);

      L1HitIndex_t irh              = 0;
      L1HitIndex_t doubletNtriplets = 0;
      int irh1                      = -1;
      while (true) {
        if (fParameters.DevIsIgnoreHitSearchAreas()) {
          irh1++;
          if ((L1HitIndex_t) irh1 >= (HitsUnusedStopIndex[istar] - HitsUnusedStartIndex[istar])) break;
          irh = irh1;
        }
        else {
          if (!area.GetNext(irh)) break;
        }

        // while (area.GetNext(irh)) {
        //for (int irh = 0; irh < ( HitsUnusedStopIndex[istar] - HitsUnusedStartIndex[istar] ); irh++){
        const L1HitPoint& hitr = vHits_r[irh];

        if (fParameters.DevIsMatchTripletsViaMc()) {
          int indM = HitsUnusedStartIndex[iStaM] + hitsm_2_tmp[i2_4];
          int indR = HitsUnusedStartIndex[iStaR] + irh;
          if (GetMcTrackIdForUnusedHit(indM) != GetMcTrackIdForUnusedHit(indR)) { continue; }
        }

#ifdef USE_EVENT_NUMBER  // NOTE:
        if ((T2.n[i2_4] != hitr.n)) continue;
#endif  // USE_EVENT_NUMBER
        const fscal zr = hitr.Z();
        //  const fscal yr = hitr.Y();

        auto [xr, yr] = star.ConvUVtoXY<fscal>(hitr.U(), hitr.V());

        L1TrackPar T_cur = T2;

        fvec dz3 = zr - T_cur.z;
        L1ExtrapolateTime(T_cur, dz3, star.timeInfo);

        L1ExtrapolateLine(T_cur, zr);

        if ((star.timeInfo) && (stam.timeInfo))
          if (fabs(T_cur.t[i2_4] - hitr.time) > sqrt(T_cur.C55[i2_4] + hitr.timeEr) * 5) continue;

        // TODO: SG: hardcoded cut of 30 ns
        if ((star.timeInfo) && (stam.timeInfo))
          if (fabs(T_cur.t[i2_4] - hitr.time) > 30) continue;

        // - check whether hit belong to the window ( track position +\- errors ) -
        // check lower boundary
        fvec y, C11;
        L1ExtrapolateYC11Line(T2, zr, y, C11);
        // cout << "sta " << iStaR << " dy = " << sqrt(C11) << endl;
        fscal dy_est2 =
          (Pick_r22[i2_4]
           * (fabs(
             C11[i2_4]
             + star.XYInfo.C11[i2_4])));  // TODO for FastPrim dx < dy - other sort is optimal. But not for doublets

        /// Covariation matrix of the hit
        auto [dxxScalRhit, dxyScalRhit, dyyScalRhit] = star.FormXYCovarianceMatrix(hitr.dU(), hitr.dV());

        if (fUseHitErrors) { dy_est2 = (Pick_r22[i2_4] * (fabs(C11[i2_4] + dyyScalRhit))); }

        const fscal dY  = yr - y[i2_4];
        const fscal dY2 = dY * dY;

        if (dY2 > dy_est2) continue;  // if (yr > y_plus_new [i2_4] ) continue;
        // check x-boundaries
        fvec x, C00;

        L1ExtrapolateXC00Line(T2, zr, x, C00);

        // cout << "sta " << iStaR << " dx = " << sqrt(C00) << endl;

        fscal dx_est2 = (Pick_r22[i2_4] * (fabs(C00[i2_4] + star.XYInfo.C00[i2_4])));

        if (fUseHitErrors) { dx_est2 = (Pick_r22[i2_4] * (fabs(C00[i2_4] + dxxScalRhit))); }

        const fscal dX = xr - x[i2_4];
        if (dX * dX > dx_est2) continue;
        // check chi2  // not effective
        fvec C10;
        L1ExtrapolateC10Line(T2, zr, C10);
        fvec chi2 = T2.chi2;

        info = star.frontInfo;

        if (fUseHitErrors) info.sigma2 = hitr.dU() * hitr.dU();

        L1FilterChi2XYC00C10C11(info, x, y, C00, C10, C11, chi2, hitr.U());
        info = star.backInfo;

        if (fUseHitErrors) info.sigma2 = hitr.dV() * hitr.dV();

        L1FilterChi2(info, x, y, C00, C10, C11, chi2, hitr.V());

        FilterTime(T_cur, hitr.time, hitr.timeEr, star.timeInfo);


#ifdef DO_NOT_SELECT_TRIPLETS
        if (isec != TRACKS_FROM_TRIPLETS_ITERATION)
#endif  // DO_NOT_SELECT_TRIPLETS
          if (chi2[i2_4] > fTripletChi2Cut || C00[i2_4] < 0 || C11[i2_4] < 0 || T_cur.C55[i2_4] < 0) {
            continue;  // chi2_triplet < CHI2_CUT
          }

#ifndef FAST_CODE
//TODO: optimise triplet portion limit and put a warning
// assert(doubletNtriplets < fParameters.GetMaxTripletPerDoublets());
#endif  // FAST_CODE

        if (doubletNtriplets >= fParameters.GetMaxTripletPerDoublets()) {
          cout << "L1: GetMaxTripletPerDoublets==" << fParameters.GetMaxTripletPerDoublets()
               << " reached in findTripletsStep0()" << endl;
          // reject already created triplets for this doublet
          n3   = n3 - doubletNtriplets;
          n3_V = n3 / fvec::size();
          n3_4 = n3 % fvec::size();
          //assert(0);
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
        du_[n3_V][n3_4]       = hitr.dU();
        dv_[n3_V][n3_4]       = hitr.dV();
        z_Pos_3[n3_V][n3_4]   = zr;
        timeR[n3_V][n3_4]     = hitr.time;
        timeER[n3_V][n3_4]    = hitr.timeEr;

        n3++;
        n3_V = n3 / fvec::size();
        n3_4 = n3 % fvec::size();

        if (0 == n3_4) {
          T_3.push_back(L1TrackPar_0);
          u_front_3.push_back(fvec::Zero());
          u_back_3.push_back(fvec::Zero());
          z_Pos_3.push_back(fvec::Zero());
          du_.push_back(fvec::Zero());
          dv_.push_back(fvec::Zero());
          timeR.push_back(fvec::Zero());
          timeER.push_back(fvec::Zero());
        }
      }  // search area

    }  // i2_4
  }    // i2_V
}

/// Add the right hits to parameters estimation.
inline void L1Algo::findTripletsStep1(  // input
  Tindex n3_V, const L1Station& star, L1Vector<fvec>& u_front_, L1Vector<fvec>& u_back_, L1Vector<fvec>& z_Pos,
  L1Vector<fvec>& dv_, L1Vector<fvec>& du_, L1Vector<fvec>& timeR, L1Vector<fvec>& timeER,
  // output
  //                L1TrackPar *T_3
  L1Vector<L1TrackPar>& T_3)
{

  for (Tindex i3_V = 0; i3_V < n3_V; ++i3_V) {

    fvec dz = z_Pos[i3_V] - T_3[i3_V].z;

    L1TrackPar& T3 = T_3[i3_V];

    // assert(T3.IsConsistent(true, -1));

    L1ExtrapolateTime(T3, dz, star.timeInfo);

    L1ExtrapolateLine(T3, z_Pos[i3_V]);

    // assert(T3.IsConsistent(true, -1));

    L1UMeasurementInfo info = star.frontInfo;

    if (fUseHitErrors) info.sigma2 = du_[i3_V] * du_[i3_V];

    bool noField = (&star - fParameters.GetStations().begin() >= fNfieldStations);

    if (noField) { L1FilterNoField(T3, info, u_front_[i3_V]); }
    else {
      L1Filter(T3, info, u_front_[i3_V]);
    }

    // assert(T3.IsConsistent(true, -1));

    info = star.backInfo;

    if (fUseHitErrors) info.sigma2 = dv_[i3_V] * dv_[i3_V];

    if (noField) { L1FilterNoField(T3, info, u_back_[i3_V]); }
    else {
      L1Filter(T3, info, u_back_[i3_V]);
    }

    // assert(T3.IsConsistent(true, -1));

    if (kGlobal != fTrackingMode && kMcbm != fTrackingMode) {
      FilterTime(T3, timeR[i3_V], timeER[i3_V], star.timeInfo);
    }

    // assert(T3.IsConsistent(true, -1));
    //  FilterTime(T_3[i3_V], timeR[i3_V], timeER[i3_V]);
  }
}


/// Refit Triplets.
inline void L1Algo::findTripletsStep2(  // input // TODO not updated after gaps introduction
  Tindex n3, int istal, L1Vector<L1TrackPar>& T_3, L1Vector<L1HitIndex_t>& hitsl_3, L1Vector<L1HitIndex_t>& hitsm_3,
  L1Vector<L1HitIndex_t>& hitsr_3, int nIterations)
{
  L1Fit fit;
  fit.SetParticleMass(fDefaultMass);

  const int NHits = 3;  // triplets

  // TODO: SG: middle and right station numbers might be different for different triplets!!!

  // prepare data
  int ista[NHits] = {istal, istal + 1, istal + 2};

  L1Station sta[3];
  for (int is = 0; is < NHits; ++is) {
    sta[is] = fParameters.GetStation(ista[is]);
  };

  for (int i3 = 0; i3 < n3; ++i3) {
    int i3_V = i3 / fvec::size();
    int i3_4 = i3 % fvec::size();

    L1TrackPar& T3 = T_3[i3_V];

    //if (!T3.IsEntryConsistent(false, i3_4)) continue;

    fscal qp0 = T3.qp[i3_4];

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

    fscal u[NHits], v[NHits], x[NHits], y[NHits], z[NHits];
    for (int ih = 0; ih < NHits; ++ih) {
      const L1Hit& hit       = fInputData.GetHit(ihit[ih]);
      u[ih]                  = hit.u;
      v[ih]                  = hit.v;
      std::tie(x[ih], y[ih]) = sta[ih].ConvUVtoXY<fscal>(u[ih], v[ih]);
      z[ih]                  = hit.z;
    };

    // initialize parameters
    L1TrackPar T;

    const fvec vINF = .1;
    T.x             = x[0];
    T.y             = y[0];

    fvec dz01 = 1. / (z[1] - z[0]);
    T.tx      = (x[1] - x[0]) * dz01;
    T.ty      = (y[1] - y[0]) * dz01;

    T.qp   = qp0;
    T.z    = z[0];
    T.chi2 = 0.;
    T.NDF  = 2.;
    T.C00  = sta[0].XYInfo.C00;
    T.C10  = sta[0].XYInfo.C10;
    T.C11  = sta[0].XYInfo.C11;

    T.C20 = T.C21 = 0;
    T.C30 = T.C31 = T.C32 = 0;
    T.C40 = T.C41 = T.C42 = T.C43 = 0;
    T.C22 = T.C33 = vINF;
    T.C44         = 1.;

    // find field along the track
    L1FieldValue B[3] _fvecalignment;
    L1FieldRegion fld _fvecalignment;

    fvec tx[3] = {(x[1] - x[0]) / (z[1] - z[0]), (x[2] - x[0]) / (z[2] - z[0]), (x[2] - x[1]) / (z[2] - z[1])};
    fvec ty[3] = {(y[1] - y[0]) / (z[1] - z[0]), (y[2] - y[0]) / (z[2] - z[0]), (y[2] - y[1]) / (z[2] - z[1])};
    for (int ih = 0; ih < NHits; ++ih) {
      fvec dz = (sta[ih].z - z[ih]);
      sta[ih].fieldSlice.GetFieldValue(x[ih] + tx[ih] * dz, y[ih] + ty[ih] * dz, B[ih]);
    };

    fld.Set(B[0], sta[0].z, B[1], sta[1].z, B[2], sta[2].z);

    // fit
    for (int ih = 1; ih < NHits; ++ih) {
      L1Extrapolate(T, z[ih], T.qp, fld);
      if constexpr (L1Constants::control::kIfUseRadLengthTable) {
        fit.L1AddMaterial(T, fParameters.GetMaterialThickness(ista[ih], T.x, T.y), T.qp, fvec::One());
      }
      else {
        fit.L1AddMaterial(T, sta[ih].materialInfo, T.qp, fvec::One());
      }
      if (ista[ih] == fNstationsBeforePipe - 1) { fit.L1AddPipeMaterial(T, T.qp, fvec::One()); }
      L1Filter(T, sta[ih].frontInfo, u[ih]);
      L1Filter(T, sta[ih].backInfo, v[ih]);
    }

    // repeat several times in order to improve precision
    for (int iiter = 0; iiter < nIterations; ++iiter) {
      // fit backward
      // keep tx,ty,q/p
      int ih = NHits - 1;
      T.x    = x[ih];
      T.y    = y[ih];
      T.z    = z[ih];
      T.chi2 = 0.;
      T.NDF  = 2.;
      T.C00  = sta[ih].XYInfo.C00;
      T.C10  = sta[ih].XYInfo.C10;
      T.C11  = sta[ih].XYInfo.C11;

      T.C20 = T.C21 = 0;
      T.C30 = T.C31 = T.C32 = 0;
      T.C40 = T.C41 = T.C42 = T.C43 = 0;
      T.C22 = T.C33 = vINF;
      T.C44         = 1.;

      //       L1Filter( T, sta[ih].frontInfo, u[ih] );
      //       L1Filter( T, sta[ih].backInfo,  v[ih] );
      for (ih = NHits - 2; ih >= 0; ih--) {
        L1Extrapolate(T, z[ih], T.qp, fld);
        if constexpr (L1Constants::control::kIfUseRadLengthTable) {
          fit.L1AddMaterial(T, fParameters.GetMaterialThickness(ista[ih], T.x, T.y), T.qp, fvec::One());
        }
        else {
          fit.L1AddMaterial(T, sta[ih].materialInfo, T.qp, fvec::One());
        }
        if (ista[ih] == fNstationsBeforePipe) fit.L1AddPipeMaterial(T, T.qp, fvec::One());

        L1Filter(T, sta[ih].frontInfo, u[ih]);
        L1Filter(T, sta[ih].backInfo, v[ih]);
      }
      // fit forward
      ih     = 0;
      T.x    = x[ih];
      T.y    = y[ih];
      T.z    = z[ih];
      T.chi2 = 0.;
      T.NDF  = 2.;
      T.C00  = sta[ih].XYInfo.C00;
      T.C10  = sta[ih].XYInfo.C10;
      T.C11  = sta[ih].XYInfo.C11;

      T.C20 = T.C21 = 0;
      T.C30 = T.C31 = T.C32 = 0;
      T.C40 = T.C41 = T.C42 = T.C43 = 0;
      T.C22 = T.C33 = vINF;
      T.C44         = 1.;

      //       L1Filter( T, sta[ih].frontInfo, u[ih] );
      //       L1Filter( T, sta[ih].backInfo,  v[ih] );
      for (ih = 1; ih < NHits; ++ih) {
        L1Extrapolate(T, z[ih], T.qp, fld);
        if constexpr (L1Constants::control::kIfUseRadLengthTable) {
          fit.L1AddMaterial(T, fParameters.GetMaterialThickness(ista[ih], T.x, T.y), T.qp, fvec::One());
        }
        else {
          fit.L1AddMaterial(T, sta[ih].materialInfo, T.qp, fvec::One());
        }
        if (ista[ih] == fNstationsBeforePipe + 1) fit.L1AddPipeMaterial(T, T.qp, fvec::One());

        L1Filter(T, sta[ih].frontInfo, u[ih]);
        L1Filter(T, sta[ih].backInfo, v[ih]);
      }
    }  // for iiter

    T3.SetOneEntry(i3_4, T, i3_4);
  }  //i3
}  // findTripletsStep2


inline void L1Algo::findTripletsStep3(  // input
  Tindex n3, int istal, int istam, int istar, L1Vector<L1TrackPar>& T_3, L1Vector<L1HitIndex_t>& hitsl_3,
  L1Vector<L1HitIndex_t>& hitsm_3, L1Vector<L1HitIndex_t>& hitsr_3,
  // output
  Tindex& nstaltriplets)
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
  ///
  /// Output:
  ///
  /// \param nstaltriplets updated number of triplets in fTriplets[istal][Thread]

  // TODO: SG: remove nstaltriplets parameter

#ifdef _OPENMP
  unsigned int Thread = omp_get_thread_num();
#else
  unsigned int Thread = 0;
#endif

  L1HitIndex_t ihitl_prev = 0;

  for (Tindex i3 = 0; i3 < n3; ++i3) {
    const Tindex i3_V = i3 / fvec::size();
    const Tindex i3_4 = i3 % fvec::size();

    L1TrackPar& T3 = T_3[i3_V];

    //if (!T3.IsEntryConsistent(false, i3_4)) continue;

    // select
    fscal chi2 = T3.chi2[i3_4];

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

#ifdef DO_NOT_SELECT_TRIPLETS
    if (isec != TRACKS_FROM_TRIPLETS_ITERATION) {
      if (chi2 > fTripletChi2Cut) { continue; }
    }
#else
    if (chi2 > fTripletChi2Cut) { continue; }
#endif  // DO_NOT_SELECT_TRIPLETS

    // assert(std::isfinite(chi2) && chi2 > 0);

    if (!std::isfinite(chi2) || chi2 < 0) { continue; }
    //if (!T3.IsEntryConsistent(false, i3_4)) { continue; }

    fscal qp = T3.qp[i3_4];

    //TODO: why sqrt's? Wouldn't it be faster to skip sqrt() here and
    //TODO: compare the squared differences dqr*dqp later?

    fscal Cqp = sqrt(fabs(T3.C44[i3_4]));

    {  // legacy

      // TODO: SG: The magic cuts below are the rest from an old conversion of the momentum to char.
      // TODO: They came from the truncation to the 0-255 range and had no other meaning.
      // TODO: But for some reason, the efficiency degrades without them.
      // TODO: It needs to be investigated. If the cuts are necessary, they need to be adjusted.

      fscal Cmax = 0.04 * fMaxInvMom[0];  // minimal momentum: 0.05 - 0.1
                                          //if ( isec == kAllPrimJumpIter ) {
      if (Cqp > Cmax) {
        //cout << "isec " << isec << " Cqp " << Cqp << " max " << Cmax << " add " << 0.05 * Cmax << endl;
        Cqp = Cmax;
      }
      Cqp += 0.05 * Cmax;  // TODO: without this line the ghost ratio increases, why?
    }

    fTriplets[istal][Thread].emplace_back(ihitl, ihitm, ihitr, istal, istam, istar, 0, 0, 0, chi2, qp, Cqp, T3.tx[i3_4],
                                          sqrt(fabs(T3.C22[i3_4])), T3.ty[i3_4], sqrt(fabs(T3.C33[i3_4])));

    L1Triplet& tr1 = fTriplets[istal][Thread].back();
    tr1.SetLevel(0);

    ++nstaltriplets;

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

      if ((curNeighbour.GetMHit() != ihitr)) continue;

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
#ifdef TRACKS_FROM_TRIPLETS
  if (isec != TRACKS_FROM_TRIPLETS_ITERATION)
#endif

    for (int istal = fParameters.GetNstationsActive() - 4; istal >= fFirstCAstation; istal--) {
      for (int tripType = 0; tripType < 3; tripType++)  // tT = 0 - 123triplet, tT = 1 - 124triplet, tT = 2 - 134triplet
      {
        if ((((isec != kFastPrimJumpIter) && (isec != kAllPrimJumpIter) && (isec != kAllSecJumpIter))
             && (tripType != 0))
            || (((isec == kFastPrimJumpIter) || (isec == kAllPrimJumpIter) || (isec == kAllSecJumpIter))
                && (tripType != 0) && (istal == fParameters.GetNstationsActive() - 4)))
          continue;

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

              if (fabs(trip.GetQp() - neigh.GetQp()) > fPickNeighbour * (trip.GetCqp() + neigh.GetCqp()))
                continue;  // neighbours should have same qp

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
}

/// ------------------- doublets on station ----------------------

inline void L1Algo::DupletsStaPort(
  /// input:
  int istal, int istam, Tindex ip, L1Vector<Tindex>& n_g, Tindex* portionStopIndex_,
  /// output:
  L1TrackPar* T_1, L1FieldRegion* fld_1, L1HitIndex_t* hitsl_1, L1Vector<char>& lmDuplets, Tindex& n_2,
  L1Vector<L1HitIndex_t>& i1_2, L1Vector<L1HitIndex_t>& hitsm_2
  ///
)
{
  /// creates duplets.
  /// input:
  ///   @istal - start station number
  ///   @istam - last station number
  ///   @ip - index of portion
  ///   @&n_g - number of elements in portion
  ///   @*portionStopIndex
  /// output:
  ///   @*T_1 - singlets perameters
  ///   @*fld_1 - field aproximation
  ///   @*hitsl_1- left hits of triplets
  ///   @&lmDuplets - existance of a doublet starting from the left hit
  ///   @&n_2 - number of douplets
  ///   @&i1_2 - index of 1st hit in portion indexed by doublet index
  ///   @&hitsm_2 - index of middle hit in hits array indexed by doublet index

  if (istam < fParameters.GetNstationsActive()) {
    const L1Station& stal = fParameters.GetStation(istal);
    const L1Station& stam = fParameters.GetStation(istam);

    // prepare data
    L1HitPoint* vHits_l = &((*vHitPointsUnused)[0]) + HitsUnusedStartIndex[istal];
    L1HitPoint* vHits_m = &((*vHitPointsUnused)[0]) + HitsUnusedStartIndex[istam];

    fvec u_front[L1Constants::size::kPortionLeftHitsP];
    fvec u_back[L1Constants::size::kPortionLeftHitsP];
    fvec dv0[L1Constants::size::kPortionLeftHitsP];
    fvec du0[L1Constants::size::kPortionLeftHitsP];
    fvec zPos[L1Constants::size::kPortionLeftHitsP];
    fvec HitTime[L1Constants::size::kPortionLeftHitsP];
    fvec HitTimeEr[L1Constants::size::kPortionLeftHitsP];
    fvec Event[L1Constants::size::kPortionLeftHitsP];

    /// prepare the portion of left hits data
    Tindex& n1 = n_g[ip];

    findSingletsStep0(  // input
      (ip - portionStopIndex_[istal + 1]) * L1Constants::size::kPortionLeftHits, n1, vHits_l,
      // output
      u_front, u_back, zPos, hitsl_1, HitTime, HitTimeEr, Event, du0, dv0);

    for (Tindex i = 0; i < n1; ++i)
      L1_ASSERT(hitsl_1[i] < HitsUnusedStopIndex[istal] - HitsUnusedStartIndex[istal],
                hitsl_1[i] << " < " << HitsUnusedStopIndex[istal] - HitsUnusedStartIndex[istal]);

    Tindex n1_V = (n1 + fvec::size() - 1) / fvec::size();

    /// Get the field approximation. Add the target to parameters estimation. Propagaete to middle station.

    findSingletsStep1(istal, istam, n1_V,

                      u_front, u_back, zPos, HitTime, HitTimeEr,
                      // output
                      T_1, fld_1, du0, dv0);

    /// Find the doublets. Reformat data in the portion of doublets.

    // TODO: repalce with constexpr if (C++17) (S.Zharko)
#ifdef DOUB_PERFORMANCE
    L1Vector<L1HitIndex_t> hitsl_2("L1CATrackFinder::hitsl_2");
#endif  // DOUB_PERFORMANCE

    findDoubletsStep0(  // input
      n1, stal, stam, vHits_m, T_1, hitsl_1,
      // output
      n_2, i1_2,
#ifdef DOUB_PERFORMANCE
      hitsl_2,
#endif  // DOUB_PERFORMANCE
      hitsm_2, Event, lmDuplets);

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

inline void L1Algo::TripletsStaPort(  /// creates triplets:
  /// input:
  ///  @istal - start station number,
  ///  @istam - middle station number,
  ///  @istar - last station number,
  ///  @ip - index of portion,
  ///  @&n_g - numer of elements in portion,
  ///  @*portionStopIndex
  int istal, int istam, int istar,

  /// @nstaltriplets - ,
  /// @*portionStopIndex,
  /// @*T_1 - track parameters for singlets,
  /// @*fld_1 - field approximation for singlets,
  /// @&n_2 - number of doublets in portion
  /// @&n_2 - number of douplets,
  /// @&i1_2 - index of 1st hit in portion indexed by doublet index,
  /// @&hitsm_2 - index of middle hit in hits array indexed by doublet index


  Tindex& nstaltriplets, L1TrackPar* T_1, L1FieldRegion* fld_1, L1HitIndex_t* hitsl_1,

  Tindex& n_2, L1Vector<L1HitIndex_t>& i1_2, L1Vector<L1HitIndex_t>& hitsm_2,

  const L1Vector<char>& mrDuplets

  /// output:
  // @*vTriplets_part - array of triplets,
  // @*TripStartIndexH,
  // @*TripStopIndexH - start/stop index of a triplet in the array

)
{
  if (istar < fParameters.GetNstationsActive()) {
    // prepare data
    const L1Station& stam = fParameters.GetStation(istam);
    const L1Station& star = fParameters.GetStation(istar);

    L1HitPoint* vHits_m = &((*vHitPointsUnused)[0]) + HitsUnusedStartIndex[istam];

    L1HitPoint* vHits_r = 0;
    vHits_r             = &((*vHitPointsUnused)[0]) + HitsUnusedStartIndex[istar];

    Tindex n3 = 0, n3_V;

    /// Add the middle hits to parameters estimation. Propagate to right station.


#ifdef _OPENMP
    int Thread = omp_get_thread_num();
#else
    int Thread = 0;
#endif

    L1Vector<L1TrackPar>& T_3       = fT_3[Thread];
    L1Vector<L1HitIndex_t>& hitsl_3 = fhitsl_3[Thread];
    L1Vector<L1HitIndex_t>& hitsm_3 = fhitsm_3[Thread];
    L1Vector<L1HitIndex_t>& hitsr_3 = fhitsr_3[Thread];
    L1Vector<fvec>& u_front3        = fu_front3[Thread];
    L1Vector<fvec>& u_back3         = fu_back3[Thread];
    L1Vector<fvec>& z_pos3          = fz_pos3[Thread];
    L1Vector<fvec>& timeR           = fTimeR[Thread];
    L1Vector<fvec>& timeER          = fTimeER[Thread];
    L1Vector<fvec>& du3             = du[Thread];
    L1Vector<fvec>& dv3             = dv[Thread];

    T_3.clear();
    hitsl_3.clear();
    hitsm_3.clear();
    hitsr_3.clear();
    u_front3.clear();
    u_back3.clear();
    z_pos3.clear();
    du3.clear();
    dv3.clear();
    timeR.clear();
    timeER.clear();

    /// Find the triplets(right hit). Reformat data in the portion of triplets.

    findTripletsStep0(  // input
      vHits_r, stam, star,

      istam, istar, vHits_m, T_1, fld_1, hitsl_1,

      n_2, hitsm_2, i1_2,

      mrDuplets,
      // output
      n3, T_3, hitsl_3, hitsm_3, hitsr_3, u_front3, u_back3, z_pos3, du3, dv3, timeR, timeER);


    n3_V = (n3 + fvec::size() - 1) / fvec::size();

    for (Tindex i = 0; i < static_cast<Tindex>(hitsl_3.size()); ++i)
      L1_assert(hitsl_3[i] < HitsUnusedStopIndex[istal] - HitsUnusedStartIndex[istal]);
    for (Tindex i = 0; i < static_cast<Tindex>(hitsm_3.size()); ++i)
      L1_assert(hitsm_3[i] < HitsUnusedStopIndex[istam] - HitsUnusedStartIndex[istam]);
    for (Tindex i = 0; i < static_cast<Tindex>(hitsr_3.size()); ++i)
      L1_assert(hitsr_3[i] < HitsUnusedStopIndex[istar] - HitsUnusedStartIndex[istar]);

    //        if (n3 >= MaxPortionTriplets) cout << "isec: " << isec << " station: " << istal << " portion number: " << ip << " CATrackFinder: Warning: Too many Triplets created in portion" << endl;

    /// Add the right hits to parameters estimation.
    findTripletsStep1(  // input
      n3_V, star, u_front3, u_back3, z_pos3, du3, dv3, timeR, timeER,
      // output
      T_3);


    /// refit
    //         findTripletsStep2(  n3, istal, _RealIHit,          T_3,         hitsl_3, hitsm_3, hitsr_3, 0 );

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
      if (T_3[i_V].chi2[i_4] < fTripletChi2Cut) { fL1Eff_triplets2->AddOne(iHits); }
    }
#endif  // TRIP_PERFORMANCE


    /// Fill Triplets.
    findTripletsStep3(  // input
      n3, istal, istam, istar, T_3, hitsl_3, hitsm_3, hitsr_3,
      // output
      nstaltriplets);
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


/***********************************************************************************************/ /**
 *                                                                                                *
 *                            ------ CATrackFinder procedure ------                               *
 *                                                                                                *
 **************************************************************************************************/

void L1Algo::CATrackFinder()
{

#ifdef TRACKS_FROM_TRIPLETS
  // TODO investigate kAllPrimJumpIter & kAllSecJumpIter
  fNFindIterations = TRACKS_FROM_TRIPLETS_ITERATION + 1;
#else
  fNFindIterations = fParameters.GetNcaIterations();
#endif


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
    bool timeUninitialised = 1;
    fscal lasttime         = 0;
    fscal starttime        = 0;
    for (L1HitIndex_t ih = fInputData.GetStartHitIndex(iS); ih < fInputData.GetStopHitIndex(iS); ++ih) {
      const fscal time = fInputData.GetHit(ih).t;
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

        fFirstCAstation = caIteration.GetFirstStationIndex();
        fTrackChi2Cut   = caIteration.GetTrackChi2Cut();
        fDoubletChi2Cut = caIteration.GetDoubletChi2Cut();  //11.3449 * 2.f / 3.f;  // prob = 0.1
        fTripletChi2Cut = caIteration.GetTripletChi2Cut();  //21.1075;  // prob = 0.01%
        fPickGather     = caIteration.GetPickGather();      //3.0;
        fPickNeighbour  = caIteration.GetPickNeighbour();   //5.0;
        fMaxInvMom      = caIteration.GetMaxInvMom();       //1.0 / 0.5;  // max considered q/p
        fMaxSlopePV     = caIteration.GetMaxSlopePV();      //1.1;
        fMaxSlope       = caIteration.GetMaxSlope();        //2.748;  // corresponds to 70 grad

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
      fDupletPortionStopIndex[fParameters.GetNstationsActive() - 1] = 0;
      fDupletPortionSize.clear();
      for (int istal = fParameters.GetNstationsActive() - 2; istal >= fFirstCAstation;
           istal--) {  //start downstream chambers
        int NHits_l   = HitsUnusedStopIndex[istal] - HitsUnusedStartIndex[istal];
        int nPortions = NHits_l / L1Constants::size::kPortionLeftHits;
        int rest      = NHits_l - nPortions * L1Constants::size::kPortionLeftHits;
        for (int ipp = 0; ipp < nPortions; ipp++) {
          fDupletPortionSize.push_back(L1Constants::size::kPortionLeftHits);
        }  // start_lh - portion of left hits
        if (rest > 0) fDupletPortionSize.push_back(rest);
        fDupletPortionStopIndex[istal] = fDupletPortionSize.size();
      }  // lstations


#ifdef COUNTERS
      stat_nSinglets[isec] += nSinglets;
#endif
    }

    /*    {
         /// possible left hits of triplets are splited in portions of 16 (4 SIMDs) to use memory faster
         fDupletPortionStopIndex[fParameters.GetNstationsActive() - 1] = 0;
         unsigned int ip = 0;  //index of curent portion

         for (int istal = fParameters.GetNstationsActive() - 2; istal >= fFirstCAstation; istal--)  //start downstream chambers
         {
         int nHits = HitsUnusedStopIndex[istal] - HitsUnusedStartIndex[istal];
         
         int NHits_P = nHits/L1Constants::size::kPortionLeftHits;
         
         for( int ipp = 0; ipp < NHits_P; ipp++ )
         {
         n_g1[ip] = L1Constants::size::kPortionLeftHits;
         ip++;
         } // start_lh - portion of left hits
         
         n_g1[ip] = nHits - NHits_P * L1Constants::size::kPortionLeftHits;
         
         ip++;
         fDupletPortionStopIndex[istal] = ip;
         }// lstations
         //       nPortions = ip;
         } */

    ///   stage for triplets creation

#ifdef XXX
    TStopwatch c_timer;
    c_timer.Start();
#endif


    L1TrackPar T_1[L1Constants::size::kPortionLeftHitsP];
    L1FieldRegion fld_1[L1Constants::size::kPortionLeftHitsP];
    L1HitIndex_t hitsl_1[L1Constants::size::kPortionLeftHits];
    L1TrackPar TG_1[L1Constants::size::kPortionLeftHitsP];
    L1FieldRegion fldG_1[L1Constants::size::kPortionLeftHitsP];
    L1HitIndex_t hitslG_1[L1Constants::size::kPortionLeftHits];

    /// middle hits indexed by number of doublets in portion(i2)
    L1Vector<L1HitIndex_t> hitsm_2("L1CATrackFinder::hitsm_2");

    /// index in portion of singlets(i1) indexed by index in portion of doublets(i2)
    L1Vector<L1HitIndex_t> i1_2("L1CATrackFinder::i1_2");

    /// middle hits indexed by number of doublets in portion(i2)
    L1Vector<L1HitIndex_t> hitsmG_2("L1CATrackFinder::hitsmG_2");

    /// index in portion of singlets(i1) indexed by index in portion of doublets(i2)
    L1Vector<L1HitIndex_t> i1G_2("L1CATrackFinder::i1G_2");

    /// is exist a doublet started from indexed by left hit
    L1Vector<char> lmDuplets[L1Constants::size::kMaxNstations] {"L1CATrackFinder::lmDuplets"};

    /// is exist a doublet started from indexed by left hit
    L1Vector<char> lmDupletsG[L1Constants::size::kMaxNstations] {"L1CATrackFinder::lmDupletsG"};

    for (int i = 0; i < fParameters.GetNstationsActive(); i++) {
      lmDuplets[i].SetName(std::stringstream() << "L1CATrackFinder::lmDuplets[" << i << "]");
      lmDupletsG[i].SetName(std::stringstream() << "L1CATrackFinder::lmDupletsG[" << i << "]");
    }

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
      for (Tindex ip = fDupletPortionStopIndex[istal + 1]; ip < fDupletPortionStopIndex[istal]; ++ip) {
        Tindex n_2   = 0;  /// number of doublets in portion
        int NHitsSta = fInputData.GetStopHitIndex(istal) - fInputData.GetStartHitIndex(istal);
        lmDuplets[istal].reset(NHitsSta);
        lmDupletsG[istal].reset(NHitsSta);

        hitsm_2.clear();
        i1_2.clear();

        DupletsStaPort(istal, istal + 1, ip, fDupletPortionSize, fDupletPortionStopIndex,

                       // output
                       T_1, fld_1, hitsl_1,

                       lmDuplets[istal],


                       n_2, i1_2, hitsm_2);

        Tindex nstaltriplets = 0;

        TripletsStaPort(  // input
          istal, istal + 1, istal + 2, nstaltriplets, T_1, fld_1, hitsl_1,

          n_2, i1_2, hitsm_2,

          lmDuplets[istal + 1]
          // output
        );

        if ((isec == kFastPrimJumpIter) || (isec == kAllPrimJumpIter) || (isec == kAllSecJumpIter) || (fMissingHits)) {
          // All iterations are "jump"!
          Tindex nG_2;
          hitsmG_2.clear();
          i1G_2.clear();
          if ((fMissingHits && ((istal == 0) || (istal == 1))) || !fMissingHits) {
            DupletsStaPort(  // input
              istal, istal + 2, ip, fDupletPortionSize, fDupletPortionStopIndex,
              // output
              TG_1, fldG_1, hitslG_1,

              lmDupletsG[istal],

              nG_2, i1G_2, hitsmG_2);
          }

          if ((fMissingHits && (istal == 0)) || !fMissingHits) {
            TripletsStaPort(  // input
              istal, istal + 1, istal + 3, nstaltriplets, T_1, fld_1, hitsl_1,

              n_2, i1_2, hitsm_2, lmDupletsG[istal + 1]);
          }

          if ((fMissingHits && (istal == 1)) || !fMissingHits) {
            TripletsStaPort(  // input
              istal, istal + 2, istal + 3, nstaltriplets, TG_1, fldG_1, hitslG_1, nG_2, i1G_2, hitsmG_2,
              lmDuplets[istal + 2]);
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

    // #ifdef XXX
    //     cout<<"---- Collect track candidates. ----"<<endl;
    // #endif

    int min_level =
      caIteration.GetMinLevelTripletStart();  // min level for start triplet. So min track length = min_level+3.
                                              //    if (isec == kFastPrimJumpIter) min_level = 1;
    //if ((isec == kAllSecIter) || (isec == kAllSecEIter) || (isec == kAllSecJumpIter))
    //  min_level = 1;  // only the long low momentum tracks

#ifdef TRACKS_FROM_TRIPLETS
    LOG(FATAL) << "L1CATrackFinder: min_level undefined in " << __FILE__ << " : " << __LINE__;
    if (isec == TRACKS_FROM_TRIPLETS_ITERATION) min_level = 0;
#endif

    // TODO: Just remove it
    // // min_level: lower then this triplets would never start
    //     int min_level = 1; // min level for start triplet. So min track length = min_level+3.
    //     if (isec == kAllPrimJumpIter) min_level = 1;
    //     if ( (isec == kAllSecIter) || (isec == kAllSecJumpIter) ) min_level = 2; // only the long low momentum tracks
    // //    if (isec == -1) min_level = fParameters.GetNstationsActive() - 3 - 3; //only the longest tracks


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
//               ghost supression !!!
#ifndef FIND_GAPED_TRACKS
            if (/*(isec == kFastPrimIter) ||*/ (isec == kAllPrimIter) || (isec == kAllPrimEIter)
                || (isec == kAllSecIter) || (isec == kAllSecEIter) || (isec == kAllSecJumpIter)) {
#else
            if ((isec == kFastPrimIter) || (isec == kFastPrimIter2) || (isec == kFastPrimJumpIter)
                || (isec == kAllPrimIter) || (isec == kAllPrimEIter) || (isec == kAllPrimJumpIter)
                || (isec == kAllSecIter) || (isec == kAllSecEIter) || (isec == kAllSecJumpIter)) {
#endif
#ifdef TRACKS_FROM_TRIPLETS
              if (isec != TRACKS_FROM_TRIPLETS_ITERATION)
#endif
              {  // ghost supression !!!
                // TODO: Primary => 3 hits tracks are saved, otherwise 3 hit tracks are thrown away
                if (isec != kFastPrimIter && isec != kAllPrimIter && isec != kAllPrimEIter && isec != kAllSecEIter)
                  if (first_trip.GetLevel() == 0)
                    continue;  //ghost suppression//find track with 3 hits only if it was created from a chain of triplets, but not from only one triplet

                if (kGlobal != fTrackingMode && kMcbm != fTrackingMode) {
                  if ((firstTripletLevel == 0) && ((*vHitsUnused)[first_trip.GetLHit()].iSt != 0))
                    continue;  // ghost supression // collect only MAPS tracks-triplets  CHECK!!!
                }
              }
              if (first_trip.GetLevel() < firstTripletLevel)
                continue;  // try only triplets, which can start track with firstTripletLevel+3 length. w\o it have more ghosts, but efficiency either
            }

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

#ifndef TRACKS_FROM_TRIPLETS
            if (fGhostSuppression) {
              if (3 == best_L) {
                // if( isec == kAllSecIter ) continue; // too /*short*/ secondary track
                if (((isec == kAllSecIter) || (isec == kAllSecEIter) || (isec == kAllSecJumpIter)) && (istaF != 0))
                  continue;  // too /*short*/ non-MAPS track
                if ((isec != kAllSecIter) && (isec != kAllSecEIter) && (isec != kAllSecJumpIter) && (best_chi2 > 5.0))
                  continue;
              }
            }
#endif
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
#ifdef EXTEND_TRACKS
          if (kGlobal != fTrackingMode && kMcbm != fTrackingMode) {
            if (tr.NHits != fParameters.GetNstationsActive()) BranchExtender(tr);
          }
#endif
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

    //       // try to find more hits
    // #ifdef EXTEND_TRACKS
    //     // if( curr_L < min_best_l )
    //     if (isec != kFastPrimJumpIter && isec != kAllPrimJumpIter && isec != kAllSecJumpIter && curr_L >= 3){
    //       //curr_chi2 = BranchExtender(curr_tr);
    //       BranchExtender(curr_tr);
    //       curr_L = curr_tr.Hits.size();
    //         //      if( 2*curr_chi2 > (2*(curr_L*2-5) + 1) * 4*4 ) return;
    //     }
    // #endif // EXTEND_TRACKS

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

      //    ID = curr_trip->neighbours[in];
      //    const fscal &qp2 = curr_trip->GetQp();
      //    fscal &Cqp2 = curr_trip->Cqp;
      //    if (( fabs(qp - qp2) > fPickNeighbour * (Cqp + Cqp2) ) )  continue;

      unsigned int Station = TripletId2Station(ID);
      unsigned int Thread  = TripletId2Thread(ID);
      unsigned int Triplet = TripletId2Triplet(ID);

      const L1Triplet& new_trip = fTriplets[Station][Thread][Triplet];
      if ((new_trip.GetMHit() != curr_trip->GetRHit())) continue;
      if ((new_trip.GetLHit() != curr_trip->GetMHit())) continue;

      const fscal qp1 = curr_trip->GetQp();
      const fscal qp2 = new_trip.GetQp();
      fscal dqp       = fabs(qp1 - qp2);
      fscal Cqp       = curr_trip->GetCqp();
      Cqp += new_trip.GetCqp();
      if (kGlobal != fTrackingMode && kMcbm != fTrackingMode) {
        if (dqp > fPickNeighbour * Cqp) {
          continue;  // bad neighbour // CHECKME why do we need recheck it?? (it really change result)
        }
      }

      fscal tx1 = curr_trip->GetTx();
      fscal tx2 = new_trip.GetTx();
      fscal dtx = fabs(tx1 - tx2);
      fscal Ctx = curr_trip->GetCtx();
      Ctx += new_trip.GetCtx();

      fscal ty1 = curr_trip->GetTy();
      fscal ty2 = new_trip.GetTy();
      fscal dty = fabs(ty1 - ty2);
      fscal Cty = curr_trip->GetCty();
      Cty += new_trip.GetCty();

      // it shouldn't happen, but happens sometimes

      if (!std::isfinite(dtx)) continue;
      if (!std::isfinite(dty)) continue;
      if (!std::isfinite(Ctx)) continue;
      if (!std::isfinite(Cty)) continue;

      assert(std::isfinite(dtx));
      assert(std::isfinite(dty));
      assert(std::isfinite(Ctx));
      assert(std::isfinite(Cty));

      if (kMcbm == fTrackingMode) {
        if (dty > 3 * Cty) continue;
        if (dtx > 3 * Ctx) continue;
      }

      if (kGlobal == fTrackingMode) {
        if (dty > fPickNeighbour * sqrt(Cty)) continue;  //SGtrd2d
        if (dtx > fPickNeighbour * sqrt(Ctx)) continue;
      }

      //if (GetFUsed((*fStripFlag)[(*vHitsUnused)[new_trip.GetLHit()].f]
      //             | (*fStripFlag)[(*vHitsUnused)[new_trip.GetLHit()].b])) {  //hits are used
      //L1_SHOW(fInputData.GetNhitKeys());
      //L1_SHOW(fvHitKeyFlags.size());
      //L1_SHOW((*vHitsUnused)[new_trip.GetLHit()].f);
      //L1_SHOW((*vHitsUnused)[new_trip.GetLHit()].b);
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
        fscal new_chi2      = curr_chi2;

        dqp = dqp / Cqp;

        if (kGlobal == fTrackingMode) {  //SGtrd2d!!!
          dtx = dtx / sqrt(Ctx);
          dty = dty / sqrt(Cty);
        }
        else {
          dtx = dtx / Ctx;  // TODO: SG: it must be /sqrt(Ctx);
          dty = dty / Cty;  // TODO: SG: it must be /sqrt(Cty);
        }

        assert(std::isfinite(dtx));
        assert(std::isfinite(dty));

        if (kGlobal == fTrackingMode || kMcbm == fTrackingMode) {
          new_chi2 += dtx * dtx;
          new_chi2 += dty * dty;
        }
        else {
          new_chi2 += dqp * dqp;
        }

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
