/* Copyright (C) 2010-2020 Frankfurt Institute for Advanced Studies, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ivan Kisel, Sergey Gorbunov, Igor Kulakov [committer], Valentina Akishina, Maksym Zyzak */

#include <iostream>
#include <vector>

#include "L1Algo.h"
#include "L1Extrapolation.h"
#include "L1Filtration.h"  // for KFTrackFitter_simple
#include "L1Fit.h"
#include "L1TrackPar.h"
#include "L1TrackParFit.h"

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

/// Fit reconstracted track like it fitted d_uring the reconstruction.
void L1Algo::KFTrackFitter_simple()  // TODO: Add pipe.
{
  //  cout << " Start KF Track Fitter " << endl;
  int start_hit = 0;  // for interation in fRecoHits[]
  for (unsigned int itrack = 0; itrack < fTracks.size(); itrack++) {
    L1Track& t = fTracks[itrack];  // current track

    // get hits of current track
    std::vector<unsigned short int> hits;  // array of indeses of hits of current track
    hits.clear();
    int nHits = t.NHits;
    for (int i = 0; i < nHits; i++) {
      hits.push_back(fRecoHits[start_hit++]);
    }

    L1TrackPar T;  // fitting parametr coresponding to current track

    L1Fit fit;
    fit.SetParticleMass(GetDefaultParticleMass());

    fvec qp0(0.25);
    //fvec qp0 = 2./t.Momentum;
    for (int iter = 0; iter < 3; iter++) {
      //cout<<" Back 1"<<endl;
      {  // fit backward
        const L1Hit& hit0 = fInputData.GetHit(hits[nHits - 1]);
        const L1Hit& hit1 = fInputData.GetHit(hits[nHits - 2]);
        const L1Hit& hit2 = fInputData.GetHit(hits[nHits - 3]);

        int ista0 = hit0.iSt;
        int ista1 = hit1.iSt;
        int ista2 = hit2.iSt;


        //cout<<"back: ista012="<<ista0<<" "<<ista1<<" "<<ista2<<endl;
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
        auto [x2, y2] = sta1.ConvUVtoXY<fvec>(u2, v2);
        // fvec z2 = hit2.z;

        fvec dzi = fvec(1.) / (z1 - z0);

        //        const fvec vINF = .1;
        T.x = x0;
        T.y = y0;
        if (iter == 0) {
          T.tx = (x1 - x0) * dzi;
          T.ty = (y1 - y0) * dzi;
        }

        T.qp   = qp0;
        T.z    = z0;
        T.chi2 = fvec(0.);
        T.NDF  = fvec(2.);
        T.C00  = sta0.XYInfo.C00;
        T.C10  = sta0.XYInfo.C10;
        T.C11  = sta0.XYInfo.C11;

        T.C20 = T.C21 = fvec(0.);
        T.C30 = T.C31 = T.C32 = fvec(0.);
        T.C40 = T.C41 = T.C42 = T.C43 = fvec(0.);
        T.C22 = T.C33 = vINF;
        T.C44         = fvec(1.);

        //        static L1FieldValue fldB0, fldB1, fldB2 _fvecalignment;
        //        static L1FieldRegion fld _fvecalignment;
        L1FieldValue fldB0, fldB1, fldB2 _fvecalignment;
        L1FieldRegion fld _fvecalignment;
        fvec fldZ0 = sta1.z;  // suppose field is smoth
        fvec fldZ1 = sta2.z;
        fvec fldZ2 = sta0.z;

        sta1.fieldSlice.GetFieldValue(x1, y1, fldB0);
        sta2.fieldSlice.GetFieldValue(x2, y2, fldB1);
        sta0.fieldSlice.GetFieldValue(x0, y0, fldB2);

        fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);

        int ista = ista2;
        //cout<<"\nfit, iter=:"<<iter<<endl;
        for (int i = nHits - 2; i >= 0; i--) {
          //  if( fabs(T.qp[0])>2. ) break;  // iklm. Don't know it need for
          const L1Hit& hit = fInputData.GetHit(hits[i]);
          ista             = hit.iSt;

          const L1Station& sta = fParameters.GetStation(ista);

          //    L1Extrapolate( T, hit.z, qp0, fld );
          L1ExtrapolateLine(T, hit.z);
          //           T.L1Extrapolate( sta.z, qp0, fld );
          //         L1Extrapolate( T, hit.z, qp0, fld );
          if constexpr (L1Constants::control::kIfUseRadLengthTable) {
            fit.L1AddMaterial(T, fParameters.GetMaterialThickness(i, T.x, T.y), qp0, ONE);
          }
          else {
            fit.L1AddMaterial(T, sta.materialInfo, qp0, ONE);
          }

          //         if (ista == fNstationsBeforePipe - 1) fit.L1AddPipeMaterial( T, qp0, 1);

          fvec u      = hit.u;
          fvec v      = hit.v;
          auto [x, y] = sta.ConvUVtoXY<fvec>(u, v);
          L1Filter(T, sta.frontInfo, u);
          L1Filter(T, sta.backInfo, v);
          fldB0 = fldB1;
          fldB1 = fldB2;
          fldZ0 = fldZ1;
          fldZ1 = fldZ2;
          sta.fieldSlice.GetFieldValue(x, y, fldB2);

          fldZ2 = sta.z;
          fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);
        }  // i

        // write received parametres in track
        t.TFirst[0] = T.x[0];
        t.TFirst[1] = T.y[0];
        t.TFirst[2] = T.tx[0];
        t.TFirst[3] = T.ty[0];
        t.TFirst[4] = T.qp[0];
        t.TFirst[5] = T.z[0];

        t.CFirst[0]  = T.C00[0];
        t.CFirst[1]  = T.C10[0];
        t.CFirst[2]  = T.C11[0];
        t.CFirst[3]  = T.C20[0];
        t.CFirst[4]  = T.C21[0];
        t.CFirst[5]  = T.C22[0];
        t.CFirst[6]  = T.C30[0];
        t.CFirst[7]  = T.C31[0];
        t.CFirst[8]  = T.C32[0];
        t.CFirst[9]  = T.C33[0];
        t.CFirst[10] = T.C40[0];
        t.CFirst[11] = T.C41[0];
        t.CFirst[12] = T.C42[0];
        t.CFirst[13] = T.C43[0];
        t.CFirst[14] = T.C44[0];

        t.chi2 = T.chi2[0];
        t.NDF  = static_cast<int>(T.NDF[0]);
        qp0    = T.qp;
      }  // fit backward

      // fit forward
      {
        //T.qp = first_trip->GetQpOrig(MaxInvMom);
        const L1Hit& hit0 = fInputData.GetHit(hits[0]);
        const L1Hit& hit1 = fInputData.GetHit(hits[1]);
        const L1Hit& hit2 = fInputData.GetHit(hits[2]);

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
        // fvec z1 = hit1.z;

        fvec u2       = hit2.u;
        fvec v2       = hit2.v;
        auto [x2, y2] = sta2.ConvUVtoXY<fvec>(u2, v2);
        //  fvec z2 = hit2.z;

        //   fvec dzi = 1./(z1-z0);

        //fvec qp0 = first_trip->GetQpOrig(MaxInvMom);

        //        const fvec vINF = .1;
        T.chi2 = fvec(0.);
        T.NDF  = fvec(2.);
        T.x    = x0;
        T.y    = y0;
        //         T.tx = (x1-x0)*dzi;
        //         T.ty = (y1-y0)*dzi;
        //         qp0 = 0;
        T.qp  = qp0;
        T.z   = z0;
        T.C00 = sta0.XYInfo.C00;
        T.C10 = sta0.XYInfo.C10;
        T.C11 = sta0.XYInfo.C11;
        T.C20 = T.C21 = 0;
        T.C30 = T.C31 = T.C32 = 0;
        T.C40 = T.C41 = T.C42 = T.C43 = 0;
        T.C22 = T.C33 = vINF;
        T.C44         = 1.;

        //        static L1FieldValue fldB0, fldB1, fldB2 _fvecalignment;
        //        static L1FieldRegion fld _fvecalignment;
        L1FieldValue fldB0, fldB1, fldB2 _fvecalignment;
        L1FieldRegion fld _fvecalignment;
        fvec fldZ0 = sta1.z;
        fvec fldZ1 = sta2.z;
        fvec fldZ2 = sta0.z;

        sta1.fieldSlice.GetFieldValue(x1, y1, fldB0);
        sta2.fieldSlice.GetFieldValue(x2, y2, fldB1);
        sta0.fieldSlice.GetFieldValue(x0, y0, fldB2);

        fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);
        int ista = ista2;

        for (int i = 1; i < nHits; i++) {
          const L1Hit& hit     = fInputData.GetHit(hits[i]);
          ista                 = hit.iSt;
          const L1Station& sta = fParameters.GetStation(ista);
          fvec u               = hit.u;
          fvec v               = hit.v;
          auto [x, y]          = sta.ConvUVtoXY<fvec>(u, v);

          //   L1Extrapolate( T, hit.z, qp0, fld );
          L1ExtrapolateLine(T, hit.z);
          //           T.L1Extrapolate( sta.z, qp0, fld );
          //           L1Extrapolate( T, hit.z, qp0, fld );
          if constexpr (L1Constants::control::kIfUseRadLengthTable) {
            fit.L1AddMaterial(T, fParameters.GetMaterialThickness(i, T.x, T.y), qp0, ONE);
          }
          else {
            fit.L1AddMaterial(T, sta.materialInfo, qp0, ONE);
          }
          //           if (ista == fNstationsBeforePipe) fit.L1AddPipeMaterial( T, qp0, 1);
          L1Filter(T, sta.frontInfo, u);
          L1Filter(T, sta.backInfo, v);

          fldB0 = fldB1;
          fldB1 = fldB2;
          fldZ0 = fldZ1;
          fldZ1 = fldZ2;
          sta.fieldSlice.GetFieldValue(x, y, fldB2);
          fldZ2 = sta.z;
          fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);
        }

        // write received parametres in track
        t.TLast[0] = T.x[0];
        t.TLast[1] = T.y[0];
        t.TLast[2] = T.tx[0];
        t.TLast[3] = T.ty[0];
        t.TLast[4] = T.qp[0];
        t.TLast[5] = T.z[0];

        t.CLast[0]  = T.C00[0];
        t.CLast[1]  = T.C10[0];
        t.CLast[2]  = T.C11[0];
        t.CLast[3]  = T.C20[0];
        t.CLast[4]  = T.C21[0];
        t.CLast[5]  = T.C22[0];
        t.CLast[6]  = T.C30[0];
        t.CLast[7]  = T.C31[0];
        t.CLast[8]  = T.C32[0];
        t.CLast[9]  = T.C33[0];
        t.CLast[10] = T.C40[0];
        t.CLast[11] = T.C41[0];
        t.CLast[12] = T.C42[0];
        t.CLast[13] = T.C43[0];
        t.CLast[14] = T.C44[0];

        t.chi2 += T.chi2[0];
        t.NDF += static_cast<int>(T.NDF[0]);
      }
      qp0 = T.qp;
    }
  }  // for(int itrack
}

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

  L1TrackParFit fit;  // fitting parametr coresponding to current track
  L1TrackPar& tr = fit.fTr;

  fit.SetParticleMass(GetDefaultParticleMass());

  L1Track* t[fvec::size()] {nullptr};

  const L1Station* sta = fParameters.GetStations().begin();
  L1Station staFirst, staLast;  // FIXME (?): Probably, we should replace these variables with references (S.Zharko)

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

  fvec x_last;
  fvec y_last;
  fvec d_xx_lst;
  fvec d_yy_lst;
  fvec d_xy_lst;

  fvec time_last;
  fvec dt2_last;
  //  fvec dt2_lst;  /// TODO: Why are there two different variables for the time error on the last station?

  fvec Sy[L1Constants::size::kMaxNstations];
  fvec w[L1Constants::size::kMaxNstations];
  fvec w_time[L1Constants::size::kMaxNstations];  // !!!
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
    ZSta[ista] = sta[ista].z;
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
      w[ista]      = ZERO;
      w_time[ista] = ZERO;
      z[ista]      = ZSta[ista];
    }

    for (int iVec = 0; iVec < nTracks_SIMD; iVec++) {

      int nHitsTrack = t[iVec]->NHits;
      int iSta[L1Constants::size::kMaxNstations];

      for (int ih = 0; ih < nHitsTrack; ih++) {

        const L1Hit& hit = fInputData.GetHit(fRecoHits[start_hit++]);
        const int ista   = hit.iSt;
        iSta[ih]         = ista;
        w[ista][iVec]    = 1.;
        if (sta[ista].timeInfo) { w_time[ista][iVec] = 1.; }

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
          z_start[iVec]             = z[ista][iVec];
          x_first[iVec]             = x[ista][iVec];
          y_first[iVec]             = y[ista][iVec];
          time_first[iVec]          = time[ista][iVec];
          dt2_first[iVec]           = dt2[ista][iVec];
          d_xx_fst[iVec]            = d_xx[ista][iVec];
          d_yy_fst[iVec]            = d_yy[ista][iVec];
          d_xy_fst[iVec]            = d_xy[ista][iVec];
          staFirst.XYInfo.C00[iVec] = sta[ista].XYInfo.C00[iVec];
          staFirst.XYInfo.C10[iVec] = sta[ista].XYInfo.C10[iVec];
          staFirst.XYInfo.C11[iVec] = sta[ista].XYInfo.C11[iVec];
        }
        else if (ih == nHitsTrack - 1) {
          z_end[iVec]              = z[ista][iVec];
          x_last[iVec]             = x[ista][iVec];
          y_last[iVec]             = y[ista][iVec];
          d_xx_lst[iVec]           = d_xx[ista][iVec];
          d_yy_lst[iVec]           = d_yy[ista][iVec];
          d_xy_lst[iVec]           = d_xy[ista][iVec];
          time_last[iVec]          = time[ista][iVec];
          dt2_last[iVec]           = dt2[ista][iVec];
          staLast.XYInfo.C00[iVec] = sta[ista].XYInfo.C00[iVec];
          staLast.XYInfo.C10[iVec] = sta[ista].XYInfo.C10[iVec];
          staLast.XYInfo.C11[iVec] = sta[ista].XYInfo.C11[iVec];
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

    GuessVec(fit, x, y, z, Sy, w, nStations, &z_end, time, w_time);

    if (kGlobal == fTrackingMode || kMcbm == fTrackingMode) { tr.qp = fvec(0.); }

    for (int iter = 0; iter < 2; iter++) {  // 1.5 iterations

      fvec qp01 = tr.qp;

      // fit backward

      int ista = nStations - 1;

      time_last = iif(w_time[ista] > fvec::Zero(), time_last, fvec::Zero());
      dt2_last  = iif(w_time[ista] > fvec::Zero(), dt2_last, fvec(100.));

      FilterFirst(fit, x_last, y_last, time_last, dt2_last, staLast, d_xx_lst, d_yy_lst, d_xy_lst);

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
        fvec w1           = iif(initialised, w[ista], fvec::Zero());
        fvec wExtr        = iif(initialised, fvec::One(), fvec::Zero());
        fvec w1_time      = iif(initialised, w_time[ista], fvec::Zero());

        fld1 = fld;

        fit.Extrapolate(z[ista], qp01, fld1, wExtr);

        //if (ista == fNstationsBeforePipe - 1) {
        //fit.AddPipeMaterial(qp01, wExtr);
        //fit.EnergyLossCorrection(fit.fPipeRadThick, qp01, fvec(1.f), wExtr);
        //}
        if constexpr (L1Constants::control::kIfUseRadLengthTable) {
          fit.AddMaterial(fParameters.GetMaterialThickness(ista, tr.x, tr.y), qp01, wExtr);
          fit.EnergyLossCorrection(fParameters.GetMaterialThickness(ista, tr.x, tr.y), qp01, fvec(1.f), wExtr);
        }
        else {
          fit.AddMaterial(sta[ista].materialInfo, qp01, wExtr);
        }

        fit.Filter(sta[ista].frontInfo, u[ista], du2[ista], w1);
        fit.Filter(sta[ista].backInfo, v[ista], dv2[ista], w1);
        fit.FilterTime(time[ista], dt2[ista], w1_time, sta[ista].timeInfo);

        fldB2 = fldB1;
        fldZ2 = fldZ1;
        fldB1 = fldB0;
        fldZ1 = fldZ0;
      }

      // extrapolate to the PV region

      L1TrackParFit fitpv = fit;
      {
        L1UMeasurementInfo vtxInfoX;
        vtxInfoX.cos_phi = 1.;
        vtxInfoX.sin_phi = 0.;
        vtxInfoX.sigma2  = fvec(1.e-4);

        L1UMeasurementInfo vtxInfoY;
        vtxInfoY.cos_phi = 0;
        vtxInfoY.sin_phi = 1.;
        vtxInfoY.sigma2  = fvec(1.e-4);

        if (kGlobal == fTrackingMode) {
          fvec qp00 = tr.qp;
          for (int vtxIter = 0; vtxIter < 2; vtxIter++) {
            fitpv        = fit;
            fitpv.fTr.qp = qp00;
            fitpv.Extrapolate(fParameters.GetTargetPositionZ(), fitpv.fTr.qp, fld, fvec(1.));
            fitpv.Filter(vtxInfoX, fParameters.GetTargetPositionX(), fvec(1.e-8), fvec::One());
            fitpv.Filter(vtxInfoY, fParameters.GetTargetPositionY(), fvec(1.e-8), fvec::One());
            qp00 = fitpv.fTr.qp;
          }
        }
        else {
          fitpv = fit;
          fitpv.Extrapolate(fParameters.GetTargetPositionZ(), fitpv.fTr.qp, fld, fvec(1.));
        }
      }

      const L1TrackPar& Tf = fit.fTr;

      for (int iVec = 0; iVec < nTracks_SIMD; iVec++) {
        t[iVec]->TFirst[0] = Tf.x[iVec];
        t[iVec]->TFirst[1] = Tf.y[iVec];
        t[iVec]->TFirst[2] = Tf.tx[iVec];
        t[iVec]->TFirst[3] = Tf.ty[iVec];
        t[iVec]->TFirst[4] = Tf.qp[iVec];
        if (kGlobal == fTrackingMode) { t[iVec]->TFirst[4] = fitpv.fTr.qp[iVec]; }
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
        t[iVec]->Tpv[0] = fitpv.fTr.x[iVec];
        t[iVec]->Tpv[1] = fitpv.fTr.y[iVec];
        t[iVec]->Tpv[2] = fitpv.fTr.tx[iVec];
        t[iVec]->Tpv[3] = fitpv.fTr.ty[iVec];
        t[iVec]->Tpv[4] = fitpv.fTr.qp[iVec];
        t[iVec]->Tpv[5] = fitpv.fTr.z[iVec];
        t[iVec]->Tpv[6] = fitpv.fTr.t[iVec];

        t[iVec]->Cpv[0]  = fitpv.fTr.C00[iVec];
        t[iVec]->Cpv[1]  = fitpv.fTr.C10[iVec];
        t[iVec]->Cpv[2]  = fitpv.fTr.C11[iVec];
        t[iVec]->Cpv[3]  = fitpv.fTr.C20[iVec];
        t[iVec]->Cpv[4]  = fitpv.fTr.C21[iVec];
        t[iVec]->Cpv[5]  = fitpv.fTr.C22[iVec];
        t[iVec]->Cpv[6]  = fitpv.fTr.C30[iVec];
        t[iVec]->Cpv[7]  = fitpv.fTr.C31[iVec];
        t[iVec]->Cpv[8]  = fitpv.fTr.C32[iVec];
        t[iVec]->Cpv[9]  = fitpv.fTr.C33[iVec];
        t[iVec]->Cpv[10] = fitpv.fTr.C40[iVec];
        t[iVec]->Cpv[11] = fitpv.fTr.C41[iVec];
        t[iVec]->Cpv[12] = fitpv.fTr.C42[iVec];
        t[iVec]->Cpv[13] = fitpv.fTr.C43[iVec];
        t[iVec]->Cpv[14] = fitpv.fTr.C44[iVec];
        t[iVec]->Cpv[15] = fitpv.fTr.C50[iVec];
        t[iVec]->Cpv[16] = fitpv.fTr.C51[iVec];
        t[iVec]->Cpv[17] = fitpv.fTr.C52[iVec];
        t[iVec]->Cpv[18] = fitpv.fTr.C53[iVec];
        t[iVec]->Cpv[19] = fitpv.fTr.C54[iVec];
        t[iVec]->Cpv[20] = fitpv.fTr.C55[iVec];
      }

      if (iter == 1) continue;  // only 1.5 iterations

      // fit forward

      ista = 0;

      FilterFirst(fit, x_first, y_first, time_first, dt2_first, staFirst, d_xx_fst, d_yy_fst, d_xy_fst);

      qp01 = tr.qp;

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
        fvec w1           = iif(initialised, w[ista], fvec::Zero());
        fvec w1_time      = iif(initialised, w_time[ista], fvec::Zero());
        fvec wExtr        = iif(initialised, fvec::One(), fvec::Zero());

        fit.Extrapolate(z[ista], qp01, fld, w1);

        //if (ista == fNstationsBeforePipe) {
        //fit.AddPipeMaterial(qp01, wExtr);
        //fit.EnergyLossCorrection(fit.fPipeRadThick, qp01, fvec(-1.f), wExtr);
        //}
        if constexpr (L1Constants::control::kIfUseRadLengthTable) {
          fit.AddMaterial(fParameters.GetMaterialThickness(ista, tr.x, tr.y), qp01, wExtr);
          fit.EnergyLossCorrection(fParameters.GetMaterialThickness(ista, tr.x, tr.y), qp01, fvec(-1.f), wExtr);
        }
        else {
          fit.AddMaterial(sta[ista].materialInfo, qp01, wExtr);
        }

        fit.Filter(sta[ista].frontInfo, u[ista], du2[ista], w1);
        fit.Filter(sta[ista].backInfo, v[ista], dv2[ista], w1);
        fit.FilterTime(time[ista], dt2[ista], w1_time, sta[ista].timeInfo);

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
        if (kGlobal == fTrackingMode) { t[iVec]->TLast[4] = fitpv.fTr.qp[iVec]; }
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


void L1Algo::GuessVec(L1TrackPar& t, fvec* xV, fvec* yV, fvec* zV, fvec* Sy, fvec* wV, int NHits, fvec* zCur)
// gives nice initial approximation for x,y,tx,ty - almost same as KF fit. qp - is shifted by 4%, resid_ual - ~3.5% (KF fit resid_ual - 1%).
{
  fvec A0, A1 = ZERO, A2 = ZERO, A3 = ZERO, A4 = ZERO, A5 = ZERO, a0, a1 = ZERO, a2 = ZERO, b0, b1 = ZERO, b2 = ZERO;
  fvec z0, x, y, z, S, w, wz, wS;

  int i = NHits - 1;
  if (zCur) z0 = *zCur;
  else
    z0 = zV[i];
  w  = wV[i];
  A0 = w;
  a0 = w * xV[i];
  b0 = w * yV[i];
  for (i = 0; i < NHits; i++) {
    x  = xV[i];
    y  = yV[i];
    w  = wV[i];
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
  t.x        = (Ai0 * a0 + Ai1 * a1 + Ai3 * a2) * det;
  t.tx       = (Ai1 * a0 + Ai2 * a1 + Ai4 * a2) * det;
  fvec txtx1 = fvec(1.) + t.tx * t.tx;
  L          = (Ai3 * a0 + Ai4 * a1 + Ai5 * a2) * det / txtx1;
  L1         = L * t.tx;
  A1         = A1 + A3 * L1;
  A2         = A2 + (A4 + A4 + A5 * L1) * L1;
  b1 += b2 * L1;
  det = fvec::One() / (-A1 * A1 + A0 * A2);

  t.y  = (A2 * b0 - A1 * b1) * det;
  t.ty = (-A1 * b0 + A0 * b1) * det;
  t.qp = -L * c_light_i / sqrt(txtx1 + t.ty * t.ty);
  t.z  = z0;
}

void L1Algo::GuessVec(L1TrackParFit& t, fvec* xV, fvec* yV, fvec* zV, fvec* Sy, fvec* wV, int NHits, fvec* zCur,
                      fvec* timeV, fvec* w_time)
// gives nice initial approximation for x,y,tx,ty - almost same as KF fit. qp - is shifted by 4%, resid_ual - ~3.5% (KF fit resid_ual - 1%).
{
  fvec A0, A1 = ZERO, A2 = ZERO, A3 = ZERO, A4 = ZERO, A5 = ZERO, a0, a1 = ZERO, a2 = ZERO, b0, b1 = ZERO, b2 = ZERO;
  fvec z0, x, y, z, S, w, wz, wS, time;

  time = fvec(0.);

  int i = NHits - 1;
  if (zCur) { z0 = *zCur; }
  else {
    z0 = zV[i];
  }

  w  = wV[i];
  A0 = w;
  a0 = w * xV[i];
  b0 = w * yV[i];

  fvec nhits = 0;
  for (i = 0; i < NHits; i++) {
    x = xV[i];
    y = yV[i];
    w = wV[i];
    if (timeV) nhits = nhits + w_time[i];
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
    if (timeV) {
      fvec dx = xV[i] - fParameters.GetTargetPositionX();
      fvec dy = yV[i] - fParameters.GetTargetPositionY();
      fvec dz = zV[i] - fParameters.GetTargetPositionZ();
      time += w_time[i] * (timeV[i] - sqrt(dx * dx + dy * dy + dz * dz) / fvec(30.));
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
  t.fTr.x    = (Ai0 * a0 + Ai1 * a1 + Ai3 * a2) * det;
  t.fTr.tx   = (Ai1 * a0 + Ai2 * a1 + Ai4 * a2) * det;
  fvec txtx1 = fvec(1.) + t.fTr.tx * t.fTr.tx;
  L          = (Ai3 * a0 + Ai4 * a1 + Ai5 * a2) * det / txtx1;
  L1         = L * t.fTr.tx;
  A1         = A1 + A3 * L1;
  A2         = A2 + (A4 + A4 + A5 * L1) * L1;
  b1 += b2 * L1;
  det = fvec::One() / (-A1 * A1 + A0 * A2);

  t.fTr.y  = (A2 * b0 - A1 * b1) * det;
  t.fTr.ty = (-A1 * b0 + A0 * b1) * det;
  t.fTr.qp = -L * c_light_i / sqrt(txtx1 + t.fTr.ty * t.fTr.ty);
  if (timeV) { t.fTr.t = iif(nhits > fvec::Zero(), time / nhits, fvec::Zero()); }

  t.fTr.z = z0;
}

void L1Algo::FilterFirst(L1TrackPar& track, fvec& x, fvec& y, L1Station& st)
{
  // initialize covariance matrix
  track.C00 = st.XYInfo.C00;
  track.C10 = st.XYInfo.C10;
  track.C11 = st.XYInfo.C11;
  track.C20 = ZERO;
  track.C21 = ZERO;
  track.C22 = vINF;
  track.C30 = ZERO;
  track.C31 = ZERO;
  track.C32 = ZERO;
  track.C33 = vINF;
  track.C40 = ZERO;
  track.C41 = ZERO;
  track.C42 = ZERO;
  track.C43 = ZERO;
  track.C44 = ONE;
  track.C50 = ZERO;
  track.C51 = ZERO;
  track.C52 = ZERO;
  track.C53 = ZERO;
  track.C55 = 2.6f * 2.6f;

  track.x    = x;
  track.y    = y;
  track.NDF  = -3.0;
  track.chi2 = ZERO;
}

void L1Algo::FilterFirst(L1TrackParFit& track, fvec& x, fvec& y, fvec& t, L1Station& st)
{
  // initialize covariance matrix
  track.fTr.C00 = st.XYInfo.C00;
  track.fTr.C10 = st.XYInfo.C10;
  track.fTr.C11 = st.XYInfo.C11;
  track.fTr.C20 = ZERO;
  track.fTr.C21 = ZERO;
  track.fTr.C22 = vINF;
  track.fTr.C30 = ZERO;
  track.fTr.C31 = ZERO;
  track.fTr.C32 = ZERO;
  track.fTr.C33 = vINF;
  track.fTr.C40 = ZERO;
  track.fTr.C41 = ZERO;
  track.fTr.C42 = ZERO;
  track.fTr.C43 = ZERO;
  track.fTr.C44 = ONE;
  track.fTr.C50 = ZERO;
  track.fTr.C51 = ZERO;
  track.fTr.C52 = ZERO;
  track.fTr.C53 = ZERO;
  track.fTr.C54 = ZERO;
  track.fTr.C55 = 2.6f * 2.6f;

  track.fTr.x    = x;
  track.fTr.y    = y;
  track.fTr.t    = t;
  track.fTr.NDF  = -3.0;
  track.fTr.chi2 = ZERO;
}


void L1Algo::FilterFirst(L1TrackParFit& track, fvec& x, fvec& y, fvec& t, fvec& dt2, L1Station& st, fvec& /*d_xx*/,
                         fvec& /*d_yy*/, fvec& /*d_xy*/)
{
  track.fTr.C00 = st.XYInfo.C00;
  track.fTr.C10 = st.XYInfo.C10;
  track.fTr.C11 = st.XYInfo.C11;
  track.fTr.C20 = ZERO;
  track.fTr.C21 = ZERO;
  track.fTr.C22 = vINF;
  track.fTr.C30 = ZERO;
  track.fTr.C31 = ZERO;
  track.fTr.C32 = ZERO;
  track.fTr.C33 = vINF;
  track.fTr.C40 = ZERO;
  track.fTr.C41 = ZERO;
  track.fTr.C42 = ZERO;
  track.fTr.C43 = ZERO;
  track.fTr.C44 = ONE;
  track.fTr.C50 = ZERO;
  track.fTr.C51 = ZERO;
  track.fTr.C52 = ZERO;
  track.fTr.C53 = ZERO;
  track.fTr.C54 = ZERO;
  track.fTr.C55 = dt2;

  track.fTr.x    = x;
  track.fTr.y    = y;
  track.fTr.t    = t;
  track.fTr.NDF  = -3.0;
  track.fTr.chi2 = ZERO;
}


void L1Algo::FilterFirst(L1TrackParFit& track, fvec& x, fvec& y, fvec& /*t*/, fvec& dt2, L1Station& st)
{
  track.fTr.C00 = st.XYInfo.C00;
  track.fTr.C10 = st.XYInfo.C10;
  track.fTr.C11 = st.XYInfo.C11;
  track.fTr.C20 = ZERO;
  track.fTr.C21 = ZERO;
  track.fTr.C22 = vINF;
  track.fTr.C30 = ZERO;
  track.fTr.C31 = ZERO;
  track.fTr.C32 = ZERO;
  track.fTr.C33 = vINF;
  track.fTr.C40 = ZERO;
  track.fTr.C41 = ZERO;
  track.fTr.C42 = ZERO;
  track.fTr.C43 = ZERO;
  track.fTr.C44 = ONE;
  track.fTr.C50 = ZERO;
  track.fTr.C51 = ZERO;
  track.fTr.C52 = ZERO;
  track.fTr.C53 = ZERO;
  track.fTr.C54 = ZERO;
  track.fTr.C55 = dt2;

  track.fTr.x    = x;
  track.fTr.y    = y;
  track.fTr.NDF  = -3.0;
  track.fTr.chi2 = ZERO;
}

void L1Algo::FilterFirstL(L1TrackParFit& track, fvec& x, fvec& y, fvec& /*t*/, fvec& dt2, L1Station& /*st*/, fvec& d_xx,
                          fvec& d_yy, fvec& d_xy)
{
  // initialize covariance matrix
  track.fTr.C00 = d_xx;
  track.fTr.C10 = d_xy;
  track.fTr.C11 = d_yy;
  //   track.fTr.C00= st.XYInfo.C00;
  //   track.fTr.C10= st.XYInfo.C10;
  //   track.fTr.C11= st.XYInfo.C11;
  track.fTr.C20 = ZERO;
  track.fTr.C21 = ZERO;
  track.fTr.C22 = vINF;
  track.fTr.C30 = ZERO;
  track.fTr.C31 = ZERO;
  track.fTr.C32 = ZERO;
  track.fTr.C33 = vINF;
  track.fTr.C40 = ZERO;
  track.fTr.C41 = ZERO;
  track.fTr.C42 = ZERO;
  track.fTr.C43 = ZERO;
  track.fTr.C44 = ONE;
  track.fTr.C50 = ZERO;
  track.fTr.C51 = ZERO;
  track.fTr.C52 = ZERO;
  track.fTr.C53 = ZERO;
  track.fTr.C54 = ZERO;
  track.fTr.C55 = dt2;

  track.fTr.x = x;
  track.fTr.y = y;
  //  track.fTr.t = t;
  track.fTr.NDF  = -3.0;  // TODO: Why -3.0? (S.Zharko)
  track.fTr.chi2 = ZERO;
}
