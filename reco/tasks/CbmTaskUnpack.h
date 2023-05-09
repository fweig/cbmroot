/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */


#ifndef CBMTASKUNPACK_H
#define CBMTASKUNPACK_H 1

#include "CbmDefs.h"
#include "CbmDigiTimeslice.h"

#include <FairTask.h>

#include "TofReadoutConfig.h"
#include "TrdReadoutConfig.h"

#include <sstream>
#include <vector>

#include "BmonReadoutConfig.h"
#include "EventBuilder.h"
#include "MuchReadoutConfig.h"
#include "StsReadoutConfig.h"
#include "UnpackBmon.h"
#include "UnpackMuch.h"
#include "UnpackSts.h"
#include "UnpackTof.h"
#include "UnpackTrd.h"

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

  /** @brief Initialisation of address maps for Trd **/
  virtual void InitTrdReadoutConfig();

private:  // members
  CbmSourceTs* fSource = nullptr;

  std::map<uint16_t, cbm::algo::UnpackSts> fAlgoSts = {};
  cbm::algo::StsReadoutConfig fStsConfig {};

  std::map<uint16_t, cbm::algo::UnpackMuch> fAlgoMuch = {};
  cbm::algo::MuchReadoutConfig fMuchConfig {};

  std::map<uint16_t, cbm::algo::UnpackTof> fAlgoTof = {};
  cbm::algo::TofReadoutConfig fTofConfig {};

  std::map<uint16_t, cbm::algo::UnpackBmon> fAlgoBmon = {};
  cbm::algo::BmonReadoutConfig fBmonConfig {};

  std::map<uint16_t, cbm::algo::UnpackTrd> fAlgoTrd = {};
  cbm::algo::TrdReadoutConfig fTrdConfig {};

  size_t fNumTs                = 0;
  size_t fNumMs                = 0;
  size_t fNumBytes             = 0;
  size_t fNumDigis             = 0;
  double fTime                 = 0.;
  CbmDigiTimeslice* fTimeslice = nullptr;  ///< Output data

  //Microslice loop
  template<class Digi, class UnpackAlgo>
  uint64_t MsLoop(const fles::Timeslice* timeslice, UnpackAlgo& algo, const uint64_t comp, std::vector<Digi>* digis,
                  size_t* numBytesInComp, size_t* numDigisInComp);

  ClassDef(CbmTaskUnpack, 2);
};

#endif /* CBMTASKUNPACK_H */
