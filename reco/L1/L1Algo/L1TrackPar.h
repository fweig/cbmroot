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

  void Print(int i = -1) const;

  void PrintCorrelations(int i = -1) const;

  bool IsEntryConsistent(bool printWhenWrong, int i) const;

  bool IsConsistent(bool printWhenWrong, int nFilled) const;

  // void L1Extrapolate
  // (
  // //  L1TrackPar &T, // input track parameters (x,y,tx,ty,Q/p) and cov.matrix
  //  fvec        z_out  , // extrapolate to this z position
  //  fvec       qp0    , // use Q/p linearisation at this value
  //  L1FieldRegion &F
  //  );


} _fvecalignment;

// =============================================================================================

inline void L1TrackPar::Print(int i) const
{
  // std::cout.setf(std::ios::scientific, std::ios::floatfield);
  if (i == -1) {
    std::cout << "T = " << std::endl;
    std::cout << " x " << x << std::endl;
    std::cout << " y " << y << std::endl;
    std::cout << " tx " << tx << std::endl;
    std::cout << " ty " << ty << std::endl;
    std::cout << " qp " << qp << std::endl;
    std::cout << " t " << t << std::endl;
    std::cout << " z " << z << std::endl;
    std::cout << "C = " << std::endl;
    std::cout << " c00 " << C00 << std::endl;
    std::cout << " c11 " << C11 << std::endl;
    std::cout << " c22 " << C22 << std::endl;
    std::cout << " c33 " << C33 << std::endl;
    std::cout << " c44 " << C44 << std::endl;
    std::cout << " c55 " << C55 << std::endl;
  }
  else {
    std::cout << "T = ";
    std::cout << " x " << x[i];
    std::cout << " y " << y[i];
    std::cout << " tx " << tx[i];
    std::cout << " ty " << ty[i];
    std::cout << " qp " << qp[i];
    std::cout << " t " << t[i];
    std::cout << " z " << z[i] << std::endl;
    std::cout << "C = ";
    std::cout << " c00 " << C00[i];
    std::cout << " c11 " << C11[i];
    std::cout << " c22 " << C22[i];
    std::cout << " c33 " << C33[i];
    std::cout << " c44 " << C44[i] << std::endl;
    std::cout << " c55 " << C55[i] << std::endl;
  }
  PrintCorrelations(i);
}

