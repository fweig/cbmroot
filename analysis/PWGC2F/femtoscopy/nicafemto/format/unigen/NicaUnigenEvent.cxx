/*
 * NicaUnigen.cxx
 *
 *  Created on: 23-06-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "NicaUnigenEvent.h"

#include "NicaEventInterface.h"
#include "NicaLink.h"
#include "NicaMCTrack.h"
#include "NicaTrack.h"
#include "NicaUnigenEventInterface.h"

#include <FairRootManager.h>
#include <TClonesArray.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TParticlePDG.h>

NicaUnigenEvent::NicaUnigenEvent() : NicaMCEvent("NicaUnigenTrack") {}

void NicaUnigenEvent::Update() {
  UEvent* temp   = ((NicaUnigenEventInterface*) fSource)->fEvent;
  fB             = temp->GetB();
  fPhi           = temp->GetPhi();
  fTotalTracksNo = temp->GetNpa();
  fTracks->Clear();
  for (int i = 0; i < fTotalTracksNo; i++) {
    UParticle* particle    = temp->GetParticle(i);
    TParticlePDG* pdg_part = fPDG->GetParticle(particle->GetPdg());
    Double_t charge        = 0;
    if (pdg_part) { charge = pdg_part->Charge() / 3.0; }
    NicaMCTrack* target_track = (NicaMCTrack*) fTracks->ConstructedAt(i);

    target_track->SetID(i);
    target_track->SetCharge(charge);
    target_track->SetPdg(particle->GetPdg());
    target_track->GetLink()->ClearLinks();
    target_track->GetLink()->SetLink(0, i);
    if (particle->GetParent() < 0) {
      target_track->SetPrimary();
    } else {
      target_track->SetMotherIndex(particle->GetParent());
    }
    target_track->GetMomentum()->SetPxPyPzE(
      particle->Px(), particle->Py(), particle->Pz(), particle->E());
    target_track->GetFreezoutPosition()->SetXYZT(
      particle->X(), particle->Y(), particle->Z(), particle->T());
    target_track->SetStatus(particle->GetStatus());
    target_track->SetEvent(this);
  }
}

void NicaUnigenEvent::Clear(Option_t* opt) {
  NicaMCEvent::Clear(opt);
  if (fSource) fSource->Clear(opt);
}

NicaUnigenEvent::NicaUnigenEvent(const NicaUnigenEvent& other)
  : NicaMCEvent(other) {}

void NicaUnigenEvent::CreateSource() {
  fSource = new NicaUnigenEventInterface();
}

NicaUnigenEvent::~NicaUnigenEvent() {}

TString NicaUnigenEvent::GetFormatName() const { return "UnigenFormat"; }

Bool_t NicaUnigenEvent::ExistInTree() const {
  FairRootManager* manager = FairRootManager::Instance();
  if (manager->CheckBranch("UEvent.")) { return kTRUE; }
  return kFALSE;
}
