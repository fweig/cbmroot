/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#ifndef CBMRECOTZERO_H
#define CBMRECOTZERO_H 1

#include "CbmTzdDigi.h"

#include "FairTask.h"

#include <vector>

class TClonesArray;


/** @struct CbmRecoTzeroMoniData
 ** @brief Monitor data for T0 reconstruction
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 10.11.2022
 **/
struct CbmRecoTzeroMoniData {
  size_t fNumTs       = 0;
  size_t fNumEvents   = 0;
  size_t fNumEvtsTzd0 = 0;
  size_t fNumEvtsTzd1 = 0;
  size_t fNumEvtsTzdn = 0;
  double fExecTime    = 0.;

  CbmRecoTzeroMoniData& operator+=(const CbmRecoTzeroMoniData& other)
  {
    fNumTs += other.fNumTs;
    fNumEvents += other.fNumEvents;
    fNumEvtsTzd0 += other.fNumEvtsTzd0;
    fNumEvtsTzd1 += other.fNumEvtsTzd1;
    fNumEvtsTzdn += other.fNumEvtsTzdn;
    fExecTime += other.fExecTime;
    return *this;
  }
};

/** @class CbmRecoTzero
 ** @brief Task class for reconstruction of the event t0
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 10.11.2022
 **
 ** The current implementation reads the t0 information from the TdzDigi object. t0 is set to -1. if
 ** no such object is in the event, and to -2. if there are several.
 **/
class CbmRecoTzero : public FairTask {

public:
  /** @brief Constructor
   **
   ** @param name    Name of task
   ** @param title   Title of task
   **/
  CbmRecoTzero(const char* name = "RecoTzero");


  /** @brief Destructor **/
  virtual ~CbmRecoTzero();


  /** @brief Initialisation **/
  virtual InitStatus Init();


  /** @brief Task execution **/
  virtual void Exec(Option_t* opt);


  /** @brief End-of-timeslice action **/
  virtual void Finish();


private:
  // --- Data
  const std::vector<CbmTzdDigi>* fTzdDigis = nullptr;  ///< TZD digis
  TClonesArray* fEvents                    = nullptr;  ///< CbmEvent

  // --- Monitor
  CbmRecoTzeroMoniData fMonitor = {};  ///< Monitor data


  ClassDef(CbmRecoTzero, 1);
};

#endif
