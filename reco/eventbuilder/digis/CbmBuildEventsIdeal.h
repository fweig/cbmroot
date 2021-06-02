/** @file CbmBuildEventsIdeal.h
  * @copyright Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmBuildEventsIdeal.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 17.09.2016
 ** @date 15.^^.1010
 **/

#ifndef CBMBUILDEVENTSIDEAL_H
#define CBMBUILDEVENTSIDEAL_H 1


#include "CbmDefs.h"

#include <FairTask.h>

#include <vector>

class TClonesArray;
class CbmDigiManager;
class CbmMatch;


/** @class CbmStsBuildEventsIdeal
 ** @brief Task class for associating digis to events
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 17.09.2016
 ** @date 15.11.2020
 **
 ** The digi event builder creates objects of class CbmEvent and fills them
 ** with digi objects. For the association of the digis to the events, the
 ** MC-truth information in the digi match objects is employed. A digi
 ** is attributed to the event of its link with the largest weight.
 **
 ** The event builder operates within one time slice; splitting of events
 ** between time slices is not treated.
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
  UInt_t EventsInMatch(const CbmMatch* match);


private:                                  // members
  CbmDigiManager* fDigiMan = nullptr;     //!
  std::vector<ECbmModuleId> fSystems {};  //  List of detector systems
  TClonesArray* fEvents   = nullptr;      //! Output array (class CbmEvent)
  Int_t fNofEntries       = 0;            //  Number of processed time slices
  Long64_t fNofEvents     = 0;
  Double_t fNofDigisTotal = 0.;
  Double_t fNofDigisAmbig = 0.;
  Double_t fNofDigisNoise = 0.;
  Double_t fTime          = 0.;


  ClassDef(CbmBuildEventsIdeal, 3);
};

#endif /* CBMBUILDEVENTSIDEAL_H */
