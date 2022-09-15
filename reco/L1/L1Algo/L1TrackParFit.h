/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak [committer], Sergey Gorbunov */

#ifndef L1TrackParFit_h
#define L1TrackParFit_h

#include "L1Def.h"
#include "L1Field.h"
#include "L1MaterialInfo.h"
#include "L1TrackPar.h"
#include "L1UMeasurementInfo.h"
#include "L1XYMeasurementInfo.h"

class L1TrackParFit {

public:
  fvec fx, fy, ftx, fty, fqp, fz, ft, C00, C10, C11, C20, C21, C22, C30, C31, C32, C33, C40, C41, C42, C43, C44, C50,
    C51, C52, C53, C54, C55, chi2, NDF;
  //  fvec n;

  fvec fMass  = 0.10565800;     // muon mass
  fvec fMass2 = fMass * fMass;  // mass squared

  L1TrackParFit()
    : fx(0.)
    , fy(0.)
    , ftx(0.)
    , fty(0.)
    , fqp(0.)
    , fz(0.)
    , ft(0.)
    , C00(0.)
    , C10(0.)
    , C11(0.)
    , C20(0.)
    , C21(0.)
    , C22(0.)
    , C30(0.)
    , C31(0.)
    , C32(0.)
    , C33(0.)
    , C40(0.)
    , C41(0.)
    , C42(0.)
    , C43(0.)
    , C44(0.)
    , C50(0.)
    , C51(0.)
    , C52(0.)
    , C53(0.)
    , C54(0.)
    , C55(0.)
    , chi2(0.)
    , NDF(0.) {};

  L1TrackParFit(double* T, double* C)
    : fx(T[0])
    , fy(T[1])
    , ftx(T[3])
    , fty(T[4])
    , fqp(T[5])
    , fz(T[6])
    , ft(T[7])
    , C00(C[0])
    , C10(C[1])
    , C11(C[2])
    , C20(C[3])
    , C21(C[4])
    , C22(C[5])
    , C30(C[6])
    , C31(C[7])
    , C32(C[8])
    , C33(C[9])
    , C40(C[10])
    , C41(C[11])
    , C42(C[12])
    , C43(C[13])
    , C44(C[14])
    , C50(C[15])
    , C51(C[16])
    , C52(C[17])
    , C53(C[18])
    , C54(C[19])
    , C55(C[20])
    , chi2(0.)
    , NDF(0.) {};

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

  void Filter(L1UMeasurementInfo& info, fvec u, fvec w = 1.);
  void FilterXY(const L1XYMeasurementInfo& info, fvec x, fvec y);
  void FilterTime(fvec t0, fvec dt0, fvec w = 1., fvec timeInfo = 1.);
  void FilterNoP(L1UMeasurementInfo& info, fvec u, fvec w = 1.);
  void Extrapolate(fvec z_out, fvec qp0, const L1FieldRegion& F, fvec* w = 0);
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

  void L1AddMaterial(const L1MaterialInfo& info, fvec qp0, fvec w);

  void L1AddMaterial(fvec radThick, fvec qp0, fvec w = 1);

  void L1AddThickMaterial(fvec radThick, fvec qp0, fvec w, fvec thickness, bool fDownstream);
  void L1AddPipeMaterial(fvec qp0, fvec w = 1);

  // void L1Extrapolate
  // (
  // //  L1TrackParFit &T, // input track parameters (x,y,tx,ty,Q/p) and cov.matrix
  //  fvec        z_out  , // extrapolate to this z position
  //  fvec       qp0    , // use Q/p linearisation at this value
  //  L1FieldRegion &F
  //  );


} _fvecalignment;

// =============================================================================================

inline void L1TrackParFit::Print(int i)
{
  std::cout.setf(std::ios::scientific, std::ios::floatfield);
  if (i == -1) {
    std::cout << "T = " << std::endl;
    std::cout << fx << std::endl;
    std::cout << fy << std::endl;
    std::cout << ftx << std::endl;
    std::cout << fty << std::endl;
    std::cout << fqp << std::endl;
    std::cout << fz << std::endl;
    std::cout << ft << std::endl;
  }
  else {
    std::cout << "T = ";
    std::cout << fx[i] << " ";
    std::cout << fy[i] << " ";
    std::cout << ftx[i] << " ";
    std::cout << fty[i] << " ";
    std::cout << fqp[i] << " ";
    std::cout << fz[i] << std::endl;
    std::cout << "C = ";
    std::cout << C00[i] << " ";
    std::cout << C11[i] << " ";
    std::cout << C22[i] << " ";
    std::cout << C33[i] << " ";
    std::cout << C44[i] << " ";
    std::cout << C55[i] << std::endl;
  }
}


