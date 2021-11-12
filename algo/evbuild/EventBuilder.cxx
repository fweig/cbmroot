/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#include "EventBuilder.h"

#include <algorithm>
#include <cassert>
#include <vector>

using std::is_sorted;
using std::vector;

namespace cbm
{
  namespace algo
  {

    // --- Execution
    std::vector<CbmDigiEvent> EventBuilder::operator()(const CbmDigiTimeslice& ts, const vector<double> triggers) const
    {
      assert(is_sorted(triggers.begin(), triggers.end()));
      vector<CbmDigiEvent> eventVec(triggers.size());
      std::transform(triggers.begin(), triggers.end(), eventVec.begin(),
                     [&ts, this](const double& trigger) { return BuildEvent(ts, trigger); });
      return eventVec;
    }


    // --- Build a single event
    CbmDigiEvent EventBuilder::BuildEvent(const CbmDigiTimeslice& ts, double trigger) const
    {
      CbmDigiEvent event;
      event.fTime = trigger;
      double tMin = trigger + fTriggerWindows.find(ECbmModuleId::kSts)->second.first;
      double tMax = trigger + fTriggerWindows.find(ECbmModuleId::kSts)->second.second;
      assert(is_sorted(ts.fData.fSts.fDigis.begin(), ts.fData.fSts.fDigis.end(), IsBefore<CbmStsDigi, CbmStsDigi>));
      event.fData.fSts.fDigis = CopyRange(ts.fData.fSts.fDigis, tMin, tMax);
      return event;
    }


  }  // namespace algo
}  // namespace cbm
