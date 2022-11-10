/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmBmonDigitize.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 07.11.2022
 **/

#ifndef CBMTZDDIGITIZE_H
#define CBMTZDDIGITIZE_H 1

#include "CbmDefs.h"
#include "CbmDigitize.h"
#include "CbmTzdDigi.h"

#include <Rtypes.h>


/** @class CbmTzdDigitize
 ** @brief Task class for simulating the detector response of the t-zero detector
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 07.11.2022
 ** @version 1.0
 **
 ** The current implementation of the TZD simulation is a placeholder until a realistic
 ** detector response model is available. It smears the MC event time with a Gaussian resolution.
 **/
class CbmTzdDigitize : public CbmDigitize<CbmTzdDigi> {

public:
  /** Constructor **/
  CbmTzdDigitize(double resolution = 0.025);


  /** Destructor **/
  virtual ~CbmTzdDigitize();


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


  ClassDef(CbmTzdDigitize, 1);
};

#endif
