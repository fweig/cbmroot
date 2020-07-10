/*
 * NicaCbmEvent.cxx
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmGlobalEvent.h"
#include "NicaCbmGlobalEventInterface.h"
#include "NicaCbmGlobalTrack.h"
#include "NicaEvent.h"

NicaCbmGlobalEvent::NicaCbmGlobalEvent() : NicaExpEvent("NicaCbmGlobalTrack") {}

void NicaCbmGlobalEvent::ShallowCopyTracks(NicaEvent* event) {
  fTracks->Clear();
  fTotalTracksNo = event->GetTotalTrackNo();
  fTracks->ExpandCreateFast(fTotalTracksNo);
  NicaCbmGlobalEvent* Event = (NicaCbmGlobalEvent*) event;
  for (int i = 0; i < fTotalTracksNo; i++) {
    NicaCbmGlobalTrack* from =
      (NicaCbmGlobalTrack*) Event->fTracks->UncheckedAt(i);
    NicaCbmGlobalTrack* to = (NicaCbmGlobalTrack*) fTracks->UncheckedAt(i);
    NicaEvent* temp_event  = from->GetEvent();
    from->SetEvent(this);
    to->CopyData(from);
    from->SetEvent(temp_event);
    to->GetLink()->SetLink(0, i);
  }
}

void NicaCbmGlobalEvent::ShallowCopyEvent(NicaEvent* event) {
  NicaExpEvent::ShallowCopyEvent(event);
}

void NicaCbmGlobalEvent::CreateSource() {
  fSource = new NicaCbmGlobalEventInterface();
}

NicaCbmGlobalEvent::NicaCbmGlobalEvent(const NicaCbmGlobalEvent& other)
  : NicaExpEvent(other) {
  if (other.fSource) CreateSource();
}

void NicaCbmGlobalEvent::Update() {
  fTracks->Clear();
  NicaCbmGlobalEventInterface* interface =
    (NicaCbmGlobalEventInterface*) fSource;
  fTotalTracksNo       = interface->GetTotalTrackNo();
  TLorentzVector start = interface->GetVertex();
  fVertex->SetXYZT(start.X(), start.Y(), start.Z(), start.T());
  fTracks->ExpandCreateFast(fTotalTracksNo);
  for (int i = 0; i < fTotalTracksNo; i++) {
    CbmGlobalTrack* tr_cbm = (CbmGlobalTrack*) interface->GetRawTrackPointer(i);
    NicaCbmGlobalTrack* tr_nica = (NicaCbmGlobalTrack*) fTracks->UncheckedAt(i);
    tr_nica->SetEvent(this);
    tr_nica->Update(tr_cbm, interface);
    tr_nica->SetEvent(this);
    tr_nica->GetLink()->SetLink(0, i);
  }
}

NicaCbmGlobalEvent::NicaCbmGlobalEvent(TString classname)
  : NicaExpEvent(classname) {}

NicaCbmGlobalEvent::~NicaCbmGlobalEvent() {
  // TODO Auto-generated destructor stub
}

Bool_t NicaCbmGlobalEvent::ExistInTree() const {
  return CheckBranches(2, "PrimaryVertex.", "GlobalTrack");
}
