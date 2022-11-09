/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

#include "L1Def.h"
#include "L1Fit.h"
#include "L1Material.h"
#include "L1TrackPar.h"

//#define cnst static const fvec
#define cnst const fvec

//const fvec PipeRadThick   = 7.87e-3f;      // 0.7 mm Aluminium
//const fvec TargetRadThick = 3.73e-2f * 2;  // 250 mum Gold

fvec L1Fit::BetheBlochIron(const float qp)
{

  float K = 0.000307075;  // [GeV*cm^2/g]
  float z = (qp > 0.) ? 1 : -1.;
  float Z = 26;
  float A = 55.845;

  float M       = 0.10565f;
  float p       = fabs(1. / qp);  //GeV
  float E       = sqrt(M * M + p * p);
  float beta    = p / E;
  float betaSq  = beta * beta;
  float gamma   = E / M;
  float gammaSq = gamma * gamma;

  float I = 260 * 1e-9;  // GeV

  float me    = 0.000511;  // GeV
  float ratio = me / M;
  float Tmax  = (2 * me * betaSq * gammaSq) / (1 + 2 * gamma * ratio + ratio * ratio);

  // density correction
  float dc = 0.;
  if (p > 0.5) {  // for particles above 1 Gev
    float rho = 7.87;
    float hwp = 28.816 * sqrt(rho * Z / A) * 1e-9;  // GeV
    dc        = log(hwp / I) + log(beta * gamma) - 0.5;
  }

  return K * z * z * (Z / A) * (1. / betaSq) * (0.5 * log(2 * me * betaSq * gammaSq * Tmax / (I * I)) - betaSq - dc);
}

fvec L1Fit::BetheBlochCarbon(const float qp)
{

  constexpr float K = 0.000307075;  // GeV * g^-1 * cm^2
  float z           = (qp > 0.) ? 1 : -1.;
  constexpr float Z = 6;
  constexpr float A = 12.011;

  constexpr float M = 0.10565f;
  float p           = fabs(1. / qp);  //GeV
  float E           = sqrt(M * M + p * p);
  float beta        = p / E;
  float betaSq      = beta * beta;
  float gamma       = E / M;
  float gammaSq     = gamma * gamma;

  const float I = 16 * std::pow(6, 0.9) * 1e-9;  // GeV  mean excitation energy in eV

  constexpr float me    = 0.000511;  // GeV
  constexpr float ratio = me / M;
  float Tmax            = (2 * me * betaSq * gammaSq) / (1 + 2 * gamma * ratio + ratio * ratio);

  // density correction
  float dc = 0.;
  if (p > 0.5) {  // for particles above 1 Gev
    constexpr float rho = 2.265;
    const float hwp     = 28.816 * sqrt(rho * Z / A) * 1e-9;  // GeV
    dc                  = log(hwp / I) + log(beta * gamma) - 0.5;
  }

  return K * z * z * (Z / A) * (1. / betaSq) * (0.5 * log(2 * me * betaSq * gammaSq * Tmax / (I * I)) - betaSq - dc);
}

fvec L1Fit::BetheBlochAl(const float qp)
{

  constexpr float K = 0.000307075;  // GeV * g^-1 * cm^2
  float z           = (qp > 0.) ? 1 : -1.;
  constexpr float Z = 13;
  constexpr float A = 26.981;

  constexpr float M = 0.10565f;
  float p           = fabs(1. / qp);  //GeV
  float E           = sqrt(M * M + p * p);
  float beta        = p / E;
  float betaSq      = beta * beta;
  float gamma       = E / M;
  float gammaSq     = gamma * gamma;

  const float I = 16 * std::pow(6, 0.9) * 1e-9;  // GeV  mean excitation energy in eV

  constexpr float me    = 0.000511;  // GeV
  constexpr float ratio = me / M;
  float Tmax            = (2 * me * betaSq * gammaSq) / (1 + 2 * gamma * ratio + ratio * ratio);

  // density correction
  float dc = 0.;
  if (p > 0.5) {  // for particles above 1 Gev
    constexpr float rho = 2.70;
    const float hwp     = 28.816 * sqrt(rho * Z / A) * 1e-9;  // GeV
    dc                  = log(hwp / I) + log(beta * gamma) - 0.5;
  }

  return K * z * z * (Z / A) * (1. / betaSq) * (0.5 * log(2 * me * betaSq * gammaSq * Tmax / (I * I)) - betaSq - dc);
}

