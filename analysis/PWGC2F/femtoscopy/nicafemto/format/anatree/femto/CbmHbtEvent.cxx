/*
 * CbmHbtEventReco.cxx
 *
 *  Created on: 21 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmHbtEvent.h"

#include "CbmHbtTrack.h"
#include "NicaCbmATEvent.h"

CbmHbtEvent::CbmHbtEvent() : NicaExpEvent("CbmHbtTrack") {}

Bool_t CbmHbtEvent::IsCompatible(const NicaEvent* non_buffered) const {
  if (non_buffered->InheritsFrom("NicaCbmATEvent")) return kTRUE;
  return kFALSE;
}

void CbmHbtEvent::ShallowCopyTracks(NicaEvent* event) {
  fTracks->Clear();
  fTotalTracksNo = event->GetTotalTrackNo();
  fTracks->ExpandCreateFast(fTotalTracksNo);
  for (int i = 0; i < fTotalTracksNo; i++) {
    NicaCbmATTrack* from = (NicaCbmATTrack*) event->GetTrack(i);
    CbmHbtTrack* to      = (CbmHbtTrack*) fTracks->UncheckedAt(i);
    to->CopyData(from);
    from->SetEvent(this);
    to->GetLink()->ClearLinks();
    to->GetLink()->SetLink(0, i);
  }
}

CbmHbtEvent::~CbmHbtEvent() {}
