/** @file CbmBuildEventsIdealNew.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmBuildEventsIdeal.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 13.03.2020
 **/
#ifndef CBMBUILDEVENTSIDEALNEW_H_
#define CBMBUILDEVENTSIDEALNEW_H 1


#include "CbmDefs.h"

#include <FairTask.h>

#include <vector>

class TClonesArray;
class CbmDigiManager;
class CbmEventStore;
class CbmEvent;
class CbmStsDigi;

/** @class CbmStsBuildEventsIdealNew
 ** @brief Task class for associating digis to events
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 13.03.2020
 ** @version 1.0
 **
 ** The event association uses MC truth (CbmMatch of CbmDigi).
 ** It operates within one time slice; splitting of events between
 ** time slice is not treated.
 **
 ** Using CbmEventStore (storable events).
 **/
class CbmBuildEventsIdealNew : public FairTask {

public:
  /** Constructor **/
  CbmBuildEventsIdealNew();

  /** Destructor **/
  virtual ~CbmBuildEventsIdealNew();

  /** Task execution **/
  virtual void Exec(Option_t* opt);


private:
  CbmDigiManager* fDigiMan = nullptr;  //!
  std::vector<ECbmModuleId> fSystems {};
  TClonesArray* fEvents = nullptr;  //! Event array
  Int_t fNofEntries     = 0;        //  Number of processed time slices

  /** Task initialisation **/
  virtual InitStatus Init();

  CbmBuildEventsIdealNew(const CbmBuildEventsIdealNew&) = delete;
  CbmBuildEventsIdealNew& operator=(const CbmBuildEventsIdealNew&) = delete;

  ClassDef(CbmBuildEventsIdealNew, 2);
};

#endif /* CBMBUILDEVENTSIDEALNEW_H */
