/*
 * CbmTofCut.cxx
 *
 *  Created on: 21 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmTofCut.h"

#include "NicaCbmATTrack.h"

CbmTofCut::CbmTofCut() : NicaTrackToFMass2Cut(2) {
  SetUnitName("Flag [AU]", 1);
}

Bool_t CbmTofCut::Init(Int_t task_id) {
  if (FormatInhertis("NicaCbmATEvent", task_id, ENicaFormatDepth::kNonBuffered))
    return kTRUE;
  return kFALSE;
}

Bool_t CbmTofCut::Pass(NicaTrack* track) {
  NicaCbmATTrack* reco = (NicaCbmATTrack*) track;
  NicaToFTrack* tof    = reco->GetTofTrack();
  SetValue(tof->GetFlag(), Flag());
  SetValue(tof->GetMass2(), M2());
  Double_t m2 = tof->GetMass2();
  Double_t p  = reco->GetMomentum()->P();
  SetValue(m2, 0);
  SetValue(tof->GetFlag(), 1);
  Double_t min = fLowLine[0] + fLowLine[1] * p + fLowLine[2] * p * p;
  Double_t max = fHighLine[0] + fHighLine[1] * p + fHighLine[2] * p * p;
  if (m2 > max || m2 < min) { return ForcedUpdate(kFALSE); }
  return Validate();
}

CbmTofCut::~CbmTofCut() {}
