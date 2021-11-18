/* Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmBuildEventsIdeal.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 17.09.2016
 **/

#ifndef CBMBUILDEVENTSIDEAL_H
#define CBMBUILDEVENTSIDEAL_H 1


#include "CbmDefs.h"
#include "CbmDigiEvent.h"
#include "CbmMatch.h"

#include <FairTask.h>

#include <vector>

class TClonesArray;
class CbmDigiManager;
class CbmMCEventList;
class CbmTimeSlice;


/** @class CbmStsBuildEventsIdeal
 ** @brief Task class for associating digis to events
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 17.09.2016
 ** @date 22.10.2021
 **
 ** The digi event builder creates objects of class CbmEvent and fills them
 ** with digi objects. For the association of the digis to the events, the
 ** MC-truth information in the digi match objects is employed. A digi
 ** is attributed each event it is linked to.
 **
 ** The event builder operates within one time slice; splitting of events
 ** between time slices is not treated. Event numbers are consecutive within
 ** the time slice; the event time is the MC true event time.
 **
 ** TODO: Currently, branches for both CbmEvent and CbmDigiEvent are created
 ** and filled. The branch CbmEvent shall be deactivated once reconstruction from
 ** CbmDigiEvent is established.
 **/
class CbmBuildEventsIdeal : public FairTask {

public:
  /** @brief Constructor **/
  CbmBuildEventsIdeal();

  /** @brief Copy constructor (disabled) **/
  CbmBuildEventsIdeal(const CbmBuildEventsIdeal&) = delete;

  /** @brief Destructor **/
  virtual ~CbmBuildEventsIdeal();

  /** @brief Task execution **/
  virtual void Exec(Option_t* opt);

  /** @brief Finish timeslice **/
  virtual void Finish();

  /** @brief Assignment operator (disabled) **/
  CbmBuildEventsIdeal& operator=(const CbmBuildEventsIdeal&) = delete;


private:  // methods
  /** @brief Task initialisation **/
  virtual InitStatus Init();

  /** @brief Number of different MC events in a match object **/
  CbmMatch EventsInMatch(const CbmMatch* match);


private:                                             // members
  CbmDigiManager* fDigiMan               = nullptr;  //! Input (digis)
  CbmTimeSlice* fTimeslice               = nullptr;  //! Input (timeslice meta-data)
  CbmMCEventList* fMCEvents              = nullptr;  //  Input (MC events)
  TClonesArray* fEvents                  = nullptr;  //! Output (CbmEvent)
  std::vector<CbmDigiEvent>* fDigiEvents = nullptr;  //  Output (CbmDigiEvent)
  std::vector<ECbmModuleId> fSystems {};             //  List of detector systems
  int32_t fNumEntries     = 0;                       //  Number of processed time slices
  Long64_t fNumEvents     = 0;
  Double_t fNumDigisTotal = 0.;
  Double_t fNumDigisAmbig = 0.;
  Double_t fNumDigisNoise = 0.;
  Double_t fTime          = 0.;


  ClassDef(CbmBuildEventsIdeal, 4);
};

#endif /* CBMBUILDEVENTSIDEAL_H */
