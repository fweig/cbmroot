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
#endif

#include "TRandom.h"

#include "L1HitsSortHelper.h"
#include "L1Timer.h"
#ifdef DRAW
#include "utils/L1AlgoDraw.h"
#endif
#ifdef PULLS
#include "utils/L1AlgoPulls.h"
#endif
#ifdef TRIP_PERFORMANCE
#include "utils/L1AlgoEfficiencyPerformance.h"
#endif
#ifdef DOUB_PERFORMANCE
#include "utils/L1AlgoEfficiencyPerformance.h"
#endif

#ifdef TBB
#include "L1AlgoTBB.h"
#endif

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>

#include <stdio.h>


// using namespace std;
using std::cout;
using std::endl;


/// Prepare the portion of data of left hits of a triplet: all hits except the last and the second last station will be procesesed in the algorythm,
/// the data is orginesed in order to be used by SIMD
inline void L1Algo::f10(  // input
  Tindex start_lh, Tindex n1_l, L1HitPoint* StsHits_l,
  // output
  fvec* u_front_l, fvec* u_back_l, fvec* zPos_l, THitI* hitsl, fvec* HitTime_l, fvec* HitTimeEr,
  // comment unused parameters, FU, 18.01.21
  fvec* /*Event_l*/, fvec* /*d_x*/, fvec* /*d_y*/, fvec* /*d_xy*/, fvec* d_u, fvec* d_v)
{
  const Tindex end_lh = start_lh + n1_l;


  for (Tindex ilh = start_lh, i1 = 0; ilh < end_lh; ++ilh, ++i1) {
    Tindex i1_V      = i1 / fvecLen;
    Tindex i1_4      = i1 % fvecLen;
    L1HitPoint& hitl = StsHits_l[ilh];


#ifdef USE_EVENT_NUMBER
    Event_l[i1_V][i1_4] = hitl.track;
#endif

    HitTime_l[i1_V][i1_4] = hitl.time;
    HitTimeEr[i1_V][i1_4] = hitl.timeEr;

    hitsl[i1]             = ilh;
    u_front_l[i1_V][i1_4] = hitl.U();
    u_back_l[i1_V][i1_4]  = hitl.V();

    if (fUseHitErrors) {
      //      d_x[i1_V][i1_4]  = hitl.dX();
      //      d_y[i1_V][i1_4]  = hitl.dY();
      //      d_xy[i1_V][i1_4] = hitl.dXY();
      d_u[i1_V][i1_4] = hitl.dU();
      d_v[i1_V][i1_4] = hitl.dV();
    }

    zPos_l[i1_V][i1_4] = hitl.Z();
  }
}


/// Get the field approximation. Add the target to parameters estimation. Propagaete to the middle station of a triplet.
inline void L1Algo::f11(  /// input 1st stage of singlet search
  int istal,
  int istam,    /// indexes of left and middle stations of a triplet
  Tindex n1_V,  ///

  fvec* u_front_l, fvec* u_back_l, fvec* zPos_l, fvec* HitTime_l, fvec* HitTimeEr,
  // output
  //                 L1TrackPar *T_1,
  L1TrackPar* T_1, L1FieldRegion* fld_1,
  // comment unused parameters, FU, 18.01.21
  fvec* /*d_x*/, fvec* /*d_y*/, fvec* /*d_xy*/, fvec* d_u, fvec* d_v)
{
  L1Station& stal = vStations[istal];
  L1Station& stam = vStations[istam];
  fvec zstal      = stal.z;
  fvec zstam      = stam.z;

  int istal_global       = 5;
  int istam_global       = 6;
  L1Station& stal_global = vStations[istal_global];
  L1Station& stam_global = vStations[istam_global];
  fvec zstal_global      = stal_global.z;
  fvec zstam_global      = stam_global.z;

  L1FieldRegion
    fld0;  // by  left   hit, target and "center" (station in-between). Used here for extrapolation to target and to middle hit
  L1FieldValue l_B_global, centB, centB_global,
    l_B _fvecalignment;  // field for singlet creation
  L1FieldValue m_B,
    m_B_global _fvecalignment;  // field for the next extrapolations

  L1Fit fit;

  if ((isec != kAllPrimEIter) && (isec != kAllSecEIter)) {
    fit.SetParticleMass(fDefaultMass);  // muon
  }
  else {
    fit.SetParticleMass(0.000511f);  // electron
  }

  for (int i1_V = 0; i1_V < n1_V; i1_V++) {
    L1TrackPar& T = T_1[i1_V];
    L1FieldRegion& fld1 =
      fld_1[i1_V];  // by  middle hit, left hit and "center" . Will be used for extrapolation to right hit

    // get the magnetic field along the track.
    // (suppose track is straight line with origin in the target)
    fvec& u = u_front_l[i1_V];
    fvec& v = u_back_l[i1_V];
    fvec xl, yl;  // left(1-st) hit coor
    fvec zl         = zPos_l[i1_V];
    fvec& time      = HitTime_l[i1_V];
    fvec& timeEr    = HitTimeEr[i1_V];
    const fvec dzli = 1. / (zl - targZ);

    fvec dx1, dy1, dxy1 = 0;

    dUdV_to_dX(d_u[i1_V], d_v[i1_V], dx1, stal);
    dUdV_to_dY(d_u[i1_V], d_v[i1_V], dy1, stal);
    dUdV_to_dXdY(d_u[i1_V], d_v[i1_V], dxy1, stal);

    StripsToCoor(u, v, xl, yl, stal);

    const fvec tx = (xl - targX) * dzli;
    const fvec ty = (yl - targY) * dzli;

    // estimate field for singlet creation
    int istac = istal / 2;  // "center" station
    //     if (istal >= NMvdStations) // to make it in the same way for both with and w\o mvd
    //       istac = (istal-NMvdStations)/2+NMvdStations;
    L1Station& stac = vStations[istac];
    fvec zstac      = stac.z;

    int istac_global = istal_global / 2;  // "center" station

    L1Station& stac_global = vStations[istac_global];
    fvec zstac_global      = stac.z;

    stac.fieldSlice.GetFieldValue(targX + tx * (zstac - targZ), targY + ty * (zstac - targZ), centB);
    stal.fieldSlice.GetFieldValue(targX + tx * (zstal - targZ), targY + ty * (zstal - targZ), l_B);

    stam_global.fieldSlice.GetFieldValue(targX + tx * (zstam_global - targZ), targY + ty * (zstam_global - targZ),
                                         m_B_global);
    stal_global.fieldSlice.GetFieldValue(targX + tx * (zstal_global - targZ), targY + ty * (zstal_global - targZ),
                                         l_B_global);
    stac_global.fieldSlice.GetFieldValue(targX + tx * (zstac_global - targZ), targY + ty * (zstac_global - targZ),
                                         centB_global);

    if (istac != istal) fld0.Set(l_B, stal.z, centB, stac.z, targB, targZ);
    else
      fld0.Set(l_B, zstal, targB, targZ);
    // estimate field for the next extrapolations
    stam.fieldSlice.GetFieldValue(targX + tx * (zstam - targZ), targY + ty * (zstam - targZ), m_B);
#define USE_3HITS
#ifndef USE_3HITS
    if (istac != istal) fld1.Set(m_B, zstam, l_B, zstal, centB, zstac);
    else
      fld1.Set(m_B, zstam, l_B, zstal, targB, targZ);
#else  // if USE_3HITS  // the best now
    L1FieldValue r_B _fvecalignment;
    L1Station& star = vStations[istam + 1];
    fvec zstar      = star.z;
    star.fieldSlice.GetFieldValue(targX + tx * (zstar - targZ), targY + ty * (zstar - targZ), r_B);
    fld1.Set(r_B, zstar, m_B, zstam, l_B, zstal);
    if ((istam + 1) >= fNfieldStations)
      fld1.Set(m_B_global, zstam_global, l_B_global, zstal_global, centB_global, zstac_global);
#endif  // USE_3HITS

    T.chi2 = 0.;
    T.NDF  = 2.;
    if ((isec == kAllSecIter) || (isec == kAllSecEIter) || (isec == kAllSecJumpIter)) T.NDF = 0;
    T.tx = tx;
    T.ty = ty;
    T.t  = time;

    T.qp  = 0.;
    T.C20 = T.C21 = 0;
    T.C30 = T.C31 = T.C32 = 0;
    T.C40 = T.C41 = T.C42 = T.C43 = 0;
    T.C50 = T.C51 = T.C52 = T.C53 = T.C54 = 0;
    T.C22 = T.C33 = MaxSlopePV * MaxSlopePV / 9.;
    if (fTrackingMode == kGlobal || fTrackingMode == kMcbm) T.C22 = T.C33 = 10;
    T.C44 = MaxInvMom / 3. * MaxInvMom / 3.;
    T.C55 = timeEr * timeEr;


    // #define BEGIN_FROM_TARGET
#ifndef BEGIN_FROM_TARGET  // the best now

    T.x   = xl;
    T.y   = yl;
    T.z   = zl;
    T.C00 = stal.XYInfo.C00;
    T.C10 = stal.XYInfo.C10;
    T.C11 = stal.XYInfo.C11;

    if (fUseHitErrors) {
      T.C00 = dx1 * dx1;
      T.C10 = dxy1;
      T.C11 = dy1 * dy1;
    }


    if (fTrackingMode == kGlobal || fTrackingMode == kMcbm)
    //  add the target
    {

      if (istal < fNfieldStations) {
        fvec eX, eY, J04, J14;
        fvec dz;
        dz = targZ - zl;
        L1ExtrapolateJXY0(T.tx, T.ty, dz, fld0, eX, eY, J04, J14);
        fvec J[6];
        J[0] = dz;
        J[1] = 0;
        J[2] = J04;
        J[3] = 0;
        J[4] = dz;
        J[5] = J14;
        L1FilterVtx(T, targX, targY, TargetXYInfo, eX, eY, J);
      }
      else {
        L1ExtrapolateLine(T, targZ);
        L1FilterXY(T, targX, targY, TargetXYInfo);
      }
    }


    else

    //add the target
    {
      fvec eX, eY, J04, J14;
      fvec dz;
      dz = targZ - zl;
      L1ExtrapolateJXY0(T.tx, T.ty, dz, fld0, eX, eY, J04, J14);
      fvec J[6];
      J[0] = dz;
      J[1] = 0;
      J[2] = J04;
      J[3] = 0;
      J[4] = dz;
      J[5] = J14;
      L1FilterVtx(T, targX, targY, TargetXYInfo, eX, eY, J);
    }


#else  // TODO: doesn't work. Why?

    T.x = targX;
    T.y = targY;

    T.z   = targZ;
    T.C00 = TargetXYInfo.C00;
    T.C10 = TargetXYInfo.C10;
    T.C11 = TargetXYInfo.C11;

    if (fTrackingMode == kGlobal || fTrackingMode == kMcbm)  // extrapolate to left hit
    {
      if (istal <= fNfieldStations) L1Extrapolate0(T, zl, fld0);
      else
        L1ExtrapolateLine(T, zl);
    }
    else
      L1Extrapolate0(T, zl, fld0);


    for (int ista = 0; ista <= istal - 1; ista++) {
#ifdef USE_RL_TABLE
      fit.L1AddMaterial(T, fRadThick[ista].GetRadThick(T.x, T.y), MaxInvMom, 1);
#else
      fit.L1AddMaterial(T, vStations[ista].materialInfo, MaxInvMom, 1);
#endif
      if (ista == NMvdStations - 1) fit.L1AddPipeMaterial(T, MaxInvMom, 1);
    }

    // add left hit
    L1UMeasurementInfo info = stal.frontInfo;

    if (fUseHitErrors) info.sigma2 = du1 * du1;

    if (fTrackingMode == kGlobal || fTrackingMode == kMcbm) {
      if (istal < fNfieldStations) L1Filter(T, info, u);
      else
        L1FilterNoField(T, info, u);
    }
    else
      L1Filter(T, info, u);

    info = stal.backInfo;

    if (fUseHitErrors) info.sigma2 = dv1 * dv1;

    if (fTrackingMode == kGlobal || fTrackingMode == kMcbm) {
      if (istal < fNfieldStations) L1Filter(T, info, v);
      else
        L1FilterNoField(T, info, v);
    }
    else
      L1Filter(T, info, v);

#endif


#ifdef USE_RL_TABLE
    if (fTrackingMode != kMcbm) fit.L1AddMaterial(T, fRadThick[istal].GetRadThick(T.x, T.y), MaxInvMom, 1);
    if (fTrackingMode == kGlobal || fTrackingMode == kMcbm)
      fit.L1AddThickMaterial(T, fRadThick[istal].GetRadThick(T.x, T.y), MaxInvMom, 1, stal.materialInfo.thick, 1);
#else
    fit.L1AddMaterial(T, stal.materialInfo, MaxInvMom, 1);
#endif
    if ((istam >= NMvdStations) && (istal <= NMvdStations - 1)) { fit.L1AddPipeMaterial(T, MaxInvMom, 1); }

    fvec dz = zstam - zl;
    L1ExtrapolateTime(T, dz, stam.timeInfo);

    if (fTrackingMode == kGlobal || fTrackingMode == kMcbm)
    // extrapolate to left hit
    {
      if (istam < fNfieldStations) L1Extrapolate0(T, zstam, fld0);
      else
        L1ExtrapolateLine(T, zstam);  // TODO: fld1 doesn't work!
    }
    else
      L1Extrapolate0(T, zstam, fld0);  // TODO: fld1 doesn't work!

  }  // i1_V
}


