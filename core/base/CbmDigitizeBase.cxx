/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmDigitizeBase.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 01.06.2018
 **/
#include "CbmDigitizeBase.h"

#include <FairEventHeader.h>  // for FairEventHeader
#include <FairRootManager.h>  // for FairRootManager
#include <FairRunAna.h>       // for FairRunAna
#include <FairTask.h>         // for FairTask

#include <TGenericClassInfo.h>  // for TGenericClassInfo

#include <cassert>  // for assert
#include <cstdio>

// -----   Default constructor   --------------------------------------------
CbmDigitizeBase::CbmDigitizeBase()
  : FairTask("Digitizer")
  , fEventMode(kFALSE)
  , fProduceNoise(kTRUE)
  , fCreateMatches(kTRUE)
  , fCurrentInput(-1)
  , fCurrentEvent(-1)
  , fCurrentMCEntry(-1)
  , fCurrentEventTime(0.)
{
}
// --------------------------------------------------------------------------


// -----   Default constructor   --------------------------------------------
CbmDigitizeBase::CbmDigitizeBase(const char* name)
  : FairTask(name)
  , fEventMode(kFALSE)
  , fProduceNoise(kTRUE)
  , fCreateMatches(kTRUE)
  , fCurrentInput(-1)
  , fCurrentEvent(-1)
  , fCurrentMCEntry(-1)
  , fCurrentEventTime(0.)
{
}
// --------------------------------------------------------------------------


// -----   Destructor   -----------------------------------------------------
CbmDigitizeBase::~CbmDigitizeBase() {}
// --------------------------------------------------------------------------


// -----   Get event information   ------------------------------------------
void CbmDigitizeBase::GetEventInfo()
{

  // --- The event number is taken from the FairRootManager
  fCurrentEvent = FairRootManager::Instance()->GetEntryNr();

  if (FairRunAna::Instance()) {
    FairEventHeader* event = FairRunAna::Instance()->GetEventHeader();
    assert(event);
    fCurrentInput     = event->GetInputFileId();
    fCurrentMCEntry   = event->GetMCEntryNumber();
    fCurrentEventTime = event->GetEventTime();
  }       //? FairRunAna
  else {  // no FairRunAna
    fCurrentInput     = -1;
    fCurrentMCEntry   = -1;
    fCurrentEventTime = 0.;
  }  //? not FairRunAna
}
// --------------------------------------------------------------------------


// -----   Read list of inactive channels   ---------------------------------
std::pair<size_t, bool> CbmDigitizeBase::ReadInactiveChannels()
{

  if (fInactiveChannelFileName.IsNull()) return std::make_pair(0, true);

  FILE* channelFile = fopen(fInactiveChannelFileName.Data(), "r");
  if (channelFile == nullptr) return std::make_pair(0, false);

  size_t nLines    = 0;
  uint32_t channel = 0;
  while (fscanf(channelFile, "%u", &channel) == 1) {
    fInactiveChannels.insert(channel);
    nLines++;
  }
  bool success = feof(channelFile);

  fclose(channelFile);
  return std::make_pair(nLines, success);
}
// --------------------------------------------------------------------------


ClassImp(CbmDigitizeBase)
