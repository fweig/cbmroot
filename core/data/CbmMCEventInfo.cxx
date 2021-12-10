/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmMCEventInfo.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 16.06.2018
 **/
#include "CbmMCEventInfo.h"

#include <sstream>  // for operator<<, basic_ostream, stringstream
#include <string>   // for char_traits

using std::string;
using std::stringstream;

// -----   Constructor   ------------------------------------------------------
CbmMCEventInfo::CbmMCEventInfo(int32_t fileId, int32_t eventId, double time)
  : fFileId(fileId)
  , fEventId(eventId)
  , fTime(time)
{
}
// ----------------------------------------------------------------------------


// -----   Destructor   -------------------------------------------------------
CbmMCEventInfo::~CbmMCEventInfo() {}
// ----------------------------------------------------------------------------


// -----   Status info   ------------------------------------------------------
string CbmMCEventInfo::ToString() const
{
  stringstream ss;
  ss << "File " << fFileId << " Event " << fEventId << " Time " << fTime;
  return ss.str();
}
// ----------------------------------------------------------------------------


ClassImp(CbmMCEventInfo)