/// Find the doublets. Reformat data in the portion of doublets.
inline void L1Algo::f20(
  /// input
  Tindex n1, L1Station& stal, L1Station& stam, L1HitPoint* vStsHits_m, L1TrackPar* T_1, THitI* hitsl_1,
  /// output
  Tindex& n2, L1Vector<THitI>& i1_2,
#ifdef DOUB_PERFORMANCE
  L1Vector<THitI>& hitsl_2,
#endif  // DOUB_PERFORMANCE
  L1Vector<THitI>& hitsm_2, fvec* /*Event*/, L1Vector<char>& lmDuplets)
{
  n2 = 0;                             // number of doublet
  for (Tindex i1 = 0; i1 < n1; ++i1)  // for each singlet
  {
    unsigned int Ndoublets = 0;
    const Tindex i1_V      = i1 / fvecLen;
    const Tindex i1_4      = i1 % fvecLen;
    L1TrackPar& T1         = T_1[i1_V];

    const int n2Saved = n2;

    const fvec Pick_m22 =
      (DOUBLET_CHI2_CUT - T1.chi2);  // if make it bigger the found hits will be rejected later because of the chi2 cut.
    // Pick_m22 is not used, search for mean squared, 2nd version

    // -- collect possible doublets --
    const fscal iz         = 1 / T1.z[i1_4];
    const float& timeError = T1.C55[i1_4];
    const float& time      = T1.t[i1_4];


    L1HitAreaTime areaTime(vGridTime[&stam - vStations], T1.x[i1_4] * iz, T1.y[i1_4] * iz,
                           (sqrt(Pick_m22 * (T1.C00 + stam.XYInfo.C00)) + MaxDZ * fabs(T1.tx))[i1_4] * iz,
                           (sqrt(Pick_m22 * (T1.C11 + stam.XYInfo.C11)) + MaxDZ * fabs(T1.ty))[i1_4] * iz, time,
                           sqrt(timeError) * 5);


    THitI imh = 0;
    int irm1  = -1;
    while (true) {
      if (fTrackingMode == kGlobal || fTrackingMode == kMcbm) {
        irm1++;
        if ((THitI) irm1 >= (StsHitsUnusedStopIndex[&stam - vStations] - StsHitsUnusedStartIndex[&stam - vStations]))
          break;
        imh = irm1;
      }
      else {
        if (!areaTime.GetNext(imh)) break;
      }
      const L1HitPoint& hitm = vStsHits_m[imh];


      // check y-boundaries
      if ((stam.timeInfo) && (stal.timeInfo))
        if (fabs(time - hitm.time) > sqrt(timeError + hitm.timeEr * hitm.timeEr) * 5) continue;
      if ((stam.timeInfo) && (stal.timeInfo))
        if (fabs(time - hitm.time) > 30) continue;

#ifdef USE_EVENT_NUMBER
      if ((Event[i1_V][i1_4] != hitm.n)) continue;
#endif
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

      if (fUseHitErrors) {
        fvec dym = 0;
        dUdV_to_dY(hitm.dU(), hitm.dV(), dym, stam);
        dy_est2 = Pick_m22[i1_4] * fabs(C11[i1_4] + dym[0] * dym[0]);
      }

      fvec xm, ym = 0;

      StripsToCoor(hitm.U(), hitm.V(), xm, ym, stam);

      const fscal dY = ym[i1_4] - y[i1_4];

      if (dY * dY > dy_est2) continue;

      // check x-boundaries
      fvec x, C00;
      L1ExtrapolateXC00Line(T1, zm, x, C00);

      fscal dx_est2 = Pick_m22[i1_4] * fabs(C00[i1_4] + stam.XYInfo.C00[i1_4]);

      if (fUseHitErrors) {
        fvec dxm = 0;
        dUdV_to_dX(hitm.dU(), hitm.dV(), dxm, stam);
        dx_est2 = Pick_m22[i1_4] * fabs(C00[i1_4] + dxm[0] * dxm[0]);
      }

      const fscal dX = xm[i1_4] - x[i1_4];

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
#endif
        if (chi2[i1_4] > DOUBLET_CHI2_CUT) continue;
          //       T1.t[i1_4] = hitm.time;

#ifdef USE_EVENT_NUMBER
      T1.n[i1_4] = hitm.n;
#endif

      info = stam.backInfo;

      if (fUseHitErrors) info.sigma2 = hitm.dV() * hitm.dV();

      L1FilterChi2(info, x, y, C00, C10, C11, chi2, hitm.V());

      // FilterTime(T1, hitm.time, hitm.timeEr);


#ifdef DO_NOT_SELECT_TRIPLETS
      if (isec != TRACKS_FROM_TRIPLETS_ITERATION)
#endif
        if (chi2[i1_4] > DOUBLET_CHI2_CUT) continue;

      i1_2.push_back(i1);
#ifdef DOUB_PERFORMANCE
      hitsl_2.push_back(hitsl_1[i1]);
#endif  // DOUB_PERFORMANCE
      hitsm_2.push_back(imh);

      Ndoublets++;
      n2++;

#ifndef FAST_CODE
//TODO: optimise triplet portion limit and put a warning
// assert(Ndoublets < fMaxDoubletsPerSinglet);
#endif

      if (Ndoublets >= fMaxDoubletsPerSinglet) {
        n2 = n2 - Ndoublets;
        i1_2.reduce(i1_2.size() - Ndoublets);
        hitsm_2.reduce(hitsm_2.size() - Ndoublets);
        break;
      }
    }
    lmDuplets[hitsl_1[i1]] = (n2Saved < n2);
  }  // for i1
}


