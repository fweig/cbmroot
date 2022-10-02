/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak [committer], Valentina Akishina */

#include "L1TrackParFit.h"

#include "L1Fit.h"

const fvec PipeRadThick   = 7.87e-3f;      // 0.7 mm Aluminium
const fvec TargetRadThick = 3.73e-2f * 2;  // 250 mum Gold


#define cnst const fvec

void L1TrackParFit::Filter(L1UMeasurementInfo& info, fvec u, fvec w)
{
  fvec zeta, HCH;
  fvec F0, F1, F2, F3, F4, F5;
  fvec K1, K2, K3, K4, K5;

  zeta = info.cos_phi * fx + info.sin_phi * fy - u;

  // F = CH'
  F0 = info.cos_phi * C00 + info.sin_phi * C10;
  F1 = info.cos_phi * C10 + info.sin_phi * C11;

  HCH = (F0 * info.cos_phi + F1 * info.sin_phi);

  F2 = info.cos_phi * C20 + info.sin_phi * C21;
  F3 = info.cos_phi * C30 + info.sin_phi * C31;
  F4 = info.cos_phi * C40 + info.sin_phi * C41;
  F5 = info.cos_phi * C50 + info.sin_phi * C51;

  const fmask maskDoFilter = (HCH < info.sigma2 * 16.f);
  //const fvec maskDoFilter = _f32vec4_true;

  // correction to HCH is needed for the case when sigma2 is so small
  // with respect to HCH that it disappears due to the roundoff error
  //
  fvec wi     = w / (info.sigma2 + fvec(1.0000001) * HCH);
  fvec zetawi = w * zeta / (iif(maskDoFilter, info.sigma2, fvec::Zero()) + HCH);

  // chi2 += iif( maskDoFilter, zeta * zetawi, fvec::Zero() );
  chi2 += zeta * zeta * wi;
  NDF += w;

  K1 = F1 * wi;
  K2 = F2 * wi;
  K3 = F3 * wi;
  K4 = F4 * wi;
  K5 = F5 * wi;

  fx -= F0 * zetawi;
  fy -= F1 * zetawi;
  ftx -= F2 * zetawi;
  fty -= F3 * zetawi;
  fqp -= F4 * zetawi;
  ft -= F5 * zetawi;

  C00 -= F0 * F0 * wi;
  C10 -= K1 * F0;
  C11 -= K1 * F1;
  C20 -= K2 * F0;
  C21 -= K2 * F1;
  C22 -= K2 * F2;
  C30 -= K3 * F0;
  C31 -= K3 * F1;
  C32 -= K3 * F2;
  C33 -= K3 * F3;
  C40 -= K4 * F0;
  C41 -= K4 * F1;
  C42 -= K4 * F2;
  C43 -= K4 * F3;
  C44 -= K4 * F4;
  C50 -= K5 * F0;
  C51 -= K5 * F1;
  C52 -= K5 * F2;
  C53 -= K5 * F3;
  C54 -= K5 * F4;
  C55 -= K5 * F5;
}

