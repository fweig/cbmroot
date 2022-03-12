/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#include "EventBuilder.h"

#include <cassert>

using std::is_sorted;
using std::vector;

namespace cbm
{
  namespace algo
  {
    // --- Execution
    std::vector<CbmDigiEvent> EventBuilder::operator()(const CbmDigiTimeslice& ts, const vector<double> triggers) const
    {
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

      // --- Loop over systems
      for (auto entry : fEventWindows) {

        auto system       = entry.first;
        const double tMin = trigger + entry.second.first;
        const double tMax = trigger + entry.second.second;

        // --- Build the event using trigger window
        switch (system) {
          case ECbmModuleId::kSts: {
            event.fData.fSts.fDigis = CopyRange(ts.fData.fSts.fDigis, tMin, tMax);
            break;
          }
          case ECbmModuleId::kRich: {
            event.fData.fRich.fDigis = CopyRange(ts.fData.fRich.fDigis, tMin, tMax);
            break;
          }
          case ECbmModuleId::kMuch: {
            event.fData.fMuch.fDigis = CopyRange(ts.fData.fMuch.fDigis, tMin, tMax);
            break;
          }
          case ECbmModuleId::kTrd: {
            event.fData.fTrd.fDigis = CopyRange(ts.fData.fTrd.fDigis, tMin, tMax);
            break;
          }
          case ECbmModuleId::kTof: {
            event.fData.fTof.fDigis = CopyRange(ts.fData.fTof.fDigis, tMin, tMax);
            break;
          }
          case ECbmModuleId::kPsd: {
            event.fData.fPsd.fDigis = CopyRange(ts.fData.fPsd.fDigis, tMin, tMax);
            break;
          }
          case ECbmModuleId::kT0: {
            event.fData.fT0.fDigis = CopyRange(ts.fData.fT0.fDigis, tMin, tMax);
            break;
          }
          default: break;
        }
      }
      return event;
    }
  }  // namespace algo
}  // namespace cbm
