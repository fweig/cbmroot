/*
 * CbmStsFarDetectorHit.cxx
 *
 *  Created on: 31 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CbmStsFarDetectorHit.h"

#include "CbmGlobalTrack.h"

CbmStsFarDetectorHit::CbmStsFarDetectorHit() : NicaTrackCut(1) {
  SetUnitName("FarHit [N]");
  SetMinMax(0, 10);
}

Bool_t CbmStsFarDetectorHit::Pass(NicaTrack* track) {
  CbmGlobalTrack* tr = (CbmGlobalTrack*) track;
  Int_t n            = 0;
  if (tr->GetTrdTrackIndex() >= 0) n++;
  if (tr->GetTofHitIndex() >= 0) n++;
  SetValue(n);
  return Validate();
}

CbmStsFarDetectorHit::~CbmStsFarDetectorHit() {}