void L1TrackParFit::FilterNoP(L1UMeasurementInfo& info, fvec u, fvec w)
{
  fvec wi, zeta, zetawi, HCH;
  fvec F0, F1, F2, F3, F4, F5;
  fvec K1, K2, K3, K4, K5;

  zeta = info.cos_phi * fx + info.sin_phi * fy - u;

  // F = CH'
  F0 = info.cos_phi * C00 + info.sin_phi * C10;
  F1 = info.cos_phi * C10 + info.sin_phi * C11;

  HCH = (F0 * info.cos_phi + F1 * info.sin_phi);

  F2 = info.cos_phi * C20 + info.sin_phi * C21;
  F3 = info.cos_phi * C30 + info.sin_phi * C31;
  F4 = info.cos_phi * C40 + info.sin_phi * C41;
  F5 = info.cos_phi * C50 + info.sin_phi * C51;

#if 0  // use mask
  const fmask mask = (HCH < info.sigma2 * 16.);
  wi = w/( (mask & info.sigma2) +HCH );
  zetawi = zeta *wi;
  chi2 +=  mask & (zeta * zetawi);
#else
  wi     = w / (info.sigma2 + HCH);
  zetawi = zeta * wi;
  chi2 += zeta * zetawi;
#endif  // 0
  NDF += w;

  K1 = F1 * wi;
  K2 = F2 * wi;
  K3 = F3 * wi;
  K4 = F4 * wi;
  K5 = F5 * wi;

  fx -= F0 * zetawi;
  fy -= F1 * zetawi;
  ftx -= F2 * zetawi;
  fty -= F3 * zetawi;
  //  fqp -= F4*zetawi;
  ft -= F5 * zetawi;

  C00 -= F0 * F0 * wi;
  C10 -= K1 * F0;
  C11 -= K1 * F1;
  C20 -= K2 * F0;
  C21 -= K2 * F1;
  C22 -= K2 * F2;
  C30 -= K3 * F0;
  C31 -= K3 * F1;
  C32 -= K3 * F2;
  C33 -= K3 * F3;
  //   C40-= K4*F0;
  //   C41-= K4*F1;
  //   C42-= K4*F2;
  //   C43-= K4*F3;
  //   C44-= K4*F4;
  C50 -= K5 * F0;
  C51 -= K5 * F1;
  C52 -= K5 * F2;
  C53 -= K5 * F3;
  C54 -= K5 * F4;
  C55 -= K5 * F5;
}

void L1TrackParFit::FilterTime(fvec t, fvec dt, fvec w, fvec timeInfo)
{
  // filter track with a time measurement

  // F = CH'
  fvec F0 = C50;
  fvec F1 = C51;
  fvec F2 = C52;
  fvec F3 = C53;
  fvec F4 = C54;
  fvec F5 = C55;

  fvec HCH = C55;

  w.setZero(timeInfo <= fvec::Zero());

  fvec dt2 = dt * dt;

  // when dt0 is much smaller than current time error,
  // set track time exactly to the measurement value without filtering
  // it helps to keep the initial time errors reasonably small
  // the calculations in the covariance matrix are not affected

  const fmask maskDoFilter = (HCH < dt2 * 16.f);
  //const fvec maskDoFilter = _f32vec4_true;

  fvec wi     = w / (dt2 + 1.0000001f * HCH);
  fvec zeta   = ft - t;
  fvec zetawi = w * zeta / (iif(maskDoFilter, dt2, fvec::Zero()) + HCH);

  //T.chi2 += maskDoFilter & (zeta * zetawi);
  chi2 += zeta * zeta * wi;
  NDF += w;

  fvec K1 = F1 * wi;
  fvec K2 = F2 * wi;
  fvec K3 = F3 * wi;
  fvec K4 = F4 * wi;
  fvec K5 = F5 * wi;

  fx -= F0 * zetawi;
  fy -= F1 * zetawi;
  ftx -= F2 * zetawi;
  fty -= F3 * zetawi;
  fqp -= F4 * zetawi;
  ft -= F5 * zetawi;

  C00 -= F0 * F0 * wi;
  C10 -= K1 * F0;
  C11 -= K1 * F1;
  C20 -= K2 * F0;
  C21 -= K2 * F1;
  C22 -= K2 * F2;
  C30 -= K3 * F0;
  C31 -= K3 * F1;
  C32 -= K3 * F2;
  C33 -= K3 * F3;
  C40 -= K4 * F0;
  C41 -= K4 * F1;
  C42 -= K4 * F2;
  C43 -= K4 * F3;
  C44 -= K4 * F4;
  C50 -= K5 * F0;
  C51 -= K5 * F1;
  C52 -= K5 * F2;
  C53 -= K5 * F3;
  C54 -= K5 * F4;
  C55 -= K5 * F5;
}


