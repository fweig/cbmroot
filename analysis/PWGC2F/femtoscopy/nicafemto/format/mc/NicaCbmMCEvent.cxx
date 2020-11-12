/*
 * NicaCbmMCEvent.cxx
 *
 *  Created on: 20 wrz 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmMCEvent.h"
#include "CbmMCTrack.h"
#include "FairLogger.h"
#include "FairRootManager.h"
#include "NicaCbmMCEventInterface.h"
#include "NicaCbmMCTrack.h"

NicaCbmMCEvent::NicaCbmMCEvent() : NicaMCEvent("NicaCbmMCTrack") {
  fSource = new NicaCbmMCEventInterface();
}

NicaCbmMCEvent::~NicaCbmMCEvent() {}

void NicaCbmMCEvent::CreateSource() { fSource = new NicaCbmMCEventInterface(); }

void NicaCbmMCEvent::Update() {
  FairMCEventHeader* event =
    (FairMCEventHeader*) ((NicaCbmMCEventInterface*) fSource)->fEvent;
  TClonesArray* tracks =
    (TClonesArray*) ((NicaCbmMCEventInterface*) fSource)->fCbmTracks;
  fB = event->GetB();
  fVertex->SetXYZT(event->GetX(), event->GetY(), event->GetZ(), event->GetT());
  fTracks->Clear();
  fTotalTracksNo = tracks->GetEntriesFast();
  fTracks->ExpandCreateFast(fTotalTracksNo);
  for (int i = 0; i < tracks->GetEntriesFast(); i++) {
    CbmMCTrack* track  = (CbmMCTrack*) tracks->UncheckedAt(i);
    Double_t charge    = CalculateCharge(track->GetPdgCode());
    NicaCbmMCTrack* mc = (NicaCbmMCTrack*) fTracks->UncheckedAt(i);
    mc->SetEvent(this);
    mc->GetLink()->ClearLinks();
    mc->Update(track, charge);
    mc->GetLink()->SetLink(0, i);
  }
}

NicaCbmMCEvent::NicaCbmMCEvent(const NicaCbmMCEvent& other)
  : NicaMCEvent(other) {}

Bool_t NicaCbmMCEvent::ExistInTree() const {
  FairRootManager* manager = FairRootManager::Instance();
  Int_t header             = manager->CheckBranch("MCEventHeader.")
                 + manager->CheckBranch("EventHeader.");
  LOG(debug) << "Branch MCEventHeader. "
             << manager->CheckBranch("MCEventHeader.");
  LOG(debug) << "Branch EventHeader. " << manager->CheckBranch("EventHeader.");
  LOG(debug) << "Branch MCTrack " << manager->CheckBranch("MCTrack");
  Int_t tracks = manager->CheckBranch("MCTrack");
  if ((header + tracks) >= 2) { return kTRUE; }
  return kFALSE;
}
