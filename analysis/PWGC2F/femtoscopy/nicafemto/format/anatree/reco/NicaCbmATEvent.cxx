/*
 * NicaCbmEvent.cxx
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmATEvent.h"

#include "AnalysisTree/Constants.hpp"
#include "AnalysisTree/Matching.hpp"
#include "NicaDetectorID.h"
#include "NicaEvent.h"
#include <TVector3.h>
#include <iostream>

#include "NicaCbmATEventInterface.h"
#include "NicaCbmATTrack.h"

NicaCbmATEvent::NicaCbmATEvent() : NicaExpEvent("NicaCbmATTrack") {}

void NicaCbmATEvent::ShallowCopyTracks(NicaEvent* event) {
  fTracks->Clear();
  fTotalTracksNo = event->GetTotalTrackNo();
  fTracks->ExpandCreateFast(fTotalTracksNo);
  NicaCbmATEvent* Event = (NicaCbmATEvent*) event;
  for (int i = 0; i < fTotalTracksNo; i++) {
    NicaCbmATTrack* from  = (NicaCbmATTrack*) Event->fTracks->UncheckedAt(i);
    NicaCbmATTrack* to    = (NicaCbmATTrack*) fTracks->UncheckedAt(i);
    NicaEvent* temp_event = from->GetEvent();
    from->SetEvent(this);
    to->CopyData(from);
    from->SetEvent(temp_event);
    to->GetLink()->ClearLinks();
    to->GetLink()->SetLink(0, i);
  }
}

void NicaCbmATEvent::ShallowCopyEvent(NicaEvent* event) {
  NicaExpEvent::ShallowCopyEvent(event);
}

void NicaCbmATEvent::CreateSource() { fSource = new NicaCbmATEventInterface(); }

NicaCbmATEvent::NicaCbmATEvent(const NicaCbmATEvent& other)
  : NicaExpEvent(other) {
  if (other.fSource) CreateSource();
}

void NicaCbmATEvent::Update() {
  fTracks->Clear();
  NicaCbmATEventInterface* interface = (NicaCbmATEventInterface*) fSource;
  CbmAnaTreeRecoSourceContainer* container =
    (CbmAnaTreeRecoSourceContainer*) interface->GetRawEventPointer();
  fTotalTracksNo = interface->GetTotalTrackNo();
  if (fTotalTracksNo > 1000) { fTotalTracksNo = 0; }
  TLorentzVector start = interface->GetVertex();
  fVertex->SetXYZT(start.X(), start.Y(), start.Z(), start.T());
  fTracks->ExpandCreateFast(fTotalTracksNo);
  AnaTreeRecoIds ids = container->GetFieldIds();
  for (int i = 0; i < fTotalTracksNo; i++) {
    AnalysisTree::Track p = container->GetVtxTracks()->GetChannel(i);
    AnalysisTree::ShortInt_t match =
      container->GetVtx2ToFMatch()->GetMatchDirect(i);
    NicaCbmATTrack* track = (NicaCbmATTrack*) fTracks->UncheckedAt(i);
    track->SetEvent(this);
    track->SetChi2(p.GetField<float>(ids.vtx_chi2));
    track->SetVertexChi2(p.GetField<float>(ids.vtx_vtxchi2));
    track->GetMomentum()->SetPxPyPzE(p.GetPx(), p.GetPy(), p.GetPz(), 0);
    track->GetDCA()->SetXYZ(p.GetField<float>(ids.vtx_dcax),
                            p.GetField<float>(ids.vtx_dcay),
                            p.GetField<float>(ids.vtx_dcaz));
    track->SetNHits(p.GetField<int>(ids.vtx_nhits));
    track->SetCharge(p.GetField<int>(ids.vtx_q));
    TVector3 mom(p.GetPx(), p.GetPy(), p.GetPz());
    TVector3 pos(
      track->GetDCA()->X(), track->GetDCA()->Y(), track->GetDCA()->Z());
    track->GetHelix()->Build(pos, mom, track->GetCharge());
    NicaToFTrack* tof =
      (NicaToFTrack*) track->GetDetTrack(NicaDetectorID::kTOF);
    if (match == AnalysisTree::UndefValueShort) {
      //no tof
      tof->SetMass2(NicaToFTrack::DummyVal());
      tof->SetBeta(NicaToFTrack::DummyVal());
      tof->SetFlag(0);
    } else {
      AnalysisTree::Hit tof_hit = container->GetTofHits()->GetChannel(match);
      Double_t P                = track->GetMomentum()->P();
      Double_t m2               = tof_hit.GetField<float>(ids.tof_mass2);
      Double_t E                = TMath::Sqrt(mom * mom + m2);
      Double_t beta             = P / E;
      tof->SetMass2(m2);
      tof->SetBeta(beta);
      tof->SetFlag(1);
    }
    track->GetLink()->SetLink(0, i);
  }
}

NicaCbmATEvent::NicaCbmATEvent(TString classname) : NicaExpEvent(classname) {}

NicaCbmATEvent::~NicaCbmATEvent() {}

Bool_t NicaCbmATEvent::ExistInTree() const {
  return CheckBranches(1, "CbmAnaTreeSourceContainer.");
}