fvec L1Fit::ApproximateBetheBloch(const fvec& bg2)
{
  //
  // This is the parameterization of the Bethe-Bloch formula inspired by Geant.
  //
  // bg2  - (beta*gamma)^2
  // kp0 - density [g/cm^3]
  // kp1 - density effect first junction point
  // kp2 - density effect second junction point
  // kp3 - mean excitation energy [GeV]
  // kp4 - mean Z/A
  //
  // The default values for the kp* parameters are for silicon.
  // The returned value is in [GeV/(g/cm^2)].
  //

  const fvec kp0 = 2.33f;
  const fvec kp1 = 0.20f;
  const fvec kp2 = 3.00f;
  const fvec kp3 = 173e-9f;
  const fvec kp4 = 0.49848f;

  constexpr float mK   = 0.307075e-3f;  // [GeV*cm^2/g]
  constexpr float _2me = 1.022e-3f;     // [GeV/c^2]
  const fvec rho       = kp0;
  const fvec x0        = kp1 * 2.303f;
  const fvec x1        = kp2 * 2.303f;
  const fvec mI        = kp3;
  const fvec mZA       = kp4;
  const fvec maxT      = _2me * bg2;  // neglecting the electron mass

  //*** Density effect
  fvec d2(0.f);
  const fvec x    = 0.5f * log(bg2);
  const fvec lhwI = log(28.816f * 1e-9f * sqrt(rho * mZA) / mI);

  fmask init   = x > x1;
  d2           = iif(init, lhwI + x - 0.5f, fvec::Zero());
  const fvec r = (x1 - x) / (x1 - x0);
  init         = (x > x0) & (x1 > x);
  d2           = iif(init, lhwI + x - 0.5f + (0.5f - lhwI - x0) * r * r * r, d2);

  return mK * mZA * (fvec(1.f) + bg2) / bg2
         * (0.5f * log(_2me * bg2 * maxT / (mI * mI)) - bg2 / (fvec(1.f) + bg2) - d2);
}

fvec L1Fit::ApproximateBetheBloch(const fvec& bg2, const fvec& kp0, const fvec& kp1, const fvec& kp2, const fvec& kp3,
                                  const fvec& kp4)
{
  //
  // This is the parameterization of the Bethe-Bloch formula inspired by Geant.
  //
  // bg2  - (beta*gamma)^2
  // kp0 - density [g/cm^3]
  // kp1 - density effect first junction point
  // kp2 - density effect second junction point
  // kp3 - mean excitation energy [GeV]
  // kp4 - mean Z/A
  //
  // The default values for the kp* parameters are for silicon.
  // The returned value is in [GeV/(g/cm^2)].
  //

  //   const fvec &kp0 = 2.33f;
  //   const fvec &kp1 = 0.20f;
  //   const fvec &kp2 = 3.00f;
  //   const fvec &kp3 = 173e-9f;
  //   const fvec &kp4 = 0.49848f;

  constexpr float mK   = 0.307075e-3f;  // [GeV*cm^2/g]
  constexpr float _2me = 1.022e-3f;     // [GeV/c^2]
  const fvec& rho      = kp0;
  const fvec x0        = kp1 * 2.303f;
  const fvec x1        = kp2 * 2.303f;
  const fvec& mI       = kp3;
  const fvec& mZA      = kp4;
  const fvec maxT      = _2me * bg2;  // neglecting the electron mass

  //*** Density effect
  fvec d2(0.f);
  const fvec x    = 0.5f * log(bg2);
  const fvec lhwI = log(28.816f * 1e-9f * sqrt(rho * mZA) / mI);

  fmask init   = x > x1;
  d2           = iif(init, lhwI + x - 0.5f, fvec::Zero());
  const fvec r = (x1 - x) / (x1 - x0);
  init         = (x > x0) & (x1 > x);
  d2           = iif(init, lhwI + x - 0.5f + (0.5f - lhwI - x0) * r * r * r, d2);

  return mK * mZA * (fvec(1.f) + bg2) / bg2
         * (0.5f * log(_2me * bg2 * maxT / (mI * mI)) - bg2 / (fvec(1.f) + bg2) - d2);
}


