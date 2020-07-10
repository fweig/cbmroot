/*
 * NicaCbmMCEventInterface.cxx
 *
 *  Created on: 20 wrz 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmMCEventInterface.h"
#include "CbmMCTrack.h"
#include "FairRootManager.h"
#include "NicaCbmMCTrackInterface.h"

NicaCbmMCEventInterface::NicaCbmMCEventInterface() {
  fEvent     = new FairMCEventHeader();
  fCbmTracks = new TClonesArray("CbmMCTrack");
}

void NicaCbmMCEventInterface::Register(Bool_t write) {
  if (fEvent == NULL) {
    fEvent     = new FairMCEventHeader();
    fCbmTracks = new TClonesArray("CbmMCTrack", 1000);
  }
  FairRootManager* manager = FairRootManager::Instance();
  TString event_name       = "MCEventHeader.";
  TString track_name       = "MCTrack";
  manager->Register(event_name, "MCEventHeader", fEvent, write);
  manager->Register(track_name, "CbmMCTrack", fCbmTracks, write);
}

void NicaCbmMCEventInterface::Clear(Option_t* opt) { fCbmTracks->Clear(opt); }

void NicaCbmMCEventInterface::Compress(Int_t* map, Int_t map_size) {
  Int_t track_pos = 0;
  for (int i = 0; i < map_size; i++) {
    Int_t good_track = map[i];
    for (int j = track_pos; j < good_track; j++) {
      fCbmTracks->RemoveAt(j);
    }
    track_pos = good_track + 1;
  }
  fCbmTracks->Compress();
}

void NicaCbmMCEventInterface::CopyData(NicaEventInterface* s) {
  FairMCEventHeader* header =
    (FairMCEventHeader*) ((NicaCbmMCEventInterface*) s)->fEvent;
  TClonesArray* tracks =
    (TClonesArray*) ((NicaCbmMCEventInterface*) s)->fCbmTracks;
  fEvent->SetRunID(header->GetRunID());
  fEvent->SetEventID(header->GetEventID());
  fEvent->SetVertex(header->GetX(), header->GetY(), header->GetZ());
  fEvent->SetRotX(header->GetRotX());
  fEvent->SetRotY(header->GetRotY());
  fEvent->SetRotZ(header->GetRotZ());
  fEvent->SetB(header->GetB());
  fEvent->SetNPrim(header->GetNPrim());
  fCbmTracks->Clear();
  fCbmTracks->ExpandCreateFast(tracks->GetEntriesFast());
  for (int i = 0; i < fCbmTracks->GetEntriesFast(); i++) {
    CbmMCTrack* from =
      (CbmMCTrack*) ((NicaCbmMCEventInterface*) s)->fCbmTracks->UncheckedAt(i);
    CbmMCTrack* to = (CbmMCTrack*) fCbmTracks->UncheckedAt(i);
    *to            = *from;
  }
}

void NicaCbmMCEventInterface::CopyAndCompress(NicaEventInterface* s,
                                              Int_t* map,
                                              Int_t map_size) {
  FairMCEventHeader* header =
    (FairMCEventHeader*) ((NicaCbmMCEventInterface*) s)->fEvent;
  fEvent->SetRunID(header->GetRunID());
  fEvent->SetEventID(header->GetEventID());
  fEvent->SetVertex(header->GetX(), header->GetY(), header->GetZ());
  fEvent->SetRotX(header->GetRotX());
  fEvent->SetRotY(header->GetRotY());
  fEvent->SetRotZ(header->GetRotZ());
  fEvent->SetB(header->GetB());
  fEvent->SetNPrim(header->GetNPrim());
  fCbmTracks->Clear();
  for (int i = 0; i < map_size; i++) {
    CbmMCTrack* from = (CbmMCTrack*) ((NicaCbmMCEventInterface*) s)
                         ->fCbmTracks->UncheckedAt(map[i]);
    CbmMCTrack* to = (CbmMCTrack*) fCbmTracks->ConstructedAt(i);
    *to            = *from;
  }
}

void NicaCbmMCEventInterface::FillTrackInterface(NicaTrackInterface* track,
                                                 Int_t index) {
  track->SetRawTrack(fCbmTracks->UncheckedAt(index));
}

NicaTrackInterface* NicaCbmMCEventInterface::GetTrackInterface() const {
  return new NicaCbmMCTrackInterface();
}

TLorentzVector NicaCbmMCEventInterface::GetVertex() const {
  TLorentzVector vec;
  TVector3 vec3d;
  fEvent->GetVertex(vec3d);
  vec.SetXYZT(vec3d.X(), vec3d.Y(), vec3d.Z(), 0);
  return vec;
}

NicaCbmMCEventInterface::~NicaCbmMCEventInterface() {
  if (CanDeleteEvent()) {
    if (fEvent) delete fEvent;
    if (fCbmTracks) delete fCbmTracks;
  }
}

void NicaCbmMCEventInterface::ConnectToTree() {
  FairRootManager* manager = FairRootManager::Instance();
  if (CanDeleteEvent()) {
    if (fEvent) delete fEvent;
    if (fCbmTracks) delete fCbmTracks;
  }
  fEvent = (FairMCEventHeader*) manager->GetObject("MCEventHeader.");
  if (fEvent == NULL)
    fEvent = (FairMCEventHeader*) manager->GetObject("EventHeader.");
  fCbmTracks = (TClonesArray*) manager->GetObject("MCTrack");
}
