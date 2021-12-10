/* Copyright (C) 2012-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Florian Uhlig */

/*
 * CbmTimeSlice.cxx
 *
 *  Created on: 17.07.2012
 *      Author: friese
 */
#include "CbmTimeSlice.h"

#include "CbmLink.h"        // for CbmLink
#include "CbmModuleList.h"  // for CbmModuleList

#include <Logger.h>  // for Logger, LOG

#include <TNamed.h>   // for TNamed

#include <cassert>  // for assert
#include <iomanip>  // for operator<<, setprecision
#include <sstream>  // for operator<<, basic_ostream, stringstream
#include <string>   // for char_traits
#include <utility>  // for pair

using std::fixed;
using std::setprecision;
using std::string;
using std::stringstream;


// -----   Constructor for flexible time-slice   ----------------------------
CbmTimeSlice::CbmTimeSlice(EType type)
  : TNamed()
  , fType(type)
  , fStartTime(-1.)
  , fLength(-1.)
  , fIsEmpty(true)
  , fNofData()
  , fTimeDataFirst(0.)
  , fTimeDataLast(0.)
  , fMatch()
{
}
// ---------------------------------------------------------------------------


// -----   Constructor for fixed-length time-slice   -------------------------
CbmTimeSlice::CbmTimeSlice(double start, double duration)
  : TNamed()
  , fType(kRegular)
  , fStartTime(start)
  , fLength(duration)
  , fIsEmpty(true)
  , fNofData()
  , fTimeDataFirst(0.)
  , fTimeDataLast(0.)
  , fMatch()
{
}
// ---------------------------------------------------------------------------


// -----   Destructor   ------------------------------------------------------
CbmTimeSlice::~CbmTimeSlice() {}
// ---------------------------------------------------------------------------


// -----   Add data with time stamp   ----------------------------------------
bool CbmTimeSlice::AddData(ECbmModuleId detector, double time)
{

  // Check for out of bounds
  if (fType == kRegular) {
    if (time < fStartTime || time > GetEndTime()) {
      LOG(error) << "Trying to add data at t = " << time << " ns to "
                 << "time slice [ " << fStartTime << ", " << GetEndTime() << " ] ns !";
      return false;
    }  //? out of bounds
  }    //? fixed-length

  // Update bookkeeping
  if (fIsEmpty) {
    fTimeDataFirst = time;
    fTimeDataLast  = time;
  }  //? time-slice empty
  else {
    if (time < fTimeDataFirst) fTimeDataFirst = time;
    if (time > fTimeDataLast) fTimeDataLast = time;
  }  //? time-slice not empty

  fNofData[detector]++;
  fIsEmpty = false;

  return true;
}
// ---------------------------------------------------------------------------


// -----   End time of time slice   ------------------------------------------
double CbmTimeSlice::GetEndTime() const
{
  double value = -1.;
  switch (fType) {
    case kRegular: value = fStartTime + fLength; break;
    case kFlexible: value = -1.; break;
    case kEvent: value = fStartTime; break;
    default: value = -1.; break;
  }
  return value;
}
// ---------------------------------------------------------------------------


// -----   Get data array size   ---------------------------------------------
int32_t CbmTimeSlice::GetNofData(ECbmModuleId detector) const
{
  auto it = fNofData.find(detector);
  if (it == fNofData.end()) return 0;
  return it->second;
}
// ---------------------------------------------------------------------------


// -----   Register data   ---------------------------------------------------
bool CbmTimeSlice::RegisterData(ECbmModuleId system, double time)
{

  // Check for out of bounds
  if (fType == kRegular) {
    if (time < fStartTime || time > GetEndTime()) {
      LOG(error) << "Trying to add data at t = " << time << " ns to "
                 << "time slice [ " << fStartTime << ", " << GetEndTime() << " ] ns !";
      return false;
    }  //? out of bounds
  }    //? fixed-length

  // Update bookkeeping
  if (fIsEmpty) {
    fTimeDataFirst = time;
    fTimeDataLast  = time;
  }  //? time-slice empty
  else {
    if (time < fTimeDataFirst) fTimeDataFirst = time;
    if (time > fTimeDataLast) fTimeDataLast = time;
  }  //? time-slice not empty
  fNofData[system]++;
  fIsEmpty = false;

  return true;
}
// ---------------------------------------------------------------------------


// -----   Register data   ---------------------------------------------------
bool CbmTimeSlice::RegisterData(ECbmModuleId system, double time, const CbmMatch& match)
{

  // Update match object
  CbmLink link;
  for (int32_t iLink = 0; iLink < match.GetNofLinks(); iLink++) {
    int32_t input = match.GetLink(iLink).GetFile();
    int32_t event = match.GetLink(iLink).GetEntry();
    double weight = match.GetLink(iLink).GetWeight();
    fMatch.AddLink(weight, -1, event, input);
  }

  return RegisterData(system, time);
}
// ---------------------------------------------------------------------------


// ----- Reset time slice   --------------------------------------------------
void CbmTimeSlice::Reset()
{
  assert(fType != kRegular);
  ResetCounters();
  fMatch.ClearLinks();
}
// ---------------------------------------------------------------------------


// ----- Reset time slice   --------------------------------------------------
void CbmTimeSlice::Reset(double start, double length)
{
  fStartTime = start;
  fLength    = length;
  ResetCounters();
  fMatch.ClearLinks();
}
// ---------------------------------------------------------------------------


// ----- Reset time slice bookkeeping   --------------------------------------
void CbmTimeSlice::ResetCounters()
{
  fNofData.clear();
  fIsEmpty       = true;
  fTimeDataFirst = 0.;
  fTimeDataLast  = 0.;
  fMatch.ClearLinks();
}
// ---------------------------------------------------------------------------


// -----   Status info   -----------------------------------------------------
string CbmTimeSlice::ToString() const
{
  stringstream ss;
  ss << "Time slice [";
  switch (fType) {
    case kRegular: ss << fixed << setprecision(0) << fStartTime << ", " << GetEndTime() << "] ns, data: "; break;
    case kFlexible: ss << "flexible], data: "; break;
    case kEvent: ss << "event], data: "; break;
    default: ss << "], data: "; break;
  }
  if (IsEmpty()) ss << "empty";
  else {
    ss << "[" << fixed << setprecision(3) << fTimeDataFirst << ", " << fTimeDataLast << "] ns, ";
    for (auto it = fNofData.begin(); it != fNofData.end(); it++) {
      if (it->second) ss << CbmModuleList::GetModuleNameCaps(it->first) << " " << it->second << " ";
    }  //# detectors
    ss << ", MC events " << fMatch.GetNofLinks();
  }  //? time-slice not empty
  return ss.str();
}
// ---------------------------------------------------------------------------


ClassImp(CbmTimeSlice)
