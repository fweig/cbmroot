/* Copyright (C) 2018-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * NicaCbmEvent.cxx
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmEvent.h"

#include <TVector3.h>

#include <iostream>

#include "AnalysisTree/Constants.hpp"
#include "AnalysisTree/Matching.hpp"
#include "NicaCbmEventInterface.h"
#include "NicaCbmTrack.h"
#include "NicaDataFormat.h"
#include "NicaEvent.h"

NicaCbmEvent::NicaCbmEvent() : NicaExpEvent("NicaCbmTrack") {}

void NicaCbmEvent::CreateSource() { fSource = new NicaCbmEventInterface(); }

NicaCbmEvent::NicaCbmEvent(const NicaCbmEvent& other) : NicaExpEvent(other)
{
  if (other.fSource) CreateSource();
}

void NicaCbmEvent::Update()
{
  fTracks->Clear();
  NicaCbmEventInterface* interface         = (NicaCbmEventInterface*) fSource;
  CbmAnaTreeRecoSourceContainer* container = (CbmAnaTreeRecoSourceContainer*) interface->GetRawEventPointer();
  fTotalTracksNo                           = interface->GetTotalTrackNo();
  if (fTotalTracksNo > 1000) { fTotalTracksNo = 0; }
  TLorentzVector start = interface->GetVertex();
  fVertex->SetXYZT(start.X(), start.Y(), start.Z(), start.T());
  fTracks->ExpandCreateFast(fTotalTracksNo);
  AnaTreeRecoIds ids = container->GetFieldIds();
  for (int i = 0; i < fTotalTracksNo; i++) {
    AnalysisTree::Track p          = container->GetVtxTracks()->GetChannel(i);
    AnalysisTree::ShortInt_t match = container->GetVtx2ToFMatch()->GetMatchDirect(i);
    NicaCbmTrack* track            = (NicaCbmTrack*) fTracks->UncheckedAt(i);
    track->ResetTrack(i, this);
    track->SetChi2(p.GetField<float>(ids.vtx_chi2));
    track->SetVertexChi2(p.GetField<float>(ids.vtx_vtxchi2));
    track->GetMomentum()->SetPxPyPzE(p.GetPx(), p.GetPy(), p.GetPz(), 0);
    track->GetDCA()->SetXYZ(p.GetField<float>(ids.vtx_dcax), p.GetField<float>(ids.vtx_dcay),
                            p.GetField<float>(ids.vtx_dcaz));
    track->SetNHits(p.GetField<int>(ids.vtx_nhits));
    track->SetCharge(p.GetField<int>(ids.vtx_q));
    track->SetNMvdHits(p.GetField<int>(ids.vtx_mvdhits));
    TVector3 mom(p.GetPx(), p.GetPy(), p.GetPz());
    TVector3 pos(track->GetDCA()->X(), track->GetDCA()->Y(), track->GetDCA()->Z());
    track->GetHelix()->Build(pos, mom, track->GetCharge());
    NicaToFTrack* tof = (NicaToFTrack*) track->GetDetTrack(NicaDetectorID::kTOF);
    if (match == AnalysisTree::UndefValueShort) {
      //no tof
      tof->SetMass2(NicaToFTrack::DummyVal());
      tof->SetBeta(NicaToFTrack::DummyVal());
      tof->SetFlag(0);
    }
    else {
      AnalysisTree::Hit tof_hit = container->GetTofHits()->GetChannel(match);
      Double_t P                = track->GetMomentum()->P();
      Double_t m2               = tof_hit.GetField<float>(ids.tof_mass2);
      Double_t E                = TMath::Sqrt(mom * mom + m2);
      Double_t beta             = P / E;
      tof->SetMass2(m2);
      tof->SetBeta(beta);
      tof->SetFlag(1);
    }
  }
}

NicaCbmEvent::NicaCbmEvent(TString classname) : NicaExpEvent(classname) {}

NicaCbmEvent::~NicaCbmEvent() {}

Bool_t NicaCbmEvent::ExistInTree() const { return CheckBranches(1, "CbmAnaTreeSourceContainer."); }