void L1TrackParFit::FilterXY(const L1XYMeasurementInfo& info, fvec x, fvec y)
{
  cnst TWO(2.);

  fvec zeta0, zeta1, S00, S10, S11, si;
  fvec F00, F10, F20, F30, F40, F50, F01, F11, F21, F31, F41, F51;
  fvec K00, K10, K20, K30, K40, K50, K01, K11, K21, K31, K41, K51;

  zeta0 = fx - x;
  zeta1 = fy - y;

  // F = CH'
  F00 = C00;
  F10 = C10;
  F20 = C20;
  F30 = C30;
  F40 = C40;
  F50 = C50;
  F01 = C10;
  F11 = C11;
  F21 = C21;
  F31 = C31;
  F41 = C41;
  F51 = C51;

  S00 = F00 + info.C00;
  S10 = F10 + info.C10;
  S11 = F11 + info.C11;

  si          = 1.f / (S00 * S11 - S10 * S10);
  fvec S00tmp = S00;
  S00         = si * S11;
  S10         = -si * S10;
  S11         = si * S00tmp;

  chi2 += zeta0 * zeta0 * S00 + 2.f * zeta0 * zeta1 * S10 + zeta1 * zeta1 * S11;
  NDF += TWO;

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

  fx -= K00 * zeta0 + K01 * zeta1;
  fy -= K10 * zeta0 + K11 * zeta1;
  ftx -= K20 * zeta0 + K21 * zeta1;
  fty -= K30 * zeta0 + K31 * zeta1;
  fqp -= K40 * zeta0 + K41 * zeta1;

  C00 -= K00 * F00 + K01 * F01;
  C10 -= K10 * F00 + K11 * F01;
  C11 -= K10 * F10 + K11 * F11;
  C20 -= K20 * F00 + K21 * F01;
  C21 -= K20 * F10 + K21 * F11;
  C22 -= K20 * F20 + K21 * F21;
  C30 -= K30 * F00 + K31 * F01;
  C31 -= K30 * F10 + K31 * F11;
  C32 -= K30 * F20 + K31 * F21;
  C33 -= K30 * F30 + K31 * F31;
  C40 -= K40 * F00 + K41 * F01;
  C41 -= K40 * F10 + K41 * F11;
  C42 -= K40 * F20 + K41 * F21;
  C43 -= K40 * F30 + K41 * F31;
  C44 -= K40 * F40 + K41 * F41;
  C50 -= K50 * F00 + K51 * F01;
  C51 -= K50 * F10 + K51 * F11;
  C52 -= K50 * F20 + K51 * F21;
  C53 -= K50 * F30 + K51 * F31;
  C54 -= K50 * F40 + K51 * F41;
  C55 -= K50 * F50 + K51 * F51;
}


void L1TrackParFit::ExtrapolateLine(fvec z_out, fvec* w)
{

  cnst c_light = 29.9792458;

  fvec dz = (z_out - fz);
  if (w) { dz.setZero(*w <= fvec::Zero()); }

  fx += dz * ftx;
  fy += dz * fty;
  fz += dz;
  ft += dz * sqrt(fvec(1.) + ftx * ftx + fty * fty) / c_light;

  const fvec k1 = dz * ftx / (c_light * sqrt(ftx * ftx + fty * fty + fvec(1.)));
  const fvec k2 = dz * fty / (c_light * sqrt(ftx * ftx + fty * fty + fvec(1.)));

  const fvec dzC32_in = dz * C32;

  C21 += dzC32_in;
  C10 += dz * (C21 + C30);

  const fvec C20_in = C20;

  C20 += dz * C22;
  C00 += dz * (C20 + C20_in);

  const fvec C31_in = C31;

  C31 += dz * C33;
  C11 += dz * (C31 + C31_in);
  C30 += dzC32_in;

  C40 += dz * C42;
  C41 += dz * C43;

  fvec c52 = C52;
  fvec c53 = C53;

  C50 += k1 * C20 + k2 * C30;
  C51 += k1 * C21 + k2 * C31;
  C52 += k1 * C22 + k2 * C32;
  C53 += k1 * C32 + k2 * C33;
  C54 += k1 * C42 + k2 * C43;
  C55 += k1 * (C52 + c52) + k2 * (C53 + c53);
}

