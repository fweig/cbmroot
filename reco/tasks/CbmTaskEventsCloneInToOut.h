/* Copyright (C) 2023 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */


#ifndef CbmTaskEventsCloneInToOut_H
#define CbmTaskEventsCloneInToOut_H 1

#include <FairTask.h>

class TClonesArray;
class FairFileSource;

/** @class CbmTaskEventsCloneInToOut
 ** @brief Task class for clone CbmEvent objects from the Input to the output to allow their update, e.g. due to new
 **        reconstruction steps.
 **
 ** @author Pierre-Alain Loizeau <v.friese@gsi.de>
 ** @since 17.01.2023
 **
 ** This tasks copies the existing CbmEvent from the input file to the memory with the persitency flag set, so that they
 ** appear in the output file and can be updated (standard inputs are const).
 **
 ** The task has to be run prior to any reconstruction task adding more information to the CbmEvent (e.g. hit or track
 ** reconstruction), but only in case the event building was done in a previous run (thus saving the events to the file
 ** used as input for this run)
 **
 **/
class CbmTaskEventsCloneInToOut : public FairTask {


public:
  /** @brief Constructor **/
  CbmTaskEventsCloneInToOut();


  /** @brief Copy constructor (disabled) **/
  CbmTaskEventsCloneInToOut(const CbmTaskEventsCloneInToOut&) = delete;


  /** @brief Destructor **/
  virtual ~CbmTaskEventsCloneInToOut();


  /** @brief Task execution **/
  virtual void Exec(Option_t* opt);


  /** @brief Finish timeslice **/
  virtual void Finish();


  /** @brief Assignment operator (disabled) **/
  CbmTaskEventsCloneInToOut& operator=(const CbmTaskEventsCloneInToOut&) = delete;


private:  // methods
  /** @brief Task initialisation **/
  virtual InitStatus Init();

private:  // members
  FairFileSource* fpFileIn          = nullptr;
  const TClonesArray* fRecoEventsIn = nullptr;
  TClonesArray* fRecoEventsOut      = nullptr;
  double fTimeTot                   = 0.;  ///< Execution time
  size_t fNumTs                     = 0;   ///< Number of processed timeslices
  size_t fNumEvents                 = 0;   ///< Number of events

  ClassDef(CbmTaskEventsCloneInToOut, 1);
};

#endif /* CbmTaskEventsCloneInToOut_H */