/// Add the middle hits to parameters estimation. Propagate to right station.
/// Find the triplets(right hit). Reformat data in the portion of triplets.
inline void L1Algo::f30(  // input
  L1HitPoint* vStsHits_r, L1Station& stam, L1Station& star, int istam, int istar, L1HitPoint* vStsHits_m,
  L1TrackPar* T_1, L1FieldRegion* fld_1, THitI* hitsl_1, Tindex n2, L1Vector<THitI>& hitsm_2, L1Vector<THitI>& i1_2,
  const L1Vector<char>& /*mrDuplets*/,
  // output
  Tindex& n3, nsL1::vector<L1TrackPar>::TSimd& T_3, L1Vector<THitI>& hitsl_3, L1Vector<THitI>& hitsm_3,
  L1Vector<THitI>& hitsr_3, nsL1::vector<fvec>::TSimd& u_front_3, nsL1::vector<fvec>::TSimd& u_back_3,
  nsL1::vector<fvec>::TSimd& z_Pos_3,
  //  nsL1::vector<fvec>::TSimd& dx_,
  //  nsL1::vector<fvec>::TSimd& dy_,
  nsL1::vector<fvec>::TSimd& dv_, nsL1::vector<fvec>::TSimd& du_, nsL1::vector<fvec>::TSimd& timeR,
  nsL1::vector<fvec>::TSimd& timeER)
{
  THitI hitsl_2[fvecLen];
  THitI hitsm_2_tmp[fvecLen];
  fvec fvec_0;
  L1TrackPar L1TrackPar_0;

  L1Fit fit;
  fit.SetParticleMass(fDefaultMass);

  Tindex n3_V = 0, n3_4 = 0;

  T_3.push_back(L1TrackPar_0);
  u_front_3.push_back(fvec_0);
  u_back_3.push_back(fvec_0);
  z_Pos_3.push_back(fvec_0);
  //  dx_.push_back(fvec_0);
  //  dy_.push_back(fvec_0);
  du_.push_back(fvec_0);
  dv_.push_back(fvec_0);
  timeR.push_back(fvec_0);
  timeER.push_back(fvec_0);


  L1TrackPar T2;
  L1FieldRegion f2;
  // pack the data
  fvec u_front_2;
  fvec u_back_2;
  fvec dx2;
  fvec dy2;
  fvec du2;
  fvec dv2;
  fvec zPos_2;
  fvec timeM;
  fvec timeMEr;
  fvec num;


  // ---- Add the middle hits to parameters estimation. Propagate to right station. ----
  if (istar < NStations) {
    for (Tindex i2 = 0; i2 < n2;) {
      Tindex n2_4 = 0;
      for (; n2_4 < fvecLen && i2 < n2; n2_4++, i2++) {
        //         if (!mrDuplets[hitsm_2[i2]]) {
        //           n2_4--;
        //           continue;
        //         }
        const Tindex& i1  = i1_2[i2];
        const Tindex i1_V = i1 / fvecLen;
        const Tindex i1_4 = i1 % fvecLen;


        const L1TrackPar& T1    = T_1[i1_V];
        const L1FieldRegion& f1 = fld_1[i1_V];
        T2.SetOneEntry(n2_4, T1, i1_4);
        f2.SetOneEntry(n2_4, f1, i1_4);

        const Tindex& imh      = hitsm_2[i2];
        const L1HitPoint& hitm = vStsHits_m[imh];
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

      dUdV_to_dX(du2, dv2, dx2, stam);
      dUdV_to_dY(du2, dv2, dy2, stam);

      fvec dz = zPos_2 - T2.z;

      L1ExtrapolateTime(T2, dz, stam.timeInfo);
      // add middle hit
      L1ExtrapolateLine(T2, zPos_2);


      L1UMeasurementInfo info = stam.frontInfo;

      if (fUseHitErrors) info.sigma2 = du2 * du2;

      if (fTrackingMode == kGlobal || fTrackingMode == kMcbm) {
        if (istam < fNfieldStations) L1Filter(T2, info, u_front_2);
        else
          L1FilterNoField(T2, info, u_front_2);
      }
      else
        L1Filter(T2, info, u_front_2);


      info = stam.backInfo;
      if (fUseHitErrors) info.sigma2 = dv2 * dv2;

      if (fTrackingMode == kGlobal || fTrackingMode == kMcbm) {
        if (istam < fNfieldStations) L1Filter(T2, info, u_back_2);
        else
          L1FilterNoField(T2, info, u_back_2);
      }
      else
        L1Filter(T2, info, u_back_2);


      FilterTime(T2, timeM, timeMEr, stam.timeInfo);
#ifdef USE_RL_TABLE
      if (fTrackingMode != kMcbm) fit.L1AddMaterial(T2, fRadThick[istam].GetRadThick(T2.x, T2.y), T2.qp, 1);

      if (fTrackingMode == kGlobal || fTrackingMode == kMcbm)
        fit.L1AddThickMaterial(T2, fRadThick[istam].GetRadThick(T2.x, T2.y), MaxInvMom, 1, stam.materialInfo.thick, 1);
#else
      fit.L1AddMaterial(T2, stam.materialInfo, T2.qp, 1);
#endif
      if ((istar >= NMvdStations) && (istam <= NMvdStations - 1)) { fit.L1AddPipeMaterial(T2, T2.qp, 1); }

      fvec dz2 = star.z - T2.z;
      L1ExtrapolateTime(T2, dz2, stam.timeInfo);

      // extrapolate to the right hit station

      if (fTrackingMode == kGlobal || fTrackingMode == kMcbm)
      // extrapolate to the right hit station
      {
        if (istar <= fNfieldStations) L1Extrapolate(T2, star.z, T2.qp, f2);
        else
          L1ExtrapolateLine(T2, star.z);
      }
      else
        L1Extrapolate(T2, star.z, T2.qp, f2);

      // ---- Find the triplets(right hit). Reformat data in the portion of triplets. ----
      for (Tindex i2_4 = 0; i2_4 < n2_4; ++i2_4) {
        if (fTrackingMode == kSts && (T2.C44[i2_4] < 0)) { continue; }
        if (T2.C00[i2_4] < 0 || T2.C11[i2_4] < 0 || T2.C22[i2_4] < 0 || T2.C33[i2_4] < 0 || T2.C55[i2_4] < 0) continue;

        if (fabs(T2.tx[i2_4]) > MaxSlope) continue;
        if (fabs(T2.ty[i2_4]) > MaxSlope) continue;

        const fvec Pick_r22    = (TRIPLET_CHI2_CUT - T2.chi2);
        const float& timeError = T2.C55[i2_4];
        const float& time      = T2.t[i2_4];

        // find first possible hit

#ifdef DO_NOT_SELECT_TRIPLETS
        if (isec == TRACKS_FROM_TRIPLETS_ITERATION) Pick_r22 = Pick_r2 + 1;
#endif
        const fscal iz = 1 / T2.z[i2_4];
        L1HitAreaTime area(vGridTime[&star - vStations], T2.x[i2_4] * iz, T2.y[i2_4] * iz,
                           (sqrt(Pick_r22 * (T2.C00 + stam.XYInfo.C00)) + MaxDZ * fabs(T2.tx))[i2_4] * iz,
                           (sqrt(Pick_r22 * (T2.C11 + stam.XYInfo.C11)) + MaxDZ * fabs(T2.ty))[i2_4] * iz, time,
                           sqrt(timeError) * 5);

        THitI irh       = 0;
        THitI Ntriplets = 0;
        int irh1        = -1;
        while (true) {
          if (fTrackingMode == kGlobal || fTrackingMode == kMcbm) {
            irh1++;
            if ((THitI) irh1 >= (StsHitsUnusedStopIndex[istar] - StsHitsUnusedStartIndex[istar])) break;
            irh = irh1;
          }
          else {
            if (!area.GetNext(irh)) break;
          }


          // while (area.GetNext(irh)) {
          //for (int irh = 0; irh < ( StsHitsUnusedStopIndex[istar] - StsHitsUnusedStartIndex[istar] ); irh++){
          const L1HitPoint& hitr = vStsHits_r[irh];

#ifdef USE_EVENT_NUMBER
          if ((T2.n[i2_4] != hitr.n)) continue;
#endif
          const fscal zr = hitr.Z();
          //  const fscal yr = hitr.Y();

          fvec xr, yr = 0;

          StripsToCoor(hitr.U(), hitr.V(), xr, yr, star);

          L1TrackPar T_cur = T2;


          fvec dz3 = zr - T_cur.z;
          L1ExtrapolateTime(T_cur, dz3, star.timeInfo);

          L1ExtrapolateLine(T_cur, zr);

          if ((star.timeInfo) && (stam.timeInfo))
            if (fabs(T_cur.t[i2_4] - hitr.time) > sqrt(T_cur.C55[i2_4] + hitr.timeEr) * 5) continue;
          if ((star.timeInfo) && (stam.timeInfo))
            if (fabs(T_cur.t[i2_4] - hitr.time) > 30) continue;


          // - check whether hit belong to the window ( track position +\- errors ) -
          // check lower boundary
          fvec y, C11;
          L1ExtrapolateYC11Line(T2, zr, y, C11);

          fscal dy_est2 =
            (Pick_r22[i2_4]
             * (fabs(
               C11[i2_4]
               + star.XYInfo.C11[i2_4])));  // TODO for FastPrim dx < dy - other sort is optimal. But not for doublets

          if (fUseHitErrors) {
            fvec dyr = 0;
            dUdV_to_dY(hitr.dU(), hitr.dV(), dyr, star);
            dy_est2 = (Pick_r22[i2_4] * (fabs(C11[i2_4] + dyr[0] * dyr[0])));
          }

          const fscal dY  = yr[i2_4] - y[i2_4];
          const fscal dY2 = dY * dY;
          if (dY2 > dy_est2 && dY2 < 0) continue;  // if (yr < y_minus_new[i2_4]) continue;

          // check upper boundary
          if (dY2 > dy_est2) continue;  // if (yr > y_plus_new [i2_4] ) continue;
          // check x-boundaries
          fvec x, C00;

          L1ExtrapolateXC00Line(T2, zr, x, C00);


          fscal dx_est2 = (Pick_r22[i2_4] * (fabs(C00[i2_4] + star.XYInfo.C00[i2_4])));

          if (fUseHitErrors) {
            fvec dxr = 0;
            dUdV_to_dX(hitr.dU(), hitr.dV(), dxr, star);
            dx_est2 = (Pick_r22[i2_4] * (fabs(C00[i2_4] + dxr[0] * dxr[0])));
          }

          const fscal dX = xr[i2_4] - x[i2_4];

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
#endif

            if (chi2[i2_4] > TRIPLET_CHI2_CUT || C00[i2_4] < 0 || C11[i2_4] < 0 || T_cur.C55[i2_4] < 0)
              continue;  // chi2_triplet < CHI2_CUT

          // pack triplet
          L1TrackPar& T3 = T_3[n3_V];

          hitsl_3.push_back(hitsl_2[i2_4]);
          hitsm_3.push_back(hitsm_2_tmp[i2_4]);
          hitsr_3.push_back(irh);


          T3.SetOneEntry(n3_4, T2, i2_4);
          u_front_3[n3_V][n3_4] = hitr.U();
          u_back_3[n3_V][n3_4]  = hitr.V();
          //dx_[n3_V][n3_4]       = hitr.dX();
          // dy_[n3_V][n3_4]       = hitr.dY();
          du_[n3_V][n3_4]     = hitr.dU();
          dv_[n3_V][n3_4]     = hitr.dV();
          z_Pos_3[n3_V][n3_4] = zr;
          timeR[n3_V][n3_4]   = hitr.time;
          timeER[n3_V][n3_4]  = hitr.timeEr;

          n3++;
          Ntriplets++;

          n3_V = n3 / fvecLen;
          n3_4 = n3 % fvecLen;

          if (!n3_4) {
            T_3.push_back(L1TrackPar_0);
            u_front_3.push_back(fvec_0);
            u_back_3.push_back(fvec_0);
            z_Pos_3.push_back(fvec_0);
            //            dx_.push_back(fvec_0);
            //            dy_.push_back(fvec_0);
            du_.push_back(fvec_0);
            dv_.push_back(fvec_0);
            timeR.push_back(fvec_0);
            timeER.push_back(fvec_0);
          }

#ifndef FAST_CODE
//TODO: optimise triplet portion limit and put a warning
// assert(Ntriplets < fMaxTripletPerDoublets);
#endif
          if (Ntriplets >= fMaxTripletPerDoublets) {

            n3 = n3 - Ntriplets;

            T_3.resize(n3 / fvecLen);
            u_front_3.resize(n3 / fvecLen);
            u_back_3.resize(n3 / fvecLen);
            z_Pos_3.resize(n3 / fvecLen);
            du_.resize(n3 / fvecLen);
            dv_.resize(n3 / fvecLen);
            timeR.resize(n3 / fvecLen);
            timeER.resize(n3 / fvecLen);

            break;
          }
        }


      }  // i2_4
    }    // i2_V
  }      // if istar
}

/// Add the right hits to parameters estimation.
inline void L1Algo::f31(  // input
  Tindex n3_V, L1Station& star, nsL1::vector<fvec>::TSimd& u_front_, nsL1::vector<fvec>::TSimd& u_back_,
  nsL1::vector<fvec>::TSimd& z_Pos,
  //  nsL1::vector<fvec>::TSimd& dx_,
  //  nsL1::vector<fvec>::TSimd& dy_,
  nsL1::vector<fvec>::TSimd& dv_, nsL1::vector<fvec>::TSimd& du_, nsL1::vector<fvec>::TSimd& timeR,
  nsL1::vector<fvec>::TSimd& timeER,
  // output
  //                L1TrackPar *T_3
  nsL1::vector<L1TrackPar>::TSimd& T_3)
{
  for (Tindex i3_V = 0; i3_V < n3_V; ++i3_V) {

    fvec dz = z_Pos[i3_V] - T_3[i3_V].z;

    L1ExtrapolateTime(T_3[i3_V], dz, star.timeInfo);
    L1ExtrapolateLine(T_3[i3_V], z_Pos[i3_V]);


    L1UMeasurementInfo info = star.frontInfo;

    if (fUseHitErrors) info.sigma2 = du_[i3_V] * du_[i3_V];

    if (fTrackingMode == kGlobal || fTrackingMode == kMcbm) {
      if ((&star - vStations) < fNfieldStations) L1Filter(T_3[i3_V], info, u_front_[i3_V]);
      else {
        L1FilterNoField(T_3[i3_V], info, u_front_[i3_V]);
      }
    }
    else
      L1Filter(T_3[i3_V], info, u_front_[i3_V]);


    info = star.backInfo;

    if (fUseHitErrors) info.sigma2 = dv_[i3_V] * dv_[i3_V];

    if (fTrackingMode == kGlobal || fTrackingMode == kMcbm) {
      if ((&star - vStations) < fNfieldStations) L1Filter(T_3[i3_V], info, u_back_[i3_V]);
      else
        L1FilterNoField(T_3[i3_V], info, u_back_[i3_V]);
    }
    else
      L1Filter(T_3[i3_V], info, u_back_[i3_V]);

    if (fTrackingMode != kMcbm) FilterTime(T_3[i3_V], timeR[i3_V], timeER[i3_V], star.timeInfo);

    //  FilterTime(T_3[i3_V], timeR[i3_V], timeER[i3_V]);
  }
}


/// Refit Triplets.
inline void L1Algo::f32(  // input // TODO not updated after gaps introduction
  Tindex n3, int istal, nsL1::vector<L1TrackPar>::TSimd& T_3, L1Vector<THitI>& hitsl_3, L1Vector<THitI>& hitsm_3,
  L1Vector<THitI>& hitsr_3, int nIterations)
{
  L1Fit fit;
  fit.SetParticleMass(fDefaultMass);

  const int NHits = 3;  // triplets

  // prepare data
  int ista[NHits] = {istal, istal + 1, istal + 2};

  L1Station sta[3];
  for (int is = 0; is < NHits; ++is) {
    sta[is] = vStations[ista[is]];
  };

  for (int i3 = 0; i3 < n3; ++i3) {
    int i3_V = i3 / fvecLen;
    int i3_4 = i3 % fvecLen;

    L1TrackPar& T3 = T_3[i3_V];
    fscal& qp0     = T3.qp[i3_4];

    // prepare data
    THitI ihit[NHits] = {(*RealIHitP)[hitsl_3[i3] + StsHitsUnusedStartIndex[ista[0]]],
                         (*RealIHitP)[hitsm_3[i3] + StsHitsUnusedStartIndex[ista[1]]],
                         (*RealIHitP)[hitsr_3[i3] + StsHitsUnusedStartIndex[ista[2]]]};

    fscal u[NHits], v[NHits], x[NHits], y[NHits], z[NHits];
    for (int ih = 0; ih < NHits; ++ih) {
      const L1Hit& hit = (*vStsHits)[ihit[ih]];
      u[ih]            = hit.u;
      v[ih]            = hit.v;
      StripsToCoor(u[ih], v[ih], x[ih], y[ih], sta[ih]);
      z[ih] = hit.z;
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
#ifdef USE_RL_TABLE
      fit.L1AddMaterial(T, fRadThick[ista[ih]].GetRadThick(T.x, T.y), T.qp, 1);
#else
      fit.L1AddMaterial(T, sta[ih].materialInfo, T.qp, 1);
#endif
      if (ista[ih] == NMvdStations - 1) fit.L1AddPipeMaterial(T, T.qp, 1);

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
#ifdef USE_RL_TABLE
        fit.L1AddMaterial(T, fRadThick[ista[ih]].GetRadThick(T.x, T.y), T.qp, 1);
#else
        fit.L1AddMaterial(T, sta[ih].materialInfo, T.qp, 1);
#endif
        if (ista[ih] == NMvdStations) fit.L1AddPipeMaterial(T, T.qp, 1);

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
#ifdef USE_RL_TABLE
        fit.L1AddMaterial(T, fRadThick[ista[ih]].GetRadThick(T.x, T.y), T.qp, 1);
#else
        fit.L1AddMaterial(T, sta[ih].materialInfo, T.qp, 1);
#endif
        if (ista[ih] == NMvdStations + 1) fit.L1AddPipeMaterial(T, T.qp, 1);

        L1Filter(T, sta[ih].frontInfo, u[ih]);
        L1Filter(T, sta[ih].backInfo, v[ih]);
      }
    }  // for iiter

    T3.SetOneEntry(i3_4, T, i3_4);
  }  //i3
}  // f32


