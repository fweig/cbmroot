/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak [committer], Valentina Akishina */

#include "L1TrackParFit.h"

#include "L1Extrapolation.h"
#include "L1Filtration.h"
#include "L1Fit.h"


#define cnst const fvec

void L1TrackParFit::Filter(const L1UMeasurementInfo& info, const fvec& u, const fvec& sigma2, const fvec& w)
{
  fvec zeta, HCH;
  fvec F0, F1, F2, F3, F4, F5;
  fvec K1, K2, K3, K4, K5;

  zeta = info.cos_phi * fTr.x + info.sin_phi * fTr.y - u;

  // F = CH'
  F0 = info.cos_phi * fTr.C00 + info.sin_phi * fTr.C10;
  F1 = info.cos_phi * fTr.C10 + info.sin_phi * fTr.C11;

  HCH = (F0 * info.cos_phi + F1 * info.sin_phi);

  F2 = info.cos_phi * fTr.C20 + info.sin_phi * fTr.C21;
  F3 = info.cos_phi * fTr.C30 + info.sin_phi * fTr.C31;
  F4 = info.cos_phi * fTr.C40 + info.sin_phi * fTr.C41;
  F5 = info.cos_phi * fTr.C50 + info.sin_phi * fTr.C51;

  const fmask maskDoFilter = (HCH < sigma2 * 16.f);
  //const fvec maskDoFilter = _f32vec4_true;

  // correction to HCH is needed for the case when sigma2 is so small
  // with respect to HCH that it disappears due to the roundoff error
  //
  fvec wi     = w / (sigma2 + fvec(1.0000001) * HCH);
  fvec zetawi = w * zeta / (iif(maskDoFilter, sigma2, fvec::Zero()) + HCH);

  // fTr.chi2 += iif( maskDoFilter, zeta * zetawi, fvec::Zero() );
  fTr.chi2 += zeta * zeta * wi;
  fTr.NDF += w;

  K1 = F1 * wi;
  K2 = F2 * wi;
  K3 = F3 * wi;
  K4 = F4 * wi;
  K5 = F5 * wi;

  fTr.x -= F0 * zetawi;
  fTr.y -= F1 * zetawi;
  fTr.tx -= F2 * zetawi;
  fTr.ty -= F3 * zetawi;
  fTr.qp -= F4 * zetawi;
  fTr.t -= F5 * zetawi;

  fTr.C00 -= F0 * F0 * wi;
  fTr.C10 -= K1 * F0;
  fTr.C11 -= K1 * F1;
  fTr.C20 -= K2 * F0;
  fTr.C21 -= K2 * F1;
  fTr.C22 -= K2 * F2;
  fTr.C30 -= K3 * F0;
  fTr.C31 -= K3 * F1;
  fTr.C32 -= K3 * F2;
  fTr.C33 -= K3 * F3;
  fTr.C40 -= K4 * F0;
  fTr.C41 -= K4 * F1;
  fTr.C42 -= K4 * F2;
  fTr.C43 -= K4 * F3;
  fTr.C44 -= K4 * F4;
  fTr.C50 -= K5 * F0;
  fTr.C51 -= K5 * F1;
  fTr.C52 -= K5 * F2;
  fTr.C53 -= K5 * F3;
  fTr.C54 -= K5 * F4;
  fTr.C55 -= K5 * F5;
}


void L1TrackParFit::FilterTime(fvec t, fvec dt2, fvec w, fvec timeInfo)
{
  // filter track with a time measurement

  // F = CH'
  fvec F0 = fTr.C50;
  fvec F1 = fTr.C51;
  fvec F2 = fTr.C52;
  fvec F3 = fTr.C53;
  fvec F4 = fTr.C54;
  fvec F5 = fTr.C55;

  fvec HCH = fTr.C55;

  w.setZero(timeInfo <= fvec::Zero());

  // when dt0 is much smaller than current time error,
  // set track time exactly to the measurement value without filtering
  // it helps to keep the initial time errors reasonably small
  // the calculations in the covariance matrix are not affected

  const fmask maskDoFilter = (HCH < dt2 * 16.f);

  fvec wi     = w / (dt2 + fvec(1.0000001) * HCH);
  fvec zeta   = fTr.t - t;
  fvec zetawi = w * zeta / (iif(maskDoFilter, dt2, fvec::Zero()) + HCH);

  fTr.chi2(maskDoFilter) += zeta * zeta * wi;
  fTr.NDF += w;

  fvec K1 = F1 * wi;
  fvec K2 = F2 * wi;
  fvec K3 = F3 * wi;
  fvec K4 = F4 * wi;
  fvec K5 = F5 * wi;

  fTr.x -= F0 * zetawi;
  fTr.y -= F1 * zetawi;
  fTr.tx -= F2 * zetawi;
  fTr.ty -= F3 * zetawi;
  fTr.qp -= F4 * zetawi;
  fTr.t -= F5 * zetawi;

  fTr.C00 -= F0 * F0 * wi;
  fTr.C10 -= K1 * F0;
  fTr.C11 -= K1 * F1;
  fTr.C20 -= K2 * F0;
  fTr.C21 -= K2 * F1;
  fTr.C22 -= K2 * F2;
  fTr.C30 -= K3 * F0;
  fTr.C31 -= K3 * F1;
  fTr.C32 -= K3 * F2;
  fTr.C33 -= K3 * F3;
  fTr.C40 -= K4 * F0;
  fTr.C41 -= K4 * F1;
  fTr.C42 -= K4 * F2;
  fTr.C43 -= K4 * F3;
  fTr.C44 -= K4 * F4;
  fTr.C50 -= K5 * F0;
  fTr.C51 -= K5 * F1;
  fTr.C52 -= K5 * F2;
  fTr.C53 -= K5 * F3;
  fTr.C54 -= K5 * F4;
  fTr.C55 -= K5 * F5;
}


void L1TrackParFit::FilterXY(const L1XYMeasurementInfo& info, fvec x, fvec y)
{
  cnst TWO(2.);

  fvec zeta0, zeta1, S00, S10, S11, si;
  fvec F00, F10, F20, F30, F40, F50, F01, F11, F21, F31, F41, F51;
  fvec K00, K10, K20, K30, K40, K50, K01, K11, K21, K31, K41, K51;

  zeta0 = fTr.x - x;
  zeta1 = fTr.y - y;

  // F = CH'
  F00 = fTr.C00;
  F10 = fTr.C10;
  F20 = fTr.C20;
  F30 = fTr.C30;
  F40 = fTr.C40;
  F50 = fTr.C50;
  F01 = fTr.C10;
  F11 = fTr.C11;
  F21 = fTr.C21;
  F31 = fTr.C31;
  F41 = fTr.C41;
  F51 = fTr.C51;

  S00 = F00 + info.C00;
  S10 = F10 + info.C10;
  S11 = F11 + info.C11;

  si          = 1.f / (S00 * S11 - S10 * S10);
  fvec S00tmp = S00;
  S00         = si * S11;
  S10         = -si * S10;
  S11         = si * S00tmp;

  fTr.chi2 += zeta0 * zeta0 * S00 + 2.f * zeta0 * zeta1 * S10 + zeta1 * zeta1 * S11;
  fTr.NDF += TWO;

  K00 = F00 * S00 + F01 * S10;
  K01 = F00 * S10 + F01 * S11;
  K10 = F10 * S00 + F11 * S10;
  K11 = F10 * S10 + F11 * S11;
  K20 = F20 * S00 + F21 * S10;
  K21 = F20 * S10 + F21 * S11;
  K30 = F30 * S00 + F31 * S10;
  K31 = F30 * S10 + F31 * S11;
  K40 = F40 * S00 + F41 * S10;
  K41 = F40 * S10 + F41 * S11;
  K50 = F50 * S00 + F51 * S10;
  K51 = F50 * S10 + F51 * S11;

  fTr.x -= K00 * zeta0 + K01 * zeta1;
  fTr.y -= K10 * zeta0 + K11 * zeta1;
  fTr.tx -= K20 * zeta0 + K21 * zeta1;
  fTr.ty -= K30 * zeta0 + K31 * zeta1;
  fTr.qp -= K40 * zeta0 + K41 * zeta1;

  fTr.C00 -= K00 * F00 + K01 * F01;
  fTr.C10 -= K10 * F00 + K11 * F01;
  fTr.C11 -= K10 * F10 + K11 * F11;
  fTr.C20 -= K20 * F00 + K21 * F01;
  fTr.C21 -= K20 * F10 + K21 * F11;
  fTr.C22 -= K20 * F20 + K21 * F21;
  fTr.C30 -= K30 * F00 + K31 * F01;
  fTr.C31 -= K30 * F10 + K31 * F11;
  fTr.C32 -= K30 * F20 + K31 * F21;
  fTr.C33 -= K30 * F30 + K31 * F31;
  fTr.C40 -= K40 * F00 + K41 * F01;
  fTr.C41 -= K40 * F10 + K41 * F11;
  fTr.C42 -= K40 * F20 + K41 * F21;
  fTr.C43 -= K40 * F30 + K41 * F31;
  fTr.C44 -= K40 * F40 + K41 * F41;
  fTr.C50 -= K50 * F00 + K51 * F01;
  fTr.C51 -= K50 * F10 + K51 * F11;
  fTr.C52 -= K50 * F20 + K51 * F21;
  fTr.C53 -= K50 * F30 + K51 * F31;
  fTr.C54 -= K50 * F40 + K51 * F41;
  fTr.C55 -= K50 * F50 + K51 * F51;
}

