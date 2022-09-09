/* Copyright (C) 2010-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer]*/

#include "LmvmUtils.h"

#include "CbmKFVertex.h"
#include "CbmL1PFFitter.h"
#include "CbmMCTrack.h"
#include "CbmStsTrack.h"
#include "cbm/elid/CbmLitGlobalElectronId.h"

#include "TClonesArray.h"
#include "TDatabasePDG.h"
#include "TMCProcess.h"
#include "TRandom3.h"

#include <iostream>

#include "LmvmCand.h"
#include "LmvmDef.h"

using std::string;
using std::vector;

ClassImp(LmvmUtils);

void LmvmUtils::CalculateAndSetTrackParams(LmvmCand* cand, CbmStsTrack* stsTrack, CbmKFVertex& kfVertex)
{
  CbmL1PFFitter fPFFitter;
  vector<CbmStsTrack> stsTracks;
  stsTracks.resize(1);
  stsTracks[0] = *stsTrack;
  vector<CbmL1PFFitter::PFFieldRegion> vField;
  vector<float> chiPrim;
  fPFFitter.GetChiToVertex(stsTracks, vField, chiPrim, kfVertex, 3e6);
  cand->fChi2sts                 = stsTracks[0].GetChiSq() / stsTracks[0].GetNDF();
  cand->fChi2Prim                = chiPrim[0];
  const FairTrackParam* vtxTrack = stsTracks[0].GetParamFirst();

  vtxTrack->Position(cand->fPosition);
  vtxTrack->Momentum(cand->fMomentum);

  cand->fMass     = TDatabasePDG::Instance()->GetParticle(11)->Mass();
  cand->fCharge   = (vtxTrack->GetQp() > 0) ? 1 : -1;
  cand->fEnergy   = sqrt(cand->fMomentum.Mag2() + cand->fMass * cand->fMass);
  cand->fRapidity = 0.5 * TMath::Log((cand->fEnergy + cand->fMomentum.Z()) / (cand->fEnergy - cand->fMomentum.Z()));
}

void LmvmUtils::CalculateArmPodParams(LmvmCand* cand1, LmvmCand* cand2, double& alpha, double& ptt)
{
  alpha = ptt = 0.;
  double spx  = cand1->fMomentum.X() + cand2->fMomentum.X();
  double spy  = cand1->fMomentum.Y() + cand2->fMomentum.Y();
  double spz  = cand1->fMomentum.Z() + cand2->fMomentum.Z();
  double sp   = sqrt(spx * spx + spy * spy + spz * spz);

  if (sp == 0.0) return;
  double pn, /*pp,*/ pln, plp;
  if (cand1->fCharge < 0.) {
    pn = cand1->fMomentum.Mag();
    //pp = cand2->fMomentum.Mag();
    pln = (cand1->fMomentum.X() * spx + cand1->fMomentum.Y() * spy + cand1->fMomentum.Z() * spz) / sp;
    plp = (cand2->fMomentum.X() * spx + cand2->fMomentum.Y() * spy + cand2->fMomentum.Z() * spz) / sp;
  }
  else {
    pn = cand2->fMomentum.Mag();
    //pp = cand1->fMomentum.Mag();
    pln = (cand2->fMomentum.X() * spx + cand2->fMomentum.Y() * spy + cand2->fMomentum.Z() * spz) / sp;
    plp = (cand1->fMomentum.X() * spx + cand1->fMomentum.Y() * spy + cand1->fMomentum.Z() * spz) / sp;
  }
  if (pn == 0.0) return;
  double ptm = (1. - ((pln / pn) * (pln / pn)));
  ptt        = (ptm >= 0.) ? pn * sqrt(ptm) : 0;
  alpha      = (plp - pln) / (plp + pln);
}

ELmvmSrc LmvmUtils::GetMcSrc(CbmMCTrack* mctrack, TClonesArray* mcTracks)
{
  if (IsMcSignalEl(mctrack)) return ELmvmSrc::Signal;
  if (IsMcPi0El(mctrack, mcTracks)) return ELmvmSrc::Pi0;
  if (IsMcGammaEl(mctrack, mcTracks)) return ELmvmSrc::Gamma;
  if (IsMcEtaEl(mctrack, mcTracks)) return ELmvmSrc::Eta;
  return ELmvmSrc::Bg;  // all bg track wich are not pi0, gamma, eta electrons
}

bool LmvmUtils::IsMcSignalEl(const CbmMCTrack* mct)
{
  if (mct != nullptr && mct->GetGeantProcessId() == kPPrimary && std::abs(mct->GetPdgCode()) == 11) return true;
  return false;
}

bool LmvmUtils::IsMcGammaEl(const CbmMCTrack* mct, TClonesArray* mcTracks)
{
  if (mct == nullptr || std::abs(mct->GetPdgCode()) != 11 || mct->GetMotherId() < 0) return false;
  CbmMCTrack* mct1 = static_cast<CbmMCTrack*>(mcTracks->At(mct->GetMotherId()));
  if (mct1 != nullptr && mct1->GetPdgCode() == 22) return true;
  return false;
}

