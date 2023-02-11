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
  const fvec ZERO(0.);
  const fvec ONE(1.);
  const fvec vINF(0.1);
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

  L1Fit fit;  // fitting parametr coresponding to current track
  L1TrackPar& tr = fit.Tr();

  fit.SetParticleMass(GetDefaultParticleMass());

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
  fvec dt2_first;

  fvec x_last, x_2last;
  fvec y_last, y_2last;
  fvec d_xx_lst;
  fvec d_yy_lst;
  fvec d_xy_lst;

  fvec time_last;
  fvec dt2_last;
  //  fvec dt2_lst;  /// TODO: Why are there two different variables for the time error on the last station?

  fvec Sy[L1Constants::size::kMaxNstations];
  fmask w[L1Constants::size::kMaxNstations];
  fmask w_time[L1Constants::size::kMaxNstations];  // !!!

  fvec y_temp;
  fvec x_temp;
  fvec fldZ0;
  fvec fldZ1;
  fvec fldZ2;
  fvec z_start;
  fvec z_end, z_2last;

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
        z[ista][iVec]            = hit.z;
        sta[ista].fieldSlice.GetFieldValue(x[ista], y[ista], fB_temp);
        std::tie(d_xx[ista], d_xy[ista], d_yy[ista]) = sta[ista].FormXYCovarianceMatrix(du2[ista], dv2[ista]);

        fB[ista].x[iVec] = fB_temp.x[iVec];
        fB[ista].y[iVec] = fB_temp.y[iVec];
        fB[ista].z[iVec] = fB_temp.z[iVec];
        if (ih == 0) {
          z_start[iVec]    = z[ista][iVec];
          x_first[iVec]    = x[ista][iVec];
          y_first[iVec]    = y[ista][iVec];
          time_first[iVec] = time[ista][iVec];
          dt2_first[iVec]  = dt2[ista][iVec];
          d_xx_fst[iVec]   = d_xx[ista][iVec];
          d_yy_fst[iVec]   = d_yy[ista][iVec];
          d_xy_fst[iVec]   = d_xy[ista][iVec];
        }

        else if (ih == nHitsTrack - 1) {
          z_end[iVec]     = z[ista][iVec];
          x_last[iVec]    = x[ista][iVec];
          y_last[iVec]    = y[ista][iVec];
          d_xx_lst[iVec]  = d_xx[ista][iVec];
          d_yy_lst[iVec]  = d_yy[ista][iVec];
          d_xy_lst[iVec]  = d_xy[ista][iVec];
          time_last[iVec] = time[ista][iVec];
          dt2_last[iVec]  = dt2[ista][iVec];
        }
        else if (ih == nHitsTrack - 2) {
          z_2last[iVec] = z[ista][iVec];
          x_2last[iVec] = x[ista][iVec];
          y_2last[iVec] = y[ista][iVec];
        }
      }

      fscal prevZ = z_end[iVec];
      fscal cursy = 0., curSy = 0.;
      for (int ih = nHitsTrack - 1; ih >= 0; ih--) {
        const int ista      = iSta[ih];
        const L1Station& st = fParameters.GetStation(ista);
        const fscal curZ    = z[ista][iVec];
        fscal dZ            = curZ - prevZ;
        fscal By            = st.fieldSlice.cy[0][0];
        curSy += dZ * cursy + dZ * dZ * By / 2.;
        cursy += dZ * By;
        Sy[ista][iVec] = curSy;
        prevZ          = curZ;
      }
    }

    if (kGlobal == fTrackingMode || kMcbm == fTrackingMode) {
      GuessVecNoField(fit, x_last, x_2last, y_last, y_2last, z_end, z_2last, time_last, w_time, dt2_last);
    }
    else {
      GuessVec(fit, x, y, z, Sy, w, nStations, &z_end, time, w_time);
    }

    if (kGlobal == fTrackingMode || kMcbm == fTrackingMode) { tr.qp = fvec(1. / 1.1); }

    // tr.qp = iif(isFieldPresent, tr.qp, fvec(1. / 0.25));

    for (int iter = 0; iter < 2; iter++) {  // 1.5 iterations

      fit.SetQp0(tr.qp);

      // fit backward

      int ista = nStations - 1;

      time_last = iif(w_time[ista], time_last, fvec::Zero());
      dt2_last  = iif(w_time[ista], dt2_last, fvec(1.e6));

      FilterFirst(fit, x_last, y_last, time_last, dt2_last, d_xx_lst, d_yy_lst, d_xy_lst);

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

      const L1TrackPar& Tf = fit.Tr();

      for (int iVec = 0; iVec < nTracks_SIMD; iVec++) {
        t[iVec]->TFirst[0] = Tf.x[iVec];
        t[iVec]->TFirst[1] = Tf.y[iVec];
        t[iVec]->TFirst[2] = Tf.tx[iVec];
        t[iVec]->TFirst[3] = Tf.ty[iVec];
        t[iVec]->TFirst[4] = Tf.qp[iVec];
        if (kGlobal == fTrackingMode) { t[iVec]->TFirst[4] = fitpv.Tr().qp[iVec]; }
        t[iVec]->TFirst[5] = Tf.z[iVec];
        t[iVec]->TFirst[6] = Tf.t[iVec];

        t[iVec]->CFirst[0]  = Tf.C00[iVec];
        t[iVec]->CFirst[1]  = Tf.C10[iVec];
        t[iVec]->CFirst[2]  = Tf.C11[iVec];
        t[iVec]->CFirst[3]  = Tf.C20[iVec];
        t[iVec]->CFirst[4]  = Tf.C21[iVec];
        t[iVec]->CFirst[5]  = Tf.C22[iVec];
        t[iVec]->CFirst[6]  = Tf.C30[iVec];
        t[iVec]->CFirst[7]  = Tf.C31[iVec];
        t[iVec]->CFirst[8]  = Tf.C32[iVec];
        t[iVec]->CFirst[9]  = Tf.C33[iVec];
        t[iVec]->CFirst[10] = Tf.C40[iVec];
        t[iVec]->CFirst[11] = Tf.C41[iVec];
        t[iVec]->CFirst[12] = Tf.C42[iVec];
        t[iVec]->CFirst[13] = Tf.C43[iVec];
        t[iVec]->CFirst[14] = Tf.C44[iVec];
        t[iVec]->CFirst[15] = Tf.C50[iVec];
        t[iVec]->CFirst[16] = Tf.C51[iVec];
        t[iVec]->CFirst[17] = Tf.C52[iVec];
        t[iVec]->CFirst[18] = Tf.C53[iVec];
        t[iVec]->CFirst[19] = Tf.C54[iVec];
        t[iVec]->CFirst[20] = Tf.C55[iVec];

        t[iVec]->chi2 = Tf.chi2[iVec];
        t[iVec]->NDF  = (int) Tf.NDF[iVec];
      }

      for (int iVec = 0; iVec < nTracks_SIMD; iVec++) {
        t[iVec]->Tpv[0] = fitpv.Tr().x[iVec];
        t[iVec]->Tpv[1] = fitpv.Tr().y[iVec];
        t[iVec]->Tpv[2] = fitpv.Tr().tx[iVec];
        t[iVec]->Tpv[3] = fitpv.Tr().ty[iVec];
        t[iVec]->Tpv[4] = fitpv.Tr().qp[iVec];
        t[iVec]->Tpv[5] = fitpv.Tr().z[iVec];
        t[iVec]->Tpv[6] = fitpv.Tr().t[iVec];

        t[iVec]->Cpv[0]  = fitpv.Tr().C00[iVec];
        t[iVec]->Cpv[1]  = fitpv.Tr().C10[iVec];
        t[iVec]->Cpv[2]  = fitpv.Tr().C11[iVec];
        t[iVec]->Cpv[3]  = fitpv.Tr().C20[iVec];
        t[iVec]->Cpv[4]  = fitpv.Tr().C21[iVec];
        t[iVec]->Cpv[5]  = fitpv.Tr().C22[iVec];
        t[iVec]->Cpv[6]  = fitpv.Tr().C30[iVec];
        t[iVec]->Cpv[7]  = fitpv.Tr().C31[iVec];
        t[iVec]->Cpv[8]  = fitpv.Tr().C32[iVec];
        t[iVec]->Cpv[9]  = fitpv.Tr().C33[iVec];
        t[iVec]->Cpv[10] = fitpv.Tr().C40[iVec];
        t[iVec]->Cpv[11] = fitpv.Tr().C41[iVec];
        t[iVec]->Cpv[12] = fitpv.Tr().C42[iVec];
        t[iVec]->Cpv[13] = fitpv.Tr().C43[iVec];
        t[iVec]->Cpv[14] = fitpv.Tr().C44[iVec];
        t[iVec]->Cpv[15] = fitpv.Tr().C50[iVec];
        t[iVec]->Cpv[16] = fitpv.Tr().C51[iVec];
        t[iVec]->Cpv[17] = fitpv.Tr().C52[iVec];
        t[iVec]->Cpv[18] = fitpv.Tr().C53[iVec];
        t[iVec]->Cpv[19] = fitpv.Tr().C54[iVec];
        t[iVec]->Cpv[20] = fitpv.Tr().C55[iVec];
      }

      if (iter == 1) continue;  // only 1.5 iterations

      // fit forward

      ista = 0;

      FilterFirst(fit, x_first, y_first, time_first, dt2_first, d_xx_fst, d_yy_fst, d_xy_fst);

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

      for (int iVec = 0; iVec < nTracks_SIMD; iVec++) {
        t[iVec]->TLast[0] = tr.x[iVec];
        t[iVec]->TLast[1] = tr.y[iVec];
        t[iVec]->TLast[2] = tr.tx[iVec];
        t[iVec]->TLast[3] = tr.ty[iVec];
        t[iVec]->TLast[4] = tr.qp[iVec];
        if (kGlobal == fTrackingMode) { t[iVec]->TLast[4] = fitpv.Tr().qp[iVec]; }
        t[iVec]->TLast[5] = tr.z[iVec];
        t[iVec]->TLast[6] = tr.t[iVec];

        t[iVec]->CLast[0]  = tr.C00[iVec];
        t[iVec]->CLast[1]  = tr.C10[iVec];
        t[iVec]->CLast[2]  = tr.C11[iVec];
        t[iVec]->CLast[3]  = tr.C20[iVec];
        t[iVec]->CLast[4]  = tr.C21[iVec];
        t[iVec]->CLast[5]  = tr.C22[iVec];
        t[iVec]->CLast[6]  = tr.C30[iVec];
        t[iVec]->CLast[7]  = tr.C31[iVec];
        t[iVec]->CLast[8]  = tr.C32[iVec];
        t[iVec]->CLast[9]  = tr.C33[iVec];
        t[iVec]->CLast[10] = tr.C40[iVec];
        t[iVec]->CLast[11] = tr.C41[iVec];
        t[iVec]->CLast[12] = tr.C42[iVec];
        t[iVec]->CLast[13] = tr.C43[iVec];
        t[iVec]->CLast[14] = tr.C44[iVec];
        t[iVec]->CLast[15] = tr.C50[iVec];
        t[iVec]->CLast[16] = tr.C51[iVec];
        t[iVec]->CLast[17] = tr.C52[iVec];
        t[iVec]->CLast[18] = tr.C53[iVec];
        t[iVec]->CLast[19] = tr.C54[iVec];
        t[iVec]->CLast[20] = tr.C55[iVec];

        t[iVec]->chi2 = tr.chi2[iVec];
        t[iVec]->NDF  = (int) tr.NDF[iVec];
      }
    }  // iter
  }
}
void L1Algo::GuessVecNoField(L1Fit& track, fvec& x_last, fvec& x_2last, fvec& y_last, fvec& y_2last, fvec& z_end,
                             fvec& z_2last, fvec& time_last, fmask* /*w_time*/, fvec& dt2_last)
{
  L1TrackPar& tr = track.Tr();

  fvec dzi = fvec(1.) / (z_end - z_2last);

  tr.x    = x_last;
  tr.y    = y_last;
  tr.tx   = (x_last - x_2last) * dzi;
  tr.ty   = (y_last - y_2last) * dzi;
  tr.t    = time_last;
  tr.z    = z_end;
  tr.chi2 = fvec(0.);
  tr.NDF  = fvec(2.);

  tr.C20 = tr.C21 = fvec(0.);
  tr.C30 = tr.C31 = tr.C32 = fvec(0.);
  tr.C40 = tr.C41 = tr.C42 = tr.C43 = fvec(0.);
  tr.C50 = tr.C51 = tr.C52 = tr.C53 = tr.C54 = fvec(0.);
  tr.C22 = tr.C33 = vINF;
  tr.C44          = fvec(1.);
  tr.C55          = fvec(dt2_last);
}