void L1Fit::EnergyLossCorrection(L1TrackPar& T, const fvec& radThick, fvec& qp0, fvec direction, fvec w)
{
  const fvec p2 = 1.f / (T.qp * T.qp);
  const fvec E2 = fMass2 + p2;

  // fvec qp =T.qp;

  const fvec bethe = ApproximateBetheBloch(p2 / fMass2);

  // fvec bethe2 = BetheBlochAl(qp[0]);


  fvec tr = sqrt(fvec(1.f) + T.tx * T.tx + T.ty * T.ty);

  fvec dE = bethe * radThick * tr * 9.34961f * 2.33f;
  // fvec dE2 = bethe2 * radThick*tr*2.265f* 2.70f;

  // dE = fabs(dE_);//dE2;//bethe * radThick*tr * 9.34961f*2.33f ;


  const fvec E2Corrected = (sqrt(E2) + direction * dE) * (sqrt(E2) + direction * dE);
  fvec corr              = sqrt(p2 / (E2Corrected - fMass2));
  fmask ok               = (corr == corr) & (fvec::Zero() < w);
  corr                   = iif(ok, corr, fvec::One());

  qp0 *= corr;
  //      fvec dqp = CalcQpAfterEloss(qp[0], (direction*dE)[0], fMass2[0]);
  //      qp0   =    dqp;
  //      T.qp  =    dqp;
  T.qp *= corr;
  T.C40 *= corr;
  T.C41 *= corr;
  T.C42 *= corr;
  T.C43 *= corr;
  T.C44 *= corr * corr;
}

template<int atomicZ>
void L1Fit::EnergyLossCorrection(float atomicA, float rho, float radLen, L1TrackPar& T, const fvec& radThick, fvec& qp0,
                                 fvec direction, fvec w)
{
  const fvec p2 = 1.f / (T.qp * T.qp);
  const fvec E2 = fMass2 + p2;

  fvec qp = T.qp;

  fvec i;
  if (atomicZ < 13) i = (12. * atomicZ + 7.) * 1.e-9;
  else
    i = (9.76 * atomicZ + 58.8 * std::pow(atomicZ, -0.19)) * 1.e-9;

  const fvec bethe = ApproximateBetheBloch(p2 / fMass2, rho, 0.20, 3.00, i, atomicZ / atomicA);

  // const fvec bethe = ApproximateBetheBloch( p2/fMass2 );

  // fvec bethe2 = BetheBlochAl(qp[0]);

  fvec tr = sqrt(fvec(1.f) + T.tx * T.tx + T.ty * T.ty);

  // fvec dE2 = 2*bethe * radThick*tr * 9.34961f*2.33f ;
  fvec dE = bethe * radThick * tr * radLen * rho;

  const fvec E2Corrected = (sqrt(E2) + direction * dE) * (sqrt(E2) + direction * dE);
  fvec corr              = sqrt(p2 / (E2Corrected - fMass2));
  fmask ok               = (corr == corr) & (fvec::Zero() < w);
  corr                   = iif(ok, corr, fvec::One());

  qp0 *= corr;
  //      fvec dqp = CalcQpAfterEloss(qp[0], (direction*dE)[0], fMass2[0]);
  //      qp0   =    dqp;
  //      T.qp  =    dqp;
  T.qp *= corr;

  fvec P = fvec(fabs(1. / qp[0]));  // GeV

  fvec Z(atomicZ);
  fvec A(atomicA);
  fvec RHO(rho);

  fvec STEP = radThick * tr * radLen;
  fvec EMASS(0.511 * 1e-3);  // GeV

  fvec BETA  = P / sqrt(E2);
  fvec GAMMA = sqrt(E2) / fMass;

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
  fvec SDEDX = ((E2) *DEDX2) / (P2 * P2 * P2);


  //   T.C40 *= corr;
  //   T.C41 *= corr;
  //   T.C42 *= corr;
  //   T.C43 *= corr;
  //  T.C44 *= corr*corr;
  T.C44 += abs(SDEDX);

  //  T.C40 *= corr;
  //  T.C41 *= corr;
  //  T.C42 *= corr;
  //  T.C43 *= corr;
  //  T.C44 *= corr*corr;
}