void L1TrackParFit::FilterExtrapolatedXY(const fvec& x, const fvec& y, const L1XYMeasurementInfo& info,
                                         const fvec& extrX, const fvec& extrY, const fvec Jx[6], const fvec Jy[6])
{
  // add a 2-D measurenent (x,y) at some z, that differs from fTr.z
  // extrX, extrY are extrapolated track parameters at z, Jx, Jy are derivatives of the extrapolation

  // ! it is assumed that in the track covariance matrix all non-diagonal covariances are 0
  // ! except of C10

  L1TrackPar& T = fTr;

  //zeta0 = T.x + Jx[2]*T.tx + Jx[3]*T.ty + Jx[4]*T.qp - x;
  //zeta1 = T.y + Jy[2]*T.tx + Jy[3]*T.ty + Jy[4]*T.qp - y;

  fvec zeta0 = extrX - x;
  fvec zeta1 = extrY - y;

  // H = 1 0 Jx[2] Jx[3] Jx[4] 0
  //     0 1 Jy[2] Jy[3] Jy[4] 0

  // F = CH'
  fvec F00 = T.C00;
  fvec F01 = T.C10;
  fvec F10 = T.C10;
  fvec F11 = T.C11;
  fvec F20 = Jx[2] * T.C22;
  fvec F21 = Jy[2] * T.C22;
  fvec F30 = Jx[3] * T.C33;
  fvec F31 = Jy[3] * T.C33;
  fvec F40 = Jx[4] * T.C44;
  fvec F41 = Jy[4] * T.C44;

  fvec S00 = info.C00 + F00 + Jx[2] * F20 + Jx[3] * F30 + Jx[4] * F40;
  fvec S10 = info.C10 + F10 + Jy[2] * F20 + Jy[3] * F30 + Jy[4] * F40;
  fvec S11 = info.C11 + F11 + Jy[2] * F21 + Jy[3] * F31 + Jy[4] * F41;

  fvec si = fvec(1.) / (S00 * S11 - S10 * S10);

  fvec S00tmp = S00;
  S00         = si * S11;
  S10         = -si * S10;
  S11         = si * S00tmp;

  T.chi2 += zeta0 * zeta0 * S00 + fvec(2.) * zeta0 * zeta1 * S10 + zeta1 * zeta1 * S11;
  T.NDF += fvec(2.);

  fvec K00 = F00 * S00 + F01 * S10;
  fvec K01 = F00 * S10 + F01 * S11;
  fvec K10 = F10 * S00 + F11 * S10;
  fvec K11 = F10 * S10 + F11 * S11;
  fvec K20 = F20 * S00 + F21 * S10;
  fvec K21 = F20 * S10 + F21 * S11;
  fvec K30 = F30 * S00 + F31 * S10;
  fvec K31 = F30 * S10 + F31 * S11;
  fvec K40 = F40 * S00 + F41 * S10;
  fvec K41 = F40 * S10 + F41 * S11;

  T.x -= K00 * zeta0 + K01 * zeta1;
  T.y -= K10 * zeta0 + K11 * zeta1;
  T.tx -= K20 * zeta0 + K21 * zeta1;
  T.ty -= K30 * zeta0 + K31 * zeta1;
  T.qp -= K40 * zeta0 + K41 * zeta1;

  T.C00 -= (K00 * F00 + K01 * F01);
  T.C10 -= (K10 * F00 + K11 * F01);
  T.C11 -= (K10 * F10 + K11 * F11);
  T.C20 = -(K20 * F00 + K21 * F01);
  T.C21 = -(K20 * F10 + K21 * F11);
  T.C22 -= (K20 * F20 + K21 * F21);
  T.C30 = -(K30 * F00 + K31 * F01);
  T.C31 = -(K30 * F10 + K31 * F11);
  T.C32 = -(K30 * F20 + K31 * F21);
  T.C33 -= (K30 * F30 + K31 * F31);
  T.C40 = -(K40 * F00 + K41 * F01);
  T.C41 = -(K40 * F10 + K41 * F11);
  T.C42 = -(K40 * F20 + K41 * F21);
  T.C43 = -(K40 * F30 + K41 * F31);
  T.C44 -= (K40 * F40 + K41 * F41);
}


void L1TrackParFit::Extrapolate  // extrapolates track parameters and returns jacobian for extrapolation of CovMatrix
  (fvec z_out,                   // extrapolate to this z position
   fvec qp0,                     // use Q/p linearisation at this value
   const L1FieldRegion& F, const fvec& w)
{
  fvec sgn = iif(fTr.z < z_out, fvec(1.), fvec(-1.));
  while (!(w * abs(z_out - fTr.z) <= fvec(1.e-6)).isFull()) {
    fvec zNew                              = fTr.z + sgn * fvec(50.);  // max. 50 cm step
    zNew(sgn * (z_out - zNew) <= fvec(0.)) = z_out;
    ExtrapolateStep(zNew, qp0, F, w);
  }
}