void L1Algo::GuessVec(L1TrackPar& tr, fvec* xV, fvec* yV, fvec* zV, fvec* Sy, fmask* wV, int NHits, fvec* zCur)
// gives nice initial approximation for x,y,tx,ty - almost same as KF fit. qp - is shifted by 4%, resid_ual - ~3.5% (KF fit resid_ual - 1%).
{

  fvec A0, A1 = ZERO, A2 = ZERO, A3 = ZERO, A4 = ZERO, A5 = ZERO, a0, a1 = ZERO, a2 = ZERO, b0, b1 = ZERO, b2 = ZERO;
  fvec z0, x, y, z, S, w, wz, wS;

  int i = NHits - 1;
  if (zCur) z0 = *zCur;
  else
    z0 = zV[i];
  w  = iif(wV[i], fvec::One(), fvec::Zero());
  A0 = w;
  a0 = w * xV[i];
  b0 = w * yV[i];
  for (i = 0; i < NHits; i++) {
    x  = xV[i];
    y  = yV[i];
    w  = iif(wV[i], fvec::One(), fvec::Zero());
    z  = zV[i] - z0;
    S  = Sy[i];
    wz = w * z;
    wS = w * S;
    A0 += w;
    A1 += wz;
    A2 += wz * z;
    A3 += wS;
    A4 += wS * z;
    A5 += wS * S;
    a0 += w * x;
    a1 += wz * x;
    a2 += wS * x;
    b0 += w * y;
    b1 += wz * y;
    b2 += wS * y;
  }

  fvec A3A3 = A3 * A3;
  fvec A3A4 = A3 * A4;
  fvec A1A5 = A1 * A5;
  fvec A2A5 = A2 * A5;
  fvec A4A4 = A4 * A4;

  fvec det = fvec::One() / (-A2 * A3A3 + A1 * (A3A4 + A3A4 - A1A5) + A0 * (A2A5 - A4A4));
  fvec Ai0 = (-A4A4 + A2A5);
  fvec Ai1 = (A3A4 - A1A5);
  fvec Ai2 = (-A3A3 + A0 * A5);
  fvec Ai3 = (-A2 * A3 + A1 * A4);
  fvec Ai4 = (A1 * A3 - A0 * A4);
  fvec Ai5 = (-A1 * A1 + A0 * A2);

  fvec L, L1;
  tr.x       = (Ai0 * a0 + Ai1 * a1 + Ai3 * a2) * det;
  tr.tx      = (Ai1 * a0 + Ai2 * a1 + Ai4 * a2) * det;
  fvec txtx1 = fvec(1.) + tr.tx * tr.tx;
  L          = (Ai3 * a0 + Ai4 * a1 + Ai5 * a2) * det / txtx1;
  L1         = L * tr.tx;
  A1         = A1 + A3 * L1;
  A2         = A2 + (A4 + A4 + A5 * L1) * L1;
  b1 += b2 * L1;
  det = fvec::One() / (-A1 * A1 + A0 * A2);

  tr.y  = (A2 * b0 - A1 * b1) * det;
  tr.ty = (-A1 * b0 + A0 * b1) * det;
  tr.qp = -L * c_light_i / sqrt(txtx1 + tr.ty * tr.ty);
  tr.z  = z0;
}