inline void L1TrackPar::PrintCorrelations(int i) const
{
  // std::cout.setf(std::ios::scientific, std::ios::floatfield);

  if (i == -1) {
    fvec s0 = sqrt(C00);
    fvec s1 = sqrt(C11);
    fvec s2 = sqrt(C22);
    fvec s3 = sqrt(C33);
    fvec s4 = sqrt(C44);
    fvec s5 = sqrt(C55);

    std::cout << "K = " << std::endl;
    std::cout << " k10 " << C10 / s1 / s0 << std::endl;

    std::cout << "\n k20 " << C20 / s2 / s0 << std::endl;
    std::cout << " k21 " << C21 / s2 / s1 << std::endl;

    std::cout << "\n k30 " << C30 / s3 / s0 << std::endl;
    std::cout << " k31 " << C31 / s3 / s1 << std::endl;
    std::cout << " k32 " << C32 / s3 / s2 << std::endl;

    std::cout << "\n k40 " << C40 / s4 / s0 << std::endl;
    std::cout << " k41 " << C41 / s4 / s1 << std::endl;
    std::cout << " k42 " << C42 / s4 / s2 << std::endl;
    std::cout << " k43 " << C43 / s4 / s3 << std::endl;

    std::cout << "\n k50 " << C50 / s5 / s0 << std::endl;
    std::cout << " k51 " << C51 / s5 / s1 << std::endl;
    std::cout << " k52 " << C52 / s5 / s2 << std::endl;
    std::cout << " k53 " << C53 / s5 / s3 << std::endl;
    std::cout << " k54 " << C54 / s5 / s4 << std::endl;
  }
  else {
    float s0 = sqrt(C00[i]);
    float s1 = sqrt(C11[i]);
    float s2 = sqrt(C22[i]);
    float s3 = sqrt(C33[i]);
    float s4 = sqrt(C44[i]);
    float s5 = sqrt(C55[i]);

    std::cout << "K = " << std::endl;
    std::cout << " " << C10[i] / s1 / s0 << std::endl;
    std::cout << " " << C20[i] / s2 / s0 << " " << C21[i] / s2 / s1 << std::endl;
    std::cout << " " << C30[i] / s3 / s0 << " " << C31[i] / s3 / s1 << " " << C32[i] / s3 / s2 << std::endl;
    std::cout << " " << C40[i] / s4 / s0 << " " << C41[i] / s4 / s1 << " " << C42[i] / s4 / s2 << " "
              << C43[i] / s4 / s3 << std::endl;
    std::cout << " " << C50[i] / s5 / s0 << " " << C51[i] / s5 / s1 << " " << C52[i] / s5 / s2 << " "
              << C53[i] / s5 / s3 << " " << C54[i] / s5 / s4 << std::endl;
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

inline bool L1TrackPar::IsEntryConsistent(bool printWhenWrong, int i) const
{
  bool ok = true;

  const fvec* v  = &x;
  const fvec* v1 = &NDF;
  for (; v < v1; v++) {
    ok = ok && std::isfinite((*v)[i]);
  }
  // verify diagonal elements

  ok = ok && (C00[i] > 0.f);
  ok = ok && (C11[i] > 0.f);
  ok = ok && (C22[i] > 0.f);
  ok = ok && (C33[i] > 0.f);
  ok = ok && (C44[i] > 0.f);
  ok = ok && (C55[i] > 0.f);

  // verify non-diagonal elements
  ok = ok && (C10[i] * C10[i] <= C11[i] * C00[i]);

  ok = ok && (C20[i] * C20[i] <= C22[i] * C00[i]);
  ok = ok && (C21[i] * C21[i] <= C22[i] * C11[i]);

  ok = ok && (C30[i] * C30[i] <= C33[i] * C00[i]);
  ok = ok && (C31[i] * C31[i] <= C33[i] * C11[i]);
  ok = ok && (C32[i] * C32[i] <= C33[i] * C22[i]);

  ok = ok && (C40[i] * C40[i] <= C44[i] * C00[i]);
  ok = ok && (C41[i] * C41[i] <= C44[i] * C11[i]);
  ok = ok && (C42[i] * C42[i] <= C44[i] * C22[i]);
  ok = ok && (C43[i] * C43[i] <= C44[i] * C33[i]);

  ok = ok && (C50[i] * C50[i] <= C55[i] * C00[i]);
  ok = ok && (C51[i] * C51[i] <= C55[i] * C11[i]);
  ok = ok && (C52[i] * C52[i] <= C55[i] * C22[i]);
  ok = ok && (C53[i] * C53[i] <= C55[i] * C33[i]);
  ok = ok && (C54[i] * C54[i] <= C55[i] * C44[i]);

  if (!ok && printWhenWrong) {
    std::cout << "L1TrackPar parameters are not consistent: " << std::endl;
    Print(i);
  }
  return ok;
}

inline bool L1TrackPar::IsConsistent(bool printWhenWrong, int nFilled) const
{
  assert(nFilled <= fvecLen);
  bool ok = true;
  if (nFilled < 0) { nFilled = fvecLen; }
  for (int i = 0; i < nFilled; ++i) {
    ok = ok && IsEntryConsistent(printWhenWrong, i);
  }

  if (!ok && printWhenWrong) {
    std::cout << "L1TrackPar parameters are not consistent: " << std::endl;
    if (nFilled == fvecLen) { std::cout << "  All vector elements are filled " << std::endl; }
    else {
      std::cout << "  Only first " << nFilled << " vector elements are filled " << std::endl;
    }
    Print(-1);
  }
  return ok;
}


#endif