void
  L1TrackParFit::ExtrapolateStep  // extrapolates track parameters and returns jacobian for extrapolation of CovMatrix
  (fvec z_out,                    // extrapolate to this z position
   fvec qp0,                      // use Q/p linearisation at this value
   const L1FieldRegion& F, const fvec& w)
{
  //
  // Forth-order Runge-Kutta method for solution of the equation
  // of motion of a particle with parameter qp = Q /P
  //              in the magnetic field B()
  //
  //  ( x )          tx
  //  ( y )          ty
  // d( tx)/dz = c_light * qp * L * (     tx*ty * Bx - (1+tx*tx) * By + ty * Bz  )
  //  ( ty)      c_light * qp * L * ( (1+ty*ty) * Bx     - tx*ty * By - tx * Bz  )  ,
  //  ( qp)           0.
  //  ( t )       L * sqrt ( 1 + m*m * qp*qp ) / c_light_ns
  //
  //   where  L = sqrt ( 1 + tx*tx + ty*ty ) .
  //   c_light = 0.000299792458 [(GeV/c)/kG/cm]
  //   c_light_ns =  29.9792458 [cm/ns]
  //
  //  In the following for RK step :
  //   r[5] = {x,y,tx,ty,t}. ( q/p parameter doesn't change during the extrapolation)
  //   dr(z)/dz = f(z,r)
  //
  //
  //========================================================================
  //
  //  NIM A395 (1997) 169-184; NIM A426 (1999) 268-282.
  //
  //  the routine is based on LHC(b) utility code
  //
  //========================================================================


  cnst c_light = 0.000299792458;

  //----------------------------------------------------------------

  z_out = iif((fvec(0.f) < w), z_out, fTr.z);

  fvec qp_in   = fTr.qp;
  const fvec h = (z_out - fTr.z);

  const fvec a[4] = {0., h * fvec(0.5), h * fvec(0.5), h};
  const fvec c[4] = {h / fvec(6.), h / fvec(3.), h / fvec(3.), h / fvec(6.)};

  fvec f0[4];  // ( dx/dz )
  fvec f1[4];  // ( dy/dz )
  fvec f2[4];  // ( dtx/dz )
  fvec f3[4];  // ( dty/dz )
  fvec f4[4];  // ( dt/dz )

  fvec f2_tx[4], f3_tx[4], f4_tx[4];  // df* / dtx
  fvec f2_ty[4], f3_ty[4], f4_ty[4];  // df* / dty
  fvec f2_qp[4], f3_qp[4], f4_qp[4];  // df* / dqp

  fvec k[5][5] = {0., 0., 0., 0., 0.};

  //   Runge-Kutta steps for track parameters
  //
  {
    fvec r0[5] = {fTr.x, fTr.y, fTr.tx, fTr.ty, fTr.t};

    for (int step = 0; step < 4; ++step) {
      fvec z = fTr.z + a[step];
      fvec r[5];
      for (int i = 0; i < 5; ++i) {
        if (step == 0) { r[i] = r0[i]; }
        else {
          r[i] = r0[i] + a[step] * k[step][i];
        }
      }

      fvec B[3];
      const fvec& Bx = B[0];
      const fvec& By = B[1];
      const fvec& Bz = B[2];

      F.Get(r[0], r[1], z, B);

      fvec tx    = r[2];
      fvec ty    = r[3];
      fvec tx2   = tx * tx;
      fvec ty2   = ty * ty;
      fvec txty  = tx * ty;
      fvec L2    = fvec(1.) + tx2 + ty2;
      fvec L2i   = fvec(1.) / L2;
      fvec L     = sqrt(L2);
      fvec cL    = c_light * L;
      fvec cLqp0 = cL * qp0;

      f0[step] = tx;
      f1[step] = ty;

      fvec f2tmp  = (txty * Bx + ty * Bz) - (fvec(1.) + tx2) * By;
      f2[step]    = cLqp0 * f2tmp;
      f2_tx[step] = cLqp0 * (tx * f2tmp * L2i + ty * Bx - fvec(2.) * tx * By);
      f2_ty[step] = cLqp0 * (ty * f2tmp * L2i + tx * Bx + Bz);
      f2_qp[step] = cL * f2tmp;

      fvec f3tmp  = -txty * By - tx * Bz + (fvec(1.) + ty2) * Bx;
      f3[step]    = cLqp0 * f3tmp;
      f3_tx[step] = cLqp0 * (tx * f3tmp * L2i - ty * By - Bz);
      f3_ty[step] = cLqp0 * (ty * f3tmp * L2i + fvec(2.) * ty * Bx - tx * By);
      f3_qp[step] = cL * f3tmp;

      fvec m2     = fMass2;
      fvec vi     = sqrt(fvec(1.) + m2 * qp0 * qp0) / fvec(29.9792458f);
      f4[step]    = vi * L;
      f4_tx[step] = vi * tx / L;
      f4_ty[step] = vi * ty / L;
      f4_qp[step] = (m2 * qp0) * (L / sqrt(fvec(1.) + m2 * qp0 * qp0) / fvec(29.9792458f));

      k[step + 1][0] = f0[step];
      k[step + 1][1] = f1[step];
      k[step + 1][2] = f2[step];
      k[step + 1][3] = f3[step];
      k[step + 1][4] = f4[step];
    }  // end of Runge-Kutta step

    fTr.x  = r0[0] + (c[0] * k[1][0] + c[1] * k[2][0] + c[2] * k[3][0] + c[3] * k[4][0]);
    fTr.y  = r0[1] + (c[0] * k[1][1] + c[1] * k[2][1] + c[2] * k[3][1] + c[3] * k[4][1]);
    fTr.tx = r0[2] + (c[0] * k[1][2] + c[1] * k[2][2] + c[2] * k[3][2] + c[3] * k[4][2]);
    fTr.ty = r0[3] + (c[0] * k[1][3] + c[1] * k[2][3] + c[2] * k[3][3] + c[3] * k[4][3]);
    fTr.t  = r0[4] + (c[0] * k[1][4] + c[1] * k[2][4] + c[2] * k[3][4] + c[3] * k[4][4]);
    fTr.z  = z_out;
  }


  // Jacobian of extrapolation

  //
  //    derivatives d/dx and d/dy
  //
  fvec Jx[5] = {1., 0., 0., 0., 0.};  // D new { x,y,tx,ty,t } / D old x
  fvec Jy[5] = {0., 1., 0., 0., 0.};  // D new { x,y,tx,ty,t } / D old y

  //
  //   Runge-Kutta steps for derivatives d/dtx
  //
  fvec Jtx[5] = {0., 0., 1., 0., 0.};  // D new { x,y,tx,ty,t } / D old tx
  {
    for (int step = 0; step < 4; ++step) {
      fvec r2        = Jtx[2] + a[step] * k[step][2];  // dtx[step]/dtx0
      fvec r3        = Jtx[3] + a[step] * k[step][3];  // dty[step]/dtx0
      k[step + 1][0] = r2;
      k[step + 1][1] = r3;
      k[step + 1][2] = f2_tx[step] * r2 + f2_ty[step] * r3;
      k[step + 1][3] = f3_tx[step] * r2 + f3_ty[step] * r3;
      k[step + 1][4] = f4_tx[step] * r2 + f4_ty[step] * r3;
    }
    for (int i = 0; i < 5; ++i) {
      Jtx[i] += c[0] * k[1][i] + c[1] * k[2][i] + c[2] * k[3][i] + c[3] * k[4][i];
    }
  }

  //
  //   Runge-Kutta steps for derivatives d/dty
  //
  fvec Jty[5] = {0., 0., 0., 1., 0.};  // D new { x,y,tx,ty,t } / D old ty
  {
    for (int step = 0; step < 4; ++step) {
      fvec r2        = Jty[2] + a[step] * k[step][2];  // dtx[step]/dty0
      fvec r3        = Jty[3] + a[step] * k[step][3];  // dty[step]/dty0
      k[step + 1][0] = r2;
      k[step + 1][1] = r3;
      k[step + 1][2] = f2_tx[step] * r2 + f2_ty[step] * r3;
      k[step + 1][3] = f3_tx[step] * r2 + f3_ty[step] * r3;
      k[step + 1][4] = f4_tx[step] * r2 + f4_ty[step] * r3;
    }
    for (int i = 0; i < 5; ++i) {
      Jty[i] += c[0] * k[1][i] + c[1] * k[2][i] + c[2] * k[3][i] + c[3] * k[4][i];
    }
  }

  //
  //   Runge-Kutta steps for derivatives d/dqp
  //
  fvec Jqp[5] = {0., 0., 0., 0., 0.};  // D new { x,y,tx,ty,t } / D old qp
  {
    for (int step = 0; step < 4; ++step) {
      fvec r2        = Jqp[2] + a[step] * k[step][2];  // dtx/dqp
      fvec r3        = Jqp[3] + a[step] * k[step][3];  // dty/dqp;  (dqp/dqp == 1)
      k[step + 1][0] = r2;
      k[step + 1][1] = r3;
      k[step + 1][2] = f2_qp[step] + f2_tx[step] * r2 + f2_ty[step] * r3;
      k[step + 1][3] = f3_qp[step] + f3_tx[step] * r2 + f3_ty[step] * r3;
      k[step + 1][4] = f4_qp[step] + f4_tx[step] * r2 + f4_ty[step] * r3;
    }
    for (int i = 0; i < 5; ++i) {
      Jqp[i] += c[0] * k[1][i] + c[1] * k[2][i] + c[2] * k[3][i] + c[3] * k[4][i];
    }
  }


  //
  //    derivatives d/dt
  //
  fvec Jt[5] = {0., 0., 0., 0., 1.};  // D new { x,y,tx,ty,t } / D old t

  {  // update parameters
    fvec dqp = qp_in - qp0;
    fTr.x += Jqp[0] * dqp;
    fTr.y += Jqp[1] * dqp;
    fTr.tx += Jqp[2] * dqp;
    fTr.ty += Jqp[3] * dqp;
    fTr.t += Jqp[4] * dqp;
  }

  //          covariance matrix transport

  // debug mode: full matrix multiplication76y
  if (0) {
    fvec J[6][6];
    for (int i = 0; i < 4; i++) {
      J[i][0] = Jx[i];   // dx,y,tx,ty / dx
      J[i][1] = Jy[i];   // dx,y,tx,ty / dy
      J[i][2] = Jtx[i];  // dx,y,tx,ty / dtx
      J[i][3] = Jty[i];  // dx,y,tx,ty / dty
      J[i][4] = Jqp[i];  // dx,y,tx,ty / dqp
      J[i][5] = Jt[i];   // dx,y,tx,ty / dt
    }
    {
      J[4][0] = 0.;  // dqp / dx
      J[4][1] = 0.;  // dqp / dy
      J[4][2] = 0.;  // dqp / dtx
      J[4][3] = 0.;  // dqp / dty
      J[4][4] = 1.;  // dqp / dqp
      J[4][5] = 0.;  // dqp / dt
    }
    {
      J[5][0] = Jx[4];   // dt / dx
      J[5][1] = Jy[4];   // dt / dy
      J[5][2] = Jtx[4];  // dt / dtx
      J[5][3] = Jty[4];  // dt / dty
      J[5][4] = Jqp[4];  // dt / dqp
      J[5][5] = Jt[4];   // dt / dt
    }

    fvec C[6][6];
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 6; j++) {
        C[i][j] = fTr.C(i, j);
      }
    }
    fvec JC[6][6];
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 6; j++) {
        JC[i][j] = 0.;
        for (int m = 0; m < 6; m++) {
          JC[i][j] += J[i][m] * C[m][j];
        }
      }
    }
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 6; j++) {
        fvec Cij = 0.;
        for (int m = 0; m < 6; m++) {
          Cij += JC[i][m] * J[j][m];
        }
        fTr.C(i, j) = Cij;
      }
    }
  }
  else {  // unrolled calculations, taking into account that some derivatives are 0 or 1

    fvec JC00 = Jtx[0] * fTr.C20 + Jty[0] * fTr.C30 + Jqp[0] * fTr.C40 + fTr.C00;
    //fvec JC01 = Jtx[0] * fTr.C21 + Jty[0] * fTr.C31 + Jqp[0] * fTr.C41 + fTr.C10;
    fvec JC02 = Jtx[0] * fTr.C22 + Jty[0] * fTr.C32 + Jqp[0] * fTr.C42 + fTr.C20;
    fvec JC03 = Jtx[0] * fTr.C32 + Jty[0] * fTr.C33 + Jqp[0] * fTr.C43 + fTr.C30;
    fvec JC04 = Jtx[0] * fTr.C42 + Jty[0] * fTr.C43 + Jqp[0] * fTr.C44 + fTr.C40;
    //fvec JC05 = Jtx[0] * fTr.C52 + Jty[0] * fTr.C53 + Jqp[0] * fTr.C54 + fTr.C50;

    fvec JC10 = Jtx[1] * fTr.C20 + Jty[1] * fTr.C30 + Jqp[1] * fTr.C40 + fTr.C10;
    fvec JC11 = Jtx[1] * fTr.C21 + Jty[1] * fTr.C31 + Jqp[1] * fTr.C41 + fTr.C11;
    fvec JC12 = Jtx[1] * fTr.C22 + Jty[1] * fTr.C32 + Jqp[1] * fTr.C42 + fTr.C21;
    fvec JC13 = Jtx[1] * fTr.C32 + Jty[1] * fTr.C33 + Jqp[1] * fTr.C43 + fTr.C31;
    fvec JC14 = Jtx[1] * fTr.C42 + Jty[1] * fTr.C43 + Jqp[1] * fTr.C44 + fTr.C41;
    //fvec JC15 = Jtx[1] * fTr.C52 + Jty[1] * fTr.C53 + Jqp[1] * fTr.C54 + fTr.C51;

    fvec JC20 = Jtx[2] * fTr.C20 + Jty[2] * fTr.C30 + Jqp[2] * fTr.C40;
    fvec JC21 = Jtx[2] * fTr.C21 + Jty[2] * fTr.C31 + Jqp[2] * fTr.C41;
    fvec JC22 = Jtx[2] * fTr.C22 + Jty[2] * fTr.C32 + Jqp[2] * fTr.C42;
    fvec JC23 = Jtx[2] * fTr.C32 + Jty[2] * fTr.C33 + Jqp[2] * fTr.C43;
    fvec JC24 = Jtx[2] * fTr.C42 + Jty[2] * fTr.C43 + Jqp[2] * fTr.C44;
    //fvec JC25 = Jtx[2] * fTr.C52 + Jty[2] * fTr.C53 + Jqp[2] * fTr.C54;

    fvec JC30 = Jtx[3] * fTr.C20 + Jty[3] * fTr.C30 + Jqp[3] * fTr.C40;
    fvec JC31 = Jtx[3] * fTr.C21 + Jty[3] * fTr.C31 + Jqp[3] * fTr.C41;
    fvec JC32 = Jtx[3] * fTr.C22 + Jty[3] * fTr.C32 + Jqp[3] * fTr.C42;
    fvec JC33 = Jtx[3] * fTr.C32 + Jty[3] * fTr.C33 + Jqp[3] * fTr.C43;
    fvec JC34 = Jtx[3] * fTr.C42 + Jty[3] * fTr.C43 + Jqp[3] * fTr.C44;
    //fvec JC35 = Jtx[3] * fTr.C52 + Jty[3] * fTr.C53 + Jqp[3] * fTr.C54;

    fvec JC40 = fTr.C40;
    fvec JC41 = fTr.C41;
    fvec JC42 = fTr.C42;
    fvec JC43 = fTr.C43;
    fvec JC44 = fTr.C44;
    //fvec JC45 = fTr.C54;

    fvec JC50 = Jtx[4] * fTr.C20 + Jty[4] * fTr.C30 + Jqp[4] * fTr.C40 + fTr.C50;
    fvec JC51 = Jtx[4] * fTr.C21 + Jty[4] * fTr.C31 + Jqp[4] * fTr.C41 + fTr.C51;
    fvec JC52 = Jtx[4] * fTr.C22 + Jty[4] * fTr.C32 + Jqp[4] * fTr.C42 + fTr.C52;
    fvec JC53 = Jtx[4] * fTr.C32 + Jty[4] * fTr.C33 + Jqp[4] * fTr.C43 + fTr.C53;
    fvec JC54 = Jtx[4] * fTr.C42 + Jty[4] * fTr.C43 + Jqp[4] * fTr.C44 + fTr.C54;
    fvec JC55 = Jtx[4] * fTr.C52 + Jty[4] * fTr.C53 + Jqp[4] * fTr.C54 + fTr.C55;

    fTr.C00 = JC02 * Jtx[0] + JC03 * Jty[0] + JC04 * Jqp[0] + JC00;

    fTr.C10 = JC12 * Jtx[0] + JC13 * Jty[0] + JC14 * Jqp[0] + JC10;
    fTr.C11 = JC12 * Jtx[1] + JC13 * Jty[1] + JC14 * Jqp[1] + JC11;

    fTr.C20 = JC22 * Jtx[0] + JC23 * Jty[0] + JC24 * Jqp[0] + JC20;
    fTr.C21 = JC22 * Jtx[1] + JC23 * Jty[1] + JC24 * Jqp[1] + JC21;
    fTr.C22 = JC22 * Jtx[2] + JC23 * Jty[2] + JC24 * Jqp[2];

    fTr.C30 = JC32 * Jtx[0] + JC33 * Jty[0] + JC34 * Jqp[0] + JC30;
    fTr.C31 = JC32 * Jtx[1] + JC33 * Jty[1] + JC34 * Jqp[1] + JC31;
    fTr.C32 = JC32 * Jtx[2] + JC33 * Jty[2] + JC34 * Jqp[2];
    fTr.C33 = JC32 * Jtx[3] + JC33 * Jty[3] + JC34 * Jqp[3];

    fTr.C40 = JC42 * Jtx[0] + JC43 * Jty[0] + JC44 * Jqp[0] + JC40;
    fTr.C41 = JC42 * Jtx[1] + JC43 * Jty[1] + JC44 * Jqp[1] + JC41;
    fTr.C42 = JC42 * Jtx[2] + JC43 * Jty[2] + JC44 * Jqp[2];
    fTr.C43 = JC42 * Jtx[3] + JC43 * Jty[3] + JC44 * Jqp[3];
    fTr.C44 = JC44;

    fTr.C50 = JC52 * Jtx[0] + JC53 * Jty[0] + JC54 * Jqp[0] + JC50;
    fTr.C51 = JC52 * Jtx[1] + JC53 * Jty[1] + JC54 * Jqp[1] + JC51;
    fTr.C52 = JC52 * Jtx[2] + JC53 * Jty[2] + JC54 * Jqp[2];
    fTr.C53 = JC52 * Jtx[3] + JC53 * Jty[3] + JC54 * Jqp[3];
    fTr.C54 = JC54;
    fTr.C55 = JC52 * Jtx[4] + JC53 * Jty[4] + JC54 * Jqp[4] + JC55;
  }
}

