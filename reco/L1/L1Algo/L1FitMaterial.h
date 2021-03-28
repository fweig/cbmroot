#ifndef L1FitMaterial_h
#define L1FitMaterial_h

#include "CbmL1Def.h"

#include "L1MaterialInfo.h"
#include "L1TrackPar.h"

//#define cnst static const fvec
#define cnst const fvec

//const fvec PipeRadThick   = 7.87e-3f;      // 0.7 mm Aluminium
//const fvec TargetRadThick = 3.73e-2f * 2;  // 250 mum Gold


inline fvec L1Fit::BetheBlochIron(const float qp)
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

inline fvec L1Fit::BetheBlochCarbon(const float qp)
{

  float K = 0.000307075;  // GeV * g^-1 * cm^2
  float z = (qp > 0.) ? 1 : -1.;
  float Z = 6;
  float A = 12.011;

  float M       = 0.10565f;
  float p       = fabs(1. / qp);  //GeV
  float E       = sqrt(M * M + p * p);
  float beta    = p / E;
  float betaSq  = beta * beta;
  float gamma   = E / M;
  float gammaSq = gamma * gamma;

  float I = 16 * std::pow(6, 0.9) * 1e-9;  // GeV  mean excitation energy in eV

  float me    = 0.000511;  // GeV
  float ratio = me / M;
  float Tmax  = (2 * me * betaSq * gammaSq) / (1 + 2 * gamma * ratio + ratio * ratio);

  // density correction
  float dc = 0.;
  if (p > 0.5) {  // for particles above 1 Gev
    float rho = 2.265;
    float hwp = 28.816 * sqrt(rho * Z / A) * 1e-9;  // GeV
    dc        = log(hwp / I) + log(beta * gamma) - 0.5;
  }

  return K * z * z * (Z / A) * (1. / betaSq) * (0.5 * log(2 * me * betaSq * gammaSq * Tmax / (I * I)) - betaSq - dc);
}

inline fvec L1Fit::BetheBlochAl(const float qp)
{

  float K = 0.000307075;  // GeV * g^-1 * cm^2
  float z = (qp > 0.) ? 1 : -1.;
  float Z = 13;
  float A = 26.981;

  float M       = 0.10565f;
  float p       = fabs(1. / qp);  //GeV
  float E       = sqrt(M * M + p * p);
  float beta    = p / E;
  float betaSq  = beta * beta;
  float gamma   = E / M;
  float gammaSq = gamma * gamma;

  float I = 16 * std::pow(6, 0.9) * 1e-9;  // GeV  mean excitation energy in eV

  float me    = 0.000511;  // GeV
  float ratio = me / M;
  float Tmax  = (2 * me * betaSq * gammaSq) / (1 + 2 * gamma * ratio + ratio * ratio);

  // density correction
  float dc = 0.;
  if (p > 0.5) {  // for particles above 1 Gev
    float rho = 2.70;
    float hwp = 28.816 * sqrt(rho * Z / A) * 1e-9;  // GeV
    dc        = log(hwp / I) + log(beta * gamma) - 0.5;
  }

  return K * z * z * (Z / A) * (1. / betaSq) * (0.5 * log(2 * me * betaSq * gammaSq * Tmax / (I * I)) - betaSq - dc);
}

inline fvec L1Fit::ApproximateBetheBloch(const fvec& bg2)
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

  const fvec& kp0 = 2.33f;
  const fvec& kp1 = 0.20f;
  const fvec& kp2 = 3.00f;
  const fvec& kp3 = 173e-9f;
  const fvec& kp4 = 0.49848f;

  const float mK   = 0.307075e-3f;  // [GeV*cm^2/g]
  const float _2me = 1.022e-3f;     // [GeV/c^2]
  const fvec& rho  = kp0;
  const fvec& x0   = kp1 * 2.303f;
  const fvec& x1   = kp2 * 2.303f;
  const fvec& mI   = kp3;
  const fvec& mZA  = kp4;
  const fvec& maxT = _2me * bg2;  // neglecting the electron mass

  //*** Density effect
  fvec d2(0.f);
  const fvec x    = 0.5f * log(bg2);
  const fvec lhwI = log(28.816f * 1e-9f * sqrt(rho * mZA) / mI);

  fvec init = x > x1;

  d2            = fvec(init & (lhwI + x - 0.5f));
  const fvec& r = (x1 - x) / (x1 - x0);
  init          = (x > x0) & (x1 > x);
  d2            = fvec(init & (lhwI + x - 0.5f + (0.5f - lhwI - x0) * r * r * r)) + fvec((!init) & d2);

  return mK * mZA * (fvec(1.f) + bg2) / bg2
         * (0.5f * log(_2me * bg2 * maxT / (mI * mI)) - bg2 / (fvec(1.f) + bg2) - d2);
}