bool LmvmUtils::IsMcPi0El(const CbmMCTrack* mct, TClonesArray* mcTracks)
{
  if (mct == nullptr || std::abs(mct->GetPdgCode()) != 11 || mct->GetMotherId() < 0) return false;
  CbmMCTrack* mct1 = static_cast<CbmMCTrack*>(mcTracks->At(mct->GetMotherId()));
  if (mct1 != nullptr && mct1->GetPdgCode() == 111) return true;
  return false;
}

bool LmvmUtils::IsMcEtaEl(const CbmMCTrack* mct, TClonesArray* mcTracks)
{
  if (mct == nullptr || std::abs(mct->GetPdgCode()) != 11 || mct->GetMotherId() < 0) return false;
  CbmMCTrack* mct1 = static_cast<CbmMCTrack*>(mcTracks->At(mct->GetMotherId()));
  if (mct1 != NULL && mct1->GetPdgCode() == 221) return true;
  return false;
}

bool LmvmUtils::IsMcPairSignal(const CbmMCTrack* mctP, const CbmMCTrack* mctM)
{
  return (IsMcSignalEl(mctM) && IsMcSignalEl(mctP));
}

bool LmvmUtils::IsMcPairPi0(const CbmMCTrack* mctP, const CbmMCTrack* mctM, TClonesArray* mcTracks)
{
  return ((mctM->GetMotherId() == mctP->GetMotherId()) && IsMcPi0El(mctM, mcTracks) && IsMcPi0El(mctP, mcTracks));
}

bool LmvmUtils::IsMcPairEta(const CbmMCTrack* mctP, const CbmMCTrack* mctM, TClonesArray* mcTracks)
{
  return ((mctM->GetMotherId() == mctP->GetMotherId()) && IsMcEtaEl(mctM, mcTracks) && IsMcEtaEl(mctP, mcTracks));
}

bool LmvmUtils::IsMcPairGamma(const CbmMCTrack* mctP, const CbmMCTrack* mctM, TClonesArray* mcTracks)
{
  return ((mctM->GetMotherId() == mctP->GetMotherId()) && IsMcGammaEl(mctM, mcTracks) && IsMcGammaEl(mctP, mcTracks));
}

bool LmvmUtils::IsMcPairBg(const CbmMCTrack* mctP, const CbmMCTrack* mctM, TClonesArray* mcTracks)
{
  bool isGamma = IsMcPairGamma(mctP, mctM, mcTracks);
  bool isEta   = IsMcPairEta(mctP, mctM, mcTracks);
  bool isPi0   = IsMcPairPi0(mctP, mctM, mcTracks);
  return (!isEta) && (!isGamma) && (!isPi0) && (!(IsMcSignalEl(mctP) || IsMcSignalEl(mctM)));
}

ELmvmSrc LmvmUtils::GetMcPairSrc(const CbmMCTrack* mctP, const CbmMCTrack* mctM, TClonesArray* mcTracks)
{
  if (IsMcPairSignal(mctP, mctM)) return ELmvmSrc::Signal;
  if (IsMcPairGamma(mctP, mctM, mcTracks)) return ELmvmSrc::Gamma;
  if (IsMcPairPi0(mctP, mctM, mcTracks)) return ELmvmSrc::Pi0;
  if (IsMcPairEta(mctP, mctM, mcTracks)) return ELmvmSrc::Eta;
  if (IsMcPairBg(mctP, mctM, mcTracks)) return ELmvmSrc::Bg;

  return ELmvmSrc::Undefined;
}


bool LmvmUtils::IsMcPairSignal(const LmvmCand& candP, const LmvmCand& candM)
{
  return (candP.IsMcSignal() && candM.IsMcSignal());
}

bool LmvmUtils::IsMcPairPi0(const LmvmCand& candP, const LmvmCand& candM)
{
  return (candP.IsMcPi0() && candM.IsMcPi0() && candP.fMcMotherId == candM.fMcMotherId);
}

bool LmvmUtils::IsMcPairEta(const LmvmCand& candP, const LmvmCand& candM)
{
  return (candP.IsMcEta() && candM.IsMcEta() && candP.fMcMotherId == candM.fMcMotherId);
}

bool LmvmUtils::IsMcPairGamma(const LmvmCand& candP, const LmvmCand& candM)
{
  return (candP.IsMcGamma() && candM.IsMcGamma() && candP.fMcMotherId == candM.fMcMotherId);
}

bool LmvmUtils::IsMcPairBg(const LmvmCand& candP, const LmvmCand& candM)
{
  bool isGamma = IsMcPairGamma(candP, candM);
  bool isEta   = IsMcPairEta(candP, candM);
  bool isPi0   = IsMcPairPi0(candP, candM);
  return (!isEta) && (!isGamma) && (!isPi0) && (!(candP.IsMcSignal() || candM.IsMcSignal()));
}

