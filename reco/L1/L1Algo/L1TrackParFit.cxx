/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak [committer], Valentina Akishina */

#include "L1TrackParFit.h"

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

void L1TrackParFit::FilterNoP(L1UMeasurementInfo& info, fvec u, fvec w)
{
  fvec wi, zeta, zetawi, HCH;
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

#if 0  // use mask
  const fmask mask = (HCH < info.sigma2 * 16.);
  wi = w/( (mask & info.sigma2) +HCH );
  zetawi = zeta *wi;
  fTr.chi2 +=  mask & (zeta * zetawi);
#else
  wi     = w / (info.sigma2 + HCH);
  zetawi = zeta * wi;
  fTr.chi2 += zeta * zetawi;
#endif  // 0
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
  //  fTr.qp -= F4*zetawi;
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
  //   fTr.C40-= K4*F0;
  //   fTr.C41-= K4*F1;
  //   fTr.C42-= K4*F2;
  //   fTr.C43-= K4*F3;
  //   fTr.C44-= K4*F4;
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


void L1TrackParFit::ExtrapolateLine(fvec z_out, fvec* w)
{

  cnst c_light = 29.9792458;

  fvec dz = (z_out - fTr.z);
  if (w) { dz.setZero(*w <= fvec::Zero()); }

  fTr.x += dz * fTr.tx;
  fTr.y += dz * fTr.ty;
  fTr.z += dz;
  fTr.t += dz * sqrt(fvec(1.) + fTr.tx * fTr.tx + fTr.ty * fTr.ty) / c_light;

  const fvec k1 = dz * fTr.tx / (c_light * sqrt(fTr.tx * fTr.tx + fTr.ty * fTr.ty + fvec(1.)));
  const fvec k2 = dz * fTr.ty / (c_light * sqrt(fTr.tx * fTr.tx + fTr.ty * fTr.ty + fvec(1.)));

  const fvec dzC32_in = dz * fTr.C32;

  fTr.C21 += dzC32_in;
  fTr.C10 += dz * (fTr.C21 + fTr.C30);

  const fvec C20_in = fTr.C20;

  fTr.C20 += dz * fTr.C22;
  fTr.C00 += dz * (fTr.C20 + C20_in);

  const fvec C31_in = fTr.C31;

  fTr.C31 += dz * fTr.C33;
  fTr.C11 += dz * (fTr.C31 + C31_in);
  fTr.C30 += dzC32_in;

  fTr.C40 += dz * fTr.C42;
  fTr.C41 += dz * fTr.C43;

  fvec c52 = fTr.C52;
  fvec c53 = fTr.C53;

  fTr.C50 += k1 * fTr.C20 + k2 * fTr.C30;
  fTr.C51 += k1 * fTr.C21 + k2 * fTr.C31;
  fTr.C52 += k1 * fTr.C22 + k2 * fTr.C32;
  fTr.C53 += k1 * fTr.C32 + k2 * fTr.C33;
  fTr.C54 += k1 * fTr.C42 + k2 * fTr.C43;
  fTr.C55 += k1 * (fTr.C52 + c52) + k2 * (fTr.C53 + c53);
}

void L1TrackParFit::ExtrapolateLine1(fvec z_out, fvec* w, fvec v)
{

  cnst c_light(29.9792458);

  fvec dz = (z_out - fTr.z);
  if (w) { dz.setZero(*w <= fvec::Zero()); }

  fTr.x += dz * fTr.tx;
  fTr.y += dz * fTr.ty;
  fTr.z += dz;

  fTr.t += dz * sqrt(fvec(1.) + fTr.tx * fTr.tx + fTr.ty * fTr.ty) / (v * c_light);

  const fvec k1 = dz * fTr.tx / ((v * c_light) * sqrt(fTr.tx * fTr.tx + fTr.ty * fTr.ty + fvec(1.)));
  const fvec k2 = dz * fTr.ty / ((v * c_light) * sqrt(fTr.tx * fTr.tx + fTr.ty * fTr.ty + fvec(1.)));

  const fvec dzC32_in = dz * fTr.C32;

  fTr.C21 += dzC32_in;
  fTr.C10 += dz * (fTr.C21 + fTr.C30);

  const fvec C20_in = fTr.C20;

  fTr.C20 += dz * fTr.C22;
  fTr.C00 += dz * (fTr.C20 + C20_in);

  const fvec C31_in = fTr.C31;

  fTr.C31 += dz * fTr.C33;
  fTr.C11 += dz * (fTr.C31 + C31_in);
  fTr.C30 += dzC32_in;

  fTr.C40 += dz * fTr.C42;
  fTr.C41 += dz * fTr.C43;

  fvec c52 = fTr.C52;
  fvec c53 = fTr.C53;

  fTr.C50 += k1 * fTr.C20 + k2 * fTr.C30;
  fTr.C51 += k1 * fTr.C21 + k2 * fTr.C31;
  fTr.C52 += k1 * fTr.C22 + k2 * fTr.C32;
  fTr.C53 += k1 * fTr.C32 + k2 * fTr.C33;
  fTr.C54 += k1 * fTr.C42 + k2 * fTr.C43;
  fTr.C55 += k1 * (fTr.C52 + c52) + k2 * (fTr.C53 + c53);
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
  //        | x |          tx
  //        | y |          ty
  // d/dz = | tx| = fTr.t * ( ty * ( B(3)+tx*b(1) ) - (1+tx**2)*B(2) )
  //        | ty|   fTr.t * (-tx * ( B(3)+ty+b(2)   - (1+ty**2)*B(1) )  ,
  //
  //   where  fTr.t = c_light*qp*sqrt ( 1 + tx**2 + ty**2 ) .
  //
  //  In the following for RK step  :
  //
  //     x=x[0], y=x[1], tx=x[3], ty=x[4].
  //
  //========================================================================
  //
  //  NIM A395 (1997) 169-184; NIM A426 (1999) 268-282.
  //
  //  the routine is based on LHC(b) utility code
  //
  //========================================================================


  cnst c_light = 0.000299792458;

  const fvec a[4] = {0., 0.5, 0.5, 1.};
  const fvec c[4] = {1. / 6., 1. / 3., 1. / 3., 1. / 6.};
  const fvec b[4] = {0., 0.5, 0.5, 1.};

  fvec k[20], x[5], k1[20];
  fvec Ax[4], Ay[4], Ax_tx[4], Ay_tx[4], Ax_ty[4], Ay_ty[4], At[4], At_tx[4], At_ty[4];

  //----------------------------------------------------------------

  z_out = iif((fvec(0.f) < w), z_out, fTr.z);

  fvec qp_in   = fTr.qp;
  const fvec h = (z_out - fTr.z);

  //   cout<<h<<" h"<<endl;
  //   cout<<fTr.tx<<" fTr.tx"<<endl;
  //   cout<<fTr.ty<<" fTr.ty"<<endl;

  fvec hC = h * c_light;
  //std::cout << "fTr.x " << fTr.x << std::endl;
  fvec x0[5];
  x0[0] = fTr.x;
  x0[1] = fTr.y;
  x0[2] = fTr.tx;
  x0[3] = fTr.ty;
  x0[4] = fTr.t;

  //
  //   Runge-Kutta step
  //

  for (int step = 0; step < 4; ++step) {

    for (int i = 0; i < 5; ++i) {
      if (step == 0) { x[i] = x0[i]; }
      else {
        x[i] = x0[i] + b[step] * k[step * 5 - 5 + i];
      }
    }

    fvec B[3];

    F.Get(x[0], x[1], fTr.z + a[step] * h, B);
    //std::cout << "extrapolation step " << step << " z " << z_in + a[step] * h << " field " << B[0] << " " << B[1] << " "
    //        << B[2] << std::endl;

    fvec tx      = x[2];
    fvec ty      = x[3];
    fvec tx2     = tx * tx;
    fvec ty2     = ty * ty;
    fvec txty    = tx * ty;
    fvec tx2ty21 = fvec(1.) + tx2 + ty2;
    // if( tx2ty21 > 1.e4 ) return 1;
    fvec I_tx2ty21 = fvec(1.) / tx2ty21 * qp0;
    fvec tx2ty2    = sqrt(tx2ty21);
    //   fvec I_tx2ty2 = qp0 * hC / tx2ty2 ; unsused ???
    tx2ty2 *= hC;
    fvec tx2ty2qp = tx2ty2 * qp0;

    //     cout<<B[0]<<" B["<<step<<"][0] "<<B[2]<<" B["<<step<<"][2] "<<B[1]<<" B["<<step<<"][1]"<<endl;
    Ax[step] = (txty * B[0] + ty * B[2] - (fvec(1.) + tx2) * B[1]) * tx2ty2;
    Ay[step] = (-txty * B[1] - tx * B[2] + (fvec(1.) + ty2) * B[0]) * tx2ty2;

    Ax_tx[step] = Ax[step] * tx * I_tx2ty21 + (ty * B[0] - fvec(2.) * tx * B[1]) * tx2ty2qp;
    Ax_ty[step] = Ax[step] * ty * I_tx2ty21 + (tx * B[0] + B[2]) * tx2ty2qp;
    Ay_tx[step] = Ay[step] * tx * I_tx2ty21 + (-ty * B[1] - B[2]) * tx2ty2qp;
    Ay_ty[step] = Ay[step] * ty * I_tx2ty21 + (-tx * B[1] + fvec(2.) * ty * B[0]) * tx2ty2qp;

    fvec m2     = fMass2;
    fvec vi     = sqrt(fvec(1.) + m2 * qp0 * qp0) / fvec(29.9792458f);
    fvec l      = sqrt(fvec(1.) + tx * tx + ty * ty);
    At[step]    = h * l * vi;
    At_tx[step] = h * tx / l * vi;
    At_ty[step] = h * ty / l * vi;

    //     cout<<Ax[step]<<" Ax[step] "<<Ay[step]<<" ay "<<At[step]<<" At[step] "<<qp0<<" qp0 "<<h<<" h"<<endl;

    int step5 = step * 5;

    k[step5 + 0] = tx * h;
    k[step5 + 1] = ty * h;
    k[step5 + 2] = Ax[step] * qp0;
    k[step5 + 3] = Ay[step] * qp0;
    k[step5 + 4] = At[step];
  }  // end of Runge-Kutta steps

  {


    //     cout<<x0[0]<<" x0[0] "<<c[0]<<" c 0 "<<k[0]<<" k0 "<<c[1]<<" c1 "<<k[5+0]<<" k5 "<<c[2]<<" c2 "<<k[10+0]<<" k10"<<c[3]<<" c3 "<<k[15+0]<<" k15"<<endl;

    //     cout << "w = " << *w << "; ";
    //     cout << "initialised = " << initialised << "; ";
    //     cout << "fTr.x = " << fTr.x;

    //std::cout << " x : x0[0] " << x0[0] << " k[0] " << k[0] << " k[5] " << k[5] << " k[10] " << k[10] << " k[15] "
    //        << k[15] << std::endl;

    fTr.x  = x0[0] + c[0] * k[0] + c[1] * k[5 + 0] + c[2] * k[10 + 0] + c[3] * k[15 + 0];
    fTr.y  = x0[1] + c[0] * k[1] + c[1] * k[5 + 1] + c[2] * k[10 + 1] + c[3] * k[15 + 1];
    fTr.tx = x0[2] + c[0] * k[2] + c[1] * k[5 + 2] + c[2] * k[10 + 2] + c[3] * k[15 + 2];
    fTr.ty = x0[3] + c[0] * k[3] + c[1] * k[5 + 3] + c[2] * k[10 + 3] + c[3] * k[15 + 3];
    fTr.t  = x0[4] + c[0] * k[4] + c[1] * k[5 + 4] + c[2] * k[10 + 4] + c[3] * k[15 + 4];
    fTr.z  = z_out;

    //     cout << "; fTr.x = " << fTr.x << endl;
  }
  //   cout<<fTr.x<<" fTr.x"<<endl;

  fvec J[36];  // Jacobian of extrapolation

  //
  //    derivatives dx/dx and dx/dy
  //

  for (int i = 0; i < 12; ++i) {
    J[i] = fvec(0.);
  }

  J[0] = fvec(1.);
  J[7] = fvec(1.);

  //
  //     Derivatives    dx/tx
  //

  x0[0] = fvec(0.);
  x0[1] = fvec(0.);
  x0[2] = fvec(1.);
  x0[3] = fvec(0.);
  x0[4] = fvec(0.);

  //
  //   Runge-Kutta step for derivatives dx/dtx
  //

  for (int step = 0; step < 4; ++step) {
    int step5 = step * 5;
    for (int i = 0; i < 5; ++i) {
      if (step == 0) { x[i] = x0[i]; }
      else if (i != 2) {
        x[i] = x0[i] + b[step] * k1[step5 - 5 + i];
      }
    }

    k1[step5 + 0] = x[2] * h;
    k1[step5 + 1] = x[3] * h;
    // k1[step5+2] = Ax_tx[step] * x[2] + Ax_ty[step] * x[3];
    k1[step5 + 3] = Ay_tx[step] * x[2] + Ay_ty[step] * x[3];
    k1[step5 + 4] = At_tx[step] * x[2] + At_ty[step] * x[3];
  }  // end of Runge-Kutta steps for derivatives dx/dtx

  for (int i = 0; i < 4; ++i) {
    if (i != 2) { J[12 + i] = x0[i] + c[0] * k1[i] + c[1] * k1[5 + i] + c[2] * k1[10 + i] + c[3] * k1[15 + i]; }
  }

  J[14] = fvec(1.);
  J[16] = fvec(0.);
  J[17] = x0[4] + c[0] * k1[4] + c[1] * k1[5 + 4] + c[2] * k1[10 + 4] + c[3] * k1[15 + 4];

  //      end of derivatives dx/dtx

  //
  //     Derivatives    dx/ty
  //

  x0[0] = fvec(0.);
  x0[1] = fvec(0.);
  x0[2] = fvec(0.);
  x0[3] = fvec(1.);
  x0[4] = fvec(0.);

  //
  //   Runge-Kutta step for derivatives dx/dty
  //

  for (int step = 0; step < 4; ++step) {
    int step5 = step * 5;
    for (int i = 0; i < 5; ++i) {
      if (step == 0) {
        x[i] = x0[i];  // ty fixed
      }
      else if (i != 3) {
        x[i] = x0[i] + b[step] * k1[step5 - 5 + i];
      }
    }
    k1[step5 + 0] = x[2] * h;
    k1[step5 + 1] = x[3] * h;
    k1[step5 + 2] = Ax_tx[step] * x[2] + Ax_ty[step] * x[3];
    // k1[step5+3] = Ay_tx[step] * x[2] + Ay_ty[step] * x[3]; //  TODO: SG: check if the simplification below is ok
    k1[step5 + 4] = At_tx[step] * x[2] + At_ty[step] * x[3];
  }  // end of Runge-Kutta steps for derivatives dx/dty

  for (int i = 0; i < 3; ++i) {
    J[18 + i] = x0[i] + c[0] * k1[i] + c[1] * k1[5 + i] + c[2] * k1[10 + i] + c[3] * k1[15 + i];
  }

  J[21] = fvec(1.);
  J[22] = fvec(0.);
  J[23] = x0[4] + c[0] * k1[4] + c[1] * k1[5 + 4] + c[2] * k1[10 + 4] + c[3] * k1[15 + 4];

  //      end of derivatives dx/dty

  //
  //     Derivatives    dx/dqp
  //

  x0[0] = fvec(0.);
  x0[1] = fvec(0.);
  x0[2] = fvec(0.);
  x0[3] = fvec(0.);
  x0[4] = fvec(0.);

  //
  //   Runge-Kutta step for derivatives dx/dqp
  //

  for (int step = 0; step < 4; ++step) {
    for (int i = 0; i < 5; ++i) {
      if (step == 0) { x[i] = x0[i]; }
      else {
        x[i] = x0[i] + b[step] * k1[step * 5 - 5 + i];
      }
    }
    int step5     = step * 5;
    k1[step5 + 0] = x[2] * h;
    k1[step5 + 1] = x[3] * h;
    k1[step5 + 2] = Ax[step] + Ax_tx[step] * x[2] + Ax_ty[step] * x[3];
    k1[step5 + 3] = Ay[step] + Ay_tx[step] * x[2] + Ay_ty[step] * x[3];
    k1[step5 + 4] = At[step] + At_tx[step] * x[2] + At_ty[step] * x[3];

  }  // end of Runge-Kutta steps for derivatives dx/dqp

  for (int i = 0; i < 4; ++i) {
    J[24 + i] = x0[i] + c[0] * k1[i] + c[1] * k1[5 + i] + c[2] * k1[10 + i] + c[3] * k1[15 + i];
  }
  J[28] = fvec(1.);
  J[29] = x0[4] + c[0] * k1[4] + c[1] * k1[5 + 4] + c[2] * k1[10 + 4] + c[3] * k1[15 + 4];

  //      end of derivatives dx/dqp

  //
  //    derivatives dx/dt
  //

  for (int i = 30; i < 35; i++) {
    J[i] = fvec(0.);
  }
  J[35] = fvec(1.);

  //      end of derivatives dx/dt

  fvec dqp = qp_in - qp0;

  {  // update parameters
    fTr.x += J[6 * 4 + 0] * dqp;
    fTr.y += J[6 * 4 + 1] * dqp;
    fTr.tx += J[6 * 4 + 2] * dqp;
    fTr.ty += J[6 * 4 + 3] * dqp;
    fTr.t += J[6 * 4 + 5] * dqp;
  }
  //    cout<<fTr.x<<" fTr.x"<<endl;
  //          covariance matrix transport

  //cout<< (fTr.t - fTr.t_old)<<" fTr.t dt "<<endl;

  //   // if ( C_in&&C_out ) CbmKFMath::multQtSQ( 5, J, C_in, C_out); // TODO
  //   j(0,2) = J[6*2 + 0];
  //   j(1,2) = J[6*2 + 1];
  //   j(2,2) = J[6*2 + 2];
  //   j(3,2) = J[6*2 + 3];
  //   j(4,2) = J[6*2 + 4];
  //   j(5,2) = J[6*2 + 5];
  //
  //   j(0,3) = J[6*3 + 0];
  //   j(1,3) = J[6*3 + 1];
  //   j(2,3) = J[6*3 + 2];
  //   j(3,3) = J[6*3 + 3];
  //   j(4,3) = J[6*3 + 4];
  //   j(5,3) = J[6*3 + 5];
  //
  //   j(0,4) = J[6*4 + 0];
  //   j(1,4) = J[6*4 + 1];
  //   j(2,4) = J[6*4 + 2];
  //   j(3,4) = J[6*4 + 3];
  //   j(4,4) = J[6*4 + 4];
  //   j(5,4) = J[6*4 + 5];

  const fvec c42 = fTr.C42, c43 = fTr.C43;

  const fvec cj00 = fTr.C00 + fTr.C20 * J[6 * 2 + 0] + fTr.C30 * J[6 * 3 + 0] + fTr.C40 * J[6 * 4 + 0];
  const fvec cj10 = fTr.C10 + fTr.C21 * J[6 * 2 + 0] + fTr.C31 * J[6 * 3 + 0] + fTr.C41 * J[6 * 4 + 0];
  const fvec cj20 = fTr.C20 + fTr.C22 * J[6 * 2 + 0] + fTr.C32 * J[6 * 3 + 0] + c42 * J[6 * 4 + 0];
  const fvec cj30 = fTr.C30 + fTr.C32 * J[6 * 2 + 0] + fTr.C33 * J[6 * 3 + 0] + c43 * J[6 * 4 + 0];
  const fvec cj40 = fTr.C40 + fTr.C42 * J[6 * 2 + 0] + fTr.C43 * J[6 * 3 + 0] + fTr.C44 * J[6 * 4 + 0];
  const fvec cj50 = fTr.C50 + fTr.C52 * J[6 * 2 + 0] + fTr.C53 * J[6 * 3 + 0] + fTr.C54 * J[6 * 4 + 0];

  //  const fvec cj01 = fTr.C10 + fTr.C20*J[6*2 + 1] + fTr.C30*J[6*3 + 1] + fTr.C40*J[6*4 + 1];
  const fvec cj11 = fTr.C11 + fTr.C21 * J[6 * 2 + 1] + fTr.C31 * J[6 * 3 + 1] + fTr.C41 * J[6 * 4 + 1];
  const fvec cj21 = fTr.C21 + fTr.C22 * J[6 * 2 + 1] + fTr.C32 * J[6 * 3 + 1] + c42 * J[6 * 4 + 1];
  const fvec cj31 = fTr.C31 + fTr.C32 * J[6 * 2 + 1] + fTr.C33 * J[6 * 3 + 1] + c43 * J[6 * 4 + 1];
  const fvec cj41 = fTr.C41 + fTr.C42 * J[6 * 2 + 1] + fTr.C43 * J[6 * 3 + 1] + fTr.C44 * J[6 * 4 + 1];
  const fvec cj51 = fTr.C51 + fTr.C52 * J[6 * 2 + 1] + fTr.C53 * J[6 * 3 + 1] + fTr.C54 * J[6 * 4 + 1];

  // const fvec cj02 = fTr.C20*J[6*2 + 2] + fTr.C30*J[6*3 + 2] + fTr.C40*J[6*4 + 2];
  // const fvec cj12 = fTr.C21*J[6*2 + 2] + fTr.C31*J[6*3 + 2] + fTr.C41*J[6*4 + 2];
  const fvec cj22 = fTr.C22 * J[6 * 2 + 2] + fTr.C32 * J[6 * 3 + 2] + c42 * J[6 * 4 + 2];
  const fvec cj32 = fTr.C32 * J[6 * 2 + 2] + fTr.C33 * J[6 * 3 + 2] + c43 * J[6 * 4 + 2];
  const fvec cj42 = fTr.C42 * J[6 * 2 + 2] + fTr.C43 * J[6 * 3 + 2] + fTr.C44 * J[6 * 4 + 2];
  const fvec cj52 = fTr.C52 * J[6 * 2 + 2] + fTr.C53 * J[6 * 3 + 2] + fTr.C54 * J[6 * 4 + 2];

  // const fvec cj03 = fTr.C20*J[6*2 + 3] + fTr.C30*J[6*3 + 3] + fTr.C40*J[6*4 + 3];
  // const fvec cj13 = fTr.C21*J[6*2 + 3] + fTr.C31*J[6*3 + 3] + fTr.C41*J[6*4 + 3];
  const fvec cj23 = fTr.C22 * J[6 * 2 + 3] + fTr.C32 * J[6 * 3 + 3] + c42 * J[6 * 4 + 3];
  const fvec cj33 = fTr.C32 * J[6 * 2 + 3] + fTr.C33 * J[6 * 3 + 3] + c43 * J[6 * 4 + 3];
  const fvec cj43 = fTr.C42 * J[6 * 2 + 3] + fTr.C43 * J[6 * 3 + 3] + fTr.C44 * J[6 * 4 + 3];
  const fvec cj53 = fTr.C52 * J[6 * 2 + 3] + fTr.C53 * J[6 * 3 + 3] + fTr.C54 * J[6 * 4 + 3];

  const fvec cj24 = fTr.C42;
  const fvec cj34 = fTr.C43;
  const fvec cj44 = fTr.C44;
  const fvec cj54 = fTr.C54;

  // const fvec cj05 = fTr.C50 + fTr.C20*J[17] + fTr.C30*J[23] + fTr.C40*J[29];
  // const fvec cj15 = fTr.C51 + fTr.C21*J[17] + fTr.C31*J[23] + fTr.C41*J[29];
  const fvec cj25 = fTr.C52 + fTr.C22 * J[17] + fTr.C32 * J[23] + fTr.C42 * J[29];
  const fvec cj35 = fTr.C53 + fTr.C32 * J[17] + fTr.C33 * J[23] + fTr.C43 * J[29];
  const fvec cj45 = fTr.C54 + fTr.C42 * J[17] + fTr.C43 * J[23] + fTr.C44 * J[29];
  const fvec cj55 = fTr.C55 + fTr.C52 * J[17] + fTr.C53 * J[23] + fTr.C54 * J[29];


  fTr.C00 = cj00 + cj20 * J[12] + cj30 * J[18] + cj40 * J[24];

  fTr.C10 = cj10 + cj20 * J[13] + cj30 * J[19] + cj40 * J[25];
  fTr.C11 = cj11 + cj21 * J[13] + cj31 * J[19] + cj41 * J[25];

  fTr.C20 = cj20 + cj30 * J[20] + cj40 * J[26];
  fTr.C21 = cj21 + cj31 * J[20] + cj41 * J[26];
  fTr.C22 = cj22 + cj32 * J[20] + cj42 * J[26];

  fTr.C30 = cj30 + cj20 * J[15] + cj40 * J[27];
  fTr.C31 = cj31 + cj21 * J[15] + cj41 * J[27];
  fTr.C32 = cj32 + cj22 * J[15] + cj42 * J[27];
  fTr.C33 = cj33 + cj23 * J[15] + cj43 * J[27];

  fTr.C40 = cj40;
  fTr.C41 = cj41;
  fTr.C42 = cj42;
  fTr.C43 = cj43;
  fTr.C44 = cj44;

  fTr.C50 = cj50 + cj20 * J[17] + cj30 * J[23] + cj40 * J[29];
  fTr.C51 = cj51 + cj21 * J[17] + cj31 * J[23] + cj41 * J[29];
  fTr.C52 = cj52 + cj22 * J[17] + cj32 * J[23] + cj42 * J[29];
  fTr.C53 = cj53 + cj23 * J[17] + cj33 * J[23] + cj43 * J[29];
  fTr.C54 = cj54 + cj24 * J[17] + cj34 * J[23] + cj44 * J[29];
  fTr.C55 = cj55 + cj25 * J[17] + cj35 * J[23] + cj45 * J[29];
}

void L1TrackParFit::AddPipeMaterial(fvec qp0, fvec w)
{
  cnst ONE = 1.f;

  //  static const fscal RadThick=0.0009f;//0.5/18.76;
  //  static const fscal logRadThick=log(RadThick);
  //const fscal RadThick=0.0009f;//0.5/18.76;

  const fscal logRadThick = log(fPipeRadThick[0]);
  fvec tx                 = fTr.tx;
  fvec ty                 = fTr.ty;
  fvec txtx               = fTr.tx * fTr.tx;
  fvec tyty               = fTr.ty * fTr.ty;
  fvec txtx1              = txtx + ONE;
  fvec h                  = txtx + tyty;
  fvec t                  = sqrt(txtx1 + tyty);
  fvec h2                 = h * h;
  fvec qp0t               = qp0 * t;

  cnst c1 = 0.0136f, c2 = c1 * 0.038f, c3 = c2 * 0.5f, c4 = -c3 / 2.0f, c5 = c3 / 3.0f, c6 = -c3 / 4.0f;
  fvec s0 = (c1 + c2 * fvec(logRadThick) + c3 * h + h2 * (c4 + c5 * h + c6 * h2)) * qp0t;
  //fvec a = ( (ONE+mass2*qp0*qp0t)*RadThick*s0*s0 );
  fvec a = ((t + fMass2 * qp0 * qp0t) * fPipeRadThick * s0 * s0);

  fTr.C22 += w * txtx1 * a;
  fTr.C32 += w * tx * ty * a;
  fTr.C33 += w * (ONE + tyty) * a;
}


void L1TrackParFit::AddMaterial(const fvec& radThick, fvec qp0, fvec w)
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

void L1TrackParFit::AddThickMaterial(fvec radThick, fvec qp0, fvec w, fvec thickness, bool fDownstream)
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


void L1TrackParFit::AddMaterial(const L1MaterialInfo& info, fvec qp0, fvec w)
{
  cnst ONE = 1.f;

  fvec tx = fTr.tx;
  fvec ty = fTr.ty;
  // fvec time = fTr.t;
  fvec txtx  = tx * tx;
  fvec tyty  = ty * ty;
  fvec txtx1 = txtx + ONE;
  fvec h     = txtx + tyty;
  fvec t     = sqrt(txtx1 + tyty);
  fvec h2    = h * h;
  fvec qp0t  = qp0 * t;

  cnst c1 = 0.0136f, c2 = c1 * 0.038f, c3 = c2 * 0.5f, c4 = -c3 / 2.0f, c5 = c3 / 3.0f, c6 = -c3 / 4.0f;

  fvec s0 = (c1 + c2 * info.logRadThick + c3 * h + h2 * (c4 + c5 * h + c6 * h2)) * qp0t;
  //fvec a = ( (ONE+mass2*qp0*qp0t)*info.RadThick*s0*s0 );
  fvec a = ((t + fMass2 * qp0 * qp0t) * info.RadThick * s0 * s0);

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

#undef cnst
