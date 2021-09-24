/* Copyright (C) 2007-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Maksym Zyzak */

#ifndef L1TrackPar_h
#define L1TrackPar_h 1

#include "L1Def.h"

class L1TrackPar {

public:
  fvec x, y, tx, ty, qp, z, t, C00, C10, C11, C20, C21, C22, C30, C31, C32, C33, C40, C41, C42, C43, C44, C50, C51, C52,
    C53, C54, C55, chi2, NDF;


  L1TrackPar()
    : x(0)
    , y(0)
    , tx(0)
    , ty(0)
    , qp(0)
    , z(0)
    , t(0)
    , C00(0)
    , C10(0)
    , C11(0)
    , C20(0)
    , C21(0)
    , C22(0)
    , C30(0)
    , C31(0)
    , C32(0)
    , C33(0)
    , C40(0)
    , C41(0)
    , C42(0)
    , C43(0)
    , C44(0)
    , C50(0)
    , C51(0)
    , C52(0)
    , C53(0)
    , C54(0)
    , C55(0)
    , chi2(0)
    , NDF(0) {};
  L1TrackPar(double* T, double* C)
    : x(T[0])
    , y(T[1])
    , tx(T[2])
    , ty(T[3])
    , qp(T[4])
    , z(T[5])
    , t(T[6])
    ,

    C00(C[0])
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
    , chi2(0)
    , NDF(0) {};

  void SetOneEntry(const int i0, const L1TrackPar& T1, const int i1);

  void Print(int i = -1);
  // void L1Extrapolate
  // (
  // //  L1TrackPar &T, // input track parameters (x,y,tx,ty,Q/p) and cov.matrix
  //  fvec        z_out  , // extrapolate to this z position
  //  fvec       qp0    , // use Q/p linearisation at this value
  //  L1FieldRegion &F
  //  );


} _fvecalignment;

// =============================================================================================

inline void L1TrackPar::Print(int i)
{
  std::cout.setf(std::ios::scientific, std::ios::floatfield);
  if (i == -1) {
    std::cout << "T = " << std::endl;
    std::cout << x << std::endl;
    std::cout << y << std::endl;
    std::cout << tx << std::endl;
    std::cout << ty << std::endl;
    std::cout << qp << std::endl;
    std::cout << z << std::endl;
  }
  else {
    std::cout << "T = ";
    std::cout << x[i] << " ";
    std::cout << y[i] << " ";
    std::cout << tx[i] << " ";
    std::cout << ty[i] << " ";
    std::cout << qp[i] << " ";
    std::cout << z[i] << std::endl;
    std::cout << "C = ";
    std::cout << C00[i] << " ";
    std::cout << C11[i] << " ";
    std::cout << C22[i] << " ";
    std::cout << C33[i] << " ";
    std::cout << C44[i] << std::endl;
  }
}

inline void L1TrackPar::SetOneEntry(const int i0, const L1TrackPar& T1, const int i1)
{
  x[i0]   = T1.x[i1];
  y[i0]   = T1.y[i1];
  tx[i0]  = T1.tx[i1];
  ty[i0]  = T1.ty[i1];
  qp[i0]  = T1.qp[i1];
  z[i0]   = T1.z[i1];
  t[i0]   = T1.t[i1];
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
}  // SetOneEntry

#endif