void L1Fit::EnergyLossCorrectionAl(L1TrackPar& T, const fvec& radThick, fvec& qp0, fvec direction, fvec w)
{
  constexpr int atomicZ   = 13;
  constexpr float atomicA = 26.981f;
  constexpr float rho     = 2.70f;
  constexpr float radLen  = 2.265f;
  EnergyLossCorrection<atomicZ>(atomicA, rho, radLen, T, radThick, qp0, direction, w);
}

void L1Fit::EnergyLossCorrectionCarbon(L1TrackPar& T, const fvec& radThick, fvec& qp0, fvec direction, fvec w)
{
  constexpr int atomicZ   = 6;
  constexpr float atomicA = 12.011f;
  constexpr float rho     = 2.265;
  constexpr float radLen  = 18.76f;
  EnergyLossCorrection<atomicZ>(atomicA, rho, radLen, T, radThick, qp0, direction, w);
}

void L1Fit::EnergyLossCorrectionIron(L1TrackPar& T, const fvec& radThick, fvec& qp0, fvec direction, fvec w)
{
  constexpr int atomicZ   = 26;
  constexpr float atomicA = 55.845f;
  constexpr float rho     = 7.87;
  constexpr float radLen  = 1.758f;
  EnergyLossCorrection<atomicZ>(atomicA, rho, radLen, T, radThick, qp0, direction, w);
}


void L1Fit::L1AddMaterial(L1TrackPar& T, fvec radThick, fvec qp0, fvec w)
{
  cnst ONE = 1.;

  fvec tx    = T.tx;
  fvec ty    = T.ty;
  fvec txtx  = tx * tx;
  fvec tyty  = ty * ty;
  fvec txtx1 = txtx + ONE;
  fvec h     = txtx + tyty;
  fvec t     = sqrt(txtx1 + tyty);
  fvec h2    = h * h;
  fvec qp0t  = qp0 * t;

  cnst c1 = 0.0136f, c2 = c1 * 0.038f, c3 = c2 * 0.5f, c4 = -c3 / 2.0f, c5 = c3 / 3.0f, c6 = -c3 / 4.0f;

  fvec s0 = (c1 + c2 * log(radThick) + c3 * h + h2 * (c4 + c5 * h + c6 * h2)) * qp0t;
  //fvec a = ( (ONE+fMass2*qp0*qp0t)*radThick*s0*s0 );
  fvec a = ((t + fMass2 * qp0 * qp0t) * radThick * s0 * s0);

  T.C22 += w * txtx1 * a;
  T.C32 += w * tx * ty * a;
  T.C33 += w * (ONE + tyty) * a;
}


// inline void L1Fit::L1AddThickMaterial( L1TrackPar &T, fvec radThick, fvec qp0, fvec thickness=0, fvec w = 1, fvec mass2 = 0.10565f*0.10565f, bool fDownstream = 1 )
// {
//   cnst ONE = 1.;
//
//   fvec tx = T.tx;
//   fvec ty = T.ty;
//   fvec txtx = tx*tx;
//   fvec tyty = ty*ty;
//   fvec txtx1 = txtx + ONE;
//   fvec h = txtx + tyty;
//   fvec t = sqrt(txtx1 + tyty);
//   fvec h2 = h*h;
//   fvec qp0t = qp0*t;
//
//   cnst c1=0.0136f, c2=c1*0.038f, c3=c2*0.5f, c4=-c3/2.0f, c5=c3/3.0f, c6=-c3/4.0f;
//
//   fvec s0 = (c1+c2*log(radThick) + c3*h + h2*(c4 + c5*h +c6*h2) )*qp0t;
//   //fvec a = ( (ONE+mass2*qp0*qp0t)*radThick*s0*s0 );
//   fvec a = ( (t+mass2*qp0*qp0t)*radThick*s0*s0 );
//
//   fvec D = (fDownstream) ? 1. : -1.;
//   fvec T23 = (thickness * thickness) / 3.0;
//   fvec T2 = thickness / 2.0;
//
//
//
//
//   T.C00 += w*txtx1*a * T23;
//   T.C10 += w*tx*ty*a * T23;
//   T.C20 += w*txtx1*a * D * T2;
//   T.C30 += w*tx*ty*a * D * T2;
//
//   T.C11 += w*(ONE+tyty)*a * T23;
//   T.C21 += w*tx*ty*a * D * T2;
//   T.C31 += w*(ONE+tyty)*a * D * T2;
//
//   T.C22 += w*txtx1*a;
//   T.C32 += w*tx*ty*a;
//   T.C33 += w*(ONE+tyty)*a;
//
// }

