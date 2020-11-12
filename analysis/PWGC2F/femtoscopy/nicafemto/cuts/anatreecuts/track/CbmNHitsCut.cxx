/*
 * CbmNHitsCut.cxx
 *
 *  Created on: 22 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmNHitsCut.h"

#include "NicaCbmATTrack.h"

CbmNHitsCut::CbmNHitsCut() : NicaTrackExpCut(1) { SetUnitName("N_{hits}"); }

Bool_t CbmNHitsCut::Init(Int_t task_id) {
  return FormatInhertis("NicaCbmATEvent", task_id);
}

Bool_t CbmNHitsCut::Pass(NicaTrack* track) {
  NicaCbmATTrack* tr = (NicaCbmATTrack*) track;
  SetValue(tr->GetNHits());
  return Validate();
}

CbmNHitsCut::~CbmNHitsCut() {}
