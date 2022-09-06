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
  const fvec c_light = 0.000299792458, c_light_i = 1. / c_light;
  const fvec ZERO = 0.;
  const fvec ONE  = 1.;
  const fvec vINF = 0.1;
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

        fvec dzi = 1. / (z1 - z0);

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

  const int nHits = fParameters.GetNstationsActive();
  int iVec = 0, i = 0;
  int nTracks_SIMD = fvec::size();
  L1TrackPar T;  // fitting parametr coresponding to current track

  L1TrackParFit T1;  // fitting parametr coresponding to current track
  T1.SetParticleMass(GetDefaultParticleMass());

  L1Fit fit;
  fit.SetParticleMass(GetDefaultParticleMass());

  L1Track* t[fvec::size()];

  const L1Station* sta = fParameters.GetStations().begin();
  L1Station staFirst, staLast;  // FIXME (?): Probably, we should replace these variables with references (S.Zharko)

  // Spatial-time position of a hit vs. station and track in the portion
  // NOTE: u- and v-axes are axes, orthogonal to front and back strips of the station, respectively.
  fvec u[L1Constants::size::kMaxNstations];    // Hit position along the u-axis [cm]
  fvec v[L1Constants::size::kMaxNstations];    // Hit position along the v-axis [cm]
  fvec d_u[L1Constants::size::kMaxNstations];  // Hit position uncertainty along the u-axis [cm]
  fvec d_v[L1Constants::size::kMaxNstations];  // Hit position uncertainty along the v-axis [cm]

  fvec x[L1Constants::size::kMaxNstations];     // Hit position along the x-axis [cm]
  fvec y[L1Constants::size::kMaxNstations];     // Hit position along the y-axis [cm]
  fvec d_xx[L1Constants::size::kMaxNstations];  // Variance of the x hit position coordinate [cm2]
  fvec d_yy[L1Constants::size::kMaxNstations];  // Variance of the y hit position coordinate [cm2]
  fvec d_xy[L1Constants::size::kMaxNstations];  // Covariance between the x and y hit position coordinates [cm2]

  fvec z[L1Constants::size::kMaxNstations];  // Hit position along the z-axis (precised) [cm]

  fvec time[L1Constants::size::kMaxNstations];    // Hit time [ns]
  fvec timeEr[L1Constants::size::kMaxNstations];  // Hit time uncertainty [ns]

  fvec x_first;
  fvec y_first;
  fvec d_xx_fst;
  fvec d_yy_fst;
  fvec d_xy_fst;

  fvec time_first;
  fvec time_er_first;

  fvec x_last;
  fvec y_last;
  fvec d_xx_lst;
  fvec d_yy_lst;
  fvec d_xy_lst;

  fvec time_last;
  fvec time_er_last;
  fvec time_er_lst;  /// TODO: Why are there two different variables for the time error on the last station?

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
  for (int iHit = 0; iHit < nHits; iHit++) {
    ZSta[iHit] = sta[iHit].z;
  }

  unsigned short N_vTracks = fTracks.size();  // number of tracks processed per one SSE register

  for (unsigned short itrack = 0; itrack < N_vTracks; itrack += fvec::size()) {
    if (N_vTracks - itrack < static_cast<unsigned short>(fvec::size())) nTracks_SIMD = N_vTracks - itrack;

    for (i = 0; i < nTracks_SIMD; i++)
      t[i] = &fTracks[itrack + i];  // current track

    // get hits of current track
    for (i = 0; i < nHits; i++) {
      w[i]      = ZERO;
      w_time[i] = ZERO;
      z[i]      = ZSta[i];
    }

    for (iVec = 0; iVec < nTracks_SIMD; iVec++) {
      int nHitsTrack = t[iVec]->NHits;
      int iSta[L1Constants::size::kMaxNstations];
      for (i = 0; i < nHitsTrack; i++) {
        const L1Hit& hit = fInputData.GetHit(fRecoHits[start_hit++]);
        const int ista   = hit.iSt;
        iSta[i]          = ista;
        w[ista][iVec]    = 1.;
        if (ista > fNstationsBeforePipe) w_time[ista][iVec] = 1.;

        u[ista][iVec]            = hit.u;
        v[ista][iVec]            = hit.v;
        d_u[ista][iVec]          = hit.du;
        d_v[ista][iVec]          = hit.dv;
        std::tie(x_temp, y_temp) = sta[ista].ConvUVtoXY<fvec>(u[ista], v[ista]);
        x[ista][iVec]            = x_temp[iVec];
        y[ista][iVec]            = y_temp[iVec];
        time[ista][iVec]         = hit.t;
        timeEr[ista][iVec]       = hit.dt;
        z[ista][iVec]            = hit.z;
        sta[ista].fieldSlice.GetFieldValue(x[ista], y[ista], fB_temp);
        std::tie(d_xx[ista], d_xy[ista], d_yy[ista]) = sta[ista].FormXYCovarianceMatrix(d_u[ista], d_v[ista]);

        fB[ista].x[iVec] = fB_temp.x[iVec];
        fB[ista].y[iVec] = fB_temp.y[iVec];
        fB[ista].z[iVec] = fB_temp.z[iVec];
        if (i == 0) {
          z_start[iVec]             = z[ista][iVec];
          x_first[iVec]             = x[ista][iVec];
          y_first[iVec]             = y[ista][iVec];
          time_first[iVec]          = time[ista][iVec];
          time_er_first[iVec]       = timeEr[ista][iVec];
          d_xx_fst[iVec]            = d_xx[ista][iVec];
          d_yy_fst[iVec]            = d_yy[ista][iVec];
          d_xy_fst[iVec]            = d_xy[ista][iVec];
          staFirst.XYInfo.C00[iVec] = sta[ista].XYInfo.C00[iVec];
          staFirst.XYInfo.C10[iVec] = sta[ista].XYInfo.C10[iVec];
          staFirst.XYInfo.C11[iVec] = sta[ista].XYInfo.C11[iVec];
        }
        else if (i == nHitsTrack - 1) {
          z_end[iVec]              = z[ista][iVec];
          x_last[iVec]             = x[ista][iVec];
          y_last[iVec]             = y[ista][iVec];
          d_xx_lst[iVec]           = d_xx[ista][iVec];
          d_yy_lst[iVec]           = d_yy[ista][iVec];
          d_xy_lst[iVec]           = d_xy[ista][iVec];
          time_last[iVec]          = time[ista][iVec];
          time_er_last[iVec]       = timeEr[ista][iVec];
          staLast.XYInfo.C00[iVec] = sta[ista].XYInfo.C00[iVec];
          staLast.XYInfo.C10[iVec] = sta[ista].XYInfo.C10[iVec];
          staLast.XYInfo.C11[iVec] = sta[ista].XYInfo.C11[iVec];
        }
      }

      fscal prevZ = z_end[iVec];
      fscal cursy = 0., curSy = 0.;
      for (i = nHitsTrack - 1; i >= 0; i--) {
        const int ista      = iSta[i];
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


    //fit backward

    GuessVec(T, x, y, z, Sy, w, nHits, &z_end);
    GuessVec(T1, x, y, z, Sy, w, nHits, &z_end, time, w_time);

    for (int iter = 0; iter < 2; iter++) {  // 1.5 iterations

      fvec qp0 = T.qp;

      fvec qp01 = T1.fqp;

      i = nHits - 1;

      FilterFirst(T, x_last, y_last, staLast);
      // FilterFirst( T1, x_last, y_last, time_last, time_er_last, staLast );

      FilterFirst(T1, x_last, y_last, time_last, time_er_last, staLast, d_xx_lst, d_yy_lst, d_xy_lst);

      T1.Filter(time[i], timeEr[i], w_time[i], sta[i].timeInfo);

      // fit.L1AddMaterial( T, sta[i].materialInfo, qp0, 1 );

      fldZ1 = z[i];

      sta[i].fieldSlice.GetFieldValue(T.x, T.y, fldB1);


      fldB1.Combine(fB[i], w[i]);

      fldZ2   = z[i - 2];
      fvec dz = fldZ2 - fldZ1;
      sta[i].fieldSlice.GetFieldValue(T.x + T.tx * dz, T.y + T.ty * dz, fldB2);
      fldB2.Combine(fB[i - 2], w[i - 2]);
      fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);
      for (--i; i >= 0; i--) {

        fldZ0 = z[i];
        dz    = (fldZ1 - fldZ0);
        sta[i].fieldSlice.GetFieldValue(T.x - T.tx * dz, T.y - T.ty * dz, fldB0);
        fldB0.Combine(fB[i], w[i]);
        fld.Set(fldB0, fldZ0, fldB1, fldZ1, fldB2, fldZ2);

        fmask initialised = (z[i] < z_end) & (z_start <= z[i]);
        fvec w1           = iif(initialised, w[i], fvec::Zero());
        fvec w1_time      = iif(initialised, w_time[i], fvec::Zero());
        fvec wIn          = iif(initialised, fvec::One(), fvec::Zero());

        fld1 = fld;


        T1.Extrapolate(z[i], qp01, fld1, &w1);
        //  T1.ExtrapolateLine(z[i]);

        L1Extrapolate(T, z[i], qp0, fld, &w1);

        if (i == fNstationsBeforePipe - 1) {

          fit.L1AddPipeMaterial(T, qp0, wIn);
          fit.EnergyLossCorrection(T, fit.PipeRadThick, qp0, fvec(1.f), wIn);


          T1.L1AddPipeMaterial(qp01, wIn);
          T1.EnergyLossCorrection(fit.PipeRadThick, qp01, fvec(1.f), wIn);
        }
        if constexpr (L1Constants::control::kIfUseRadLengthTable) {
          fit.L1AddMaterial(T, fParameters.GetMaterialThickness(i, T.x, T.y), qp0, wIn);
          fit.EnergyLossCorrection(T, fParameters.GetMaterialThickness(i, T.x, T.y), qp0, fvec(1.f), wIn);

          T1.L1AddMaterial(fParameters.GetMaterialThickness(i, T1.fx, T1.fy), qp01, wIn);
          T1.EnergyLossCorrection(fParameters.GetMaterialThickness(i, T1.fx, T1.fy), qp01, fvec(1.f), wIn);
        }
        else {
          fit.L1AddMaterial(T, sta[i].materialInfo, qp0, wIn);
          T1.L1AddMaterial(sta[i].materialInfo, qp01, wIn);
        }
        L1UMeasurementInfo info = sta[i].frontInfo;
        info.sigma2             = d_u[i] * d_u[i];

        L1Filter(T, sta[i].frontInfo, u[i], w1);
        T1.Filter(info, u[i], w1);

        info        = sta[i].backInfo;
        info.sigma2 = d_v[i] * d_v[i];

        L1Filter(T, sta[i].backInfo, v[i], w1);
        T1.Filter(info, v[i], w1);

        T1.Filter(time[i], timeEr[i], w1_time, sta[i].timeInfo);


        fldB2 = fldB1;
        fldZ2 = fldZ1;
        fldB1 = fldB0;
        fldZ1 = fldZ0;
      }
      // fit.L1AddHalfMaterial( T, sta[i].materialInfo, qp0 );

      for (iVec = 0; iVec < nTracks_SIMD; iVec++) {
        t[iVec]->TFirst[0] = T1.fx[iVec];
        t[iVec]->TFirst[1] = T1.fy[iVec];
        t[iVec]->TFirst[2] = T1.ftx[iVec];
        t[iVec]->TFirst[3] = T1.fty[iVec];
        t[iVec]->TFirst[4] = T1.fqp[iVec];
        t[iVec]->TFirst[5] = T1.fz[iVec];
        t[iVec]->TFirst[6] = T1.ft[iVec];

        t[iVec]->CFirst[0]  = T1.C00[iVec];
        t[iVec]->CFirst[1]  = T1.C10[iVec];
        t[iVec]->CFirst[2]  = T1.C11[iVec];
        t[iVec]->CFirst[3]  = T1.C20[iVec];
        t[iVec]->CFirst[4]  = T1.C21[iVec];
        t[iVec]->CFirst[5]  = T1.C22[iVec];
        t[iVec]->CFirst[6]  = T1.C30[iVec];
        t[iVec]->CFirst[7]  = T1.C31[iVec];
        t[iVec]->CFirst[8]  = T1.C32[iVec];
        t[iVec]->CFirst[9]  = T1.C33[iVec];
        t[iVec]->CFirst[10] = T1.C40[iVec];
        t[iVec]->CFirst[11] = T1.C41[iVec];
        t[iVec]->CFirst[12] = T1.C42[iVec];
        t[iVec]->CFirst[13] = T1.C43[iVec];
        t[iVec]->CFirst[14] = T1.C44[iVec];
        t[iVec]->CFirst[15] = T1.C50[iVec];
        t[iVec]->CFirst[16] = T1.C51[iVec];
        t[iVec]->CFirst[17] = T1.C52[iVec];
        t[iVec]->CFirst[18] = T1.C53[iVec];
        t[iVec]->CFirst[19] = T1.C54[iVec];
        t[iVec]->CFirst[20] = T1.C55[iVec];

        t[iVec]->chi2 = T1.chi2[iVec];
        t[iVec]->NDF  = (int) T1.NDF[iVec];
      }

      // extrapolate to the PV region
      L1TrackParFit T1PV = T1;
      T1PV.Extrapolate(0.f, T1PV.fqp, fld);

      // T1PV.ExtrapolateLine(0.f);

      for (iVec = 0; iVec < nTracks_SIMD; iVec++) {
        t[iVec]->Tpv[0] = T1PV.fx[iVec];
        t[iVec]->Tpv[1] = T1PV.fy[iVec];
        t[iVec]->Tpv[2] = T1PV.ftx[iVec];
        t[iVec]->Tpv[3] = T1PV.fty[iVec];
        t[iVec]->Tpv[4] = T1PV.fqp[iVec];
        t[iVec]->Tpv[5] = T1PV.fz[iVec];
        t[iVec]->Tpv[6] = T1PV.ft[iVec];

        t[iVec]->Cpv[0]  = T1PV.C00[iVec];
        t[iVec]->Cpv[1]  = T1PV.C10[iVec];
        t[iVec]->Cpv[2]  = T1PV.C11[iVec];
        t[iVec]->Cpv[3]  = T1PV.C20[iVec];
        t[iVec]->Cpv[4]  = T1PV.C21[iVec];
        t[iVec]->Cpv[5]  = T1PV.C22[iVec];
        t[iVec]->Cpv[6]  = T1PV.C30[iVec];
        t[iVec]->Cpv[7]  = T1PV.C31[iVec];
        t[iVec]->Cpv[8]  = T1PV.C32[iVec];
        t[iVec]->Cpv[9]  = T1PV.C33[iVec];
        t[iVec]->Cpv[10] = T1PV.C40[iVec];
        t[iVec]->Cpv[11] = T1PV.C41[iVec];
        t[iVec]->Cpv[12] = T1PV.C42[iVec];
        t[iVec]->Cpv[13] = T1PV.C43[iVec];
        t[iVec]->Cpv[14] = T1PV.C44[iVec];
        t[iVec]->Cpv[15] = T1PV.C50[iVec];
        t[iVec]->Cpv[16] = T1PV.C51[iVec];
        t[iVec]->Cpv[17] = T1PV.C52[iVec];
        t[iVec]->Cpv[18] = T1PV.C53[iVec];
        t[iVec]->Cpv[19] = T1PV.C54[iVec];
        t[iVec]->Cpv[20] = T1PV.C55[iVec];
      }


      if (iter == 1) continue;  // only 1.5 iterations
      // fit forward

      i = 0;

      FilterFirst(T, x_first, y_first, staFirst);
      // FilterFirst( T1, x_first, y_first, time_first, time_er_first, staFirst);

      FilterFirst(T1, x_first, y_first, time_first, time_er_first, staFirst, d_xx_fst, d_yy_fst, d_xy_fst);

      T1.Filter(time[i], timeEr[i], w_time[i], sta[i].timeInfo);

      // fit.L1AddMaterial( T, sta[i].materialInfo, qp0, 1 );
      qp0  = T.qp;
      qp01 = T1.fqp;

      fldZ1 = z[i];
      sta[i].fieldSlice.GetFieldValue(T.x, T.y, fldB1);
      fldB1.Combine(fB[i], w[i]);

      fldZ2 = z[i + 2];
      dz    = fldZ2 - fldZ1;
      sta[i].fieldSlice.GetFieldValue(T.x + T.tx * dz, T.y + T.ty * dz, fldB2);
      fldB2.Combine(fB[i + 2], w[i + 2]);
      fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);

      for (++i; i < nHits; i++) {
        fldZ0 = z[i];
        dz    = (fldZ1 - fldZ0);
        sta[i].fieldSlice.GetFieldValue(T.x - T.tx * dz, T.y - T.ty * dz, fldB0);
        fldB0.Combine(fB[i], w[i]);
        fld.Set(fldB0, fldZ0, fldB1, fldZ1, fldB2, fldZ2);

        fmask initialised = (z[i] <= z_end) & (z_start < z[i]);
        fvec w1           = iif(initialised, w[i], fvec::Zero());
        fvec w1_time      = iif(initialised, w_time[i], fvec::Zero());
        fvec wIn          = iif(initialised, fvec::One(), fvec::Zero());

        L1Extrapolate(T, z[i], qp0, fld, &w1);

        // L1ExtrapolateLine( T, z[i]);

        T1.Extrapolate(z[i], qp0, fld, &w1);

        // T1.ExtrapolateLine( z[i]);

        if (i == fNstationsBeforePipe) {
          fit.L1AddPipeMaterial(T, qp0, wIn);
          fit.EnergyLossCorrection(T, fit.PipeRadThick, qp0, fvec(-1.f), wIn);

          T1.L1AddPipeMaterial(qp01, wIn);
          T1.EnergyLossCorrection(fit.PipeRadThick, qp01, fvec(-1.f), wIn);
        }
        if constexpr (L1Constants::control::kIfUseRadLengthTable) {
          fit.L1AddMaterial(T, fParameters.GetMaterialThickness(i, T.x, T.y), qp0, wIn);
          fit.EnergyLossCorrection(T, fParameters.GetMaterialThickness(i, T.x, T.y), qp0, fvec(-1.f), wIn);

          T1.L1AddMaterial(fParameters.GetMaterialThickness(i, T1.fx, T1.fy), qp01, wIn);
          T1.EnergyLossCorrection(fParameters.GetMaterialThickness(i, T1.fx, T1.fy), qp01, fvec(-1.f), wIn);
        }
        else {
          fit.L1AddMaterial(T, sta[i].materialInfo, qp0, wIn);
        }
        L1Filter(T, sta[i].frontInfo, u[i], w1);
        L1Filter(T, sta[i].backInfo, v[i], w1);

        L1UMeasurementInfo info = sta[i].frontInfo;
        info.sigma2             = d_u[i] * d_u[i];

        T1.Filter(info, u[i], w1);

        info        = sta[i].backInfo;
        info.sigma2 = d_v[i] * d_v[i];

        T1.Filter(info, v[i], w1);

        T1.Filter(time[i], timeEr[i], w1_time, sta[i].timeInfo);

        fldB2 = fldB1;
        fldZ2 = fldZ1;
        fldB1 = fldB0;
        fldZ1 = fldZ0;
      }
      // fit.L1AddHalfMaterial( T, sta[i].materialInfo, qp0 );

      for (iVec = 0; iVec < nTracks_SIMD; iVec++) {
        t[iVec]->TLast[0] = T1.fx[iVec];
        t[iVec]->TLast[1] = T1.fy[iVec];
        t[iVec]->TLast[2] = T1.ftx[iVec];
        t[iVec]->TLast[3] = T1.fty[iVec];
        t[iVec]->TLast[4] = T1.fqp[iVec];
        t[iVec]->TLast[5] = T1.fz[iVec];
        t[iVec]->TLast[6] = T1.ft[iVec];

        t[iVec]->CLast[0]  = T1.C00[iVec];
        t[iVec]->CLast[1]  = T1.C10[iVec];
        t[iVec]->CLast[2]  = T1.C11[iVec];
        t[iVec]->CLast[3]  = T1.C20[iVec];
        t[iVec]->CLast[4]  = T1.C21[iVec];
        t[iVec]->CLast[5]  = T1.C22[iVec];
        t[iVec]->CLast[6]  = T1.C30[iVec];
        t[iVec]->CLast[7]  = T1.C31[iVec];
        t[iVec]->CLast[8]  = T1.C32[iVec];
        t[iVec]->CLast[9]  = T1.C33[iVec];
        t[iVec]->CLast[10] = T1.C40[iVec];
        t[iVec]->CLast[11] = T1.C41[iVec];
        t[iVec]->CLast[12] = T1.C42[iVec];
        t[iVec]->CLast[13] = T1.C43[iVec];
        t[iVec]->CLast[14] = T1.C44[iVec];
        t[iVec]->CLast[15] = T1.C50[iVec];
        t[iVec]->CLast[16] = T1.C51[iVec];
        t[iVec]->CLast[17] = T1.C52[iVec];
        t[iVec]->CLast[18] = T1.C53[iVec];
        t[iVec]->CLast[19] = T1.C54[iVec];
        t[iVec]->CLast[20] = T1.C55[iVec];

        t[iVec]->chi2 = T1.chi2[iVec];
        t[iVec]->NDF  = (int) T1.NDF[iVec];
      }
    }  // iter
  }
}

