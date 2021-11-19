/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#ifndef CBMTASKBUILDEVENTS_H
#define CBMTASKBUILDEVENTS_H 1


#include "CbmDefs.h"
#include "CbmDigiEvent.h"

#include <FairTask.h>

#include <vector>

#include "EventBuilder.h"

class CbmDigiManager;


/** @class CbmTaskBuildEvents
 ** @brief Task class for associating digis to events
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 15.11.2021
 **
 ** Creates objects of class CbmDigiEvent and fills them with digi objects,
 ** using the algorithm EventBuilder.
 **
 ** TOFO: The current implementation is for STS only and with a dummy trigger list
 ** just to establish the framework integration of algorithm and data interfaces.
 **/
class CbmTaskBuildEvents : public FairTask {


public:
  /** @brief Constructor **/
  CbmTaskBuildEvents();


  /** @brief Copy constructor (disabled) **/
  CbmTaskBuildEvents(const CbmTaskBuildEvents&) = delete;


  /** @brief Destructor **/
  virtual ~CbmTaskBuildEvents();


  /** @brief Task execution **/
  virtual void Exec(Option_t* opt);


  /** @brief Finish timeslice **/
  virtual void Finish();


  /** @brief Assignment operator (disabled) **/
  CbmTaskBuildEvents& operator=(const CbmTaskBuildEvents&) = delete;


  /** @brief Configure the event building time intervals
   ** @param tMin    Trigger window start time w.r.t. trigger time
   ** @param tMax    Trigger window end time w.r.t. trigger time
   **/
  void SetTimeWindow(double tMin, double tMax)
  {
    fEvtTimeStsMin = tMin;
    fEvtTimeStsMax = tMax;
  }


private:  // methods
  /** @brief Task initialisation **/
  virtual InitStatus Init();


private:                                           // members
  CbmDigiManager* fDigiMan             = nullptr;  //! Input data (digis)
  const std::vector<double>* fTriggers = nullptr;  //! Input data (triggers)
  std::vector<ECbmModuleId> fSystems {};           //  List of detector systems
  std::vector<CbmDigiEvent>* fEvents = nullptr;    //! Output data (events)
  cbm::algo::EventBuilder fAlgo {};                //! Algorithm
  double fEvtTimeStsMin = 0.;
  double fEvtTimeStsMax = 0.;
  size_t fNumTs         = 0;  //  Number of processed time slices
  size_t fNumTriggers   = 0;  //  Number of triggers
  size_t fNumEvents     = 0;  //  Number of produced events
  size_t fNumDigisStsTs = 0;  //  Number of digis in timeslices
  size_t fNumDigisStsEv = 0;  //  Number of digis in events
  double fTimeFillTs    = 0.;
  double fTimeBuildEvt  = 0.;
  double fTimeTot       = 0.;


  ClassDef(CbmTaskBuildEvents, 1);
};

#endif /* CBMTASKBUILDEVENTS_H */