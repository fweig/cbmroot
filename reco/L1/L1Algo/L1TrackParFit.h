/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak [committer], Sergey Gorbunov */

#ifndef L1TrackParFit_h
#define L1TrackParFit_h

#include "L1Def.h"
#include "L1Field.h"
#include "L1TrackPar.h"
#include "L1UMeasurementInfo.h"
#include "L1XYMeasurementInfo.h"

class L1TrackParFit {

public:
  L1TrackPar fTr {};
  fvec fQp0;

  fvec fMass  = 0.10565800;     // muon mass
  fvec fMass2 = fMass * fMass;  // mass squared

  fvec fPipeRadThick {7.87e-3f};        // 0.7 mm Aluminium  // TODO:
  fvec fTargetRadThick {3.73e-2f * 2};  // 250 mum Gold      // TODO:

  L1TrackParFit() = default;

  L1TrackParFit(double* T, double* C) : fTr(T, C) {}

  void SetOneEntry(const int i0, const L1TrackParFit& T1, const int i1);

  void Print(int i = -1);

  //Fit functionality

  /// set particle mass for the fit
  void SetParticleMass(float mass)
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
  void FilterNoP(L1UMeasurementInfo& info, fvec u, fvec w = 1.);

  void Extrapolate(fvec z_out, fvec qp0, const L1FieldRegion& F, const fvec& w);
  void ExtrapolateStep(fvec z_out, fvec qp0, const L1FieldRegion& F, const fvec& w);

  void ExtrapolateLine(fvec z_out, fvec* w = 0);
  void ExtrapolateLine1(fvec z_out, fvec* w = 0, fvec v = 0);

  void Compare(L1TrackPar& T);

  void EnergyLossCorrection(const fvec& radThick, fvec& qp0, fvec direction, fvec w);

  template<int atomicZ>
  void EnergyLossCorrection(float atomicA, float rho, float radLen, const fvec& radThick, fvec& qp0, fvec direction,
                            fvec w);

  void EnergyLossCorrectionIron(const fvec& radThick, fvec& qp0, fvec direction, fvec w);
  void EnergyLossCorrectionCarbon(const fvec& radThick, fvec& qp0, fvec direction, fvec w);
  void EnergyLossCorrectionAl(const fvec& radThick, fvec& qp0, fvec direction, fvec w);

  void AddMaterial(const fvec& radThick, fvec qp0, fvec w = 1);

  void AddThickMaterial(fvec radThick, fvec qp0, fvec w, fvec thickness, bool fDownstream);
  void AddPipeMaterial(fvec qp0, fvec w);

  // void L1Extrapolate
  // (
  // //  L1TrackParFit &T, // input track parameters (x,y,tx,ty,Q/p) and cov.matrix
  //  fvec        z_out  , // extrapolate to this z position
  //  fvec       qp0    , // use Q/p linearisation at this value
  //  L1FieldRegion &F
  //  );

} _fvecalignment;

// =============================================================================================

inline void L1TrackParFit::Print(int i) { fTr.Print(i); }


inline void L1TrackParFit::SetOneEntry(const int i0, const L1TrackParFit& T1, const int i1)
{
  fTr.SetOneEntry(i0, T1.fTr, i1);
  fQp0[i0] = T1.fQp0[i1];
}

#endif
