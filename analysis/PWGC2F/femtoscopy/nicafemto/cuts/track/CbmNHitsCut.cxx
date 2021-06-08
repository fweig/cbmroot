/* Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Daniel Wielanek [committer] */

/*
 * CbmNHitsCut.cxx
 *
 *  Created on: 22 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmNHitsCut.h"

#include "NicaCbmTrack.h"

CbmNHitsCut::CbmNHitsCut() : NicaTrackExpCut(1) { SetUnitName("N_{hits}"); }

Bool_t CbmNHitsCut::Init(Int_t task_id) { return FormatInhertis("NicaCbmEvent", task_id); }

Bool_t CbmNHitsCut::Pass(NicaTrack* track)
{
  NicaCbmTrack* tr = (NicaCbmTrack*) track;
  SetValue(tr->GetNHits());
  return Validate();
}

CbmNHitsCut::~CbmNHitsCut() {}
