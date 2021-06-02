/** @file TTrbHeader.cxx
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#include "TTrbHeader.h"

#include <TNamed.h>  // for TNamed

#include <algorithm>  // for fill

TTrbHeader::TTrbHeader()
  : TNamed()
  , fuTriggerPattern(0)
  , fuTriggerType(0)
  , fdTimeInSpill(0.)
  , fdTimeInRun(0.)
  , fiSpillIndex(-1)
  , fuEventDAQDate(0)
  , fuEventDAQTime(0)
  , fiTriggerIndex(-1)
  , fdCTSBusyTime(0.)
  , fdCTSIdleTime(0.)
  , fuSubeventSizes()
{
}

void TTrbHeader::Clear(Option_t*)
{
  fuTriggerPattern = 0;
  fuTriggerType    = 0;
  fdTimeInSpill    = 0.;
  fdTimeInRun      = 0.;
  fiSpillIndex     = -1;
  fuEventDAQDate   = 0;
  fuEventDAQTime   = 0;
  fiTriggerIndex   = -1;
  fdCTSBusyTime    = 0.;
  fdCTSIdleTime    = 0.;
  std::fill(fuSubeventSizes.begin(), fuSubeventSizes.end(), 0);
}

void TTrbHeader::SetSubeventSizes(const std::vector<UShort_t>& vuVal) { fuSubeventSizes = vuVal; }

UShort_t TTrbHeader::GetSubeventSize(Int_t iSubevent) const
{
  // throws an out_of_range exception if iSubevent is out of bounds
  // should not happen if properly used
  return fuSubeventSizes.at(iSubevent);
}

Int_t TTrbHeader::GetNSubevents() const { return fuSubeventSizes.size(); }

Bool_t TTrbHeader::TriggerFired(Int_t iTrg)
{
  // check whether Trigger Pattern matches iTrg in any bit
  if (fuTriggerPattern & (0x1 << iTrg)) { return kTRUE; }
  else {
    return kFALSE;
  }
  return kFALSE;
}

ClassImp(TTrbHeader)
