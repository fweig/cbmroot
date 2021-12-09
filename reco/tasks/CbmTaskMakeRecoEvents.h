/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#ifndef CBMTASKMAKERECOEVENTS_H
#define CBMTASKMAKERECOEVENTS_H 1


#include "CbmDefs.h"
#include "CbmDigiEvent.h"

#include <FairTask.h>

#include <vector>

class TClonesArray;


/** @class CbmTaskMakeRecoEvents
 ** @brief Task class for interfacing storable raw events in the CbmDigiEvent format to
 ** the current offline reconstruction chain.
 **
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 08.12.2021
 **
 ** This tasks creates the established data interfaces (digi branches, CbmEvent) as input
 ** to the reconstruction tasks from digis stored in the new event data format CbmDigiEvent
 ** as created by trigger and event builder from experiment or simulated data. It is to be
 ** understood as intermediate solution until the reconstruction routines will be properly
 ** interfaced to the new format. The expense is a duplication of digis in memory.
 **
 ** The task has to be run prior to any reconstruction task making use of digis (cluster and
 ** hit finder).
 **
 ** TOFO: The current implementation is for STS only and shall be expanded to all digi types.
 **/
class CbmTaskMakeRecoEvents : public FairTask {


public:
  /** @brief Constructor **/
  CbmTaskMakeRecoEvents();


  /** @brief Copy constructor (disabled) **/
  CbmTaskMakeRecoEvents(const CbmTaskMakeRecoEvents&) = delete;


  /** @brief Destructor **/
  virtual ~CbmTaskMakeRecoEvents();


  /** @brief Task execution **/
  virtual void Exec(Option_t* opt);


  /** @brief Finish timeslice **/
  virtual void Finish();


  /** @brief Assignment operator (disabled) **/
  CbmTaskMakeRecoEvents& operator=(const CbmTaskMakeRecoEvents&) = delete;


private:  // methods
  /** @brief Task initialisation **/
  virtual InitStatus Init();


private:  // members
  const std::vector<CbmDigiEvent>* fDigiEvents = nullptr;
  TClonesArray* fRecoEvents                    = nullptr;
  std::vector<CbmStsDigi>* fStsDigis           = nullptr;
  double fTimeTot                              = 0.;  ///< Execution time
  size_t fNumTs                                = 0;   ///< Number of processed timeslices
  size_t fNumEvents                            = 0;   ///< Number of events

  ClassDef(CbmTaskMakeRecoEvents, 1);
};

#endif /* CBMTASKMAKERECOEVENTS_H */