void L1Fit::L1AddThickMaterial(L1TrackPar& T, fvec radThick, fvec qp0, fvec w, fvec thickness, bool fDownstream)
{
  cnst ONE = 1.;

  fvec tx    = T.tx;
  fvec ty    = T.ty;
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
  //fvec a = ( (ONE+fMass2*qp0*qp0t)*radThick*s0*s0 );
  fvec a = ((t + fMass2 * qp0 * qp0t) * radThick * s0 * s0);

  fvec D   = (fDownstream) ? fvec(1.) : fvec(-1.);
  fvec T23 = (thickness * thickness) / fvec(3.0);
  fvec T2  = thickness / fvec(2.0);


  T.C00 += w * txtx1 * a * T23;
  T.C10 += w * tx * ty * a * T23;
  T.C20 += w * txtx1 * a * D * T2;
  T.C30 += w * tx * ty * a * D * T2;

  T.C11 += w * (ONE + tyty) * a * T23;
  T.C21 += w * tx * ty * a * D * T2;
  T.C31 += w * (ONE + tyty) * a * D * T2;

  T.C22 += w * txtx1 * a;
  T.C32 += w * tx * ty * a;
  T.C33 += w * (ONE + tyty) * a;
}


void L1Fit::L1AddPipeMaterial(L1TrackPar& T, fvec qp0, fvec w)
{
  cnst ONE = 1.f;

  //  static const fscal RadThick=0.0009f;//0.5/18.76;
  //  static const fscal logRadThick=log(RadThick);
  //const fscal RadThick=0.0009f;//0.5/18.76;

  const fscal logRadThick = log(PipeRadThick[0]);
  fvec tx                 = T.tx;
  fvec ty                 = T.ty;
  fvec txtx               = tx * tx;
  fvec tyty               = ty * ty;
  fvec txtx1              = txtx + ONE;
  fvec h                  = txtx + tyty;
  fvec t                  = sqrt(txtx1 + tyty);
  fvec h2                 = h * h;
  fvec qp0t               = qp0 * t;

  cnst c1 = 0.0136f, c2 = c1 * 0.038f, c3 = c2 * 0.5f, c4 = -c3 / 2.0f, c5 = c3 / 3.0f, c6 = -c3 / 4.0f;
  fvec s0 = (c1 + c2 * fvec(logRadThick) + c3 * h + h2 * (c4 + c5 * h + c6 * h2)) * qp0t;
  //fvec a = ( (ONE+fMass2*qp0*qp0t)*RadThick*s0*s0 );
  fvec a = ((t + fMass2 * qp0 * qp0t) * PipeRadThick * s0 * s0);

  T.C22 += w * txtx1 * a;
  T.C32 += w * tx * ty * a;
  T.C33 += w * (ONE + tyty) * a;
}

void L1Fit::L1AddTargetMaterial(L1TrackPar& T, fvec qp0, fvec w)
{
  cnst ONE = 1.f;

  const fscal logRadThick = log(TargetRadThick[0]);
  fvec tx                 = T.tx;
  fvec ty                 = T.ty;
  fvec txtx               = tx * tx;
  fvec tyty               = ty * ty;
  fvec txtx1              = txtx + ONE;
  fvec h                  = txtx + tyty;
  fvec t                  = sqrt(txtx1 + tyty);
  fvec h2                 = h * h;
  fvec qp0t               = qp0 * t;

  cnst c1 = 0.0136f, c2 = c1 * 0.038f, c3 = c2 * 0.5f, c4 = -c3 / 2.0f, c5 = c3 / 3.0f, c6 = -c3 / 4.0f;
  fvec s0 = (c1 + c2 * fvec(logRadThick) + c3 * h + h2 * (c4 + c5 * h + c6 * h2)) * qp0t;
  //fvec a = ( (ONE+fMass2*qp0*qp0t)*RadThick*s0*s0 );
  fvec a = ((t + fMass2 * qp0 * qp0t) * TargetRadThick * s0 * s0);

  T.C22 += w * txtx1 * a;
  T.C32 += w * tx * ty * a;
  T.C33 += w * (ONE + tyty) * a;
}

#undef cnst