inline fvec L1Fit::ApproximateBetheBloch(const fvec& bg2, const fvec& kp0, const fvec& kp1, const fvec& kp2,
                                         const fvec& kp3, const fvec& kp4)
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

  const float mK   = 0.307075e-3f;  // [GeV*cm^2/g]
  const float _2me = 1.022e-3f;     // [GeV/c^2]
  const fvec& rho  = kp0;
  const fvec& x0   = kp1 * 2.303f;
  const fvec& x1   = kp2 * 2.303f;
  const fvec& mI   = kp3;
  const fvec& mZA  = kp4;
  const fvec& maxT = _2me * bg2;  // neglecting the electron mass

  //*** Density effect
  fvec d2(0.f);
  const fvec x    = 0.5f * log(bg2);
  const fvec lhwI = log(28.816f * 1e-9f * sqrt(rho * mZA) / mI);

  fvec init = x > x1;

  d2            = fvec(init & (lhwI + x - 0.5f));
  const fvec& r = (x1 - x) / (x1 - x0);
  init          = (x > x0) & (x1 > x);
  d2            = fvec(init & (lhwI + x - 0.5f + (0.5f - lhwI - x0) * r * r * r)) + fvec((!init) & d2);

  return mK * mZA * (fvec(1.f) + bg2) / bg2
         * (0.5f * log(_2me * bg2 * maxT / (mI * mI)) - bg2 / (fvec(1.f) + bg2) - d2);
}


inline float L1Fit::CalcQpAfterEloss(float qp, float eloss, float mass2)
{

  float p    = fabs(1. / qp);
  float E    = sqrt(p * p + mass2);
  float q    = (qp > 0) ? 1. : -1.;
  float Enew = E + eloss;
  // float pnew = (Enew > sqrt(fMass2)) ? std::sqrt(Enew * Enew - fMass2) : 0.;
  float pnew = std::sqrt(Enew * Enew - mass2);
  // if (pnew!=0)
  return q / pnew;
  // else { return 1e5; }
}

