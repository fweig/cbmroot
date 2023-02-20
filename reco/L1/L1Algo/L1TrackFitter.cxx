/* Copyright (C) 2010-2020 Frankfurt Institute for Advanced Studies, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ivan Kisel, Sergey Gorbunov, Igor Kulakov [committer], Valentina Akishina, Maksym Zyzak */

#include <iostream>
#include <vector>

#include "L1Algo.h"
#include "L1Fit.h"
#include "L1TrackPar.h"

using std::cout;
using std::endl;
using std::vector;


namespace NS_L1TrackFitter
{
  const fvec c_light(0.000299792458), c_light_i(fvec(1.) / c_light);
}  // namespace NS_L1TrackFitter
using namespace NS_L1TrackFitter;


void L1Algo::L1KFTrackFitter()
{
  //  cout << " Start L1 Track Fitter " << endl;
  int start_hit = 0;  // for interation in fRecoHits[]

  //  static L1FieldValue fldB0, fldB1, fldB2 _fvecalignment;
  //  static L1FieldRegion fld _fvecalignment;
  L1FieldValue fldB0, fldB1, fldB2 _fvecalignment;
  L1FieldRegion fld _fvecalignment;


  L1FieldValue fldB01, fldB11, fldB21 _fvecalignment;
  L1FieldRegion fld1 _fvecalignment;

  const int nStations = fParameters.GetNstationsActive();
  int nTracks_SIMD    = fvec::size();

  L1Fit fit;  // fit parameters coresponding to the current track
  L1TrackPar& tr = fit.Tr();
  fit.SetParticleMass(GetDefaultParticleMass());
  fit.SetDoFitVelocity(true);

  L1Track* t[fvec::size()] {nullptr};

  const L1Station* sta = fParameters.GetStations().begin();

  // Spatial-time position of a hit vs. station and track in the portion
  // NOTE: u- and v-axes are axes, orthogonal to front and back strips of the station, respectively.
  fvec u[L1Constants::size::kMaxNstations];    // Hit position along the u-axis [cm]
  fvec v[L1Constants::size::kMaxNstations];    // Hit position along the v-axis [cm]
  fvec du2[L1Constants::size::kMaxNstations];  // Hit position uncertainty along the u-axis [cm]
  fvec dv2[L1Constants::size::kMaxNstations];  // Hit position uncertainty along the v-axis [cm]

  fvec x[L1Constants::size::kMaxNstations];     // Hit position along the x-axis [cm]
  fvec y[L1Constants::size::kMaxNstations];     // Hit position along the y-axis [cm]
  fvec d_xx[L1Constants::size::kMaxNstations];  // Variance of the x hit position coordinate [cm2]
  fvec d_yy[L1Constants::size::kMaxNstations];  // Variance of the y hit position coordinate [cm2]
  fvec d_xy[L1Constants::size::kMaxNstations];  // Covariance between the x and y hit position coordinates [cm2]

  fvec z[L1Constants::size::kMaxNstations];  // Hit position along the z-axis (precised) [cm]

  fvec time[L1Constants::size::kMaxNstations];  // Hit time [ns]
  fvec dt2[L1Constants::size::kMaxNstations];   // Hit time uncertainty [ns] squared

  fvec x_first;
  fvec y_first;
  fvec d_xx_fst;
  fvec d_yy_fst;
  fvec d_xy_fst;

  fvec time_first;
  fvec wtime_first;
  fvec dt2_first;

  fvec x_last;
  fvec y_last;
  fvec d_xx_lst;
  fvec d_yy_lst;
  fvec d_xy_lst;

  fvec time_last;
  fvec wtime_last;
  fvec dt2_last;
  //  fvec dt2_lst;  /// TODO: Why are there two different variables for the time error on the last station?

  fvec By[L1Constants::size::kMaxNstations];
  fmask w[L1Constants::size::kMaxNstations];
  fmask w_time[L1Constants::size::kMaxNstations];  // !!!

  fvec y_temp;
  fvec x_temp;
  fvec fldZ0;
  fvec fldZ1;
  fvec fldZ2;
  fvec z_start;
  fvec z_end;

  L1FieldValue fB[L1Constants::size::kMaxNstations], fB_temp _fvecalignment;


  fvec ZSta[L1Constants::size::kMaxNstations];
  for (int ista = 0; ista < nStations; ista++) {
    ZSta[ista] = sta[ista].fZ;
  }

  unsigned short N_vTracks = fTracks.size();  // number of tracks processed per one SSE register

  for (unsigned short itrack = 0; itrack < N_vTracks; itrack += fvec::size()) {

    if (N_vTracks - itrack < static_cast<unsigned short>(fvec::size())) nTracks_SIMD = N_vTracks - itrack;

    for (int i = 0; i < nTracks_SIMD; i++) {
      t[i] = &fTracks[itrack + i];
    }

    // fill the rest of the SIMD vectors with something reasonable
    for (uint i = nTracks_SIMD; i < fvec::size(); i++) {
      t[i] = &fTracks[itrack];
    }

    // get hits of current track
    for (int ista = 0; ista < nStations; ista++) {
      w[ista]      = fmask::Zero();
      w_time[ista] = fmask::Zero();
      z[ista]      = ZSta[ista];
    }

    //fmask isFieldPresent = fmask::Zero();

    for (int iVec = 0; iVec < nTracks_SIMD; iVec++) {

      int nHitsTrack = t[iVec]->NHits;
      int iSta[L1Constants::size::kMaxNstations];

      for (int ih = 0; ih < nHitsTrack; ih++) {

        const L1Hit& hit = fInputData.GetHit(fRecoHits[start_hit++]);
        const int ista   = hit.iSt;

        //if (sta[ista].fieldStatus) { isFieldPresent[iVec] = true; }

        iSta[ih]      = ista;
        w[ista][iVec] = true;
        if (sta[ista].timeInfo) { w_time[ista][iVec] = true; }

        u[ista][iVec]            = hit.u;
        v[ista][iVec]            = hit.v;
        du2[ista][iVec]          = hit.du2;
        dv2[ista][iVec]          = hit.dv2;
        std::tie(x_temp, y_temp) = sta[ista].ConvUVtoXY<fvec>(u[ista], v[ista]);
        x[ista][iVec]            = x_temp[iVec];
        y[ista][iVec]            = y_temp[iVec];
        time[ista][iVec]         = hit.t;
        dt2[ista][iVec]          = hit.dt2;
        if (!sta[ista].timeInfo) { dt2[ista][iVec] = 1.e4; }
        z[ista][iVec] = hit.z;
        sta[ista].fieldSlice.GetFieldValue(x[ista], y[ista], fB_temp);
        std::tie(d_xx[ista], d_xy[ista], d_yy[ista]) = sta[ista].FormXYCovarianceMatrix(du2[ista], dv2[ista]);

        fB[ista].x[iVec] = fB_temp.x[iVec];
        fB[ista].y[iVec] = fB_temp.y[iVec];
        fB[ista].z[iVec] = fB_temp.z[iVec];
        if (ih == 0) {
          z_start[iVec]     = z[ista][iVec];
          x_first[iVec]     = x[ista][iVec];
          y_first[iVec]     = y[ista][iVec];
          time_first[iVec]  = time[ista][iVec];
          wtime_first[iVec] = sta[ista].timeInfo ? 1. : 0.;
          dt2_first[iVec]   = dt2[ista][iVec];
          d_xx_fst[iVec]    = d_xx[ista][iVec];
          d_yy_fst[iVec]    = d_yy[ista][iVec];
          d_xy_fst[iVec]    = d_xy[ista][iVec];
        }

        else if (ih == nHitsTrack - 1) {
          z_end[iVec]      = z[ista][iVec];
          x_last[iVec]     = x[ista][iVec];
          y_last[iVec]     = y[ista][iVec];
          d_xx_lst[iVec]   = d_xx[ista][iVec];
          d_yy_lst[iVec]   = d_yy[ista][iVec];
          d_xy_lst[iVec]   = d_xy[ista][iVec];
          time_last[iVec]  = time[ista][iVec];
          dt2_last[iVec]   = dt2[ista][iVec];
          wtime_last[iVec] = sta[ista].timeInfo ? 1. : 0.;
        }
      }

      for (int ih = nHitsTrack - 1; ih >= 0; ih--) {
        const int ista      = iSta[ih];
        const L1Station& st = fParameters.GetStation(ista);
        By[ista][iVec]      = st.fieldSlice.cy[0][0];
      }
    }

    fit.GuessTrack(z_end, x, y, z, time, By, w, w_time, nStations);

    if (kGlobal == fTrackingMode || kMcbm == fTrackingMode) { tr.qp = fvec(1. / 1.1); }

    // tr.qp = iif(isFieldPresent, tr.qp, fvec(1. / 0.25));

    for (int iter = 0; iter < 2; iter++) {  // 1.5 iterations

      fit.SetQp0(tr.qp);

      // fit backward

      int ista = nStations - 1;

      time_last = iif(w_time[ista], time_last, fvec::Zero());
      dt2_last  = iif(w_time[ista], dt2_last, fvec(1.e6));

      tr.ResetErrors(d_xx_lst, d_yy_lst, 0.1, 0.1, 1.0, dt2_last, 1.e-2);
      tr.C10 = d_xy_lst;
      tr.x   = x_last;
      tr.y   = y_last;
      tr.t   = time_last;
      tr.vi  = L1TrackPar::kClightNsInv;
      tr.InitVelocityRange(0.5);
      tr.NDF = -3.0;

      tr.nTimeMeasurements = wtime_last;

      fldZ1 = z[ista];

      sta[ista].fieldSlice.GetFieldValue(tr.x, tr.y, fldB1);

      fldB1.Combine(fB[ista], w[ista]);

      fldZ2   = z[ista - 2];
      fvec dz = fldZ2 - fldZ1;
      sta[ista].fieldSlice.GetFieldValue(tr.x + tr.tx * dz, tr.y + tr.ty * dz, fldB2);
      fldB2.Combine(fB[ista - 2], w[ista - 2]);
      fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);

      for (--ista; ista >= 0; ista--) {

        fldZ0 = z[ista];
        dz    = (fldZ1 - fldZ0);
        sta[ista].fieldSlice.GetFieldValue(tr.x - tr.tx * dz, tr.y - tr.ty * dz, fldB0);
        fldB0.Combine(fB[ista], w[ista]);
        fld.Set(fldB0, fldZ0, fldB1, fldZ1, fldB2, fldZ2);

        fmask initialised = (z[ista] < z_end) & (z_start <= z[ista]);

        fld1 = fld;

        fit.SetMask(initialised);
        fit.Extrapolate(z[ista], fld1);
        fit.AddMsInMaterial(fParameters.GetMaterialThickness(ista, tr.x, tr.y));
        fit.EnergyLossCorrection(fParameters.GetMaterialThickness(ista, tr.x, tr.y), fvec(1.f));

        fit.SetMask(initialised && w[ista]);
        fit.Filter(sta[ista].frontInfo, u[ista], du2[ista]);
        fit.Filter(sta[ista].backInfo, v[ista], dv2[ista]);
        fit.FilterTime(time[ista], dt2[ista], sta[ista].timeInfo);


        fldB2 = fldB1;
        fldZ2 = fldZ1;
        fldB1 = fldB0;
        fldZ1 = fldZ0;
      }

      // extrapolate to the PV region

      L1Fit fitpv = fit;
      {
        fitpv.SetMask(fmask::One());

        L1UMeasurementInfo vtxInfoX;
        vtxInfoX.cos_phi = 1.;
        vtxInfoX.sin_phi = 0.;

        L1UMeasurementInfo vtxInfoY;
        vtxInfoY.cos_phi = 0;
        vtxInfoY.sin_phi = 1.;

        if (kGlobal == fTrackingMode) {
          for (int vtxIter = 0; vtxIter < 2; vtxIter++) {
            fitpv.SetQp0(fitpv.Tr().qp);
            fitpv.Tr()    = fit.Tr();
            fitpv.Tr().qp = fitpv.Qp0();
            fitpv.Extrapolate(fParameters.GetTargetPositionZ(), fld);
            fitpv.Filter(vtxInfoX, fParameters.GetTargetPositionX(), fvec(1.e-8));
            fitpv.Filter(vtxInfoY, fParameters.GetTargetPositionY(), fvec(1.e-8));
          }
        }
        else {
          fitpv.SetQp0(fitpv.Tr().qp);
          fitpv.Extrapolate(fParameters.GetTargetPositionZ(), fld);
        }
      }

      //fit.SetQp0(tr.qp);
      //fit.SetMask(fmask::One());
      //fit.MeasureVelocityWithQp();
      //fit.FilterVi(L1TrackPar::kClightNsInv);

      L1TrackPar Tf = fit.Tr();
      if (kGlobal == fTrackingMode) { Tf.qp = fitpv.Tr().qp; }

      for (int iVec = 0; iVec < nTracks_SIMD; iVec++) {
        Tf.copyToArrays(iVec, t[iVec]->TFirst, t[iVec]->CFirst);
        t[iVec]->chi2 = Tf.chi2[iVec];
        t[iVec]->NDF  = (int) Tf.NDF[iVec];
      }

      for (int iVec = 0; iVec < nTracks_SIMD; iVec++) {
        fitpv.Tr().copyToArrays(iVec, t[iVec]->Tpv, t[iVec]->Cpv);
      }

      if (iter == 1) { break; }  // only 1.5 iterations

      // fit forward

      ista = 0;

      tr.ResetErrors(d_xx_fst, d_yy_fst, 0.1, 0.1, 1., dt2_first, 1.e-2);
      tr.C10 = d_xy_fst;

      tr.x  = x_first;
      tr.y  = y_first;
      tr.t  = time_first;
      tr.vi = L1TrackPar::kClightNsInv;
      tr.InitVelocityRange(0.5);

      tr.NDF = -3.0;

      tr.nTimeMeasurements = wtime_first;

      fit.SetQp0(tr.qp);

      fldZ1 = z[ista];
      sta[ista].fieldSlice.GetFieldValue(tr.x, tr.y, fldB1);
      fldB1.Combine(fB[ista], w[ista]);

      fldZ2 = z[ista + 2];
      dz    = fldZ2 - fldZ1;
      sta[ista].fieldSlice.GetFieldValue(tr.x + tr.tx * dz, tr.y + tr.ty * dz, fldB2);
      fldB2.Combine(fB[ista + 2], w[ista + 2]);
      fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);

      for (++ista; ista < nStations; ista++) {
        fldZ0 = z[ista];
        dz    = (fldZ1 - fldZ0);
        sta[ista].fieldSlice.GetFieldValue(tr.x - tr.tx * dz, tr.y - tr.ty * dz, fldB0);
        fldB0.Combine(fB[ista], w[ista]);
        fld.Set(fldB0, fldZ0, fldB1, fldZ1, fldB2, fldZ2);

        fmask initialised = (z[ista] <= z_end) & (z_start < z[ista]);

        fit.SetMask(initialised);
        fit.Extrapolate(z[ista], fld);
        fit.AddMsInMaterial(fParameters.GetMaterialThickness(ista, tr.x, tr.y));
        fit.EnergyLossCorrection(fParameters.GetMaterialThickness(ista, tr.x, tr.y), fvec(-1.f));
        fit.SetMask(initialised && w[ista]);
        fit.Filter(sta[ista].frontInfo, u[ista], du2[ista]);
        fit.Filter(sta[ista].backInfo, v[ista], dv2[ista]);
        fit.FilterTime(time[ista], dt2[ista], sta[ista].timeInfo);

        fldB2 = fldB1;
        fldZ2 = fldZ1;
        fldB1 = fldB0;
        fldZ1 = fldZ0;
      }

      //fit.SetQp0(tr.qp);
      //fit.SetMask(fmask::One());
      //fit.MeasureVelocityWithQp();
      //fit.FilterVi(L1TrackPar::kClightNsInv);

      L1TrackPar Tl = fit.Tr();
      if (kGlobal == fTrackingMode) { Tl.qp = fitpv.Tr().qp; }

      for (int iVec = 0; iVec < nTracks_SIMD; iVec++) {
        Tl.copyToArrays(iVec, t[iVec]->TLast, t[iVec]->CLast);
        t[iVec]->chi2 = Tl.chi2[iVec];
        t[iVec]->NDF  = (int) Tl.NDF[iVec];
      }

    }  // iter
  }
}