void L1Algo::GuessVec(L1Fit& track, fvec* xV, fvec* yV, fvec* zV, fvec* Sy, fmask* wV, int NHits, fvec* zCur,
                      fvec* timeV, fmask* w_time)
// gives nice initial approximation for x,y,tx,ty - almost same as KF fit. qp - is shifted by 4%, resid_ual - ~3.5% (KF fit resid_ual - 1%).
{
  L1TrackPar& tr = track.Tr();

  fvec A0, A1 = ZERO, A2 = ZERO, A3 = ZERO, A4 = ZERO, A5 = ZERO, a0, a1 = ZERO, a2 = ZERO, b0, b1 = ZERO, b2 = ZERO;
  fvec z0, x, y, z, S, w, wz, wS, time;

  time = fvec(0.);

  int i = NHits - 1;
  if (zCur) { z0 = *zCur; }
  else {
    z0 = zV[i];
  }

  w  = iif(wV[i], fvec::One(), fvec::Zero());
  A0 = w;
  a0 = w * xV[i];
  b0 = w * yV[i];

  fvec nhits = 0;
  for (i = 0; i < NHits; i++) {
    x = xV[i];
    y = yV[i];
    w = iif(wV[i], fvec::One(), fvec::Zero());
    ;
    if (timeV) nhits(w_time[i]) += fvec::One();
    z = zV[i] - z0;
    S = Sy[i];

    wz = w * z;
    wS = w * S;

    A0 += w;
    A1 += wz;
    A2 += wz * z;
    A3 += wS;
    A4 += wS * z;
    A5 += wS * S;
    a0 += w * x;
    a1 += wz * x;
    a2 += wS * x;
    b0 += w * y;
    b1 += wz * y;
    b2 += wS * y;
    if (timeV) {
      fvec dx = xV[i] - fParameters.GetTargetPositionX();
      fvec dy = yV[i] - fParameters.GetTargetPositionY();
      fvec dz = zV[i] - fParameters.GetTargetPositionZ();
      time(w_time[i]) += (timeV[i] - sqrt(dx * dx + dy * dy + dz * dz) / fvec(30.));
    }
  }

  fvec A3A3 = A3 * A3;
  fvec A3A4 = A3 * A4;
  fvec A1A5 = A1 * A5;
  fvec A2A5 = A2 * A5;
  fvec A4A4 = A4 * A4;

  fvec det = fvec::One() / (-A2 * A3A3 + A1 * (A3A4 + A3A4 - A1A5) + A0 * (A2A5 - A4A4));
  fvec Ai0 = (-A4A4 + A2A5);
  fvec Ai1 = (A3A4 - A1A5);
  fvec Ai2 = (-A3A3 + A0 * A5);
  fvec Ai3 = (-A2 * A3 + A1 * A4);
  fvec Ai4 = (A1 * A3 - A0 * A4);
  fvec Ai5 = (-A1 * A1 + A0 * A2);

  fvec L, L1;
  tr.x       = (Ai0 * a0 + Ai1 * a1 + Ai3 * a2) * det;
  tr.tx      = (Ai1 * a0 + Ai2 * a1 + Ai4 * a2) * det;
  fvec txtx1 = fvec(1.) + tr.tx * tr.tx;
  L          = (Ai3 * a0 + Ai4 * a1 + Ai5 * a2) * det / txtx1;
  L1         = L * tr.tx;
  A1         = A1 + A3 * L1;
  A2         = A2 + (A4 + A4 + A5 * L1) * L1;
  b1 += b2 * L1;
  det   = fvec::One() / (-A1 * A1 + A0 * A2);
  tr.y  = (A2 * b0 - A1 * b1) * det;
  tr.ty = (-A1 * b0 + A0 * b1) * det;
  tr.qp = -L * c_light_i / sqrt(txtx1 + tr.ty * tr.ty);
  if (timeV) { tr.t = iif(nhits > fvec::Zero(), time / nhits, fvec::Zero()); }

  tr.z = z0;
}


void L1Algo::FilterFirst(L1Fit& track, fvec& x, fvec& y, fvec& t, fvec& dt2, fvec& d_xx, fvec& d_yy, fvec& d_xy)
{
  L1TrackPar& tr = track.Tr();

  tr.C00 = d_xx;
  tr.C10 = d_xy;
  tr.C11 = d_yy;
  tr.C20 = ZERO;
  tr.C21 = ZERO;
  tr.C22 = vINF;
  tr.C30 = ZERO;
  tr.C31 = ZERO;
  tr.C32 = ZERO;
  tr.C33 = vINF;
  tr.C40 = ZERO;
  tr.C41 = ZERO;
  tr.C42 = ZERO;
  tr.C43 = ZERO;
  tr.C44 = ONE;
  tr.C50 = ZERO;
  tr.C51 = ZERO;
  tr.C52 = ZERO;
  tr.C53 = ZERO;
  tr.C54 = ZERO;
  tr.C55 = dt2;

  tr.x    = x;
  tr.y    = y;
  tr.t    = t;
  tr.NDF  = -3.0;
  tr.chi2 = ZERO;
}
