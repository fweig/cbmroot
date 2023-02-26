/* Copyright (C) 2007-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Maksym Zyzak */

#ifndef L1TrackPar_h
#define L1TrackPar_h 1

#include "CaSimd.h"
#include "L1Def.h"

using namespace cbm::algo::ca;

class L1TrackPar {

public:
  static constexpr int kNparTr {8};
  static constexpr int kNparCov {28};

  fvec x {0.}, y {0.}, tx {0.}, ty {0.}, qp {0.}, z {0.}, t {0.}, vi {0.};

  fvec C00 {0.},

    C10 {0.}, C11 {0.},

    C20 {0.}, C21 {0.}, C22 {0.},

    C30 {0.}, C31 {0.}, C32 {0.}, C33 {0.},

    C40 {0.}, C41 {0.}, C42 {0.}, C43 {0.}, C44 {0.},

    C50 {0.}, C51 {0.}, C52 {0.}, C53 {0.}, C54 {0.}, C55 {0.},

    C60 {0.}, C61 {0.}, C62 {0.}, C63 {0.}, C64 {0.}, C65 {0.}, C66 {0.};

  fvec chi2 {0.}, NDF {0.};

  fvec nTimeMeasurements {0.};

  L1TrackPar() = default;

  template<typename T>
  L1TrackPar(const T* tr, const T* C)
  {
    Set(tr, C);
  }

  //template<typename T>
  //void Set(const T* tr, const T* C);

  void SetOneEntry(const int i0, const L1TrackPar& T1, const int i1);

  fvec C(int i, int j) const
  {
    const fvec* c = &C00;
    int ind       = (j <= i) ? i * (1 + i) / 2 + j : j * (1 + j) / 2 + i;
    return c[ind];
  }

  fvec& C(int i, int j)
  {
    fvec* c = &C00;
    int ind = (j <= i) ? i * (1 + i) / 2 + j : j * (1 + j) / 2 + i;
    return c[ind];
  }

  void ResetErrors(fvec c00, fvec c11, fvec c22, fvec c33, fvec c44, fvec c55, fvec c66);

  void Print(int i = -1) const;

  void PrintCorrelations(int i = -1) const;

  bool IsEntryConsistent(bool printWhenWrong, int i) const;

  bool IsConsistent(bool printWhenWrong, int nFilled) const;

  template<typename T>
  void copyToArrays(const int iVec, T p[kNparTr], T c[kNparCov]) const;

  template<typename T>
  void copyFromArrays(const int iVec, const T p[kNparTr], const T c[kNparCov]);

  template<typename T>
  void copyFromArrays(const T p[kNparTr], const T c[kNparCov]);

  void InitVelocityRange(fscal minP);

  static constexpr fscal kClightNs {29.9792458};          // the speed of light cm/ns
  static constexpr fscal kClightNsInv {1. / 29.9792458};  // inverse speed of light
  static constexpr fscal kProtonMass   = 0.93827208816;
  static constexpr fscal kPionMass     = 0.1395703918;
  static constexpr fscal kMuonMass     = 0.105658375523;
  static constexpr fscal kElectronMass = 0.0005109989500015;

} _fvecalignment;

// =============================================================================================

template<typename T>
inline void L1TrackPar::copyToArrays(const int iVec, T p[kNparTr], T c[kNparCov]) const
{
  p[0] = x[iVec];
  p[1] = y[iVec];
  p[2] = tx[iVec];
  p[3] = ty[iVec];
  p[4] = qp[iVec];
  p[5] = z[iVec];
  p[6] = t[iVec];
  p[7] = vi[iVec];

  const fvec* tC = &(C00);
  for (int i = 0; i < kNparCov; i++) {
    c[i] = tC[i][iVec];
  }
}

template<typename T>
inline void L1TrackPar::copyFromArrays(const int iVec, const T p[kNparTr], const T c[kNparCov])
{
  x[iVec]  = p[0];
  y[iVec]  = p[1];
  tx[iVec] = p[2];
  ty[iVec] = p[3];
  qp[iVec] = p[4];
  z[iVec]  = p[5];
  t[iVec]  = p[6];
  vi[iVec] = p[7];

  fvec* tC = &(C00);
  for (int i = 0; i < kNparCov; i++) {
    tC[i][iVec] = c[i];
  }

  chi2 = 0.;
  NDF  = 0.;
}

template<typename T>
inline void L1TrackPar::copyFromArrays(const T p[kNparTr], const T c[kNparCov])
{
  x  = p[0];
  y  = p[1];
  tx = p[2];
  ty = p[3];
  qp = p[4];
  z  = p[5];
  t  = p[6];
  vi = p[7];

  fvec* tC = &(C00);
  for (int i = 0; i < kNparCov; i++) {
    tC[i] = c[i];
  }

  chi2 = 0.;
  NDF  = 0.;
}


inline void L1TrackPar::SetOneEntry(const int i0, const L1TrackPar& T1, const int i1)
{
  x[i0]  = T1.x[i1];
  y[i0]  = T1.y[i1];
  tx[i0] = T1.tx[i1];
  ty[i0] = T1.ty[i1];
  qp[i0] = T1.qp[i1];
  z[i0]  = T1.z[i1];
  t[i0]  = T1.t[i1];
  vi[i0] = T1.vi[i1];

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
  C60[i0] = T1.C60[i1];
  C61[i0] = T1.C61[i1];
  C62[i0] = T1.C62[i1];
  C63[i0] = T1.C63[i1];
  C64[i0] = T1.C64[i1];
  C65[i0] = T1.C65[i1];
  C66[i0] = T1.C66[i1];

  chi2[i0] = T1.chi2[i1];
  NDF[i0]  = T1.NDF[i1];
}  // SetOneEntry

inline void L1TrackPar::ResetErrors(fvec c00, fvec c11, fvec c22, fvec c33, fvec c44, fvec c55, fvec c66)
{
  C10 = 0.;
  C20 = C21 = 0.;
  C30 = C31 = C32 = 0.;
  C40 = C41 = C42 = C43 = 0.;
  C50 = C51 = C52 = C53 = C54 = 0.;
  C60 = C61 = C62 = C63 = C64 = C65 = 0.;

  C00 = c00;
  C11 = c11;
  C22 = c22;
  C33 = c33;
  C44 = c44;
  C55 = c55;
  C66 = c66;

  chi2 = 0.;
  NDF  = -6.;
  nTimeMeasurements = 0.;
}

inline void L1TrackPar::InitVelocityRange(fscal minP)
{
  // initialise the velocity range with respect to the minimal momentum minP {GeV/c}

  fscal maxVi = sqrt(1. + (kProtonMass / minP) * (kProtonMass / minP)) * kClightNsInv;
  fscal minVi = kClightNsInv;
  fscal vmean = minVi + 0.4 * (maxVi - minVi);
  fscal dvi   = (maxVi - vmean) / 3.;
  vi          = vmean;
  C66         = dvi * dvi;
}

#endif
