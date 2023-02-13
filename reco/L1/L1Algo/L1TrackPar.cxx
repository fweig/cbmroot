/* Copyright (C) 2007-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Maksym Zyzak */

#include "L1TrackPar.h"

#include <iomanip>
#include <iostream>


void L1TrackPar::Print(int i) const
{
  std::ios_base::fmtflags coutFlags(std::cout.flags());
  std::cout.setf(std::ios::scientific, std::ios::floatfield);
  if (i == -1) {
    std::cout << "T = " << std::endl;
    std::cout << " x " << x << std::endl;
    std::cout << " y " << y << std::endl;
    std::cout << " tx " << tx << std::endl;
    std::cout << " ty " << ty << std::endl;
    std::cout << " qp " << qp << std::endl;
    std::cout << " t " << t << std::endl;
    std::cout << " v " << fvec(1.) / vi << std::endl;
    std::cout << " z " << z << std::endl;
    std::cout << "C = " << std::endl;
    std::cout << " c00 " << C00 << std::endl;
    std::cout << " c11 " << C11 << std::endl;
    std::cout << " c22 " << C22 << std::endl;
    std::cout << " c33 " << C33 << std::endl;
    std::cout << " c44 " << C44 << std::endl;
    std::cout << " c55 " << C55 << std::endl;
    std::cout << " c66 " << C66 << std::endl;
  }
  else {
    std::cout << "T = ";
    std::cout << " x " << x[i];
    std::cout << " y " << y[i];
    std::cout << " tx " << tx[i];
    std::cout << " ty " << ty[i];
    std::cout << " qp " << qp[i];
    std::cout << " t " << t[i];
    std::cout << " v " << 1. / vi[i];

    std::cout << " z " << z[i] << std::endl;
    std::cout << "C = ";
    std::cout << " c00 " << C00[i];
    std::cout << " c11 " << C11[i];
    std::cout << " c22 " << C22[i];
    std::cout << " c33 " << C33[i];
    std::cout << " c44 " << C44[i] << std::endl;
    std::cout << " c55 " << C55[i] << std::endl;
    std::cout << " c66 " << C66[i] << std::endl;
  }
  PrintCorrelations(i);
  std::cout.flags(coutFlags);
}

void L1TrackPar::PrintCorrelations(int i) const
{
  auto flagSafe = std::cout.flags();
  // std::cout.setf(std::ios::scientific, std::ios::floatfield);
  std::cout << std::setprecision(6);

  if (i == -1) {
    fvec s0 = sqrt(C00);
    fvec s1 = sqrt(C11);
    fvec s2 = sqrt(C22);
    fvec s3 = sqrt(C33);
    fvec s4 = sqrt(C44);
    fvec s5 = sqrt(C55);
    fvec s6 = sqrt(C66);

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

    std::cout << "\n k60 " << C60 / s6 / s0 << std::endl;
    std::cout << " k61 " << C61 / s6 / s1 << std::endl;
    std::cout << " k62 " << C62 / s6 / s2 << std::endl;
    std::cout << " k63 " << C63 / s6 / s3 << std::endl;
    std::cout << " k64 " << C64 / s6 / s4 << std::endl;
    std::cout << " k65 " << C65 / s6 / s5 << std::endl;
  }
  else {
    float s0 = sqrt(C00[i]);
    float s1 = sqrt(C11[i]);
    float s2 = sqrt(C22[i]);
    float s3 = sqrt(C33[i]);
    float s4 = sqrt(C44[i]);
    float s5 = sqrt(C55[i]);
    float s6 = sqrt(C66[i]);

    std::cout << "K = " << std::endl;
    std::cout << " " << C10[i] / s1 / s0 << std::endl;
    std::cout << " " << C20[i] / s2 / s0 << " " << C21[i] / s2 / s1 << std::endl;
    std::cout << " " << C30[i] / s3 / s0 << " " << C31[i] / s3 / s1 << " " << C32[i] / s3 / s2 << std::endl;
    std::cout << " " << C40[i] / s4 / s0 << " " << C41[i] / s4 / s1 << " " << C42[i] / s4 / s2 << " "
              << C43[i] / s4 / s3 << std::endl;
    std::cout << " " << C50[i] / s5 / s0 << " " << C51[i] / s5 / s1 << " " << C52[i] / s5 / s2 << " "
              << C53[i] / s5 / s3 << " " << C54[i] / s5 / s4 << std::endl;
    std::cout << " " << C60[i] / s6 / s0 << " " << C61[i] / s6 / s1 << " " << C62[i] / s6 / s2 << " "
              << C63[i] / s6 / s3 << " " << C64[i] / s6 / s4 << " " << C65[i] / s6 / s5 << std::endl;
  }
  std::cout.flags(flagSafe);
}

