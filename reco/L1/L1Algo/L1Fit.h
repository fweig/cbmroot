/// \file   L1Fit.h
/// \brief  Definition of the L1Fit class
/// \author Sergey Gorbunov <se.gorbunov@gsi.de>
/// \date   15.03.2021

#ifndef L1Fit_H
#define L1Fit_H

#include "CbmL1Def.h"

#include "L1MaterialInfo.h"
#include "L1TrackPar.h"

///
/// A collection of fit routines used by the CA tracker
///
class L1Fit {

public:
  /// constructor
  L1Fit() {};

  /// destructor
  ~L1Fit() {};

  /// set particle mass for the fit
  void SetParticleMass(fvec mass) { fMass2 = mass * mass; }

  /// get the particle mass squared
  fvec GetParticleMass2() const { return fMass2; }

  /// === routines for material

  fvec BetheBlochIron(const float qp);

  fvec BetheBlochCarbon(const float qp);

  fvec BetheBlochAl(const float qp);

  static fvec ApproximateBetheBloch(const fvec& bg2);

  static fvec ApproximateBetheBloch(const fvec& bg2, const fvec& kp0, const fvec& kp1, const fvec& kp2, const fvec& kp3,
                                    const fvec& kp4);


  float CalcQpAfterEloss(float qp, float eloss, float mass2);

  void EnergyLossCorrection(L1TrackPar& T, const fvec& radThick, fvec& qp0, fvec direction, fvec w);

  void EnergyLossCorrectionAl(L1TrackPar& T, const fvec& radThick, fvec& qp0, fvec direction, fvec w);


  void EnergyLossCorrectionCarbon(L1TrackPar& T, const fvec& radThick, fvec& qp0, fvec direction, fvec w);


  void EnergyLossCorrectionIron(L1TrackPar& T, const fvec& radThick, fvec& qp0, fvec direction, fvec w);

  void L1AddMaterial(L1TrackPar& T, fvec radThick, fvec qp0, fvec w);

  void L1AddMaterial(L1TrackPar& T, L1MaterialInfo& info, fvec qp0, fvec w);


  void L1AddThickMaterial(L1TrackPar& T, fvec radThick, fvec qp0, fvec w, fvec thickness, bool fDownstream);

  void L1AddHalfMaterial(L1TrackPar& T, L1MaterialInfo& info, fvec qp0);

  void L1AddPipeMaterial(L1TrackPar& T, fvec qp0, fvec w);

  void L1AddTargetMaterial(L1TrackPar& T, fvec qp0, fvec w);

  const fvec PipeRadThick   = 7.87e-3f;      // 0.7 mm Aluminium
  const fvec TargetRadThick = 3.73e-2f * 2;  // 250 mum Gold

private:
  fvec fMass2 = 0.10565800 * 0.10565800;  // muon mass by default (pion 0.13957000f )

  //ClassDefNV(L1Fit, 0)
};

#include "L1FitMaterial.h"

#endif
