/* Copyright (C) 2015-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva [committer], Semen Lebedev */

#ifndef LMVM_CAND_H
#define LMVM_CAND_H

#include "TVector3.h"

#include "LmvmDef.h"

class LmvmCand {
public:
  LmvmCand() {}

  void ResetMcParams()
  {
    fMcSrc         = ELmvmSrc::Undefined;
    fMcMotherId    = -1;
    fStsMcTrackId  = -1;
    fRichMcTrackId = -1;
    fTrdMcTrackId  = -1;
    fTofMcTrackId  = -1;
  }

  void SetIsTopologyCutElectron(ELmvmTopologyCut cut, bool value)
  {
    if (cut == ELmvmTopologyCut::TT) { fIsTtCut = value; }
    if (cut == ELmvmTopologyCut::ST) { fIsStCut = value; }
    if (cut == ELmvmTopologyCut::RT) { fIsRtCut = value; }
  }

  bool IsCutTill(ELmvmAnaStep step) const
  {
    if (step == ELmvmAnaStep::Mc || step == ELmvmAnaStep::Acc || step == ELmvmAnaStep::Reco) return true;
    if (step == ELmvmAnaStep::Chi2Prim && fIsChi2Prim) return true;
    if (step == ELmvmAnaStep::ElId && IsCutTill(ELmvmAnaStep::Chi2Prim) && fIsElectron) return true;
    if (step == ELmvmAnaStep::GammaCut && IsCutTill(ELmvmAnaStep::ElId) && fIsGammaCut) return true;
    if (step == ELmvmAnaStep::Mvd1Cut && IsCutTill(ELmvmAnaStep::GammaCut) && fIsMvd1Cut) return true;
    if (step == ELmvmAnaStep::Mvd2Cut && IsCutTill(ELmvmAnaStep::Mvd1Cut) && fIsMvd2Cut) return true;
    if (step == ELmvmAnaStep::StCut && IsCutTill(ELmvmAnaStep::Mvd2Cut) && fIsStCut) return true;
    if (step == ELmvmAnaStep::RtCut && IsCutTill(ELmvmAnaStep::StCut) && fIsRtCut) return true;
    if (step == ELmvmAnaStep::TtCut && IsCutTill(ELmvmAnaStep::RtCut) && fIsTtCut) return true;
    if (step == ELmvmAnaStep::PtCut && IsCutTill(ELmvmAnaStep::TtCut) && fIsPtCut) return true;
    return false;
  }

  // track parameters
  TVector3 fPosition;
  TVector3 fMomentum;
  double fMass     = 0.;
  double fEnergy   = 0.;
  double fRapidity = 0.;
  int fCharge      = 0;
  double fChi2Prim = 0.;
  double fChi2sts  = 0.;

  int fMcMotherId    = -1;
  int fEventNumber   = 0;
  int fStsMcTrackId  = -1;
  int fRichMcTrackId = -1;
  int fTrdMcTrackId  = -1;
  int fTofMcTrackId  = -1;
  int fStsInd        = -1;
  int fRichInd       = -1;
  int fTrdInd        = -1;
  int fTofInd        = -1;
  int fMcPdg         = -1;
  double fRichAnn    = 0.;
  double fTrdAnn     = 0.;
  double fMass2      = 0.;
  double fTrdLikeEl  = -1.;
  double fTrdLikePi  = -1.;

  // Cuts. If true then cut is passed
  bool fIsChi2Prim = false;
  bool fIsElectron = false;
  bool fIsGammaCut = true;  // Will be set to 'false' as soon as a partner with minv < 25 MeV is found
  bool fIsMvd1Cut  = false;
  bool fIsMvd2Cut  = false;
  bool fIsTtCut    = false;
  bool fIsStCut    = false;
  bool fIsRtCut    = false;
  bool fIsPtCut    = false;

  // MC
  ELmvmSrc fMcSrc = ELmvmSrc::Undefined;
  bool IsMcSignal() const { return fMcSrc == ELmvmSrc::Signal; }
  bool IsMcPi0() const { return fMcSrc == ELmvmSrc::Pi0; }
  bool IsMcGamma() const { return fMcSrc == ELmvmSrc::Gamma; }
  bool IsMcEta() const { return fMcSrc == ELmvmSrc::Eta; }
  // for candidates BG is all candidates which are not signal
  bool fIsMcBg() const { return fMcSrc != ELmvmSrc::Signal; }
};

#endif