bool L1TrackPar::IsEntryConsistent(bool printWhenWrong, int k) const
{
  bool ok = true;

  // verify that all the numbers in the object are valid floats
  const fvec* memberFirst = &x;
  const fvec* memberLast  = &NDF;
  for (int i = 0; i < &memberLast - &memberFirst + 1; i++) {
    if (!std::isfinite(memberFirst[i][k])) {
      ok = false;
      if (printWhenWrong) {
        std::cout << " L1TrackPar member N " << i << ", vector entry " << k
                  << " is not a number: " << memberFirst[i][k];
      }
    }
  }

  // verify diagonal elements.
  // Cii is a squared dispersion of i-th parameter, it must be positive

  for (int i = 0; i < 7; i++) {
    if (C(i, i)[k] <= 0.f) {
      ok = false;
      if (printWhenWrong) {
        std::cout << " L1TrackPar: C[" << i << "," << i << "], vec entry " << k << " is not positive: " << C(i, i)[k]
                  << std::endl;
      }
    }
  }

  // verify non-diagonal elements.
  // Cij/sqrt(Cii*Cjj) is a correlation between i-th and j-th parameter,
  // it must belong to [-1,1]

  for (int i = 1; i < 7; i++) {
    for (int j = 0; j < i; j++) {
      double tolerance = 1.0;
      if (C(i, j)[k] * C(i, j)[k] > tolerance * (C(i, i)[k] * C(j, j)[k])) {
        ok = false;
        if (printWhenWrong) {
          std::cout << " L1TrackPar: correlation [" << i << "," << j << "], vec entry " << k
                    << " is too large: " << C(i, i)[k] / sqrt(C(i, i)[k] * C(j, j)[k]) << std::endl;
        }
      }
    }
  }

  // verify triplets of correlations
  // Kxy * Kxy + Kxz * Kxz + Kyz * Kyz <= 1 + 2 * Kxy * Kxz * Kyz

  for (int i = 2; i < 7; i++) {
    for (int j = 1; j < i; j++) {
      for (int m = 0; m < j; m++) {
        double tolerance = 1.0;
        double Cxx       = C(i, i)[k];
        double Cyy       = C(j, j)[k];
        double Czz       = C(m, m)[k];
        double Cxy       = C(i, j)[k];
        double Cxz       = C(i, m)[k];
        double Cyz       = C(j, m)[k];
        if (Cxx * Cyz * Cyz + Cyy * Cxz * Cxz + Czz * Cxy * Cxy
            > tolerance * (Cxx * Cyy * Czz + 2. * Cxy * Cyz * Cxz)) {
          ok = false;
          if (printWhenWrong) {
            double Kxy = Cxy / sqrt(Cxx * Cyy);
            double Kxz = Cxz / sqrt(Cxx * Czz);
            double Kyz = Cyz / sqrt(Cyy * Czz);
            std::cout << " L1TrackPar: correlations between parametetrs " << i << ", " << j << ", " << m
                      << ", vec entry " << k << " are wrong: " << Kxy << " " << Kxz << " " << Kyz << std::endl;
            std::cout << " inequation: " << Kxy * Kxy + Kxz * Kxz + Kyz * Kyz << " > " << 1 + 2 * Kxy * Kxz * Kyz
                      << std::endl;
          }
        }
      }
    }
  }

  if (!ok && printWhenWrong) {
    std::cout << "L1TrackPar parameters are not consistent: " << std::endl;
    Print(k);
  }
  return ok;
}

bool L1TrackPar::IsConsistent(bool printWhenWrong, int nFilled) const
{
  assert(nFilled <= (int) fvec::size());
  bool ok = true;
  if (nFilled < 0) { nFilled = fvec::size(); }
  for (int i = 0; i < nFilled; ++i) {
    ok = ok && IsEntryConsistent(printWhenWrong, i);
  }

  if (!ok && printWhenWrong) {
    std::cout << "L1TrackPar parameters are not consistent: " << std::endl;
    if (nFilled == (int) fvec::size()) { std::cout << "  All vector elements are filled " << std::endl; }
    else {
      std::cout << "  Only first " << nFilled << " vector elements are filled " << std::endl;
    }
    Print(-1);
  }
  return ok;
}
