/** @file NicaCbmMCEvent.cxx
  * @copyright Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Daniel Wielanek [committer] **/

/*
 * NicaCbmDstMcEvent.cxx
 *
 *  Created on: 17 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "NicaCbmMCEvent.h"

#include "CbmAnaTreeSource.h"

#include "AnalysisTree/EventHeader.hpp"
#include "AnalysisTree/Particle.hpp"
#include "NicaCbmMCEventInterface.h"
#include "NicaCbmMCTrack.h"

NicaCbmMCEvent::NicaCbmMCEvent() : NicaMCEvent("NicaCbmMCTrack") {}

void NicaCbmMCEvent::CreateSource() { fSource = new NicaCbmMCEventInterface(); }

NicaCbmMCEvent::NicaCbmMCEvent(const NicaCbmMCEvent& other) : NicaMCEvent(other) {}

void NicaCbmMCEvent::Update()
{
  NicaCbmMCEventInterface* s        = static_cast<NicaCbmMCEventInterface*>(fSource);
  CbmAnaTreeMcSourceContainer* data = (CbmAnaTreeMcSourceContainer*) s->GetRawEventPointer();
  AnaTreeMcIds conf                 = data->GetFieldIds();
  fTotalTracksNo                    = data->GetParticles()->GetNumberOfChannels();
  fB                                = data->GetEventHeader()->GetField<Float_t>(conf.event_b);
  fPhi                              = data->GetEventHeader()->GetField<Float_t>(conf.event_psi);
  fTracks->Clear();
  fTracks->ExpandCreateFast(fTotalTracksNo);
  Bool_t UseFreez = kTRUE;
  if (conf.freezX == AnalysisTree::UndefValueShort) UseFreez = kFALSE;


  for (int i = 0; i < fTotalTracksNo; i++) {
    NicaCbmMCTrack* track = (NicaCbmMCTrack*) fTracks->UncheckedAt(i);
    track->ResetTrack(i, this);
    AnalysisTree::Particle particle = data->GetParticles()->GetChannel(i);
    Double_t px                     = particle.GetField<float>(conf.px);
    Double_t py                     = particle.GetField<float>(conf.py);
    Double_t pz                     = particle.GetField<float>(conf.pz);
    Double_t mass                   = particle.GetField<float>(conf.mass);

    Int_t mother_id = particle.GetField<int>(conf.motherId);
    Int_t pid       = particle.GetField<int>(conf.pdg);
    track->GetMomentum()->SetXYZM(px, py, pz, mass);
    track->SetPdg(pid);
    if (UseFreez) {
      Double_t x = particle.GetField<float>(conf.freezX);
      Double_t y = particle.GetField<float>(conf.freezY);
      Double_t z = particle.GetField<float>(conf.freezZ);
      Double_t t = particle.GetField<float>(conf.freezT);
      track->GetFreezoutPosition()->SetXYZT(x, y, z, t);
    }

    track->SetMotherIndex(mother_id);
    if (mother_id < 0) { track->SetPrimary(); }
    else {
      track->SetMotherIndex(track->GetMotherIndex());
    }
    track->SetCharge(CalculateCharge(pid));
  }
}

Bool_t NicaCbmMCEvent::ExistInTree() const { return CheckBranches(1, "CbmAnaTreeMcSourceContainer."); }

NicaCbmMCEvent::~NicaCbmMCEvent() {}
