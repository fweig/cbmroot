/*
 * NicaCbmDstMcEvent.cxx
 *
 *  Created on: 17 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmATMCEvent.h"

#include "AnalysisTree/EventHeader.hpp"
#include "AnalysisTree/Particle.hpp"
#include "CbmAnaTreeSource.h"
#include "NicaCbmATMCEventInterface.h"
#include "NicaCbmATMCTrack.h"

NicaCbmATMCEvent::NicaCbmATMCEvent() : NicaMCEvent("NicaCbmATMCTrack") {}

void NicaCbmATMCEvent::CreateSource() {
  fSource = new NicaCbmATMCEventInterface();
}

NicaCbmATMCEvent::NicaCbmATMCEvent(const NicaCbmATMCEvent& other)
  : NicaMCEvent(other) {}

void NicaCbmATMCEvent::Update() {
  NicaCbmATMCEventInterface* s =
    static_cast<NicaCbmATMCEventInterface*>(fSource);
  CbmAnaTreeMcSourceContainer* data =
    (CbmAnaTreeMcSourceContainer*) s->GetRawEventPointer();
  AnaTreeMcIds conf = data->GetFieldIds();
  fTotalTracksNo    = data->GetParticles()->GetNumberOfChannels();
  fB                = data->GetEventHeader()->GetField<Float_t>(conf.event_b);
  fPhi              = data->GetEventHeader()->GetField<Float_t>(conf.event_psi);
  fTracks->Clear();
  fTracks->ExpandCreateFast(fTotalTracksNo);

  for (int i = 0; i < fTotalTracksNo; i++) {
    NicaCbmATMCTrack* track = (NicaCbmATMCTrack*) fTracks->UncheckedAt(i);
    track->SetEvent(this);
    AnalysisTree::Particle particle = data->GetParticles()->GetChannel(i);
    Double_t px                     = particle.GetField<float>(conf.px);
    Double_t py                     = particle.GetField<float>(conf.py);
    Double_t pz                     = particle.GetField<float>(conf.pz);
    Double_t mass                   = particle.GetField<float>(conf.mass);
    Int_t mother_id                 = particle.GetField<int>(conf.motherId);
    Int_t pid                       = particle.GetField<int>(conf.pdg);
    track->GetMomentum()->SetXYZM(px, py, pz, mass);
    track->SetPdg(pid);
    track->SetMotherIndex(mother_id);
    track->GetLink()->ClearLinks();
    track->GetLink()->SetLink(0, i);
    if (mother_id < 0) {
      track->SetPrimary();
    } else {
      track->SetMotherIndex(track->GetMotherIndex());
    }
    track->SetCharge(CalculateCharge(pid));
  }
}

Bool_t NicaCbmATMCEvent::ExistInTree() const {
  return CheckBranches(1, "CbmAnaTreeMcSourceContainer.");
}

NicaCbmATMCEvent::~NicaCbmATMCEvent() {}