inline void L1Algo::f4(  // input
  Tindex n3, int istal, int istam, int istar, nsL1::vector<L1TrackPar>::TSimd& T_3, L1Vector<THitI>& hitsl_3,
  L1Vector<THitI>& hitsm_3, L1Vector<THitI>& hitsr_3,
  // output
  Tindex& nstaltriplets, L1Vector<THitI>* /*hitsn_3*/, L1Vector<THitI>* /*hitsr_5*/
)
{

  /// Select good triplets and save them into fTriplets. Find neighbouring triplets at the next station.

#ifdef _OPENMP
  unsigned int Thread = omp_get_thread_num();
#else
  unsigned int Thread = 0;
#endif

  THitI ihitl_priv = 0;

  for (Tindex i3 = 0; i3 < n3; ++i3) {
    const Tindex i3_V = i3 / fvecLen;
    const Tindex i3_4 = i3 % fvecLen;

    L1TrackPar& T3 = T_3[i3_V];

    // select
    fscal& chi2 = T3.chi2[i3_4];

    const THitI ihitl = hitsl_3[i3] + StsHitsUnusedStartIndex[istal];
    const THitI ihitm = hitsm_3[i3] + StsHitsUnusedStartIndex[istam];
    const THitI ihitr = hitsr_3[i3] + StsHitsUnusedStartIndex[istar];
    L1_ASSERT(ihitl < StsHitsUnusedStopIndex[istal], ihitl << " < " << StsHitsUnusedStopIndex[istal]);
    L1_ASSERT(ihitm < StsHitsUnusedStopIndex[istam], ihitm << " < " << StsHitsUnusedStopIndex[istam]);
    L1_ASSERT(ihitr < StsHitsUnusedStopIndex[istar], ihitr << " < " << StsHitsUnusedStopIndex[istar]);

    unsigned int Location = PackTripletId(istal, Thread, fTriplets[istal][Thread].size());

    if (ihitl_priv == 0 || ihitl_priv != hitsl_3[i3]) {
      TripForHit[0][ihitl] = Location;
      TripForHit[1][ihitl] = Location;
    }

    ihitl_priv = hitsl_3[i3];

#ifdef DO_NOT_SELECT_TRIPLETS
    if (isec != TRACKS_FROM_TRIPLETS_ITERATION)
#endif
      if (!finite(chi2) || chi2 < 0 || chi2 > TRIPLET_CHI2_CUT) continue;

    fscal qp = T3.qp[i3_4];

    //TODO: why sqrt's? Wouldn't it be faster to skip sqrt() here and
    //TODO: compare the squared differences dqr*dqp later?

    fscal Cqp = sqrt(fabs(T3.C44[i3_4]));

    {  // legacy

      // TODO: SG: The magic cuts below are the rest from an old conversion of the momentum to char.
      // TODO: They came from the truncation to the 0-255 range and had no other meaning.
      // TODO: But for some reason, the efficiency degrades without them.
      // TODO: It needs to be investigated. If the cuts are necessary, they need to be adjusted.

      fscal Cmax = 0.04 * MaxInvMom[0];  // minimal momentum: 0.05 - 0.1
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

    ++nstaltriplets;

    TripForHit[1][ihitl] = Location + 1;

    if (istal > (NStations - 4)) continue;

    unsigned int nNeighbours = TripForHit[1][ihitm] - TripForHit[0][ihitm];

    unsigned int neighLocation = TripForHit[0][ihitm];
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

    for (int istal = NStations - 4; istal >= FIRSTCASTATION; istal--) {
      for (int tripType = 0; tripType < 3; tripType++)  // tT = 0 - 123triplet, tT = 1 - 124triplet, tT = 2 - 134triplet
      {
        if ((((isec != kFastPrimJumpIter) && (isec != kAllPrimJumpIter) && (isec != kAllSecJumpIter))
             && (tripType != 0))
            || (((isec == kFastPrimJumpIter) || (isec == kAllPrimJumpIter) || (isec == kAllSecJumpIter))
                && (tripType != 0) && (istal == NStations - 4)))
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
            //           float  chi2 = trip->GetChi2();

            THitI ihitl = trip.GetLHit();
            THitI ihitm = trip.GetMHit();
            THitI ihitr = trip.GetRHit();
            L1_ASSERT(ihitl < StsHitsUnusedStopIndex[istal], ihitl << " < " << StsHitsUnusedStopIndex[istal]);
            L1_ASSERT(ihitm < StsHitsUnusedStopIndex[istam], ihitm << " < " << StsHitsUnusedStopIndex[istam]);
            L1_ASSERT(ihitr < StsHitsUnusedStopIndex[istar], ihitr << " < " << StsHitsUnusedStopIndex[istar]);

            L1Vector<unsigned int> neighCands("L1CATrackFinder::neighCands");  // save neighbour candidates
            neighCands.reserve(8);                                             // ~average is 1-2 for central, up to 5

            unsigned int nNeighbours = TripForHit[1][ihitm] - TripForHit[0][ihitm];

            unsigned int neighLocation = TripForHit[0][ihitm];
            unsigned int neighStation  = TripletId2Station(neighLocation);
            unsigned int neighThread   = TripletId2Thread(neighLocation);
            unsigned int neighTriplet  = TripletId2Triplet(neighLocation);

            for (unsigned int iN = 0; iN < nNeighbours; ++iN, ++neighTriplet, ++neighLocation) {
              //    for (iN = first_triplet; iN <= last_triplet; ++iN){
              L1Triplet& neigh = fTriplets[neighStation][neighThread][neighTriplet];
              //      if (neigh.GetMSta() != istar) continue; // neighbours should have 2 common hits
              //      if (neigh.GetMHit() != ihitr) continue; //!!!

              if (fabs(trip.GetQp() - neigh.GetQp()) > PickNeighbour * (trip.GetCqp() + neigh.GetCqp()))
                continue;  // neighbours should have same qp

              // calculate level
              unsigned char jlevel = neigh.GetLevel();
              if (level <= jlevel) level = jlevel + 1;
              if (level == jlevel + 1) neighCands.push_back(neighLocation);
            }

            // trip->neighbours.resize(0);
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
  L1TrackPar* T_1, L1FieldRegion* fld_1, THitI* hitsl_1, L1Vector<char>& lmDuplets, Tindex& n_2, L1Vector<THitI>& i1_2,
  L1Vector<THitI>& hitsm_2
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

  if (istam < NStations) {
    L1Station& stal = vStations[istal];
    L1Station& stam = vStations[istam];

    // prepare data
    L1HitPoint* vStsHits_l = &((*vStsHitPointsUnused)[0]) + StsHitsUnusedStartIndex[istal];
    L1HitPoint* vStsHits_m = &((*vStsHitPointsUnused)[0]) + StsHitsUnusedStartIndex[istam];

    fvec u_front[Portion / fvecLen], u_back[Portion / fvecLen];
    fvec dx0[Portion / fvecLen], dy0[Portion / fvecLen], dxy0[Portion / fvecLen];
    fvec dv0[Portion / fvecLen], du0[Portion / fvecLen];
    fvec zPos[Portion / fvecLen];
    fvec HitTime[Portion / fvecLen];
    fvec HitTimeEr[Portion / fvecLen];
    fvec Event[Portion / fvecLen];

    /// prepare the portion of left hits data
    Tindex& n1 = n_g[ip];

    f10(  // input
      (ip - portionStopIndex_[istal + 1]) * Portion, n1, vStsHits_l,
      // output
      u_front, u_back, zPos, hitsl_1, HitTime, HitTimeEr, Event, dx0, dy0, dxy0, du0, dv0);

    for (Tindex i = 0; i < n1; ++i)
      L1_ASSERT(hitsl_1[i] < StsHitsUnusedStopIndex[istal] - StsHitsUnusedStartIndex[istal],
                hitsl_1[i] << " < " << StsHitsUnusedStopIndex[istal] - StsHitsUnusedStartIndex[istal]);

    Tindex n1_V = (n1 + fvecLen - 1) / fvecLen;

    /// Get the field approximation. Add the target to parameters estimation. Propagaete to middle station.

    f11(istal, istam, n1_V,

        u_front, u_back, zPos, HitTime, HitTimeEr,
        // output
        T_1, fld_1, dx0, dy0, dxy0, du0, dv0);

    /// Find the doublets. Reformat data in the portion of doublets.

#ifdef DOUB_PERFORMANCE
    L1Vector<THitI> hitsl_2("L1CATrackFinder::hitsl_2");
#endif  // DOUB_PERFORMANCE

    f20(  // input
      n1, stal, stam, vStsHits_m, T_1, hitsl_1,
      // output
      n_2, i1_2,
#ifdef DOUB_PERFORMANCE
      hitsl_2,
#endif  // DOUB_PERFORMANCE
      hitsm_2, Event, lmDuplets);

    for (Tindex i = 0; i < static_cast<Tindex>(hitsm_2.size()); ++i)
      L1_ASSERT(hitsm_2[i] < StsHitsUnusedStopIndex[istam] - StsHitsUnusedStartIndex[istam],
                hitsm_2[i] << " " << StsHitsUnusedStopIndex[istam] - StsHitsUnusedStartIndex[istam]);

#ifdef DOUB_PERFORMANCE
    THitI* RealIHitL = &((*RealIHitP)[StsHitsUnusedStartIndex[istal]]);
    THitI* RealIHitM = &((*RealIHitP)[StsHitsUnusedStartIndex[istam]]);
    for (Tindex i = 0; i < n2; ++i) {
      // int i_4 = i%4;
      // int i_V = i/4;
      THitI iHits[2] = {RealIHitL[hitsl_2[i]], RealIHitM[hitsm_2[i]]};
      fL1Eff_doublets->AddOne(iHits);
    }
#endif  // DOUB_PERFORMANCE
  }
}


/// ------------------- Triplets on station ----------------------

inline void
L1Algo::TripletsStaPort(  /// creates triplets: input: @istal - start station number, @istam - middle station number, @istar - last station number, @ip - index of portion, @&n_g - numer of elements in portion, @*portionStopIndex
  int istal, int istam, int istar,

  ///@nstaltriplets - , @*portionStopIndex, @*T_1 - track parameters for singlets, @*fld_1 - field approximation for singlets, @&n_2 - number of doublets in portion
  ///  @&n_2 - number of douplets,@&i1_2 - index of 1st hit in portion indexed by doublet index, @&hitsm_2 - index of middle hit in hits array indexed by doublet index


  Tindex& nstaltriplets, L1TrackPar* T_1, L1FieldRegion* fld_1, THitI* hitsl_1,

  Tindex& n_2, L1Vector<THitI>& i1_2, L1Vector<THitI>& hitsm_2,

  const L1Vector<char>& mrDuplets

  /// output: @*vTriplets_part - array of triplets, @*TripStartIndexH, @*TripStopIndexH - start/stop index of a triplet in the array

)
{
  if (istar < NStations) {
    // prepare data
    L1Station& stam = vStations[istam];
    L1Station& star = vStations[istar];

    L1HitPoint* vStsHits_m = &((*vStsHitPointsUnused)[0]) + StsHitsUnusedStartIndex[istam];

    L1HitPoint* vStsHits_r = 0;
    vStsHits_r             = &((*vStsHitPointsUnused)[0]) + StsHitsUnusedStartIndex[istar];

    Tindex n3 = 0, n3_V;

    /// Add the middle hits to parameters estimation. Propagate to right station.


#ifdef _OPENMP
    int Thread = omp_get_thread_num();
#else
    int Thread = 0;
#endif

    nsL1::vector<L1TrackPar>::TSimd& T_3 = fT_3[Thread];
    L1Vector<THitI>& hitsl_3             = fhitsl_3[Thread];
    L1Vector<THitI>& hitsm_3             = fhitsm_3[Thread];
    L1Vector<THitI>& hitsr_3             = fhitsr_3[Thread];
    nsL1::vector<fvec>::TSimd& u_front3  = fu_front3[Thread];
    nsL1::vector<fvec>::TSimd& u_back3   = fu_back3[Thread];
    nsL1::vector<fvec>::TSimd& z_pos3    = fz_pos3[Thread];
    nsL1::vector<fvec>::TSimd& timeR     = fTimeR[Thread];
    nsL1::vector<fvec>::TSimd& timeER    = fTimeER[Thread];
    //    nsL1::vector<fvec>::TSimd& dx3       = dx[Thread];
    //    nsL1::vector<fvec>::TSimd& dy3       = dy[Thread];
    nsL1::vector<fvec>::TSimd& du3 = du[Thread];
    nsL1::vector<fvec>::TSimd& dv3 = dv[Thread];

    T_3.clear();
    hitsl_3.clear();
    hitsm_3.clear();
    hitsr_3.clear();
    u_front3.clear();
    u_back3.clear();
    z_pos3.clear();
    //    dx3.clear();
    //    dy3.clear();
    du3.clear();
    dv3.clear();
    timeR.clear();
    timeER.clear();

    /// Find the triplets(right hit). Reformat data in the portion of triplets.


    f30(  // input
      vStsHits_r, stam, star,

      istam, istar, vStsHits_m, T_1, fld_1, hitsl_1,

      n_2, hitsm_2, i1_2,

      mrDuplets,
      // output
      n3, T_3, hitsl_3, hitsm_3, hitsr_3, u_front3, u_back3, z_pos3,
      //      dx3,
      //      dy3,
      du3, dv3, timeR, timeER);


    n3_V = (n3 + fvecLen - 1) / fvecLen;

    for (Tindex i = 0; i < static_cast<Tindex>(hitsl_3.size()); ++i)
      L1_assert(hitsl_3[i] < StsHitsUnusedStopIndex[istal] - StsHitsUnusedStartIndex[istal]);
    for (Tindex i = 0; i < static_cast<Tindex>(hitsm_3.size()); ++i)
      L1_assert(hitsm_3[i] < StsHitsUnusedStopIndex[istam] - StsHitsUnusedStartIndex[istam]);
    for (Tindex i = 0; i < static_cast<Tindex>(hitsr_3.size()); ++i)
      L1_assert(hitsr_3[i] < StsHitsUnusedStopIndex[istar] - StsHitsUnusedStartIndex[istar]);

    //        if (n3 >= MaxPortionTriplets) cout << "isec: " << isec << " station: " << istal << " portion number: " << ip << " CATrackFinder: Warning: Too many Triplets created in portion" << endl;

    /// Add the right hits to parameters estimation.
    f31(  // input
      n3_V, star, u_front3, u_back3, z_pos3,
      //      dx3,
      //      dy3,
      du3, dv3, timeR, timeER,
      // output
      T_3);


    /// refit
    //         f32(  n3, istal, _RealIHit,          T_3,         hitsl_3, hitsm_3, hitsr_3, 0 );

#ifdef TRIP_PERFORMANCE
    THitI* RealIHitL = &((*RealIHitP)[StsHitsUnusedStartIndex[istal]]);
    THitI* RealIHitM = &((*RealIHitP)[StsHitsUnusedStartIndex[istam]]);
    THitI* RealIHitR = &((*RealIHitP)[StsHitsUnusedStartIndex[istar]]);
    for (Tindex i = 0; i < n3; ++i) {
      Tindex i_4     = i % 4;
      Tindex i_V     = i / 4;
      THitI iHits[3] = {RealIHitL[hitsl_3[i]], RealIHitM[hitsm_3[i]], RealIHitR[hitsr_3[i]]};
#ifdef PULLS
      if (fL1Eff_triplets->AddOne(iHits)) fL1Pulls->AddOne(T_3[i_V], i_4, iHits[2]);
#else
      fL1Eff_triplets->AddOne(iHits);
#endif
      if (T_3[i_V].chi2[i_4] < TRIPLET_CHI2_CUT) fL1Eff_triplets2->AddOne(iHits);
    }
#endif  // TRIP_PERFORMANCE


    /// Fill Triplets.
    f4(  // input
      n3, istal, istam, istar, T_3, hitsl_3, hitsm_3, hitsr_3,
      // output
      nstaltriplets

    );
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
///**********************************************************************************************************************
///**********************************************************************************************************************
///**********************************************************************************************************************
///*                                                                                                                    *
///*                                                                                                                    *
///*                                                                                                                    *
///*                                          CATrackFinder procedure                                                   *
///*                                                                                                                    *
///*                                                                                                                    *
///*                                                                                                                    *
///**********************************************************************************************************************
///**********************************************************************************************************************
///**********************************************************************************************************************


void L1Algo::CATrackFinder()
{

#ifdef _OPENMP
  omp_set_num_threads(fNThreads);
#endif


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


#endif

#ifdef COUNTERS
  static Tindex stat_nStartHits              = 0;
  static Tindex stat_nHits[fNFindIterations] = {0};

  static Tindex stat_nSinglets[fNFindIterations] = {0};
  //  static Tindex stat_nDoublets[fNFindIterations] = {0};
  static Tindex stat_nTriplets[fNFindIterations] = {0};

  static Tindex stat_nLevels[fkMaxNstations - 2][fNFindIterations] = {{0}};
  static Tindex stat_nCalls[fNFindIterations]                      = {0};  // n calls of CAFindTrack
  static Tindex stat_nTrCandidates[fNFindIterations]               = {0};
#endif

  RealIHitP                           = &RealIHit_v;
  RealIHitPBuf                        = &RealIHit_v_buf;
  vStsHitsUnused                      = &vStsDontUsedHits_B;  /// array of hits used on current iteration
  L1Vector<L1Hit>* vStsHitsUnused_buf = &vStsDontUsedHits_A;  /// buffer for copy

  vStsHitPointsUnused = &vStsDontUsedHitsxy_B;  /// array of info for hits used on current iteration
  L1Vector<L1HitPoint>* vStsHitPointsUnused_buf = &vStsDontUsedHitsxy_A;

  NHitsIsecAll = 0;
  fTracks.clear();
  fRecoHits.clear();

  fRecoHits.reserve(2 * vStsHits->size());
  fTracks.reserve(2 * vStsHits->size() / NStations);

  int nDontUsedHits = 0;

  // #pragma omp parallel for  reduction(+:nDontUsedHits)
  for (int ista = 0; ista < NStations; ++ista) {
    nDontUsedHits += (StsHitsStopIndex[ista] - StsHitsStartIndex[ista]);
    StsHitsUnusedStartIndex[ista] = StsHitsStartIndex[ista];
    StsHitsUnusedStopIndex[ista]  = StsHitsStopIndex[ista];
  }

  float lasttime  = 0;
  float starttime = std::numeric_limits<float>::max();
  ;

  for (int ist = 0; ist < NStations; ++ist)
    for (THitI ih = StsHitsStartIndex[ist]; ih < StsHitsStopIndex[ist]; ++ih) {

      const float& time = (*vStsHits)[ih].t;
      if ((lasttime < time) && (!std::isinf(time))) lasttime = time;
      if ((starttime > time) && (time > 0)) starttime = time;
    }


#ifdef XXX
  c_time.Start();
  c_timerG.Start();

#endif

  float yStep = 1.5 / sqrt(nDontUsedHits);  // empirics. 0.01*sqrt(2374) ~= 0.5


  //  float yStep = 0.5 / sqrt(nDontUsedHits);  // empirics. 0.01*sqrt(2374) ~= 0.5
  if (yStep > 0.3) yStep = 0.3;
  float xStep = yStep * 3;
  // float xStep = yStep * 3;

  //  yStep = 0.0078;
  //  const float hitDensity = sqrt( nDontUsedHits );

  //     float yStep = 0.7*4/hitDensity; // empirics. 0.01*sqrt(2374) ~= 0.5
  //     if (yStep > 0.3)
  //      yStep = 1.25;
  //      xStep = 2.05;

  vStsHitsUnused = &vStsDontUsedHits_Buf;


  for (int iS = 0; iS < NStations; ++iS) {

    vGridTime[iS].BuildBins(-1, 1, -0.6, 0.6, starttime, lasttime, xStep, yStep, (lasttime - starttime + 1));
    int start = StsHitsUnusedStartIndex[iS];
    int nhits = StsHitsUnusedStopIndex[iS] - start;
    if (nhits > 0) {
      vGridTime[iS].StoreHits(nhits, &((*vStsHits)[start]), iS, *this, start, &(vStsDontUsedHits_Buf[start]),
                              &((*vStsHits)[start]), &(RealIHit_v[start]));
    }
    else {  // to avoid out-of-range crash in array[start]
      vGridTime[iS].StoreHits(nhits, nullptr, iS, *this, start, nullptr, nullptr, nullptr);
    }
  }


  for (int ist = 0; ist < NStations; ++ist)
    for (THitI ih = StsHitsStartIndex[ist]; ih < StsHitsStopIndex[ist]; ++ih) {
      L1Hit& h = (*vStsHits)[ih];
      SetFUnUsed((*fStripFlag)[h.f]);
      SetFUnUsed((*fStripFlag)[h.b]);
    }

  for (int ista = 0; ista < NStations; ++ista) {


#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic, 5)
#endif
    for (THitI ih = StsHitsStartIndex[ista]; ih < StsHitsStopIndex[ista]; ++ih) {
      CreateHitPoint(vStsDontUsedHits_Buf[ih], vStsDontUsedHitsxy_B[ih]);
    }
  }

#ifdef COUNTERS
  stat_nStartHits += nDontUsedHits;
#endif

#ifdef XXX
  c_timerG.Stop();
  gti["init  "] = c_timerG;
  c_timerG.Start();
#endif

  TStopwatch c_time1;
  c_time1.Start();

  for (isec = 0; isec < fNFindIterations; ++isec)  // all finder
  {
    if (fTrackingMode == kMcbm) {
      if (isec > 1) { continue; }
    }
    // n_g1.assign(n_g1.size(), Portion);

    for (int n = 0; n < fNThreads; n++) {
      for (int j = 0; j < NStations; j++) {
        fTriplets[j][n].clear();
      }
    }
    /// isec - number of current iterations, fNFindIterations - number of all iterations
#ifdef COUNTERS
    Tindex nSinglets = 0;
#endif

    if (isec != 0) {
      L1Vector<THitI>* RealIHitPTmp = RealIHitP;
      RealIHitP                     = RealIHitPBuf;
      RealIHitPBuf                  = RealIHitPTmp;

      L1Vector<L1Hit>* vStsHitsUnused_temp = vStsHitsUnused;
      vStsHitsUnused                       = vStsHitsUnused_buf;
      vStsHitsUnused_buf                   = vStsHitsUnused_temp;

      L1Vector<L1HitPoint>* vStsHitsUnused_temp2 = vStsHitPointsUnused;
      vStsHitPointsUnused                        = vStsHitPointsUnused_buf;
      vStsHitPointsUnused_buf                    = vStsHitsUnused_temp2;
    }

    for (int ist = 0; ist < NStations; ++ist) {
      for (THitI ih = StsHitsUnusedStartIndex[ist]; ih < StsHitsUnusedStopIndex[ist]; ++ih) {
        //SG!!
        TripForHit[0][ih] = 0;
        TripForHit[1][ih] = 0;
      }
    }
    /*
   TripForHit[0].assign(StsHitsUnusedStopIndex[NStations-1],0);
   TripForHit[1].assign(StsHitsUnusedStopIndex[NStations-1],0);
*/
    {
      // #pragma omp  task
      {
        // --- SET PARAMETERS FOR THE ITERATION ---

        FIRSTCASTATION = 0;
        // if ( (isec == kAllSecIter) || (isec == kAllSecEIter) || (isec == kAllSecJumpIter) )
        //   FIRSTCASTATION = 2;

        DOUBLET_CHI2_CUT = 11.3449 * 2.f / 3.f;  // prob = 0.1

        TRIPLET_CHI2_CUT = 21.1075;  // prob = 0.01%

        switch (isec) {
          case kFastPrimIter:
            TRIPLET_CHI2_CUT = 7.815 * 3;  // prob = 0.05
            break;
          case kAllPrimIter:
          case kAllPrimEIter:
            TRIPLET_CHI2_CUT = 7.815 * 3;  // prob = 0.05
            break;
          case kAllPrimJumpIter:
            TRIPLET_CHI2_CUT = 6.252 * 3;  // prob = 0.1
            break;
          case kAllSecIter:
          case kAllSecEIter:
            TRIPLET_CHI2_CUT = 6.252 * 3;  //2.706; // prob = 0.1
            break;
        }

        Pick_gather = 3.0;  /// coefficient for size of region for attach new hits to the created track
        if ((isec == kAllPrimIter) || (isec == kAllPrimEIter) || (isec == kAllPrimJumpIter) || (isec == kAllSecIter)
            || (isec == kAllSecEIter) || (isec == kAllSecJumpIter))
          Pick_gather = 4.0;

        PickNeighbour = 5.0;  // (PickNeighbour < dp/dp_error)  =>  triplets are neighbours
        // if ( (isec == kFastPrimIter) )
        //   PickNeighbour = 5.0*0.5; // TODO understand why works with 0.2

        MaxInvMom = 1.0 / 0.5;  // max considered q/p

        if (fTrackingMode == kMcbm) MaxInvMom = 1.5 / 0.1;  // max considered q/p
        if ((isec == kAllPrimJumpIter) || (isec == kAllSecIter) || (isec == kAllSecJumpIter)) MaxInvMom = 1.0 / 0.1;
        if ((isec == kAllPrimIter) || (isec == kAllPrimEIter) || (isec == kAllSecEIter)) MaxInvMom = 1. / 0.05;

        MaxSlopePV = 1.1;
        if (  // (isec == kAllPrimIter) || (isec == kAllPrimEIter) || (isec == kAllPrimJumpIter) ||
          (isec == kAllSecIter) || (isec == kAllSecEIter) || (isec == kAllSecJumpIter))
          MaxSlopePV = 1.5;

        MaxSlope = 2.748;  // corresponds to 70 grad
        // define the target
        targX = 0;
        targY = 0;
        targZ = 0;  //  suppose, what target will be at (0,0,0)

        float SigmaTargetX = 0, SigmaTargetY = 0;  // target constraint [cm]
        if ((isec == kFastPrimIter) || (isec == kFastPrimIter2) || (isec == kFastPrimJumpIter) || (isec == kAllPrimIter)
            || (isec == kAllPrimEIter) || (isec == kAllPrimJumpIter)) {  // target
          targB = vtxFieldValue;
          if ((isec == kFastPrimIter) || (isec == kAllPrimIter) || (isec == kAllPrimEIter))
            SigmaTargetX = SigmaTargetY = 1;  // target
          else
            SigmaTargetX = SigmaTargetY = 5;
        }
        if ((isec == kAllSecIter) || (isec == kAllSecEIter)
            || (isec == kAllSecJumpIter)) {  //use outer radius of the 1st station as a constraint
          L1Station& st = vStations[0];
          SigmaTargetX = SigmaTargetY = 10;  //st.Rmax[0];
          targZ                       = 0.;  //-1.;
          st.fieldSlice.GetFieldValue(0, 0, targB);
        }

        TargetXYInfo.C00 = SigmaTargetX * SigmaTargetX;
        TargetXYInfo.C10 = 0;
        TargetXYInfo.C11 = SigmaTargetY * SigmaTargetY;

        /// Set correction in order to take into account overlaping and iff z.
        /// The reason is that low momentum tracks are too curved and goes not from target direction. That's why sort by hit_y/hit_z is not work idealy
        /// If sort by y then it is max diff between same station's modules (~0.4cm)
        MaxDZ = 0;
        if ((isec == kAllPrimIter) || (isec == kAllPrimEIter) || (isec == kAllPrimJumpIter) || (isec == kAllSecIter)
            || (isec == kAllSecEIter) || (isec == kAllSecJumpIter))
          MaxDZ = 0.1;

        if (NStations > (int) fkMaxNstations) cout << " CATrackFinder: Error: Too many Stations" << endl;
      }

#ifndef L1_NO_ASSERT
      for (int istal = NStations - 1; istal >= 0; istal--) {
        L1_ASSERT(StsHitsUnusedStopIndex[istal] >= StsHitsUnusedStartIndex[istal],
                  StsHitsUnusedStopIndex[istal] << " >= " << StsHitsUnusedStartIndex[istal]);
        L1_ASSERT(StsHitsUnusedStopIndex[istal] <= (*vStsHitsUnused).size(),
                  StsHitsUnusedStopIndex[istal] << " <= " << (*vStsHitsUnused).size());
      }
#endif  // L1_NO_ASSERT
    }


    {
      /// possible left hits of triplets are splited in portions of 16 (4 SIMDs) to use memory faster
      fDupletPortionStopIndex[NStations - 1] = 0;
      fDupletPortionSize.clear();
      for (int istal = NStations - 2; istal >= FIRSTCASTATION; istal--) {  //start downstream chambers
        int NHits_l   = StsHitsUnusedStopIndex[istal] - StsHitsUnusedStartIndex[istal];
        int nPortions = NHits_l / Portion;
        int rest      = NHits_l - nPortions * Portion;
        for (int ipp = 0; ipp < nPortions; ipp++) {
          fDupletPortionSize.push_back(Portion);
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
         fDupletPortionStopIndex[NStations-1] = 0;
         unsigned int ip = 0;  //index of curent portion
         
         for (int istal = NStations-2; istal >= FIRSTCASTATION; istal--)  //start downstream chambers
         {
         int nHits = StsHitsUnusedStopIndex[istal] - StsHitsUnusedStartIndex[istal];
         
         int NHits_P = nHits/Portion;
         
         for( int ipp = 0; ipp < NHits_P; ipp++ )
         {
         n_g1[ip] = Portion;
         ip++;
         } // start_lh - portion of left hits
         
         n_g1[ip] = nHits - NHits_P*Portion;
         
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


    const Tindex vPortion = Portion / fvecLen;
    L1TrackPar T_1[vPortion];
    L1FieldRegion fld_1[vPortion];
    THitI hitsl_1[Portion];
    L1TrackPar TG_1[vPortion];
    L1FieldRegion fldG_1[vPortion];
    THitI hitslG_1[Portion];

    L1Vector<THitI> hitsm_2("L1CATrackFinder::hitsm_2");  /// middle hits indexed by number of doublets in portion(i2)
    L1Vector<THitI> i1_2(
      "L1CATrackFinder::i1_2");  /// index in portion of singlets(i1) indexed by index in portion of doublets(i2)


    L1Vector<THitI> hitsmG_2("L1CATrackFinder::hitsmG_2");  /// middle hits indexed by number of doublets in portion(i2)
    L1Vector<THitI> i1G_2(
      "L1CATrackFinder::i1G_2");  /// index in portion of singlets(i1) indexed by index in portion of doublets(i2)
    L1Vector<char> lmDuplets[fkMaxNstations] {
      "L1CATrackFinder::lmDuplets"};  // is exist a doublet started from indexed by left hit
    L1Vector<char> lmDupletsG[fkMaxNstations] {
      "L1CATrackFinder::lmDupletsG"};  // is exist a doublet started from indexed by left hit

    for (int i = 0; i < NStations; i++) {
      lmDuplets[i].SetName(std::stringstream() << "L1CATrackFinder::lmDuplets[" << i << "]");
      lmDupletsG[i].SetName(std::stringstream() << "L1CATrackFinder::lmDupletsG[" << i << "]");
    }

    hitsm_2.reserve(9000);  // TODO: make reasonable cuts on n combinations, put them to the header
    i1_2.reserve(9000);  // TODO: why that large numbers are needed even for mbias??? something goes wrong sometimes..
    hitsmG_2.reserve(9000);
    i1G_2.reserve(9000);

    for (int istal = NStations - 2; istal >= FIRSTCASTATION; istal--)  //  //start downstream chambers
    {

#ifdef _OPENMP
#pragma omp parallel for firstprivate(T_1, fld_1, hitsl_1, hitsm_2, i1_2, TG_1, fldG_1, hitslG_1, hitsmG_2,            \
                                      i1G_2)  //schedule(dynamic, 2)
#endif
      for (Tindex ip = fDupletPortionStopIndex[istal + 1]; ip < fDupletPortionStopIndex[istal]; ++ip) {
        Tindex n_2;  /// number of doublets in portion
        int NHitsSta = StsHitsStopIndex[istal] - StsHitsStartIndex[istal];
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


        if ((isec == kFastPrimJumpIter) || (isec == kAllPrimJumpIter) || (isec == kAllSecJumpIter)) {
          Tindex nG_2;
          hitsmG_2.clear();
          i1G_2.clear();

          DupletsStaPort(  // input
            istal, istal + 2, ip, fDupletPortionSize, fDupletPortionStopIndex,
            // output
            TG_1, fldG_1, hitslG_1,

            lmDupletsG[istal],

            nG_2, i1G_2, hitsmG_2);

          TripletsStaPort(  // input
            istal, istal + 1, istal + 3, nstaltriplets, T_1, fld_1, hitsl_1,

            n_2, i1_2, hitsm_2, lmDupletsG[istal + 1]);

          TripletsStaPort(  // input
            istal, istal + 2, istal + 3, nstaltriplets, TG_1, fldG_1, hitslG_1,

            nG_2, i1G_2, hitsmG_2, lmDuplets[istal + 2]

          );
        }
      }  //
    }

    //     int nlevels[fkMaxNstations];  // number of triplets with some number of neighbours.
    //     for (int il = 0; il < NStations; ++il) nlevels[il] = 0;
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

    int min_level = 0;  // min level for start triplet. So min track length = min_level+3.
    //    if (isec == kFastPrimJumpIter) min_level = 1;
    if ((isec == kAllSecIter) || (isec == kAllSecEIter) || (isec == kAllSecJumpIter))
      min_level = 1;  // only the long low momentum tracks

#ifdef TRACKS_FROM_TRIPLETS
    if (isec == TRACKS_FROM_TRIPLETS_ITERATION) min_level = 0;
#endif

    //     int min_level = 1; // min level for start triplet. So min track length = min_level+3.
    //     if (isec == kAllPrimJumpIter) min_level = 1;
    //     if ( (isec == kAllSecIter) || (isec == kAllSecJumpIter) ) min_level = 2; // only the long low momentum tracks
    // //    if (isec == -1) min_level = NStations-3 - 3; //only the longest tracks


    L1Branch curr_tr;
    L1Branch new_tr[fkMaxNstations];
    L1Branch best_tr;
    fscal curr_chi2 = 0;

    fscal best_chi2      = 0;
    unsigned char best_L = 0;

    unsigned char curr_L = 1;
    int ndf              = 1;


    // collect consequtive: the longest tracks, shorter, more shorter and so on
    for (int ilev = NStations - 3; ilev >= min_level; ilev--) {
      // choose length in triplets number - ilev - the maximum possible triplet level among all triplets in the searched candidate
      TStopwatch Time;

      //  how many levels to check
      int nlevel = (NStations - 2) - ilev + 1;

      const unsigned char min_best_l = (ilev > min_level) ? ilev + 2 : min_level + 3;  // loose maximum


      for (int i = 0; i < fNThreads; ++i) {
        fTrackCandidates[i].clear();
      }

      fStripToTrack.reset(NStsStrips, -1);
      fStripToTrackB.reset(NStsStrips, -1);

      for (int istaF = FIRSTCASTATION; istaF <= NStations - 3 - ilev; ++istaF) {

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
            if (GetFUsed((*fStripFlag)[(*vStsHitsUnused)[first_trip.GetLHit()].f]
                         | (*fStripFlag)[(*vStsHitsUnused)[first_trip.GetLHit()].b]))
              continue;


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
                if (isec != kFastPrimIter && isec != kAllPrimIter && isec != kAllPrimEIter && isec != kAllSecEIter)
                  if (first_trip.GetLevel() == 0)
                    continue;  // ghost suppression // find track with 3 hits only if it was created from a chain of triplets, but not from only one triplet

                if (fTrackingMode != kMcbm)
                  if ((ilev == 0) && (GetFStation((*fStripFlag)[(*vStsHitsUnused)[first_trip.GetLHit()].f]) != 0))
                    continue;  // ghost supression // collect only MAPS tracks-triplets  CHECK!!!
              }
              if (first_trip.GetLevel() < ilev)
                continue;  // try only triplets, which can start track with ilev+3 length. w\o it have more ghosts, but efficiency either
            }


            //  curr_tr.Momentum = 0.f;
            curr_tr.chi2 = 0.f;
            //   curr_tr.Lengtha  = 0;
            curr_tr.ista = 0;
            curr_tr.fStsHits.clear();
            curr_tr.fStsHits.push_back((*RealIHitP)[first_trip.GetLHit()]);
            curr_tr.NHits = 1;

            curr_L    = 1;
            curr_chi2 = first_trip.GetChi2();

            best_tr = (curr_tr);

            best_chi2 = curr_chi2;
            best_L    = curr_L;

            CAFindTrack(istaF, best_tr, best_L, best_chi2, &first_trip, (curr_tr), curr_L, curr_chi2, min_best_l,
                        new_tr);  /// reqursive func to build a tree of possible track-candidates and choose the best

            //              if ( best_L < min_best_l ) continue;
            if (best_L < ilev + 2) continue;  // lose maximum one hit

            if (best_L < min_level + 3) continue;  // should find all hits for min_level

            ndf       = best_L * 2 - 5;
            best_chi2 = best_chi2 / ndf;  //normalize

#ifndef TRACKS_FROM_TRIPLETS
            if (fGhostSuppression) {
              if (best_L == 3) {
                // if( isec == kAllSecIter ) continue; // too /*short*/ secondary track
                if (((isec == kAllSecIter) || (isec == kAllSecEIter) || (isec == kAllSecJumpIter)) && (istaF != 0))
                  continue;  // too /*short*/ non-MAPS track
                if ((isec != kAllSecIter) && (isec != kAllSecEIter) && (isec != kAllSecJumpIter) && (best_chi2 > 5.0))
                  continue;
              }
            }
#endif
            best_tr.Set(istaF, best_L, best_chi2, first_trip.GetQp());
            fTrackCandidates[thread_num].push_back(best_tr);

            L1Branch& tr = fTrackCandidates[thread_num].back();
            tr.CandIndex = fTrackCandidates[thread_num].size() - 1 + thread_num * 10000000;

            bool check = 1;

            for (L1Vector<THitI>::iterator phitIt = tr.fStsHits.begin();  /// used strips are marked
                 phitIt != tr.fStsHits.end(); ++phitIt) {
              const L1Hit& h = (*vStsHits)[*phitIt];
#ifdef _OPENMP
              omp_set_lock(&fHitToBestTrackB[h.b]);
#endif
              int& strip1 = (fStripToTrackB)[h.b];

              if (strip1 != -1) {
                int thread = strip1 / 10000000;
                int num    = (strip1 - thread * 10000000);

                if (L1Branch::compareCand(tr, fTrackCandidates[thread][num])) {
                  fTrackCandidates[thread][num].CandIndex = -1;
                  strip1                                  = tr.CandIndex;
                }
                else {
                  check = 0;
#ifdef _OPENMP
                  omp_unset_lock(&fHitToBestTrackB[h.b]);
#endif
                  break;
                }
              }
              else
                strip1 = tr.CandIndex;
#ifdef _OPENMP
              omp_unset_lock(&fHitToBestTrackB[h.b]);
#endif

              if (check) {
#ifdef _OPENMP
                omp_set_lock(&fHitToBestTrackF[h.f]);
#endif
                int& strip2 = (fStripToTrack)[h.f];
                if (strip2 != -1) {
                  int thread = strip2 / 10000000;
                  int num    = (strip2 - thread * 10000000);

                  if (L1Branch::compareCand(tr, fTrackCandidates[thread][num])) {
                    fTrackCandidates[thread][num].CandIndex = -1;
                    strip2                                  = tr.CandIndex;
                  }
                  else {
                    check = 0;
#ifdef _OPENMP
                    omp_unset_lock(&fHitToBestTrackF[h.f]);
#endif
                    break;
                  }
                }
                else
                  strip2 = tr.CandIndex;
#ifdef _OPENMP
                omp_unset_lock(&fHitToBestTrackF[h.f]);
#endif
              }
            }
            if (!check) { fTrackCandidates[thread_num].pop_back(); }
          }  // itrip
        }    // iThread
      }      // istaF

      if (--nlevel == 0) break;

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

          bool check = 1;

          if (fTrackCandidates[i][iCandidate].CandIndex != -1) {
            for (L1Vector<THitI>::iterator phIt = tr.fStsHits.begin();  /// used strips are marked
                 phIt != tr.fStsHits.end(); ++phIt) {
              const L1Hit& h = (((*vStsHits))[*phIt]);
              if (((fStripToTrackB)[h.b] != tr.CandIndex) || ((fStripToTrack)[h.f] != tr.CandIndex)) {
                check = 0;
                break;
              }
            }

            if (fTrackingMode == kMcbm) {
              if (tr.NHits <= 3) { check = 0; }
            }
            else {
              if (tr.NHits < 3) { check = 0; }
            }

            if (check) {
#ifdef EXTEND_TRACKS
              if (fTrackingMode != kMcbm)
                if (tr.NHits != NStations) BranchExtender(tr);
#endif
              float sumTime = 0;


#ifdef _OPENMP

              int num_thread = omp_get_thread_num();

#else

              int num_thread = 0;

#endif

              for (L1Vector<THitI>::iterator phIt = tr.fStsHits.begin();  /// used strips are marked
                   phIt != tr.fStsHits.end(); ++phIt) {
                L1Hit& h = (*vStsHits)[*phIt];


                SetFUsed((*fStripFlag)[h.f]);
                SetFUsed((*fStripFlag)[h.b]);

                fRecoHits_local[num_thread].push_back(*phIt);

                const L1Hit& hit = (*vStsHits)[*phIt];


                L1HitPoint tempPoint = CreateHitPoint(hit);  //TODO take number of station from hit

                float xcoor, ycoor = 0;
                L1Station stah = vStations[0];
                StripsToCoor(tempPoint.U(), tempPoint.V(), xcoor, ycoor, stah);
                float zcoor = tempPoint.Z();

                float timeFlight = sqrt(xcoor * xcoor + ycoor * ycoor + zcoor * zcoor) / 30.f;  // c = 30[cm/ns]
                sumTime += (hit.t - timeFlight);
              }

              t.NHits = tr.NHits;
              // t.Momentum   = tr.Momentum;
              t.fTrackTime = sumTime / t.NHits;
              fTracks_local[num_thread].push_back(t);
            }
          }
        }
      }


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
    }  //istaf

#ifdef XXX
    c_timer.Stop();
    ti[isec]["tracks"] = c_timer;
    c_timer.Start();
#endif


    if (isec < (fNFindIterations - 1)) {
      int NHitsOnStation = 0;

      for (int ista = 0; ista < NStations; ++ista) {
        int start                = StsHitsUnusedStartIndex[ista];
        int Nelements            = StsHitsUnusedStopIndex[ista] - start;
        L1Hit* staHits           = nullptr;  // to avoid out-of-range error in ..[start]
        THitI* staHitIndices     = nullptr;
        L1HitPoint* staHitPoints = nullptr;
        if (Nelements > 0) {
          staHits       = &((*vStsHitsUnused)[start]);
          staHitIndices = &((*RealIHitP)[start]);
          staHitPoints  = &((*vStsHitPointsUnused)[start]);
        }

        int NHitsOnStationTmp = NHitsOnStation;

        vGridTime[ista].UpdateIterGrid(Nelements, staHits, RealIHitPBuf, staHitIndices, vStsHitsUnused_buf,
                                       vStsHitPointsUnused_buf, staHitPoints, NHitsOnStation, ista, *this, fStripFlag);

        StsHitsUnusedStartIndex[ista] = NHitsOnStationTmp;
        StsHitsUnusedStopIndex[ista]  = NHitsOnStation;
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
      //     draw->DrawRestHits(StsHitsUnusedStartIndex, StsHitsUnusedStopIndex, RealIHit);
      //     draw->DrawRecoTracks();
      //     draw->SaveCanvas("Reco_"+isec+"_");
      //     draw->DrawAsk();
      // #endif

      // #ifdef PULLS
      //       fL1Pulls->Build(1);
      // #endif
#ifdef COUNTERS
      //  stat_nHits[isec] += (vStsHitsUnused*)->Size();

      cout << "iter = " << isec << endl;
      cout << " NSinglets = " << stat_nSinglets[isec] / stat_N << endl;
      //    cout << " NDoublets = " << stat_nDoublets[isec]/stat_N << endl;
      cout << " NTriplets = " << stat_nTriplets[isec] / stat_N << endl;
      cout << " NHitsUnused = " << stat_nHits[isec] / stat_N << endl;

#endif  // COUNTERS
    }
  }  // for (int isec

#ifdef XXX
  c_timerG.Stop();
  gti["iterts"] = c_timerG;
  c_timerG.Start();
#endif

#ifdef MERGE_CLONES
  CAMergeClones();
#endif

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
  float tripl_speed = 1000. / (tmp_ti.GetTimerAll()["tripl1"].Real());
  filestr << tripl_speed << " ";
  filestr.close();


#if 0
        static long int NTimes =0, NHits=0, HitSize =0, NStrips=0, StripSize =0, NStripsB=0, StripSizeB =0,
        NDup=0, DupSize=0, NTrip=0, TripSize=0, NBranches=0, BranchSize=0, NTracks=0, TrackSize=0 ;
        
        NTimes++;
        NHits += vStsHitsUnused->size();
        HitSize += vStsHitsUnused->size()*sizeof(L1Hit);
        NStrips+= vStsStrips.size();
        StripSize += vStsStrips.size()*sizeof(fscal) + (*fStripFlag).size()*sizeof(unsigned char);
        NStripsB+= (*fStripFlagB).size();
        StripSizeB += vStsStripsB.size()*sizeof(fscal) + (*fStripFlagB).size()*sizeof(unsigned char);
        NDup += stat_max_n_dup;
        DupSize += stat_max_n_dup*sizeof(/*short*/ int);
        NTrip += stat_max_n_trip;
        TripSize += stat_max_trip_size;
        
        NBranches += stat_max_n_branches;
        BranchSize += stat_max_BranchSize;
        NTracks += fTracks.size();
        TrackSize += sizeof(L1Track)*fTracks.size() + sizeof(THitI)*fRecoHits.size();
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
    const THitI& ihitm = curr_trip->GetMHit();
    const THitI& ihitr = curr_trip->GetRHit();


    if (!GetFUsed((*fStripFlag)[(*vStsHitsUnused)[ihitm].f] | (*fStripFlag)[(*vStsHitsUnused)[ihitm].b])) {

      //        curr_tr.StsHits.push_back((*RealIHitP)[ihitm]);

      curr_tr.fStsHits.push_back((*RealIHitP)[ihitm]);

      curr_tr.NHits++;

      curr_L++;
    }

    if (!GetFUsed((*fStripFlag)[(*vStsHitsUnused)[ihitr].f] | (*fStripFlag)[(*vStsHitsUnused)[ihitr].b])) {

      //curr_tr.StsHits.push_back((*RealIHitP)[ihitr]);
      curr_tr.fStsHits.push_back((*RealIHitP)[ihitr]);

      curr_tr.NHits++;

      curr_L++;
    }

    //if( curr_L < min_best_l - 1 ) return; // suppouse that only one hit can be added by extender
    if (curr_chi2 > TRACK_CHI2_CUT * (curr_L * 2 - 5.0)) return;


    //       // try to find more hits
    // #ifdef EXTEND_TRACKS
    //     // if( curr_L < min_best_l )
    //     if (isec != kFastPrimJumpIter && isec != kAllPrimJumpIter && isec != kAllSecJumpIter && curr_L >= 3){
    //       //curr_chi2 = BranchExtender(curr_tr);
    //       BranchExtender(curr_tr);
    //       curr_L = curr_tr.StsHits.size();
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
      //    if (( fabs(qp - qp2) > PickNeighbour * (Cqp + Cqp2) ) )  continue;

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

      if (fTrackingMode != kMcbm) {
        if (dqp > PickNeighbour * Cqp) {
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

      if (fTrackingMode == kGlobal || fTrackingMode == kMcbm) {
        if (dty > 3 * Cty) continue;
        if (dtx > 3 * Ctx) continue;
      }


      if (GetFUsed((*fStripFlag)[(*vStsHitsUnused)[new_trip.GetLHit()].f]
                   | (*fStripFlag)[(*vStsHitsUnused)[new_trip.GetLHit()].b])) {  //hits are used
        //  no used hits allowed -> compare and store track
        if ((curr_L > best_L) || ((curr_L == best_L) && (curr_chi2 < best_chi2))) {
          best_tr = curr_tr;

          best_chi2 = curr_chi2;
          best_L    = curr_L;
        }
      }
      else {  // if hits are used add new triplet to the current track

        new_tr[ista] = curr_tr;

        unsigned char new_L = curr_L;
        fscal new_chi2      = curr_chi2;

        // add new hit
        new_tr[ista].fStsHits.push_back((*RealIHitP)[new_trip.GetLHit()]);
        new_tr[ista].NHits++;
        new_L += 1;

        dqp = dqp / Cqp;

        dtx = dtx / Ctx;
        dty = dty / Cty;

        if (fTrackingMode == kGlobal || fTrackingMode == kMcbm) {
          new_chi2 += dtx * dtx;
          new_chi2 += dty * dty;
        }
        else {
          new_chi2 += dqp * dqp;
        }

        if (new_chi2 > TRACK_CHI2_CUT * new_L) continue;

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
