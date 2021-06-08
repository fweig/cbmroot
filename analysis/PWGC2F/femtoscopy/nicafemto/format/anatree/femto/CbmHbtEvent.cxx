/* Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

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

#include "NicaCbmEvent.h"

CbmHbtEvent::CbmHbtEvent() : NicaExpEvent("CbmHbtTrack") {}

Bool_t CbmHbtEvent::IsCompatible(const NicaEvent* non_buffered) const
{
  if (non_buffered->InheritsFrom("NicaCbmEvent")) return kTRUE;
  return kFALSE;
}

void CbmHbtEvent::ShallowCopyTracks(NicaEvent* event)
{
  fTracks->Clear();
  fTotalTracksNo = event->GetTotalTrackNo();
  fTracks->ExpandCreateFast(fTotalTracksNo);
  for (int i = 0; i < fTotalTracksNo; i++) {
    NicaCbmTrack* from = (NicaCbmTrack*) event->GetTrack(i);
    CbmHbtTrack* to    = (CbmHbtTrack*) fTracks->UncheckedAt(i);
    to->ResetTrack(i, this);
    to->CopyData(from);
  }
}

CbmHbtEvent::~CbmHbtEvent() {}