void L1TrackParFit::
  ExtrapolateStepAnalytic  // extrapolates track parameters and returns jacobian for extrapolation of CovMatrix
  (fvec z_out,             // extrapolate to this z position
   fvec qp0,               // use Q/p linearisation at this value
   const L1FieldRegion& F, const fvec& w)
{
  //
  //  Part of the analytic extrapolation formula with error (c_light*B*dz)^4/4!
  //

  // TODO: the time parameter is not extrapolated!

  cnst c_light(0.000299792458);

  cnst c1(1.), c2(2.), c3(3.), c4(4.), c6(6.), c9(9.), c15(15.), c18(18.), c45(45.), c2i(1. / 2.), c3i(1. / 3.),
    c6i(1. / 6.), c12i(1. / 12.);

  const fvec qp = fTr.qp;
  fvec dz       = (z_out - fTr.z);

  dz.setZero(w <= fvec::Zero());

  const fvec dz2 = dz * dz;
  const fvec dz3 = dz2 * dz;

  // construct coefficients

  const fvec x     = fTr.tx;
  const fvec y     = fTr.ty;
  const fvec xx    = x * x;
  const fvec xy    = x * y;
  const fvec yy    = y * y;
  const fvec y2    = y * c2;
  const fvec x2    = x * c2;
  const fvec x4    = x * c4;
  const fvec xx31  = xx * c3 + c1;
  const fvec xx159 = xx * c15 + c9;

  const fvec Ay   = -xx - c1;
  const fvec Ayy  = x * (xx * c3 + c3);
  const fvec Ayz  = -c2 * xy;
  const fvec Ayyy = -(c15 * xx * xx + c18 * xx + c3);

  const fvec Ayy_x  = c3 * xx31;
  const fvec Ayyy_x = -x4 * xx159;

  const fvec Bx   = yy + c1;
  const fvec Byy  = y * xx31;
  const fvec Byz  = c2 * xx + c1;
  const fvec Byyy = -xy * xx159;

  const fvec Byy_x  = c6 * xy;
  const fvec Byyy_x = -y * (c45 * xx + c9);
  const fvec Byyy_y = -x * xx159;

  // end of coefficients calculation

  const fvec t2 = c1 + xx + yy;
  const fvec t  = sqrt(t2);
  const fvec h  = qp0 * c_light;
  const fvec ht = h * t;

  // get field integrals
  const fvec ddz = fTr.z - F.z0;
  const fvec Fx0 = F.cx0 + F.cx1 * ddz + F.cx2 * ddz * ddz;
  const fvec Fx1 = (F.cx1 + c2 * F.cx2 * ddz) * dz;
  const fvec Fx2 = F.cx2 * dz2;
  const fvec Fy0 = F.cy0 + F.cy1 * ddz + F.cy2 * ddz * ddz;
  const fvec Fy1 = (F.cy1 + c2 * F.cy2 * ddz) * dz;
  const fvec Fy2 = F.cy2 * dz2;
  const fvec Fz0 = F.cz0 + F.cz1 * ddz + F.cz2 * ddz * ddz;
  const fvec Fz1 = (F.cz1 + c2 * F.cz2 * ddz) * dz;
  const fvec Fz2 = F.cz2 * dz2;

  //

  const fvec sx = (Fx0 + Fx1 * c2i + Fx2 * c3i);
  const fvec sy = (Fy0 + Fy1 * c2i + Fy2 * c3i);
  const fvec sz = (Fz0 + Fz1 * c2i + Fz2 * c3i);

  const fvec Sx = (Fx0 * c2i + Fx1 * c6i + Fx2 * c12i);
  const fvec Sy = (Fy0 * c2i + Fy1 * c6i + Fy2 * c12i);
  const fvec Sz = (Fz0 * c2i + Fz1 * c6i + Fz2 * c12i);

  fvec syz;
  {
    constexpr double d = 360.;
    cnst c00(30. * 6. / d), c01(30. * 2. / d), c02(30. / d), c10(3. * 40. / d), c11(3. * 15. / d), c12(3. * 8. / d),
      c20(2. * 45. / d), c21(2. * 2. * 9. / d), c22(2. * 2. * 5. / d);
    syz = Fy0 * (c00 * Fz0 + c01 * Fz1 + c02 * Fz2) + Fy1 * (c10 * Fz0 + c11 * Fz1 + c12 * Fz2)
          + Fy2 * (c20 * Fz0 + c21 * Fz1 + c22 * Fz2);
  }

  fvec Syz;
  {
    constexpr double d = 2520.;
    cnst c00(21. * 20. / d), c01(21. * 5. / d), c02(21. * 2. / d), c10(7. * 30. / d), c11(7. * 9. / d),
      c12(7. * 4. / d), c20(2. * 63. / d), c21(2. * 21. / d), c22(2. * 10. / d);
    Syz = Fy0 * (c00 * Fz0 + c01 * Fz1 + c02 * Fz2) + Fy1 * (c10 * Fz0 + c11 * Fz1 + c12 * Fz2)
          + Fy2 * (c20 * Fz0 + c21 * Fz1 + c22 * Fz2);
  }

  const fvec syy  = sy * sy * c2i;
  const fvec syyy = syy * sy * c3i;

  fvec Syy;
  {
    constexpr double d = 2520.;
    cnst c00(420. / d), c01(21. * 15. / d), c02(21. * 8. / d), c03(63. / d), c04(70. / d), c05(20. / d);
    Syy = Fy0 * (c00 * Fy0 + c01 * Fy1 + c02 * Fy2) + Fy1 * (c03 * Fy1 + c04 * Fy2) + c05 * Fy2 * Fy2;
  }

  fvec Syyy;
  {
    constexpr double d = 181440.;
    cnst c000(7560 / d), c001(9 * 1008 / d), c002(5 * 1008 / d), c011(21 * 180 / d), c012(24 * 180 / d),
      c022(7 * 180 / d), c111(540 / d), c112(945 / d), c122(560 / d), c222(112 / d);
    const fvec Fy22 = Fy2 * Fy2;
    Syyy = Fy0 * (Fy0 * (c000 * Fy0 + c001 * Fy1 + c002 * Fy2) + Fy1 * (c011 * Fy1 + c012 * Fy2) + c022 * Fy22)
           + Fy1 * (Fy1 * (c111 * Fy1 + c112 * Fy2) + c122 * Fy22) + c222 * Fy22 * Fy2;
  }


  const fvec sA1   = sx * xy + sy * Ay + sz * y;
  const fvec sA1_x = sx * y - sy * x2;
  const fvec sA1_y = sx * x + sz;

  const fvec sB1   = sx * Bx - sy * xy - sz * x;
  const fvec sB1_x = -sy * y - sz;
  const fvec sB1_y = sx * y2 - sy * x;

  const fvec SA1   = Sx * xy + Sy * Ay + Sz * y;
  const fvec SA1_x = Sx * y - Sy * x2;
  const fvec SA1_y = Sx * x + Sz;

  const fvec SB1   = Sx * Bx - Sy * xy - Sz * x;
  const fvec SB1_x = -Sy * y - Sz;
  const fvec SB1_y = Sx * y2 - Sy * x;


  const fvec sA2   = syy * Ayy + syz * Ayz;
  const fvec sA2_x = syy * Ayy_x - syz * y2;
  const fvec sA2_y = -syz * x2;
  const fvec sB2   = syy * Byy + syz * Byz;
  const fvec sB2_x = syy * Byy_x + syz * x4;
  const fvec sB2_y = syy * xx31;

  const fvec SA2   = Syy * Ayy + Syz * Ayz;
  const fvec SA2_x = Syy * Ayy_x - Syz * y2;
  const fvec SA2_y = -Syz * x2;
  const fvec SB2   = Syy * Byy + Syz * Byz;
  const fvec SB2_x = Syy * Byy_x + Syz * x4;
  const fvec SB2_y = Syy * xx31;

  const fvec sA3   = syyy * Ayyy;
  const fvec sA3_x = syyy * Ayyy_x;
  const fvec sB3   = syyy * Byyy;
  const fvec sB3_x = syyy * Byyy_x;
  const fvec sB3_y = syyy * Byyy_y;


  const fvec SA3   = Syyy * Ayyy;
  const fvec SA3_x = Syyy * Ayyy_x;
  const fvec SB3   = Syyy * Byyy;
  const fvec SB3_x = Syyy * Byyy_x;
  const fvec SB3_y = Syyy * Byyy_y;

  const fvec ht1    = ht * dz;
  const fvec ht2    = ht * ht * dz2;
  const fvec ht3    = ht * ht * ht * dz3;
  const fvec ht1sA1 = ht1 * sA1;
  const fvec ht1sB1 = ht1 * sB1;
  const fvec ht1SA1 = ht1 * SA1;
  const fvec ht1SB1 = ht1 * SB1;
  const fvec ht2sA2 = ht2 * sA2;
  const fvec ht2SA2 = ht2 * SA2;
  const fvec ht2sB2 = ht2 * sB2;
  const fvec ht2SB2 = ht2 * SB2;
  const fvec ht3sA3 = ht3 * sA3;
  const fvec ht3sB3 = ht3 * sB3;
  const fvec ht3SA3 = ht3 * SA3;
  const fvec ht3SB3 = ht3 * SB3;

  fTr.x += (x + ht1SA1 + ht2SA2 + ht3SA3) * dz;
  fTr.y += (y + ht1SB1 + ht2SB2 + ht3SB3) * dz;
  fTr.tx += ht1sA1 + ht2sA2 + ht3sA3;
  fTr.ty += ht1sB1 + ht2sB2 + ht3sB3;
  fTr.z += dz;

  const fvec ctdz  = c_light * t * dz;
  const fvec ctdz2 = c_light * t * dz2;

  const fvec dqp  = qp - qp0;
  const fvec t2i  = c1 / t2;
  const fvec xt2i = x * t2i;
  const fvec yt2i = y * t2i;
  const fvec tmp0 = ht1SA1 + c2 * ht2SA2 + c3 * ht3SA3;
  const fvec tmp1 = ht1SB1 + c2 * ht2SB2 + c3 * ht3SB3;
  const fvec tmp2 = ht1sA1 + c2 * ht2sA2 + c3 * ht3sA3;
  const fvec tmp3 = ht1sB1 + c2 * ht2sB2 + c3 * ht3sB3;

  const fvec j02 = dz * (c1 + xt2i * tmp0 + ht1 * SA1_x + ht2 * SA2_x + ht3 * SA3_x);
  const fvec j12 = dz * (xt2i * tmp1 + ht1 * SB1_x + ht2 * SB2_x + ht3 * SB3_x);
  const fvec j22 = c1 + xt2i * tmp2 + ht1 * sA1_x + ht2 * sA2_x + ht3 * sA3_x;
  const fvec j32 = xt2i * tmp3 + ht1 * sB1_x + ht2 * sB2_x + ht3 * sB3_x;

  const fvec j03 = dz * (yt2i * tmp0 + ht1 * SA1_y + ht2 * SA2_y);
  const fvec j13 = dz * (c1 + yt2i * tmp1 + ht1 * SB1_y + ht2 * SB2_y + ht3 * SB3_y);
  const fvec j23 = yt2i * tmp2 + ht1 * sA1_y + ht2 * sA2_y;
  const fvec j33 = c1 + yt2i * tmp3 + ht1 * sB1_y + ht2 * sB2_y + ht3 * sB3_y;

  const fvec j04 = ctdz2 * (SA1 + c2 * ht1 * SA2 + c3 * ht2 * SA3);
  const fvec j14 = ctdz2 * (SB1 + c2 * ht1 * SB2 + c3 * ht2 * SB3);
  const fvec j24 = ctdz * (sA1 + c2 * ht1 * sA2 + c3 * ht2 * sA3);
  const fvec j34 = ctdz * (sB1 + c2 * ht1 * sB2 + c3 * ht2 * sB3);


  // extrapolate inverse momentum

  fTr.x += j04 * dqp;
  fTr.y += j14 * dqp;
  fTr.tx += j24 * dqp;
  fTr.ty += j34 * dqp;

  //          covariance matrix transport

  const fvec c42 = fTr.C42, c43 = fTr.C43;

  const fvec cj00 = fTr.C00 + fTr.C20 * j02 + fTr.C30 * j03 + fTr.C40 * j04;
  //  const fvec cj10 = fTr.C10 + fTr.C21*j02 + fTr.C31*j03 + fTr.C41*j04;
  const fvec cj20 = fTr.C20 + fTr.C22 * j02 + fTr.C32 * j03 + c42 * j04;
  const fvec cj30 = fTr.C30 + fTr.C32 * j02 + fTr.C33 * j03 + c43 * j04;

  const fvec cj01 = fTr.C10 + fTr.C20 * j12 + fTr.C30 * j13 + fTr.C40 * j14;
  const fvec cj11 = fTr.C11 + fTr.C21 * j12 + fTr.C31 * j13 + fTr.C41 * j14;
  const fvec cj21 = fTr.C21 + fTr.C22 * j12 + fTr.C32 * j13 + c42 * j14;
  const fvec cj31 = fTr.C31 + fTr.C32 * j12 + fTr.C33 * j13 + c43 * j14;

  //  const fvec cj02 = fTr.C20*j22 + fTr.C30*j23 + fTr.C40*j24;
  //  const fvec cj12 = fTr.C21*j22 + fTr.C31*j23 + fTr.C41*j24;
  const fvec cj22 = fTr.C22 * j22 + fTr.C32 * j23 + c42 * j24;
  const fvec cj32 = fTr.C32 * j22 + fTr.C33 * j23 + c43 * j24;

  //  const fvec cj03 = fTr.C20*j32 + fTr.C30*j33 + fTr.C40*j34;
  //  const fvec cj13 = fTr.C21*j32 + fTr.C31*j33 + fTr.C41*j34;
  const fvec cj23 = fTr.C22 * j32 + fTr.C32 * j33 + c42 * j34;
  const fvec cj33 = fTr.C32 * j32 + fTr.C33 * j33 + c43 * j34;

  fTr.C40 += c42 * j02 + c43 * j03 + fTr.C44 * j04;  // cj40
  fTr.C41 += c42 * j12 + c43 * j13 + fTr.C44 * j14;  // cj41
  fTr.C42 = c42 * j22 + c43 * j23 + fTr.C44 * j24;   // cj42
  fTr.C43 = c42 * j32 + c43 * j33 + fTr.C44 * j34;   // cj43

  fTr.C00 = cj00 + j02 * cj20 + j03 * cj30 + j04 * fTr.C40;
  fTr.C10 = cj01 + j02 * cj21 + j03 * cj31 + j04 * fTr.C41;
  fTr.C11 = cj11 + j12 * cj21 + j13 * cj31 + j14 * fTr.C41;

  fTr.C20 = j22 * cj20 + j23 * cj30 + j24 * fTr.C40;
  fTr.C30 = j32 * cj20 + j33 * cj30 + j34 * fTr.C40;
  fTr.C21 = j22 * cj21 + j23 * cj31 + j24 * fTr.C41;
  fTr.C31 = j32 * cj21 + j33 * cj31 + j34 * fTr.C41;
  fTr.C22 = j22 * cj22 + j23 * cj32 + j24 * fTr.C42;
  fTr.C32 = j32 * cj22 + j33 * cj32 + j34 * fTr.C42;
  fTr.C33 = j32 * cj23 + j33 * cj33 + j34 * fTr.C43;
  //cout<<"Extrapolation ok"<<endl;
}


