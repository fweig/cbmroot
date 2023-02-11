/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer], Maksym Zyzak */


/// \file L1Fit.h
/// \brief Kalman filter fitter for L1 tracking
/// \since 10.02.2023
/// \author S.Gorbunov

#ifndef L1Fit_h
#define L1Fit_h

#include "L1Def.h"
#include "L1Field.h"
#include "L1TrackPar.h"
#include "L1UMeasurementInfo.h"
#include "L1XYMeasurementInfo.h"

#define cnst const fvec

/// Kalman filter fitter for L1 tracking
///
class L1Fit {

public:
  L1Fit() = default;

  L1Fit(const L1TrackPar& t) { SetTrack(t); }

  template<typename T>
  L1Fit(const T* tr, const T* C)
  {
    SetTrack(tr, C);
  }

  void SetMask(const fmask& m)
  {
    fMask  = m;
    fMaskF = iif(m, fvec::One(), fvec::Zero());
  }

  template<typename T>
  void SetTrack(const T* tr, const T* C)
  {
    fTr.Set(tr, C);
    fQp0 = fTr.qp;
  }

  void SetTrack(const L1TrackPar& t)
  {
    fTr  = t;
    fQp0 = fTr.qp;
  }

  void SetQp0(cnst& qp0) { fQp0 = qp0; }

  L1TrackPar& Tr() { return fTr; }

  fvec& Qp0() { return fQp0; }

  void SetOneEntry(const int i0, const L1Fit& T1, const int i1);

  void Print(int i = -1);

  //Fit functionality

  /// get target radiation thickness
  fvec GetTargetRadThick() const { return fTargetRadThick; }

  /// set particle mass for the fit
  void SetParticleMass(fvec mass)
  {
    fMass  = mass;
    fMass2 = mass * mass;
  }

  /// get the particle mass
  fvec GetParticleMass() const { return fMass; }

  /// get the particle mass squared
  fvec GetParticleMass2() const { return fMass2; }

  void Filter(const L1UMeasurementInfo& info, cnst& u, cnst& sigma2);
  void FilterXY(const L1XYMeasurementInfo& info, cnst& x, cnst& y);
  void FilterTime(cnst& t, cnst& dt2, cnst& timeInfo);
  void FilterExtrapolatedXY(cnst& x, cnst& y, const L1XYMeasurementInfo& info, cnst& extrX, cnst& extrY, cnst Jx[6],
                            cnst Jy[6]);

  void Extrapolate(cnst& z_out, const L1FieldRegion& F);

  void ExtrapolateStep(cnst& z_out, const L1FieldRegion& F);
  void ExtrapolateStepAnalytic(cnst& z_out, const L1FieldRegion& F);
  void ExtrapolateLine(cnst& z_out);
  void ExtrapolateLine(cnst& z_out, const L1FieldRegion& F);

  void EnergyLossCorrection(cnst& radThick, cnst& upstreamDirection);

  template<int atomicZ>
  void EnergyLossCorrection(float atomicA, float rho, float radLen, cnst& radThick, cnst& direction);

  void EnergyLossCorrectionIron(cnst& radThick, cnst& direction);
  void EnergyLossCorrectionCarbon(cnst& radThick, cnst& direction);
  void EnergyLossCorrectionAl(cnst& radThick, cnst& direction);

  void AddMsInMaterial(cnst& radThick);

  void AddMsInThickMaterial(cnst& radThick, cnst& thickness, bool fDownstream);

  void GetExtrapolatedXYline(cnst& z, const L1FieldRegion& F, fvec& extrX, fvec& extrY, fvec Jx[6], fvec Jy[6]) const;

  void ExtrapolateXC00Line(cnst& z_out, fvec& x, fvec& C00) const;
  void ExtrapolateYC11Line(cnst& z_out, fvec& y, fvec& C11) const;
  void ExtrapolateC10Line(cnst& z_out, fvec& C10) const;


  void AddTargetToLine(cnst& targX, cnst& targY, cnst& targZ, const L1XYMeasurementInfo& targXYInfo,
                       const L1FieldRegion& F);

  static fvec ApproximateBetheBloch(cnst& bg2);

  static fvec ApproximateBetheBloch(cnst& bg2, cnst& kp0, cnst& kp1, cnst& kp2, cnst& kp3, cnst& kp4);

  static void FilterChi2XYC00C10C11(const L1UMeasurementInfo& info, fvec& x, fvec& y, fvec& C00, fvec& C10, fvec& C11,
                                    fvec& chi2, cnst& u, cnst& du2);

  static void FilterChi2(const L1UMeasurementInfo& info, cnst& x, cnst& y, cnst& C00, cnst& C10, cnst& C11, fvec& chi2,
                         cnst& u, cnst& du2);

private:
  fmask fMask {fmask::One()};  // mask of active elements in simd vectors
  fvec fMaskF {fvec::One()};   // floating point representation of fMask

  L1TrackPar fTr {};
  fvec fQp0 {0.};

  fvec fMass {0.10565800};      // muon mass
  fvec fMass2 {fMass * fMass};  // mass squared

  fvec fPipeRadThick {7.87e-3f};        // 0.7 mm Aluminium  // TODO:
  fvec fTargetRadThick {3.73e-2f * 2};  // 250 mum Gold      // TODO:
} _fvecalignment;

// =============================================================================================

inline void L1Fit::Print(int i) { fTr.Print(i); }


inline void L1Fit::SetOneEntry(const int i0, const L1Fit& T1, const int i1)
{
  fTr.SetOneEntry(i0, T1.fTr, i1);
  fQp0[i0] = T1.fQp0[i1];
}

inline void L1Fit::ExtrapolateXC00Line(cnst& z_out, fvec& x, fvec& C00) const
{
  cnst dz = (z_out - fTr.z);
  x       = fTr.x + fTr.tx * dz;
  C00     = fTr.C00 + dz * (2 * fTr.C20 + dz * fTr.C22);
}

inline void L1Fit::ExtrapolateYC11Line(cnst& z_out, fvec& y, fvec& C11) const
{
  cnst dz = (z_out - fTr.z);
  y       = fTr.y + fTr.ty * dz;
  C11     = fTr.C11 + dz * (2 * fTr.C31 + dz * fTr.C33);
}

inline void L1Fit::ExtrapolateC10Line(cnst& z_out, fvec& C10) const
{
  cnst dz = (z_out - fTr.z);
  C10     = fTr.C10 + dz * (fTr.C21 + fTr.C30 + dz * fTr.C32);
}

#undef cnst


#endif
