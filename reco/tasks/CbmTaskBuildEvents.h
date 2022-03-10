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
   ** @param system  Detector system identifier
   ** @param tMin    Event window start time w.r.t. event time
   ** @param tMax    Event window end time w.r.t. event time
   **/
  void SetEventWindow(ECbmModuleId system, double tMin, double tMax)
  {
    fEventWindows[system] = std::make_pair(tMin, tMax);
  }

private:  // methods
  /** @brief Task initialisation **/
  virtual InitStatus Init();


  /** @brief Construct a DigiTimeslice from the data in CbmDigiManager **/
  CbmDigiTimeslice FillTimeSlice();


  /** @brief Number of digis for a given system
   ** @param data    CbmDigiData object (DigiTimeslice or DigiEvent)
   ** @param system  System identifier (enum ECbmModuleId)
   ** @return        Number of digis for the system
   **/
  size_t GetNumDigis(const CbmDigiData& data, ECbmModuleId system);


private:                                           // members
  const CbmDigiTimeslice* fTimeslice   = nullptr;  //! Input data (from unpacking)
  CbmDigiManager* fDigiMan             = nullptr;  //! Input data (from simulation)
  const std::vector<double>* fTriggers = nullptr;  //! Input data (triggers)
  std::vector<CbmDigiEvent>* fEvents   = nullptr;  //! Output data (events)
  cbm::algo::EventBuilder fAlgo {};                //! Algorithm

  std::map<ECbmModuleId, std::pair<double, double>> fEventWindows;

  // for diagnostics
  std::map<ECbmModuleId, size_t> fNumDigisTs;  //  Number of digis in timeslices
  std::map<ECbmModuleId, size_t> fNumDigisEv;  //  Number of digis in events
  size_t fNumTs        = 0;                    //  Number of processed time slices
  size_t fNumTriggers  = 0;                    //  Number of triggers
  size_t fNumEvents    = 0;                    //  Number of produced events
  double fTimeFillTs   = 0.;
  double fTimeBuildEvt = 0.;
  double fTimeTot      = 0.;

  ClassDef(CbmTaskBuildEvents, 1);
};

#endif /* CBMTASKBUILDEVENTS_H */