void L1TrackParFit::ExtrapolateLine1(fvec z_out, fvec* w, fvec v)
{

  cnst c_light(29.9792458);

  fvec dz = (z_out - fz);
  if (w) { dz.setZero(*w <= fvec::Zero()); }

  fx += dz * ftx;
  fy += dz * fty;
  fz += dz;

  ft += dz * sqrt(fvec(1.) + ftx * ftx + fty * fty) / (v * c_light);

  const fvec k1 = dz * ftx / ((v * c_light) * sqrt(ftx * ftx + fty * fty + fvec(1.)));
  const fvec k2 = dz * fty / ((v * c_light) * sqrt(ftx * ftx + fty * fty + fvec(1.)));

  const fvec dzC32_in = dz * C32;

  C21 += dzC32_in;
  C10 += dz * (C21 + C30);

  const fvec C20_in = C20;

  C20 += dz * C22;
  C00 += dz * (C20 + C20_in);

  const fvec C31_in = C31;

  C31 += dz * C33;
  C11 += dz * (C31 + C31_in);
  C30 += dzC32_in;

  C40 += dz * C42;
  C41 += dz * C43;

  fvec c52 = C52;
  fvec c53 = C53;

  C50 += k1 * C20 + k2 * C30;
  C51 += k1 * C21 + k2 * C31;
  C52 += k1 * C22 + k2 * C32;
  C53 += k1 * C32 + k2 * C33;
  C54 += k1 * C42 + k2 * C43;
  C55 += k1 * (C52 + c52) + k2 * (C53 + c53);
}

