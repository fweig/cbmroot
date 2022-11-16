/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Denis Bertini [committer], Andrey Lebedev, Florian Uhlig */

/**
 ** \file CbmTofTrack.h
 ** \author V.Friese <v.friese@gsi.de>
 ** \since 26.01.05
 ** \date 07.09.15
 ** Updated 28/11/2021 by V.Akishina <v.akishina@gsi.de>
 **/
#include "CbmTofTrack.h"

#include <FairTrackParam.h>  // for FairTrackParam

#include <sstream>  // for operator<<, basic_ostream, stringstream

using std::stringstream;


// -----   Constructor   ---------------------------------------------------
CbmTofTrack::CbmTofTrack() : CbmTrack() {}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmTofTrack::~CbmTofTrack() {}
// -------------------------------------------------------------------------


// -----   Debug   ---------------------------------------------------------
std::string CbmTofTrack::ToString() const
{
  stringstream ss;
  ss << "CbmTofTrack: time " << fTime << " ns | hits STS " << GetNofTofHits() << " | q/p " << GetParamFirst()->GetQp()
     << " | chisq " << GetChiSq() << " | NDF " << GetNDF() << " | STS hits ";
  for (int32_t iHit = 0; iHit < GetNofTofHits(); iHit++) {
    ss << GetTofHitIndex(iHit) << " ";
  }
  return ss.str();
}
// -------------------------------------------------------------------------


ClassImp(CbmTofTrack)
