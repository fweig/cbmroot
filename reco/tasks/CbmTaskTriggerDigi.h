/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#ifndef CBMTASKTRIGGERDIGI_H
#define CBMTASKTRIGGERDIGI_H 1


#include "CbmDefs.h"

#include <FairTask.h>

#include "TimeClusterTrigger.h"

#include <vector>

class CbmDigiManager;


/** @class CbmTaskTriggerDigi
 ** @brief Task class for minimum-bias event trigger from time-distribution of digi data
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 20.11.2021
 **
 ** The tasks calls algo::TimeClusterTrigger with the digi time distribution of the trigger detector.
 **
 ** TOFO: The current implementation is for STS only as trigger detector.
 **/
class CbmTaskTriggerDigi : public FairTask {


public:
  /** @brief Constructor **/
  CbmTaskTriggerDigi();


  /** @brief Copy constructor (disabled) **/
  CbmTaskTriggerDigi(const CbmTaskTriggerDigi&) = delete;


  /** @brief Destructor **/
  virtual ~CbmTaskTriggerDigi();


  /** @brief Task execution **/
  virtual void Exec(Option_t* opt);


  /** @brief Finish timeslice **/
  virtual void Finish();


  /** @brief Assignment operator (disabled) **/
  CbmTaskTriggerDigi& operator=(const CbmTaskTriggerDigi&) = delete;


  /** @brief Configure the trigger algorithm
   ** @param triggerWindow    Size of trigger window [ns]
   ** @param minNumDigis      Trigger threshold on number of digis in trigger windows
   ** @param deadTime         Minimum time between two triggers [ns]
   **/
  void SetAlgoParams(double triggerWindow, int32_t minNumDigis, double deadTime)
  {
    fTriggerWindow = triggerWindow;
    fMinNumDigis   = minNumDigis;
    fDeadTime      = deadTime;
  }


private:  // methods
  /** @brief Task initialisation **/
  virtual InitStatus Init();


private:                                     // members
  CbmDigiManager* fDigiMan = nullptr;        //! Input data
  std::vector<ECbmModuleId> fSystems {};     //  List of detector systems
  std::vector<double>* fTriggers = nullptr;  //! Output data
  cbm::algo::TimeClusterTrigger fAlgo {};    //! Algorithm
  double fTriggerWindow = 0.;
  int32_t fMinNumDigis  = 0;
  double fDeadTime      = 0.;
  size_t fNumTs         = 0;  //  Number of processed time slices
  size_t fNumDigis      = 0;  //  Number of digis from trigger detector
  size_t fNumTriggers   = 0;  //  Number of found triggers
  double fTimeExtract   = 0.;
  double fTimeFind      = 0.;
  double fTimeTot       = 0.;


  ClassDef(CbmTaskTriggerDigi, 1);
};

#endif /* CBMTASKTRIGGERDIGI_H */
