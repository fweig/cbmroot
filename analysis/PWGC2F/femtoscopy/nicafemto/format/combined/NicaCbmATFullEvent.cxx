/*
 * NicaCbmATEvent.cxx
 *
 *  Created on: 17 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "NicaCbmATFullEvent.h"

#include "AnalysisTree/Constants.hpp"
#include "NicaCbmATEvent.h"
#include "NicaCbmATEventInterface.h"
#include "NicaCbmATMCEvent.h"
#include "NicaComplexTrack.h"


NicaCbmATFullEvent::NicaCbmATFullEvent()
  : NicaComplexEvent(new NicaCbmATEvent(), new NicaCbmATMCEvent()) {}

void NicaCbmATFullEvent::Update() {
  fImgEvent->Update();
  fRealEvent->Update();
  NicaEvent::ShallowCopyEvent(fRealEvent);
  fTracks->Clear();
  fTotalTracksNo = fRealEvent->GetTotalTrackNo();
  fTracks->ExpandCreateFast(fTotalTracksNo);
  NicaCbmATEventInterface* s = (NicaCbmATEventInterface*) fSource;
  CbmAnaTreeRecoSourceContainer* reco =
    (CbmAnaTreeRecoSourceContainer*) s->GetRawEventPointer();
  for (Int_t i = 0; i < fTotalTracksNo; i++) {
    NicaComplexTrack* track = (NicaComplexTrack*) fTracks->UncheckedAt(i);
    track->SetRealTrack(fRealEvent->GetTrack(i));
    track->NicaTrack::CopyData(fRealEvent->GetTrack(i));
    Int_t match = reco->GetVtx2Sim()->GetMatchDirect(i);

    if (match < 0 || match == AnalysisTree::UndefValueInt) {
      track->SetImgTrack(nullptr);
    } else {
      track->SetImgTrack(fImgEvent->GetTrack(match));
      track->SetMatchID(match);
    }
    track->SetEvent(this);
  }
}

NicaEvent* NicaCbmATFullEvent::GetNewEvent() const {
  return new NicaCbmATFullEvent();
}

NicaCbmATFullEvent::~NicaCbmATFullEvent() {}
