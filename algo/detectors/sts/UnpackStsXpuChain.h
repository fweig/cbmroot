/* Copyright (C) 2023 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */


#ifndef UnpackStsXpuChain_H
#define UnpackStsXpuChain_H 1

#include "CbmDefs.h"
#include "CbmStsDigi.h"

#include <sstream>
#include <vector>

#include "EventBuilder.h"
#include "StsReadoutConfig.h"
#include "UnpackStsXpu.h"
#include "Timeslice.hpp"


class CbmDigiManager;
class CbmSourceTs;


/** @class UnpackStsXpuChain
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
class UnpackStsXpuChain {


public:
  /** @brief Constructor **/
  UnpackStsXpuChain();


  /** @brief Copy constructor (disabled) **/
  UnpackStsXpuChain(const UnpackStsXpuChain&) = delete;


  /** @brief Destructor **/
  ~UnpackStsXpuChain();

  /** @brief Task initialisation **/
  void Init(const cbm::algo::StsReadoutConfig &stsConfig);


  /** @brief Task execution **/
  std::vector<CbmStsDigi> Exec(const fles::Timeslice& ts);


  /** @brief Finish timeslice **/
  void Finish();


  /** @brief Assignment operator (disabled) **/
  UnpackStsXpuChain& operator=(const UnpackStsXpuChain&) = delete;


private:  // methods

private:  // members
  // CbmSourceTs* fSource = nullptr;

  cbm::algo::UnpackStsXpu fAlgoStsXpu;
  std::optional<cbm::algo::StsReadoutConfig> fStsConfig {};

  size_t fNumTs                = 0;
  size_t fNumMs                = 0;
  size_t fNumBytes             = 0;
  size_t fNumDigis             = 0;
  double fTime                 = 0.;
  // CbmDigiTimeslice* fTimeslice = nullptr;  ///< Output data

};

#endif /* UnpackStsXpuChain_H */