ELmvmSrc LmvmUtils::GetMcPairSrc(const LmvmCand& candP, const LmvmCand& candM)
{
  if (IsMcPairSignal(candP, candM)) return ELmvmSrc::Signal;
  if (IsMcPairGamma(candP, candM)) return ELmvmSrc::Gamma;
  if (IsMcPairPi0(candP, candM)) return ELmvmSrc::Pi0;
  if (IsMcPairEta(candP, candM)) return ELmvmSrc::Eta;
  if (IsMcPairBg(candP, candM)) return ELmvmSrc::Bg;

  return ELmvmSrc::Undefined;
}

ELmvmBgPairSrc LmvmUtils::GetBgPairSrc(const LmvmCand& candP, const LmvmCand& candM)
{
  if (candM.IsMcGamma()) {
    if (candP.IsMcGamma() && candP.fMcMotherId != candM.fMcMotherId) return ELmvmBgPairSrc::GG;
    if (candP.IsMcPi0()) return ELmvmBgPairSrc::GP;
    return ELmvmBgPairSrc::GO;
  }
  else if (candM.IsMcPi0()) {
    if (candP.IsMcGamma()) return ELmvmBgPairSrc::GP;
    if (candP.IsMcPi0() && candP.fMcMotherId != candM.fMcMotherId) return ELmvmBgPairSrc::PP;
    return ELmvmBgPairSrc::PO;
  }
  else {
    if (candP.IsMcGamma()) return ELmvmBgPairSrc::GO;
    if (candP.IsMcPi0()) return ELmvmBgPairSrc::PO;
    return ELmvmBgPairSrc::OO;
  }

  return ELmvmBgPairSrc::Undefined;
}

bool LmvmUtils::IsMismatch(const LmvmCand& cand)
{
  if (cand.fStsMcTrackId == cand.fRichMcTrackId && cand.fStsMcTrackId == cand.fTrdMcTrackId
      && cand.fStsMcTrackId == cand.fTofMcTrackId && cand.fStsMcTrackId != -1)
    return false;
  return true;
}

bool LmvmUtils::IsGhost(const LmvmCand& cand)
{
  if (cand.fStsMcTrackId == -1 || cand.fRichMcTrackId == -1 || cand.fTrdMcTrackId == -1 || cand.fTofMcTrackId == -1)
    return true;
  return false;
}

double LmvmUtils::Distance(double x1, double y1, double x2, double y2) { return std::sqrt(Distance2(x1, y1, x2, y2)); }

double LmvmUtils::Distance2(double x1, double y1, double x2, double y2)
{
  return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

void LmvmUtils::IsElectron(int globalTrackIndex, double momentum, double momentumCut, LmvmCand* cand)
{
  bool richEl    = CbmLitGlobalElectronId::GetInstance().IsRichElectron(globalTrackIndex, momentum);
  cand->fRichAnn = CbmLitGlobalElectronId::GetInstance().GetRichAnn(globalTrackIndex, momentum);

  bool trdEl = (momentum < 1.) ? true : CbmLitGlobalElectronId::GetInstance().IsTrdElectron(globalTrackIndex, momentum);
  //bool trdEl     = CbmLitGlobalElectronId::GetInstance().IsTrdElectron(globalTrackIndex, momentum);

  //cand->fTrdAnn  = CbmLitGlobalElectronId::GetInstance().GetTrdAnn(globalTrackIndex, momentum); //TODO: uncomment this and delete next line when TrdAnn is working again
  cand->fTrdLikeEl = CbmLitGlobalElectronId::GetInstance().GetTrdAnn(globalTrackIndex, momentum);

  bool tofEl     = CbmLitGlobalElectronId::GetInstance().IsTofElectron(globalTrackIndex, momentum);
  cand->fMass2   = CbmLitGlobalElectronId::GetInstance().GetTofM2(globalTrackIndex, momentum);
  bool isMomCut  = (momentumCut > 0.) ? (momentum < momentumCut) : true;

  cand->fIsElectron = (richEl && trdEl && tofEl && isMomCut);
}

void LmvmUtils::IsElectronMc(LmvmCand* cand, TClonesArray* mcTracks, double pionMisidLevel)
{
  // Use MC information for PID to set a required level of pion suppression
  if (cand->fStsMcTrackId < 0 || cand->fStsMcTrackId >= mcTracks->GetEntriesFast()) { cand->fIsElectron = false; }
  else {
    CbmMCTrack* mcTrack = static_cast<CbmMCTrack*>(mcTracks->At(cand->fStsMcTrackId));
    if (std::abs(mcTrack->GetPdgCode()) == 11) { cand->fIsElectron = true; }
    else {
      cand->fIsElectron = (gRandom->Rndm() < pionMisidLevel);
    }
  }
}

string LmvmUtils::GetChargeStr(const LmvmCand* cand)
{
  if (cand->fCharge == 0) return "0";
  return (cand->fCharge > 0) ? "+" : "-";
}

string LmvmUtils::GetChargeStr(const CbmMCTrack* mct)
{
  if (mct->GetCharge() == 0) return "0";
  return (mct->GetCharge() > 0) ? "+" : "-";
}