void L1TrackParFit::Extrapolate  // extrapolates track parameters and returns jacobian for extrapolation of CovMatrix
  (fvec z_out,                   // extrapolate to this z position
   fvec qp0,                     // use Q/p linearisation at this value
   const L1FieldRegion& F, const fvec& w)
{
  fvec sgn = iif(fz < z_out, fvec(1.), fvec(-1.));
  while (!(w * abs(z_out - fz) <= fvec(1.e-6)).isFull()) {
    fvec zNew                              = fz + sgn * fvec(50.);  // max. 50 cm step
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
  // d/dz = | tx| = ft * ( ty * ( B(3)+tx*b(1) ) - (1+tx**2)*B(2) )
  //        | ty|   ft * (-tx * ( B(3)+ty+b(2)   - (1+ty**2)*B(1) )  ,
  //
  //   where  ft = c_light*qp*sqrt ( 1 + tx**2 + ty**2 ) .
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

  z_out = iif((fvec(0.f) < w), z_out, fz);

  fvec qp_in      = fqp;
  const fvec h    = (z_out - fz);

  //   cout<<h<<" h"<<endl;
  //   cout<<ftx<<" ftx"<<endl;
  //   cout<<fty<<" fty"<<endl;

  fvec hC = h * c_light;
  //std::cout << "fx " << fx << std::endl;
  fvec x0[5];
  x0[0] = fx;
  x0[1] = fy;
  x0[2] = ftx;
  x0[3] = fty;
  x0[4] = ft;

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

    F.Get(x[0], x[1], fz + a[step] * h, B);
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
    //     cout << "fx = " << fx;

    //std::cout << " x : x0[0] " << x0[0] << " k[0] " << k[0] << " k[5] " << k[5] << " k[10] " << k[10] << " k[15] "
    //        << k[15] << std::endl;

    fx  = x0[0] + c[0] * k[0] + c[1] * k[5 + 0] + c[2] * k[10 + 0] + c[3] * k[15 + 0];
    fy  = x0[1] + c[0] * k[1] + c[1] * k[5 + 1] + c[2] * k[10 + 1] + c[3] * k[15 + 1];
    ftx = x0[2] + c[0] * k[2] + c[1] * k[5 + 2] + c[2] * k[10 + 2] + c[3] * k[15 + 2];
    fty = x0[3] + c[0] * k[3] + c[1] * k[5 + 3] + c[2] * k[10 + 3] + c[3] * k[15 + 3];
    ft  = x0[4] + c[0] * k[4] + c[1] * k[5 + 4] + c[2] * k[10 + 4] + c[3] * k[15 + 4];
    fz  = z_out;

    //     cout << "; fx = " << fx << endl;
  }
  //   cout<<fx<<" fx"<<endl;

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
    fx += J[6 * 4 + 0] * dqp;
    fy += J[6 * 4 + 1] * dqp;
    ftx += J[6 * 4 + 2] * dqp;
    fty += J[6 * 4 + 3] * dqp;
    ft += J[6 * 4 + 5] * dqp;
  }
  //    cout<<fx<<" fx"<<endl;
  //          covariance matrix transport

  //cout<< (ft - ft_old)<<" ft dt "<<endl;

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

  const fvec c42 = C42, c43 = C43;

  const fvec cj00 = C00 + C20 * J[6 * 2 + 0] + C30 * J[6 * 3 + 0] + C40 * J[6 * 4 + 0];
  const fvec cj10 = C10 + C21 * J[6 * 2 + 0] + C31 * J[6 * 3 + 0] + C41 * J[6 * 4 + 0];
  const fvec cj20 = C20 + C22 * J[6 * 2 + 0] + C32 * J[6 * 3 + 0] + c42 * J[6 * 4 + 0];
  const fvec cj30 = C30 + C32 * J[6 * 2 + 0] + C33 * J[6 * 3 + 0] + c43 * J[6 * 4 + 0];
  const fvec cj40 = C40 + C42 * J[6 * 2 + 0] + C43 * J[6 * 3 + 0] + C44 * J[6 * 4 + 0];
  const fvec cj50 = C50 + C52 * J[6 * 2 + 0] + C53 * J[6 * 3 + 0] + C54 * J[6 * 4 + 0];

  //  const fvec cj01 = C10 + C20*J[6*2 + 1] + C30*J[6*3 + 1] + C40*J[6*4 + 1];
  const fvec cj11 = C11 + C21 * J[6 * 2 + 1] + C31 * J[6 * 3 + 1] + C41 * J[6 * 4 + 1];
  const fvec cj21 = C21 + C22 * J[6 * 2 + 1] + C32 * J[6 * 3 + 1] + c42 * J[6 * 4 + 1];
  const fvec cj31 = C31 + C32 * J[6 * 2 + 1] + C33 * J[6 * 3 + 1] + c43 * J[6 * 4 + 1];
  const fvec cj41 = C41 + C42 * J[6 * 2 + 1] + C43 * J[6 * 3 + 1] + C44 * J[6 * 4 + 1];
  const fvec cj51 = C51 + C52 * J[6 * 2 + 1] + C53 * J[6 * 3 + 1] + C54 * J[6 * 4 + 1];

  // const fvec cj02 = C20*J[6*2 + 2] + C30*J[6*3 + 2] + C40*J[6*4 + 2];
  // const fvec cj12 = C21*J[6*2 + 2] + C31*J[6*3 + 2] + C41*J[6*4 + 2];
  const fvec cj22 = C22 * J[6 * 2 + 2] + C32 * J[6 * 3 + 2] + c42 * J[6 * 4 + 2];
  const fvec cj32 = C32 * J[6 * 2 + 2] + C33 * J[6 * 3 + 2] + c43 * J[6 * 4 + 2];
  const fvec cj42 = C42 * J[6 * 2 + 2] + C43 * J[6 * 3 + 2] + C44 * J[6 * 4 + 2];
  const fvec cj52 = C52 * J[6 * 2 + 2] + C53 * J[6 * 3 + 2] + C54 * J[6 * 4 + 2];

  // const fvec cj03 = C20*J[6*2 + 3] + C30*J[6*3 + 3] + C40*J[6*4 + 3];
  // const fvec cj13 = C21*J[6*2 + 3] + C31*J[6*3 + 3] + C41*J[6*4 + 3];
  const fvec cj23 = C22 * J[6 * 2 + 3] + C32 * J[6 * 3 + 3] + c42 * J[6 * 4 + 3];
  const fvec cj33 = C32 * J[6 * 2 + 3] + C33 * J[6 * 3 + 3] + c43 * J[6 * 4 + 3];
  const fvec cj43 = C42 * J[6 * 2 + 3] + C43 * J[6 * 3 + 3] + C44 * J[6 * 4 + 3];
  const fvec cj53 = C52 * J[6 * 2 + 3] + C53 * J[6 * 3 + 3] + C54 * J[6 * 4 + 3];

  const fvec cj24 = C42;
  const fvec cj34 = C43;
  const fvec cj44 = C44;
  const fvec cj54 = C54;

  // const fvec cj05 = C50 + C20*J[17] + C30*J[23] + C40*J[29];
  // const fvec cj15 = C51 + C21*J[17] + C31*J[23] + C41*J[29];
  const fvec cj25 = C52 + C22 * J[17] + C32 * J[23] + C42 * J[29];
  const fvec cj35 = C53 + C32 * J[17] + C33 * J[23] + C43 * J[29];
  const fvec cj45 = C54 + C42 * J[17] + C43 * J[23] + C44 * J[29];
  const fvec cj55 = C55 + C52 * J[17] + C53 * J[23] + C54 * J[29];


  C00 = cj00 + cj20 * J[12] + cj30 * J[18] + cj40 * J[24];

  C10 = cj10 + cj20 * J[13] + cj30 * J[19] + cj40 * J[25];
  C11 = cj11 + cj21 * J[13] + cj31 * J[19] + cj41 * J[25];

  C20 = cj20 + cj30 * J[20] + cj40 * J[26];
  C21 = cj21 + cj31 * J[20] + cj41 * J[26];
  C22 = cj22 + cj32 * J[20] + cj42 * J[26];

  C30 = cj30 + cj20 * J[15] + cj40 * J[27];
  C31 = cj31 + cj21 * J[15] + cj41 * J[27];
  C32 = cj32 + cj22 * J[15] + cj42 * J[27];
  C33 = cj33 + cj23 * J[15] + cj43 * J[27];

  C40 = cj40;
  C41 = cj41;
  C42 = cj42;
  C43 = cj43;
  C44 = cj44;

  C50 = cj50 + cj20 * J[17] + cj30 * J[23] + cj40 * J[29];
  C51 = cj51 + cj21 * J[17] + cj31 * J[23] + cj41 * J[29];
  C52 = cj52 + cj22 * J[17] + cj32 * J[23] + cj42 * J[29];
  C53 = cj53 + cj23 * J[17] + cj33 * J[23] + cj43 * J[29];
  C54 = cj54 + cj24 * J[17] + cj34 * J[23] + cj44 * J[29];
  C55 = cj55 + cj25 * J[17] + cj35 * J[23] + cj45 * J[29];
}