inline void L1TrackParFit::Compare(L1TrackPar& T)
{
  std::cout.precision(8);

  //   if (fabs(T.x[0]-fx[0])/T.x[0] > 1.e-7)  std::cout << fx  <<" x "<< T.x << std::endl;
  //
  //   if (fabs(T.y[0]-fy[0])/T.y[0] > 1.e-7)  std::cout << fy  <<" y "<< T.y << std::endl;
  //
  //   if (fabs(T.tx[0]-ftx[0])/T.tx[0] > 1.e-7)  std::cout << ftx  <<" tx "<< T.tx << std::endl;
  //
  //   if (fabs(T.ty[0]-fty[0])/T.ty[0] > 1.e-7)  std::cout << fty  <<" ty "<< T.ty << std::endl;
  //
  //   if (fabs(T.qp[0]-fqp[0])/T.qp[0] > 1.e-7)  std::cout << fqp  <<" qp "<< T.qp << std::endl;
  //
  //   if (fabs(T.z[0]-fz[0])/T.z[0] > 1.e-7)  std::cout << fz  <<" z "<< T.z << std::endl;
  //
  //   if (fabs(T.C00[0]-C00[0])/T.C00[0] > 1.e-7)  std::cout << C00  <<" C00 "<< T.C00 << std::endl;
  //   if (fabs(T.C11[0]-C11[0])/T.C11[0] > 1.e-7)  std::cout << C11  <<" C11 "<< T.C11 << std::endl;
  //   if (fabs(T.C22[0]-C22[0])/T.C22[0] > 1.e-7)  std::cout << C22  <<" C22 "<< T.C22 << std::endl;
  //   if (fabs(T.C33[0]-C33[0])/T.C33[0] > 1.e-7)  std::cout << C33  <<" C33 "<< T.C33 << std::endl;
  //   if (fabs(T.C44[0]-C44[0])/T.C44[0] > 1.e-7)  std::cout << C44  <<" C44 "<< T.C44 << std::endl;


  //   if (!(T.x==fx)[0])  std::cout << fx  <<" x "<< T.x << std::endl;
  //   if (!(T.y==fy)[0])  std::cout << fy  << " y "<< T.y << std::endl;
  //   if (!(T.tx==ftx)[0])  std::cout << ftx << " ty "<< T.tx << std::endl;
  //   if (!(T.ty==fty)[0])  std::cout << fty << " ty "<< T.ty << std::endl;
  //   if (!(T.qp==fqp)[0])  std::cout << fqp << " qp "<< T.qp << std::endl;
  //   if (!(T.z==fz)[0])  std::cout << fz  << " z "<< T.z << std::endl;
  //   // if (T.t!=ft)  std::cout << ft  << " fx "<< T.x << std::endl;


  //   if (!(T.C00==C00)[0])  std::cout << C00  << " C00 "<< T.C00 << std::endl;
  //   if (!(T.C11==C11)[0])  std::cout << C11  << " C11 "<< T.C11 << std::endl;
  //   if (!(T.C22==C22)[0])  std::cout << C22  <<  " C22 "<< T.C22 << std::endl;
  //   if (!(T.C33==C33)[0])  std::cout << C33  << " C33 "<< T.C33 << std::endl;
  //   if (!(T.C44==C44)[0])  std::cout << C44  << " C44 "<< T.C44 << std::endl;
  // if (T.C55!=C55)  std::cout << C55  <<  " fx "<< T.C55 << std::endl;


  std::cout << "parameters:" << std::endl;
  std::cout << T.x[0] << " " << T.y[0] << " " << T.z[0] << " " << T.tx[0] << " " << T.ty[0] << " " << T.qp[0]
            << std::endl;
  std::cout << fx[0] << " " << fy[0] << " " << fz[0] << " " << ftx[0] << " " << fty[0] << " " << fqp[0] << " " << ft[0]
            << std::endl;

  std::cout << "Covariance matrix:" << std::endl;
  std::cout << T.C00[0] << " " << T.C10[0] << " " << T.C11[0] << " " << T.C20[0] << " " << T.C21[0] << " " << T.C22[0]
            << " " << T.C30[0] << " " << T.C31[0] << " " << T.C32[0] << " " << T.C33[0] << " " << T.C40[0] << " "
            << T.C41[0] << " " << T.C42[0] << " " << T.C43[0] << " " << T.C44[0] << std::endl;

  std::cout << C00[0] << " " << C10[0] << " " << C11[0] << " " << C20[0] << " " << C21[0] << " " << C22[0] << " "
            << C30[0] << " " << C31[0] << " " << C32[0] << " " << C33[0] << " " << C40[0] << " " << C41[0] << " "
            << C42[0] << " " << C43[0] << " " << C44[0] << std::endl;
  std::cout << "    Time covariance:" << std::endl;
  std::cout << "    " << C50[0] << " " << C51[0] << " " << C52[0] << " " << C53[0] << " " << C54[0] << " " << C55[0]
            << " " << std::endl;

  std::cout << std::endl;
}

inline void L1TrackParFit::SetOneEntry(const int i0, const L1TrackParFit& T1, const int i1)
{
  fx[i0]  = T1.fx[i1];
  fy[i0]  = T1.fy[i1];
  ftx[i0] = T1.ftx[i1];
  fty[i0] = T1.fty[i1];
  fqp[i0] = T1.fqp[i1];
  fz[i0]  = T1.fz[i1];
  ft[i0]  = T1.ft[i1];
  C00[i0] = T1.C00[i1];
  C10[i0] = T1.C10[i1];
  C11[i0] = T1.C11[i1];
  C20[i0] = T1.C20[i1];
  C21[i0] = T1.C21[i1];
  C22[i0] = T1.C22[i1];
  C30[i0] = T1.C30[i1];
  C31[i0] = T1.C31[i1];
  C32[i0] = T1.C32[i1];
  C33[i0] = T1.C33[i1];
  C40[i0] = T1.C40[i1];
  C41[i0] = T1.C41[i1];
  C42[i0] = T1.C42[i1];
  C43[i0] = T1.C43[i1];
  C44[i0] = T1.C44[i1];
  C50[i0] = T1.C50[i1];
  C51[i0] = T1.C51[i1];
  C52[i0] = T1.C52[i1];
  C53[i0] = T1.C53[i1];
  C54[i0] = T1.C54[i1];
  C55[i0] = T1.C55[i1];

  chi2[i0] = T1.chi2[i1];
  NDF[i0]  = T1.NDF[i1];
  //  time[i0] = T1.time[i1];
  // n[i0] = T1.n[i1];
}  // SetOneEntry

#endif