void L1TrackParFit::ExtrapolateLine(fvec z_out, const fvec& w)
{
  // extrapolate the track assuming fQp0 == 0
  //
  // it is a copy of a sequence two routines
  //  L1ExtrapolateTime() and L1ExtrapolateLine()
  // TODO: make it right

  cnst c_light(29.9792458);
  fvec dz = (z_out - fTr.z);
  dz.setZero(w <= fvec::Zero());

  L1TrackPar& T = fTr;

  // extrapolate time
  fvec L = sqrt(T.tx * T.tx + T.ty * T.ty + fvec(1.));
  T.t += dz * L / c_light;

  const fvec k1 = dz * T.tx / (c_light * L);
  const fvec k2 = dz * T.ty / (c_light * L);

  fvec c52 = T.C52;
  fvec c53 = T.C53;

  T.C50 += k1 * T.C20 + k2 * T.C30;
  T.C51 += k1 * T.C21 + k2 * T.C31;
  T.C52 += k1 * T.C22 + k2 * T.C32;
  T.C53 += k1 * T.C32 + k2 * T.C33;
  T.C54 += k1 * T.C42 + k2 * T.C43;
  T.C55 += k1 * (T.C52 + c52) + k2 * (T.C53 + c53);

  // extrapolate trajectory

  T.x += T.tx * dz;
  T.y += T.ty * dz;
  T.z += dz;

  const fvec dzC32_in = dz * T.C32;

  T.C21 += dzC32_in;
  T.C10 += dz * (T.C21 + T.C30);

  const fvec C20_in = T.C20;

  T.C20 += dz * T.C22;
  T.C00 += dz * (T.C20 + C20_in);

  const fvec C31_in = T.C31;

  T.C31 += dz * T.C33;
  T.C11 += dz * (T.C31 + C31_in);
  T.C30 += dzC32_in;

  T.C40 += dz * T.C42;
  T.C41 += dz * T.C43;
}


