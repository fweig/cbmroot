/** @file CbmTofM2PolyCut.cxx
  * @copyright Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Daniel Wielanek [committer] **/

/*
 * CbmTofM2PolyCut.cxx
 *
 *  Created on: 21 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmTofM2PolyCut.h"

#include "CbmDetectorID.h"

#include "NicaExpTrack.h"

CbmTofM2PolyCut::CbmTofM2PolyCut() {}

Bool_t CbmTofM2PolyCut::Pass(NicaTrack* track)
{
  NicaToFTrack* tof = (NicaToFTrack*) static_cast<NicaExpTrack*>(track)->GetDetTrack(NicaCbmDetectorID::kTOF);
  if (tof->GetFlag() == 0) { SetValue(NicaToFTrack::DummyVal()); }
  SetValue(tof->GetMass2());
  return Validate();
}

CbmTofM2PolyCut::~CbmTofM2PolyCut() {}