void L1TrackParFit::L1AddPipeMaterial(fvec qp0, fvec w)
{
  cnst ONE = 1.f;

  //  static const fscal RadThick=0.0009f;//0.5/18.76;
  //  static const fscal logRadThick=log(RadThick);
  //const fscal RadThick=0.0009f;//0.5/18.76;

  const fscal logRadThick = log(PipeRadThick[0]);
  fvec tx                 = ftx;
  fvec ty                 = fty;
  fvec txtx               = ftx * ftx;
  fvec tyty               = fty * fty;
  fvec txtx1              = txtx + ONE;
  fvec h                  = txtx + tyty;
  fvec t                  = sqrt(txtx1 + tyty);
  fvec h2                 = h * h;
  fvec qp0t               = qp0 * t;

  cnst c1 = 0.0136f, c2 = c1 * 0.038f, c3 = c2 * 0.5f, c4 = -c3 / 2.0f, c5 = c3 / 3.0f, c6 = -c3 / 4.0f;
  fvec s0 = (c1 + c2 * fvec(logRadThick) + c3 * h + h2 * (c4 + c5 * h + c6 * h2)) * qp0t;
  //fvec a = ( (ONE+mass2*qp0*qp0t)*RadThick*s0*s0 );
  fvec a = ((t + fMass2 * qp0 * qp0t) * PipeRadThick * s0 * s0);

  C22 += w * txtx1 * a;
  C32 += w * tx * ty * a;
  C33 += w * (ONE + tyty) * a;
}