void L1TrackParFit::AddMsInMaterial(const fvec& radThick, fvec qp0, fvec w)
{
  cnst ONE = 1.;

  fvec tx    = fTr.tx;
  fvec ty    = fTr.ty;
  fvec txtx  = tx * tx;
  fvec tyty  = ty * ty;
  fvec txtx1 = txtx + ONE;
  fvec h     = txtx + tyty;
  fvec t     = sqrt(txtx1 + tyty);
  fvec h2    = h * h;
  fvec qp0t  = qp0 * t;

  cnst c1 = 0.0136f, c2 = c1 * 0.038f, c3 = c2 * 0.5f, c4 = -c3 / 2.0f, c5 = c3 / 3.0f, c6 = -c3 / 4.0f;

  fvec s0 = (c1 + c2 * log(radThick) + c3 * h + h2 * (c4 + c5 * h + c6 * h2)) * qp0t;
  //fvec a = ( (ONE+mass2*qp0*qp0t)*radThick*s0*s0 );
  fvec a = ((t + fMass2 * qp0 * qp0t) * radThick * s0 * s0);

  fTr.C22 += w * txtx1 * a;
  fTr.C32 += w * tx * ty * a;
  fTr.C33 += w * (ONE + tyty) * a;
}

void L1TrackParFit::AddMsInThickMaterial(fvec radThick, fvec qp0, fvec w, fvec thickness, bool fDownstream)
{
  cnst ONE = 1.;

  fvec tx    = fTr.tx;
  fvec ty    = fTr.ty;
  fvec txtx  = tx * tx;
  fvec tyty  = ty * ty;
  fvec txtx1 = txtx + ONE;
  fvec h     = txtx + tyty;
  fvec t     = sqrt(txtx1 + tyty);
  fvec h2    = h * h;
  fvec qp0t  = qp0 * t;

  cnst c1(0.0136), c2 = c1 * fvec(0.038), c3 = c2 * fvec(0.5), c4 = -c3 / fvec(2.0), c5 = c3 / fvec(3.0),
                   c6 = -c3 / fvec(4.0);

  fvec s0 = (c1 + c2 * log(radThick) + c3 * h + h2 * (c4 + c5 * h + c6 * h2)) * qp0t;
  //fvec a = ( (ONE+mass2*qp0*qp0t)*radThick*s0*s0 );
  fvec a = ((t + fMass2 * qp0 * qp0t) * radThick * s0 * s0);

  fvec D   = (fDownstream) ? fvec(1.) : fvec(-1.);
  fvec T23 = (thickness * thickness) / fvec(3.0);
  fvec T2  = thickness / fvec(2.0);

  fTr.C00 += w * txtx1 * a * T23;
  fTr.C10 += w * tx * ty * a * T23;
  fTr.C20 += w * txtx1 * a * D * T2;
  fTr.C30 += w * tx * ty * a * D * T2;

  fTr.C11 += w * (ONE + tyty) * a * T23;
  fTr.C21 += w * tx * ty * a * D * T2;
  fTr.C31 += w * (ONE + tyty) * a * D * T2;

  fTr.C22 += w * txtx1 * a;
  fTr.C32 += w * tx * ty * a;
  fTr.C33 += w * (ONE + tyty) * a;
}


