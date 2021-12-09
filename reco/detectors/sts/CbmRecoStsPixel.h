/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

/** @file CbmRecoStsPixel.h
 ** @author Sergey Gorbunov
 ** @since 09.12.2021
 **/


#ifndef CbmRecoStsPixel_H
#define CbmRecoStsPixel_H 1

#include "CbmRecoSts.h"

#include <FairTask.h>

#include <TStopwatch.h>


class CbmDigiManager;
class CbmEvent;
class CbmStsElement;
class CbmStsParAsic;
class CbmStsParModule;
class CbmStsParSensor;
class CbmStsParSensorCond;
class CbmStsParSetModule;
class CbmStsParSetSensor;
class CbmStsParSetSensorCond;
class CbmStsParSim;
class CbmStsRecoModule;
class CbmStsSensor;
class CbmStsSetup;
class CbmMCDataArray;

/** @class CbmRecoStsPixel
 ** @brief Task class for local reconstruction in the STS Pixel detector
 ** @author Sergey Gorbunov
 ** @since 09.12.2021
 ** @date 09.12.2021
 **
 ** Local reconstruction for the experimental STS Pixel detector
 **
 ** The STS digis must be produced by the CbmStsDigitizePixel task.
 ** To run it, replace CbmRecoSts task by CbmRecoStsPixel task in the run_reco.C macro
 **
 **/
class CbmRecoStsPixel : public FairTask {

public:
  /** @brief Constructor **/
  CbmRecoStsPixel(ECbmRecoMode mode = kCbmRecoTimeslice);


  /** @brief Copy constructor (disabled) **/
  CbmRecoStsPixel(const CbmRecoStsPixel&) = delete;


  /** @brief Assignment operator (disabled) **/
  CbmRecoStsPixel operator=(const CbmRecoStsPixel&) = delete;


  /** @brief Destructor  **/
  ~CbmRecoStsPixel();


  /** @brief Task execution **/
  void Exec(Option_t* opt);


  /** @brief End-of-run action **/
  void Finish();


  /** @brief Initialisation **/
  InitStatus Init();


  /** @brief Set event-by-event mode
     ** @param choice  If true, event-by-event mode is used
     **
     ** In the event-by-event mode, the event objects in the input tree
     ** are used, and events are processed one after the other.
     ** An event builder has to be run before, creating the event objects.
     ** By default, time-slice mode is applied.
     **
     ** Alternative to using SetMode.
     **/
  void SetEventMode(Bool_t choice = kTRUE) { fMode = (choice ? kCbmRecoEvent : kCbmRecoTimeslice); }


  /** @brief Set execution mode
     ** @param mode  Time-slice or event
     **
     ** In the time-slice mode, the entire time-slice (input arrays)
     ** will be processed. In the event mode, events read from the event
     ** branch are processed one after the other.
     **/
  void SetMode(ECbmRecoMode mode) { fMode = mode; }


  /** @brief Define the needed parameter containers **/
  void SetParContainers();


private:
  /** @brief Process one time slice or event
     ** @param event  Pointer to CbmEvent object
     **
     ** If a null event pointer is given, the entire input array is processed.
     **/
  void ProcessData(CbmEvent* event = nullptr);

private:
  // --- I/O
  TClonesArray* fEvents        = nullptr;  //! Input array of events
  CbmDigiManager* fDigiManager = nullptr;  //! Interface to digi branch
  CbmMCDataArray* fStsPoints {nullptr};
  TClonesArray* fClusters = nullptr;  //! Output cluster array
  TClonesArray* fHits     = nullptr;  //! Output hit array

  // --- Setup and parameters
  CbmStsSetup* fSetup                 = nullptr;  //! Instance of STS setup
  CbmStsParSim* fParSim               = nullptr;  ///< Simulation settings
  CbmStsParSetModule* fParSetModule   = nullptr;  ///< Module parameters
  CbmStsParSetSensor* fParSetSensor   = nullptr;  ///< Sensor parameters
  CbmStsParSetSensorCond* fParSetCond = nullptr;  ///< Sensor conditions


  // --- Settings
  ECbmRecoMode fMode = kCbmRecoEvent;  ///< Time-slice or event

  ClassDef(CbmRecoStsPixel, 0);
};

#endif
