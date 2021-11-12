/* Copyright (C) 2011-2021 UGiessen/JINR-LIT, Giessen/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer], Andrey Lebedev */

/**
 * \file CbmLitReconstructionQa.cxx
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 **/

#include "elid/CbmLitGlobalElectronId.h"

#include "CbmGlobalTrack.h"
#include "CbmRichElectronIdAnn.h"
#include "CbmRichRing.h"
#include "CbmTofHit.h"
#include "CbmTrdTrack.h"
#include "utils/CbmRichUtil.h"

#include "FairRootManager.h"

#include "TClonesArray.h"
#include "TMath.h"
#include "TString.h"
#include "TSystem.h"

#include <iostream>

#include <cmath>

CbmLitGlobalElectronId::CbmLitGlobalElectronId()
  : fTrdAnnCut(0.6)
  , fRichAnnCut(-0.4)
  , fRichUseAnn(true)
  , fRichMeanA(-1.)
  , fRichMeanB(-1.)
  , fRichRmsA(-1.)
  , fRichRmsB(-1.)
  , fRichRmsCoeff(-1.)
  , fRichDistCut(-1.)
  , fGlobalTracks(NULL)
  , fRichRings(NULL)
  , fTrdTracks(NULL)
  , fTofHits(NULL)
{
  Init();
}

CbmLitGlobalElectronId::~CbmLitGlobalElectronId() {}

void CbmLitGlobalElectronId::Init()
{
  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman != nullptr) {
    fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
    fRichRings    = (TClonesArray*) ioman->GetObject("RichRing");
    fTrdTracks    = (TClonesArray*) ioman->GetObject("TrdTrack");
    fTofHits      = (TClonesArray*) ioman->GetObject("TofHit");
  }
}

Bool_t CbmLitGlobalElectronId::IsRichElectron(Int_t globalTrackIndex, Double_t momentum)
{
  if (fRichUseAnn == false) {
    if (NULL == fGlobalTracks || NULL == fRichRings) return false;
    const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(globalTrackIndex));
    Int_t richId                      = globalTrack->GetRichRingIndex();
    if (richId < 0) return false;
    CbmRichRing* ring = static_cast<CbmRichRing*>(fRichRings->At(richId));
    if (NULL == ring) return false;
    Double_t axisA = ring->GetAaxis();
    Double_t axisB = ring->GetBaxis();
    Double_t dist  = CbmRichUtil::GetRingTrackDistance(globalTrackIndex);
    if (fabs(axisA - fRichMeanA) < fRichRmsCoeff * fRichRmsA && fabs(axisB - fRichMeanB) < fRichRmsCoeff * fRichRmsB
        && dist < fRichDistCut) {
      return true;
    }
    else {
      return false;
    }
  }
  else {
    Double_t ann = CbmRichElectronIdAnn::GetInstance().CalculateAnnValue(globalTrackIndex, momentum);
    if (ann > fRichAnnCut) return true;
    else
      return false;
  }
}

Bool_t CbmLitGlobalElectronId::IsTrdElectron(Int_t globalTrackIndex, Double_t momentum)
{
  if (NULL == fGlobalTracks || NULL == fTrdTracks) return false;
  const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(globalTrackIndex));
  Int_t trdId                       = globalTrack->GetTrdTrackIndex();
  if (trdId < 0) return false;
  CbmTrdTrack* trdTrack = static_cast<CbmTrdTrack*>(fTrdTracks->At(trdId));
  if (NULL == trdTrack) return false;

  Double_t ann = trdTrack->GetPidLikeEL();
  if (ann > fTrdAnnCut) return true;
  else
    return false;
}

Bool_t CbmLitGlobalElectronId::IsTofElectron(Int_t globalTrackIndex, Double_t momentum, Double_t eventTime)
{
  Double_t mass2 = GetTofM2(globalTrackIndex, momentum, eventTime);
  if (mass2 == -1.) return false;

  if (momentum >= 1.) {
    if (mass2 < (0.013 * momentum - 0.003)) { return true; }
  }
  else {
    if (mass2 < 0.01) {
      return true;  //fTofM2
    }
  }
  return false;
}

Double_t CbmLitGlobalElectronId::GetRichAnn(Int_t globalTrackIndex, Double_t momentum)
{
  return CbmRichElectronIdAnn::GetInstance().CalculateAnnValue(globalTrackIndex, momentum);
}

Double_t CbmLitGlobalElectronId::GetTrdAnn(Int_t globalTrackIndex, Double_t momentum)
{
  if (NULL == fGlobalTracks || NULL == fTrdTracks) return -1.;
  const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(globalTrackIndex));
  Int_t trdId                       = globalTrack->GetTrdTrackIndex();
  if (trdId < 0) return -1.;
  CbmTrdTrack* trdTrack = static_cast<CbmTrdTrack*>(fTrdTracks->At(trdId));
  if (NULL == trdTrack) return -1.;

  return trdTrack->GetPidLikeEL();
}

Double_t CbmLitGlobalElectronId::GetTofM2(Int_t globalTrackIndex, Double_t momentum, Double_t eventTime)
{
  if (NULL == fGlobalTracks || NULL == fTofHits) return -1.;
  const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(globalTrackIndex));
  Double_t trackLength              = globalTrack->GetLength() / 100.;
  Int_t tofId                       = globalTrack->GetTofHitIndex();
  if (tofId < 0) return -1.;
  CbmTofHit* tofHit = (CbmTofHit*) fTofHits->At(tofId);
  if (NULL == tofHit) return -1.;

  Double_t noOffsetTime = tofHit->GetTime() - eventTime;
  Double_t time         = 0.2998 * noOffsetTime;  // time in ns -> transfrom to ct in m
  Double_t mass2        = momentum * momentum * (TMath::Power(time / trackLength, 2) - 1);

  return mass2;
}

ClassImp(CbmLitGlobalElectronId);
