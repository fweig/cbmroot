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

      // --- Loop over systems
      for (const auto& system : fSystems) {

        // --- Look for trigger window in list
        if (fTriggerWindows.find(system) == fTriggerWindows.end()) {
          //LOG(fatal) << "EventBuilder::BuildEvent(): no trigger window supplied for requested detector.";
          std::cout << "EventBuilder::BuildEvent(): no trigger window supplied for requested detector." << std::endl;
          exit(1);
        }
        const double tMin = trigger + fTriggerWindows.find(system)->second.first;
        const double tMax = trigger + fTriggerWindows.find(system)->second.second;

        // --- Build the event using trigger window
        switch (system) {
          case ECbmModuleId::kMuch: {  //we do not support the "MuchBeamTimeDigi"
            assert(is_sorted(ts.fData.fMuch.fDigis.begin(), ts.fData.fMuch.fDigis.end(),
                             IsBefore<CbmMuchDigi, CbmMuchDigi>));
            event.fData.fMuch.fDigis = CopyRange(ts.fData.fMuch.fDigis, tMin, tMax);
            break;
          }
          case ECbmModuleId::kSts: {
            assert(
              is_sorted(ts.fData.fSts.fDigis.begin(), ts.fData.fSts.fDigis.end(), IsBefore<CbmStsDigi, CbmStsDigi>));
            event.fData.fSts.fDigis = CopyRange(ts.fData.fSts.fDigis, tMin, tMax);
            break;
          }
          case ECbmModuleId::kTof: {
            assert(
              is_sorted(ts.fData.fTof.fDigis.begin(), ts.fData.fTof.fDigis.end(), IsBefore<CbmTofDigi, CbmTofDigi>));
            event.fData.fTof.fDigis = CopyRange(ts.fData.fTof.fDigis, tMin, tMax);
            break;
          }
          case ECbmModuleId::kTrd: {
            assert(
              is_sorted(ts.fData.fTrd.fDigis.begin(), ts.fData.fTrd.fDigis.end(), IsBefore<CbmTrdDigi, CbmTrdDigi>));
            event.fData.fTrd.fDigis = CopyRange(ts.fData.fTrd.fDigis, tMin, tMax);
            break;
          }
          case ECbmModuleId::kRich: {
            assert(is_sorted(ts.fData.fRich.fDigis.begin(), ts.fData.fRich.fDigis.end(),
                             IsBefore<CbmRichDigi, CbmRichDigi>));
            event.fData.fRich.fDigis = CopyRange(ts.fData.fRich.fDigis, tMin, tMax);
            break;
          }
          case ECbmModuleId::kPsd: {
            assert(
              is_sorted(ts.fData.fPsd.fDigis.begin(), ts.fData.fPsd.fDigis.end(), IsBefore<CbmPsdDigi, CbmPsdDigi>));
            event.fData.fPsd.fDigis = CopyRange(ts.fData.fPsd.fDigis, tMin, tMax);
            break;
          }
          case ECbmModuleId::kT0: {  //T0 has Tof digis
            assert(is_sorted(ts.fData.fT0.fDigis.begin(), ts.fData.fT0.fDigis.end(), IsBefore<CbmTofDigi, CbmTofDigi>));
            event.fData.fT0.fDigis = CopyRange(ts.fData.fT0.fDigis, tMin, tMax);
            break;
          }
          default: {
            //LOG(fatal) << "EventBuilder::BuildEvent():GetName(): Reading digis from unknown detector type!";
            std::cout << "EventBuilder::BuildEvent():GetName(): Reading digis from unknown detector type!" << std::endl;
            exit(1);
          }
        }
      }
      return event;
    }
  }  // namespace algo
}  // namespace cbm
