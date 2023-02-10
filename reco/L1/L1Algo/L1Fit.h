/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak [committer], Sergey Gorbunov */

#ifndef L1Fit_h
#define L1Fit_h

#include "L1Def.h"
#include "L1Field.h"
#include "L1TrackPar.h"
#include "L1UMeasurementInfo.h"
#include "L1XYMeasurementInfo.h"

class L1Fit {

public:
  L1Fit() = default;

  L1Fit(const L1TrackPar& t) { SetTrack(t); }

  template<typename T>
  L1Fit(const T* tr, const T* C)
  {
    SetTrack(tr, C);
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

  void Filter(const L1UMeasurementInfo& info, const fvec& u, const fvec& sigma2, const fvec& w);
  void FilterXY(const L1XYMeasurementInfo& info, fvec x, fvec y);
  void FilterTime(fvec t, fvec dt2, fvec w, fvec timeInfo);
  void FilterExtrapolatedXY(const fvec& x, const fvec& y, const L1XYMeasurementInfo& info, const fvec& extrX,
                            const fvec& extrY, const fvec Jx[6], const fvec Jy[6]);

  void Extrapolate(fvec z_out, const L1FieldRegion& F, const fvec& w);

  void ExtrapolateStep(fvec z_out, const L1FieldRegion& F, const fvec& w);
  void ExtrapolateStepAnalytic(fvec z_out, const L1FieldRegion& F, const fvec& w);
  void ExtrapolateLine(fvec z_out, const fvec& w);
  void ExtrapolateLine(fvec z_out, const L1FieldRegion& F, const fvec& w);

  void EnergyLossCorrection(const fvec& radThick, fvec upstreamDirection, fvec w);

  template<int atomicZ>
  void EnergyLossCorrection(float atomicA, float rho, float radLen, const fvec& radThick, fvec direction, fvec w);

  void EnergyLossCorrectionIron(const fvec& radThick, fvec direction, fvec w);
  void EnergyLossCorrectionCarbon(const fvec& radThick, fvec direction, fvec w);
  void EnergyLossCorrectionAl(const fvec& radThick, fvec direction, fvec w);

  void AddMsInMaterial(const fvec& radThick, fvec w);

  void AddMsInThickMaterial(fvec radThick, fvec w, fvec thickness, bool fDownstream);

  void GetExtrapolatedXYline(const fvec& z, const L1FieldRegion& F, fvec& extrX, fvec& extrY, fvec Jx[6],
                             fvec Jy[6]) const;

  void ExtrapolateXC00Line(fvec z_out, fvec& x, fvec& C00) const;
  void ExtrapolateYC11Line(fvec z_out, fvec& y, fvec& C11) const;
  void ExtrapolateC10Line(fvec z_out, fvec& C10) const;


  void AddTargetToLine(const fvec& targX, const fvec& targY, const fvec& targZ, const L1XYMeasurementInfo& targXYInfo,
                       const L1FieldRegion& F);

  static fvec ApproximateBetheBloch(const fvec& bg2);

  static fvec ApproximateBetheBloch(const fvec& bg2, const fvec& kp0, const fvec& kp1, const fvec& kp2, const fvec& kp3,
                                    const fvec& kp4);

  static void FilterChi2XYC00C10C11(const L1UMeasurementInfo& info, fvec& x, fvec& y, fvec& C00, fvec& C10, fvec& C11,
                                    fvec& chi2, const fvec& u, const fvec& du2);

  static void FilterChi2(const L1UMeasurementInfo& info, const fvec& x, const fvec& y, const fvec& C00, const fvec& C10,
                         const fvec& C11, fvec& chi2, const fvec& u, const fvec& du2);

public:
  L1TrackPar fTr {};
  fvec fQp0;

  fvec fMass  = 0.10565800;     // muon mass
  fvec fMass2 = fMass * fMass;  // mass squared

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

inline void L1Fit::ExtrapolateXC00Line(fvec z_out, fvec& x, fvec& C00) const
{
  const fvec dz = (z_out - fTr.z);
  x             = fTr.x + fTr.tx * dz;
  C00           = fTr.C00 + dz * (2 * fTr.C20 + dz * fTr.C22);
}

inline void L1Fit::ExtrapolateYC11Line(fvec z_out, fvec& y, fvec& C11) const
{
  const fvec dz = (z_out - fTr.z);
  y             = fTr.y + fTr.ty * dz;
  C11           = fTr.C11 + dz * (2 * fTr.C31 + dz * fTr.C33);
}

inline void L1Fit::ExtrapolateC10Line(fvec z_out, fvec& C10) const
{
  const fvec dz = (z_out - fTr.z);
  C10           = fTr.C10 + dz * (fTr.C21 + fTr.C30 + dz * fTr.C32);
}
#endif
