/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmBmonDigitize.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 07.11.2022
 **/

#ifndef CBMBMONDIGITIZE_H
#define CBMBMONDIGITIZE_H 1

#include "CbmBmonDigi.h"
#include "CbmDefs.h"
#include "CbmDigitize.h"

#include <Rtypes.h>


/** @class CbmBmonDigitize
 ** @brief Task class for simulating the detector response of the t-zero detector
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 07.11.2022
 ** @version 1.0
 **
 ** The current implementation of the BMON simulation is a placeholder until a realistic
 ** detector response model is available. It smears the MC event time with a Gaussian resolution.
 **/
class CbmBmonDigitize : public CbmDigitize<CbmBmonDigi> {

public:
  /** Constructor **/
  CbmBmonDigitize(double resolution = 0.025);


  /** Destructor **/
  virtual ~CbmBmonDigitize();


  /** @brief Detector system ID
   ** @return kT0
   **/
  ECbmModuleId GetSystemId() const { return ECbmModuleId::kT0; }


  /** Execution **/
  virtual void Exec(Option_t* opt);


  /** Re-initialisation **/
  virtual InitStatus ReInit();


  /** Set the time resolution **/
  void SetResolution(double sigma) { fResolution = sigma; }


private:
  // --- Parameters
  double fResolution = 0.025;  ///< Time resolution [ns]

  // --- Run counters
  size_t fNofEvents = 0;   ///< Total number of procesed events
  Double_t fTimeTot = 0.;  ///< Total execution time


  /** End-of-run action **/
  virtual void Finish();


  /** Initialisation **/
  virtual InitStatus Init();


  ClassDef(CbmBmonDigitize, 1);
};

#endif
