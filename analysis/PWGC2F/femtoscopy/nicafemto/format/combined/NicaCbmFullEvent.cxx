/** @file NicaCbmFullEvent.cxx
  * @copyright Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Daniel Wielanek [committer] **/

/*
 * NicaCbmATEvent.cxx
 *
 *  Created on: 17 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "NicaCbmFullEvent.h"

#include "AnalysisTree/Constants.hpp"
#include "NicaCbmEvent.h"
#include "NicaCbmEventInterface.h"
#include "NicaCbmMCEvent.h"
#include "NicaComplexTrack.h"


NicaCbmFullEvent::NicaCbmFullEvent() : NicaComplexEvent(new NicaCbmEvent(), new NicaCbmMCEvent()) {}

void NicaCbmFullEvent::Update()
{
  fImgEvent->Update();
  fRealEvent->Update();
  NicaEvent::ShallowCopyEvent(fRealEvent);
  fTracks->Clear();
  fTotalTracksNo = fRealEvent->GetTotalTrackNo();
  fTracks->ExpandCreateFast(fTotalTracksNo);
  NicaCbmEventInterface* s            = (NicaCbmEventInterface*) fSource;
  CbmAnaTreeRecoSourceContainer* reco = (CbmAnaTreeRecoSourceContainer*) s->GetRawEventPointer();
  for (Int_t i = 0; i < fTotalTracksNo; i++) {
    NicaComplexTrack* track = (NicaComplexTrack*) fTracks->UncheckedAt(i);
    track->ResetTrack(i, this);
    track->SetRealTrack(fRealEvent->GetTrack(i));
    track->NicaTrack::CopyData(fRealEvent->GetTrack(i));
    Int_t match = reco->GetVtx2Sim()->GetMatchDirect(i);

    if (match < 0 || match == AnalysisTree::UndefValueInt) { track->SetImgTrack(nullptr); }
    else {
      track->SetImgTrack(fImgEvent->GetTrack(match));
      track->SetMatchID(match);
    }
  }
}

NicaEvent* NicaCbmFullEvent::GetNewEvent() const { return new NicaCbmFullEvent(); }

NicaCbmFullEvent::~NicaCbmFullEvent() {}

NicaCbmFullEvent::NicaCbmFullEvent(NicaEvent* re, NicaEvent* im) : NicaComplexEvent(re, im) {}
