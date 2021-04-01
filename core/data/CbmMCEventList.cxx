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
#include <iostream>   // for operator<<, basic_ostream, stringstream
#include <string>     // for char_traits

#include <string.h>  // for strcmp, size_t

using std::lower_bound;
using std::string;
using std::stringstream;
using std::vector;

// -----   Constructor   ------------------------------------------------------
CbmMCEventList::CbmMCEventList()
  : TNamed("MCEventList", "List of MC events"), fEvents(), fIsSorted(kFALSE) {}
// ----------------------------------------------------------------------------


// -----   Destructor   -------------------------------------------------------
CbmMCEventList::~CbmMCEventList() {}
// ----------------------------------------------------------------------------


// -----   Check double occurrences of events   -------------------------------
Bool_t CbmMCEventList::Check() {

  Int_t lastFile    = -1;
  Int_t lastEvent   = -1;
  Double_t lastTime = 0.;
  Int_t thisFile    = -1;
  Int_t thisEvent   = -1;
  Double_t thisTime = 0.;

  for (auto& eventInfo : fEvents) {
    thisFile  = eventInfo.GetFileId();
    thisEvent = eventInfo.GetEventId();
    thisTime  = eventInfo.GetTime();
    if (thisFile == lastFile && thisEvent == lastEvent) {
      LOG(error) << fName << ": double entry for event " << thisEvent
                 << ", file " << thisFile << ", first time " << lastTime
                 << ", second time " << thisTime;
      return kFALSE;
    }
    lastFile  = thisFile;
    lastEvent = thisEvent;
    lastTime  = thisTime;
  }
  return kTRUE;
}
// ----------------------------------------------------------------------------


// -----   Find an event in the list   ----------------------------------------
vector<CbmMCEventInfo>::iterator CbmMCEventList::Find(UInt_t file,
                                                      UInt_t event) {
  if (!fIsSorted) Sort();
  auto it = lower_bound(
    fEvents.begin(), fEvents.end(), CbmMCEventInfo(file, event, -1.));
  if (it->GetFileId() != Int_t(file)) return fEvents.end();
  if (it->GetEventId() != Int_t(event)) return fEvents.end();
  return (it);
}
// ----------------------------------------------------------------------------


// -----   Get event number for event at index in list   ----------------------
Int_t CbmMCEventList::GetEventIdByIndex(UInt_t index) {
  if (!fIsSorted) Sort();
  if (index >= GetNofEvents()) return -1;
  return fEvents[index].GetEventId();
}
// ----------------------------------------------------------------------------


// -----   Get event time of a MC event   -------------------------------------
Double_t CbmMCEventList::GetEventTime(UInt_t eventId, UInt_t fileId) {
  if (!fIsSorted) Sort();
  auto it = Find(fileId, eventId);
  if (it == fEvents.end()) return -1.;
  return it->GetTime();
}
// ----------------------------------------------------------------------------


// -----   Get event time for event at index in list   ------------------------
Double_t CbmMCEventList::GetEventTimeByIndex(UInt_t index) {
  if (!fIsSorted) Sort();
  if (index >= GetNofEvents()) return -1.;
  auto info = fEvents[index];
  return fEvents[index].GetTime();
}
// ----------------------------------------------------------------------------


// -----   Get file number for event at index in list   -----------------------
Int_t CbmMCEventList::GetFileIdByIndex(UInt_t index) {
  if (!fIsSorted) Sort();
  if (index >= GetNofEvents()) return -1;
  auto info = fEvents[index];
  return fEvents[index].GetFileId();
}
// ----------------------------------------------------------------------------


// -----   Insert an event   --------------------------------------------------
Bool_t CbmMCEventList::Insert(UInt_t event, UInt_t file, Double_t time) {
  if (time < 0.) return kFALSE;
  fEvents.push_back(CbmMCEventInfo(file, event, time));
  fIsSorted = kFALSE;
  return kTRUE;
}
// ----------------------------------------------------------------------------


// -----   Print to screen   --------------------------------------------------
void CbmMCEventList::Print(Option_t* opt) const {
  std::cout << ToString(opt) << std::endl;
}
// ----------------------------------------------------------------------------


// -----   Sort events   ------------------------------------------------------
void CbmMCEventList::Sort() {
  if (fIsSorted) return;
  std::sort(fEvents.begin(), fEvents.end());
  assert(Check());
  fIsSorted = kTRUE;
}
// ----------------------------------------------------------------------------


// -----   Status info   ------------------------------------------------------
string CbmMCEventList::ToString(const char* option) const {
  stringstream ss;
  ss << fName << ": " << GetNofEvents() << " MC events in list\n";
  if (!strcmp(option, "long"))
    for (std::size_t index = 0; index < GetNofEvents(); index++)
      ss << fEvents[index].ToString() << "\n";
  return ss.str();
}
// ----------------------------------------------------------------------------


ClassImp(CbmMCEventList)