void L1TrackParFit::EnergyLossCorrection(const fvec& radThick, fvec& qp0, fvec direction, fvec w)
{
  const fvec qp2cut(1. / (10. * 10.));  // 10 GeV cut
  const fvec qp02 = max(qp0 * qp0, qp2cut);
  const fvec p2   = fvec(1.) / qp02;
  const fvec E2   = fMass2 + p2;

  const fvec bethe = L1Fit::ApproximateBetheBloch(p2 / fMass2);

  fvec tr = sqrt(fvec(1.f) + fTr.tx * fTr.tx + fTr.ty * fTr.ty);

  const fvec dE = bethe * radThick * tr * 2.33f * 9.34961f;

  const fvec E2Corrected = (sqrt(E2) + direction * dE) * (sqrt(E2) + direction * dE);
  fvec corr              = sqrt(p2 / (E2Corrected - fMass2));
  fmask ok               = (corr == corr) & (fvec::Zero() < w);
  corr                   = iif(ok, corr, fvec::One());

  qp0 *= corr;
  fTr.qp *= corr;
  fTr.C40 *= corr;
  fTr.C41 *= corr;
  fTr.C42 *= corr;
  fTr.C43 *= corr;
  fTr.C44 *= corr * corr;
  fTr.C54 *= corr;
}

