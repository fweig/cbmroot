/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#ifndef CBMTASKUNPACK_H
#define CBMTASKUNPACK_H 1

#include "CbmDefs.h"
#include "CbmDigiTimeslice.h"

#include <FairTask.h>

#include <sstream>
#include <vector>

#include "EventBuilder.h"
#include "StsReadoutConfig.h"
#include "UnpackSts.h"

class CbmDigiManager;
class CbmSourceTs;


/** @class CbmTaskUnpack
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
class CbmTaskUnpack : public FairTask {


public:
  /** @brief Constructor **/
  CbmTaskUnpack();


  /** @brief Copy constructor (disabled) **/
  CbmTaskUnpack(const CbmTaskUnpack&) = delete;


  /** @brief Destructor **/
  virtual ~CbmTaskUnpack();


  /** @brief Task execution **/
  virtual void Exec(Option_t* opt);


  /** @brief Finish timeslice **/
  virtual void Finish();


  /** @brief Assignment operator (disabled) **/
  CbmTaskUnpack& operator=(const CbmTaskUnpack&) = delete;


private:  // methods
  /** @brief Task initialisation **/
  virtual InitStatus Init();


private:  // members
  CbmSourceTs* fSource                              = nullptr;
  std::map<uint16_t, cbm::algo::UnpackSts> fAlgoSts = {};
  cbm::algo::StsReadoutConfig fStsConfig {};
  size_t fNumTs                = 0;
  size_t fNumMs                = 0;
  size_t fNumBytes             = 0;
  size_t fNumDigis             = 0;
  double fTime                 = 0.;
  CbmDigiTimeslice* fTimeslice = nullptr;  ///< Output data


  ClassDef(CbmTaskUnpack, 1);
};

#endif /* CBMTASKUNPACK_H */
