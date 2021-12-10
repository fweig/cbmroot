/* Copyright (C) 2009 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** CbmStsTrackMatch.cxx
 *@author V.Friese <v.friese@gsi.de>
 *@since 07.05.2009
 **/


#include "CbmTrackMatch.h"


// -----   Default constructor   -------------------------------------------
CbmTrackMatch::CbmTrackMatch() : fMCTrackId(-1), fNofTrueHits(0), fNofWrongHits(0), fNofFakeHits(0), fNofMCTracks(0) {}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmTrackMatch::CbmTrackMatch(int32_t mcTrackId, int32_t nTrue, int32_t nWrong, int32_t nFake, int32_t nTracks)
  : fMCTrackId(mcTrackId)
  , fNofTrueHits(nTrue)
  , fNofWrongHits(nWrong)
  , fNofFakeHits(nFake)
  , fNofMCTracks(nTracks)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmTrackMatch::~CbmTrackMatch() {}
// -------------------------------------------------------------------------


ClassImp(CbmTrackMatch)