template<int atomicZ>
void L1TrackParFit::EnergyLossCorrection(float atomicA, float rho, float radLen, const fvec& radThick, fvec& qp0,
                                         fvec direction, fvec w)
{
  const fvec qp2cut(1. / (10. * 10.));  // 10 GeV cut
  const fvec qp02 = max(qp0 * qp0, qp2cut);
  const fvec p2   = fvec(1.) / qp02;
  const fvec E2   = fMass2 + p2;

  fvec i;
  if (atomicZ < 13) i = (12. * atomicZ + 7.) * 1.e-9;
  else
    i = (9.76 * atomicZ + 58.8 * std::pow(atomicZ, -0.19)) * 1.e-9;

  const fvec bethe = L1Fit::ApproximateBetheBloch(p2 / fMass2, rho, 0.20, 3.00, i, atomicZ / atomicA);

  fvec tr = sqrt(fvec(1.f) + fTr.tx * fTr.tx + fTr.ty * fTr.ty);

  fvec dE = bethe * radThick * tr * radLen * rho;

  const fvec E2Corrected = (sqrt(E2) + direction * dE) * (sqrt(E2) + direction * dE);
  fvec corr              = sqrt(p2 / (E2Corrected - fMass2));
  fmask ok               = (corr == corr) & (fvec::Zero() < w);
  corr                   = iif(ok, corr, fvec::One());

  qp0 *= corr;
  fTr.qp *= corr;

  fvec P(sqrt(p2));  // GeV

  fvec Z(atomicZ);
  fvec A(atomicA);
  fvec RHO(rho);

  fvec STEP = radThick * tr * radLen;
  fvec EMASS(0.511 * 1e-3);  // GeV

  fvec BETA  = P / sqrt(E2Corrected);
  fvec GAMMA = sqrt(E2Corrected) / fMass;

  // Calculate xi factor (KeV).
  fvec XI = (fvec(153.5) * Z * STEP * RHO) / (A * BETA * BETA);

  // Maximum energy transfer to atomic electron (KeV).
  fvec ETA   = BETA * GAMMA;
  fvec ETASQ = ETA * ETA;
  fvec RATIO = EMASS / fMass;
  fvec F1    = fvec(2.) * EMASS * ETASQ;
  fvec F2    = fvec(1.) + fvec(2.) * RATIO * GAMMA + RATIO * RATIO;
  fvec EMAX  = fvec(1e6) * F1 / F2;

  fvec DEDX2 = XI * EMAX * (fvec(1.) - (BETA * BETA / fvec(2.))) * fvec(1e-12);

  fvec P2    = P * P;
  fvec SDEDX = (E2 * DEDX2) / (P2 * P2 * P2);

  //   T.fTr.C40 *= corr;
  //   T.fTr.C41 *= corr;
  //   T.fTr.C42 *= corr;
  //   T.fTr.C43 *= corr;
  // T.fTr.C44 *= corr*corr;
  fTr.C44 += abs(SDEDX);
}


void L1TrackParFit::EnergyLossCorrectionIron(const fvec& radThick, fvec& qp0, fvec direction, fvec w)
{
  constexpr int atomicZ   = 26;
  constexpr float atomicA = 55.845f;
  constexpr float rho     = 7.87;
  constexpr float radLen  = 1.758f;
  EnergyLossCorrection<atomicZ>(atomicA, rho, radLen, radThick, qp0, direction, w);
}


void L1TrackParFit::EnergyLossCorrectionCarbon(const fvec& radThick, fvec& qp0, fvec direction, fvec w)
{
  constexpr int atomicZ   = 6;
  constexpr float atomicA = 12.011f;
  constexpr float rho     = 2.265;
  constexpr float radLen  = 18.76f;
  EnergyLossCorrection<atomicZ>(atomicA, rho, radLen, radThick, qp0, direction, w);
}

void L1TrackParFit::EnergyLossCorrectionAl(const fvec& radThick, fvec& qp0, fvec direction, fvec w)
{
  constexpr int atomicZ   = 13;
  constexpr float atomicA = 26.981f;
  constexpr float rho     = 2.70f;
  constexpr float radLen  = 2.265f;
  EnergyLossCorrection<atomicZ>(atomicA, rho, radLen, radThick, qp0, direction, w);
}

void L1TrackParFit::GetExtrapolatedXYline(const fvec& z, const L1FieldRegion& F, fvec& extrX, fvec& extrY, fvec Jx[6],
                                          fvec Jy[6]) const
{
  // extrapolate track assuming it is straight (qp==0)
  // return the extrapolated X, Y and the derivatives of the extrapolated X and Y

  cnst c_light(0.000299792458), c1(1.), c2i(0.5), c6i(1. / 6.), c12i(1. / 12.);

  const fvec& tx = fTr.tx;
  const fvec& ty = fTr.ty;
  fvec dz        = z - fTr.z;

  fvec dz2     = dz * dz;
  fvec dzc6i   = dz * c6i;
  fvec dz2c12i = dz2 * c12i;

  fvec xx = tx * tx;
  fvec yy = ty * ty;
  fvec xy = tx * ty;

  fvec Ay = -xx - c1;
  fvec Bx = yy + c1;

  fvec ctdz2 = c_light * sqrt(c1 + xx + yy) * dz2;

  fvec Sx = F.cx0 * c2i + F.cx1 * dzc6i + F.cx2 * dz2c12i;
  fvec Sy = F.cy0 * c2i + F.cy1 * dzc6i + F.cy2 * dz2c12i;
  fvec Sz = F.cz0 * c2i + F.cz1 * dzc6i + F.cz2 * dz2c12i;

  extrX = fTr.x + tx * dz;
  extrY = fTr.y + ty * dz;

  Jx[0] = fvec(1.);
  Jx[1] = fvec(0.);
  Jx[2] = dz;
  Jx[3] = fvec(0.);
  Jx[4] = ctdz2 * (Sx * xy + Sy * Ay + Sz * ty);
  Jx[5] = fvec(0.);

  Jy[0] = fvec(0.);
  Jy[1] = fvec(1.);
  Jy[2] = fvec(0.);
  Jy[3] = dz;
  Jy[4] = ctdz2 * (Sx * Bx - Sy * xy - Sz * tx);
  Jy[5] = fvec(0.);
}


void L1TrackParFit::AddTargetToLine(const fvec& targX, const fvec& targY, const fvec& targZ,
                                    const L1XYMeasurementInfo& targXYInfo, const L1FieldRegion& F)
{
  // Add the target constraint to a straight line track

  fvec eX, eY, Jx[6], Jy[6];
  GetExtrapolatedXYline(targZ, F, eX, eY, Jx, Jy);
  FilterExtrapolatedXY(targX, targY, targXYInfo, eX, eY, Jx, Jy);
}

#undef cnst
