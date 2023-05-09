/* Copyright (C) 2023 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */


#ifndef CBMTASKUNPACKXPU_H
#define CBMTASKUNPACKXPU_H 1

#include "CbmDefs.h"
#include "CbmDigiTimeslice.h"

#include <FairTask.h>

#include <sstream>
#include <vector>

#include "EventBuilder.h"
#include "StsReadoutConfig.h"
#include "UnpackStsXpu.h"


class CbmDigiManager;
class CbmSourceTs;


/** @class CbmTaskUnpackXpu
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
class CbmTaskUnpackXpu : public FairTask {


public:
  /** @brief Constructor **/
  CbmTaskUnpackXpu();


  /** @brief Copy constructor (disabled) **/
  CbmTaskUnpackXpu(const CbmTaskUnpackXpu&) = delete;


  /** @brief Destructor **/
  virtual ~CbmTaskUnpackXpu();


  /** @brief Task execution **/
  virtual void Exec(Option_t* opt);


  /** @brief Finish timeslice **/
  virtual void Finish();


  /** @brief Assignment operator (disabled) **/
  CbmTaskUnpackXpu& operator=(const CbmTaskUnpackXpu&) = delete;


private:  // methods
  /** @brief Task initialisation **/
  virtual InitStatus Init();

private:  // members
  CbmSourceTs* fSource = nullptr;

  cbm::algo::UnpackStsXpu fAlgoStsXpu;
  cbm::algo::StsReadoutConfig fStsConfig {};

  size_t fNumTs                = 0;
  size_t fNumMs                = 0;
  size_t fNumBytes             = 0;
  size_t fNumDigis             = 0;
  double fTime                 = 0.;
  CbmDigiTimeslice* fTimeslice = nullptr;  ///< Output data

  ClassDef(CbmTaskUnpackXpu, 1);
};

#endif /* CBMTASKUNPACKXPU_H */