inline void L1Fit::EnergyLossCorrection(L1TrackPar& T, const fvec& radThick, fvec& qp0, fvec direction, fvec w)
{
  const fvec& p2 = 1.f / (T.qp * T.qp);
  const fvec& E2 = fMass2 + p2;

  // fvec qp =T.qp;

  const fvec& bethe = ApproximateBetheBloch(p2 / fMass2);

  // fvec bethe2 = BetheBlochAl(qp[0]);


  fvec tr = sqrt(fvec(1.f) + T.tx * T.tx + T.ty * T.ty);

  fvec dE = bethe * radThick * tr * 9.34961f * 2.33f;
  // fvec dE2 = bethe2 * radThick*tr*2.265f* 2.70f;

  // dE = fabs(dE_);//dE2;//bethe * radThick*tr * 9.34961f*2.33f ;


  const fvec& E2Corrected = (sqrt(E2) + direction * dE) * (sqrt(E2) + direction * dE);
  fvec corr               = sqrt(p2 / (E2Corrected - fMass2));
  fvec init               = fvec(!(corr == corr)) | fvec(w < 1);
  corr                    = fvec(fvec(1.f) & init) + fvec(corr & fvec(!(init)));


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

inline void L1Fit::EnergyLossCorrectionAl(L1TrackPar& T, const fvec& radThick, fvec& qp0, fvec direction, fvec w)
{
  const fvec& p2 = 1.f / (T.qp * T.qp);
  const fvec& E2 = fMass2 + p2;

  fvec qp = T.qp;

  int atomicZ   = 13;
  float atomicA = 26.981f;
  float rho     = 2.70f;
  float radLen  = 2.265f;

  //   int atomicZ = 18;
  //   float atomicA = 39.95;
  //   float rho = 0.001780;
  //   float radLen = 1.097e+04;


  fvec i;
  if (atomicZ < 13) i = (12. * atomicZ + 7.) * 1.e-9;
  else
    i = (9.76 * atomicZ + 58.8 * std::pow(atomicZ, -0.19)) * 1.e-9;

  const fvec& bethe = ApproximateBetheBloch(p2 / fMass2, rho, 0.20, 3.00, i, atomicZ / atomicA);

  // const fvec& bethe = ApproximateBetheBloch( p2/fMass2 );

  // fvec bethe2 = BetheBlochAl(qp[0]);


  fvec tr = sqrt(fvec(1.f) + T.tx * T.tx + T.ty * T.ty);

  // fvec dE2 = 2*bethe * radThick*tr * 9.34961f*2.33f ;
  fvec dE = bethe * radThick * tr * radLen * rho;


  const fvec& E2Corrected = (sqrt(E2) + direction * dE) * (sqrt(E2) + direction * dE);
  fvec corr               = sqrt(p2 / (E2Corrected - fMass2));
  fvec init               = fvec(!(corr == corr)) | fvec(w < 1);
  corr                    = fvec(fvec(1.f) & init) + fvec(corr & fvec(!(init)));

  qp0 *= corr;
  //      fvec dqp = CalcQpAfterEloss(qp[0], (direction*dE)[0], fMass2[0]);
  //      qp0   =    dqp;
  //      T.qp  =    dqp;
  T.qp *= corr;

  float P = fabs(1. / qp[0]);  // GeV

  float Z   = atomicZ;
  float A   = atomicA;
  float RHO = rho;

  fvec STEP  = radThick * tr * radLen;
  fvec EMASS = 0.511 * 1e-3;  // GeV

  fvec BETA  = P / sqrt(E2);
  fvec GAMMA = sqrt(E2) / sqrt(fMass2);

  // Calculate xi factor (KeV).
  fvec XI = (153.5 * Z * STEP * RHO) / (A * BETA * BETA);

  // Maximum energy transfer to atomic electron (KeV).
  fvec ETA   = BETA * GAMMA;
  fvec ETASQ = ETA * ETA;
  fvec RATIO = EMASS / sqrt(fMass2);
  fvec F1    = 2. * EMASS * ETASQ;
  fvec F2    = 1. + 2. * RATIO * GAMMA + RATIO * RATIO;
  fvec EMAX  = 1e6 * F1 / F2;

  fvec DEDX2 = XI * EMAX * (1. - (BETA * BETA / 2.)) * 1e-12;

  fvec SDEDX = ((E2) *DEDX2) / std::pow(P, 6);


  //   T.C40 *= corr;
  //   T.C41 *= corr;
  //   T.C42 *= corr;
  //   T.C43 *= corr;
  //  T.C44 *= corr*corr;
  T.C44 += fabs(SDEDX);

  //  T.C40 *= corr;
  //  T.C41 *= corr;
  //  T.C42 *= corr;
  //  T.C43 *= corr;
  //  T.C44 *= corr*corr;
}


inline void L1Fit::EnergyLossCorrectionCarbon(L1TrackPar& T, const fvec& radThick, fvec& qp0, fvec direction, fvec w)
{
  const fvec& p2 = 1.f / (T.qp * T.qp);
  const fvec& E2 = fMass2 + p2;

  fvec qp = T.qp;


  int atomicZ   = 6;
  float atomicA = 12.011f;
  float rho     = 2.265;
  float radLen  = 18.76f;

  fvec i;
  if (atomicZ < 13) i = (12. * atomicZ + 7.) * 1.e-9;
  else
    i = (9.76 * atomicZ + 58.8 * std::pow(atomicZ, -0.19)) * 1.e-9;

  const fvec& bethe = ApproximateBetheBloch(p2 / fMass2, rho, 0.20, 3.00, i, atomicZ / atomicA);


  // const fvec& bethe = ApproximateBetheBloch( p2/fMass2 );

  //  fvec bethe2 = BetheBlochCarbon(qp[0]);


  fvec tr = sqrt(fvec(1.f) + T.tx * T.tx + T.ty * T.ty);


  fvec dE = bethe * radThick * tr * rho * radLen;
  //   fvec dE2 = bethe2 * radThick*tr*2.70f* 18.76f;

  const fvec& E2Corrected = (sqrt(E2) + direction * dE) * (sqrt(E2) + direction * dE);
  fvec corr               = sqrt(p2 / (E2Corrected - fMass2));
  fvec init               = fvec(!(corr == corr)) | fvec(w < 1);
  corr                    = fvec(fvec(1.f) & init) + fvec(corr & fvec(!(init)));

  qp0 *= corr;
  T.qp *= corr;


  float P = fabs(1. / qp[0]);  // GeV

  float Z   = atomicZ;
  float A   = atomicA;
  float RHO = rho;

  fvec STEP  = radThick * tr * radLen;
  fvec EMASS = 0.511 * 1e-3;  // GeV

  fvec BETA  = P / sqrt(E2Corrected);
  fvec GAMMA = sqrt(E2) / sqrt(fMass2);

  // Calculate xi factor (KeV).
  fvec XI = (153.5 * Z * STEP * RHO) / (A * BETA * BETA);

  // Maximum energy transfer to atomic electron (KeV).
  fvec ETA   = BETA * GAMMA;
  fvec ETASQ = ETA * ETA;
  fvec RATIO = EMASS / sqrt(fMass2);
  fvec F1    = 2. * EMASS * ETASQ;
  fvec F2    = 1. + 2. * RATIO * GAMMA + RATIO * RATIO;
  fvec EMAX  = 1e6 * F1 / F2;

  fvec DEDX2 = XI * EMAX * (1. - (BETA * BETA / 2.)) * 1e-12;

  fvec SDEDX = ((E2) *DEDX2) / std::pow(P, 6);

  //    fvec dqp = CalcQpAfterEloss(qp[0], (direction*dE)[0], fMass2[0]);
  //      qp0   =    dqp;
  //      T.qp  =    dqp;


  //   T.C40 *= corr;
  //   T.C41 *= corr;
  //   T.C42 *= corr;
  //   T.C43 *= corr;
  // T.C44 *= corr*corr;
  T.C44 += fabs(SDEDX);

  //  T.C40 *= corr;
  //  T.C41 *= corr;
  //  T.C42 *= corr;
  //  T.C43 *= corr;
  //  T.C44 *= corr*corr;
}


inline void L1Fit::EnergyLossCorrectionIron(L1TrackPar& T, const fvec& radThick, fvec& qp0, fvec direction, fvec w)
{
  const fvec& p2 = 1.f / (T.qp * T.qp);
  const fvec& E2 = fMass2 + p2;

  fvec qp = T.qp;

  int atomicZ   = 26;
  float atomicA = 55.845f;
  float rho     = 7.87;
  float radLen  = 1.758f;

  fvec i;
  if (atomicZ < 13) i = (12. * atomicZ + 7.) * 1.e-9;
  else
    i = (9.76 * atomicZ + 58.8 * std::pow(atomicZ, -0.19)) * 1.e-9;

  const fvec& bethe = ApproximateBetheBloch(p2 / fMass2, rho, 0.20, 3.00, i, atomicZ / atomicA);

  // fvec bethe2 = BetheBlochIron(T.qp[0]);


  fvec tr = sqrt(fvec(1.f) + T.tx * T.tx + T.ty * T.ty);


  fvec dE = bethe * radThick * tr * radLen * rho;
  //fvec dE2 = bethe2 * radThick*tr * 1.758f*2.33;

  const fvec& E2Corrected = (sqrt(E2) + direction * dE) * (sqrt(E2) + direction * dE);
  fvec corr               = sqrt(p2 / (E2Corrected - fMass2));
  fvec init               = fvec(!(corr == corr)) | fvec(w < 1);
  corr                    = fvec(fvec(1.f) & init) + fvec(corr & fvec(!(init)));

  qp0 *= corr;
  T.qp *= corr;

  float P = fabs(1. / qp[0]);  // GeV

  float Z   = atomicZ;
  float A   = atomicA;
  float RHO = rho;

  fvec STEP  = radThick * tr * radLen;
  fvec EMASS = 0.511 * 1e-3;  // GeV

  fvec BETA  = P / sqrt(E2Corrected);
  fvec GAMMA = sqrt(E2) / sqrt(fMass2);

  // Calculate xi factor (KeV).
  fvec XI = (153.5 * Z * STEP * RHO) / (A * BETA * BETA);

  // Maximum energy transfer to atomic electron (KeV).
  fvec ETA   = BETA * GAMMA;
  fvec ETASQ = ETA * ETA;
  fvec RATIO = EMASS / sqrt(fMass2);
  fvec F1    = 2. * EMASS * ETASQ;
  fvec F2    = 1. + 2. * RATIO * GAMMA + RATIO * RATIO;
  fvec EMAX  = 1e6 * F1 / F2;

  fvec DEDX2 = XI * EMAX * (1. - (BETA * BETA / 2.)) * 1e-12;

  fvec SDEDX = ((E2) *DEDX2) / std::pow(P, 6);

  //   T.C40 *= corr;
  //   T.C41 *= corr;
  //   T.C42 *= corr;
  //   T.C43 *= corr;
  // T.C44 *= corr*corr;
  T.C44 += fabs(SDEDX);

  //  T.C40 *= corr;
  //  T.C41 *= corr;
  //  T.C42 *= corr;
  //  T.C43 *= corr;
  //  T.C44 *= corr*corr;
}


inline void L1Fit::L1AddMaterial(L1TrackPar& T, fvec radThick, fvec qp0, fvec w)
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

inline void L1Fit::L1AddMaterial(L1TrackPar& T, L1MaterialInfo& info, fvec qp0, fvec w)
{
  cnst ONE = 1.f;

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

  fvec s0 = (c1 + c2 * info.logRadThick + c3 * h + h2 * (c4 + c5 * h + c6 * h2)) * qp0t;
  //fvec a = ( (ONE+fMass2*qp0*qp0t)*info.RadThick*s0*s0 );
  fvec a = ((t + fMass2 * qp0 * qp0t) * info.RadThick * s0 * s0);

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

inline void L1Fit::L1AddThickMaterial(L1TrackPar& T, fvec radThick, fvec qp0, fvec w, fvec thickness, bool fDownstream)
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

  fvec D   = (fDownstream) ? 1. : -1.;
  fvec T23 = (thickness * thickness) / 3.0;
  fvec T2  = thickness / 2.0;


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


inline void L1Fit::L1AddHalfMaterial(L1TrackPar& T, L1MaterialInfo& info, fvec qp0)
{
  cnst ONE   = 1.f;
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

  fvec s0 = (c1 + c2 * (info.logRadThick + log(0.5)) + c3 * h + h2 * (c4 + c5 * h + c6 * h2)) * qp0t;
  //fvec a = ( (ONE+fMass2*qp0*qp0t)*info.RadThick*0.5*s0*s0 );
  fvec a = ((t + fMass2 * qp0 * qp0t) * info.RadThick * 0.5 * s0 * s0);

  T.C22 += txtx1 * a;
  T.C32 += tx * ty * a;
  T.C33 += (ONE + tyty) * a;
}

inline void L1Fit::L1AddPipeMaterial(L1TrackPar& T, fvec qp0, fvec w)
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

inline void L1Fit::L1AddTargetMaterial(L1TrackPar& T, fvec qp0, fvec w)
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

#endif
