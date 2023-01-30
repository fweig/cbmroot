/* Copyright (C) 2015-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmMCEventList.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 24.11.2015
 **/
#include "CbmMCEventList.h"

#include <Logger.h>  // for Logger, LOG

#include <TNamed.h>   // for TNamed
#include <TString.h>  // for operator<<, TString

#include <algorithm>  // for lower_bound, sort
#include <cassert>    // for assert
#include <cstring>    // for strcmp, size_t
#include <iostream>   // for operator<<, basic_ostream, stringstream
#include <string>     // for char_traits

using std::lower_bound;
using std::string;
using std::stringstream;
using std::vector;

// -----   Constructor   ------------------------------------------------------
CbmMCEventList::CbmMCEventList() : TNamed("MCEventList", "List of MC events"), fEvents(), fIsSorted(false) {}
// ----------------------------------------------------------------------------


// -----   Destructor   -------------------------------------------------------
CbmMCEventList::~CbmMCEventList() {}
// ----------------------------------------------------------------------------


// -----   Check double occurrences of events   -------------------------------
bool CbmMCEventList::Check()
{

  int32_t lastFile  = -1;
  int32_t lastEvent = -1;
  double lastTime   = 0.;
  int32_t thisFile  = -1;
  int32_t thisEvent = -1;
  double thisTime   = 0.;

  for (auto& eventInfo : fEvents) {
    thisFile  = eventInfo.GetFileId();
    thisEvent = eventInfo.GetEventId();
    thisTime  = eventInfo.GetTime();
    if (thisFile == lastFile && thisEvent == lastEvent) {
      LOG(error) << fName << ": double entry for event " << thisEvent << ", file " << thisFile << ", first time "
                 << lastTime << ", second time " << thisTime;
      return false;
    }
    lastFile  = thisFile;
    lastEvent = thisEvent;
    lastTime  = thisTime;
  }
  return true;
}
// ----------------------------------------------------------------------------


// -----   Find an event in the list   ----------------------------------------
vector<CbmMCEventInfo>::iterator CbmMCEventList::Find(uint32_t file, uint32_t event)
{
  if (!fIsSorted) Sort();
  auto it = lower_bound(fEvents.begin(), fEvents.end(), CbmMCEventInfo(file, event, -1.));
  if (it->GetFileId() != int32_t(file)) return fEvents.end();
  if (it->GetEventId() != int32_t(event)) return fEvents.end();
  return (it);
}
// ----------------------------------------------------------------------------


// -----   Get event number for event at index in list   ----------------------
int32_t CbmMCEventList::GetEventIdByIndex(uint32_t index)
{
  if (!fIsSorted) Sort();
  if (index >= GetNofEvents()) return -1;
  return fEvents[index].GetEventId();
}
// ----------------------------------------------------------------------------


// -----   Get event time of a MC event   -------------------------------------
double CbmMCEventList::GetEventTime(uint32_t eventId, uint32_t fileId)
{
  if (!fIsSorted) Sort();
  auto it = Find(fileId, eventId);
  if (it == fEvents.end()) return -1.;
  return it->GetTime();
}
// ----------------------------------------------------------------------------


// -----   Get event index in the list   -------------------------------------
Int_t CbmMCEventList::GetEventIndex(UInt_t eventId, UInt_t fileId)
{
  if (!fIsSorted) Sort();
  auto it = Find(fileId, eventId);
  if (it == fEvents.end()) return -1.;
  return (it - fEvents.begin());
}
// ----------------------------------------------------------------------------


// -----   Get event time for event at index in list   ------------------------
double CbmMCEventList::GetEventTimeByIndex(uint32_t index)
{
  if (!fIsSorted) Sort();
  if (index >= GetNofEvents()) return -1.;
  auto info = fEvents[index];
  return fEvents[index].GetTime();
}
// ----------------------------------------------------------------------------


// -----   Get file number for event at index in list   -----------------------
int32_t CbmMCEventList::GetFileIdByIndex(uint32_t index)
{
  if (!fIsSorted) Sort();
  if (index >= GetNofEvents()) return -1;
  auto info = fEvents[index];
  return fEvents[index].GetFileId();
}
// ----------------------------------------------------------------------------

// -----   Get file number for event at index in list   -----------------------
CbmLink CbmMCEventList::GetEventLinkByIndex(uint32_t index)
{
  if (!fIsSorted) Sort();
  if (index >= GetNofEvents()) return CbmLink();
  return fEvents[index].GetCbmLink();
}
// ----------------------------------------------------------------------------


// -----   Insert an event   --------------------------------------------------
bool CbmMCEventList::Insert(uint32_t event, uint32_t file, double time)
{
  if (time < 0.) return false;
  fEvents.push_back(CbmMCEventInfo(file, event, time));
  fIsSorted = false;
  return true;
}
// ----------------------------------------------------------------------------


// -----   Print to screen   --------------------------------------------------
void CbmMCEventList::Print(Option_t* opt) const { std::cout << ToString(opt) << std::endl; }
// ----------------------------------------------------------------------------


// -----   Sort events   ------------------------------------------------------
void CbmMCEventList::Sort()
{
  if (fIsSorted) return;
  std::sort(fEvents.begin(), fEvents.end());
  assert(Check());
  fIsSorted = true;
}
// ----------------------------------------------------------------------------


// -----   Status info   ------------------------------------------------------
string CbmMCEventList::ToString(const char* option) const
{
  stringstream ss;
  ss << fName << ": " << GetNofEvents() << " MC events in list\n";
  if (!strcmp(option, "long"))
    for (std::size_t index = 0; index < GetNofEvents(); index++)
      ss << fEvents[index].ToString() << "\n";
  return ss.str();
}
// ----------------------------------------------------------------------------


ClassImp(CbmMCEventList)