void L1Algo::L1KFTrackFitterMuch()
{
  //  cout << " Start L1 Track Fitter " << endl;
  int start_hit = 0;  // for interation in fRecoHits[]

  //  static L1FieldValue fldB0, fldB1, fldB2 _fvecalignment;
  //  static L1FieldRegion fld _fvecalignment;
  L1FieldValue fldB0, fldB1, fldB2 _fvecalignment;
  L1FieldRegion fld _fvecalignment;


  L1FieldValue fldB01, fldB11, fldB21 _fvecalignment;
  L1FieldRegion fld1 _fvecalignment;

  const int nHits = fParameters.GetNstationsActive();
  int iVec = 0, i = 0;
  int nTracks_SIMD = fvec::size();
  L1TrackPar T;  // fitting parametr coresponding to current track

  L1TrackParFit T1;  // fitting parametr coresponding to current track
  T1.SetParticleMass(GetDefaultParticleMass());

  L1Fit fit;
  fit.SetParticleMass(GetDefaultParticleMass());

  L1Track* t[fvec::size()];

  const L1Station* sta = fParameters.GetStations().begin();
  L1Station staFirst, staLast;


  fvec u[L1Constants::size::kMaxNstations];
  fvec v[L1Constants::size::kMaxNstations];
  fvec d_u[L1Constants::size::kMaxNstations];
  fvec d_v[L1Constants::size::kMaxNstations];

  fvec x[L1Constants::size::kMaxNstations];
  fvec y[L1Constants::size::kMaxNstations];
  fvec d_xx[L1Constants::size::kMaxNstations];
  fvec d_yy[L1Constants::size::kMaxNstations];
  fvec d_xy[L1Constants::size::kMaxNstations];

  fvec z[L1Constants::size::kMaxNstations];

  fvec time[L1Constants::size::kMaxNstations];
  fvec timeEr[L1Constants::size::kMaxNstations];

  fvec x_first;
  fvec y_first;
  fvec d_xx_fst;
  fvec d_yy_fst;
  fvec d_xy_fst;

  fvec time_first;
  fvec time_er_fst;

  fvec x_last;
  fvec y_last;
  fvec d_xx_lst;
  fvec d_yy_lst;
  fvec d_xy_lst;

  fvec time_last;
  fvec time_er_lst;
  fvec dz;  /// !!!

  fvec Sy[L1Constants::size::kMaxNstations];
  fvec w[L1Constants::size::kMaxNstations];
  fvec y_temp;
  fvec x_temp;
  fvec fldZ0;
  fvec fldZ1;
  fvec fldZ2;
  fvec z_start;
  fvec z_end;


  L1FieldValue fB[L1Constants::size::kMaxNstations], fB_temp _fvecalignment;

  int iSta[L1Constants::size::kMaxNstations];  /// !!!
  fvec ZSta[L1Constants::size::kMaxNstations];
  for (int iHit = 0; iHit < nHits; iHit++) {
    ZSta[iHit] = sta[iHit].z;
  }

  unsigned short N_vTracks = fTracks.size();

  for (unsigned short itrack = 0; itrack < N_vTracks; itrack += fvec::size()) {
    if (N_vTracks - itrack < static_cast<unsigned short>(fvec::size())) nTracks_SIMD = N_vTracks - itrack;

    for (i = 0; i < nTracks_SIMD; i++)
      t[i] = &fTracks[itrack + i];  // current track

    // get hits of current track
    for (i = 0; i < nHits; i++) {
      w[i] = ZERO;
      z[i] = ZSta[i];
    }

    for (iVec = 0; iVec < nTracks_SIMD; iVec++) {
      for (i = 0; i < fParameters.GetNstationsActive(); i++) {
        d_xx[i][iVec] = 0;
        d_yy[i][iVec] = 0;
      }
    }

    for (iVec = 0; iVec < nTracks_SIMD; iVec++) {
      int nHitsTrack      = t[iVec]->NHits;
      int nHitsTrackField = 0;
      for (i = 0; i < nHitsTrack; i++) {
        const L1Hit& hit = fInputData.GetHit(fRecoHits[start_hit++]);
        const int ista   = hit.iSt;
        if (ista < fNfieldStations) nHitsTrackField++;
        iSta[i]       = ista;
        w[ista][iVec] = 1.;

        d_xx[i][iVec] = 0;
        d_yy[i][iVec] = 0;

        u[ista][iVec]                                = hit.u;
        v[ista][iVec]                                = hit.v;
        std::tie(x_temp, y_temp)                     = sta[ista].ConvUVtoXY<fvec>(u[ista], v[ista]);
        x[ista][iVec]                                = x_temp[iVec];
        y[ista][iVec]                                = y_temp[iVec];
        time[ista][iVec]                             = hit.t;
        timeEr[ista][iVec]                           = hit.dt;
        d_u[ista][iVec]                              = hit.du;
        d_v[ista][iVec]                              = hit.dv;
        std::tie(d_xx[ista], d_xy[ista], d_yy[ista]) = sta[ista].FormXYCovarianceMatrix(d_u[ista], d_v[ista]);

        //  mom[ista][iVec] = hit.p;
        z[ista][iVec] = hit.z;
        sta[ista].fieldSlice.GetFieldValue(x[ista], y[ista], fB_temp);
        fB[ista].x[iVec] = fB_temp.x[iVec];
        fB[ista].y[iVec] = fB_temp.y[iVec];
        fB[ista].z[iVec] = fB_temp.z[iVec];
        if (i == 0) {
          z_start[iVec]             = z[ista][iVec];
          x_first[iVec]             = x[ista][iVec];
          y_first[iVec]             = y[ista][iVec];
          time_first[iVec]          = time[ista][iVec];
          time_er_fst[iVec]         = timeEr[ista][iVec];
          d_xx_fst[iVec]            = d_xx[ista][iVec];
          d_yy_fst[iVec]            = d_yy[ista][iVec];
          d_xy_fst[iVec]            = d_xy[ista][iVec];
          staFirst.XYInfo.C00[iVec] = sta[ista].XYInfo.C00[iVec];
          staFirst.XYInfo.C10[iVec] = sta[ista].XYInfo.C10[iVec];
          staFirst.XYInfo.C11[iVec] = sta[ista].XYInfo.C11[iVec];
        }
        else if (i == nHitsTrack - 1) {
          z_end[iVec]              = z[ista][iVec];
          x_last[iVec]             = x[ista][iVec];
          y_last[iVec]             = y[ista][iVec];
          time_last[iVec]          = time[ista][iVec];
          time_er_lst[iVec]        = timeEr[ista][iVec];
          d_xx_lst[iVec]           = d_xx[ista][iVec];
          d_yy_lst[iVec]           = d_yy[ista][iVec];
          d_xy_lst[iVec]           = d_xy[ista][iVec];
          staLast.XYInfo.C00[iVec] = sta[ista].XYInfo.C00[iVec];
          staLast.XYInfo.C10[iVec] = sta[ista].XYInfo.C10[iVec];
          staLast.XYInfo.C11[iVec] = sta[ista].XYInfo.C11[iVec];
        }
      }

      // fscal prevZ = z_end[iVec];
      fscal prevZ = z_start[iVec];
      fscal cursy = 0., curSy = 0.;
      //   for(i = nHitsTrack - 1; i >= 0; i-- ){
      for (i = 0; i < nHitsTrackField; i++) {
        const int ista      = iSta[i];
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

    //fit backward

    int nHitsField = 0;

    for (i = 0; i < nHits; i++)
      if (iSta[i] < fNfieldStations) nHitsField++;

    GuessVec(T, x, y, z, Sy, w, nHitsField, &z_start);


    GuessVec(T1, x, y, z, Sy, w, nHitsField, &z_start);


    for (int iter = 0; iter < 2; iter++) {  // 1.5 iterations

      fvec qp0 = T.qp;

      fvec qp01 = T1.fqp;

      i = 0;

      FilterFirst(T, x_first, y_first, staFirst);

      FilterFirst(T1, x_first, y_first, time_first, time_er_fst, staFirst, d_xx_fst, d_yy_fst, d_xy_fst);

      fldZ1 = z[i];

      sta[i].fieldSlice.GetFieldValue(T.x, T.y, fldB1);

      fldB1.Combine(fB[i], w[i]);

      fldZ2 = z[i + 2];
      dz    = fldZ2 - fldZ1;
      sta[i].fieldSlice.GetFieldValue(T.x + T.tx * dz, T.y + T.ty * dz, fldB2);
      fldB2.Combine(fB[i + 2], w[i + 2]);

      fld.Set(fldB2, fldZ2, fldB1, fldZ1, fldB0, fldZ0);

      for (++i; i < nHits; i++) {
        fmask initialised = (z[i] <= z_end) & (z_start < z[i]);
        fvec w1           = iif(initialised, w[i], fvec::Zero());
        fvec wIn          = iif(initialised, fvec::One(), fvec::Zero());

        fldZ0 = z[i];
        dz    = (fldZ1 - fldZ0);


        if (i < 8) {

          sta[i].fieldSlice.GetFieldValue(T.x - T.tx * dz, T.y - T.ty * dz, fldB0);
          fldB0.Combine(fB[i], w[i]);
          fld.Set(fldB0, fldZ0, fldB1, fldZ1, fldB2, fldZ2);

          L1Extrapolate(T, z[i], qp0, fld, &w1);

          T1.Extrapolate(z[i], qp01, fld, &w1);

          if (i == fNstationsBeforePipe) {
            fit.L1AddPipeMaterial(T, qp0, wIn);
            fit.EnergyLossCorrection(T, fit.PipeRadThick, qp0, fvec(-1.f), wIn);

            T1.L1AddPipeMaterial(qp01, wIn);
            T1.EnergyLossCorrection(fit.PipeRadThick, qp01, fvec(-1.f), wIn);
          }

          fldB2 = fldB1;
          fldZ2 = fldZ1;
          fldB1 = fldB0;
          fldZ1 = fldZ0;

          if constexpr (L1Constants::control::kIfUseRadLengthTable) {
            T1.EnergyLossCorrection(fParameters.GetMaterialThickness(i, T1.fx, T1.fy), qp01, fvec(-1.f), wIn);

            T1.L1AddThickMaterial(fParameters.GetMaterialThickness(i, T1.fx, T1.fy), qp01, wIn,
                                  sta[i].materialInfo.thick, 1);
          }
          else {
            //         L1AddMaterial( T, sta[i].materialInfo, qp0, wIn );
          }

          L1UMeasurementInfo info = sta[i].frontInfo;
          info.sigma2             = d_xx[i];
          L1Filter(T, info, u[i], w1);
          T1.Filter(info, u[i], w1);

          info        = sta[i].backInfo;
          info.sigma2 = d_yy[i];

          L1Filter(T, info, v[i], w1);
          T1.Filter(info, v[i], w1);


          T1.Filter(time[i], timeEr[i], w1, sta[i].timeInfo);
        }

        if (i >= 8) {

          fvec z_last = z[i];

          //     T1.ExtrapolateLine( T1.fz + 10, &w1);
          //          L1ExtrapolateLine( T, T.z + 10);

          fvec d_z = T1.fz - z_last;

          const fvec st = fvec(10);

          fvec nofSteps = (abs(d_z) / 10);

          int max_steps = 0;

          for (int j = 0; j < 4; j++) {
            nofSteps[j] = int(fabs(d_z[j]) / 10);
            if (max_steps < nofSteps[j]) max_steps = nofSteps[j];
          }


          fvec nofSteps1 = fvec::Zero();

          fvec stepSize = iif(nofSteps < fvec::One(), d_z, st) * w1;
          fvec z_cur    = T1.fz;
          fvec w2       = w1;


          for (int iStep = 0; iStep < max_steps + 1; iStep++) {

            const fmask maskLastStep = (nofSteps == nofSteps1);
            z_cur                    = iif(maskLastStep, z_last, T1.fz + stepSize);

            //  fvec v_mc = fabs(1/qp01)/sqrt(mass2+fabs(1/qp01)*fabs(1/qp01));
            // T1.ExtrapolateLine1( z, &w2, v_mc);

            T1.ExtrapolateLine(z_cur, &w2);
            nofSteps1 += iif(!maskLastStep, fvec::One(), fvec::Zero());
            w2 = iif(!maskLastStep, w2, fvec::Zero());

            //          T1.ExtrapolateLine( z_last, &w1);
            //          L1ExtrapolateLine( T, z_last);
            //          TODO: Unify energy loss corrections (S.Zharko)
            if constexpr (L1Constants::control::kIfUseRadLengthTable) {
              if (i == 11 || i == 14 || i == 17)
                T1.EnergyLossCorrectionIron(fParameters.GetMaterialThickness(i, T1.fx, T1.fy) / (nofSteps + 1), qp01,
                                            fvec(-1.f), wIn);
              if (i == 8)
                T1.EnergyLossCorrectionCarbon(fParameters.GetMaterialThickness(i, T1.fx, T1.fy) / (nofSteps + 1), qp01,
                                              fvec(-1.f), wIn);
              if (i == 9 || i == 10 || i == 12 || i == 13 || i == 15 || i == 16 || i == 18 || i == 19)
                T1.EnergyLossCorrectionAl(fParameters.GetMaterialThickness(i, T1.fx, T1.fy) / (nofSteps + 1), qp01,
                                          fvec(-1.f), wIn);

              T1.L1AddThickMaterial(fParameters.GetMaterialThickness(i, T1.fx, T1.fy) / (nofSteps + fvec(1)), qp01, wIn,
                                    sta[i].materialInfo.thick / (nofSteps + fvec(1)), 1);

              wIn = iif(!maskLastStep, wIn, fvec::Zero());
            }
            else {
              fit.L1AddMaterial(T, sta[i].materialInfo, qp0, wIn);
            }
          }

          //         T1.ExtrapolateLine( z_last, &w1);
          //
          L1UMeasurementInfo info = sta[i].frontInfo;
          info.sigma2             = d_xx[i];
          L1Filter(T, info, u[i], w1);
          T1.Filter(info, u[i], w1);

          info        = sta[i].backInfo;
          info.sigma2 = d_yy[i];
          L1Filter(T, info, v[i], w1);
          T1.Filter(info, v[i], w1);

          T1.Filter(time[i], timeEr[i], w1, sta[i].timeInfo);
        }
      }
      // fit.L1AddHalfMaterial( T, sta[i].materialInfo, qp0 );

      for (iVec = 0; iVec < nTracks_SIMD; iVec++) {

        t[iVec]->TLast[0] = T1.fx[iVec];
        t[iVec]->TLast[1] = T1.fy[iVec];
        t[iVec]->TLast[2] = T1.ftx[iVec];
        t[iVec]->TLast[3] = T1.fty[iVec];
        t[iVec]->TLast[4] = T1.fqp[iVec];
        t[iVec]->TLast[5] = T1.fz[iVec];
        t[iVec]->TLast[6] = T1.ft[iVec];

        t[iVec]->CLast[0]  = T1.C00[iVec];
        t[iVec]->CLast[1]  = T1.C10[iVec];
        t[iVec]->CLast[2]  = T1.C11[iVec];
        t[iVec]->CLast[3]  = T1.C20[iVec];
        t[iVec]->CLast[4]  = T1.C21[iVec];
        t[iVec]->CLast[5]  = T1.C22[iVec];
        t[iVec]->CLast[6]  = T1.C30[iVec];
        t[iVec]->CLast[7]  = T1.C31[iVec];
        t[iVec]->CLast[8]  = T1.C32[iVec];
        t[iVec]->CLast[9]  = T1.C33[iVec];
        t[iVec]->CLast[10] = T1.C40[iVec];
        t[iVec]->CLast[11] = T1.C41[iVec];
        t[iVec]->CLast[12] = T1.C42[iVec];
        t[iVec]->CLast[13] = T1.C43[iVec];
        t[iVec]->CLast[14] = T1.C44[iVec];
        t[iVec]->CLast[15] = T1.C50[iVec];
        t[iVec]->CLast[16] = T1.C51[iVec];
        t[iVec]->CLast[17] = T1.C52[iVec];
        t[iVec]->CLast[18] = T1.C53[iVec];
        t[iVec]->CLast[19] = T1.C54[iVec];
        t[iVec]->CLast[20] = T1.C55[iVec];


        t[iVec]->chi2 = T1.chi2[iVec];
        t[iVec]->NDF  = (int) T1.NDF[iVec];
      }

      if (iter == 1) continue;  // only 1.5 iterations
      // fit backward

      i = nHits - 1;  //0;

      FilterFirst(T, x_last, y_last, staLast);

      FilterFirstL(T1, x_last, y_last, time_last, time_er_lst, staLast, d_xx_lst, d_yy_lst, d_xy_lst);

      qp0  = T.qp;
      qp01 = T1.fqp;


      //       fvec initialised = fvec(z[i] < z_end) & fvec(z_start <= z[i]);
      //       fvec wIn = (ONE  & (initialised));

      //       T1.EnergyLossCorrectionAl( fParameters.GetMaterialThickness(i, T1.fx, T1.fy), qp01, fvec(1.f), wIn);
      //
      //       T1.L1AddThickMaterial(fParameters.GetMaterialThickness(i, T1.fx, T1.fy), qp01, ONE, sta[i].materialInfo.thick, 0);

      for (--i; i >= 0; i--) {

        fmask initialised = (z[i] < z_end) & (z_start <= z[i]);
        fvec w1           = iif(initialised, w[i], fvec::Zero());
        fvec wIn          = iif(initialised, fvec::One(), fvec::Zero());

        if (i >= fNfieldStations - 1) {

          fvec z_last = z[i];

          //          T1.ExtrapolateLine( T1.fz + 10, &w1);
          //          L1ExtrapolateLine( T, T.z + 10);


          fvec d_z      = T1.fz - z_last;
          const fvec st = fvec(10);
          fvec nofSteps = (abs(d_z) / 10);

          int max_steps = 0;

          for (size_t j = 0; j < fvec::size(); j++) {
            nofSteps[j] = int(fabs(d_z[j]) / 10);  //*w1[i];
            if (max_steps < nofSteps[j]) max_steps = nofSteps[j];
          }

          fvec nofSteps1 = fvec(0);
          fvec stepSize  = wIn * iif((nofSteps < fvec::One()), d_z, st);
          fvec z_cur     = T1.fz;
          fvec w2        = wIn;

          for (int iStep = 0; iStep < max_steps + 1; iStep++) {

            const fmask maskLastStep = (nofSteps == nofSteps1);
            z_cur                    = iif(maskLastStep, z_last, T1.fz - stepSize);

            //               fvec v_mc = fabs(1/qp01)/sqrt(mass2+fabs(1/qp01)*fabs(1/qp01));
            //               T1.ExtrapolateLine1( z_cur, &w2, v_mc);

            T1.ExtrapolateLine(z_cur, &w2);
            nofSteps1 += iif(!maskLastStep, fvec::One(), fvec::Zero());

            // TODO: Unify the selection of energy loss correction! (S.Zharko)
            if constexpr (L1Constants::control::kIfUseRadLengthTable) {
              if (i == 11 || i == 14 || i == 17)
                T1.EnergyLossCorrectionIron(fParameters.GetMaterialThickness(i, T1.fx, T1.fy) / (nofSteps + 1), qp01,
                                            fvec(1.f), w2);
              if (i == 8)
                T1.EnergyLossCorrectionCarbon(fParameters.GetMaterialThickness(i, T1.fx, T1.fy) / (nofSteps + 1), qp01,
                                              fvec(1.f), w2);
              if (i == 9 || i == 10 || i == 12 || i == 13 || i == 15 || i == 16 || i == 18)
                T1.EnergyLossCorrectionAl(fParameters.GetMaterialThickness(i, T1.fx, T1.fy) / (nofSteps + 1), qp01,
                                          fvec(1.f), w2);

              T1.L1AddThickMaterial(fParameters.GetMaterialThickness(i, T1.fx, T1.fy) / (nofSteps + fvec(1)), qp01, w2,
                                    sta[i].materialInfo.thick / (nofSteps + fvec(1)), 0);

              w2 = iif(!maskLastStep, w2, fvec::Zero());
            }
            else {
              fit.L1AddMaterial(T, sta[i].materialInfo, qp0, w2);
            }
          }

          //fvec v_mc = fabs(1/qp01)/sqrt(mass2+fabs(1/qp01)*fabs(1/qp01));
          //  T1.ExtrapolateLine1( z_last, &wIn, v_mc);

          T1.ExtrapolateLine(z_last, &wIn);
          L1ExtrapolateLine(T, z_last);


          L1UMeasurementInfo info = sta[i].frontInfo;
          info.sigma2             = d_xx[i];

          L1Filter(T, info, u[i], w1);
          T1.Filter(info, u[i], w1);

          info        = sta[i].backInfo;
          info.sigma2 = d_yy[i];

          L1Filter(T, info, v[i], w1);
          T1.Filter(info, v[i], w1);

          T1.Filter(time[i], timeEr[i], w1, sta[i].timeInfo);
        }

        if (i < fNfieldStations - 1) {

          if (i == fNfieldStations - 2) {  // next-to-last field station

            T1.ExtrapolateLine(z[i + 1]);

            fldZ1 = z[i + 1];
            sta[i + 1].fieldSlice.GetFieldValue(T1.fx, T1.fy, fldB1);
            fldB1.Combine(fB[i + 1], w[i + 1]);

            fldZ2 = z[i - 1];
            dz    = fldZ2 - fldZ1;

            sta[i + 1].fieldSlice.GetFieldValue(T1.fx + T1.ftx * dz, T1.fy + T1.fty * dz, fldB2);
            fldB2.Combine(fB[i - 1], w[i - 1]);
          }


          fldZ0 = z[i];
          dz    = (fldZ1 - fldZ0);
          sta[i].fieldSlice.GetFieldValue(T1.fx - T1.ftx * dz, T1.fy - T1.fty * dz, fldB0);
          fldB0.Combine(fB[i], w[i]);

          fld.Set(fldB0, fldZ0, fldB1, fldZ1, fldB2, fldZ2);

          // fvec v_mc = fabs(1/qp01)/sqrt(mass2+fabs(1/qp01)*fabs(1/qp01));

          T1.Extrapolate(z[i], qp01, fld, &w1);
          //          L1Extrapolate( T, z[i], qp0, fld,&w1 );


          if constexpr (L1Constants::control::kIfUseRadLengthTable) {
            T1.EnergyLossCorrection(fParameters.GetMaterialThickness(i, T1.fx, T1.fy), qp01, fvec(1.f), wIn);
            T1.L1AddThickMaterial(fParameters.GetMaterialThickness(i, T1.fx, T1.fy), qp01, wIn,
                                  sta[i].materialInfo.thick, 0);
          }
          else {
            fit.L1AddMaterial(T, sta[i].materialInfo, qp0, wIn);
          }

          L1UMeasurementInfo info = sta[i].frontInfo;
          //   info.sigma2 = d_u[i] * d_u[i];
          T1.Filter(info, u[i], w1);

          info = sta[i].backInfo;
          //   info.sigma2 = d_v[i] * d_v[i];
          T1.Filter(info, v[i], w1);

          T1.Filter(time[i], timeEr[i], w1, sta[i].timeInfo);


          fldB2 = fldB1;
          fldZ2 = fldZ1;
          fldB1 = fldB0;
          fldZ1 = fldZ0;
        }
      }
      // fit.L1AddHalfMaterial( T, sta[i].materialInfo, qp0 );


      for (iVec = 0; iVec < nTracks_SIMD; iVec++) {

        t[iVec]->TFirst[0] = T1.fx[iVec];
        t[iVec]->TFirst[1] = T1.fy[iVec];
        t[iVec]->TFirst[2] = T1.ftx[iVec];
        t[iVec]->TFirst[3] = T1.fty[iVec];
        t[iVec]->TFirst[4] = T1.fqp[iVec];
        t[iVec]->TFirst[5] = T1.fz[iVec];
        t[iVec]->TFirst[6] = T1.ft[iVec];

        t[iVec]->CFirst[0]  = T1.C00[iVec];
        t[iVec]->CFirst[1]  = T1.C10[iVec];
        t[iVec]->CFirst[2]  = T1.C11[iVec];
        t[iVec]->CFirst[3]  = T1.C20[iVec];
        t[iVec]->CFirst[4]  = T1.C21[iVec];
        t[iVec]->CFirst[5]  = T1.C22[iVec];
        t[iVec]->CFirst[6]  = T1.C30[iVec];
        t[iVec]->CFirst[7]  = T1.C31[iVec];
        t[iVec]->CFirst[8]  = T1.C32[iVec];
        t[iVec]->CFirst[9]  = T1.C33[iVec];
        t[iVec]->CFirst[10] = T1.C40[iVec];
        t[iVec]->CFirst[11] = T1.C41[iVec];
        t[iVec]->CFirst[12] = T1.C42[iVec];
        t[iVec]->CFirst[13] = T1.C43[iVec];
        t[iVec]->CFirst[14] = T1.C44[iVec];
        t[iVec]->CFirst[15] = T1.C50[iVec];
        t[iVec]->CFirst[16] = T1.C51[iVec];
        t[iVec]->CFirst[17] = T1.C52[iVec];
        t[iVec]->CFirst[18] = T1.C53[iVec];
        t[iVec]->CFirst[19] = T1.C54[iVec];
        t[iVec]->CFirst[20] = T1.C55[iVec];

        t[iVec]->chi2 = T1.chi2[iVec];
        t[iVec]->NDF  = (int) T1.NDF[iVec];
      }


      // extrapolate to the PV region
      L1TrackParFit T1PV = T1;
      T1PV.Extrapolate(0.f, T1PV.fqp, fld);
      for (iVec = 0; iVec < nTracks_SIMD; iVec++) {
        t[iVec]->Tpv[0] = T1PV.fx[iVec];
        t[iVec]->Tpv[1] = T1PV.fy[iVec];
        t[iVec]->Tpv[2] = T1PV.ftx[iVec];
        t[iVec]->Tpv[3] = T1PV.fty[iVec];
        t[iVec]->Tpv[4] = T1PV.fqp[iVec];
        t[iVec]->Tpv[5] = T1PV.fz[iVec];
        t[iVec]->Tpv[6] = T1PV.ft[iVec];

        t[iVec]->Cpv[0]  = T1PV.C00[iVec];
        t[iVec]->Cpv[1]  = T1PV.C10[iVec];
        t[iVec]->Cpv[2]  = T1PV.C11[iVec];
        t[iVec]->Cpv[3]  = T1PV.C20[iVec];
        t[iVec]->Cpv[4]  = T1PV.C21[iVec];
        t[iVec]->Cpv[5]  = T1PV.C22[iVec];
        t[iVec]->Cpv[6]  = T1PV.C30[iVec];
        t[iVec]->Cpv[7]  = T1PV.C31[iVec];
        t[iVec]->Cpv[8]  = T1PV.C32[iVec];
        t[iVec]->Cpv[9]  = T1PV.C33[iVec];
        t[iVec]->Cpv[10] = T1PV.C40[iVec];
        t[iVec]->Cpv[11] = T1PV.C41[iVec];
        t[iVec]->Cpv[12] = T1PV.C42[iVec];
        t[iVec]->Cpv[13] = T1PV.C43[iVec];
        t[iVec]->Cpv[14] = T1PV.C44[iVec];
        t[iVec]->Cpv[15] = T1PV.C50[iVec];
        t[iVec]->Cpv[16] = T1PV.C51[iVec];
        t[iVec]->Cpv[17] = T1PV.C52[iVec];
        t[iVec]->Cpv[18] = T1PV.C53[iVec];
        t[iVec]->Cpv[19] = T1PV.C54[iVec];
        t[iVec]->Cpv[20] = T1PV.C55[iVec];
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
  fvec txtx1 = 1. + t.tx * t.tx;
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

  time = 0;

  int i = NHits - 1;
  if (zCur) z0 = *zCur;
  else
    z0 = zV[i];
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
    if (timeV) time += w_time[i] * (timeV[i] - sqrt(x * x + y * y + z * z) / 30.f);
  }
  time = time / nhits;

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
  t.fx       = (Ai0 * a0 + Ai1 * a1 + Ai3 * a2) * det;
  t.ftx      = (Ai1 * a0 + Ai2 * a1 + Ai4 * a2) * det;
  fvec txtx1 = 1. + t.ftx * t.ftx;
  L          = (Ai3 * a0 + Ai4 * a1 + Ai5 * a2) * det / txtx1;
  L1         = L * t.ftx;
  A1         = A1 + A3 * L1;
  A2         = A2 + (A4 + A4 + A5 * L1) * L1;
  b1 += b2 * L1;
  det = fvec::One() / (-A1 * A1 + A0 * A2);

  t.fy  = (A2 * b0 - A1 * b1) * det;
  t.fty = (-A1 * b0 + A0 * b1) * det;
  t.fqp = -L * c_light_i / sqrt(txtx1 + t.fty * t.fty);
  if (timeV) { t.ft = iif(nhits > fvec::Zero(), time, fvec::Zero()); }

  t.fz = z0;
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

  track.x    = x;
  track.y    = y;
  track.NDF  = -3.0;
  track.chi2 = ZERO;
}

void L1Algo::FilterFirst(L1TrackParFit& track, fvec& x, fvec& y, fvec& t, L1Station& st)
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
  track.C54 = ZERO;
  track.C55 = 2.6f * 2.6f;

  track.fx   = x;
  track.fy   = y;
  track.ft   = t;
  track.NDF  = -3.0;
  track.chi2 = ZERO;
}
void L1Algo::FilterFirst(L1TrackParFit& track, fvec& x, fvec& y, fvec& t, fvec& dt, L1Station& st, fvec& /*d_xx*/,
                         fvec& /*d_yy*/, fvec& /*d_xy*/)
{
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
  track.C54 = ZERO;
  track.C55 = dt * dt;

  track.fx   = x;
  track.fy   = y;
  track.ft   = t;
  track.NDF  = -3.0;
  track.chi2 = ZERO;
}

void L1Algo::FilterFirst(L1TrackParFit& track, fvec& x, fvec& y, fvec& /*t*/, fvec& dt, L1Station& st)
{
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
  track.C54 = ZERO;
  track.C55 = dt * dt;

  track.fx   = x;
  track.fy   = y;
  track.NDF  = -3.0;
  track.chi2 = ZERO;
}


void L1Algo::FilterFirstL(L1TrackParFit& track, fvec& x, fvec& y, fvec& /*t*/, fvec& dt, L1Station& /*st*/, fvec& d_xx,
                          fvec& d_yy, fvec& d_xy)
{
  // initialize covariance matrix
  track.C00 = d_xx;
  track.C10 = d_xy;
  track.C11 = d_yy;
  //   track.C00= st.XYInfo.C00;
  //   track.C10= st.XYInfo.C10;
  //   track.C11= st.XYInfo.C11;
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
  track.C54 = ZERO;
  track.C55 = dt * dt;

  track.fx = x;
  track.fy = y;
  //  track.ft = t;
  track.NDF  = -3.0;  // TODO: Why -3.0? (S.Zharko)
  track.chi2 = ZERO;
}
