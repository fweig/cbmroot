/* Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * CbmDeltaPhiDeltaThetaCut.cxx
 *
 *  Created on: 22 cze 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CbmDeltaPhiDeltaThetaCut.h"

#include "CbmHbtTrack.h"
#include "CbmHelix.h"

#include <TLorentzVector.h>
#include <TVector3.h>

#include "NicaCbmTrack.h"
#include "NicaCut.h"
#include "NicaEvent.h"
#include "NicaPackage.h"
#include "NicaParameter.h"
#include "NicaStd.h"
#include "NicaTrack.h"
#include "NicaTwoTrack.h"

CbmDeltaPhiDeltaThetaStarCut::CbmDeltaPhiDeltaThetaStarCut()
  : NicaTwoTrackCut(2)
  , fR(0.0)
  , fDataType(NicaCbmFormats::ECbmFormatType::kUnknown)
{
  SetUnitName("#Delta#phi^{*} [rad]", 0);
  SetUnitName("#Delta#eta^{*} [rad]", 1);
}

Bool_t CbmDeltaPhiDeltaThetaStarCut::Init(Int_t task_id)
{
  fDataType = NicaCbmFormats::GetFormatType(task_id, ENicaFormatDepth::kBuffered);
  if (fDataType == NicaCbmFormats::ECbmFormatType::kUnknown) return kFALSE;
  return kTRUE;
}

Bool_t CbmDeltaPhiDeltaThetaStarCut::Pass(NicaTwoTrack* pair)
{
  TVector3 mom1, mom2;
  switch (fDataType) {
    case NicaCbmFormats::ECbmFormatType::kAnaTree: {
      NicaCbmTrack* tr1 = static_cast<NicaCbmTrack*>(pair->GetTrack1());
      NicaCbmTrack* tr2 = static_cast<NicaCbmTrack*>(pair->GetTrack2());
      CbmHelix* h1      = tr1->GetHelix();
      CbmHelix* h2      = tr2->GetHelix();
      h1->Eval(fR + tr1->GetEvent()->GetVertex()->Z(), mom1);
      h2->Eval(fR + tr2->GetEvent()->GetVertex()->Z(), mom2);
    } break;
    case NicaCbmFormats::ECbmFormatType::kHbt: {
      CbmHbtTrack* tr1 = static_cast<CbmHbtTrack*>(pair->GetTrack1());
      CbmHbtTrack* tr2 = static_cast<CbmHbtTrack*>(pair->GetTrack2());
      tr1->CalculateMomAt(fR);
      tr2->CalculateMomAt(fR);
      mom1 = *tr1->GetMomAtR();
      mom2 = *tr2->GetMomAtR();
    } break;
    case NicaCbmFormats::ECbmFormatType::kUnknown: {

    } break;
  }

  SetValue(mom1.Phi() - mom2.Phi(), 0);  //dleta phi
  SetValue(mom1.Theta() - mom2.Theta(), 1);
  if (GetValue(0) > GetMin(0) && GetValue(0) < GetMax(0) && GetValue(1) > GetMin(1) && GetValue(1) < GetMax(1)) {
    return ForcedUpdate(kFALSE);
  }
  return ForcedUpdate(kTRUE);
}

NicaPackage* CbmDeltaPhiDeltaThetaStarCut::Report() const
{
  NicaPackage* pack = NicaTwoTrackCut::Report();
  pack->AddObject(new NicaParameterDouble("R", fR));
  return pack;
}

CbmDeltaPhiDeltaThetaStarCut& CbmDeltaPhiDeltaThetaStarCut::operator=(const CbmDeltaPhiDeltaThetaStarCut& other)
{
  if (this == &other) return *this;
  fDataType = other.fDataType;
  return *this;
}

CbmDeltaPhiDeltaThetaStarCut::~CbmDeltaPhiDeltaThetaStarCut() {}

CbmDeltaPhiDeltaThetaCut::CbmDeltaPhiDeltaThetaCut() : NicaTwoTrackCut(2)
{
  SetUnitName("#Delta#phi [rad]", 0);
  SetUnitName("#Delta#eta [rad]", 1);
}

Bool_t CbmDeltaPhiDeltaThetaCut::Init(Int_t /*task_id*/) { return kTRUE; }

Bool_t CbmDeltaPhiDeltaThetaCut::Pass(NicaTwoTrack* pair)
{
  NicaTrack* tr1 = static_cast<NicaTrack*>(pair->GetTrack1());
  NicaTrack* tr2 = static_cast<NicaTrack*>(pair->GetTrack2());
  SetValue(tr1->GetMomentum()->Phi() - tr2->GetMomentum()->Phi(), 0);
  SetValue(tr1->GetMomentum()->Theta() - tr2->GetMomentum()->Theta(), 1);
  if (GetValue(0) > GetMin(0) && GetValue(0) < GetMax(0) && GetValue(1) > GetMin(1) && GetValue(1) < GetMax(1)) {
    return ForcedUpdate(kFALSE);
  }
  return ForcedUpdate(kTRUE);
}

CbmDeltaPhiDeltaThetaCut::~CbmDeltaPhiDeltaThetaCut() {}
