/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#ifndef CBM_ALGO_EVENTBUILDER_H
#define CBM_ALGO_EVENTBUILDER_H 1

#include "CbmDefs.h"
#include "CbmDigiEvent.h"
#include "CbmDigiTimeslice.h"

#include <FairLogger.h>

#include <algorithm>
#include <map>
#include <vector>

namespace cbm
{
  namespace algo
  {

    /** @brief Time comparison of two objects with time stamps (class implementing GetTime()) **/
    template<typename Data1, typename Data2>
    bool IsBefore(const Data1& obj1, const Data2& obj2)
    {
      return obj1.GetTime() < obj2.GetTime();
    }

    /** @class EventBuilder
     ** @author Volker Friese <v.friese@gsi.de>
     ** @since 2021
     ** @brief Constructs CbmDigiEvents out of CbmDigiTimeslices
     **
     ** Events are constructed by copying digi data from the source (CbmDigiTimeslice).
     ** Digis are selected in trigger windows, the sizes of which relative to a trigger time are configurable.
     ** For each trigger time, an event is generated. The time intervals may overlap, resulting in digis
     ** being attributed to multiple events.
     **
     ** The source digi vectors (in CbmDigiTimeslice) must be sorted w.r.t. time, otherwise the behaviour is
     ** undefined.
     **
     ** The trigger vector must be sorted.
     **/
    class EventBuilder {

    public:
      /** @brief Constructor **/
      EventBuilder() {};

      /** @brief Destructor **/
      virtual ~EventBuilder() {};

      /** @brief Execution
         ** @param  ts       Digi source (timeslice)
         ** @param  triggers List of trigger times
         ** @return Vector of constructed events
         **/
      std::vector<CbmDigiEvent> operator()(const CbmDigiTimeslice& ts, const std::vector<double> triggers) const;

      /** @brief Build a single event from a trigger time
         ** @param  ts      Digi source (timeslice)
         ** @param  trigger Trigger time
         ** @return Digi event
         **/
      CbmDigiEvent BuildEvent(const CbmDigiTimeslice& ts, double trigger) const;

      /** @brief Add a detector system 
 	 ** @param system    System to be added
	 **/
      void AddSystem(ECbmModuleId system)
      {
        if (std::find(fSystems.begin(), fSystems.end(), system) != fSystems.end()) return;
        fSystems.push_back(system);
      }

      /** @brief Configure the trigger windows
         ** @param system  Detector system identifier
         ** @param tMin    Trigger window start time w.r.t. trigger time
         ** @param tMax    Trigger window end time w.r.t. trigger time
         **/
      void SetTriggerWindow(ECbmModuleId system, double tMin, double tMax)
      {
        if (std::find(fSystems.begin(), fSystems.end(), system) == fSystems.end()) {
          //LOG(fatal) << "EventBuilder::SetTriggerWindow(): setting trigger window for non-added detector.";
          std::cout << "EventBuilder::SetTriggerWindow(): setting trigger window for non-added detector." << std::endl;
          exit(1);
        }
        fTriggerWindows[system] = std::make_pair(tMin, tMax);
      }

    private:  // methods
      /** @brief Copy data objects in a given time interval from the source to the target vector
         ** @param source Source data vector
         ** @param tMin   Minimal time
         ** @param tMax   Maximal time
         ** @return Target data vector
         **
         ** The Data class specialisation must implement the method double GetTime(), which is used to
         ** check whether the Data object falls into the specified time interval.
         **
         ** The source vector must be ordered w.r.t. GetTime(), otherwise the behaviour is undefined.
         **
         ** TODO: The current implementation searches, for each trigger, the entire source vector. This
         ** can surely be optimised when the contract that the trigger vector be sorted is properly exploited,
         ** e.g., by starting the search for the first digi in the trigger window from the start of the
         ** previous trigger window. This, however, requires bookkeeping hardly to be realised without
         ** changing the state of the class. I leave this for the future and for bright specialists.
         **/
      template<typename Data>
      static typename std::vector<Data> CopyRange(const std::vector<Data>& source, double tMin, double tMax)
      {
        auto comp1 = [](const Data& obj, double value) { return obj.GetTime() < value; };
        auto comp2 = [](double value, const Data& obj) { return value < obj.GetTime(); };
        auto lower = std::lower_bound(source.begin(), source.end(), tMin, comp1);
        auto upper = std::upper_bound(lower, source.end(), tMax, comp2);
        return std::vector<Data>(lower, upper);
      }

    private:  // data members
      std::map<ECbmModuleId, std::pair<double, double>> fTriggerWindows;
      std::vector<ECbmModuleId> fSystems {};  //  List of detector systems
    };
  }  // namespace algo
}  // namespace cbm

#endif /* CBM_ALGO_EVENTBUILDER_H */