void L1TrackParFit::L1AddMaterial(fvec radThick, fvec qp0, fvec w)
{
  cnst ONE = 1.;

  fvec tx    = ftx;
  fvec ty    = fty;
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

  C22 += w * txtx1 * a;
  C32 += w * tx * ty * a;
  C33 += w * (ONE + tyty) * a;
}

void L1TrackParFit::L1AddThickMaterial(fvec radThick, fvec qp0, fvec w, fvec thickness, bool fDownstream)
{
  cnst ONE = 1.;

  fvec tx    = ftx;
  fvec ty    = fty;
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

  C00 += w * txtx1 * a * T23;
  C10 += w * tx * ty * a * T23;
  C20 += w * txtx1 * a * D * T2;
  C30 += w * tx * ty * a * D * T2;

  C11 += w * (ONE + tyty) * a * T23;
  C21 += w * tx * ty * a * D * T2;
  C31 += w * (ONE + tyty) * a * D * T2;

  C22 += w * txtx1 * a;
  C32 += w * tx * ty * a;
  C33 += w * (ONE + tyty) * a;
}


void L1TrackParFit::L1AddMaterial(const L1MaterialInfo& info, fvec qp0, fvec w)
{
  cnst ONE = 1.f;

  fvec tx = ftx;
  fvec ty = fty;
  // fvec time = ft;
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

  C22 += w * txtx1 * a;
  C32 += w * tx * ty * a;
  C33 += w * (ONE + tyty) * a;
}


void L1TrackParFit::EnergyLossCorrection(const fvec& radThick, fvec& qp0, fvec direction, fvec w)
{
  const fvec qp2cut(1. / (10. * 10.));  // 10 GeV cut
  const fvec qp02 = max(qp0 * qp0, qp2cut);
  const fvec p2   = fvec(1.) / qp02;
  const fvec E2   = fMass2 + p2;

  const fvec bethe = L1Fit::ApproximateBetheBloch(p2 / fMass2);

  fvec tr = sqrt(fvec(1.f) + ftx * ftx + fty * fty);

  const fvec dE = bethe * radThick * tr * 2.33f * 9.34961f;

  const fvec E2Corrected = (sqrt(E2) + direction * dE) * (sqrt(E2) + direction * dE);
  fvec corr              = sqrt(p2 / (E2Corrected - fMass2));
  fmask ok               = (corr == corr) & (fvec::Zero() < w);
  corr                   = iif(ok, corr, fvec::One());

  qp0 *= corr;
  fqp *= corr;
  C40 *= corr;
  C41 *= corr;
  C42 *= corr;
  C43 *= corr;
  C44 *= corr * corr;
  C54 *= corr;
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

  fvec tr = sqrt(fvec(1.f) + ftx * ftx + fty * fty);

  fvec dE = bethe * radThick * tr * radLen * rho;

  const fvec E2Corrected = (sqrt(E2) + direction * dE) * (sqrt(E2) + direction * dE);
  fvec corr              = sqrt(p2 / (E2Corrected - fMass2));
  fmask ok               = (corr == corr) & (fvec::Zero() < w);
  corr                   = iif(ok, corr, fvec::One());

  qp0 *= corr;
  fqp *= corr;

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

  //   T.C40 *= corr;
  //   T.C41 *= corr;
  //   T.C42 *= corr;
  //   T.C43 *= corr;
  // T.C44 *= corr*corr;
  C44 += abs(SDEDX);
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
